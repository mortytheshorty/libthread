// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef LIBTHREAD_task_H    // BEGIN INCLUDE GUARD
#define LIBTHREAD_task_H

/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/


/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/

typedef struct task task;

enum TaskStatus {
    /** task is inside the queue */
    TASK_QUEUED = 0,
    /** task is currently running */
    TASK_RUNNING = 1,
    /** task is done */
    TASK_COMPLETED = 2,
    /** max status value */
    TASK_STATUS_MAX,
};

/*********************************************************************************************************/
/*                                           FUNCTION DEFINITIONS                                        */
/*********************************************************************************************************/




/**
 * @brief Creates a task
 * 
 * @param name 
 * @param function 
 * @param arg 
 * @return int 
 */
int task_create(task **task, const char *name, void* (*function)(void *arg), void *arg);

/**
 * @brief Destroys a task only if it has finished (this maybe change later to cancel the task and destroy it)
 * 
 * @param task 
 * @return int 
 */
int task_destroy(task **task);

/**
 * @brief Returns the current task status (task_QUEUE, task_RUNNING, task_COMPLETED) (expands to task_CANCELED)
 * 
 * @param task 
 * @return int 
 */
int task_getStatus(task *task);

int task_setStatus(task *task, int status);

int task_setProgress(double progress);

int task_isRunnning(task *task);

/* this function waits until semaphore gets posted to */
int task_wait(task *task);

void* task_getResult(task *task, int *err);



int task_pause(task *task);
int task_resume(task *task);

#endif