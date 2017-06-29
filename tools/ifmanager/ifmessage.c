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
#include "ifmanager.h"
#include <messages.h>
#include <stdlib.h>
#include <string.h>

#ifdef _CUNIT_
int unit_test_result = 0;
#endif

STATIC_FUNCTION ifm_event_t * ifm_alloc_event(ifm_event_code_t code, void * const info, size_t info_size)
{
    ifm_event_t * const event = malloc(sizeof *event + info_size);

    if (event == NULL) goto done;

    event->code = code;
    /* take a copy of any event specific info */
    if (info != NULL)
    {
        event->info = (event + 1);
        memcpy(event->info, info, info_size);
    }
    else
    {
        event->info = NULL;
    }

done:
    return event;
}

int ifm_send_event(ifm_event_code_t code, void * const info, size_t info_size)
{
    int result = -1;
    ifm_config_t const * const context = ifm_get_context();
    ifm_event_t * const event = ifm_alloc_event(code, info, info_size);

    if (event == NULL) goto done;

    result = send_event_to_event_manager(context->event_manager, event);
    if (result != 0)
    {
        free(event);
        ifm_error("could not send event to event manager\n");
    }

done:
    return result;
}

STATIC_FUNCTION int ifm_send_pong(ping_info_st const * const pinfo)
{
    return send_pong_message(pinfo->pong_destination, pinfo->message_id);
}

static int send_event_with_interface_info(message_codes_t const message_code, ifm_event_code_t const event_code, interface_info_st const * const pinfo)
{
    ifm_if_info_t if_info;

    if_info.instance = pinfo->instance;
    if_info.interface = pinfo->interface;
    if_info.name = interface_get_name_from_type(pinfo->interface);
    ifm_debug("sent message %d %s %zu\n", message_code, if_info.name, if_info.instance);

    return ifm_send_event(event_code, &if_info, sizeof if_info);
}

int ifm_message_callback(int fd, size_t const instance, message_codes_t const message_code, void * * const info, size_t const info_size)
{
    int result = -1;

    /* All messages we currently handle expect additional info. This may not always be the case. */
    if ((info == NULL) || (*info == NULL) || (info_size == 0))
    {
        ifm_debug("empty additional info: %d\n", message_code);
        goto done;
    }

    switch (message_code)
    {
        case ping_message:
            result = ifm_send_pong(*info);
            break;

        case connected_message:
            result = send_event_with_interface_info(message_code, ifm_event_code_connected, *info);
            break;

        case disconnected_message:
            result = send_event_with_interface_info(message_code, ifm_event_code_disconnected, *info);
            break;

        case update_message:
        {
            ifm_update_info_t update_info;
            update_info_st const * const pinfo = *info;

            update_info.instance = pinfo->instance;
            update_info.config_group = pinfo->config_group;
            update_info.name = get_config_group_name_from_id(pinfo->config_group);
            ifm_debug("received message %d %s %zu\n", message_code, update_info.name, update_info.instance);

            result = ifm_send_event(ifm_event_code_update, &update_info, sizeof update_info);
            break;
        }

        case joined_bridge_message:
            result = send_event_with_interface_info(message_code, ifm_event_code_joined_bridge, *info);
            break;

        case left_bridge_message:
            result = send_event_with_interface_info(message_code, ifm_event_code_left_bridge, *info);
            break;

        case dhcp_assigned_gateway:
        {
            dhcp_assigned_gateway_st gateway_info;
            dhcp_assigned_gateway_st const * const pinfo = *info;

            gateway_info = *pinfo;
            result = ifm_send_event(ifm_event_code_dhcp_assigned_gateway, &gateway_info, sizeof gateway_info);
            break;
        }
        default:
            ifm_debug("unexpected message: %d\n", message_code);
            break;
    }

done:
    return result;
}

void ifm_timeout_callback(void * context)
{
    failover_info_t * const failover = context;

    if (failover != NULL)
    {
        failover_event_info_t failover_event = {
                    .failover = failover,
                    .instance = failover->instance,
        };

        int const result = ifm_send_event(ifm_event_code_timer_timeout, &failover_event, sizeof failover_event);

#if IFMANAGER_DEBUG
        ifm_debug("failover%d timer timeout (%d).\n",  failover->instance, result);
#else
        (void)result;
#endif
    }
}

void ifm_probe_timeout_callback(void * context)
{
    failover_info_t * const failover = context;

    if (failover != NULL)
    {
        failover_event_info_t failover_event = {
                    .failover = failover,
                    .instance = failover->instance,
        };

        int const result = ifm_send_event(ifm_event_code_probe_timeout, &failover_event, sizeof failover_event);

#if IFMANAGER_DEBUG
        ifm_debug("failover%d probe timeout (%d).\n", failover->instance, result);
#else
        (void)result;
#endif
    }
}

void ifm_probe_interval_timeout_callback(void * context)
{
    failover_info_t * const failover = context;

    if (failover != NULL)
    {
        failover_event_info_t failover_event = {
                    .failover = failover,
                    .instance = failover->instance,
        };

        int const result = ifm_send_event(ifm_event_code_probe_interval_timeout, &failover_event, sizeof failover_event);

#if IFMANAGER_DEBUG
        ifm_debug("failover%d probe interval timeout (%d).\n", failover->instance, result);
#else
        (void)result;
#endif
    }
}

void ifm_switch_primary_up_timeout_callback(void * context)
{
    failover_info_t * const failover = context;

    if (failover != NULL)
    {
        failover_event_info_t failover_event = {
                    .failover = failover,
                    .instance = failover->instance,
        };

        int const result = ifm_send_event(ifm_event_code_switch_primary_up_timeout, &failover_event, sizeof failover_event);

#if IFMANAGER_DEBUG
        ifm_debug("failover%d switch-primary-up timeout (%d).\n", failover->instance, result);
#else
        (void)result;
#endif
    }
}

void ifm_show_failover_plain_socket_callback(int const socket_fd, void * const user_context)
{
    ifm_debug("EVENT ======= %d for %s:%zu\n", ifm_event_code_show, "failover show plain socket", socket_fd);
    if (ifm_get_context() != NULL)
    {
        int const result = ifm_send_event(ifm_event_code_show, (void * const) &socket_fd, sizeof socket_fd);

#if IFMANAGER_DEBUG
        ifm_debug("failover ======= %d ifm_event_code_show (%d).\n", socket_fd, result);
#else
        (void)result;
#endif
    }
}
