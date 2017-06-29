/*****************************************************************************
* Copyright (c) 2015 Digi International Inc., All Rights Reserved
*
* This software contains proprietary and confidential information of Digi
* International Inc.  By accepting transfer of this copy, Recipient agrees
* to retain this software in confidence, to prevent disclosure to others,
* and to make no use of this software other than that for which it was
* delivered.  This is an unpublished copyrighted work of Digi International
* Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
* prohibited.
*
* Restricted Rights Legend
*
* Use, duplication, or disclosure by the Government is subject to
* restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
* Technical Data and Computer Software clause at DFARS 252.227-7031 or
* subparagraphs (c)(1) and (2) of the Commercial Computer Software -
* Restricted Rights at 48 CFR 52.227-19, as applicable.
*
* Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
*
*****************************************************************************/

#include "timer.h"
#include "timespec_helpers.h"
#include "tlr_common.h"
#include "unittest_support.h"
#include "tlr_eintr.h"

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/queue.h>
#include <semaphore.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <syslog.h>

#define TIMER_SIGNAL	SIGRTMIN

struct timer_id_st
{
    timer_context_st * context;
	TAILQ_ENTRY(timer_id_st) entry;
    bool in_active_list;
	int unique_id;
	struct timespec expiry_time;
	void (*callback)(void * data);
	void * data;
};

struct timer_context_st
{
	pthread_t timer_thread_id;
	int next_unique_id;
	sigset_t original_signal_mask;
	pthread_mutex_t   unique_id_lock;

	TAILQ_HEAD(, timer_id_st) active_list;
	sem_t active_list_sem;
};

/* replacement signal handler */
static void timer_signal_handler (int sig)
{
}

/* get an ID for a new timer. Must not be 0.
 * Should be unique. The current implementation doesn't guarantee that,
 * but it should be pretty unlikely.
 */
static int get_next_timer_id(timer_context_st * context)
{
	int id;

	pthread_mutex_lock(&context->unique_id_lock);
	if ((id = ++context->next_unique_id) == 0)
	{
		id = ++context->next_unique_id;
	}
	pthread_mutex_unlock(&context->unique_id_lock);

	return id;
}

static void insert_entry_into_active_list(timer_id_st * entry)
{
    timer_context_st * const context = entry->context;

	TLR_TEMP_FAILURE_RETRY(sem_wait(&context->active_list_sem));

    entry->in_active_list = true;
    TAILQ_INSERT_TAIL(&context->active_list, entry, entry);

	sem_post(&context->active_list_sem);

	return;
}

static void free_timer(timer_id_st * active)
{
	free(active);
}

static int remove_entry_from_active_list(timer_id_st * const timer_id)
{
    timer_context_st * const context = timer_id->context;
    int result = -1;

	TLR_TEMP_FAILURE_RETRY(sem_wait(&context->active_list_sem));

    if (timer_id->in_active_list == true)
    {
        TAILQ_REMOVE(&context->active_list, timer_id, entry);
        timer_id->in_active_list = false;
        result = 0;
    }

	sem_post(&context->active_list_sem);

    return result;
}

static bool find_soonest_expiry(timer_context_st * context, struct timespec * const shortest_expiry)
{
	bool result;
	struct timespec * shortest = NULL;
	timer_id_st * timer_id;

	TLR_TEMP_FAILURE_RETRY(sem_wait(&context->active_list_sem));

	TAILQ_FOREACH(timer_id, &context->active_list, entry)
	{
        if (shortest == NULL || timerspec_compare(&timer_id->expiry_time, shortest) < 0)
        {
			shortest = &timer_id->expiry_time;
        }
	}
	if (shortest != NULL)
	{
		*shortest_expiry = *shortest;
		result = true;
	}
    else
    {
		result = false;
    }

	sem_post(&context->active_list_sem);

	return result;
}

