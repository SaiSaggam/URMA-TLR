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

#define FW_ID_STRING_LENGTH     128  /* bytes */

/* time to send target list to keep download alive */
#define FW_TARGET_LIST_MSG_INTERVAL_IN_SECONDS     30

/**
 * Firmware Upgrade Facility Opcodes
 * @doc These are the valid opcodes for the Firmware Upgrade Facility
 */
typedef enum {
    fw_target_list_opcode,
    fw_info_request_opcode,
    fw_info_response_opcode,
    fw_download_request_opcode,
    fw_download_response_opcode,
    fw_binary_block_opcode,
    fw_binary_block_ack_opcode,
    fw_download_abort_opcode,
    fw_download_complete_opcode,
    fw_download_complete_response_opcode,
    fw_target_reset_opcode,
    fw_download_status_opcode,
    fw_error_opcode
} fw_opcodes_t;

typedef union {
    enum {
        fw_invalid_target,
        fw_invalid_opcode,
        fw_invalid_msg
    } error_status;

    enum {
        fw_user_abort,
        fw_device_error,
        fw_invalid_offset,
        fw_invalid_data,
        fw_hardware_error
    } abort_status;

    connector_firmware_status_t user_status;

} fw_abort_status_t;

/* Firmware message header format:
 *  ------------------------
 * |   0    |   1    | 3... |
 *  ------------------------
 * | opcode | target | data |
 *  ------------------------
 *
 */
enum fw_message {
    field_define(fw_message, opcode, uint8_t),
    field_define(fw_message, target, uint8_t),
    record_end(fw_message)
};
#define FW_MESSAGE_HEADER_SIZE record_bytes(fw_message)
#define FW_MESSAGE_RESPONSE_MAX_SIZE    16

enum fw_target_list_hdr {
    field_define(fw_target_list, opcode, uint8_t)
};

/* target + version pairs format: */
enum fw_target_list{
    field_define(fw_target_list, target, uint8_t),
    field_define(fw_target_list, version, uint32_t),
    record_end(fw_target_list)
};

static size_t const target_list_header_size = field_named_data(fw_target_list, opcode, size);
static size_t const target_list_size = record_bytes(fw_target_list);

typedef struct {
    connector_data_t * connector_ptr;
    unsigned long last_fw_keepalive_sent_time;

    size_t desc_length;
    size_t spec_length;
    size_t  response_size;
    connector_bool_t send_busy;
    connector_bool_t update_started;
    connector_bool_t fw_keepalive_start;
    connector_firmware_info_t target_info;

    uint8_t response_buffer[FW_MESSAGE_RESPONSE_MAX_SIZE + PACKET_EDP_FACILITY_SIZE];
    uint8_t target_count;
} connector_firmware_data_t;

STATIC connector_status_t get_fw_config(connector_firmware_data_t * const fw_ptr,
                                        connector_request_id_firmware_t const fw_request_id,
                                        void * const data)
{

    connector_data_t * const connector_ptr = fw_ptr->connector_ptr;
    connector_status_t result = connector_working;

    unsigned long end_time_stamp = 0;

    {
        connector_request_id_t request_id;
        connector_callback_status_t status;

        request_id.firmware_request = fw_request_id;
        status = connector_callback(connector_ptr->callback, connector_class_id_firmware, request_id, data, connector_ptr->context);

        if (get_system_time(connector_ptr, &end_time_stamp) != connector_working)
        {
            result = connector_abort;
            goto done;
        }

        switch (status)
        {
        case connector_callback_continue:
            break;
        case connector_callback_busy:
            result = connector_pending;
            break;
        case connector_callback_abort:
        case connector_callback_unrecognized:
        case connector_callback_error:
            result = connector_abort;
            goto done;
        }
    }

    if (result == connector_pending && fw_ptr->last_fw_keepalive_sent_time > 0)
    {
        /*
         * Check whether we need to send target list message
         * to keep connection alive.
         */
        fw_ptr->fw_keepalive_start = ((end_time_stamp - fw_ptr->last_fw_keepalive_sent_time) >= FW_TARGET_LIST_MSG_INTERVAL_IN_SECONDS) ? connector_true : connector_false;
    }
    else
    {
        fw_ptr->fw_keepalive_start = connector_false;
    }

done:
    return result;
}

