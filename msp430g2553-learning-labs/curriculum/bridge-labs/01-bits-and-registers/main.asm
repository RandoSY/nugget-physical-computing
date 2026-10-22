; Bridge 01 MSP430 assembler: standalone assembly-only CCS project.
; IMPORTANT: Do not link with the C source or another startup vector.
; Educational source, requires verification with installed TI assembler.
            .cdecls C,LIST,"msp430.h"
            .text
            .retain
            .retainrefs
RESET       mov.w   #__STACK_END,SP
            mov.w   #WDTPW|WDTHOLD,&WDTCTL
            bic.b   #BIT0,&P1SEL
            bic.b   #BIT0,&P1SEL2
            bic.b   #BIT0,&P1OUT
            bis.b   #BIT0,&P1DIR
TOGGLE      xor.b   #BIT0,&P1OUT
            mov.w   #0FFFFh,R6
DELAY       dec.w   R6
            jnz     DELAY
            jmp     TOGGLE
            .global __STACK_END
            .sect   .stack
            .sect   ".reset"
            .short  RESET
