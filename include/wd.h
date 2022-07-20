/**
 * @file wd.h
 * @author Tzach Halfon
 * @brief Watch dog library
 * @version 1.0
 * @date 2022-07-16
 *
 */
#ifndef __WATCH_DOG_H__
#define __WATCH_DOG_H__

#include <stddef.h> /*size_t*/

/**
 * @brief WatchDog Protector starter on the user code
 * 		  on failure WD will revive the user with the command line arguments
 *
 * @param argv Command line arguments for the user process to be load with,
 * 			   When no need for command line arguments put NULL.
 *
 * @return VOID, but the program will have an exit code that can be
 * 		   read from the terminal
 */
void WDStart(char **argv);

/**
 * @brief WatchDog Protector exit, after calling this function thw WD will stop
 *        all memory allocated by the WD will be free.
 *
 * @param timeout Limit in seconds for the function to run,
 * 				  after that the user will get control back
 * @return void
 */
void WDStop(size_t timeout);

/**
 * @brief do not touch
 *
 */
extern int is_wd;

/**
 * @brief Exit code when the watch dog failed to run
 *
 */
typedef enum
{
	HANDLER_ERROR = 1,
	THREAD_ERROR,
	FORK_ERROR,
	SCH_ERROR,
	SEM_ERROR
} exit_status_t;

#endif /* __WATCH_DOG_H__ */