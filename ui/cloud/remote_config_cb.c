/*****************************************************************************
* Copyright (c) 2016 Digi International Inc., All Rights Reserved
*
* This software contains proprietary and confidential information of Digi
* International Inc.  By accepting transfer of this copy, Recipient agrees
* to retain this software in confidence, to prevent disclosure to others,
* and to make no use of this software other than that for which it was
* delivered.  This is an unpublished copyrighted work of Digi International
* Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
* prohibited.
*
* Restricted Rights Legend
*
* Use, duplication, or disclosure by the Government is subject to
* restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
* Technical Data and Computer Software clause at DFARS 252.227-7031 or
* subparagraphs (c)(1) and (2) of the Commercial Computer Software -
* Restricted Rights at 48 CFR 52.227-19, as applicable.
*
* Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
*
****************************************************************************/
#include "connector_config.h"
#include "connector_api.h"
#include "platform.h"
#include "remote_config_cb.h"
#include "config_mgr.h"

typedef struct
{
    tlr_handle_t handle;
    char * buffer;
    bool need_save;
    connector_remote_group_type_t type;
} session_ptr_t;

static char const empty_ipv4[] = "0.0.0.0";

static connector_callback_status_t process_session_start(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t const status = connector_callback_continue;
    session_ptr_t * session = malloc(sizeof *session);

    APP_DEBUG("session start\n");

    if (session == NULL)
    {
        APP_DEBUG("malloc failed in session_start\n");
        remote_config->error_id = connector_global_error_memory_fail;
        goto done;
    }

    session->need_save = false;
    session->buffer = NULL;
    remote_config->user_context = session;
    remote_config->error_id = 0;

done:
    return status;
}

static connector_callback_status_t process_session_end(connector_remote_config_t * const remote_config)
{
    session_ptr_t * const session = remote_config->user_context;
    APP_DEBUG("session end\n");

    if (session == NULL) goto done;

    if (session->buffer != NULL)
    {
        free(session->buffer);
        session->buffer = NULL;
    }

    free(session);
    remote_config->user_context = NULL;

done:
    return connector_callback_continue;
}

static connector_callback_status_t process_action_end(connector_remote_config_t * const remote_config)
{
    session_ptr_t const * const session = remote_config->user_context;

    if (session == NULL)
    {
        APP_DEBUG("empty session in action_end\n");
        remote_config->error_id = connector_global_error_save_fail;
        goto done;
    }

    if (!session->need_save) goto done;

    if (system("action_save_config -f ascii -i 1 -u cloud") == -1)
    {
        APP_INFO("failed to save\n");
        remote_config->error_id = connector_global_error_save_fail;
    }

done:
    return connector_callback_continue;
}

static void skip_entry(connector_remote_config_t * const remote_config)
{
    switch (remote_config->action)
    {
        case connector_remote_action_query:
            remote_config->response.compare_matches = connector_true;
            break;
        case connector_remote_action_set:
            // @TODO: Create a new error: connector_global_error_element_not_settable and use it.
            remote_config->error_id = connector_global_error_COUNT;
            break;
    }
}

static connector_callback_status_t process_group_start(connector_remote_config_t * const remote_config)
{
    session_ptr_t * const session = remote_config->user_context;

    // APP_DEBUG("group %zd[%zd] start\n", remote_config->group.id, remote_config->group.index);

    if (session == NULL)
    {
        APP_DEBUG("empty session in group_start\n");
        remote_config->error_id = connector_global_error_load_fail;
        goto done;
    }

    session->type = remote_config->group.type;
    switch (remote_config->group.type)
    {
        case connector_remote_group_setting:
            session->handle = tlr_config_open(remote_config->group.id, remote_config->group.index, config_mgr_access_level_super);
            if (session->handle == NULL)
            {
                skip_entry(remote_config);
            }
            break;

        case connector_remote_group_state:
            if (tlr_is_json_supported(remote_config->group.id))
            {
                session->handle = tlr_status_open(remote_config->group.id, remote_config->group.index);
                remote_config->error_id = (session->handle == NULL) ? connector_rci_error_bad_command : 0;
            }
            else
            {
                session->handle = NULL;
                skip_entry(remote_config);
            }
            break;
    }

done:
    return connector_callback_continue;
}

