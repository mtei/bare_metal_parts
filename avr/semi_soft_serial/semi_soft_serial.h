/* AVR half-duplex(one-wire) semi-soft asynchronous serial communication routines */
/* clang-format off */
#ifndef HALF_DUPLEX_SEMI_SOFT_SERIAL_H
#define HALF_DUPLEX_SEMI_SOFT_SERIAL_H
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
void HDSS_SEND_BYTES(const uint8_t *datap, uint16_t datalen, _Bool change_receiver);

#ifdef __cplusplus
}
#endif

#endif /* HALF_DUPLEX_SEMI_SOFT_SERIAL_H */
