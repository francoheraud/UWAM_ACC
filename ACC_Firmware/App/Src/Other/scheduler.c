/*
 * scheduler.c
 *
 *  Created on: Nov 12, 2025
 *      Author: Franc
 */


#include "Other/scheduler.h"

/**
 * @brief Lightweight scheduler. Intended to be used with i.e.,
 * a (scaled) hardware timer variable or system tick variable.
 *
* @param *timer_ptr (**Must be in milliseconds!**) -> Must also be casted to integer.
* @return SchedulerState (enum)
* @note Auto-gen: fill details.
*/
SchedulerState Schedule_Task(uint32_t *timer_ptr) {

	switch (*timer_ptr) {
	case 1:
		return EXEC_1MS;
	case 10:
		return EXEC_10MS;
	case 100:
		return EXEC_100MS;
	}

	if (*timer_ptr >= 100) *timer_ptr = 0;

}




