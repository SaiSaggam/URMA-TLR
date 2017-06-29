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
/**
 * @file
 *  @brief Routines which implement Cloud Connector network interface for
 *  @ref CONNECTOR_TRANSPORT_TCP.
 */
#include "connector_api.h"
#include "platform.h"

/**
 * @brief   Connect to Device Cloud
 *
 * This routine sets up a TCP connection to the requested URL
 * and fills in a network handle which is then passed to subsequent TCP networking calls.
 * This routine will take the given host_name and attempt to
 * resolve the name and connect.
 *
 * @param data @ref connector_network_open_t
 *  <ul>
 *   <li><b><i>device_cloud.url</i></b> - FQDN of Device Cloud </li>
 *   <li><b><i>handle</i></b> - This is filled in with the value
 *   of a network handle, passed to subsequent networking calls,
 *   @ref connector_network_handle_t "connector_network_handle_t"
 *      is defined in public\include\connector_types.h.
 *  </li>
 * </ul>
 *
 * @retval connector_callback_continue	The routine has successfully established connection.
 * @retval connector_callback_busy 		The routine will be called again to complete connection.
 * @retval connector_callback_error     The connection can't be established, Cloud Connector
 *                                  will exit @ref connector_run "connector_run()" or @ref connector_step "connector_step()".
 * @retval connector_callback_abort     The application aborts Cloud Connector.
 *
 * @see @ref open "Network API callback Open"
 */
connector_callback_status_t app_network_tcp_open(connector_network_open_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}

/**
 * @brief   Send data to Device Cloud
 *
 * This routine sends data to the Device Cloud. This
 * function must not block.
 *
 * @param data @ref connector_network_send_t
 *  <ul>
 *   <li><b><i>handle</i></b> - Network handle </li>
 *   <li><b><i>buffer</i></b> - Data to be sent </li>
 *   <li><b><i>bytes_available</i></b> - Number of bytes to send
 *  </li>
 *   <li><b><i>bytes_used</i></b> - Number of bytes sent </li>
 * </ul>
 *
 * @retval connector_callback_continue	The routine has successfully sent up to requested number of data bytes.
 * @retval connector_callback_busy 		The routine has encountered EAGAIN or EWOULDBLOCK error and could not send
 *                                  any data. It will be called again to send data.
 * @retval connector_callback_error     An irrecoverable error has occurred,  Cloud Connector will call
 *                                  @ref app_network_tcp_close.
 * @retval connector_callback_abort     The application aborts Cloud Connector.
 *
 * @see @ref send "Network API callback Send"
 */
connector_callback_status_t app_network_tcp_send(connector_network_send_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}

/**
 * @brief   Receive data from Device Cloud
 *
 * This routine receives data from Device Cloud. This
 * function must not block.
 *
 * @param data @ref connector_network_receive_t
 *  <ul>
 *   <li><b><i>handle</i></b> - Network handle </li>
 *   <li><b><i>buffer</i></b> - Buffer to place received data
 *   </li>
 *   <li><b><i>bytes_available</i></b> - Buffer size in bytes
 *  </li>
 *   <li><b><i>bytes_used</i></b> - Number of bytes received
 *   </li>
 * </ul>
 *
 * @retval connector_callback_continue	The routine has received some data.
 * @retval connector_callback_busy 		No data is pending, the routine has encountered EAGAIN or
 *                                  EWOULDBLOCK error. It will be called again to receive data.
 * @retval connector_callback_error     An irrecoverable error has occurred,  Cloud Connector will call
 *                                  @ref app_network_tcp_close.
 * @retval connector_callback_abort     The application aborts Cloud Connector.
 *
 * @see @ref receive "Network API callback Receive"
 */
connector_callback_status_t app_network_tcp_receive(connector_network_receive_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}


/**
 * @brief   Close the connection to Device Cloud
 *
 * This callback requests an application to close it's network handle.
 *
 * @param data @ref connector_network_close_t
 *  <ul>
 *   <li><b><i>handle</i></b> - Network handle </li>
 *   <li><b><i>reconnect</i></b> - Set to connector_true to
 *   reconnect, without exiting connector_run(). Set
 *   to connector_false otherwise.
 *   </li>
 * </ul>
 *
 * @retval connector_callback_continue	The callback has successfully closed the connection.
 * @retval connector_callback_busy 		The network device is busy, the routine will be called again to complete close.
 * @retval connector_callback_abort     The application aborts Cloud Connector.
 *
 * @see @ref close "Network API callback Close"
 */
connector_callback_status_t app_network_tcp_close(connector_network_close_t * const data)
{
    connector_callback_status_t rc = connector_callback_continue;

    UNUSED_ARGUMENT(data);

    return rc;
}


/**
 * @cond DEV
 */

/*
 *  Callback routine to handle all networking related calls.
 */
connector_callback_status_t app_network_tcp_handler(connector_request_id_network_t const request_id,
                                                    void * const data)
{
    connector_callback_status_t status;

    switch (request_id)
    {
    case connector_request_id_network_open:
        status = app_network_tcp_open(data);
        break;

    case connector_request_id_network_send:
        status = app_network_tcp_send(data);
        break;

    case connector_request_id_network_receive:
        status = app_network_tcp_receive(data);
        break;

    case connector_request_id_network_close:
        status = app_network_tcp_close(data);
        break;

    default:
        APP_DEBUG("app_network_tcp_handler: unrecognized callback request_id [%d]\n", request_id);
        status = connector_callback_unrecognized;
        break;

    }

    return status;
}

/**
 * @endcond
 */
