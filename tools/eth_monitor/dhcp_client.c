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
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "interfaces.h"
#include "tlr_print.h"
#include "digiutils.h"
#include "dhcp_client.h"

#define LIBRARY_NAME        "DHCP Client"
#define UDHCPC_EXECUTABLE   "/sbin/udhcpc"

#if defined(UNIT_TEST)
#define STATIC
#else
#define STATIC static
#endif // UNIT_TEST

#define _10_MILLISECONDS_IN_NANOSECONDS 10000
STATIC char const * const udhcp_nameservers_format = "/ram/var/dns/%s%zu/udhcpc_nameservers";
STATIC char const * const udhcp_pidfile_format = "/ram/var/dns/%s%zu/udhcpc.pid";

void dhcp_client_delete_nameservers_file(interfaces_t const interface, size_t const instance)
{
    char * nameservers_file = NULL;
    char const * const interface_name = interface_get_name_from_type(interface);

    if (asprintf(&nameservers_file, udhcp_nameservers_format, interface_name, instance) == -1)
    {
        tlr_debug("delete_dhcp_nameservers_file:  out of memory, asprintf failed\n");
        goto done;
    }
    if (unlink(nameservers_file) != 0)
    {
        tlr_debug("delete_dhcp_nameservers_file:  Error deleting udhcpc nameservers file %s", nameservers_file);
    }

done:
    free(nameservers_file);
}

char * dhcp_client_get_pid_filename(interfaces_t const interface, size_t const instance)
{
    char const * const interface_name = interface_get_name_from_type(interface);
    char * pidfilename = NULL;

    if (asprintf(&pidfilename, udhcp_pidfile_format, interface_name, instance) == -1)
    {
        tlr_debug("dhcp_client_get_pid_filename:  out of memory, asprintf failed\n");
        pidfilename = NULL;
    }

    return pidfilename; /* caller must free this */
}

pid_t dhcp_client_get_pid(interfaces_t const interface, size_t const instance)
{
    char * pid_filename = dhcp_client_get_pid_filename(interface, instance);
    pid_t pid = DHCP_CLIENT_NO_PID;

    if (pid_filename != NULL)
    {
        FILE * pid_file = fopen(pid_filename, "r");

        if (pid_file)
        {
            if (fscanf(pid_file, " %u ", &pid) != 1)
            {
                pid = 0;
            }
            fclose(pid_file);
        }
    }

    free(pid_filename);
    return pid;
}

int dhcp_client_start(interfaces_t const interface, size_t const instance)
{
    char const * const interface_name = interface_get_name_from_type(interface);
    char * pid_filename = dhcp_client_get_pid_filename(interface, instance);
    int result = 0;

    if (pid_filename)
    {
        tlr_debug("dhcp_client_start: starting DHCP client on %s%zu\n", interface_name, instance);
        if (system_command(UDHCPC_EXECUTABLE" -i %s%zu -p %s -s /usr/share/udhcpc/tlr_dhcp_client_script.sh &", interface_name, instance, pid_filename) != 0)
        {
            // system command error
            result = -1;
        }
        free(pid_filename);
    }
    else
    {
        // out of memory error
        result = -1;
    }

    if (result)
    {
        tlr_debug("dhcp_client_start: Error starting DHCP client on %s%zu\n", interface_name, instance);
    }

    return result;
}

int dhcp_client_stop(interfaces_t const interface, size_t const instance)
{
    char const * const interface_name = interface_get_name_from_type(interface);
    char * pid_filename = dhcp_client_get_pid_filename(interface, instance);
    pid_t pid = dhcp_client_get_pid(interface, instance);
    int result = 0;
    int const SIG_RELEASE_DHCP_LEASE = SIGUSR2;

    tlr_debug("dhcp_client_stop: stopping DHCP client on %s%zu\n", interface_name, instance);
    if (kill(pid, SIG_RELEASE_DHCP_LEASE))
    {
        tlr_debug("dhcp_client_start: Unable to release DHCP lease");
    }
    usleep(_10_MILLISECONDS_IN_NANOSECONDS);
    if (kill(pid, SIGKILL))
    {
        tlr_debug("dhcp_client_start: Unable to kill udhcpc process");
        result = -1;
    }

    if ( pid_filename )
    {
        unlink(pid_filename);
    }
    dhcp_client_delete_nameservers_file(interface, instance);

    if ( pid_filename )
    {
        free(pid_filename);
    }
    return result;
}


