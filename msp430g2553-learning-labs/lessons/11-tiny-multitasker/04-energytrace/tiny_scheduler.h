#ifndef TINY_SCHEDULER_H
#define TINY_SCHEDULER_H
/*
 * IMPORTANT: THIS HEADER DOES NOT CREATE TASKS OR INTERRUPTS.
 * It contains one tiny data structure, one function, and one macro.
 *
 *                PROGRESSIVE EXPANSION (STUDENT EXERCISE)
 *
 * Friendly:   RUN_EVERY(blink_job, now, blink_led);
 *
 * Expands:    do {
 *                 if (job_is_due(&blink_job, now)) {
 *                     blink_led();
 *                 }
 *             } while (0);
 *
 * Underneath: (uint16_t)(now - job->last_run_ms) >= job->period_ms
 *             tests how much time has passed since the last run.
 *
 * Each task is called from main(). It shares the ordinary C call stack.
 * There are NO independent task stacks, switches or priorities.
 */
#include <stdint.h>

typedef struct
{
    uint16_t last_run_ms;  /* time of last invocation, modulo 65536 */
    uint16_t period_ms;    /* expected interval, 1..32767 recommended */
} periodic_job_t;

static unsigned char job_is_due(periodic_job_t *job, uint16_t now)
{
    /* The cast forces 16-bit modulo arithmetic even in host-side tests.
     * This handles a wrap from 65535 back to 0 for frequently polled jobs.
     * Assumption: do not miss an ENTIRE 65.536-second counter revolution.
     */
    if ((uint16_t)(now - job->last_run_ms) < job->period_ms)
    {
        return 0u;             /* Not time yet: let another job run. */
    }
    job->last_run_ms = now;     /* Skip missed periods instead of bursts. */
    return 1u;                 /* Run exactly once on this pass. */
}

/* A deliberately SMALL teaching macro: it merely calls the function
 * above and invokes the named job. Work is not performed in an ISR.
 * The do/while(0) makes the macro behave as a single C statement.
 * Keep task names free of side effects; invoke as RUN_EVERY(..., foo).
 */
#define RUN_EVERY(job, now, task_name)     \
    do {                                  \
        if (job_is_due(&(job), (now))) {   \
            task_name();                  \
        }                                 \
    } while (0)
#endif
