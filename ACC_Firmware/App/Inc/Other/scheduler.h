#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "acc_config.h"
#include <stdio.h>
#include <stdint.h>
/*
 * This is basically just a lightweight scheduler that implements some basic rate-grouping.
 * Feel free to make up some arbitrary timing requirements.
 * TODO: Double check w/ electrical team if this should be adjusted.
 */

typedef void (*Task_Cb_t)(void);

typedef struct SchedulerConfig {
	Task_Cb_t callback;
	uint32_t period_ms;
	uint32_t last_run_ms;
} Task_t;

void Schedule_Tasks(Task_t *tasks, uint32_t *now, uint8_t num_tasks);

#ifdef __cplusplus
}
#endif
#endif
