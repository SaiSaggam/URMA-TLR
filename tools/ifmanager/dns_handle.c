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
#include <event_manager.h>
#include <messages.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <tlr_print.h>
#include <linux/limits.h>

#include "unittest_support.h"
#include "digiutils.h"
#include "dns_handle.h"
#include "dns_handle_private.h"



typedef struct
{
    int     metric;
    char    * ifname;
}   t_route;

STATIC_FUNCTION void trim_trailing_white(char * const str)
{
    char * end;

    if (str != NULL)
    {
        end = str + strlen(str) - 1;
        while (end >= str && isspace(*end))
            end--;
        *(end + 1) = '\0';
    }
}

STATIC_FUNCTION bool extract_metric_and_iface(char * *saved_strok_context, int * const pmetric, char * * const ifname)
{
    char    * nextword;
    const char s[2] = " ";
    int     idx;
    bool    got_metric = false;
    bool    got_iface = false;

    for (idx = 0; idx < 7; idx++)
    {
        nextword = strtok_r(NULL, s, saved_strok_context);
        if (nextword != NULL)
        {
            if (idx == 3)
            {
                if (sscanf(nextword, "%d", pmetric) == 1)
                {
                    got_metric = true;
                }
            }
            else if (idx == 6)
            {
                if (got_metric)
                {
                    *ifname = strdup(nextword);
                    if (*ifname != NULL)
                    {
                        trim_trailing_white(*ifname);
                        got_iface = true;
                    }
                }
            }
        }
    }
    return got_metric && got_iface;
}


// Determine if this is a route worth considering by looking at the first word.
STATIC_FUNCTION bool word_matches_route_type(char const * const word, ROUTE_TYPE route_type)
{
    int     scanned;
    int     dec1;
    int     dec2;
    int     dec3;
    int     dec4;
    bool    result = false;

    scanned = sscanf(word, "%d.%d.%d.%d", &dec1, &dec2, &dec3, &dec4);
    if (scanned == 4)
    {
        switch (route_type)
        {
            case DEFAULT_ROUTE:
                if (dec1 == 0 && dec2 == 0 && dec3 == 0 && dec4 == 0)
                {
                    result = true;
                }
                break;
            case ANYTHING_ROUTE:
                result = true;
                break;
        }
    }
    return result;
}

STATIC_FUNCTION bool get_interface_and_metric_from_line(char * const line, ROUTE_TYPE route_type, char * * const ifname, int * metric)
{
    bool got_details;
    char * save;
    char * firstword;
    const char s[2] = " ";

    firstword = strtok_r(line, s, &save);
    if (firstword != NULL && word_matches_route_type(firstword, route_type))
    {
        // Got a route. Pull out the metric and interface name.
        if (extract_metric_and_iface(&save, metric, ifname))
        {
            got_details = true;
            goto done;
        }
    }
    got_details = false;

done:
    return got_details;
}

// Look for the next route line and get the iface and metric from it.
// Return true if found something.
STATIC_FUNCTION bool get_next_route(FILE * file, t_route * p_route, ROUTE_TYPE route_type)
{
    char * line = NULL;
    size_t len = 0;
    ssize_t readcount;
    bool found_a_route = false;

    while (!found_a_route && (readcount = getline(&line, &len, file)) != -1)
    {
        found_a_route = get_interface_and_metric_from_line(line, route_type, &p_route->ifname, &p_route->metric);
    }

    free(line);
    return found_a_route;
}

// Returns the best route and returns the interface name, which indicates which is the currently active WAN.
// Returns NULL if couldn't find one.
// The string returned should be eventually be freed.
STATIC_FUNCTION char * find_best_iface(ROUTE_TYPE route_type)
{
    FILE * route_process = NULL;
    char * best_iface = NULL;
    int best_metric = 1000000;
    t_route  route;

    route_process = popen("route -n", "r");
    if (route_process == NULL)
    {
        tlr_debug("unable to open subprocess");
        goto ERROR;
    }
    while (get_next_route(route_process, &route, route_type))
    {
        if (route.metric < best_metric)
        {
            // Got a new "best"
            best_metric = route.metric;
            free(best_iface);
            best_iface = route.ifname;
            route.ifname = NULL;
        }
        else
        {
            free(route.ifname);
        }
    }

    pclose(route_process);
ERROR:
    return best_iface;
}



// Create the full file & pathname for the resolv.conf for the given interface
// Returns malloced memory that should be freed.
STATIC_FUNCTION char * create_var_dns_fname(char const * const ifname)
{
    const char * const ram_dns_root_directory = "/ram/var/dns";
    char    * pathname = NULL;

    if (asprintf(&pathname, "%s/%s/resolv.conf", ram_dns_root_directory, ifname) == -1)
    {
        goto ERROR;
    }
    return pathname;
ERROR:
    free(pathname);
    return NULL;
}


STATIC_FUNCTION int create_soft_link_to_resolv(char const * const ifresolv, char const * const globalresolv)
{
    struct stat   buffer;
    int     ret;

    if (lstat(globalresolv, &buffer) == 0)
    {
        char buf[PATH_MAX];
        ssize_t len;

        if ((len = readlink(globalresolv, buf, sizeof(buf) - 1)) != -1)
        {
            buf[len] = '\0';
            if (strcmp(buf, ifresolv) == 0)
            {
                // all good.. already what we want.
                ret = 0;
                goto done;
            }
        }
        system_command("rm -r %s", globalresolv);
    }
    if (stat(ifresolv, &buffer) == 0)
    {
        ret = system_command("ln -s %s %s", ifresolv, globalresolv);
    }
    else
    {
        tlr_debug("DNS: error %s not present/n.", ifresolv);
        ret = -1;
    }
done:
    return ret;
}


// An interface has just become the current active WAN, apply any dns updates by pointing to the correct /var/dns/xxx/resolve.conf file
int create_dns_link()
{
    int ret = -1;
    char       * var_dns_fname = NULL;
    char  const * const      etc_resolv_path = "/ram/var/dns/resolv.conf";
    char         * best_iface = NULL;

    if ((best_iface = find_best_iface(DEFAULT_ROUTE)) == NULL)
    {
        // Couldn't find a default route.. try and find any old route.
        if ((best_iface = find_best_iface(ANYTHING_ROUTE)) == NULL)
        {
            tlr_debug("DNS:no routes.\n");
            goto error;
        }
    }
    if ((var_dns_fname = create_var_dns_fname(best_iface)) == NULL)
    {
        goto error;
    }
    if (create_soft_link_to_resolv(var_dns_fname, etc_resolv_path) < 0)
    {
        tlr_debug("cannot create %s/n.", etc_resolv_path);
        goto error;
    }
    ret = 0;
error:
    free(var_dns_fname);
    free(best_iface);
    return ret;
}



