/* ADC10 A4 P1.4 = 3.3V potentiometer wiper; ends to 3.3V/GND.
 * Use integer-only serial output to avoid 16-bit printf overhead. */
#include "edu430_system.h"
#include "edu430_adc.h"
#include "edu430_uart.h"
static void serial_decimal(uint16_t value)
{
    char buf[5]; uint8_t n=0;
    do {buf[n++]=(char)('0'+value%10u);value/=10u;}while(value);
    while(n) (void)edu_uart_put((uint8_t)buf[--n],100000UL);
}
int main(void)
{
    uint16_t counts;
    EDU_WATCHDOG_STOP(); if(!edu_clock_1mhz())while(1){}
    edu_uart_9600_init(); edu_adc10_init_a4();
    while(1) {
        if(edu_adc10_sample(&counts,100000UL)) {
            (void)edu_uart_text("adc10,",100000UL);
            serial_decimal(counts);
            (void)edu_uart_text("\r\n",100000UL);
        }
        __delay_cycles(200000);
    }
}
