/* Hardware USCI_B0 I2C teaching exercise.
 * P1.6 SCL, P1.7 SDA: external 3.3V pullups required.
 * Writes 0x00 to a device at 7-bit address 0x48 (device-specific).
 * This is NOT a universal probe; only use on a known compatible device.
 * Do not touch P1.6 LED while hardware I2C is active.
 */
#include "edu430_system.h"
#include "edu430_i2c_hw.h"
#include "edu430_uart.h"
int main(void)
{
    static const uint8_t command[] = {0x00u};
    EDU_WATCHDOG_STOP(); if(!edu_clock_1mhz())while(1){}
    edu_uart_9600_init();edu_i2c_begin();
    while(1) {
        if(edu_i2c_write(0x48u,command,1u,100000UL))
            (void)edu_uart_text("I2C WRITE OK\r\n",100000UL);
        else (void)edu_uart_text("I2C WRITE FAIL\r\n",100000UL);
        __delay_cycles(500000);
    }
}
