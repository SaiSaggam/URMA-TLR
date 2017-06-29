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

#include "show_lan_work.h"
#include "tlr_common.h"
#include "config_mgr.h"
#include "config_def.h"
#include "tlr_print.h"
#include "interfaces.h"
#include "ip_helper.h"
#include "show_parse_helper.h"
#include "show_status_helper.h"
#include "strlcpy.h"
#include "unittest_support.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <json.h>
#include <ctype.h>

static tlr_log_t debug_log_level = tlr_log_debug;

static const char NAMESERVER_KEYWORD[] = "nameserver";
#define MAX_DNS_NAMESERVER_SIZE     sizeof "255.255.255.255"
#define MAX_NAMESERVER_KEYWORD_SIZE sizeof NAMESERVER_KEYWORD

static char up_string[] = "Up";
static char down_string[] = "Down";

#define TO_OPERATIONAL_STATUS_STRING(operstate)  (operstate) ? up_string : down_string

#ifndef _CUNIT_
#define show_lan_free        free
#define show_lan_opendir     opendir
#define show_lan_readdir     readdir
#define show_lan_closedir    closedir

#else
extern void show_lan_free(void *);
extern DIR  * show_lan_opendir(const char *filename);
extern int show_lan_closedir(DIR * dirp);
extern struct dirent  * show_lan_readdir(DIR * dirp);
#endif


static void output_summary_ascii(lan_status_t * const lan)
{
    char const description_string[] = "Description";
    size_t const ip_spaces = sizeof lan->ip_address + 1;
    size_t const description_spaces = strlen(description_string);

    if (lan->instance == 1)
    {
        fprintf_status_header(stdout, DIVIDER_WIDTH_AUTO, " %-3s %-8s %-*s %-*s %-*s\n",
                        "#", "Status",
                        (int)ip_spaces, "IP Address",
                        (int)ip_spaces, "Mask",
                        (int)description_spaces, description_string);
    }

    if (lan->admin_status)
    {
        fprintf(stdout, " %-3zu %-8s %-*s %-*s %-s\r\n",
                    lan->instance, TO_OPERATIONAL_STATUS_STRING(lan->oper_status),
                    (int)ip_spaces, lan->ip_address,
                    (int)ip_spaces, lan->mask,
                    lan->description);
    }
}

