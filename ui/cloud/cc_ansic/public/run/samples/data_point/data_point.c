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
#include <time.h>
#include <unistd.h>
#include "connector_api.h"
#include "platform.h"

typedef struct
{
    unsigned long user;
    unsigned long nice;
    unsigned long system;
    unsigned long idle;
    unsigned long iowait;
} stat_cpu_t;

static connector_bool_t app_are_data_points_valid = connector_false;
static connector_bool_t app_dp_waiting_for_response = connector_false;

connector_request_data_point_t * app_allocate_data(size_t const stream_count, size_t const point_count)
{
    connector_request_data_point_t * dp_ptr = malloc(sizeof *dp_ptr);

    if (dp_ptr != NULL)
    {
        dp_ptr->stream = malloc(stream_count * sizeof *dp_ptr->stream);

        if (dp_ptr->stream != NULL)
        {
            connector_data_stream_t * stream = dp_ptr->stream;
            size_t stream_index;

            for (stream_index = 0; stream_index < stream_count; stream_index++)
            {
                stream->point = malloc(point_count * sizeof *dp_ptr->stream->point);

                if (stream->point != NULL)
                {
                    connector_data_point_t * point = stream->point;
                    size_t point_index;

                    for (point_index = 0; point_index < point_count; point_index++)
                    {
                        /* Chain stream to the next one. Last one points to NULL. */
                        if (point_index < point_count - 1)
                            point->next = point + 1;
                        else
                            point->next = NULL;

                        point++;
                    }
                }

                /* Chain stream to the next one. Last one points to NULL. */
                if (stream_index < stream_count - 1)
                    stream->next = stream + 1;
                else
                    stream->next = NULL;

                stream++;
            }

            dp_ptr->user_context = dp_ptr;
            dp_ptr->transport = connector_transport_tcp;
            app_are_data_points_valid = connector_true;
        }
        else
        {
            free(dp_ptr);
            dp_ptr = NULL;
        }
    }

    return dp_ptr;
}

void app_free_data(connector_request_data_point_t * dp_ptr, size_t const stream_count)
{
    if (dp_ptr != NULL)
    {
        if (dp_ptr->stream != NULL)
        {
            size_t stream_index;

            for (stream_index = 0; stream_index < stream_count; stream_index++)
            {
                connector_data_stream_t * const stream = &dp_ptr->stream[stream_index];
                /* All points were allocated in one malloc() call, so only one free() is necessary. */
                free(stream->point);
            }
            /* All streams were allocated in one malloc() call, so only one free() is necessary. */
            free(dp_ptr->stream);
        }
        free(dp_ptr);
    }
    app_are_data_points_valid = connector_false;
}

static connector_bool_t get_cpu_stat(stat_cpu_t * const stat)
{
    connector_bool_t success = connector_false;
    char stat_path[] = "/proc/stat";
    FILE * file_ptr = NULL;

    file_ptr = fopen(stat_path, "r");
    if (file_ptr != NULL)
    {
        if (fscanf(file_ptr, "cpu %lu %lu %lu %lu %lu", &stat->user, &stat->nice, &stat->system, &stat->idle, &stat->iowait) == 5)
            success = connector_true;
    }

    if (file_ptr != NULL)
        fclose(file_ptr);

    return success;
}

static int app_get_cpu_usage(void)
{
    int cpu_usage;
    static stat_cpu_t last_stat;
    stat_cpu_t current_stat;

    if (get_cpu_stat(&current_stat))
    {
        static connector_bool_t first_time = connector_true;

        if (first_time)
        {
            first_time = connector_false;
            cpu_usage = 0;
        }
        else
        {
            int const user_time = (int)(current_stat.user - last_stat.user);
            int const nice_time = (int)(current_stat.nice - last_stat.nice);
            int const system_time = (int)(current_stat.system - last_stat.system);
            int const iowait_time = (int)(current_stat.iowait - last_stat.iowait);
            int const idle_time = (int)(current_stat.idle - last_stat.idle);
            int const active_time = user_time + nice_time + system_time + iowait_time;
            int const total_time = active_time + idle_time;

            cpu_usage = (total_time > 0) ? (active_time * 100) / total_time : 0;
        }

        last_stat = current_stat; /* use memcpy on the platform where this doesn't work */
    }
    else
    {
        static connector_bool_t first_time = connector_true;

        if (first_time)
        {
            APP_DEBUG("Failed to get cpu usage, using random value...\n");
            first_time = connector_false;
        }

        cpu_usage = rand() % 100;
    }

    return cpu_usage;
}

