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
#include <stdlib.h>

#include "connector_config.h"
#include "connector_api.h"
#include "platform.h"
#include "remote_config_cb.h"

typedef struct {
    int32_t signed_integer;
} device_state_config_data_t;

device_state_config_data_t device_state_config_data = {-10};

connector_callback_status_t app_device_state_group_init(connector_remote_config_t * const remote_config)
{

    remote_group_session_t * const session_ptr = remote_config->user_context;

    ASSERT(session_ptr != NULL);

    session_ptr->group_context = &device_state_config_data;

    return connector_callback_continue;
}

connector_callback_status_t app_device_state_group_get(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;

    device_state_config_data_t * device_state_ptr;

    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    device_state_ptr = session_ptr->group_context;

    switch (remote_config->element.id)
    {
    case connector_state_device_state_system_up_time:
    {
        unsigned long uptime;

        ASSERT(remote_config->element.type == connector_element_type_uint32);

        status = app_os_get_system_time(&uptime);
        remote_config->response.element_value->unsigned_integer_value = (uint32_t)uptime;

        break;
    }
    case connector_state_device_state_signed_integer:
        ASSERT(remote_config->element.type == connector_element_type_int32);
        remote_config->response.element_value->signed_integer_value = device_state_ptr->signed_integer;
        break;

    default:
        ASSERT(0);
        break;
    }

    return status;
}

connector_callback_status_t app_device_state_group_set(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;

    remote_group_session_t * const session_ptr = remote_config->user_context;
    device_state_config_data_t * device_state_ptr;

    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    device_state_ptr = session_ptr->group_context;

    switch (remote_config->element.id)
    {
    case connector_state_device_state_signed_integer:
        ASSERT(remote_config->element.type == connector_element_type_int32);
        device_state_ptr->signed_integer= remote_config->element.value->signed_integer_value;
        break;

    default:
        ASSERT(0);
        break;
    }

    return status;
}

