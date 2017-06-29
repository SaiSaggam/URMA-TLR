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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <libgen.h>
#include <unistd.h>
#include <json.h>
#include <string.h>
#include <syslog.h>
#include "csman.h"
#include "csid/csid_3g.h"
#include "cellular_work.h"
#include "cellular_status.h"
#include "cellular_show_json.h"
#include "unittest_support.h"
#include "tlr_common.h"
#include "ip_helper.h"
#include "show_parse_helper.h"
#include "tlr_print.h"

static char const excellent_string[] = "Excellent";
static char const good_string[] = "Good";
static char const fair_string[] = "Fair";
static char const poor_string[] = "Poor";
static char const no_service_string[] = "No service";
static char const not_available_string[] = "Not available";
static char const fair_to_poor_string[] = "Fair to Poor";

static char const * const cellular_sim_status_strings[] = {
        "Off", "Using SIM1", "Using SIM2"
};
size_t const cellular_sim_status_strings_count = ARRAY_SIZE(cellular_sim_status_strings);


static char const * get_element_label(config_state_cellular_t const element_id)
{
    char const * name = NULL;

    switch (element_id)
    {
        case config_state_cellular_description:
            name = "Description";
            break;

        case config_state_cellular_module:
            name = "Module";
            break;

        case config_state_cellular_firmware_version:
            name = "Firmware version";
            break;

        case config_state_cellular_hardware_version:
            name = "Hardware version";
            break;

        case config_state_cellular_imei:
            name = "IMEI";
            break;

        case config_state_cellular_sim_status:
            name = "SIM status";
            break;

        case config_state_cellular_signal_strength:
            name = "Signal strength";
            break;

        case config_state_cellular_signal_quality:
            name = "Signal quality";
            break;

        case config_state_cellular_registration_status:
            name = "Registration status";
            break;

        case config_state_cellular_network_provider:
            name = "Network provider";
            break;

        case config_state_cellular_connection_type:
            name = "Connection type";
            break;

        case config_state_cellular_radio_band:
            name = "Radio Band";
            break;

        case config_state_cellular_channel:
            name = "Channel";
            break;

        case config_state_cellular_pdp_context:
            name = "APN in use";
            break;

        case config_state_cellular_ip_address:
            name = "IP address";
            break;

        case config_state_cellular_mask:
            name = "Mask";
            break;

        case config_state_cellular_gateway:
            name = "Gateway";
            break;

        case config_state_cellular_dns_servers:
            name = "DNS servers";
            break;

        case config_state_cellular_rx_packets:
            name = "Receive packets";
            break;

        case config_state_cellular_tx_packets:
            name = "Sent packets";
            break;

        case config_state_cellular_rx_bytes:
            name = "Receive bytes";
            break;

        case config_state_cellular_tx_bytes:
            name = "Sent bytes";
            break;

        case config_state_cellular_temperature:
            name = "Temperature";
            break;

    }

    return name;
}

STATIC_FUNCTION int get_cellular_description(size_t const active_sim, char * const description, size_t const length)
{
    int ret = 0;

    if ((description == NULL) || (length == 0))
    {
        ret = -1;
        cellular_error("Invalid parameter for cellular description\n");
        goto done;
    }

    strcpy(description, "");

    if (active_sim > 0)
    {
        if (tlr_get_element_value(config_cellular, active_sim, config_cellular_description,
                                    description, length,
                                    config_mgr_element_type_string, config_mgr_access_level_read_only) != config_status_success)
        {
            ret = -1;
        }
    }

done:
    return ret;
}

