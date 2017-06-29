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

#include "count_sas.h"
#include "debug.h"
#include "vpn_monitor_defs.h"
#include "unittest_support.h"
#include <tlr_common.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/*
 * count_installed_sas:
 * count the current number of installed SAs for the specified tunnel
 * return: -1 on error
 * else the number of installed SAs
 */

STATIC_FUNCTION int count_sas_from_ipsec_status_output(FILE * const fp)
{
    int sa_count;
    char * line = NULL;
    size_t len = 0;
    char const search_string[] = "INSTALLED";

    sa_count = 0;
    while (getline(&line, &len, fp) != -1)
    {
        DEBUG("line: '%s'", line);
        if (strstr(line, search_string) != NULL)
        {
            sa_count++;
        }
    }

    free(line);
    return sa_count;
}

int count_installed_sas(char const * const tunnel_name)
{
    /* execute "ipsec status <tunnel name>" and count up the number of lines with "INSTALLED" in
     * them. It looks like there is one line like this for each set of installed SAs
     */
    int ret;
    char * cmd = NULL;
    FILE * fp = NULL;

    if (asprintf(&cmd, "%s status %s", IPSEC_APPLICATION_NAME, tunnel_name) < 0)
    {
        ret = -1;
        goto error;
    }
    DEBUG("cmd: '%s'\n", cmd);
    fp = popen(cmd, "r");
    if (fp == NULL)
    {
        ret = -1;
        goto error;
    }

    ret = count_sas_from_ipsec_status_output(fp);
    DEBUG("SA count for %s: %d\n", tunnel_name, ret);

error:
    free(cmd);
    if (fp != NULL)
    {
        pclose(fp);
    }

    return ret;
}