static void output_ascii(lan_status_t * const lan)
{
    char const packets_string[] = "Packets";
    char const bytes_string[] = "Bytes";
    size_t const packets_length = sizeof packets_string;
    size_t const bytes_length = sizeof bytes_string;

    size_t const spaces = packets_length > bytes_length ? packets_length : bytes_length;
    char const empty_space[] = "";

    fprintf_status_header(stdout, DIVIDER_WIDTH_AUTO, " LAN %zu Status and Statistics\n", lan->instance);

    fprintf(stdout, " Admin Status  : %s\r\n",
                        (lan->admin_status) ? up_string : down_string);

    fprintf(stdout, " Oper Status   : %s\r\n\r\n",  TO_OPERATIONAL_STATUS_STRING(lan->oper_status));

    fprintf(stdout, " Description   : %s\r\n\r\n", lan->description != NULL ? lan->description : empty_space);

    fprintf(stdout, " Interfaces    : %s\r\n", lan->interfaces != NULL ? lan->interfaces : empty_space);
    fprintf(stdout, " MTU           : %d\r\n\r\n", lan->mtu);

    fprintf(stdout, " DHCP client   : %s\r\n", lan->dhcp );
    fprintf(stdout, " IP Address    : %s\r\n", lan->ip_address );
    fprintf(stdout, " Mask          : %s\r\n", lan->mask );
    fprintf(stdout, " DNS Server(s) : %s", lan->dns1 );
    if (strlen(lan->dns2) != 0)
    {
        fprintf(stdout, ", %s", lan->dns2);
    }
    fprintf(stdout, "\r\n\r\n");

    if (lan->admin_status)
    {
        size_t const tx_packets_spaces = lan->tx_packets > (UINT64_MAX >> 1) ?
                                    UINT64_STRING_LENGTH : (UINT64_STRING_LENGTH >> 1);
        size_t const tx_bytes_spaces = lan->tx_bytes > (UINT64_MAX >> 1) ?
                                    UINT64_STRING_LENGTH : (UINT64_STRING_LENGTH >> 1);

        size_t const rx_packets_spaces = lan->rx_packets > (UINT64_MAX >> 1) ?
                                    UINT64_STRING_LENGTH : (UINT64_STRING_LENGTH >> 1);
        size_t const rx_bytes_spaces = lan->rx_bytes > (UINT64_MAX >> 1) ?
                                    UINT64_STRING_LENGTH : (UINT64_STRING_LENGTH >> 1);

        size_t const packets_spaces = (tx_packets_spaces >  rx_packets_spaces) ?
                                        tx_packets_spaces : rx_packets_spaces;

        size_t const bytes_spaces = (tx_bytes_spaces >  rx_bytes_spaces) ?
                                        tx_bytes_spaces : rx_bytes_spaces;

        fprintf(stdout, " %-*s %*s   %*s\r\n", (int)spaces, empty_space,
                            (int)packets_spaces, "Received",
                            (int)bytes_spaces, "Sent");

        fprintf(stdout, " %-*s %*s   %*s\r\n", (int)spaces, empty_space,
                            (int)packets_spaces, "--------",
                            (int)bytes_spaces, "----");

        fprintf(stdout, " %-*s %*" PRIu64  "   %*" PRIu64 "\r\n",
                            (int)spaces, packets_string,
                            (int)packets_spaces, lan->rx_packets,
                            (int)bytes_spaces, lan->tx_packets);

        fprintf(stdout, " %-*s %*" PRIu64  "   %*" PRIu64 "\r\n",
                            (int)spaces, bytes_string,
                            (int)packets_spaces, lan->rx_bytes,
                            (int)bytes_spaces, lan->tx_bytes);

    }
}

STATIC_FUNCTION bool get_lan_state(size_t const instance)
{
    bool is_on = false;

    unsigned int * state = NULL;

    config_status_t const status = tlr_get_element_alloc_value(config_lan, instance, config_lan_state, (void **)&state,
                                    config_mgr_element_type_on_off, config_mgr_access_level_super);

    if (state != NULL)
    {
        is_on = (*state != 0);
        show_lan_free(state);
    }

    if (status != config_status_success)
    {
        tlr_event_debug(debug_log_level, "unable to get state configuration (%d)\n", status);
    }

    return is_on;
}

STATIC_FUNCTION bool get_operational_status(char const * const interface_name)
{
    char * operstate = tlr_get_sys_class_net(interface_name, "operstate");

    bool is_operational = (strcasecmp((operstate), down_string) != 0);

    show_lan_free(operstate);

    return is_operational;
}

STATIC_FUNCTION char * get_lan_description(size_t const instance)
{
    char * description = NULL;

    config_status_t const status = tlr_get_element_alloc_value(config_lan, instance, config_lan_description, (void **)&description,
                                        config_mgr_element_type_string, config_mgr_access_level_super);

    if (status != config_status_success)
    {
        tlr_event_debug(debug_log_level, "unable to get description configuration (%d)\n", status);
    }

    return description;
}

STATIC_FUNCTION void get_dhcp(char const * const interface_name, tlr_handle_t const handle, char * const dhcp, size_t const dhcp_size)
{
    *dhcp = '\0';
    if (handle != NULL)
    {
        if (tlr_config_read(handle, config_lan_dhcp_client, dhcp, dhcp_size) != config_status_success)
        {
            tlr_event_debug(debug_log_level, "unable to read configuration of DHCP client for %s", interface_name);
        }
    }

}

