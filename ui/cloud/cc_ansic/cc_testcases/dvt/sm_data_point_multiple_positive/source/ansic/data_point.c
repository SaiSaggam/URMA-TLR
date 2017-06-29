/*
 * Copyright (c) 2012, 2013 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */
#include <time.h>
#include <sys/time.h>
#include "connector_config.h"
#include "connector_api.h"
#include "platform.h"

typedef struct
{
    void * req_ptr;
    size_t requested;
    size_t responsed;
    connector_transport_t transport;
} dvt_dp_t;

static dvt_dp_t dvt_dp_info[] = {
    #if (defined CONNECTOR_TRANSPORT_TCP)
    {NULL, 0, 0, connector_transport_tcp},
    #endif
    #if (defined CONNECTOR_TRANSPORT_UDP)
    {NULL, 0, 0, connector_transport_udp},
    #endif
    #if (defined CONNECTOR_TRANSPORT_SMS)
    {NULL, 0, 0, connector_transport_sms}
    #endif
};

static size_t const dvt_dp_transport_count = ARRAY_SIZE(dvt_dp_info);

static void free_dp_multiple_ptr(connector_request_data_point_t * const req_ptr)
{
    connector_data_stream_t * stream = NULL;

    if (req_ptr == NULL) goto done;

    stream = req_ptr->stream;

    while(stream != NULL)
    {
        connector_data_stream_t * next_stream = stream->next;

        if (stream->stream_id != NULL)
        {
            APP_DEBUG("free_req_ptr: Freeing %s\n", stream->stream_id);
            free(stream->stream_id);
            stream->stream_id = NULL;
        }

        /* Points have been allocated in one singe malloc, so no need to loop */
        if (stream->point != NULL)
        {
            free(stream->point);
            stream->point = NULL;
        }

        free(stream);
        stream = next_stream;
    }

    free(req_ptr);

done:
    return;
}

static char * get_stream_id_name(connector_transport_t const transport, connector_data_point_type_t const type)
{
    size_t const stream_id_max_size = sizeof("_dp_tcp_double_");
    static char * app_transport[] = {"tcp", "udp", "sms"};
    static char * app_type[] = {"int", "long", "float", "double", "string", "binary"};
    char * stream_id_name = malloc(stream_id_max_size);

    ASSERT(transport < ARRAY_SIZE(app_transport));
    ASSERT(type < ARRAY_SIZE(app_type));

    if (stream_id_name == NULL)
    {
        APP_DEBUG("get_stream_id_name: Failed to malloc stream_id_name [%" PRIsize "]\n", stream_id_max_size);
        goto error;
    }

    snprintf(stream_id_name, stream_id_max_size, "dp_%s_%s", app_transport[transport], app_type[type]);

error:
    return stream_id_name;
}

static connector_data_stream_t * get_stream(connector_transport_t transport, connector_data_point_type_t const type)
{
    connector_data_stream_t * stream = malloc(sizeof(connector_data_stream_t));

    if (stream == NULL)
    {
        APP_DEBUG("get_stream: Failed to malloc data_stream\n");
        goto done;
    }

    stream->type = type;
    {
        static char * units[] = {"mph", "lb", NULL, "\'F", ""};

        stream->unit = units[type];
    }

    /* TODO: Add appropriate forward to */
    stream->forward_to = NULL;

    stream->stream_id = get_stream_id_name(transport, type);
    if (stream->stream_id == NULL)
    {
        free(stream);
        stream = NULL;
        goto done;
    }

    stream->next = NULL;
    stream->point = NULL;

    APP_DEBUG("get_stream: %s\n", stream->stream_id);
done:

    return stream;
}

