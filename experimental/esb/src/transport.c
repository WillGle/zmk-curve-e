/* SPDX-License-Identifier: MIT */
#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/logging/log.h>
#include <mbedtls/platform_util.h>
#include <curve_esb/protocol.h>
#include <curve_esb/transport.h>
#include <zmk/esb.h>
#include <zmk/hid.h>
#include <zmk/usb_hid.h>
#include <zmk/usb.h>
#include "pair-key.h"

LOG_MODULE_REGISTER(curve_esb, LOG_LEVEL_INF);
BUILD_ASSERT(CONFIG_ZMK_HID_REPORT_TYPE_NKRO &&
             !IS_ENABLED(CONFIG_ZMK_HID_KEYBOARD_NKRO_EXTENDED_REPORT), "ESB v1 uses 104 NKRO bits");
BUILD_ASSERT(sizeof(zmk_hid_get_keyboard_report()->body.keys) == 13);
BUILD_ASSERT(CONFIG_ZMK_HID_CONSUMER_REPORT_USAGES_FULL &&
             CONFIG_ZMK_HID_CONSUMER_REPORT_SIZE == 6, "ESB v1 has six uint16 consumer usages");
BUILD_ASSERT(sizeof(zmk_hid_get_mouse_report()->body.buttons) == 1);
BUILD_ASSERT(!IS_ENABLED(CONFIG_ZMK_POINTING_SMOOTH_SCROLLING), "ESB v1 has no resolution negotiation");

static const uint8_t pair_key[16] = CURVE_ESB_PAIR_KEY;
static struct k_work_q work_queue;
K_THREAD_STACK_DEFINE(work_stack, 4096);
static struct ce_session session;
static uint8_t challenge[CE_CHALLENGE_SIZE];
static atomic_t ready;
static bool running;
static bool hello_valid;
static struct k_spinlock mailbox_lock;
static int64_t last_valid;
static uint8_t last_keyboard[CE_KEYBOARD_SIZE], last_consumer[CE_CONSUMER_SIZE];
static uint8_t last_mouse_buttons;
static struct k_work_delayable tick;
static struct k_work refresh;

struct report { uint8_t type; uint8_t body[CE_KEYBOARD_SIZE]; };
K_MSGQ_DEFINE(reports, sizeof(struct report), 16, 4);
struct k_work_q *curve_esb_queue(void) { return &work_queue; }
bool curve_esb_ready(void) { return atomic_get(&ready) != 0; }

static void refresh_endpoint(struct k_work *work) { curve_esb_endpoint_refresh(); }
static void set_ready(bool value) {
    if (atomic_set(&ready, value) != value && !IS_ENABLED(CONFIG_CURVE_ESB_RECEIVER)) {
        k_work_submit(&refresh);
    }
}
static int send_packet(const uint8_t *data, size_t len) {
    if (!len || len > CE_PACKET_MAX) { return -EMSGSIZE; }
    struct zmk_esb_payload p = {.pipe = 0, .noack = false, .length = len};
    memcpy(p.data, data, len);
    return zmk_esb_write_payload(&p);
}
static int new_keyboard_session(void) {
    mbedtls_platform_zeroize(&session, sizeof(session));
    set_ready(false);
    k_msgq_purge(&reports);
    k_spinlock_key_t lock = k_spin_lock(&mailbox_lock);
    memset(last_keyboard, 0, sizeof(last_keyboard));
    memset(last_consumer, 0, sizeof(last_consumer));
    last_mouse_buttons = 0;
    k_spin_unlock(&mailbox_lock, lock);
    hello_valid = false;
    int ret = sys_csrand_get(session.device_nonce, sizeof(session.device_nonce));
    if (ret) { return ret; }
    hello_valid = true;
    uint8_t hello[CE_HELLO_SIZE];
    ret = ce_hello(pair_key, session.device_nonce, hello);
    if (!ret) { ret = send_packet(hello, sizeof(hello)); }
    return ret;
}
static int send_body(uint8_t type, const uint8_t *body) {
    uint8_t packet[CE_PACKET_MAX];
    int len = ce_seal(&session, type, body, ce_body_size(type), packet);
    if (len == -EOVERFLOW) { (void)new_keyboard_session(); return -ENOTCONN; }
    if (len < 0) { return len; }
    return send_packet(packet, len);
}

