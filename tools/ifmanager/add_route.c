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

#include <event_manager.h>
#include <messages.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/wait.h>
#include "config_status.h"
#include "config_mgr.h"
#include "digiutils.h"
#include "unittest_support.h"
#include "route.h"
#include "getlinefd.h"
#include "tlr_common.h"
#include "dns_handle.h"

// Enums used for configuration items
typedef enum
{
    defroute_gateway,
    defroute_interface,
    defroute_metric,
    defroute_count
} defroute_t;

typedef enum {
    route_destination,
    route_mask,
    route_gateway,
    route_metric,
    route_interface,
    route_count
} route_t;

#define VAL_LEN 128
#define CMD_LEN 128

STATIC_FUNCTION bool is_interface_up(char * const ifname)
{
    FILE *fd=NULL;
    bool ret=false;
    char cmd[CMD_LEN];

    snprintf(cmd, sizeof cmd, "ifconfig | grep %s", ifname);
    //snprintf(cmd, sizeof cmd, "ip link ls dev %s", ifname);
    fd = popen(cmd, "r");
    if (fd != NULL)
    {
        char *line = NULL;
        size_t len=0;

        //snprintf(cmd, sizeof cmd, "%s:", ifname);

        while (getline(&line, &len, fd) != -1)
        {
            if (strstr(line, ifname) != NULL)
            {
                ret = true;
                break;
            }
        }

        free(line);
        pclose(fd);
    }
    tlr_debug("is_interface_up %d [%s]\n", ret, cmd);
    return ret;
}

// Execute the route command and check for errors
execute_route_return_t ifm_execute_route_command(char * const cmd)
{
    FILE *fd=NULL;
    execute_route_return_t ret= route_add_error;
    int exit_status, wstat;

    tlr_debug("executing command %s", cmd);
    fd = popen(cmd, "r");
    if (fd != NULL)
    {
        size_t len=0;
        char *line=NULL;

        // On success the route command returns nothing
        if (getline(&line, &len, fd) != -1)
        {
            tlr_debug("cmd failed [%s]\n", line);
            if (strstr(line, "File exists") != NULL)
            {
                tlr_debug("Route already exists\n");
                ret = route_already_exists;
            }
            exit_status = pclose(fd);
            goto error;
        }

        exit_status = pclose(fd);
        wstat = WEXITSTATUS(exit_status);

        if ((wstat != 0) && (wstat != (128+SIGPIPE)))
        {
            tlr_debug("add route failed [%d]\n", wstat);
            goto error;
        }
        if (line)
        {
            free(line);
        }
    }
    else
    {
        tlr_debug("Could not execute command %s\n", cmd);
    }


    ret = route_added_ok;
error:

    return ret;
}

STATIC_FUNCTION int route_remove(char * const command) {
    int ret = -1;

    tlr_debug("removing route [%s]", command);
    if (system_command(command) < 0)
    {
        tlr_debug("unable to remove route from route");
        goto error;
    }

    ret = 0;
error:
    return ret;
}

STATIC_FUNCTION int read_default_route_configuration(config_defroute_t * const parameters, size_t const instance, char values[][VAL_LEN])
{
    int ret = -1;
    config_group_t group = config_defroute;
    config_mgr_access_level_t const access_level = config_mgr_access_level_read_only;
    tlr_handle_t handle = tlr_config_open(group, instance, access_level);
    if (handle == NULL) {
        tlr_debug("cannot open uci file");
        goto error;
    }
    config_status_t config_status;
    for (int i = 0; i < defroute_count; i++)
    {
        config_status = tlr_config_read(handle, parameters[i], values[i], VAL_LEN * sizeof(char));
        if (config_status != config_status_success) {
            tlr_debug("cannot read uci file");
            goto error;
        }
    }
    ret = 0;
    error:
        if (handle != NULL)
            tlr_config_close(handle);
        return ret;
}

