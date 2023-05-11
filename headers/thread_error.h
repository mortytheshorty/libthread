// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef LIBTHREAD_THREAD_ERROR_H    // BEGIN INCLUDE GUARD
#define LIBTHREAD_THREAD_ERROR_H

/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/

/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/

enum thread_errors {
    /**
     * Everything is alright (success)
     */
    THREAD_OK = 0,
    /**
     * Thread error created by stdlib.h (handle with errno)
     */
    THREAD_ERROR_NO_MEM = -1,
    /**
     * Thread error created by a pthread.h function (handle with errno)
     */
    THREAD_ERROR_PTHREAD = -2,
    /**
     * Thread received an argument that is invalid
     */
    THREAD_ERROR_INVAL = -3,
    /**
     * Thread error created by stdlib.h (check errno)
     */
    TASKQUEUE_ERROR_NO_MEM = -4,
    /**
     * Taskqueue error created by pthread.h (check errno)
     */
    TASKQUEUE_ERROR_PTHREAD = -5,
    /**
     * Semaphore error created by semaphore.h (check errno)
     */
    TASKQUEUE_ERROR_SEMAPHORE = -6,
    /**
     * Taskqueue got invalid argument
     */
    TASKQUEUE_ERROR_INVAL = -7,
    /** Task error if an invalid arguments is used */
    TASK_ERROR_INVAL = -8,
    /** Task error created by pthread.h */
    TASK_ERROR_PTHREAD = -9,
    /** Task error created by semaphore.h */
    TASK_ERROR_SEMAPHORE = -10,
    /** Task error created by thread.h */
    TASK_ERROR_THREAD = -10,
    /** Task error created by stdlib.h */
    TASK_ERROR_NO_MEM = -11,
};

extern const char *thread_error_strings[];
extern const char *thread_status_string[];

/*********************************************************************************************************/
/*                                           FUNCTION DEFINITIONS                                        */
/*********************************************************************************************************/


#endif