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

#include <malloc.h>
#include "connector_config.h"
#include "connector_api.h"
#include "platform.h"
#include "remote_config_cb.h"

#define VALUE_TO_STRING(value)   # value
#define MACRO_TO_STRING(macro)   VALUE_TO_STRING(macro)


#define DEVICE_INFO_PRODUCT_LENGTH  64
#define DEVICE_INFO_MODEL_LENGTH  32
#define DEVICE_INFO_COMPANY_LENGTH  64
#define DEVICE_INFO_DESC_LENGTH  128

typedef struct {
    char product[DEVICE_INFO_PRODUCT_LENGTH];
    char model[DEVICE_INFO_MODEL_LENGTH];
    char company[DEVICE_INFO_COMPANY_LENGTH];
    char desc[DEVICE_INFO_DESC_LENGTH];
    size_t desc_length;
} device_info_config_data_t;

device_info_config_data_t device_info_config_data = {"Cloud Connector Product\0", "\0", "Digi International Inc.\0",
        "Cloud Connector Demo on Linux\nwith firmware upgrade, and remote configuration supports\0",
        102};

void print_device_info_desc(void)
{
    printf("%s", device_info_config_data.desc);
}
connector_callback_status_t app_device_info_group_init(connector_remote_config_t * const remote_config)
{

    void * ptr;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    device_info_config_data_t * device_info_ptr = NULL;

    ASSERT(session_ptr != NULL);

    ptr = malloc(sizeof *device_info_ptr);

    if (ptr == NULL)
    {
        remote_config->error_id = connector_global_error_memory_fail;
        goto done;
    }

    device_info_ptr = ptr;
    *device_info_ptr = device_info_config_data;

    session_ptr->group_context = device_info_ptr;
done:
    return connector_callback_continue;
}

connector_callback_status_t app_device_info_group_get(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;

    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    switch (remote_config->element.id)
    {
    case connector_setting_device_info_version:
        ASSERT(remote_config->element.type == connector_element_type_0x_hex32);
        remote_config->response.element_value->unsigned_integer_value = CONNECTOR_VERSION;
        break;
    case connector_setting_device_info_product:
    case connector_setting_device_info_model:
    case connector_setting_device_info_company:
    case connector_setting_device_info_desc:
    {
        device_info_config_data_t * const device_info_ptr = session_ptr->group_context;

        char * config_data[connector_setting_device_info_COUNT];

        config_data[connector_setting_device_info_version] = NULL;
        config_data[connector_setting_device_info_product] = device_info_ptr->product;
        config_data[connector_setting_device_info_model] = device_info_ptr->model;
        config_data[connector_setting_device_info_company] = device_info_ptr->company;
        config_data[connector_setting_device_info_desc] = device_info_ptr->desc;

        ASSERT(ARRAY_SIZE(config_data) == connector_setting_device_info_COUNT);

        remote_config->response.element_value->string_value = config_data[remote_config->element.id];
        if (remote_config->element.id == connector_setting_device_info_desc)
        {
            ASSERT(remote_config->element.type == connector_element_type_multiline_string);
        }
        else
        {
            ASSERT(remote_config->element.type == connector_element_type_string);
        }
        break;
    }
    default:
        ASSERT(0);
        break;
    }

    return status;
}

connector_callback_status_t app_device_info_group_set(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;

    device_info_config_data_t * device_info_ptr;
    char * src_ptr = NULL;

    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    device_info_ptr = session_ptr->group_context;

    switch (remote_config->element.id)
    {
    case connector_setting_device_info_product:
        ASSERT(strlen(remote_config->element.value->string_value) < sizeof device_info_ptr->product);
        src_ptr = device_info_ptr->product;
        break;
    case connector_setting_device_info_model:
        ASSERT(strlen(remote_config->element.value->string_value) < sizeof device_info_ptr->model);
        src_ptr = device_info_ptr->model;
        break;
    case connector_setting_device_info_company:
        ASSERT(strlen(remote_config->element.value->string_value) < sizeof device_info_ptr->company);
        src_ptr = device_info_ptr->company;
        break;
    case connector_setting_device_info_desc:
        ASSERT(strlen(remote_config->element.value->string_value) < sizeof device_info_ptr->desc);
        src_ptr = device_info_ptr->desc;
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

        if (remote_config->element.id == connector_setting_device_info_desc)
        {
             device_info_ptr->desc_length = length;
        }
    }

    return status;
}

connector_callback_status_t app_device_info_group_end(connector_remote_config_t * const remote_config)
{

    device_info_config_data_t * device_info_ptr;
    remote_group_session_t * const session_ptr = remote_config->user_context;

    /* save the data */

    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    device_info_ptr = session_ptr->group_context;

    if (remote_config->action == connector_remote_action_set)
    {
        device_info_config_data = *device_info_ptr;
    }

    if (device_info_ptr != NULL)
    {
        free(device_info_ptr);
    }
    return connector_callback_continue;
}

void app_device_info_group_cancel(connector_remote_config_cancel_t * const remote_config)
{
    remote_group_session_t * const session_ptr = remote_config->user_context;

    if (session_ptr != NULL)
    {
        free(session_ptr->group_context);
    }

}

