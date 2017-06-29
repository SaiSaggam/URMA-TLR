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
#include "connector_config.h"
#include "connector_api.h"
#include "platform.h"

#if !defined CONNECTOR_RCI_SERVICE
#error "Must define CONNECTOR_RCI_SERVICE in connector_config.h to run this sample"
#endif

typedef enum {
    no_fail,
    fail_start_session,
    fail_start_action,
    fail_start_group,
    fail_process_group,
    fail_end_group,
    fail_end_action,
    fail_end_session
} session_fail_state_t;

session_fail_state_t session_fail_state = no_fail;

static connector_callback_status_t app_process_session_start(connector_remote_config_t * const remote_config)
{

    APP_DEBUG("app_process_session_start\n");

    if (session_fail_state == fail_start_session)
    {
        remote_config->error_id = connector_global_error_memory_fail;
        remote_config->response.error_hint = NULL;
        session_fail_state = no_fail;
    }

    return connector_callback_continue;
}

static connector_callback_status_t app_process_session_end(connector_remote_config_t * const remote_config)
{
    APP_DEBUG("app_process_session_end\n");

    if (session_fail_state == fail_end_session)
    {
        remote_config->error_id = connector_global_error_memory_fail;
        remote_config->response.error_hint = NULL;
        session_fail_state = no_fail;
    }

    return connector_callback_continue;
}

static connector_callback_status_t app_process_action_start(connector_remote_config_t * const remote_config)
{

    APP_DEBUG("app_process_action_start\n");

    if (session_fail_state == fail_start_action)
    {
        remote_config->error_id = connector_global_error_memory_fail;
        remote_config->response.error_hint = NULL;
        session_fail_state = no_fail;
    }

    return connector_callback_continue;
}

static connector_callback_status_t app_process_action_end(connector_remote_config_t * const remote_config)
{
    APP_DEBUG("app_process_action_end\n");

    if (session_fail_state == fail_end_action)
    {
        remote_config->error_id = connector_global_error_memory_fail;
        remote_config->response.error_hint = NULL;
        session_fail_state = no_fail;
    }

    return connector_callback_continue;
}

static connector_callback_status_t app_process_group_start(connector_remote_config_t * const remote_config)
{

    if (session_fail_state == fail_start_group)
    {
        remote_config->error_id = connector_global_error_memory_fail;
        remote_config->response.error_hint = NULL;
        session_fail_state = no_fail;
    }

    return connector_callback_continue;
}

static connector_callback_status_t app_process_group_process(connector_remote_config_t * const remote_config)
{

    if (session_fail_state == fail_process_group)
    {
        remote_config->error_id = connector_global_error_load_fail;
        remote_config->response.error_hint = "Hint";
        session_fail_state = no_fail;
        goto done;
    }

    if (remote_config->action == connector_remote_action_query)
    {
        remote_config->response.element_value->string_value = "my data";
        goto done;
    }

    switch (remote_config->element.id)
    {
        case connector_setting_test_error_start_session:
            session_fail_state = fail_start_session;
            break;
        case connector_setting_test_error_start_action:
            session_fail_state = fail_start_action;
            break;
        case connector_setting_test_error_start_group:
            session_fail_state = fail_start_group;
            break;
        case connector_setting_test_error_process_group:
            session_fail_state = fail_process_group;
            break;
        case connector_setting_test_error_end_group:
            session_fail_state = fail_end_group;
            break;
        case connector_setting_test_error_end_action:
            session_fail_state = fail_end_action;
            break;
        case connector_setting_test_error_end_session:
            session_fail_state = fail_end_session;
            break;
    }

done:
    return connector_callback_continue;
}

static connector_callback_status_t app_process_group_end(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;

    if (session_fail_state == fail_end_group)
    {
        remote_config->error_id = connector_global_error_memory_fail;
        remote_config->response.error_hint = NULL;
        session_fail_state = no_fail;
    }

    return status;
}

static connector_callback_status_t app_process_session_cancel(connector_remote_config_cancel_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;

    UNUSED_ARGUMENT(remote_config);

    APP_DEBUG("app_process_session_cancel\n");

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
        status = app_process_group_start( data);
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
    }

    return status;
}

