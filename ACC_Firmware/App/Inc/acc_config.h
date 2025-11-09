// AUTO-GENERATED FILE. DO NOT EDIT.
// Generated from acc_config.yaml

#pragma once

// Project: UWA Motorsports Accumulator Cooling Control
// Version: 1.0.0
// Author:  Franco Heraud
// Contributors: ...

#define ACC_ADC_RES_BITS      12
#define ACC_ADC_VREF_VOLTS    3.3f
#define ACC_TIMER_TICK_HZ     1000000u

#define ACC_FAN_COUNT         2
#define ACC_FAN_PULSES_PER_REV 1
#define ACC_FAN_PWM_MIN_DUTY  0.2f
#define ACC_FAN_PWM_MAX_DUTY  0.95f

// CAN IDs
#define ACC_CAN_ID_ACC_BASE       0x440u
#define ACC_CAN_ID_ACC_TEMP       0x441u
#define ACC_CAN_ID_ACC_PRESSURE   0x442u
#define ACC_CAN_ID_ACC_SEG_TEMP   0x443u
#define ACC_CAN_ID_ACC_TACH       0x444u
#define ACC_CAN_ID_ACC_POWER      0x445u

// Pressure sensors
#define ACC_PRESSURE_SENSOR_COUNT  4u

#define ACC_PRESS_P1_ADC_CH       1u
#define ACC_PRESS_P1_MIN_PSI       0f
#define ACC_PRESS_P1_MAX_PSI       100f

#define ACC_PRESS_P2_ADC_CH       2u
#define ACC_PRESS_P2_MIN_PSI       0f
#define ACC_PRESS_P2_MAX_PSI       100f

#define ACC_PRESS_P3_ADC_CH       3u
#define ACC_PRESS_P3_MIN_PSI       0f
#define ACC_PRESS_P3_MAX_PSI       100f

#define ACC_PRESS_P4_ADC_CH       4u
#define ACC_PRESS_P4_MIN_PSI       0f
#define ACC_PRESS_P4_MAX_PSI       100f

// Thermistor (Steinhart-Hart) coefficients (common)
#define ACC_TH_A   0.003354016f
#define ACC_TH_B   0.000250275f
#define ACC_TH_C   2.42945e-06f
#define ACC_TH_D   -7.3121e-08f

#define ACC_TEMP_SENSOR_COUNT       4u

#define ACC_TEMP_T1_ADC_CH       5u
#define ACC_TEMP_T2_ADC_CH       6u
#define ACC_TEMP_T3_ADC_CH       7u
#define ACC_TEMP_T4_ADC_CH       8u

// Control thresholds
#define ACC_TEMP_THRESHOLD1_DEGC   40f
#define ACC_TEMP_THRESHOLD2_DEGC   60f
#define ACC_TEMP_HYSTERESIS_DEGC   3f
