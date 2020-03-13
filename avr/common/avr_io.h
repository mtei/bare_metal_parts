#ifndef _AVR_IO_EXTEND_WRAP_H_
#define _AVR_IO_EXTEND_WRAP_H_
#pragma once
/* clang-format off */
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

#include <stddef.h>
#include <avr/io.h>

#ifdef __cplusplus
extern "C" {
#endif

#define _PINx(p,b)   PIN##p
#define PINx(p)      _PINx(p)
#define _DDRx(p,b)   DDR##p
#define DDRx(p)      _DDRx(p)
#define _PORTx(p,b)  PORT##p
#define PORTx(p)     _PORTx(p)
#define _P_BITx(p,b) b
#define P_BITx(p)    _P_BITx(p)

typedef struct avr_gpio_reg {
    volatile uint8_t pin;
    volatile uint8_t ddr;
    volatile uint8_t port;
} avr_gpio_reg;

#define GET_STRUCT_TOP(ptr, struct_name, ptr_member_name)               \
    ( (struct_name *)                                                   \
      ( (uint8_t *) (ptr) - offsetof(struct_name, ptr_member_name) )    \
    )

#define PORT2DDR(portreg)  (GET_STRUCT_TOP(portreg, avr_gpio_reg, port)->ddr)
// usage: PORT2DDR( &PORTB )
#define PIN2DDR(portreg)  (GET_STRUCT_TOP(portreg, avr_gpio_reg, pin)->ddr)
// usage: PIN2DDR( &PINA )

#ifdef __cplusplus
}
#endif
#endif /* _AVR_IO_EXTEND_WRAP_H_ */
