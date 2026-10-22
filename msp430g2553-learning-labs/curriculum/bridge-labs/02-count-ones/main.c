/* Bridge 02: standalone C alternative to main.asm.
 * Count set bits in 16-bit value and hold result for CCS debugger.
 */
#include <msp430.h>
#include <stdint.h>
volatile uint16_t input_word = 0xEDCBu;
volatile uint16_t ones_count = 0u;
static uint16_t count_ones(uint16_t word) {
    uint16_t count=0;
    while(word != 0u) { count += (uint16_t)(word & 1u); word >>= 1; }
    return count;
}
int main(void) {
    WDTCTL=WDTPW|WDTHOLD;
    ones_count=count_ones(input_word);
    for(;;) { __no_operation(); }
}
