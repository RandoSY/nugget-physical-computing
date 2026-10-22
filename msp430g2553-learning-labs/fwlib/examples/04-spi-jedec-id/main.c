/* SPI JEDEC-ID read for compatible 3.3V memory devices (0x9F).
 * P1.5 CLK, P1.6 MISO, P1.7 MOSI, P2.0 manual chip-select.
 * SPI SPI flash chip pin mappings vary; check the specific datasheet.
 * Disable/use no LED_B blinking: its P1.6 pad belongs to SPI now.
 */
#include "edu430_system.h"
#include "edu430_spi.h"
#include "edu430_gpio.h"
#include "edu430_uart.h"
static void hex(uint8_t x)
{
    const char digits[]="0123456789ABCDEF";
    (void)edu_uart_put((uint8_t)digits[x>>4],100000UL);
    (void)edu_uart_put((uint8_t)digits[x&15u],100000UL);
}
int main(void)
{
    uint8_t a=0,b=0,c=0;
    EDU_WATCHDOG_STOP();if(!edu_clock_1mhz())while(1){}
    edu_uart_9600_init();
    EDU_GPIO_OUTPUT_HIGH(P2,BIT0);  /* CS inactive */
    edu_spi_begin(EDU_SPI_MODE0,8u);
    while(1) {
        uint8_t ok;
        EDU_PIN_OFF(P2,BIT0); /* select device */
        ok=edu_spi_transfer(0x9Fu,0,100000UL);
        ok=(uint8_t)(ok && edu_spi_transfer(0xFFu,&a,100000UL));
        ok=(uint8_t)(ok && edu_spi_transfer(0xFFu,&b,100000UL));
        ok=(uint8_t)(ok && edu_spi_transfer(0xFFu,&c,100000UL));
        EDU_PIN_ON(P2,BIT0);  /* release chip select */
        if(ok) {
            (void)edu_uart_text("JEDEC,",100000UL);
            hex(a);hex(b);hex(c);
            (void)edu_uart_text("\r\n",100000UL);
        } else (void)edu_uart_text("ERR SPI TIMEOUT\r\n",100000UL);
        __delay_cycles(500000);
    }
}
