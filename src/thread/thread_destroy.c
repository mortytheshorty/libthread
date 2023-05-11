#include <libthread_private.h>


int thread_destroy(thread **hthread)
{
    int err = THREAD_OK;
    if(pthread_mutex_lock(&(*hthread)->lock) != 0) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
        err = THREAD_ERROR_PTHREAD;
    }

    *(*hthread)->shutdown = 1;
    /** added this because the shutdown condition was checked after sem_wait() in thread_worker 
     * NOTE: commented out if needed (figuring out if it is okay to put check before sem_wait())
    */
    if(sem_post(&(*hthread)->taskqueue->has_tasks) != 0) {
        debug_error("failed to notify threads taskqueue");
        err = THREAD_ERROR_PTHREAD;
    }

    if(pthread_mutex_unlock(&(*hthread)->lock) != 0) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_UNLOCK);
        err = THREAD_ERROR_PTHREAD;
    }
    *hthread = NULL;

    debug(DEBUG_DONE);
    return err;
}