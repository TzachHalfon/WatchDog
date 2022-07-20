#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "task.h"
#include "priorityq.h"


typedef enum return_type
{
	success = 0,
	fail,
	stop_run
	
}return_type_t;

typedef struct scheduler scheduler_t;


scheduler_t* SchedulerCreate(void);

void SchedulerDestroy(scheduler_t *scheduler);

UID_t SchedulerAddTask(scheduler_t *scheduler, action_func *func, void* param, size_t interval);

int SchedulerRemoveTask(scheduler_t *scheduler, UID_t uid);

void SchedulerClear(scheduler_t *scheduler);

int SchedulerRun(scheduler_t *scheduler);

void SchedulerStop(scheduler_t *scheduler);

size_t SchedulerSize(scheduler_t *scheduler);

int SchedulerIsEmpty(scheduler_t *scheduler);

#endif /*__SCHEDULER_H__*/



