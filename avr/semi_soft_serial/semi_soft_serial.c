/* AVR half-duplex(one-wire) semi-soft asynchronous serial communication routines */
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

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>
#include "semi_soft_serial.h"

#ifndef __OPTIMIZE__
  #warning "Compiler optimizations disabled; functions from semi_soft_serial.c won't work as designed"
#endif

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__)
/* alternate function PD0:RXD,PCINT16, PD1:TXD,PCINT17 */
#define USART_RX_PORT D,0
#define USART_TX_PORT D,1

#define RX_vect    USART_RX_vect
#define UDRE_vect  USART_UDRE_vect
#define TX_vect    USART_TX_vect

#define UBRRL      UBRR0L   /* USART Baud Rate Register 0 Low */
#define UBRRH      UBRR0H   /* USART Baud Rate Register 0 High */
#define UDR        UDR0     /* USART I/O Data Register 0 */
#define UCSRA      UCSR0A   /* USART Control and Status Register 0 A */
#define UCSRB      UCSR0B   /* USART Control and Status Register 0 B */
#define UCSRC      UCSR0C   /* USART Control and Status Register 0 C */

#elif defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega32U2__)
/* alternate function PD2:RXD1,INT2, PD3:TXD1,INT3 */
#define USART_RX_PORT D,2
#define USART_TX_PORT D,3

#define RX_vect    USART1_RX_vect
#define UDRE_vect  USART1_UDRE_vect
#define TX_vect    USART1_TX_vect

#define UBRRL      UBRR1L   /* USART Baud Rate Register 1 Low */
#define UBRRH      UBRR1H   /* USART Baud Rate Register 1 High */
#define UDR        UDR1     /* USART I/O Data Register 1 */
#define UCSRA      UCSR1A   /* USART Control and Status Register 1 A */
#define UCSRB      UCSR1B   /* USART Control and Status Register 1 B */
#define UCSRC      UCSR1C   /* USART Control and Status Register 1 C */

#elif defined (__AVR_ATtiny2313__)
/* alternate function PD0:RXD, PD1:TXD */
#define USART_RX_PORT D,0
#define USART_TX_PORT D,1

#define RX_vect    USART_RX_vect
#define UDRE_vect  USART_UDRE_vect
#define TX_vect    USART_TX_vect

//#define UBRRL      UBRRL   /* USART Baud Rate Register 0 Low */
//#define UBRRH      UBRRH   /* USART Baud Rate Register 0 High */
//#define UDR        UDR     /* USART I/O Data Register 0 */
//#define UCSRA      UCSRA   /* USART Control and Status Register A */
//#define UCSRB      UCSRB   /* USART Control and Status Register B */
//#define UCSRC      UCSRC   /* USART Control and Status Register C */

#else
   #error semi_soft_serial.c not yet support your AVR
#endif

/* USART Control and Status Register x A */
#define   RXCV   (1<<7)   /*   Receive Complate (R only) */
#define   TXCV   (1<<6)   /*   Transmit Complate (R/W) */
#define   UDREV  (1<<5)   /*   Transmit Data Register Empty (R only) */
#define   FEV    (1<<4)   /*   Framing Error (R only) */
#define   DORV   (1<<3)   /*   Data OverRun (R only) */
#define   UPEV   (1<<2)   /*   Parity Error (R only) */
#define   U2XV   (1<<1)   /*   Double the USART Transmission Speed (R/W) */
#define   MPCMV  (1<<0)   /*   Multi-Procesor Communication Mode (R/W) */
/* USART Control and Status Register x B */
#define   RXCIEV (1<<7)   /*   Receive Complete Interrupt Enable (R/W) */
#define   TXCIEV (1<<6)   /*   Transmit Complete Inter Enable (R/W) */
#define   UDRIEV (1<<5)   /*   Transmit Data Regi Empty Inter Enable(R/W)*/
#define   RXENV  (1<<4)   /*   Receiver Enable (R/W) */
#define   TXENV  (1<<3)   /*   Transmitter Enable (R/W) */
#define   UCSZ2V (1<<2)   /*   Character Size (9bit) (R/W) */
#define   RXB8V  (1<<1)   /*   Receive Data Bit 8 (R only) */
#define   TXB8V  (1<<0)   /*   Transmit Data Bit 8 (R/W) */
/* USART Control and Status Register x C */
#define   UMSEL_ASYNC  0     /*   Mode Select bit 7:6 (R/W) */
#define   UPM_NONE     0     /*   Parity Mode bit 5:4 (R/W) */
#define   UPM_EVEN     0x20
#define   UPM_ODD      0x30
#define   USBS_1       0     /*   Stop Bit Select 1,2 (R/W) */
#define   USBS_2       0x8
#define   UCSZ_5       0     /*   Character Size 5,6,7,8 */
#define   UCSZ_6       0x2
#define   UCSZ_7       0x4
#define   UCSZ_8       0x6

