/* SPDX-License-Identifier: MIT */
#include <curve_esb/protocol.h>
#include <errno.h>
#include <string.h>
#include <mbedtls/ccm.h>
#include <mbedtls/md.h>
#include <mbedtls/platform_util.h>

static uint32_t get32(const uint8_t *p) {
    return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 |
           (uint32_t)p[3] << 24;
}
static void put32(uint8_t *p, uint32_t v) {
    for (int i = 0; i < 4; i++) { p[i] = v >> (8 * i); }
}
static int mac(const uint8_t key[16], const uint8_t *in, size_t len, uint8_t out[32]) {
    const mbedtls_md_info_t *md = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
    if (!md) { return -ENOTSUP; }
    return mbedtls_md_hmac(md, key, 16, in, len, out) ? -EIO : 0;
}
static bool equal_tag(const uint8_t *a, const uint8_t *b) {
    uint8_t diff = 0;
    for (int i = 0; i < 8; i++) { diff |= a[i] ^ b[i]; }
    return diff == 0;
}
static int authenticate(const uint8_t key[16], const uint8_t *in, size_t n,
                        const uint8_t tag[8]) {
    uint8_t digest[32];
    int ret = mac(key, in, n, digest);
    if (!ret && !equal_tag(digest, tag)) { ret = -EACCES; }
    mbedtls_platform_zeroize(digest, sizeof(digest));
    return ret;
}
static int derive(const uint8_t key[16], struct ce_session *s) {
    /* Separate handshake authentication from the session-key derivation domain. */
    uint8_t input[28] = "curve-esb-v1";
    uint8_t digest[32];
    memcpy(input + 12, s->device_nonce, 8);
    memcpy(input + 20, s->receiver_nonce, 8);
    int ret = mac(key, input, sizeof(input), digest);
    if (!ret) {
        memcpy(s->key, digest, 16);
        s->counter = 0;
        s->acknowledged = 0;
        s->received = false;
        s->established = true;
    }
    mbedtls_platform_zeroize(digest, sizeof(digest));
    return ret;
}
int ce_body_size(uint8_t type) {
    switch (type) {
    case CE_KEYBOARD: return CE_KEYBOARD_SIZE;
    case CE_CONSUMER: return CE_CONSUMER_SIZE;
    case CE_MOUSE: return CE_MOUSE_SIZE;
    case CE_KEEPALIVE: return 0;
    default: return -EINVAL;
    }
}
int ce_hello(const uint8_t psk[16], const uint8_t nonce[8], uint8_t out[CE_HELLO_SIZE]) {
    out[0] = CE_VERSION; out[1] = CE_HELLO;
    memcpy(out + 2, nonce, 8);
    uint8_t digest[32];
    int ret = mac(psk, out, 10, digest);
    if (!ret) { memcpy(out + 10, digest, 8); }
    mbedtls_platform_zeroize(digest, sizeof(digest));
    return ret;
}
int ce_challenge(const uint8_t psk[16], const uint8_t hello[CE_HELLO_SIZE],
                 const uint8_t nonce[8], struct ce_session *s,
                 uint8_t out[CE_CHALLENGE_SIZE]) {
    if (hello[0] != CE_VERSION || hello[1] != CE_HELLO) { return -EPROTO; }
    int ret = authenticate(psk, hello, 10, hello + 10);
    if (ret) { return ret; }
    struct ce_session next = {0};
    memcpy(next.device_nonce, hello + 2, 8);
    memcpy(next.receiver_nonce, nonce, 8);
    out[0] = CE_VERSION; out[1] = CE_CHALLENGE;
    memcpy(out + 2, next.device_nonce, 8);
    memcpy(out + 10, next.receiver_nonce, 8);
    uint8_t digest[32];
    ret = mac(psk, out, 18, digest);
    if (!ret) { memcpy(out + 18, digest, 8); ret = derive(psk, &next); }
    if (!ret) { *s = next; }
    mbedtls_platform_zeroize(digest, sizeof(digest));
    mbedtls_platform_zeroize(&next, sizeof(next));
    return ret;
}
int ce_accept(const uint8_t psk[16], const uint8_t challenge[CE_CHALLENGE_SIZE],
              struct ce_session *s) {
    if (s->established) { return -EALREADY; }
    if (challenge[0] != CE_VERSION || challenge[1] != CE_CHALLENGE ||
        memcmp(challenge + 2, s->device_nonce, 8)) { return -EPROTO; }
    int ret = authenticate(psk, challenge, 18, challenge + 18);
    if (ret) { return ret; }
    memcpy(s->receiver_nonce, challenge + 10, 8);
    return derive(psk, s);
}
static void nonce_for(const struct ce_session *s, uint32_t counter, uint8_t nonce[13]) {
    memcpy(nonce, s->receiver_nonce, 8);
    put32(nonce + 8, counter);
    nonce[12] = 0; /* keyboard -> receiver only; no encrypted reverse traffic in v1 */
}
int ce_seal(struct ce_session *s, uint8_t type, const uint8_t *body, size_t len,
            uint8_t out[CE_PACKET_MAX]) {
    if (!s->established) { return -ENOTCONN; }
    if (ce_body_size(type) < 0 || len != (size_t)ce_body_size(type)) { return -EINVAL; }
    if (s->counter == UINT32_MAX) { return -EOVERFLOW; }
    /* Consume the nonce even if encryption fails. */
    uint32_t counter = ++s->counter;
    out[0] = CE_VERSION; out[1] = type; put32(out + 2, counter);
    uint8_t nonce[13]; nonce_for(s, counter, nonce);
    mbedtls_ccm_context ctx;
    mbedtls_ccm_init(&ctx);
    int ret = mbedtls_ccm_setkey(&ctx, MBEDTLS_CIPHER_ID_AES, s->key, 128);
    if (!ret) {
        ret = mbedtls_ccm_encrypt_and_tag(&ctx, len, nonce, sizeof(nonce), out, 6,
                                          body, out + 6, out + 6 + len, 8);
    }
    mbedtls_ccm_free(&ctx);
    return ret ? -EIO : (int)(len + 14);
}
int ce_open(struct ce_session *s, const uint8_t *packet, size_t len,
            uint8_t *type, uint8_t body[CE_KEYBOARD_SIZE]) {
    if (!s->established) { return -ENOTCONN; }
    if (len < 14 || len > CE_PACKET_MAX || packet[0] != CE_VERSION) { return -EPROTO; }
    int size = ce_body_size(packet[1]);
    if (size < 0 || len != (size_t)size + 14) { return -EMSGSIZE; }
    uint32_t counter = get32(packet + 2);
    if (!counter || (s->received && counter <= s->counter)) { return -EACCES; }
    uint8_t nonce[13], plain[CE_KEYBOARD_SIZE]; nonce_for(s, counter, nonce);
    mbedtls_ccm_context ctx;
    mbedtls_ccm_init(&ctx);
    int ret = mbedtls_ccm_setkey(&ctx, MBEDTLS_CIPHER_ID_AES, s->key, 128);
    if (!ret) {
        ret = mbedtls_ccm_auth_decrypt(&ctx, size, nonce, sizeof(nonce), packet, 6,
                                      packet + 6, plain, packet + 6 + size, 8);
    }
    mbedtls_ccm_free(&ctx);
    if (ret) { mbedtls_platform_zeroize(plain, sizeof(plain)); return -EACCES; }
    memcpy(body, plain, size);
    mbedtls_platform_zeroize(plain, sizeof(plain));
    *type = packet[1]; s->counter = counter; s->received = true;
    return size;
}

