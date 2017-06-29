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
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>


#include "connector_api.h"
#include "platform.h"

static int * edp_client_socket;

int app_get_edp_conn_ip_address(connector_config_ip_address_t * const config_ip)
{
    static struct sockaddr_in name;
    int namelen = sizeof name;
    int result = 0;

    name.sin_family = AF_INET;

    result = getsockname(*edp_client_socket, (struct sockaddr *) &name, &namelen);
    if (result == SOCKET_ERROR)
    {
        int const error = WSAGetLastError();
        APP_DEBUG("app_get_edp_conn_ip_address: getsockname() failed %d\n", error);
        goto done;
    }
    config_ip->address = (uint8_t *) &name.sin_addr.s_addr;
    config_ip->ip_address_type = connector_ip_address_ipv4;
    if (*config_ip->address == 0)
    {
        APP_DEBUG("app_get_edp_conn_ip_address: getsockname() returned 0 IP address\n");
        result = SOCKET_ERROR;
    }
done:
    return result;
}

static int app_dns_resolve_name(char const * const domain_name, unsigned long * ip_addr)
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
    return ret;
}

static connector_callback_status_t app_network_tcp_open(connector_network_open_t * const data)
{
    connector_callback_status_t status = connector_callback_error;
    static int fd = -1;

   data->handle = &fd;
   edp_client_socket = &fd;

    if (fd == -1)
    {
        unsigned long ip_addr;
        struct sockaddr_in sin;
        int enabled = 1;
        int result;

        /*
         * Check if it's a dotted-notation IP address, if it's a domain name,
         * attempt to resolve it.
         */
        ip_addr = inet_addr(data->device_cloud.url);
        if (ip_addr == INADDR_NONE)
        {
            if (app_dns_resolve_name(data->device_cloud.url, &ip_addr) != 0)
            {
                APP_DEBUG("network_connect: Can't resolve DNS for %s\n", data->device_cloud.url);
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
                APP_DEBUG("network_connect: connect() failed %d\n", error);
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
            APP_DEBUG("network_connect: select timeout\r\n");
            goto done;
        }

        /* Check whether the socket is now writable (connection succeeded). */
        if (FD_ISSET(fd, &write_set))
        {
            /* We expect "socket writable" when the connection succeeds. */
            /* If we also got a "socket readable" we have an error. */
            if (FD_ISSET(fd, &read_set))
            {
                APP_DEBUG("network_connect: error to connect to %s server\n", data->device_cloud.url);
                goto done;
            }
            status = connector_callback_continue;
            APP_DEBUG("network_connect: connected to [%s] server\n", data->device_cloud.url);
        }
    }

done:
    if ((status == connector_callback_error) && (fd != INVALID_SOCKET))
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

static connector_callback_status_t app_network_tcp_send(connector_network_send_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    int * const fd = data->handle;

    int const ccode = send(*fd, data->buffer, data->bytes_available, 0);
    if (ccode >= 0)
    {
        data->bytes_used = (size_t)ccode;
    }
    else
    {
		int const error = WSAGetLastError();
        if (error == WSAEWOULDBLOCK)
        {
            status = connector_callback_busy;
        }
        else
        {
            status = connector_callback_error;
            APP_DEBUG("network_send: send() failed %d\n", error);
        }
    }

    return status;
}



/*
 * This routine reads a specified number of bytes from the iDigi server.  This 
 * function must not block. If it encounters EAGAIN or EWOULDBLOCK error, 0 
 * bytes must be returned and iDigi Connector will continue calling this function.
 */

static connector_callback_status_t app_network_tcp_receive(connector_network_receive_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    int * const fd = data->handle;

    int ccode = recv(*fd, data->buffer, data->bytes_available, 0);
    if (ccode > 0)
    {
        data->bytes_used = (size_t)ccode;
    }
    else
    if (ccode == 0)
    {
        /* EOF on input: the connection was closed. */
        APP_DEBUG("network_receive: EOF on socket\n");
        status = connector_callback_error;
    }
    else
    {
		int const error = WSAGetLastError();
        if (error == WSAEWOULDBLOCK)
        {
            status = connector_callback_busy;
        }
        else
        {
            APP_DEBUG("network_receive: recv() failed %d\n", error);
            status = connector_callback_error;
        }
    }

    return status;
}

static connector_callback_status_t app_network_tcp_close(connector_network_close_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    int * const fd = data->handle;

    data->reconnect = app_connector_reconnect(connector_class_id_network_tcp, data->status);

    if (closesocket(*fd) < 0)
    {
       APP_DEBUG("network close: close() fails: %d\n", WSAGetLastError());
    }
    else
        APP_DEBUG("network_tcp_close: fd %d\n", *fd);

    *fd = -1;

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

