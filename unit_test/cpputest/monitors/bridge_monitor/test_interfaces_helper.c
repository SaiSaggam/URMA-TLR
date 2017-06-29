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
#include "test_interfaces_helper.h"
#include "bridge_monitor_init.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static char const * test_interface_name;
static char * * test_commands;
static size_t command_capacity;
static size_t system_command_call_count;
static int system_command_result;

char const * bridge_interface_name_get(void)
{
    return test_interface_name;
}

int system_command_with_log(char const * const process_name, char const * const format, ...)
{
    va_list args;
    int result;

    if (system_command_call_count < command_capacity)
    {
        va_start(args, format);
        vasprintf(&test_commands[system_command_call_count], format, args);
        va_end(args);
    }
    result = system_command_result;

    system_command_call_count++;

    return result;
}

void test_bridge_interface_name_set(char const * const name)
{
    test_interface_name = name;
}

void test_system_command_result_set(int result)
{
    system_command_result = result;
}

void test_system_commands_cleanup(void)
{
    if (test_commands != NULL)
    {
        size_t i;

        for (i = 0; i < command_capacity; i++)
        {
            free(test_commands[i]);
            test_commands[i] = NULL;
        }
        free(test_commands);
    }
    test_commands = NULL;
    command_capacity = 0;
}

void test_system_command_call_count_init(size_t const expected_call_count)
{
    /* prepare for the expected number of calls */
    command_capacity = expected_call_count;
    test_commands = calloc(command_capacity, sizeof(char *));
    system_command_call_count = 0;
}

char const * test_system_command_get(size_t const index)
{
    char const * command;

    if (index < command_capacity)
    {
        command = test_commands[index];
    }
    else
    {
        command = NULL;
    }

    return command;
}

size_t test_system_command_call_count_get(void)
{
    return system_command_call_count;
}

