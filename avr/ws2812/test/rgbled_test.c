/* ws2812 test */
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
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

#ifndef LED_NUMS
#define LED_NUMS 10
#endif
#define ALT_LED_NUMS 8
#define G1 50,0,0
#define R1 0,50,0
#define B1 0,0,50
#define W1 30,30,30
#define K1 0,0,0
#define GRBK G1, R1, B1, K1

uint8_t led_buffer[LED_NUMS*3+4*3];
uint8_t led_black[] = { K1 };
uint8_t led_white[] = { W1 };
uint8_t led_green[] = { G1 };
uint8_t led_red[]   = { R1 };
uint8_t led_blue[]  = { B1 };
uint8_t led_grbk[]  = { GRBK, GRBK };

typedef struct led_patterns_t {
    uint8_t *data;
    uint8_t size;
} led_patterns_t;

led_patterns_t led_patterns[] = {
    { led_black,    sizeof(led_black)  },
    { led_white,    sizeof(led_white)  },
    { led_black,    sizeof(led_black)  },
    { led_green,    sizeof(led_green)  },
    { led_black,    sizeof(led_black)  },
    { led_red,      sizeof(led_red)    },
    { led_black,    sizeof(led_black)  },
    { led_blue,     sizeof(led_blue)   },
    { led_black,    sizeof(led_black)  },
    { led_grbk,     sizeof(led_grbk)/2 },
    { led_grbk+3,   sizeof(led_grbk)/2 },
    { led_grbk+3*2, sizeof(led_grbk)/2 },
    { led_grbk+3*3, sizeof(led_grbk)/2 },
};

void fill_led_buffer(int index)
{
    led_patterns_t *pattern = led_patterns + index;
    uint8_t *dst, *dstend;
    dstend = led_buffer + sizeof(led_buffer);
    for (dst = led_buffer; dst + pattern->size < dstend; dst += pattern->size){
         memcpy(dst, pattern->data, pattern->size);
    }
}

uint8_t testled[][3*3] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 0, 0, 0, 5, 5, 5, 10, 10, 10 },
    { 255, 255, 255, 255, 255, 255, 255, 255, 255 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

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
        _delay_us(WS2812_RESET);
        for(int i = 0; i < ALT_LED_NUMS; i++ ) {
            ws2812_display_rgb(1<<i, ALT_LED_NUMS, true, disp_rgb2[i&1]);
            DELAY(200);
        }
        for(int i = ALT_LED_NUMS-1; i >= 0; i-- ) {
            ws2812_display(1<<i, ALT_LED_NUMS, true);
            DELAY(200);
        }

        ws2812_display(++tcount, ALT_LED_NUMS, true);
        DELAY(2000);
        for (int i = 0; i < (sizeof(led_patterns)/sizeof(led_patterns[0]));
             i++ ) {
            led_blink(100,900);
            fill_led_buffer(i);
            ws2812_send_bytes_port(led_buffer, LED_NUMS*3,
                                   &PORTx(TEST_DI_PIN), _BV(P_BITx(TEST_DI_PIN)));
        }
        led_blink(100,900);
    }
    return 0;
}
