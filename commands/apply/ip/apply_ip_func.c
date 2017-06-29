/*****************************************************************************
 * Copyright (c) 2015 Digi International Inc., All Rights Reserved
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

#include <string.h>

#include "apply_ip.h"
#include "digiutils.h"
#include "config_def.h"
#include "config_mgr.h"
#include "tlr_print.h"

#include "unittest_support.h"

/********************
	Functions
********************/

STATIC_FUNCTION int ip_read_hostname(size_t const instance, char * const hostname, size_t const length)
{
    int ret = -1;

    config_status_t const status = tlr_get_element_value(config_ip, instance, config_ip_hostname,
                                                        hostname, length,
                                                        config_mgr_element_type_string, config_mgr_access_level_read_only);

    if (status != config_status_success)
        goto done;

    ret = 0;

done:
    return ret;
}

STATIC_FUNCTION int generate_hosts_file(char * const hostname)
{
    int ret = EXIT_FAILURE;
    FILE *fd = NULL;

    /* Create new hosts file */
    fd = fopen(SYSTEM_HOSTS_FILENAME, "w");
    if (fd == NULL)
    {
        tlr_error("cannot create hosts file");
        goto done;
    }

    if (fprintf(fd, SYSTEM_HOSTS_FILE_CONTENT, hostname) <= 0)
    {
        tlr_error("couldn't write hosts file");
        goto done;
    }

    ret = EXIT_SUCCESS;

done:
    if (fd != NULL)
        fclose(fd);

    return ret;
}

STATIC_FUNCTION int ip_apply_hostname(char *value)
{
    int ret = EXIT_FAILURE;
    const char const *none_hostname = "(none)";

    /* Use default (none) hostname if it needs to be cleared */
    if (strlen(value) == 0)
        value = (char *)none_hostname;

    if (system_command("hostname \"%s\" >/dev/null 2>&1", value) != 0)
        goto error;

    if (generate_hosts_file(value) != EXIT_SUCCESS)
        goto error;

    ret = EXIT_SUCCESS;

error:
    return ret;
}


int apply_ip_main (int argc, char *argv[])
{

    int ret = EXIT_FAILURE;
    const char * const process_name = basename(argv[0]);

    size_t const hostname_length = tlr_get_config_value_max_length(config_mgr_table_type_config, config_ip, config_ip_hostname);
    char hostname[hostname_length];

    size_t instance;

    openlog(process_name, 0, LOG_USER);

    if (get_instance(argc, argv, &instance, config_ip))
    {
        goto error;
    }

    if (ip_read_hostname(instance, hostname, sizeof hostname) < 0)
    {
        goto error;
    }

    if (ip_apply_hostname(hostname) != EXIT_SUCCESS)
    {
        goto error;
    }

    ret = EXIT_SUCCESS;

error:
    closelog();

    return ret;
}