static int cellular_read_ip_statistics(char const * const interface_name, config_state_cellular_t const sid, uint64_t * const value, size_t const value_size)
{
    int ret= -1;

    if ((value == NULL) || (value_size != sizeof (uint64_t)))
    {
        cellular_debug("Invalid parameter for cellular status  (incorrect size %d)\n", (int)value_size);
        goto done;
    }

    if (interface_name == NULL)
    {
        cellular_debug("Can't get interface name for cellular status\n");
        goto done;
    }

    if (*interface_name == '\0')
    {
        ret = 0;  //let other items update and display
        goto done;
    }

    switch (sid)
    {
        /* these cases are handled with cellular_read_status() in libcellular */
        case config_state_cellular_description:
        case config_state_cellular_module:
        case config_state_cellular_firmware_version:
        case config_state_cellular_hardware_version:
        case config_state_cellular_imei:
        case config_state_cellular_sim_status:
        case config_state_cellular_signal_strength:
        case config_state_cellular_signal_quality:
        case config_state_cellular_registration_status:
        case config_state_cellular_network_provider:
        case config_state_cellular_temperature:
        case config_state_cellular_connection_type:
        case config_state_cellular_radio_band:
        case config_state_cellular_channel:
        case config_state_cellular_ip_address:
        case config_state_cellular_mask:
        case config_state_cellular_gateway:
        case config_state_cellular_dns_servers:
        case config_state_cellular_pdp_context:
            break;

        case config_state_cellular_rx_packets:
            ret = get_rx_packets(interface_name, value);
            break;
        case config_state_cellular_tx_packets:
            ret = get_tx_packets(interface_name, value);
            break;
        case config_state_cellular_rx_bytes:
            ret = get_rx_bytes(interface_name, value);
            break;
        case config_state_cellular_tx_bytes:
            ret = get_tx_bytes(interface_name, value);
            break;
    }

    done:
    return ret;
}

STATIC_FUNCTION int get_cellular_stats(cellular_status_t * const pstatus)
{
    int ret = -1;
    int fd = -1;

    if (pstatus == NULL) goto done;

    fd = cellular_open_status();
    if (fd < 0) goto done;

    ret = cellular_read_status(fd, pstatus->index_3g, config_state_cellular_module, &pstatus->module, sizeof pstatus->module);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain cellular module\n");
    }


    ret = cellular_read_status(fd, pstatus->index_3g, config_state_cellular_sim_status, &pstatus->active_sim, sizeof pstatus->active_sim);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain SIM status\n");
    }

    switch (get_cellular_state(pstatus->active_sim))
    {
        case config_cellular_state_off:
        {
            pstatus->active_sim = 0;
            break;
        }
        case config_cellular_state_on_demand:
            switch (cellular_get_connection_status(fd, pstatus->index_3g))
            {
                case cellular_disconnecting:
                case cellular_disconnected:
                    pstatus->active_sim = 0;
                    break;
                case cellular_connecting:
                case cellular_connected:
                    break;
            }
            break;
        case config_cellular_state_on:
            break;

    }

    ret =cellular_read_status(fd, pstatus->index_3g, config_state_cellular_firmware_version, pstatus->firmware, sizeof pstatus->firmware);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain cellular firmware version\n");
    }

    ret =cellular_read_status(fd, pstatus->index_3g, config_state_cellular_hardware_version, pstatus->hardware, sizeof pstatus->hardware);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain cellular hardware version\n");
    }

    ret =cellular_read_status(fd, pstatus->index_3g, config_state_cellular_imei, pstatus->imei, sizeof pstatus->imei);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain IMEI\n");
    }

    ret =cellular_read_status(fd, pstatus->index_3g, config_state_cellular_signal_strength, &pstatus->signal_strength, sizeof pstatus->signal_strength);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain signal strength\n");
    }

    ret =cellular_read_status(fd, pstatus->index_3g, config_state_cellular_signal_quality, &pstatus->signal_quality, sizeof pstatus->signal_quality);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain cellular signal quality\n");
    }

    /* description from UCI */
    ret = get_cellular_description(pstatus->active_sim, pstatus->description, sizeof pstatus->description);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain cellular description\n");
    }


    ret =cellular_read_status(fd, pstatus->index_3g, config_state_cellular_temperature, &pstatus->temperature, sizeof pstatus->temperature);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain temperature\n");
    }

    ret =cellular_read_status(fd, pstatus->index_3g, config_state_cellular_registration_status, &pstatus->registration_status, sizeof pstatus->registration_status);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain cellular registration status\n");
    }

    ret =cellular_read_status(fd, pstatus->index_3g, config_state_cellular_network_provider, pstatus->network_provider, sizeof pstatus->network_provider);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain network provider\n");
    }

    ret =cellular_read_status(fd, pstatus->index_3g, config_state_cellular_connection_type, &pstatus->connection_type, sizeof pstatus->connection_type);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain cellular connection type\n");
    }

    ret =cellular_read_status(fd, pstatus->index_3g, config_state_cellular_radio_band, pstatus->radio_band, sizeof pstatus->radio_band);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain radio band\n");
    }

    ret =cellular_read_status(fd, pstatus->index_3g, config_state_cellular_channel, &pstatus->channel, sizeof pstatus->channel);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain cellular channel\n");
    }

    ret =cellular_read_status(fd, pstatus->index_3g, config_state_cellular_pdp_context, pstatus->pdp_context, sizeof pstatus->pdp_context);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain cellular PDP context\n");
    }

    ret =cellular_read_status(fd, pstatus->index_3g, config_state_cellular_ip_address, pstatus->ipaddr, sizeof pstatus->ipaddr);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain cellular IP address\n");
    }

    ret =cellular_read_status(fd, pstatus->index_3g, config_state_cellular_mask, pstatus->netmask, sizeof pstatus->netmask);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain cellular netmask\n");
    }

    ret =cellular_read_status(fd, pstatus->index_3g, config_state_cellular_gateway, pstatus->gateway, sizeof pstatus->gateway);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain cellular gateway\n");
    }

    ret =cellular_read_status(fd, pstatus->index_3g, config_state_cellular_dns_servers, pstatus->dns_servers, sizeof pstatus->dns_servers);
    if (ret < 0)
    {
        cellular_debug("Unable to obtain cellular DNS\n");
    }

    {
        char const *const interface_name = cellular_get_interface_name(fd, pstatus->index_3g);


        ret = cellular_read_ip_statistics(interface_name, config_state_cellular_rx_packets, &pstatus->rx_packets,
                                          sizeof pstatus->rx_packets);
        if (ret < 0)
        {
            cellular_debug("Unable to obtain rx packets\n");
        }

        ret = cellular_read_ip_statistics(interface_name, config_state_cellular_tx_packets, &pstatus->tx_packets,
                                          sizeof pstatus->tx_packets);
        if (ret < 0)
        {
            cellular_debug("Unable to obtain tx packets\n");
        }

        ret = cellular_read_ip_statistics(interface_name, config_state_cellular_rx_bytes, &pstatus->rx_bytes,
                                          sizeof pstatus->rx_bytes);
        if (ret < 0)
        {
            cellular_debug("Unable to obtain rx bytes\n");
        }

        ret = cellular_read_ip_statistics(interface_name, config_state_cellular_tx_bytes, &pstatus->tx_bytes,
                                          sizeof pstatus->tx_bytes);
        if (ret < 0)
        {
            cellular_debug("Unable to obtain tx bytes\n");
        }
    }

    done:
    cellular_close_status(fd);
    return ret;
}

