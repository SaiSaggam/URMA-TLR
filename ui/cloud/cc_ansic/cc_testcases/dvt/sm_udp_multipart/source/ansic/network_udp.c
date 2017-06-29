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

#if defined CONNECTOR_TRANSPORT_UDP

static connector_callback_status_t app_network_udp_close(connector_network_close_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    int * const fd = data->handle;

    data->reconnect = app_connector_reconnect(connector_class_id_network_udp, data->status);

    if (close(*fd) < 0)
    {
        APP_DEBUG("network_tcp_close: close() failed, errno %d\n", errno);
    }

    *fd = -1;

    return status;
}


/*
 * This routine reads a specified number of bytes from Device Cloud.  This
 * function must not block. If it encounters EAGAIN  error, return
 * connector_callback_busy and Cloud Connector will ignore the read_data and read_length
 * and continue calling this function.
 */
static connector_callback_status_t app_network_udp_receive(connector_network_receive_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    int * const fd = data->handle;

    int ccode = read(*fd, data->buffer, data->bytes_available);

    if (ccode < 0)
    {
        int const err = errno;
        /* An error of some sort occurred: handle it appropriately. */
        if (err == EAGAIN)
        {
            status = connector_callback_busy;
        }
        else
        {
            APP_DEBUG("app_network_udp_receive: recv() failed, errno %d\n", err);
            /* if not timeout (no data) return an error */
            app_dns_cache_invalidate(connector_class_id_network_udp);
            status = connector_callback_error;
        }
    }

    data->bytes_used = (size_t)ccode;

    return status;
}

/*
 * Sends data to Device Cloud, this routine must not block.  If it encounters
 * EAGAIN  error, return connector_callback_busy and Cloud Connector will ignore the
 * sent_length and continue calling this function.
 */
static connector_callback_status_t app_network_udp_send(connector_network_send_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    int * const fd = data->handle;

    int bytes_sent = write(*fd, data->buffer, data->bytes_available);
    if (bytes_sent < 0)
    {
        int const err = errno;
        if (err == EAGAIN)
        {
            status = connector_callback_busy;
        }
        else
        {
            status = connector_callback_error;
            APP_DEBUG("app_network_udp_send: send() failed, errno %d\n", err);
            app_dns_cache_invalidate(connector_class_id_network_udp);
        }
    }
    data->bytes_used = (size_t)bytes_sent;
    APP_DEBUG("app_network_udp_send: send() %" PRIsize " bytes\n", data->bytes_used);

    return status;
}

static int  app_udp_create_socket(void)
{
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd >= 0)
    {
        int enabled = 1;

        if (ioctl(fd, FIONBIO, &enabled) < 0)
        {
            APP_DEBUG("ioctl: FIONBIO failed, errno %d\n", errno);
            close(fd);
            fd = -1;
        }
    }
    else
    {
        APP_DEBUG("Could not open UDP socket, errno %d\n", errno);
    }

    return fd;
}

static connector_callback_status_t app_udp_connect(int const fd, in_addr_t const ip_addr)
{
    connector_callback_status_t status = connector_callback_continue;
    struct sockaddr_in sin = {0};

    memcpy(&sin.sin_addr, &ip_addr, sizeof sin.sin_addr);
    sin.sin_port   = htons(CONNECTOR_UDP_PORT);
    sin.sin_family = AF_INET;

    if (connect(fd, &sin, sizeof(sin)) < 0)
    {
        APP_DEBUG("network_udp_open: connect() failed, errno %d\n", errno);
        status = connector_callback_error;
    }
    return status;
}

static connector_callback_status_t app_network_udp_open(connector_network_open_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    in_addr_t ip_addr;
    static int fd = -1;

    data->handle = &fd;

    status = app_dns_resolve(connector_class_id_network_udp, data->device_cloud.url, &ip_addr);
    if (status != connector_callback_continue)
    {
        APP_DEBUG("app_network_udp_open: Can't resolve DNS for %s\n", data->device_cloud.url);
        goto done;
    }

    if (fd == -1)
    {
        fd = app_udp_create_socket();
        if (fd == -1)
        {
            status = connector_callback_error;
            goto done;
        }

        status = app_udp_connect(fd, ip_addr);
    }

    if ((status == connector_callback_error) && (fd >= 0))
    {
        close(fd);
        fd = -1;
    }

done:
    return status;
}


/*
 *  Callback routine to handle all networking related calls.
 */
connector_callback_status_t app_network_udp_handler(connector_request_id_network_t const request_id,
                                                    void * const data)
{
    connector_callback_status_t status;

    switch (request_id)
    {
    case connector_request_id_network_open:
        status = app_network_udp_open(data);
        break;

    case connector_request_id_network_send:
        status = app_network_udp_send(data);
        break;

    case connector_request_id_network_receive:
        status = app_network_udp_receive(data);
        break;

    case connector_request_id_network_close:
        status = app_network_udp_close(data);
        break;

    default:
        APP_DEBUG("app_network_udp_handler: unrecognized callback request_id [%d]\n", request_id);
        status = connector_callback_unrecognized;
        break;

    }

    return status;
}
#endif

