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


#define MANDATORY_FACILITY          (connector_request_id_config_t)-1

#define SET_FACILITY_SUPPORT(i) (UINT32_C(0x01) << (i))
#define IS_FACILITY_SUPPORTED(connector_ptr, table_index)    (connector_ptr->edp_data.facilities.supported_mask & SET_FACILITY_SUPPORT(table_index))

typedef connector_status_t (* connector_facility_service_init_cb_t)(struct connector_data * const connector_ptr, unsigned int const facility_index);
typedef connector_status_t (* connector_facility_service_delete_cb_t)(struct connector_data * const connector_ptr);
typedef connector_status_t (* connector_facility_service_process_cb_t )(struct connector_data * const connector_ptr,
                                                                 void * const facility_data,
                                                                 uint8_t * const packet,
                                                                 unsigned int * const receive_timeout);

typedef struct {
    connector_request_id_t request_id;
    connector_facility_service_init_cb_t init_cb;
    connector_facility_service_delete_cb_t delete_cb;
    connector_facility_service_delete_cb_t cleanup_cb;
    connector_facility_service_process_cb_t discovery_cb;
    connector_facility_service_process_cb_t process_cb;
} connector_facility_service_t;

/* Table of all supported facilities.
 *
 * The connector will call the callback to see whether it supports each optional facility.
 * It will call init_cb to initialize the facility and delete_cb to remove the facility.
 * The init_cb must call add_facility_data() to add the facility into the facility list.
 * The delete_cb is called to delete the facility from the facility list when user terminates the connector.
 */
static connector_facility_service_t const connector_supported_service_table[] = {
        /* mandatory facilities */
        {{MANDATORY_FACILITY}, connector_facility_cc_init, connector_facility_cc_delete, connector_facility_cc_cleanup, cc_discovery, cc_process},

        /* list of optional facilities */
#if (defined CONNECTOR_FIRMWARE_SERVICE) || (defined CONNECTOR_RCI_SERVICE)
    #if (defined CONNECTOR_FIRMWARE_SUPPORT) || (defined CONNECTOR_RCI_SERVICE)
        {{MANDATORY_FACILITY}, connector_facility_firmware_init, connector_facility_firmware_delete, NULL, fw_discovery, fw_process},
    #else
        {{connector_request_id_config_firmware_facility}, connector_facility_firmware_init, connector_facility_firmware_delete, NULL, fw_discovery, fw_process},
    #endif
#endif
#if (defined CONNECTOR_DATA_SERVICE)
    #if (defined CONNECTOR_DATA_SERVICE_SUPPORT)
        {{MANDATORY_FACILITY}, connector_facility_data_service_init, connector_facility_data_service_delete, connector_facility_data_service_cleanup, msg_discovery, msg_process},
    #else
        {{connector_request_id_config_data_service}, connector_facility_data_service_init, connector_facility_data_service_delete, connector_facility_data_service_cleanup, msg_discovery, msg_process},
    #endif
#endif
#if (defined CONNECTOR_FILE_SYSTEM)
    #if (defined CONNECTOR_FILE_SYSTEM_SUPPORT)
        {{MANDATORY_FACILITY}, connector_facility_file_system_init, connector_facility_file_system_delete, connector_facility_file_system_cleanup, msg_discovery, msg_process},
    #else
        {{connector_request_id_config_file_system}, connector_facility_file_system_init, connector_facility_file_system_delete, connector_facility_file_system_cleanup, msg_discovery, msg_process},
    #endif
#endif

#if (defined CONNECTOR_RCI_SERVICE)
    #if (defined CONNECTOR_REMOTE_CONFIGURATION_SUPPORT)
        {{MANDATORY_FACILITY}, connector_facility_rci_service_init, connector_facility_rci_service_delete, connector_facility_rci_service_cleanup, msg_discovery, msg_process}
    #else
        {{connector_request_id_config_remote_configuration}, connector_facility_rci_service_init, connector_facility_rci_service_delete, connector_facility_rci_service_cleanup, msg_discovery, msg_process}
    #endif
#endif
};

static size_t const connector_facility_count = ARRAY_SIZE(connector_supported_service_table);


STATIC connector_status_t layer_remove_facilities(connector_data_t * const connector_ptr, connector_supported_facility_cb_index_t cb_index)
{
    connector_status_t result = connector_idle;
    size_t i;

    for (i=0; (i < connector_facility_count) && ((result == connector_idle) || (result == connector_working)); i++)
    {
        if (IS_FACILITY_SUPPORTED(connector_ptr,i))
        {
            switch (cb_index)
            {
            case facility_callback_delete:
                if (connector_supported_service_table[i].delete_cb != NULL)
                {
                    result = connector_supported_service_table[i].delete_cb(connector_ptr);
                }
                break;
            case facility_callback_cleanup:
                if (connector_supported_service_table[i].cleanup_cb != NULL)
                {
                    result = connector_supported_service_table[i].cleanup_cb(connector_ptr);
                }
                break;
            }
            ASSERT(result != connector_pending);
        }
    }
    return result;
}



