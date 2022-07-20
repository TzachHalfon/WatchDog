/*============================LIBRARIES && MACROS ============================*/
#include <assert.h>  /*assert*/
#include <stdlib.h> /*malloc,free*/

#include "dlist.h"

/*========================== FUNCTION DECLARATION ============================*/
static void FreeNodes(dlist_t *list);

/*====================== STRUCT && FUNCTION DEFINITION =======================*/

struct dlist_node
{
    void *data;
    dlist_node_t *next;
    dlist_node_t *prev;
};

struct dlist
{
    dlist_node_t head;
    dlist_node_t tail;
};



dlist_t *DoublyListCreate(void)
{
	dlist_t *list = (dlist_t *)malloc(sizeof(dlist_t));
	if(NULL == list)
	{
		return (NULL);
	}
    
	list->head.next = &list->tail;
	list->head.prev = NULL;
	list->head.data = NULL;
	
	list->tail.next = NULL;
	list->tail.prev = &list->head;
	list->tail.data = NULL;

	return (list);
}



void DoublyListDestroy(dlist_t *dlist)
{
	assert(NULL != dlist);
	FreeNodes(dlist);
	free(dlist);
}



static void FreeNodes(dlist_t *list)
{
	dlist_node_t *tmp;
	dlist_node_t *node_runner;
	dlist_node_t *tail = &list->tail; 
	
	assert(NULL != list);
	
	node_runner = list->head.next;

	while(node_runner != tail)
	{
		tmp = node_runner;
		node_runner = node_runner->next;
		free(tmp);
	}	
}


dlist_iter_t DoublyListPushFront(dlist_t *dlist , void *data)
{
	assert(NULL != dlist);
	assert(NULL != data);
	
	return (DoublyListInsertBefore(dlist, DoublyListBegin(dlist), data));
}


dlist_iter_t DoublyListPushBack(dlist_t *dlist , void *data)
{
	assert(NULL != dlist);
	assert(NULL != data);
	
	return (DoublyListInsertBefore(dlist, DoublyListEnd(dlist), data));
}


void *DoublyListPopFront(dlist_t *dlist)
{
	void *data;
	assert(NULL != dlist);
	data = dlist->head.next->data;
	
	DoublyListRemove(dlist->head.next);
	return (data);
}


void *DoublyListPopBack(dlist_t *dlist)
{
	void *data;
	assert(NULL != dlist);
	data = dlist->tail.prev->data;
	
	DoublyListRemove(dlist->tail.prev);
	return (data);
}


dlist_iter_t DoublyListInsertBefore(dlist_t *dlist, dlist_iter_t place_to_insert, void *data)
{
	dlist_node_t *node_to_insert;
	
	assert(NULL != dlist);
	assert(NULL != place_to_insert);
	assert(NULL != data);
	
	node_to_insert = (dlist_node_t *)malloc(sizeof(dlist_node_t));
	if(NULL == node_to_insert)
    {
        return (&dlist->tail);
    }
    
    node_to_insert->data = data;
    node_to_insert->next = place_to_insert;
    node_to_insert->prev = place_to_insert->prev;
    place_to_insert->prev = node_to_insert;
	node_to_insert->prev->next = node_to_insert;
	
	return(node_to_insert);
}


void DoublyListRemove(dlist_iter_t iter)
{
	dlist_node_t *before_iter;
	dlist_node_t *after_iter;
	
	assert(NULL != iter);

	before_iter = iter->prev;
	after_iter  = iter->next;
	
	before_iter->next = after_iter;
	after_iter->prev  = before_iter;
	
	free(iter);
}



int DoublyListIsEmpty(const dlist_t *list)
{
	assert(NULL != list);
	return (&list->tail == list->head.next);
}



size_t DoublyListSize(const dlist_t *dlist)
{
	size_t count = 0;
	dlist_node_t *runner;
	
	assert(NULL != dlist);
	
	runner = (dlist_node_t *)dlist->head.next;
	
	while(runner->next != NULL)
	{
		runner = DoublyListIterNext(runner);
		++count;
	}
    return (count);
}



void *DoublyListGetData(const dlist_iter_t iter)
{
	assert(NULL != iter);
	return (iter->data);
}



void DoublyListSetData(const dlist_iter_t iter, void *data)
{
	assert(NULL != iter);
	iter->data = data;
}



int DoublyListIsSameIter(dlist_iter_t iter_one, dlist_iter_t iter_two)
{
	assert(NULL != iter_one);
	assert(NULL != iter_two);
	return (iter_one == iter_two);
}



dlist_iter_t DoublyListIterNext(dlist_iter_t iter)
{
	assert(NULL != iter);
	(void)iter;
	return (iter->next);
}



dlist_iter_t DoublyListIterPrev(dlist_iter_t iter)
{
	assert(NULL != iter);
	(void)iter;
	return (iter->prev);
}



dlist_iter_t DoublyListBegin(const dlist_t *dlist)
{
	assert(NULL != dlist);
	return (DoublyListIterNext((dlist_iter_t)&dlist->head));
}



dlist_iter_t DoublyListEnd(const  dlist_t *dlist)
{
	assert(NULL != dlist);

	return ((dlist_iter_t)&dlist->tail);
}



int DoublyListForEach(dlist_iter_t curr, dlist_iter_t to,\
						dlist_action_t action_func, void *param)
{
	int status = 0;

	assert(NULL != curr);
	assert(NULL != to);
	assert(NULL != param);
	assert(NULL != action_func);
	
	while(!DoublyListIsSameIter(curr, to))
	{
		status = action_func(curr->data,param);
		if(1 == status)
		{
			return (status);
		}
		curr = DoublyListIterNext(curr);
	}
	return (status);
}



dlist_iter_t DoublyListFind(dlist_iter_t curr, dlist_iter_t to,\
								is_match_t FindMatch, const void *param)
{
	int found = 0;

	assert(NULL != curr);
	assert(NULL != to);
	assert(NULL != param);
	assert(NULL != FindMatch);	
	

	while(!DoublyListIsSameIter(curr, to))
	{
		found = FindMatch(curr->data, param);
		if(1 == found)
		{
			return (curr);
		}
		curr = DoublyListIterNext(curr);
	}
	return (to);
}

 
 
size_t DoublyListMultiFind(dlist_iter_t curr, dlist_iter_t to, is_match_t FindMatch,\
							const void *param, dlist_t *output_list)
{
	int found = 0;
	size_t count = 0;

	assert(NULL != curr);
	assert(NULL != to);
	assert(NULL != param);
	assert(NULL != FindMatch);	
	
	
	while(!DoublyListIsSameIter(curr, to))
	{
		found = FindMatch(curr->data, param);
		if(1 == found)
		{
			DoublyListPushFront(output_list, curr->data);
			++count;
		}
		curr = DoublyListIterNext(curr);
	}
	return (count);
}



dlist_iter_t DoublyListSplice(dlist_iter_t iter, dlist_iter_t from, dlist_iter_t to)
{
	dlist_node_t *before_to = to->prev;
	dlist_node_t *before_iter = iter->prev;
	
	assert(NULL != iter);
	assert(NULL != to);
	assert(NULL != from);
	
	to->prev = from->prev;
	from->prev->next = to;
	
	from->prev = before_iter;
	before_iter->next = from;
	
	before_to->next = iter;
	iter->prev = before_to;
	
	return (from);
}