STATIC fw_abort_status_t get_abort_status_code(connector_firmware_status_t const status)
{
    fw_abort_status_t code;

    code.abort_status = fw_user_abort;

    /* convert status to abort status code for abort message */
    switch (status)
    {
    case connector_firmware_status_user_abort:
        code.abort_status = fw_user_abort;
        break;
    case connector_firmware_status_invalid_offset:
        code.abort_status = fw_invalid_offset;
        break;
    case connector_firmware_status_invalid_data:
        code.abort_status = fw_invalid_data;
        break;
    case connector_firmware_status_hardware_error:
        code.abort_status = fw_hardware_error;
        break;
    case connector_firmware_status_device_error:
    case connector_firmware_status_download_denied:
    case connector_firmware_status_download_invalid_size:
    case connector_firmware_status_download_invalid_version:
    case connector_firmware_status_download_unauthenticated:
    case connector_firmware_status_download_not_allowed:
    case connector_firmware_status_download_configured_to_reject:
    case connector_firmware_status_encountered_error:
        /* not abort status so default to device error */
        code.abort_status = fw_device_error;
        break;
    case connector_firmware_status_success:
        ASSERT(connector_false);
        break;

    }
    return code;
}

/* abort and error message format:
 *  --------------------------
 * |   0    |    1   |    2   |
 *  --------------------------
 * | opcode | target | status |
 *  --------------------------
 *
 */
enum fw_abort {
    field_define(fw_abort, opcode, uint8_t),
    field_define(fw_abort, target, uint8_t),
    field_define(fw_abort, status, uint8_t),
    record_end(fw_abort)
};

#define FW_ABORT_HEADER_SIZE    record_bytes(fw_abort)

STATIC connector_status_t send_fw_message(connector_firmware_data_t * const fw_ptr)
{

    connector_status_t result;

    result = tcp_initiate_send_facility_packet(fw_ptr->connector_ptr, fw_ptr->response_buffer, fw_ptr->response_size, E_MSG_FAC_FW_NUM, NULL, NULL);
    fw_ptr->send_busy = (result == connector_pending) ? connector_true : connector_false;
    return result;

}

STATIC connector_status_t send_fw_abort(connector_firmware_data_t * const fw_ptr, uint8_t const target, uint8_t const msg_opcode, fw_abort_status_t const abort_status)
{

    connector_status_t result;

    uint8_t * fw_abort = GET_PACKET_DATA_POINTER(fw_ptr->response_buffer, PACKET_EDP_FACILITY_SIZE);
    uint8_t abort_code = (uint8_t)abort_status.error_status;

    ASSERT(abort_status.error_status <= UCHAR_MAX);

    /* need to adjust abort status code in the fw_status_t */
    if (msg_opcode != fw_error_opcode)
    {
        fw_abort_status_t status;
        status = get_abort_status_code(abort_status.user_status);

        ASSERT(status.abort_status <= UCHAR_MAX);
        abort_code = (uint8_t)status.abort_status;

    }

    ASSERT((sizeof fw_ptr->response_buffer - PACKET_EDP_FACILITY_SIZE) > FW_ABORT_HEADER_SIZE);

    /* build abort message */
    message_store_u8(fw_abort, opcode, msg_opcode);
    message_store_u8(fw_abort, target, target);
    message_store_u8(fw_abort, status, abort_code);

    fw_ptr->response_size = FW_ABORT_HEADER_SIZE;
    result = send_fw_message(fw_ptr);

    if (fw_ptr->target_info.target_number == target)
    {
        fw_ptr->update_started = connector_false;
    }
    return result;

}

