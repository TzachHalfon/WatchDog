#include <pthread.h> /*mutex*/

#include "UID.h"

const UID_t badUID = {(clock_t)0, (pid_t)0, (size_t)0};
static atomic_size_t count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

UID_t UIDCreate(void)
{
	UID_t uid = badUID;
	pthread_mutex_lock(&lock);
	uid.time = clock();
	uid.pid = getpid();
	uid.counter = count;
	atomic_fetch_add(&count, 1);
	pthread_mutex_unlock(&lock);
	return uid;
}

int UIDIsSame(UID_t UID1, UID_t UID2)
{
	return ((UID1.time == UID2.time) && (UID1.pid == UID2.pid) && (UID1.counter == UID2.counter));
}
