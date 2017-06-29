#ifndef __WIFI_MONITOR_EVENT_H__
#define __WIFI_MONITOR_EVENT_H__
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

#include <message_codes.h>
#include <stddef.h>
#include <stdbool.h>
/*
 * The events that can affect the state machine
*/
typedef enum wifi_monitor_event_codes_t
{
    /* private events*/
    PING_RESPONSE_TIMER_EVENT = first_private_message,

    /* global events */
    CONNECT_MESSAGE_EVENT = connect_message,
    DISCONNECT_MESSAGE_EVENT = disconnect_message,
    CONNECTED_MESSAGE_EVENT = connected_message,
    DISCONNECTED_MESSAGE_EVENT = disconnected_message,
    UPDATE_MESSAGE_EVENT = update_message,
	PING_MESSAGE_EVENT = ping_message
} wifi_monitor_event_codes_t;

typedef struct wifi_monitor_event_t
{
	wifi_monitor_event_codes_t code;
	size_t instance;
	void const * info;
    size_t info_size;

} wifi_monitor_event_t;

int send_event_to_wifi_monitor(size_t const instance, int const code, void * * const info, size_t const info_size);
bool wifi_monitor_event_init(size_t const event_queue_size);

#if defined(UNIT_TEST)
void wifi_monitor_event_handler(void * const user_context, void * const data);
#endif

#endif /* __WIFI_MONITOR_EVENT_H__ */
