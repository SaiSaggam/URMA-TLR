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

#include "bridge_interfaces.h"
#include "bridge_monitor_init.h"
#include "bridge_monitor_log.h"
#include "digiutils.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define BRCTL_EXECUTABLE "/usr/sbin/brctl"

static void bridge_interface_clear_forwarding_delay(char const * const interface_name, size_t const instance)
{
    /* It isn't the end of the world if this command fails, so there is no need to do anything
     * other than log an error if it fails.
     */
    if (system_command_with_log(BRIDGEMON_PROCESS_NAME, BRCTL_EXECUTABLE" setfd %s%zu 0", interface_name, instance) == -1)
    {
        bridgemon_debug("Unable to clear forwarding delay on %s%u", interface_name, instance);
    }
}

static bool bridge_interface_create(char const * const interface_name, size_t const instance)
{
    bool interface_created;

    if (system_command_with_log(BRIDGEMON_PROCESS_NAME, BRCTL_EXECUTABLE" addbr %s%zu", interface_name, instance) == -1)
    {
        interface_created = false;
        goto done;
    }
    interface_created = true;

done:
    return interface_created;
}

bool bridge_interfaces_create(size_t const first_instance, size_t const num_bridge_interfaces)
{
    bool all_interfaces_created;
    unsigned int i;
    char const * const interface_name = bridge_interface_name_get();

    for (i = 0; i < num_bridge_interfaces; i++)
    {
        size_t const instance = first_instance + i;

        if (!bridge_interface_create(interface_name, instance))
        {
            all_interfaces_created = false;
            goto done;
        }
        bridge_interface_clear_forwarding_delay(interface_name, instance);
    }

    all_interfaces_created = true;

done:
    return all_interfaces_created;
}

bool bridge_interface_add_child_interfaces(size_t const instance, char const * const interface_names)
{
    bool interface_added;

    if (system_command_with_log(BRIDGEMON_PROCESS_NAME, BRCTL_EXECUTABLE" addif %s%zu %s", bridge_interface_name_get(), instance, interface_names) == -1)
    {
        interface_added = false;
        goto done;
    }
    interface_added = true;

done:
    return interface_added;
}

bool bridge_interface_delete_child_interfaces(size_t const instance, char const * const interface_names)
{
    bool interface_deleted;

    if (system_command_with_log(BRIDGEMON_PROCESS_NAME, BRCTL_EXECUTABLE" delif %s%zu %s", bridge_interface_name_get(), instance, interface_names) == -1)
    {
        interface_deleted = false;
        goto done;
    }
    interface_deleted = true;

done:
    return interface_deleted;
}
