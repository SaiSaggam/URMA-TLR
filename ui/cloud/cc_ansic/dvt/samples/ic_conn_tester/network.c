/*
 * Copyright (c) 2013 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "connector_api.h"
#include "platform.h"
#include "application.h"

int app_dns_resolve_name(char const * const domain_name, unsigned long * ip_addr)
{
    int ret = -1;
    struct addrinfo *res_list;
    struct addrinfo *res;

    if ((domain_name == NULL) || (ip_addr == NULL))
    {
        goto done;
    }

    {
        struct addrinfo hint = {0};
        int error;

        hint.ai_socktype = SOCK_STREAM;
        hint.ai_family   = AF_INET;
        error = getaddrinfo(domain_name, NULL, &hint, &res_list);
        if (error != 0)
        {
            APP_DEBUG("dns_resolve_name: DNS resolution failed for [%s]\n", domain_name);
            goto done;
        }
    }

    /* loop over all returned results and look for a V4 IP address */
    for (res = res_list; res; res = res->ai_next)
    {
        if (res->ai_family == PF_INET)
        {
            struct in_addr const ipv4_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr;

            *ip_addr = ipv4_addr.s_addr;
            APP_DEBUG("dns_resolve_name: ip address = [%s]\n", inet_ntoa(ipv4_addr));
            ret = 0;
            break;
        }
    }

    freeaddrinfo(res_list);

done:
    if (ret == 0)
        app_inc_stats(dns_error_id);
    return ret;
}

static connector_callback_status_t app_network_open(connector_class_id_t const class_id,
                                                char const * const domain_name,
                                                size_t const length,
                                                connector_network_handle_t ** network_handle)
{
    connector_callback_status_t status = connector_callback_abort;
    static int fd = -1;
    char server_name[64];

    UNUSED_ARGUMENT(class_id);

    *network_handle = &fd;

    if (fd == -1)
    {
        unsigned long ip_addr;
        struct sockaddr_in sin;
        int enabled = 1;
        int result;

        memcpy(server_name, domain_name, length);
        server_name[length] = '\0';

        /*
         * Check if it's a dotted-notation IP address, if it's a domain name,
         * attempt to resolve it.
         */
        ip_addr = inet_addr(server_name);
        if (ip_addr == INADDR_NONE)
        {
            if (app_dns_resolve_name(server_name, &ip_addr) != 0)
            {
                APP_DEBUG("network_connect: Can't resolve DNS for %s\n", server_name);
                goto done;
            }
        }

        fd = (int) socket(AF_INET, SOCK_STREAM, 0);
        if (fd == INVALID_SOCKET)
        {
            APP_DEBUG("Could not open socket %d\n", WSAGetLastError());
            goto done;
        }
		else
        {
            if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&enabled, sizeof(enabled)) < 0)
            {
                APP_DEBUG("open_socket: setsockopt SO_KEEPALIVE failed %d\n", WSAGetLastError());
            }

            if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char*)&enabled, sizeof(enabled)) < 0)
            {
                APP_DEBUG("open_socket: setsockopt TCP_NODELAY failed %d\n", WSAGetLastError());
            }
        }

        if (ioctlsocket(fd, FIONBIO, &enabled) < 0)
        {
            APP_DEBUG("ioctl: FIONBIO failed %d\n", WSAGetLastError());
            goto done;
        }

        memset((char *)&sin, 0, sizeof(sin));
        memcpy(&sin.sin_addr, &ip_addr, sizeof sin.sin_addr);
        sin.sin_port   = htons(CONNECTOR_PORT);
        sin.sin_family = AF_INET;
        result = connect(fd, (struct sockaddr *)&sin, sizeof(sin));
        if (result == SOCKET_ERROR)
        {
			int const error = WSAGetLastError();
            if (error != WSAEWOULDBLOCK)
            {
                APP_PRINTF("network_connect: connect() failed %d\n", error);
                goto done;
            }
        }
    }

    {
        struct timeval timeout = {1, 0};
        fd_set read_set;
        fd_set write_set;
        int result;

        FD_ZERO(&read_set);
        FD_SET((unsigned)fd, &read_set);
        write_set = read_set;

        result = select(fd+1, &read_set, &write_set, NULL, &timeout);
        if (result < 0)
        {
            goto done;
        }

        if (result == 0)
        {
            status = connector_callback_busy;
            APP_PRINTF("network_connect: select timeout\r\n");
            goto done;
        }

        /* Check whether the socket is now writable (connection succeeded). */
        if (FD_ISSET(fd, &write_set))
        {
            /* We expect "socket writable" when the connection succeeds. */
            /* If we also got a "socket readable" we have an error. */
            if (FD_ISSET(fd, &read_set))
            {
                APP_PRINTF("network_connect: error to connect to %.*s server\n", length, server_name);
                goto done;
            }
            status = connector_callback_continue;
            APP_DEBUG("network_connect: connected to [%.*s] server\n", length, server_name);
            app_inc_stats(open_conn_id);
        }
    }

done:
    if (status == connector_callback_abort)
        app_inc_stats(open_conn_fail_id);

    if ((status == connector_callback_abort) && (fd != INVALID_SOCKET))
    {
        closesocket(fd);
        fd = INVALID_SOCKET;
    }

    return status;
}

