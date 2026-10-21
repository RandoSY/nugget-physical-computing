/* Button: read an active-low input
 * Learning goal: Input pull-up, polling, debounce
 * Hardware: Onboard P1.3 pushbutton
 * Built for MSP430G2553 G2/G2ET LaunchPad, Code Composer Studio.
 * Read the macros in edu430.h beside register-level C.
 */
/*
 * TEACHING NOTES — WHY THE REGISTERS MATTER
 * CONCEPT MAP
 *   P1REN enables a resistor; P1OUT selects pull-up or pull-down.
 *   A pressed, pull-up button connects P1.3 to ground: input is ZERO.
 *   The 20 ms debounce sample is simple teaching code, not an
 *   interrupt-driven, fully nonblocking event detector.
 *   Test: Why does removing the resistor make the idle input unreliable?
 */
#include "edu430.h"
static unsigned char button_stable(void)
{
    /* Simple debounce: sample, wait 20 ms, and sample again. */
    if (!BUTTON_DOWN()) return 0;
    __delay_cycles(20000);
    return BUTTON_DOWN();
}
int main(void)
{
    board_clock_1mhz(); board_leds(); board_button();
    while (1) {
        if (button_stable()) LED_ON(LED_A);
        else LED_OFF(LED_A);
    }
}
