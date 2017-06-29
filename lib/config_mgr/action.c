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

#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>

#include "config_def.h"
#include "config_table.h"
#include "config_mgr.h"
#include "config_status.h"
#include "validate.h"
#include "tlr_popen.h"
#include <tlr_common.h>

tlr_handle_t tlr_action_open(char const * const format, int const id, char const * const user, char const * const parameter, config_mgr_access_level_t const access)
{
    FILE * fp = NULL;

    if (id < config_mgr_action_table[0].count)
    {
        char command[CONFIG_MGR_NAME_LENGTH + 128];
        int length;

        config_mgr_action_t const * const action = &config_mgr_action_table[0].actions[id];

        tlr_config_debug("action id %d %s\n", id, parameter);

        if (tlr_verify_access(action->access, config_mgr_write_operation, access) != config_status_success)
        {
            if (tlr_verify_access(action->access, config_mgr_read_operation, access) != config_status_success)
            {
                tlr_config_error("Permission denied\n");
                goto ret;
            }
        }

        if ((parameter != NULL) && (action->elements.count > 0) && (action->elements.element != NULL) &&
            (action->elements.element[0].type == config_mgr_action_element))
        {
            char * begin_ptr = (char *) parameter;
            size_t i;

            {
                char * ptr = strchr(begin_ptr, ' ');

                if (ptr != NULL)
                    length = (ptr - begin_ptr);
                else
                    length = strlen(begin_ptr);

                strncpy(command, begin_ptr, length);
                begin_ptr += (length +1);
                command[length] = '\0';
            }

            for (i=0; i < action->elements.count; i++)
            {
                config_mgr_action_element_t const * const action_element = &action->elements.element[i];

                if (tlr_verify_access(action_element->access, config_mgr_write_operation, access) != config_status_success)
                {
                    if (tlr_verify_access(action_element->access, config_mgr_read_operation, access) != config_status_success)
                    {
                        tlr_config_error("Permission denied\n");
                        goto ret;
                    }
                }

                if (strcmp(command, action_element->name) == 0)
                {
                    config_mgr_action_parameter_t const * const element = action_element->element;

                    if (tlr_verify_range(element->validation, element->value_limit,
                                            element->type, command) != config_status_success)
                        goto ret;

                    tlr_config_debug(DESTDIR_STR "/usr/bin/action_%s_%s -f %s -u %s %s", action->name, action_element->name, format, user, begin_ptr);

                    length = snprintf(command, sizeof command, DESTDIR_STR "/usr/bin/action_%s_%s -f %s -u %s %s",
                                        action->name, action_element->name, format, user, begin_ptr);
                    goto cont;
                }
            }
        }

        tlr_config_debug(DESTDIR_STR "/usr/bin/action_%s -f %s -u %s %s", action->name, format, user, (parameter != NULL) ? parameter : "");

        length = snprintf(command, sizeof command, DESTDIR_STR "/usr/bin/action_%s -f %s -u %s %s",
                        action->name, format, user, (parameter != NULL) ? parameter : "");

cont:
        if (length == sizeof command)
        {
            tlr_config_error(" Internal error insufficient command length for %s action command\n", action->name);
            goto ret;
        }

        tlr_config_debug("executing \"%s\"\n", command);
        fp = tlr_popen(command, "r");
        if (fp == NULL) goto ret;
    }
ret:
    return fp;
}
