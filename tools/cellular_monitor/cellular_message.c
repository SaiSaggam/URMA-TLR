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
#include "cellular_config.h"
#include "cellular_message.h"
#include "cellular_modem.h"
#include "monitor_debug.h"

int monitor_message_callback(int fd, size_t const instance, message_codes_t const message_code, void * * const info, size_t const info_size)
{
    int result;

    //monitor_debug_printf("monitor_message_callback: pid = %u ppid = %u\n", getpid(), getppid());

    monitor_debug_printf("monitor_message_callback: cellular%zu/%d %zu fd %d %p\n", instance, interface_instance_max, message_code, fd, cellular_contexts);
    switch (message_code)
    {
        case query_connect:
            if (instance > 0 && instance <= interface_instance_max)
            {

                cellular_context_t const * const context = &cellular_contexts[INSTANCE_TO_INDEX(instance)];
                size_t const active_sim = context->modem_status->active_sim;


                if (is_current_context_active_sim(context, active_sim) &&
                    context->modem_status->connect_status == cellular_connected)
                {
                    dprintf(fd, "connected.");
                }
                else
                {
                    dprintf(fd, "disconnected.");
                }
                result = 0;
            }
            else
            {
                result = -1;
            }
            show_cellular_debug(instance);
            break;
        case connect_message:
        case disconnect_message:
        case update_message:
//            cellular_debug_priority = (tlr_log_t)tlr_get_config_loglevel(config_cellular);
            /* no break; */
        default:
            result = send_message_to_cellular_monitor(instance, message_code, info, info_size);
            break;
    }

    return result;
}

