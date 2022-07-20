/*============================LIBRARIES && MACROS =================================*/
#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc, free*/

#include "scheduler_heap.h"
#include "pq_heap.h"

#define STOP 0
#define RUN 1

/*=========================== FUNCTION DECLARATION ================================*/
static int CmpUID(const void *first_task, const void *uid);
static int CmpTime(const void *first_task, const void *second_task);

/*================================= STRUCTS =======================================*/
struct scheduler
{
	pq_heap_t *queue;
	int run_flag;
};

/*============================= FUNCTION DEFINITION ==============================*/
scheduler_t *HSchedulerCreate(void)
{
	scheduler_t *scheduler = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL == scheduler)
	{
		return (NULL);
	}

	scheduler->queue = PQHeapCreate(CmpTime);
	if (NULL == scheduler->queue)
	{
		free(scheduler);
		return (NULL);
	}

	scheduler->run_flag = STOP;

	return (scheduler);
}

void HSchedulerDestroy(scheduler_t *scheduler)
{
	assert(NULL != scheduler);

	HSchedulerClear(scheduler);

	PQHeapQDestroy(scheduler->queue);
	scheduler->queue = NULL;
	free(scheduler);
}

UID_t HSchedulerAddTask(scheduler_t *scheduler, action_func *func, void *param, size_t interval)
{
	task_t *new_task;
	int is_pushed = 0;

	assert(NULL != scheduler);
	assert(NULL != func);

	new_task = TaskCreate(func, interval, param);
	if (NULL == new_task)
	{
		return (badUID);
	}

	is_pushed = PQHeapQEnqueue(scheduler->queue, (void *)new_task);
	if (0 == is_pushed)
	{
		TaskDestroy(new_task);
		return (badUID);
	}

	return (TaskGetUID(new_task));
}

int HSchedulerRemoveTask(scheduler_t *scheduler, UID_t uid)
{
	void *data;
	assert(NULL != scheduler);

	data = PQHeapQErase(scheduler->queue, CmpUID, (void *)&uid);

	if ((NULL != data))
	{
		TaskDestroy(data);
	}

	return ((NULL != data));
}

void HSchedulerClear(scheduler_t *scheduler)
{
	assert(NULL != scheduler);

	while (!HSchedulerIsEmpty(scheduler))
	{
		TaskDestroy(PQHeapQDequeue(scheduler->queue));
	}
}

int HSchedulerRun(scheduler_t *scheduler)
{
	task_t *curr_task;
	time_t delta_time;
	int return_val;
	assert(NULL != scheduler);

	scheduler->run_flag = RUN;

	while (scheduler->run_flag && !HSchedulerIsEmpty(scheduler))
	{
		curr_task = (task_t *)PQHeapQDequeue(scheduler->queue);
		delta_time = TaskGetNextRunTime(curr_task) - time(0);

		while (delta_time > 0)
		{
			sleep(1);
			--delta_time;
		}

		return_val = TaskRun(curr_task);

		if (0 == return_val)
		{
			TaskUpdateNextRunTime(curr_task);
			if (0 == PQHeapQEnqueue(scheduler->queue, (void *)curr_task))
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

	if (STOP == scheduler->run_flag)
	{
		return (stop_run);
	}
	else if (HSchedulerIsEmpty(scheduler))
	{
		return (success);
	}

	return (fail);
}

void HSchedulerStop(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	scheduler->run_flag = STOP;
}

size_t HSchedulerSize(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	return PQHeapQSize(scheduler->queue);
}

int HSchedulerIsEmpty(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	return PQHeapQIsEmpty(scheduler->queue);
}

static int CmpUID(const void *first_task, const void *uid)
{
	return (TaskCompare((const task_t *)first_task, *(UID_t *)uid));
}

static int CmpTime(const void *first_task, const void *second_task)
{
	return (TaskGetNextRunTime((task_t *)first_task) - TaskGetNextRunTime((task_t *)second_task));
}
