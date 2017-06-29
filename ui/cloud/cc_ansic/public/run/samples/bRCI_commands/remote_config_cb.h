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

#ifndef REMOTE_CONFIG_CB_H_
#define REMOTE_CONFIG_CB_H_

#include "connector_debug.h"

typedef connector_callback_status_t(* remote_group_cb_t) (connector_remote_config_t * const remote_config);
typedef void (* remote_group_cancel_cb_t) (connector_remote_config_cancel_t * const remote_config);

typedef struct remote_group_table {
    remote_group_cb_t init_cb;
    remote_group_cb_t set_cb;
    remote_group_cb_t get_cb;
    remote_group_cb_t end_cb;
    remote_group_cancel_cb_t cancel_cb;
} remote_group_table_t;

typedef struct {
    remote_group_table_t * group;
    void * group_context;
    char * buf_ptr;
} remote_group_session_t;

extern connector_callback_status_t app_gps_stats_group_get(connector_remote_config_t * const remote_config);

extern connector_callback_status_t app_system_group_init(connector_remote_config_t * const remote_config);
extern connector_callback_status_t app_system_group_set(connector_remote_config_t * const remote_config);
extern connector_callback_status_t app_system_group_get(connector_remote_config_t * const remote_config);
extern connector_callback_status_t app_system_group_end(connector_remote_config_t * const remote_config);
extern void app_system_group_cancel(connector_remote_config_cancel_t * const remote_config);

/* These are device security callbacks in devicesecurity.c
* extern connector_callback_status_t app_device_security_group_init(connector_remote_config_t * const remote_config);
* extern connector_callback_status_t app_device_security_group_set(connector_remote_config_t * const remote_config);
* extern connector_callback_status_t app_device_security_group_get(connector_remote_config_t * const remote_config);
* extern connector_callback_status_t app_device_security_group_end(connector_remote_config_t * const remote_config);
* extern void app_device_security_group_cancel(connector_remote_config_cancel_t * const remote_config);
*/
#endif
