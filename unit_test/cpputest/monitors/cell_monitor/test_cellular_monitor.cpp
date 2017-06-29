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

extern "C"
{
    #include "cellular_passthrough.h"
    #include "cellular_status.h"
    #include <digiutils.h>
    #include "monitor_debug.h"
    #include "messages.h"
    #include "config_mgr.h"
    #include "tlr_common.h"
}

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <stdint.h>
#include <stdlib.h>

/* mock functions */
tlr_log_t cellular_debug_priority = tlr_log_nolog;
bool pass_through_enabled = true;
unsigned int port_number = 1;
bool system_command_return_normal = true;

/* Copied from ../vpn_monitor */
int system_command(char const * const format, ...)
{
    char * command = NULL;
    va_list args;

    va_start(args, format);
    if (vasprintf(&command, format, args) > 0)
    {
        mock().actualCall("system_command").withParameter("command", command);
        free(command);
    }
    va_end(args);

    if (system_command_return_normal)
        return mock().returnValue().getIntValue();
    else
        return -1;
}

void tlr_event_debug(tlr_log_t const priority, char * fmt, ...)
{
}

bool cellular_is_pass_through_enabled(void)
{
    mock().actualCall("cellular_is_pass_through_enabled");
    return pass_through_enabled;
}

unsigned int cellular_get_pass_through_port_number(void)
{
    mock().actualCall("cellular_get_pass_through_port_number");
    return port_number;
}

TEST_GROUP(test_cellular_passthrough)
{
    void setup()
    {
        /*printf("Setup...\n");*/
    }

    void teardown()
    {
       /*printf("Teardown...\n");*/
    }
};

TEST(test_cellular_passthrough, send_modem_passthrough_returns_true_when_enabled)
{
    bool actual_value, expected_value;

    pass_through_enabled = true;
    port_number = 1;
    system_command_return_normal = true;
    expected_value = true;

    mock().expectOneCall("cellular_is_pass_through_enabled");
    mock().expectOneCall("system_command")
       .withParameter("command", "/usr/bin/modem_passthrough.sh start 1");
    mock().expectOneCall("cellular_get_pass_through_port_number");

    actual_value = cellular_start_pass_through_if_enabled();
    CHECK_EQUAL(actual_value, expected_value);

}

TEST(test_cellular_passthrough, send_modem_passthrough_returns_true_when_disabled)
{
    bool actual_value, expected_value;

    pass_through_enabled = false;
    port_number = 1;
    system_command_return_normal = true;
    expected_value = false;

    mock().expectOneCall("cellular_is_pass_through_enabled");

    actual_value = cellular_start_pass_through_if_enabled();
    CHECK_EQUAL(actual_value, expected_value);
}


TEST(test_cellular_passthrough, send_modem_passthrough_returns_false_when_system_command_fails)
{
    bool actual_value, expected_value;

    pass_through_enabled = true;
    port_number = 1;
    system_command_return_normal = false;
    expected_value = false;

    mock().expectOneCall("cellular_is_pass_through_enabled");
    mock().expectOneCall("system_command")
       .withParameter("command", "/usr/bin/modem_passthrough.sh start 1");
    mock().expectOneCall("cellular_get_pass_through_port_number");

    actual_value = cellular_start_pass_through_if_enabled();
    CHECK_EQUAL(actual_value, expected_value);
}
