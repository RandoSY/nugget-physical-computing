/* UART explorer: open Windows' USB backchannel at 9600 8N1.
 * P1.1=RX and P1.2=TX; check old board RXD/TXD jumper orientation.
 */
#include "edu430_system.h"
#include "edu430_uart.h"
int main(void)
{
    uint8_t received;
    EDU_WATCHDOG_STOP(); if(!edu_clock_1mhz()) while(1){}
    edu_uart_9600_init();
    (void)edu_uart_text("MSP430 library UART ready\r\n",100000UL);
    while(1) {
        if(edu_uart_get(&received)) (void)edu_uart_put(received,100000UL);
    }
}
