#include <libthread_private.h>

int thread_create(thread **hthread)
{
/*  if( !_thread ) {
        return THREAD_ERROR_INVAL;
    }

    if( !(*_thread) ) {
        return THREAD_ERROR_INVAL;
    }
 */

    int err = THREAD_OK;
    thread *new_thread = malloc(sizeof *new_thread);
    if( !new_thread ) {
        debug_error(DEBUG_ERROR_NO_MEM);
        return THREAD_ERROR_NO_MEM;
    }

    taskqueue *jq = NULL;
    err = taskqueue_create(&jq);
    if( err ) {
        free(new_thread);
        return TASKQUEUE_ERROR_NO_MEM;
    }

    new_thread->taskqueue = jq;
    new_thread->tp = NULL;
    new_thread->status = THREAD_PAUSED;

    if(pthread_mutex_init(&new_thread->lock, NULL) != 0) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_INIT);
        goto THREAD_CREATE_CLEANUP;
    }

    if(pthread_create(&new_thread->id, NULL, thread_worker, new_thread) != 0) {
        debug_error(DEBUG_ERROR_PTHREAD_CREATE);
        goto THREAD_CREATE_CLEANUP;
    }


    /* assign thread pointer to users thread pointer */
    *hthread = new_thread;

    debug(DEBUG_DONE);

    return THREAD_OK;

    THREAD_CREATE_CLEANUP:

    if(pthread_mutex_destroy(&new_thread->lock) != 0) {
        debug_error("THREAD_CREATE_CLEANUP: failed to destroy mutex");
        err = THREAD_ERROR_PTHREAD;
    }

    if((err = taskqueue_destroy(&new_thread->taskqueue)) != TASKQUEUE_OK) {
        debug_error("THREAD_CREATE_CLEANUP: failed to destroy taskqueue");
    }

    debug_error("failed");
    free(new_thread);
    return err;
}
