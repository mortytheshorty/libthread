#include <task_private.h>

int task_create(task **htask, const char *name, void* (*function)(void *arg), void *arg)
{
/*     if( !htask || !(*htask) ) {
        return TASK_ERROR_INVAL;
    } */

    task *new_task = malloc(sizeof *new_task);
    if( !htask ) {
        debug_error(DEBUG_ERROR_NO_MEM);
        return TASK_ERROR_NO_MEM;
    }
    
    new_task->task_name = name;
    new_task->next = NULL;
    new_task->prev = NULL;
    new_task->parent_thread = NULL;
    new_task->retval = NULL;
    new_task->function = function;
    new_task->arg = arg;
    new_task->status = -1;
    
    if(sem_init(&new_task->sem, 0, 0) != 0) {
        debug_error(DEBUG_ERROR_SEMAPHORE_INIT);
        free(new_task);
        return TASK_ERROR_SEMAPHORE;
    }

    if(pthread_mutex_init(&new_task->lock, NULL) != 0) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_INIT);
        free(new_task);
        return TASK_ERROR_PTHREAD;
    }

    *htask = new_task;

    debug(DEBUG_DONE);
    return TASK_OK;
}

