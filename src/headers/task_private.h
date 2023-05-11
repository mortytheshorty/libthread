// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef LIBTHREAD_TASK_PRIVATE_H    // BEGIN INCLUDE GUARD
#define LIBTHREAD_TASK_PRIVATE_H

/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/

/* #include <thread_private.h>
#include <debug.h>
 */

#include <libthread_private.h>


/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/

typedef struct task {
    /** pointer to the previous task_t in queue */
    struct task *prev;
    /** pointer to the next task_t in queue */
    struct task *next;
    /** name of the task */
    const char *task_name;
    /** function pointer for thread function */
    /** used by:  task_create(), task_execute()*/
    void* (*function)(void*);
    /** argument handed to thread_task_t::function */
    /** used by:  task_create(), task_execute()*/
    void *arg;
    /** return value of the threaded function */
    /** used by: task_setResult(), task_getResult() */
    void *retval;
    /** lock for task_t manipulation */
    /** used by: all task.c functions */
    pthread_mutex_t lock;
    /** semaphore used for notifying caller if task is ready */
    /** used by: task_execute(), task_wait()*/
    sem_t sem;
    /** task status (TASK_QUEUED, TASK_RUNNING, TASK_FINISHED) */
    /** used by: taskqueue_push(), task_execute()*/
    int status;
    /** the library provied a function to update the progress */
    double progress;
    /** pthread id pointer for ability to pause and resume task */
    /** used by: task_pause(), task_resume(), task_execute() */
    thread *parent_thread;
} task;

enum task_error {
    TASK_OK = 0,
};

/*********************************************************************************************************/
/*                                           FUNCTION DEFINITIONS                                        */
/*********************************************************************************************************/

int task_lock(task *task);
int task_unlock(task *task);
int task_notify(task *task);
int task_execute(task *htask, thread *parent_thread);

/* must be only called from internal thread functions */
int task_setResult(task *task, void *result);

#endif


