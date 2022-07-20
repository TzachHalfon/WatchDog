/*
    team: OL125-126    
    version: 1
    Date: 10.05.2022
*/
#ifndef __SORTEDLIST_H__
#define __SORTEDLIST_H__

#include <stddef.h> /* size_t */
#include "dlist.h"

typedef struct sortedlist sortedlist_t;
typedef struct sortedlist_iter sortedlist_iter_t;
	
typedef int (*compare_t)(const void *data1, const void *data2);
typedef int (*sortedlist_action_t)(void *data, void *param);

struct sortedlist_iter
{
    dlist_iter_t iter;
    #ifndef NDEBUG 
    sortedlist_t *sortedlist;
    #endif
};
/*
 * DESCRIPTION:
 * Function creates an empty Sortedd List.
 *
 * PARAMS:
 * func - compare function needs to return 1 if data1 is bigger then data 2
 * else compare will return 0 
 *         
 * RETURN:
 * Returns a pointer to the Sortedd List
 *
 * COMPLEXITY:
 * time: best - O(1), worst - indeterminable
 * space: O(1)
 */
sortedlist_t *SortedListCreate(compare_t func);

/* DESCRIPTION:
 * Function destroys and performs cleanup on the given Sortedd List.
 * passing an invalid head pointer would result in undefined behaviour
 *
 * PARAMS:
 * sortedlist - pointer to the sorted list to be destroyed
 *         
 * RETURN:
 * void
 *
 * COMPLEXITY:
 * time: O(n)
 * space: O(1)
 */
void SortedListDestroy(sortedlist_t *sortedlist);

/* DESCRIPTION:
 * Function returns an iterator to the beginning of the Sortedd List.
 * passing an empty Sortedd List would result in undefined behaviour.
 * 
 * PARAMS:
 * list         - pointer to start of the list 
 *      
 * RETURN:
 * iterator to the start of the list.
 * 
 * COMPLEXITY:
 * time: O(1)
 * space: O(1)
 */
sortedlist_iter_t SortedListBegin(sortedlist_t *sortedlist);

/* DESCRIPTION:
 * Function returns an iterator to the end of the Sortedd List.
 * passing an empty list would result in undefined behaviour.
 * 
 * PARAMS:
 * sortedlist         - pointer to start of the Sortedd List. 
 *      
 * RETURN:
 * iterator to the end of the Sortedd List.
 * 
 * COMPLEXITY:
 * time: O(1)
 * space: O(1)
 */
sortedlist_iter_t SortedListEnd(sortedlist_t *sortedlist);

/* DESCRIPTION:
 * Function checks whether the given sorted list is empty
 * passing an invalid list pointer would result in undefined behaviour
 *
 * PARAMS:
 * sortedlist - pointer to the sorted list to be checked
 *         
 * RETURN:
 * 1 if empty, 0 otherwise
 *
 * COMPLEXITY:
 * time: O(1)
 * space: O(1)
 */
int SortedListIsEmpty(const sortedlist_t *list);

/* DESCRIPTION:
 * Function returns the data of the given element after inserting it
 * in place
 * insert will be decided by compare func of the list (see create)
 * passing an invalid iterator would result in undefined behaviour.
 * passing an invalid data would result in undefined behaviour.
 *
 * PARAMS:
 * place_to_insert - iterator position in the  Double list to enter before
 * data - the data to insert
 *      
 * RETURN:                                                          
 * an iterator to the data that has been inserted if not returns the given
 * iterator again (place_to_insert).
 * 
 * COMPLEXITY:
 * time: O(1) 
 * space: O(1)
 */
sortedlist_iter_t SortedListInsert(sortedlist_t *sortedlist, void *data);

/* DESCRIPTION:
 * Function removes the selected element from the  Sortedd List.
 * passing an invalid iterator would result in undefined behaviour.
 *
 * PARAMS:
 * iterator - selected element to remove.
 *
 * RETURN:
 * void
 *
 * COMPLEXITY: 
 * time: O(1)
 * space: O(1)
 */
sortedlist_iter_t SortedListRemove(sortedlist_iter_t iter);


/* DESCRIPTION:
 * Function gets the data in the iterator's place
 * passing an invalid iterator would result in undefined behaviour.
 *
 * PARAMS:
 * iter - iterator to the Sortedd List to access from
 *         
 * RETURN:
 * Returns a pointer to the data that has been accessed
 *
 * COMPLEXITY:
 * time: O(1)
 * space: O(1)
 */
 void *SortedListGetData(sortedlist_iter_t iter); 
 
 /* DESCRIPTION:
 * Function sets the data in the iterator's place
 * passing an invalid iterator would result in undefined behaviour.
 *
 * PARAMS:
 * iter - iterator to the double list to access from
 *         
 * RETURN:
 * Returns void
 *
 * COMPLEXITY:
 * time: O(1)
 * space: O(1)
 */
 void SortedListSetData(sortedlist_iter_t iter, void *data);

