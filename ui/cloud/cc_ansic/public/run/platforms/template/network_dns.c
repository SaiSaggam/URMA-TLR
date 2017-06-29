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

#if defined (CONNECTOR_TRANSPORT_TCP) || (defined CONNECTOR_TRANSPORT_UDP)

void app_dns_set_redirected(connector_class_id_t const class_id, int const state)
{
    UNUSED_ARGUMENT(class_id);
    UNUSED_ARGUMENT(state);
}

void app_dns_cache_invalidate(connector_class_id_t const class_id)
{
    UNUSED_ARGUMENT(class_id);
}

connector_callback_status_t app_dns_resolve(connector_class_id_t const class_id,
                                       char const * const device_cloud_url,
                                       in_addr_t * const ip_addr)
{
    connector_callback_status_t status = connector_callback_continue;

    UNUSED_ARGUMENT(class_id);
    UNUSED_ARGUMENT(device_cloud_url);
    UNUSED_ARGUMENT(ip_addr);
    return status;
}
#endif

