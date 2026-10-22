/* Put a breakpoint at each labeled step to see literal bit changes.
 * In CCS, Watch P1DIR and P1OUT before and after each operation.
 */
#include "edu430_gpio.h"
#include "edu430_system.h"
int main(void)
{
    EDU_WATCHDOG_STOP();if(!edu_clock_1mhz())while(1){}
    /* 1: plain registers */
    P1OUT &= (uint8_t)~BIT0;
    P1DIR |= BIT0;
    /* 2: generic bit macros */
    EDU_SET(P1OUT,BIT0);
    EDU_TOGGLE(P1OUT,BIT0);
    /* 3: named pin operations, still the same register */
    EDU_PIN_ON(P1,BIT0);
    while(1){EDU_PIN_TOGGLE(P1,BIT0);__delay_cycles(250000);}
}