#ifndef SOFT_TX_PORT
  /* One-wire half-duplex operation: Use the hardware RXD pin for soft TX */
  #define SOFT_TX_PORT USART_RX_PORT
#else
  /* transmission only */
  #define HDSS_TRANSMISSION_ONLY
#endif

#ifndef AVR_UBRR_VALUE
  #define AVR_UBRR_VALUE  0
#endif

#define ODD_PARITY  0xff
#define EVEN_PARITY 0

static uint8_t parity_mode = EVEN_PARITY;

#ifdef PARITY_ENABLE
void hdss_set_parity_mode_even(bool is_even)
{
    if (is_even) {
        parity_mode = EVEN_PARITY;
#ifndef HDSS_TRANSMISSION_ONLY
        UCSRC = UPM_EVEN | (UCSRC & ~(UPM_EVEN|UPM_ODD));
#endif
    } else {
        parity_mode = ODD_PARITY;
#ifndef HDSS_TRANSMISSION_ONLY
        UCSRC = UPM_ODD | (UCSRC & ~(UPM_EVEN|UPM_ODD));
#endif
    }
}
#endif

#define _PINx(p,b)  PIN##p
#define PINx(p)  _PINx(p)
#define _DDRx(p,b)  DDR##p
#define DDRx(p)  _DDRx(p)
#define _PORTx(p,b) PORT##p
#define PORTx(p) _PORTx(p)
#define _P_BITx(p,b) b
#define P_BITx(p) _P_BITx(p)

#ifdef HDSS_DEBUG_PIN
#define DEBUG_PIN_INIT DDRx(HDSS_DEBUG_PIN) |= _BV(P_BITx(HDSS_DEBUG_PIN));
#define DEBUG_PIN_ON   PORTx(HDSS_DEBUG_PIN) |= _BV(P_BITx(HDSS_DEBUG_PIN));
#define DEBUG_PIN_OFF  PORTx(HDSS_DEBUG_PIN) &= ~_BV(P_BITx(HDSS_DEBUG_PIN));
#else
#define DEBUG_PIN_INIT
#define DEBUG_PIN_ON
#define DEBUG_PIN_OFF
#endif

#define AVR_USART_SAMPLES 16
#define BITWIDTH_CYCLE (AVR_USART_SAMPLES * (AVR_UBRR_VALUE + 1))

static void hdss_init_common(void)
{
    DEBUG_PIN_INIT;
#ifndef HDSS_TRANSMISSION_ONLY
    //#error not yet test hdss_init_xxx()
    UBRRH = (AVR_UBRR_VALUE >> 8) ;
    UBRRL = (AVR_UBRR_VALUE & 0xff);
    UCSRA = 0;
    UCSRB = 0;
    UCSRC = (UMSEL_ASYNC | UPM_NONE | USBS_1 | UCSZ_8);
  #ifdef PARITY_ENABLE
    hdss_set_parity_mode_even(parity_mode == EVEN_PARITY);
  #endif
#endif
}

static void hdss_change_sender(void)
{
    DDRx(SOFT_TX_PORT)  |= _BV(P_BITx(SOFT_TX_PORT)); // set output mode
    PORTx(SOFT_TX_PORT) |= _BV(P_BITx(SOFT_TX_PORT)); // set output HIGH
#ifndef HDSS_TRANSMISSION_ONLY
    UCSRB &= ~(RXCIEV | RXENV);
#endif
}

#ifndef HDSS_TRANSMISSION_ONLY
static void hdss_change_receiver(void)
{
    DDRx(SOFT_TX_PORT)  &= ~_BV(P_BITx(SOFT_TX_PORT)); // set input mode
    PORTx(SOFT_TX_PORT) |= _BV(P_BITx(SOFT_TX_PORT));  // set pullup
    UCSRB |= (RXCIEV | RXENV);
}
#endif

void HDSS_INITIATOR_INIT(void)
{
    uint8_t sreg_prev;
    sreg_prev = SREG;
    cli();
    hdss_init_common();
    hdss_change_sender();
    SREG = sreg_prev;
}

//__builtin_avr_delay_cycles(4);   //S13,14, 15,16
//(16-4) = 12  bit_loop
#define BIT_LOOP_STEPS 12
#define bit_loop_delay       (BITWIDTH_CYCLE - BIT_LOOP_STEPS)

