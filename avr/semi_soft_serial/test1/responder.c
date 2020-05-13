#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr_io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <semi_soft_serial.h>
#include "debug_serial.h"

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

void skip_data(void)
{
    int16_t data;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        while( 1 ) {
            data = receive_byte();
            if (data == HDSS_ERROR) {
                hdss_get_receive_error();
                break;
            } else if (data == HDSS_NO_DATA) {
                break;
            }
        }
    }
}

uint8_t  data[HDSS_RECEIVE_BUFFER_SIZE+10];

int main(void)
{
    int16_t data0, data1;

    disable_usb();
    hdss_responder_init();
    debug_print_initiator_init();
    led_init();
    DELAY(2000);
    for( int i = 0; i < 10; i++ ) {
        led_blink(100,100);
    }
    DELAY(1000);
    sei();
    while(1) {
#ifdef PARITY_ENABLE
        hdss_set_parity_mode_even(true);
#endif
        data0 = receive_byte();
        data1 = receive_byte();
        if (data0 >= 0 && data1 >= 0) {
            data[0] = data0;
            data[1] = data1;
            debug_print_send_bytes(data, 2, false);
            if (data0 + 1 != data1) {
                led_blink(100,50);
                led_blink(100,50);
                skip_data();
            } else {
                led_blink(100,50);
            }
        } else {
            led_blink(100,100);
            led_blink(100,100);
            led_blink(100,100);
            led_blink(100,100);
            skip_data();
        }
    }
}
