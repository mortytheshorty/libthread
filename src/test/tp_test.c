#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <threadpool.h>

void *thread_test(void *arg)
{
    struct timespec timespec = { 0 };
    timespec.tv_nsec = CLOCKS_PER_SEC * 500;

    int i = 0;
    while(i < 10) {
        printf("%s\n", (char *) arg);
        nanosleep(&timespec, NULL);
        i++;
        //task_setProgress((double) i * 10);
    }
    return arg;
}

int main(void)
{
    int err = 0;
    threadpool *tp = NULL;
    task *task = NULL;

    err = task_create(&task, "Test Task 1", thread_test, "Teststring No. 1");

    err = threadpool_create(&tp, 10);
    thread_perror("threadpool_create", err);
    err = threadpool_addtask(tp, task);
    printf("TP: %p\nTask: %p\n", tp, task);
    thread_perror("threadpool_addtask", err);

    //sleep(1);

    threadpool_stats(tp);

    sleep(10);

    threadpool_destroy(&tp);
}