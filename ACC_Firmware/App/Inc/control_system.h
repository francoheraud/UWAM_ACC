/*
 * control_system.h
 *
 *  Created on: Nov 3, 2025
 *      Author: Franc
 */

#ifndef INC_CONTROL_SYSTEM_H_
#define INC_CONTROL_SYSTEM_H_

#include "main.h"
#include "can_driver.h"
#include "sensor_inputs.h"
#include <stdbool.h>
#include <string.h>
#include "acc_config.h"


#define TEMP_THRESHOLD1		40.0f
#define TEMP_THRESHOLD2		50.0f
#define HYSTERESIS_WIDTH	3.0f
#define ACC_DOUT_CAN_ID1	(uint16_t)0x4A2
#define ACC_DOUT_CAN_ID2	(uint16_t)0x4A3




typedef struct {
	float setpoint_temperature;		// deg C
	float setpoint_pressure;		// Pa

	float actual_temperature;
	float actual_pressure;

} ACC_t;





#endif /* INC_CONTROL_SYSTEM_H_ */