static connector_data_point_t * get_points(connector_data_point_type_t const type)
{
    #define APP_MAX_POINTS  20
    size_t const num_of_points = (rand() % APP_MAX_POINTS) + 1;
    size_t const bytes_needed = (num_of_points * sizeof(connector_data_point_t));
    connector_data_point_t * data_points = malloc(bytes_needed);
    connector_data_point_t * point = data_points;
    size_t test_case = 0;

    if (point == NULL)
    {
        APP_DEBUG("get_points: Failed to malloc data_points [%" PRIsize " bytes]\n", bytes_needed);
        goto error;
    }

    if (!num_of_points)
        APP_DEBUG("get_points: num_of_points=0 !!!!!!!!!!!!!!!!\n");
    else
        APP_DEBUG("get_points: num_of_points=%" PRIsize "\n", num_of_points);

    do
    {
        /* update data */
        switch (type)
        {
            case connector_data_point_type_integer:
                if (test_case == 1)
                {
                    static char value[sizeof(int) * 3];

                    point->data.type = connector_data_type_text;
                    snprintf(value, sizeof value, "%d", rand());
                    point->data.element.text = value;
                }
                else
                {
                    point->data.type = connector_data_type_native;
                    point->data.element.native.int_value = rand();
                }
                break;
            case connector_data_point_type_long:
                #if (defined CONNECTOR_SUPPORTS_64_BIT_INTEGERS)
                if (test_case == 1)
                {
                    point->data.type = connector_data_type_native;
                    point->data.element.native.long_value = lrand48();
                }
                else
                #endif
                {
                    static char value[sizeof(long) * 3];

                    point->data.type = connector_data_type_text;
                    snprintf(value, sizeof value, "%ld", lrand48());
                    point->data.element.text = value;
                }
                break;
            case connector_data_point_type_float:
                #if (defined CONNECTOR_SUPPORTS_FLOATING_POINT)
                if (test_case == 1)
                {
                    point->data.type = connector_data_type_native;
                    point->data.element.native.float_value = (float)drand48();
                }
                else
                #endif
                {
                    static char value[APP_MAX_POINTS][sizeof(float) * 2];

                    point->data.type = connector_data_type_text;
                    snprintf(value[test_case], sizeof value[test_case], "%f", (float)drand48()); /* this also requires floating point, works for DVT */
                    point->data.element.text = value[test_case];
                }
                break;

            case connector_data_point_type_double:
                #if (defined CONNECTOR_SUPPORTS_FLOATING_POINT)
                if (test_case == 1)
                {
                    point->data.type = connector_data_type_native;
                    point->data.element.native.double_value = drand48();
                }
                else
                #endif
                {
                    static char value[APP_MAX_POINTS][sizeof(double) * 2];

                    point->data.type = connector_data_type_text;
                    snprintf(value[test_case], sizeof value[test_case], "%lf", drand48()); /* this also requires floating point, works for DVT */
                    point->data.element.text = value[test_case];
                }
                break;

            case connector_data_point_type_string:
            {
                /* TODO: Make 'long string' longer so it's bigger than MSG_MAX_SEND_PACKET_SIZE 512 (for TCP) and SM_PACKET_SIZE_UDP 1472 (for UDP) currently fails */
                #define AV_STR_PATTERNS 7
                static char * value[AV_STR_PATTERNS] = {
                "long string 300 chars...............................................................................1...................................................................................................2...................................................................................................3...................................................................................................",
                "Hello World!", "c,o,m,m,a", "line\nfeed", "t\ta\tb", "\"quote\"", "s p a c e"};

                point->data.type = connector_data_type_native;
                point->data.element.native.string_value = value[test_case % AV_STR_PATTERNS];
                break;
            }

            default:
                APP_DEBUG("get_advanced_points: Unsupported type [%d]\n", type);
                goto error;
        }

        /* update time */
        {
            size_t const time_case = (rand() % (connector_time_local_iso8601 + 1));
            struct timeval current_time;
            int const status = gettimeofday(&current_time, NULL);

            if (status != 0) goto error;
            switch (time_case)
            {
                case connector_time_local_epoch_fractional:
                    point->time.source = connector_time_local_epoch_fractional;
                    point->time.value.since_epoch_fractional.seconds = (uint32_t)current_time.tv_sec;
                    point->time.value.since_epoch_fractional.milliseconds = current_time.tv_usec/1000;
                    break;

                case connector_time_local_iso8601:
                {
                    #define MAX_ISO_8601_BYTES  24
                    static char value[connector_data_point_type_binary][APP_MAX_POINTS][MAX_ISO_8601_BYTES];
                    struct tm * const tmval = localtime(&current_time.tv_sec);

                    ASSERT(tmval != NULL);
                    snprintf(value[type][test_case], sizeof value[type][test_case], "%04d-%02d-%02dT%02d:%02d:%02d",
                             tmval->tm_year+1900, tmval->tm_mon+1, tmval->tm_mday, tmval->tm_hour, tmval->tm_min, tmval->tm_sec);
                    point->time.value.iso8601_string = value[type][test_case];
                    point->time.source = connector_time_local_iso8601;
                    break;
                }

                #if (defined CONNECTOR_SUPPORTS_64_BIT_INTEGERS)
                case connector_time_local_epoch_whole:
                    point->time.source = connector_time_local_epoch_whole;
                    point->time.value.since_epoch_whole.milliseconds = (current_time.tv_sec * UINT64_C(1000)) + (current_time.tv_usec / UINT64_C(1000));
                    break;
                #endif

                default:
                    point->time.source = connector_time_cloud;
                    break;
            }
        }

        /* update location */
        {
            size_t const location_case = (rand() % (connector_location_type_text + 1));

            switch(location_case)
            {
                case connector_location_type_text:
                {
                    static char latitude_str[] = "42.32";
                    static char longitude_str[] = "71.23";
                    static char elevation_str[] = "63.95";

                    point->location.type = connector_location_type_text;
                    point->location.value.text.latitude = latitude_str;
                    point->location.value.text.longitude = longitude_str;
                    point->location.value.text.elevation = elevation_str;
                    break;
                }

                #if (defined CONNECTOR_SUPPORTS_FLOATING_POINT)
                case connector_location_type_native:
                    point->location.type = connector_location_type_native;
                    point->location.value.native.latitude = 42.22;
                    point->location.value.native.longitude = 71.14;
                    point->location.value.native.elevation = 60.67;
                    break;
                #endif

                default:
                    point->location.type = connector_location_type_ignore;
                    break;
            }
        }

        if (!(rand() % 3))
        {
            static char dp_description[] = "CC DVT data point";

            point->description = dp_description;
        }
        else
            point->description = NULL;

        {
            int const quality_value = rand();

            if ((quality_value % 3) == 0)
            {
                point->quality.type = connector_quality_type_native;
                point->quality.value = quality_value;
            }
            else
                point->quality.type = connector_quality_type_ignore;
        }

        test_case++;
        if (test_case < num_of_points)
        {
            point->next = point + 1;
            point++;
        }
        else
        {
            point->next = NULL;
            goto done;
        }

    } while (test_case < num_of_points);

error:
    if (data_points != NULL)
    {
        free(data_points);
        data_points = NULL;
    }

done:
    return data_points;
}

