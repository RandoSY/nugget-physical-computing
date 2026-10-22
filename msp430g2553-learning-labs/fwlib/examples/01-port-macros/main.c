/* STEP A: inspect the literal registers, then these macros.
 * No dynamic memory, no hidden scheduler.  Both LaunchPads have P1.0.
 */
#include "edu430_gpio.h"
#include "edu430_system.h"
int main(void)
{
    EDU_WATCHDOG_STOP();
    if(!edu_clock_1mhz()) while(1) { }
    EDU_GPIO_OUTPUT_LOW(P1,BIT0); /* P1OUT &= ~BIT0; P1DIR |= BIT0 */
    while(1) {
        EDU_PIN_TOGGLE(P1,BIT0); /* becomes P1OUT ^= BIT0 */
        __delay_cycles(200000);  /* intentionally blocks CPU */
    }
}
