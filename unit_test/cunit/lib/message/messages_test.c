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

#include <messages.h>
#include <messages_private.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#define NB_ELEMENTS(array)  (sizeof((array))/sizeof((array[0])))
static bool callback_called;
static size_t const expected_instance = 2345;
static char const * expected_instance_str = "2345";
static size_t const bad_instance = 1111;
static bool got_expected_instance_str;
static int instance_passed_to_callback;
static size_t info_size_passed;
static update_info_st update_info;
static interfaces_t const unexpected_config_group = config_serial;
static interfaces_t const expected_config_group = config_eth;
static char const * const expected_config_group_str = "eth";
static size_t const expected_group_instance = 4567;
static size_t const unexpected_group_instance = 7654;
static message_codes_t message_code_to_return;
static bool get_instance_return_value;

static int init_suite(void)
{
    return 0;
}

static int clean_suite(void)
{
    return 0;
}

/* stubs */
int get_update_info(int const argc, char ** const argv, void ** const interface_info_result, size_t * const interface_info_size)
{
    update_info_st * info;
    info = *interface_info_result = calloc(1, sizeof *info);
    *interface_info_size = sizeof *info;
    info->config_group = expected_config_group;
    info->instance = expected_group_instance;

    return 0;
}

int get_interface_info(int const argc, char ** const argv, void ** const interface_info_result, size_t * const interface_info_size)
{
    return 0;
}

int get_ping_message_info(int const argc, char ** const argv, void ** const interface_info_result, size_t * const interface_info_size)
{
    return 0;
}

int get_pong_message_info(int const argc, char ** const argv, void ** const interface_info_result, size_t * const interface_info_size)
{
    return 0;
}

int get_dhcp_assigned_gateway_info(int const argc, char ** const argv, void ** const info_result, size_t * const info_size)
{
    return 0;
}

int dprintf(int fd, const char * __fmt, ...)
{
    return 0;
}

bool get_instance_from_string(char const * const str, size_t * const instance)
{
    if (strcmp(str, expected_instance_str) == 0)
    {
        got_expected_instance_str = true;
    }
    *instance = expected_instance;

    return get_instance_return_value;
}

int get_global_message_code_from_name(char const * const name)
{
    return message_code_to_return;
}
/************* Test case functions ****************/
static int parse_message_callback(int fd, size_t const instance, message_codes_t const message_code, void * * const info, size_t const info_size)
{
    instance_passed_to_callback = instance;
    callback_called = true;
    info_size_passed = info_size;
    if (*info != NULL && info_size == sizeof update_info)
    {
        update_info_st * p = *info;
        update_info = *p;
    }
    return 0;
}

/******* Test Cases ******/
extern void parse_message(int fd, char * const line, message_handler_info_st const * const info);

static void test_parse_message(void)
{
    message_handler_info_st const info = { .message_callback = parse_message_callback, .sock = 0 };
    char test_line[100];

    /* setup */
    instance_passed_to_callback = bad_instance;
    callback_called = false;
    got_expected_instance_str = false;
    snprintf(test_line, sizeof test_line, "%zu update", expected_instance);
    get_instance_return_value = true;
    /* perform test */
    parse_message(0, test_line, &info);
    /* check result */
    CU_ASSERT_TRUE(callback_called);
    CU_ASSERT_TRUE(got_expected_instance_str);
    CU_ASSERT_EQUAL(instance_passed_to_callback, expected_instance);
}

static void test_parse_message_update(void)
{
    /* test that the callback gets called with the expected information when parse_message() is
     * called with an update message.
     */
    message_handler_info_st const info = { .message_callback = parse_message_callback, .sock = 0 };
    char test_line[100];

    /* setup */
    message_code_to_return = update_message;
    update_info.config_group = unexpected_config_group;
    update_info.instance = unexpected_group_instance;
    snprintf(test_line, sizeof test_line, "1 update %s %zu", expected_config_group_str, expected_group_instance);
    get_instance_return_value = true;
    /* perform test */
    parse_message(0, test_line, &info);
    /* check result */
    /* update_info should have been updated by the callback function */
    CU_ASSERT_EQUAL(update_info.config_group, expected_config_group);
    CU_ASSERT_EQUAL(update_info.instance, expected_group_instance);
}

static void test_parse_message_too_few_args(void)
{
    message_handler_info_st const info = { .message_callback = parse_message_callback, .sock = 0 };
    char test_line[100];

    /* setup */
    instance_passed_to_callback = bad_instance;
    callback_called = false;
    got_expected_instance_str = false;
    snprintf(test_line, sizeof test_line, "%zu", expected_instance);
    get_instance_return_value = true;
    /* perform test */
    parse_message(0, test_line, &info);
    /* check result */
    CU_ASSERT_FALSE(callback_called);
}

static void test_parse_message_bad_instance(void)
{
    message_handler_info_st const info = { .message_callback = parse_message_callback, .sock = 0 };
    char test_line[100];

    /* setup */
    instance_passed_to_callback = bad_instance;
    callback_called = false;
    got_expected_instance_str = false;
    snprintf(test_line, sizeof test_line, "bad_instance update");
    get_instance_return_value = false;
    /* perform test */
    parse_message(0, test_line, &info);
    /* check result */
    CU_ASSERT_FALSE(callback_called);
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
    pSuite = CU_add_suite("libmessage_messages_test_suite", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ( 0
         || (CU_add_test(pSuite, "test_parse_message", test_parse_message) == NULL)
         || (CU_add_test(pSuite, "test_parse_message_update", test_parse_message_update) == NULL)
         || (CU_add_test(pSuite, "test_parse_message_too_few_args", test_parse_message_too_few_args) == NULL)
         || (CU_add_test(pSuite, "test_parse_message_bad_instance", test_parse_message_bad_instance) == NULL)
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



