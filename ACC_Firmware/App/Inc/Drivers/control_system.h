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


/**
 * @brief Defines PWM modes of controlling each of the 3 fans.
 */
typedef enum {
	BASE_MODE,  /**< BASE_MODE */
	ABOVE_40DEG,/**< ABOVE_40DEG */
	ABOVE_50DEG /**< ABOVE_50DEG */
} Ctrl_State_t;


typedef enum {
	CONTROL_STATE_CHANGED,
	OVERCURRENT_WARNING,
	SWITCH_ENABLE,
	SWITCH_DISABLE
	// WIP...

} ACC_State_t;

typedef enum {
	SW_EN_LED_ON,
	SW_EN_LED_OFF,
	SW_OVERCURRENT
} LED_t;

typedef struct {
	Ctrl_State_t ctrl;
	ACC_State_t flag;
	LED_t led;
} ACC_t;


void ACC_Control_Loop(ACC_t *acc, SensorInputs_t *si, CAN_Driver_t *can);
void CAN_TransmitAll_SensorData(SensorInputs_t *si, CAN_Driver_t *can);
void Set_SwitchEnable(ACC_t *acc, bool en);
void Toggle_Status_LEDs(ACC_t *acc);

#endif /* INC_CONTROL_SYSTEM_H_ */
