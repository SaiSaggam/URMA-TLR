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
    void * dp_ptr;
    size_t request_count;
    size_t response_count;
    connector_data_point_type_t data_type;
    connector_transport_t transport;
} dvt_dp_t;

static dvt_dp_t dvt_dp_info[] = {
    #if (defined CONNECTOR_TRANSPORT_TCP)
    {NULL, 0, 0, connector_data_point_type_integer, connector_transport_tcp},
    #endif
    #if (defined CONNECTOR_TRANSPORT_UDP)
    {NULL, 0, 0, connector_data_point_type_integer, connector_transport_udp},
    #endif
    #if (defined CONNECTOR_TRANSPORT_SMS)
    {NULL, 0, 0, connector_data_point_type_integer, connector_transport_sms}
    #endif
};

static size_t const dvt_dp_transport_count = ARRAY_SIZE(dvt_dp_info);

static void free_dp_binary_ptr(connector_request_data_point_binary_t * const dp_ptr)
{
    if (dp_ptr == NULL) goto done;

    if (dp_ptr->path != NULL)
    {
        APP_DEBUG("free_dp_ptr: Freeing %s\n", dp_ptr->path);
        free(dp_ptr->path);
        dp_ptr->path = NULL;
    }

    if (dp_ptr->point != NULL)
    {
        free(dp_ptr->point);
        dp_ptr->point = NULL;
    }

    free(dp_ptr);

done:
    return;
}

static void free_dp_ptr(connector_request_data_point_t * const dp_ptr)
{
    if (dp_ptr == NULL) goto done;

    if (dp_ptr->stream != NULL)
    {
        if (dp_ptr->stream->stream_id != NULL)
        {
            APP_DEBUG("free_dp_ptr: Freeing %s\n", dp_ptr->stream->stream_id);
            free(dp_ptr->stream->stream_id);
            dp_ptr->stream->stream_id = NULL;
        }

        if (dp_ptr->stream->point != NULL)
        {
            free(dp_ptr->stream->point);
            dp_ptr->stream->point = NULL;
        }

        free(dp_ptr->stream);
        dp_ptr->stream = NULL;
    }

    free(dp_ptr);

done:
    return;
}

