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

#define JSON_MAX_SIZE   128

typedef connector_bool_t (* dev_health_query_fn_t)(connector_indexes_t const * const indexes, void * const value);
typedef unsigned int (* dev_health_get_instances_fn_t)(unsigned int upper_index);

typedef enum {
    DEV_HEALTH_TYPE_NONE,
    DEV_HEALTH_TYPE_INT32,
    DEV_HEALTH_TYPE_UINT64,
    DEV_HEALTH_TYPE_FLOAT,
    DEV_HEALTH_TYPE_STRING,
    DEV_HEALTH_TYPE_JSON,
    DEV_HEALTH_TYPE_GEOJSON
} dev_health_value_type_t;

typedef struct {
    char const * name;
    size_t const name_len;
    dev_health_value_type_t type;
    dev_health_query_fn_t getter;
} dev_health_item_t;

typedef enum {
    ITEMS,
    SUBGROUPS
} dev_health_path_type_t;

typedef struct dev_health_path_group {
    char const * name;
    size_t const name_len;
    dev_health_get_instances_fn_t multi_instance;

    struct {
        dev_health_path_type_t type;
        union {
            struct {
                dev_health_item_t const * const * array;
                unsigned int size;
            } items;
            struct {
                struct dev_health_path_group const * const * array;
                unsigned int size;
            } subgroups;
        } data;
    } child;
} dev_health_path_group_t;

static unsigned int count_special_chars(char const * const string)
{
    unsigned int i;
    unsigned int escaped_chars = 0;

    for (i = 0; string[i] != '\0'; i++)
    {
        if (string[i] == '"' || string[i] == '\\')
        {
            escaped_chars++;
        }
    }

    return escaped_chars;
}

static void escape_string(char * * const string)
{
    unsigned int escape_chars;

    ASSERT(*string != NULL);
    escape_chars = count_special_chars(*string);

    if (escape_chars)
    {
        unsigned int const new_size = strlen(*string) + escape_chars + 1;
        char * string_escaped = cc_dev_health_malloc_string(new_size);

        ASSERT(string_escaped != NULL);
        dp_process_string(*string, string_escaped, new_size, NULL, connector_false, connector_true);
        cc_dev_health_free_string(*string);
        *string = string_escaped;
    }
}

static connector_bool_t cc_dev_health_get_mobile_module_json(connector_indexes_t const * const indexes, connector_json_t * const value)
{
    connector_bool_t const present = cc_dev_health_mobile_module_present(indexes);

    if (present)
    {
        char * const module_json = cc_dev_health_malloc_string(JSON_MAX_SIZE);
        char * modem_id = NULL;
        char * imei = NULL;
        unsigned int sims;
        int json_size;

        ASSERT(module_json != NULL);
        cc_dev_health_get_mobile_module_modem_id(indexes, &modem_id);
        cc_dev_health_get_mobile_module_imei(indexes, &imei);
        cc_dev_health_get_mobile_module_sims(indexes, &sims);

        ASSERT(modem_id != NULL);
        ASSERT(imei != NULL);

        escape_string(&modem_id);
        escape_string(&imei);

        json_size = sprintf(module_json, "{\"modem_id\":\"%s\",\"imei\":\"%s\",\"sims\":%u}", modem_id, imei, sims);
        UNUSED_VARIABLE(json_size); /* Prevent variable set but not used in non-debug */
        ASSERT(json_size <= JSON_MAX_SIZE);

        cc_dev_health_free_string(modem_id);
        cc_dev_health_free_string(imei);

        *value = module_json;
    }

    return present;
}

static connector_bool_t cc_dev_health_get_mobile_net_status_json(connector_indexes_t const * const indexes, connector_json_t * const value)
{
    connector_bool_t const present = cc_dev_health_mobile_module_net_status_present(indexes);

    if (present)
    {
        char * const net_status_json = cc_dev_health_malloc_string(JSON_MAX_SIZE);
        char * registration = NULL;
        char * attachment = NULL;
        int json_size;

        ASSERT(net_status_json != NULL);
        cc_dev_health_get_mobile_net_status_registration(indexes, &registration);
        cc_dev_health_get_mobile_net_status_attachment(indexes, &attachment);

        ASSERT(registration != NULL);
        ASSERT(attachment != NULL);

        escape_string(&registration);
        escape_string(&attachment);

        json_size = sprintf(net_status_json, "{\"registration\":\"%s\",\"attachment\":\"%s\"}", registration, attachment);
        UNUSED_VARIABLE(json_size); /* Prevent variable set but not used in non-debug */
        ASSERT(json_size <= JSON_MAX_SIZE);

        cc_dev_health_free_string(registration);
        cc_dev_health_free_string(attachment);

        *value = net_status_json;
    }

    return present;
}

static connector_bool_t cc_dev_health_get_mobile_net_info_json(connector_indexes_t const * const indexes, connector_json_t * const value)
{
    connector_bool_t const present = cc_dev_health_get_mobile_net_info_present(indexes);

    if (present)
    {
        char * const net_info_json = cc_dev_health_malloc_string(JSON_MAX_SIZE);
        char * iccid = NULL;
        char * imsi = NULL;
        char * phone_num = NULL;
        int json_size;

        ASSERT(net_info_json != NULL);

        cc_dev_health_get_mobile_net_info_iccid(indexes, &iccid);
        cc_dev_health_get_mobile_net_info_imsi(indexes, &imsi);
        cc_dev_health_get_mobile_net_info_phone_num(indexes, &phone_num);

        ASSERT(iccid != NULL);
        ASSERT(imsi != NULL);
        ASSERT(phone_num != NULL);

        escape_string(&iccid);
        escape_string(&imsi);
        escape_string(&phone_num);

        json_size = sprintf(net_info_json, "{\"iccid\":\"%s\",\"imsi\":\"%s\",\"phone_num\":\"%s\"}", iccid, imsi, phone_num);
        UNUSED_VARIABLE(json_size); /* Prevent variable set but not used in non-debug */
        ASSERT(json_size <= JSON_MAX_SIZE);

        cc_dev_health_free_string(iccid);
        cc_dev_health_free_string(imsi);
        cc_dev_health_free_string(phone_num);

        *value = net_info_json;
    }

    return present;
}

