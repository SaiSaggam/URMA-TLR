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
#include "debug.h"

/* facility opcodes */
#define E_MSG_MT2_MSG_NUM          0x0000
#define E_MSG_FAC_FW_NUM           0x0070
#define E_MSG_FAC_RCI_NUM          0x00a0
#define E_MSG_FAC_MSG_NUM          0x00c0
#define E_MSG_FAC_DEV_LOOP_NUM     0xff00
#define E_MSG_FAC_CC_NUM           0xffff

#define named_buffer_id(name)  name##_static_buffer_id
typedef enum
{
    named_buffer_id(none) = 0,
    named_buffer_id(msg_facility),
    named_buffer_id(msg_session),
    named_buffer_id(msg_service),
    named_buffer_id(msg_session_client),
    named_buffer_id(put_request),
    named_buffer_id(sm_session),
    named_buffer_id(sm_data_block),
    named_buffer_id(sm_packet),
    named_buffer_id(data_point_block),
    named_buffer_id(connector_data) = 29,
    named_buffer_id(cc_facility) = 30,
    named_buffer_id(fw_facility) = 31

} connector_static_buffer_id_t;

#ifdef CONNECTOR_NO_MALLOC
STATIC connector_status_t malloc_static_data(connector_data_t * const connector_ptr,
                                                  size_t const size,
                                                  connector_static_buffer_id_t const buffer_id,
                                                  void ** const ptr);

STATIC void free_static_data(connector_data_t * const connector_ptr,
                                 connector_static_buffer_id_t const buffer_id,
                                 void * const ptr);
#endif


STATIC connector_callback_status_t connector_callback(connector_callback_t const callback, connector_class_id_t const class_id,  connector_request_id_t const request_id,
                                                                       void * const data, void * const context)
{
    connector_callback_status_t status = callback(class_id, request_id, data, context);

    switch (status)
    {
        case connector_callback_continue:
        case connector_callback_busy:
        case connector_callback_error:
            break;

        case connector_callback_unrecognized:
            connector_debug_line("connector_callback : callback returns unrecognized request for request=%d class_id = %d", request_id.config_request, class_id);
            break;

        case connector_callback_abort:
            connector_debug_line("connector_callback: callback for class id = %d request id = %d returned abort", class_id, request_id.config_request);
            break;

        default:
            connector_debug_line("connector_callback: callback for class id = %d request id = %d returned invalid return code %d", class_id, request_id.config_request, status);
            status = connector_callback_abort;
            break;
    }

    return status;
}


STATIC connector_status_t get_system_time(connector_data_t * const connector_ptr, unsigned long * const uptime)
{
    connector_status_t result = connector_abort;
    connector_callback_status_t status;
    connector_request_id_t request_id;
    connector_os_system_up_time_t data;

    *uptime = 0;

    /* Call callback to get system up time in second */
    request_id.os_request = connector_request_id_os_system_up_time;
    status = connector_callback(connector_ptr->callback, connector_class_id_operating_system, request_id, &data, connector_ptr->context);
    switch (status)
    {
    case connector_callback_continue:
        /* coverity[uninit_use] */
        *uptime = data.sys_uptime;
        result = connector_working;
        break;
    case connector_callback_abort:
        break;
    default:
        ASSERT(connector_false);
        break;
    }

    return result;
}

#if !(defined CONNECTOR_NO_MALLOC)
STATIC connector_status_t malloc_cb(connector_callback_t const callback, size_t const length, void ** ptr, void * const context)
{
    connector_status_t result = connector_working;
    connector_callback_status_t status;
    connector_os_malloc_t data;
    connector_request_id_t request_id;

    request_id.os_request = connector_request_id_os_malloc;
    data.size = length;
    data.ptr = *ptr = NULL;

    status = connector_callback(callback, connector_class_id_operating_system, request_id, &data, context);
    switch (status)
    {
    case connector_callback_continue:
        *ptr = data.ptr;
        if (data.ptr == NULL)
        {
            result = (notify_error_status(callback, connector_class_id_operating_system, request_id, connector_invalid_data, context) == connector_working) ? connector_pending : connector_abort;
        }
        break;

    case connector_callback_busy:
        *ptr = NULL;
        result = connector_pending;
        break;

    case connector_callback_abort:
    case connector_callback_unrecognized:
    case connector_callback_error:
        *ptr = NULL;
        result = connector_abort;
        break;
    }

    return result;
}

