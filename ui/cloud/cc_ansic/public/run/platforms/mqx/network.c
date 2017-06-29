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
#include <mqx.h>
#include <bsp.h>
#include <rtcs.h>
#include <ipcfg.h>
#include <errno.h>
#include <connector_api.h>
#include <connector_types.h>
#include <platform.h>
#include <connector_debug.h>
#include <os_support.h>

#if !defined CONNECTOR_USE_SSL

connector_bool_t app_connector_reconnect(connector_class_id_t const class_id, connector_close_status_t const status)
{
    UNUSED_ARGUMENT(class_id);


    switch (status)
    {
           /* if either the server or our application cuts the connection, don't reconnect */
        case connector_close_status_device_terminated:
        case connector_close_status_device_stopped:
        case connector_close_status_abort:
             return connector_false;

       /* otherwise it's an error and we want to retry */
       default:
    	   return connector_true;
    }
}

static boolean app_dns_resolve(char const * const name, _ip_address * const ip_addr)
{
    boolean result = FALSE;
    size_t tries = 3;

    APP_DEBUG("dns_resolve_name: DNS Address : %d.%d.%d.%d\n",
        IPBYTES(ipcfg_get_dns_ip(IPCFG_default_enet_device, 0)));

    #define ONE_SECOND_DELAY    1000
    /* Try three times to get name */
    do
    {
        result = RTCS_resolve_ip_address((char_ptr)name, ip_addr, NULL, 0);

        if (result) break;

        APP_DEBUG("Failed - name not resolved\n");
        _time_delay(ONE_SECOND_DELAY);

    } while (--tries > 0);

    return result;
}

static int app_tcp_create_socket(void)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

	if (fd == RTCS_SOCKET_ERROR)
	{
		APP_DEBUG("app_tcp_create_socket: Could not open socket!\n");
		fd = -1;
	}

    return fd;
}

static int set_socket_options(int const fd)
{
	#define SOCKET_BUFFER_SIZE 		512
	#define SOCKET_TIMEOUT_MSEC 	1000000
	uint_32 option;
	int retval;
	
    option = TRUE;
	retval = setsockopt(fd, SOL_TCP, OPT_RECEIVE_NOWAIT, &option, sizeof option); 
	if(retval != RTCS_OK)
	{
		APP_DEBUG("set_non_blocking_socket: setsockopt OPT_RECEIVE_NOWAIT failed\n");
		goto done;
	}
	
	/* Reduce buffer size of socket to save memory */
	option = SOCKET_BUFFER_SIZE;
	retval = setsockopt(fd, SOL_TCP, OPT_TBSIZE, &option, sizeof option);
	if (retval != RTCS_OK)
	{
		APP_DEBUG("network_connect: setsockopt OPT_TBSIZE failed\n");
		goto done;
	}
	
	retval = setsockopt(fd, SOL_TCP, OPT_RBSIZE, &option, sizeof option);		
	if (retval != RTCS_OK)
	{
		APP_DEBUG("network_connect: setsockopt OPT_RBSIZE failed\n");
		goto done;
	}
	
	option = TRUE;
	retval = setsockopt(fd, SOL_TCP, OPT_KEEPALIVE, &option, sizeof option);
	if (retval != RTCS_OK)
	{
		APP_DEBUG("network_connect: setsockopt OPT_RBSIZE failed\n");
		goto done;
	}
	
	/* set a socket timeout */
	option = SOCKET_TIMEOUT_MSEC;
	retval = setsockopt(fd, SOL_TCP, OPT_TIMEWAIT_TIMEOUT, &option, sizeof option);
	if (retval != RTCS_OK)
	{
		APP_DEBUG("network_connect: setsockopt OPT_TIMEWAIT_TIMEOUT failed\n");
		goto done;
	}

done:
	return retval;
}

static connector_callback_status_t app_tcp_connect(int const fd, _ip_address const ip_addr)
{
    sockaddr_in addr;
    connector_callback_status_t status = connector_callback_error;
    int result;
    
	/* Connect to device */
	addr.sin_family      = AF_INET;
	addr.sin_port        = CONNECTOR_PORT;
	addr.sin_addr.s_addr = ip_addr;
	result = connect(fd, &addr, sizeof addr);
	if (result != RTCS_OK)
	{
        APP_DEBUG("app_tcp_connect: connect() failed, fd %d, returned %d\n", fd, result);
	} 
	else
	{
	    status = connector_callback_continue;
	}
	
    return status;
}

