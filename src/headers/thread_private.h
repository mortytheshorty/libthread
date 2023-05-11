// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef LIBTHREAD_THREAD_PRIVATE_H    // BEGIN INCLUDE GUARD
#define LIBTHREAD_THREAD_PRIVATE_H

/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/

/* #include <task_private.h>
#include <taskqueue_private.h>
#include <debug.h> */

#include <libthread_private.h>


/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/

extern __thread thread *local_thread;

/* thread local variable which says thread can pause */
extern __thread volatile int thread_on_hold_available;
/** thread contains a pointer to this variable which will be set by thread_destroy()
 * thread_worker is reading on this variable to see if it has to exit
 */
extern __thread volatile int thread_exit;

/** thread local log file handle */
extern __thread volatile FILE *thread_log;

enum thread_status_private {
    THREAD_EXIT = MAX_THREAD_STATUS + 1,   
    THREAD_RESUMED = THREAD_EXIT + 1,
    MAX_PRIVATE_THREAD_STATUS,
};

/**
 * @brief thread_t which represents the thread object
 */
typedef struct thread {
    /** pthread id to get "some" control over the thread */
    pthread_t id;
    /** taskqueue where future tasks are queued in */
    taskqueue *taskqueue;
    /** a pointer to the current executed task */
    task *current_task;
    /** threadpool pointer which will be checked on in the threadworker function */
    threadpool *tp;
    /** state of thread (will maybe replace thread::pause) */
    int status;
    /** mutex for locking the thread for manipulation */
    pthread_mutex_t lock;
    /** pointer to a thread local variable which causes the thread worker to destroy itself */
    volatile int *shutdown;
    /** last return value of thread */
    void *ret;
} thread;


/*********************************************************************************************************/
/*                                           FUNCTION DEFINITIONS                                        */
/*********************************************************************************************************/


void* thread_worker(void *worker_arg);
int thread_setStatus(thread *thread, int status);
task* thread_pull_task(thread *thread, int *err);
int thread_setThreadPool(thread *thread, threadpool *tp);

#endif