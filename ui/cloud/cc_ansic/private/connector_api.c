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

/*  This was moved here to correct a software engineering defect.
 * These defines need to be private
 */

#include <stddef.h>

#define CONNECTOR_CONST_PROTECTION

#if (defined UNIT_TEST)
#define STATIC
#else
#define STATIC      static
#endif

/*  WARNING: connector_api.h must be the first connector_* header file
 * to guarantee CONNECTOR_VERSION is properly applied to all files */
#include "connector_api.h"
#include "connector_debug.h"

#include "connector_info.h"
#include "connector_def.h"

#include "chk_config.h"
#include "bele.h"

STATIC connector_status_t notify_error_status(connector_callback_t const callback, connector_class_id_t const class_number, connector_request_id_t const request_number, connector_status_t const status, void * const context);
#include "os_intf.h"
#include "connector_global_config.h"

STATIC connector_status_t connector_stop_callback(connector_data_t * const connector_ptr, connector_transport_t const transport, void * const user_context);
#if !(defined CONNECTOR_NETWORK_TCP_START) || (defined CONNECTOR_TRANSPORT_UDP) || defined (CONNECTOR_TRANSPORT_SMS)
STATIC connector_status_t get_config_connect_status(connector_data_t * const connector_ptr, connector_request_id_config_t const request_id, connector_config_connect_type_t * const config_ptr);
#endif

#if (defined CONNECTOR_DATA_POINTS)
#include "connector_data_point.h"
#endif

#if (defined CONNECTOR_TRANSPORT_TCP)
#include "connector_edp.h"
#endif

#if (defined CONNECTOR_SHORT_MESSAGE)
#include "connector_sm.h"
#endif

#ifdef CONNECTOR_NO_MALLOC
#include "connector_static_buffer.h"
#endif

#define DEVICE_ID_LENGTH 16


#if !(defined CONNECTOR_NETWORK_TCP_START) || (defined CONNECTOR_TRANSPORT_UDP) || defined (CONNECTOR_TRANSPORT_SMS)
STATIC connector_status_t get_config_connect_status(connector_data_t * const connector_ptr,
                                                        connector_request_id_config_t const config_request_id,
                                                        connector_config_connect_type_t * const config_connect)
{
    connector_status_t result = connector_working;

    ASSERT(config_connect != NULL);
    ASSERT((config_request_id == connector_request_id_config_network_tcp) ||
           (config_request_id == connector_request_id_config_network_udp) ||
           (config_request_id == connector_request_id_config_network_sms));

    config_connect->type = connector_connect_auto;

    {
        connector_callback_status_t status;
        connector_request_id_t request_id;

        request_id.config_request = config_request_id;
        status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, config_connect, connector_ptr->context);

        switch (status)
        {
        case connector_callback_continue:
            switch (config_connect->type)
            {
            case connector_connect_auto:
            case connector_connect_manual:
                break;
            default:
                notify_error_status(connector_ptr->callback, connector_class_id_config, request_id, connector_invalid_data_range, connector_ptr->context);
                result = connector_abort;
                break;
            }
            break;

        case connector_callback_busy:
        case connector_callback_abort:
        case connector_callback_error:
            result = connector_abort;
            break;

        case connector_callback_unrecognized:
            break;
        }
    }
    return result;
}
#endif

STATIC connector_status_t manage_device_id(connector_data_t * const connector_ptr)
{
    connector_status_t result;
    unsigned int i;
    connector_ptr->connector_got_device_id = connector_false;

    result = get_config_device_id(connector_ptr);
    COND_ELSE_GOTO(result == connector_working, error);
    /* If the returned Device ID is zero, Cloud Connector will ask the Device Cloud for a Device ID. */
    connector_ptr->connector_got_device_id = connector_false;
    for (i = 0; i < DEVICE_ID_LENGTH; i++)
    {
        if (connector_ptr->device_id[i] != 0)
        {
            connector_ptr->connector_got_device_id = connector_true;
            break;
        }
    }

error:
    return result;
}

