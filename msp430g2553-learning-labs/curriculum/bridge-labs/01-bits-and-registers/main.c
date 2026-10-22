/* Bridge 01 C: choose this INSTEAD OF main.asm in the CCS project.
 * Same physical action is expressed first as an EDU_ macro, then as
 * the underlying register operation. The student should compare them.
 * Delays are 1 MHz approximations, not measurement-grade timing.
 */
#include <msp430.h>
#define EDU_PIN_TOGGLE(port,mask) ((port##OUT) ^= (mask))
int main(void) {
    WDTCTL=WDTPW|WDTHOLD;
    if(CALBC1_1MHZ == 0xFF) while(1) {};
    DCOCTL=0; BCSCTL1=CALBC1_1MHZ; DCOCTL=CALDCO_1MHZ;
    P1SEL &= ~BIT0; P1SEL2 &= ~BIT0;
    P1OUT &= ~BIT0; P1DIR |= BIT0;
    for(;;) {
        EDU_PIN_TOGGLE(P1,BIT0); /* Expands to P1OUT ^= BIT0. */
        __delay_cycles(500000);
    }
}
