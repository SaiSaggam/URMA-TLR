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
    #include "bridge_monitor_config_commands.h"

    /* include test helper include files */
    #include "test_interfaces_helper.h"
}

static const char * test_interface_name = "test_iface";

/* tests */
TEST_GROUP(connect_disconnect)
{
    size_t expected_call_count;
    size_t expected_interface;
    char const * expected_direction;

    void setup()
    {
        test_bridge_interface_name_set(test_interface_name);
    }

    void teardown()
    {
        test_system_commands_cleanup();
    }

    void init(size_t interface, size_t expected_system_call_count, char const * const direction, int system_command_result)
    {
        expected_call_count = expected_system_call_count;
        expected_interface = interface;
        expected_direction = direction;
        test_system_command_call_count_init(expected_call_count);
        test_system_command_result_set(system_command_result);
    }

    void check_system_command()
    {
        size_t const i = 0;

        LONGS_EQUAL(expected_call_count, test_system_command_call_count_get());

        char * command = NULL;

        asprintf(&command, "/sbin/ifconfig %s%zu %s", test_interface_name, expected_interface, expected_direction);
        STRCMP_EQUAL(command, test_system_command_get(i));
        free(command);
    }

    void perform_test(size_t const instance, int const system_command_result, bool const do_connect, bool const expected_result)
    {
        bool result;
        char const * const direction = do_connect ? "up" : "down";

        init(instance, 1, direction, system_command_result);

        /* perform test*/
        if (do_connect)
        {
            result = bridge_connect_interface(instance);
        }
        else
        {
            result = bridge_disconnect_interface(instance);
        }

        /* check result */
        LONGS_EQUAL(expected_result, result);
        check_system_command();
    }


};

TEST(connect_disconnect, connect_instance_1)
{
    perform_test(1, 0, true, true);
}

TEST(connect_disconnect, connect_instance_2)
{
    perform_test(2, 0, true, true);
}

TEST(connect_disconnect, connect_with_failed_system_command_returns_false)
{
    perform_test(1, -1, true, false);
}

TEST(connect_disconnect, disconnect_instance_1)
{
    perform_test(1, 0, false, true);
}

TEST(connect_disconnect, disconnect_instance_2)
{
    perform_test(2, 0, false, true);
}

TEST(connect_disconnect, disconnect_with_failed_system_command_returns_false)
{
    perform_test(1, -1, false, false);
}

TEST_GROUP(mtu)
{
    size_t expected_call_count;
    size_t expected_interface;
    size_t expected_mtu;

    void setup()
    {
        test_bridge_interface_name_set(test_interface_name);
    }

    void teardown()
    {
        test_system_commands_cleanup();
    }

    void init(size_t interface, size_t system_call_count, size_t const mtu, int const system_command_result)
    {
        expected_call_count = system_call_count;
        expected_interface = interface;
        expected_mtu = mtu;
        test_system_command_call_count_init(expected_call_count);
        test_system_command_result_set(system_command_result);
    }

    void check_system_command()
    {
        size_t const i = 0;

        LONGS_EQUAL(expected_call_count, test_system_command_call_count_get());

        char * command = NULL;

        asprintf(&command, "/sbin/ifconfig %s%zu mtu %zd", test_interface_name, expected_interface, expected_mtu);
        STRCMP_EQUAL(command, test_system_command_get(i));
        free(command);
    }

    void perform_test(size_t const instance, size_t const mtu, int const system_command_result, bool const expected_result)
    {
        bool result;
        char mtu_buf[50];

        init(instance, 1, mtu, system_command_result);
        snprintf(mtu_buf, sizeof(mtu_buf), "%zd", mtu);
        /* perform test*/
        result = bridge_interface_set_mtu(instance, mtu_buf);

        /* check result */
        LONGS_EQUAL(expected_result, result);
        check_system_command();
    }


};

TEST(mtu, instance_1_mtu_1500)
{
    perform_test(1, 1500, 0, true);
}

TEST(mtu, honours_interface)
{
    perform_test(2, 1500, 0, true);
}

TEST(mtu, honours_mtu)
{
    perform_test(2, 1500, 0, true);
}

TEST(mtu, error_with_failed_system_command)
{
    perform_test(2, 1500, -1, false);
}

TEST_GROUP(set_ip_mask)
{
    size_t expected_call_count;
    size_t expected_interface;
    char const * expected_ip_address;
    char const * ip_address_to_command;
    char const * expected_mask;

    void setup()
    {
        test_bridge_interface_name_set(test_interface_name);
    }

    void teardown()
    {
        test_system_commands_cleanup();
    }

    void init(size_t const interface, char const * const ip_address_to_set, char const * const ip_address_to_expect, char const * const mask_string, int const system_command_result)
    {
        expected_call_count = 1;
        expected_interface = interface;
        expected_ip_address = ip_address_to_expect;
        expected_mask = mask_string;
        ip_address_to_command = ip_address_to_set;

        test_system_command_call_count_init(expected_call_count);
        test_system_command_result_set(system_command_result);
    }

    void check_system_command()
    {
        size_t const i = 0;

        LONGS_EQUAL(expected_call_count, test_system_command_call_count_get());

        char * command = NULL;

        asprintf(&command, "/sbin/ifconfig %s%zu %s netmask %s", test_interface_name, expected_interface, expected_ip_address, expected_mask);
        STRCMP_EQUAL(command, test_system_command_get(i));
        free(command);
    }

    void perform_test(size_t const instance,
                      char const * const ip_address_to_set,
                      char const * const ip_address_to_expect,
                      char const * const mask_string,
                      int const system_command_result,
                      bool const expected_result)
    {
        bool result;

        init(instance, ip_address_to_set, ip_address_to_expect, mask_string, system_command_result);

        /* perform test*/
        result = bridge_interface_set_ip_address_and_mask(instance, ip_address_to_set, mask_string);

        /* check result */
        LONGS_EQUAL(expected_result, result);
        check_system_command();
    }


};

TEST(set_ip_mask, plain_command_successful)
{
    perform_test(1, "1.2.3.4", "1.2.3.4", "255.255.255.0", 0, true);
}

TEST(set_ip_mask, empty_ip_address_sets_0_0_0_0)
{
    perform_test(1, "", "0.0.0.0", "255.255.255.0", 0, true);
}

TEST(set_ip_mask, failed_system_command_returns_false)
{
    perform_test(1, "", "0.0.0.0", "255.255.255.0", -1, false);
}