static connector_status_t  process_fw_info_request(connector_firmware_data_t * const fw_ptr, uint8_t * const fw_message, uint16_t const length)
{
/* firmware info response message format:
 *  ---------------------------------------------------
 * |   0    |    1   |  2 - 5  |  6 - 9    |  10 ...   |
 *  ---------------------------------------------------
 * | opcode | target | version | Available | Firmware  |
 * |        |        |         | code size | ID string |
 *  ---------------------------------------------------
 *
 *  Firmware ID string = [descr]0xa[file name spec]
*/
enum fw_info {
    field_define(fw_info, opcode, uint8_t),
    field_define(fw_info, target, uint8_t),
    field_define(fw_info, version, uint32_t),
    field_define(fw_info, code_size, uint32_t),
    record_end(fw_info)
};

#define MAX_FW_INFO_REQUEST_LENGTH  2

    connector_data_t * const connector_ptr = fw_ptr->connector_ptr;
    connector_status_t result;
    uint8_t const target = message_load_u8(fw_message, target);

    connector_debug_line("Firmware Facility: process info request");
    /* parse firmware info request
     *  -----------------
     * |   0    |    1   |
     *  -----------------
     * | opcode | target |
     *  -----------------
     */
    fw_ptr->last_fw_keepalive_sent_time = 0;
    fw_ptr->fw_keepalive_start = connector_false;

    if (length != MAX_FW_INFO_REQUEST_LENGTH)
    {
        fw_abort_status_t fw_status;
        connector_debug_line("process_fw_info_request: invalid message length");

        fw_status.error_status = fw_invalid_msg;
        result = send_fw_abort(fw_ptr, target, fw_error_opcode, fw_status);
        goto done;

    }

    /* let's get and build target info response */
    {
        connector_firmware_info_t * firmware_info = &fw_ptr->target_info;

        firmware_info->target_number = target;

        result = get_fw_config(fw_ptr, connector_request_id_firmware_info, firmware_info);
        if (result != connector_working)
        {
            goto done;
        }

        fw_ptr->desc_length = strlen(firmware_info->description);
        fw_ptr->spec_length = strlen(firmware_info->filespec);

        if ((fw_ptr->desc_length + fw_ptr->spec_length) > (FW_ID_STRING_LENGTH -1))
        {
            connector_request_id_t request_id;

            request_id.firmware_request = connector_request_id_firmware_info;
            connector_debug_line("process_fw_info_request: description length = %lu + name spec length = %lu",
                                    (unsigned long int)fw_ptr->desc_length, (unsigned long int)fw_ptr->spec_length);
            notify_error_status(connector_ptr->callback, connector_class_id_firmware, request_id, connector_invalid_data_size, connector_ptr->context);
            fw_ptr->desc_length = 0;
            fw_ptr->spec_length = 0;
            result = connector_abort;
            goto done;
        }
    }

    /* let's build a response.
     * build and send firmware info response
    */
    {
        connector_firmware_info_t * firmware_info = &fw_ptr->target_info;

        uint8_t * edp_header;
        uint8_t * fw_info;
        uint8_t * start_ptr;
        size_t avail_length;

        edp_header = tcp_get_packet_buffer(connector_ptr, E_MSG_FAC_FW_NUM, &fw_info, &avail_length);
        if (edp_header == NULL)
        {
            result = connector_pending;
            goto done;
        }
        start_ptr = fw_info;

        ASSERT(avail_length > (record_bytes(fw_info) + fw_ptr->desc_length + fw_ptr->spec_length + 1));

        message_store_u8(fw_info, opcode, fw_info_response_opcode);
        message_store_u8(fw_info, target, target);
        message_store_be32(fw_info, version, FW_VERSION_NUMBER(firmware_info->version));
        message_store_be32(fw_info, code_size, INT32_C(0));
        fw_info += record_bytes(fw_info);

        connector_debug_line("firmware description = %d %s %s", fw_ptr->desc_length, firmware_info->description, firmware_info->filespec);
        if (firmware_info->description != NULL)
        {
            memcpy(fw_info, firmware_info->description, fw_ptr->desc_length);
            fw_info += fw_ptr->desc_length;
        }
        *fw_info++ = '\n';

        if (firmware_info->filespec != NULL)
        {
            memcpy(fw_info, firmware_info->filespec, fw_ptr->spec_length);
            fw_info += fw_ptr->spec_length;
        }

        /* reset back to initial values */
        firmware_info->version.major = 0;
        firmware_info->version.minor = 0;
        firmware_info->version.revision = 0;
        firmware_info->version.build = 0;
        fw_ptr->desc_length = 0;
        fw_ptr->spec_length = 0;

        {
            size_t const send_packet_length = (size_t)(fw_info-start_ptr);

            result = tcp_initiate_send_facility_packet(connector_ptr, edp_header, send_packet_length, E_MSG_FAC_FW_NUM, tcp_release_packet_buffer, NULL);

            if (result != connector_working)
            {
                tcp_release_packet_buffer(connector_ptr, edp_header, connector_working, NULL);
            }
        }
    }

done:
    return result;
}

