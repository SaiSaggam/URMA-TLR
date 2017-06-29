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

#include "bridge_interfaces_update.h"
#include "bridge_interfaces.h"
#include "bridge_monitor_log.h"
#include "bridge_monitor.h"
#include "messages.h"
#include "str_replace_char.h"
#include "unittest_support.h"

#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

bool bridge_is_connected(size_t const instance)
{
    bridge_context_st const * const context = bridge_get_context(instance);

    assert(context != NULL);

    return context->current_state == bridge_state_connected;
}

STATIC_FUNCTION bool parse_interface_name(char const * const interface_name, interfaces_t * const interface_type, size_t * const instance)
{
    bool interface_name_parsed;
    char * const interface_name_working_copy = strdup(interface_name);
    char * ptr;
    size_t  namelen;

    if (interface_name_working_copy == NULL)
    {
        interface_name_parsed = false;
        goto done;
    }

    ptr = interface_name_working_copy;
    namelen = strlen(ptr);
    ptr += namelen;
    // Find the instance number from the ass end of the interface_name in case the interface_name itself has a number
    // in it ... eg.. "wifi5g".
    while ((namelen > 0) && isdigit(*(ptr-1)))
    {
        ptr--;
        namelen--;
    }

    if (*ptr != '\0' && namelen > 0)
    {
        *instance = strtoul(ptr, NULL, 10);
        *ptr = '\0';
    }
    else
    {
        *instance = 1;  // default for singleton interfaces
    }

    *interface_type = interface_get_type_from_name(interface_name_working_copy);

    interface_name_parsed = *interface_type != invalid_interface;

done:
    free(interface_name_working_copy);

    return interface_name_parsed;
}

static bool send_message_to_interface_name(message_codes_t const message_code, char const * const interface_name)
{
    bool message_sent;
    interfaces_t interface_type;
    size_t instance;

    if (!parse_interface_name(interface_name, &interface_type, &instance))
    {
        message_sent = false;
        goto done;
    }

    message_sent = send_message_to_interface_monitor(interface_type, instance, message_code, NULL) == 0;

done:
    return message_sent;
}


static bool add_or_delete_interface(bool const add_not_delete, size_t const instance, char const * const interface_name)
{
    bool interface_added_or_deleted;
    message_codes_t     message_code;

    if (add_not_delete)
    {
        if (!bridge_interface_add_child_interfaces(instance, interface_name))
        {
            interface_added_or_deleted = false;
            goto done;
        }
        message_code = joined_bridge_message;
    }
    else
    {
        if (!bridge_interface_delete_child_interfaces(instance, interface_name))
        {
            interface_added_or_deleted = false;
            goto done;
        }
        message_code = left_bridge_message;
    }
    if (!send_message_to_interface_name(message_code, interface_name))
    {
        // It's not the end of the world if we couldn't send the message.
        // so we still return OK if couldn't send the message. (otherwise we would think we couldn't add the interface.)
        bridgemon_debug("send message fail in bridgemon");
    }
    interface_added_or_deleted = true;

done:
    return interface_added_or_deleted;
}

static bool add_or_delete_interfaces(bool const add_not_delete, size_t const instance, char const * const interface_names)
{
    bool interfaces_added_or_deleted;
    char * const interface_names_working_copy = strdup(interface_names);
    char * str = interface_names_working_copy;
    char * save;
    char * token;

    if (interface_names_working_copy == NULL)
    {
        interfaces_added_or_deleted = false;
        goto done;
    }

    while ((token = strtok_r(str, ",", &save)) != NULL)
    {
        str = NULL;

        if (strcmp(token, "none") == 0)
        {
            continue;
        }

        if (!add_or_delete_interface(add_not_delete, instance, token))
        {
            interfaces_added_or_deleted = false;
            goto done;
        }
    }

    interfaces_added_or_deleted = true;

done:
    free(interface_names_working_copy);

    return interfaces_added_or_deleted;
}

static bool interface_names_subtract(char const * const minuend, char const * const subtrahend, char ** const difference)
{
    bool interface_names_subtracted;
    char * const minuend_working_copy = strdup(minuend);
    char * str = minuend_working_copy;
    char * save;
    char * token;

    if (minuend_working_copy == NULL)
    {
        interface_names_subtracted = false;
        goto done;
    }

    *difference = calloc(1, strlen(minuend) + 1);
    if (*difference == NULL)
    {
        interface_names_subtracted = false;
        goto done;
    }

    while ((token = strtok_r(str, ",", &save)) != NULL)
    {
        str = NULL;
        if (strstr(subtrahend, token) == NULL)
        {
            if (**difference != '\0')
            {
                strcat(*difference, ",");
            }
            strcat(*difference, token);
        }
    }

    interface_names_subtracted = true;

done:
    free(minuend_working_copy);

    return interface_names_subtracted;
}

static char * get_interfaces_deleted(char const * const old_interface_names, char const * const new_interface_names)
{
    char * interfaces_deleted;

    if (!interface_names_subtract(old_interface_names, new_interface_names, &interfaces_deleted))
    {
        interfaces_deleted = NULL;
    }

    return interfaces_deleted;
}

static char * get_interfaces_added(char const * const old_interface_names, char const * const new_interface_names)
{
    char * interfaces_added;

    if (!interface_names_subtract(new_interface_names, old_interface_names, &interfaces_added))
    {
        interfaces_added = NULL;
    }

    return interfaces_added;
}

bool bridge_interfaces_changed(size_t const instance, char const * const old_value, char const * const new_value)
{
    bool interfaces_set;
    char * interfaces_deleted = NULL;
    char * interfaces_added = NULL;

    if (!bridge_is_connected(instance))
    {
        interfaces_set = false;
        goto done;
    }

    interfaces_deleted = get_interfaces_deleted(old_value, new_value);
    if (interfaces_deleted == NULL)
    {
        interfaces_set = false;
        goto done;
    }

    if (*interfaces_deleted != '\0' && !add_or_delete_interfaces(false, instance, interfaces_deleted))
    {
        interfaces_set = false;
        goto done;
    }

    interfaces_added = get_interfaces_added(old_value, new_value);
    if (interfaces_added == NULL)
    {
        interfaces_set = false;
        goto done;
    }

    if (*interfaces_added != '\0' && !add_or_delete_interfaces(true, instance, interfaces_added))
    {
        interfaces_set = false;
        goto done;
    }

    interfaces_set = true;

done:
    free(interfaces_deleted);
    free(interfaces_added);

    return interfaces_set;
}

bool bridge_config_delete_interfaces(size_t const instance)
{
    bridge_context_st const * const context = bridge_get_context(instance);
    char const * interface_names;

    assert(context != NULL);

    interface_names = config_get_element_value(context->config, config_lan_interfaces);
    return add_or_delete_interfaces(false, instance, interface_names);
}

bool bridge_config_add_interfaces(size_t const instance)
{
    bridge_context_st const * const context = bridge_get_context(instance);
    char const * interface_names;

    assert(context != NULL);

    interface_names = config_get_element_value(context->config, config_lan_interfaces);
    return add_or_delete_interfaces(true, instance, interface_names);
}