STATIC_FUNCTION int read_static_route_configuration(config_route_t * const parameters, size_t const instance, char values[][VAL_LEN])
{
    int ret = -1;
    config_group_t group = config_route;
    config_mgr_access_level_t const access_level = config_mgr_access_level_read_only;
    tlr_handle_t handle = tlr_config_open(group, instance, access_level);
    if (handle == NULL)
    {
        tlr_debug("cannot open uci file");
        goto error;
    }
    config_status_t config_status;
    for (int i = 0; i < route_count; i++)
    {
        config_status = tlr_config_read(handle, parameters[i], values[i], VAL_LEN * sizeof(char));
        if (config_status != config_status_success) {
            tlr_debug("cannot read uci file");
            goto error;
        }
    }
    ret = 0;
error:
    if (handle != NULL)
    {
        tlr_config_close(handle);
    }
    return ret;
}

// We save the 'ip route delete ...' command in a file each time a route
// is added, these routines manipulate that file
STATIC_FUNCTION int check_log_file(char * const filename)
{
    int ret = -1;
    if (access(filename, F_OK))
    {
        char command[CMD_LEN];
        snprintf(command, sizeof command, "touch %s", filename);
        if (system_command(command) < 0) {
            tlr_debug("unable to create route.log");
            goto error;
        }
    }
    ret = 0;
    error:
        return ret;
}

// Look for the given route instance in the log file.
STATIC_FUNCTION bool does_instance_exist(FILE * const file, int const instance , char *route)
{
    char text[CMD_LEN];
    char line[CMD_LEN];
    fseek(file, 0, SEEK_SET);
    while (fgets ((char *) line, sizeof line, file) != NULL)
    {
        int i = 0;
        while (line[i++] != ' ');
        strncpy(text, line, i);
        text[i] = '\0';
        int number = atoi(text);
        if (instance == number)
        {
            tlr_debug("instance_found: found %d\n", instance);
            strcpy(route, line+i);
            goto FOUND;
        }
    }
    route[0] = '\0';
    return false;
FOUND:
    return true;
}

/*
 * The routing log files have the format:
 * 1 ip route delete 10.0.0.0/255.0.0.0 via 10.52.18.1 metric 2 2>&1
 * where the 1 above is the instance followed by the command to delete the route.
 */
STATIC_FUNCTION bool get_route_for_interface(FILE * const file, char const * const ifname, char *route, int *instance)
{
    char *line = NULL;
    size_t len=0;
    bool ret=false;
    char text[CMD_LEN];

    fseek(file, 0, SEEK_SET);

    while (getline(&line, &len, file) != -1)
    {
        if (strstr(line, ifname) != NULL)
        {
            int i = 0;

            while (line[i++] != ' '); // Get the instance
            strncpy(text, line, i);
            text[i] = '\0';
            *instance = atoi(text);

            strcpy(route, &line[i]);
            tlr_debug("found route [%s] for interface [%s] instance [%d]", route, ifname, *instance);
            ret = true;
        }
    }

    free(line);
    return ret;
}

STATIC_FUNCTION int instance_remove(FILE * const file, int const instance) {
    int ret = -1;
    FILE * newfile=NULL;
    char new_filename[PATH_MAX];
    long offset;
    char text[CMD_LEN];
    char line[CMD_LEN];

    snprintf(new_filename, sizeof new_filename, "/var/log/~temp_route_file");
    newfile = fopen(new_filename, "w+");
    if (newfile == NULL)
    {
        goto error;
    }

    rewind(file);
    while (fgets (line, sizeof line, file) != NULL)
    {
        int i = 0;
        while (line[i++] != ' ');
        strncpy(text, line, i);
        text[i] = '\0';
        int number = atoi(text);
        if (instance != number)
            fputs(line, newfile);
    }
    rewind(file);
    rewind(newfile);
    while (fgets (line, sizeof line, newfile) != NULL)
    {
        fputs(line, file);
    }
    offset = ftell(file);
    if (offset >= 0)
    {
        ftruncate(fileno(file), (off_t)offset);
    }
    if (fclose(newfile))
    {
        goto error;
    }
    snprintf(line, sizeof line, "rm %s", new_filename);

    if (system_command(line) < 0)
    {
        tlr_debug("unable to remove log file");
    }

    ret = 0;
error:
    return ret;
}

