#include "connector_app.h"

extern bool os_malloc(size_t const size, void const ** ptr);
extern bool os_realloc(size_t const size, void const * ptr);
extern void os_free(void ** const ptr);

typedef struct led_callback {
    int led;
    char * target;
} led_callback_t;

led_callback_t led_callbacks[] = {
    { RED_LED, "red_led" },
    { YELLOW_LED, "yellow_led" },
    { GREEN_LED, "green_led" }
};

typedef struct device_request_handle {
    void * device_handle;
    char * request_data;
    char * response_data;
    led_callback_t * callback;
    size_t length_in_bytes;
} device_request_handle_t;

#define ARRAY_SIZE(array)  (sizeof array/sizeof array[0])

static connector_callback_status_t process_device_request(
    connector_data_service_msg_request_t const * const request_data,
    connector_data_service_msg_response_t * const response_data){

    connector_callback_status_t status = connector_callback_continue;

    connector_data_service_device_request_t * request = request_data->service_context;
    connector_data_service_block_t * server_data = request_data->server_data;

    device_request_handle_t * client_device_request = response_data->user_context;

    if((server_data->flags & CONNECTOR_MSG_FIRST_DATA) == CONNECTOR_MSG_FIRST_DATA){
        /* target should not be null on 1st chunk of data */
        led_callback_t * callback = NULL;
        int i;
        for(i = 0; i < ARRAY_SIZE(led_callbacks); i++){
            led_callback_t * led = &led_callbacks[i];
            if(strcmp(request->target, led->target) == 0){
                callback = led;
                break;
            }
        }

        if(callback != NULL){
            bool allocated = os_malloc(sizeof *client_device_request, (const void **)(&client_device_request));
            if(!allocated || client_device_request == NULL){
                /* no memory so cancel this request. */
                response_data->message_status = connector_msg_error_memory;
            }
            else{
                client_device_request->length_in_bytes = 0;
                client_device_request->device_handle = request->device_handle;
                client_device_request->callback = callback;
                client_device_request->request_data = NULL;
                client_device_request->response_data = NULL;
                response_data->user_context = client_device_request;
            }

        }
        else{
            /* unsupported target. */
            response_data->message_status = connector_msg_error_cancel;
            return status;
        }
    }

    char * device_request_data = server_data->data;
    if(client_device_request->request_data == NULL){
        bool allocated = os_malloc(server_data->length_in_bytes, (const void **)(&(client_device_request->request_data)));
        if(!allocated || client_device_request->request_data == NULL){
            /* no memory so cancel this request. */
            response_data->message_status = connector_msg_error_memory;
            return status;
        }
        memcpy(client_device_request->request_data, device_request_data, server_data->length_in_bytes);
    }
    else{
        int cur_size = sizeof(&(client_device_request->request_data));
        int new_size = cur_size + server_data->length_in_bytes;
        bool allocated = os_realloc(new_size, client_device_request->request_data);
        if(!allocated || client_device_request->request_data == NULL){
            /* no memory so cancel this request. */
            response_data->message_status = connector_msg_error_memory;
            return status;
        }
        memcpy(client_device_request->request_data + cur_size,
            device_request_data, server_data->length_in_bytes);
    }

    if((server_data->flags & CONNECTOR_MSG_LAST_DATA) == CONNECTOR_MSG_LAST_DATA){
        // If we have request data and a defined callback, processed it.
        if(client_device_request->request_data != NULL && 
                client_device_request->callback != NULL){
            int led = client_device_request->callback->led;
            if(strcmp(client_device_request->request_data, "on") == 0){
                // Turn LED on.
                mPORTDSetBits(led);
                client_device_request->response_data = "LED turned on.";
            }
            else if(strcmp(client_device_request->request_data, "off") == 0){
                // Turn LED off.
                mPORTDClearBits(led);
                client_device_request->response_data = "LED turned off.";
            }
            else if(strcmp(client_device_request->request_data, "toggle") == 0){
                // Toggle led.
                mPORTDToggleBits(led);
                client_device_request->response_data = "LED toggled.";
            }
            else{
                client_device_request->response_data = "Unknown Command.";
            }
            client_device_request->length_in_bytes = strlen(client_device_request->response_data);

        }
        else{
            // Something isn't defined . Cancel request.
            response_data->message_status = connector_msg_error_cancel;
            return status;
        }
    }

    return status;
}

static connector_callback_status_t process_device_response(
    connector_data_service_msg_request_t const * const request_data,
    connector_data_service_msg_response_t * const response_data){

    device_request_handle_t * const client_device_request = response_data->user_context;

    connector_data_service_block_t * const client_data = response_data->client_data;
    /* get number of bytes written to the client data buffer */
    size_t const bytes = (client_device_request->length_in_bytes
        < client_data->length_in_bytes) ?
            client_device_request->length_in_bytes :
            client_data->length_in_bytes;

    /* let's copy the response data to service_response buffer */
    memcpy(client_data->data, client_device_request->response_data, bytes);
    client_device_request->response_data += bytes;
    client_device_request->length_in_bytes -= bytes;

    client_data->length_in_bytes = bytes;
    client_data->flags = (client_device_request->length_in_bytes == 0) ? CONNECTOR_MSG_LAST_DATA : 0;
    if (client_device_request->callback == NULL)
    {
        client_data->flags |= CONNECTOR_MSG_DATA_NOT_PROCESSED;
    }

    response_data->message_status = connector_msg_error_none;

    if (client_device_request->length_in_bytes == 0){
        os_free((void **)&client_device_request);
    }
    return connector_callback_continue;
}


static connector_callback_status_t process_put_request(
    connector_data_service_msg_request_t const * const request_data,
    connector_data_service_msg_response_t * const response_data){

    connector_data_service_block_t * message = response_data->client_data;
    connector_data_service_put_request_t * put_request = request_data->service_context;

    const char * data = put_request->context;
    size_t const len = strlen(data);
    DEBUG_PRINTF("Putting Message: %s.  Length: %d.\n", data, len);
    char * data_ptr = message->data;
    memcpy(data_ptr, data, len);
    os_free((void **)&data);
    message->length_in_bytes = len;
    message->flags = CONNECTOR_MSG_LAST_DATA | CONNECTOR_MSG_FIRST_DATA;
    response_data->message_status = connector_msg_error_none;

    return connector_callback_continue;
}

connector_callback_status_t connector_data_service_callback(
        connector_data_service_request_t request, const void * const request_data,
        void * response_data, size_t * response_length){

    connector_callback_status_t status = connector_callback_continue;
    connector_data_service_msg_request_t const * const data_request = request_data;

    if(request == connector_data_service_device_request){
        switch(data_request->message_type){
            case connector_data_service_type_have_data:
                status = process_device_request(data_request, response_data);
                break;
            case connector_data_service_type_need_data:
                status = process_device_response(data_request, response_data);
                break;
            case connector_data_service_type_error:
                break;
        }
    }
    else if(request == connector_data_service_put_request){
        switch(data_request->message_type){
            case connector_data_service_type_need_data:
                status = process_put_request(data_request, response_data);
            case connector_data_service_type_have_data:
                break;
            case connector_data_service_type_error:
                break;
        }
    }
    return status;
}