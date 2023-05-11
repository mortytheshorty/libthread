// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/

#include <task_private.h>
#include <thread_private.h>

#include <stdlib.h>
#include <sys/prctl.h>

/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/




/*********************************************************************************************************/
/*                                           FUNCTION DEFINITIONS                                        */
/*********************************************************************************************************/

int task_lock(task *htask)
{
    if(pthread_mutex_lock(&htask->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
        return TASK_ERROR_PTHREAD;
    }

    return TASK_OK;
}

int task_unlock(task *htask)
{
    if(pthread_mutex_unlock(&htask->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_UNLOCK);
        return TASK_ERROR_PTHREAD;
    }

    return TASK_OK;
}

int task_notify(task *htask)
{
    if(sem_post(&htask->sem) != 0) {
        debug_error(DEBUG_ERROR_SEMAPHORE_POST);
        return TASK_ERROR_SEMAPHORE;
    }
    return TASK_OK;
}

int task_getStatus(task *htask)
{
    int status = 0;
    if(pthread_mutex_lock(&htask->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
        return TASK_ERROR_PTHREAD;
    }

    status = htask->status;
    if(pthread_mutex_unlock(&htask->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_UNLOCK);
        return TASK_ERROR_PTHREAD;
    }

    return status;
}

int task_setStatus(task *htask, int status)
{
    int err = TASK_OK;
    
    if(status < 0 && status >= TASK_STATUS_MAX) {
        return TASK_ERROR_INVAL;
    }

    if(pthread_mutex_lock(&htask->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
        err = TASK_ERROR_PTHREAD;
    }

    htask->status = status;
    
    if(pthread_mutex_unlock(&htask->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_UNLOCK);
        err = TASK_ERROR_PTHREAD;
    }

    return err;
}

int task_setProgress(double progress)
{
    int err = TASK_OK;

    if(pthread_mutex_lock(&local_thread->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
        err = TASK_ERROR_PTHREAD;
    }
    task *htask = local_thread->current_task;
    if(pthread_mutex_unlock(&local_thread->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_UNLOCK);
        err = TASK_ERROR_PTHREAD;
    }

    if(pthread_mutex_lock(&htask->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
        err = TASK_ERROR_PTHREAD;
    }

    htask->progress = progress;

    if(pthread_mutex_unlock(&htask->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_UNLOCK);
        err = TASK_ERROR_PTHREAD;
    }
    return err;
}

int task_isRunnning(task *htask)
{
    return task_getStatus(htask) == TASK_RUNNING;
}

void* task_getResult(task *htask, int *err)
{
    *err = TASK_OK;
    void *result = NULL;

    if(pthread_mutex_lock(&htask->lock) == EINVAL) {
        debug_error("failed to lock mutex");
        *err = TASK_ERROR_PTHREAD;
    }

    /* error < 0 not finished */
    if(htask->status != TASK_COMPLETED) {
        *err = -1;
    }

    result = htask->retval;

    if(pthread_mutex_unlock(&htask->lock) == EINVAL) {
        debug_error("failed to unlock mutex");
        *err = TASK_ERROR_PTHREAD;
    }

    return result;
}