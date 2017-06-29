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
#include "apply_lan.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include "tlr_print.h"
#include <stdarg.h>

#include <messages.h>
#include "digiutils.h"
#include "config_def.h"
#include "config_status.h"
#include "tlr_common.h"

#include "unittest_support.h"

static char * process_name;

STATIC_FUNCTION void send_update_messages(size_t const instance)
{
    char * msg = NULL;

    char const *  ifname = interface_get_name_from_type(bridge_interface);
    bool send_error = false;

    if (ifname == NULL) goto error;

    if (asprintf(&msg, "%s %zu", ifname, instance) < 0)
    {
        goto error;
    }

    if (send_message(interface_manager, 1, update_message, msg, NULL))
    {
        send_error = true;
        tlr_event_log(tlr_log_error, process_name, "send_message() to interface_manager failed");
    }


    if (send_message(bridge_monitor, instance, update_message, NULL, NULL))
    {
        send_error = true;
        tlr_event_log(tlr_log_error, process_name, "send_message() to bridge_monitor failed");
    }

    if (send_error)
        fprintf(stderr, "Unable to start bridging\n");

error:
    free(msg);
    return;
}

STATIC_FUNCTION config_status_t get_lan_current_config(size_t const instance, lan_info_t * const pinfo)
{

    config_status_t status;

    {
        char * interfaces = NULL;
        config_mgr_enum_multi_value_t * const multi_list = &pinfo->interface_list;

        status = tlr_get_element_alloc_value(config_lan, instance, config_lan_interfaces,
                                    (void **)&interfaces, config_mgr_element_type_string, config_mgr_access_level_read_only);

        if (status != config_status_success) goto done;

        status =  tlr_config_string_to_value(config_lan, config_lan_interfaces, interfaces, multi_list);

        free(interfaces);

        if (status != config_status_success) goto done;

    }


    {
        unsigned int * state = NULL;

        status = tlr_get_element_alloc_value(config_lan, instance, config_lan_state, (void **)&state,
                                         config_mgr_element_type_on_off, config_mgr_access_level_read_only);

        if (status != config_status_success) goto done;

        pinfo->state = *state;

        free(state);
    }

done:
    return status;
}



STATIC_FUNCTION bool lan_is_in_interface_list (config_lan_interfaces_t const lanint, config_mgr_enum_multi_value_t  const * const lan_list)
{
    // Determine if the given LAN interface is in the given list of LAN interfaces
    size_t     i;
    bool        result = false;

    if (lan_list == NULL)
        goto done;

    for (i = 0; i < lan_list->count; i++)
    {
        if (lanint == lan_list->value[i])
        {
            result = true;
            goto done;
        }
    }
done:
    return result;
}


STATIC_FUNCTION bool is_interface_listed_in_lan_config(size_t const lan_instance, config_mgr_enum_multi_value_t  const * const lan_list)
{
    config_status_t status;
    unsigned int    * state = NULL;
    config_mgr_enum_multi_value_t   * multi_list = NULL;
    bool    result = false;

    status = tlr_get_element_alloc_value(config_lan, lan_instance, config_lan_interfaces,
                                         (void **)&multi_list, config_mgr_element_type_enum_multi, config_mgr_access_level_read_only);
    if (status != config_status_success)
        goto done;

    status = tlr_get_element_alloc_value(config_lan, lan_instance, config_lan_state, (void **)&state,
                                         config_mgr_element_type_on_off, config_mgr_access_level_read_only);
    if (status != config_status_success)
        goto done;

    if (*state != 0)
    {
        size_t  i;

        for (i = 0; i < lan_list->count; i++)
        {
            if (lan_is_in_interface_list(lan_list->value[i], multi_list) == true)
            {
                result = true;
                goto done;
            }
        }
    }

done:
    free(state);
    free(multi_list);
    return result;
}


STATIC_FUNCTION bool lan_interface_check(size_t const instance, config_mgr_enum_multi_value_t  const * const lan_list, unsigned int const lan_state)
{
    // Check all interfaces in this LAN do NOT exist in any other LAN.
    // Returns true if we are all ok (i.e no interface in any other LAN).
    config_mgr_group_table_t const * const table_ptr =  tlr_get_table(config_mgr_table_type_config);
    config_mgr_group_t * const group_ptr = &table_ptr->groups[config_lan];
    size_t lan_instance;


    bool    result = true;

    if (lan_state == 0)     // if LAN is turned off ... you can have whatever you like.
        goto done;


    for (lan_instance = 1; lan_instance <= group_ptr->instances; lan_instance++)
    {
        if (lan_instance == instance)
            continue;


        if (is_interface_listed_in_lan_config(lan_instance, lan_list) == true)
        {
            result = false;
            goto done;
        }

    }
done:
    return result;
}