STATIC connector_status_t connector_stop_callback(connector_data_t * const connector_ptr, connector_transport_t const transport, void * const user_context)
{
    connector_status_t result = connector_working;

    connector_initiate_stop_request_t stop_request;

    connector_request_id_t request_id;
    request_id.status_request = connector_request_id_status_stop_completed;

    stop_request.transport = transport;
    stop_request.user_context = user_context;

    {
        connector_callback_status_t const status =  connector_callback(connector_ptr->callback, connector_class_id_status, request_id, &stop_request, connector_ptr->context);

        switch (status)
        {
        case connector_callback_continue:
        case connector_callback_unrecognized:
            break;
        case connector_callback_busy:
            result = connector_pending;
            break;
        default:
            result = connector_abort;
            break;
        }
    }

    connector_debug_line("connector_stop_callback: %s", transport_to_string(transport));
    return result;
}


#define CONNECTOR_IS_STOP(state, value)    ((state) == (value))

STATIC connector_bool_t is_connector_stopped(connector_data_t * const connector_ptr, connector_close_status_t const close_status)
{
    int count = 0;
    connector_transport_state_t wait_state = (close_status == connector_close_status_device_stopped) ? connector_transport_idle : connector_transport_terminate;

#if (defined CONNECTOR_TRANSPORT_UDP)
    if (!connector_bool(CONNECTOR_IS_STOP(connector_ptr->sm_udp.transport.state, wait_state))) count++;
#endif

#if (defined CONNECTOR_TRANSPORT_SMS)
    if (!connector_bool(CONNECTOR_IS_STOP(connector_ptr->sm_sms.transport.state, wait_state))) count++;
#endif

#if (defined CONNECTOR_TRANSPORT_TCP)
    if (!connector_bool(CONNECTOR_IS_STOP(edp_get_active_state(connector_ptr), wait_state))) count++;
#endif

    return connector_bool(count == 0);
}

STATIC void abort_connector(connector_data_t * const connector_ptr)
{
    switch (connector_ptr->stop.state)
    {
        case connector_state_terminate_by_initiate_action:
        case connector_state_abort_by_callback:
            /* already shutting down - nothing to do here. */
            break;

        default:
        {
            connector_status_t status;

#if (defined CONNECTOR_TRANSPORT_UDP) ||(defined CONNECTOR_TRANSPORT_SMS)
            status = sm_initiate_action(connector_ptr, connector_initiate_terminate, NULL);
            if (status != connector_success)
                connector_debug_line("abort_connector: sm_initiate_action returns error %d", status);

#if (defined CONNECTOR_TRANSPORT_UDP)
            connector_ptr->sm_udp.close.status = connector_close_status_abort;
#endif

#if (defined CONNECTOR_TRANSPORT_SMS)
            connector_ptr->sm_sms.close.status = connector_close_status_abort;
#endif
#endif


#if (defined CONNECTOR_TRANSPORT_TCP)
            status = edp_initiate_action(connector_ptr, connector_initiate_terminate, NULL);
            if (status != connector_success)
                connector_debug_line("abort_connector: edp_initiate_action returns error %d", status);

            edp_set_close_status(connector_ptr, connector_close_status_abort);
#endif

            connector_ptr->stop.state = connector_state_abort_by_callback;
            break;
        }
    }
}

