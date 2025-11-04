/*
* sensor_inputs.c
* Created on: Nov 1, 2025
* Author: Franc
*/

#include "sensor_inputs.h"
#include <string.h>
#include <math.h>
extern TIM_HandleTypeDef htim2;

#ifndef VDDA_MV_DEFAULT
#define VDDA_MV_DEFAULT 3300.0f
#endif

#ifndef ADC_RESOLUTION
#define ADC_RESOLUTION 4095.0f 			// 12-bit
#endif

/* Divider on sensors 1..5: 30k (top) / 57.6k (bottom):
Vadc = Vsensor * 57.6 / (30 + 57.6) = 0.657 * Vsensor
Undo divider by multiplying by ~1/0.657 = 1.522
*/
#ifndef SENSOR_DIVIDER_UNDO_GAIN
#define SENSOR_DIVIDER_UNDO_GAIN 	1.522f
#endif

// Honeywell MIPAM1XX050PAAAX (0–50 psi, absolute, 0.5–4.5 V @ 5 V)
#define P_SENSOR_VSUPPLY 			5.0f // Vs
#define P_SENSOR_PMIN_PSI 			0.0f
#define P_SENSOR_PMAX_PSI 			50.0f
#define PSI_TO_BAR 					0.0689476f

// Some helper functions:

static inline float Constrain(float x, float low, float high) {
	if (x < low) return low;
	if (x > high) return high;
	return x;
}

static inline float Convert_ADC_Code_To_mV(uint16_t code) {
	return ( (float)code * VDDA_MV_DEFAULT ) / ADC_RESOLUTION;
}


// SOURCE: ...
static inline float Convert_Pressure_PSI_from_mV(float mV_adc) {
	float v_adc = mV_adc / 1000.0f; // V at MCU pin
	float v_sensor = v_adc * SENSOR_DIVIDER_UNDO_GAIN; // V at sensor pin
	float ratio = v_sensor / P_SENSOR_VSUPPLY; // Vout / Vs
	float norm = (ratio - 0.10f) / 0.80f; // 0..1 across range
	if (norm < 0.0f) norm = 0.0f;
	if (norm > 1.0f) norm = 1.0f;
	return P_SENSOR_PMIN_PSI + norm * (P_SENSOR_PMAX_PSI - P_SENSOR_PMIN_PSI);
}


// NOTE: THIS IS WRONG!!
// I need to either: Do my own calibration to find beta or get the data sheet for the thermistors!!
static inline float Convert_Temperature_C_from_mV(float mV_adc) {
	float v_adc = mV_adc / 1000.0f;
	const float beta = 3991.0f;
	const float temp_ref = 298.15f; // 25 deg C
	//const float r0 = (float)pow(10,4), r_fixed = (float)pow(10,4);
	const float ratio = 20.05f;

	// using the NTC thermistor temperature equation:
	// 1/T = 1/T0 + 1/beta * ln (R / R0)

	float r_therm =  r_fixed * (v_adc / (3.3f - v_adc));
	float temp_inv = 1.0f/temp_ref + (1.0f/beta) * log(ratio);
	return 1.0f / temp_inv;
}



HAL_StatusTypeDef Sensors_Init(SensorInputs_t *si, ADC_HandleTypeDef *adc, TIM_HandleTypeDef *tim) {

	if (!si || !adc || !tim) return HAL_ERROR;

	memset((void*)si->adc_raw, 0, sizeof(si->adc_raw));
	memset((void*)si->temp_c, 0, sizeof(si->temp_c));
	memset((void*)si->pressure_psi, 0, sizeof(si->pressure_psi));

	si->adc = adc;
	si->tim = tim;

	si->ch1_duty_cycle = 0.0f;
	si->ch2_duty_cycle = 0.0f;
	si->ch3_duty_cycle = 0.0f;

	return HAL_OK;
}

HAL_StatusTypeDef Sensors_Start(SensorInputs_t *si) {
	if (!si || !si->adc || !si->tim) return HAL_ERROR;


	if (HAL_ADC_Start_DMA(si->adc, (uint32_t*)si->adc_raw, ADC_CH_COUNT) != HAL_OK)
		return HAL_ERROR;

	// start 3 PWM outputs: TIM1 CH1..CH3
	if (HAL_TIM_PWM_Start(si->tim, TIM_CHANNEL_1) != HAL_OK) return HAL_ERROR;
	if (HAL_TIM_PWM_Start(si->tim, TIM_CHANNEL_2) != HAL_OK) return HAL_ERROR;
	if (HAL_TIM_PWM_Start(si->tim, TIM_CHANNEL_3) != HAL_OK) return HAL_ERROR;

	PWM_SetAll(si);

	return HAL_OK;
}

