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
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "connector_api.h"
#include "platform.h"

enum {
    no_delay_receive,
    start_delay_receive,
    delay_receive_started
};
int delay_receive_flag = no_delay_receive;
unsigned long delay_receive_time = 0;

static int app_dns_resolve_name(char const * const domain_name, in_addr_t * const ip_addr)
{
    int ret = -1;
    struct addrinfo *res_list;
    struct addrinfo *res;

    if ((domain_name == NULL) || (ip_addr == NULL))
        goto done;

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

    /* loop over all returned results and look for a IPv4 address */
    for (res = res_list; res; res = res->ai_next)
    {
        if (res->ai_family == PF_INET)
        {
            struct sockaddr_in * const sa = cast_for_alignment(struct sockaddr_in *, res->ai_addr);
            struct in_addr const ipv4_addr = sa->sin_addr;

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

static connector_callback_status_t app_network_tcp_open(connector_network_open_t * const network_data)
{
    connector_callback_status_t rc = connector_callback_error;
    static int fd = -1;

    if (fd == -1)
    {
        in_addr_t ip_addr;

         /*
         * Check if it's a dotted-notation IP address, if it's a domain name,
         * attempt to resolve it.
         */
        ip_addr = inet_addr(network_data->device_cloud.url);
        if (ip_addr == INADDR_NONE)
        {
            if (app_dns_resolve_name(network_data->device_cloud.url, &ip_addr) != 0)
            {
                APP_DEBUG("network_connect: Can't resolve DNS for %s\n", network_data->device_cloud.url);
                goto done;
            }
        }

        fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd >= 0)
        {
            int enabled = 1;

            if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&enabled, sizeof(enabled)) < 0)
            {
                APP_DEBUG("open_socket: setsockopt SO_KEEPALIVE failed, errno %d\n", errno);
            }

            if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char*)&enabled, sizeof(enabled)) < 0)
            {
                APP_DEBUG("open_socket: setsockopt TCP_NODELAY failed, errno %d\n", errno);
            }
        }
        else
        {
            APP_DEBUG("Could not open socket, errno %d\n", errno);
            goto done;
        }

        {
            int opt=1;

            if (ioctl(fd, FIONBIO, &opt) < 0)
            {
                APP_DEBUG("ioctl: FIONBIO failed, errno %d\n", errno);
                goto done;
            }
        }

        {
            struct sockaddr_in sin = {0};

            memcpy(&sin.sin_addr, &ip_addr, sizeof sin.sin_addr);
            sin.sin_port   = htons(CONNECTOR_PORT);
            sin.sin_family = AF_INET;

            if (connect(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
            {
                switch (errno)
                {
                case EAGAIN:
                case EINPROGRESS:
                    break;
                default:
                    APP_DEBUG("network_connect: connect() failed, errno %d\n", errno);
                    goto done;
                }
            }
        }
    }

    {
        struct timeval timeout = {30, 0};
        fd_set read_set;
        fd_set write_set;
        int ccode;

        FD_ZERO(&read_set);
        FD_SET(fd, &read_set);
        write_set = read_set;

        ccode = select(fd+1, &read_set, &write_set, NULL, &timeout);
        if (ccode <= 0)
        {
            APP_DEBUG("network_connect: select returned %d\n", ccode);
            goto done;
        }

        /* Check whether the socket is now writable (connection succeeded). */
        if (FD_ISSET(fd, &write_set))
        {
            /* We expect "socket writable" when the connection succeeds. */
            /* If we also got a "socket readable" we have an error. */
            if (FD_ISSET(fd, &read_set))
            {
                APP_DEBUG("network_connect: error to connect to %s server\n", network_data->device_cloud.url);
                goto done;
            }
            network_data->handle = &fd;
            rc = connector_callback_continue;
            APP_DEBUG("network_connect: connected to [%s] server\n", network_data->device_cloud.url);
        }
    }

done:
    if ((rc == connector_callback_error) && (fd >= 0))
    {
        close(fd);
        fd = -1;
    }

    return rc;
}