void queue_data_stream(connector_request_data_point_t * req_ptr, connector_data_stream_t * ds_ptr_to_queue)
{
    if (req_ptr->stream == NULL)
        req_ptr->stream = ds_ptr_to_queue;
    else
    {
        connector_data_stream_t * ds_ptr = req_ptr->stream;

        while (ds_ptr->next != NULL)
        {
            ds_ptr = ds_ptr->next;
        }

        ds_ptr->next = ds_ptr_to_queue;
    }
}

void queue_data_point(connector_data_stream_t * ds_ptr, connector_data_point_t * dp_ptr_to_queue)
{
    if (ds_ptr->point == NULL)
        ds_ptr->point = dp_ptr_to_queue;
    else
    {
        connector_data_point_t * dp_ptr = ds_ptr->point;

        while (dp_ptr->next != NULL)
        {
            dp_ptr = dp_ptr->next;
        }

        dp_ptr->next = dp_ptr_to_queue;
    }
}

connector_status_t app_allocate_data(void)
{
    connector_status_t status = connector_idle;
    size_t transport = 0;
    dvt_dp_t * dvt_ptr = NULL;
	connector_data_stream_t * ds_ptr;
	connector_data_point_t * dp_ptr;

    do
    {
        dvt_ptr = &dvt_dp_info[transport];
        connector_request_data_point_t * req_ptr = malloc(sizeof(connector_request_data_point_t));

        if (req_ptr == NULL)
        {
            APP_DEBUG("app_send_data_point: Failed to malloc req_ptr\n");
            status = connector_init_error;
            goto done;
        }

        req_ptr->transport = dvt_ptr->transport;

        if (req_ptr->transport != connector_transport_tcp)
        {
            static connector_bool_t response_needed = connector_true;

            req_ptr->response_required = response_needed;
            if (req_ptr->response_required)
                req_ptr->timeout_in_seconds = CONNECTOR_SM_TX_TIMEOUT;
            response_needed = !response_needed;
        }

        req_ptr->stream = NULL;

        req_ptr->user_context = req_ptr;
        req_ptr->request_id = NULL;

        dvt_ptr->req_ptr = req_ptr;

        for(connector_data_point_type_t type=connector_data_point_type_integer ; type<connector_data_point_type_binary ; type++)
        {
            /* Get a data stream of this type and link it to the multiple request */
            ds_ptr = get_stream(transport, type);
            queue_data_stream(req_ptr, ds_ptr);

            /* Get some random number of data points of this type and link all to the stream */
            dp_ptr = get_points(type);
            queue_data_point(ds_ptr, dp_ptr);
        }

        dvt_ptr = &dvt_dp_info[transport];

        transport++;
    } while (transport < dvt_dp_transport_count);

done:

    return status;
}