// TODO: this (or something like it) should be in the tlr library...move it/rename it when working.
// Returns a string that needs to be freed afterwards..
char * multi_enum_value_to_string(config_mgr_enum_multi_value_t * const multi_list)
{

    char    * str = NULL;

    if (multi_list == NULL || multi_list->count == 0)
    {
        char const empty_string[] = "";
        if (asprintf(&str, "%s", empty_string) < 0)
        {
            goto done;
        }
    }
    else
    {
        config_mgr_group_table_t const * const table_ptr =  tlr_get_table(config_mgr_table_type_config);
        config_mgr_group_t * const group_ptr = &table_ptr->groups[config_lan];
        config_mgr_element_t * const ele_ptr = &group_ptr->elements.element[config_lan_interfaces];
        config_mgr_element_enum_limit_t  * const limit_ptr = (config_mgr_element_enum_limit_t  *)ele_ptr->value_limit;

        int     enumidx;

        for (enumidx = 0; enumidx < multi_list->count; enumidx++)
        {
            char    * newstr=NULL;

            {
                config_mgr_element_enum_t const * ele = &limit_ptr->value[multi_list->value[enumidx]];

                if (enumidx == 0)
                {
                    if (asprintf(&newstr, "%s", ele->name) < 0)
                        goto done;
                }
                else
                {
                    if (asprintf(&newstr, "%s,%s", str, ele->name) < 0)
                        goto done;
                }
                free(str);
                str = newstr;
            }
        }
    }
done:
    return str;
}


STATIC_FUNCTION config_status_t revert_lan_config(size_t const instance, lan_info_t * const pinfo)
{
    // Stick the LAN config back to previous config givin in 'pinfo'
    char        * interfaces;
    config_status_t status = config_status_success;
    tlr_handle_t tlr_handle = NULL;

    // cannot use tlr_config_write_native() for config_mgr_element_type_enum_multi types unfortunately.
    interfaces = multi_enum_value_to_string(&pinfo->interface_list);
    if (interfaces != NULL)
    {
        status = tlr_set_element_value(config_lan, instance, config_lan_interfaces,
                              interfaces, config_mgr_access_level_read_write);
        free(interfaces);

        if (status != config_status_success)
        {
            goto done;
        }
    }

    tlr_handle = tlr_config_open(config_lan, instance, config_mgr_access_level_super);
    if (tlr_handle == NULL)
    {
        tlr_debug("tlr_config_open failed\n");
        status = config_status_group_not_found;
        goto done;
    }
    status = tlr_config_write_native(tlr_handle, config_lan_state, &pinfo->state);

done:
    if (tlr_handle != NULL)
        tlr_config_close(tlr_handle);

    return status;
}

STATIC_FUNCTION int apply_lan_config_change(size_t const instance)
{
    int     result = EXIT_FAILURE;


    config_mgr_element_enum_limit_t const * const interfaces_limit = tlr_get_element_enum_limit(config_mgr_table_type_config,
                                                                                        config_lan,
                                                                                        config_lan_interfaces);
    lan_info_t * current_config;
    uint8_t config_buffer[sizeof *current_config + (sizeof(unsigned int) * interfaces_limit->count)];
    config_status_t status;


    current_config = (lan_info_t *)config_buffer;

    current_config->interface_list.count = interfaces_limit->count;
    status =  get_lan_current_config(instance, current_config);
    if (status != config_status_success)
    {
        tlr_error("failed to get current lan config %d\n", status);
        goto done;
    }

    if (lan_interface_check(instance, &current_config->interface_list, current_config->state) == true)
    {
        status = tlr_set_last_config(config_lan, instance, current_config, sizeof config_buffer);
        if (status != config_status_success)
        {
            tlr_error("failed to set last config\n");
        }
        result = EXIT_SUCCESS;
    }
    else
    {
        lan_info_t * const last_config = (lan_info_t *)config_buffer;

        status = tlr_get_last_config(config_lan, instance, last_config, sizeof config_buffer);
        if (status != config_status_success)
        {
            last_config->interface_list.count = 0;
        }

        revert_lan_config(instance, last_config);
        fprintf(stderr, "Interfaces conflict\n");

    }


done:
    return result;
}

#if !defined(_CUNIT_)

int main(int argc, char *argv[])
{
    int result;
    size_t instance = 0;
    int opt;

    process_name = basename(argv[0]);

    opterr = 0;
    openlog(process_name, 0, LOG_USER);

    while ((opt = getopt(argc, argv, "i:u:a:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            instance = strtol(optarg, NULL, 10);
            break;

        default:
            break;
        }
    }

    result = apply_lan_config_change(instance);
    if (result == EXIT_SUCCESS)
    {
        send_update_messages(instance);     // Send our update message to the monitors.
    }

    closelog();


    return result;
}
#endif

