#define _XOPEN_SOURCE 700 /* struct sigaction */
/*============================LIBRARIES && MACROS =================================*/
#include <pthread.h>   /*pthread_create, join*/
#include <sys/sem.h>   /*sys v semaphore*/
#include <stdlib.h>	   /*getenv, setenv */
#include <stdio.h>	   /* fopen fclose */
#include <stdatomic.h> /*atomic_int */
#include <signal.h>	   /*sigaction */
#include <sys/types.h> /*pid_t */
#include <string.h>	   /*strlen*/
#include <sys/time.h>

#include "wd.h"
#include "scheduler.h"

typedef enum logger
{
	ERR = 1,
	INFO,
	WARN
} logger_status_t;

#define WD (1)
#define USR (0)
#define POST (1)
#define WAIT (-1)
#define RW (0666)
#define CYCLIC (0)
#define SUCCESS (0)
#define SEND_INTERVAL (1)
#define CHECK_INTERVAL (5)
#define MIN_REC_SIGNALS (1)
#define EXPECTED_SIGNALS ((CHECK_INTERVAL) / (SEND_INTERVAL))
typedef void (*handler_func)(int, siginfo_t *, void *);
/*================================= GLOBALS =======================================*/
static atomic_int sigusr1_counter = 0, sigusr2_counter = 0;
static pthread_t commun_thread;
static pid_t pid_to_send_signal;
static scheduler_t *sch;
static int sem_id;
int is_wd = USR;
/*============================= SEM FUNCTIONS =====================================*/
static int ChangeSemVal(int modify, int sem_id);
/*=========================== Scheduler FUNCTIONS =================================*/
static void *SchedulerRunWrapper(void *sch);
static int SetUpScheduler(char **argv);
/* ============================ TASK FUNCTIONS =================================== */
static int CheckSigusr1CountTask(void *argv);
static int CheckSigusr2CountTask(void *sch);
static int SignalProcess(void *unused);
/* ============================ SIGNAL HANDLERS ================================== */
static void Sigusr2Handler(int unused, siginfo_t *unused2, void *unused3);
static void Sigusr1Handler(int unused, siginfo_t *info, void *unused2);
static void CreateSignalHandler(int signal_to_recive, handler_func);
static void ReviveUser(char **argv);
static void ReviveWD(char **argv);
static void InitHandlers(void);
static void CloseSem(void);
/* ============================ ERROR HANDLERS =================================== */
static void ExitOnErrors(int got_error, int err_status, char *msg);
/* =============================== LOGGER ======================================== */
static void Logger(char *msg, int level);

/*========================== FUNCTIONS DEFINITION =================================*/
void WDStart(char **argv)
{
	sigset_t set = {0};
	InitHandlers();
	Logger("SIGUSR1 & SIGUSR2 HANDLERS GOT SET UP", INFO);
	/* add signals sigusr1 && sigusr2 to the signals set */
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);

	/* saving sem id int global, used for setup sync between processes */
	ExitOnErrors((-1 == (sem_id = semget(ftok(argv[0], 'o'), 1, RW | IPC_CREAT))), SEM_ERROR, "FAILED OPENING SEMAPHORE");
	Logger("OPENED SEMAPHORE", INFO);
	if (NULL == getenv("IS_WD_ACTIVE"))
	{ /* entering only when WDStart was called for the first time , no env var was created */
		setenv("IS_WD_ACTIVE", "1", 1);
		pid_to_send_signal = fork();
		ExitOnErrors((-1 == pid_to_send_signal), FORK_ERROR, "FAILED TO EXCUTE FORK");
		if (0 == pid_to_send_signal) /* child process becomes process for WD */
		{
			execv("./watchdog.out", argv);
		}
		else /* user process sets scheduler and waits for WD to finish setting up */
		{
			ExitOnErrors((SCH_ERROR == SetUpScheduler(argv)), SCH_ERROR, "SCHEDULER SETUP FAILED");
			ExitOnErrors((-1 == ChangeSemVal(WAIT, sem_id)), SEM_ERROR, "FAILED OPEN SEMAPHORE"); /* post will happen in line 84 */
		}
	}
	else /* entering in every call after the first one when one of the processes dies and come to be restored */
	{
		pid_to_send_signal = getppid();
		ExitOnErrors((SCH_ERROR == SetUpScheduler(argv)), SCH_ERROR, "SCHEDULER SETUP FAILED");
		ExitOnErrors((-1 == ChangeSemVal(POST, sem_id)), SEM_ERROR, "FAILED CHANGING SEMAPHORE VALUE");
	}
	/* after preperations (scheduler setup, sem value update)*/
	if (is_wd == WD)
	{
		SchedulerRunWrapper(sch);
	}
	else /*main user thread now block the whole set of signals && create the communication thread*/
	{
		Logger("USER THREAD GOT SET UP", INFO);
		ExitOnErrors((SUCCESS != pthread_create(&commun_thread, NULL, SchedulerRunWrapper, sch)), THREAD_ERROR, "FAILED TO OPEN THREAD");
		pthread_sigmask(SIG_BLOCK, &set, NULL);
		Logger("USER MASKED SIGUSR1 & SIGUSR2 SIGNALS", INFO);
	}
}

