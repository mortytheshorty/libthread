// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/

#include <thread_error.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <pthread.h>
#include <sys/prctl.h>

/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/
#ifdef DEBUG
pthread_mutex_t debug_lock = PTHREAD_MUTEX_INITIALIZER;

extern __thread FILE *thread_log;

extern const char* thread_strerror(int err);

/*********************************************************************************************************/
/*                                           FUNCTION DEFINITIONS                                        */
/*********************************************************************************************************/

/*  void xfree(void *ptr, const char *file, int line)
 {
   printf("call of free in file %s:%d\n", file, line);
   free(ptr);
 } */

void xdebug(pthread_t threadid, const char *function, const char *fmt, ...)
{
    FILE *fp = thread_log == NULL ? stdout : thread_log;
    const char name[16] = { 0 };
    prctl(PR_GET_NAME, name);

    pthread_mutex_lock(&debug_lock);
    va_list args;
    va_start(args, fmt);

    fprintf(fp, "DEBUG: %16s[%ld] %s(): ", name, threadid, function);
    vfprintf(fp, fmt, args);
    fprintf(fp, "\n");
    fflush(fp);
    
    va_end(args);
    pthread_mutex_unlock(&debug_lock);
}


void xdebug_error(pthread_t threadid, const char *function, const char *filename, int line, const char *fmt, ...)
{
    FILE *fp = thread_log == NULL ? stderr : thread_log;
    const char name[16] = { 0 };
    prctl(PR_GET_NAME, name);
    
    pthread_mutex_lock(&debug_lock);
    int err = errno;
    va_list args;
    va_start(args, fmt);

    fprintf(fp, "ERROR: %s[%ld] %s(): ", name, threadid, function);
    vfprintf(fp, fmt, args);
    if(err) {
        fprintf(fp, " (%s)", strerror(errno));
        errno = 0;
    }
    fprintf(fp, " in file %s at line %d\n", filename, line);
    fflush(fp);

    va_end(args);
    pthread_mutex_unlock(&debug_lock);
}
#endif