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

enum {
    parity_none,
    parity_odd,
    parity_even
};

enum {
    xbreak_off,
    xbreak_on
};
#define SERIAL_COUNT    2

#define SERIAL_INVALID_STORED_VALUE_HINT "Invalid value"
#define SERIAL_NO_MEMORY_HINT            "Memory"

typedef struct {
    unsigned int baud;
    unsigned int parity;
    unsigned int databits;
    unsigned char xbreak_enable;
} serial_config_data_t;

static uint32_t serial_txbytes[SERIAL_COUNT];

static serial_config_data_t serial_config_data[SERIAL_COUNT] = {
    {9600, parity_none, 8, xbreak_off},
    {115200, parity_none, 8, xbreak_off},
};

connector_callback_status_t app_serial_group_init(connector_remote_config_t * const remote_config)
{
    void * ptr;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    serial_config_data_t * serial_ptr = NULL;
    int group_index = remote_config->group.index -1;

    ASSERT(session_ptr != NULL);

    ptr = malloc(sizeof *serial_ptr);
    if (ptr == NULL)
    {
        remote_config->error_id = connector_global_error_memory_fail;
        goto done;
    }

    serial_ptr = ptr;

    *serial_ptr = serial_config_data[group_index];
done:
    session_ptr->group_context = serial_ptr;
    return connector_callback_continue;
}

connector_callback_status_t app_serial_group_get(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;

    remote_group_session_t * const session_ptr = remote_config->user_context;
    serial_config_data_t * serial_ptr;

    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    serial_ptr = session_ptr->group_context;

    switch (remote_config->element.id)
    {
    case connector_setting_serial_baud:
        switch (serial_ptr->baud)
        {
        case 2400:
            remote_config->response.element_value->enum_value = connector_setting_serial_baud_2400;
            break;
        case 4800:
            remote_config->response.element_value->enum_value = connector_setting_serial_baud_4800;
            break;
        case 9600:
            remote_config->response.element_value->enum_value = connector_setting_serial_baud_9600;
            break;
        case 19200:
            remote_config->response.element_value->enum_value = connector_setting_serial_baud_19200;
            break;
        case 38400:
            remote_config->response.element_value->enum_value = connector_setting_serial_baud_38400;
            break;
        case 57600:
            remote_config->response.element_value->enum_value = connector_setting_serial_baud_57600;
            break;
        case 115200:
            remote_config->response.element_value->enum_value = connector_setting_serial_baud_115200;
            break;
        case 230400:
            remote_config->response.element_value->enum_value = connector_setting_serial_baud_230400;
            break;
        default:
            remote_config->error_id = connector_setting_serial_error_invalid_baud;
            remote_config->response.error_hint = SERIAL_INVALID_STORED_VALUE_HINT;
            break;
        }
        break;
    case connector_setting_serial_parity:
        switch (serial_ptr->parity)
        {
        case parity_none:
            remote_config->response.element_value->enum_value = connector_setting_serial_parity_none;
            break;
        case parity_odd:
            remote_config->response.element_value->enum_value = connector_setting_serial_parity_odd;
            break;
        case parity_even:
            remote_config->response.element_value->enum_value = connector_setting_serial_parity_even;
            break;
        default:
            remote_config->error_id = connector_setting_serial_error_invalid_parity;
            remote_config->response.error_hint = SERIAL_INVALID_STORED_VALUE_HINT;
            break;
        }
        break;
    case connector_setting_serial_databits:
        remote_config->response.element_value->signed_integer_value = serial_ptr->databits;
        break;
    case connector_setting_serial_xbreak:
        remote_config->response.element_value->on_off_value = (serial_ptr->xbreak_enable) ? connector_on: connector_off;
        break;
    case connector_setting_serial_txbytes:
        remote_config->response.element_value->unsigned_integer_value = serial_txbytes[remote_config->group.index-1];
        break;
    default:
        ASSERT(0);
        break;
    }

    return status;
}

connector_callback_status_t app_serial_group_set(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;

    remote_group_session_t * const session_ptr = remote_config->user_context;
    serial_config_data_t * serial_ptr;


    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    serial_ptr = session_ptr->group_context;

    switch (remote_config->element.id)
    {
    case connector_setting_serial_baud:
    {
        unsigned int const serial_supported_baud_rates[] = { 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400};

        switch (remote_config->element.value->enum_value)
        {
        case connector_setting_serial_baud_2400:
        case connector_setting_serial_baud_4800:
        case connector_setting_serial_baud_9600:
        case connector_setting_serial_baud_19200:
        case connector_setting_serial_baud_38400:
        case connector_setting_serial_baud_57600:
        case connector_setting_serial_baud_115200:
        case connector_setting_serial_baud_230400:
            ASSERT(remote_config->element.type == connector_element_type_enum);
            serial_ptr->baud = serial_supported_baud_rates[remote_config->element.value->enum_value];
            break;
        default:
            remote_config->error_id = connector_setting_serial_error_invalid_baud;
            remote_config->response.error_hint = NULL;
            break;
        }
        break;
    }
    case connector_setting_serial_parity:
    {
        unsigned int const serial_supported_parity[] = {parity_none, parity_odd, parity_even};

        switch (remote_config->element.value->enum_value)
        {
        case connector_setting_serial_parity_none:
        case connector_setting_serial_parity_odd:
        case connector_setting_serial_parity_even:
            ASSERT(remote_config->element.type == connector_element_type_enum);
            serial_ptr->parity = serial_supported_parity[remote_config->element.value->enum_value];
            break;
        default:
            remote_config->error_id = connector_setting_serial_error_invalid_parity;
            remote_config->response.error_hint = NULL;
            break;
        }
        break;
    }
    case connector_setting_serial_databits:
        ASSERT(remote_config->element.type == connector_element_type_uint32);
        serial_ptr->databits = remote_config->element.value->unsigned_integer_value;
        break;
    case connector_setting_serial_xbreak:
        ASSERT(remote_config->element.type == connector_element_type_on_off);
        serial_ptr->xbreak_enable = (remote_config->element.value->on_off_value == connector_on) ? 1: 0;
        break;
    default:
        ASSERT(0);
        break;
    }

    return status;

}

connector_callback_status_t app_serial_group_end(connector_remote_config_t * const remote_config)
{

    remote_group_session_t * const session_ptr = remote_config->user_context;
    serial_config_data_t * serial_ptr;

    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    serial_ptr = session_ptr->group_context;

    if (remote_config->action == connector_remote_action_set)
    {
        serial_config_data[remote_config->group.index-1] = *serial_ptr;
    }

    if (serial_ptr != NULL)
    {
        free(serial_ptr);
    }

    return connector_callback_continue;
}

void app_serial_group_cancel(connector_remote_config_cancel_t * const remote_config)
{
    remote_group_session_t * const session_ptr = remote_config->user_context;

    if (session_ptr != NULL)
    {
        free(session_ptr->group_context);
    }

}

