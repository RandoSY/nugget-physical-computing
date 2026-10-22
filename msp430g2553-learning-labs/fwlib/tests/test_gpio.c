#include <assert.h>
#include <stdint.h>
#include "edu430_gpio.h"
volatile unsigned int P1OUT,P1DIR,P1SEL,P1SEL2,P1REN,P1IN,P1IES,P1IFG,P1IE;
int main(void)
{
    P1OUT=BIT6; P1DIR=0; P1SEL=BIT0;P1SEL2=BIT0;
    EDU_GPIO_OUTPUT_LOW(P1,BIT0);
    assert(P1OUT==BIT6);assert((P1DIR & BIT0)!=0);
    assert((P1SEL & BIT0)==0);assert((P1SEL2 & BIT0)==0);
    EDU_PIN_ON(P1,BIT0);assert(P1OUT==(BIT0|BIT6));
    EDU_PIN_TOGGLE(P1,BIT0);assert(P1OUT==BIT6);
    EDU_GPIO_PULLUP(P1,BIT3);
    assert((P1REN & BIT3)!=0);assert((P1OUT & BIT3)!=0);
    P1IN=BIT3;assert(!EDU_PIN_LOW(P1,BIT3));
    P1IN=0;assert(EDU_PIN_LOW(P1,BIT3));
    return 0;
}
