/*
 * Copyright (c) 2014 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */

#include <stddef.h>

#include "connector_edp_util.h"

#include "connector_tcp_config.h"
#include "connector_tcp_send.h"
#include "connector_tcp_recv.h"
#include "connector_tcp_open.h"
#include "connector_tcp_close.h"

#include "connector_cc.h"

#if (defined CONNECTOR_DATA_SERVICE) || (defined CONNECTOR_FILE_SYSTEM) || (defined CONNECTOR_RCI_SERVICE)
#include "connector_msg.h"
#endif
#if (defined CONNECTOR_DATA_SERVICE)
#include "connector_data_service.h"
#endif
#if (defined CONNECTOR_FILE_SYSTEM)
#include "connector_file_system.h"
#endif

#if (defined CONNECTOR_FIRMWARE_SERVICE)
#include "connector_firmware.h"
#endif

#include "layer.h"


STATIC connector_status_t edp_config_init(connector_data_t * const connector_ptr)
{
    connector_status_t result;

    result = get_config_connection_type(connector_ptr);
    COND_ELSE_GOTO(result == connector_working, done);

    switch (connector_ptr->connection_type)
    {
    case connector_connection_type_lan:
        break;
    case connector_connection_type_wan:
#if (!defined CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND)
        result = get_config_link_speed(connector_ptr);
        COND_ELSE_GOTO(result == connector_working, done);
#endif

#if (!defined CONNECTOR_WAN_PHONE_NUMBER_DIALED)
        result = get_config_phone_number(connector_ptr);
        COND_ELSE_GOTO(result == connector_working, done);
#endif

        break;
    }

#if !(defined CONNECTOR_VENDOR_ID)
    result = get_config_vendor_id(connector_ptr);
    COND_ELSE_GOTO(result == connector_working, done);
#endif

#if !(defined CONNECTOR_DEVICE_TYPE)
    result = get_config_device_type(connector_ptr);
    COND_ELSE_GOTO(result == connector_working, done);
#endif

#if !(defined CONNECTOR_IDENTITY_VERIFICATION)
    result = get_config_identity_verification(connector_ptr);
    COND_ELSE_GOTO(result == connector_working, done);
    {
        connector_identity_verification_t const identity_verification = connector_ptr->edp_data.config.identity_verification;
#else
    {
        connector_identity_verification_t const identity_verification = CONNECTOR_IDENTITY_VERIFICATION;
#endif
        ASSERT((identity_verification == connector_identity_verification_simple) || (identity_verification == connector_identity_verification_password));
        if (identity_verification == connector_identity_verification_password)
        {
            /* get password for password identity verification */
            result =  get_config_password(connector_ptr);
            COND_ELSE_GOTO(result == connector_working, done);
        }
    }

    result = connector_working;

done:
    return result;
}


STATIC connector_status_t connector_edp_init(connector_data_t * const connector_ptr)
{
    connector_status_t result;

    edp_reset_initial_data(connector_ptr);
    connector_ptr->edp_data.facilities.supported_mask = 0;

    result = edp_config_init(connector_ptr);
    if (result != connector_working)
    {
        goto done;
    }

    result = edp_layer_get_supported_facilities(connector_ptr);
    if (result != connector_working)
    {
        goto done;
    }

    result = edp_layer_initialize_facilities(connector_ptr);
    if (result != connector_working)
    {
        goto done;
    }

done:
    return result;
}


connector_status_t connector_edp_step(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_idle;
    connector_transport_state_t init_active_state = edp_get_active_state(connector_ptr);

    while (result == connector_idle)
    {
        connector_transport_state_t const edp_current_active_state = edp_get_active_state(connector_ptr);

        switch (edp_current_active_state)
        {
        case connector_transport_idle:
            if (connector_ptr->edp_data.stop.auto_connect)
            {
                connector_ptr->edp_data.network_handle = CONNECTOR_NETWORK_HANDLE_NOT_INITIALIZED;
                edp_set_active_state(connector_ptr, connector_transport_open);
            }

            goto check_state;

        case connector_transport_open:
            result = edp_tcp_open_process(connector_ptr);
            break;

        case connector_transport_send:
            result = edp_tcp_send_process(connector_ptr);
            if (edp_get_active_state(connector_ptr) == edp_current_active_state)
            {
                edp_set_active_state(connector_ptr, connector_transport_receive);
            }
            break;
        case connector_transport_receive:
            result = edp_tcp_receive_process(connector_ptr);
            if (edp_get_active_state(connector_ptr) == edp_current_active_state)
            {
                edp_set_active_state(connector_ptr, connector_transport_send);
            }
            break;
        case connector_transport_close:
        case connector_transport_terminate:
            result = edp_close_process(connector_ptr);
            break;

        case connector_transport_redirect:
            result = edp_redirect_process(connector_ptr);
           break;

        case connector_transport_wait_for_reconnect:
        {
            if (connector_ptr->edp_data.connect_at == 0)
            {
                connector_debug_line("Waiting %d second before reconnecting TCP transport", CONNECTOR_TRANSPORT_RECONNECT_AFTER);
                result = get_system_time(connector_ptr, &connector_ptr->edp_data.connect_at);
                if (result != connector_working)
                    goto done;
                connector_ptr->edp_data.connect_at += CONNECTOR_TRANSPORT_RECONNECT_AFTER;
            } else {
                unsigned long int uptime;

                result = get_system_time(connector_ptr, &uptime);
                if (result != connector_working)
                    goto done;
                if (uptime >= connector_ptr->edp_data.connect_at)
                    edp_set_active_state(connector_ptr, connector_transport_open);
            }
            break;
        }
        }

        if (result == connector_device_terminated || result == connector_abort)
        {
            connector_debug_line("connector_edp_step: done with status = %d", result);
            goto done;
        }


        /* done all cases */
        if (edp_get_active_state(connector_ptr) == init_active_state) break;
    }

check_state:
    switch (edp_get_initiate_state(connector_ptr))
    {
        case connector_transport_close:
            switch (edp_get_active_state(connector_ptr))
            {
                case connector_transport_idle:
                    connector_ptr->edp_data.stop.auto_connect = connector_false;
                    edp_set_initiate_state(connector_ptr, connector_transport_idle);

                    if (edp_is_stop_request(connector_ptr))
                    {
                        result = connector_stop_callback(connector_ptr, connector_transport_tcp, connector_ptr->edp_data.stop.user_context);

                        if (result == connector_pending)
                        {
                            /* restore the initiate state */
                            edp_set_initiate_state(connector_ptr, connector_transport_close);
                        }
                        else if (result == connector_working)
                        {
                            edp_clear_stop_request(connector_ptr);
                        }
                    }
                    /* exit edp so let connector_step does next step */
                    goto done;

                case connector_transport_close:
                    connector_ptr->edp_data.stop.auto_connect = connector_false;
                    /* no break */
                case connector_transport_terminate:
                    break;

                default:
                    if (!tcp_is_send_active(connector_ptr) && result == connector_idle)
                    {
                        edp_set_stop_condition(connector_ptr, connector_stop_immediately);
                    }
                    if (edp_is_stop_immediately(connector_ptr))
                    {
                        connector_ptr->edp_data.stop.auto_connect = connector_false;
                        edp_set_close_status(connector_ptr, connector_close_status_device_stopped);
                        edp_set_active_state(connector_ptr, connector_transport_close);
                    }
                    break;
            }
            break;
        case connector_transport_open:
            switch (edp_get_active_state(connector_ptr))
            {
                case connector_transport_terminate:
                    break;
                case connector_transport_close:
                case connector_transport_idle:
                    if (result != connector_open_error)
                        edp_set_active_state(connector_ptr, connector_transport_open);
                    else
                        edp_set_initiate_state(connector_ptr, connector_transport_idle);
                    break;
                default:
                    edp_set_initiate_state(connector_ptr, connector_transport_idle);
                    break;
            }
            break;

        case connector_transport_terminate:
            edp_set_close_status(connector_ptr, connector_close_status_device_terminated);
            edp_set_active_state(connector_ptr, connector_transport_close);
            break;
        default:
            break;
    }

done:
    return result;
}

connector_status_t edp_initiate_action(connector_data_t * const connector_ptr, connector_initiate_request_t const request, void const * const request_data)
{
    connector_status_t result = connector_init_error;

    switch (request)
    {
    case connector_initiate_terminate:
        UNUSED_PARAMETER(request_data);
        edp_set_initiate_state(connector_ptr, connector_transport_terminate);
        result = connector_success;
        break;

#if (defined CONNECTOR_DATA_SERVICE)
    case connector_initiate_send_data:
        if (edp_get_edp_state(connector_ptr) == edp_communication_connect_to_cloud || edp_get_edp_state(connector_ptr) == edp_configuration_init)
        {
            goto done;
        }

        switch (edp_get_active_state(connector_ptr))
        {
        case connector_transport_idle:
            result = connector_unavailable;
            goto done;

        case connector_transport_close:
            if (edp_get_initiate_state(connector_ptr) == connector_transport_open)
            {
                break;
            }
            /* no break  to return error */
        case connector_transport_redirect:
            result = connector_unavailable;
            goto done;

        case connector_transport_wait_for_reconnect:
        case connector_transport_terminate:
            goto done;

        case connector_transport_open:
        case connector_transport_send:
        case connector_transport_receive:
            switch (edp_get_initiate_state(connector_ptr))
            {
                case connector_transport_close:
                    result = connector_unavailable;
                    goto done;
                default:
                    break;
            }
            break;
        }
        result = data_service_initiate(connector_ptr, request_data);
        break;
#endif
#if (defined CONNECTOR_DATA_POINTS)
    case connector_initiate_data_point:
    case connector_initiate_data_point_binary:
        if (edp_get_edp_state(connector_ptr) == edp_communication_connect_to_cloud || edp_get_edp_state(connector_ptr) == edp_configuration_init)
        {
            goto done;
        }

        switch (edp_get_active_state(connector_ptr))
        {
        case connector_transport_idle:
            result = connector_unavailable;
            goto done;

        case connector_transport_close:
            if (edp_get_initiate_state(connector_ptr) == connector_transport_open)
            {
                break;
            }
            /* no break  to return error */
        case connector_transport_redirect:
            result = connector_unavailable;
            goto done;

        case connector_transport_wait_for_reconnect:
        case connector_transport_terminate:
            goto done;

        case connector_transport_open:
        case connector_transport_send:
        case connector_transport_receive:
            switch (edp_get_initiate_state(connector_ptr))
            {
                case connector_transport_close:
                    result = connector_unavailable;
                    goto done;
                default:
                    break;
            }
            break;
        }
        switch (request)
        {
            case connector_initiate_data_point:
                result = dp_initiate_data_point(request_data);
                break;
            case connector_initiate_data_point_binary:
                result = dp_initiate_data_point_binary(request_data);
                break;
            /* default: */
            case connector_initiate_transport_start:
            case connector_initiate_transport_stop:
#if (defined CONNECTOR_DATA_SERVICE)
            case connector_initiate_send_data:
#endif
#if (defined CONNECTOR_SHORT_MESSAGE)
            case connector_initiate_ping_request:
            case connector_initiate_session_cancel:
            case connector_initiate_session_cancel_all:
#endif
            case connector_initiate_terminate:
                break;
        }
        break;
#endif

    case connector_initiate_transport_stop:
        switch (edp_get_active_state(connector_ptr))
        {
        case connector_transport_open:
        case connector_transport_send:
        case connector_transport_receive:
        case connector_transport_redirect:
            break;

        case connector_transport_idle:
        case connector_transport_close:
            /* Return busy if start initiate action was called and cannot stop now */
            if (edp_get_initiate_state(connector_ptr) == connector_transport_open)
            {
                result = connector_service_busy;
                goto done;
            }
            break;

        default:
             goto done;
        }

        if (edp_get_initiate_state(connector_ptr) == connector_transport_close &&
            edp_is_stop_request(connector_ptr))
        {
            /* already requesting stop before */
            result = connector_service_busy;
        }
        else
        {
            connector_initiate_stop_request_t const * const stop_request = request_data;

            edp_set_stop_condition(connector_ptr, stop_request->condition);

            if (stop_request->transport == connector_transport_tcp)
            {
                edp_set_stop_request(connector_ptr, stop_request->user_context);
            }

            edp_set_initiate_state(connector_ptr, connector_transport_close);
            result = connector_success;
        }
        break;

    case connector_initiate_transport_start:
        if (request_data == NULL)
        {
            result = connector_invalid_data;
            goto done;
        }

        switch (edp_get_active_state(connector_ptr))
        {
        case connector_transport_open:
        case connector_transport_send:
        case connector_transport_receive:
        case connector_transport_redirect:
            /* Return busy if stop initiate action was called and cannot start now */
            result = (edp_get_initiate_state(connector_ptr) == connector_transport_close) ? connector_service_busy : connector_success;
            break;

        case connector_transport_idle:
        case connector_transport_close:
            if (edp_get_initiate_state(connector_ptr) == connector_transport_close)
            {
                result = connector_service_busy;
            }
            else
            {
                edp_set_initiate_state(connector_ptr, connector_transport_open);
                edp_get_device_cloud(connector_ptr);
                result = connector_success;
            }
            break;

        case connector_transport_wait_for_reconnect:
        case connector_transport_terminate:
            goto done;
        }

        break;

    default:
        ASSERT(connector_false);
        result = connector_abort;
        break;
    }

done:

    return result;
}
