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

#include "dsl_show_ascii.h"
#include "dsl_context.h"
#include "dsl_config.h"
#include "dsl_modem.h"
#include "dsl_log.h"
#include "ifuptime_lib.h"
#include "ip_helper.h"

#include <stdio.h>
#include <strings.h>
#include <inttypes.h>
#include <assert.h>

static char const * get_enum_description(size_t const element_id, size_t const enum_id)
{
    config_mgr_element_enum_limit_t const * const element_enum_limit = tlr_get_element_enum_limit(config_mgr_table_type_show, config_state_dsl, element_id);
    config_mgr_element_enum_t const * const element_enum = element_enum_limit->value;
    char const * enum_description = NULL;
    int i;

    for (i = 0; i < element_enum_limit->count; i++)
    {
        if (element_enum[i].id == enum_id)
        {
            enum_description = element_enum[i].description;
            break;
        }
    }

    assert(enum_description != NULL);

    return enum_description;
}

void dsl_show_ascii(int const socket_fd, dsl_context_t const * const context)
{
    char buf[100];
    char const * const dsl_interface_name = interface_get_name_from_type(dsl_interface);
    int mtu = 0;

	dprintf(socket_fd, " DSL Status and Statistics\n");
	dprintf(socket_fd, " -------------------------\n");
	dprintf(socket_fd, " Description      : %s\n", context->config[config_dsl_description].value);
	dprintf(socket_fd, " Admin Status     : %s\n", get_enum_description(config_state_dsl_admin_status, context->admin_status));
    dprintf(socket_fd, " Oper Status      : %s\n", get_enum_description(config_state_dsl_oper_status, context->oper_status));

    if (context->current_state == dsl_state_connected)
    {
        uint64_t upsecs = 0;
        get_ifuptime(dsl_interface_name, &upsecs);
        format_ifuptime(upsecs, buf, sizeof buf);
        dprintf(socket_fd, " Up Time          : %s\n", buf);
    }

	dprintf(socket_fd, " HW Version       : %s\n", dsl_modem_status->hw_version);
	dprintf(socket_fd, " FW Version       : %s\n", dsl_modem_status->ras_fw_version);
	dprintf(socket_fd, " System FW ID     : %s\n", dsl_modem_status->system_fw_id);
	dprintf(socket_fd, "\n");
	dprintf(socket_fd, " Line Status      : %s", get_enum_description(config_state_dsl_line_status, dsl_modem_status->line_status));

	if (dsl_modem_status->line_status != config_state_dsl_line_status_up)
	{
        dprintf(socket_fd, "\n");
        goto done;
	}

	format_ifuptime(dsl_modem_status->uptime_secs, buf, sizeof buf);
	dprintf(socket_fd, " (%s)\n", buf);

	dprintf(socket_fd, " Mode             : %s\n", get_enum_description(config_state_dsl_line_mode, dsl_modem_status->line_mode));
    dprintf(socket_fd, " Encapsulation    : %s\n", get_enum_description(config_state_dsl_encapsulation, dsl_modem_config->encapsulation));
    dprintf(socket_fd, " VPI/VCI          : %s/%s\n", context->config[config_dsl_vpi].value, context->config[config_dsl_vci].value);

    get_mtu(dsl_interface_name, &mtu);
    dprintf(socket_fd, " MTU              : %d\n", mtu);

	dprintf(socket_fd, " Remote Vendor ID : %s\n", dsl_modem_status->remote_vendor_id);
	dprintf(socket_fd, "\n");

    get_ipaddr(dsl_interface_name, buf, sizeof buf);
    dprintf(socket_fd, " IP Address       : %s\n", buf);

    get_newmaskaddr(dsl_interface_name, buf, sizeof buf);
    dprintf(socket_fd, " Mask             : %s\n", buf);

    get_destaddr(dsl_interface_name, buf, sizeof buf);
    dprintf(socket_fd, " Gateway          : %s\n", buf);

    if (context->current_state == dsl_state_connected)
    {
        uint64_t rx_count;
        uint64_t tx_count;

        dprintf(socket_fd, "\n");
        dprintf(socket_fd, "                  %-24s%-24s\n", "Received", "Sent");
        dprintf(socket_fd, "                  %-24s%-24s\n", "--------", "----");

        rx_count = 0;
        get_rx_packets(dsl_interface_name, &rx_count);
        tx_count = 0;
        get_tx_packets(dsl_interface_name, &tx_count);
        dprintf(socket_fd, " Packets          %-24" PRIu64 "%-20" PRIu64 "\n", rx_count, tx_count);

        rx_count = 0;
        get_rx_bytes(dsl_interface_name, &rx_count);
        tx_count = 0;
        get_tx_bytes(dsl_interface_name, &tx_count);
        dprintf(socket_fd, " Bytes            %-24" PRIu64 "%-20" PRIu64 "\n", rx_count, tx_count);
    }

    dprintf(socket_fd, "\n");
    dprintf(socket_fd, "\n");
	dprintf(socket_fd, "                        %-20s%-20s\n", "Downstream", "Upstream");
	dprintf(socket_fd, "                        %-20s%-20s\n", "----------", "--------");
	dprintf(socket_fd, " Speed (kbps)           %-20lu%-20lu\n", dsl_modem_status->downstream.speed, dsl_modem_status->upstream.speed);
	dprintf(socket_fd, " Channel Type           %-20s%-20s\n",
        get_enum_description(config_state_dsl_downstream_channel_type, dsl_modem_status->downstream.channel_type),
        get_enum_description(config_state_dsl_upstream_channel_type, dsl_modem_status->upstream.channel_type));
	dprintf(socket_fd, " Relative Capacity (%%)  %-20s%-20s\n", dsl_modem_status->downstream.relative_capacity, dsl_modem_status->upstream.relative_capacity);
	dprintf(socket_fd, " Attenuation (dB)       %-20s%-20s\n", dsl_modem_status->downstream.attenuation, dsl_modem_status->upstream.attenuation);
	dprintf(socket_fd, " Noise Margin (dB)      %-20s%-20s\n", dsl_modem_status->downstream.noise_margin, dsl_modem_status->upstream.noise_margin);
	dprintf(socket_fd, " Output Power (dBm)     %-20s%-20s\n", dsl_modem_status->upstream.output_power, dsl_modem_status->downstream.output_power);	// Note: output power is intentionally reversed
	dprintf(socket_fd, " FEC                    %-20u%-20u\n", dsl_modem_status->downstream.fec_error, dsl_modem_status->upstream.fec_error);
	dprintf(socket_fd, " CRC                    %-20u%-20u\n", dsl_modem_status->downstream.crc_error, dsl_modem_status->upstream.crc_error);
	dprintf(socket_fd, " HEC                    %-20u%-20u\n", dsl_modem_status->downstream.hec_error, dsl_modem_status->upstream.hec_error);
	dprintf(socket_fd, "\n");
	dprintf(socket_fd, " Errored Seconds in 15 Minutes  : %u\n", dsl_modem_status->errored_secs_15min);
	dprintf(socket_fd, " Errored Seconds in 24 Hours    : %u\n", dsl_modem_status->errored_secs_24hr);
	dprintf(socket_fd, " Errored Seconds after Line Up  : %u\n", dsl_modem_status->errored_secs_powerup);

done:
	dprintf(socket_fd, "\n");
}
