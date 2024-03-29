// Copyright (c) 2020 Takeshi Ishii (mtei@github)
// SPDX-License-Identifier: MIT

// simple WS2812 data output routine for AVR with 16MHz or higher clock

/* clang-format off */
#include <stdint.h>
#include "avr_io.h"
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "ws2812.h"

#ifndef __OPTIMIZE__
  #error "Compiler optimizations disabled; functions from ws2812.c won't work as designed"
#endif
#if !defined(WS2812_DI_PIN) && !defined(WS2812_DI_FREEPIN)
  #error ws2812.c need define WS2812_DI_PIN or WS2812_DI_FREEPIN
#endif

/*
     +--------+
     |        |                |   0_code
   __|        |________________|
     +----------------+
     |                |        |   1_code
   __|                |________|
     |<------>|<----->|<------>|
         b1       b2      b3
     |<----------------------->|
            bit width
*/

#ifdef WS2812_AUTO_INIT
__attribute__((naked))
__attribute__((section(".init3")))
#endif
void WS2812_INIT(void)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        /* set output mode */
        DDRx(WS2812_DI_PIN)  |= _BV(P_BITx(WS2812_DI_PIN));
        /* set output LOW */
        PORTx(WS2812_DI_PIN) &= ~_BV(P_BITx(WS2812_DI_PIN));
    }
}

/* one cycle of 16MHz = 62.5ns */
#define BIT_TIME 1250  // 62.5ns * 20 = 1250.0
#define B1_TIME   312  // 62.5ns * 5  = 312.5
#define B2_TIME   375  // 62.5ns * 6  = 375.0
#define B3_TIME (BIT_TIME - B1_TIME - B2_TIME) // 62.5ns * 9  = 562.5

#define B1_CYCLES (((F_CPU / 1000) * B1_TIME + 500000) / 1000000)
#define B2_CYCLES (((F_CPU / 1000) * B2_TIME + 500000) / 1000000)
#define B3_CYCLES (((F_CPU / 1000) * B3_TIME + 500000) / 1000000)

/* Ruby script for checking calculation results
#! /usr/bin/ruby
F_CPU = 16000000
F_SYC = 1000.0/(F_CPU/1000000)

bit_time = 1250
b1_time = 312
b2_time = 375
b3_time = (bit_time - b1_time - b2_time)

b1_cycles = (((F_CPU / 1000) * b1_time + 500000) / 1000000)
b2_cycles = (((F_CPU / 1000) * b2_time + 500000) / 1000000)
b3_cycles = (((F_CPU / 1000) * b3_time + 500000) / 1000000)

print "b1_cycles = #{b1_cycles} (#{b1_cycles*F_SYC} nsec)\n"
print "b2_cycles = #{b2_cycles} (#{b2_cycles*F_SYC} nsec)\n"
print "b3_cycles = #{b3_cycles} (#{b3_cycles*F_SYC} nsec)\n"
print "total = #{(b1_cycles+b2_cycles+b3_cycles)*F_SYC} nsec\n"
 */

#define ASMV asm volatile

#ifdef WS2812_DI_PIN

#define WS2812_PORT _SFR_IO_ADDR(PORTx(WS2812_DI_PIN))
#define WS2812_PIN  (0xFF)&(_BV(P_BITx(WS2812_DI_PIN)))

#define ASM_OP1 \
    :  [cbyte]   "=r"(cbyte)   \
     , [datap]   "+z"(datap)   \
     , [datalen] "+x"(datalen) \
     , [bitcnt]  "=d"(bitcnt)  \
     , [obufh]   "=d"(obufh)   \
     , [obufl]   "=d"(obufl)   \
    :  [PORT] "I"(WS2812_PORT) \
     , [PIN]  "M"(WS2812_PIN)  \

#define ASM_OP2 \
    :  [cbyte]   "+r"(cbyte)   \
     , [datap]   "+z"(datap)   \
     , [datalen] "+x"(datalen) \
     , [bitcnt]  "+d"(bitcnt)  \
     , [obufh]   "+d"(obufh)   \
     , [obufl]   "+d"(obufl)   \
    :  [PORT] "I"(WS2812_PORT) \
     , [PIN]  "M"(WS2812_PIN)  \

