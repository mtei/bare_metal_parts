/* TX, RX loop back  test */
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr_io.h>
#include <avr/interrupt.h>

#define SOFT_TX_PORT D,3  // TXO
#define HDSS_FUNC_PREFIX debug_print
#ifdef PARITY_ENABLE
#error party
#endif
#include <semi_soft_serial.c>

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega32U2__)
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
#endif

#ifndef SOFT_TX_PORT
  /* One-wire half-duplex operation: Use the hardware RXD pin for soft TX */
  #define SOFT_TX_PORT USART_RX_PORT
#else
  /* transmission only */
  #define HDSS_TRANSMISSION_ONLY
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

#ifndef AVR_UBRR_VALUE
  #define AVR_UBRR_VALUE  0
#endif

void disable_usb(void)
{
#ifdef USBCON
    USBCON &= ~(_BV(USBE)|_BV(OTGPADE)|_BV(VBUSTE));
#endif
}

void disable_jtag(void) {
// To use PF4-7 (PC2-5 on ATmega32A), 
//    disable JTAG by writing JTD bit twice within four cycles.
#if (defined(__AVR_AT90USB646__)     \
     || defined(__AVR_AT90USB647__)  \
     || defined(__AVR_AT90USB1286__) \
     || defined(__AVR_AT90USB1287__) \
     || defined(__AVR_ATmega16U4__)  \
     || defined(__AVR_ATmega32U4__))
    MCUCR |= _BV(JTD);
    MCUCR |= _BV(JTD);
#elif defined(__AVR_ATmega32A__)
    MCUCSR |= _BV(JTD);
    MCUCSR |= _BV(JTD);
#endif
}

static void hdss_init_common2(void)
{
    UBRRH = (AVR_UBRR_VALUE >> 8) ;
    UBRRL = (AVR_UBRR_VALUE & 0xff);
    UCSRA = 0;
    UCSRB = 0;
    UCSRC = (UMSEL_ASYNC | UPM_NONE | USBS_1 | UCSZ_8);
}

/*set the watchdog-
1. save SREG
2. turn off irq's
3. reset watchdog timer
4. enable watchdog change
5. write watchdog value
6. restore SREG (restoring irq status)
*/
#define WD_OFF 0
#define WD_SET(val, ...) \
    __asm__ __volatile__( \
    "in __tmp_reg__,__SREG__"    "\n\t" \
    "cli"                        "\n\t" \
    "wdr"                        "\n\t" \
    "sts %[wdreg],%[wden]"       "\n\t" \
    "sts %[wdreg],%[wdval]"      "\n\t" \
    "out __SREG__,__tmp_reg__"   "\n\t" \
    :                                   \
    : [ wdreg ] "M"(&WDTCSR)            \
      , [ wden ] "r"((uint8_t)(0x18))   \
      , [ wdval ] "r"((uint8_t)(val | (__VA_ARGS__ + 0))) \
    : "r0")

static void hdss_change_receiver(void)
{
    //DDRx(USART_RX_PORT)  &= ~_BV(P_BITx(USART_RX_PORT)); // set input mode
    //PORTx(USART_RX_PORT) |= _BV(P_BITx(USART_RX_PORT));  // set pullup
    UCSRB |= (RXENV);
}

static void hdss_change_sender2(void)
{
    DDRx(USART_RX_PORT)  &= ~_BV(P_BITx(USART_RX_PORT)); // set input mode
    PORTx(USART_RX_PORT) |= _BV(P_BITx(USART_RX_PORT));  // set pullup
    UCSRB |= (TXENV|RXENV|RXCIEV);
}

void hdss_initiator_init(void)
{
    uint8_t sreg_prev;
    sreg_prev = SREG;
    cli();
    hdss_init_common2();
    hdss_change_sender2();
    SREG = sreg_prev;
}

void hdss_responder_init(void)
{
    uint8_t sreg_prev;
    sreg_prev = SREG;
    cli();
    hdss_init_common2();
    hdss_change_receiver();
    SREG = sreg_prev;
}

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
    if ( UCSRA & RXCV ) {
        return UDR;
    } else {
        return -1;
    }
}