void Store_Temperature_Readings(SensorInputs_t *si) {
	if (!si) return;

	for (uint8_t i = 0; i < 4; i++) {
		float mV_value = Convert_ADC_Code_To_mV(si->adc_raw[i]);
		si->temp_c[i] = Convert_Temperature_C_from_mV(mV_value);
	}
}

/* Convert and store pressures from adc_raw into pressure_psi[] */
void Store_Pressure_Readings(SensorInputs_t *si) {
	if (!si) return;

	// TODO: Do we want PSI or Pa? (1 psi = 6894.757 Pa)
	// const float PSI_TO_PA = 6894.757f;
	for (uint8_t i = 4; i < 7; i++) {
	float mV_value = Convert_ADC_Code_To_mV(si->adc_raw[i]); // in psi
	si->pressure_psi[i - 4] = Convert_Pressure_PSI_from_mV(mV_value) /* * PSI_TO_PA*/;
	}

	// TODO: Plz address the non ADC sensor input!!
	si->pressure_psi[3] = 0.0f;
}

// Apply the struct member duty cycles to TIM1 CH1..CH3
void PWM_SetAll(SensorInputs_t *si) {
	if (!si || !si->tim) return;

	float d1 = Constrain(si->ch1_duty_cycle, 0.0f, 100.0f);
	float d2 = Constrain(si->ch2_duty_cycle, 0.0f, 100.0f);
	float d3 = Constrain(si->ch3_duty_cycle, 0.0f, 100.0f);

	// default res is 255
	uint32_t d1_scaled = (uint32_t)(d1 * (float)PWM_RESOLUTION);
	uint32_t d2_scaled = (uint32_t)(d2 * (float)PWM_RESOLUTION);
	uint32_t d3_scaled = (uint32_t)(d3 * (float)PWM_RESOLUTION);


	__HAL_TIM_SET_COMPARE(si->tim, TIM_CHANNEL_1, d1_scaled);
	__HAL_TIM_SET_COMPARE(si->tim, TIM_CHANNEL_2, d2_scaled);
	__HAL_TIM_SET_COMPARE(si->tim, TIM_CHANNEL_3, d3_scaled);

	/* If CCR preload is enabled, latch all three together */
	//__HAL_TIM_GENERATE_EVENT(si->tim, TIM_EVENTSOURCE_UPDATE);
	HAL_TIM_GenerateEvent(si->tim, TIM_EVENTSOURCE_UPDATE);
}


// TODO: Add a specific can filter config in the near future??
// Read segment temperature data on the CAN bus coming from AMS
bool Update_Segment_Temperature_Values(SensorInputs_t *si, CAN_Driver_t *can) {
    if (!si || !can || !can->hcan1) return false;

	CAN_RxHeaderTypeDef rx = {0};

	if (CAN_Receive(can, &rx) != HAL_OK)
		return false;
	if (HAL_CAN_GetRxFifoFillLevel(can->hcan1, CAN_RX_FIFO0) == 0)
	    return false;
	if (rx.IDE != CAN_ID_STD || rx.DLC < AMS_SEGMENT_COUNT || rx.StdId != AMS_SEG_TEMP_CAN_ID) // how many segs? + whats the id
		return false;

	for (uint8_t i = 0; i < AMS_SEGMENT_COUNT; i++) {
		si->seg_temp_c[i] = (float)can->rx_data[i];
	}

	return true;
}



//TODO: Verify PULSES_PER_REVOLUTION plz!!
const uint8_t PULSES_PER_REVOLUTION = 1;
static volatile uint32_t 	tach_last_ticks 	= 0;
static volatile uint32_t 	tach_delta_ticks 	= 0;
static volatile uint8_t 	tach_new_period	 	= 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == TACH_IN_Pin) {
		uint32_t now = __HAL_TIM_GET_COUNTER(&htim2);

		uint32_t delta;

		if (now >= tach_last_ticks) delta = now - tach_last_ticks;
		else delta = UINT_MAX - tach_last_ticks + now + 1;

		tach_last_ticks = now;

		if (delta > 0) {
			tach_delta_ticks = delta;
			tach_new_period = 1;
		}
	}
}


// Guess work for when it comes to detecting the fan rpm
bool Update_Fan_Speed_Value(SensorInputs_t *si) {
	if (!si) return false;

	if (!tach_new_period || tach_delta_ticks == 0) {
	    si->fan_rpm = 0.0f;
	    return false;
	}

	const float tick_freq_hz = (float)pow(10, 6);
	float pulse_freq_hz = tick_freq_hz / (float)tach_delta_ticks;
	si->fan_rpm = (pulse_freq_hz * 60) / (float)PULSES_PER_REVOLUTION;

	return true;
}









