/* Four-mode pattern progression: STAGE 3 — named actions
 * This code is a teaching transformation inspired by Joel Brigida's
 * OnBoard-LED-Pattern.c, NOT a verbatim copy. See ../README.md.
 * P1.0 = LED A; P1.6 = LED B; P1.3 = active-low button.
 */
#include "edu430.h"

/* Show one half-cycle of the selected pattern. */
static void show(unsigned char mode, unsigned char phase)
{
    LED_OFF(LED_BOTH);
    switch (mode) {
    case 0: if (phase) LED_ON(LED_A); break;
    case 1: if (phase) LED_ON(LED_B); break;
    case 2: if (phase) LED_ON(LED_BOTH); break;
    default: LED_ON(phase ? LED_A : LED_B); break;
    }
}
int main(void)
{
    unsigned char mode = 0, phase = 0;
    board_clock_1mhz(); board_leds(); board_button();
    for (;;) {
        /* Observe: one pattern is shown for each 100-ms half-cycle.
         * Press between cycles to advance the mode. */
        if (BUTTON_DOWN()) {
            __delay_cycles(20000);
            if (BUTTON_DOWN()) {
                while (BUTTON_DOWN()) { }
                __delay_cycles(20000);
                mode = (unsigned char)((mode + 1u) % 4u);
            }
        }
        phase ^= 1u;
        show(mode, phase);
        __delay_cycles(100000);
    }
}
