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
#define CONNECTOR_RCI_PARSER_INTERNAL_DATA
#define CONNECTOR_CONST_PROTECTION

#include "rci_binary.h"


STATIC void set_rci_service_error(msg_service_request_t * const service_request, connector_session_error_t const error_code)
{
    service_request->error_value = error_code;
    service_request->service_type = msg_service_type_error;
}

STATIC connector_status_t rci_service_callback(connector_data_t * const connector_ptr, msg_service_request_t * const service_request)
{
    connector_status_t status = connector_working;
    connector_session_error_t error_status = connector_session_error_none;
    msg_session_t * session;
    rci_service_data_t * service_data;

    ASSERT_GOTO(connector_ptr != NULL, done);
    ASSERT_GOTO(service_request != NULL, done);

    session = service_request->session;
    ASSERT_GOTO(session != NULL, done);

    service_data = session->service_context;

    switch (service_request->service_type)
    {
    case msg_service_type_need_data:
    case msg_service_type_have_data:
    {
        rci_status_t rci_status;
        rci_session_t parser_action = rci_session_active;

        ASSERT_GOTO(service_request->have_data != NULL, done);
        ASSERT_GOTO(service_request->need_data != NULL, done);

        if (session->service_context == NULL)
        {
            /* 1st time here so let's allocate service context memory for rci parser */
            void * ptr;

            status = malloc_data_buffer(connector_ptr, sizeof *service_data, named_buffer_id(msg_service), &ptr);
            if (status != connector_working)
            {
                if (status != connector_pending)
                {
                    error_status = connector_session_error_cancel;
                }
                goto done;
            }

            service_data = ptr;
            session->service_context = service_data;

            MsgSetStart(service_request->need_data->flags);

            connector_debug_print_buffer("rci_service.request", service_request->have_data->data_ptr, service_request->have_data->length_in_bytes);
            parser_action = rci_session_start;
        }

        ASSERT(service_data != NULL);

        service_data->connector_ptr = connector_ptr;

        service_data->input.data = service_request->have_data->data_ptr;
        service_data->input.bytes = service_request->have_data->length_in_bytes;

        service_data->output.data = service_request->need_data->data_ptr;
        service_data->output.bytes = service_request->need_data->length_in_bytes;

        service_data->input.flags = service_request->have_data->flags;
        service_data->output.flags = service_request->need_data->flags;
        rci_status = rci_binary(connector_ptr, parser_action, service_data);

        switch (rci_status)
        {
        case rci_status_complete:
            status = connector_working;
            MsgSetLastData(service_request->need_data->flags);
            MsgSetSendNow(service_request->need_data->flags);
            connector_debug_print_buffer("rci_service.response", service_data->output.data, service_data->output.bytes);
            service_request->need_data->length_in_bytes = service_data->output.bytes;
#if !(defined CONNECTOR_NO_MALLOC)
            free_rci_internal_data(connector_ptr);
#endif
            break;

        case rci_status_busy:
            status = connector_pending;
            break;

        case rci_status_more_input:
            status = connector_working;
            break;

        case rci_status_flush_output:
            MsgSetSendNow(service_request->need_data->flags);
            connector_debug_print_buffer("rci_service.response", service_data->output.data, service_data->output.bytes);
            service_request->need_data->length_in_bytes = service_data->output.bytes;
            status = connector_active;
            break;

        case rci_status_error:
            status = connector_abort;

            /* no break; */
        case rci_status_internal_error:
            error_status = connector_session_error_cancel;
            break;
        }
        break;
    }
    case msg_service_type_error:
    {
        rci_status_t const rci_status = rci_binary(connector_ptr, rci_session_lost, service_data);

        switch (rci_status)
        {
        case rci_status_complete:
            status = connector_working;
            break;

        case rci_status_busy:
            status = connector_pending;
            break;

        case rci_status_more_input:
        case rci_status_flush_output:
            ASSERT(connector_false);
            break;

        case rci_status_error:
            status = connector_abort;
            break;

        case rci_status_internal_error:
            /* just ignore */
            break;

        }
        break;
    }
    case msg_service_type_free:
        status = free_data_buffer(connector_ptr, named_buffer_id(msg_service), session->service_context);
        break;

    default:
        ASSERT(connector_false);
        break;

    }

done:
    if (error_status != connector_session_error_none)
    {
        set_rci_service_error(service_request, error_status);
    }

    return status;
}

STATIC connector_status_t connector_facility_rci_service_cleanup(connector_data_t * const connector_ptr)
{
    msg_service_id_t const service_id = msg_service_id_brci;
    return msg_cleanup_all_sessions(connector_ptr, service_id);
}

