/* HX711 load-cell scale
 * Measurement appendix, requires external sensor and validation.
 * Wiring: HX711 at 3.3 V: DOUT=P2.0, PD_SCK=P2.1, common GND; two-point calibration.
 */
#include "edu430.h"
#define HX_DATA  BIT0 /* P2.0, from HX711 DOUT */
#define HX_CLOCK BIT1 /* P2.1, to HX711 PD_SCK */
/* Enter values from calibration: raw value with empty platform,
 * and counts per gram measured with a known calibration mass.
 * Illustrative defaults must be replaced for actual measurements!
 */
#define EMPTY_RAW_COUNTS 0L
#define COUNTS_PER_GRAM  100L
static void hx_init(void)
{
    CLEAR_BITS(P2DIR, HX_DATA);
    CLEAR_BITS(P2OUT, HX_CLOCK);
    SET_BITS(P2DIR, HX_CLOCK);
}
/* Return 1 if conversion ready, 0 after timeout.
 * A low DOUT indicates a completed 24-bit sample.
 */
static unsigned char hx_read(long *result)
{
    unsigned long bits = 0;
    unsigned long tries;
    unsigned char i;
    for (tries = 0; tries < 100000UL; ++tries) {
        if (!(P2IN & HX_DATA)) break;
        __delay_cycles(10);
    }
    if (tries == 100000UL) return 0;
    /* Do not let an interrupt stretch PD_SCK high >60 us. */
    __disable_interrupt();
    for (i = 0; i < 24; ++i) {
        SET_BITS(P2OUT, HX_CLOCK);
        __delay_cycles(2);
        bits = (bits << 1) | ((P2IN & HX_DATA) ? 1UL : 0UL);
        CLEAR_BITS(P2OUT, HX_CLOCK);
        __delay_cycles(2);
    }
    /* 25th clock chooses channel A, gain 128 next conversion. */
    SET_BITS(P2OUT, HX_CLOCK); __delay_cycles(2);
    CLEAR_BITS(P2OUT, HX_CLOCK);
    __enable_interrupt();  /* This stand-alone example otherwise disables IRQs */
    /* Sign extend the HX711 24-bit twos-complement reading. */
    if (bits & 0x800000UL) bits |= 0xFF000000UL;
    *result = (long)bits;
    return 1;
}
int main(void)
{
    long raw, grams;
    board_clock_1mhz(); board_uart_9600(); hx_init();
    serial_text("raw,grams_approx\r\n");
    while (1) {
        if (hx_read(&raw)) {
            grams = (raw - EMPTY_RAW_COUNTS) / COUNTS_PER_GRAM;
            serial_long(raw); serial_char(','); serial_long(grams);
            serial_text("\r\n");
        } else serial_text("HX711_TIMEOUT\r\n");
        __delay_cycles(200000);
    }
}
