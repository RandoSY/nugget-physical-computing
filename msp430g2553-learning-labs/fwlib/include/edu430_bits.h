#ifndef EDU430_BITS_H
#define EDU430_BITS_H
/* Pure, transparent register and bit helpers. Arguments are evaluated ONCE.
 * Each call uses the standard, visible C register operation on the right.
 *   EDU_SET(P1OUT, BIT0)       -> P1OUT |= BIT0
 *   EDU_CLEAR(P1OUT, BIT0)     -> P1OUT &= ~BIT0
 *   EDU_TOGGLE(P1OUT, BIT0)    -> P1OUT ^= BIT0
 * Never pass expressions with side effects as `reg` or `mask`. */
#define EDU_SET(reg, mask)       ((reg) |= (mask))
#define EDU_CLEAR(reg, mask)     ((reg) &= ~(mask))
#define EDU_TOGGLE(reg, mask)    ((reg) ^= (mask))
#define EDU_ALL_SET(reg, mask)   (((reg) & (mask)) == (mask))
#define EDU_ANY_SET(reg, mask)   (((reg) & (mask)) != 0)
#define EDU_ALL_CLEAR(reg,mask)  (((reg) & (mask)) == 0)
#define EDU_WRITE_BITS(reg, mask, value) do { \
    (reg) = ((reg) & ~(mask)) | ((value) & (mask)); \
} while(0)
/* Token-paste expands P1, P2, or P3 into P1DIR, P2DIR, etc.
 * Only use with literal port tokens (P1/P2/P3), not runtime variables. */
#define EDU_PORT_REG_(port, suffix) port##suffix
#define EDU_PORT_REG(port, suffix) EDU_PORT_REG_(port,suffix)
#endif
