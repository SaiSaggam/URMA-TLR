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
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>
#include <sys/stat.h>
#include <security.h>
#include <digiutils.h>
#include "tlr_print.h"

static int restart_system_related_services(void)
{
    int retval = -1;

    if (system_command("/usr/etc/init.d/init_snmp.sh reload >/dev/null 2>&1") < 0)
    {
        tlr_debug("could not reload SNMP");
        goto error;
    }
    if (system_command("/usr/etc/init.d/S07syslogd.sh restart >/dev/null 2>&1") < 0)
    {
        tlr_debug("could not restart syslogd");
        goto error;
    }
    retval = 0;
error:
    return retval;
}

static int create_device_key_file(void)
{
    int retval = -1;
    struct stat file_info;

    if (stat(DEVICE_KEY_FILE, &file_info) < 0)
    {
        if (system_command("/bin/base64 /dev/urandom | head -c 256 > %s >/dev/null 2>&1", DEVICE_KEY_FILE))
        {
            tlr_debug("could not create device key file");
            goto error;
        }
    }

    retval = 0;
error:
    return retval;
}

int main (int argc, char *argv[])
{
    int retval = EXIT_FAILURE;
    bool called_from_startup = false;
    char const *user=NULL;

    openlog("apply_system", 0, LOG_USER);

    if (!tlr_parse_args(argc, argv, NULL, &user))
    {
        tlr_debug("tlr_parse_args failed");
        goto error;
    }

    if (user != NULL)
    {
        called_from_startup = (strcmp(user, "boot") == 0) ? true : false;
    }

    if (!called_from_startup)
    {
        if (restart_system_related_services() < 0)
        {
            goto error;
        }
    }

    if (create_device_key_file() < 0)
    {
        goto error;
    }

    retval = EXIT_SUCCESS;
error:
    closelog();

    return retval;
}

