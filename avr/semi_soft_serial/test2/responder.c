#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr_io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <semi_soft_serial.h>
#include "debug_serial.h"

#ifdef HDSS_DEBUG_PIN
#define DEBUG_PIN_INIT DDRx(HDSS_DEBUG_PIN) |= _BV(P_BITx(HDSS_DEBUG_PIN));
#define DEBUG_PIN_ON   PORTx(HDSS_DEBUG_PIN) |= _BV(P_BITx(HDSS_DEBUG_PIN));
#define DEBUG_PIN_OFF  PORTx(HDSS_DEBUG_PIN) &= ~_BV(P_BITx(HDSS_DEBUG_PIN));
#else
#define DEBUG_PIN_INIT
#define DEBUG_PIN_ON
#define DEBUG_PIN_OFF
#endif

#if defined(__AVR_ATmega328P__)
  #define LED_PORT  B,5    /* onboard LED */
  /* #define LED_PORT  D,7  * digital 7 */
#elif defined(__AVR_ATmega32U4__)
  //          ====
  //  +-------====------+
  //  | D3/TXO      RAW |
  //  | D2/RXI      GND |
  //  | GND         RST |
  //  | GND         Vcc |
  //  | D1/SDA       F4 |
  //  | D0/SCL       F5 |
  //  | D4           F6 |
  //  | C6           F7 |
  //  | D7       SCK/B1 |
  //  | E6      MISO/B3 |
  //  | B4      MOSI/B2 |
  //  | B5           B6 |
  //  +-----------------+
  /* assume ProMicro */
  #define TX_LED_PORT D,5  /* onboard TX LED */
  #define RX_LED_PORT B,0  /* onboard RX LED */
  #define LED_PORT  RX_LED_PORT
  #define ERRLED_PORT  TX_LED_PORT
#else
  #error Unknown board
#endif

#define DELAY _delay_ms

void disable_usb(void)
{
#ifdef USBCON
    USBCON &= ~(_BV(USBE)|_BV(OTGPADE)|_BV(VBUSTE));
#endif
}

void led_init(void)
{
    DDRx(LED_PORT)  |= _BV(P_BITx(LED_PORT));  /* set output mode */
    PORTx(LED_PORT) |= _BV(P_BITx(LED_PORT));
    DDRx(ERRLED_PORT)  |= _BV(P_BITx(ERRLED_PORT));  /* set output mode */
    PORTx(ERRLED_PORT) |= _BV(P_BITx(ERRLED_PORT));
}

void led_blink(int on, int off)
{
    PORTx(LED_PORT) &= ~_BV(P_BITx(LED_PORT));
    while( on-- ) { DELAY(1); }
    PORTx(LED_PORT) |= _BV(P_BITx(LED_PORT));
    while( off-- ) { DELAY(1); }
}

void errled_set(_Bool on)
{
    if (on) {
        PORTx(ERRLED_PORT) &= ~_BV(P_BITx(ERRLED_PORT));
    } else {
        PORTx(ERRLED_PORT) |= _BV(P_BITx(ERRLED_PORT));
    }
}

void errled_blink(int on, int off)
{
    errled_set(true);
    while( on-- ) { DELAY(1); }
    errled_set(false);
    while( off-- ) { DELAY(1); }
}

int16_t receive_byte(void)
{
    int16_t data;
    do {
        data = hdss_receive_byte();
    } while (data == HDSS_NO_DATA);
    return data;
}

#ifndef DELTA
  #define DELTA 0
#endif
#ifndef BULK
  #define BULK 0
#endif

uint8_t  data[HDSS_RECEIVE_BUFFER_SIZE+DELTA*2+2];

int main(void)
{
    int16_t data0;

    disable_usb();
    hdss_responder_init();
    debug_print_initiator_init();
    led_init();
    DELAY(2000);
    for( int i = 0; i < 10; i++ ) {
        led_blink(100,100);
    }
    sei();
#if BULK == 0
    /* Read one byte at a time. */
    while(1) {
 #ifdef PARITY_ENABLE
        hdss_set_parity_mode_even(true);
 #endif
        data0 = receive_byte();
        if (data0 >= 0) {
            data[0] = data0;
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                debug_print_send_bytes(data, 1, false);
                //__builtin_avr_delay_cycles(8*13);
                //__builtin_avr_delay_cycles(8*16);
            }
        } else {
            DEBUG_PIN_OFF;
            data[0] = hdss_get_receive_error();
            data[1] = 0;
            DEBUG_PIN_ON;
            debug_print_send_bytes(data, 2, false);
            DEBUG_PIN_OFF;
        }
    }
#else
    /* BULK bytes read together. */
    while(1) {
 #ifdef PARITY_ENABLE
        hdss_set_parity_mode_even(true);
 #endif
        uint16_t i;
        for (i = 0; i < BULK; i++ ) {
            data0 = receive_byte();
            if (data0 >= 0) {
                data[i] = data0;
            } else {
                data[i] = 0xff;
                data[i+1] = hdss_get_receive_error();
                i++;
            }
        }
        debug_print_send_bytes(data, i, false);
    }
#endif
}
