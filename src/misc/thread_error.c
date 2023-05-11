
const char *thread_error_strings[] = {
    "THREAD_OK",
    "thread object failed to request memory",
    "thread object failed executing pthread.h function",
    "thread object received an invalid argument",
    "jobqueue failed to request memory",
    "bsem object received an invalid argument",
    "bsem object failed executing pthread.h function",
};

const char *thread_status_string[] = {
    "THREAD_WAITING",
    "THREAD_PAUSE",
    "THREAD_RUNNING",
    "THREAD_LOADED",
    "THREAD_STATUS_MAX",
    "THREAD_EXIT",
    "THREAD_RESUME",
};

const char *DEBUG_DONE = "done.";
const char *DEBUG_ERROR_PTHREAD_CREATE = "pthread creation failed";
const char *DEBUG_ERROR_PTHREAD_MUTEX_INIT = "mutex init failed";
const char *DEBUG_ERROR_PTHREAD_MUTEX_DESTROY = "mutex destroy failed";
const char *DEBUG_ERROR_PTHREAD_MUTEX_LOCK = "mutex lock failed";
const char *DEBUG_ERROR_PTHREAD_MUTEX_UNLOCK = "mutex unlock failed";
const char *DEBUG_ERROR_PTHREAD_KILL = "pthread_kill() failed";
const char *DEBUG_ERROR_NO_MEM = "memory allocation failed";
const char *DEBUG_ERROR_INVAL = "invalid argument";
const char *DEBUG_ERROR_SEMAPHORE_INIT = "sem_init() failed";
const char *DEBUG_ERROR_SEMAPHORE_DESTROY = "sem_destroy() failed";
const char *DEBUG_ERROR_SEMAPHORE_WAIT = "sem_wait() failed";
const char *DEBUG_ERROR_SEMAPHORE_POST = "sem_post() failed";
