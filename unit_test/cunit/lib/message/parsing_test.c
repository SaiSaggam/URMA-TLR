/*****************************************************************************
 * Copyright (c) 2015 Digi International Inc., All Rights Reserved
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
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include <parsing.h>
#include <messages.h>
#include <config_mgr.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

static int init_suite(void)
{
    return 0;
}

static int clean_suite(void)
{
    return 0;
}

/* stubs */
interfaces_t interface_get_type_from_name(char const * const name)
{
    return first_valid_interface;
}

message_destination_t get_message_destination_from_monitor_name(char const * const monitor_name)
{
    return ethernet_monitor;
}

ssize_t tlr_get_group_id(char const * const group_name)
{
    return 0;
}

void const * tlr_get_table(config_mgr_table_type_t const type)
{
	return NULL;
}

/************* Test case functions ****************/

/******* Test Cases ******/
static void test_get_unsigned_integer_from_string(void)
{
    extern bool get_unsigned_integer_from_string(char const * const str, unsigned int * const instance);
    static char const negative_number[] = "-1";
    static char const invalid_positive_number[] = "4294967296000000000000000000";
    static char const zero[] = "0";
    static size_t zero_value = 0;
    static char const valid_positive_number[] = "2";
    static size_t valid_positive_value = 2;

    char const * string_to_test;
    bool expected_result;
    bool result;
    unsigned int unsigned_integer_storage;

    /* setup */
    string_to_test = negative_number;
    expected_result = false;
    /* perform test */
    result = get_unsigned_integer_from_string(string_to_test, &unsigned_integer_storage);
    /* check result */
    CU_ASSERT_EQUAL(result, expected_result);

    /* setup */
    string_to_test = zero;
    expected_result = true;
    /* perform test */
    result = get_unsigned_integer_from_string(string_to_test, &unsigned_integer_storage);
    /* check result */
    CU_ASSERT_EQUAL(result, expected_result);
    CU_ASSERT_EQUAL(unsigned_integer_storage, zero_value);

    /* setup */
    string_to_test = valid_positive_number;
    expected_result = true;
    /* perform test */
    result = get_unsigned_integer_from_string(string_to_test, &unsigned_integer_storage);
    /* check result */
    CU_ASSERT_EQUAL(result, expected_result);
    CU_ASSERT_EQUAL(unsigned_integer_storage, valid_positive_value);

    /* setup */
    string_to_test = invalid_positive_number;
    expected_result = false;
    /* perform test */
    result = get_unsigned_integer_from_string(string_to_test, &unsigned_integer_storage);
    /* check result */
    CU_ASSERT_EQUAL(result, expected_result);
}

static void test_get_interface_info(void)
{
    int const not_enough_args_argc = 1;
    char * not_enough_args_argv[] = {"eth", NULL};
    int const enough_args_argc = 2;
    char * enough_args_argv[] = {"eth", "1", NULL};
    void * info_result;
    size_t info_size;
    int result;

    /*setup */
    info_result = NULL;
    /* perform test */
    result = get_interface_info(not_enough_args_argc, not_enough_args_argv, &info_result, &info_size);
    /*check result*/
    CU_ASSERT_EQUAL(result, -1);

    /*setup */
    info_result = NULL;
    /* perform test */
    result = get_interface_info(enough_args_argc, enough_args_argv, &info_result, &info_size);
    /*check result*/
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_PTR_NOT_NULL(info_result);
    CU_ASSERT_TRUE(info_size > 0);
    /* cleanup */
    free(info_result);
}

static void test_get_ping_message_info(void)
{
    int const not_enough_args_argc = 1;
    char * not_enough_args_argv[] = {"ethmon", NULL};
    int const enough_args_argc = 2;
    char * enough_args_argv[] = {"ethmon", "1", NULL};
    void * info_result;
    size_t info_size;
    int result;

    /*setup */
    info_result = NULL;
    /* perform test */
    result = get_ping_message_info(not_enough_args_argc, not_enough_args_argv, &info_result, &info_size);
    /*check result*/
    CU_ASSERT_EQUAL(result, -1);

    /*setup */
    info_result = NULL;
    /* perform test */
    result = get_ping_message_info(enough_args_argc, enough_args_argv, &info_result, &info_size);
    /*check result*/
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_PTR_NOT_NULL(info_result);
    CU_ASSERT_TRUE(info_size > 0);
    /* cleanup */
    free(info_result);
}

static void test_get_pong_message_info(void)
{
    int const not_enough_args_argc = 0;
    char * not_enough_args_argv[] = {NULL};
    int const enough_args_argc = 1;
    char * enough_args_argv[] = {"1", NULL};
    void * info_result;
    size_t info_size;
    int result;

    /*setup */
    info_result = NULL;
    /* perform test */
    result = get_pong_message_info(not_enough_args_argc, not_enough_args_argv, &info_result, &info_size);
    /*check result*/
    CU_ASSERT_EQUAL(result, -1);

    /*setup */
    info_result = NULL;
    /* perform test */
    result = get_pong_message_info(enough_args_argc, enough_args_argv, &info_result, &info_size);
    /*check result*/
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_PTR_NOT_NULL(info_result);
    CU_ASSERT_TRUE(info_size > 0);
    /* cleanup */
    free(info_result);
}

