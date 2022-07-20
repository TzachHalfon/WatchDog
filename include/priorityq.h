/*
    team: OL125-126    
    version: 1.0

*/
#ifndef __PRIORITYQ_H__
#define __PRIORITYQ_H__

#include <stddef.h> /* size_t */


typedef struct priority_q priority_q_t;

typedef int (*priority_q_cmp_t)(const void *data1, const void *data2);
typedef int (*priority_q_is_match_t)(const void *data, const void *param);


/*
typedef struct priority_q
{
	sorted_list_t *list;
}priority_q_t;
*/

/* DESCRIPTION:
 * Function creates an empty priority queue
 *
 * PARAMS:
 * compare function
 *         
 * RETURN:
 * Returns a pointer to the created priority queue
 *
 * COMPLEXITY:
 * time: best - O(1), worst - indeterminable
 * space: O(1)
 */
priority_q_t *PriorityQCreate(priority_q_cmp_t func);

/* DESCRIPTION:
 * Function destroys and performs cleanup on the given queue.
 * passing an invalid queue pointer would result in undefined behaviour
 * This function should be used when finshing using the queue.
 *
 * PARAMS:
 * queue - pointer to the queue to be destroyed
 *         
 * RETURN:
 * void
 *
 * COMPLEXITY:
 * time: O(n)
 * space: O(1)
 */
void PriorityQDestroy(priority_q_t *queue);

/* DESCRIPTION:
 * Function checks whether the queue is empty
 *
 * PARAMS:
 * queue - pointer to the queue to check if empty
 *         
 * RETURN:
 * 1 if the queue is empty or 0 otherwise
 *
 * COMPLEXITY:
 * time: O(1) 
 * space: O(1)
 */
int PriorityQIsEmpty(const priority_q_t *queue);

/* DESCRIPTION:
 * Function inserts the given data to the queue in the correct position.
 * 
 *
 * PARAMS:
 * queue - pointer to the queue to insert data into
 * data - the data to insert
 *      
 *
 * COMPLEXITY:
 * time: O(n) 
 * space: O(1)
 */
int PriorityQEnqueue(priority_q_t *queue, void *data);

/* DESCRIPTION:
 * Function removes the first element of the queue and returns it
 * trying to Dequeue an empty queue will result in undefined behavior
 *
 * PARAMS:
 * queue - pointer to the queue
 *         
 * RETURN:
 * pointer to the element.
 *
 * COMPLEXITY:
 * time: O(1) 
 * space: O(1)
 */
void *PriorityQDequeue(priority_q_t *queue);

/* DESCRIPTION:
 * Function finds element base on the return value of is_match function, removes it from queue and returns it.
 * passing an invalid queue would result in undefined behaviour.
 *
 * PARAMS:
 * func          - function to check if the values match
 * param         - paramter to be passed to is_match func
 *         
 * RETURN:
 * pointer to the element.
 *
 * COMPLEXITY:
 * time: O(n)
 * space: O(1)
 */
void *PriorityQErase(priority_q_t *queue, priority_q_is_match_t is_match, const void *param);

/* DESCRIPTION:
 * Function returns the number of elements in the queue.
 * passing an invalid queue would result in undefined behaviour.
 *
 * PARAMS:
 * queue - pointer to a queue  
 *      
 * RETURN:
 * number of elements
 * COMPLEXITY:
 * time: O(n) 
 * space: O(1)
 */
size_t PriorityQSize(const priority_q_t *queue);

/* DESCRIPTION:
 * Function gets the data in the start of the queue.
 * passing an invalid queue would result in undefined behaviour.
 * passing an empty queue would result in undefined behaviour.
 *
 * PARAMS:
 * queue - pointer to a queue to peek at
 *         
 * RETURN:
 * Returns a pointer to the data that has been accessed.
 *
 * COMPLEXITY:
 * time: O(1)
 * space: O(1)
 */
void *PriorityQPeek(const priority_q_t *queue); 
/* DESCRIPTION:
 * Function clears the quere.
 * passing an invalid queue would result in undefined behaviour.
 * passing an empty queue would result in undefined behaviour.
 *
 * PARAMS:
 * queue - pointer to a queue to clear.
 *         
 *
 * COMPLEXITY:
 * time: O(n)
 * space: O(1)
 */
void PriorityQClear(priority_q_t *queue);



#endif /* __PRIORITYQ_H__ */