static void release_all(void) {
    zmk_hid_keyboard_clear(); zmk_hid_consumer_clear(); zmk_hid_mouse_clear();
    if (zmk_usb_get_status() == USB_DC_CONFIGURED) {
        (void)zmk_usb_hid_send_keyboard_report();
        (void)zmk_usb_hid_send_consumer_report();
        (void)zmk_usb_hid_send_mouse_report();
    }
}
static int apply_report(uint8_t type, const uint8_t *body) {
    switch (type) {
    case CE_KEYBOARD: {
        struct zmk_hid_keyboard_report_body *r = &zmk_hid_get_keyboard_report()->body;
        r->modifiers = body[0]; r->_reserved = 0; memcpy(r->keys, body + 1, 13);
        return zmk_usb_hid_send_keyboard_report();
    }
    case CE_CONSUMER: {
        struct zmk_hid_consumer_report_body *r = &zmk_hid_get_consumer_report()->body;
        for (int i = 0; i < 6; i++) { r->keys[i] = sys_get_le16(body + i * 2); }
        return zmk_usb_hid_send_consumer_report();
    }
    case CE_MOUSE: {
        struct zmk_hid_mouse_report_body *r = &zmk_hid_get_mouse_report()->body;
        r->buttons = body[0]; r->d_x = (int16_t)sys_get_le16(body + 1);
        r->d_y = (int16_t)sys_get_le16(body + 3);
        r->d_scroll_y = (int16_t)sys_get_le16(body + 5);
        r->d_scroll_x = (int16_t)sys_get_le16(body + 7);
        return zmk_usb_hid_send_mouse_report();
    }
    default: return 0;
    }
}
static void receive_packet(const struct zmk_esb_payload *p) {
    if (IS_ENABLED(CONFIG_CURVE_ESB_RECEIVER)) {
        if (p->length == CE_HELLO_SIZE && p->data[0] == CE_VERSION && p->data[1] == CE_HELLO) {
            /* Repeating a HELLO retries the same ACK, without resetting replay state. */
            if (session.established && !memcmp(p->data + 2, session.device_nonce, 8)) {
                /* Authenticate even the retry before acknowledging it. */
                uint8_t expected[CE_HELLO_SIZE];
                if (ce_hello(pair_key, session.device_nonce, expected) ||
                    memcmp(expected, p->data, sizeof(expected))) { return; }
                (void)send_packet(challenge, sizeof(challenge));
                return;
            }
            uint8_t nonce[8];
            if (sys_csrand_get(nonce, sizeof(nonce))) { return; }
            struct ce_session next = {0};
            uint8_t response[CE_CHALLENGE_SIZE];
            if (ce_challenge(pair_key, p->data, nonce, &next, response)) { return; }
            if (curve_esb_ready()) { release_all(); }
            set_ready(false);
            session = next;
            mbedtls_platform_zeroize(&next, sizeof(next));
            memcpy(challenge, response, sizeof(challenge));
            (void)send_packet(challenge, sizeof(challenge));
            return;
        }
        uint8_t type, body[CE_KEYBOARD_SIZE];
        int len = ce_open(&session, p->data, p->length, &type, body);
        if (len < 0) { return; }
        uint8_t status[14];
        if (!ce_status(&session, status)) { (void)send_packet(status, sizeof(status)); }
        last_valid = k_uptime_get();
        set_ready(true);
        if (zmk_usb_get_status() == USB_DC_CONFIGURED) { (void)apply_report(type, body); }
    } else {
        if (p->length == 14 && !ce_accept_status(&session, p->data)) {
            last_valid = k_uptime_get();
        }
        if (hello_valid && p->length == CE_CHALLENGE_SIZE && !session.established &&
            !ce_accept(pair_key, p->data, &session)) {
            last_valid = k_uptime_get();
            set_ready(true);
            k_work_reschedule_for_queue(&work_queue, &tick, K_NO_WAIT);
        }
    }
}
static void esb_event(const struct zmk_esb_event *event) {
    if (!running) { return; }
    if (event->evt_id == ZMK_ESB_EVENT_RX_RECEIVED) {
        struct zmk_esb_payload p;
        while (!zmk_esb_read_rx_payload(&p)) { receive_packet(&p); }
    }
    /* A bare hardware ACK does not prove that the receiver still has our session. */
}
static void tick_handler(struct k_work *work) {
    if (!running) { return; }
    if (IS_ENABLED(CONFIG_CURVE_ESB_RECEIVER)) {
        if (curve_esb_ready() && k_uptime_get() - last_valid >= 500) {
            set_ready(false); release_all();
        }
    } else {
        if (session.established && k_uptime_get() - last_valid >= 1000) {
            (void)new_keyboard_session();
        }
        if (!session.established && !hello_valid) { (void)new_keyboard_session(); }
        if (!session.established && hello_valid) {
            uint8_t hello[CE_HELLO_SIZE];
            if (!ce_hello(pair_key, session.device_nonce, hello)) {
                (void)send_packet(hello, sizeof(hello));
            }
        } else if (session.established) {
            struct report report;
            while (!k_msgq_get(&reports, &report, K_NO_WAIT)) {
                (void)send_body(report.type, report.body);
            }
            uint8_t keyboard[CE_KEYBOARD_SIZE], consumer[CE_CONSUMER_SIZE];
            uint8_t mouse[CE_MOUSE_SIZE] = {0};
            k_spinlock_key_t lock = k_spin_lock(&mailbox_lock);
            memcpy(keyboard, last_keyboard, sizeof(keyboard));
            memcpy(consumer, last_consumer, sizeof(consumer));
            mouse[0] = last_mouse_buttons;
            k_spin_unlock(&mailbox_lock, lock);
            (void)send_body(CE_KEYBOARD, keyboard);
            (void)send_body(CE_CONSUMER, consumer);
            (void)send_body(CE_MOUSE, mouse);
            (void)send_body(CE_KEEPALIVE, NULL);
        }
    }
    k_work_reschedule_for_queue(&work_queue, &tick, K_MSEC(100));
}
int curve_esb_send(uint8_t type) {
    if (!curve_esb_ready()) { return -ENOTCONN; }
    struct report report = {.type = type};
    if (type == CE_KEYBOARD) {
        const struct zmk_hid_keyboard_report_body *r = &zmk_hid_get_keyboard_report()->body;
        report.body[0] = r->modifiers; memcpy(report.body + 1, r->keys, 13);
    } else if (type == CE_CONSUMER) {
        const struct zmk_hid_consumer_report_body *r = &zmk_hid_get_consumer_report()->body;
        for (int i = 0; i < 6; i++) { sys_put_le16(r->keys[i], report.body + 2 * i); }
    } else if (type == CE_MOUSE) {
        const struct zmk_hid_mouse_report_body *r = &zmk_hid_get_mouse_report()->body;
        report.body[0] = r->buttons; sys_put_le16(r->d_x, report.body + 1);
        sys_put_le16(r->d_y, report.body + 3); sys_put_le16(r->d_scroll_y, report.body + 5);
        sys_put_le16(r->d_scroll_x, report.body + 7);
    } else { return -EINVAL; }
    /* Retain the latest state even if the edge queue overflows. Mouse movement is not replayed. */
    k_spinlock_key_t lock = k_spin_lock(&mailbox_lock);
    if (type == CE_KEYBOARD) { memcpy(last_keyboard, report.body, CE_KEYBOARD_SIZE); }
    if (type == CE_CONSUMER) { memcpy(last_consumer, report.body, CE_CONSUMER_SIZE); }
    if (type == CE_MOUSE) { last_mouse_buttons = report.body[0]; }
    k_spin_unlock(&mailbox_lock, lock);
    int ret = k_msgq_put(&reports, &report, K_NO_WAIT);
    k_work_reschedule_for_queue(&work_queue, &tick, K_NO_WAIT);
    return ret;
}
static int init(void) {
    k_work_queue_start(&work_queue, work_stack, K_THREAD_STACK_SIZEOF(work_stack), 5, NULL);
    k_work_init(&refresh, refresh_endpoint);
    k_work_init_delayable(&tick, tick_handler);
    struct zmk_esb_config config = {
        .mode = IS_ENABLED(CONFIG_CURVE_ESB_RECEIVER) ? ZMK_ESB_MODE_PRX : ZMK_ESB_MODE_PTX,
        .bitrate = ZMK_ESB_BITRATE_2MBPS, .crc = ZMK_ESB_CRC_16BIT,
        .tx_mode = ZMK_ESB_TXMODE_AUTO, .event_handler = esb_event,
        .retransmit_delay = 500, .retransmit_count = 5,
    };
    int ret = zmk_esb_init(&config);
    if (ret) { return ret; }
    uint8_t address[4];
    ret = ce_address(pair_key, address);
    if (ret) { zmk_esb_disable(); return ret; }
    uint8_t prefix = 0x5a;
    zmk_esb_set_base_address_0(address); zmk_esb_set_prefixes(&prefix, 1);
    zmk_esb_set_rf_channel(76); zmk_esb_set_tx_power(4);
    running = true;
    if (IS_ENABLED(CONFIG_CURVE_ESB_RECEIVER)) { ret = zmk_esb_start_rx(); }
    else { ret = new_keyboard_session(); }
    if (ret) { running = false; zmk_esb_disable(); return ret; }
    k_work_reschedule_for_queue(&work_queue, &tick, K_MSEC(100));
    return 0;
}
SYS_INIT(init, APPLICATION, 95);