/* DESCRIPTION:
 * Function returns the next element in the Sortedd List.
 * passing an invalid iterator would result in undefined behaviour.
 * passing the last iterator would result in undefined behaviour.
 * 
 * PARAMS:
 * iter - iterator of the part of the Sortedd List to get the next from.
 * 
 * RETURN:
 * iterator to the next element.
 *
 * COMPLEXITY:
 * time: O(1)
 * space: O(1)
 */
sortedlist_iter_t SortedListIterNext(sortedlist_iter_t iter);

/* DESCRIPTION:
 * Function returns the previous element in the Sortedd List.
 * passing an invalid iterator would result in undefined behaviour.
 * passing the first iterator would result in undefined behaviour.
 * PARAMS:
 * iter - iterator of the part of the Sortedd List to get the previous of.
 *      
 * RETURN:
 * iterator to the previous element.
 *
 * COMPLEXITY:
 * time: O(1)
 * space: O(1)
 */
sortedlist_iter_t SortedListIterPrev(sortedlist_iter_t iter);

/* DESCRIPTION:
 * Function returns the number of elements on the  Sortedd List.
 * passing an invalid list would result in undefined behaviour.
 *
 * PARAMS:
 * Function removes the selected element from the  Sortedd List.
 * passing an invalid iterator would result in undefined behaviour.
 *
 * PARAMS:
 * sortedlist - head of the sortedlist to count from
 *      
 * RETURN:
 * number of elements
 *
 * COMPLEXITY:
 * time: O(n) 
 * space: O(1)
 */
size_t SortedListSize(const sortedlist_t *sortedlist);

/* DESCRIPTION:
 * Function compares between two iterators.
 * passing an invalid iterator would result in undefined behaviour.
 * 
 * PARAMS:
 * iter_one         - first iterator to compare 
 * iter_two         - second iterator to compare
 * 
 * RETURN:
 * 1 when iterators are identical, 0 otherwise.
 * 
 * COMPLEXITY:
 * time: O(1)
 * space: O(1)
 */
int SortedListIsSameIter(sortedlist_iter_t iter_one, sortedlist_iter_t iter_two);


/* DESCRIPTION:
 * Function removes the front element of the Sortedd List.
 * passing an invalid list would result in undefined behaviour.
 *
 * PARAMS:
 * sortedlist - the list to pop from
 *
 * RETURN:
 * void
 *
 * COMPLEXITY: 
 * time: O(1)
 * space: O(1)
 */
void *SortedListPopFront(sortedlist_t *sortedlist);

/* DESCRIPTION:
 * Function removes back element of the Sortedd List.
 * passing an invalid list would result in undefined behaviour.
 *
 * PARAMS:
 * sortedlist - the list to pop from
 *
 * RETURN:
 * void
 *
 * COMPLEXITY:
 * time: O(1)
 * space: O(1)
 */
void *SortedListPopBack(sortedlist_t *sortedlist);

/* DESCRIPTION:
 * Function performs an action on each element in a given range.
 * passing an invalid iterator would result in undefined behaviour.
 * passing an invalid function would result in undefined behaviour.
 * 
 * PARAMS:
 * from         - iterator to the part of the list to start from 
 * to           - iterator to the end of the iteration
 * action_func  - function pointer to perform an action on an element
 * param        - element for action function
 *      
 * RETURN:
 * last action return value
 *
 * COMPLEXITY:
 * time: O(n)
 * space: O(1)
 */
int SortedListForEach(sortedlist_iter_t from, sortedlist_iter_t to, sortedlist_action_t action_func, void *param); 

/* DESCRIPTION:
 * Function finds given nodes given a range, based on is_match function's 
 * return value.
 * passing an invalid iterator would result in undefined behaviour.
 * Trying to find an empty node will result in undefined behavior.
 *
 * PARAMS:
 * from     - iterator to the part of the Sortedd List to start from 
 * to       - iterator to the end of the iteration
 * func     - function to check if the values match
 * data     - element to find
 * 
 * RETURN:
 * iterator to the matching data,
 * if not found it will return iterator "to" inserted.
 *
 * COMPLEXITY:
 * time: O(n)
 * space: O(1)
 */
sortedlist_iter_t SortedListFindIf(sortedlist_iter_t from, sortedlist_iter_t to, is_match_t func, const void *param);

sortedlist_iter_t SortedListFind(const sortedlist_t *sortedlist, sortedlist_iter_t from, sortedlist_iter_t to, const void *data_to_find);



void SortedListMerge(sortedlist_t *sorted_list_one,sortedlist_t *sorted_list_two); 


#endif /* __SORTEDLIST_H__ */

