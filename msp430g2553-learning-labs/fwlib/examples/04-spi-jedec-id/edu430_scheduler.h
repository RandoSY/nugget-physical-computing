#ifndef EDU430_SCHEDULER_H
#define EDU430_SCHEDULER_H
#include <stdint.h>
/* Tick clock is supplied by the user's Timer_A ISR.
 * No task stacks, heap, thread switching, or hidden ISR work.
 * Reset timestamps to 'now' to skip missed deadlines after overruns.
 * uint16_t wraps after 65.536s: periods MUST be less than 32768ms.
 */
typedef struct {uint16_t last_ms; uint16_t interval_ms;} edu_job_t;
static inline uint8_t edu_job_due(edu_job_t *job,uint16_t now)
{
    if(!job || job->interval_ms==0u || job->interval_ms>=32768u) return 0u;
    if((uint16_t)(now-job->last_ms)<job->interval_ms) return 0u;
    job->last_ms=now; return 1u;
}
#define EDU_RUN_EVERY(job,now,task) do { \
    if(edu_job_due(&(job),(now))) {task();} \
}while(0)
/* Students can expand the macro into an ordinary `if` statement. */
#endif
