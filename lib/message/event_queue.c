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

#include "event_queue.h"
#include "queue.h"
#include <stdlib.h>

struct event_queue_st
{
	queue_st * queue;
};

int send_event(event_queue_st * const event_queue, void * const data)
{
	int result;

	put_queue_item(event_queue->queue, data);
	result = 0;

	return result;
}

void * recv_event(event_queue_st * const event_queue)
{
	void * event;

	event = get_queue_item(event_queue->queue);

	return event;
}

event_queue_st * alloc_event_queue(size_t const event_queue_size)
{
	event_queue_st * event_queue;

	event_queue = calloc(1, sizeof *event_queue);
	if (event_queue != NULL)
	{
		event_queue->queue = alloc_queue(event_queue_size);
        if (event_queue->queue == NULL)
        {
            free(event_queue);
            event_queue = NULL;
        }
    }

	return event_queue;
}

void free_event_queue(event_queue_st * const event_queue)
{
	/* NB assumes that the event queue is empty and all resources it contained have been freed*/

    free_queue(event_queue->queue);
	free(event_queue);
}