bool send_byte(uint8_t data)
{
    if (UCSRA & (UDREV)) {
        UCSRA &= ~TXCV;
        UDR = data;
        return true;
    } else {
        return false;
    }
}

uint8_t rbuf;
uint8_t rbuf2;
uint8_t tintr;
uint8_t data[10];

int main(void)
{
    DELAY(2000);
    debug_print_initiator_init();
    DELAY(1);
    data[0] = 0xf0; debug_print_send_bytes(data, 1, false);
    hdss_initiator_init();
    led_init();
    data[0] = 0xf1; debug_print_send_bytes(data, 1, false);
    for( int i = 0; i < 10; i++ ) {
        led_blink(100,100);
    }
    data[0] = 0xf2; debug_print_send_bytes(data, 1, false);
    data[0] = 0xaa;
    data[1] = MCUCR;
    data[2] = MCUSR;
    data[3] = WDTCSR;
    debug_print_send_bytes(data, 4, false);
    disable_usb();
    MCUSR &= ~_BV(WDRF);
    //WD_SET(WD_OFF);
    //disable_jtag();
    data[0] = 0xa0;
    data[1] = MCUCR;
    data[2] = MCUSR;
    data[3] = WDTCSR;
    data[4] = USBCON;
    data[5] = UDIEN;
    debug_print_send_bytes(data, 6, false);
    data[0] = 0xf3; debug_print_send_bytes(data, 1, false);
    sei();
    data[0] = 0xf4; debug_print_send_bytes(data, 1, false);
    led_blink(100,900);
    while(1) {
        rbuf = 0;
        rbuf2 = 0;
        tintr = 0;
        data[0] = 0x55;
        data[1] = MCUCR;
        data[2] = UCSRA;
        data[3] = UCSRB;
        data[4] = UCSRC;
        send_byte(0x5a);
        data[5] = UCSRA;
        data[6] = send_byte(0xa5);
        data[7] = UCSRA;
        debug_print_send_bytes(data, 6, false);
        data[0] = 0x51;
        data[1] = MCUCR;
        data[2] = UCSRA;
        data[3] = rbuf;
        data[4] = rbuf2;
        data[5] = UCSRA;
        data[6] = tintr;
        debug_print_send_bytes(data, 7, false);
        led_blink(100,900);
    }
}

#if 1
__attribute__((naked))
__attribute__((section(".init3")))
void ws2812_init(void)
{
    if (DDRx(SOFT_TX_PORT) & _BV(P_BITx(SOFT_TX_PORT))) {
        data[0] = 0xff;
        data[1] = MCUCR;
        data[2] = MCUSR;
        data[3] = WDTCSR;
        debug_print_send_bytes(data, 4, false);
        while(1) {}
    }
}
#endif

void disp_vect(uint8_t v)
{
    if (DDRx(SOFT_TX_PORT) & _BV(P_BITx(SOFT_TX_PORT))) {
        data[0] = 0xe0;
        data[1] = v;
        data[2] = MCUSR;
        data[3] = MCUCR;
        data[4] = WDTCSR;
        data[5] = USBCON;
        data[6] = UDIEN;
        debug_print_send_bytes(data, 7, false);
        while(1) {}
    }
}

ISR(USB_GEN_vect) {
    disp_vect(USB_GEN_vect_num);
}
ISR(USB_COM_vect) {
    disp_vect(USB_COM_vect_num);
}

#if 1
ISR(WDT_vect) {
    if (DDRx(SOFT_TX_PORT) & _BV(P_BITx(SOFT_TX_PORT))) {
        data[0] = WDT_vect_num;
        data[1] = MCUSR;
        data[2] = MCUCR;
        data[3] = WDTCSR;
        debug_print_send_bytes(data, 4, false);
        //_delay_ms(1000);
        tintr |= 0x10;
        while(1){}
    }
}
#endif

ISR(RX_vect) {
    if( tintr & 4 )
        rbuf2 = receive_byte();
    else
        rbuf = receive_byte();
    tintr |= 4;
}

ISR(TX_vect) {
    tintr |= 1;
}

ISR(UDRE_vect) {
    tintr |= 2;
}
