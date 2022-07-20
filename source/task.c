#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc, free*/
#include <time.h> /*time, time_t*/
#include "task.h"

#define MAX(x,y) ((x>y)? (x) : (y))

struct task
{
	UID_t uid;
	action_func *func;
	void* param;
	size_t interval_in_seconds;
	time_t next_run_time;
	
};


task_t *TaskCreate(action_func *func, size_t interval_in_seconds, void *param)
{
	task_t *task = (task_t *)malloc(sizeof(task_t));
	if(NULL == task)
	{
		return NULL;
	}
	task->uid = UIDCreate();
    task->func = func;
    task->param = param;
    task->interval_in_seconds = MAX(interval_in_seconds, 1);
    TaskUpdateNextRunTime(task);
	
	return (task);
}

void TaskDestroy(task_t *task)
{
	assert(NULL != task);
	free(task);
}

int TaskRun(task_t *task)
{
    assert(NULL != task);
    return (task->func(task->param));
}

UID_t TaskGetUID(const task_t *task)
{
	assert(NULL != task);
	return (task->uid);
}

int TaskCompare(const task_t *task, UID_t uid)
{
	assert(NULL != task);
	return (UIDIsSame(task->uid, uid));
}

time_t TaskGetNextRunTime(const task_t *task)
{
	assert(NULL != task);
	return (task->next_run_time);
}

void TaskUpdateNextRunTime(task_t *task)
{
	assert(NULL != task);
	task->next_run_time = task->interval_in_seconds + time(0);
}
