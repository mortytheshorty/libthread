// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef LIBTHREAD_TASKQUEUE_PRIVATE_H    // BEGIN INCLUDE GUARD
#define LIBTHREAD_TASKQUEUE_PRIVATE_H

/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/

/* #include <task_private.h>
#include <debug.h> */

#include <libthread_private.h>


/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/

typedef struct taskqueue {
    /** mutex to lock the taskqueue */
    pthread_mutex_t lock;
    /** first task (is the next task to execute)*/
    task *first;
    /** last task (to append to queue) */
    task *last;
    /** semaphore to listen on for new tasks and post to if job available */
    sem_t has_tasks;
    /** number of tasks in queue */
    int len;
} taskqueue;

enum taskqueue_errors {
    /**
     * Everything is alright (success)
     */
    TASKQUEUE_OK = 0,
};

/*********************************************************************************************************/
/*                                           FUNCTION DEFINITIONS                                        */
/*********************************************************************************************************/

/**
 * @brief Creates an empty task queue
 * 
 * @return int 
 */
int taskqueue_create(taskqueue **taskqueue);

/**
 * @brief Clears the taskqueue
 * 
 * @param taskqueue 
 * @return int 
 */
int taskqueue_clear(taskqueue *taskqueue);

/**
 * @brief Destroys taskqueue
 * 
 * @param taskqueue 
 * @return int 
 */
int taskqueue_destroy(taskqueue **taskqueue);

/**
 * @brief Returns the current number of tasks
 * 
 * @param taskqueue 
 * @param err 
 * @return int 
 */
int taskqueue_getLen(taskqueue *taskqueue);


/**
 * @brief Pushes a tasks function pointer and argument into the queue (FIFO)
 * 
 * @param taskqueue 
 * @param function 
 * @param arg 
 * @return int 
 */
int taskqueue_push(taskqueue *taskqueue, task *task);

/**
 * @brief Pulls out a task for the thread worker to work on if tasks available
 * 
 * @param taskqueue 
 * @param err 
 * @return Task* 
 */
task* taskqueue_pull(taskqueue *taskqueue, int *err);


/**
 * @brief Removes a task from list
 * 
 * @param taskqueue 
 * @param task 
 */
void taskqueue_remove(taskqueue *taskqueue, task *task);

#endif