#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include "sortedlist.h"
#include "priorityq.h"

struct priority_q
{
	sortedlist_t *list;
};



priority_q_t *PriorityQCreate(priority_q_cmp_t func)
{
	priority_q_t *queue = (priority_q_t *)malloc(sizeof(priority_q_t));
	if(NULL == queue)
	{
		return NULL;
	}
	
	queue->list = SortedListCreate(func);
	if(NULL == queue->list)
	{
		free(queue);
		return NULL;
	}
	
	return queue;
}


void PriorityQDestroy(priority_q_t *queue)
{
	assert(NULL != queue);
	SortedListDestroy(queue->list);
	queue->list = NULL;
	free(queue);
}


void PriorityQClear(priority_q_t *queue)
{
	sortedlist_iter_t runner;
	sortedlist_iter_t next_runner;
	
	assert(NULL != queue);
	
	runner = SortedListBegin(queue->list);
	next_runner = SortedListIterNext(runner);
	
	while(!SortedListIsSameIter(runner, SortedListEnd(queue->list)))
	{
		SortedListRemove(runner);
		runner = next_runner;
		next_runner = SortedListIterNext(runner);
	}
}


void *PriorityQErase(priority_q_t *queue, priority_q_is_match_t is_match, const void *param)
{
	sortedlist_iter_t runner;
	void *data;
	
	assert(NULL != queue);

	runner = SortedListFindIf(SortedListBegin(queue->list), SortedListEnd(queue->list), is_match, param);
	
	data = SortedListGetData(runner);
	
	if(!SortedListIsSameIter(SortedListEnd(queue->list), runner))
	{
		SortedListRemove(runner);		
	}
	
	return data;
}


int PriorityQEnqueue(priority_q_t *queue, void *data)
{
	assert(NULL != queue);
	return !SortedListIsSameIter(SortedListEnd(queue->list), SortedListInsert(queue->list, data));
}


void *PriorityQDequeue(priority_q_t *queue)
{
	assert(NULL != queue);
	return SortedListPopBack(queue->list);
}


void *PriorityQPeek(const priority_q_t *queue)
{
	assert(NULL != queue);
	return SortedListGetData(SortedListIterPrev(SortedListEnd(queue->list))); 
}


int PriorityQIsEmpty(const priority_q_t *queue)
{
	assert(NULL != queue);
	return SortedListIsEmpty(queue->list);
}

size_t PriorityQSize(const priority_q_t *queue)
{
	assert(NULL != queue);
	return SortedListSize(queue->list);
}