STATIC connector_status_t process_fw_download_request(connector_firmware_data_t * const fw_ptr, uint8_t * fw_download_request, uint16_t const length)
{
#define FW_STRING_ID_ITEMS  2

/* Firmware download request message format:
 *  -----------------------------------------------------------
 * |   0    |   1    |  2 - 5  |  6 - 9    |  10...             |
 *  -----------------------------------------------------------
 * | opcode | target | version | code size | firmware ID string |
 *  ------------------------------------------------------------
 *
 *  Firmware ID string: [label]0x0a[file name spec]0xa[file name]
 *
 *  Call the callback with these values and send download request response.
 */
enum fw_download_request {
    field_define(fw_download_request, opcode, uint8_t),
    field_define(fw_download_request, target, uint8_t),
    field_define(fw_download_request, version, uint32_t),
    field_define(fw_download_request, code_size, uint32_t),
    record_end(fw_download_request)
};


/* Firmware download response message format:
 *  ---------------------------------
 * |  0     |   1    |     2         |
 *  ---------------------------------
 * | opcode | target | response type |
 *  ---------------------------------
 *
 */
enum fw_download_response {
    field_define(fw_download_response, opcode, uint8_t),
    field_define(fw_download_response, target, uint8_t),
    field_define(fw_download_response, response_type, uint8_t),
    record_end(fw_download_response)
};

    connector_status_t result = connector_working;
    connector_firmware_download_start_t download_request;
    fw_abort_status_t response_status;
    size_t string_id_length;

    uint8_t abort_opcode = fw_download_abort_opcode;

    download_request.target_number = message_load_u8(fw_download_request, target);

    response_status.user_status = connector_firmware_status_device_error;
    if (length < record_bytes(fw_download_request))
    {
        connector_debug_line("process_fw_download_request: invalid message length");
        abort_opcode = fw_error_opcode;
        response_status.error_status = fw_invalid_msg;
        goto error;
    }

    if (fw_ptr->update_started == connector_true)
    {
        connector_debug_line("process_fw_download_request: cannot start another firmware update target %d", download_request.target_number);
        goto error;
    }

    /* Parse firmware download request. Then, call the callback
     * with these values and send download request response.
     */
    download_request.code_size = message_load_be32(fw_download_request, code_size);
    string_id_length = (size_t)(length - record_bytes(fw_download_request));

    {
        char * string_id_ptr = (char *)fw_download_request;
        unsigned int i;

        string_id_ptr += record_bytes(fw_download_request);

        /* parse firmware ID String for label and filename spec
         * separated by 0x0a.
         */
        for (i=0; i < FW_STRING_ID_ITEMS; i++)
        {
            char * end_ptr;

            end_ptr = strchr(string_id_ptr, '\n');
            if (end_ptr != NULL) *end_ptr = '\0';
            {
                size_t const label_length = (size_t)(end_ptr - string_id_ptr);
                string_id_length -= (UINT32_C(1) + label_length);
            }
            string_id_ptr++;
        }
        /* get filename */
        download_request.filename = string_id_ptr;
        *(download_request.filename + string_id_length) = '\0';
    }

    /* call callback */
    download_request.status = connector_firmware_status_success;
    result = get_fw_config(fw_ptr, connector_request_id_firmware_download_start, &download_request);
    if (result == connector_working) response_status.user_status = download_request.status;

error:
    if (result != connector_pending)
    {
        uint8_t * fw_download_response = GET_PACKET_DATA_POINTER(fw_ptr->response_buffer, PACKET_EDP_FACILITY_SIZE);

        if (response_status.user_status >= connector_firmware_status_user_abort)
        {
            send_fw_abort(fw_ptr, download_request.target_number, abort_opcode, response_status);
            goto done;
        }

        /* get a buffer for sending a response */
        ASSERT((sizeof fw_ptr->response_buffer - PACKET_EDP_FACILITY_SIZE) > record_bytes(fw_download_response));

        /* send firmware download response */
        message_store_u8(fw_download_response, opcode, fw_download_response_opcode);
        message_store_u8(fw_download_response, target, download_request.target_number);
        message_store_u8(fw_download_response, response_type, download_request.status);

        fw_ptr->response_size = record_bytes(fw_download_response);

        send_fw_message(fw_ptr);
        if (download_request.status == connector_firmware_status_success)
        {
            fw_ptr->update_started = connector_true;
            fw_ptr->target_info.target_number = download_request.target_number;
        }

    }

done:
    return result;
}

