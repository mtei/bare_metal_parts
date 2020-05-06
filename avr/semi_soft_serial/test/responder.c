#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr_io.h>
#include <avr/interrupt.h>
#include <semi_soft_serial.h>

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

int main(void)
{
    int16_t data0, data1;

    disable_usb();
    hdss_responder_init();
    led_init();
    DELAY(2000);
    for( int i = 0; i < 10; i++ ) {
        led_blink(100,100);
    }
    sei();
    while(1) {
#ifdef PARITY_ENABLE
        hdss_set_parity_mode_even(true);
#endif
        for( uint16_t i = 0; i <= 255; i++ ) {
            data1 = 0;
            data0 = receive_byte();
            if (data0 >= 0)
                data1 = receive_byte();
            if (data0 < 0) {
                errled_blink(50,100);
            } else if (data1 < 0) {
                errled_blink(100,50);
            } else if (data0 + 1 != data1) {
                errled_blink(50,30);
                errled_blink(50,30);
            } else {
                //errled_set(false);
                errled_blink(50,100);
                led_blink(100,50);
            }
        }
        led_blink(100,900);
#ifdef PARITY_ENABLE
        hdss_set_parity_mode_even(false);
#endif
        for( int16_t i = 255; i >= 0; i-- ) {
            data1 = 0;
            data0 = receive_byte();
            if (data0 >= 0)
                data1 = receive_byte();
            if (data0 < 0) {
                errled_blink(50,100);
            } else if (data1 < 0) {
                errled_blink(100,50);
            } else if (data0 + 1 != data1) {
                errled_blink(50,30);
                errled_blink(50,30);
            } else {
                errled_set(false);
                errled_blink(50,100);
                led_blink(100,50);
            }
        }
        led_blink(100,900);
    }
}
