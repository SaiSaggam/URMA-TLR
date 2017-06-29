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

#include "config_mgr.h"
#include "digiutils.h"
#include "tlr_print.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>

static char const * const process_name = "update_dsl";
static char const * const network1 = "192.168.123";    // just some random local network
static char const * const network2 = "192.168.235";    // just some random local network

#define run_command(x...) system_command_with_log(process_name, x)

static bool parse_command(int const argc, char * const argv[], char const ** const filename_ptr, tlr_response_format_t * const format_ptr)
{
    bool success = false;
    char const * user;

    if (!tlr_parse_args(argc, argv, format_ptr, &user))
    {
        goto done;
    }

    if (optind >= argc)
    {
        goto done;
    }

    *filename_ptr = argv[optind];
    success = true;

done:
    if (!success)
    {
        tlr_printf(*format_ptr, "Usage: update dsl <filename>\n");
    }

    return success;
}

static bool check_file(tlr_response_format_t const format, char const * const filename, char const * const pathname)
{
    bool success = false;

    if (access(pathname, R_OK) != 0)
    {
        tlr_printf(format, "Cannot access %s\n", filename);
        goto done;
    }

    // TODO: Check file integrity, e.g. checksum? CRC? magic number? Awaiting info from AMIT...
    success = true;

done:
    return success;
}

#ifndef _CUNIT_
static bool network_in_use(char const * const network)
{
    return run_command("/sbin/route | grep \"^%s\"", network) == 0;
}

static bool set_modem_ip_address(char const * const network)
{
     return run_command("adsl_cmd \"ip addr %s.6\"", network) == 0;
}

static bool set_local_ip_address(char const * const network)
{
     return run_command("/sbin/ifconfig eth5 %s.254 netmask 255.255.255.0", network) == 0;
}

static bool ping_modem_before_update(char const * const network)
{
     return run_command("ping %s.6 -c 1 -W 1", network) == 0;
}

static bool upload_firmware(char const * const pathname, char const * const network)
{
     return run_command("tftp -l %s -r ras -p %s.6", pathname, network) == 0;
}

static bool ping_modem_after_update(char const * const network)
{
     return run_command("ping %s.6 -c 1 -w 30", network) == 0;
}
#endif

static void reset_local_ip_address(void)
{
    (void)run_command("/sbin/ifconfig eth5 0.0.0.0");
}

static bool start_update(tlr_response_format_t const format, char const * const filename, char const * const pathname)
{
    bool success = false;
    char const * network;

    tlr_printf(format, "Updating DSL modem firmware using %s; please wait...\n", filename);
    tlr_event_log(tlr_log_info, process_name, "DSL modem firmware update started\n");

    network = network1;
    if (network_in_use(network))
    {
        network = network2;
        if (network_in_use(network))
        {
            goto error;
        }
    }

    if (!set_modem_ip_address(network))
    {
        goto error;
    }

    if (!set_local_ip_address(network))
    {
        goto error;
    }

    if (!ping_modem_before_update(network))
    {
        goto done;
    }

    if (!upload_firmware(pathname, network))
    {
        goto done;
    }

    sleep(2);

    if (!ping_modem_after_update(network))
    {
        goto done;
    }

    success = true;

done:
    reset_local_ip_address();

error:
    tlr_printf(format, "DSL modem firmware update %s\n", success ? "complete" : "failed");
    tlr_event_log(tlr_log_info, process_name, "DSL modem firmware update %s\n", success ? "completed" : "failed");

    return success;
}

static bool update_dsl_main(int const argc, char * const argv[])
{
    char const * filename;
    tlr_response_format_t format = tlr_response_format_ascii;
    bool success = false;
    char * pathname = NULL;

    if (!parse_command(argc, argv, &filename, &format))
    {
        goto error;
    }

    if (asprintf(&pathname, "%s/%s", USER_DIR, filename) < 0)
    {
        goto error;
    }

    if (!check_file(format, filename, pathname))
    {
        goto error;
    }

    success = start_update(format, filename, pathname);

error:
    free(pathname);

    return success;
}

#ifndef _CUNIT_
int main(int const argc, char * const argv[])
{
	bool const success = update_dsl_main(argc, argv);

	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
#endif