#define delay_cycles(x) __builtin_avr_delay_cycles(x)

__attribute__((noinline))
void WS2812_SEND_BYTES(const uint8_t *datap, uint16_t datalen)
{
    uint8_t cbyte, bitcnt, obufh, obufl;
    uint8_t sreg_prev;

    if (datalen == 0) { return; }
    sreg_prev = SREG;
    cli();
#ifdef WS2812_EVERY_TIME_INIT
    DDRx(WS2812_DI_PIN)  |= _BV(P_BITx(WS2812_DI_PIN));  /* set output mode */
    PORTx(WS2812_DI_PIN) &= ~_BV(P_BITx(WS2812_DI_PIN)); /* output LOW */
#endif
    ASMV(
         "        in    %[obufh], %[PORT]"   "\n\t"
         "        ori   %[obufh], %[PIN]"    "\n\t"
         "        ldi   %[obufl], %[PIN]"    "\n\t"
         "        com   %[obufl]"            "\n\t"
         "        and   %[obufl], %[obufh]"  "\n\t"
         /* byte_loop: */ "10: \n\t"
         "        ld    %[cbyte], Z+"        "\n\t"  //S6,S7 C0
         "        ldi   %[bitcnt], 7"        "\n\t"  //S8
         /* bit_loop: */  "20:  \n\t"
         "        out   %[PORT], %[obufh]"   "\n\t"  //S1 C1  (B1)
         ASM_OP1); delay_cycles(B1_CYCLES - 2); ASMV(
         "        sbrs  %[cbyte],7"          "\n\t"  //S2
         "        out   %[PORT], %[obufl]"   "\n\t"  //S1     (B2)
         "        lsl   %[cbyte]"            "\n\t"  //S2
         ASM_OP2); delay_cycles(B2_CYCLES - 2); ASMV(
         "        out   %[PORT], %[obufl]"   "\n\t"  //S1     (B3)
         "        dec   %[bitcnt]"           "\n\t"  //S2
         "        breq  30f   \n\t" // last_bit:     //S3(S4 -> C2)
         ASM_OP2); delay_cycles(B3_CYCLES - 5); ASMV(
         "        rjmp  20b   \n\t" // bit_loop:     //S4,S5 -> C1

         /* last_bit: */ "30: \n\t"
         ASM_OP2); delay_cycles(B3_CYCLES - 4); ASMV(
         "        out   %[PORT], %[obufh]"   "\n\t"  //S1 C2  (B1)
         ASM_OP2); delay_cycles(B1_CYCLES - 2); ASMV(
         "        sbrs  %[cbyte],7"          "\n\t"  //S2
         "        out   %[PORT], %[obufl]"   "\n\t"  //S1     (B2)
         ASM_OP2); delay_cycles(B2_CYCLES - 1); ASMV(
         "        out   %[PORT], %[obufl]"   "\n\t"  //S1     (B3)
         ASM_OP2); delay_cycles(B3_CYCLES - 8); ASMV(
         "        subi  r26,1"               "\n\t"  //S2
         "        sbc   r27, __zero_reg__"   "\n\t"  //S3
         "        brne  10b   \n\t" // byte_loop:    //S4(S5 -> C0)
         );
    SREG = sreg_prev;
}
#endif /* WS2812_DI_PIN */

#ifdef WS2812_DI_FREEPIN

void ws2812_init_port(volatile uint8_t *port, uint8_t bitpattern)
{
    PORT2DDR(port) |= bitpattern;  /* set output mode */
    *port          &= ~bitpattern; /* output LOW */
}

#define ASM_OP3 \
    :  [tmp]     "=r"(tmp)     \
     , [cbyte]   "=r"(cbyte)   \
     , [datap]   "+z"(datap)   \
     , [datalen] "+x"(datalen) \
     , [bitcnt]  "=d"(bitcnt)  \
     , [obufh]   "=d"(obufh)   \
     , [obufl]   "=d"(obufl)   \
    :  [port]    "y"(port)     \
     , [pin]     "r"(bitpattern)  \

