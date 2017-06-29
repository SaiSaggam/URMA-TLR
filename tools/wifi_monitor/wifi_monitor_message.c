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

#include "wifi_monitor_message.h"
#include "wifi_monitor_event.h"
#include "wifi_monitor_log.h"
#include "messages.h"
#include "unittest_support.h"
#include <stdlib.h>

static message_thread_st * message_context;

/*
 * Called whenever a message is received from another process. This function will be called from a
 * new thread.
 */
STATIC_FUNCTION int wifi_monitor_message_callback(int fd, size_t const instance, message_codes_t const message_code, void * * const info, size_t const info_size)
{
    (void)fd;
    return send_event_to_wifi_monitor(instance, message_code, info, info_size);
}

/*
 * Start listening for messages from other processes.
*/
bool wifi_messaging_setup(interfaces_t const interface)
{
    bool result;
    message_destination_t const messaging_destination = (interface == wifi_interface) ? wifi_monitor : wifi5g_monitor;

    message_context = open_messaging_context(messaging_destination, wifi_monitor_message_callback);
    if (message_context == NULL)
    {
        wifi_monitor_debug("could not open messaging context\n");
        result = false;
        goto done;
    }

    result = true;

done:
    return result;
}

/*
 * Needs work. There needs to be a close_messaging_context function that stops this application
 * listening for messages, but as yet there isn't any support for this.
*/
void wifi_messaging_teardown(void)
{
    free(message_context);
    message_context = NULL;
}