int ce_status(const struct ce_session *s, uint8_t out[14]) {
    if (!s->established || !s->received) { return -ENOTCONN; }
    out[0] = CE_VERSION; out[1] = CE_STATUS; put32(out + 2, s->counter);
    uint8_t digest[32];
    int ret = mac(s->key, out, 6, digest);
    if (!ret) { memcpy(out + 6, digest, 8); }
    mbedtls_platform_zeroize(digest, sizeof(digest));
    return ret;
}
int ce_accept_status(struct ce_session *s, const uint8_t packet[14]) {
    if (!s->established || packet[0] != CE_VERSION || packet[1] != CE_STATUS) {
        return -EPROTO;
    }
    uint32_t counter = get32(packet + 2);
    if (!counter || counter > s->counter || counter <= s->acknowledged) { return -EACCES; }
    int ret = authenticate(s->key, packet, 6, packet + 6);
    if (!ret) { s->acknowledged = counter; }
    return ret;
}

int ce_address(const uint8_t psk[16], uint8_t out[4]) {
    static const uint8_t label[] = "curve-esb-address-v1";
    uint8_t digest[32];
    int ret = mac(psk, label, sizeof(label) - 1, digest);
    if (!ret) { out[0] = 0xc3; memcpy(out + 1, digest, 3); }
    mbedtls_platform_zeroize(digest, sizeof(digest));
    return ret;
}
