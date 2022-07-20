/*
    team: OL125-126    
    version: 1.0
    Date: 09.05.2022
*/
#ifndef __DLIST_H__
#define __DLIST_H__

#include <stddef.h> /* size_t */

struct dlist;
struct dlist_node;

typedef struct dlist dlist_t;
typedef struct dlist_node dlist_node_t;
typedef dlist_node_t *dlist_iter_t;

typedef int (*is_match_t)(const void *data, const void *param);
typedef int (*dlist_action_t)(void *data, void *param);


/* DESCRIPTION:
 * Function creates an empty Doubly List.
 *
 * PARAMS:
 * none
 *         
 * RETURN:
 * Returns a pointer to the Doubly List
 *
 * COMPLEXITY:
 * time: best - O(1), worst - indeterminable
 * space: O(1)
 */
dlist_t *DoublyListCreate(void);


/* DESCRIPTION:
 * Function destroys and performs cleanup on the given Doubly List.
 * passing an invalid head pointer would result in undefined behaviour
 *
 * PARAMS:
 * Dlist - pointer to the Double list to be destroyed
 *         
 * RETURN:
 * void
 *
 * COMPLEXITY:
 * time: O(n)
 * space: O(1)
 */
void DoublyListDestroy(dlist_t *dlist);


/* DESCRIPTION:
 * Function finds given nodes given a range, based on is_match function's 
 * return value.
 * passing an invalid iterator would result in undefined behaviour.
 * Trying to find an empty node will result in undefined behavior.
 *
 * PARAMS:
 * from         - iterator to the part of the Doubly List to start from 
 * to           - iterator to the end of the iteration
 * func         - function to check if the values match
 * data         - element to find
 * 
 *         
 * RETURN:
 * iterator to the found data if not found it will return iterator "to" that inserted.
 * COMPLEXITY:
 * time: O(n)
 * space: O(1)
 */
dlist_iter_t DoublyListFind(dlist_iter_t from, dlist_iter_t to, is_match_t func, const void *param);


/* DESCRIPTION:
 * Function finds given nodes data in a given range, based on is_match function's 
 * return value.
 * passing an invalid iterator would result in undefined behaviour.
 * Trying to find an empty node will result in undefined behavior.
 *
 * PARAMS:
 * from         - iterator to the part of the  Doubly List to start from 
 * to           - iterator to the end of the iteration
 * func         - function to check if the values match
 * data         - element to find
 * 
 *         
 * RETURN:
 * number of found matching nodes.
 * COMPLEXITY:
 * time: O(n)
 * space: O(1)
 */
size_t DoublyListMultiFind(dlist_iter_t from, dlist_iter_t to, is_match_t func, const void *param, dlist_t *output_list );


/* DESCRIPTION:
 * Function removes the selected element from the  Doubly List.
 * passing an invalid iterator would result in undefined behaviour.
 *
 * PARAMS:
 * iterator - selected element to remove.
        
 * RETURN:
 * VOID
 * COMPLEXITY: 
 * time: O(1)
 * space: O(1)
 */
void DoublyListRemove(dlist_iter_t iter);

/* DESCRIPTION:
 * Function returns the data of the given element after inserting it before the given iterator.
 * passing an invalid iterator would result in undefined behaviour.
 * passing an invalid data would result in undefined behaviour.
 *
 * PARAMS:
 * place_to_insert - iterator position in the  Double list to enter before
 * data - the data to insert
 *      
 * RETURN:                                                          
 * an iterator to the data that has been inserted if not returns the given iterator again (place_to_insert).
 * COMPLEXITY:
 * time: O(1) 
 * space: O(1)
 */
dlist_iter_t DoublyListInsertBefore(dlist_t *dlist, dlist_iter_t place_to_insert, void *data);


/* DESCRIPTION:
 * Function returns the number of elements on the  Doubly List.
 * passing an invalid list would result in undefined behaviour.
 *
 * PARAMS:
 * Function removes the selected element from the  Doubly List.
 * passing an invalid iterator would result in undefined behaviour.
 *
 * PARAMS:
 * dlist - head of the dlist to count from
 *      
 * RETURN:
 * number of elements
 * COMPLEXITY:
 * time: O(n) 
 * space: O(1)
 */
size_t DoublyListSize(const dlist_t *dlist);


/* DESCRIPTION:
 * Function checks whether the given dobuly list is empty
 * passing an invalid list pointer would result in undefined behaviour
 *
 * PARAMS:
 * dlist - pointer to the dobuly list to be checked
 *         
 * RETURN:
 * 1 if empty, 0 otherwise
 *
 * COMPLEXITY:
 * time: O(1)
 * space: O(1)
 */

int DoublyListIsEmpty(const dlist_t *list);


/* DESCRIPTION:
 * Function gets the data in the iterator's place
 * passing an invalid iterator would result in undefined behaviour.
 *
 * PARAMS:
 * iter - iterator to the double list to access from
 *         
 * RETURN:
 * Returns a pointer to the data that has been accessed,
 * NULL if the list is empty
 *
 * COMPLEXITY:
 * time: O(1)
 * space: O(1)
 */
 void *DoublyListGetData(const dlist_iter_t iter);
 
 
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
 void DoublyListSetData(const dlist_iter_t iter, void *data); 


