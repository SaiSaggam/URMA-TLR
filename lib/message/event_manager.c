#include "event_manager.h"
#include "event_queue.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <tlr_print.h>

struct event_manager_st
{
	event_handler event_handler;
	event_queue_st * event_queue;
	void const * user_context;

    pthread_t user_thread_id; /* the worker thread ID. This is the thread the user code runs in */
};
#define hand_user_context(ptr) ((void *) (ptr))

static void free_event_manager(event_manager_st * const event_manager)
{
    // TODO: this all needs to be better.
    // e.g. free up event queue, quit thread, all that stuff
	free(event_manager);
}

static void run_event_manager(event_manager_st * const event_manager)
{
	event_queue_st * const event_queue = event_manager->event_queue;

	// XXX break on certain conditions?
	while (1)
	{
		void * event;

		event = recv_event(event_queue);
		if (event == NULL)
		{
			fprintf(stderr, "NULL event!\n");
			continue;
		}

		event_manager->event_handler(hand_user_context(event_manager->user_context), event);
	}
}

/*
 * event_manager_worker
 * This is the worker thread that is responsible for pulling events off the event queue and
 * delivering to the user by way of the callback.
 * The user code runs in the context of this thread.
 */
static void * event_manager_worker(void * data)
{
	event_manager_st * event_manager = data;

    run_event_manager(event_manager);
    /* shouldn't return, but if it does, do some cleanup */
    free_event_manager(event_manager);

    tlr_debug("event_manager_worker thread exiting");
	pthread_exit(NULL);
}

int send_event_to_event_manager(event_manager_st * const event_manager, void * const event)
{
    int result;

    result = send_event(event_manager->event_queue, event);

    return result;
}

/* This function can be called to create a new state machine  */
event_manager_st * new_event_manager(void const * const user_context, size_t event_queue_size, event_handler event_handler_cb)
{
    event_manager_st  * event_manager;
    int pthread_create_result;

    /*
     * TODO: option to create the event manager without actually starting it. In addition, there
     * should be a new functions to call to start and stop event handling.
     * This would allow the caller to control when events should start getting handled
     */
    /* Allocate storage for the object. */
    event_manager = calloc(1, sizeof *event_manager);

    if (event_manager == NULL)
        goto error;

    event_manager->user_context = user_context;
    event_manager->event_handler = event_handler_cb;
    event_manager->event_queue = alloc_event_queue(event_queue_size);
    if (event_manager->event_queue == NULL)
        goto error;

    pthread_create_result = pthread_create(&event_manager->user_thread_id, NULL, event_manager_worker, event_manager);
    if (pthread_create_result != 0)
        goto error;

    goto done;

error:
    if (event_manager != NULL)
    {
        if (event_manager->event_queue == NULL)
        {
            free_event_queue(event_manager->event_queue);
        }
        free(event_manager);
        event_manager = NULL;

    }
done:

    /* Return the new manager */
    return event_manager;
}