STATIC connector_status_t layer_discovery_facility(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_idle;
    connector_facility_t * fac_ptr;

    /* invoke any facility that needs to send any message to Device Cloud
     * during initialization phase at discovery layer.
     */
    fac_ptr = (connector_ptr->edp_data.facilities.current == NULL)? connector_ptr->edp_data.facilities.list: connector_ptr->edp_data.facilities.current;

    for (;fac_ptr != NULL && result == connector_idle; fac_ptr = fac_ptr->next)
    {
        unsigned int const i = fac_ptr->service_index;

        if (connector_supported_service_table[i].discovery_cb != NULL)
        {   /* function to send facility discovery */
            result = connector_supported_service_table[i].discovery_cb(connector_ptr, fac_ptr->facility_data,
                                                                   NULL, &connector_ptr->edp_data.receive_packet.timeout);
            connector_ptr->edp_data.facilities.current = (result == connector_pending) ? fac_ptr : fac_ptr->next;
        }
    }

    if (result == connector_working || result == connector_idle)
    {
        if (connector_ptr->edp_data.facilities.current == NULL)
        {
            /* done all facilities */
            result = connector_working;
        }
        else if (result == connector_working)
        {
            /* continue next facility */
            result = connector_idle;
        }
    }
    return result;
}


STATIC connector_status_t edp_layer_get_supported_facilities(connector_data_t * const connector_ptr)
{
#define NUMBER_FACILITY_PER_BYTE CHAR_BIT
    connector_status_t result = connector_working;
    unsigned int i;

    connector_ptr->edp_data.facilities.supported_mask = 0;

    ASSERT(CHAR_BIT == 8);
    ASSERT(connector_facility_count <= (sizeof connector_ptr->edp_data.facilities.supported_mask * NUMBER_FACILITY_PER_BYTE));

    /* connector_supported_service_table[] table includes a list of supported facilities.
     * Call callback to see which facility is supported.
     */

    for (i=0; i < connector_facility_count; i++)
    {
        connector_request_id_t const request_id = connector_supported_service_table[i].request_id;
        connector_config_supported_t config_status;

        config_status.supported = connector_bool(request_id.config_request == MANDATORY_FACILITY);

        if (request_id.config_request != MANDATORY_FACILITY)
        {   /* this is optional facility so ask application whether it supports this facility */
            connector_callback_status_t status;

            status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &config_status, connector_ptr->context);
            switch (status)
            {
            case connector_callback_busy:
                /* not allowed to returned busy */
                connector_debug_line("edp_layer_get_supported_facilities: callback returns connector_callback_busy which is not allowed");
                result = connector_abort;
                goto error;
            case connector_callback_abort:
            case connector_callback_error:
                result = connector_abort;
                goto done;
            case connector_callback_unrecognized:
                config_status.supported = connector_false;
                break;
            case connector_callback_continue:
                break;
            }
        }

        switch (config_status.supported)
        {
        case connector_true:
            connector_ptr->edp_data.facilities.supported_mask |= (uint16_t)SET_FACILITY_SUPPORT(i);
            break;
        case connector_false:
            break;
        default:
            result = connector_invalid_data_range;
            goto error;
        }

    }

error:
    if (result != connector_working)
    {
        connector_request_id_t const request_id = connector_supported_service_table[i].request_id;
        if (notify_error_status(connector_ptr->callback, connector_class_id_config, request_id, result, connector_ptr->context) != connector_working)
            result = connector_abort;
    }

done:
    return result;
}

STATIC connector_status_t edp_layer_initialize_facilities(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_working;
    size_t  i;

    for (i=0; i < connector_facility_count; i++)
    {
        if (IS_FACILITY_SUPPORTED(connector_ptr,i) &&
           connector_supported_service_table[i].init_cb != NULL)
        {
            result = connector_supported_service_table[i].init_cb(connector_ptr, i);
            if ( result != connector_working) goto done;
        }
    }

    {
        connector_facility_t * fac_ptr;
        /* initialize packet pointer for each facility */
        for (fac_ptr = connector_ptr->edp_data.facilities.list; fac_ptr != NULL; fac_ptr = fac_ptr->next)
        {
            fac_ptr->packet_buffer = NULL;
        }
    }

done:
    return result;
}

STATIC connector_status_t layer_facility_process(connector_data_t * const connector_ptr)
{
    connector_status_t result = connector_idle;
    connector_facility_t * fac_ptr;


    /* Invoke facility process.
     *
     * Run all facilities. But each starting facility
     * will be alternated.
     */
    fac_ptr = (connector_ptr->edp_data.facilities.current != NULL) ? connector_ptr->edp_data.facilities.current : connector_ptr->edp_data.facilities.list;
    connector_ptr->edp_data.facilities.current = fac_ptr;
    do
    {
        /* We want to run all facilities processes */
        unsigned int const i = fac_ptr->service_index;

        if (connector_supported_service_table[i].process_cb)
        {
            uint8_t * const packet = (fac_ptr->packet_buffer != NULL) ? fac_ptr->packet_buffer->buffer: NULL;

            result = connector_supported_service_table[i].process_cb(connector_ptr, fac_ptr->facility_data,
                                                                 packet, &connector_ptr->edp_data.receive_packet.timeout);

            if (result != connector_pending && result != connector_active && fac_ptr->packet_buffer != NULL)
            {   /* release the packet when it's done */
                tcp_release_receive_packet(connector_ptr, fac_ptr->packet_buffer);
                fac_ptr->packet_buffer = NULL;
            }
            fac_ptr = (fac_ptr->next != NULL) ? fac_ptr->next : connector_ptr->edp_data.facilities.list;
        }

    } while (result == connector_idle && fac_ptr != connector_ptr->edp_data.facilities.current);

    /* setup next starting facility process */
    connector_ptr->edp_data.facilities.current = (connector_ptr->edp_data.facilities.current != NULL) ? connector_ptr->edp_data.facilities.current->next : connector_ptr->edp_data.facilities.list;

    if (result == connector_abort) edp_set_close_status(connector_ptr, connector_close_status_abort);

    return result;
}