/* DESCRIPTION:
 * Function performs an action on each element in a given range.
 * passing an invalid iterator would result in undefined behaviour.
 * PARAMS:
 * from         - iterator to the part of the list to start from 
 * to           - iterator to the end of the iteration
 * action_func  - function pointer to perform an action on an element
   param        - element for action function
 *      
 * RETURN:
 * 0 if succes 1 else.
 * time: O(n)
 * space: O(1)
 */
int DoublyListForEach(dlist_iter_t from, dlist_iter_t to, dlist_action_t action_func, void *param); 


/* DESCRIPTION:
 * Function returns an iterator to the next element in the Doubly List.
 * passing an invalid iterator would result in undefined behaviour.
 * PARAMS:
 * iter         - iterator of the part of the Doubly List to get the next from.
 *      
 * RETURN:
 * iterator to the next element.
 * time: O(1)
 * space: O(1)
 */
dlist_iter_t DoublyListIterNext(dlist_iter_t iter);


/* DESCRIPTION:
 * Function returns an iterator to the previous element in the Doubly List.
 * passing an invalid iterator would result in undefined behaviour.
 * PARAMS:
 * iter         - iterator of the part of the double list to get the previous of.
 *      
 * RETURN:
 * iterator to the previous element.
 * time: O(1)
 * space: O(1)
 */
dlist_iter_t DoublyListIterPrev(dlist_iter_t iter);


/* DESCRIPTION:
 * Function returns an iterator to the beginning of the Doubly List.
 * passing an empty double list would result in undefined behaviour.
 * PARAMS:
 * list         - pointer to start of the list 
 *      
 * RETURN:
 * iterator to the start of the list.
 * 
 * time: O(1)
 * space: O(1)
 */
dlist_iter_t DoublyListBegin(const dlist_t *dlist);


/* DESCRIPTION:
 * Function returns an iterator to the end of the Doubly List.
 * passing an empty list would result in undefined behaviour.
 * PARAMS:
 * dlist         - pointer to start of the Doubly List. 
 *      
 * RETURN:
 * iterator to the end of the double list.
 * 
 * time: O(1)
 * space: O(1)
 */
dlist_iter_t DoublyListEnd(const  dlist_t *dlist);


/* DESCRIPTION:
 * Function compares between two iterators.
 * passing an invalid iterator would result in undefined behaviour.
 * 
 * PARAMS:
 * iter_one         - first iterator to compare 
 * iter_two         - second iterator to compare
 * 
 * RETURN:
 * 0 when iterators are identical, 1 otherwise.
 * 
 * time: O(1)
 * space: O(1)
 */
int DoublyListIsSameIter(dlist_iter_t iter_one, dlist_iter_t iter_two);


/* DESCRIPTION:
 * Function returns the iterator after inserting data to the front of the Doubly List.
 * passing an invalid data would result in undefined behaviour.
 *
 * PARAMS:
 * data - the data to push
 * dlist - the list to push to
 *
 * RETURN:                                                          
 * an iterator to the data that has been inserted to the front of the double list. if failed returns NULL.
 * COMPLEXITY:
 * time: O(1) 
 * space: O(1)
 */
dlist_iter_t DoublyListPushFront(dlist_t *dlist , void *data);


/* DESCRIPTION:
 * Function returns the iterator after inserting data to the end of the Doubly List.
 * passing an invalid data would result in undefined behaviour.
 *
 * PARAMS:
 * data - the data to push
 * dlist - the list to push to

 * RETURN:                                                          
 * an iterator to the data that has been inserted to the end of the Doubly List, if failed returns NULL.
 * COMPLEXITY:
 * time: O(1) 
 * space: O(1)
 */
dlist_iter_t DoublyListPushBack(dlist_t *dlist , void *data);


/* DESCRIPTION:
 * Function removes the selected element from the front of the Doubly List.
 * passing an invalid iterator would result in undefined behaviour.
 * PARAMS:
 * dlist - the list to pop from
 * RETURN:
 * void
 * COMPLEXITY: 
 * time: O(1)
 * space: O(1)
 */
void *DoublyListPopFront(dlist_t *dlist);


/* DESCRIPTION:
 * Function removes the selected element from the end of the Doubly List.
 * passing an invalid iterator would result in undefined behaviour.
 * PARAMS:
 * dlist - the list to pop from
 *
 * RETURN:
 * void
 * COMPLEXITY: 
 * time: O(1)
 * space: O(1)
 */
void *DoublyListPopBack(dlist_t *dlist);


/* DESCRIPTION:
 * Function slice a part from a doubly list and insert it infront of another list
 * passing an invalid iterator would result in undefined behaviour.
 * PARAMS:
 * iter - the itereator to insert the sliced part before
 * to/from - the begin and end part to slice
 *
 * RETURN: iterator to the head of the other list
 * 
 * COMPLEXITY: 
 * time: O(1)
 * space: O(1)
*/
dlist_iter_t DoublyListSplice(dlist_iter_t iter, dlist_iter_t from, dlist_iter_t to);

#endif /* __list_H__ */









