/* PWM: variable brightness
 * Learning goal: Hardware PWM and duty cycle
 * Hardware: LED B on P1.6
 * Built for MSP430G2553 G2/G2ET LaunchPad, Code Composer Studio.
 * Read the macros in edu430.h beside register-level C.
 */
/*
 * TEACHING NOTES — WHY THE REGISTERS MATTER
 * CONCEPT MAP
 *   PWM rapidly alternates a digital output between LOW and HIGH.
 *   The ratio of HIGH time to total cycle is the duty cycle.
 *   P1SEL gives a hardware timer control of P1.6 rather than GPIO code.
 *   CCR0 sets the period; CCR1 and OUTMOD_7 set switching behavior.
 *   Test: use a scope to compare 25%, 50%, and 75% duty cycles.
 */
#include "edu430.h"
static void pwm_init(void)
{
    /* P1.6 = TA0.1 timer output on MSP430G2553. */
    SET_BITS(P1DIR, LED_B);
    SET_BITS(P1SEL, LED_B);
    CLEAR_BITS(P1SEL2, LED_B);
    TA0CCR0 = 999;             /* 1 kHz PWM, 1 MHz clock */
    TA0CCTL1 = OUTMOD_7;       /* Reset at CCR1; set at CCR0 */
    TA0CCR1 = 0;              /* Duty value begins at zero */
    TA0CTL = TASSEL_2 | MC_1 | TACLR;
}
int main(void)
{
    unsigned int duty;
    board_clock_1mhz(); pwm_init();
    while (1) {
        for (duty = 0; duty <= 950; duty += 25) {
            TA0CCR1 = duty;   /* PWM intensity proportional to duty */
            __delay_cycles(20000);
        }
        for (duty = 950; duty >= 25; duty -= 25) {
            TA0CCR1 = duty;
            __delay_cycles(20000);
        }
    }
}