connector_handle_t connector_init(connector_callback_t const callback, void * const context)
{
    connector_data_t * connector_handle = NULL;
    connector_status_t status;

#if (defined CONNECTOR_SW_DESCRIPTION)
    connector_debug_line("Cloud Connector v%s %s", CONNECTOR_SW_VERSION, CONNECTOR_SW_DESCRIPTION);
#else
    connector_debug_line("Cloud Connector v%s", CONNECTOR_SW_VERSION);
#endif

    {
        void * handle;

#if (defined CONNECTOR_NO_MALLOC)
        status = malloc_data_buffer(NULL, sizeof *connector_handle, named_buffer_id(connector_data), &handle);
#else
        status = malloc_cb(callback, sizeof *connector_handle, &handle, context);
#endif

        COND_ELSE_GOTO(status == connector_working, done);
        memset(handle, 0x00, sizeof *connector_handle); /* Init structure, all pointers to NULL */
        connector_handle = handle;
    }

    connector_handle->callback = callback;
    connector_handle->context = context;

    status = manage_device_id(connector_handle);
    COND_ELSE_GOTO(status == connector_working, error);

    /* make a copy of the cloud url */
#if (defined CONNECTOR_TRANSPORT_TCP) || (defined CONNECTOR_TRANSPORT_UDP)
#if (defined CONNECTOR_CLOUD_URL)
    {
        static char const connector_device_cloud_url[]= CONNECTOR_CLOUD_URL;
        connector_handle->device_cloud_url = (char *)connector_device_cloud_url;
        connector_handle->device_cloud_url_length = sizeof connector_device_cloud_url -1;
    }
#else
    status = get_config_device_cloud_url(connector_handle);
    COND_ELSE_GOTO(status == connector_working, error);
#endif
#endif /* (defined CONNECTOR_TRANSPORT_TCP) || (defined CONNECTOR_TRANSPORT_UDP) */

#if (defined CONNECTOR_TRANSPORT_SMS)
#if (defined CONNECTOR_CLOUD_PHONE)
    {
        static char const connector_device_cloud_phone[]= CONNECTOR_CLOUD_PHONE;
        connector_handle->device_cloud_phone = connector_device_cloud_phone;
        connector_handle->device_cloud_phone_length = sizeof connector_device_cloud_phone -1;
    }
#endif

#if (defined CONNECTOR_CLOUD_SERVICE_ID)
    {
        static char const connector_device_cloud_service_id[]= CONNECTOR_CLOUD_SERVICE_ID;
        connector_handle->device_cloud_service_id = connector_device_cloud_service_id;
        connector_handle->device_cloud_service_id_length = sizeof connector_device_cloud_service_id -1;
    }
#endif
#endif /* (defined CONNECTOR_TRANSPORT_SMS) */

#if (defined CONNECTOR_TRANSPORT_TCP)
#if !(defined CONNECTOR_NETWORK_TCP_START)
    {
        connector_config_connect_type_t config_connect;

        status = get_config_connect_status(connector_handle, connector_request_id_config_network_tcp, &config_connect);
        COND_ELSE_GOTO(status == connector_working, done);

        connector_handle->edp_data.connect_type = config_connect.type;
        connector_handle->edp_data.stop.auto_connect = connector_bool(config_connect.type == connector_connect_auto);
    }
#else
    ASSERT((CONNECTOR_NETWORK_TCP_START == connector_connect_auto) || (CONNECTOR_NETWORK_TCP_START == connector_connect_manual));
    if (CONNECTOR_NETWORK_TCP_START == connector_connect_auto)
    {
        edp_set_active_state(connector_handle, connector_transport_open);
    }
    connector_handle->edp_data.stop.auto_connect = connector_bool(CONNECTOR_NETWORK_TCP_START == connector_connect_auto);
#endif
#endif

#if (defined CONNECTOR_SHORT_MESSAGE)
    connector_handle->last_request_id = SM_DEFAULT_REQUEST_ID;

#if defined CONNECTOR_TRANSPORT_UDP
    {
        connector_sm_data_t * const sm_ptr = get_sm_data(connector_handle, connector_transport_udp);

        #if !(defined CONNECTOR_NETWORK_UDP_START)
        {
            connector_config_connect_type_t config_connect;

            status = get_config_connect_status(connector_handle, connector_request_id_config_network_udp, &config_connect);
            ASSERT_GOTO(status == connector_working, error);

            sm_ptr->transport.connect_type = config_connect.type;
        }
        #else
        ASSERT((CONNECTOR_NETWORK_UDP_START == connector_connect_auto) || (CONNECTOR_NETWORK_UDP_START == connector_connect_manual));
        sm_ptr->transport.connect_type = CONNECTOR_NETWORK_UDP_START;
        status = connector_working;
        #endif
        if (sm_ptr->transport.connect_type == connector_connect_auto)
        {
            status = sm_initialize(connector_handle, connector_transport_udp);
            COND_ELSE_GOTO(status == connector_working, error);
        }
    }
#endif

#if defined CONNECTOR_TRANSPORT_SMS
    {
        connector_sm_data_t * const sm_ptr = get_sm_data(connector_handle, connector_transport_sms);

        #if !(defined CONNECTOR_NETWORK_SMS_START)
        {
            connector_config_connect_type_t config_connect;

            status = get_config_connect_status(connector_handle, connector_request_id_config_network_sms, &config_connect);
            ASSERT_GOTO(status == connector_working, error);

            sm_ptr->transport.connect_type = config_connect.type;
        }
        #else
        ASSERT((CONNECTOR_NETWORK_SMS_START == connector_connect_auto) || (CONNECTOR_NETWORK_SMS_START == connector_connect_manual));
        sm_ptr->transport.connect_type = CONNECTOR_NETWORK_SMS_START;
        status = connector_working;
        #endif
        if (sm_ptr->transport.connect_type == connector_connect_auto)
        {
            status = sm_initialize(connector_handle, connector_transport_sms);
            COND_ELSE_GOTO(status == connector_working, error);
        }
    }
#endif

#endif

#if (defined CONNECTOR_TRANSPORT_COUNT > 1)
    connector_handle->first_running_network = (connector_network_type_t) 0;
#endif
#if (defined CONNECTOR_DATA_POINTS)
    connector_handle->process_csv = connector_true;
#endif

    goto done;

error:
    free_data_buffer(connector_handle, named_buffer_id(connector_data), connector_handle);
    connector_handle = NULL;

done:
    return connector_handle;
}


