// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

/*********************************************************************************************************/
/*                                               DESCRIPION                                              */
/*********************************************************************************************************/
/*                                                                                                       */
/* This file contains code which describes the functionality of the thread type.                       */
/* See. thread.h for the interface to thread                                                           */
/*                                                                                                       */
/*********************************************************************************************************/

/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/

#define _XOPEN_SOURCE 700

#include <libthread_private.h>

/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/



/*********************************************************************************************************/
/*                                           FUNCTION DEFINITIONS                                        */
/*********************************************************************************************************/

int thread_addtask(thread *hthread, task *htask)
{   
    int err = THREAD_OK;
    
    if( !hthread ) {
        err = THREAD_ERROR_INVAL;
        debug_error("%s", thread_strerror(err));
        return err;
    }

    if( !htask ) {
        debug_error("failed to create task");
        return err;
    }

    if((err = taskqueue_push(hthread->taskqueue, htask)) != TASKQUEUE_OK) {
        debug_error("failed to add task to thread worker");
        free(htask);
        return err;
    }

    debug("task added to queue");
    return err;
}

int thread_getPending(thread *hthread)
{
    return taskqueue_getLen(hthread->taskqueue);
}

int thread_getStatus(thread *hthread)
{
    int status = -1;
    if(pthread_mutex_lock(&hthread->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
        return TASK_ERROR_PTHREAD;
    }
    status = hthread->status;
    if(pthread_mutex_unlock(&hthread->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_UNLOCK);
        return THREAD_ERROR_PTHREAD;
    }
    return status;
}


int thread_setThreadPool(thread *hthread, threadpool *tp)
{
    if(pthread_mutex_lock(&hthread->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
        return THREAD_ERROR_PTHREAD;
    }
    hthread->tp = tp;
    if(pthread_mutex_unlock(&hthread->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
        return THREAD_ERROR_PTHREAD;
    }
    return THREAD_OK;
}

int thread_perror(const char *msg, int err)
{
    if(err) {
        fprintf(stderr, "thread_error: %s %s (%d)", msg, thread_strerror(err), err);
        return err;
    }

    return THREAD_OK;
}



int thread_setStatus(thread *hthread, int status)
{
    if(status > -1 && status < MAX_PRIVATE_THREAD_STATUS) {
        if(pthread_mutex_lock(&hthread->lock) == EINVAL) {
            debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
            return THREAD_ERROR_PTHREAD;

        }
        hthread->status = status;
        if(pthread_mutex_unlock(&hthread->lock) == EINVAL) {
            debug_error(DEBUG_ERROR_PTHREAD_MUTEX_UNLOCK);
            return THREAD_ERROR_PTHREAD;
        }       
        debug(DEBUG_DONE);
        return THREAD_OK;
    }
    debug_error(DEBUG_ERROR_INVAL);
    return THREAD_ERROR_INVAL;
}


const char *thread_strerror(int err)
{
    return thread_error_strings[err];
}


const char *thread_strstatus(int status)
{
    if(status < 0 && status > MAX_THREAD_STATUS) {
        return "NULL";
    }
    return thread_status_string[status];
}

task* thread_pull_task(thread *hthread, int *err)
{
    if(pthread_mutex_lock(&hthread->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
        *err = TASK_ERROR_PTHREAD;
        return NULL;
    }

    task *htask = taskqueue_pull(hthread->taskqueue, err);
    if( *err ) {
        return NULL;
    }

    int len = taskqueue_getLen(hthread->taskqueue);
    if( len < 0 ) {
        return NULL;
    }
    /* if number of task is bigger than 0 status is THREAD_LOADED otherwise thread is THREAD_WAITING */
    if(len > 0) {
        hthread->status = THREAD_LOADED;
    } else {
        hthread->status = THREAD_WAITING;
    }

    if(pthread_mutex_unlock(&hthread->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
        *err = TASK_ERROR_PTHREAD;
    }

    debug(DEBUG_DONE);

    return htask;
}