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

#ifndef __DSL_EVENT_H__
#define __DSL_EVENT_H__

#include "event_manager.h"
#include "message_codes.h"

#include <stddef.h>

typedef enum
{
    dsl_event_line_up = first_private_message,
    dsl_event_line_down,
    dsl_event_connecting_timeout,
    dsl_event_delay_up_timeout,
    dsl_event_disconnecting_timeout,
    dsl_event_ping_response_timeout,

    dsl_event_connect_message = connect_message,
    dsl_event_disconnect_message = disconnect_message,
    dsl_event_connected_message = connected_message,
    dsl_event_disconnected_message = disconnected_message,
    dsl_event_update_message = update_message,
    dsl_event_ping_message = ping_message,
} dsl_event_code_t;

typedef struct
{
	dsl_event_code_t code;
	void const * info;
    size_t info_size;
} dsl_event_t;

void dsl_event_handler(void * const user_context, void * const data);
int dsl_event_send_to_dsl_monitor(event_manager_st * const event_manager, dsl_event_code_t const code, void * * const info, size_t const info_size);

#endif /* __DSL_EVENT_H__ */
