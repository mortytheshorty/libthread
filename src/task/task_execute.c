#include <task_private.h>
#include <sys/prctl.h>

int task_execute(task *htask, thread *parent_thread)
{
    int err = TASK_OK;
    void *result = NULL;
    void* (*function)(void *arg) = NULL;
    void *arg = NULL;
    
    /* we are locking the task to get a local copy of all needed values */
    if((err = task_lock(htask)) != 0) {
        //debug_error("failed to lock task");
        return err;
    }

    htask->parent_thread = parent_thread;
    htask->status = TASK_RUNNING;
    function = htask->function;
    arg = htask->arg;

    /** setting the name for debugging */
    prctl(PR_SET_NAME, htask->task_name);

    if((err = task_unlock(htask)) != 0) {
        return err;
    }

    /* we execute the function locally */
    result = function(arg);
    
    /* after execution we lock the task again to set the return value and delete connection to thread */
    if((err = task_lock(htask)) != 0) {
        return err;
    }

    htask->retval = result;
    htask->status = TASK_COMPLETED;
    htask->parent_thread = NULL;

    if((err = task_unlock(htask)) != 0) {
        return err;
    }
    
    if(task_notify(htask) != 0) {
        err = TASK_ERROR_SEMAPHORE;
        return err;
    }

    debug(DEBUG_DONE);

    return err;
}