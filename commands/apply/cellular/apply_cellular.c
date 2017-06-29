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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config_def.h"
#include "messages.h"
#include "message_codes.h"
#include "tlr_print.h"
#include "interfaces.h"
#include "config_mgr.h"
#include "digiutils.h"
#include "cellular_status.h"

int main(int argc, char * argv[])
{
    int ret = EXIT_FAILURE;
    size_t instance;
    char const *  ifname = NULL;
    char * log_message = NULL;

    if (cellular_is_pass_through_enabled())
    {
        goto DONE;
    }

    ifname = interface_get_name_from_type(cellular_interface);
    if (ifname == NULL)
        goto ERROR;

    if (get_instance(argc, argv, &instance, config_cellular))
        goto ERROR;

    {
        char msg[strlen(ifname) + sizeof "nnn"];

        snprintf(msg, sizeof msg, "%s %d", ifname, instance);
        if (send_message(cellular_monitor, instance, update_message, msg, NULL) != 0)
        {
            char const to_monitor_failed_log[] = "sent message to cellular monitor failed";

            tlr_event_log(tlr_log_error, "apply cellular", to_monitor_failed_log);

        }
        else
        {
            char const to_monitor_log[] = "sent update_message to cellular monitor";
            log_message = (char *)to_monitor_log;
        }

        if (send_message(interface_manager, INTERFACE_MANAGER_INSTANCE, update_message, msg, NULL) != 0)
        {
            char const to_ifm_failed_log[] = "sent message to interface manager failed";

            tlr_event_log(tlr_log_error, "apply cellular", to_ifm_failed_log);

        }
        else if (log_message == NULL)
        {
            char const to_ifm_log[] = "sent update_message to interface_manager";
            log_message = (char *)to_ifm_log;
        }
        else
        {
            char const to_monitor_ifm_log[] = "sent update_message to cellular monitor and interface manager";
            log_message = (char *)to_monitor_ifm_log;
        }

        if (log_message != NULL)
        {
            tlr_event_log(tlr_log_debug, "apply cellular", log_message);
        }
    }

DONE:
    ret = EXIT_SUCCESS;

ERROR:
    return ret;
}