static connector_callback_status_t app_network_tcp_open(connector_network_open_t * const data)
{
    connector_callback_status_t status = connector_callback_abort;
    static _mqx_int socket_fd = RTCS_SOCKET_ERROR;

    if (socket_fd == RTCS_SOCKET_ERROR)
    {
        _ip_address server_ip_addr;

        if (app_dns_resolve(data->device_cloud.url, &server_ip_addr) == FALSE)
        {
        	goto done;
        }

        APP_DEBUG("network_connect: Resolved IP Address for %s: %d.%d.%d.%d\n", data->device_cloud.url, IPBYTES(server_ip_addr));

        socket_fd = app_tcp_create_socket();
        if (socket_fd == -1)
        {
            status = connector_callback_error;
            goto done;
        }
        
        if (set_socket_options(socket_fd) < 0)
        {
        	status = connector_callback_error;
			goto done;
        }

		APP_DEBUG("Connecting to %s...\n", data->device_cloud.url);

        status = app_tcp_connect(socket_fd, server_ip_addr);
        if (status != connector_callback_continue)
        {
        	goto done;
        }
    }
    
    APP_DEBUG("network_connect: 3 way handshake, connected to [%s] server\n", data->device_cloud.url);
    data->handle = &socket_fd;

done:
    if (status != connector_callback_continue)
    {
    	shutdown(socket_fd, FLAG_ABORT_CONNECTION);
		socket_fd = RTCS_SOCKET_ERROR;
    }

    return status;
}


/*
 * Sends data to the Device Cloud, this routine must not block.  If it encounters
 * EAGAIN  error, return connector_callback_busy and Cloud Connector will ignore the
 * sent_length and continue calling this function.
 */
static connector_callback_status_t app_network_tcp_send(connector_network_send_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    size_t bytes_sent;
    int rtcs_error;
    _mqx_int_ptr fd = data->handle;

    bytes_sent = send(*fd, (char _PTR_)data->buffer, data->bytes_available, 0);
    if (bytes_sent == RTCS_ERROR)
    {
    	rtcs_error = RTCS_geterror(*fd);
    	if (rtcs_error == MQX_EAGAIN) 
    	{
    		status = connector_callback_busy;
    		goto done;
    	}
    		
        status = connector_callback_abort;
        APP_DEBUG("network_send: send() failed RTCS error [%lx]\n", rtcs_error);
        shutdown(*fd, FLAG_ABORT_CONNECTION);
        *fd = RTCS_SOCKET_ERROR;
    }
    else
    {
        data->bytes_used = bytes_sent;
        if (bytes_sent == 0)
        {
            status = connector_callback_busy;
        }
    }

done:
   return status;
}

/*
 * This routine reads a specified number of bytes from the Device Cloud.  This
 * function must not block. If it encounters EAGAIN  error, return
 * connector_callback_busy and Cloud Connector will ignore the read_data and read_length
 * and continue calling this function.
 */
static connector_callback_status_t app_network_tcp_receive(connector_network_receive_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    int_32 bytes_read;
    _mqx_int_ptr fd = data->handle;

    data->bytes_used = 0;
    bytes_read = recv(*fd, (char *)data->buffer, (int)data->bytes_available, 0);
    if (bytes_read == RTCS_ERROR)
    {
        APP_DEBUG("network_receive: Error, recv() failed RTCS error [%lx]\n", RTCS_geterror(*fd));
        shutdown(*fd, FLAG_ABORT_CONNECTION);
        *fd = RTCS_SOCKET_ERROR;
        status = connector_callback_abort;
    }
    else
    {
        data->bytes_used = bytes_read;
        if (bytes_read == 0)
        {
            status = connector_callback_busy;
        }
    }

    if (data->bytes_used == 0)
    {
        _time_delay(1);
    }

    return status;
}

static connector_callback_status_t app_network_tcp_close(connector_network_close_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
	uint_32 result;
    _mqx_int_ptr fd = data->handle;

	ASSERT(*fd == socket_fd);

	/* Note: this does a graceful close - like linger */
	result = shutdown(*fd, FLAG_CLOSE_TX);
	if (result != RTCS_OK)
	{
		APP_DEBUG("network_close: failed, code = %lx\n", result);
		goto done;
	}

	*fd = RTCS_SOCKET_ERROR;

    data->reconnect = app_connector_reconnect(connector_class_id_network_tcp, data->status);

done:
    return status;
}

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
#endif
