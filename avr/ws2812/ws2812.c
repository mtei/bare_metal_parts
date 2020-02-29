/* simple WS2812 data output routine for AVR with 16MHz or higher clock */
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

#include <stdint.h>
#include "avr_io.h"
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "ws2812.h"

#ifndef __OPTIMIZE__
  #error "Compiler optimizations disabled; functions from ws2812.c won't work as designed"
#endif
#ifndef WS2812_DI_PIN
  #error ws2812.c need define WS2812_DI_PIN
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

/* one cycle of 16MHz = 62.5ns */
#define BIT_TIME 1250  // 62.5ns * 20 = 1250.0
#define B1_TIME   312  // 62.5ns * 5  = 312.5
#define B2_TIME   375  // 62.5ns * 6  = 375.0
#define B3_TIME (BIT_TIME - B1_TIME - B2_TIME) // 62.5ns * 9  = 562.5

#define B1_CYCLES (((F_CPU / 1000) * B1_TIME + 500000) / 1000000)
#define B2_CYCLES (((F_CPU / 1000) * B2_TIME + 500000) / 1000000)
#define B3_CYCLES (((F_CPU / 1000) * B3_TIME + 500000) / 1000000)

#define WS2812_PORT _SFR_IO_ADDR(PORTx(WS2812_DI_PIN))
#define WS2812_PIN  (0xFF)&(_BV(P_BITx(WS2812_DI_PIN)))

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
    DDRx(WS2812_DI_PIN)  |= _BV(P_BITx(WS2812_DI_PIN));  /* set output mode */
    PORTx(WS2812_DI_PIN) &= ~_BV(P_BITx(WS2812_DI_PIN)); /* output LOW */
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