static int handle_expired_timers(timer_context_st * const context, struct timespec * const now)
{
    int num_expired_timers_handled = 0;
	timer_id_st * timer_id, *temp;

	TLR_TEMP_FAILURE_RETRY(sem_wait(&context->active_list_sem));

	for (timer_id = TAILQ_FIRST(&context->active_list); timer_id != NULL; timer_id = temp)
	{
		temp = TAILQ_NEXT(timer_id, entry);
		if (timerspec_compare(&timer_id->expiry_time, now) <= 0)
		{
			void * data = timer_id->data;

            TAILQ_REMOVE(&context->active_list, timer_id, entry);
            timer_id->in_active_list = false;

			/* inform the timer owner that the timer has expired */
			timer_id->callback(data);

            num_expired_timers_handled++;
		}
	}

	sem_post(&context->active_list_sem);

    return num_expired_timers_handled;
}

STATIC_FUNCTION int do_expired_timer_check(timer_context_st * const context)
{
    int result;
    int res;
    struct timespec timeout;
    struct timespec sleep_time;
    bool any_timers_to_check;
    struct timespec * timespec_to_use;
    struct timespec now_time_timespec;
    int num_expired_timers_handled;

    /* handling expired timers may result in significant time passing, so
     * repeat the process until no timers have expired.
     */
    do
    {
        clock_gettime(CLOCK_MONOTONIC, &now_time_timespec);
        num_expired_timers_handled = handle_expired_timers(context, &now_time_timespec);
    }
    while (num_expired_timers_handled > 0);

    /* at this point there will either be no timers left, or timers that expire after 'now'
     */
    any_timers_to_check = find_soonest_expiry(context, &timeout);
    if (any_timers_to_check == true)
    {
        /* the difference between the time in the soonest entry and the time now
         * is the amount of time we need to sleep for
         */
        timerspec_sub(&timeout, &now_time_timespec, &sleep_time);

        // Sanity check that the sleep time is not negative. If it is we just
        // reset it to zero, which will cause the timer to be handled
        // immediately, which is what we want.
        if (sleep_time.tv_sec < 0 || sleep_time.tv_nsec < 0)
        {
            sleep_time.tv_sec = 0;
            sleep_time.tv_nsec = 0;
        }

        timespec_to_use = &sleep_time;
    }
    else
    {
        /* no timers running. just block in the pselect call by using a NULL timespec pointer */
        timespec_to_use = NULL;
    }

    /* wait for a timer to expire, or a signal that a timers has been started or stopped */
    res = pselect (0, NULL, NULL, NULL, timespec_to_use, &context->original_signal_mask);

    if (res < 0)
    {
        /* the thread will get signalled whenever a timer is started or stopped */
        if (errno != EINTR)
        {
            syslog(LOG_ERR, "timer thread got select error (%d)\n", errno);
        }
    }

    result = 0;

    return result;
}

static int wait_for_expired_timers(timer_context_st * const context)
{
    int result;

    do
    {
        result = do_expired_timer_check(context);
    }
    while(result == 0);

    return result;
}

static void * timer_thread(void * data)
{
    timer_context_st * const context = data;
    struct sigaction act;
    sigset_t mask;

    /*
     * we replace the default handler with our own, as we don't want the application to end
     * when one is received.
     */
    memset (&act, 0, sizeof act);
    act.sa_handler = timer_signal_handler;

    if (sigaction(TIMER_SIGNAL, &act, 0))
    {
        goto done;
    }
    /* block the timer signal. It will be unblocked in the pselect call
     * We do this to avoid a race condition between setting up the fd set and the select call
     * pselect will unblock this signal and previously blocked signals will then be received.
     */
    sigemptyset (&mask);
    sigaddset (&mask, TIMER_SIGNAL);

    pthread_sigmask(SIG_BLOCK, &mask, &context->original_signal_mask);

    /* signal to the parent that initialisation has been performed */
    sem_post(&context->active_list_sem);

    /* get down to the business of waiting for expired timers */
    wait_for_expired_timers(context);

done:
    pthread_exit(NULL);
}

