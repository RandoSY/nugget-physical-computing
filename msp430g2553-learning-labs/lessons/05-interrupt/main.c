/* Button interrupts: asynchronous events
 * Learning goal: Port interrupt, event counters
 * Hardware: Onboard button and LEDs
 * Built for MSP430G2553 G2/G2ET LaunchPad, Code Composer Studio.
 * Read the macros in edu430.h beside register-level C.
 */
/*
 * TEACHING NOTES — WHY THE REGISTERS MATTER
 * CONCEPT MAP
 *   P1IE permits a pin to request an interrupt; P1IES chooses its edge.
 *   P1IFG records the request. We acknowledge it inside the ISR.
 *   The ISR only updates a count and masks further button interrupts;
 *   main() debounces the release and re-arms the circuit.
 *   Test: What if the pending flag is not cleared before arming?
 */
#include "edu430.h"
static volatile unsigned int presses = 0;
#pragma vector=PORT1_VECTOR
__interrupt void button_isr(void)
{
    if (P1IFG & BUTTON) {
        presses++;             /* Notify the main program */
        CLEAR_BITS(P1IFG, BUTTON); /* Acknowledge interrupt */
        /* In this simple lesson, software re-arms after button release. */
        CLEAR_BITS(P1IE, BUTTON);
    }
}
static void arm_button(void)
{
    SET_BITS(P1IES, BUTTON);   /* Falling edge: high to low */
    CLEAR_BITS(P1IFG, BUTTON); /* Clear stale request BEFORE enabling */
    SET_BITS(P1IE, BUTTON);
    __enable_interrupt();
}
int main(void)
{
    unsigned int last_seen = 0;
    board_clock_1mhz(); board_leds(); board_button(); arm_button();
    while (1) {
        unsigned int now;
        __disable_interrupt(); now = presses; __enable_interrupt();
        if (now != last_seen) {
            last_seen = now;
            LED_TOGGLE(LED_A);
            if ((now % 4u) == 0u) LED_TOGGLE(LED_B);
        }
        /* Re-arm only once the button is released and contacts settle. */
        if (!(P1IE & BUTTON) && !BUTTON_DOWN()) {
            __delay_cycles(20000);
            if (!BUTTON_DOWN()) {
                CLEAR_BITS(P1IFG, BUTTON);
                SET_BITS(P1IE, BUTTON);
            }
        }
    }
}
