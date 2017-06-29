#include "health_metrics_api.h"

connector_callback_status_t dev_health_handle_data_callback(connector_data_service_send_data_t * const data_ptr)
{
    connector_callback_status_t status = connector_callback_error;
    dev_health_data_push_t * const dev_health_data_push = data_ptr->user_context;
    unsigned int bytes_used;

    ASSERT_GOTO(dev_health_data_push != NULL, error);

    bytes_used = MIN_VALUE(data_ptr->bytes_available, dev_health_data_push->bytes_available);

    memcpy(data_ptr->buffer, dev_health_data_push->p_csv, bytes_used);
    data_ptr->bytes_used = bytes_used;
    data_ptr->more_data = connector_bool(dev_health_data_push->bytes_available);

    dev_health_data_push->p_csv += bytes_used;
    dev_health_data_push->bytes_available -= bytes_used;

    status = connector_callback_continue;

error:
    return status;
}

connector_callback_status_t dev_health_handle_response_callback(connector_data_service_send_response_t * const data_ptr)
{
    connector_callback_status_t const status = connector_callback_continue;

    hm_print_line("dev_health_handle_response_callback, response %d '%s'", data_ptr->response, data_ptr->hint);
    return status;
}

connector_callback_status_t dev_health_handle_status_callback(connector_data_service_status_t * const data_ptr)
{
    connector_callback_status_t const status = connector_callback_continue;
    dev_health_data_push_t * const dev_health_data_push = data_ptr->user_context;
    health_metrics_data_t * const health_metrics_data = dev_health_data_push->health_metrics_data;

    hm_print_line("dev_health_handle_status_callback, status %d", data_ptr->status);
    hm_free_data(dev_health_data_push);
    health_metrics_data->info.csv.status = DEV_HEALTH_CSV_STATUS_SENT;

    return status;
}


#if (defined CONNECTOR_SHORT_MESSAGE)
connector_callback_status_t dev_health_handle_length_callback(connector_data_service_length_t * const data_ptr)
{
    connector_callback_status_t status = connector_callback_abort;
    dev_health_data_push_t * const dev_health = data_ptr->user_context;
    ASSERT_GOTO(dev_health != NULL, error);

    data_ptr->total_bytes = dev_health->bytes_available;
    status = connector_callback_continue;
error:
    return status;
}
#endif