STATIC_FUNCTION int save_delete_route_info(FILE * const file, int const instance, char const * const command) {
    int ret = -1;
    char line[CMD_LEN];

    if (does_instance_exist(file, instance, line))
    {
        instance_remove(file, instance);
    }

    fseek(file, 0, SEEK_END);
    snprintf(line, sizeof line, "%d %s\n", instance, command);
    tlr_debug("saving delete route information: [%s]", line);
    fputs(line, file);

    ret = 0;
    return ret;
}

// Add the default route to Linux
STATIC_FUNCTION int add_static_route(FILE * const file, char values[][VAL_LEN], size_t instance)
{
    int ret = -1;
    char command[CMD_LEN];
    execute_route_return_t result;

    bool has_interface   = (strcmp(values[route_interface], "none") == 0) ? false : true;


    if (has_interface && is_interface_up(values[route_interface]))
    {
        // the 2>&1 redirects stderr to stdout so we can catch it with popen()
        snprintf(command, sizeof command, "ip route add %s/%s via %s metric %s dev %s 2>&1",
                values[route_destination], values[route_mask], values[route_gateway],
                values[route_metric], values[route_interface]);
        tlr_event_log(tlr_log_info, "route", "Adding route [%s] for interface\n", command);

        if ((result = ifm_execute_route_command(command)) != route_added_ok)
        {
            if (result != route_already_exists)
            {
                tlr_event_log(tlr_log_info, "route", "unable to apply route");
            }
        }
        else
        {
            snprintf(command, sizeof command, "ip route delete %s/%s via %s metric %s dev %s 2>&1",
                     values[route_destination], values[route_mask], values[route_gateway],
                     values[route_metric], values[route_interface]);
            save_delete_route_info(file, instance, command);
        }
    }
    else
    {
        snprintf(command, sizeof command, "ip route add %s/%s via %s metric %s 2>&1",
                values[route_destination], values[route_mask], values[route_gateway],
                values[route_metric]);
        tlr_event_log(tlr_log_info, "route", "Adding route [%s] for interface\n", command);
        if ((result = ifm_execute_route_command(command)) != route_added_ok)
        {
            if (result != route_already_exists)
            {
                tlr_event_log(tlr_log_info, "route", "unable to apply route");
            }
        }
        else
        {
            snprintf(command, sizeof command, "ip route delete %s/%s via %s metric %s 2>&1",
                     values[route_destination], values[route_mask], values[route_gateway],
                     values[route_metric]);
            save_delete_route_info(file, instance, command);
        }
    }

    ret = 0;
    return ret;
}

// Add the default route to Linux
STATIC_FUNCTION int add_default_route(FILE * const file,  char values[][VAL_LEN], size_t instance)
{
    int ret = 0;
    char command[CMD_LEN];
    bool has_interface = (strcmp(values[defroute_interface], "none") == 0) ? false : true;
    execute_route_return_t result;

    tlr_debug("add_default_route1 interface [%s]\n", values[defroute_interface]);

    // The interface takes precedence over the gateway field.
    if (has_interface && is_interface_up(values[defroute_interface]))
    {
        snprintf(command, sizeof command, "ip route add default via 0.0.0.0 metric %s dev %s  2>&1",
                values[defroute_metric], values[defroute_interface]);
        tlr_event_log(tlr_log_info, "defroute", "Adding default route [%s]\n", command);

        if ((result = ifm_execute_route_command(command)) != route_added_ok)
        {
            if (result != route_already_exists)
            {
                tlr_event_log(tlr_log_info, "route", "unable to apply route");
            }
        }
        else
        {
            snprintf(command, sizeof command, "ip route delete default via 0.0.0.0 metric %s dev %s  2>&1",
                     values[defroute_metric], values[defroute_interface]);
            save_delete_route_info(file, instance, command);
        }
    }
    else
    {
        snprintf(command, sizeof command, "ip route add default via %s metric %s  2>&1",
                values[defroute_gateway], values[defroute_metric]);
        tlr_event_log(tlr_log_info,"defroute", "Adding default route [%s] for gateway\n", command);
        if ((result = ifm_execute_route_command(command)) != route_added_ok)
        {
            if (result != route_already_exists)
            {
                tlr_event_log(tlr_log_info, "route", "unable to apply route");
            }
        }
        else
        {
            snprintf(command, sizeof command, "ip route delete default via %s metric %s  2>&1",
                values[defroute_gateway], values[defroute_metric]);
            save_delete_route_info(file, instance, command);
        }
    }

    return ret;
}

