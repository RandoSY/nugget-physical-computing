/* Fake optical sensor: validates the application-level sensor state machine,
 * not the timing of electrical I2C signals. Compile with an ordinary C11 cc.
 */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "edu430_vl6180x.h"

typedef struct {
    uint8_t registers[256];
    uint8_t model, fresh, irq, status, reading;
    unsigned int tuning_writes, waits, attempts, clear_count, reads;
    unsigned int fail_on_write;
} fake_t;
static uint8_t fake_read(void *context, uint16_t reg, uint8_t *out)
{
    fake_t *f=(fake_t*)context;
    ++f->reads;
    if (reg == EDU_VL6180X_REG_MODEL_ID) *out=f->model;
    else if (reg == EDU_VL6180X_REG_FRESH) *out=f->fresh;
    else if (reg == EDU_VL6180X_REG_IRQ_STATUS) *out=f->irq;
    else if (reg == EDU_VL6180X_REG_RANGE_STATUS) *out=f->status;
    else if (reg == EDU_VL6180X_REG_RANGE_MM) *out=f->reading;
    else return 0u;
    return 1u;
}
static uint8_t fake_write(void *context, uint16_t reg, uint8_t value)
{
    fake_t *f=(fake_t*)context;
    ++f->attempts;
    if (f->fail_on_write && f->attempts==f->fail_on_write) return 0u;
    if (reg==EDU_VL6180X_REG_FRESH) { f->fresh=value;return 1u; }
    if (reg==EDU_VL6180X_REG_RANGE_START) return value==1u;
    if (reg==EDU_VL6180X_REG_IRQ_CLEAR) {
        if (value != 7u) return 0u;
        ++f->clear_count;return 1u;
    }
    ++f->tuning_writes;
    return 1u;
}
static void fake_wait(void *context)
{
    fake_t *f=(fake_t*)context;
    ++f->waits;
    if (f->waits == 3u) f->irq=4u;
}
int main(void)
{
    fake_t f={0};
    edu_vl6180x_bus_t b={fake_read,fake_write,fake_wait,&f};
    edu_vl6180x_status_t s;
    uint8_t distance=0u;
    const unsigned int tuning=(unsigned int)(sizeof edu_vl6180x_tuning /
                                       sizeof edu_vl6180x_tuning[0]);
    assert(tuning==43u);
    assert(edu_vl6180x_initialize(0)==EDU_VL_BAD_ARGUMENT);
    f.model=0x99u;
    assert(edu_vl6180x_initialize(&b)==EDU_VL_WRONG_DEVICE);
    f.model=0xB4u;f.fresh=1u;
    assert(edu_vl6180x_initialize(&b)==EDU_VL_OK);
    assert(f.tuning_writes==tuning && f.fresh==0u);
    assert(edu_vl6180x_initialize(&b)==EDU_VL_OK);
    assert(f.tuning_writes==tuning); /* Do NOT retune after the fresh flag. */
    f.waits=0u;f.irq=0u;f.status=0u;f.reading=63u;
    s=edu_vl6180x_range_mm(&b,&distance,100u);
    assert(s==EDU_VL_OK && distance==63u && f.waits==3u);
    assert(f.clear_count==1u);
    f.irq=4u;f.status=0x10u;distance=9u;
    assert(edu_vl6180x_range_mm(&b,&distance,10u)==EDU_VL_RANGE_ERROR);
    assert(distance==9u);  /* Error must not return an invalid result. */
    assert(f.clear_count==2u);
    f.irq=0u;f.waits=99u;f.status=0u;
    assert(edu_vl6180x_range_mm(&b,&distance,2u)==EDU_VL_NOT_READY);
    f.fresh=1u;f.tuning_writes=0u;f.attempts=0u;f.fail_on_write=5u;
    assert(edu_vl6180x_initialize(&b)==EDU_VL_BUS_ERROR);
    puts("PASS: VL6180X 43 settings, identity, ranging, timeout and errors");
    return 0;
}
