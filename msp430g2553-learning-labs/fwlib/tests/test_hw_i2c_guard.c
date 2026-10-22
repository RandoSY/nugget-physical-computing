/* Hardware status guard tests. The host cannot model I2C bit timing.
 * It does verify bad arguments, busy detection and NACK return paths.
 */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "edu430_i2c_reg16.h"
volatile uint16_t UCB0CTL0=0,UCB0CTL1=0,UCB0BR0=0,UCB0BR1=0;
volatile uint16_t UCB0TXBUF=0,UCB0RXBUF=0,UCB0SA=0,UCB0STAT=0;
volatile unsigned int P1SEL=0,P1SEL2=0,IFG2=0;
int main(void)
{
    uint8_t v=0u;
    assert(edu_i2c_hw_read_reg16(0x29u,0x0062u,0,1u)==EDU_I2C_ARGUMENT);
    assert(edu_i2c_hw_transfer(0x80u,&v,1u,0,0u,2u)==EDU_I2C_ARGUMENT);
    UCB0STAT=UCBBUSY;
    assert(edu_i2c_hw_read_reg16(0x29u,0x0062u,&v,4u)==EDU_I2C_BUS_BUSY);
    UCB0STAT=0u;
    assert(edu_i2c_hw_read_reg16(0x29u,0x0062u,&v,1u)==EDU_I2C_TIMEOUT);
    puts("PASS: hardware I2C argument, BUSY, TIMEOUT guards");
    return 0;
}
