/* ADC: analog becomes a number
 * Learning goal: ADC10, scaling and thresholds
 * Hardware: 10 kOhm potentiometer P1.4, 3.3V
 * Built for MSP430G2553 G2/G2ET LaunchPad, Code Composer Studio.
 * Read the macros in edu430.h beside register-level C.
 */
/*
 * TEACHING NOTES — WHY THE REGISTERS MATTER
 * CONCEPT MAP
 *   A 10-bit ADC represents analog voltage as integer codes 0..1023.
 *   ADC10AE0 connects the chosen analog input to the converter.
 *   ADC10MEM stores a sample; the numbers are NOT volts until scaled.
 *   Test: change the potentiometer, note code and LED thresholds.
 */
#include "edu430.h"
int main(void)
{
    unsigned int sample;
    board_clock_1mhz(); board_leds(); board_adc_a4();
    while (1) {
        sample = adc_a4_read();  /* ADC10MEM: 0..1023 */
        if (sample >= 512u) LED_ON(LED_A);
        else LED_OFF(LED_A);
        if (sample >= 768u) LED_ON(LED_B);
        else LED_OFF(LED_B);
        __delay_cycles(20000);
    }
}
