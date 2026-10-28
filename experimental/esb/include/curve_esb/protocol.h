/* SPDX-License-Identifier: MIT */
#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define CE_VERSION 1
#define CE_HELLO 1
#define CE_CHALLENGE 2
#define CE_STATUS 3
#define CE_KEYBOARD 16
#define CE_CONSUMER 17
#define CE_MOUSE 18
#define CE_KEEPALIVE 19
#define CE_PACKET_MAX 32
#define CE_HELLO_SIZE 18
#define CE_CHALLENGE_SIZE 26
#define CE_KEYBOARD_SIZE 14
#define CE_CONSUMER_SIZE 12
#define CE_MOUSE_SIZE 9

/* One state per peer, owned by the transport's serialized work queue. */
struct ce_session {
    uint8_t key[16];
    uint8_t device_nonce[8];
    uint8_t receiver_nonce[8];
    uint32_t counter;
    uint32_t acknowledged;
    bool established;
    bool received;
};
int ce_hello(const uint8_t psk[16], const uint8_t nonce[8], uint8_t out[CE_HELLO_SIZE]);
int ce_challenge(const uint8_t psk[16], const uint8_t hello[CE_HELLO_SIZE],
                 const uint8_t nonce[8], struct ce_session *s,
                 uint8_t out[CE_CHALLENGE_SIZE]);
int ce_accept(const uint8_t psk[16], const uint8_t challenge[CE_CHALLENGE_SIZE],
              struct ce_session *s);
int ce_seal(struct ce_session *s, uint8_t type, const uint8_t *body, size_t len,
            uint8_t out[CE_PACKET_MAX]);
int ce_open(struct ce_session *s, const uint8_t *packet, size_t len,
            uint8_t *type, uint8_t body[CE_KEYBOARD_SIZE]);
int ce_body_size(uint8_t type);

int ce_status(const struct ce_session *s, uint8_t out[14]);
int ce_accept_status(struct ce_session *s, const uint8_t packet[14]);

int ce_address(const uint8_t psk[16], uint8_t out[4]);