connector_status_t app_send_data(connector_handle_t const handle)
{
    connector_status_t status = connector_idle;
    static size_t current_transport = 0;
    dvt_dp_t * dvt_ptr = &dvt_dp_info[current_transport];

    if (current_transport >= dvt_dp_transport_count)
    {
        status = connector_success;
        goto done;
    }

    status = connector_initiate_action(handle, connector_initiate_data_point, dvt_ptr->req_ptr);
    APP_DEBUG("Data point sent, transport[%d], status[%d]\n", dvt_ptr->transport, status);
    switch (status)
    {
        case connector_success:
            dvt_ptr->requested++;
            current_transport++;
            APP_DEBUG("dvt_ptr->requested[%d]=%" PRIsize "\n", dvt_ptr->transport, dvt_ptr->requested);
            dvt_ptr->req_ptr = NULL; /* allow next data point message */
            break;

        case connector_service_busy:
        case connector_init_error:
        case connector_unavailable:
            break;  /* try to send again later */

        default:
            if (dvt_ptr->req_ptr != NULL)
            {
                free_dp_multiple_ptr(dvt_ptr->req_ptr);

                dvt_ptr->req_ptr = NULL;
            }
            break;
    }

done:

    return status;
}

connector_callback_status_t app_data_point_handler(connector_request_id_data_point_t const request_id, void * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    dvt_dp_t * dvt_ptr = NULL;

    switch (request_id)
    {
        case connector_request_id_data_point_response:
        {
            connector_data_point_response_t * const resp_ptr = data;
            connector_request_data_point_t * const req_ptr = resp_ptr->user_context;

            if (req_ptr == NULL)
            {
                APP_DEBUG("Error: Received null multiple dp response context\n");
                status = connector_callback_error;
                goto error;
            }

            dvt_ptr = &dvt_dp_info[req_ptr->transport];
            APP_DEBUG("Received dp multiple response [%d]: %s\n", resp_ptr->response, (resp_ptr->hint != NULL) ? resp_ptr->hint : "null");
            break;
        }

        case connector_request_id_data_point_status:
        {
            connector_data_point_status_t * const status_ptr = data;
            connector_request_data_point_t * const req_ptr = status_ptr->user_context;

            if (req_ptr == NULL)
            {
                APP_DEBUG("Error: Received null multiple dp status context\n");
                status = connector_callback_error;
                goto error;
            }

            dvt_ptr = &dvt_dp_info[req_ptr->transport];
            APP_DEBUG("Received dp multiple status %d\n", status_ptr->status);
            free_dp_multiple_ptr(req_ptr);
            if (dvt_ptr != NULL)
            {
                dvt_ptr->responsed++;
                APP_DEBUG("dvt_ptr->responsed[%d]=%" PRIsize "\n", dvt_ptr->transport, dvt_ptr->responsed);
            }
            break;
        }

        default:
            APP_DEBUG("data_point DVT callback: Request not supported: %d\n", request_id);
            status = connector_callback_unrecognized;
            break;
    }

error:
    return status;
}

