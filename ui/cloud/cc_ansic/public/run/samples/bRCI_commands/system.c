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

system_data_t system_config_data[2] = {{"\0", "\0", "\0"},{"\0", "\0", "\0"}};

connector_callback_status_t app_system_group_init(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    system_data_t * system_ptr;
    system_data_t const * const system_persistent_ptr = &system_config_data[remote_config->group.index-1];

    void * ptr;

    /* Just a test: Skip reporting this group index for queries if 'compare_to' attribute is 'defaults' and content has not changed */
    if (remote_config->action == connector_remote_action_query && remote_config->attribute.compare_to == rci_query_setting_attribute_compare_to_defaults)
    {
        if (strlen(system_persistent_ptr->description) == 0 && strlen(system_persistent_ptr->contact) == 0 && strlen(system_persistent_ptr->location) == 0)
        {
            remote_config->response.compare_matches = connector_true;
            session_ptr->group_context = NULL;

            printf("skipping system group index %d\n", remote_config->group.index);
            goto done;
        }
    }

    ptr = malloc(sizeof *system_ptr);
    if (ptr == NULL)
    {
        remote_config->error_id = connector_global_error_memory_fail;
        goto done;
    }

    system_ptr = ptr;
    *system_ptr = *system_persistent_ptr; /* load data */
    session_ptr->group_context = system_ptr;

done:
    return status;
}

connector_callback_status_t app_system_group_get(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    system_data_t * const system_ptr = session_ptr->group_context;
    char const * data_ptr; 

    switch (remote_config->element.id)
    {
    case connector_setting_system_contact:
        data_ptr = system_ptr->contact;
        break;
    case connector_setting_system_location:
        data_ptr = system_ptr->location;
        break;
    case connector_setting_system_description:
        data_ptr = system_ptr->description;
        break;
    default:
        status = connector_callback_error;
        goto done;
        break;
    }

    /* Just a test: Skip reporting this element if 'compare_to' attribute is 'defaults' and content has not changed */
    if (remote_config->attribute.compare_to == rci_query_setting_attribute_compare_to_defaults && strlen(data_ptr) == 0)
    {
        remote_config->response.compare_matches = connector_true;
    }
    else
    {
        remote_config->response.element_value->string_value = data_ptr;
    }

done:
    return status;
}

connector_callback_status_t app_system_group_set(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    system_data_t * const system_ptr = session_ptr->group_context;

    char * src_ptr = NULL;

    switch (remote_config->element.id)
    {
    case connector_setting_system_contact:
        src_ptr = system_ptr->contact;
        break;
    case connector_setting_system_location:
        src_ptr = system_ptr->location;
        break;
    case connector_setting_system_description:
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
    system_data_t * const system_ptr = session_ptr->group_context;
    system_data_t * system_persistent_ptr = &system_config_data[remote_config->group.index-1];

    if (system_ptr == NULL)
    {
        goto done;
    }

    if (remote_config->action == connector_remote_action_set)
    {
        /* save data */
        *system_persistent_ptr = *system_ptr;
    }

    free(system_ptr);

done:
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

