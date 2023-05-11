#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <thread.h>
#include <pthread.h>

void *threadest(void *arg)
{
    struct timespec timespec = { 0 };
    timespec.tv_nsec = CLOCKS_PER_SEC * 500;

    int i = 0;
    while(i < 3) {
        printf("%s\n", (char *) arg);
        nanosleep(&timespec, NULL);
        i++;
        //task_setProgress((double) i * 10);
    }
    return arg;
}

int main(void)
{

    int rc = THREAD_OK;
    thread *thread = NULL;
    task *task1 = NULL;
    task *task2 = NULL;
    task *task3 = NULL;    


    rc = thread_create(&thread);
    if(thread_perror("thread_create", rc)) {
        printf("error##########\n");
        return -1;
    }
    rc = task_create(&task1, "Test Thread 1", threadest, "Thread 1");
    printf("RC: %d\n\n", rc);
    if(thread_perror("thread_addjob", rc)) {
    
        return -1;
    }
    rc = task_create(&task2, "Test Thread 2", threadest, "Thread 2");
    if(thread_perror("thread_addjob", rc)) {
        return -1;
    }
    rc = task_create(&task3, "Test Thread 3", threadest, "Thread 3");
    if(thread_perror("thread_addjob", rc)) {
        return -1;
    }

    rc = thread_addtask(thread, task1);
    if(thread_perror("thread_addjob", rc)) {
        return -1;
    }
    //sleep(2);

    printf("Task 1 is Running: %d\n", task_isRunnning(task1));

    while(task_isRunnning(task1) != 1) {
        sleep(1);
    }
    task_pause(task1);
    sleep(2);
    task_resume(task1);

    rc = thread_addtask(thread, task2);
    if(thread_perror("thread_addjob", rc)) {
        return -1;
    }
    
    

/* 
    sleep(1);
    task_pause(task2);
    sleep(1);
    task_resume(task2); */
    
    rc = thread_addtask(thread, task3);
    if(thread_perror("thread_addjob", rc)) {
        return -1;
    }


    
    task_wait(task1);
    printf("Result 1: %s\n", (char*) task_getResult(task1, &rc));

    printf("Task 2 is Running: %d\n", task_isRunnning(task2));

    task_wait(task2);
    printf("Result 2: %s\n", (char*) task_getResult(task2, &rc));
    printf("Task 3 is Running: %d\n", task_isRunnning(task3));

    task_wait(task3);
    printf("Result 3: %s\n", (char*) task_getResult(task3, &rc));


    printf("Ret: %d\n", thread_destroy(&thread));

    task_destroy(&task1);
    task_destroy(&task2);
    task_destroy(&task3);

    pthread_exit(NULL);
}