static connector_bool_t cc_dev_health_get_wifi_radio_json(connector_indexes_t const * const indexes, connector_json_t * const value)
{
    connector_bool_t const present = cc_dev_health_get_wifi_radio_present(indexes);

    if (present)
    {
        char * const radio_json = cc_dev_health_malloc_string(JSON_MAX_SIZE);
        char * mode = NULL;
        char * ssid = NULL;
        char * protocol = NULL;
        unsigned int channel = 0;
        int json_size;

        ASSERT(radio_json != NULL);

        cc_dev_health_get_wifi_radio_mode(indexes, &mode);
        cc_dev_health_get_wifi_radio_ssid(indexes, &ssid);
        cc_dev_health_get_wifi_radio_protocol(indexes, &protocol);
        cc_dev_health_get_wifi_radio_channel(indexes, &channel);

        ASSERT(mode != NULL);
        ASSERT(ssid != NULL);
        ASSERT(protocol != NULL);

        escape_string(&mode);
        escape_string(&ssid);
        escape_string(&protocol);

        json_size = sprintf(radio_json, "{\"mode\":\"%s\",\"ssid\":\"%s\",\"channel\":%u,\"protocol\":\"%s\"}", mode, ssid, channel, protocol);
        UNUSED_VARIABLE(json_size); /* Prevent variable set but not used in non-debug */
        ASSERT(json_size <= JSON_MAX_SIZE);

        cc_dev_health_free_string(mode);
        cc_dev_health_free_string(ssid);
        cc_dev_health_free_string(protocol);

        *value = radio_json;
    }

    return present;
}

static const dev_health_item_t dev_health_mobile_module = {"module", sizeof "module" - 1, DEV_HEALTH_TYPE_JSON, (dev_health_query_fn_t)cc_dev_health_get_mobile_module_json};
static dev_health_item_t const * const dev_health_mobile_module_elements[] =
{
    &dev_health_mobile_module
};

static const dev_health_item_t dev_health_mobile_temperature = {"temperature", sizeof "temperature" - 1, DEV_HEALTH_TYPE_FLOAT, (dev_health_query_fn_t)cc_dev_health_get_mobile_temperature};
static dev_health_item_t const * const dev_health_mobile_temperature_elements[] =
{
    &dev_health_mobile_temperature
};

static const dev_health_item_t dev_health_mobile_active_sim = {"active_sim", sizeof "active_sim" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_active_sim};
static dev_health_item_t const * const dev_health_mobile_active_sim_elements[] =
{
    &dev_health_mobile_active_sim
};


static const dev_health_item_t dev_health_mobile_net_cell_id = {"cell_id", sizeof "cell_id" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_cell_id};
static dev_health_item_t const * const dev_health_mobile_net_cell_id_elements[] =
{
    &dev_health_mobile_net_cell_id
};