int apply_default_route(int const instance)
{
    int ret = -1;
    config_defroute_t parameters[defroute_count];
    parameters[defroute_gateway] = config_defroute_gateway;
    parameters[defroute_interface] = config_defroute_interface;
    parameters[defroute_metric]  = config_defroute_metric;
    char values[route_count][VAL_LEN];
    FILE *file=NULL;
    // File used to save delete route information.
    static char *filename = "/var/log/defroute.log";

    if (check_log_file(filename))
    {
        goto error;
    }

    file = fopen(filename, "r+");
    if (file == NULL)
    {
        tlr_debug("apply_defroute: unable to open route log file [%s]", filename);
        goto error;
    }

    if (read_default_route_configuration(parameters, instance, values) < 0)
    {
        tlr_debug("Could not read uci configuration\n");
        goto error;
    }

    tlr_debug("apply_defroute instance %d\n", instance);
    tlr_debug("apply_defroute gateway [%s]\n", values[defroute_gateway] );
    tlr_debug("apply_defroute interface [%s]\n", values[defroute_interface]);

    if (add_default_route(file, values, instance) < 0)
    {
        tlr_debug("add_default_route failed\n");
        goto error;
    }

    ret = 0;
error:
    if (file != NULL)
    {
        fclose(file);
    }

    return ret;
}

// Delete any routes from the specified interface
int ifm_delete_routes(char const * const ifname)
{
    int ret = -1, i=0;
    char route[CMD_LEN];
    FILE *file=NULL;
    char *defroute_filename = "/var/log/defroute.log";
    char *route_filename = "/var/log/route.log";
    int instance = 0;

    if (ifname == NULL)
    {
        tlr_debug("ifm_delete_routes name is null");
        goto error;
    }

    tlr_debug("ifm_delete_routes: deleting routes for interface [%s]", ifname);

    do
    {
        char *ptr_filename;
        if (i == 0)
        {
            ptr_filename = defroute_filename;
        }
        else
        {
            ptr_filename = route_filename;
        }
        if (check_log_file(ptr_filename) < 0) // Log file used to keep track of routes.
        {
            goto error;
        }
        file = fopen(ptr_filename, "r+");
        if (file == NULL)
        {
            tlr_debug("ifm_delete_routes: unable to open route log file [%s]", ptr_filename);
            goto error;
        }
        while (get_route_for_interface(file, ifname, route, &instance))
        {
            instance_remove(file, instance);
            route_remove(route);
        }
        fclose(file);
    } while (i++ < 2);

    ret = 0;
error:

    return ret;
}


// Configuration parameter has changed on this row
int update_default_route(int const instance)
{
    int ret = -1;
    char line[CMD_LEN];
    FILE * file = NULL;
    config_defroute_t parameters[defroute_count];
    parameters[defroute_gateway] = config_defroute_gateway;
    parameters[defroute_interface] = config_defroute_interface;
    parameters[defroute_metric] = config_defroute_metric;
    static char * filename = "/var/log/defroute.log";
    char values[route_count][VAL_LEN];

    tlr_debug("update_defroute instance %d", instance);

    if (check_log_file(filename) < 0) // Log file used to keep track of routes.
    {
        goto error;
    }

    if (read_default_route_configuration(parameters, instance, values) < 0)
    {
        goto error;
    }

    file = fopen(filename, "r+");
    if (file == NULL)
    {
        tlr_debug("apply_defroute: unable to open route log file [%s]", filename);
        goto error;
    }
    if (does_instance_exist(file, instance, line))
    {
        instance_remove(file, instance);
        route_remove(line);
    }
    // See if we have a valid configuration.
    if (is_default_route_valid(instance))
    {
        if (add_default_route(file, values, instance) < 0)
        {
            goto error;
        }
    }

    if (create_dns_link() < 0)
    {
        tlr_debug("create dns: could not create dns links");
    }

    ret = 0;
error:
    if (file != NULL)
    {
        fclose(file);
    }
    return ret;
}

