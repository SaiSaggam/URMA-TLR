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
*****************************************************************************/

#include "cellular_show_json.h"
#include "cellular_status.h"
#include "config_mgr.h"
#include "ip_helper.h"
#include "tlr_common.h"
#include "strlcpy.h"

#include <stdio.h>
#include <assert.h>
#include <json.h>
#include <string.h>

static void add_element_value_to_show_object(cellular_status_t const * const pstatus,
                                             tlr_show_object_t * const show_object,
                                             config_state_cellular_t const element_id)
{
    void const * value = NULL;
    size_t const length_required = UINT64_STRING_LENGTH;
    char buffer[length_required];

    switch (element_id)
    {
        case config_state_cellular_description:
            value = pstatus->description;
            break;

        case config_state_cellular_module:
            value = pstatus->module;
            break;

        case config_state_cellular_firmware_version:
            value = pstatus->firmware;
            break;

        case config_state_cellular_hardware_version:
            value = pstatus->hardware;
            break;

        case config_state_cellular_imei:
            value = pstatus->imei;
            break;

        case config_state_cellular_sim_status:
            if ((pstatus->active_sim >= 0) && (pstatus->active_sim < cellular_get_sim_status_strings_count()))
            {
                value = cellular_get_sim_status_string(pstatus->active_sim);
            }
            break;

        case config_state_cellular_signal_strength:
            value = cellular_get_signal_strength_status_string(pstatus);
            break;

        case config_state_cellular_signal_quality:
            value = cellular_get_signal_quality_status_string(pstatus);
            break;

        case config_state_cellular_registration_status:
            value = cellular_get_registration_status_string(pstatus);
            break;

        case config_state_cellular_network_provider:
            value = pstatus->network_provider;
            break;

        case config_state_cellular_temperature:
            value = &pstatus->temperature;
            break;
        case config_state_cellular_connection_type:
            value = cellular_get_connection_type_string(pstatus);
            break;

        case config_state_cellular_radio_band:
            value = pstatus->radio_band;
            break;

        case config_state_cellular_channel:
        {
             snprintf(buffer, sizeof buffer, "%zu", pstatus->channel);
             value = buffer;
             break;
        }

        case config_state_cellular_ip_address:
            value = pstatus->ipaddr;
            break;

        case config_state_cellular_mask:
            value = pstatus->netmask;
            break;

        case config_state_cellular_gateway:
            value = pstatus->gateway;
            break;

        case config_state_cellular_dns_servers:
            value = pstatus->dns_servers;
            break;

        case config_state_cellular_pdp_context:
            value = pstatus->pdp_context;
            break;

        case config_state_cellular_rx_packets:
            value = &pstatus->rx_packets;
            break;

        case config_state_cellular_tx_packets:
            value = &pstatus->tx_packets;
            break;

        case config_state_cellular_rx_bytes:
            value = &pstatus->rx_bytes;
            break;

        case config_state_cellular_tx_bytes:
            value = &pstatus->tx_bytes;
            break;
    }

    if (value != NULL)
    {
        tlr_show_object_add_element(show_object, element_id, value);
    }
}

static void add_element_to_show_object(cellular_status_t const * const pstatus, tlr_show_object_t * const show_object, config_mgr_element_t const * const element)
{
    size_t const element_id = element->id;

    add_element_value_to_show_object(pstatus, show_object, element_id);
}

static void add_elements_to_show_object(cellular_status_t const * const pstatus,
                                        tlr_show_object_t * const show_object, int const instance)
{
    config_mgr_group_t const * const group = tlr_get_group_ptr(config_state_cellular, instance, config_mgr_get_group);
    int i;

    assert(group != NULL);

    for (i = 0; i < group->elements.count; i++)
    {
        //printf("adding element %s\n", group->elements.element[i].name);
        add_element_to_show_object(pstatus, show_object, &group->elements.element[i]);
    }
}

int cellular_show_json(cellular_status_t const * const pstatus, int const instance)
{
    int result = -1;

    tlr_show_object_t * const show_object = tlr_show_object_create(config_state_cellular, instance);

    if (show_object == NULL)
    {
        cellular_error("failed to create show object\n");
        goto error;
    }

    add_elements_to_show_object(pstatus, show_object, instance);

    printf("%s\n", tlr_show_object_to_json_string(show_object));
    tlr_show_object_destroy(show_object);

    result = 0;
error:
    return result;

}