//__builtin_avr_delay_cycles(6); //S11,12, 13,14, 15,16
//(16-10) = 6  parity_bit
#define PARITY_BIT_STEPS 10
#define parity_bit_delay     (BITWIDTH_CYCLE - PARITY_BIT_STEPS)

//__builtin_avr_delay_cycles(12); //S5,6, 7,8, 9,10, 11,12 ,13,14, 15,16
//(16-12) = 4   paritystop_bit
#define PARITYSTOP_BIT_STEPS 4
#define paritystop_bit_delay (BITWIDTH_CYCLE - PARITYSTOP_BIT_STEPS)

//__builtin_avr_delay_cycles(8); //S9,10, 11,12 ,13,14, 15,16
//(16-8) = 8  stop_bit
#define STOP_BIT_STEPS 8
#define stop_bit_delay       (BITWIDTH_CYCLE - STOP_BIT_STEPS)

//__builtin_avr_delay_cycles(6); //S11,12, 13,14, 15,16
//(16-6) = 10   byte_interval
#define BYTE_INTERVAL_STEP 10
#define byte_interval_delay  (BITWIDTH_CYCLE - BYTE_INTERVAL_STEP)

#define SEMISOFT_SERIAL_PORT _SFR_IO_ADDR(PORTx(SOFT_TX_PORT))
#define SEMISOFT_SERIAL_PIN  (0xFF)&(~_BV(P_BITx(SOFT_TX_PORT)))

#define ASMV asm volatile
#define ASM_OP1 \
    :  [tmp]     "=d"(tmp)     \
     , [cbyte]   "=r"(cbyte)   \
     , [datap]   "+z"(datap)   \
     , [datalen] "+x"(datalen) \
     , [obuf]    "=d"(obuf)    \
     , [bitcnt]  "=d"(bitcnt)  \
     , [parity]  "=d"(parity)  \
    :  [PORT] "I"(SEMISOFT_SERIAL_PORT) \
     , [MASK] "M"(SEMISOFT_SERIAL_PIN) \
     , [parity_init] "r" (parity_init) \

#define ASM_OP2 \
    :  [tmp]     "+d"(tmp)     \
     , [cbyte]   "+r"(cbyte)   \
     , [datap]   "+z"(datap)   \
     , [datalen] "+x"(datalen) \
     , [obuf]    "+d"(obuf)    \
     , [bitcnt]  "+d"(bitcnt)  \
     , [parity]  "+d"(parity)  \
    :  [PORT] "I"(SEMISOFT_SERIAL_PORT) \
     , [MASK] "M"(SEMISOFT_SERIAL_PIN) \
     , [parity_init] "r" (parity_init) \

#define delay_cycles(x) __builtin_avr_delay_cycles(x)

__attribute__((noinline))
void HDSS_SEND_BYTES(const uint8_t *datap, uint16_t datalen, bool change_receiver)
{
    uint8_t tmp, cbyte, obuf, bitcnt, parity, parity_init;
    uint8_t sreg_prev;

    if (datalen == 0) { return; }

    parity_init = parity_mode;
    sreg_prev = SREG;
    cli();
    ASMV(
         "        in    %[obuf], %[PORT]"   "\n\t"
         /* byte_loop: */ "10: \n\t"
         "        ld    %[cbyte], Z+"       "\n\t"  //S6,S7 C1
         "        ldi   %[bitcnt], 9"       "\n\t"  //S8
         "        mov   %[parity], %[parity_init]" "\n\t"  // S9
         "        andi  %[obuf], %[MASK]"   "\n\t"  //S10(*) -- set start bit
         /* bit_loop: */  "20:  \n\t"
         "        out   %[PORT], %[obuf]"   "\n\t"  //S1
         "        eor   %[parity], %[obuf]" "\n\t"  //S2
         "        dec   %[bitcnt]"          "\n\t"  //S3
         "        breq  30f  \n\t" // parity_bit:   //S4(S5 -> C2)

         "        andi  %[obuf], %[MASK]"   "\n\t"  //S5
         "        ldi   %[tmp], %[MASK]"    "\n\t"  //S6
         "        com   %[tmp]"             "\n\t"  //S7
         "        sbrc  %[cbyte], 0"        "\n\t"  //S8
         "        or    %[obuf], %[tmp]"    "\n\t"  //S9
         "        lsr   %[cbyte]"           "\n\t"  //S10
         ASM_OP1); delay_cycles(bit_loop_delay); ASMV(
         "        rjmp  20b   \n\t" // bit_loop:    // S11,S12 (*)

         /* parity_bit: */ "30: \n\t"
#ifdef PARITY_ENABLE
         "        ldi   %[tmp], %[MASK]"    "\n\t"  //S6 C2
         "        and   %[obuf], %[tmp]"    "\n\t"  //S7
         "        com   %[tmp]"             "\n\t"  //S8
         "        and   %[parity], %[tmp]"  "\n\t"  //S9
         "        or    %[obuf], %[parity]" "\n\t"  //S10 (*) -- set parity bit
         ASM_OP2); delay_cycles(parity_bit_delay); ASMV(
         "        out   %[PORT], %[obuf]"   "\n\t"  //S1 -- out parity bit
#endif /* PARITY_ENABLE */
         /* stop_bit: */
         "        ldi   %[tmp], %[MASK]"    "\n\t"  //S2 | S6 C2'
         "        com   %[tmp]"             "\n\t"  //S3 | S7
         "        or    %[obuf], %[tmp]"    "\n\t"  //S4(p*) | S8(*) -- set stop bit
         ASM_OP2);
#ifdef PARITY_ENABLE
    delay_cycles(paritystop_bit_delay);
#else /* ! PARITY_ENABLE */
    delay_cycles(stop_bit_delay);
#endif /* PARITY_ENABLE */
    ASMV(
         "        out   %[PORT], %[obuf]"   "\n\t"  //S1 -- out stop bit
         ASM_OP2); delay_cycles(byte_interval_delay); ASMV(
         "        subi  r26,1"              "\n\t"  //S2 -- dec X
         "        sbc   r27, __zero_reg__"  "\n\t"  //S3
         "        brne  10b   \n\t" // byte_loop:   //S4,S5 -> C1
         ASM_OP2);
#ifndef HDSS_TRANSMISSION_ONLY
    if (change_receiver) {
        hdss_change_receiver();
    }
#endif
    SREG = sreg_prev;
}