static int show_ascii(cellular_status_t *pstatus, int instance)
{

    fprintf(stdout, " Cellular Status and Statistics\n");
    fprintf(stdout, " ------------------------------\n\n");

    if (!STRING_IS_EMPTY(pstatus->description))
        fprintf(stdout, " %-20s %3s %-s\n\n", get_element_label(config_state_cellular_description), ":", pstatus->description);

    fprintf(stdout, " %-20s %3s %-s\n", get_element_label(config_state_cellular_module), ":", pstatus->module);
    fprintf(stdout, " %-20s %3s %-s\n", get_element_label(config_state_cellular_firmware_version), ":", pstatus->firmware);
    fprintf(stdout, " %-20s %3s %-s\n", get_element_label(config_state_cellular_hardware_version), ":", pstatus->hardware);
    fprintf(stdout, " %-20s %3s %-s\n", get_element_label(config_state_cellular_imei), ":", pstatus->imei);

    if ((pstatus->active_sim >= 0) && (pstatus->active_sim < cellular_get_sim_status_strings_count()))
    {
        fprintf(stdout, "\n %-20s %3s %-s\n", get_element_label(config_state_cellular_sim_status), ":",
                cellular_get_sim_status_string(pstatus->active_sim));
    }

    fprintf(stdout, "\n %-20s %3s %-s\n", get_element_label(config_state_cellular_signal_strength), ":",
            cellular_get_signal_strength_status_string(pstatus));
    fprintf(stdout, " %-20s %3s %-s\n\n", get_element_label(config_state_cellular_signal_quality), ":",
            cellular_get_signal_quality_status_string(pstatus));

    fprintf(stdout, "\n %-20s %3s %-s\n\n", get_element_label(config_state_cellular_registration_status), ":",
            cellular_get_registration_status_string(pstatus));
    if (!STRING_IS_EMPTY(pstatus->network_provider))
        fprintf(stdout, " %-20s %3s %-s\n", get_element_label(config_state_cellular_network_provider), ":", pstatus->network_provider);

    if (pstatus->temperature != 0)
    {
        fprintf(stdout, " %-20s %3s %-dC\n", get_element_label(config_state_cellular_temperature), ":", pstatus->temperature);
    }

    fprintf(stdout, " %-20s %3s %-s\n", get_element_label(config_state_cellular_connection_type), ":",
            cellular_get_connection_type_string(pstatus));
    fprintf(stdout, " %-20s %3s %-s\n", get_element_label(config_state_cellular_radio_band), ":", pstatus->radio_band);

    if (pstatus->channel > 0)
        fprintf(stdout, " %-20s %3s %-zd\n\n", get_element_label(config_state_cellular_channel), ":", pstatus->channel);
#if 0
    else
        fprintf(stdout, "%-20s %3s %-s\n", get_element_label(config_state_cellular_channel), ":", "");
#endif

    //if (!STRING_IS_EMPTY(pstatus->pdp_context))
    fprintf(stdout, " %-20s %3s %-s\n", get_element_label(config_state_cellular_pdp_context), ":", pstatus->pdp_context);

    //if (!STRING_IS_EMPTY(pstatus->ipaddr))
    fprintf(stdout, "\n %-20s %3s %-s\n", get_element_label(config_state_cellular_ip_address), ":", pstatus->ipaddr);
    //if (!STRING_IS_EMPTY(pstatus->netmask))
    fprintf(stdout, " %-20s %3s %-s\n", get_element_label(config_state_cellular_mask), ":", pstatus->netmask);
    //if (!STRING_IS_EMPTY(pstatus->gateway))
    fprintf(stdout, " %-20s %3s %-s\n", get_element_label(config_state_cellular_gateway), ":", pstatus->gateway);
    //if (!STRING_IS_EMPTY(pstatus->dns_servers))
    fprintf(stdout, " %-20s %3s %-s\n", get_element_label(config_state_cellular_dns_servers), ":", pstatus->dns_servers);

    if (pstatus->rx_packets > 0 || pstatus->tx_packets > 0 || pstatus->rx_bytes > 0 || pstatus->tx_bytes > 0)
    {
        fprintf(stdout,"\n");
        fprintf(stdout,"                   %-24s%-24s\n", "Received", "Sent");
        fprintf(stdout,"                   %-24s%-24s\n", "--------", "----");
        fprintf(stdout," Packets           %-24" PRIu64 "%-20" PRIu64 "\n",  pstatus->rx_packets, pstatus->tx_packets);
        fprintf(stdout," Bytes             %-24" PRIu64 "%-20" PRIu64 "\n", pstatus->rx_bytes, pstatus->tx_bytes);
    }

    fprintf(stdout,"\n");

    return 0;

}

