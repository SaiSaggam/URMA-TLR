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

#include "utils.h"
#include "tlr_print.h"
#include "tlr_common.h"
#include <get_field.h>
#include <stdlib.h>
#include <string.h>

static char const * mask_lookup_table[33] =
{
    "0.0.0.0",
    "128.0.0.0",
    "192.0.0.0",
    "224.0.0.0",
    "240.0.0.0",
    "248.0.0.0",
    "252.0.0.0",
    "254.0.0.0",
    "255.0.0.0",
    "255.128.0.0",
    "255.192.0.0",
    "255.224.0.0",
    "255.240.0.0",
    "255.248.0.0",
    "255.252.0.0",
    "255.254.0.0",
    "255.255.0.0",
    "255.255.128.0",
    "255.255.192.0",
    "255.255.224.0",
    "255.255.240.0",
    "255.255.248.0",
    "255.255.252.0",
    "255.255.254.0",
    "255.255.255.0",
    "255.255.255.128",
    "255.255.255.192",
    "255.255.255.224",
    "255.255.255.240",
    "255.255.255.248",
    "255.255.255.252",
    "255.255.255.254",
    "255.255.255.255"
};
#define NUM_CIDR_MASKS ARRAY_SIZE(mask_lookup_table)
#define MINUTES_TO_SECONDS(minutes) ((minutes)*60)
#define HOURS_TO_SECONDS(hours) ((hours)*60*60)
#define DAYS_TO_SECONDS(days) ((days)*24*60*60)

static char const * bits_to_mask(size_t bits)
{
    char const * mask;

    if (bits >= NUM_CIDR_MASKS)
    {
        mask = "0.0.0.0";
    }
    else
    {
        mask = mask_lookup_table[bits];
    }

    return mask;
}

uint32_t parse_ipsec_uptime_seconds(char const * const original_uptime_str)
{
    /* Takes a string of the form
     * "<s> seconds"
     *  or
     * "<m> minutes"
     *  or
     * "<h> hours"
     *  or
     * "<d> days"
     *
     * and converts this into the the number of seconds represents by that amount of time.
     * e.g.
     * "2 hours" will return 7200.
     */
    uint32_t uptime_value;
    uint32_t uptime_seconds;
    char * uptime_str = NULL;
    char * uptime_field;
    char * units_field;    /* input string may specify seconds, minutes, hours, days */

    /* get_field modifies the input string (FIXME), so we take a copy of the original string */
    uptime_str = strdup(original_uptime_str);
    if (uptime_str == NULL)
    {
        uptime_seconds = 0;
        goto done;
    }
    uptime_field = get_field(uptime_str, " ", 1);
    if (uptime_field == NULL)
    {
        uptime_seconds = 0;
        goto done;
    }
    uptime_value = strtoul(uptime_field, NULL, 0);
    free(uptime_str);

    /* we now need to get the units */
    uptime_str = strdup(original_uptime_str);
    if (uptime_str == NULL)
    {
        uptime_seconds = 0;
        goto done;
    }
    units_field = get_field(uptime_str, " ", 2);
    if (units_field == NULL)
    {
        uptime_seconds = 0;
        goto done;
    }
    if (strcmp(units_field, "seconds") == 0)
    {
        uptime_seconds = uptime_value;
    }
    else if (strcmp(units_field, "minutes") == 0)
    {
        uptime_seconds = MINUTES_TO_SECONDS(uptime_value);
    }
    else if (strcmp(units_field, "hours") == 0)
    {
        uptime_seconds = HOURS_TO_SECONDS(uptime_value);
    }
    else if (strcmp(units_field, "days") == 0)
    {
        uptime_seconds = DAYS_TO_SECONDS(uptime_value);
    }
    else
    {
        tlr_debug("show_ipsec got unsupported uptime units '%s'\n", units_field);
        uptime_seconds = 0;
    }

done:
    free(uptime_str);

    return uptime_seconds;
}

char * parse_network(char const * const cidr_network)
{
    /*
     * Takes a string of the form aa.bb.cc.dd/xx and returns another string of the form aa.bb.cc.dd.
     * Note that the caller must free the returned string.
     */
    char * ip;
    char const delim = '/';
    char * cidr_network_copy = strdup(cidr_network);
    char * pslash;
    char * result;

    if (cidr_network_copy == NULL)
    {
        result = NULL;
        goto done;
    }

    pslash = strchr(cidr_network_copy, delim);
    if (pslash == NULL) /* may legitimately happen if the tunnel is down */
    {
        ip = "0.0.0.0";
    }
    else
    {
        *pslash = '\0';
        ip = cidr_network_copy;
    }

    result = strdup(ip);
done:

    free(cidr_network_copy);
    return result;
}

char * parse_netmask(char const * const cidr_network)
{
    /* Takes a string of the form aa.bb.cc.dd/xx and returns a string of the form ee.ff.gg.hh
     * where ee.ff.gg.hh is obtained by using xx as the length of a mask, so xx == 1 will return
     * "128.0.0.0" and xx == 2 will return "192.0.0.0" etc. Note that the caller must free the
     * returned string.
     */
    size_t mask_bits;
    char const delim = '/';
    char * pslash;
    char * result;

    pslash = strchr(cidr_network, delim);
    if (pslash == NULL) /* may legitimately happen if the tunnel is down */
    {
        mask_bits = 0;
    }
    else
    {
        mask_bits = strtod(pslash + 1, NULL);
    }
    result = strdup(bits_to_mask(mask_bits));

    return result;
}


