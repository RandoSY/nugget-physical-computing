#ifndef EDU430_PWM_H
#define EDU430_PWM_H
#include <msp430.h>
#include <stdint.h>
/* Timer_A0.1 PWM on P1.6, up mode. Duty is 1..999 PER MILLE,
 * i.e. 500=50%.  Values 0 and 1000 require special force-high/low
 * output modes; intentionally exclude them from this tiny teaching helper.
 * Timer_A0 now belongs to PWM: don't also use it for 1ms scheduler ticks.
 * P1.6 shares LED_B and USCI_B0 SPI/I2C. Choose ONE function per pin.
 */
static inline void edu_pwm_ta0_p16_begin(uint16_t duty_per_mille)
{
    if(duty_per_mille<1u) duty_per_mille=1u;
    if(duty_per_mille>999u) duty_per_mille=999u;
    P1DIR |= BIT6;
    P1SEL |= BIT6; P1SEL2 &= (uint8_t)~BIT6;
    TA0CCR0=999u;
    TA0CCR1=duty_per_mille;
    TA0CCTL1=OUTMOD_7; /* reset/set on compare/rollover */
    TA0CTL=TASSEL_2 | MC_1 | TACLR; /* 1 MHz SMCLK = 1 kHz PWM */
}
static inline void edu_pwm_ta0_p16_duty(uint16_t duty_per_mille)
{
    if(duty_per_mille<1u) duty_per_mille=1u;
    if(duty_per_mille>999u) duty_per_mille=999u;
    TA0CCR1=duty_per_mille;
}
#endif
