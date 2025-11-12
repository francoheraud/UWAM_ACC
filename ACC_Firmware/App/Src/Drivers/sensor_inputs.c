/*
* sensor_inputs.c
* Created on: Nov 1, 2025
* Author: Franc
*/

#include <Drivers/sensor_inputs.h>

extern TIM_HandleTypeDef htim2;

/**
 * @brief Helper constrain function.
* @param float x, float low, float high
* @return x -> Constrained between low and high.
* @note Auto-gen: fill details.
*/
static float Constrain(float x, float low, float high) {
	if (x < low) return low;
	if (x > high) return high;
	return x;
}

/**
 * @brief Helper function which linearly maps a float value from one range to another
* @param float value, float in_min, float in_max, float out_min, float out_max
* @return return_type
* @note Auto-gen: fill details.
*/
static float Linear_Map(float value, float in_min, float in_max, float out_min, float out_max) {
    float normalized_value = (value - in_min) / (in_max - in_min);
    float mapped_value = out_min + (normalized_value * (out_max - out_min));
    return mapped_value;
}

/**
 * @brief Converts output sensor voltage to kPa for BOSCH 0 261 230 280.
 * Assumes linear relationship in pressure calibration as found in spec sheet.
* @param float volts
* @return float p_abs_kPa
* @note Auto-gen: fill details.
*/
static float Voltage_To_kPa_Bosch(float volts) {
	float p_abs_kPa = Linear_Map(volts, 0.4f, 4.65f, 20.0f, 300.0f);
	return p_abs_kPa;
}


/**
 * @brief Converts output sensor voltage to kPa for MIP series pressure sensor.
* @param float volts
* @return float p_abs_kPa
* @note Auto-gen: fill details.
*/
static float Voltage_To_kPa_MIP(float volts) {
    const float v_supply    = 5.0f;		// volts
    const float p_min  		= 0.0f;     // psi
    const float p_max  		= 100.0f;   // psi

    float ratio 	= (volts / v_supply - 0.10f) / 0.80f;
    ratio 			= Constrain(ratio, 0.0f, 1.0f);
    float p_psi 	= p_min + ratio * (p_max - p_min);

    const float psi_to_pa = 6894.757f;
    float p_abs_kPa = (psi_to_pa * p_psi) / 1000.0f;

    return p_abs_kPa;
}


/**
 * @brief Temperature sensor driver for the A-1325.
 * Uses the Stein-Hart equation to compute the temperature.
* @param Output voltage from sensor
* @return float temp_degC -> Temperature in degrees Celsius
* @note Auto-gen: fill details.
*/
static float Voltage_To_DegC_A1325(float volts) {
	const float r_ref = 280.0f, r_25 = 2752.0f;				// ohms

	const float v_supply 	= 5.0f;							// volts
	float r_therm 			= (volts * r_ref) / (v_supply - volts);	// ohms
	float ratio 			= r_therm / r_25;
	float ln_ratio 			= log(ratio);

	// assumption made here: will need to physically test the Rt/25 value
	const float a = 0.003354016f, b = 0.000250275f, c = 2.42945e-6f, d = -7.3121e-8f;

	float temp_inv_K	= a + b * ln_ratio + c * ln_ratio * ln_ratio + d * ln_ratio * ln_ratio * ln_ratio;
	float temp_degC		= (1 / temp_inv_K) - 273.15f;

	return temp_degC;
}

/**
 * @brief Temperature sensor driver for the BOSCH 0 261 230 280.
 * Uses beta parameter equation to compute the temperature.
 * Beta found from: https://www.ametherm.com/thermistor/ntc-thermistor-beta
* @param float volts -> Output voltage from sensor
* @return float temp_degC -> Temperature in degrees Celsius
* @note Auto-gen: fill details.
*/
static float Voltage_To_DegC_Bosch(float volts) {
	const float r_25 		= 2057.0f;
	const float temp_ref 	= 298.15f;		// 25 deg C
	const float beta 		= 3491.03f;
	const float v_supply 	= 5.0f;

	// assuming some random value for reference series res - doesn't come built in:
	const float r_ref 	= 1000.0f;
	const float r_therm = (volts * r_ref) / (v_supply - volts);

	float temp_inv_K 	=  (1 / temp_ref) + (1 / beta) * log(r_therm / r_25);
	float temp_degC 	=  (1 / temp_inv_K) - 273.15f;
	return temp_degC;
}


/**
 * @brief Sensor initialization function.
* @param SensorInputs_t *si, ADC_HandleTypeDef *adc, TIM_HandleTypeDef *tim
* @return HAL_StatusTypeDef
* @note Auto-gen: fill details.
*/
HAL_StatusTypeDef Sensors_Init(SensorInputs_t *si, ADC_HandleTypeDef *adc, TIM_HandleTypeDef *tim) {

	if (!si || !adc || !tim) return HAL_ERROR;

	memset((void*)si->adc_raw, 0, sizeof(si->adc_raw));
	memset((void*)si->adc_V, 0, sizeof(si->adc_V));
	memset((void*)si->temp_c, 0, sizeof(si->temp_c));
	memset((void*)si->pressure_psi, 0, sizeof(si->pressure_psi));

	si->adc = adc;
	si->tim = tim;

	si->ch1_duty_cycle = 0.0f;
	si->ch2_duty_cycle = 0.0f;
	si->ch3_duty_cycle = 0.0f;

	return HAL_OK;
}

