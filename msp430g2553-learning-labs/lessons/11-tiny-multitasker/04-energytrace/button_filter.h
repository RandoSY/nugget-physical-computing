#ifndef BUTTON_FILTER_H
#define BUTTON_FILTER_H
/* Three matching 10 ms samples = a debounced change of ~30 ms.
 * A button press event is reported ONCE, after a stable HIGH->LOW
 * transition in the physical P1.3 input. 1 means pressed in this API.
 * The filter does not read hardware; pass 0 or 1 in from the caller.
 */
#include <stdint.h>
typedef struct {
    uint8_t last_sample;
    uint8_t stable_state;
    uint8_t same_samples;
} button_filter_t;

static uint8_t button_new_press(button_filter_t *b, uint8_t is_pressed)
{
    if (is_pressed != b->last_sample) {
        b->last_sample = is_pressed;
        b->same_samples = 1u;
    } else if (b->same_samples < 3u) {
        b->same_samples++;
    }
    if (b->same_samples == 3u && is_pressed != b->stable_state) {
        b->stable_state = is_pressed;
        if (is_pressed) return 1u;
    }
    return 0u;
}
#endif
