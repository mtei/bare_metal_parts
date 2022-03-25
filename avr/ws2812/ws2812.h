// Copyright (c) 2020 Takeshi Ishii (mtei@github)
// SPDX-License-Identifier: MIT

// simple WS2812 data output routine for AVR with 16MHz or higher clock

/* clang-format off */
#ifndef WS2812_SEND_BYTES_H
#define WS2812_SEND_BYTES_H
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WS2812_FUNC_PREFIX
  #define _JOIN_NAME(prefix, body) prefix##body
  #define JOIN_NAME(prefix, body) _JOIN_NAME(prefix, body)
  #define WS2812_INIT JOIN_NAME(WS2812_FUNC_PREFIX,_init)
  #define WS2812_SEND_BYTES JOIN_NAME(WS2812_FUNC_PREFIX,_send_bytes)
#else
  #define WS2812_INIT ws2812_init
  #define WS2812_SEND_BYTES ws2812_send_bytes
#endif


    /* WS2812_DI_PIN */
#ifndef WS2812_AUTO_INIT
    void WS2812_INIT(void);
#endif
    void WS2812_SEND_BYTES(const uint8_t *datap, uint16_t datalen);

    /* WS2812_DI_FREEPIN*/
#ifndef WS2812_AUTO_INIT
    void ws2812_init_port(volatile uint8_t *port, uint8_t bitpattern);
#endif
    void ws2812_send_bytes_port(const uint8_t *datap, uint16_t datalen,
                                volatile uint8_t *port, uint8_t bitpattern);

#ifdef __cplusplus
}
#endif

#endif /* WS2812_SEND_BYTES_H */
