#include <task_private.h>

int task_pause(task *htask)
{
    thread *parent_thread = NULL;
    task_lock(htask);
    parent_thread = htask->parent_thread;
    task_unlock(htask);

    if(thread_pause(parent_thread) != THREAD_OK) {
        debug_error("failed to pause task thread");
        return TASK_ERROR_THREAD;
    }

    debug(DEBUG_DONE);

    return TASK_OK;
}

int task_resume(task *htask)
{   
    thread *parent_thread = NULL;
    task_lock(htask);
    parent_thread = htask->parent_thread;
    task_unlock(htask);

    if(thread_resume(parent_thread) != THREAD_OK) {
        return TASK_ERROR_THREAD;
    }
    debug(DEBUG_DONE);

    return TASK_OK;
}