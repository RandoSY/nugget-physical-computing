/* Four-mode pattern progression: STAGE 1 — plain registers
 * This code is a teaching transformation inspired by Joel Brigida's
 * OnBoard-LED-Pattern.c, NOT a verbatim copy. See ../README.md.
 * P1.0 = LED A; P1.6 = LED B; P1.3 = active-low button.
 */
#include <msp430.h>
#define LED_A BIT0
#define LED_B BIT6
#define BUTTON BIT3

/* Show one half-cycle of the selected pattern. */
static void show(unsigned char mode, unsigned char phase)
{
    P1OUT &= ~(LED_A | LED_B);
    switch (mode) {
    case 0: if (phase) P1OUT |= LED_A; break;
    case 1: if (phase) P1OUT |= LED_B; break;
    case 2: if (phase) P1OUT |= (LED_A | LED_B); break;
    default: P1OUT |= (phase ? LED_A : LED_B); break;
    }
}
int main(void)
{
    unsigned char mode = 0, phase = 0;
    WDTCTL = WDTPW | WDTHOLD;
    if (CALBC1_1MHZ == 0xFF) while (1) {}
    DCOCTL=0; BCSCTL1=CALBC1_1MHZ; DCOCTL=CALDCO_1MHZ;
    P1OUT &= ~(LED_A | LED_B);
    P1DIR |= LED_A | LED_B;
    P1DIR &= ~BUTTON; P1REN |= BUTTON; P1OUT |= BUTTON;
    for (;;) {
        /* Observe: one pattern is shown for each 100-ms half-cycle.
         * Press between cycles to advance the mode. */
        if (((P1IN & BUTTON) == 0)) {
            __delay_cycles(20000);
            if (((P1IN & BUTTON) == 0)) {
                while (((P1IN & BUTTON) == 0)) { }
                __delay_cycles(20000);
                mode = (unsigned char)((mode + 1u) % 4u);
            }
        }
        phase ^= 1u;
        show(mode, phase);
        __delay_cycles(100000);
    }
}
