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
#include <time.h>
#include <stdlib.h>
#include <malloc.h>
#include "connector_config.h"
#include "connector_api.h"
#include "platform.h"
#include "remote_config_cb.h"

#define DEVICESECURITY_PASSWORD_MAX_LENGTH 135

typedef struct {
    connector_setting_devicesecurity_identityVerificationForm_id_t identity_verification_form;
    char password[DEVICESECURITY_PASSWORD_MAX_LENGTH];
} device_security_data_t;

device_security_data_t device_security_data = {connector_setting_devicesecurity_identityVerificationForm_simple, "\0"};

connector_callback_status_t app_device_security_group_init(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    device_security_data_t * device_security_ptr;

    void * ptr;

    ASSERT(session_ptr != NULL);

    ptr = malloc(sizeof *device_security_ptr);
    if (ptr == NULL)
    {
        remote_config->error_id = connector_global_error_memory_fail;
        goto done;
    }

    device_security_ptr = ptr;
    *device_security_ptr = device_security_data; /* load data */
    session_ptr->group_context = device_security_ptr;

done:
    return status;
}

connector_callback_status_t app_device_security_group_get(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    device_security_data_t * device_security_ptr;

    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    device_security_ptr = session_ptr->group_context;

     switch (remote_config->element.id)
    {
    case connector_setting_devicesecurity_identityVerificationForm:
    {
        ASSERT(remote_config->element.type == connector_element_type_enum);
        remote_config->response.element_value->enum_value = device_security_ptr->identity_verification_form;
        break;
    }
    case connector_setting_devicesecurity_password:
        ASSERT(remote_config->element.type == connector_element_type_password);
        remote_config->response.element_value->string_value = device_security_ptr->password;
        break;

    default:
        ASSERT(0);
        break;
    }

    return status;
}

connector_callback_status_t app_device_security_group_set(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    device_security_data_t * device_security_ptr;

    ASSERT(remote_config->element.value != NULL);
    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    device_security_ptr = session_ptr->group_context;

    switch (remote_config->element.id)
    {
    case connector_setting_devicesecurity_identityVerificationForm:
        ASSERT(remote_config->element.type == connector_element_type_enum);

        device_security_ptr->identity_verification_form = (connector_setting_devicesecurity_identityVerificationForm_id_t)remote_config->element.value->enum_value;
        break;

    case connector_setting_devicesecurity_password:
    {
        size_t length = strlen(remote_config->element.value->string_value);

        ASSERT(remote_config->element.type == connector_element_type_password);
        ASSERT(length < sizeof device_security_ptr->password);

        memcpy(device_security_ptr->password, remote_config->element.value->string_value, length);
        device_security_ptr->password[length] = '\0';
        break;
    }

    default:
        ASSERT(0);
        break;
    }

    return status;
}

connector_callback_status_t app_device_security_group_end(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    device_security_data_t * device_security_ptr;

    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    device_security_ptr = session_ptr->group_context;

    if (remote_config->action == connector_remote_action_set)
    {
        /* save data */
        device_security_data = *device_security_ptr;
    }

    free(device_security_ptr);

    return status;
}

void app_device_security_group_cancel(connector_remote_config_cancel_t * const remote_config)
{
    remote_group_session_t * const session_ptr = remote_config->user_context;
    device_security_data_t * const device_security_ptr = session_ptr->group_context;

    if (device_security_ptr != NULL)
    {
        free(device_security_ptr);
    }

}

