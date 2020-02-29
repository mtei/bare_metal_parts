/* simple WS2812 data output routine for AVR with 16MHz or higher clock */
/* clang-format off */
#ifndef WS2812_SEND_BYTES_H
#define WS2812_SEND_BYTES_H
#pragma once

/*
MIT License

Copyright (c) 2020 Takeshi Ishii

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WS2812_FUNC_PREFIX
  #define _JOIN_NAME(prefix, body) prefix##body
  #define JOIN_NAME(prefix, body) _JOIN_NAME(prefix, body)
  #define WS2812_SEND_BYTES JOIN_NAME(WS2812_FUNC_PREFIX,_send_bytes)
#else
  #define WS2812_SEND_BYTES ws2812_send_bytes
#endif

void WS2812_SEND_BYTES(const uint8_t *datap, uint16_t datalen);

#ifdef __cplusplus
}
#endif

#endif /* WS2812_SEND_BYTES_H */
