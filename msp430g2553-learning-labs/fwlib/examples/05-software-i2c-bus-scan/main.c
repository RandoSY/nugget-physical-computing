/* P2.0 SCL, P2.1 SDA. Both need external 4.7k pullups to 3.3V.
 * Scan addresses 0x08 .. 0x77.  Each ACK is a measurement.
 * Uses the software I2C engine, leaving USCI_B0 free for SPI.
 */
#include "edu430_system.h"
#include "edu430_i2c_soft.h"
#include "edu430_uart.h"
static void hex(uint8_t value)
{
    static const char digit[]="0123456789ABCDEF";
    (void)edu_uart_put((uint8_t)digit[value>>4],100000UL);
    (void)edu_uart_put((uint8_t)digit[value&15u],100000UL);
}
int main(void)
{
    uint8_t address;
    EDU_WATCHDOG_STOP(); if(!edu_clock_1mhz()) while(1){}
    edu_uart_9600_init();
    if(!edu_si2c_init()) (void)edu_uart_text("ERR I2C PULLUPS\r\n",100000UL);
    while(1) {
        for(address=8u;address<=0x77u;address++) {
            uint8_t ack=0u;
            if(edu_si2c_start()) {
                ack=edu_si2c_write((uint8_t)(address<<1));
                edu_si2c_stop();
            }
            if(ack) {
                (void)edu_uart_text("I2C ACK 0x",100000UL);
                hex(address); (void)edu_uart_text("\r\n",100000UL);
            }
        }
        (void)edu_uart_text("SCAN DONE\r\n",100000UL);
        __delay_cycles(500000);
    }
}
