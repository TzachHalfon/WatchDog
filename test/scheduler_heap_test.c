#include <stdio.h> /* printf */

#include "scheduler_heap.h"

static void TestAllFuncs();
static void TestCreate();
static void TestDestroy();
static void TestAddTask();
static void TestRemoveTask();
static void TestSizeNIsEmpty();
static void TestClear();
static void TestRunNStop();

static int PrintNum(void *param);
static int PrintSomething(void *param);
static int StopScheduler(void *param);

int main()
{
	TestAllFuncs();
	return (0);
}

static void TestAllFuncs()
{
	printf("     ~START OF TEST FUNCTION~ \n");
	TestCreate();
	TestSizeNIsEmpty();
	TestAddTask();
	TestClear();
	TestRemoveTask();
	TestRunNStop();
	TestDestroy();
	printf("      ~END OF TEST FUNCTION~ \n");
}

static void TestDestroy()
{
	printf("*Run vlg to test SortedListDestroy*\n");
}

static void TestCreate()
{
	scheduler_t *scheduler = HSchedulerCreate();

	if (NULL != scheduler)
	{
		printf("SchedulerCreate working!                             V\n");
	}
	else
	{
		printf("SchedulerCreate NOT working!                         X\n");
	}

	HSchedulerDestroy(scheduler);
}

static void TestAddTask()
{
	int num1 = 5;
	int num2 = 15;
	int size_before = -1;
	int size_after = -1;
	scheduler_t *scheduler = HSchedulerCreate();

	size_before = HSchedulerSize(scheduler);

	HSchedulerAddTask(scheduler, PrintNum, (void *)&num1, 17);
	HSchedulerAddTask(scheduler, PrintNum, (void *)&num2, 15);
	HSchedulerAddTask(scheduler, PrintSomething, (void *)&num1, 12);
	HSchedulerAddTask(scheduler, PrintSomething, (void *)&num2, 18);

	size_after = HSchedulerSize(scheduler);

	if ((0 == size_before) && (4 == size_after))
	{
		printf("SchedulerAddTask working!                            V\n");
	}
	else
	{
		printf("SchedulerAddTask NOT working!                        X\n");
	}

	HSchedulerDestroy(scheduler);
}

static void TestRemoveTask()
{
	int num1 = 5;
	int num2 = 15;
	UID_t to_remove1;
	UID_t to_remove2;
	int size_before = -1;
	int size_after = -1;
	scheduler_t *scheduler = HSchedulerCreate();

	HSchedulerAddTask(scheduler, PrintNum, (void *)&num1, 10);
	to_remove1 = HSchedulerAddTask(scheduler, PrintNum, (void *)&num2, 15);
	to_remove2 = HSchedulerAddTask(scheduler, PrintSomething, (void *)&num1, 12);
	HSchedulerAddTask(scheduler, PrintSomething, (void *)&num2, 18);

	size_before = HSchedulerSize(scheduler);

	HSchedulerRemoveTask(scheduler, to_remove1);
	HSchedulerRemoveTask(scheduler, to_remove2);

	size_after = HSchedulerSize(scheduler);

	if ((4 == size_before) && (2 == size_after))
	{
		printf("SchedulerRemoveTask working!                         V\n");
	}
	else
	{
		printf("SchedulerRemoveTask NOT working!                     X\n");
	}

	HSchedulerDestroy(scheduler);
}

static void TestSizeNIsEmpty()
{
	int num1 = 5;
	int num2 = 15;
	int empty = -1;
	int not_empty = -1;
	int size_before = -1;
	int size_after = -1;

	scheduler_t *scheduler = HSchedulerCreate();

	size_before = HSchedulerSize(scheduler);
	empty = HSchedulerIsEmpty(scheduler);

	HSchedulerAddTask(scheduler, PrintNum, (void *)&num1, 10);
	HSchedulerAddTask(scheduler, PrintNum, (void *)&num2, 15);
	HSchedulerAddTask(scheduler, PrintSomething, (void *)&num1, 12);
	HSchedulerAddTask(scheduler, PrintSomething, (void *)&num2, 18);

	size_after = HSchedulerSize(scheduler);
	not_empty = HSchedulerIsEmpty(scheduler);

	if ((0 == size_before) && (4 == size_after))
	{
		printf("SchedulerSize working!                               V\n");
	}
	else
	{
		printf("SchedulerSize NOT working!                           X\n");
	}

	if ((1 == empty) && (0 == not_empty))
	{
		printf("SchedulerIsEmpty working!                            V\n");
	}
	else
	{
		printf("SchedulerIsEmpty NOT working!                        X\n");
	}

	HSchedulerDestroy(scheduler);
}

static void TestClear()
{
	int num1 = 5;
	int num2 = 15;
	int size_before = -1;
	int size_after = -1;

	scheduler_t *scheduler = HSchedulerCreate();

	HSchedulerAddTask(scheduler, PrintNum, (void *)&num1, 10);
	HSchedulerAddTask(scheduler, PrintNum, (void *)&num2, 15);
	HSchedulerAddTask(scheduler, PrintSomething, (void *)&num1, 12);
	HSchedulerAddTask(scheduler, PrintSomething, (void *)&num2, 18);

	size_before = HSchedulerSize(scheduler);

	HSchedulerClear(scheduler);

	size_after = HSchedulerSize(scheduler);

	if ((4 == size_before) && (0 == size_after))
	{
		printf("SchedulerSize working!                               V\n");
	}
	else
	{
		printf("SchedulerSize NOT working!                           X\n");
	}

	HSchedulerDestroy(scheduler);
}

static void TestRunNStop()
{
	int num1 = 5;
	int num2 = 123;
	int status = -1;
	int status2 = -1;

	scheduler_t *scheduler = HSchedulerCreate();
	scheduler_t *scheduler2 = HSchedulerCreate();

	HSchedulerAddTask(scheduler, PrintNum, (void *)&num1, 2);
	HSchedulerAddTask(scheduler, PrintNum, (void *)&num2, 3);
	HSchedulerAddTask(scheduler, PrintSomething, (void *)&num1, 7);
	HSchedulerAddTask(scheduler, StopScheduler, (void *)scheduler, 20);

	printf("*First scheduler should stop in 20 seconds*\n");
	status = HSchedulerRun(scheduler);

	HSchedulerAddTask(scheduler2, PrintSomething, (void *)&num1, 3);

	printf("*Second scheduler should stop in 5 seconds*\n");
	status2 = HSchedulerRun(scheduler2);

	if ((2 == status) && (0 == status2))
	{
		printf("SchedulerRun & SchedulerStop working!                V\n");
	}
	else
	{
		printf("SchedulerRun & SchedulerStop NOT working!            X\n");
	}

	HSchedulerDestroy(scheduler);
	HSchedulerDestroy(scheduler2);
}

static int PrintNum(void *param)
{
	printf("I am a func that prints the num: %d\n", *(int *)param);
	return (0);
}

static int PrintSomething(void *param)
{
	(void)param;
	printf("I am a func that prints something only once\n");
	return (1);
}

static int StopScheduler(void *param)
{
	printf("Managing task, Stopping scheduler.\n");
	HSchedulerStop((scheduler_t *)param);

	return (1);
}