STATIC connector_status_t process_fw_binary_block(connector_firmware_data_t * const fw_ptr, uint8_t * const fw_binary_block, uint16_t const length)
{
/* Firmware binary block message format:
 *  --------------------------------------------------------
 * |   0    |   1    |     2        |  3 - 6  |   7..       |
 *  --------------------------------------------------------
 * | opcode | target | Ack required |  offset | binary data |
 *  --------------------------------------------------------
 *
 */
enum fw_binary_block {
    field_define(fw_binary_block, opcode, uint8_t),
    field_define(fw_binary_block, target, uint8_t),
    field_define(fw_binary_block, ack_required, uint8_t),
    field_define(fw_binary_block, offset, uint32_t),
    record_end(fw_binary_block)
};

/* Firmware binary block acknowledge message format:
 *  -----------------------------------
 * |   0    |    1   | 2 - 5  |    6   |
 *  -----------------------------------
 * | opcode | target | offset | status |
 *  -----------------------------------
 *
 */
enum fw_binary_ack {
    field_define(fw_binary_ack, opcode, uint8_t),
    field_define(fw_binary_ack, target, uint8_t),
    field_define(fw_binary_ack, offset, uint32_t),
    field_define(fw_binary_ack, status, uint8_t),
    record_end(fw_binary_ack)
};
    connector_status_t result = connector_idle;

    uint8_t ack_required;
    connector_firmware_download_data_t download_data;

    /* Parse firmware binary block */
    download_data.target_number = message_load_u8(fw_binary_block, target);

    if (fw_ptr->update_started == connector_false)
    {
        /* Ignore this packet since we have not started downloading.
         * We may already abort download request.
         */
        goto done;
    }

    if ((fw_ptr->target_info.target_number != download_data.target_number) || (length < record_bytes(fw_binary_block)))
    {
        fw_abort_status_t fw_status;

        connector_debug_line("process_fw_binary_block: invalid target or message length");
        fw_status.error_status = fw_invalid_msg;
        result = send_fw_abort(fw_ptr, download_data.target_number, fw_error_opcode, fw_status);
        goto done;
    }

    /* Parse firmware binary block */
    ack_required = message_load_u8(fw_binary_block, ack_required);
    download_data.image.offset = message_load_be32(fw_binary_block, offset);
    download_data.image.bytes_used = (size_t)(length - record_bytes(fw_binary_block));

    download_data.image.data = (fw_binary_block + record_bytes(fw_binary_block));
    download_data.status = connector_firmware_status_success;

    result = get_fw_config(fw_ptr, connector_request_id_firmware_download_data, &download_data);

    if (result == connector_working && download_data.status == connector_firmware_status_success)
    {

        if(ack_required)
        {
            uint8_t * fw_binary_ack = GET_PACKET_DATA_POINTER(fw_ptr->response_buffer, PACKET_EDP_FACILITY_SIZE);

            ASSERT((sizeof fw_ptr->response_buffer - PACKET_EDP_FACILITY_SIZE) > record_bytes(fw_binary_ack));
            /* send firmware binary block acknowledge */
            message_store_u8(fw_binary_ack, opcode, fw_binary_block_ack_opcode);
            message_store_u8(fw_binary_ack, target, download_data.target_number);
            message_store_be32(fw_binary_ack, offset, download_data.image.offset);
            message_store_u8(fw_binary_ack, status, connector_firmware_status_success);

            fw_ptr->response_size = record_bytes(fw_binary_ack);
            result = send_fw_message(fw_ptr);
        }
    }
    else if (result != connector_pending)
    {
        fw_abort_status_t fw_status;
        fw_status.user_status = download_data.status;
        result = send_fw_abort(fw_ptr, download_data.target_number, fw_download_abort_opcode, fw_status);
    }
done:
    return result;
}

STATIC connector_status_t process_fw_abort(connector_firmware_data_t * const fw_ptr, uint8_t * const fw_abort, uint16_t const length)
{

    connector_status_t result = connector_working;

    /* parse firmware download abort */
    if (length != FW_ABORT_HEADER_SIZE)
    {
        connector_debug_line("process_fw_abort: invalid message length");
    }
    else if (fw_ptr->update_started)
    {
        connector_firmware_download_abort_t request_data;
        uint8_t abort_status = message_load_u8(fw_abort, status);

        request_data.target_number = message_load_u8(fw_abort, target);
        switch (abort_status)
        {
        case fw_user_abort:
            request_data.status = connector_firmware_status_user_abort;
            break;
        case fw_device_error:
            request_data.status = connector_firmware_status_device_error;
            break;
        case fw_invalid_offset:
            request_data.status = connector_firmware_status_invalid_offset;
            break;
        case fw_invalid_data:
            request_data.status = connector_firmware_status_invalid_data;
            break;
        case fw_hardware_error:
            request_data.status = connector_firmware_status_hardware_error;
            break;
        default:
            ASSERT(connector_false);
            break;
        }

        /* call callback */
        if (fw_ptr->target_info.target_number == request_data.target_number)
        {
            result = get_fw_config(fw_ptr, connector_request_id_firmware_download_abort, &request_data);
            if (result != connector_pending)
            {
                fw_ptr->update_started = connector_false;
            }
            fw_ptr->last_fw_keepalive_sent_time = 0;
        }
    }
    return result;

}

