// Copyright (c) 2020 Takeshi Ishii (mtei@github)
// SPDX-License-Identifier: MIT

#ifndef _AVR_IO_EXTEND_WRAP_H_
#define _AVR_IO_EXTEND_WRAP_H_
#pragma once

/* clang-format off */
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
