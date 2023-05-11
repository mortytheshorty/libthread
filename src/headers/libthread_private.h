#ifndef LIBTHREAD_PRIVATE_H
#define LIBTHREAD_PRIVATE_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <time.h>

#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <semaphore.h>

#include <task.h>
#include <thread.h>
#include <threadpool.h>

#include <task_private.h>
#include <taskqueue_private.h>
#include <thread_private.h>
#include <threadpool_private.h>
#include <thread_debug.h>

extern const char *thread_error_strings[];
extern const char *thread_status_string[];

extern const char *DEBUG_DONE;
extern const char *DEBUG_ERROR_PTHREAD_CREATE;
extern const char *DEBUG_ERROR_PTHREAD_MUTEX_INIT;
extern const char *DEBUG_ERROR_PTHREAD_MUTEX_DESTROY;
extern const char *DEBUG_ERROR_PTHREAD_MUTEX_LOCK;
extern const char *DEBUG_ERROR_PTHREAD_MUTEX_UNLOCK;
extern const char *DEBUG_ERROR_PTHREAD_KILL;
extern const char *DEBUG_ERROR_NO_MEM;
extern const char *DEBUG_ERROR_INVAL;
extern const char *DEBUG_ERROR_SEMAPHORE_INIT;
extern const char *DEBUG_ERROR_SEMAPHORE_DESTROY;
extern const char *DEBUG_ERROR_SEMAPHORE_WAIT;
extern const char *DEBUG_ERROR_SEMAPHORE_POST;

#endif