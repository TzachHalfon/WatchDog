/*============================LIBRARIES && MACROS ============================*/
#include <assert.h>  /*assert*/
#include <stdlib.h> /*malloc,free*/

#include "sortedlist.h"

/*==========================  FUNCTION DECLARATIONS ==========================*/

static sortedlist_iter_t DoublyToSorted(dlist_iter_t doubly_iter, const sortedlist_t *list);
static void AssertListInDebug(sortedlist_iter_t from, sortedlist_iter_t to);

/*====================== STRUCT && FUNCTION DEFINITION =======================*/

struct sortedlist
{
    dlist_t *sorted_list;
    compare_t compare;
};


sortedlist_t *SortedListCreate(compare_t func)
{
	sortedlist_t *list = (sortedlist_t *)malloc(sizeof(sortedlist_t));
	if(NULL == list)
	{
		return (NULL);
	}
	list->sorted_list = DoublyListCreate();
	
	if(NULL == list->sorted_list)
	{
		DoublyListDestroy(list->sorted_list);
		return (NULL);
	}
	
	list->compare = func;
	return (list);	
}


void SortedListDestroy(sortedlist_t *sortedlist)
{
	assert(NULL != sortedlist);
	DoublyListDestroy(sortedlist->sorted_list);
	free(sortedlist);
}

void *SortedListGetData(sortedlist_iter_t iter)
{
	assert(NULL != iter.iter);
	return (DoublyListGetData(iter.iter));
}


void SortedListSetData(sortedlist_iter_t iter, void *data)
{
	assert(NULL != data);
	assert(NULL != iter.iter);
	DoublyListSetData(iter.iter, data); 
}


sortedlist_iter_t SortedListBegin(sortedlist_t *list)
{
    sortedlist_iter_t iter;

    assert(NULL != list);
    
    iter = DoublyToSorted(DoublyListBegin(list->sorted_list), list);
    
    return (iter);
}

sortedlist_iter_t SortedListEnd(sortedlist_t *list)
{
    sortedlist_iter_t iter;

    assert(NULL != list);
    
    iter = DoublyToSorted(DoublyListEnd(list->sorted_list), list);
    
    return (iter);
}


sortedlist_iter_t SortedListIterNext(sortedlist_iter_t iter)
{
	assert(NULL != iter.iter);
	
	iter.iter = DoublyListIterNext(iter.iter); 	
	return (iter);
}


sortedlist_iter_t SortedListIterPrev(sortedlist_iter_t iter)
{
	assert(NULL != iter.iter);
	
	iter.iter = DoublyListIterPrev(iter.iter); 	
	return (iter);
}


int SortedListIsEmpty(const sortedlist_t *list)
{
	assert(NULL != list);
	return (DoublyListIsEmpty(list->sorted_list));
}


int SortedListIsSameIter(sortedlist_iter_t iter_one, sortedlist_iter_t iter_two)
{
	assert(NULL != iter_one.iter);
	assert(NULL != iter_two.iter);
	return (DoublyListIsSameIter(iter_one.iter, iter_two.iter));
}


size_t SortedListSize(const sortedlist_t *sortedlist)
{
	assert(NULL != sortedlist);
	return (DoublyListSize(sortedlist->sorted_list));
}


sortedlist_iter_t SortedListInsert(sortedlist_t *sortedlist, void *data)
{
	sortedlist_iter_t list_runner;
	
	assert(NULL != sortedlist);
	assert(NULL != data);
	
	list_runner = SortedListBegin(sortedlist);
	if(!SortedListIsEmpty(sortedlist))
	{
		while(!SortedListIsSameIter(list_runner,SortedListEnd(sortedlist)) &&
			 (0 > sortedlist->compare(SortedListGetData(list_runner),data)))
		{
			list_runner = SortedListIterNext(list_runner);
		}
	}
	list_runner.iter = DoublyListInsertBefore(sortedlist->sorted_list,list_runner.iter, data);
	
	return (list_runner);
}


