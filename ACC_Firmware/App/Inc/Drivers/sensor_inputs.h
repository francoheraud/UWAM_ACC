/*
* sensor_inputs.h
*
* Created on: Nov 1, 2025
* Author: Franc
*
*/



#ifndef INC_SENSOR_INPUTS_H_
#define INC_SENSOR_INPUTS_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "can_driver.h"
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <math.h>
#include "acc_config.h"



// pc8 adc no worky >:(
#define ADC_CH_COUNT 			8
#define ADC_RESOLUTION		 	4095
#define PWM_RESOLUTION			255

#define TEMP_ACC1_IN0 			0
#define TEMP_ACC2_IN1 			1
#define TEMP_DUCT1_IN2 			2
#define TEMP_DUCT2_IN3 			3

#define PRES_ACC1_IN7 			4
#define PRES_ACC2_IN8 			5
#define PRES_DUCT1_IN9 			6
#define PRES_DUCT2_INX 			99 		// placeholder for now...

#define STATUS_LED1 			GPIO_PIN_6
#define STATUS_LED2  			GPIO_PIN_7
#define STATUS_LED3 			GPIO_PIN_11
#define STATUS_LED4 			GPIO_PIN_12
#define STATUS_LED5 			GPIO_PIN_13

#define P_SENSOR_VSUP1 			4.5f 	// V
#define P_SENSOR_PMAX1 			50.0f 	// psi
#define P_SENSOR_PMIN1 			0.0f 	// psi

#define AMS_SEGMENT_COUNT		4
#define AMS_SEG_TEMP_CAN_ID		(uint16_t)0x00000000

#define TACH_TIMER_CLK_HZ		16000000 // 16 MHz


typedef struct {
	ADC_HandleTypeDef *adc;
	TIM_HandleTypeDef *tim;

	// adc dma buffers
	volatile uint16_t adc_raw[ADC_CH_COUNT];
	volatile float adc_mV[ADC_CH_COUNT];

	// data buffers for temperature and pressure values
	float temp_c[4];
	float pressure_psi[4]; // (what unit??)
	float seg_temp_c[AMS_SEGMENT_COUNT];

	// pwm
	float ch1_duty_cycle;
	float ch2_duty_cycle;
	float ch3_duty_cycle;

	// other
	float fan_rpm;


} SensorInputs_t;

HAL_StatusTypeDef Sensors_Init(SensorInputs_t *si, ADC_HandleTypeDef *adc, TIM_HandleTypeDef *tim);
HAL_StatusTypeDef Sensors_Start(SensorInputs_t *si);

void Store_Temperature_Readings(SensorInputs_t *si);
void Store_Pressure_Readings(SensorInputs_t *si);
void PWM_SetAll(SensorInputs_t *si);
void Update_Fan_Speed(SensorInputs_t *si);
void Update_ADC_Buffers(SensorInputs_t *si);


#endif /* INC_SENSOR_INPUTS_H_ */
