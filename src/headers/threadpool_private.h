// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef LIBTHREAD_THREADPOOL_PRIVATE_H    // BEGIN INCLUDE GUARD
#define LIBTHREAD_THREADPOOL_PRIVATE_H

/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/

#include <libthread_private.h>

/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/
typedef struct threadpool {
    /** lock for locking the threadpool for manipulation */
    pthread_mutex_t tplock;
    /** number of thread workers doing nothing */
    int n_threads_idle;
    /** number of thread workers running a function */
    int n_threads_running;
    /** number of threads in threadpool */
    int n_threads;
    /** threads from 'thread.h' */
    thread **threads;
    /** taskqueue containing tasks to do */
    taskqueue *taskqueue;
} threadpool;


/*********************************************************************************************************/
/*                                           FUNCTION DEFINITIONS                                        */
/*********************************************************************************************************/

int threadpool_thread_running(threadpool *tp);
int threadpool_thread_idle(threadpool *tp);

#endif