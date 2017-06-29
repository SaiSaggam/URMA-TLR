/*****************************************************************************
* Copyright (c) 2016 Digi International Inc., All Rights Reserved
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

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/queue.h>
#include <pthread.h>

struct queue_entry_st
{
	TAILQ_ENTRY(queue_entry_st) entry;
	void * data;
};

struct queue_st
{
	size_t queue_len;   /* number of entries in the active queue */
	size_t slots;       /* number of slots in the queue */
	TAILQ_HEAD(, queue_entry_st)free_list;
	TAILQ_HEAD(, queue_entry_st)active_list;

	pthread_mutex_t lock;
	pthread_cond_t wait_room;
	pthread_cond_t wait_data;
};

queue_st * alloc_queue(unsigned int const slots)
{
	queue_st * queue;
	struct queue_entry_st * entry;
	int i;

	queue = calloc(1, sizeof *queue);
	if (queue == NULL)
	{
		errno = ENOMEM;
		goto done;
	}
	queue->slots = slots;

    pthread_mutex_init(&queue->lock, NULL);
    pthread_cond_init(&queue->wait_room, NULL);
    pthread_cond_init(&queue->wait_data, NULL);

	TAILQ_INIT(&queue->free_list);
	TAILQ_INIT(&queue->active_list);

	/* allocate the desired number of entries and add them to the free list */
	for (i = 0; i < slots; i++)
	{
		entry = calloc(1, sizeof *entry);
		if (entry == NULL)
		{
			errno = ENOMEM;
			goto error;
		}
		TAILQ_INSERT_HEAD(&queue->free_list, entry, entry);
	}

    goto done;

error:
    free_queue(queue);
    queue = NULL;

done:
	return queue;
}

void * get_queue_item(queue_st * const queue)
{
	void * pdata;
	struct queue_entry_st * entry;

	if (queue == NULL)
	{
        pdata = NULL;
        goto done;
	}

    pthread_mutex_lock(&queue->lock);

	/* wait until there is an entry in the queue */
	while ((entry = TAILQ_FIRST(&queue->active_list)) == NULL)
        pthread_cond_wait(&queue->wait_data, &queue->lock);

	/* remove from the active list */
	TAILQ_REMOVE(&queue->active_list, entry, entry);
	pdata = entry->data;
	entry->data = NULL;

	/* place into the free list*/
	TAILQ_INSERT_TAIL(&queue->free_list, entry, entry);
	queue->queue_len--;

	/* signal queue_put() that there is room for a new entry */
	pthread_cond_signal(&queue->wait_room);

    pthread_mutex_unlock(&queue->lock);

done:
	return pdata;
}

void put_queue_item(queue_st * const queue, void * const pdata)
{
	struct queue_entry_st * entry;

	if (queue == NULL)
	{
        goto done;
	}

	pthread_mutex_lock(&queue->lock);

	/* wait until there is a free entry */
    while ((entry=TAILQ_FIRST(&queue->free_list)) == NULL)
        pthread_cond_wait(&queue->wait_room, &queue->lock);

	/* remove from the free list */
	TAILQ_REMOVE(&queue->free_list, entry, entry);
	entry->data = pdata;

	/* place into the active list */
	TAILQ_INSERT_TAIL(&queue->active_list, entry, entry);
	queue->queue_len++;

	/* signal the waiters in queue_get() that a message is waiting */
	pthread_cond_signal(&queue->wait_data);

    pthread_mutex_unlock(&queue->lock);

done:
	return;
}

size_t get_queue_len(queue_st * const queue)
{
	size_t len;

	if (queue == NULL)
	{
        len = 0;
        goto done;
	}

	pthread_mutex_lock(&queue->lock);

	len = queue->queue_len;

	pthread_mutex_unlock(&queue->lock);

done:
	return len;
}

void free_queue(queue_st * const queue)
{
    struct queue_entry_st * entry;

    if (queue == NULL)
    {
        goto done;
    }

    /*
     * TODO: FIXME. this is no good if there are messages on the active list
     * OK to call at init time, but not after the queue is in use.
     *
     */


    while ((entry=TAILQ_FIRST(&queue->free_list))!= NULL)
    {
        TAILQ_REMOVE(&queue->free_list, entry, entry);
        free(entry);
    }
    /* and again for the active list */
    while ((entry=TAILQ_FIRST(&queue->active_list))!= NULL)
    {
        TAILQ_REMOVE(&queue->free_list, entry, entry);
        free(entry);
    }

done:
    free(queue);
}
