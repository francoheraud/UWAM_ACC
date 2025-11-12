#include "unity.h"

#include "cmock.h"
#include "fan_control.h"
#include "mock_temperature_sensor.h"

void setUp(void)
{
    CMock_Init();
    mock_temperature_sensor_Init();
    FanControl_Reset();
}

void tearDown(void)
{
    CMock_Verify();
    CMock_Destroy();
}

void test_FanControl_TurnsFanOnAboveThreshold(void)
{
    TemperatureSensor_Read_ExpectAndReturn(FAN_CONTROL_TEMP_ON_C + 2);

    TEST_ASSERT_TRUE(FanControl_ShouldEnableCooling());
    TEST_ASSERT_EQUAL_INT(FAN_CONTROL_TEMP_ON_C + 2, FanControl_GetLastTemperature());
}

void test_FanControl_TurnsFanOffBelowHysteresis(void)
{
    const int sequence[] = {
        FAN_CONTROL_TEMP_ON_C + 3,
        FAN_CONTROL_TEMP_ON_C - 1,
        FAN_CONTROL_TEMP_OFF_C - 2
    };

    TemperatureSensor_Read_ExpectAndReturnSequence(sequence, 3);

    TEST_ASSERT_TRUE(FanControl_ShouldEnableCooling());
    TEST_ASSERT_TRUE(FanControl_ShouldEnableCooling());
    TEST_ASSERT_FALSE(FanControl_ShouldEnableCooling());
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_FanControl_TurnsFanOnAboveThreshold);
    RUN_TEST(test_FanControl_TurnsFanOffBelowHysteresis);

    return UNITY_END();
}
