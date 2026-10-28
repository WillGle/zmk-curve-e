/* SPDX-License-Identifier: MIT */
#pragma once
#include <stdbool.h>
#include <stdint.h>
struct k_work_q;
struct k_work_q *curve_esb_queue(void);
bool curve_esb_ready(void);
int curve_esb_send(uint8_t type);
/* Implemented by the guarded upstream endpoint patch. */
void curve_esb_endpoint_refresh(void);
