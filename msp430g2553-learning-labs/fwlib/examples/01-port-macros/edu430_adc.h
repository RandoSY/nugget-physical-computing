#ifndef EDU430_ADC_H
#define EDU430_ADC_H
#include <msp430.h>
#include <stdint.h>
/* ADC10 examples use AVCC/AVSS reference.  Source must remain between
 * GND and VCC.  P1.0 .. P1.7 are A0 .. A7, but check conflicts with LEDs. */
static inline void edu_adc10_init_a4(void)
{
    ADC10CTL0=ADC10SHT_2 | ADC10ON;
    ADC10CTL1=INCH_4;
    ADC10AE0 |= BIT4;
}
static inline uint8_t edu_adc10_sample(uint16_t *out, uint32_t timeout)
{
    if (!out) return 0u;
    ADC10CTL0 |= ENC | ADC10SC;
    while ((ADC10CTL1 & ADC10BUSY) && timeout) --timeout;
    if (ADC10CTL1 & ADC10BUSY) return 0u;
    *out=ADC10MEM; return 1u;
}
/* Macros expose the hardware, while a FUNCTION handles waiting/errors. */
#define EDU_ADC10_RAW() (ADC10MEM)
#define EDU_ADC10_OFF() (ADC10CTL0 &= ~(ENC | ADC10ON))
#endif
