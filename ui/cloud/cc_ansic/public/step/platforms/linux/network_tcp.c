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
#include "network_dns.h"

#if defined CONNECTOR_TRANSPORT_TCP


/* Global structure of connected interface */
static struct sockaddr_in interface_addr;

connector_callback_status_t app_get_interface_ip_address(uint8_t ** ip_address, size_t * size)
{
    *size       = sizeof(interface_addr.sin_addr.s_addr);
    *ip_address = (uint8_t *)&interface_addr.sin_addr.s_addr;

    return connector_callback_continue;
}

static connector_callback_status_t app_network_tcp_close(connector_network_close_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    int * const fd = data->handle;

    app_dns_set_redirected(connector_class_id_network_tcp, data->status == connector_close_status_cloud_redirected);

    data->reconnect = app_connector_reconnect(connector_class_id_network_tcp, data->status);

    if (close(*fd) < 0)
    {
        APP_DEBUG("network_tcp_close: close() failed, fd %d, errno %d\n", *fd, errno);
    }
    else
        APP_DEBUG("network_tcp_close: fd %d\n", *fd);

    *fd = -1;

    return status;
}


/*
 * This routine reads a specified number of bytes from Device Cloud.  This
 * function must not block. If it encounters EAGAIN  error, return
 * connector_callback_busy and Cloud Connector will ignore the read_data and read_length
 * and continue calling this function.
 */
static connector_callback_status_t app_network_tcp_receive(connector_network_receive_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    int * const fd = data->handle;

    int ccode = read(*fd, data->buffer, data->bytes_available);
    if (ccode > 0)
    {
        data->bytes_used = (size_t)ccode;
    }
    else
    if (ccode == 0)
    {
        /* EOF on input: the connection was closed. */
        APP_DEBUG("network_receive: EOF on socket\n");
        errno = ECONNRESET;
        status = connector_callback_error;
    }
    else
    {
        int const err = errno;
        /* An error of some sort occurred: handle it appropriately. */
        if (err == EAGAIN)
        {
            status = connector_callback_busy;
        }
        else
        {
            APP_DEBUG("network_receive: recv() failed, errno %d\n", err);
            /* if not timeout (no data) return an error */
            app_dns_cache_invalidate(connector_class_id_network_tcp);
            status = connector_callback_error;
        }
    }
    return status;
}

/*
 * Sends data to Device Cloud, this routine must not block.  If it encounters
 * EAGAIN  error, return connector_callback_busy and Cloud Connector will ignore the
 * sent_length and continue calling this function.
 */
static connector_callback_status_t app_network_tcp_send(connector_network_send_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    int * const fd = data->handle;

    int ccode = write(*fd, data->buffer, data->bytes_available);
    if (ccode >= 0)
    {
        data->bytes_used = (size_t)ccode;
    }
    else
    {
        int const err = errno;
        if (err == EAGAIN)
        {
            status = connector_callback_busy;
        }
        else
        {
            status = connector_callback_error;
            APP_DEBUG("app_network_tcp_send: send() failed, errno %d\n", err);
            app_dns_cache_invalidate(connector_class_id_network_tcp);
        }
    }

    return status;
}


static int app_tcp_create_socket(void)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd >= 0)
    {
        int enabled = 1;

        if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &enabled, sizeof(enabled)) < 0)
        {
            APP_DEBUG("open_socket: setsockopt SO_KEEPALIVE failed, errno %d\n", errno);
        }

        if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &enabled, sizeof(enabled)) < 0)
        {
            APP_DEBUG("open_socket: setsockopt TCP_NODELAY failed, errno %d\n", errno);
        }

        if (ioctl(fd, FIONBIO, &enabled) < 0)
        {
            APP_DEBUG("ioctl: FIONBIO failed, errno %d\n", errno);
            close(fd);
            fd = -1;
        }
    }
    else
    {
        APP_DEBUG("Could not open tcp socket, errno %d\n", errno);
    }

    return fd;
}

