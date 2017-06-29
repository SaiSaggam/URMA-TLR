#ifndef __CREATE_WORKER_THREAD_H__
#define __CREATE_WORKER_THREAD_H__

#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>

/*
 * create_worker_thread
 * Create a new thread specified by thread_fn
 * thread_id: a pointer to contain the new thread ID
 * create_detached: true if the new thread should be created detached (no need to call
 * pthread_join(), else false for attached (owner should call pthread_join() so that resources used
 * by thread get released when the thread ends)
 * maximum_tries: The maximum number of times to attempt to create the thread
 * retry_delay_milliseconds: The time between attempts to create the thread
 * thread_fn: The thread function
 * thread_fn_arg: The arg to pass to the thread function when it is called
 *
 * Returns:
 *  success: 0. In this case *thread_id will contain the new thread ID error: -1 if some error
 *  occurred before calling pthread_create
 *         otherwise returns the return value from pthread_create()
 *
 */
int create_worker_thread(pthread_t * const thread_id,
                         bool create_detached,
                         size_t maximum_tries,
                         size_t retry_delay_milliseconds,
                         void * (*thread_fn)(void *),
                         void * const thread_fn_arg);

#endif /* __CREATE_WORKER_THREAD_H__ */
