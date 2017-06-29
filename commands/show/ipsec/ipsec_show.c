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

#include "show_ipsec.h"
#include "show_ipsec_ascii.h"
#include "show_ipsec_json.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>
#include <string.h>
#include <ctype.h>
#include "config_def.h"
#include "config_mgr.h"
#include "unittest_support.h"
#include "show_parse_helper.h"
#include "tlr_common.h"
#include <get_field.h>
#include <assert.h>

#define USE_CMD 1

int get_tunnel_status(tunnel_status_t * ptr_tunnel)
{
    int ret=-1;
    char * cmd=NULL;
    FILE * fp = NULL;
    extern int asprintf(char **ret, const char *format, ...);
#if USE_CMD
    if (asprintf(&cmd, "ipsec statusall %s", ptr_tunnel->name) < 0)
    {
        goto error;
    }

    fp = popen(cmd, "r");
    if (fp == NULL)
    {
        ret = -1;
        goto error;
    }
#else
    fp = fopen("/home/whagstro/ipsec.txt", "r"); // for testing
    if (fp == NULL)
    {
        ret = -1;
        goto error;
    }
#endif

    ret = read_tunnel_info_from_ipsec_status(fp, ptr_tunnel);

error:
    free(cmd);
    if (fp != NULL)
    {
        pclose(fp);
    }

    return ret;
}

// Is this tunnel enabled?
int get_admin_status(tunnel_status_t *ptr_tunnel)
{
    int result=0;

    config_status_t status;
    size_t const max_length = 10;
    char value[max_length+1];

    status = tlr_get_element_value(config_ipsec, ptr_tunnel->instance, config_ipsec_state, value,
                                   sizeof value, config_mgr_element_type_string, config_mgr_access_level_read_only);
    if (status != config_status_success)
    {
        goto error;
    }

    status = tlr_config_string_to_value(config_ipsec, config_ipsec_state, value, &ptr_tunnel->is_tunnel_enabled);

error:
    result = (status == config_status_success) ? 0 : -1;

    return result;
}

static char * get_ipsec_description(size_t instance)
{
    char * description;
    config_status_t status;

    status = tlr_get_element_alloc_value(config_ipsec, instance, config_ipsec_description, (void * *)&description,
                                   config_mgr_element_type_string, config_mgr_access_level_read_only);
    if (status != config_status_success)
    {
        description = NULL;
    }

    return description;
}

void tunnel_status_context_init(tunnel_status_t * ptr_tunnel, size_t instance)
{
    memset(ptr_tunnel, 0, sizeof *ptr_tunnel);
    ptr_tunnel->instance = instance;
    snprintf(ptr_tunnel->name, sizeof(ptr_tunnel->name), "ipsec%zd", instance);
}

void tunnel_status_context_cleanup(tunnel_status_t * ptr_tunnel)
{
    free(ptr_tunnel->description);
    ptr_tunnel->description = NULL;
}

bool read_all_tunnel_status_data(tunnel_status_t * ptr_tunnel)
{
    bool result;

    ptr_tunnel->description = get_ipsec_description(ptr_tunnel->instance);
    if (ptr_tunnel->description == NULL)
    {
        result = false;
        goto error;
    }

    if (get_admin_status(ptr_tunnel) < 0)
    {
        result = false;
        goto error;
    }

    if (get_tunnel_status(ptr_tunnel) < 0)
    {
        result = false;
        goto error;
    }

    result = true;

error:
    return result;
}

int show_ipsec_main(int argc, char ** argv)
{
    int result = EXIT_FAILURE;
    char * const progname = basename(argv[0]);
    tlr_response_format_t format;
    int instance;
    int num_ipsec_instances;

    num_ipsec_instances = tlr_get_max_instance(config_state_ipsec, config_mgr_table_type_show);

    if (parse_show_options(argc, argv, &format, &instance, 0, num_ipsec_instances) != 0)
    {
        usage_show(progname);
        goto error;
    }

    switch (format)
    {
        case tlr_response_format_ascii:
            show_ipsec_ascii(instance, num_ipsec_instances);
            break;
        case tlr_response_format_json:
            if (!show_ipsec_json(instance))
            {
                goto error;
            }
            break;
        case tlr_response_format_unknown:
            goto error;
    }

    result = EXIT_SUCCESS;

error:
    return result;
}
