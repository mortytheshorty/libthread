#include <libthread_private.h>


int thread_pause(thread *hthread)
{
    int rc = THREAD_OK;

    if( !hthread ) {
        debug(DEBUG_ERROR_INVAL);
        return THREAD_ERROR_INVAL;
    } 

    if((rc = pthread_kill(hthread->id, SIGUSR1)) != 0) {
        debug_error(DEBUG_ERROR_PTHREAD_KILL);
        return THREAD_ERROR_PTHREAD;
    }

    debug(DEBUG_DONE);
    return rc;
}

int thread_resume(thread *hthread)
{   
    if( !hthread ) {
        debug_error(DEBUG_ERROR_INVAL);
        return THREAD_ERROR_INVAL;
    }
    
    int err = THREAD_OK;
    err = thread_setStatus(hthread, THREAD_RESUMED);

    debug(DEBUG_DONE);
    return err;
}