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
    session_ptr->buf_ptr = NULL;

done:
    remote_config->user_context = ptr;
    return status;
}

static connector_callback_status_t app_process_session_end(connector_remote_config_t * const remote_config)
{
    remote_group_session_t * const session_ptr = remote_config->user_context;

    APP_DEBUG("app_process_session_end\n");

    if (session_ptr != NULL)
    {
        if (session_ptr->buf_ptr != NULL)
        {
            free(session_ptr->buf_ptr);
        }

        free(session_ptr);
    }
    return connector_callback_continue;
}

static connector_callback_status_t app_process_action_start(connector_remote_config_t * const remote_config)
{
    APP_DEBUG("app_process_action_start: ");

    switch(remote_config->action)
    {
        case connector_remote_action_set:
            APP_DEBUG("connector_remote_action_set\n");
            break;
        case connector_remote_action_query:
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
                case rci_query_setting_attribute_compare_to_none:
                    APP_DEBUG("'rci_query_setting_attribute_compare_to_none'\n");
                    break;
                case rci_query_setting_attribute_compare_to_current:
                    APP_DEBUG("'rci_query_setting_attribute_compare_to_current'\n");
                    break;
                case rci_query_setting_attribute_compare_to_stored:
                    APP_DEBUG("'rci_query_setting_attribute_compare_to_stored'\n");
                    break;
                case rci_query_setting_attribute_compare_to_defaults:
                    APP_DEBUG("'rci_query_setting_attribute_compare_to_defaults'\n");
                    break;
            }
            break;
        case connector_remote_action_do_command:
            APP_DEBUG("connector_remote_action_do_command\n");
            break;
        case connector_remote_action_reboot:
            APP_DEBUG("connector_remote_action_reboot\n");
            break;
        case connector_remote_action_set_factory_def:
            APP_DEBUG("connector_remote_action_set_factory_def\n");
            break;
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


        if (session_ptr->buf_ptr != NULL)
        {
            free(session_ptr->buf_ptr);
        }

        free(session_ptr);
    }
    return status;
}

connector_callback_status_t app_process_do_command(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = (remote_group_session_t *)remote_config->user_context;
    char const * target = remote_config->attribute.target;
    char const * const request_payload = remote_config->element.value->string_value;
    char const * * response_payload = &remote_config->response.element_value->string_value;

    if (target != NULL)
    {
        APP_DEBUG("app_process_do_command for target '%s':\n", target);
    }
    else
    {
        APP_DEBUG("app_process_do_command with no target\n");
        *response_payload = "no target";
        goto done;
    }

    if (request_payload != NULL)
    {
        APP_DEBUG("request_payload len=%d\n", strlen(request_payload));
        APP_DEBUG("request_payload='%s'\n", request_payload);
    }
    else
    {
        APP_DEBUG("no payload!\n");
    }

    if (!strcmp(target, "echo"))
    {
        char * my_answer = malloc(strlen(request_payload) + 1);
        strcpy(my_answer, request_payload);

        /* Update session so buffer is freed in the session_end callback */
        session_ptr->buf_ptr = my_answer;
        *response_payload = my_answer;
    }
    else if (!strcmp(target, "ping"))
    {
        *response_payload = "pong";
    }
    else if (!strcmp(target, "no answer"))
    {
    }
    else if (!strcmp(target, "error"))
    {
        remote_config->error_id = connector_global_error_do_command_fail;
        remote_config->response.error_hint = "do command intentional failure";
    }
    else if (!strcmp(target, "busy"))
    {
        static int i = 0;

        if (i++ < 2) 
        {
            status = connector_callback_busy;
        }
        else
        {
            *response_payload = "I returned busy twice";
        }
    }
    else if (!strcmp(target, "malloc"))
    {
        #define ALLOCATED_MEM_SIZE 5000

        char * allocated_mem = malloc(ALLOCATED_MEM_SIZE + 1);

        if (allocated_mem != NULL)
        {
            memset(allocated_mem, '-', ALLOCATED_MEM_SIZE);

            allocated_mem[ALLOCATED_MEM_SIZE] = '\0';

            /* Update session so buffer is freed in the session_end callback */
            session_ptr->buf_ptr = allocated_mem;

            *response_payload = allocated_mem;
        }
    }
    else
    {
        *response_payload = "not supported command";
    }

done:
    return status;
}

int my_set_factory_default_went_wrong(void)
{
    return 0;
}

connector_callback_status_t app_process_set_factory_default(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;

    APP_DEBUG("app_process_set_factory_default\n");

    if (my_set_factory_default_went_wrong())
    {
        remote_config->error_id = connector_global_error_set_factory_default_fail;
        remote_config->response.error_hint = "can't do that";
    }

    return status;
}

int do_not_want_to_reboot(void)
{
    return 0;
}

connector_callback_status_t app_process_reboot(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;

    APP_DEBUG("app_process_reboot\n");

    if (do_not_want_to_reboot())
    {
        remote_config->error_id = connector_global_error_reboot_fail;
        remote_config->response.error_hint = "don't want to reboot";
    }
    else
    {
        APP_DEBUG("app_process_reboot: The system will be rebooted!\n");
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

    case connector_request_id_remote_config_do_command:
        status = app_process_do_command(data);
        break;

    case connector_request_id_remote_config_set_factory_def:
        status = app_process_set_factory_default(data);
        break;

    case connector_request_id_remote_config_reboot:
        status = app_process_reboot(data);
        break;

    default:
        APP_DEBUG("app_remote_config_handler: unknown request id %d\n", request_id);
        break;
    }

    return status;
}