void WDStop(size_t timeout)
{
	time_t start = time(0);

	if (NULL != sch)
	{
		SchedulerStop(sch); /* stops user scheduler cause cleanup*/
	}
	CloseSem();
	do
	{
		kill(pid_to_send_signal, SIGUSR2); /* send sigusr2 to WD to stop scheduler destroy it */
										   /* keep sending signals until alteast one got back */
	} while (sigusr2_counter == 0 && (size_t)(time(NULL) - start) < (size_t)timeout);
	pthread_join(commun_thread, NULL);
	Logger("WATCH DOG FINISHED", INFO);

	/*wait for communication thread to finish sch destroy for final cleanup in case user app will finish faster*/
}

static void CreateSignalHandler(int signal_to_recive, handler_func handler)
{
	struct sigaction action = {0};
	action.sa_flags = SA_SIGINFO; /* telling sigaction we want three arguments in the handler */
	action.sa_sigaction = handler;
	/* define the signal handler */
	ExitOnErrors((0 > sigaction(signal_to_recive, &action, NULL)), HANDLER_ERROR, "FAILED TO DEFINE HANDLERS");
}

static int SignalProcess(void *unused) /* scheduler task */
{
	(void)unused;
	kill(pid_to_send_signal, SIGUSR1); /* send signal to partner */
	Logger("SIGNAL GOT SENT TO PARNTER", INFO);
	return (CYCLIC); /* making sure task will run repeatedly */
}

static void Sigusr1Handler(int unused, siginfo_t *info, void *unused2)
{
	(void)unused;
	(void)unused2;
	if (pid_to_send_signal == info->si_pid) /* making sure no other process sent the signal except the partner */
	{
		atomic_fetch_add(&sigusr1_counter, 1);
	}
}

static void Sigusr2Handler(int unused, siginfo_t *info, void *unused2)
{
	(void)unused;
	(void)unused2;
	if (pid_to_send_signal == info->si_pid) /* making sure no other process sent the signal except the partner */
	{
		kill(pid_to_send_signal, SIGUSR1); /*WD reply to user and vice versa*/
		atomic_fetch_add(&sigusr2_counter, 1);
	}
}

