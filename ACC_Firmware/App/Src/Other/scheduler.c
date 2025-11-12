/*
 * scheduler.c
 *
 *  Created on: Nov 12, 2025
 *      Author: Franc
 */


#include "Other/scheduler.h"



__weak void _10ms_TaskSet_Callback(void) {
	// do something
}
__weak void _50ms_TaskSet_Callback(void) {
	// do something
}
__weak void _100ms_TaskSet_Callback(void) {
	// do something
}

volatile uint32_t millis_prev1 = 0;
volatile uint32_t millis_prev2 = 0;
volatile uint32_t millis_prev3 = 0;
const uint32_t _10ms = 10;
const uint32_t _50ms = 50;
const uint32_t _100ms = 100;


inline void Scheduler_Init(uint32_t *millis_ptr) {
	millis_prev1 = millis_prev2 = millis_prev3 = *millis_ptr;
}


/**
 * @brief Lightweight scheduler. Intended to be used with i.e.,
 * a (scaled) hardware timer variable or system tick variable.
 *
* @param *millis_ptr 		Must be in milliseconds
* @return SchedulerState 	Enum for timing definitions.
* @note Auto-gen: fill details.
*/
void Schedule_Tasks(uint32_t *millis_ptr) {

	uint32_t now = *millis_ptr;

	while ((uint32_t)(now - millis_prev3) >= _100ms) {
		millis_prev3 += _100ms;
	    _100ms_TaskSet_Callback();
	}
	while ((uint32_t)(now - millis_prev2) >= _50ms) {
		millis_prev2 += _50ms;
	    _50ms_TaskSet_Callback();
	}
	while ((uint32_t)(now - millis_prev1) >= _10ms) {
		millis_prev1 += _10ms;
	    _10ms_TaskSet_Callback();
	}

	return;
}




