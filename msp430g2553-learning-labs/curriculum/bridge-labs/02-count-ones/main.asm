; Bridge 02 MSP430 assembler, standalone entry point.
; 16 shift cycles; clear carry before each RRC to ensure a logical
; right shift. Result count in R6, input discarded in R5.
            .cdecls C,LIST,"msp430.h"
            .text
            .retain
            .retainrefs
RESET       mov.w   #__STACK_END,SP
            mov.w   #WDTPW|WDTHOLD,&WDTCTL
            mov.w   #0EDCBh,R5
            clr.w   R6
            mov.w   #16,R7
NEXT        clrc
            rrc.w   R5
            jnc     NOT_ONE
            inc.w   R6
NOT_ONE     dec.w   R7
            jnz     NEXT
DONE        jmp     DONE
            .global __STACK_END
            .sect   .stack
            .sect   ".reset"
            .short  RESET
