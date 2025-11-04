/**
 * Main ACC Control Code
 * - Franco Heraud
 */


#include "control_system.h"

static float highest_input_temp(SensorInputs_t *si) {
	float largest_temp = -1000.0f;
	for (uint8_t i = 0; i < 4; i++) {
		if (largest_temp < si->temp_c[i])
			largest_temp = si->temp_c[i];
	}
	return largest_temp;
}

static float highest_seg_temp(SensorInputs_t *si) {
	float largest_temp = -1000.0f;
	for (uint8_t i = 0; i < AMS_SEGMENT_COUNT; i++) {
		if (largest_temp < si->seg_temp_c[i])
			largest_temp = si->seg_temp_c[i];
	}
	return largest_temp;
}


// Send all the data in the temp + pressure buffers over to the relevant CAN bus
void CAN_TransmitAll_SensorData(SensorInputs_t *si, CAN_Driver_t *can) {
	for (uint8_t i = 0; i < 8; i++) {
		if (i >= 4) {
			can->tx_data[i] = si->pressure_psi[i - 4];
			continue;
		}
		can->tx_data[i] = si->temp_c[i];
	}
	CAN_TxHeaderTypeDef tx = {0};
	tx.StdId 	= ACC_DOUT_CAN_ID1;
	tx.DLC 		= 8;
	CAN_Transmit2(can, &tx);

	memset((void*)can->tx_data, 0, sizeof(can->tx_data));
	for (uint8_t j = 0; j < AMS_SEGMENT_COUNT; j++) {
		can->tx_data[j] = si->seg_temp_c[j];
	}

	can->tx_data[AMS_SEGMENT_COUNT] = si->fan_rpm;
	tx.StdId 	= ACC_DOUT_CAN_ID2;
	tx.DLC 		= AMS_SEGMENT_COUNT;
	CAN_Transmit2(can, &tx);

	memset((void*)can->tx_data, 0, sizeof(can->tx_data));

	return;
}

typedef enum {
	BASE_MODE,
	ABOVE_40DEG,
	ABOVE_50DEG
} State;

// Uses a simple threshold algorithm with hysteresis
void ACC_Control_Loop(ACC_t *acc, SensorInputs_t *si, CAN_Driver_t *can) {

	static State control_state = BASE_MODE;

	Store_Temperature_Readings(si);
	Store_Pressure_Readings(si);

	float tseg_max 	= highest_seg_temp(si);
	float tin_max	= highest_input_temp(si);
	float tmax;
	if (tseg_max > tin_max) tmax = tseg_max;
	else tmax = tin_max;


	switch (control_state) {
	case (BASE_MODE):
		si->ch1_duty_cycle = 0.3f;
		si->ch2_duty_cycle = 0.3f;
		si->ch3_duty_cycle = 0.3f;
		if (tmax > TEMP_THRESHOLD1)
			control_state = ABOVE_40DEG;
		break;

	case (ABOVE_40DEG):
		si->ch1_duty_cycle = 0.7f;
		si->ch2_duty_cycle = 0.7f;
		si->ch3_duty_cycle = 0.7f;
		if (tmax > TEMP_THRESHOLD2)
			control_state = ABOVE_50DEG;

		else if (tmax < TEMP_THRESHOLD1 - HYSTERESIS_WIDTH)
			control_state = BASE_MODE;

		break;
	case (ABOVE_50DEG):
		si->ch1_duty_cycle = 1.0f;
		si->ch2_duty_cycle = 1.0f;
		si->ch3_duty_cycle = 1.0f;

		if (tmax < TEMP_THRESHOLD2 - HYSTERESIS_WIDTH)
			control_state = ABOVE_40DEG;

		break;
	}

	PWM_SetAll(si);
	return ;
}


// Experimental PID Controller (probably wont be used)
const float kp = 1.0f, ki = 1.0f, kd = 1.0f;
static uint8_t PID_Controller(float setpoint_rpm, float actual_rpm) {
	static uint8_t error_old = 0, error_old2 = 0, pwm_old = 0;
	uint8_t error = (uint8_t)(setpoint_rpm - actual_rpm);
	uint8_t pwm = pwm_old + (uint8_t)(kp * (float)(error - error_old))
			+ (uint8_t)(ki * (float)(error + error_old)/2.0f)
			+ (uint8_t)(kd * (float)(error - 2.0f * error_old + error_old2));

	error_old2 = error_old;
	error_old = error;
	pwm_old = pwm;
	return pwm;
}