static void test_get_update_info(void)
{
    int const not_enough_args_argc = 1;
    char * not_enough_args_argv[] = {"eth", NULL};
    int const enough_args_argc = 2;
    char * enough_args_argv[] = {"eth", "1", NULL};
    void * info_result;
    size_t info_size;
    int result;

    /*setup */
    info_result = NULL;
    /* perform test */
    result = get_update_info(not_enough_args_argc, not_enough_args_argv, &info_result, &info_size);
    /*check result*/
    CU_ASSERT_EQUAL(result, -1);

    /*setup */
    info_result = NULL;
    /* perform test */
    result = get_update_info(enough_args_argc, enough_args_argv, &info_result, &info_size);
    /*check result*/
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_PTR_NOT_NULL(info_result);
    CU_ASSERT_TRUE(info_size > 0);
    /* cleanup */
    free(info_result);
}

static void test_get_dhcp_assigned_gateway_info(void)
{
    void * info_result;
    size_t info_size;
    struct test_info
    {
        dhcp_assigned_gateway_st expected;
        char * const argv[3];
        int const argc;
        int const expected_ccode;
        size_t const expected_info_size;
        bool const expected_info_is_null;
    };
    struct test_info const tests[] =
    {
        {
            {
                config_eth,
                1,
                "1.2.3.4"
            },
            {"wan", "eth1", "1.2.3.4"},
            3,
            0,
            sizeof(dhcp_assigned_gateway_st),
            false
        },
        {
            {
                config_eth,
                1,
                "1.2.3.4"
            },
            {"wan", "eth1"},
            2,
            -1,
            sizeof(dhcp_assigned_gateway_st),
            true
        },
        {
            {
                config_cellular,
                1,
                "1.2.3.4"
            },
            {"wan", "cellular1", "1.2.3.4"},
            3,
            0,
            sizeof(dhcp_assigned_gateway_st),
            false
        },
        {
            {
                config_cellular,
                1,
                "255.255.255.255"
            },
            {"wan", "cellular1", "255.255.255.255"},
            3,
            0,
            sizeof(dhcp_assigned_gateway_st),
            false
        }
    };
    const unsigned int number_tests = sizeof(tests)/sizeof(tests[0]);
    unsigned int test_index;

    for (test_index = 0; test_index < number_tests; test_index++)
    {
        struct test_info const * const test = &tests[test_index];
        int result;

        info_result = NULL;
        result = get_dhcp_assigned_gateway_info(test->argc, (char ** const) test->argv, &info_result, &info_size);
        CU_ASSERT_EQUAL(result, test->expected_ccode);
        if (test->expected_info_is_null)
        {
            CU_ASSERT_PTR_NULL(info_result);
        }
        else
        {
            struct dhcp_assigned_gateway_t * ginfo = (struct dhcp_assigned_gateway_t *) info_result;

            CU_ASSERT_PTR_NOT_NULL(info_result);
            printf("info_size = %zu, test->expected_info_size = %zu\n", info_size, test->expected_info_size);
            CU_ASSERT_TRUE(info_size == test->expected_info_size);
            printf("ginfo->instance = %u, test->expected.instance = %u\n", ginfo->instance, test->expected.instance);
            CU_ASSERT_TRUE(ginfo->instance == test->expected.instance);
            printf("ginfo->gateway = %s, test->expected.gateway = %s\n", ginfo->gateway, test->expected.gateway);
            CU_ASSERT_TRUE(strcmp(ginfo->gateway, test->expected.gateway) == 0);
        }
    }

}

/************* Test Runner Code goes here **************/

int main(void)
{
    int number_of_failures;
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("libmessage_parsing_test_suite", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ( 0
         || (CU_add_test(pSuite, "test_get_unsigned_integer_from_string", test_get_unsigned_integer_from_string) == NULL)
         || (CU_add_test(pSuite, "test_get_interface_info", test_get_interface_info) == NULL)
         || (CU_add_test(pSuite, "test_get_ping_message_info", test_get_ping_message_info) == NULL)
         || (CU_add_test(pSuite, "test_get_pong_message_info", test_get_pong_message_info) == NULL)
         || (CU_add_test(pSuite, "test_get_update_info", test_get_update_info) == NULL)
         || (CU_add_test(pSuite, "test_get_dhcp_assigned_gateway_info", test_get_dhcp_assigned_gateway_info) == NULL)
         )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");

    //	Clean up registry and return
    number_of_failures = CU_get_number_of_failures();
    CU_cleanup_registry();

    //	Ensure we return an error if any tests failed
    if (number_of_failures)
        return -1;

    return CU_get_error();
}



