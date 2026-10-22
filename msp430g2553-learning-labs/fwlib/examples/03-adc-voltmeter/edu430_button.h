#ifndef EDU430_BUTTON_H
#define EDU430_BUTTON_H
#include <stdint.h>
/* Run once per N ms, supplied by a timer-based scheduler. No delays.
 * Count consecutive equal samples to reject switch bounce. */
typedef struct {uint8_t stable;uint8_t last;uint8_t samples;} edu_button_t;
static inline void edu_button_reset(edu_button_t *b)
{
    if(b){b->stable=0;b->last=0;b->samples=0;}
}
static inline uint8_t edu_button_pressed(edu_button_t *b,uint8_t pressed,uint8_t consecutive)
{
    if(!b || consecutive==0u) return 0u;
    pressed=(pressed!=0u);
    if(b->last==pressed) {if(b->samples<consecutive)++b->samples;}
    else {b->last=pressed;b->samples=1u;}
    if(b->samples<consecutive || b->stable==pressed) return 0u;
    b->stable=pressed;
    return pressed; /* event only for stable press, not release */
}
#endif