/**
 * @brief Checks ADC DMA status and the status of all 3 PWM channels.
* @param SensorInputs_t *si
* @return HAL_StatusTypeDef
* @note Auto-gen: fill details.
*/
HAL_StatusTypeDef Sensors_Start(SensorInputs_t *si) {
	if (!si || !si->adc || !si->tim) return HAL_ERROR;


	if (HAL_ADC_Start_DMA(si->adc, (uint32_t*)si->adc_raw, ADC_CH_COUNT) != HAL_OK)
		return HAL_ERROR;

	if (HAL_TIM_PWM_Start(si->tim, TIM_CHANNEL_1) != HAL_OK) return HAL_ERROR;
	if (HAL_TIM_PWM_Start(si->tim, TIM_CHANNEL_2) != HAL_OK) return HAL_ERROR;
	if (HAL_TIM_PWM_Start(si->tim, TIM_CHANNEL_3) != HAL_OK) return HAL_ERROR;

	PWM_SetAll(si);

	return HAL_OK;
}

/**
 * @brief Performs a single ADC read and updates the adc voltage buffer (in V)
* @param SensorInputs_t *si
* @return void
* @note Auto-gen: fill details.
*/
void Update_ADC_Buffers(SensorInputs_t *si) {

	HAL_ADC_Start(si->adc);

	for (uint8_t i = 0; i < ADC_CH_COUNT; i++)
		si->adc_V[i] = (si->adc_raw[i] / (float)ADC_RESOLUTION) * 3.30f;
}

/**
 * @brief Store converted temperature readings to temperature buffer.
* @param SensorInputs_t *si
* @return void
* @note Auto-gen: fill details.
*/
void Store_Temperature_Readings(SensorInputs_t *si) {
	for (uint8_t i = 0; i < 2; i++)
		si->temp_c[i] = Voltage_To_DegC_Bosch(si->adc_V[i]);

	for (uint8_t i = 2; i < 4; i++)
		si->temp_c[i] = Voltage_To_DegC_A1325(si->adc_V[i]);
}

/**
 * @brief Store converted p readings to pressure buffer.
* @param SensorInputs_t *si
* @return void
* @note Auto-gen: fill details.
*/
void Store_Pressure_Readings(SensorInputs_t *si) {
	for (uint8_t i = 4; i < 6; i++)
		si->temp_c[i] = Voltage_To_kPa_Bosch(si->adc_V[i]);

	for (uint8_t i = 6; i < ADC_CH_COUNT; i++)
		si->temp_c[i] = Voltage_To_kPa_MIP(si->adc_V[i]);
}

/**
 * @brief Apply the struct member duty cycles to TIM1 CH1, CH2 & CH3.
* @param SensorInputs_t *si
* @return void
* @note Auto-gen: fill details.
*/
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



/**
 * @brief Read segment temperature data coming from CAN bus.
* @param SensorInputs_t *si, CAN_Driver_t *can
* @return bool
* @note Auto-gen: fill details.
*/
bool Update_Segment_Temperature_Values(SensorInputs_t *si, CAN_Driver_t *can) {

	can->rx1.StdId 	= AMS_SEG_TEMP_CAN_ID;
	can->rx1.DLC	= 8;			// TODO: Assume 8 for now, check intended CAN IDs with elec team...
	HAL_StatusTypeDef can_rx_status = CAN_Receive1(can);

	if (can_rx_status != HAL_OK)
		return false;

	// check if rx fifo is full
	if (HAL_CAN_GetRxFifoFillLevel(can->hcan1, CAN_RX_FIFO0) == 0)
	    return false;

	for (uint8_t i = 0; i < AMS_SEGMENT_COUNT; i++) {
		si->seg_temp_c[i] = (float)can->rx_data[i];
	}
	return true;
}


// TODO: Verify PULSES_PER_REVOLUTION!
const uint8_t PULSES_PER_REVOLUTION = 1;
static volatile uint32_t tach_last_ticks  = 0;
static volatile uint32_t tach_delta_ticks = 0;
static volatile uint8_t  tach_new_period  = 0;


/**
 * @brief ISR callback function used for the fan speed logic.
* @param uint16_t GPIO_Pin
* @return void
* @note Auto-gen: fill details.
*/
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

/**
 * @brief Call to update the fan speed into si->fan_rpm.
* @param SensorInputs_t *si
* @return void
* @note Auto-gen: fill details.
*/
void Update_Fan_Speed(SensorInputs_t *si) {
	if (!tach_new_period || tach_delta_ticks == 0) si->fan_rpm = 0.0f;
	const float tick_freq_hz = (float)pow(10, 6);
	float pulse_freq_hz = tick_freq_hz / (float)tach_delta_ticks;
	si->fan_rpm = (pulse_freq_hz * 60) / (float)PULSES_PER_REVOLUTION;
}


