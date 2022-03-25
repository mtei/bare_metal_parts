// Copyright (c) 2020 Takeshi Ishii (mtei@github)
// SPDX-License-Identifier: MIT

// AVR half-duplex(one-wire) semi-soft asynchronous serial communication routines

/* clang-format off */
#ifndef HALF_DUPLEX_SEMI_SOFT_SERIAL_H
#define HALF_DUPLEX_SEMI_SOFT_SERIAL_H
#pragma once

#include <stdint.h>
#include <stdbool.h>

// compile options
//  -DF_CPU=16000000UL
//  -DPARITY_ENABLE
//  -DAVR_UBRR_VALUE=0
//  -DHDSS_RECEIVE_BUFFER_SIZE=8

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HDSS_FUNC_PREFIX
  #define _JOIN_NAME(prefix, body) prefix##body
  #define JOIN_NAME(prefix, body) _JOIN_NAME(prefix, body)
  #define HDSS_INITIATOR_INIT JOIN_NAME(HDSS_FUNC_PREFIX,_initiator_init)
  #define HDSS_SEND_BYTES JOIN_NAME(HDSS_FUNC_PREFIX,_send_bytes)
#else
  #define HDSS_INITIATOR_INIT hdss_initiator_init
  #define HDSS_SEND_BYTES hdss_send_bytes
#endif

void HDSS_INITIATOR_INIT(void);
void hdss_responder_init(void);
void HDSS_SEND_BYTES(const uint8_t *datap, uint16_t datalen, bool change_receiver);
void hdss_set_parity_mode_even(bool is_even);

int16_t hdss_receive_byte(void);
#define HDSS_NO_DATA  -1
#define HDSS_ERROR    -2

int8_t hdss_get_receive_error(void);
#define HDSS_ERROR_FRAMING       0x01
#define HDSS_ERROR_PARITY        0x02
#define HDSS_ERROR_OVERUN        0x04 /* hardware buffer overrun */
#define HDSS_ERROR_BUFFER_OVERUN 0x08 /* software buffer overrun */

int16_t hdss_get_receive_counter(void);

#ifdef __cplusplus
}
#endif

#endif /* HALF_DUPLEX_SEMI_SOFT_SERIAL_H */