#define ASM_OP4 \
    :  [tmp]     "+r"(tmp)     \
     , [cbyte]   "+r"(cbyte)   \
     , [datap]   "+z"(datap)   \
     , [datalen] "+x"(datalen) \
     , [bitcnt]  "+d"(bitcnt)  \
     , [obufh]   "+d"(obufh)   \
     , [obufl]   "+d"(obufl)   \
    :  [port]    "y"(port)     \


__attribute__((noinline))
void ws2812_send_bytes_port(const uint8_t *datap, uint16_t datalen,
                       volatile uint8_t *port, uint8_t bitpattern)
{
    uint8_t cbyte, bitcnt, obufh, obufl, tmp;
    uint8_t sreg_prev;

    if (datalen == 0) { return; }
    sreg_prev = SREG;
    cli();
#ifdef WS2812_EVERY_TIME_INIT
    PORT2DDR(port) |= bitpattern;  /* set output mode */
    *port          &= ~bitpattern; /* output LOW */
#endif
    ASMV(
         "        ld    %[obufh], Y"         "\n\t"
         "        or    %[obufh], %[pin]"    "\n\t"
         "        mov   %[obufl], %[pin]"    "\n\t"
         "        com   %[obufl]"            "\n\t"
         "        and   %[obufl], %[obufh]"  "\n\t"
         /* byte_loop: */ "10: \n\t"
         "        ld    %[cbyte], Z+"        "\n\t"  //S7,S8 C0
         "        ldi   %[bitcnt], 7"        "\n\t"  //S9
         /* bit_loop: */  "20:  \n\t"
         "        st    Y, %[obufh]"         "\n\t"  //S1,S2 C1  (B1)
         ASM_OP3); delay_cycles(B1_CYCLES - 5); ASMV(
         "        mov   %[tmp],%[obufh]"     "\n\t"  //S3
         "        sbrs  %[cbyte],7"          "\n\t"  //S4
         "        mov   %[tmp],%[obufl]"     "\n\t"  //S5
         "        st    Y, %[tmp]"           "\n\t"  //S1,S2     (B2)
         "        lsl   %[cbyte]"            "\n\t"  //S3
         ASM_OP4); delay_cycles(B2_CYCLES - 3); ASMV(
         "        st    Y, %[obufl]"         "\n\t"  //S1,S2     (B3)
         "        dec   %[bitcnt]"           "\n\t"  //S3
         "        breq  30f   \n\t" // last_bit:     //S4(S5 -> C2)
         ASM_OP4); delay_cycles(B3_CYCLES - 6); ASMV(
         "        rjmp  20b   \n\t" // bit_loop:     //S5,S6 -> C1

         /* last_bit: */ "30: \n\t"
         ASM_OP4); delay_cycles(B3_CYCLES - 5); ASMV(
         "        st    Y, %[obufh]"         "\n\t"  //S1,S2 C2  (B1)
         ASM_OP4); delay_cycles(B1_CYCLES - 5); ASMV(
         "        mov   %[tmp],%[obufh]"     "\n\t"  //S3
         "        sbrs  %[cbyte],7"          "\n\t"  //S4
         "        mov   %[tmp],%[obufl]"     "\n\t"  //S5
         "        st    Y, %[tmp]"           "\n\t"  //S1,S2     (B2)
         ASM_OP4); delay_cycles(B2_CYCLES - 2); ASMV(
         "        st    Y, %[obufl]"         "\n\t"  //S1,S2     (B3)
         ASM_OP4); delay_cycles(B3_CYCLES - 9); ASMV(
         "        subi  r26,1"               "\n\t"  //S3
         "        sbc   r27, __zero_reg__"   "\n\t"  //S4
         "        brne  10b   \n\t" // byte_loop:    //S5(S6 -> C0)
         );
    SREG = sreg_prev;
}

#endif /* WS2812_DI_FREEPIN */
