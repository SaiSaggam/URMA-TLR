/*
 * Copyright (c) 2012 Digi International Inc.,
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

#if defined CONNECTOR_USE_SSL
#include <cyassl/ssl.h>

/* Global structure of connected interface */
static struct sockaddr_in interface_addr;
typedef struct
{
    int sfd;
    CYASSL_CTX * ctx;
    CYASSL * ssl;
} app_ssl_t;

extern const unsigned int app_ssl_ca_cert_len;
extern const unsigned char app_ssl_ca_cert[];

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

connector_callback_status_t app_get_interface_ip_address(uint8_t ** ip_address, size_t * size)
{
    *size       = sizeof(interface_addr.sin_addr.s_addr);
    *ip_address = (uint8_t *)&interface_addr.sin_addr.s_addr;

    return connector_callback_continue;
}

static int app_setup_server_socket(void)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd == RTCS_SOCKET_ERROR) {
    	APP_DEBUG("Could not open socket\n");
    	shutdown(socket_fd, FLAG_ABORT_CONNECTION);
   	    socket_fd = -1;
    }
        
    return socket_fd;
}

static int app_connect_to_server(int fd, _ip_address const ip_addr)
{
    int result;
    sockaddr_in addr;

	/* Connect to device */
	addr.sin_family      = AF_INET;
	addr.sin_port        = CONNECTOR_SSL_PORT;
	addr.sin_addr.s_addr = ip_addr;

	result = connect(fd, &addr, sizeof addr);
	if (result != RTCS_OK)
	{
        APP_DEBUG("app_tcp_connect: connect() failed, fd %d, returned %d\n", fd, result);
	} 

    return result;
}

static int app_load_certificate_and_key(CYASSL_CTX * const ctx)
{
    int ret = -1;

    {
    	ret = CyaSSL_CTX_load_verify_buffer(ctx, app_ssl_ca_cert, app_ssl_ca_cert_len, SSL_FILETYPE_PEM);
        if (ret != 1)
        {
            APP_DEBUG("Failed to load CA cert %d\n", ret);
            goto error;
        }
    }

error:
    return ret;
}

static void app_free_ssl_info(app_ssl_t * const ssl_ptr)
{
    if (ssl_ptr->ssl != NULL)
    {
        CyaSSL_free(ssl_ptr->ssl);
        ssl_ptr->ssl = NULL;
    }

    if (ssl_ptr->ctx != NULL)
    {
        CyaSSL_CTX_free(ssl_ptr->ctx);
        ssl_ptr->ctx = NULL;
    }

    if (ssl_ptr->sfd != -1)
    {
        shutdown(ssl_ptr->sfd, FLAG_ABORT_CONNECTION);
        ssl_ptr->sfd = -1;
    }
}


static int app_ssl_connect(app_ssl_t * const ssl_ptr)
{
    int ret = -1;

	CyaSSL_Init();
	ssl_ptr->ctx = CyaSSL_CTX_new(CyaTLSv1_client_method());
	if (ssl_ptr->ctx == 0) {
		printf("ERROR: setting up ctx\n");
		goto error;
	}
	
	ret = CyaSSL_CTX_load_verify_buffer(ssl_ptr->ctx, app_ssl_ca_cert, app_ssl_ca_cert_len, SSL_FILETYPE_PEM);
	if (ret != SSL_SUCCESS) {
		printf("ERROR: CyaSSL_CTX_load_verify_buffer: %d\n", ret);
	}
	
	ssl_ptr->ssl = CyaSSL_new(ssl_ptr->ctx);
	if(ssl_ptr->ssl == NULL) {
		printf("CyaSSL_new failed\n");
		goto error;
	}

	ret = CyaSSL_set_fd(ssl_ptr->ssl, ssl_ptr->sfd);
	if (ret != SSL_SUCCESS) {
		printf("CyaSSL_set_fd failed: %d\n", ret);
		goto error;
	}
	
	ret = CyaSSL_connect(ssl_ptr->ssl);
	if (ret != SSL_SUCCESS) {
		printf("CyaSSL_connect failed: %d\n",  CyaSSL_get_error(ssl_ptr->ssl, ret));
		goto error;
	}
	
    ret = 0;

error:
    return ret;
}

