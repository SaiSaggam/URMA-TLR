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

#include "wifi_monitor_event_alloc.h"
#include <stdlib.h>

wifi_monitor_event_t * wifi_monitor_event_alloc(int const code, size_t const instance, void * * const info, size_t const info_size)
{
    wifi_monitor_event_t * event;

    /*
     * Hmm, I can't figure out how to mock out malloc() in the test framework, so I'll use another
     * name for test purposes.
    */
    event = EVENT_ALLOC_MALLOC(sizeof *event);
    if (event == NULL)
    {
        goto done;
    }

    event->code = code;
    event->instance = instance;
    if (info != NULL)
    {
        /* claim the memory passed with the event. It'll get freed when the event is freed */
        event->info = *info;
        *info = NULL;
    }
	else
	{
		event->info = NULL;
	}
	event->info_size = info_size;

done:
    return event;
}

void wifi_monitor_event_free(wifi_monitor_event_t * const event)
{
    if (event != NULL)
    {
        /*
         * Hmm, I can't figure out how to mock out free() in the test framework, so I'll use another
         * name for test purposes.
         */
        EVENT_ALLOC_FREE((void *)event->info);
        EVENT_ALLOC_FREE(event);
    }
}


