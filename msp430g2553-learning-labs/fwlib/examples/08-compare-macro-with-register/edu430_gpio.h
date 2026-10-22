#ifndef EDU430_GPIO_H
#define EDU430_GPIO_H
#include <msp430.h>
#include "edu430_bits.h"
/* Port-token macros deliberately preserve the underlying registers.
 * EDU_GPIO_OUTPUT(P1,BIT0) -> P1SEL &= ~BIT0; P1SEL2 &= ~BIT0;
 *                              P1DIR |= BIT0;
 * P3 GPIO is available on large packages only; the 20-pin LaunchPad
 * MSP430G2553 exposes Port 1 and Port 2, so prefer P1/P2 in labs.
 */
#define EDU_GPIO_SELECT_IO(port,mask) do { \
    EDU_CLEAR(EDU_PORT_REG(port,SEL),mask); \
    EDU_CLEAR(EDU_PORT_REG(port,SEL2),mask); \
} while(0)
#define EDU_GPIO_OUTPUT(port,mask) do { \
    EDU_GPIO_SELECT_IO(port,mask); \
    EDU_SET(EDU_PORT_REG(port,DIR),mask); \
} while(0)
#define EDU_GPIO_OUTPUT_LOW(port,mask) do { \
    EDU_CLEAR(EDU_PORT_REG(port,OUT),mask); \
    EDU_GPIO_OUTPUT(port,mask); \
} while(0)
#define EDU_GPIO_OUTPUT_HIGH(port,mask) do { \
    EDU_SET(EDU_PORT_REG(port,OUT),mask); \
    EDU_GPIO_OUTPUT(port,mask); \
} while(0)
#define EDU_GPIO_INPUT(port,mask) do { \
    EDU_GPIO_SELECT_IO(port,mask); \
    EDU_CLEAR(EDU_PORT_REG(port,DIR),mask); \
} while(0)
#define EDU_GPIO_PULLUP(port,mask) do { \
    EDU_GPIO_INPUT(port,mask); \
    EDU_SET(EDU_PORT_REG(port,OUT),mask); \
    EDU_SET(EDU_PORT_REG(port,REN),mask); \
} while(0)
#define EDU_GPIO_PULLDOWN(port,mask) do { \
    EDU_GPIO_INPUT(port,mask); \
    EDU_CLEAR(EDU_PORT_REG(port,OUT),mask); \
    EDU_SET(EDU_PORT_REG(port,REN),mask); \
} while(0)
#define EDU_GPIO_FLOAT(port,mask) do { \
    EDU_GPIO_INPUT(port,mask); \
    EDU_CLEAR(EDU_PORT_REG(port,REN),mask); \
} while(0)
#define EDU_PIN_ON(port,mask)     EDU_SET(EDU_PORT_REG(port,OUT),mask)
#define EDU_PIN_OFF(port,mask)    EDU_CLEAR(EDU_PORT_REG(port,OUT),mask)
#define EDU_PIN_TOGGLE(port,mask) EDU_TOGGLE(EDU_PORT_REG(port,OUT),mask)
#define EDU_PIN_HIGH(port,mask)   EDU_ALL_SET(EDU_PORT_REG(port,IN),mask)
#define EDU_PIN_LOW(port,mask)    EDU_ALL_CLEAR(EDU_PORT_REG(port,IN),mask)
/* The following interrupt operations exist ONLY on P1 and P2, not P3. */
#define EDU_GPIO_IRQ_ENABLE(port,mask) EDU_SET(EDU_PORT_REG(port,IE),mask)
#define EDU_GPIO_IRQ_DISABLE(port,mask) EDU_CLEAR(EDU_PORT_REG(port,IE),mask)
#define EDU_GPIO_IRQ_CLEAR(port,mask) EDU_CLEAR(EDU_PORT_REG(port,IFG),mask)
#define EDU_GPIO_IRQ_FALLING(port,mask) EDU_SET(EDU_PORT_REG(port,IES),mask)
#define EDU_GPIO_IRQ_RISING(port,mask) EDU_CLEAR(EDU_PORT_REG(port,IES),mask)
#endif