static connector_callback_status_t app_network_tcp_send(connector_network_send_t * const write_data)
{
    connector_callback_status_t rc = connector_callback_continue;
    int * const fd = write_data->handle;

    int ccode = write(*fd, (char *)write_data->buffer, write_data->bytes_available);

    if (ccode < 0)
    {
        int err = errno;
        if (err == EAGAIN)
        {
            rc = connector_callback_busy;
        }
        else
        {
            char buffer[64];

            APP_DEBUG("network_send: send() failed, errno %d\n", err);

            do {
                ccode = read(*fd, (char *)buffer, sizeof buffer);

                if (ccode == 0)
                {
                    /* EOF on input: the connection was closed. */
                    APP_DEBUG("network_send: receive EOF on socket\r\n");
                }
                else if (ccode < 0)
                {
                    err = errno;
                    /* An error of some sort occurred: handle it appropriately. */
                    if (err != EAGAIN)
                    {
                       APP_DEBUG("network_send: receive: recv() failed, errno %d\n", err);
                        /* if not timeout (no data) return an error */
                    }
                }
                else
                {
                    APP_DEBUG("network_send: receive bytes = %d\n", ccode);
                }
            } while (ccode > 0);

            rc = connector_callback_error;
        }
    }
    write_data->bytes_used = ccode;

    return rc;
}

static connector_callback_status_t app_network_tcp_receive(connector_network_receive_t * const read_data)
{
    connector_callback_status_t rc = connector_callback_continue;
    struct timeval timeout = {0, 0};
    fd_set read_set;
    int ccode;

    int * const fd = read_data->handle;

    read_data->bytes_used = 0;

    switch (delay_receive_flag)
    {
    case start_delay_receive:
        APP_DEBUG("start delay...\n");
        delay_receive_flag = delay_receive_started;
        app_os_get_system_time(&delay_receive_time);
        break;
    case delay_receive_started:
    {
        unsigned long elapsed_time;

        app_os_get_system_time(&elapsed_time);
        elapsed_time -= delay_receive_time;

        if (elapsed_time > 5 && elapsed_time < 60)
        {
            /* hold off receive for 60 seconds */
            rc = connector_callback_busy;
            goto done;
        }
        break;
    }
    default:
        break;
    }

    FD_ZERO(&read_set);
    FD_SET(*fd, &read_set);

    /* Blocking point for IIK */
    ccode = select(*fd + 1, &read_set, NULL, NULL, &timeout);
    if (ccode < 0)
    {
        goto done;
    }

    if (ccode == 0)
    {
        rc = connector_callback_busy;
        goto done;
    }

    ccode = read(*fd, (char *)read_data->buffer, (int)read_data->bytes_available);

    if (ccode == 0)
    {
        /* EOF on input: the connection was closed. */
        APP_DEBUG("network_receive: EOF on socket\r\n");
        rc = connector_callback_error;
    }
    else if (ccode < 0)
    {
        /* An error of some sort occurred: handle it appropriately. */
        int const err = errno;
        if (err == EAGAIN)
        {
            rc = connector_callback_busy;
        }
        else
        {
            APP_DEBUG("network_receive: recv() failed, errno %d\n", err);
            /* if not timeout (no data) return an error */
            rc = connector_callback_error;
        }
    }

    read_data->bytes_used = (size_t)ccode;
done:

    return rc;
}

static connector_callback_status_t app_network_tcp_close(connector_network_close_t * const close_data)
{
    connector_callback_status_t status = connector_callback_continue;
    int * const fd = close_data->handle;


    if (close(*fd) < 0)
    {
        APP_DEBUG("network_tcp_close: close() failed, errno %d\n", errno);
    }

    *fd = -1;

    close_data->reconnect = app_connector_reconnect(connector_class_id_network_tcp, close_data->status);

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
