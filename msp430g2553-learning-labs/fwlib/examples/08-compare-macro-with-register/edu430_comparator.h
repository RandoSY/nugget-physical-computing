#ifndef EDU430_COMPARATOR_H
#define EDU430_COMPARATOR_H
#include <msp430.h>
#include <stdint.h>
/* Comparator_A+: students should inspect comparator inputs and reference
 * selections in SLAS735 datasheet before energizing any external source. */
#define EDU_COMP_ON() (CACTL1 |= CAON)
#define EDU_COMP_OFF() (CACTL1 &= (uint8_t)~CAON)
#define EDU_COMP_OUTPUT_HIGH() ((CACTL2 & CAOUT)!=0u)
#define EDU_COMP_IRQ_ENABLE() (CACTL1 |= CAIE)
#define EDU_COMP_IRQ_DISABLE() (CACTL1 &= (uint8_t)~CAIE)
#define EDU_COMP_IRQ_CLEAR() (CACTL1 &= (uint8_t)~CAIFG)
#endif
