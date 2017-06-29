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

#include "health_metrics_api.h"
#include "health_metrics_structs.h"
#include "health_metrics_process.h"

static char const dev_health_path[] = "DataPoint/.csv";
static size_t const dev_health_path_strlen = sizeof dev_health_path - 1;

size_t dp_process_string(char const * const string, char * const buffer, size_t const bytes_available, size_t * bytes_used_ptr, connector_bool_t need_quotes, connector_bool_t first_chunk)
{
    size_t bytes_processed = 0;
    size_t i;
    size_t extra_chars = 0;
    size_t const max_strlen = bytes_available - 1;

    ASSERT(string != NULL);

    if (need_quotes && first_chunk)
    {
        if (bytes_processed < max_strlen)
        {
            buffer[bytes_processed] = '\"';
        }
        bytes_processed++;
        extra_chars++;
    }

    for (i = 0; string[i] != '\0'; i++)
    {
        if (string[i] == '\"' || string[i] == '\\')
        {
            if (bytes_processed < max_strlen)
            {
                buffer[bytes_processed] = '\\';
            }
            bytes_processed++;
            extra_chars++;
        }

        if (bytes_processed < max_strlen)
        {
            buffer[bytes_processed] = string[i];
        }
        bytes_processed++;
    }

    if (need_quotes)
    {
        if (bytes_processed < max_strlen)
        {
            buffer[bytes_processed] = '\"';
        }
        bytes_processed++;
        extra_chars++;
    }

    buffer[bytes_processed] = '\0';

    if (bytes_used_ptr != NULL)
    {
        *bytes_used_ptr = bytes_processed - extra_chars;
    }

    return bytes_processed;
}

connector_bool_t string_needs_quotes(char const * const string)
{
    connector_bool_t need_quotes = connector_false;
    size_t index;

    for (index = 0; !need_quotes && string[index] != '\0'; index++)
    {
        switch(string[index])
        {
            case ' ':
            case ',':
            case '\"':
            case '\t':
            case '\n':
            case '\r':
            case '\\':
                need_quotes = connector_true;
                break;
            default:
                break;
        }
    }

    return need_quotes;
}

connector_status_t dev_health_send_metrics(connector_handle_t const connector_handle, health_metrics_data_t * const health_metrics_data)
{
    connector_status_t status = connector_init_error;
    dev_health_info_t * const dev_health_info = &health_metrics_data->info;
    dev_health_data_push_t * dev_health_data_push = NULL;
    connector_request_data_service_send_t * send_request = NULL;
    char const * const next_header = strstr(dev_health_info->csv.next_header + sizeof csv_header - 1, csv_header);
    int error;

    error = hm_malloc_data(sizeof *dev_health_data_push, (void * *)&dev_health_data_push);
    ASSERT_GOTO(error == 0, done);

    dev_health_data_push->p_csv = dev_health_info->csv.next_header;
    dev_health_data_push->bytes_available = next_header != NULL ? (size_t)(next_header - dev_health_info->csv.next_header) : strlen(dev_health_data_push->p_csv);
    dev_health_data_push->health_metrics_data = health_metrics_data;
    send_request = &dev_health_data_push->send_request;

    send_request->user_context = dev_health_data_push;
    send_request->content_type = "";
    send_request->option = connector_data_service_send_option_overwrite;
    send_request->path = dev_health_path;
    send_request->request_id = NULL;
    send_request->timeout_in_seconds = 0;
    send_request->transport = connector_transport_tcp;

    status = connector_initiate_action(connector_handle, connector_initiate_send_data, send_request);
done:
    if (status != connector_success)
    {
        if (dev_health_data_push != NULL)
        {
            hm_free_data(dev_health_data_push);
        }
    }
    else
    {
        dev_health_info->csv.next_header = next_header;
    }
    return status;
}