static connector_callback_status_t app_tcp_connect(int const fd, in_addr_t const ip_addr)
{

    struct sockaddr_in sin = {0};
    connector_callback_status_t status = connector_callback_continue;

    memcpy(&sin.sin_addr, &ip_addr, sizeof sin.sin_addr);
    sin.sin_port   = htons(CONNECTOR_PORT);
    sin.sin_family = AF_INET;

    APP_DEBUG("app_tcp_connect: fd %d\n", fd);


    if (connect(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        int const err = errno;
        switch (err)
        {
        case EINTR:
        case EAGAIN:
        case EINPROGRESS:
            status = connector_callback_busy;
            break;

        default:
            APP_DEBUG("app_tcp_connect: connect() failed, fd %d, errno %d\n", fd, err);
            status = connector_callback_error;
        }
    }

    return status;
}

static connector_callback_status_t app_is_tcp_connect_complete(int const fd)
{
    connector_callback_status_t status = connector_callback_busy;
    struct timeval timeout = {0};
    fd_set read_set, write_set;
    int rc;

    FD_ZERO(&read_set);
    FD_SET(fd, &read_set);
    write_set = read_set;

    rc = select(fd+1, &read_set, &write_set, NULL, &timeout);
    if (rc < 0)
    {
        if (errno != EINTR) {
            APP_DEBUG("app_is_tcp_connect_complete: select on fd %d returned %d, errno %d\n", fd, rc, errno);
            status = connector_callback_error;
        }
    }
    else
    /* Check whether the socket is now writable (connection succeeded). */
    if (rc > 0 && FD_ISSET(fd, &write_set))
    {
        /* We expect "socket writable" when the connection succeeds. */
        /* If we also got a "socket readable" we have an error. */
        if (FD_ISSET(fd, &read_set))
        {
            APP_DEBUG("app_is_tcp_connect_complete: FD_ISSET for read, fd %d\n", fd);
            status = connector_callback_error;

        }
        else
        {
            status = connector_callback_continue;
        }
    }
    return status;
}

static connector_callback_status_t app_network_tcp_open(connector_network_open_t * const data)
{
#define APP_CONNECT_TIMEOUT 30

    static unsigned long connect_time;
    static int fd = -1;
    socklen_t interface_addr_len;

    connector_callback_status_t status = connector_callback_error;
    data->handle = &fd;

    if (fd == -1)
    {
        in_addr_t ip_addr;

        status = app_dns_resolve(connector_class_id_network_tcp, data->device_cloud.url, &ip_addr);
        if (status != connector_callback_continue)
        {
            APP_DEBUG("app_network_tcp_open: Can't resolve DNS for %s\n", data->device_cloud.url);
            goto done;
        }

        fd = app_tcp_create_socket();
        if (fd == -1)
        {
            status = connector_callback_error;
            goto done;
        }

        app_os_get_system_time(&connect_time);
        status = app_tcp_connect(fd, ip_addr);
        if (status != connector_callback_continue)
            goto error;
    }

    /* Get socket info of connected interface */
    interface_addr_len = sizeof(interface_addr);
    if (getsockname(fd, (struct sockaddr *)&interface_addr, &interface_addr_len))
    {
        APP_DEBUG("network_connect: getsockname error, errno %d\n", errno);
        goto done;
    }

    status = app_is_tcp_connect_complete(fd);
    if (status == connector_callback_continue)
    {
         APP_DEBUG("app_network_tcp_open: connected to %s\n", data->device_cloud.url);
         goto done;
    }

    if (status == connector_callback_busy)
    {
        unsigned long elapsed_time;

        app_os_get_system_time(&elapsed_time);
        elapsed_time -= connect_time;

        if (elapsed_time >= APP_CONNECT_TIMEOUT)
        {
            APP_DEBUG("app_network_tcp_open: failed to connect withing 30 seconds\n");
            status = connector_callback_error;
        }
    }

error:
    if (status == connector_callback_error)
    {
        APP_DEBUG("app_network_tcp_open: failed to connect to %s\n", data->device_cloud.url);
        app_dns_set_redirected(connector_class_id_network_tcp, 0);

        if (fd >= 0)
        {
            close(fd);
            fd = -1;
        }
    }

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

