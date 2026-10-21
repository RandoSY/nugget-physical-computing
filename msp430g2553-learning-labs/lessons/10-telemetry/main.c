/* Capstone: make an instrument
 * Learning goal: ADC + switch + event records + UART
 * Hardware: Potentiometer P1.4, onboard button, USB UART
 * Built for MSP430G2553 G2/G2ET LaunchPad, Code Composer Studio.
 * Read the macros in edu430.h beside register-level C.
 */
/*
 * TEACHING NOTES — WHY THE REGISTERS MATTER
 * CONCEPT MAP
 *   A measurement record needs a defined sample, unit, and condition.
 *   CSV values here mean trial number, ADC code and threshold status.
 *   The UART is a transport, not a calibrated sensor or clock.
 *   Test: plot 10 successive ADC codes at five potentiometer settings.
 */
#include "edu430.h"
/* Each report records trial number, ADC reading, and LED state.
 * Serial output is intentionally plain ASCII CSV for easy graphing.
 */
static unsigned int trial = 0;
static unsigned char latched = 0;
int main(void)
{
    unsigned int adc;
    board_clock_1mhz(); board_leds(); board_button();
    board_adc_a4(); board_uart_9600();
    serial_text("trial,adc,above_midpoint\r\n");
    while (1) {
        if (BUTTON_DOWN() && !latched) {
            __delay_cycles(20000);
            if (BUTTON_DOWN()) { trial++; latched = 1; }
        }
        if (!BUTTON_DOWN()) latched = 0;
        adc = adc_a4_read();
        if (adc >= 512u) LED_ON(LED_A);
        else LED_OFF(LED_A);
        serial_uint(trial); serial_char(',');
        serial_uint(adc); serial_char(',');
        serial_uint(adc >= 512u ? 1 : 0);
        serial_text("\r\n");
        __delay_cycles(1000000);
    }
}