STATIC_FUNCTION bool get_current_dns_servers(char const * const interface_name, tlr_handle_t const handle,
                                             char * const dns1, size_t const dns1_size,
                                             char * const dns2, size_t const dns2_size)
{
    char const resolv_format[] = "/ram/var/dns/%s/resolv.conf";
    char * filename = NULL;
    FILE * resolv = NULL;
    char keyword[MAX_NAMESERVER_KEYWORD_SIZE+1];
    char value[MAX_DNS_NAMESERVER_SIZE+1];
    char * line_buffer = NULL;
    size_t bytes_read = 0;
    bool got_dns_servers = false;
    char const nameserver_format_input[] = " %%%zus %%%zus ";
    char * nameserver_format = NULL;

    *dns1 = '\0';
    *dns2 = '\0';
    if (asprintf(&filename, resolv_format, interface_name) == -1)
    {
        tlr_error("out of memory");
        goto ERROR;
    }
    if (asprintf(&nameserver_format, nameserver_format_input, MAX_NAMESERVER_KEYWORD_SIZE, MAX_DNS_NAMESERVER_SIZE) == -1)
    {
        tlr_error("out of memory");
        goto ERROR;
    }

    resolv = fopen(filename, "r");
    if (resolv == NULL)
    {
        goto ERROR;
    }
    do
    {
        bytes_read = getline(&line_buffer, &bytes_read, resolv);
        if (((int) bytes_read) > 0)
        {
            int num_value_read = sscanf(line_buffer, nameserver_format, keyword, value);
            if (num_value_read == 2)
            {
                if (strcmp(keyword, "nameserver") == 0)
                {
                    if (*dns1 == 0)
                    {
                        strlcpy(dns1, value, dns1_size);
                    }
                    else if (*dns2 == 0)
                    {
                        strlcpy(dns2, value, dns2_size);
                    }
                }
            }
        }
    } while ((bytes_read != -1) && (*dns2 == 0));

    got_dns_servers = strlen(dns1) > 0;

ERROR:
    free(line_buffer);
    free(filename);
    free(nameserver_format);

    if (resolv != NULL)
    {
        fclose(resolv);
    }
    return got_dns_servers;
}

STATIC_FUNCTION void get_dns_servers(char const * const interface_name, tlr_handle_t const handle,
                                     char * const dns1, size_t const dns1_len,
                                     char * const dns2, size_t const dns2_len)
{
    if (!get_current_dns_servers(interface_name, handle, dns1, dns1_len, dns2, dns2_len))
    {
        if (handle != NULL)
        {
            if (tlr_config_read(handle, config_lan_dns1, dns1, dns1_len) != config_status_success)
            {
                tlr_event_debug(debug_log_level, "unable to read dns1 configuration for %s", interface_name);
                goto done;
            }
            if (tlr_config_read(handle, config_lan_dns2, dns2, dns2_len) != config_status_success)
            {
                tlr_event_debug(debug_log_level, "unable to read dns2 configuration for %s", interface_name);
                goto done;
            }
        }
    }
done:
    return;
}


STATIC_FUNCTION char * get_lan_interfaces(char const * const interface_name)
{
    char * interfaces = NULL;
    DIR * dp;
    char * dirname = NULL;

    if ((interface_name == NULL) || (asprintf(&dirname, "/sys/class/net/%s/brif/", interface_name) < 0) || (dirname == NULL))
    {
        goto done;
    }

    dp = show_lan_opendir(dirname);

    if (dp != NULL)
    {
        size_t const length =  tlr_get_config_value_max_length(config_mgr_table_type_config, config_lan, config_lan_interfaces);
        char directories[length];

        char * ptr = directories;
        size_t avail_length = length;

        struct dirent * ep;

        while ((avail_length > 0) && ((ep = show_lan_readdir(dp)) != NULL))
        {
            if ((strcmp(ep->d_name, ".") != 0) && (strcmp(ep->d_name, "..") != 0))
            {

                int written_length = snprintf(ptr, avail_length, "%s%s",
                                                (avail_length != length) ? "," : "",
                                                        ep->d_name);

                ptr += written_length;
                avail_length -= written_length;
            }
        }

        if (avail_length != length)
        {
            asprintf(&interfaces, "%s", directories);
        }
        show_lan_closedir(dp);
    }

    show_lan_free(dirname);

done:

    return interfaces;
}

