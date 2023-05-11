// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef LIBTHREAD_THREADPOOL_H    // BEGIN INCLUDE GUARD
#define LIBTHREAD_THREADPOOL_H

/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/

#include <thread.h>

/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/

typedef struct threadpool threadpool;

/*********************************************************************************************************/
/*                                           FUNCTION DEFINITIONS                                        */
/*********************************************************************************************************/

int threadpool_create(threadpool **tp, size_t n_threads);
int threadpool_destroy(threadpool **tp);
int threadpool_addtask(threadpool *tp, task *task);
void threadpool_stats(threadpool *tp);


#endif