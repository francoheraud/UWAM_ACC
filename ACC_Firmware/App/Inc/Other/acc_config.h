// AUTO-GENERATED FILE. DO NOT EDIT.
// Generated from acc_config.yaml

#pragma once

// Project: UWA Motorsports Accumulator Cooling Control
// Version: 1.0.0
// Author:  Franco Heraud
// Contributors: 

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "main.h"

#define ADC_CH_COUNT              8u
#define ACC_ADC_RES_BITS          12
#define ACC_ADC_VREF_VOLTS        3.3f
#define ACC_TIMER_TICK_HZ         1000000u
#define ACC_FAN_PULSES_PER_REV    1

// Status LEDs
#define STATUS_LED1                (uint16_t)0x0040
#define STATUS_LED2                (uint16_t)0x0080
#define STATUS_LED3                (uint16_t)0x0800
#define STATUS_LED4                (uint16_t)0x1000
#define STATUS_LED5                (uint16_t)0x2000

#define ACC_PWM_RESOLUTION        255u

// CAN IDs
#define ACC_CAN_ID_BASE           0x440u
#define ACC_CAN_ID_TEMP           0x441u
#define ACC_CAN_ID_PRESSURE       0x442u
#define ACC_CAN_ID_SEG_TEMP       0x443u
#define ACC_CAN_ID_TACH           0x444u
#define ACC_CAN_ID_POWER          0x445u

// Pressure sensors
#define ACC_PRESSURE_SENSOR_COUNT 4u

#define ACC_PRESS_PIN1_ADC_CH         1u
#define ACC_PRESS_PIN2_ADC_CH         2u
#define ACC_PRESS_PIN3_ADC_CH         3u
#define ACC_PRESS_PIN4_ADC_CH         4u
// Temperature sensors
#define ACC_TEMP_SENSOR_COUNT     4u

#define ACC_TEMP_TIN1_ADC_CH         5u
#define ACC_TEMP_TIN2_ADC_CH         6u
#define ACC_TEMP_TIN3_ADC_CH         7u
#define ACC_TEMP_TIN4_ADC_CH         8u

// Control thresholds
#define ACC_TEMP_THRESHOLD1_DEGC  40.0f
#define ACC_TEMP_THRESHOLD2_DEGC  60.0f
#define ACC_TEMP_HYSTERESIS_DEGC  3.0f

#define ACC_NUM_SEG_TEMPS         4u

// End
