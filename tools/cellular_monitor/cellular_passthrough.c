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
****************************************************************************/

#include "cellular_passthrough.h"
#include "cellular_status.h"
#include "digiutils.h"
#include "monitor_debug.h"

#define PASS_THROUGH_COMMAND "/usr/bin/modem_passthrough.sh start"

static bool send_modem_passthrough_command(unsigned int const port_number)
{
    bool command_executed_ok = false;
    char * pass_through_command = NULL;

    if (asprintf(&pass_through_command, "%s %u", PASS_THROUGH_COMMAND, port_number) <= 0)
    {
        monitor_error("running pass through script failed on %d\n", port_number);
        goto error;
    }

    if (system_command(pass_through_command) < 0)
    {
        monitor_error("running pass through script failed\n");
        goto error;
    }
    command_executed_ok = true;

error:
    free(pass_through_command);
    return command_executed_ok;
}

bool cellular_start_pass_through(void)
{
    bool pass_through_started = false;
    unsigned int const port_number = cellular_get_pass_through_port_number();

    monitor_debug("Pass through mode enabled.\n");

    if (!send_modem_passthrough_command(port_number))
    {
        goto error;
    }
    pass_through_started = true;

error:
    return pass_through_started;
}



