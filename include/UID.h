/*
	team: OL125-126
	version: 1.0

*/
#ifndef __UID_H__
#define __UID_H__

#include <stddef.h>	   /* size_t */
#include <time.h>	   /* time_t */
#include <unistd.h>	   /* pid_t */
#include <sys/types.h> /* pid_t */
#include <stdatomic.h> /*atomic_size_t*/

typedef struct UID
{
	clock_t time;
	pid_t pid;
	atomic_size_t counter;
} UID_t;

extern const UID_t badUID;

/* DESCRIPTION:
 * Function creates an UID
 *
 * RETURN:
 * Returns created UID
 *
 * COMPLEXITY:
 * time: best - O(1), worst - indeterminable
 * space: O(1)
 */
UID_t UIDCreate(void);

/* DESCRIPTION:
 * Function recieves two UIDs and compares them.
 *
 * PARAMS:
 * UID1,2 - UIDs to compare.
 *
 * RETURN:
 * 1 if equal, 0 otherwise.
 *
 * COMPLEXITY:
 * time: O(1)
 * space: O(1)
 */
int UIDIsSame(UID_t UID1, UID_t UID2);

#endif /* __UID_H__ */
