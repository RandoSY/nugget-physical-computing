#ifndef EDU430_TIMER_H
#define EDU430_TIMER_H
#include <msp430.h>
#include "edu430_bits.h"
/* Literal timer token TA0 or TA1 builds a register name.  Examples:
 * EDU_TIMER_UP(TA0, 999, TASSEL_2 | ID_0) ->
 *   TA0CCR0=999; TA0CCTL0=CCIE; TA0CTL=TASSEL_2|ID_0|MC_1|TACLR;
 * Do not configure Timer_A twice: drivers and scheduler must coordinate. */
#define EDU_TIMER_UP(timer,top,clockbits) do { \
    EDU_PORT_REG(timer,CTL) = MC_0; \
    EDU_PORT_REG(timer,CCR0) = (top); \
    EDU_PORT_REG(timer,CCTL0) = CCIE; \
    EDU_PORT_REG(timer,CTL) = (clockbits) | MC_1 | TACLR; \
} while(0)
#define EDU_TIMER_STOP(timer) EDU_CLEAR(EDU_PORT_REG(timer,CTL),MC_3)
#define EDU_TIMER_READ(timer) (EDU_PORT_REG(timer,R))
#define EDU_TIMER_COMPARE(timer,channel,value) (EDU_PORT_REG(timer,CCR##channel)=(value))
/* 1MHz SMCLK / 1000 ticks -> 1 kHz (= 1ms per timer interrupt).
 * ISR must increment a volatile uint16_t in the application. */
#define EDU_TIMER_1MS_TA0() EDU_TIMER_UP(TA0,999u,TASSEL_2 | ID_0)
#endif
