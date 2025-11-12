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
 * Made up some arbitrary timing requirements.
 * TODO: Double check w/ electrical team if this should be adjusted.
 */

void Scheduler_Init(uint32_t *millis_ptr);
void Schedule_Tasks(uint32_t *millis_ptr);

#ifdef __cplusplus
}
#endif
#endif