STATIC connector_status_t process_fw_complete(connector_firmware_data_t * const fw_ptr, uint8_t * const fw_complete_request, uint16_t const length)
{

/* Firmware download complete message format:
 *  ----------------------------------------
 * |   0    |   1    |   2 - 5   |  6 - 9   |
 *  ----------------------------------------
 * | opcode | target | code size | checksum |
 *  ----------------------------------------
 *
 */
enum fw_complete_request {
    field_define(fw_complete_request, opcode, uint8_t),
    field_define(fw_complete_request, target, uint8_t),
    field_define(fw_complete_request, code_size, uint32_t),
    field_define(fw_complete_request, checksum, uint32_t),
    record_end(fw_complete_request)
};

/* Firmware download complete response message format:
 *  -------------------------------------------------
 * |   0    |   1    |  2 - 5  |  6 - 9     |  10    |
 *  --------------------------------------------------
 * | opcode | target | version | calculated | status |
 * |        |        |         |  checksum  |        |
 *  --------------------------------------------------
 */
enum fw_complete_response {
    field_define(fw_complete_response, opcode, uint8_t),
    field_define(fw_complete_response, target, uint8_t),
    field_define(fw_complete_response, version, uint32_t),
    field_define(fw_complete_response, checksum, uint32_t),
    field_define(fw_complete_response, status, uint8_t),
    record_end(fw_complete_response)
};

    connector_status_t result;
    connector_firmware_download_complete_t download_complete;

    download_complete.target_number = message_load_u8(fw_complete_request, target);
    download_complete.status = connector_firmware_download_success;

    if ((length != record_bytes(fw_complete_request)) ||
        (fw_ptr->update_started == connector_false) ||
        (fw_ptr->target_info.target_number != download_complete.target_number))
    {
        fw_abort_status_t  fw_status;

        connector_debug_line("process_fw_complete: invalid message length, invalid target or no firmware update started");
        fw_status.error_status = fw_invalid_msg;
        result = send_fw_abort(fw_ptr, download_complete.target_number, fw_error_opcode, fw_status);
        goto done;
    }


    /* call callback */
    result = get_fw_config(fw_ptr, connector_request_id_firmware_download_complete, &download_complete);
    if (result == connector_working)
    {
        uint8_t * fw_complete_response = GET_PACKET_DATA_POINTER(fw_ptr->response_buffer, PACKET_EDP_FACILITY_SIZE);

        ASSERT((sizeof fw_ptr->response_buffer - PACKET_EDP_FACILITY_SIZE) > record_bytes(fw_complete_response));

        /* send firmware download complete response */
        message_store_u8(fw_complete_response, opcode, fw_download_complete_response_opcode);
        message_store_u8(fw_complete_response, target, download_complete.target_number);
        message_store_be32(fw_complete_response, version, INT32_C(0));
        message_store_be32(fw_complete_response, checksum, INT32_C(0));
        message_store_u8(fw_complete_response, status, download_complete.status);

        fw_ptr->last_fw_keepalive_sent_time = 0;
        fw_ptr->fw_keepalive_start = connector_false;

        fw_ptr->response_size = record_bytes(fw_complete_response);
        result = send_fw_message(fw_ptr);
        fw_ptr->update_started = connector_false;
    }
    else if (result != connector_pending)
    {
        fw_abort_status_t fw_status;

        fw_status.user_status = connector_firmware_status_user_abort;
        send_fw_abort(fw_ptr, download_complete.target_number, fw_download_abort_opcode, fw_status);
    }


done:
    return result;

}

STATIC connector_status_t process_target_reset(connector_firmware_data_t * const fw_ptr, uint8_t * const fw_message, uint16_t const length)
{
    connector_status_t result;
    connector_firmware_reset_t firmware_reset;

    UNUSED_PARAMETER(length);
    connector_debug_line("Firmware Facility: process target reset");

    firmware_reset.target_number = message_load_u8(fw_message, target);

    result = get_fw_config(fw_ptr, connector_request_id_firmware_target_reset, &firmware_reset);

    return result;
}

STATIC connector_status_t send_discovery_packet_callback(connector_data_t * const connector_ptr, uint8_t const * const packet,
                                           connector_status_t const send_status, void * const user_data)
{
    connector_status_t result;
    connector_firmware_data_t * const fw_ptr = user_data;
    /* update fw download keepalive timing */
    result = get_system_time(connector_ptr, &fw_ptr->last_fw_keepalive_sent_time);

    tcp_release_packet_buffer(connector_ptr, packet, send_status, user_data);

    return result;
}

