#ifndef MSP430_H
#define MSP430_H
#include <stdint.h>
#define __interrupt
#define __enable_interrupt() do {} while(0)
#define __disable_interrupt() do {} while(0)
#define __delay_cycles(x) do {(void)(x);} while(0)
#define __bis_SR_register(x) do {(void)(x);} while(0)
#define __bic_SR_register_on_exit(x) do {(void)(x);} while(0)
#define __no_operation() do {} while(0)
#define LPM0_bits 0x10u
#define GIE 0x08u
#define BIT0 0x01u
#define BIT1 0x02u
#define BIT2 0x04u
#define BIT3 0x08u
#define BIT4 0x10u
#define BIT5 0x20u
#define BIT6 0x40u
#define BIT7 0x80u
extern volatile unsigned int WDTCTL;
extern volatile unsigned int DCOCTL;
extern volatile unsigned int BCSCTL1;
extern volatile unsigned int P1DIR;
extern volatile unsigned int P1REN;
extern volatile unsigned int P1OUT;
extern volatile unsigned int P1IN;
extern volatile unsigned int P1SEL;
extern volatile unsigned int P1SEL2;
extern volatile unsigned int P1IES;
extern volatile unsigned int P1IFG;
extern volatile unsigned int P1IE;
extern volatile unsigned int P2DIR;
extern volatile unsigned int P2OUT;
extern volatile unsigned int P2IN;
extern volatile unsigned int P2SEL;
extern volatile unsigned int P2SEL2;
extern volatile unsigned int UCA0CTL1;
extern volatile unsigned int UCA0BR0;
extern volatile unsigned int UCA0BR1;
extern volatile unsigned int UCA0MCTL;
extern volatile unsigned int UCA0TXBUF;
extern volatile unsigned int UCA0RXBUF;
extern volatile unsigned int IFG2;
extern volatile unsigned int ADC10CTL0;
extern volatile unsigned int ADC10CTL1;
extern volatile unsigned int ADC10AE0;
extern volatile unsigned int ADC10MEM;
extern volatile unsigned int TA0CCR0;
extern volatile unsigned int TA0CCR1;
extern volatile unsigned int TA0CCTL0;
extern volatile unsigned int TA0CCTL1;
extern volatile unsigned int TA0CTL;
extern volatile unsigned int TA1CTL;
extern volatile unsigned int TA1R;
#define WDTPW 0x1u
#define WDTHOLD 0x1u
#define CALBC1_1MHZ 0x1u
#define CALDCO_1MHZ 0x1u
#define UCSWRST 0x1u
#define UCSSEL_2 0x1u
#define UCBRS0 0x1u
#define UCA0TXIFG 0x1u
#define UCA0RXIFG 0x1u
#define ADC10SHT_2 0x1u
#define ADC10ON 0x1u
#define ADC10BUSY 0x1u
#define INCH_4 0x1u
#define ENC 0x1u
#define ADC10SC 0x1u
#define TASSEL_2 0x1u
#define MC_1 0x1u
#define MC_2 0x1u
#define TACLR 0x1u
#define CCIE 0x1u
#define OUTMOD_7 0x1u

#define LPM3_bits 0x20u
#define CPUOFF 0x10u
#define WDTIS0 0x01u
#define WDTCNTCL 0x08u
#define MC_0 0u
#define MC_3 0x0030u
#define ID_0 0u
#define UCCKPH 0x80u
#define UCCKPL 0x40u
#define UCMST 0x08u
#define UCMSB 0x20u
#define UCSYNC 0x01u
#define UCMODE_3 0x06u
#define UCTR 0x10u
#define UCTXSTT 0x02u
#define UCTXSTP 0x04u
#define UCNACKIFG 0x01u
#define UCBBUSY 0x02u
#define UCB0TXIFG 0x04u
#define UCB0RXIFG 0x08u
extern volatile uint16_t BCSCTL2;
extern volatile uint16_t P2REN,P2IE,P2IFG,P2IES;
extern volatile uint16_t P3OUT,P3IN,P3DIR,P3REN,P3SEL,P3SEL2;
extern volatile uint16_t TA0R,TA0CCR2,TA0CCTL2;
extern volatile uint16_t TA1CCR0,TA1CCTL0,TA1CCR1,TA1CCTL1,TA1CCR2,TA1CCTL2;
extern volatile uint16_t UCB0CTL0,UCB0CTL1,UCB0BR0,UCB0BR1;
extern volatile uint16_t UCB0TXBUF,UCB0RXBUF,UCB0SA,UCB0STAT;
#define TIMER0_A0_VECTOR 0x01u
#endif