sortedlist_iter_t SortedListRemove(sortedlist_iter_t iter)
{
	sortedlist_iter_t after;
	
	assert(NULL != iter.iter);
	
	after = iter;
	after = SortedListIterNext(after);
	DoublyListRemove(iter.iter);
	return (after);
}


void *SortedListPopFront(sortedlist_t *sortedlist)
{
	assert(NULL != sortedlist);
	return (DoublyListPopFront(sortedlist->sorted_list));
}


void *SortedListPopBack(sortedlist_t *sortedlist)
{
	assert(NULL != sortedlist);
	return (DoublyListPopBack(sortedlist->sorted_list));
}


int SortedListForEach(sortedlist_iter_t from, sortedlist_iter_t to, sortedlist_action_t action_func, void *param)
{
	assert(NULL != param);	
	assert(NULL != action_func);
	assert(NULL != from.iter);
	assert(NULL != to.iter);
	AssertListInDebug(from, to);
	
	return (DoublyListForEach(from.iter, to.iter, action_func, param)); 
}


sortedlist_iter_t SortedListFindIf(sortedlist_iter_t from, sortedlist_iter_t to, is_match_t func, const void *param)
{
	sortedlist_iter_t sorted;
	assert(NULL != param);	
	assert(NULL != func);
	assert(NULL != from.iter);
	assert(NULL != to.iter);
	
	sorted.iter = DoublyListFind(from.iter, to.iter, func, param);
	#ifndef NDEBUG
    sorted.sortedlist = from.sortedlist;
    #endif
	return	(sorted);
}

sortedlist_iter_t SortedListFind(const sortedlist_t *sortedlist,\
		 sortedlist_iter_t curr, sortedlist_iter_t to, const void *data_to_find)
{
	int found = 1;

	assert(NULL != curr.iter);
	assert(NULL != to.iter);
	assert(NULL != sortedlist);
	AssertListInDebug(curr,to);	
	
	while(!SortedListIsSameIter(curr, to))
	{
		found = sortedlist->compare(SortedListGetData(curr), data_to_find);
		if(0 == found)
		{
			break;
		}
		curr = SortedListIterNext(curr);
	}
	return (curr);
}


void SortedListMerge(sortedlist_t *dst, sortedlist_t *src)
{
	sortedlist_iter_t src_to_runner;
    sortedlist_iter_t src_from;
    sortedlist_iter_t dest_runner;
    
    assert(NULL != dst);
    assert(NULL != src);
    
    src_to_runner = SortedListBegin(src);
    src_from = src_to_runner;
    dest_runner = SortedListBegin(dst);
    
    while(!SortedListIsSameIter(src_from, SortedListEnd(src)))
    {
        if(SortedListIsSameIter(dest_runner, SortedListEnd(dst)))
        {
            DoublyListSplice(dest_runner.iter, src_from.iter, SortedListEnd(src).iter);
            break;
        }
        
        
        if(0 > dst->compare(SortedListGetData(dest_runner), SortedListGetData(src_from)))
        {
            dest_runner = SortedListIterNext(dest_runner);
            continue;
        }
        
        
        if(0 > dst->compare(SortedListGetData(dest_runner), SortedListGetData(src_to_runner)))
        {
			DoublyListSplice(dest_runner.iter, src_from.iter, src_to_runner.iter);
			src_from = SortedListBegin(src);
			src_to_runner = src_from;
            continue;
        }
        
        src_to_runner = SortedListIterNext(src_to_runner);
    }
}

static void AssertListInDebug(sortedlist_iter_t from, sortedlist_iter_t to)
{
	#ifndef NDEBUG 
	assert(from.sortedlist == to.sortedlist);
	#endif
	(void)from;
	(void)to;
}

static sortedlist_iter_t DoublyToSorted(dlist_iter_t doubly_iter, const sortedlist_t *list)
{
    sortedlist_iter_t iter;
    iter.iter = doubly_iter;
    #ifndef NDEBUG
    iter.sortedlist = (sortedlist_t*) list;
    #endif
    (void)doubly_iter;
	(void)list;

    return (iter);
}
