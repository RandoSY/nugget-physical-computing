#ifndef EDU430_H
#define EDU430_H
#include <msp430.h>
/* BEGINNER VIEW: command                  REGISTER-LEVEL VIEW:
 * SET_BITS(P1OUT, BIT0);                     P1OUT |= BIT0;
 * CLEAR_BITS(P1OUT, BIT0);                   P1OUT &= ~BIT0;
 * TOGGLE_BITS(P1OUT, BIT0);                  P1OUT ^= BIT0;
 * Each macro keeps the original register operation visible.
 * Parentheses protect the arguments against operator precedence surprises.
 */
#define SET_BITS(reg, mask)     ((reg) |= (mask))
#define CLEAR_BITS(reg, mask)   ((reg) &= ~(mask))
#define TOGGLE_BITS(reg, mask)  ((reg) ^= (mask))
#define BITS_HIGH(reg, mask)    (((reg) & (mask)) == (mask))
#define BITS_LOW(reg, mask)     (((reg) & (mask)) == 0)

/* Both LaunchPad variants use these same MCU pins. LED colors vary. */
#define LED_A           BIT0       /* Port 1, bit 0 */
#define LED_B           BIT6       /* Port 1, bit 6 */
#define LED_BOTH        (LED_A | LED_B)
#define BUTTON          BIT3       /* Active-low switch on P1.3 */
#define LED_ON(mask)    SET_BITS(P1OUT, mask)
#define LED_OFF(mask)   CLEAR_BITS(P1OUT, mask)
#define LED_TOGGLE(mask) TOGGLE_BITS(P1OUT, mask)
#define BUTTON_DOWN()   BITS_LOW(P1IN, BUTTON)

static void board_clock_1mhz(void)
{
    WDTCTL = WDTPW | WDTHOLD;     /* Basic lessons deliberately stop WDT */
    if (CALBC1_1MHZ == 0xFF) {   /* Erased calibration: do not use bad clock */
        while (1) { }           /* Investigate device calibration first */
    }
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
}
static void board_leds(void)
{
    LED_OFF(LED_BOTH);          /* Set output latch low before enabling */
    SET_BITS(P1DIR, LED_BOTH); /* P1DIR |= BIT0 | BIT6; */
}
static void board_button(void)
{
    CLEAR_BITS(P1DIR, BUTTON); /* Input */
    SET_BITS(P1REN, BUTTON);   /* Enable resistor */
    SET_BITS(P1OUT, BUTTON);   /* Pull UP => pressed reads LOW */
}
/* On G2553, P1.1=UCA0RXD and P1.2=UCA0TXD.
 * Physical LaunchPad TX/RX jumpers must connect these to USB backchannel.
 * At 1 MHz: 1000000 / 9600 = 104.1667; modulation UCBRS0.
 */
static void board_uart_9600(void)
{
    SET_BITS(P1SEL, BIT1 | BIT2);
    SET_BITS(P1SEL2, BIT1 | BIT2);
    UCA0CTL1 = UCSWRST | UCSSEL_2;
    UCA0BR0 = 104;
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;
    CLEAR_BITS(UCA0CTL1, UCSWRST);
}
static void serial_char(char ch)
{
    while (!(IFG2 & UCA0TXIFG)) { } /* Wait for TX buffer empty */
    UCA0TXBUF = (unsigned char)ch;
}
static void serial_text(const char *s)
{
    while (*s) serial_char(*s++);
}
static void serial_uint(unsigned int value)
{
    char digits[6]; unsigned char count = 0;
    do { digits[count++] = (char)('0' + value % 10); value /= 10; }
    while (value && count < sizeof digits);
    while (count) serial_char(digits[--count]);
}
static void serial_long(long value)
{
    char digits[12]; unsigned char count = 0;
    unsigned long magnitude;
    if (value < 0) {
        serial_char('-');
        /* Safe even for LONG_MIN; no signed negation overflow. */
        magnitude = (unsigned long)(-(value + 1)) + 1UL;
    } else magnitude = (unsigned long)value;
    do {digits[count++] = (char)('0' + magnitude % 10UL); magnitude /= 10UL;}
    while (magnitude && count < sizeof digits);
    while (count) serial_char(digits[--count]);
}
static void board_adc_a4(void)
{
    ADC10CTL0 = ADC10SHT_2 | ADC10ON;
    ADC10CTL1 = INCH_4;       /* Input channel A4 = P1.4 */
    SET_BITS(ADC10AE0, BIT4); /* Analog input enabled */
}
static unsigned int adc_a4_read(void)
{
    SET_BITS(ADC10CTL0, ENC | ADC10SC);
    while (ADC10CTL1 & ADC10BUSY) { }
    return ADC10MEM;          /* 10-bit number: 0..1023 */
}
#endif
