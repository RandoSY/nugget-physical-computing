/* Blink: a visible output
 * Learning goal: GPIO direction and bit masks
 * Hardware: P1.0 and P1.6 LEDs
 * Built for MSP430G2553 G2/G2ET LaunchPad, Code Composer Studio.
 * Read the macros in edu430.h beside register-level C.
 */
/*
 * TEACHING NOTES — WHY THE REGISTERS MATTER
 * CONCEPT MAP
 *   P1DIR bit=1 means a pin is an OUTPUT; P1OUT bit=1 drives it HIGH.
 *   The |= operator sets a selected bit while preserving all other bits.
 *   The &= ~ operator clears a selected bit while preserving the rest.
 *   Predict: at 1 MHz each 250,000-cycle pause is roughly 0.25 s.
 *   Test: What changes if the bit mask is BIT6 instead of BIT0?
 */
#include "edu430.h"
int main(void)
{
    board_clock_1mhz(); /* Watchdog off, CPU clock calibrated to 1 MHz */
    board_leds();       /* P1DIR |= BIT0 | BIT6; */
    while (1) {
        LED_ON(LED_A);       /* Equivalent to P1OUT |= BIT0; */
        __delay_cycles(250000); /* 250 ms at 1 MHz */
        LED_OFF(LED_A);      /* Equivalent to P1OUT &= ~BIT0; */
        __delay_cycles(250000);
    }
}