STATIC connector_status_t fw_discovery(connector_data_t * const connector_ptr, void * const facility_data,
                                            uint8_t * const packet, unsigned int * receive_timeout)
{
/* Firmware target list message format:
 *
 * --------------------------------------------------------
 * |   0    |    1   |  2 - 5  |  6 ...                    |
 *  -------------------------------------------------------
 * | opcode | target | version | Additional target-version |
 * |        |        |         |       pairs               |
 *  -------------------------------------------------------
 *
 */

    connector_status_t result = connector_idle;
    connector_firmware_data_t * const fw_ptr = facility_data;

    UNUSED_PARAMETER(packet);
    UNUSED_PARAMETER(receive_timeout);

    /* Construct a target list message.
     * Get target count and then get version for each target to build target list message
     *
     */

    if (fw_ptr->target_count > 0)
    {
        uint8_t * edp_header;
        uint8_t * fw_target_list;
        size_t avail_length;
        size_t discovery_length = 0;
        uint8_t target_number;

        /* get packet pointer for constructing target list info */
        edp_header = tcp_get_packet_buffer(connector_ptr, E_MSG_FAC_FW_NUM, &fw_target_list, &avail_length);
        if (edp_header == NULL)
        {
            result = connector_pending;
            goto done;
        }

        message_store_u8(fw_target_list, opcode, fw_target_list_opcode);

        discovery_length = target_list_header_size;
        fw_target_list += discovery_length;

        for (target_number=0; target_number < fw_ptr->target_count; target_number++)
        {
            connector_firmware_info_t firmware_info;

            /* get the current firmware version for this target */
            firmware_info.target_number = target_number;
            memset(&firmware_info.version, 0x00, sizeof firmware_info.version);
            /* call callback */
            result = get_fw_config(fw_ptr, connector_request_id_firmware_info, &firmware_info);
            if (result == connector_working)
            {
                message_store_u8(fw_target_list, target, firmware_info.target_number);
                message_store_be32(fw_target_list, version, FW_VERSION_NUMBER(firmware_info.version));

                /* set up for next target pair info*/
                fw_target_list += target_list_size;
                discovery_length += target_list_size;
            }
            else
            {
                goto error;
            }
        }

        result = tcp_initiate_send_facility_packet(connector_ptr, edp_header, discovery_length,
                                               E_MSG_FAC_FW_NUM, send_discovery_packet_callback, fw_ptr);
error:
        if (result != connector_working)
        {
            tcp_release_packet_buffer(connector_ptr, edp_header, connector_working, NULL);
        }
    }

done:
    return result;
}

