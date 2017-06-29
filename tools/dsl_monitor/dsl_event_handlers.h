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

#ifndef __DSL_EVENT_HANDLERS_H__
#define __DSL_EVENT_HANDLERS_H__

#include "dsl_context.h"
#include "dsl_event.h"

void dsl_event_line_up_handler(dsl_context_t * const context);
void dsl_event_line_down_handler(dsl_context_t * const context);
void dsl_event_connecting_timeout_handler(dsl_context_t * const context);
void dsl_event_delay_up_timeout_handler(dsl_context_t * const context);
void dsl_event_disconnecting_timeout_handler(dsl_context_t * const context);
void dsl_event_ping_response_timeout_handler(dsl_context_t * const context);
void dsl_event_connect_message_handler(dsl_context_t * const context);
void dsl_event_disconnect_message_handler(dsl_context_t * const context);
void dsl_event_connected_message_handler(dsl_context_t * const context);
void dsl_event_disconnected_message_handler(dsl_context_t * const context);
void dsl_event_update_message_handler(dsl_context_t * const context);
void dsl_event_ping_message_handler(dsl_context_t * const context, dsl_event_t * const event);

#endif // __DSL_EVENT_HANDLERS_H__
