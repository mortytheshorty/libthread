#include <libthread_private.h>

/* this function waits until semaphore gets posted to */
int task_wait(task *htask)
{
    int err = TASK_OK;

    /* nothing to wait for, no error */
    if(task_getStatus(htask) == TASK_COMPLETED) {
        return 0;
    }

    debug("waiting on task to finish");

    if(sem_wait(&htask->sem) != 0) {
        debug_error("failed to wait on semaphore");
        err = TASK_ERROR_SEMAPHORE;
    }

    return err;
}