static int CheckSigusr1CountTask(void *argv) /* scheduler task */
{
	if (EXPECTED_SIGNALS != sigusr1_counter)
	{
		Logger("Unexpected amount of signals recieved", WARN);
	}
	if (MIN_REC_SIGNALS > sigusr1_counter) /* only enter when the partner dies meaning not enough signals arrived */
	{
		pid_to_send_signal = fork();
		ExitOnErrors((-1 == pid_to_send_signal), FORK_ERROR, "FAILED TO EXCUTE FORK");
		if (0 == pid_to_send_signal)
		{
			if (USR == is_wd) /* checking how am i to know how died... im WD user app is dead && vice versa */
			{
				ReviveWD((char **)argv);
			}
			else
			{
				ReviveUser((char **)argv);
			}
		}
		ExitOnErrors((-1 == ChangeSemVal(WAIT, sem_id)), SEM_ERROR, "FAILED CHANGING SEMAPHORE VALUE"); /* POST will happen in WDStart line 84 after partner got set up*/
	}
	atomic_fetch_sub(&sigusr1_counter, sigusr1_counter);
	return (CYCLIC);
}

static int CheckSigusr2CountTask(void *sch) /* scheduler task */
{
	if (sigusr2_counter > 0) /*arriving when user wants to stop WD*/
	{
		SchedulerStop((scheduler_t *)sch); /* stops WD || USER(will be stop sooner in WDStop) scheduler */
	}
	return (CYCLIC);
}

static void ReviveWD(char **argv)
{
	Logger("REVIVING WATCHDOG", WARN);
	execv("./watchdog.out", argv);
}

static void ReviveUser(char **argv)
{
	Logger("REVIVING USER APP", WARN);
	execv(argv[0], argv);
}

static int SetUpScheduler(char **argv)
{
	sch = SchedulerCreate();

	if (NULL == sch)
	{
		return (SCH_ERROR);
	}
	if (1 == UIDIsSame(SchedulerAddTask(sch, SignalProcess, NULL, SEND_INTERVAL), badUID))
	{
		return (SCH_ERROR);
	}
	if (1 == UIDIsSame(SchedulerAddTask(sch, CheckSigusr1CountTask, argv, CHECK_INTERVAL), badUID))
	{
		return (SCH_ERROR);
	}
	if (1 == UIDIsSame(SchedulerAddTask(sch, CheckSigusr2CountTask, sch, CHECK_INTERVAL), badUID))
	{
		return (SCH_ERROR);
	}
	Logger("SECHEDULER GOT SET UP", INFO);
	return (SUCCESS);
}

static void *SchedulerRunWrapper(void *sch)
{
	Logger("SECHEDULER STARTING TO RUN", INFO);
	SchedulerRun((scheduler_t *)sch);
	SchedulerDestroy((scheduler_t *)sch);
	Logger("SECHEDULER CLOSED", INFO);
	return (NULL);
}

static int ChangeSemVal(int inc_or_dec, int sem_id)
{
	struct sembuf action = {0};
	action.sem_num = 0;
	action.sem_op = inc_or_dec;
	return (semop(sem_id, &action, 1));
}

static void InitHandlers(void)
{
	CreateSignalHandler(SIGUSR1, Sigusr1Handler);
	CreateSignalHandler(SIGUSR2, Sigusr2Handler);
}

static void CloseSem(void)
{
	ExitOnErrors(-1 == semctl(sem_id, 0, IPC_RMID), SEM_ERROR, "FAILED CLOSING SEMAPHORE");
	Logger("SEMAPHORE CLOSED", INFO);
}

static void ExitOnErrors(int got_error, int err_status, char *msg)
{
	if (got_error == 1)
	{
		WDStop(0);
		Logger(msg, ERR);
		exit(err_status);
	}
}

static void Logger(char *msg, int level)
{
	FILE *logger;
	time_t now = time(0);
	char *who_am_i = ((is_wd == WD) ? "watch dog" : "user app ");
	char *state = (level == ERR) ? "ERROR  " : (level == INFO) ? "INFO   "
															   : "WARNING";
	struct tm *current = localtime(&now);
	logger = fopen("logger.txt", "a");
	fprintf(logger, "%s || [%2d:%2d:%2d] || %s || %s\n", state, current->tm_hour, current->tm_min, current->tm_sec, who_am_i, msg);
	fclose(logger);
}