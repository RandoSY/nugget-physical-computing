/* Four patterns: make decisions
 * Learning goal: State, selection, button release
 * Hardware: Onboard LEDs and button
 * Built for MSP430G2553 G2/G2ET LaunchPad, Code Composer Studio.
 * Read the macros in edu430.h beside register-level C.
 */
/*
 * TEACHING NOTES — WHY THE REGISTERS MATTER
 * CONCEPT MAP
 *   A mode variable describes which pattern to run, not which GPIO
 *   register bit to toggle; the display() function does the translation.
 *   A press EVENT differs from a constantly held button LEVEL.
 *   While the button is held, this example intentionally pauses motion.
 *   Test: Draw a state diagram for mode 0 -> 1 -> 2 -> 3 -> 0.
 */
#include "edu430.h"
#define MODE_A 0
#define MODE_B 1
#define MODE_BOTH 2
#define MODE_ALTERNATE 3
static void pause_100ms(void) { __delay_cycles(100000); }
static unsigned char press_event(void)
{
    if (!BUTTON_DOWN()) return 0;
    __delay_cycles(20000);       /* Debounce press */
    if (!BUTTON_DOWN()) return 0;
    while (BUTTON_DOWN()) { }   /* Wait for release; intentionally blocking */
    __delay_cycles(20000);       /* Debounce release */
    return 1;
}
static void display(unsigned char mode, unsigned char phase)
{
    LED_OFF(LED_BOTH);           /* Reset both latches first */
    switch (mode) {
    case MODE_A: if (phase) LED_ON(LED_A); break;
    case MODE_B: if (phase) LED_ON(LED_B); break;
    case MODE_BOTH: if (phase) LED_ON(LED_BOTH); break;
    default: LED_ON(phase ? LED_A : LED_B); break;
    }
}
int main(void)
{
    unsigned char mode = MODE_A, phase = 0;
    board_clock_1mhz(); board_leds(); board_button();
    while (1) {
        if (press_event()) mode = (unsigned char)((mode + 1) % 4);
        phase ^= 1;             /* Toggle between zero and one */
        display(mode, phase);
        pause_100ms();
    }
}
