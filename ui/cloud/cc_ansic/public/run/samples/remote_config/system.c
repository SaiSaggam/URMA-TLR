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
#include "connector_config.h"
#include "connector_api.h"
#include "platform.h"
#include "remote_config_cb.h"

#define SYSTEM_STRING_LENGTH 64


typedef struct {
    char contact[SYSTEM_STRING_LENGTH];
    char location[SYSTEM_STRING_LENGTH];
    char description[SYSTEM_STRING_LENGTH];
} system_data_t;

system_data_t system_config_data = {"\0", "\0", "\0"};

connector_callback_status_t app_system_group_init(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    system_data_t * system_ptr;

    void * ptr;

    ASSERT(session_ptr != NULL);

    ptr = malloc(sizeof *system_ptr);
    if (ptr == NULL)
    {
        remote_config->error_id = connector_global_error_memory_fail;
        goto done;
    }

    system_ptr = ptr;
    *system_ptr = system_config_data;  /* load data */
    session_ptr->group_context = system_ptr;

done:
    return status;
}

connector_callback_status_t app_system_group_get(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    system_data_t * system_ptr;

    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);
    system_ptr = session_ptr->group_context;

    ASSERT(remote_config->element.type == connector_element_type_string);

    switch (remote_config->element.id)
    {
    case connector_setting_system_contact:
        remote_config->response.element_value->string_value = system_ptr->contact;
        break;
    case connector_setting_system_location:
        remote_config->response.element_value->string_value = system_ptr->location;

        break;
    case connector_setting_system_description:
        remote_config->response.element_value->string_value = system_ptr->description;
        break;
    default:
        ASSERT(0);
        break;
    }

    return status;
}

connector_callback_status_t app_system_group_set(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    system_data_t *  system_ptr;

    char * src_ptr = NULL;

    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    system_ptr = session_ptr->group_context;

    ASSERT(remote_config->element.type == connector_element_type_string);

    switch (remote_config->element.id)
    {
    case connector_setting_system_contact:
        ASSERT(strlen(remote_config->element.value->string_value) < sizeof system_ptr->contact);
        src_ptr = system_ptr->contact;
        break;
    case connector_setting_system_location:
        ASSERT(strlen(remote_config->element.value->string_value) < sizeof system_ptr->location);
        src_ptr = system_ptr->location;
        break;
    case connector_setting_system_description:
        ASSERT(strlen(remote_config->element.value->string_value) < sizeof system_ptr->description);
        src_ptr = system_ptr->description;
        break;
    default:
        ASSERT(0);
        break;
    }

    if (src_ptr != NULL)
    {
        size_t const length = strlen(remote_config->element.value->string_value);
        memcpy(src_ptr, remote_config->element.value->string_value, length);
        src_ptr[length] = '\0';
    }
    return status;
}

connector_callback_status_t app_system_group_end(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    system_data_t * system_ptr;

    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    system_ptr = session_ptr->group_context;

    if (remote_config->action == connector_remote_action_set)
    {
        /* save data */
        system_config_data = *system_ptr;
    }

    free(system_ptr);

    return status;
}

void app_system_group_cancel(connector_remote_config_cancel_t * const remote_config)
{
    remote_group_session_t * const session_ptr = remote_config->user_context;
    system_data_t * const system_ptr = session_ptr->group_context;

    if (system_ptr != NULL)
    {
        free(system_ptr);
    }

}

