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
    #include "bridge_interfaces.h"

    /* include test helper include files */
    #include "test_interfaces_helper.h"
}

static const char * test_interface_name = "test_iface";

/* tests */
TEST_GROUP(interfaces_create)
{
    size_t expected_call_count;
    size_t first_interface;

    void setup()
    {
        test_bridge_interface_name_set(test_interface_name);
    }

    void teardown()
    {
        test_system_commands_cleanup();
    }

    void init(size_t expected_first_interface, size_t expected_system_call_count, int system_command_result)
    {
        expected_call_count = expected_system_call_count;
        first_interface = expected_first_interface;
        test_system_command_call_count_init(expected_call_count);
        test_system_command_result_set(system_command_result);
    }

    void check_system_commands()
    {
        size_t i;

        LONGS_EQUAL(expected_call_count, test_system_command_call_count_get());
        for (i = 0; i < expected_call_count; i++)
        {
            char * command = NULL;

            if ((i & 1) == 0)   /* The first call in a pair should always be the one to add a bridge. */
            {
                asprintf(&command, "/usr/sbin/brctl addbr %s%zu", test_interface_name, first_interface + (i / 2));
            }
            else    /* then there is a call to set the forwarding delay if the previous system command didn't result in an error */
            {
                asprintf(&command, "/usr/sbin/brctl setfd %s%zu 0", test_interface_name, first_interface + (i / 2));
            }
            STRCMP_EQUAL(command, test_system_command_get(i));
            free(command);
        }
    }

    void perform_test(size_t num_interfaces, size_t first_instance, size_t expected_system_calls, int system_command_result, bool expected_result)
    {
        bool result;

        init(first_instance, expected_system_calls, system_command_result);

        /* perform test*/
        result = bridge_interfaces_create(first_instance, num_interfaces);

        /* check result */
        LONGS_EQUAL(expected_result, result);
        check_system_commands();
    }


};

TEST(interfaces_create, create_zero_interfaces_succeeds)
{
    perform_test(0, 1, 0, 0, true);
}

TEST(interfaces_create, create_1_interface)
{
    perform_test(1, 1, 2, 0, true);
}

TEST(interfaces_create, create_multiple_interfaces)
{
    perform_test(3, 1, 6, 0, true);
}

TEST(interfaces_create, failed_system_command_results_in_error)
{
    perform_test(1, 1, 1, -1, false);
}

TEST(interfaces_create, bails_after_first_failure)
{
    perform_test(2, 1, 1, -1, false);
}

TEST(interfaces_create, command_honours_first_interface)
{
    perform_test(1, 2, 2, 0, true);
}

TEST_GROUP(interface_add)
{
    size_t expected_call_count;
    size_t first_interface;
    size_t expected_bridge_instance;
    char const * expected_interface_name;

    void setup()
    {
        test_bridge_interface_name_set(test_interface_name);
        expected_call_count = 1;
    }

    void teardown()
    {
        test_system_commands_cleanup();
    }

    void init(size_t bridge_instance, int system_command_result, char const * name_of_interface_to_add)
    {
        expected_bridge_instance = bridge_instance;
        expected_interface_name = name_of_interface_to_add;
        test_system_command_call_count_init(expected_call_count);
        test_system_command_result_set(system_command_result);
    }

    void check_system_command()
    {
        size_t i;

        LONGS_EQUAL(expected_call_count, test_system_command_call_count_get());
        for (i = 0; i < expected_call_count; i++)
        {
            char * command = NULL;

            asprintf(&command, "/usr/sbin/brctl addif %s%zu %s", test_interface_name, expected_bridge_instance, expected_interface_name);
            STRCMP_EQUAL(command, test_system_command_get(i));
            free(command);
        }
    }

    void perform_test(size_t bridge_instance, int system_command_result, char const * const name_of_interface_to_add, bool expected_result)
    {
        bool result;

        init(bridge_instance, system_command_result, name_of_interface_to_add);

        /* perform test*/
        result = bridge_interface_add_child_interfaces(bridge_instance, name_of_interface_to_add);

        /* check result */
        LONGS_EQUAL(expected_result, result);
        check_system_command();
    }

};

TEST(interface_add, base_test)
{
    perform_test(1, 0, "add_interface", true);
}

TEST(interface_add, honours_bridge_instance)
{
    perform_test(2, 0, "add_interface", true);
}

TEST(interface_add, fails_with_failed_system_command)
{
    perform_test(1, -1, "add_interface", false);
}

TEST(interface_add, honours_change_in_interface_name)
{
    perform_test(1, 0, "another_interface", true);
}

TEST_GROUP(interface_delete)
{
    size_t expected_call_count;
    size_t first_interface;
    size_t expected_bridge_instance;
    char const * expected_interface_name;

    void setup()
    {
        test_bridge_interface_name_set(test_interface_name);
        expected_call_count = 1;
    }

    void teardown()
    {
        test_system_commands_cleanup();
    }

    void init(size_t bridge_instance, int system_command_result, char const * name_of_interface_to_delete)
    {
        expected_bridge_instance = bridge_instance;
        expected_interface_name = name_of_interface_to_delete;
        test_system_command_call_count_init(expected_call_count);
        test_system_command_result_set(system_command_result);
    }

    void check_system_command()
    {
        size_t i;

        LONGS_EQUAL(expected_call_count, test_system_command_call_count_get());
        for (i = 0; i < expected_call_count; i++)
        {
            char * command = NULL;

            asprintf(&command, "/usr/sbin/brctl delif %s%zu %s", test_interface_name, expected_bridge_instance, expected_interface_name);
            STRCMP_EQUAL(command, test_system_command_get(i));
            free(command);
        }
    }

    void perform_test(size_t bridge_instance, int system_command_result, char const * const name_of_interface_to_add, bool expected_result)
    {
        bool result;

        init(bridge_instance, system_command_result, name_of_interface_to_add);

        /* perform test*/
        result = bridge_interface_delete_child_interfaces(bridge_instance, name_of_interface_to_add);

        /* check result */
        LONGS_EQUAL(expected_result, result);
        check_system_command();
    }

};

TEST(interface_delete, base_test)
{
    perform_test(1, 0, "delete_interface", true);
}

TEST(interface_delete, honours_bridge_instance)
{
    perform_test(2, 0, "delete_interface", true);
}

TEST(interface_delete, fails_with_failed_system_command)
{
    perform_test(1, -1, "delete_interface", false);
}

TEST(interface_delete, honours_change_in_interface_name)
{
    perform_test(1, 0, "another_interface", true);
}