connector_bool_t app_dp_test_running(void)
{
    connector_bool_t test_running = connector_true;
    size_t count;

    for (count = 0; count < dvt_dp_transport_count; count++)
    {
        dvt_dp_t * dvt_ptr = &dvt_dp_info[count];

        if (!dvt_ptr->requested)
            goto done;

        if (dvt_ptr->responsed < dvt_ptr->requested)
            goto done;
    }

    APP_DEBUG("Data point test complete\n");
    test_running = connector_false;

done:
    return test_running;
}

#if defined CONNECTOR_TRANSPORT_UDP
connector_callback_status_t app_sm_handler(connector_request_id_sm_t const request, void * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request)
    {
        case connector_request_id_sm_ping_request:
        {
            connector_sm_receive_ping_request_t * const ping_request = data;

            APP_DEBUG("Received ping request. response %s needed\n", ping_request->response_required ? "is" : "is not");
            break;
        }

        case connector_request_id_sm_ping_response:
        {
            connector_sm_ping_response_t * const ping_resp = data;

            APP_DEBUG("Received ping response [%d].\n", ping_resp->status);
            break;
        }

        case connector_request_id_sm_opaque_response:
        {
            connector_sm_opaque_response_t * const response = data;

            APP_DEBUG("Received %" PRIsize " opaque bytes on id %d\n", response->bytes_used, response->id);
            break;
        }

        case connector_request_id_sm_more_data:
        {
            connector_sm_more_data_t * const more_data = data;

            APP_DEBUG("More SM data is waiting on %s in Etherios Device Cloud\n", (more_data->transport == connector_transport_udp) ? "UDP" : "SMS");
            break;
        }

        case connector_request_id_sm_request_connect:
        {
            connector_sm_request_connect_t * const request_connect = data;
            request_connect->allow = connector_true;
            APP_DEBUG("connector_request_id_sm_request_connect called %s...\n", request_connect->allow ? "ALLOWING" : "REJECTING");

            status = connector_callback_continue;
            break;
        }

        default:
            APP_DEBUG("Request not supported in this sample: %d\n", request);
            break;
    }

    return status;
}
#endif

static connector_callback_status_t app_tcp_status(connector_status_tcp_event_t const * const tcp_event)
{
    /* We don't want to see first missed and restored keepalive debug printf.
     * Keepalive sometimes missed and restored almost at the same time.
     */
    static size_t keepalive_missed_count = 0;

    switch (tcp_event->status)
    {
    case connector_tcp_communication_started:
        keepalive_missed_count = 0;
        APP_DEBUG("connector_tcp_communication_started\n");
        break;
    case connector_tcp_keepalive_missed:
        if (keepalive_missed_count > 0)
            APP_DEBUG("connector_tcp_keepalive_missed\n");
        keepalive_missed_count++;
        break;
    case connector_tcp_keepalive_restored:
        keepalive_missed_count--;
        if (keepalive_missed_count > 0)
            APP_DEBUG("connector_tcp_keepalive_restored\n");
        break;
    }

    return connector_callback_continue;
}

connector_callback_status_t app_status_handler(connector_request_id_status_t const request, void * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request)
    {
        case connector_request_id_status_tcp:
            status = app_tcp_status(data);
            break;

        case connector_request_id_status_stop_completed:
            APP_DEBUG("connector_restore_keepalive\n");
            break;

        default:
            APP_DEBUG("Status request not supported in sm_udp: %d\n", request);
            break;
    }

    return status;
}