STATIC connector_status_t fw_process(connector_data_t * const connector_ptr, void * const facility_data,
                                          uint8_t * const edp_header, unsigned int * const receive_timeout)
{
    connector_status_t result = connector_idle;
    connector_firmware_data_t * const fw_ptr = facility_data;
    uint8_t opcode;
    uint8_t target;
    uint8_t * fw_message;
    uint16_t length;

    if (edp_header == NULL)
    {
        if (fw_ptr->update_started)
        {
            result = connector_pending;
        }
        goto done;
    }

    if (fw_ptr->fw_keepalive_start)
    {
        result = fw_discovery(connector_ptr, facility_data, edp_header, receive_timeout);
        if (result == connector_working)
        {
            if (get_system_time(connector_ptr, &fw_ptr->last_fw_keepalive_sent_time) != connector_working)
            {
                result = connector_abort;
                goto done;
            }
            fw_ptr->fw_keepalive_start = connector_false;
            result = connector_pending;
        }
        goto done;
    }

    if (fw_ptr->send_busy == connector_true)
    {
        /* callback is already called for this message.
         * We're here because we were unable to send a response
         * message which already message is constructed in
         * fw_ptr->response_buffer.
         */
        result = send_fw_message(fw_ptr);
        goto done;
    }

    length = message_load_be16(edp_header, length);
    if (length < FW_MESSAGE_HEADER_SIZE)
    {
        connector_debug_line("fw_process: invalid packet size %d", length);
        goto done;
    }

    fw_message = GET_PACKET_DATA_POINTER(edp_header, PACKET_EDP_FACILITY_SIZE);
    opcode = message_load_u8(fw_message, opcode);
    target = message_load_u8(fw_message, target);

    if (target >= fw_ptr->target_count)
    {
        fw_abort_status_t  fw_status;

        connector_debug_line("fw_process: invalid target");

        fw_status.error_status = fw_invalid_target;
        result = send_fw_abort(fw_ptr, target, fw_error_opcode, fw_status);
        goto done;
    }

    switch(opcode)
    {
    case fw_info_request_opcode:
        result = process_fw_info_request(fw_ptr, fw_message, length);
        break;
    case fw_download_request_opcode:
        result = process_fw_download_request(fw_ptr, fw_message, length);
        break;
    case fw_binary_block_opcode:
        result = process_fw_binary_block(fw_ptr, fw_message, length);
        break;
    case fw_download_abort_opcode:
        result = process_fw_abort(fw_ptr, fw_message, length);
        fw_ptr->last_fw_keepalive_sent_time = 0;
        fw_ptr->fw_keepalive_start = connector_false;
        break;
    case fw_download_complete_opcode:
        if (fw_ptr->last_fw_keepalive_sent_time == 0)
        {
            /* start firmware keepalive which allow firmware download complete
             * callback to start flash (which requires device to
             * send target list message).
             *
             * Note. We only start firmware keepalive when we receive this complete
             * code. Can we start when we receive block opcode?
             */
            fw_ptr->last_fw_keepalive_sent_time = connector_ptr->edp_data.keepalive.last_tx_received_time;
        }
        result = process_fw_complete(fw_ptr, fw_message, length);
        break;
    case fw_target_reset_opcode:
        result = process_target_reset(fw_ptr, fw_message, length);
        fw_ptr->last_fw_keepalive_sent_time = 0;
        fw_ptr->fw_keepalive_start = connector_false;
        break;
    default:
    {
        fw_abort_status_t  fw_status;
        fw_status.error_status = fw_invalid_opcode;
        result = send_fw_abort(fw_ptr, target, fw_error_opcode, fw_status);
        break;
    }
    }

done:
    return result;
}

STATIC connector_status_t connector_facility_firmware_delete(connector_data_t * const connector_ptr)
{
    return del_facility_data(connector_ptr, E_MSG_FAC_FW_NUM);
}

STATIC connector_status_t connector_facility_firmware_init(connector_data_t * const connector_ptr, unsigned int const facility_index)
{
    connector_status_t result;
    connector_firmware_data_t * fw_ptr;

    /* Add firmware access facility to Connector
     *
     * Make sure firmware access facility is not already created. If firmware
     * access facility is already created, we probably reconnect to Device Cloud
     * so just need to reset to initial state.
     *
     */
    fw_ptr = get_facility_data(connector_ptr, E_MSG_FAC_FW_NUM);
    if (fw_ptr == NULL)
    {
        void * ptr;
        result = add_facility_data(connector_ptr, facility_index, E_MSG_FAC_FW_NUM, &ptr, sizeof *fw_ptr);

        if (result != connector_working || ptr == NULL)
        {
            goto done;
        }
        fw_ptr = ptr;
   }
    fw_ptr->target_count = 0;
    fw_ptr->target_info.target_number = 0;
    fw_ptr->desc_length = 0;
    fw_ptr->spec_length = 0;
    fw_ptr->last_fw_keepalive_sent_time = 0;
    fw_ptr->fw_keepalive_start = connector_false;
    fw_ptr->send_busy = connector_false;
    fw_ptr->update_started = connector_false;
    fw_ptr->connector_ptr = connector_ptr;

    {
        connector_firmware_count_t firmware_data;

        result = get_fw_config(fw_ptr, connector_request_id_firmware_target_count, &firmware_data);
        if (result == connector_working)
        {
            /* coverity[uninit_use] */
            fw_ptr->target_count = firmware_data.count;
            if (fw_ptr->target_count > 0)
            {
                size_t const buffer_size = sizeof connector_ptr->edp_data.send_packet.packet_buffer.buffer;
                size_t const overhead = (PACKET_EDP_FACILITY_SIZE + target_list_header_size);
                size_t const max_targets = (buffer_size - overhead) / target_list_size;

                /* get max count of targets that fit into the response buffer */
                if (fw_ptr->target_count > max_targets)
                {
                    connector_request_id_t request_id;

                    request_id.firmware_request = connector_request_id_firmware_target_count;
                    notify_error_status(connector_ptr->callback, connector_class_id_firmware, request_id, connector_invalid_data_range, connector_ptr->context);
                    result = connector_abort;
                    goto done;
                }
            }
            else
            {
                connector_debug_line("fw_discovery: No target supported");
            }
        }
   }

done:
    return result;
}

