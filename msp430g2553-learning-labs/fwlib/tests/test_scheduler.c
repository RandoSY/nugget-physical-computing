/* Host deterministic logic verification for 16-bit unsigned rollover. */
#include <assert.h>
#include <stdint.h>
#include "edu430_scheduler.h"
int main(void)
{
    edu_job_t job={65530u,10u};
    assert(edu_job_due(&job,65535u)==0u);
    assert(edu_job_due(&job,4u)==1u); /* wraps after 65535 */
    assert(job.last_ms==4u);
    assert(edu_job_due(&job,8u)==0u);
    assert(edu_job_due(&job,15u)==1u);
    job.interval_ms=0u; assert(edu_job_due(&job,40u)==0u);
    job.interval_ms=40000u; assert(edu_job_due(&job,40u)==0u);
    assert(edu_job_due(0,40u)==0u);
    return 0;
}
