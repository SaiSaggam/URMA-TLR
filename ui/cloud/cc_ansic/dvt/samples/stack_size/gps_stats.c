/*
 * Copyright (c) 2013 Digi International Inc.,
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
#include "float.h"

#define VALUE_TO_STRING(value)   # value
#define MACRO_TO_STRING(macro)   VALUE_TO_STRING(macro)

#define GPS_STATS_LOCATION_STRING_LENGTH   sizeof (MACRO_TO_STRING(FLT_MAX))

typedef struct {
    float latitude;
    float longitude;
} gps_location_t;

static gps_location_t gps_data = { 45.01049, -93.254674 };

connector_callback_status_t app_gps_stats_group_get(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    char * ptr;
    float value;

    switch (remote_config->element.id)
    {
    case connector_state_gps_stats_latitude:
    {
        static char gps_latitude[GPS_STATS_LOCATION_STRING_LENGTH];
        ptr = gps_latitude;
        value = gps_data.latitude;
        break;
    }
    case connector_state_gps_stats_longitude:
    {
        static char gps_longitude[GPS_STATS_LOCATION_STRING_LENGTH];
        ptr = gps_longitude;
        value = gps_data.longitude;
        break;
    }
    default:
        ASSERT(0);
        goto done;
    }

    snprintf(ptr, GPS_STATS_LOCATION_STRING_LENGTH, "%f", value);
    remote_config->response.element_value->string_value = ptr;

done:
    return status;
}

