#include <assert.h> /*assert*/
#include <stdlib.h>/*malloc, free*/
#include "scheduler.h"

#define STOP 0
#define RUN 1

static int CmpUID(const void *first_task, const void *uid);
static int CmpTime(const void *first_task, const void *second_task);


struct scheduler
{
	priority_q_t *queue;
	int run_flag;
};


scheduler_t *SchedulerCreate(void)
{
	scheduler_t *scheduler = (scheduler_t *)malloc(sizeof(scheduler_t));
	if(NULL == scheduler)
	{
		return (NULL);
	}
	
	scheduler->queue = PriorityQCreate(CmpTime);
	if(NULL == scheduler->queue)
	{
		free(scheduler);	
		return (NULL);
	}
	
	scheduler->run_flag = STOP;	
	
	return (scheduler);
}


void SchedulerDestroy(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	
	SchedulerClear(scheduler);
	
	PriorityQDestroy(scheduler->queue);
	scheduler->queue = NULL;
	free(scheduler);
}

UID_t SchedulerAddTask(scheduler_t *scheduler, action_func *func, void *param, size_t interval)
{
	task_t *new_task;
	int is_pushed = 0;
	
	assert(NULL != scheduler);
	assert(NULL != func);
	
	new_task = TaskCreate(func, interval, param);
	if(NULL == new_task)
	{
		return (badUID);
	}
	
	is_pushed = PriorityQEnqueue(scheduler->queue,(void *)new_task);	
	if(0 == is_pushed)
	{
		TaskDestroy(new_task);
		return (badUID);
	}
	
	return (TaskGetUID(new_task));
}


int SchedulerRemoveTask(scheduler_t *scheduler, UID_t uid)
{
	void *data;
	assert(NULL != scheduler);
	
	data = PriorityQErase(scheduler->queue, CmpUID, (void *)&uid);
	
	if((NULL != data))
	{
		TaskDestroy(data);
	}
	
	return ((NULL != data));
}


void SchedulerClear(scheduler_t *scheduler)
{
	task_t *curr_task;
	
	assert(NULL != scheduler);

	while(!SchedulerIsEmpty(scheduler))
	{
		curr_task = (task_t *)PriorityQDequeue(scheduler->queue);
		TaskDestroy(curr_task);
	}
}


int SchedulerRun(scheduler_t *scheduler)
{
	task_t *curr_task;
	time_t delta_time;
	int return_val;
	assert(NULL != scheduler);

	scheduler->run_flag = RUN;
	
	while(scheduler->run_flag && !SchedulerIsEmpty(scheduler))
	{
		curr_task = (task_t *)PriorityQDequeue(scheduler->queue);
		delta_time = TaskGetNextRunTime(curr_task) - time(0);
		
		while(delta_time > 0)
		{
			sleep(1);
			--delta_time;
		}
		
		return_val = TaskRun(curr_task);
		
		if(0 == return_val)
		{
			TaskUpdateNextRunTime(curr_task);
			if(0 == PriorityQEnqueue(scheduler->queue, (void *)curr_task))
			{
				TaskDestroy(curr_task);
				break;
			}
		}
		else
		{
			TaskDestroy(curr_task);
		}
	}
	
	if(STOP == scheduler->run_flag)
	{
		return (stop_run);
	}
	else if(SchedulerIsEmpty(scheduler))
	{
		return (success);		
	}
	
	return (fail);
}


void SchedulerStop(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	scheduler->run_flag = STOP;
}


size_t SchedulerSize(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	return PriorityQSize(scheduler->queue);
}


int SchedulerIsEmpty(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	return PriorityQIsEmpty(scheduler->queue);
}



static int CmpUID(const void *first_task, const void *uid)
{
	return (TaskCompare((const task_t *)first_task, *(UID_t *)uid));
}			


static int CmpTime(const void *first_task, const void *second_task)
{
	return (TaskGetNextRunTime((task_t*)second_task) - TaskGetNextRunTime((task_t*)first_task));
}

