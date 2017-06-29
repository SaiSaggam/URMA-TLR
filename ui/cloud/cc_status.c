/*****************************************************************************
* Copyright (c) 2016 Digi International Inc., All Rights Reserved
*
* This software contains proprietary and confidential information of Digi
* International Inc.  By accepting transfer of this copy, Recipient agrees
* to retain this software in confidence, to prevent disclosure to others,
* and to make no use of this software other than that for which it was
* delivered.  This is an unpublished copyrighted work of Digi International
* Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
* prohibited.
*
* Restricted Rights Legend
*
* Use, duplication, or disclosure by the Government is subject to
* restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
* Technical Data and Computer Software clause at DFARS 252.227-7031 or
* subparagraphs (c)(1) and (2) of the Commercial Computer Software -
* Restricted Rights at 48 CFR 52.227-19, as applicable.
*
* Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
*
****************************************************************************/
#include "connector_api.h"
#include "platform.h"
#include "config_mgr.h"
#include "cc_status.h"
#include "ip_helper.h"
#include "odm_specific.h"
#include "show_parse_helper.h"

#include <unittest_support.h>

STATIC cloud_status_t tlr_cloud_status;

static void update_device_id_string(void)
{
    uint8_t const * const id_ptr = cc_get_device_id();
    char * const id_str = tlr_cloud_status.device_id_str;
    char const format[] = "%02X%02X%02X%02X-%02X%02X%02X%02X-%02X%02X%02X%02X-%02X%02X%02X%02X";

    snprintf(id_str, sizeof tlr_cloud_status.device_id_str, format,
             id_ptr[0], id_ptr[1], id_ptr[2], id_ptr[3],
             id_ptr[4], id_ptr[5], id_ptr[6], id_ptr[7],
             id_ptr[8], id_ptr[9], id_ptr[10], id_ptr[11],
             id_ptr[12], id_ptr[13], id_ptr[14], id_ptr[15]);
}

static connector_callback_status_t app_tcp_status(connector_status_tcp_event_t const * const tcp_event)
{
    /* We don't want to see first missed and restored keepalive debug printf.
     * Keepalive sometimes missed and restored almost at the same time.
     */
    static size_t keepalive_missed_count = 0;

    switch (tcp_event->status)
    {
    case connector_tcp_communication_started:
        cc_set_connected();
        keepalive_missed_count = 0;
        APP_DEBUG("connector_tcp_communication_started\n");
        break;

    case connector_tcp_keepalive_missed:
        if (keepalive_missed_count > 0)
        {
            APP_DEBUG("connector_tcp_keepalive_missed\n");
        }
        keepalive_missed_count++;
        break;

    case connector_tcp_keepalive_restored:
        keepalive_missed_count--;
        if (keepalive_missed_count > 0)
        {
            APP_DEBUG("connector_tcp_keepalive_restored\n");
        }
        break;
    }

    return connector_callback_continue;
}

connector_callback_status_t app_status_handler(connector_request_id_status_t const request,
                                               void * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request)
    {
    case connector_request_id_status_tcp:
        status = app_tcp_status(data);
        break;
    case connector_request_id_status_stop_completed:
        APP_DEBUG("app_status_handle: connector_request_id_status_stop_completed\n");
        break;
    default:
        /* unsupported */
        APP_DEBUG("app_status_handler unsupported request %d\n", request);
        status = connector_callback_unrecognized;
        break;
    }

    return status;
}

uint8_t * const cc_get_mac_addr(void)
{
    char const interface[] = "eth1";
    struct ifreq ifr;

    if (get_ifr(interface, SIOCGIFHWADDR, &ifr))
    {
        memcpy(tlr_cloud_status.mac_addr, ifr.ifr_hwaddr.sa_data, sizeof tlr_cloud_status.mac_addr);
    }
    else
    {
        memset(tlr_cloud_status.mac_addr, 0, sizeof tlr_cloud_status.mac_addr);
    }

    return tlr_cloud_status.mac_addr;
}

uint8_t * const cc_get_device_id(void)
{
    uint8_t const * const mac_ptr = cc_get_mac_addr();

    tlr_cloud_status.device_id[8] = mac_ptr[0];
    tlr_cloud_status.device_id[9] = mac_ptr[1];
    tlr_cloud_status.device_id[10] = mac_ptr[2];
    tlr_cloud_status.device_id[11] = 0xFF;
    tlr_cloud_status.device_id[12] = 0xFF;
    tlr_cloud_status.device_id[13] = mac_ptr[3];
    tlr_cloud_status.device_id[14] = mac_ptr[4];
    tlr_cloud_status.device_id[15] = mac_ptr[5];

    return tlr_cloud_status.device_id;
}

char * const cc_get_cloud_url(void)
{
    config_status_t status;

    status = tlr_get_element_value(config_cloud, FIRST_INSTANCE, config_cloud_server,
                                   tlr_cloud_status.server_name, sizeof tlr_cloud_status.server_name,
                                   config_mgr_element_type_string, config_mgr_access_level_read_only);

    if (status != config_status_success)
    {
        tlr_cloud_status.server_name[0] = '\0';
    }

    return tlr_cloud_status.server_name;
}

char * const cc_get_device_type(void)
{
    char const * model_name = NULL;

    if (get_tlr_platform_name(&model_name) == 0)
    {
        // snprintf(tlr_cloud_status.device_type, sizeof tlr_cloud_status.device_type, "Transport %s", model_name);
        snprintf(tlr_cloud_status.device_type, sizeof tlr_cloud_status.device_type, "%s", model_name);
    }
    else
    {
        tlr_cloud_status.device_type[0] = '\0';
    }

    return tlr_cloud_status.device_type;
}

#define update_cloud_url() (void)cc_get_cloud_url()
#define update_device_type() (void)cc_get_device_type()

void cc_clear_cloud_status(void)
{
    memset(&tlr_cloud_status, 0, sizeof tlr_cloud_status);
    update_device_id_string();
    update_cloud_url();
    update_device_type();
}

cloud_status_t * cc_get_cloud_status(void)
{
    return &tlr_cloud_status;
}

void cc_set_restart_cloud(void)
{
    tlr_cloud_status.restart_cloud = true;
}

void cc_set_connected_status(bool const state)
{
    tlr_cloud_status.connected = state;

    if (state)
    {
        tlr_cloud_status.start_time = time(NULL);
    }
}
