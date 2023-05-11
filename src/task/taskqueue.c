// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/

#include <libthread_private.h>

/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/


/*********************************************************************************************************/
/*                                           FUNCTION DEFINITIONS                                        */
/*********************************************************************************************************/

int taskqueue_create(taskqueue **queue)
{
    taskqueue *jq = calloc(1, sizeof *jq);
    if( !jq ) {
        return TASKQUEUE_ERROR_INVAL;
    }

    if(pthread_mutex_init(&jq->lock, NULL) != 0) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_INIT);
        free(jq);
        return TASKQUEUE_ERROR_PTHREAD;
    }

    if(sem_init(&jq->has_tasks, 0, 0) != 0) {
        debug_error(DEBUG_ERROR_SEMAPHORE_INIT);
        pthread_mutex_destroy(&jq->lock);
        free(jq);
        return TASKQUEUE_ERROR_SEMAPHORE;
    }

    *queue = jq;
    debug(DEBUG_DONE);

    return TASKQUEUE_OK;
}

int taskqueue_clear(taskqueue *queue)
{
    int err = TASKQUEUE_OK;

    task *tmp = NULL;

    if(pthread_mutex_lock(&queue->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
        return TASK_ERROR_PTHREAD;
    }
    #ifdef DEBUG
    int len = 0, ctr = 0;
    len = (int) queue->len;
    #endif

    while( queue->first ) {
        tmp = queue->first;
        queue->first = queue->first->next;
        free(tmp);
        if((err = sem_post(&queue->has_tasks)) != 0) {
            debug_error(DEBUG_ERROR_SEMAPHORE_POST);
            return err;
        }
        queue->len--;
        #ifdef DEBUG
         debug("removing task %d of %d", ctr++, len);
        #endif
    }

    queue->len = 0;
    queue->first = NULL;
    queue->last  = NULL;

    if((err = sem_destroy(&queue->has_tasks)) != 0) {
        debug_error(DEBUG_ERROR_SEMAPHORE_DESTROY);
        return err;
    }

    if((err = sem_init(&queue->has_tasks, 0, 0)) != 0) {
        debug_error(DEBUG_ERROR_SEMAPHORE_INIT);
        return err;
    }

    if(pthread_mutex_unlock(&queue->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_UNLOCK);
        return TASK_ERROR_PTHREAD;
    }

    debug(DEBUG_DONE);
    return err;
}

int taskqueue_destroy(taskqueue **queue)
{
    if( !queue ) {
        return TASKQUEUE_ERROR_INVAL;
    }
    int rc = TASKQUEUE_OK;
    if((rc = taskqueue_clear(*queue)) != TASKQUEUE_OK) {
        debug_error("failed to destroy taskqueue");
        return rc;
    }
    
    if((rc = sem_destroy(&(*queue)->has_tasks)) != 0) {
        debug_error(DEBUG_ERROR_SEMAPHORE_DESTROY);
    }

    free(*queue);

    debug(DEBUG_DONE);

    return rc;
}

void insertAfter(taskqueue *queue, task *htask, task *new_task)
{
    new_task->prev = htask;
    if( !htask->next ) {
        new_task->next = NULL;
        queue->last = new_task;
    }
    else {
        new_task->next = htask->next;
        htask->next->prev = new_task;
    }

    htask->next = new_task;
}

void insertBefore(taskqueue *queue, task *htask, task *new_task)
{
    new_task->next = htask;
    if( !htask->prev) {
        new_task->prev = NULL;
        queue->first = new_task;
    }
    else {
        new_task->prev = htask->prev;
        htask->prev->next = new_task;
    }
    htask->prev = new_task;
}

void taskqueue_remove(taskqueue *queue, task *htask)
{
    if( !htask->prev ) {
        queue->first = htask->next;
    }
    else {
        htask->prev->next = htask->next;
    }

    if( !htask->next ) {
        queue->last = htask->prev;
    }
    else {
        htask->next->prev = htask->prev;
    }
}

int taskqueue_push(taskqueue *queue, task *htask)
{
    int err = TASKQUEUE_OK;
    if( !htask ) {
        debug_error(DEBUG_ERROR_INVAL);
        return TASKQUEUE_ERROR_INVAL;
    }

    if(pthread_mutex_lock(&queue->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
        return TASK_ERROR_PTHREAD;
    }
    
    if( !queue->first ) {
        queue->first = htask;
        queue->last = htask;
    }
    else {
        insertAfter(queue, queue->first, htask);
    }
    htask->status = TASK_QUEUED;
    queue->len++;

    if(sem_post(&queue->has_tasks) != 0) {
        debug_error(DEBUG_ERROR_SEMAPHORE_POST);
        return TASKQUEUE_ERROR_SEMAPHORE;
    }

    if(pthread_mutex_unlock(&queue->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_UNLOCK);
        return TASK_ERROR_PTHREAD;
    }

    debug(DEBUG_DONE);

    return err;
}

task* taskqueue_pull(taskqueue *queue, int *err)
{
    if(pthread_mutex_lock(&queue->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
        *err = TASK_ERROR_PTHREAD;
        return NULL;
    }
    task *htask = queue->first;
    *err = TASKQUEUE_OK;

    if( htask ) {
        
        if(sem_post(&queue->has_tasks) != 0) {
            debug_error(DEBUG_ERROR_SEMAPHORE_POST);
            if(pthread_mutex_unlock(&queue->lock) == EINVAL) {
                debug_error(DEBUG_ERROR_PTHREAD_MUTEX_UNLOCK);
                *err = TASK_ERROR_PTHREAD;
                return NULL;
            }
            *err = TASKQUEUE_ERROR_SEMAPHORE;
            return NULL;
        }
        queue->len--;
        queue->first = htask->next;
        debug("task found of %u remaining", queue->len);
    }

    if(pthread_mutex_unlock(&queue->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_UNLOCK);
        *err = TASK_ERROR_PTHREAD;
    }

    return htask;
}

int taskqueue_getLen(taskqueue *queue)
{
    int len = 0;
    if(pthread_mutex_lock(&queue->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
        return TASK_ERROR_PTHREAD;
    }
    len = (int) queue->len;
    if(pthread_mutex_unlock(&queue->lock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_UNLOCK);
        return TASK_ERROR_PTHREAD;
    }
    
    return (int) len;
}