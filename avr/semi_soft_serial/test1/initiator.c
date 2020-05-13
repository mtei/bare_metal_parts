#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr_io.h>
#include <semi_soft_serial.h>

#if defined(__AVR_ATmega328P__)
  /* assume Arduino UNO */
  // ANALOG  0 --> PC0
  // ANALOG  1 --> PC1
  // ...
  // ANALOG  4 --> PC4
  // ANALOG  5 --> PC5
  // DIGITAL 2 --> PD2
  // DIGITAL 3 --> PD3
  // ......
  // DIGITAL 6 --> PD6
  // DIGITAL 7 --> PD7
  // http://i.stack.imgur.com/b3YjT.jpg
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
#else
  #error Unknown board
#endif

#define DELAY _delay_ms

void led_init(void)
{
    DDRx(LED_PORT)  |= _BV(P_BITx(LED_PORT));  /* set output mode */
    PORTx(LED_PORT) |= _BV(P_BITx(LED_PORT));
}

void led_blink(int on, int off)
{
    PORTx(LED_PORT) &= ~_BV(P_BITx(LED_PORT));
    while( on-- ) { DELAY(1); }
    PORTx(LED_PORT) |= _BV(P_BITx(LED_PORT));
    while( off-- ) { DELAY(1); }
}

int main(void)
{
    uint8_t data[10];
    hdss_initiator_init();
    led_init();
    DELAY(2000);
    for( int i = 0; i < 10; i++ ) {
        led_blink(100,100);
    }

    while(1) {
#ifdef PARITY_ENABLE
        hdss_set_parity_mode_even(true);
#endif
        for( uint16_t i = 0; i <= 255; i++ ) {
            data[0] = i;
            data[1] = i+1;
            led_blink(100,900);
            hdss_send_bytes(data, 2, false);
        }
        led_blink(100,900);
#ifdef PARITY_ENABLE
        hdss_set_parity_mode_even(false);
#endif
        for( int16_t i = 255; i >= 0; i-- ) {
            data[0] = i;
            data[1] = i+1;
            led_blink(100,900);
            hdss_send_bytes(data, 2, false);
        }
        led_blink(100,900);
    }
}
