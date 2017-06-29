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
#include "connector_config.h"
#include "connector_api.h"
#include "platform.h"
#include "remote_config_cb.h"

#if !defined CONNECTOR_RCI_SERVICE
#error "Must define CONNECTOR_RCI_SERVICE in connector_config.h to run this sample"
#endif


remote_group_table_t remote_setting_table[] = {
    {app_system_group_init,
     app_system_group_set,
     app_system_group_get,
     app_system_group_end,
     app_system_group_cancel
    },
/* These are device security callbacks in devicesecurity.c
*    {app_device_security_group_init,
*     app_device_security_group_set,
*     app_device_security_group_get,
*     app_device_security_group_end,
*     app_device_security_group_cancel
*    }
*/
};


remote_group_table_t remote_state_table[] = {
    {NULL,
     NULL,
     app_gps_stats_group_get,
     NULL,
     NULL
    }
};


static connector_callback_status_t app_process_session_start(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    void * ptr;
    remote_group_session_t * session_ptr;

    APP_DEBUG("app_process_session_start\n");

    ptr = malloc(sizeof *session_ptr);
    if (ptr == NULL)
    {
        remote_config->error_id = connector_global_error_memory_fail;
        goto done;
    }

    session_ptr = ptr;
    session_ptr->group = NULL;
    session_ptr->group_context = NULL;

done:
    remote_config->user_context = ptr;
    return status;
}

static connector_callback_status_t app_process_session_end(connector_remote_config_t * const remote_config)
{
    APP_DEBUG("app_process_session_end\n");

    if (remote_config->user_context != NULL)
    {
        free(remote_config->user_context);
    }
    return connector_callback_continue;
}

static connector_callback_status_t app_process_action_start(connector_remote_config_t * const remote_config)
{
    APP_DEBUG("app_process_action_start\n");

    if (remote_config->action == connector_remote_action_query)
    {
        APP_DEBUG("connector_remote_action_query\n");
        APP_DEBUG("source=");
        switch (remote_config->attribute.source)
        {
            case rci_query_setting_attribute_source_current:
                APP_DEBUG("'rci_query_setting_attribute_source_current'\n");
                break;
            case rci_query_setting_attribute_source_stored:
                APP_DEBUG("'rci_query_setting_attribute_source_stored'\n");
                break;
            case rci_query_setting_attribute_source_defaults:
                APP_DEBUG("'rci_query_setting_attribute_source_defaults'\n");
                break;
        }
        APP_DEBUG("compare_to=");
        switch (remote_config->attribute.compare_to)
        {
            case rci_query_setting_attribute_compare_to_current:
                APP_DEBUG("'rci_query_setting_attribute_compare_to_current'\n");
                break;
            case rci_query_setting_attribute_compare_to_stored:
                APP_DEBUG("'rci_query_setting_attribute_compare_to_stored'\n");
                break;
            case rci_query_setting_attribute_compare_to_defaults:
                APP_DEBUG("'rci_query_setting_attribute_compare_to_defaults'\n");
                break;
            case rci_query_setting_attribute_compare_to_none:
                APP_DEBUG("'rci_query_setting_attribute_compare_to_none'\n");
                break;
        }
    }

    return connector_callback_continue;
}

static connector_callback_status_t app_process_action_end(connector_remote_config_t * const remote_config)
{
    APP_DEBUG("app_process_action_end\n");

    UNUSED_ARGUMENT(remote_config);
    return connector_callback_continue;
}

static remote_group_table_t * get_group_table(connector_remote_group_type_t group_type, unsigned int group_index)
{
    remote_group_table_t * group_ptr = NULL;

    switch (group_type)
    {
    case connector_remote_group_setting:

        if (group_index < ARRAY_SIZE(remote_setting_table))
        {
            group_ptr = &remote_setting_table[group_index];
        }
        break;
    case connector_remote_group_state:
        if (group_index < ARRAY_SIZE(remote_state_table))
        {
            group_ptr = &remote_state_table[group_index];
        }
        break;
    default:
        APP_DEBUG("get_group_table: group not found. type = %d index = %d\n", group_type, group_index);
    }

    return group_ptr;
}
static connector_callback_status_t app_process_group_start(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_table_t * const group_ptr = get_group_table(remote_config->group.type, remote_config->group.id);
    remote_group_session_t * const session_ptr = remote_config->user_context;

    ASSERT(group_ptr != NULL);

    if (group_ptr->init_cb != NULL)
    {
        status = group_ptr->init_cb(remote_config);
    }
    session_ptr->group = group_ptr;

    return status;
}

static connector_callback_status_t app_process_group_process(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    remote_group_table_t * const group_ptr = session_ptr->group;
    remote_group_cb_t callback;

    ASSERT(group_ptr != NULL);

    callback = (remote_config->action == connector_remote_action_set) ? group_ptr->set_cb : group_ptr->get_cb;

    if (callback)
    {
        status = callback(remote_config);
    }

    return status;
}
static connector_callback_status_t app_process_group_end(connector_remote_config_t * const remote_config)
    {
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    remote_group_table_t * const group_ptr = session_ptr->group;

    ASSERT(group_ptr != NULL);

    if (group_ptr->end_cb != NULL)
    {
        status = group_ptr->end_cb(remote_config);
    }

    return status;
}

static connector_callback_status_t app_process_session_cancel(connector_remote_config_cancel_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = (remote_group_session_t *)remote_config->user_context;

    APP_DEBUG("app_process_session_cancel\n");
    if (session_ptr != NULL)
    {
        remote_group_table_t * const group_ptr = session_ptr->group_context;

        if (group_ptr != NULL && group_ptr->cancel_cb != NULL)
            group_ptr->cancel_cb(remote_config);
        free(session_ptr);
    }
    return status;
}

connector_callback_status_t app_remote_config_handler(connector_request_id_remote_config_t const request_id,
                                                      void * const data)
{
    connector_callback_status_t status = connector_callback_unrecognized;


    switch (request_id)
    {
    case connector_request_id_remote_config_session_start:
        status = app_process_session_start(data);
        break;

    case connector_request_id_remote_config_action_start:
        status = app_process_action_start(data);
        break;

    case connector_request_id_remote_config_group_start:
        status = app_process_group_start(data);
        break;

    case connector_request_id_remote_config_group_process:
        status = app_process_group_process(data);
        break;

    case connector_request_id_remote_config_group_end:
        status = app_process_group_end(data);
        break;

    case connector_request_id_remote_config_action_end:
        status = app_process_action_end(data);
        break;

    case connector_request_id_remote_config_session_end:
        status = app_process_session_end(data);
        break;

    case connector_request_id_remote_config_session_cancel:
        status = app_process_session_cancel(data);
        break;
    default:
        APP_DEBUG("app_remote_config_handler: unknown request id %d\n", request_id);
        break;
    }

    return status;
}

