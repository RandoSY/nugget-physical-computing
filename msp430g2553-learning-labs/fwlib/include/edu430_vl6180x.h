#ifndef EDU430_VL6180X_H
#define EDU430_VL6180X_H
/* ST VL6180X device-layer exemplar: NO MSP430 register references here.
 * The bus API below can point to USCI_B0 hardware I2C, software I2C,
 * or a fake device used in classroom tests. This is the useful boundary:
 *   experiment -> optical sensor -> I2C transaction -> USCI_B0 registers.
 *
 * The sensor accepts 16-bit register addresses, reads/writes 8-bit values.
 * Register names and factory settings are based on ST DT0037.
 * Range is short (~0 to 100 mm under suitable optical conditions).
 */
#include <stdint.h>
#define EDU_VL6180X_ADDR7                  0x29u
#define EDU_VL6180X_MODEL_ID              0xB4u
#define EDU_VL6180X_REG_MODEL_ID          0x0000u
#define EDU_VL6180X_REG_FRESH             0x0016u
#define EDU_VL6180X_REG_RANGE_START       0x0018u
#define EDU_VL6180X_REG_IRQ_CLEAR         0x0015u
#define EDU_VL6180X_REG_RANGE_STATUS      0x004Du
#define EDU_VL6180X_REG_IRQ_STATUS        0x004Fu
#define EDU_VL6180X_REG_RANGE_MM          0x0062u

/* Return 1 for success, 0 for failure: transport maps its own errors.
 * Use `context` to select a simulator or another physical bus in the future.
 */
typedef struct {
    uint8_t (*read)(void *context, uint16_t reg, uint8_t *value);
    uint8_t (*write)(void *context, uint16_t reg, uint8_t value);
    void (*wait_1ms)(void *context); /* Used only while waiting on a sample. */
    void *context;
} edu_vl6180x_bus_t;

typedef enum {
    EDU_VL_OK = 0,
    EDU_VL_BAD_ARGUMENT,
    EDU_VL_BUS_ERROR,
    EDU_VL_WRONG_DEVICE,
    EDU_VL_NOT_READY,
    EDU_VL_RANGE_ERROR
} edu_vl6180x_status_t;

typedef struct {
    uint16_t address;
    uint8_t value;
} edu_vl6180x_reg_t;

/* ST DT0037 revision 1: mandatory post-reset settings. This small static
 * table lives in program memory in a normal MSP430 toolchain build.
 * Do not casually omit entries because their purpose is undocumented.
 */
static const edu_vl6180x_reg_t edu_vl6180x_tuning[] = {
    {0x0207,0x01},{0x0208,0x01},{0x0133,0x01},{0x0096,0x00},
    {0x0097,0xFD},{0x00E3,0x00},{0x00E4,0x04},{0x00E5,0x02},
    {0x00E6,0x01},{0x00E7,0x03},{0x00F5,0x02},{0x00D9,0x05},
    {0x00DB,0xCE},{0x00DC,0x03},{0x00DD,0xF8},{0x009F,0x00},
    {0x00A3,0x3C},{0x00B7,0x00},{0x00BB,0x3C},{0x00B2,0x09},
    {0x00CA,0x09},{0x0198,0x01},{0x01B0,0x17},{0x01AD,0x00},
    {0x00FF,0x05},{0x0100,0x05},{0x0199,0x05},{0x0109,0x07},
    {0x010A,0x30},{0x003F,0x46},{0x01A6,0x1B},{0x01AC,0x3E},
    {0x01A7,0x1F},{0x0103,0x01},{0x0030,0x00},{0x001B,0x0A},
    {0x003E,0x0A},{0x0131,0x04},{0x0011,0x10},{0x0014,0x24},
    {0x0031,0xFF},{0x00D2,0x01},{0x00F2,0x01}
};

static inline edu_vl6180x_status_t edu_vl6180x_initialize(
                                               const edu_vl6180x_bus_t *bus)
{
    uint8_t id, fresh;
    unsigned int i;
    if (!bus || !bus->read || !bus->write) return EDU_VL_BAD_ARGUMENT;
    if (!bus->read(bus->context, EDU_VL6180X_REG_MODEL_ID, &id))
        return EDU_VL_BUS_ERROR;
    if (id != EDU_VL6180X_MODEL_ID) return EDU_VL_WRONG_DEVICE;
    if (!bus->read(bus->context, EDU_VL6180X_REG_FRESH, &fresh))
        return EDU_VL_BUS_ERROR;
    if (fresh & 1u) {
        for (i=0u; i<(sizeof edu_vl6180x_tuning /
                         sizeof edu_vl6180x_tuning[0]); ++i)
            if (!bus->write(bus->context, edu_vl6180x_tuning[i].address,
                                          edu_vl6180x_tuning[i].value))
                return EDU_VL_BUS_ERROR;
        /* Record that the sensor has received its post-reset calibration. */
        if (!bus->write(bus->context, EDU_VL6180X_REG_FRESH, 0x00u))
            return EDU_VL_BUS_ERROR;
    }
    return EDU_VL_OK;
}

/* Single-shot range measurement. A successful result is a value in mm.
 * We poll the documented 'new range sample' flag, not an arbitrary delay.
 * max_wait_ms may be ~150 to 250 for classroom use; 0 means no waiting.
 * Failure conditions remain distinct and accessible to student code.
 */
static inline edu_vl6180x_status_t edu_vl6180x_range_mm(
                          const edu_vl6180x_bus_t *bus,
                          uint8_t *millimeters, uint16_t max_wait_ms)
{
    uint8_t irq=0, status=0, range=0;
    uint16_t count;
    if (!bus || !bus->read || !bus->write || !millimeters)
        return EDU_VL_BAD_ARGUMENT;
    if (!bus->write(bus->context, EDU_VL6180X_REG_RANGE_START, 0x01u))
        return EDU_VL_BUS_ERROR;
    for (count=0u; count < max_wait_ms; ++count) {
        if (!bus->read(bus->context, EDU_VL6180X_REG_IRQ_STATUS, &irq))
            return EDU_VL_BUS_ERROR;
        if ((irq & 0x07u) == 0x04u) break; /* Range interrupt: new sample. */
        if (bus->wait_1ms) bus->wait_1ms(bus->context);
    }
    if ((irq & 0x07u) != 0x04u) return EDU_VL_NOT_READY;
    if (!bus->read(bus->context, EDU_VL6180X_REG_RANGE_STATUS, &status))
        return EDU_VL_BUS_ERROR;
    if (!bus->read(bus->context, EDU_VL6180X_REG_RANGE_MM, &range))
        return EDU_VL_BUS_ERROR;
    /* Always acknowledge the completed sample, including a range error. */
    if (!bus->write(bus->context, EDU_VL6180X_REG_IRQ_CLEAR, 0x07u))
        return EDU_VL_BUS_ERROR;
    if (status & 0xF0u) return EDU_VL_RANGE_ERROR;
    *millimeters = range;
    return EDU_VL_OK;
}
#endif