connector_status_t connector_step(connector_handle_t const handle)
{
    connector_status_t result = connector_init_error;
    connector_data_t * const connector_ptr = handle;

    ASSERT_GOTO(handle != NULL, done);

    switch (connector_ptr->stop.state)
    {
        case connector_state_running:
            break;
        case connector_state_stop_by_initiate_action:
            if (is_connector_stopped(connector_ptr, connector_close_status_device_stopped))
            {
                result = connector_stop_callback(connector_ptr, connector_transport_all, connector_ptr->stop.user_context);
                if (result == connector_abort)
                {
                    goto error;
                }
                else if (result == connector_working)
                {
                    connector_ptr->stop.state = connector_state_running;
                }
            }
            break;
        case connector_state_abort_by_callback:
        case connector_state_terminate_by_initiate_action:
            if (is_connector_stopped(connector_ptr, connector_close_status_device_terminated))
            {
                connector_ptr->connector_got_device_id = connector_false; /* TODO, Probably this should not be done with provisioning! */
                result = (connector_ptr->stop.state == connector_state_terminate_by_initiate_action) ? connector_device_terminated : connector_abort;
                connector_debug_line("connector_step: free Cloud Connector");
#if (defined CONNECTOR_RCI_SERVICE && !defined CONNECTOR_NO_MALLOC)
                free_rci_internal_data(connector_ptr);
#endif
                free_data_buffer(connector_ptr, named_buffer_id(connector_data), connector_ptr);
                goto done;
            }
            break;
        default:
            break;
    }

#if (CONNECTOR_TRANSPORT_COUNT == 1)
#if (defined CONNECTOR_TRANSPORT_TCP)
    result = connector_edp_step(connector_ptr);
#endif
#if (defined CONNECTOR_TRANSPORT_UDP)
    result = connector_udp_step(connector_ptr);
#endif
#if (defined CONNECTOR_TRANSPORT_SMS)
    result = connector_sms_step(connector_ptr);
#endif

#else
    {
#define next_network(current_network) (((current_network) == last_network_index) ? first_network_index : (connector_network_type_t) ((current_network) + 1))

        connector_network_type_t const first_network_index = (connector_network_type_t) 0;
        connector_network_type_t const last_network_index = (connector_network_type_t) (connector_network_count - 1);
        connector_network_type_t const first_network_checked = connector_ptr->first_running_network;
        connector_network_type_t current_network = first_network_checked;

        do
        {
            connector_status_t (*step_func)(connector_data_t * const connector_ptr);

            switch (current_network)
            {
#if (defined CONNECTOR_TRANSPORT_TCP)
            case connector_network_tcp: step_func = connector_edp_step; break;
#endif
#if (defined CONNECTOR_TRANSPORT_UDP)
            case connector_network_udp: step_func = connector_udp_step; break;
#endif
#if (defined CONNECTOR_TRANSPORT_SMS)
            case connector_network_sms: step_func = connector_sms_step; break;
#endif
            default:
                ASSERT(connector_false);
                result = connector_abort;
                goto error; break;
            }

            result = step_func(connector_ptr);

            current_network = next_network(current_network);
        } while ((current_network != first_network_checked) && (result == connector_idle));

        connector_ptr->first_running_network = (result == connector_idle) ? next_network(first_network_checked): current_network;

#undef next_network
    }
#endif

error:
    switch (result)
    {
    case connector_abort:
        abort_connector(connector_ptr);
        /* no break; */
    case connector_device_terminated:
        result = connector_working;
        break;
    default:
        break;
    }

done:
    return result;
}