connector_status_t health_metrics_report_step(health_metrics_config_t const * const health_metrics_config, health_metrics_data_t * const health_metrics_data, connector_handle_t connector_handle)
{
#define SECONDS_IN_A_MINUTE 60
    unsigned long now;
    connector_status_t error;
    dev_health_info_t * const dev_health_info = &health_metrics_data->info;

    error = hm_get_system_time(&now);
    if (error != 0)
    {
        hm_print_line("Error while getting system uptime");
        goto done;
    }

    switch (dev_health_info->csv.status)
    {
        case DEV_HEALTH_CSV_STATUS_PROCESSING:
        {
#if !(defined DEVICE_HEALTH_FIRST_REPORT_AT)
#define DEVICE_HEALTH_FIRST_REPORT_AT    0
#endif
            unsigned long const elapsed_seconds = now - health_metrics_data->last_check;
            connector_bool_t const first_check = health_metrics_data->last_check == 0 ? connector_true : connector_false;
            dev_health_root_t root_group;

            if (elapsed_seconds < SECONDS_IN_A_MINUTE && !first_check)
            {
                goto done;
            }
            health_metrics_data->last_check = now;


            if (health_metrics_data->info.csv.data == NULL)
            {
                dev_health_allocate_csv_data(health_metrics_data);
            }

            for (root_group = dev_health_root_eth; root_group < dev_health_root_COUNT; root_group++)
            {
                static const char * paths[] = {"eth", "mobile", "sys"};
                dev_health_simple_metric_t const * item = NULL;
                unsigned long const reporting_interval = health_metrics_config->report_rate * SECONDS_IN_A_MINUTE;
                unsigned long * const sample_at = &health_metrics_data->simple_metrics.sample_at[root_group];
                unsigned long * const report_at = &health_metrics_data->simple_metrics.report_at;
                unsigned long sampling_interval;

                if (reporting_interval == 0)
                {
                    break;
                }

                switch (root_group)
                {
                    case dev_health_root_eth:
                        item = &health_metrics_config->eth;
                        break;
                    case dev_health_root_mobile:
                        item = &health_metrics_config->mobile;
                        break;
                    case dev_health_root_sys:
                        item = &health_metrics_config->sys;
                        break;
                    case dev_health_root_COUNT:
                        ASSERT_GOTO(root_group < dev_health_root_COUNT, done);
                }

                sampling_interval = item->sample_rate * SECONDS_IN_A_MINUTE;

                if (item->metrics == connector_false || item->sample_rate == 0)
                {
                    continue;
                }

                if (*sample_at == 0)
                {
               		*sample_at = DEVICE_HEALTH_FIRST_REPORT_AT;
                }

                if (*report_at == 0)
                {
                	*report_at = DEVICE_HEALTH_FIRST_REPORT_AT;
                }

                if (now >= *sample_at)
                {
                    dev_health_process_path(health_metrics_data, paths[root_group]);
                    *sample_at = now + sampling_interval;
                }

                if (now >= *report_at)
                {
                    dev_health_info->csv.status = DEV_HEALTH_CSV_STATUS_READY_TO_SEND;
                    *report_at = now + reporting_interval;
                }
            }

            if (dev_health_info->csv.status == DEV_HEALTH_CSV_STATUS_READY_TO_SEND)
            {
                dev_health_info->csv.next_header = dev_health_info->csv.data;
            }
            break;
        }
        case DEV_HEALTH_CSV_STATUS_READY_TO_SEND:
        {
            connector_status_t const status =  dev_health_send_metrics(connector_handle, health_metrics_data);

            if (status == connector_success)
            {
                dev_health_info->csv.status = DEV_HEALTH_CSV_STATUS_SENDING;
            }
            else
            {
                hm_print_line("Enhanced Services: couldn't send metrics, reason %d, retrying", status);
            }
            break;
        }
        case DEV_HEALTH_CSV_STATUS_SENDING:
        {
            break;
        }
        case DEV_HEALTH_CSV_STATUS_SENT:
        {
            if (dev_health_info->csv.next_header != NULL)
            {
                dev_health_info->csv.status = DEV_HEALTH_CSV_STATUS_READY_TO_SEND;
            }
            else
            {
                dev_health_reset_csv_data(&health_metrics_data->info);
                health_metrics_data->info.csv.status = DEV_HEALTH_CSV_STATUS_PROCESSING;
            }
            break;
        }
    }

done:
    return error;
}
