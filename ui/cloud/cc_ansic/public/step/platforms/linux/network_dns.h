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

#ifndef _NETWORK_DNS_H
#define _NETWORK_DNS_H

extern connector_callback_status_t app_dns_resolve(connector_class_id_t const class_id,
                                               char const * const domain_name,
                                               in_addr_t * const ip_addr);
extern void app_dns_set_redirected(connector_class_id_t const class_id, int const state);
extern void app_dns_cache_invalidate(connector_class_id_t const class_id);

#endif
