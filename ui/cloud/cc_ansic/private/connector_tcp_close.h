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
STATIC connector_status_t layer_remove_facilities(connector_data_t * const connector_ptr, connector_supported_facility_cb_index_t cb_index);

#if (defined CONNECTOR_DEBUG)

static char const * close_status_to_string(connector_close_status_t const value)
{
    char const * result = NULL;
    switch (value)
    {
        enum_to_case(connector_close_status_cloud_disconnected);
        enum_to_case(connector_close_status_cloud_redirected);
        enum_to_case(connector_close_status_device_terminated);
        enum_to_case(connector_close_status_device_stopped);
        enum_to_case(connector_close_status_no_keepalive);
        enum_to_case(connector_close_status_abort);
        enum_to_case(connector_close_status_device_error);

        default:
        {
            connector_debug_line("Unknown close status %d", value);
        }
    }
    return result;
}
#else

#define close_status_to_string(value)       NULL
#endif

STATIC connector_status_t tcp_close_cloud(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_idle;
    connector_close_status_t close_status = edp_get_close_status(connector_ptr);

    if (edp_get_edp_state(connector_ptr) != edp_communication_connect_to_cloud && edp_get_edp_state(connector_ptr) != edp_configuration_init)
    {
        connector_callback_status_t status;
        connector_request_id_t request_id;

        connector_network_close_t close_data;

        close_data.handle = connector_ptr->edp_data.network_handle;
        close_data.status = edp_get_close_status(connector_ptr);
        close_data.reconnect = connector_true;

        connector_debug_line("tcp_close_cloud: status = %s", close_status_to_string(close_data.status));
        request_id.network_request = connector_request_id_network_close;

        status = connector_callback(connector_ptr->callback, connector_class_id_network_tcp, request_id, &close_data, connector_ptr->context);
        ASSERT(status != connector_callback_unrecognized);
        switch (status)
        {
        case connector_callback_busy:
            result = connector_pending;
            goto done;

        case connector_callback_continue:
            connector_ptr->edp_data.network_handle = CONNECTOR_NETWORK_HANDLE_NOT_INITIALIZED;
            result = connector_working;
            break;

        default:
            result = connector_abort;
            close_status = connector_close_status_abort;
            break;
        }


        {
                connector_ptr->edp_data.stop.auto_connect = close_data.reconnect;
                edp_set_active_state(connector_ptr, connector_transport_idle);

                tcp_send_complete_callback(connector_ptr, connector_abort);

        }
        layer_remove_facilities(connector_ptr, facility_callback_cleanup);

        edp_reset_initial_data(connector_ptr);
    }
    else
        edp_set_active_state(connector_ptr, connector_transport_idle);

    switch (close_status)
    {
        case connector_close_status_device_terminated:
        case connector_close_status_abort:
        {
            /*
              * Terminated by connector_dispatch call
              * Free all memory.
              */
            connector_status_t const status = layer_remove_facilities(connector_ptr, facility_callback_delete);
            if (status != connector_working && status != connector_idle)
            {
                connector_debug_line("tcp_close_cloud: layer_remove_facilities failed %d", result);
            }
            edp_set_active_state(connector_ptr, connector_transport_terminate);
            result = (close_status == connector_close_status_device_terminated) ? connector_device_terminated : connector_abort;
            goto done;
        }
        default:
            break;
    }

done:
    return result;
}

STATIC connector_status_t edp_close_process(connector_data_t * const connector_ptr)
{
    connector_status_t result;

    result = tcp_close_cloud(connector_ptr);

    return result;
}
