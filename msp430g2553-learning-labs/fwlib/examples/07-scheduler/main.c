/* Uses Timer_A hardware 1ms ISR and a small cooperative scheduler.
 * No task stacks; each function must return quickly.
 * Compare the RUN_EVERY macro with its expanded `if` condition. */
#include <msp430.h>
#include <stdint.h>
#include "edu430_system.h"
#include "edu430_gpio.h"
#include "edu430_timer.h"
#include "edu430_scheduler.h"
static volatile uint16_t ms=0;
#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer_tick(void){ms++;}
static void blink_a(void){EDU_PIN_TOGGLE(P1,BIT0);}
static void blink_b(void){EDU_PIN_TOGGLE(P1,BIT6);}
int main(void)
{
    edu_job_t a={0u,250u}, b={0u,750u};
    EDU_WATCHDOG_STOP();if(!edu_clock_1mhz())while(1){}
    EDU_GPIO_OUTPUT_LOW(P1,BIT0|BIT6);
    EDU_TIMER_1MS_TA0(); EDU_IRQ_ON();
    while(1) {
        uint16_t now=ms;
        EDU_RUN_EVERY(a,now,blink_a);
        EDU_RUN_EVERY(b,now,blink_b);
    }
}