// apply the route for this instance in the defroute table
int apply_static_route(int const instance)
{
    int ret = -1;
    config_route_t parameters[route_count];
    parameters[route_destination]   = config_route_destination;
    parameters[route_mask]          = config_route_mask;
    parameters[route_gateway]       = config_route_gateway;
    parameters[route_metric]        = config_route_metric;
    parameters[route_interface]     = config_route_interface;
    char *filename = "/var/log/route.log";
    char values[route_count][VAL_LEN];
    FILE *file=NULL;

    if (check_log_file(filename))
    {
        goto error;
    }

    file = fopen(filename, "r+");
    if (file == NULL)
    {
        tlr_debug("apply_static_route: unable to open route log file [%s]", filename);
        goto error;
    }

    if (read_static_route_configuration(parameters, instance, values) < 0)
    {
        tlr_debug("apply_static_route: Could not read uci configuration\n");
        goto error;
    }

//    tlr_debug("apply_static_route instance %d\n", instance);
//    tlr_debug("apply_static_route destination [%s]\n", values[route_destination] );
//    tlr_debug("apply_static_route gateway [%s]\n", values[route_gateway] );
//    tlr_debug("apply_static_route interface [%s]\n", values[route_interface]);
//    tlr_debug("apply_static_route metric [%s]\n", values[route_metric]);

    if (add_static_route(file, values, instance) < 0)
    {
        tlr_debug("add_static_route failed\n");
        goto error;
    }

    ret = 0;
error:
    if (file != NULL)
    {
        fclose(file);
    }

    return ret;
}


int update_static_route(int const instance)
{
    int ret = -1;
    char line[CMD_LEN];
    FILE *file=NULL;
    config_route_t parameters[route_count];
    parameters[route_destination]   = config_route_destination;
    parameters[route_mask]          = config_route_mask;
    parameters[route_gateway]       = config_route_gateway;
    parameters[route_metric]        = config_route_metric;
    parameters[route_interface]     = config_route_interface;
    char values[route_count][VAL_LEN];
    static char *filename = "/var/log/route.log";

    tlr_debug("update_static_route instance %d", instance);

    if (check_log_file(filename) < 0) // Log file used to keep track of routes.
    {
        goto error;
    }

    if (read_static_route_configuration(parameters, instance, values) < 0)
    {
         goto error;
    }
#ifdef DEBUG_ROUTE
    tlr_debug("update_static_route instance %d\n", instance);
    tlr_debug("update_static_route destination [%s]\n", values[route_destination] );
    tlr_debug("update_static_route gateway [%s]\n", values[route_gateway] );
    tlr_debug("update_static_route interface [%s]\n", values[route_interface]);
    tlr_debug("update_static_route metric [%s]\n", values[route_metric]);
#endif

    file = fopen(filename, "r+");
    if (file == NULL)
    {
        tlr_debug("apply_defroute: unable to open route log file [%s]", filename);
        goto error;
    }
    if (does_instance_exist(file, instance, line))
    {
         instance_remove(file, instance);
         route_remove(line);
    }

    tlr_debug("calling add_static_route");
    if (is_static_route_valid(instance))
    {
        if (add_static_route(file, values, instance) < 0)
        {
            goto error;
        }
    }

    ret = 0;
error:
    if (file != NULL)
    {
        fclose(file);
    }
    return ret;
}

// Check if we can apply this route with the current configuration.
bool is_default_route_valid(int const instance)
{
    config_defroute_t parameters[defroute_count];
    parameters[defroute_gateway] = config_defroute_gateway;
    parameters[defroute_interface] = config_defroute_interface;
    parameters[defroute_metric]  = config_defroute_metric;
    bool ret=false;
    char values[route_count][VAL_LEN];

    if (read_default_route_configuration(parameters, instance, values) < 0)
    {
        tlr_debug("Could not read uci configuration\n");
        goto error;
    }
#ifdef DEBUG_ROUTE
    tlr_debug("is_default_route_valid instance %d\n", instance);
    tlr_debug("is_default_route_valid gateway [%s]\n", values[defroute_gateway] );
    tlr_debug("is_default_route_valid interface [%s]\n", values[defroute_interface]);
    tlr_debug("is_default_route_valid metric [%s]\n", values[defroute_metric]);
#endif


    bool has_interface = (strcmp(values[defroute_interface], "none") == 0) ? false : true;
    bool has_gateway   = (strlen(values[defroute_gateway]) > 0) ? true : false;

    if (has_interface && is_interface_up(values[defroute_interface]))
    {
        ret = true;
    }
    else if (has_gateway)
    {
        ret = true;
    }
    else
    {
        ret = false;
    }

error:
    return ret;
}