/*
 * Send data to the iDigi server, this routine must not block.  If it encounters
 * EAGAIN or EWOULDBLOCK error, 0 bytes must be returned and iDigi Connector will continue
 * calling this function.
 */
static connector_callback_status_t app_network_send(connector_class_id_t const class_id,
                                                connector_write_request_t const * const write_data,
                                                size_t * const sent_length)
{
    connector_callback_status_t status = connector_callback_continue;

    int const bytes_sent = send(*write_data->network_handle, write_data->buffer, (int) write_data->length, 0);

    UNUSED_ARGUMENT(class_id);

    if (bytes_sent == SOCKET_ERROR)
    {
		int const error = WSAGetLastError();
        if (error == WSAEWOULDBLOCK)
        {
            status = connector_callback_busy;
        }
        else
        {
            status = connector_callback_abort;
            APP_DEBUG("network_send: send() failed %d\n", error);
        }
    }
    *sent_length = bytes_sent;

    return status;
}


/*
 * This routine reads a specified number of bytes from the iDigi server.  This 
 * function must not block. If it encounters EAGAIN or EWOULDBLOCK error, 0 
 * bytes must be returned and iDigi Connector will continue calling this function.
 */
static connector_callback_status_t app_network_receive(connector_class_id_t const class_id,
                                                   connector_read_request_t const * const read_data,
                                                   size_t * const read_length)
{
    connector_callback_status_t status = connector_callback_continue;
    int ccode;

    UNUSED_ARGUMENT(class_id);

    *read_length = 0;

    ccode = recv(*read_data->network_handle, read_data->buffer, (int) read_data->length, 0);
    if (ccode == 0)
    {
        /* EOF on input: the connection was closed. */
        APP_PRINTF("network_receive: EOF on socket\r\n");
        status = connector_callback_abort;
        goto done;
    }

    if (ccode == SOCKET_ERROR)
    {
		int const error = WSAGetLastError();
        if (error == WSAEWOULDBLOCK)
        {
            status = connector_callback_busy;
        }
        else
        {
            APP_DEBUG("network_receive: recv() failed %d\n", error);
            status = connector_callback_abort;
        }
    }

    *read_length = (size_t)ccode;
done:

    return status;
}


static connector_callback_status_t app_network_close(connector_class_id_t const class_id,
                                                 connector_close_request_t const * const close_data,
                                                 connector_connect_auto_type_t * const is_to_reconnect)
{
    connector_callback_status_t status = connector_callback_continue;
    connector_network_handle_t * const fd = close_data->network_handle;
    struct linger ling_opt;

    UNUSED_ARGUMENT(class_id);

    ling_opt.l_onoff = 1;
    ling_opt.l_linger = 1;

    if (setsockopt(*fd, SOL_SOCKET, SO_LINGER, (char *) &ling_opt, sizeof(ling_opt) ) < 0)
    {
   		int const error = WSAGetLastError();
        APP_PRINTF("network close: setsockopt fails: %d\n", error);
        if (error == WSAEWOULDBLOCK)
        {
            status = connector_callback_busy;
            goto done;
        }
    }

    if (closesocket(*fd) < 0)
    {
        int const error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK)
        {
            APP_PRINTF("network close: close() fails: %d\n", WSAGetLastError());
            status = connector_callback_abort;
            goto done;
        }
    }

    {
        int * user_fd = (int *)fd;
        *user_fd = INVALID_SOCKET;
    }

    *is_to_reconnect = close_data->status == connector_device_stopped ? connector_connect_manual : connector_connect_auto;
    app_inc_stats(closed_conn_id);
    app_set_stats(close_reason_id, close_data->status);
    APP_PRINTF("Network close\n");

done:
    return status;
}


bool application_check_connection(void)
{
    return true;
    /*
   static unsigned long started;
   bool result = true;
    
    if (started == 0)
        app_os_get_system_time(&started);

    if (close_cnt > APP_RECONNECT_FAIL_CNT)
    {
        unsigned long now;

        app_os_get_system_time(&now);

        if ((now - started) < APP_STABLE_TIME)
        {
            result = false;
        }
        else
        {
            app_os_get_system_time(&started);
            close_cnt = 0;
        }
    }
    return result;
    */
}

/*
 *  Callback routine to handle all networking related calls.
 */
connector_callback_status_t app_network_handler(connector_class_id_t const class_id,
                                            connector_request_id_network_t const request,
                                            void const * const request_data, size_t const request_length,
                                            void * response_data, size_t * const response_length)
{
    connector_callback_status_t status;
    UNUSED_ARGUMENT(request_length);

    switch (request)
    {
    case connector_network_open:
        status = app_network_open(class_id, request_data, request_length, response_data);
        *response_length = sizeof(connector_network_handle_t);
        break;

    case connector_network_send:
        status = app_network_send(class_id, request_data, response_data);
         break;

    case connector_network_receive:
        status = app_network_receive(class_id, request_data, response_data);
        break;

    case connector_network_close:
        status = app_network_close(class_id, request_data, response_data);
        break;

    default:
        APP_PRINTF("app_network_handler: unrecognized callback request [%d]\n", request);
        status = connector_callback_unrecognized;
        break;

    }
    return status;
}

