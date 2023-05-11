// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/

#include <threadpool_private.h>

/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/



/*********************************************************************************************************/
/*                                           FUNCTION DEFINITIONS                                        */
/*********************************************************************************************************/

int threadpool_create(threadpool **tp, size_t n_threads)
{
    if( *tp ) {
        return THREAD_ERROR_INVAL;
    }
    
    int err = THREAD_OK;
    threadpool *pool = calloc(1, sizeof *pool);
    if( !pool ) {
        debug_error("failed to alloacte memory for threadpool");
        return THREAD_ERROR_NO_MEM;
    }

    thread **threads = calloc((size_t) n_threads, sizeof *pool->threads);
    if( !threads ) {
        debug_error("failed to alloacte memory for threads");
        free(pool);
        return THREAD_ERROR_NO_MEM;
    }

    taskqueue *tq = NULL;
    err = taskqueue_create(&tq);
    if( err ) {
        free(pool->threads);
        free(pool);
        return err;
    }

    pool->taskqueue = tq;
    pool->threads = threads;
    pool->n_threads = n_threads;

    for(size_t i = 0; i < n_threads; i++) {
        err = thread_create(&pool->threads[i]);
        if( err ) {
            // handle error
        }

        err = thread_setThreadPool(pool->threads[i], pool);
        if( err ) {
            // handle error
        }

        pool->n_threads_idle++;
    }

    *tp = pool;

    return THREAD_OK;
}

int threadpool_addtask(threadpool *tp, task *htask)
{
    if( !tp || !htask) {
        debug_error(DEBUG_ERROR_INVAL);
        return THREAD_ERROR_INVAL;
    }

    if(pthread_mutex_lock(&tp->tplock) == EINVAL) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
        return THREAD_ERROR_PTHREAD;
    }
    int err = 0;
    if((err = taskqueue_push(tp->taskqueue, htask)) != TASKQUEUE_OK) {
        return err;
    }
    if(sem_post(&tp->taskqueue->has_tasks) != 0) {
        debug_error(DEBUG_ERROR_SEMAPHORE_POST);
        return TASKQUEUE_ERROR_SEMAPHORE;
    }

    if(pthread_mutex_unlock(&tp->tplock) != 0) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_UNLOCK);
        return THREAD_ERROR_PTHREAD;
    }

    debug(DEBUG_DONE);
    return THREAD_OK;
}

int threadpool_destroy(threadpool **tp)
{
    int err = 0;
    err = taskqueue_destroy(&(*tp)->taskqueue);

    for(int i = 0; i < (*tp)->n_threads; i++) {
        err = thread_destroy(&(*tp)->threads[i]);
        if( err ) {
            debug_error("thread_destroy() failed");
        }
    }

    free((*tp)->threads);
    free(*tp);
    *tp = NULL;

    return THREAD_OK;
}

int threadpool_thread_running(threadpool *tp)
{
    if(pthread_mutex_lock(&tp->tplock) == EINVAL) {
        debug_error("failed to lock mutex");
        return THREAD_ERROR_PTHREAD;
    }

    tp->n_threads_running += 1;
    tp->n_threads_idle    -= 1;

    if(pthread_mutex_unlock(&tp->tplock) == EINVAL) {
        debug_error("failed to lock mutex");
        return THREAD_ERROR_PTHREAD;
    }

    return THREAD_OK;
}

int threadpool_thread_idle(threadpool *tp)
{
    if(pthread_mutex_lock(&tp->tplock) == EINVAL) {
        debug_error("failed to lock mutex");
        return THREAD_ERROR_PTHREAD;
    }

    tp->n_threads_running -= 1;
    tp->n_threads_idle    += 1;

    if(pthread_mutex_unlock(&tp->tplock) == EINVAL) {
        debug_error("failed to lock mutex");
        return THREAD_ERROR_PTHREAD;
    }

    return THREAD_OK;
}

void threadpool_stats(threadpool *tp)
{
    pthread_mutex_lock(&tp->tplock);

    debug("STAT THREADS_IDLE   : %d", tp->n_threads_idle);
    debug("STAT THREADS_RUNNING: %d", tp->n_threads_running);

    pthread_mutex_unlock(&tp->tplock);
}