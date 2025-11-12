#include "mock_temperature_sensor.h"

#include <string.h>

#include "cmock.h"
#include "unity.h"

#define MAX_EXPECTED_READS 16

typedef struct
{
    int     expected_returns[MAX_EXPECTED_READS];
    size_t  expected_count;
    size_t  actual_count;
} MockTemperatureSensorState;

static MockTemperatureSensorState MockTemperatureSensor;

static void ensure_capacity(void)
{
    if (MockTemperatureSensor.expected_count >= MAX_EXPECTED_READS)
    {
        TEST_FAIL_MESSAGE("Too many TemperatureSensor_Read expectations registered. Increase MAX_EXPECTED_READS.");
    }
}

void mock_temperature_sensor_Init(void)
{
    memset(&MockTemperatureSensor, 0, sizeof(MockTemperatureSensor));
    CMock_RegisterVerify(mock_temperature_sensor_Verify);
    CMock_RegisterDestroy(mock_temperature_sensor_Destroy);
}

void mock_temperature_sensor_Verify(void)
{
    TEST_ASSERT_EQUAL_MESSAGE(MockTemperatureSensor.expected_count,
                              MockTemperatureSensor.actual_count,
                              "TemperatureSensor_Read call count mismatch.");
}

void mock_temperature_sensor_Destroy(void)
{
    memset(&MockTemperatureSensor, 0, sizeof(MockTemperatureSensor));
}

void TemperatureSensor_Read_ExpectAndReturn(int return_value)
{
    ensure_capacity();
    MockTemperatureSensor.expected_returns[MockTemperatureSensor.expected_count++] = return_value;
}

void TemperatureSensor_Read_ExpectAndReturnSequence(const int *values, size_t length)
{
    TEST_ASSERT_NOT_NULL_MESSAGE(values, "TemperatureSensor_Read sequence values must not be NULL.");

    for (size_t i = 0; i < length; ++i)
    {
        TemperatureSensor_Read_ExpectAndReturn(values[i]);
    }
}

int TemperatureSensor_Read(void)
{
    if (MockTemperatureSensor.actual_count >= MockTemperatureSensor.expected_count)
    {
        TEST_FAIL_MESSAGE("TemperatureSensor_Read called more times than expected.");
    }

    return MockTemperatureSensor.expected_returns[MockTemperatureSensor.actual_count++];
}
