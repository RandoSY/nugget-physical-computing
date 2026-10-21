/* TMP36 temperature measurement
 * Measurement appendix, requires external sensor and validation.
 * Wiring: Analog TMP36: +Vs=3.3 V, OUT=P1.4, GND=GND; read CSV on UART.
 */
#include "edu430.h"
/* TMP36: output = 500 mV at 0 C, sensitivity = 10 mV/C.
 * With ADC reference = measured VCC (assumed 3300 mV), ADC = 0..1023.
 * Use unsigned long during multiplication: 1023*3300 exceeds 16-bit int.
 * Accuracy depends on the real supply voltage and sensor calibration.
 */
int main(void)
{
    unsigned int raw;
    unsigned long millivolts;
    long celsius_x10;
    board_clock_1mhz(); board_adc_a4(); board_uart_9600();
    serial_text("adc,millivolts,temp_C_x10\r\n");
    while (1) {
        raw = adc_a4_read();
        millivolts = ((unsigned long)raw * 3300UL + 511UL) / 1023UL;
        celsius_x10 = (long)millivolts - 500L;
        serial_uint(raw); serial_char(',');
        serial_uint((unsigned int)millivolts); serial_char(',');
        serial_long(celsius_x10); serial_text("\r\n");
        __delay_cycles(1000000);
    }
}
