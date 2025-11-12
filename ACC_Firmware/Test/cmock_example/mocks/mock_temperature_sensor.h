#ifndef MOCK_TEMPERATURE_SENSOR_H
#define MOCK_TEMPERATURE_SENSOR_H

#include <stddef.h>
#include "temperature_sensor.h"

void mock_temperature_sensor_Init(void);
void mock_temperature_sensor_Verify(void);
void mock_temperature_sensor_Destroy(void);

void TemperatureSensor_Read_ExpectAndReturn(int return_value);
void TemperatureSensor_Read_ExpectAndReturnSequence(const int *values, size_t length);

#endif // MOCK_TEMPERATURE_SENSOR_H
