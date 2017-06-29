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
#include <stdlib.h>
#include "connector_config.h"
#include "connector_api.h"
#include "platform.h"
#include "remote_config_cb.h"
#include "health_metrics_api.h"

health_metrics_config_t health_metrics_config;

connector_callback_status_t app_health_metrics_group_init(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    health_metrics_config_t * health_metrics_config_ptr;
    void * const ptr = malloc(sizeof *health_metrics_config_ptr);

    if (ptr == NULL)
    {
        remote_config->error_id = connector_global_error_memory_fail;
        goto done;
    }

    health_metrics_config_ptr = ptr;
    *health_metrics_config_ptr = health_metrics_config;  /* load data */
    session_ptr->group_context = health_metrics_config_ptr;

done:
    return status;
}

connector_callback_status_t app_health_metrics_group_get(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    health_metrics_config_t * const health_metrics_config_ptr = session_ptr->group_context;

    switch (remote_config->element.id)
    {
        case connector_setting_simple_enhanced_services_report_rate:
            remote_config->response.element_value->unsigned_integer_value = health_metrics_config_ptr->report_rate;
            break;
        case connector_setting_simple_enhanced_services_eth_metrics:
            remote_config->response.element_value->on_off_value = health_metrics_config_ptr->eth.metrics == connector_true ? connector_on : connector_off;
            break;
        case connector_setting_simple_enhanced_services_eth_sample_rate:
            remote_config->response.element_value->unsigned_integer_value = health_metrics_config_ptr->eth.sample_rate;
            break;
        case connector_setting_simple_enhanced_services_mobile_metrics:
            remote_config->response.element_value->on_off_value = health_metrics_config_ptr->mobile.metrics == connector_true ? connector_on : connector_off;
            break;
        case connector_setting_simple_enhanced_services_mobile_sample_rate:
            remote_config->response.element_value->unsigned_integer_value = health_metrics_config_ptr->mobile.sample_rate;
            break;
        case connector_setting_simple_enhanced_services_sys_metrics:
            remote_config->response.element_value->on_off_value = health_metrics_config_ptr->sys.metrics == connector_true ? connector_on : connector_off;
            break;
        case connector_setting_simple_enhanced_services_sys_sample_rate:
            remote_config->response.element_value->unsigned_integer_value = health_metrics_config_ptr->sys.sample_rate;
            break;
        default:
            ASSERT(0);
            break;
    }

    return status;
}

connector_callback_status_t app_health_metrics_group_set(connector_remote_config_t * const remote_config)
{
    remote_group_session_t * const session_ptr = remote_config->user_context;
    health_metrics_config_t * const health_metrics_config_ptr = session_ptr->group_context;
    connector_callback_status_t status = connector_callback_continue;

    switch (remote_config->element.id)
    {
        case connector_setting_simple_enhanced_services_report_rate:
            health_metrics_config_ptr->report_rate = remote_config->element.value->unsigned_integer_value;
            break;
        case connector_setting_simple_enhanced_services_eth_metrics:
            health_metrics_config_ptr->eth.metrics = remote_config->element.value->on_off_value == connector_on ? connector_true : connector_false;
            break;
        case connector_setting_simple_enhanced_services_eth_sample_rate:
            health_metrics_config_ptr->eth.sample_rate = remote_config->element.value->unsigned_integer_value;
            break;
        case connector_setting_simple_enhanced_services_mobile_metrics:
            health_metrics_config_ptr->mobile.metrics = remote_config->element.value->on_off_value == connector_on ? connector_true : connector_false;
            break;
        case connector_setting_simple_enhanced_services_mobile_sample_rate:
            health_metrics_config_ptr->mobile.sample_rate = remote_config->element.value->unsigned_integer_value;
            break;
        case connector_setting_simple_enhanced_services_sys_metrics:
            health_metrics_config_ptr->sys.metrics = remote_config->element.value->on_off_value == connector_on ? connector_true : connector_false;
            break;
        case connector_setting_simple_enhanced_services_sys_sample_rate:
            health_metrics_config_ptr->sys.sample_rate = remote_config->element.value->unsigned_integer_value;
            break;
        default:
            ASSERT(0);
            break;
    }

    return status;
}

connector_callback_status_t app_health_metrics_group_end(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    health_metrics_config_t * const health_metrics_config_ptr = session_ptr->group_context;

    if (remote_config->action == connector_remote_action_set)
    {
        /* save data */
        health_metrics_config = *health_metrics_config_ptr;
    }

    free(health_metrics_config_ptr);

    return status;
}

void app_health_metrics_group_cancel(connector_remote_config_cancel_t * const remote_config)
{
    UNUSED_ARGUMENT(remote_config);
    return;
}