STATIC_FUNCTION void get_lan_status(int const instance, lan_status_t * const lan)
{

    lan->instance = instance;


    {
        char const * name = interface_get_name_from_type(bridge_interface);
        char interface_name[strlen(name) + INTEGER_STRING_LENGTH];
        tlr_handle_t handle = tlr_config_open(config_lan, instance, config_mgr_access_level_read_only);

        if (handle == NULL)
        {
            tlr_event_debug(debug_log_level, "unable to read configuration of %s", name);
        }

        snprintf(interface_name, sizeof interface_name, "%s%zu", name, lan->instance);

        lan->interfaces = get_lan_interfaces(interface_name);

        lan->mac_address = tlr_get_mac_address(interface_name);

        lan->oper_status = get_operational_status(interface_name);

        get_mtu(interface_name, &lan->mtu);

        get_ipaddr(interface_name, lan->ip_address, sizeof lan->ip_address);

        get_dhcp(interface_name, handle, lan->dhcp, sizeof(lan->dhcp));

        get_dns_servers(interface_name, handle, lan->dns1, sizeof(lan->dns1), lan->dns2, sizeof(lan->dns2));

        get_newmaskaddr(interface_name, lan->mask, sizeof lan->mask);

        if (get_rx_packets(interface_name, &lan->rx_packets) < 0)
        {
            tlr_event_debug(debug_log_level, "unable to obtain RX packets on %s\n", interface_name);
        }

        if (get_tx_packets(interface_name, &lan->tx_packets) < 0)
        {
            tlr_event_debug(debug_log_level, "unable to obtain TX packets on %s\n", interface_name);
        }

        if (get_rx_bytes(interface_name, &lan->rx_bytes) < 0)
        {
            tlr_event_debug(debug_log_level, "unable to obtain RX bytes on %s\n", interface_name);
        }

        if (get_tx_bytes(interface_name, &lan->tx_bytes) < 0)
        {
            tlr_event_debug(debug_log_level, "unable to obtain TX bytes on %s\n", interface_name);
        }
        if (handle != NULL)
        {
            tlr_config_close(handle);
        }
    }

    //    lan->interfaces = get_lan_interfaces(instance);

    lan->admin_status = (get_lan_state(instance) && (lan->interfaces != NULL || strcmp(lan->ip_address, NOT_ASSIGNED_STRING) != 0));

    if (!lan->admin_status)
    {
        lan->oper_status = false;
    }

    lan->description = get_lan_description(instance);

}

static void output_syntax(char const * const progname)
{
    fprintf(stderr, "\rUsage: %s [-i <instance>] [-f <format>]\n", progname);
}

static void free_lan_status(lan_status_t * const lan)
{
    show_lan_free((void *)lan->interfaces);
    show_lan_free((void *)lan->description);
    show_lan_free((void *)lan->mac_address);
}


static void add_lan_json_information_element(tlr_show_object_t * const show_obj, lan_status_t * const lan, config_state_lan_t const info_element)
{
    void const * value = NULL;
    config_state_lan_admin_status_t      admin_state;
    config_state_lan_oper_status_t       oper_state;
    char const empty_space[] = "";
    uint64_t  const   zero64 = 0;

    switch (info_element)
    {
        case config_state_lan_admin_status:
            admin_state = (lan->admin_status) ?  config_state_lan_admin_status_up : config_state_lan_admin_status_down;
            value = &admin_state;
            break;
        case config_state_lan_oper_status:
            oper_state = (lan->oper_status) ?  config_state_lan_oper_status_up : config_state_lan_oper_status_down;
            value = &oper_state;
            break;
        case config_state_lan_description:
            value = lan->description != NULL ? lan->description : empty_space;
            break;
        case config_state_lan_interfaces:
            value = lan->interfaces != NULL ? lan->interfaces : empty_space;
            break;
        case config_state_lan_mtu:
            value = &lan->mtu;
            break;
        case config_state_lan_ip_address:
            value = isdigit(*lan->ip_address) ? lan->ip_address : empty_space;
            break;
        case config_state_lan_mask:
            value = isdigit(*lan->mask) ? lan->mask : empty_space;
            break;
        case config_state_lan_rx_bytes:
            value = (lan->admin_status) ? &lan->rx_bytes : &zero64;
            break;
        case config_state_lan_rx_packets:
            value = (lan->admin_status) ? &lan->rx_packets : &zero64;
            break;
        case config_state_lan_tx_bytes:
            value = (lan->admin_status) ? &lan->tx_bytes : &zero64;
            break;
        case config_state_lan_tx_packets:
            value = (lan->admin_status) ? &lan->tx_packets : &zero64;
            break;
    }
    tlr_show_object_add_element(show_obj, info_element, value);
}



