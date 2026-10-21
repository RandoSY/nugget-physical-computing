/* Host-executed deterministic tests (not a hardware validation). */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../03-run-every-macro/tiny_scheduler.h"
#include "../03-run-every-macro/button_filter.h"

static void expect(unsigned int condition, const char *message)
{
    if (!condition) { fprintf(stderr, "FAIL: %s\n", message); exit(1); }
    printf("PASS: %s\n", message);
}
int main(void)
{
    periodic_job_t j = {0u, 250u};
    periodic_job_t wrap = {65530u, 10u};
    button_filter_t b = {0u, 0u, 0u};
    unsigned int presses = 0u;
    unsigned int i;
    unsigned char samples[] = {0,0,1,0,1,1,1,1,1,0,0,0,1,1,1};

    expect(!job_is_due(&j, 249u), "timer not due at 249ms");
    expect(job_is_due(&j, 250u), "timer due at 250ms");
    expect(!job_is_due(&j, 251u), "timer not immediately due again");
    expect(job_is_due(&j, 501u), "timer fires after subsequent period");
    expect(job_is_due(&j, 1200u), "overrun performs exactly one catch-up call");
    expect(!job_is_due(&j, 1201u), "overrun does not cause burst of callbacks");
    expect(!job_is_due(&wrap, 2u), "wrap: only 8ms elapsed");
    expect(job_is_due(&wrap, 4u), "wrap: 10ms elapsed");
    expect(!job_is_due(&wrap, 5u), "wrap: no immediate repeat");

    for (i=0; i<sizeof(samples)/sizeof(samples[0]); i++)
        presses += button_new_press(&b, samples[i]);
    expect(presses == 2u, "debouncer: two real presses despite bounce and hold");
    puts("All host logic tests passed; MSP430 real-time behavior NOT tested.");
    return 0;
}