static void * const get_data_ptr(connector_remote_config_t * const remote_config, bool * const is_string)
{
    bool const is_get = (remote_config->action == connector_remote_action_query);
    connector_element_value_t * const value = is_get ? remote_config->response.element_value : remote_config->element.value;
    void * data_ptr = NULL;

    *is_string = false;
    switch (remote_config->element.type)
    {
        case connector_element_type_string:
        case connector_element_type_password:
        case connector_element_type_fqdnv6:
        case connector_element_type_ipv4:
            if (is_get)
            {
                session_ptr_t * const session = remote_config->user_context;

                if (session->buffer != NULL)
                {
                    free(session->buffer);
                    session->buffer = NULL;
                }
            }
            else
                data_ptr = (void *)value->string_value;  /* unconsting */

            *is_string = true;
            break;

        case connector_element_type_int32:
            data_ptr = &value->signed_integer_value;
            break;

        case connector_element_type_uint32:
            data_ptr = &value->unsigned_integer_value;
            break;

        case connector_element_type_enum:
            data_ptr = &value->enum_value;
            break;

        case connector_element_type_on_off:
            data_ptr = &value->on_off_value;
            break;
    }

    return data_ptr;
}

static void access_state_element(connector_remote_config_t * const remote_config)
{
    bool is_string;
    config_status_t result = config_status_success;
    session_ptr_t * const session = remote_config->user_context;
    void * const data_ptr = get_data_ptr(remote_config, &is_string);

    if (is_string)
    {
        if (session->handle != NULL)
        {
            result = tlr_status_get_string_ptr(session->handle, remote_config->element.id, &remote_config->response.element_value->string_value);
        }
        else
        {
            remote_config->response.element_value->string_value = "";
        }
    }
    else
    {
        if (data_ptr == NULL)
        {
            APP_DEBUG("failed to read state element %zd[%zd]:%zd\n", remote_config->group.id, remote_config->group.index, remote_config->element.id);
            remote_config->error_id = connector_rci_error_bad_command;
            goto done;
        }

        if (session->handle != NULL)
        {
            static size_t const unused_size_param = 0;

            result = tlr_status_read(session->handle, remote_config->element.id, data_ptr, unused_size_param);
        }
        else
        {
            *(int *)data_ptr = 0;
        }
    }

    if (result != config_status_success)
    {
        APP_DEBUG("failed to access config element %zd[%zd]:%zd\n", remote_config->group.id, remote_config->group.index, remote_config->element.id);
        remote_config->error_id = connector_rci_error_bad_command;
    }

done:
    return;
}

static void access_config_element(connector_remote_config_t * const remote_config)
{
    session_ptr_t * const session = remote_config->user_context;

    if (session->handle == NULL)
    {
        APP_DEBUG("Empty handle in access_element\n");
        remote_config->error_id = connector_global_error_load_fail;
        goto done;
    }

    {
        bool is_string;
        void * const data_ptr = get_data_ptr(remote_config, &is_string);
        config_status_t result;

        switch (remote_config->action)
        {
            case connector_remote_action_set:

                if (data_ptr == NULL) goto done;

                if (is_string)
                {   /* @TODO: find out why empty ipv4 is invalid? */
                    bool const is_ipv4 = remote_config->element.type == connector_element_type_ipv4;
                    char const * const value = (is_ipv4 && strcmp(data_ptr, empty_ipv4) == 0) ? "" : data_ptr;

                    result = tlr_config_write(session->handle, remote_config->element.id, value);
                }
                else
                    result = tlr_config_write_native(session->handle, remote_config->element.id, data_ptr);

                if (result == config_status_success)
                    session->need_save = true;
                break;

            case connector_remote_action_query:
                if (is_string)
                {
                    result = tlr_config_read_alloc_value(session->handle, remote_config->element.id, (void **)&session->buffer, config_mgr_element_type_string);
                    remote_config->response.element_value->string_value = session->buffer;
                }
                else
                {
                    if (data_ptr == NULL) goto done;
                    result = tlr_config_read_native(session->handle, remote_config->element.id, data_ptr);
                }
                break;
        }

        if (result != config_status_success)
        {
            APP_DEBUG("failed to access config element %zd[%zd]:%zd\n", remote_config->group.id, remote_config->group.index, remote_config->element.id);
            remote_config->error_id = connector_rci_error_bad_command;
        }
    }

done:
    return;
}