static float app_get_cpu_temperature(void)
{
    float cpu_temperature;

    /* Use a random float value in 0.0 to 99.99 range */
    cpu_temperature = (float)((rand()%10000)/100.0);

    return cpu_temperature;
}


static float app_get_incremental(void)
{
    static int incremental = 0;

    return incremental++;
}

void app_setup_stream(connector_data_stream_t * stream, char * stream_id, char * units, connector_data_point_type_t const type, char * forward_to)
{
    stream->stream_id = stream_id;
    stream->unit = units;
    stream->type = type;
    stream->forward_to = forward_to;
}

void app_update_point(connector_request_data_point_t * const dp_ptr, size_t const stream_index, size_t const point_index)
{
    connector_data_point_t * const point = &dp_ptr->stream[stream_index].point[point_index];

    {
        time_t const current_time = time(NULL);

        point->time.source = connector_time_local_epoch_fractional;
        point->time.value.since_epoch_fractional.seconds = current_time;
        point->time.value.since_epoch_fractional.milliseconds = 0;
    }

    {
        static char latitude_str[] = "42.32";
        static char longitude_str[] = "71.23";
        static char elevation_str[] = "63.95";

        point->location.type = connector_location_type_text;
        point->location.value.text.latitude = latitude_str;
        point->location.value.text.longitude = longitude_str;
        point->location.value.text.elevation = elevation_str;
    }

    point->quality.type = connector_quality_type_ignore;
    point->description = NULL;

    point->data.type = connector_data_type_native;
    switch (stream_index)
    {
        case 0:
            point->data.element.native.int_value = app_get_cpu_usage();
            break;

        case 1:
            point->data.element.native.float_value = app_get_cpu_temperature();
            break;

        case 2:
            point->data.element.native.int_value = app_get_incremental();
            break;

        default:
            ASSERT(0);
            break;
    }
}

connector_status_t app_send_data(connector_handle_t const handle, connector_request_data_point_t * const dp_ptr)
{
    connector_status_t status = connector_success;

    if (app_dp_waiting_for_response)
    {
        APP_DEBUG("Data point app: waiting for a response...\n");
        status = connector_service_busy;
        goto done;
    }

    app_dp_waiting_for_response = connector_true;
    status = connector_initiate_action(handle, connector_initiate_data_point, dp_ptr);
    APP_DEBUG("Data point message sent, status[%d]\n", status);
    if (status != connector_success)
        app_dp_waiting_for_response = connector_false;

done:
    return status;
}

connector_callback_status_t app_data_point_handler(connector_request_id_data_point_t const request_id, void * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    ASSERT(app_are_data_points_valid == connector_true);

    switch (request_id)
    {
        case connector_request_id_data_point_response:
        {
            connector_data_point_response_t * const resp_ptr = data;

            app_dp_waiting_for_response = connector_false;
            APP_DEBUG("Received data point response [%d]\n", resp_ptr->response);
            if (resp_ptr->hint != NULL)
            {
                APP_DEBUG("Hint: %s\n", resp_ptr->hint);
            }
            break;
        }

        case connector_request_id_data_point_status:
        {
            connector_data_point_status_t * const status_ptr = data;

            app_dp_waiting_for_response = connector_false;
            APP_DEBUG("Received data point status [%d]\n", status_ptr->status);
            break;
        }

        default:
            APP_DEBUG("Data point callback: Request not supported: %d\n", request_id);
            status = connector_callback_unrecognized;
            break;
    }

    return status;
}
