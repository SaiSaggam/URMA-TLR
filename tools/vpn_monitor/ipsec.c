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

#include "ipsec.h"
#include "debug.h"
#include "vpn_monitor_defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include <messages.h>
#include <message_codes.h>
#include <timer.h>
#include <event_manager.h>
#include <tlr_print.h>
#include <tlr_common.h>
#include <interfaces.h>
#include <parsing.h>
#include <digiutils.h>


static int ipsec_execute_strongswan_init_script(char const * const arg)
{
    int result;

    if (system_command(INIT_STRONGSWAN_SCRIPT_NAME " %s", arg) < 0)
    {
        tlr_error("unable to execute strongswan init script (%s)", arg);
        result = -1;
        goto error;
    }
    result = 0;

error:
    return result;
}

int ipsec_start_strongswan(void)
{
    int result;

    /* the script will parse the config and issue the ipsec start command */
    if (ipsec_execute_strongswan_init_script("start") < 0)
    {
        result = -1;
        goto error;
    }
    result = 0;

error:
    return result;
}

int ipsec_reload_configuration(void)
{
    int result;

    // This script determines changes in configuration, and updates them on the running daemon.
    // Currently established connections are not affected
    // Note that this script also executes an "ipsec reload" command
    if (ipsec_execute_strongswan_init_script("reload") < 0)
    {
        result = -1;
        goto error;
    }
    result = 0;

error:
    return result;
}

static int kill_old_ipsec_up_command(vpn_tunnel_context_st * const context)
{
    int ret;
    char * pkill_command = NULL;

    /*
     * It turns out that the TLR can sometimes get into a state where an "ipsec up <tunnel name>"
     * takes a very long time to execute, and an "ipsec down <tunnel name>" doesn't cause the 'up'
     * command to end.
     * What we can do to try and avoid this is execute
     * pkill -f "pkill -f "/usr/libexec/strongswan/stroke up <tunnel name>"
     * and
     * pkill -f "ipsec up <tunnel_name>"
     * which are the applications the "ipsec up <tunnel name>" are waiting for
     * Because we don't know if one of these "ipsec up" processes is running, we execute the pkill
     * command unconditionally.
     */
    if (asprintf(&pkill_command, "%s -f \"%s up %s\"", PKILL_APPLICATION_NAME, STROKE_APPLICATION_NAME, context->name) < 0)
    {
        ret = -1;
        goto error;
    }

    /* we may get an error if the process we're trying to kill doesn't exist, but that's entirely
     * normal.
     */
    (void)system_command(pkill_command);

    ret = 0;

error:
    free(pkill_command);

    return ret;
}

// The script init_strongswan.sh initialized ipsec.conf, we
// just start, stop or restart ipsec
int ipsec_start_tunnel(vpn_tunnel_context_st * const context)
{
    int ret;
    char * cmd = NULL;

    tlr_debug("ipsec_start_tunnel: [%s]", context->name);

    if (kill_old_ipsec_up_command(context) < 0)
    {
        tlr_error("error killing old ipsec up command");
        /* not a big enough error to stop us trying to continue though */
    }

    /* use the '&' option so that the command returns immediately.
     * It turns out that although vpn_monitor is killed, the messaging
     * socket isn't actually closed down until after the children also die.
     * XXX FIXME. The '&' thing doesn't acutally seem to work. I think the 'disown' option/command might be
     * the thing to use, but it doesn't appear to be supported on the TLR.
     * Update: perhaps the nohup option does the trick, but it also appears to be not supported.
     */
    if (asprintf(&cmd, "%s up %s &", IPSEC_APPLICATION_NAME, context->name) < 0)
    {
        ret = -1;
        goto error;
    }

    if (system_command(cmd) < 0)
    {
        tlr_error("unable to execute ipsec up");
        ret = -1;
        goto error;
    }

    ret = 0;

error:
    free(cmd);

    return ret;
}

int ipsec_stop_tunnel(vpn_tunnel_context_st * const context)
{
    int ret = -1;
    char *cmd = NULL;

    tlr_debug("ipsec_stop_tunnel: [%s]", context->name);
    if (asprintf(&cmd, "%s down %s", IPSEC_APPLICATION_NAME, context->name) < 0)
    {
        goto error;
    }

    if (system_command(cmd) < 0)
    {
        tlr_error("unable to execute ipsec down");
        goto error;
    }

    ret = 0;
error:
    free(cmd);

    return ret;
}

int ipsec_update_tunnel_configuration(vpn_tunnel_context_st * const context)
{
    int ret;

    /*
     * It appears that there is no need to perform a 'down' command first as executing an 'up'
     * operation results in tunnels getting renegotiated.
     * So just update the config and then exectute an 'up' command.
    */
    /* XXX do we need to do this give that we've just done a 'reload' operation? */
    // Sends a HUP signal to ipsec starter which in turn determines any changes in
    // ipsec.conf and updates the configuration on the running IKE daemon charon.
    // Currently established connections are not affected by configuration changes.
    if (ipsec_execute_strongswan_init_script("update") < 0)
    {
        tlr_error("unable to execute update command");
        ret = -1;
        goto error;
    }

    ret = 0;

error:

    return ret;
}