char const * cellular_get_sim_status_string(int const index)
{

    char const *status_string = NULL;

    if (index < cellular_sim_status_strings_count)
    {
        status_string = cellular_sim_status_strings[index];
    }
    return status_string;
}

int cellular_get_sim_status_strings_count(void)
{
    return cellular_sim_status_strings_count;
}

char * cellular_get_signal_strength_status_string(cellular_status_t const * const pstatus)
{

    static char const signal_strength_format[] = "%s (%ddBm)";
    static char signal_strength_string[sizeof signal_strength_format + sizeof not_available_string + INTEGER_STRING_LENGTH];

    char * status;

    if (pstatus->signal_strength == 0)
    {
        *signal_strength_string = '\0';
        goto done;
    }

    /* TODO: use RSRP for 4G connection */
    switch (pstatus->connection_type)
    {
        case cellular_conenction_type_4g:
            if (pstatus->signal_strength > -90)
                status = (char *)excellent_string;
            else if (pstatus->signal_strength > -106)
                status = (char *)good_string;
            else if (pstatus->signal_strength > -116)
                status = (char *)fair_string;
            else if (pstatus->signal_strength > -121)
                status = (char *)poor_string;
            else
                status = (char *)no_service_string;
            break;

        default:
            if (pstatus->signal_strength > -70)
                status = (char *)excellent_string;
            else if (pstatus->signal_strength > -86)
                status = (char *)good_string;
            else if (pstatus->signal_strength > -101)
                status = (char *)fair_string;
            else if (pstatus->signal_strength == -110)
                status = (char *)no_service_string;
            else
                status = (char *)poor_string;
            break;
    }

    snprintf(signal_strength_string, sizeof signal_strength_string, signal_strength_format,
                                                status, pstatus->signal_strength);

done:
    return signal_strength_string;
}

