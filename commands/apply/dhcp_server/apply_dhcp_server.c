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
#include "tlr_backup.h"
#include "tlr_print.h"
#include "digiutils.h"
#include "config_mgr.h"
#include "config_status.h"
#include "tlr_common.h"

#include "unittest_support.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <stdarg.h>
#include <termios.h>
#include <sys/stat.h>

static bool check_minimum_configuration(unsigned int const * const state, char const * const ip_address_start, char const * const ip_address_end)
{
    bool config_valid = false;

    if (state == NULL || ip_address_start == NULL || ip_address_end == NULL)
    {
        goto done;
    }

    if (*state == 1 && *ip_address_start != '\0' && *ip_address_end != '\0')
    {
        /* The minimum configuration is having start and end ip addresses */
        config_valid = true;
    }

done:
    return config_valid;
}

STATIC_FUNCTION int apply_dhcp_server(size_t const instance)
{
    size_t const group_id = config_dhcp_server;
    int     result = EXIT_FAILURE;
    unsigned int    * state = NULL;
    char * ip_address_start = NULL;
    char * ip_address_end = NULL;
    config_status_t status;
    bool config_valid = false;

    config_mgr_group_t const * const group = tlr_get_group_ptr(group_id, instance,
                                                            config_mgr_set_group);
    if (group == NULL)
    {
        goto done;
    }

    status = tlr_get_element_alloc_value(config_dhcp_server, instance, config_dhcp_server_state, (void **)&state,
                                         config_mgr_element_type_on_off, config_mgr_access_level_read_only);
    if (status != config_status_success)
    {
        goto done;
    }

    status = tlr_get_element_alloc_value(config_dhcp_server, instance, config_dhcp_server_ip_address_start, (void **)&ip_address_start,
                                         config_mgr_element_type_ipaddr, config_mgr_access_level_read_only);
    if (status != config_status_success)
    {
        goto done;
    }

    status = tlr_get_element_alloc_value(config_dhcp_server, instance, config_dhcp_server_ip_address_end, (void **)&ip_address_end,
                                         config_mgr_element_type_ipaddr, config_mgr_access_level_read_only);
    if (status != config_status_success)
    {
        goto done;
    }

    /* The minimum configuration is having start and end ip addresses */
    config_valid = check_minimum_configuration(state , ip_address_start, ip_address_end);

    if (*state == 0 || config_valid)
    {
        if (system_command("/usr/etc/init.d/init_dnsmasq.sh restart >/dev/null 2>&1") != 0)
        {
            tlr_error("Unable to apply %s %zd configuration\n", group->name, instance);
        }

    }
    else
    {
        if (*state == 1)
        {
            tlr_error("Not enough information, unable to apply %s %zd configuration\n", group->name, instance);
        }
    }

    result = EXIT_SUCCESS;

done:
    free(state);
    free(ip_address_start);
    free(ip_address_end);
    return result;
}

#if !defined(_CUNIT_)

int main(int argc, char *argv[])
{
    int result = EXIT_FAILURE;

    size_t instance = 0;
    char * progname;
     int max_instance = tlr_get_max_instance(config_dhcp_server, config_mgr_table_type_config);

     openlog("apply_dhcp_server", 0, LOG_USER);

     if (max_instance <= 0)
     {
         fprintf(stderr, "Unsupported command\n");
         goto done;
     }

     progname = basename(argv[0]);


    if (get_instance(argc, argv, &instance, config_dhcp_server))
    {
        fprintf(stderr, "Usage: %s -i [1 - %zd]\n", progname, max_instance);
        goto done;
    }

    result = apply_dhcp_server(instance);

done:
    closelog();
    return result;
}
#endif

