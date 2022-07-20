#ifndef __TASK_H__
#define __TASK_H__

#include "UID.h"

typedef int(action_func)(void *param);

typedef struct task task_t;

/**
 * @brief Create a task
 *
 * @param func func to do when called
 * @param interval_in_seconds In what time interval to use the function
 * @param param parameter to use in the function
 * @return task_t* the new task
 */
task_t *TaskCreate(action_func *func, size_t interval_in_seconds, void *param);

/**
 * @brief free and destroy the pointer to the given task
 *
 * @param task task to be destroyed
 */
void TaskDestroy(task_t *task);

/**
 * @brief activation of the task
 *
 * @param task the task to activate
 * @return int the return value of the task
 */
int TaskRun(task_t *task);

/**
 * @brief get the uid of the task
 *
 * @param task the task to search its uid
 * @return UID_t the wanted uid
 */
UID_t TaskGetUID(const task_t *task);

/**
 * @brief compare bewtween task uid and wanted uid to check if they are the same
 *
 * @param task task to check its uid
 * @param uid uid to check against the task uid
 * @return int 1 if equal, 0 otherwise.
 */
int TaskCompare(const task_t *task, UID_t uid);

/**
 * @brief Get the next time in seconds the task should run
 * 
 * @param task Task to get its next run time
 * @return time_t time in seconds
 */
time_t TaskGetNextRunTime(const task_t *task);

void TaskUpdateNextRunTime(task_t *task);

#endif /*__TASK_H__*/