char * cellular_get_signal_quality_status_string(cellular_status_t const * const pstatus)
{

    static char const signal_quality_format[] = "%s (%ddB)";
    static char signal_quality_string[sizeof signal_quality_format + sizeof fair_to_poor_string + INTEGER_STRING_LENGTH];

    char * status;

    if (pstatus->signal_quality == 0)
    {
        *signal_quality_string = '\0';
        goto done;
    }

    /* TODO: use RSRP for 4G connection */
    switch (pstatus->connection_type)
    {
        case cellular_conenction_type_4g:
            if (pstatus->signal_quality > -9)
                status = (char *)excellent_string;
            else if (pstatus->signal_quality > -13)
                status = (char *)good_string;
            else
                status = (char *)fair_to_poor_string;
            break;

        default:
            if (pstatus->signal_quality > -7)
                status = (char *)excellent_string;
            else if (pstatus->signal_quality > -11)
                status = (char *)good_string;
            else
                status = (char *)fair_to_poor_string;
    }

    snprintf(signal_quality_string, sizeof signal_quality_string, signal_quality_format,
                                                status, pstatus->signal_quality);
done:
    return signal_quality_string;
}

char const * cellular_get_registration_status_string(cellular_status_t const * const pstatus)
{
    static char * registration_status_strings[] = { "Not registered", "Registered", "Searching","Roaming"};

    char * status = NULL;

    if (pstatus->registration_status < (sizeof registration_status_strings/ sizeof registration_status_strings[0]))
        status = registration_status_strings[pstatus->registration_status];

    return status;
}

char const * cellular_get_connection_type_string(cellular_status_t const * const pstatus)
{
    static char * connection_types_strings[] = { "", "2G", "3G", "4G"};

    char * type;

    if (pstatus->connection_type < (sizeof connection_types_strings/ sizeof connection_types_strings[0]))
        type = connection_types_strings[pstatus->connection_type];
    else
        type = connection_types_strings[0];

    return type;
}

int cellular_main (int argc, char *argv[])
{
    static cellular_status_t cellular_status;
    int ret = EXIT_FAILURE;
    tlr_response_format_t format;
    int instance;
    char *progname;
    int max_instance = tlr_get_max_instance(config_state_cellular, config_mgr_table_type_show);

    openlog("Show Cellular", LOG_PID, LOG_USER);

    if (max_instance <= 0)
    {
        goto error;
    }

    progname = basename(argv[0]);

    if (parse_show_options(argc, argv, &format, &instance, 0, max_instance) != 0)
    {
        usage_show(progname);
        goto error;
    }

    if (instance == 0)
    {
        instance = 1; // only 1 show cellular
    }

    cellular_status.index_3g = CELLULAR_INDEX_3G;  /* only 1 modem */

    get_cellular_stats(&cellular_status);

    switch (format)
    {
        case tlr_response_format_ascii:
            if (show_ascii(&cellular_status, instance) < 0)
            {
                goto error;
            }
            break;
        case tlr_response_format_json:
            if (cellular_show_json(&cellular_status, instance) < 0)
            {
                goto error;
            }
            break;
        case tlr_response_format_unknown:
            cellular_error("Invalid format %d\n", format);
            goto error;
    }

    ret = EXIT_SUCCESS;

error:
    closelog();

    return ret;
}

