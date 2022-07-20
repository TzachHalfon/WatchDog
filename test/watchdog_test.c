#define _XOPEN_SOURCE 700 /* struct sigaction */
#include <stdlib.h>		  /* atoi */
#include <time.h>		  /* time */
#include <unistd.h>		  /* fork/exec */
#include <stdio.h>		  /* printf */
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "wd.h"

static void TestWatchdog(char **argv)
{
	pid_t pid;
	int unused_status;

	if (NULL == argv[1])
	{
		pid = fork();
		if (0 == pid)
		{
			argv[1] = "test";
			printf("Started\n");
			WDStart(argv);
			sleep(10);
			WDStop(3);
			printf("Finished\n");
		}
		else
		{
			sleep(10);
			kill(pid, SIGKILL);
			waitpid(pid, &unused_status, 0);
		}
	}
	else
	{
		WDStart(argv);
		printf("second time %d\n", getpid());
		WDStop(0);
		printf("Finished\n");
		return;
	}
}

int main(int argc, char **argv)
{
	(void)argc;
	printf("Starting main\n");
	TestWatchdog(argv);
	return 0;
}