static int set_socket_options(int const fd)
{
	#define SOCKET_BUFFER_SIZE 		512
	#define SOCKET_TIMEOUT_MSEC 	1000000
	boolean success = FALSE;
	uint_32 option = TRUE;
	int retval;
	
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

static connector_callback_status_t app_tcp_connect(_ip_address const ip_addr, connector_network_open_t * open_data)
{
    connector_callback_status_t status = connector_callback_error;
    static app_ssl_t ssl_info = {0};
    unsigned short interface_addr_len;

    ssl_info.sfd = app_setup_server_socket();
    if (ssl_info.sfd < 0)
    {
    	APP_DEBUG("app_setup_server_socket failed\n");
    	goto done;
    }

    if (app_connect_to_server(ssl_info.sfd, ip_addr) < 0) {
    	APP_DEBUG("app_connect_to_server failed\n");
    	goto error;
    }

    if (app_ssl_connect(&ssl_info) < 0) {
    	APP_DEBUG("app_ssl_connect failed\n");
        goto error;
    }

    /* make it non-blocking now */
    if (set_socket_options(ssl_info.sfd) < 0)
    {
    	status = connector_callback_error;
    	APP_DEBUG("set_socket_options failed\n");
		goto done;
    }
    
    /* Get socket info of connected interface */
    interface_addr_len = sizeof(interface_addr);
    if (getsockname(ssl_info.sfd, &interface_addr, &interface_addr_len))
    {
        APP_DEBUG("network_connect: getsockname error, errno %d\n", errno);
        goto done;
    }

    APP_DEBUG("network_connect: connected\n");
    open_data->handle = (connector_network_handle_t *)&ssl_info;
    status = connector_callback_continue;
    goto done;

error:
    app_free_ssl_info(&ssl_info);

done:
    return status;
}

/*
 * Send data to the Device Cloud, this routine must not block.
 */
static connector_callback_status_t app_network_tcp_send(connector_network_send_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    app_ssl_t * const ssl_ptr = (app_ssl_t *)data->handle;
    int bytes_sent = 0;

    bytes_sent = CyaSSL_write(ssl_ptr->ssl, data->buffer, data->bytes_available);
    if (bytes_sent == SSL_FATAL_ERROR)
    {
        APP_DEBUG("network_send: send() failed RTCS error [%lx]\n", CyaSSL_get_error(ssl_ptr->ssl, bytes_sent));
        status = connector_callback_error;
    }
    else
	{
		data->bytes_used = bytes_sent;
		if (bytes_sent == 0)
		{
			status = connector_callback_busy;
		}
	}

    return status;
}

/*
 * This routine reads a specified number of bytes from the Device Cloud.
 */
static connector_callback_status_t app_network_tcp_receive(connector_network_receive_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    app_ssl_t * const ssl_ptr = (app_ssl_t *)data->handle;
    int bytes_read = 0;

    if (CyaSSL_pending(ssl_ptr->ssl) == 0)
    {
        int ready;
        struct timeval timeout;
        fd_set read_set;

        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        FD_ZERO(&read_set);
        FD_SET(ssl_ptr->sfd, &read_set);

        /* Blocking point for Connector */
        ready = select(ssl_ptr->sfd + 1, &read_set, NULL, NULL, &timeout);
        if (ready == 0)
        {
            status = connector_callback_busy;
            goto done;
        }

        if (ready < 0)
        {
           APP_DEBUG("app_network_receive: select failed\n");
           goto done;
        }
    }

    bytes_read = CyaSSL_read(ssl_ptr->ssl, data->buffer, data->bytes_available);
    if (bytes_read == SSL_FATAL_ERROR)
    {
        int ssl_error = CyaSSL_get_error(ssl_ptr->ssl, bytes_read);
        if (ssl_error == SSL_ERROR_WANT_READ)
        {
            status = connector_callback_busy;
            goto done;
        }

        /* EOF on input: the connection was closed. */
        APP_DEBUG("SSL_read failed %d\n", bytes_read);
        status = connector_callback_error;
    }
    else
	{
		data->bytes_used = bytes_read;
		if (bytes_read == 0)
		{
			status = connector_callback_busy;
		}
	}

done:
    return status;
}

static connector_callback_status_t app_network_tcp_close(connector_network_close_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    app_ssl_t * const ssl_ptr = (app_ssl_t *)data->handle;

    /* send close notify to peer */
    if (CyaSSL_shutdown(ssl_ptr->ssl) == 0)
    {
        CyaSSL_shutdown(ssl_ptr->ssl);  /* wait for peer's close notify */
    }

    app_free_ssl_info(ssl_ptr);
    data->reconnect = app_connector_reconnect(connector_class_id_network_tcp, data->status);

    return status;
}

static connector_callback_status_t app_network_tcp_open(connector_network_open_t * const data)
{
    connector_callback_status_t status;
    _ip_address server_ip_addr;

    if (app_dns_resolve(data->device_cloud.url, &server_ip_addr) == FALSE)
    {
        APP_DEBUG("app_network_tcp_open: Can't resolve DNS for %s\n", data->device_cloud.url);
        goto done;
    }

    status = app_tcp_connect(server_ip_addr, data);

    if (status == connector_callback_continue)
        APP_DEBUG("network_tcp_open: connected to %s\n", data->device_cloud.url);
    else
    if (status == connector_callback_error)
        APP_DEBUG("network_tcp_open: failed to connect to %s\n", data->device_cloud.url);

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
