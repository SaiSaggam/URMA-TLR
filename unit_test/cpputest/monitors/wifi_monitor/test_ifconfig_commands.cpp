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
#include <CppUTest/TestHarness.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CppUTestExt/MockSupport.h"

/*
 * This is where we include the header files of the C code under test
 */
extern "C"
{
    /* include C header files of code under test */
    #include "wifi_monitor_ifconfig_commands.h"
    #include "wifi_monitor_init.h"
    #include "digiutils.h"

}

static char const expected_interface_name[] = "interface_name";
static char const expected_process_name[] = "process_name";
static char const expected_connect_command[] = "/sbin/ifconfig interface_name1 up";
static char const expected_disconnect_command[] = "/sbin/ifconfig interface_name1 down";

char const * wifi_interface_name_get(void)
{
    return expected_interface_name;
}

char const * wifi_process_name_get(void)
{
    return expected_process_name;
}

int system_command_with_log(char const * const process_name, char const * const format, ...)
{
    char * command = NULL;
    va_list args;

    va_start(args, format);

    CHECK_TRUE(vasprintf(&command, format, args) > 0);

    mock().actualCall("system_command_with_log")
        .withParameter("command", command)
        .withParameter("process_name", process_name);

    free(command);

    return mock().returnIntValueOrDefault(-1);  /* Default to error return. */
}

/* tests */
TEST_GROUP(connect_disconnect)
{
    void setup()
    {
    }

    void teardown()
    {
        mock().clear();
    }

};

TEST(connect_disconnect, connect_interface_calls_system_command_returns_true_if_command_succeeds)
{
    size_t const instance_to_use = 1;
    bool actual_result;
    bool expected_result;

    /* setup */
    mock().expectOneCall("system_command_with_log")
        .withParameter("process_name", expected_process_name)
        .withParameter("command", expected_connect_command)
        .andReturnValue(0);
    expected_result = true;

    /* perform test */
    actual_result = wifi_connect_interface(instance_to_use);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();
}

TEST(connect_disconnect, connect_interface_calls_system_command_returns_false_if_command_fails)
{
    size_t const instance_to_use = 1;
    bool actual_result;
    bool expected_result;

    /* setup */
    mock().expectOneCall("system_command_with_log")
        .withParameter("process_name", expected_process_name)
        .withParameter("command", expected_connect_command)
        .andReturnValue(-1);
    expected_result = false;

    /* perform test */
    actual_result = wifi_connect_interface(instance_to_use);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();
}

TEST(connect_disconnect, disconnect_interface_calls_system_command_returns_true_if_command_succeeds)
{
    size_t const instance_to_use = 1;
    bool actual_result;
    bool expected_result;

    /* setup */
    mock().expectOneCall("system_command_with_log")
        .withParameter("process_name", expected_process_name)
        .withParameter("command", expected_disconnect_command)
        .andReturnValue(0);
    expected_result = true;

    /* perform test */
    actual_result = wifi_disconnect_interface(instance_to_use);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();
}

TEST(connect_disconnect, disconnect_interface_calls_system_command_returns_false_if_command_fails)
{
    size_t const instance_to_use = 1;
    bool actual_result;
    bool expected_result;

    /* setup */
    mock().expectOneCall("system_command_with_log")
        .withParameter("process_name", expected_process_name)
        .withParameter("command", expected_disconnect_command)
        .andReturnValue(-1);
    expected_result = false;

    /* perform test */
    actual_result = wifi_disconnect_interface(instance_to_use);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();
}