static void add_lan_json(tlr_show_object_t * const show_obj, lan_status_t * const lan, int const instance)
{
    int     element_idx;
    config_mgr_group_t const * const group_ptr = tlr_get_group_ptr(config_state_lan, instance, config_mgr_get_group);

    if (group_ptr != NULL)
    {
        for (element_idx = 0; element_idx < group_ptr->elements.count; element_idx++)
        {
            config_mgr_element_t * const element_ptr = &group_ptr->elements.element[element_idx];
            if (element_ptr->access != config_mgr_access_no_access)
            {
                add_lan_json_information_element(show_obj, lan, element_ptr->id);
            }
        }
    }
}

static void output_lan_json(lan_status_t * const lan, int const instance)
{
    tlr_show_object_t * const show_obj = tlr_show_object_create(config_state_lan, instance);

    if (show_obj == NULL)
    {
        goto done;
    }

    add_lan_json(show_obj, lan, instance);
    printf("%s\n", tlr_show_object_to_json_string(show_obj));

done:
    if (show_obj != NULL)
    {
        tlr_show_object_destroy(show_obj);
    }
}

static void output_summary(lan_status_t * const lan, int const instance, tlr_response_format_t format)
{
    switch (format)
    {
        case tlr_response_format_json:
            break;
        case tlr_response_format_ascii:
            output_summary_ascii(lan);
            break;
        case tlr_response_format_unknown:
            break;
    }
}

static void output_extensive(lan_status_t * const lan, int const instance, tlr_response_format_t format)
{
    switch (format)
    {
        case tlr_response_format_json:
            output_lan_json(lan, instance);
            break;
        case tlr_response_format_ascii:
            output_ascii(lan);
            break;
        case tlr_response_format_unknown:
            break;
    }
}

int show_lan_work(int argc, char * argv[])
{
    int result = EXIT_FAILURE;
    int instance = -1;
    tlr_response_format_t format;
    lan_status_t lan_status = { 0 };
    char * progname;
    size_t max_instance;
    size_t i;
    bool summary = false;

    progname = basename(argv[0]);

    openlog(progname, 0, LOG_USER);

    max_instance = tlr_get_max_instance(config_state_lan, config_mgr_table_type_show);
    if (max_instance <= 0)
    {
        fprintf(stderr, "LAN is not supported.");
        goto ret;
    }

    if (parse_show_options(argc, argv, &format, &instance, 0, max_instance) != 0)
    {
        output_syntax(progname);
        goto ret;
    }

    if ((instance <= 0) || (instance > max_instance))
    {
        if (format == tlr_response_format_json)
        {
            // We won't be doing any summaries for Json format...
            // the receiver of the json data can decide to summarise or not
            // Instance must be specified correctly for json format.
            goto ret;
        }
        summary = true;
    }

    if (summary)
    {
        // When doing a summary we go through every instance.
        for (i = 1; i <= max_instance; i++)
        {
            get_lan_status(i, &lan_status);
            output_summary(&lan_status, instance, format);
            free_lan_status(&lan_status);
        }
        fprintf(stdout, "\r\n");
    }
    else
    {
        get_lan_status(instance, &lan_status);
        output_extensive(&lan_status, instance, format);
        free_lan_status(&lan_status);
        fprintf(stdout, "\r\n");
    }

    result = EXIT_SUCCESS;

ret:
    return result;
}



