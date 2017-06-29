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

#ifndef __CELLULAR_EVENT_H__
#define __CELLULAR_EVENT_H__

#include "context.h"

#include "event_manager.h"
#include "messages.h"

#include <stddef.h>


typedef struct
{
    int code;
    size_t instance;
    void const * info;
    size_t info_size;
} cellular_event_t;

int send_connected_event_to_interface_manager(size_t interface_instance);
int send_disconnected_event_to_interface_manager(size_t interface_instance);
int send_message_to_cellular_monitor(size_t const instance, int const event, void * * const info, size_t const info_size);
int send_monitor_event(cellular_context_t * const context, int event, void * * const info, size_t const info_size);
void free_event(cellular_event_t * handler_event);

void modem_status_handler(cellular_context_t * const context, size_t active_sim);
bool is_current_context_active_sim(cellular_context_t const * const context, size_t const active_sim);

#endif /* __CELLULAR_EVENT_H__ */
