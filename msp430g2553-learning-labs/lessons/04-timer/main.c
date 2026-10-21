/* Timer_A: replace a busy delay
 * Learning goal: Compare interrupt and time base
 * Hardware: Onboard LED
 * Built for MSP430G2553 G2/G2ET LaunchPad, Code Composer Studio.
 * Read the macros in edu430.h beside register-level C.
 */
/*
 * TEACHING NOTES — WHY THE REGISTERS MATTER
 * CONCEPT MAP
 *   Timer_A is a hardware counter independent of main-loop speed.
 *   CCR0=999 at 1 MHz gives 1000 clocks = 1 ms per interrupt.
 *   An ISR is a small function called automatically by hardware.
 *   volatile prevents the compiler treating ISR-shared state as constant.
 *   Test: contrast timer ticks with the software __delay_cycles().
 */
#include "edu430.h"
static volatile unsigned int milliseconds = 0;
#pragma vector=TIMER0_A0_VECTOR
__interrupt void tick_isr(void)
{
    milliseconds++;              /* 1 tick for every 1000 CPU cycles */
}
static void timer_start(void)
{
    TA0CCR0 = 999;              /* Up mode counts 0..999 => 1000 us */
    TA0CCTL0 = CCIE;           /* Interrupt when counter hits CCR0 */
    TA0CTL = TASSEL_2 | MC_1 | TACLR; /* SMCLK, up, reset */
    __enable_interrupt();
}
int main(void)
{
    unsigned int previous = 0;
    board_clock_1mhz(); board_leds(); timer_start();
    while (1) {
        unsigned int now;
        __disable_interrupt(); now = milliseconds; __enable_interrupt();
        if ((unsigned int)(now - previous) >= 500u) {
            previous = now;
            LED_TOGGLE(LED_A);   /* P1OUT ^= BIT0; */
        }
    }
}