static connector_data_point_t * get_stream_points(connector_data_point_type_t const type)
{
    #define APP_MAX_POINTS  40
    size_t const num_of_points = (rand() % APP_MAX_POINTS) + 1;
    size_t const bytes_needed = (num_of_points * sizeof(connector_data_point_t));
    connector_data_point_t * data_points = malloc(bytes_needed);
    connector_data_point_t * point = data_points;
    size_t test_case = 0;

    if (point == NULL)
    {
        APP_DEBUG("get_stream_points: Failed to malloc data_points [%" PRIsize " bytes]\n", bytes_needed);
        goto error;
    }

    if (!num_of_points)
        APP_DEBUG("get_stream_points: num_of_points=0 !!!!!!!!!!!!!!!!\n");

    do
    {
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
                /* TODO: Make 'long string' longer so it's bigger than MSG_MAX_SEND_PACKET_SIZE 512 (for TCP) and SM_PACKET_SIZE_UDP 1472 (for UDP) 
				   currently fails 
                */
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

static char * get_stream_name(connector_transport_t const transport, connector_data_point_type_t const type)
{
    size_t const path_max_size = sizeof("_dp_tcp_double_");
    static char * app_transport[] = {"tcp", "udp", "sms"};
    static char * app_type[] = {"int", "long", "float", "double", "string", "binary"};
    char * path_name = malloc(path_max_size);

    ASSERT(transport < ARRAY_SIZE(app_transport));
    ASSERT(type < ARRAY_SIZE(app_type));

    if (path_name == NULL)
    {
        APP_DEBUG("get_stream_name: Failed to malloc path_name [%" PRIsize "]\n", path_max_size);
        goto error;
    }

    snprintf(path_name, path_max_size, "dp_%s_%s", app_transport[transport], app_type[type]);

error:
    return path_name;
}

static void * get_data_point(dvt_dp_t * dvt_ptr)
{
    connector_request_data_point_t * dp_ptr = malloc(sizeof *dp_ptr);

    if (dp_ptr == NULL)
    {
        APP_DEBUG("app_send_data_point: Failed to malloc dp_ptr\n");
        goto error;
    }

    dp_ptr->stream = malloc(1 * sizeof *dp_ptr->stream); /* One single stream per request */

    if (dp_ptr->stream == NULL)
    {
        APP_DEBUG("app_send_data_point: Failed to malloc dp_ptr->stream\n");
        goto error;
    }

    dp_ptr->stream->next = NULL;
    dp_ptr->stream->stream_id = NULL;
    dp_ptr->transport = dvt_ptr->transport;
    dp_ptr->stream->type = dvt_ptr->data_type;
    dp_ptr->stream->point = get_stream_points(dp_ptr->stream->type);
    if (dp_ptr->stream->point == NULL) goto error;

    {
        static char * units[] = {"mph", "lb", NULL, "\'F", ""};

        dp_ptr->stream->unit = units[dp_ptr->stream->type];
    }

    /* TODO: Add appropriate forward to */
    dp_ptr->stream->forward_to = NULL;

    dp_ptr->stream->stream_id = get_stream_name(dp_ptr->transport, dvt_ptr->data_type);
    if (dp_ptr->stream->stream_id == NULL) goto error;

    if (dp_ptr->transport != connector_transport_tcp)
    {
        static connector_bool_t response_needed = connector_true;

        dp_ptr->response_required = response_needed;
 
        if (dp_ptr->response_required)
            dp_ptr->timeout_in_seconds = CONNECTOR_SM_TX_TIMEOUT;
 
        response_needed = !response_needed;
    }

    dp_ptr->user_context = dp_ptr;
    dp_ptr->request_id = NULL;
    goto done;

error:
    free_dp_ptr(dp_ptr);
    dp_ptr = NULL;

done:
    return dp_ptr;
}

static void * get_binary_point(size_t * bytes)
{
    size_t const rand_bytes = (rand() % 16384) + 1;
    unsigned char * data_ptr = malloc(rand_bytes);

    if (data_ptr != NULL)
    {
        size_t i;

        for (i = 0; i < rand_bytes; i++)
            data_ptr[i] = rand() % UCHAR_MAX;

        *bytes = rand_bytes;
        APP_DEBUG("get_binary_point: rand_bytes %" PRIsize " bytes\n", rand_bytes);
    }
    else
    {
        APP_DEBUG("get_binary_point: Failed to malloc binary point %" PRIsize " bytes\n", rand_bytes);
    }

    return data_ptr;
}

static void * get_data_point_binary(dvt_dp_t * dvt_ptr)
{
    connector_request_data_point_binary_t * dp_ptr = malloc(sizeof(connector_request_data_point_binary_t));

    if (dp_ptr == NULL)
    {
        APP_DEBUG("app_send_data_point: Failed to malloc dp_ptr\n");
        goto error;
    }

    dp_ptr->path = NULL;
    dp_ptr->transport = dvt_ptr->transport;
    dp_ptr->point = get_binary_point(&dp_ptr->bytes_used);
    if (dp_ptr->point == NULL) goto error;

    dp_ptr->path = get_stream_name(dp_ptr->transport, dvt_ptr->data_type);
    if (dp_ptr->path == NULL) goto error;

    if (dp_ptr->transport != connector_transport_tcp)
    {
        static connector_bool_t response_needed = connector_true;

        dp_ptr->response_required = response_needed;
        if (dp_ptr->response_required)
                dp_ptr->timeout_in_seconds = CONNECTOR_SM_TX_TIMEOUT;
        response_needed = !response_needed;
    }

    dp_ptr->user_context = dp_ptr;
    dp_ptr->request_id = NULL;
    goto done;

error:
    free_dp_binary_ptr(dp_ptr);
    dp_ptr = NULL;

done:
    return dp_ptr;
}

connector_status_t app_send_data_point(connector_handle_t const handle)
{
    connector_status_t status = connector_idle;
    static size_t current_transport = 0;
    size_t transport = current_transport;
    dvt_dp_t * dvt_ptr = &dvt_dp_info[transport];

    do
    {
        transport++;
        if (transport == dvt_dp_transport_count) transport = 0;

        if (dvt_ptr->request_count <= connector_data_point_type_binary)
        {
            if (dvt_ptr->dp_ptr == NULL)
                dvt_ptr->dp_ptr = (dvt_ptr->data_type == connector_data_point_type_binary) ? get_data_point_binary(dvt_ptr) : get_data_point(dvt_ptr);

            if (dvt_ptr->dp_ptr != NULL) break;
        }

        dvt_ptr = &dvt_dp_info[transport];

    } while (transport != current_transport);

    current_transport = transport;
    if (dvt_ptr->dp_ptr == NULL) goto done;

    status = connector_initiate_action(handle, (dvt_ptr->data_type == connector_data_point_type_binary)? connector_initiate_data_point_binary : connector_initiate_data_point, dvt_ptr->dp_ptr);
    APP_DEBUG("Data point sent, request[%d], status[%d]\n", dvt_ptr->data_type, status);
    switch (status)
    {
        case connector_success:
            if (dvt_ptr->data_type < connector_data_point_type_binary)
                dvt_ptr->data_type++;
            dvt_ptr->request_count++;
            APP_DEBUG("dvt_ptr->request_count[%d]=%" PRIsize "\n", dvt_ptr->transport, dvt_ptr->request_count);
            dvt_ptr->dp_ptr = NULL; /* allow next data point message */
            break;

        case connector_service_busy:
        case connector_init_error:
        case connector_unavailable:
            break;  /* try to send again later */

        default:
            if (dvt_ptr->dp_ptr != NULL)
            {
                if (dvt_ptr->data_type == connector_data_point_type_binary)
                    free_dp_binary_ptr(dvt_ptr->dp_ptr);
                else
                    free_dp_ptr(dvt_ptr->dp_ptr);

                dvt_ptr->dp_ptr = NULL;
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
        case connector_request_id_data_point_binary_response:
        {
            connector_data_point_response_t * const resp_ptr = data;
            connector_request_data_point_binary_t * const dp_ptr = resp_ptr->user_context;

            if (dp_ptr == NULL)
            {
                APP_DEBUG("Error: Received null binary dp response context\n");
                status = connector_callback_error;
                goto error;
            }

            dvt_ptr = &dvt_dp_info[dp_ptr->transport];
            APP_DEBUG("Received dp binary response %s [%d]: %s\n", dp_ptr->path, resp_ptr->response, (resp_ptr->hint != NULL) ? resp_ptr->hint : "null");
            break;
        }

        case connector_request_id_data_point_binary_status:
        {
            connector_data_point_status_t * const status_ptr = data;
            connector_request_data_point_binary_t * const dp_ptr = status_ptr->user_context;

            if (dp_ptr == NULL)
            {
                APP_DEBUG("Error: Received null binary dp status context\n");
                status = connector_callback_error;
                goto error;
            }

            dvt_ptr = &dvt_dp_info[dp_ptr->transport];
            APP_DEBUG("Received dp binary status %d for %s\n", status_ptr->status, dp_ptr->path);
            free_dp_binary_ptr(dp_ptr);
            if (dvt_ptr != NULL)
            {
                dvt_ptr->response_count++;
                APP_DEBUG("dvt_ptr->response_count[%d]=%" PRIsize "\n", dvt_ptr->transport, dvt_ptr->response_count);
            }
            break;
        }

        case connector_request_id_data_point_response:
        {
            connector_data_point_response_t * const resp_ptr = data;
            connector_request_data_point_t * const dp_ptr = resp_ptr->user_context;

            if (dp_ptr == NULL)
            {
                APP_DEBUG("Error: Received null dp response context\n");
                status = connector_callback_error;
                goto error;
            }

            dvt_ptr = &dvt_dp_info[dp_ptr->transport];
            APP_DEBUG("Received dp response %s [%d]: %s\n", dp_ptr->stream->stream_id, resp_ptr->response, (resp_ptr->hint != NULL) ? resp_ptr->hint : "null");
            break;
        }

        case connector_request_id_data_point_status:
        {
            connector_data_point_status_t * const status_ptr = data;
            connector_request_data_point_t * const dp_ptr = status_ptr->user_context;

            if (dp_ptr == NULL)
            {
                APP_DEBUG("Error: Received null dp status context\n");
                status = connector_callback_error;
                goto error;
            }

            dvt_ptr = &dvt_dp_info[dp_ptr->transport];
            APP_DEBUG("Received dp status %d for %s\n", status_ptr->status, dp_ptr->stream->stream_id);
            free_dp_ptr(dp_ptr);
            if (dvt_ptr != NULL)
            {
                dvt_ptr->response_count++;
                APP_DEBUG("dvt_ptr->response_count[%d]=%" PRIsize "\n", dvt_ptr->transport, dvt_ptr->response_count);
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

        if (dvt_ptr->request_count <= connector_data_point_type_binary)
            goto done;

        if (dvt_ptr->response_count < dvt_ptr->request_count)
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

