/*
* sensor_inputs.c
* Created on: Nov 1, 2025
* Author: Franc
*/

#include "sensor_inputs.h"

extern TIM_HandleTypeDef htim2;

static float Constrain(float x, float low, float high) {
	if (x < low) return low;
	if (x > high) return high;
	return x;
}

// Most like wrong -> Made an assumption on the min and max pressures in psi
static float Convert_Voltage_To_Pressure_PSI(float volts) {
    const float v_supply    	= 5.0f;		// volts
    const float pressure_min  	= 0.0f;     // psi
    const float pressure_max  	= 100.0f;   // psi

    float ratio 	= (volts / v_supply - 0.10f) / 0.80f;
    ratio 			= Constrain(ratio, 0.0f, 1.0f);

    return pressure_min + ratio * (pressure_max - pressure_min);
}


// Most likely wrong -> Will need to confirm the proper range of Rt/25 values!!
static float Convert_Voltage_To_Temperature_degC(float volts) {
	const float r_ref = 280.0f, r_25 = 2752.0f;				// ohms

	const float v_supply 	= 5.0f;							// volts
	float r_therm 			= (volts * r_ref) / (v_supply - volts);	// ohms
	float ratio 			= r_therm / r_25;
	float ln_ratio 			= log(ratio);

	// assumption made here: will need to physically test the Rt/25 value
	const float a = 0.003354016f, b = 0.000250275f, c = 2.42945e-6f, d = -7.3121e-8f;

	float temp_inv_K	= a + b * ln_ratio + c * ln_ratio * ln_ratio + d * ln_ratio * ln_ratio * ln_ratio;
	float temp_K		= 1 / temp_inv_K;
	float temp_degC		= temp_K - 273.15f;

	return temp_degC;
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

// Note: A sampling time of atleast 181.5 cycles prevents adc buffer indexes changing according to my testing
void Update_ADC_Buffers(SensorInputs_t *si) {

	// perform a single adc read
	HAL_ADC_Start(si->adc);

	for (uint8_t i = 0; i < ADC_CH_COUNT; i++)
		si->adc_mV[i] = (si->adc_raw[i] / (float)ADC_RESOLUTION) * 3.30f;
}

void Store_Temperature_Readings(SensorInputs_t *si) {
	for (uint8_t i = 0; i < 4; i++)
		si->temp_c[i] = Convert_Voltage_To_Temperature_degC(si->adc_mV[i]);
}

/* Convert and store pressures from adc_raw into pressure_psi[] */
void Store_Pressure_Readings(SensorInputs_t *si) {
	// TODO: Do we want PSI or Pa? (1 psi = 6894.757 Pa)
	// const float PSI_TO_PA = 6894.757f;
	for (uint8_t i = 4; i < 7; i++)
		si->pressure_psi[i - 4] = Convert_Voltage_To_Pressure_PSI(si->adc_mV[i]);

	// TODO: Plz address the non ADC sensor input!!
	//si->pressure_psi[3] = 0.0f;
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

}



// TODO: Add a specific can filter config in the near future??
// Read segment temperature data on the CAN bus coming from AMS
bool Update_Segment_Temperature_Values(SensorInputs_t *si, CAN_Driver_t *can) {
    if (!si || !can || !can->hcan1) return false;

	CAN_RxHeaderTypeDef rx = {0};

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

static volatile uint32_t tach_last_ticks  = 0;
static volatile uint32_t tach_delta_ticks = 0;
static volatile uint8_t  tach_new_period  = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == TACH_IN_Pin) {
        uint32_t now = __HAL_TIM_GET_COUNTER(&htim2);

        uint32_t delta = now - tach_last_ticks;
        tach_last_ticks = now;

        if (delta > 0) {
            tach_delta_ticks = delta;
            tach_new_period  = 1;
        }
    }
}

void Update_Fan_Speed(SensorInputs_t *si) {
	if (!tach_new_period || tach_delta_ticks == 0) si->fan_rpm = 0.0f;
	const float tick_freq_hz = (float)pow(10, 6);
	float pulse_freq_hz = tick_freq_hz / (float)tach_delta_ticks;
	si->fan_rpm = (pulse_freq_hz * 60) / (float)PULSES_PER_REVOLUTION;
}