bool is_static_route_valid(int const instance)
{
    config_route_t parameters[route_count];
    parameters[route_destination]   = config_route_destination;
    parameters[route_mask]          = config_route_mask;
    parameters[route_gateway]       = config_route_gateway;
    parameters[route_metric]        = config_route_metric;
    parameters[route_interface]     = config_route_interface;
    char values[route_count][VAL_LEN];
    bool ret=false;

    if (read_static_route_configuration(parameters, instance, values) < 0)
    {
         goto error;
    }
    bool has_gateway     = (strlen(values[route_gateway]) > 0) ? true : false;
    bool has_destination = (strlen(values[route_destination]) > 0) ? true : false;
    bool has_mask = (strlen(values[route_destination]) > 0) ? true : false;
    bool has_interface   = (strcmp(values[route_interface], "none") == 0) ? false : true;

#ifdef DEBUG_ROUTE
    tlr_debug("apply_static_route instance %d\n", instance);
    tlr_debug("apply_static_route destination [%s]\n", values[route_destination] );
    tlr_debug("apply_static_route gateway [%s]\n", values[route_gateway] );
    tlr_debug("apply_static_route interface [%s]\n", values[route_interface]);
    tlr_debug("apply_static_route metric [%s]\n", values[route_metric]);
#endif

    if (has_gateway && has_destination && has_mask)
    {
        if (has_interface && !is_interface_up(values[route_interface]))
        {
            ret = false;
        }
        else
        {
            ret = true;
        }
    }
    else
    {
        ret = false;
    }
error:
     return ret;
}

STATIC_FUNCTION bool is_interface_ppp(char const * const ifname)
{
    // Ethernet is the only non-ppp interface we currently have
    return memcmp(ifname, "eth", 3) == 0 ? false : true;
}

bool does_interface_match_default_route(char const * const ifname, int const instance)
{
    config_defroute_t parameters[defroute_count];
    parameters[defroute_gateway] = config_defroute_gateway;
    parameters[defroute_interface] = config_defroute_interface;
    parameters[defroute_metric]  = config_defroute_metric;
    bool ret=false;
    char values[route_count][VAL_LEN];

    if (read_default_route_configuration(parameters, instance, values) < 0)
    {
        tlr_debug("Could not read uci configuration\n");
        goto error;
    }

    bool has_gateway = (strlen(values[route_gateway]) > 0) ? true : false;

    if (strcmp(values[defroute_interface], ifname) == 0)
    {
        ret = true;
    }
    // if there is no interface specified, we have a gateway and this is not
    // a point to point interface then apply this route, ppp interfaces
    // don't support default routes with a gateway specified.
    else if ((strcmp(values[defroute_interface], "none") == 0) && has_gateway
            && !is_interface_ppp(ifname))
    {
        ret = true;
    }
    else
    {
        ret = false;
    }
error:
    return ret;
}

bool does_interface_match_static_route(char const * const ifname, int const instance)
{
    config_route_t parameters[route_count];
    parameters[route_destination]   = config_route_destination;
    parameters[route_mask]          = config_route_mask;
    parameters[route_gateway]       = config_route_gateway;
    parameters[route_metric]        = config_route_metric;
    parameters[route_interface]     = config_route_interface;
    char values[route_count][VAL_LEN];
    bool ret=false;

    if (read_static_route_configuration(parameters, instance, values) < 0)
    {
         goto error;
    }

    bool has_interface   = (strcmp(values[route_interface], "none") == 0) ? false : true;

    // An interface is specified different than the one passed in.
    if (has_interface)
    {
        if (strcmp(values[route_interface], ifname) == 0)
        {
            ret = true;
        }
        else
        {
            ret = false;
        }
    }
    else
    {
        ret = true;
    }
error:
    return ret;
}

