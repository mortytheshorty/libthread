#include <libthread_private.h>


#define THREAD_SLEEP_TIME 250000000

/* thread local thread_t pointer which is set after thread creation */
__thread thread *local_thread = NULL;

/* thread local variable which says thread can pause */
__thread volatile int thread_on_hold_available = 1;
/** thread contains a pointer to this variable which will be set by thread_destroy()
 * thread_worker is reading on this variable to see if it has to exit
 */
__thread volatile int thread_exit = 0;

/** thread local log file handle */
__thread volatile FILE *thread_log = NULL;


void thread_worker_pause_action_handler(int sig_id)
{
    debug("thread pause action handler called");
    (void)sig_id;

    struct timespec timespec = { 0 };
    timespec.tv_nsec = THREAD_SLEEP_TIME;

    int old_state = thread_getStatus(local_thread);
    if(thread_setStatus(local_thread, THREAD_PAUSED) != THREAD_OK) {
        debug_error("failed to set thread state before pause");
    }
    
    while( thread_getStatus(local_thread) != THREAD_RESUMED ) {
        debug("still sleeping");
        nanosleep(&timespec, NULL);
    }

    if(thread_setStatus(local_thread, old_state) != THREAD_OK) {
        debug_error("failed to set thread state after pause");
    }

}

int thread_worker_enable_pause(void)
{
    struct sigaction act_pause;
    sigemptyset(&act_pause.sa_mask);
    act_pause.sa_flags = 0;
    act_pause.sa_handler = thread_worker_pause_action_handler;

    if(sigaction(SIGUSR1, &act_pause, NULL) == -1) {
        debug_error("failed to register sigaction handler for pausing the thread");
        thread_on_hold_available = 0;
        return -1;
    }

    debug("registering sigaction handler for puasing the thread");

    return 0;
}

/**
 * @brief thread worker function to destroy itself
 * 
 * NOTE: if we leave this, there is no control for valgrind to follow up the cleanup of thread_t because the thread is detached
 * 
 * @param thread 
 */
void *thread_selfdestroy(thread *hthread)
{
    int rc = 0;
    
    debug("killing myself");

    if(pthread_mutex_lock(&hthread->lock) != 0) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_LOCK);
    }

    if( !hthread->tp ) {
        if((rc = taskqueue_destroy(&hthread->taskqueue)) != TASKQUEUE_OK) {
            if(rc != TASKQUEUE_ERROR_INVAL) {
                debug_error("failed to destroy thread[%ld] taskqueue", hthread->id);
            }
        }
    }

    if(pthread_mutex_unlock(&hthread->lock) != 0) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_UNLOCK);
    }

    if(pthread_mutex_destroy(&hthread->lock) != 0) {
        debug_error(DEBUG_ERROR_PTHREAD_MUTEX_DESTROY);
    }

    free(hthread);
    debug(DEBUG_DONE);
    return NULL;
}

/** thread worker function handed to pthread_create in thread_create() */
void* thread_worker(void *worker_arg)
{
    char buf[32] = { 0 };
    snprintf(buf, sizeof(buf), "%ld.log", pthread_self());

    if(pthread_detach(pthread_self()) != 0) {
        debug_error("pthread_detach() failed");
    }

    thread *hthread = worker_arg;

    // we need to set thread thread to a thread_local global variable
    // so signal handlers can get some status for single threads
    local_thread = hthread;
    hthread->shutdown = &thread_exit;

    if( thread_worker_enable_pause() != 0 ) {
        debug_error("enabling thread pausing option failed");
    }
    
    /* if thread belongs to a threadpool,
    lock the threadpool thread,
    increment threads_alive_count */
    if( hthread->tp ) {

    }



    while (1) {
        debug("waiting for tasks");
        /* waiting on event on taskqueue */
        if(sem_wait(&hthread->taskqueue->has_tasks) != 0) {
            debug_error(DEBUG_ERROR_SEMAPHORE_WAIT);
        }

        /* if thread belongs to a threadpool,
        lock the threadpool,
        decrement task_running_count */
        if( hthread->tp ) {

        }
        
        /* if thread_ */
        if( thread_exit ) {
            return thread_selfdestroy(hthread);
        }


        int err = TASKQUEUE_OK;
        task *htask = thread_pull_task(hthread,  &err);
        if( !err && htask ) {
            
            if((err = thread_setStatus(hthread, THREAD_RUNNING)) != THREAD_OK) {
                debug_error("failed to set state THREAD_RUNNING");
            }

            /* if thread belongs to threadpool manipulate threadpool */
            if(hthread->tp) {
                threadpool_thread_running(hthread->tp);
            }

            /* executing current task */
            if(task_execute(htask, hthread) != TASK_OK) {
                debug_error("failed to execute task correctly");
            }
            if(hthread->tp) {
                threadpool_thread_idle(hthread->tp);
            }

            debug("task_t finished");
        }

        /* if thread belongs to a threadpool,
        lock the threadpool,
        decrement task_running_count */
        if( hthread->tp ) {

        }


    }

    /* if thread belongs to a threadpool,
    lock the threadpool thread,
    decrement threads_alive_count */
    if( hthread->tp ) {

    }

    return NULL;
}