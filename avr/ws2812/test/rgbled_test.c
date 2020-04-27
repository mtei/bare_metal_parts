/* ws2812 test */
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include "avr_io.h"
#include "ws2812.h"

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
/*#define DELAY _delay_us*/

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

#define LED_NUMS 10
#define G1 10,0,0
#define R1 0,10,0
#define B1 0,0,10
#define W1 20,20,20

#define K1 0,0,0
#define G10  G1,G1,G1,G1,G1, G1,G1,G1,G1,G1
#define R10  R1,R1,R1,R1,R1, R1,R1,R1,R1,R1
#define B10  B1,B1,B1,B1,B1, B1,B1,B1,B1,B1
#define W10  W1,W1,W1,W1,W1, W1,W1,W1,W1,W1
#define K10  K1,K1,K1,K1,K1, K1,K1,K1,K1,K1
#define GRB  G1, R1, B1
#define GRBK G1, R1, B1, K1

uint8_t testled[][3*3] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 0, 0, 0, 5, 5, 5, 10, 10, 10 },
    { 255, 255, 255, 255, 255, 255, 255, 255, 255 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

#if LED_NUMS == 10
uint8_t led[][LED_NUMS*3] = {
    { K10 },
    { W10 },
    { K10 },
    { G10 },
    { K10 },
    { R10 },
    { K10 },
    { B10 },
    { K10 },

    { GRBK, GRBK, R1, B1  },
    { K1, GRBK, GRBK, R1  },
    { B1, K1, GRBK, GRBK  },
    { R1, B1, K1, GRBK, GRB  },
    { GRBK, GRBK, R1, B1  },
    { K1, GRBK, GRBK, R1  },
    { B1, K1, GRBK, GRBK  },
    { R1, B1, K1, GRBK, GRB  },
};
#endif

uint8_t led_run[LED_NUMS*3*2] = {
    [LED_NUMS*3] = 60,    [LED_NUMS*3+1] = 60,    [LED_NUMS*3+2] = 60,
};

#ifndef WS2812_LONG_RESET
  #define WS2812_RESET 50
#else
  #define WS2812_RESET 500
#endif

void ws2812_display_rgb(int data, uint8_t numleds, bool finish,
                        uint8_t bit_on[])
{
    static uint8_t bit_off[3] = {0,0,0};
    for( uint8_t i = 0; i < numleds; i++ ) {
        // output to WS2812_DI_PIN
        ws2812_send_bytes((data & 1)?bit_on:bit_off, 3);
        data >>= 1;
    }
    if(finish) { _delay_us(WS2812_RESET); };
}

void ws2812_display(int data, uint8_t numleds, bool finish)
{
    static uint8_t bit_on[3] = {32,32,32};
    ws2812_display_rgb(data, numleds, finish, bit_on);
}

int main(void)
{
    int tcount = 0;
    led_init();
    ws2812_init();
    ws2812_init_port(&PORTx(TEST_DI_PIN), _BV(P_BITx(TEST_DI_PIN)));
    DELAY(2000);
    for( int i = 0; i < 10; i++ ) {
        led_blink(100,100);
    }
    while(1) {
        uint8_t disp_rgb2[][3] = {
            {255, 0, 255}, {255, 255, 0}
        };
        for(int i = 0; i < LED_NUMS; i++ ) {
            ws2812_display_rgb(1<<i, LED_NUMS, true, disp_rgb2[i&1]);
            DELAY(200);
        }
        for(int i = LED_NUMS-1; i >= 0; i-- ) {
            ws2812_display(1<<i, LED_NUMS, true);
            DELAY(200);
        }

        ws2812_display(++tcount, LED_NUMS, true);
        DELAY(2000);
        for (int i = 0; i < (sizeof(led)/sizeof(led[0])); i++ ) {
            led_blink(100,900);
            ws2812_send_bytes_port(led[i], sizeof(led[0]),
                                   &PORTx(TEST_DI_PIN), _BV(P_BITx(TEST_DI_PIN)));
        }
    }
    return 0;
}
