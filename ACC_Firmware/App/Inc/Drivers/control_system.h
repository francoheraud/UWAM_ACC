/*
 * control_system.h
 *
 *  Created on: Nov 3, 2025
 *      Author: Franc
 */

#ifndef INC_CONTROL_SYSTEM_H_
#define INC_CONTROL_SYSTEM_H_

#include "Drivers/can_driver.h"
#include "Drivers/sensor_inputs.h"
#include "Other/acc_config.h"

typedef struct {
	float setpoint_temperature;		// deg C
	float setpoint_pressure;		// Pa

	float actual_temperature;
	float actual_pressure;

} ACC_t;


#endif /* INC_CONTROL_SYSTEM_H_ */
