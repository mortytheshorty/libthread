#include <task_private.h>

int task_destroy(task **htask)
{
    int err = TASK_OK;

    if(sem_destroy(&(*htask)->sem) != 0) {
        debug_error(DEBUG_ERROR_SEMAPHORE_DESTROY);
        err = TASK_ERROR_SEMAPHORE;
    }

    if(pthread_mutex_destroy(&(*htask)->lock) != 0) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_DESTROY);
        err = TASK_ERROR_PTHREAD;
    }

    free(*htask);
    *htask = NULL;

    debug(DEBUG_DONE);
    return err;
}