static dev_health_path_group_t const dev_health_mobile_net_cell_id_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_cell_id_elements,
                ARRAY_SIZE(dev_health_mobile_net_cell_id_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_network = {"network", sizeof "network" - 1, DEV_HEALTH_TYPE_STRING, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_network};
static dev_health_item_t const * const dev_health_mobile_net_network_elements[] =
{
    &dev_health_mobile_net_network
};

static dev_health_path_group_t const dev_health_mobile_net_network_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_network_elements,
                ARRAY_SIZE(dev_health_mobile_net_network_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_rssi = {"rssi", sizeof "rssi" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_rssi};
static dev_health_item_t const * const dev_health_mobile_net_rssi_elements[] =
{
    &dev_health_mobile_net_rssi
};

static dev_health_path_group_t const dev_health_mobile_net_rssi_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_rssi_elements,
                ARRAY_SIZE(dev_health_mobile_net_rssi_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_ecio = {"ecio", sizeof "ecio" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_ecio};
static dev_health_item_t const * const dev_health_mobile_net_ecio_elements[] =
{
    &dev_health_mobile_net_ecio
};

static dev_health_path_group_t const dev_health_mobile_net_ecio_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_ecio_elements,
                ARRAY_SIZE(dev_health_mobile_net_ecio_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_rsrp = {"rsrp", sizeof "rsrp" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_rsrp};
static dev_health_item_t const * const dev_health_mobile_net_rsrp_elements[] =
{
    &dev_health_mobile_net_rsrp
};

static dev_health_path_group_t const dev_health_mobile_net_rsrp_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_rsrp_elements,
                ARRAY_SIZE(dev_health_mobile_net_rsrp_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_rsrq = {"rsrq", sizeof "rsrq" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_rsrq};
static dev_health_item_t const * const dev_health_mobile_net_rsrq_elements[] =
{
    &dev_health_mobile_net_rsrq
};

static dev_health_path_group_t const dev_health_mobile_net_rsrq_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_rsrq_elements,
                ARRAY_SIZE(dev_health_mobile_net_rsrq_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_sinr = {"sinr", sizeof "sinr" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_sinr};
static dev_health_item_t const * const dev_health_mobile_net_sinr_elements[] =
{
    &dev_health_mobile_net_sinr
};

static dev_health_path_group_t const dev_health_mobile_net_sinr_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_sinr_elements,
                ARRAY_SIZE(dev_health_mobile_net_sinr_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_snr = {"snr", sizeof "snr" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_snr};
static dev_health_item_t const * const dev_health_mobile_net_snr_elements[] =
{
    &dev_health_mobile_net_snr
};

static dev_health_path_group_t const dev_health_mobile_net_snr_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_snr_elements,
                ARRAY_SIZE(dev_health_mobile_net_snr_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_status = {"status", sizeof "status" - 1, DEV_HEALTH_TYPE_JSON, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_status_json};
static dev_health_item_t const * const dev_health_mobile_net_status_elements[] =
{
    &dev_health_mobile_net_status
};

static dev_health_path_group_t const dev_health_mobile_net_status_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_status_elements,
                ARRAY_SIZE(dev_health_mobile_net_status_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_lac = {"lac", sizeof "lac" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_lac};
static dev_health_item_t const * const dev_health_mobile_net_lac_elements[] =
{
    &dev_health_mobile_net_lac
};

static dev_health_path_group_t const dev_health_mobile_net_lac_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_lac_elements,
                ARRAY_SIZE(dev_health_mobile_net_lac_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_tac = {"tac", sizeof "tac" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_tac};
static dev_health_item_t const * const dev_health_mobile_net_tac_elements[] =
{
    &dev_health_mobile_net_tac
};

static dev_health_path_group_t const dev_health_mobile_net_tac_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_tac_elements,
                ARRAY_SIZE(dev_health_mobile_net_tac_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_session = {"session", sizeof "session" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_session};
static dev_health_item_t const * const dev_health_mobile_net_session_elements[] =
{
    &dev_health_mobile_net_session
};

static dev_health_path_group_t const dev_health_mobile_net_session_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_session_elements,
                ARRAY_SIZE(dev_health_mobile_net_session_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_info = {"info", sizeof "info" - 1, DEV_HEALTH_TYPE_JSON, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_info_json};
static dev_health_item_t const * const dev_health_mobile_net_info_elements[] =
{
    &dev_health_mobile_net_info
};

static dev_health_path_group_t const dev_health_mobile_net_info_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_info_elements,
                ARRAY_SIZE(dev_health_mobile_net_info_elements)
            }
        }
    }
};

static connector_bool_t cc_dev_health_get_mobile_net_2g_rxbytes(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_rxbytes(indexes, NETWORK_TECH_2G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_2g_txbytes(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_txbytes(indexes, NETWORK_TECH_2G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_2g_latency_min(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_latency_min(indexes, NETWORK_TECH_2G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_2g_latency_avg(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_latency_avg(indexes, NETWORK_TECH_2G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_2g_latency_max(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_latency_max(indexes, NETWORK_TECH_2G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_2g_transactions_count(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_transactions_count(indexes, NETWORK_TECH_2G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_2g_fdrop_count(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_fdrop_count(indexes, NETWORK_TECH_2G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_2g_losspercent(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_losspercent(indexes, NETWORK_TECH_2G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_2g_drop_count(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_drop_count(indexes, NETWORK_TECH_2G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_2g_oos_count(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_oos_count(indexes, NETWORK_TECH_2G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_2g_uptime(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_uptime(indexes, NETWORK_TECH_2G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_3g_rxbytes(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_rxbytes(indexes, NETWORK_TECH_3G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_3g_txbytes(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_txbytes(indexes, NETWORK_TECH_3G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_3g_latency_min(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_latency_min(indexes, NETWORK_TECH_3G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_3g_latency_avg(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_latency_avg(indexes, NETWORK_TECH_3G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_3g_latency_max(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_latency_max(indexes, NETWORK_TECH_3G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_3g_transactions_count(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_transactions_count(indexes, NETWORK_TECH_3G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_3g_fdrop_count(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_fdrop_count(indexes, NETWORK_TECH_3G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_3g_losspercent(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_losspercent(indexes, NETWORK_TECH_3G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_3g_drop_count(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_drop_count(indexes, NETWORK_TECH_3G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_3g_oos_count(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_oos_count(indexes, NETWORK_TECH_3G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_3g_uptime(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_uptime(indexes, NETWORK_TECH_3G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_4g_rxbytes(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_rxbytes(indexes, NETWORK_TECH_4G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_4g_txbytes(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_txbytes(indexes, NETWORK_TECH_4G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_4g_latency_min(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_latency_min(indexes, NETWORK_TECH_4G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_4g_latency_avg(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_latency_avg(indexes, NETWORK_TECH_4G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_4g_latency_max(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_latency_max(indexes, NETWORK_TECH_4G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_4g_transactions_count(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_transactions_count(indexes, NETWORK_TECH_4G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_4g_fdrop_count(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_fdrop_count(indexes, NETWORK_TECH_4G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_4g_losspercent(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_losspercent(indexes, NETWORK_TECH_4G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_4g_drop_count(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_drop_count(indexes, NETWORK_TECH_4G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_4g_oos_count(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_oos_count(indexes, NETWORK_TECH_4G, value);
}

static connector_bool_t cc_dev_health_get_mobile_net_4g_uptime(connector_indexes_t const * const indexes, int32_t * const value)
{
    return cc_dev_health_get_mobile_net_uptime(indexes, NETWORK_TECH_4G, value);
}

static const dev_health_item_t dev_health_mobile_net_2g_rxbytes = {"rxbytes", sizeof "rxbytes" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_2g_rxbytes};
static dev_health_item_t const * const dev_health_mobile_net_2g_rxbytes_elements[] =
{
    &dev_health_mobile_net_2g_rxbytes
};

static dev_health_path_group_t const dev_health_mobile_net_2g_rxbytes_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_2g_rxbytes_elements,
                ARRAY_SIZE(dev_health_mobile_net_2g_rxbytes_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_2g_txbytes = {"txbytes", sizeof "txbytes" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_2g_txbytes};
static dev_health_item_t const * const dev_health_mobile_net_2g_txbytes_elements[] =
{
    &dev_health_mobile_net_2g_txbytes
};

static dev_health_path_group_t const dev_health_mobile_net_2g_txbytes_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_2g_txbytes_elements,
                ARRAY_SIZE(dev_health_mobile_net_2g_txbytes_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_2g_losspercent = {"losspercent", sizeof "losspercent" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_2g_losspercent};
static dev_health_item_t const * const dev_health_mobile_net_2g_losspercent_elements[] =
{
    &dev_health_mobile_net_2g_losspercent
};

static dev_health_path_group_t const dev_health_mobile_net_2g_losspercent_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_2g_losspercent_elements,
                ARRAY_SIZE(dev_health_mobile_net_2g_losspercent_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_2g_uptime = {"uptime", sizeof "uptime" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_2g_uptime};
static dev_health_item_t const * const dev_health_mobile_net_2g_uptime_elements[] =
{
    &dev_health_mobile_net_2g_uptime
};

static dev_health_path_group_t const dev_health_mobile_net_2g_uptime_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_2g_uptime_elements,
                ARRAY_SIZE(dev_health_mobile_net_2g_uptime_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_2g_latency_min = {"min", sizeof "min" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_2g_latency_min};
static const dev_health_item_t dev_health_mobile_net_2g_latency_avg = {"avg", sizeof "avg" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_2g_latency_avg};
static const dev_health_item_t dev_health_mobile_net_2g_latency_max = {"max", sizeof "max" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_2g_latency_max};

static dev_health_item_t const * const dev_health_mobile_net_2g_latency_elements[] =
{
    &dev_health_mobile_net_2g_latency_min,
    &dev_health_mobile_net_2g_latency_avg,
    &dev_health_mobile_net_2g_latency_max
};

static dev_health_path_group_t const dev_health_mobile_net_2g_latency_group =
{
    "latency",
    sizeof "latency" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_2g_latency_elements,
                ARRAY_SIZE(dev_health_mobile_net_2g_latency_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_2g_transactions_count = {"count", sizeof "count" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_2g_transactions_count};
static dev_health_item_t const * const dev_health_mobile_net_2g_transactions_elements[] =
{
    &dev_health_mobile_net_2g_transactions_count
};

static dev_health_path_group_t const dev_health_mobile_net_2g_transactions_group =
{
    "transactions",
    sizeof "transactions" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_2g_transactions_elements,
                ARRAY_SIZE(dev_health_mobile_net_2g_transactions_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_2g_fdrop_count = {"count", sizeof "count" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_2g_fdrop_count};
static dev_health_item_t const * const dev_health_mobile_net_2g_fdrop_elements[] =
{
    &dev_health_mobile_net_2g_fdrop_count
};

static dev_health_path_group_t const dev_health_mobile_net_2g_fdrop_group =
{
    "fdrop",
    sizeof "fdrop" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_2g_fdrop_elements,
                ARRAY_SIZE(dev_health_mobile_net_2g_fdrop_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_2g_drop_count = {"count", sizeof "count" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_2g_drop_count};
static dev_health_item_t const * const dev_health_mobile_net_2g_drop_elements[] =
{
    &dev_health_mobile_net_2g_drop_count
};

static dev_health_path_group_t const dev_health_mobile_net_2g_drop_group =
{
    "drop",
    sizeof "drop" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_2g_drop_elements,
                ARRAY_SIZE(dev_health_mobile_net_2g_drop_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_2g_oos_count = {"count", sizeof "count" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_2g_oos_count};
static dev_health_item_t const * const dev_health_mobile_net_2g_oos_elements[] =
{
    &dev_health_mobile_net_2g_oos_count
};

static dev_health_path_group_t const dev_health_mobile_net_2g_oos_group =
{
    "oos",
    sizeof "oos" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_2g_oos_elements,
                ARRAY_SIZE(dev_health_mobile_net_2g_oos_elements)
            }
        }
    }
};

static dev_health_path_group_t const * const dev_health_mobile_net_2g_subgroups[] =
{
    &dev_health_mobile_net_2g_rxbytes_group,
    &dev_health_mobile_net_2g_txbytes_group,
    &dev_health_mobile_net_2g_losspercent_group,
    &dev_health_mobile_net_2g_uptime_group,
    &dev_health_mobile_net_2g_latency_group,
    &dev_health_mobile_net_2g_transactions_group,
    &dev_health_mobile_net_2g_fdrop_group,
    &dev_health_mobile_net_2g_drop_group,
    &dev_health_mobile_net_2g_oos_group
};

static dev_health_path_group_t const dev_health_mobile_net_2g_group =
{
    "2g",
    sizeof "2g" - 1,
    NULL,
    {
        SUBGROUPS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_2g_subgroups,
                ARRAY_SIZE(dev_health_mobile_net_2g_subgroups)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_3g_rxbytes = {"rxbytes", sizeof "rxbytes" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_3g_rxbytes};
static dev_health_item_t const * const dev_health_mobile_net_3g_rxbytes_elements[] =
{
    &dev_health_mobile_net_3g_rxbytes
};

static dev_health_path_group_t const dev_health_mobile_net_3g_rxbytes_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_3g_rxbytes_elements,
                ARRAY_SIZE(dev_health_mobile_net_3g_rxbytes_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_3g_txbytes = {"txbytes", sizeof "txbytes" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_3g_txbytes};
static dev_health_item_t const * const dev_health_mobile_net_3g_txbytes_elements[] =
{
    &dev_health_mobile_net_3g_txbytes
};

static dev_health_path_group_t const dev_health_mobile_net_3g_txbytes_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_3g_txbytes_elements,
                ARRAY_SIZE(dev_health_mobile_net_3g_txbytes_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_3g_losspercent = {"losspercent", sizeof "losspercent" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_3g_losspercent};
static dev_health_item_t const * const dev_health_mobile_net_3g_losspercent_elements[] =
{
    &dev_health_mobile_net_3g_losspercent
};

static dev_health_path_group_t const dev_health_mobile_net_3g_losspercent_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_3g_losspercent_elements,
                ARRAY_SIZE(dev_health_mobile_net_3g_losspercent_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_3g_uptime = {"uptime", sizeof "uptime" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_3g_uptime};
static dev_health_item_t const * const dev_health_mobile_net_3g_uptime_elements[] =
{
    &dev_health_mobile_net_3g_uptime
};

static dev_health_path_group_t const dev_health_mobile_net_3g_uptime_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_3g_uptime_elements,
                ARRAY_SIZE(dev_health_mobile_net_3g_uptime_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_3g_latency_min = {"min", sizeof "min" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_3g_latency_min};
static const dev_health_item_t dev_health_mobile_net_3g_latency_avg = {"avg", sizeof "avg" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_3g_latency_avg};
static const dev_health_item_t dev_health_mobile_net_3g_latency_max = {"max", sizeof "max" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_3g_latency_max};

static dev_health_item_t const * const dev_health_mobile_net_3g_latency_elements[] =
{
    &dev_health_mobile_net_3g_latency_min,
    &dev_health_mobile_net_3g_latency_avg,
    &dev_health_mobile_net_3g_latency_max
};

static dev_health_path_group_t const dev_health_mobile_net_3g_latency_group =
{
    "latency",
    sizeof "latency" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_3g_latency_elements,
                ARRAY_SIZE(dev_health_mobile_net_3g_latency_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_3g_transactions_count = {"count", sizeof "count" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_3g_transactions_count};
static dev_health_item_t const * const dev_health_mobile_net_3g_transactions_elements[] =
{
    &dev_health_mobile_net_3g_transactions_count
};

static dev_health_path_group_t const dev_health_mobile_net_3g_transactions_group =
{
    "transactions",
    sizeof "transactions" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_3g_transactions_elements,
                ARRAY_SIZE(dev_health_mobile_net_3g_transactions_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_3g_fdrop_count = {"count", sizeof "count" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_3g_fdrop_count};
static dev_health_item_t const * const dev_health_mobile_net_3g_fdrop_elements[] =
{
    &dev_health_mobile_net_3g_fdrop_count
};

static dev_health_path_group_t const dev_health_mobile_net_3g_fdrop_group =
{
    "fdrop",
    sizeof "fdrop" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_3g_fdrop_elements,
                ARRAY_SIZE(dev_health_mobile_net_3g_fdrop_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_3g_drop_count = {"count", sizeof "count" - 1,  DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_3g_drop_count};
static dev_health_item_t const * const dev_health_mobile_net_3g_drop_elements[] =
{
    &dev_health_mobile_net_3g_drop_count
};

static dev_health_path_group_t const dev_health_mobile_net_3g_drop_group =
{
    "drop",
    sizeof "drop" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_3g_drop_elements,
                ARRAY_SIZE(dev_health_mobile_net_3g_drop_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_3g_oos_count = {"count", sizeof "count" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_3g_oos_count};
static dev_health_item_t const * const dev_health_mobile_net_3g_oos_elements[] =
{
    &dev_health_mobile_net_3g_oos_count
};

static dev_health_path_group_t const dev_health_mobile_net_3g_oos_group =
{
    "oos",
    sizeof "oos" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_3g_oos_elements,
                ARRAY_SIZE(dev_health_mobile_net_3g_oos_elements)
            }
        }
    }
};

static dev_health_path_group_t const * const dev_health_mobile_net_3g_subgroups[] =
{
    &dev_health_mobile_net_3g_rxbytes_group,
    &dev_health_mobile_net_3g_txbytes_group,
    &dev_health_mobile_net_3g_losspercent_group,
    &dev_health_mobile_net_3g_uptime_group,
    &dev_health_mobile_net_3g_latency_group,
    &dev_health_mobile_net_3g_transactions_group,
    &dev_health_mobile_net_3g_fdrop_group,
    &dev_health_mobile_net_3g_drop_group,
    &dev_health_mobile_net_3g_oos_group
};

static dev_health_path_group_t const dev_health_mobile_net_3g_group =
{
    "3g",
    sizeof "3g" - 1,
    NULL,
    {
        SUBGROUPS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_3g_subgroups,
                ARRAY_SIZE(dev_health_mobile_net_3g_subgroups)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_4g_rxbytes = {"rxbytes", sizeof "rxbytes" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_4g_rxbytes};
static dev_health_item_t const * const dev_health_mobile_net_4g_rxbytes_elements[] =
{
    &dev_health_mobile_net_4g_rxbytes
};

static dev_health_path_group_t const dev_health_mobile_net_4g_rxbytes_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_4g_rxbytes_elements,
                ARRAY_SIZE(dev_health_mobile_net_4g_rxbytes_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_4g_txbytes = {"txbytes", sizeof "txbytes" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_4g_txbytes};
static dev_health_item_t const * const dev_health_mobile_net_4g_txbytes_elements[] =
{
    &dev_health_mobile_net_4g_txbytes
};

static dev_health_path_group_t const dev_health_mobile_net_4g_txbytes_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_4g_txbytes_elements,
                ARRAY_SIZE(dev_health_mobile_net_4g_txbytes_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_4g_losspercent = {"losspercent", sizeof "losspercent" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_4g_losspercent};
static dev_health_item_t const * const dev_health_mobile_net_4g_losspercent_elements[] =
{
    &dev_health_mobile_net_4g_losspercent
};

static dev_health_path_group_t const dev_health_mobile_net_4g_losspercent_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_4g_losspercent_elements,
                ARRAY_SIZE(dev_health_mobile_net_4g_losspercent_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_4g_uptime = {"uptime", sizeof "uptime" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_4g_uptime};
static dev_health_item_t const * const dev_health_mobile_net_4g_uptime_elements[] =
{
    &dev_health_mobile_net_4g_uptime
};

static dev_health_path_group_t const dev_health_mobile_net_4g_uptime_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_4g_uptime_elements,
                ARRAY_SIZE(dev_health_mobile_net_4g_uptime_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_4g_latency_min = {"min", sizeof "min" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_4g_latency_min};
static const dev_health_item_t dev_health_mobile_net_4g_latency_avg = {"avg", sizeof "avg" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_4g_latency_avg};
static const dev_health_item_t dev_health_mobile_net_4g_latency_max = {"max", sizeof "max" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_4g_latency_max};

static dev_health_item_t const * const dev_health_mobile_net_4g_latency_elements[] =
{
    &dev_health_mobile_net_4g_latency_min,
    &dev_health_mobile_net_4g_latency_avg,
    &dev_health_mobile_net_4g_latency_max
};

static dev_health_path_group_t const dev_health_mobile_net_4g_latency_group =
{
    "latency",
    sizeof "latency" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_4g_latency_elements,
                ARRAY_SIZE(dev_health_mobile_net_4g_latency_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_4g_transactions_count = {"count", sizeof "count" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_4g_transactions_count};
static dev_health_item_t const * const dev_health_mobile_net_4g_transactions_elements[] =
{
    &dev_health_mobile_net_4g_transactions_count
};

static dev_health_path_group_t const dev_health_mobile_net_4g_transactions_group =
{
    "transactions",
    sizeof "transactions" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_4g_transactions_elements,
                ARRAY_SIZE(dev_health_mobile_net_4g_transactions_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_4g_fdrop_count = {"count", sizeof "count" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_4g_fdrop_count};
static dev_health_item_t const * const dev_health_mobile_net_4g_fdrop_elements[] =
{
    &dev_health_mobile_net_4g_fdrop_count
};

static dev_health_path_group_t const dev_health_mobile_net_4g_fdrop_group =
{
    "fdrop",
    sizeof "fdrop" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_4g_fdrop_elements,
                ARRAY_SIZE(dev_health_mobile_net_4g_fdrop_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_4g_drop_count = {"count", sizeof "count" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_4g_drop_count};
static dev_health_item_t const * const dev_health_mobile_net_4g_drop_elements[] =
{
    &dev_health_mobile_net_4g_drop_count
};

static dev_health_path_group_t const dev_health_mobile_net_4g_drop_group =
{
    "drop",
    sizeof "drop" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_4g_drop_elements,
                ARRAY_SIZE(dev_health_mobile_net_4g_drop_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_mobile_net_4g_oos_count = {"count", sizeof "count" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_mobile_net_4g_oos_count};
static dev_health_item_t const * const dev_health_mobile_net_4g_oos_elements[] =
{
    &dev_health_mobile_net_4g_oos_count
};

static dev_health_path_group_t const dev_health_mobile_net_4g_oos_group =
{
    "oos",
    sizeof "oos" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_4g_oos_elements,
                ARRAY_SIZE(dev_health_mobile_net_4g_oos_elements)
            }
        }
    }
};

static dev_health_path_group_t const * const dev_health_mobile_net_4g_subgroups[] =
{
    &dev_health_mobile_net_4g_rxbytes_group,
    &dev_health_mobile_net_4g_txbytes_group,
    &dev_health_mobile_net_4g_losspercent_group,
    &dev_health_mobile_net_4g_uptime_group,
    &dev_health_mobile_net_4g_latency_group,
    &dev_health_mobile_net_4g_transactions_group,
    &dev_health_mobile_net_4g_fdrop_group,
    &dev_health_mobile_net_4g_drop_group,
    &dev_health_mobile_net_4g_oos_group
};

static dev_health_path_group_t const dev_health_mobile_net_4g_group =
{
    "4g",
    sizeof "4g" - 1,
    NULL,
    {
        SUBGROUPS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_4g_subgroups,
                ARRAY_SIZE(dev_health_mobile_net_4g_subgroups)
            }
        }
    }
};


static dev_health_path_group_t const * const dev_health_mobile_net_subgroups[] =
{
    &dev_health_mobile_net_cell_id_group,
    &dev_health_mobile_net_network_group,
    &dev_health_mobile_net_rssi_group,
    &dev_health_mobile_net_ecio_group,
    &dev_health_mobile_net_rsrp_group,
    &dev_health_mobile_net_rsrq_group,
    &dev_health_mobile_net_sinr_group,
    &dev_health_mobile_net_snr_group,
    &dev_health_mobile_net_status_group,
    &dev_health_mobile_net_lac_group,
    &dev_health_mobile_net_tac_group,
    &dev_health_mobile_net_session_group,
    &dev_health_mobile_net_info_group,
    &dev_health_mobile_net_2g_group,
    &dev_health_mobile_net_3g_group,
    &dev_health_mobile_net_4g_group
};

static dev_health_path_group_t const dev_health_mobile_module_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_module_elements,
                ARRAY_SIZE(dev_health_mobile_module_elements)
            }
        }
    }
};

static dev_health_path_group_t const dev_health_mobile_temperature_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_temperature_elements,
                ARRAY_SIZE(dev_health_mobile_temperature_elements)
            }
        }
    }
};

static dev_health_path_group_t const dev_health_mobile_active_sim_group =
{
    "",
    sizeof "" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_active_sim_elements,
                ARRAY_SIZE(dev_health_mobile_active_sim_elements)
            }
        }
    }
};

static dev_health_path_group_t const dev_health_mobile_net_group =
{
    "net",
    sizeof "net" - 1,
    cc_dev_health_get_mobile_net_instances,
    {
        SUBGROUPS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_net_subgroups,
                ARRAY_SIZE(dev_health_mobile_net_subgroups)
            }
        }
    }
};

static dev_health_path_group_t const * const dev_health_mobile_levels[] =
{
    &dev_health_mobile_module_group,
    &dev_health_mobile_temperature_group,
    &dev_health_mobile_active_sim_group,
    &dev_health_mobile_net_group
};

static dev_health_path_group_t const dev_health_root_group_mobile =
{
    "mobile",
    sizeof "mobile" - 1,
    cc_dev_health_get_mobile_instances,
    {
        SUBGROUPS,
        {
            {
                (dev_health_item_t const * const *)dev_health_mobile_levels,
                ARRAY_SIZE(dev_health_mobile_levels)
            }
        }
    }
};

static const dev_health_item_t dev_health_eth_tx_bytes = {"bytes", sizeof "bytes" - 1, DEV_HEALTH_TYPE_UINT64, (dev_health_query_fn_t)cc_dev_health_get_eth_tx_bytes};
static const dev_health_item_t dev_health_eth_tx_packets = {"packets", sizeof "packets" - 1, DEV_HEALTH_TYPE_UINT64, (dev_health_query_fn_t)cc_dev_health_get_eth_tx_packets};
static const dev_health_item_t dev_health_eth_tx_dropped = {"dropped", sizeof "dropped" - 1, DEV_HEALTH_TYPE_UINT64, (dev_health_query_fn_t)cc_dev_health_get_eth_tx_dropped};
static const dev_health_item_t dev_health_eth_tx_overruns = {"overruns", sizeof "overruns" - 1, DEV_HEALTH_TYPE_UINT64, (dev_health_query_fn_t)cc_dev_health_get_eth_tx_overruns};

static dev_health_item_t const * const dev_health_eth_tx_elements[] =
{
    &dev_health_eth_tx_bytes,
    &dev_health_eth_tx_packets,
    &dev_health_eth_tx_dropped,
    &dev_health_eth_tx_overruns
};

static dev_health_path_group_t const dev_health_eth_tx =
{
    "tx",
    sizeof "tx" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                dev_health_eth_tx_elements,
                ARRAY_SIZE(dev_health_eth_tx_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_eth_rx_bytes = {"bytes", sizeof "bytes" - 1, DEV_HEALTH_TYPE_UINT64, (dev_health_query_fn_t)cc_dev_health_get_eth_rx_bytes};
static const dev_health_item_t dev_health_eth_rx_packets = {"packets", sizeof "packets" - 1, DEV_HEALTH_TYPE_UINT64, (dev_health_query_fn_t)cc_dev_health_get_eth_rx_packets};
static const dev_health_item_t dev_health_eth_rx_dropped = {"dropped", sizeof "dropped" - 1, DEV_HEALTH_TYPE_UINT64, (dev_health_query_fn_t)cc_dev_health_get_eth_rx_dropped};

static dev_health_item_t const * const dev_health_eth_rx_elements[] =
{
    &dev_health_eth_rx_bytes,
    &dev_health_eth_rx_packets,
    &dev_health_eth_rx_dropped
};

static dev_health_path_group_t const dev_health_eth_rx =
{
    "rx",
    sizeof "rx" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                dev_health_eth_rx_elements,
                ARRAY_SIZE(dev_health_eth_rx_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_eth_link_down_count = {"down_count", sizeof "down_count" - 1, DEV_HEALTH_TYPE_UINT64, (dev_health_query_fn_t)cc_dev_health_get_eth_link_down_count};
static const dev_health_item_t dev_health_eth_link_down_duration = {"down_duration", sizeof "down_duration" - 1, DEV_HEALTH_TYPE_UINT64, (dev_health_query_fn_t)cc_dev_health_get_eth_link_down_duration};

static dev_health_item_t const * const dev_health_eth_link_elements[] =
{
    &dev_health_eth_link_down_count,
    &dev_health_eth_link_down_duration
};

static dev_health_path_group_t const dev_health_eth_link =
{
    "link",
    sizeof "link" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                dev_health_eth_link_elements,
                ARRAY_SIZE(dev_health_eth_link_elements)
            }
        }
    }
};

static dev_health_path_group_t const * const dev_health_eth_levels[] =
{
    &dev_health_eth_tx,
    &dev_health_eth_rx,
    &dev_health_eth_link
};

static dev_health_path_group_t const dev_health_root_group_eth =
{
    "eth",
    sizeof "eth" - 1,
    cc_dev_health_get_eth_instances,
    {
        SUBGROUPS,
        {
            {
                (dev_health_item_t const * const *)dev_health_eth_levels,
                ARRAY_SIZE(dev_health_eth_levels)
            }
        }
    }
};

static const dev_health_item_t dev_health_wifi_radio = {"radio", sizeof "radio" - 1, DEV_HEALTH_TYPE_JSON, (dev_health_query_fn_t)cc_dev_health_get_wifi_radio_json};
static const dev_health_item_t dev_health_wifi_status = {"status", sizeof "status" - 1, DEV_HEALTH_TYPE_STRING, (dev_health_query_fn_t)cc_dev_health_get_wifi_status};
static const dev_health_item_t dev_health_wifi_rssi = {"rssi", sizeof "rssi" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_wifi_rssi};
static const dev_health_item_t dev_health_wifi_rate = {"rate", sizeof "rate" - 1, DEV_HEALTH_TYPE_FLOAT, (dev_health_query_fn_t)cc_dev_health_get_wifi_rate};
static const dev_health_item_t dev_health_wifi_clients = {"clients", sizeof "clients" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_wifi_clients};

static dev_health_item_t const * const dev_health_wifi_elements[] =
{
    &dev_health_wifi_radio,
    &dev_health_wifi_status,
    &dev_health_wifi_rssi,
    &dev_health_wifi_rate,
    &dev_health_wifi_clients
};

static dev_health_path_group_t const dev_health_root_group_wifi =
{
    "wifi",
    sizeof "wifi" - 1,
    cc_dev_health_get_wifi_instances,
    {
        ITEMS,
        {
            {
                dev_health_wifi_elements,
                ARRAY_SIZE(dev_health_wifi_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_system_mem_available = {"available", sizeof "available" - 1, DEV_HEALTH_TYPE_UINT64, (dev_health_query_fn_t)cc_dev_health_get_system_mem_available};
static const dev_health_item_t dev_health_system_mem_free = {"free", sizeof "free" - 1, DEV_HEALTH_TYPE_UINT64, (dev_health_query_fn_t)cc_dev_health_get_system_mem_free};

static dev_health_item_t const * const dev_health_system_mem_elements[] =
{
    &dev_health_system_mem_available,
    &dev_health_system_mem_free
};

static dev_health_path_group_t const dev_health_system_mem =
{
    "mem",
    sizeof "mem" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                dev_health_system_mem_elements,
                ARRAY_SIZE(dev_health_system_mem_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_system_reboots_items = {"", sizeof "" - 1,  DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_system_reboots};

static dev_health_item_t const * const dev_health_system_reboots_elements[] =
{
    &dev_health_system_reboots_items,
};

static dev_health_path_group_t const dev_health_system_reboots =
{
    "reboots",
    sizeof "reboots" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                dev_health_system_reboots_elements,
                ARRAY_SIZE(dev_health_system_reboots_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_system_msg_free = {"free", sizeof "free" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_system_msg_free};
static const dev_health_item_t dev_health_system_msg_min = {"min", sizeof "min" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_system_msg_min};

static dev_health_item_t const * const dev_health_system_msg_elements[] =
{
    &dev_health_system_msg_free,
    &dev_health_system_msg_min
};

static dev_health_path_group_t const dev_health_system_msg =
{
    "msg",
    sizeof "msg" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                dev_health_system_msg_elements,
                 ARRAY_SIZE(dev_health_system_msg_elements)
            }
        }

    }
};

static const dev_health_item_t dev_health_system_buf_free = {"free", sizeof "free" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_system_buf_free};
static const dev_health_item_t dev_health_system_buf_min = {"min", sizeof "min" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_system_buf_min};

static dev_health_item_t const * const dev_health_system_buf_elements[] =
{
    &dev_health_system_buf_free,
    &dev_health_system_buf_min
};

static dev_health_path_group_t const dev_health_system_buf =
{
    "buf",
    sizeof "buf" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                dev_health_system_buf_elements,
                ARRAY_SIZE(dev_health_system_buf_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_system_bigbuf_free = {"free", sizeof "free" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_system_bigbuf_free};
static const dev_health_item_t dev_health_system_bigbuf_min = {"min", sizeof "min" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_system_bigbuf_min};

static dev_health_item_t const * const dev_health_system_bigbuf_elements[] =
{
    &dev_health_system_bigbuf_free,
    &dev_health_system_bigbuf_min
};

static dev_health_path_group_t const dev_health_system_bigbuf =
{
    "bigbuf",
    sizeof "bigbuf" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                dev_health_system_bigbuf_elements,
                ARRAY_SIZE(dev_health_system_bigbuf_elements)
            }
        }
    }
};

static const dev_health_item_t dev_health_system_cpu_used = {"used", sizeof "used" - 1, DEV_HEALTH_TYPE_INT32, (dev_health_query_fn_t)cc_dev_health_get_system_cpu_used};

static dev_health_item_t const * const dev_health_system_cpu_elements[] =
{
    &dev_health_system_cpu_used
};

static dev_health_path_group_t const dev_health_system_cpu =
{
    "cpu",
    sizeof "cpu" - 1,
    NULL,
    {
        ITEMS,
        {
            {
                dev_health_system_cpu_elements,
                ARRAY_SIZE(dev_health_system_cpu_elements)
            }
        }
    }
};

static dev_health_path_group_t const * const dev_health_system_groups[] =
{
    &dev_health_system_mem,
    &dev_health_system_reboots,
    &dev_health_system_msg,
    &dev_health_system_buf,
    &dev_health_system_bigbuf,
    &dev_health_system_cpu
};

static dev_health_path_group_t const dev_health_root_group_system =
{
    "sys",
    sizeof "sys" - 1,
    NULL,
    {
        SUBGROUPS,
        {
            {
                (dev_health_item_t const * const *)dev_health_system_groups,
                ARRAY_SIZE(dev_health_system_groups)
            }
        }
    }
};

static dev_health_path_group_t const * const dev_health_root_groups[] =
{
    &dev_health_root_group_mobile,
    &dev_health_root_group_eth,
    /*&dev_health_root_group_wifi,*/ /* TODO: IC4C-402 */
    &dev_health_root_group_system
};