static connector_callback_status_t process_element_access(connector_remote_config_t * const remote_config)
{
    session_ptr_t * const session = remote_config->user_context;

    if (session == NULL) goto done;

    switch (remote_config->group.type)
    {
        case connector_remote_group_setting:
            access_config_element(remote_config);
            break;

        case connector_remote_group_state:
            access_state_element(remote_config);
            break;
    }

    /* @TODO: the empty ipv4 string is not allowed, need find out why... */
    if (remote_config->action != connector_remote_action_query) goto done;
    if (remote_config->element.type != connector_element_type_ipv4) goto done;
    if (remote_config->response.element_value->string_value == NULL ||
        remote_config->response.element_value->string_value[0] == '\0')
    {
        remote_config->response.element_value->string_value = empty_ipv4;
    }

done:
    return connector_callback_continue;
}

static void apply_config_change(connector_remote_config_t * const remote_config)
{
    bool const success = tlr_apply_group(remote_config->group.id, remote_config->group.index, "cloud", config_mgr_access_level_super);

    if (!success)
    {
        remote_config->error_id = connector_global_error_load_fail;
    }
}

static void close_group_handle(session_ptr_t * const session)
{
    switch (session->type)
    {
        case connector_remote_group_setting:
            tlr_config_close(session->handle);
            break;

        case connector_remote_group_state:
            tlr_status_close(session->handle);
            break;
    }

    session->handle = NULL;
}

static connector_callback_status_t process_group_end(connector_remote_config_t * const remote_config)
{
    session_ptr_t * const session = remote_config->user_context;

    // APP_DEBUG("group %zd[%zd] end\n", remote_config->group.id, remote_config->group.index);

    if (session->handle == NULL) goto done;
    if (remote_config->action != connector_remote_action_set) goto close;
    if (!session->need_save) goto close;

    apply_config_change(remote_config);

close:
    close_group_handle(session);

done:
    return connector_callback_continue;
}

static connector_callback_status_t process_session_cancel(connector_remote_config_cancel_t * const remote_config)
{
    session_ptr_t * const session = remote_config->user_context;
    connector_callback_status_t status = connector_callback_continue;

    APP_DEBUG("session cancel\n");

    if (session == NULL) goto done;

    if (session->handle != NULL)
    {
        close_group_handle(session);
    }

    if (session->buffer != NULL)
    {
        free(session->buffer);
        session->buffer = NULL;
    }

    free(session);
    remote_config->user_context = NULL;

done:
    return status;
}

connector_callback_status_t app_remote_config_handler(connector_request_id_remote_config_t const request_id,
                                                      void * const data)
{
    connector_callback_status_t status = connector_callback_unrecognized;


    switch (request_id)
    {
    case connector_request_id_remote_config_session_start:
        status = process_session_start(data);
        break;

    case connector_request_id_remote_config_action_start:
        status = connector_callback_continue;
        break;

    case connector_request_id_remote_config_group_start:
        status = process_group_start(data);
        break;

    case connector_request_id_remote_config_group_process:
        status = process_element_access(data);
        break;

    case connector_request_id_remote_config_group_end:
        status = process_group_end(data);
        break;

    case connector_request_id_remote_config_action_end:
        status = process_action_end(data);
        break;

    case connector_request_id_remote_config_session_end:
        status = process_session_end(data);
        break;

    case connector_request_id_remote_config_session_cancel:
        status = process_session_cancel(data);
        break;

    default:
        APP_DEBUG("unknown request id %d\n", request_id);
        break;
    }

    return status;
}

