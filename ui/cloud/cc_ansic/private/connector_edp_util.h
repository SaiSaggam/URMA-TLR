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

#define edp_set_active_state(connector_ptr, value)  (connector_ptr)->edp_data.state.active = (value)
#define edp_get_active_state(connector_ptr)         (connector_ptr)->edp_data.state.active

#define edp_set_initiate_state(connector_ptr, value)  (connector_ptr)->edp_data.state.initiate = (value)
#define edp_get_initiate_state(connector_ptr)         (connector_ptr)->edp_data.state.initiate

#define edp_get_edp_state(connector_ptr)                (connector_ptr)->edp_data.state.edp.current
#define edp_get_next_edp_state(connector_ptr)           (connector_ptr)->edp_data.state.edp.next
#define edp_set_edp_state(connector_ptr, value)         (connector_ptr)->edp_data.state.edp.current = (value)
#define edp_set_next_edp_state(connector_ptr, value)    (connector_ptr)->edp_data.state.edp.next = (value)

#define edp_get_close_status(connector_ptr)                (connector_ptr)->edp_data.close_status
#define edp_set_close_status(connector_ptr, value)         (connector_ptr)->edp_data.close_status = (value)

#define edp_set_stop_condition(connector_ptr, value)    (connector_ptr)->edp_data.stop.condition = (value)
#define edp_is_stop_immediately(connector_ptr)          connector_bool((connector_ptr)->edp_data.stop.condition == connector_stop_immediately)
#define edp_is_stop_request(connector_ptr)              connector_bool((connector_ptr)->edp_data.stop.is_set == connector_true)
#define edp_set_stop_request(connector_ptr, context)             (connector_ptr)->edp_data.stop.is_set = connector_true; (connector_ptr)->edp_data.stop.user_context = (context)
#define edp_clear_stop_request(connector_ptr)           (connector_ptr)->edp_data.stop.is_set = connector_false

STATIC void edp_reset_initial_data(connector_data_t * const connector_ptr)
{
    edp_set_edp_state(connector_ptr, edp_communication_connect_to_cloud);
    edp_set_next_edp_state(connector_ptr, edp_communication_connect_to_cloud);

    connector_ptr->edp_data.facilities.current = NULL;
    connector_ptr->edp_data.keepalive.last_rx_sent_time = 0;
    connector_ptr->edp_data.keepalive.last_tx_received_time = 0;
    connector_ptr->edp_data.keepalive.miss_tx_count = 0;

    connector_ptr->edp_data.send_packet.total_length = 0;
    connector_ptr->edp_data.send_packet.bytes_sent = 0;
    connector_ptr->edp_data.send_packet.ptr = NULL;
    connector_ptr->edp_data.send_packet.complete_cb = NULL;

    connector_ptr->edp_data.receive_packet.total_length = 0;
    connector_ptr->edp_data.receive_packet.bytes_received = 0;
    connector_ptr->edp_data.receive_packet.index = 0;
    connector_ptr->edp_data.receive_packet.ptr = NULL;
    connector_ptr->edp_data.receive_packet.data_packet = NULL;
    connector_ptr->edp_data.receive_packet.timeout = MAX_RECEIVE_TIMEOUT_IN_SECONDS;
    connector_ptr->edp_data.close_status = (connector_close_status_t)0;

    edp_set_stop_condition(connector_ptr, connector_stop_immediately);

}

STATIC connector_bool_t is_valid_timing_limit(connector_data_t * const connector_ptr, unsigned long const start, unsigned long const limit)
{
    unsigned long elapsed = start;
    connector_bool_t rc = connector_false;

    if (get_system_time(connector_ptr, &elapsed) == connector_working)
    {
        elapsed -= start;
        rc = (elapsed < limit) ? connector_true : connector_false;
    }

    return rc;
}

STATIC void * get_facility_data(connector_data_t * const connector_ptr, uint16_t const facility_num)
{
    connector_facility_t * fac_ptr;
    void * ptr = NULL;

    /* search and return the facility data which allocated from add_facility_data() */
    for (fac_ptr = connector_ptr->edp_data.facilities.list; fac_ptr != NULL; fac_ptr = fac_ptr->next)
    {
        if (fac_ptr->facility_num == facility_num)
        {
            ptr = fac_ptr->facility_data;
            break;
        }
    }

    return ptr;
}

STATIC connector_static_buffer_id_t get_facility_buffer_id(uint16_t const facility_num)
{
    connector_static_buffer_id_t id;

    switch(facility_num)
    {
        case E_MSG_FAC_CC_NUM:
            id = named_buffer_id(cc_facility);
            break;
        case E_MSG_FAC_FW_NUM:
            id = named_buffer_id(fw_facility);
            break;
        case E_MSG_FAC_MSG_NUM:
            id = named_buffer_id(msg_facility);
            break;
        default:
            id = named_buffer_id(none);
    }
    return id;
}

STATIC connector_status_t add_facility_data(connector_data_t * const connector_ptr, unsigned int const service_index,
                                                 uint16_t const facility_num, void ** fac_ptr, size_t const size)
{
    connector_status_t status;
    connector_facility_t * facility;
    void * ptr;
    size_t const facility_size = sizeof *facility;

    /* allocate facility data and buffer*/

    *fac_ptr = NULL;
    status = malloc_data_buffer(connector_ptr, size + facility_size, get_facility_buffer_id(facility_num), &ptr);
    if (status == connector_working)
    {
        /* add facility to the facility list */
        facility = ptr;
        facility->facility_num = facility_num;
        facility->size = size;
        facility->service_index = service_index;

        add_node(&connector_ptr->edp_data.facilities.list, facility);

        /* setup facility data which is at the end of connector_facility_t */
        facility->facility_data = facility + 1;
         *fac_ptr = facility->facility_data;
    }

    return status;
}

STATIC connector_status_t del_facility_data(connector_data_t * const connector_ptr, uint16_t const facility_num)
{
    connector_status_t status = connector_working;
    connector_facility_t * fac_ptr;

    /* find and free the facility */
    for (fac_ptr = connector_ptr->edp_data.facilities.list; fac_ptr != NULL; fac_ptr = fac_ptr->next)
    {
        if (fac_ptr->facility_num == facility_num)
        {

            remove_node(&connector_ptr->edp_data.facilities.list, fac_ptr);
            /* Ignore the return status. Just continue freeing next facility */
            free_data_buffer(connector_ptr, get_facility_buffer_id(facility_num), fac_ptr);
            break;
        }
    }
    return status;
}

STATIC connector_status_t notify_status(connector_callback_t const callback, connector_tcp_status_t const status, void * const context)
{
    connector_status_t result;
    connector_request_id_t request_id;
    connector_status_tcp_event_t tcp_event;

    tcp_event.status = status;

    request_id.status_request = connector_request_id_status_tcp;

    {
        connector_callback_status_t const callback_status =  connector_callback(callback, connector_class_id_status, request_id, &tcp_event, context);

        switch (callback_status)
        {
            case connector_callback_continue:
            case connector_callback_unrecognized:
                result = connector_working;
                break;
            default:
                result = connector_abort;
                break;
        }
    }

    return result;
}

STATIC void edp_get_device_cloud(connector_data_t * const connector_ptr)
{
    memcpy(connector_ptr->edp_data.config.cloud_url, connector_ptr->device_cloud_url, connector_ptr->device_cloud_url_length);
    connector_ptr->edp_data.config.cloud_url[connector_ptr->device_cloud_url_length] = 0x0;
    connector_ptr->edp_data.config.cloud_url_length = connector_ptr->device_cloud_url_length;
}

