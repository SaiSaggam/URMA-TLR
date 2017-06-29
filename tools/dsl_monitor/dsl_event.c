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

#include "dsl_event.h"
#include "dsl_context.h"
#include "dsl_event_handlers.h"
#include "dsl_led.h"
#include "dsl_log.h"

#include <stdlib.h>

static dsl_event_t * dsl_event_alloc(dsl_event_code_t const code, void * * const info, size_t const info_size)
{
	dsl_event_t * const event = calloc(1, sizeof *event);

	if (event == NULL)
	{
		dsl_error("calloc(%d) failed\n", sizeof *event);
		goto done;
	}

	event->code = code;

    if (info != NULL)
    {
        event->info = *info;
        *info = NULL;
    }

    event->info_size = info_size;

done:
	return event;
}

static void dsl_event_free(dsl_event_t * const event)
{
    if (event != NULL)
    {
        free((void *)event->info);
        free(event);
    }
}

static char const * dsl_event_name_from_code(dsl_event_code_t const code)
{
	char const * name = NULL;

    switch (code)
    {
        case dsl_event_line_up:
            name = "line up";
            break;
        case dsl_event_line_down:
            name = "line down";
            break;
        case dsl_event_connecting_timeout:
            name = "connecting timeout";
            break;
        case dsl_event_delay_up_timeout:
            name = "delay-up timeout";
            break;
        case dsl_event_disconnecting_timeout:
            name = "disconnecting timeout";
            break;
        case dsl_event_ping_response_timeout:
            name = "ping response timeout";
            break;
        case dsl_event_connect_message:
        case dsl_event_disconnect_message:
        case dsl_event_connected_message:
        case dsl_event_disconnected_message:
        case dsl_event_update_message:
        case dsl_event_ping_message:
            name = get_global_message_name_from_code(code);
            break;
    }

	return name;
}

void dsl_event_handler(void * const user_context, void * const data)
{
    dsl_context_t * const context = user_context;
    dsl_event_t * const event = data;

    dsl_debug("event '%s' in state '%s'\n", dsl_event_name_from_code(event->code), dsl_state_name_from_value(context->current_state));

    switch (event->code)
    {
        case dsl_event_line_up:
            dsl_event_line_up_handler(context);
            break;
        case dsl_event_line_down:
            dsl_event_line_down_handler(context);
            break;
        case dsl_event_connecting_timeout:
            dsl_event_connecting_timeout_handler(context);
            break;
        case dsl_event_delay_up_timeout:
            dsl_event_delay_up_timeout_handler(context);
            break;
        case dsl_event_disconnecting_timeout:
            dsl_event_disconnecting_timeout_handler(context);
            break;
        case dsl_event_ping_response_timeout:
            dsl_event_ping_response_timeout_handler(context);
            break;
        case dsl_event_connect_message:
            dsl_event_connect_message_handler(context);
            break;
        case dsl_event_disconnect_message:
            dsl_event_disconnect_message_handler(context);
            break;
        case dsl_event_connected_message:
            dsl_event_connected_message_handler(context);
            break;
        case dsl_event_disconnected_message:
            dsl_event_disconnected_message_handler(context);
            break;
        case dsl_event_update_message:
            dsl_event_update_message_handler(context);
            break;
        case dsl_event_ping_message:
            dsl_event_ping_message_handler(context, event);
            break;
    }

    dsl_event_free(event);

    dsl_led_update_state(context);
}

int dsl_event_send_to_dsl_monitor(event_manager_st * const event_manager, dsl_event_code_t const code, void * * const info, size_t const info_size)
{
	int result = -1;
	dsl_event_t * const event = dsl_event_alloc(code, info, info_size);

	if (event == NULL)
	{
		goto done;
	}

    if (send_event_to_event_manager(event_manager, event) != 0)
    {
        dsl_event_free(event);
    	dsl_error("could not send event to event manager\n");
        goto done;
    }

    result = 0;

done:
	return result;
}