#ifndef HDSS_TRANSMISSION_ONLY
void hdss_responder_init(void)
{
    uint8_t sreg_prev;
    sreg_prev = SREG;
    cli();
    hdss_init_common();
    hdss_change_receiver();
    SREG = sreg_prev;
}

#ifndef HDSS_RECEIVE_BUFFER_SIZE
  #define HDSS_RECEIVE_BUFFER_SIZE 8
#endif

typedef struct receive_buf_t {
    uint8_t  status;
    uint8_t  count;
    uint8_t  index;
    uint8_t  buf[HDSS_RECEIVE_BUFFER_SIZE];
} receive_buf_t;

static receive_buf_t receive_buf = {};

static inline
void queue_data(uint8_t data)
{
    uint8_t index;
    if (receive_buf.count < sizeof(receive_buf.buf)) {
        index = (receive_buf.index+receive_buf.count) % sizeof(receive_buf.buf);
        receive_buf.buf[index] = data;
        receive_buf.count ++;
    }
}

static inline
int16_t dequeue_data(void)
{
    uint8_t result;
    if (receive_buf.count > 0) {
        result = receive_buf.buf[receive_buf.index];
        receive_buf.count --;
        receive_buf.index = (receive_buf.index + 1) % sizeof(receive_buf.buf);
        return result;
    }
    return HDSS_NO_DATA;
}

ISR(RX_vect) {
    uint8_t status;
    DEBUG_PIN_ON;
    if (receive_buf.status) {
        /* already error */
        while (UCSRA & RXCV) {
            UDR; /* Ignore incoming data */
        }
        DEBUG_PIN_OFF;
        return;
    }
    status = (UCSRA & (RXCV|FEV|DORV|UPEV));
    if (status & (FEV|DORV|UPEV)) {
        receive_buf.status = status;
        while (UCSRA & RXCV) {
            UDR; /* Ignore incoming data */
        }
        DEBUG_PIN_OFF;
        return;
    } else if (status & RXCV) {
        uint8_t data;
        data = UDR;
        if (receive_buf.count < sizeof(receive_buf.buf)) {
            queue_data(data);
        } else {
            receive_buf.status = DORV;
        }
    }
    DEBUG_PIN_OFF;
}

int16_t hdss_receive_byte(void)
{
    int16_t result;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        result = dequeue_data();
        if (result == HDSS_NO_DATA && receive_buf.status) {
            result = HDSS_ERROR;
        }
    }
    return result;
}

int8_t hdss_get_receive_error(void)
{
    int8_t result;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        result = 0;
        if (receive_buf.status) {
            if (receive_buf.status & FEV) {
                result |= HDSS_FRAMING_ERROR;
            }
            if (receive_buf.status & UPEV) {
                result |= HDSS_PARITY_ERROR;
            }
            if (receive_buf.status & DORV) {
                result |= HDSS_OVERUN_ERROR;
            }
            receive_buf.status = 0;
        }
    }
    return result;
}

#endif /* HDSS_TRANSMISSION_ONLY */
