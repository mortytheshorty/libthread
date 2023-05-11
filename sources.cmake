set(SOURCES
src/task/task_create.c
src/task/task_destroy.c
src/task/task_execute.c
src/task/task_pause.c
src/task/task_wait.c
src/task/task.c
src/task/taskqueue.c
src/thread/thread_create.c
src/thread/thread_destroy.c
src/thread/thread_pause.c
src/thread/thread_worker.c
src/thread/thread.c
src/threadpool/threadpool.c
src/misc/thread_debug.c
src/misc/thread_error.c
)

set(PUBLIC_HEADERS
headers/thread.h
headers/threadpool.h
headers/thread_error.h
)

set(PRIVATE_HEADERS
src/headers/libthread_private.h
src/headers/task_private.h
src/headers/taskqueue_private.h
src/headers/thread_private.h
src/headers/threadpool_private.h
src/headers/thread_debug.h
)

set_property(GLOBAL PROPERTY PUBLIC_HEADERS ${PUBLIC_HEADERS})