connector_status_t connector_run(connector_handle_t const handle)
{
    connector_status_t rc;

    do {
        rc = connector_step(handle);

        if (rc == connector_idle || rc == connector_working || rc == connector_pending || rc == connector_active || rc == connector_success)
        {
            if (yield_process(handle, rc) != connector_working)
            {
                abort_connector(handle);
                rc = connector_success;
            }
        }

    } while (rc == connector_idle || rc == connector_working || rc == connector_pending || rc == connector_active || rc == connector_success);

    return rc;
}

connector_status_t connector_initiate_action(connector_handle_t const handle, connector_initiate_request_t const request, void const * const request_data)
{
    connector_status_t result = connector_init_error;
    connector_data_t * connector_ptr = (connector_data_t *)handle;

    ASSERT_GOTO(handle != NULL, done);

    switch (request)
    {
    case connector_initiate_terminate:

#if (defined CONNECTOR_TRANSPORT_TCP)
        result = edp_initiate_action(connector_ptr, request, request_data);
        COND_ELSE_GOTO(result == connector_success, done);
#endif

#if (defined CONNECTOR_SHORT_MESSAGE)
        result = sm_initiate_action(connector_ptr, request, request_data);
        COND_ELSE_GOTO(result == connector_success, done);
#endif

        connector_ptr->stop.state = connector_state_terminate_by_initiate_action;
        result = connector_success;
        break;

   default:
        if (connector_ptr->stop.state == connector_state_terminate_by_initiate_action)
        {
            result = connector_device_terminated;
            goto done;
        }

        {
            connector_transport_t const * const transport = request_data;

            if (transport == NULL)
            {
                result = connector_invalid_data;
                goto done;
            }

            switch (*transport)
            {
            case connector_transport_all:
                if (request != connector_initiate_transport_stop)
                {
                    result = connector_invalid_data;
                    goto done;
                }

                if (connector_ptr->stop.state != connector_state_running)
                {
                    /* already in close state */
                    result = (connector_ptr->stop.state == connector_state_terminate_by_initiate_action) ? connector_device_terminated: connector_service_busy;
                    goto done;
                }

                /* no break */

#if (defined CONNECTOR_SHORT_MESSAGE)
#if (defined CONNECTOR_TRANSPORT_UDP)
            case connector_transport_udp:
#endif
#if (defined CONNECTOR_TRANSPORT_SMS)
            case connector_transport_sms:
#endif
                result = sm_initiate_action(connector_ptr, request, request_data);

                if (*transport != connector_transport_all)  break;
                else if (result != connector_success) break;
                else if (request != connector_initiate_transport_stop) break;
                /* no break; */
#endif

#if (defined CONNECTOR_TRANSPORT_TCP)
            case connector_transport_tcp:
                result = edp_initiate_action(connector_ptr, request, request_data);

                if (*transport != connector_transport_all)  break;
                else if (result != connector_success) break;
                else if (request != connector_initiate_transport_stop) break;
#endif
                {
                    connector_initiate_stop_request_t const * const stop_request = request_data;
                    connector_ptr->stop.condition = stop_request->condition;
                    connector_ptr->stop.user_context = stop_request->user_context;
                    connector_ptr->stop.state = connector_state_stop_by_initiate_action;
                    result = connector_success;
                }
                break;

            default:
                result = connector_invalid_data;
                goto done;
            }
        }
    }
done:
    return result;
}

