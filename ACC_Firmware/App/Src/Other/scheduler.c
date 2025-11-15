/*
 * scheduler.c
 *
 *  Created on: Nov 12, 2025
 *      Author: Franc
 */


#include "Other/scheduler.h"


__weak void Scheduler_Default_Callback(void) {

}


/**
 * @brief Lightweight scheduler to implement some basic rate-grouping. Intended to be
 * used with i.e., a hardware timer or system tick variable.
 *
 * @param 	t			Task_t struct ptr
 * @param	*now		Current time variable (preferred to be in ms)
 * @paran	num_tasks	Number of tasks
 * @return	void
 *
 */
void Schedule_Tasks(Task_t *tasks, uint32_t *now, uint8_t num_tasks) {

	if ((tasks = NULL) || (num_tasks == 0)) return;

	Scheduler_Default_Callback();

	uint32_t current = *now;

	for (uint8_t i = 0; i < num_tasks; ++i) {

		Task_t *t = &tasks[i];
		uint32_t elapsed = current - t->last_run_ms;

		if (elapsed >= t->period_ms) {
			t->last_run_ms = current;
			t->callback();
		}
	}
}


/**
 * ```
 * Example Usage (Pseudocode):
 * static Task_t tasks;
 *
 * static void my_callback1(void) { } // task 1
 * static void my_callback2(void) { } // task 2
 *
 * tasks = {
 * {
 *	.callback = my_callback1(),
 *	.period_ms = 10u,
 *	.last_run_ms = 0u
 *	},
 *	{
 *	.callback = my_callback2(),
 *	.period_ms = 50u,
 *	.last_run_ms = 0u
 *	}
 * }
 *
 * uint32_t now = get_time_ms(...);
 * Schedule_Tasks(&tasks, now, 2);
 * ...
 * ...
 * ```
 */