STATIC connector_status_t malloc_data(connector_data_t * const connector_ptr, size_t const length, void ** ptr)
{
    return malloc_cb(connector_ptr->callback, length, ptr, connector_ptr->context);
}

static connector_status_t realloc_data(connector_data_t * const connector_ptr, size_t const old_length, size_t const new_length, void ** ptr)
{
    connector_status_t result = connector_working;
    connector_callback_status_t status;
    connector_os_realloc_t data;
    connector_request_id_t request_id;

    request_id.os_request = connector_request_id_os_realloc;
    data.old_size = old_length;
    data.new_size = new_length;
    data.ptr = *ptr;

    status = connector_callback(connector_ptr->callback, connector_class_id_operating_system, request_id, &data, connector_ptr->context);
    switch (status)
    {
        case connector_callback_continue:
            *ptr = data.ptr;
            if (data.ptr == NULL)
            {
                result = (notify_error_status(connector_ptr->callback, connector_class_id_operating_system, request_id, connector_invalid_data, connector_ptr->context) == connector_working) ? connector_pending : connector_abort;
            }
            break;

        case connector_callback_busy:
            *ptr = NULL;
            result = connector_pending;
            break;

        case connector_callback_abort:
        case connector_callback_unrecognized:
        case connector_callback_error:
            *ptr = NULL;
            result = connector_abort;
            break;
    }

    return result;
}

STATIC connector_status_t free_data(connector_data_t * const connector_ptr, void * const ptr)
{
    connector_status_t result = connector_working;
    connector_request_id_t request_id;
    connector_os_free_t data;

    request_id.os_request = connector_request_id_os_free;
    data.ptr = ptr;

    {
        connector_callback_status_t const callback_status = connector_callback(connector_ptr->callback, connector_class_id_operating_system, request_id, &data, connector_ptr->context);
        switch (callback_status)
        {
            case connector_callback_continue:
                break;
            default:
                result = connector_abort;
                break;
        }
    }

    return result;
}
#endif


STATIC connector_status_t malloc_data_buffer(connector_data_t * const connector_ptr, size_t const length, connector_static_buffer_id_t id, void ** ptr)
{
    connector_status_t status;

#ifndef CONNECTOR_NO_MALLOC
    UNUSED_PARAMETER(id);
    status = malloc_data(connector_ptr, length, ptr);
#else
    status = malloc_static_data(connector_ptr, length, id, ptr);
#endif
    return status;
}

STATIC connector_status_t free_data_buffer(connector_data_t * const connector_ptr, connector_static_buffer_id_t id, void * const ptr)
{
    connector_status_t status;

#if !(defined CONNECTOR_NO_MALLOC)
    UNUSED_PARAMETER(id);
    status = free_data(connector_ptr, ptr);
#else
    status = connector_working;
    free_static_data(connector_ptr, id, ptr);
#endif

    return status;
}

STATIC connector_status_t yield_process(connector_data_t * const connector_ptr, connector_status_t const status)
{
    connector_status_t result = connector_working;

    {
        connector_request_id_t request_id;
        connector_os_yield_t data;
        connector_callback_status_t callback_status;

        request_id.os_request = connector_request_id_os_yield;
        data.status = status;

        callback_status = connector_callback(connector_ptr->callback, connector_class_id_operating_system, request_id, &data, connector_ptr->context);

        switch (callback_status)
        {
            case connector_callback_continue:
                break;
            default:
                result = connector_abort;
                break;
        }
    }

    return result;
}

STATIC connector_status_t connector_reboot(connector_data_t * const connector_ptr)
{
    connector_status_t result;

    connector_callback_status_t status = connector_callback_continue;
    connector_request_id_t request_id;

    connector_class_id_t class_id = connector_class_id_operating_system;
    request_id.os_request = connector_request_id_os_reboot;

    /* Device Cloud reboots us */

    status = connector_callback(connector_ptr->callback, class_id, request_id, NULL, connector_ptr->context);
    switch (status)
    {
    case connector_callback_continue:
        result = connector_working;
        break;
    case connector_callback_busy:
        result = connector_pending;
        break;
    default:
        result = connector_abort;
        break;
    }

    return result;

}