STATIC connector_status_t connector_facility_rci_service_delete(connector_data_t * const connector_ptr)
{
    msg_service_id_t const service_id = msg_service_id_brci;
    return msg_delete_facility(connector_ptr, service_id);
}

#if (defined CONNECTOR_DEBUG)
#if (defined CONNECTOR_FIRMWARE_SERVICE)
STATIC uint32_t get_fw_target_zero_version(connector_data_t const * const connector_ptr)
{
    connector_callback_status_t status;
    connector_request_id_t request;
    connector_firmware_info_t target_zero_data;

    request.firmware_request = connector_request_id_firmware_info;

    target_zero_data.target_number = 0;
    target_zero_data.version.major = 0;
    target_zero_data.version.minor = 0;
    target_zero_data.version.revision = 0;
    target_zero_data.version.build = 0;
    target_zero_data.description = NULL;
    target_zero_data.filespec = NULL;

    status = connector_callback(connector_ptr->callback, connector_class_id_firmware, request, &target_zero_data, connector_ptr->context);
    switch (status)
    {
        case connector_callback_continue:
            break;
        default:
            ASSERT(status == connector_callback_continue);
            break;
    }

    return FW_VERSION_NUMBER(target_zero_data.version);
}
#endif

STATIC void validate_rci_tuple(connector_data_t const * const connector_ptr, char const * const device_type, uint32_t const vendor_id, uint32_t const fw_target_zero_version)
{
#if (defined CONNECTOR_VENDOR_ID)
    uint32_t const reported_vendor_id = CONNECTOR_VENDOR_ID;
#else
    uint32_t const reported_vendor_id = connector_ptr->edp_data.config.vendor_id;
#endif
#if (defined CONNECTOR_DEVICE_TYPE)
    char const * const reported_device_type = CONNECTOR_DEVICE_TYPE;
#else
    char const * const reported_device_type = connector_ptr->edp_data.config.device_type;
#endif
#if (defined CONNECTOR_FIRMWARE_SERVICE)
    uint32_t const reported_fw_target_zero_version = get_fw_target_zero_version(connector_ptr);

    if (fw_target_zero_version != reported_fw_target_zero_version)
    {
        connector_debug_line("RCI: Using a descriptor for a different FW version (0x%x instead of 0x%x)", reported_fw_target_zero_version, fw_target_zero_version);
        ASSERT(fw_target_zero_version == reported_fw_target_zero_version);
    }
#else
    UNUSED_PARAMETER(connector_ptr);
    UNUSED_PARAMETER(fw_target_zero_version);
#endif
    if (device_type == NULL || strcmp(device_type, reported_device_type) != 0)
    {
        connector_debug_line("RCI: Using a descriptor for a different Device Type ('%s' instead of '%s')", reported_device_type, device_type);
        ASSERT(strcmp(device_type, reported_device_type) == 0);
    }

    if (vendor_id != reported_vendor_id)
    {
        connector_debug_line("RCI: Using a descriptor for a different Vendor ID (0x%x instead of 0x%x)", reported_vendor_id, vendor_id);
        ASSERT(vendor_id == reported_vendor_id);
    }
}
#endif

STATIC connector_status_t connector_facility_rci_service_init(connector_data_t * const connector_ptr, unsigned int const facility_index)
{
    connector_status_t result = connector_invalid_data;
    msg_service_id_t const service_id = msg_service_id_brci;
    connector_request_id_t request_id;
    connector_callback_status_t callback_status;
    connector_config_rci_descriptor_data_t config_rci_descriptor;

    config_rci_descriptor.rci_data = NULL;

    request_id.config_request = connector_request_id_config_rci_descriptor_data;
    callback_status = connector_callback(connector_ptr->callback, connector_class_id_config,
                                         request_id, &config_rci_descriptor, connector_ptr->context);
    switch (callback_status)
    {
        case connector_callback_unrecognized:
            result = connector_idle;
            goto done;

        case connector_callback_continue:
        {
            connector_remote_config_data_t const * const rci_data = config_rci_descriptor.rci_data;

            ASSERT_GOTO(rci_data != NULL, done);
            ASSERT(rci_data->group_table != NULL);
            ASSERT(rci_data->vendor_id != 0x00);
            ASSERT(rci_data->device_type != NULL);
            ASSERT(rci_data->error_table != NULL);
            ASSERT(rci_data->global_error_count >= connector_rci_error_COUNT);

#if (defined CONNECTOR_DEBUG)
            validate_rci_tuple(connector_ptr, rci_data->device_type, rci_data->vendor_id, rci_data->firmware_target_zero_version);
#endif
            connector_ptr->rci_data = rci_data;
            break;
        }
        default:
            result = connector_abort;
            goto done;

    }

    result = msg_init_facility(connector_ptr, facility_index, service_id, rci_service_callback);

done:
    return result;
}