timer_context_st * open_timer_context(void)
{
    timer_context_st * context;
    int rc;

    context = calloc(1, sizeof *context);
    if (context == NULL)
    {
        goto error;
    }

    sem_init(&context->active_list_sem, 0, 0);
    pthread_mutex_init(&context->unique_id_lock, NULL);
    TAILQ_INIT(&context->active_list);

    rc = pthread_create(&context->timer_thread_id, NULL, timer_thread, context);
    if (rc)
    {
        goto error;
    }

    /*
     *  Wait for the timer thread to get up and running.
     *  We do this to avoid having other threads attempt to start timers before this thread has
     *  initialised.
     *  We achieve this by waiting for the timer thread to post to the active list semaphore, which
     *  it does after initialisation has completed.
     */
    TLR_TEMP_FAILURE_RETRY(sem_wait(&context->active_list_sem));
    /* post to the semaphore again so that new timers can be added to the list */
    sem_post(&context->active_list_sem);

    /* timer context created successfully */
    goto done;

error:
    free(context);
    context = NULL;

done:
    return context;
}

timer_id_st * create_timer(timer_context_st * const context, void (*callback)(void * data), void * data)
{
    timer_id_st * timer = calloc(1, sizeof * timer);

    if (timer == NULL)
    {
        goto done;
    }
    timer->context = context;
    timer->unique_id = get_next_timer_id(context);
    timer->callback = callback;
    timer->data = data;
    timer->in_active_list = false;

done:
    return timer;
}

void start_timer(timer_id_st * const timer_id, unsigned int millisecs)
{
    struct timespec timeout_time;
	struct timespec now_time_timespec;
    long remainder;

    if (timer_id == NULL)
    {
        goto done;
    }
    /*
     * to deal with the case where the timer may already be running, let's remove it from the
     * list first
     */
    (void)remove_entry_from_active_list(timer_id);

    clock_gettime(CLOCK_MONOTONIC, &now_time_timespec);

	timeout_time.tv_sec = milliseconds_to_seconds(millisecs, &remainder);
	timeout_time.tv_nsec = milliseconds_to_nanoseconds(remainder);

    timerspec_add(&now_time_timespec, &timeout_time, &timer_id->expiry_time);

    insert_entry_into_active_list(timer_id);

    /* signal the timer thread that we have updated the list of running timers */
	pthread_kill(timer_id->context->timer_thread_id, TIMER_SIGNAL);

done:
    return;
}

/*
 * stop_timer
 * returns -1 if the timer was not found in the current list of timers.
 */
int stop_timer(timer_id_st * const timer_id)
{
	int result;

    if (timer_id == NULL)
    {
        result = -1;
        goto done;
    }
    result = remove_entry_from_active_list(timer_id);

    /* signal the timer thread that we have updated the list of running timers */
    if (result == 0)
    {
        pthread_kill(timer_id->context->timer_thread_id, TIMER_SIGNAL);
    }

done:
	return result;
}

int destroy_timer(timer_id_st * const timer_id)
{
    if (timer_id != NULL)
    {
        stop_timer(timer_id);
        free_timer(timer_id);
    }

    return 0;
}

bool timer_is_running(timer_id_st * const timer_id)
{
    bool is_running;

    if (timer_id == NULL)
    {
        is_running = false;
    }
    else
    {
        is_running = timer_id->in_active_list;
    }

    return is_running;
}

int seconds_remaining(timer_id_st * const timer_id)
{
    timer_context_st * context;
    int result;
    struct timespec time_now;
    struct timespec time_remaining;
    bool got_semaphore;

    if (timer_id == NULL)
    {
        result = 0;
        got_semaphore = false;
        goto done;
    }

    context = timer_id->context;

    TLR_TEMP_FAILURE_RETRY(sem_wait(&context->active_list_sem));
    got_semaphore = true;

    if (!timer_id->in_active_list)
    {
        result = 0;
        goto done;
    }

    clock_gettime(CLOCK_MONOTONIC, &time_now);
    timerspec_sub(&timer_id->expiry_time, &time_now, &time_remaining);

    result = time_remaining.tv_sec;
    if (result < 0)
    {
        result = 0;
    }

done:
    if (got_semaphore)
    {
        sem_post(&context->active_list_sem);
    }

    return result;
}

