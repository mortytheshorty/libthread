// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef LIBTHREAD_THREAD_H    // BEGIN INCLUDE GUARD
#define LIBTHREAD_THREAD_H

/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/

#include <task.h>
#include <thread_error.h>

/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/

typedef struct thread thread;

enum thread_status {
    /** THREAD_STATE_MIN is intalally defined in thread_internal.h */
    //THREAD_STATE_MIN = -1,
    /** thread has no jobs in jobqueue */
    THREAD_WAITING = 0,
    /** thread is paused */
    THREAD_PAUSED = 1,
    /** thread is executing a job */
    THREAD_RUNNING = 2,
    /** thread has jobs in queue and will imediately execute the next
     * NOTE: to get this state returned the thread_getState() call is timed between
     * the call to thread_pull_job() and its execution so the chance is very low
     * to get this state returned. use thread_getPendingJobs() to get the number of pending jobs
    */
    THREAD_LOADED = 3,
    MAX_THREAD_STATUS,    
};

/*********************************************************************************************************/
/*                                           FUNCTION DEFINITIONS                                        */
/*********************************************************************************************************/

/**
 * @brief Creates a thread
 * NOTE: such a thread can be part of a threadpool which will be implemented later
 * @param thread 
 * @return int 
 */
int thread_create(thread **thread);

/**
 * @brief Destroys a thread (it will finish its tasks in background an destroys itself)
 * 
 * @param thread 
 * @retval 0 on success
 * @retval n on failure
 */
int thread_destroy(thread **thread);

/**
 * @brief Adds a function and its argument to the threads jobqueue
 * 
 * @param thread 
 * @param err 
 * @param function 
 * @param arg 
 * @return task_t* 
 */

/**
 * @brief Adds a task to thread
 * 
 * @param thread 
 * @param task 
 * @return int 
 */
int thread_addtask(thread *thread, task *task);

/**
 * @brief Returns the number of pending jobs
 * 
 * @param thread 
 * @return int 
 */
int thread_getPending(thread *thread);

/**
 * @brief Returns current state of thread
 * NOTE: possible values are:
 * 
 * enum thread_state {
 *  THREAD_WAITING = 0,
 *  THREAD_PAUSE = 1,
 *  THREAD_RESUME = 2,
 *  THREAD_RUNNING = 3,
 * };
 * 
 * @param thread 
 * @return int 
 */
int thread_getStatus(thread *thread);

/**
 * @brief Pauses the thread
 * 
 * @param thread 
 * @return int 
 */
int thread_pause(thread *thread);

/**
 * @brief Resumes the thread
 * 
 * @param thread
 * @retval 0 on success
 */
int thread_resume(thread *thread);

/**
 * @brief Returns the string representation of the thread error code
 * 
 * @param err 
 * @return const char* 
 */
const char *thread_strerror(int err);

/**
 * @brief Prints a msg with the string representation of thread error code to stderr
 * 
 * @param msg 
 * @param err 
 * @return int 
 */
int thread_perror(const char *msg, int err);

#endif