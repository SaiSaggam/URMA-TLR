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
#include "connector_config.h"
#include "connector_api.h"
#include "platform.h"
#include "remote_config_cb.h"

#define GPS_STATS_LOCATION_STRING_LENGTH 15

typedef struct {
    char latitude[GPS_STATS_LOCATION_STRING_LENGTH];
    char longitude[GPS_STATS_LOCATION_STRING_LENGTH];
} gps_location_t;

static gps_location_t gps_data = { "44.932017", "-93.461594"};

connector_callback_status_t app_gps_stats_group_get(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (remote_config->element.id)
    {
    case connector_state_gps_stats_latitude:
        remote_config->response.element_value->string_value = gps_data.latitude;
        break;
    case connector_state_gps_stats_longitude:
        remote_config->response.element_value->string_value = gps_data.longitude;
        break;
    default:
        ASSERT(0);
        goto done;
    }

done:
    return status;
}

