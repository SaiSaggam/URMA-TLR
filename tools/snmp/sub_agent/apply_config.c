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

#include "apply_config.h"
#include "config_mgr.h"
#include "tlr_common.h"
#include "digiutils.h"
#include "snmp_agent_debug.h"

#define APPLY_PROCESS_READ_BUFFER_SIZE 256

static bool did_apply_read_succeed(config_status_t const config_status)
{
    return (config_status == config_status_success)
        || (config_status == config_status_read_complete);
}

static int get_apply_read_result(tlr_handle_t apply_handle)
{
    int retval = -1;
    config_status_t config_status;
    char output_buffer[APPLY_PROCESS_READ_BUFFER_SIZE];

    config_status = tlr_apply_read(apply_handle, output_buffer, sizeof output_buffer, WAIT_FOREVER);

    if (!did_apply_read_succeed(config_status))
    {
        snmp_debug("tlr_apply_read failed %d\n", config_status);
        goto error;
    }

    retval = 0;
error:
    return retval;
}

static int save_config_file(void)
{
    int retval = -1;

    if (system_command("action_save_config -f ascii -i 1 -u snmp") < 0)
    {
        snmp_debug("unable to save configuration for SNMP set\n");
        goto error;
    }

    retval = 0;
error:
    return retval;
}

int snmp_apply_config(size_t const group_id, size_t const instance)
{
    int retval = -1;
    tlr_handle_t apply_handle;

    apply_handle = tlr_apply_open(group_id, instance, "snmp", config_mgr_access_level_super);

    if (apply_handle == NULL)
    {
        snmp_debug("tlr_apply_open failed\n");
        goto error;
    }

    if (get_apply_read_result(apply_handle) < 0)
    {
        goto error;
    }

    if (save_config_file() < 0)
    {
        goto error;
    }

    retval = 0;
error:
    if (apply_handle != NULL)
    {
        tlr_apply_close(apply_handle);
    }

    return retval;

}


