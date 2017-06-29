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

#include "dsl_show_json.h"
#include "dsl_context.h"
#include "dsl_config.h"
#include "dsl_modem.h"
#include "dsl_log.h"
#include "config_mgr.h"
#include "ifuptime_lib.h"
#include "ip_helper.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static void add_element_value_to_show_object(dsl_context_t const * const context, tlr_show_object_t * const show_object, config_state_dsl_t const element_id)
{
    char const * const dsl_interface_name = interface_get_name_from_type(dsl_interface);

    switch (element_id)
    {
        case config_state_dsl_description:
            tlr_show_object_add_element(show_object, element_id, context->config[config_dsl_description].value);
            break;

        case config_state_dsl_admin_status:
            tlr_show_object_add_element(show_object, element_id, &context->admin_status);
            break;

        case config_state_dsl_oper_status:
            tlr_show_object_add_element(show_object, element_id, &context->oper_status);
            break;

        case config_state_dsl_uptime:
        {
            uint64_t uptime_in_seconds = 0;
            get_ifuptime(dsl_interface_name, &uptime_in_seconds);
            uint32_t const uint32_uptime_in_seconds = uptime_in_seconds;
            tlr_show_object_add_element(show_object, element_id, &uint32_uptime_in_seconds);
            break;
        }

        case config_state_dsl_hardware_version:
            tlr_show_object_add_element(show_object, element_id, dsl_modem_status->hw_version);
            break;

        case config_state_dsl_firmware_version:
            tlr_show_object_add_element(show_object, element_id, dsl_modem_status->ras_fw_version);
            break;

        case config_state_dsl_system_firmware_id:
            tlr_show_object_add_element(show_object, element_id, dsl_modem_status->system_fw_id);
            break;

        case config_state_dsl_line_status:
            tlr_show_object_add_element(show_object, element_id, &dsl_modem_status->line_status);
            break;

        case config_state_dsl_line_uptime:
            tlr_show_object_add_element(show_object, element_id, &dsl_modem_status->uptime_secs);
            break;

        case config_state_dsl_line_mode:
            tlr_show_object_add_element(show_object, element_id, &dsl_modem_status->line_mode);
            break;

        case config_state_dsl_encapsulation:
            tlr_show_object_add_element(show_object, element_id, &dsl_modem_config->encapsulation);
            break;

        case config_state_dsl_vpi:
        {
            int const vpi = strtoul(context->config[config_dsl_vpi].value, NULL, 10);
            tlr_show_object_add_element(show_object, element_id, &vpi);
            break;
        }

        case config_state_dsl_vci:
        {
            int const vci = strtoul(context->config[config_dsl_vci].value, NULL, 10);
            tlr_show_object_add_element(show_object, element_id, &vci);
            break;
        }

        case config_state_dsl_mtu:
        {
            int mtu;
            get_mtu(dsl_interface_name, &mtu);
            tlr_show_object_add_element(show_object, element_id, &mtu);
            break;
        }

        case config_state_dsl_remote_vendor_id:
            tlr_show_object_add_element(show_object, element_id, dsl_modem_status->remote_vendor_id);
            break;

        case config_state_dsl_ip_address:
        {
            char ip_address[IP_ADDRESS_SIZE];
            get_ipaddr(dsl_interface_name, ip_address, sizeof ip_address);
            tlr_show_object_add_element(show_object, element_id, ip_address);
            break;
        }

        case config_state_dsl_mask:
        {
            char netmask[IP_ADDRESS_SIZE];
            get_newmaskaddr(dsl_interface_name, netmask, sizeof netmask);
            tlr_show_object_add_element(show_object, element_id, netmask);
            break;
        }

        case config_state_dsl_gateway:
        {
            char gateway[IP_ADDRESS_SIZE];
            get_destaddr(dsl_interface_name, gateway, sizeof gateway);
            tlr_show_object_add_element(show_object, element_id, gateway);
            break;
        }

        case config_state_dsl_rx_packets:
        {
            uint64_t rx_packets = 0;
            if (context->current_state == dsl_state_connected)
            {
                get_rx_packets(dsl_interface_name, &rx_packets);
            }
            tlr_show_object_add_element(show_object, element_id, &rx_packets);
            break;
        }

        case config_state_dsl_tx_packets:
        {
            uint64_t tx_packets = 0;
            if (context->current_state == dsl_state_connected)
            {
                get_tx_packets(dsl_interface_name, &tx_packets);
            }
            tlr_show_object_add_element(show_object, element_id, &tx_packets);
            break;
        }

        case config_state_dsl_rx_bytes:
        {
            uint64_t rx_bytes = 0;
            if (context->current_state == dsl_state_connected)
            {
                get_rx_bytes(dsl_interface_name, &rx_bytes);
            }
            tlr_show_object_add_element(show_object, element_id, &rx_bytes);
            break;
        }

        case config_state_dsl_tx_bytes:
        {
            uint64_t tx_bytes = 0;
            if (context->current_state == dsl_state_connected)
            {
                get_tx_bytes(dsl_interface_name, &tx_bytes);
            }
            tlr_show_object_add_element(show_object, element_id, &tx_bytes);
            break;
        }

        case config_state_dsl_downstream_speed:
            tlr_show_object_add_element(show_object, element_id, &dsl_modem_status->downstream.speed);
            break;

        case config_state_dsl_upstream_speed:
            tlr_show_object_add_element(show_object, element_id, &dsl_modem_status->upstream.speed);
            break;

        case config_state_dsl_downstream_channel_type:
            tlr_show_object_add_element(show_object, element_id, &dsl_modem_status->downstream.channel_type);
            break;

        case config_state_dsl_upstream_channel_type:
            tlr_show_object_add_element(show_object, element_id, &dsl_modem_status->upstream.channel_type);
            break;

        case config_state_dsl_downstream_relative_capacity:
        {
            uint32_t const downstream_relative_capacity = strtoul(dsl_modem_status->downstream.relative_capacity, NULL, 10);
            tlr_show_object_add_element(show_object, element_id, &downstream_relative_capacity);
            break;
        }

        case config_state_dsl_upstream_relative_capacity:
        {
            uint32_t const upstream_relative_capacity = strtoul(dsl_modem_status->upstream.relative_capacity, NULL, 10);
            tlr_show_object_add_element(show_object, element_id, &upstream_relative_capacity);
            break;
        }

        case config_state_dsl_downstream_attentuation:
            tlr_show_object_add_element(show_object, element_id, dsl_modem_status->downstream.attenuation);
            break;

        case config_state_dsl_upstream_attentuation:
            tlr_show_object_add_element(show_object, element_id, dsl_modem_status->upstream.attenuation);
            break;

        case config_state_dsl_downstream_noise_margin:
            tlr_show_object_add_element(show_object, element_id, dsl_modem_status->downstream.noise_margin);
            break;

        case config_state_dsl_upstream_noise_margin:
            tlr_show_object_add_element(show_object, element_id, dsl_modem_status->upstream.noise_margin);
            break;

        case config_state_dsl_downstream_output_power:
            tlr_show_object_add_element(show_object, element_id, dsl_modem_status->upstream.output_power);   // Note: output power is intentionally reversed
            break;

        case config_state_dsl_upstream_output_power:
            tlr_show_object_add_element(show_object, element_id, dsl_modem_status->downstream.output_power); // Note: output power is intentionally reversed
            break;

        case config_state_dsl_downstream_fec_errors:
            tlr_show_object_add_element(show_object, element_id, &dsl_modem_status->downstream.fec_error);
            break;

        case config_state_dsl_upstream_fec_errors:
            tlr_show_object_add_element(show_object, element_id, &dsl_modem_status->upstream.fec_error);
            break;

        case config_state_dsl_downstream_crc_errors:
            tlr_show_object_add_element(show_object, element_id, &dsl_modem_status->downstream.crc_error);
            break;

        case config_state_dsl_upstream_crc_errors:
            tlr_show_object_add_element(show_object, element_id, &dsl_modem_status->upstream.crc_error);
            break;

        case config_state_dsl_downstream_hec_errors:
            tlr_show_object_add_element(show_object, element_id, &dsl_modem_status->downstream.hec_error);
            break;

        case config_state_dsl_upstream_hec_errors:
            tlr_show_object_add_element(show_object, element_id, &dsl_modem_status->upstream.hec_error);
            break;

        case config_state_dsl_errored_secs_15min:
            tlr_show_object_add_element(show_object, element_id, &dsl_modem_status->errored_secs_15min);
            break;

        case config_state_dsl_errored_secs_24hr:
            tlr_show_object_add_element(show_object, element_id, &dsl_modem_status->errored_secs_24hr);
            break;

        case config_state_dsl_errored_secs_lineup:
            tlr_show_object_add_element(show_object, element_id, &dsl_modem_status->errored_secs_powerup);
            break;
    }
}

static void add_element_to_show_object(dsl_context_t const * const context, tlr_show_object_t * const show_object, config_mgr_element_t const * const element)
{
    size_t const element_id = element->id;

    add_element_value_to_show_object(context, show_object, element_id);
}

static void add_elements_to_show_object(dsl_context_t const * const context, tlr_show_object_t * const show_object)
{
    config_mgr_group_t const * const group = tlr_get_group_ptr(config_state_dsl, context->instance, config_mgr_get_group);
    int i;

    assert(group != NULL);

    for (i = 0; i < group->elements.count; i++)
    {
        add_element_to_show_object(context, show_object, &group->elements.element[i]);
    }
}

void dsl_show_json(int const socket_fd, dsl_context_t const * const context)
{
    tlr_show_object_t * const show_object = tlr_show_object_create(config_state_dsl, context->instance);

    if (show_object == NULL)
    {
        dsl_error("failed to create show object\n");
        goto done;
    }

    add_elements_to_show_object(context, show_object);
    dprintf(socket_fd, "%s\n", tlr_show_object_to_json_string(show_object));
    tlr_show_object_destroy(show_object);

done:
    ;
}
