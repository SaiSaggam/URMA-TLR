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

#include "cellular.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdarg.h>
#include <config_def.h>
#include <strlcpy.h>

extern int get_cellular_item(size_t index_3g, config_state_cellular_t item, char * const value, size_t value_size);

static size_t index_3g_passed;
static config_state_cellular_t item_passed;
static char * buffer_passed;
static char buffer_size_passed;
static int cellular_read_status_return_value;
static char * value_to_return;
static int total_sleep_time;
static int cellular_open_status_return_value;

static int init_suite(void)
{
    return 0;
}

static int clean_suite(void)
{
    return 0;
}

/* stubs */
unsigned int sleep(unsigned int seconds)
{
    total_sleep_time += seconds;
    return 0;
}

int cellular_open_status(void)
{
    return cellular_open_status_return_value;
}

void cellular_close_status(int fd)
{
    return;
}

int cellular_read_status(int const fd, size_t const index_3g, config_state_cellular_t const sid, void * value, size_t const value_size)
{
    index_3g_passed = index_3g;
    item_passed = sid;
    buffer_passed = value;
    buffer_size_passed = value_size;

    if (cellular_read_status_return_value == 0)
    {
        strlcpy(value, value_to_return, value_size);
    }

    return cellular_read_status_return_value;
}

/************* Test case functions ****************/

/******* Test Cases ******/
static void test_get_cellular_item(void)
{
    int result;
    config_state_cellular_t item_to_get;
    config_state_cellular_t expected_item_to_get;
    size_t index_3g;
    size_t expected_index_3g_value;
    char buffer[100];   /* use a buffer large enough to hold our expected values */

    /* test that index_3g, item, buffer and buffer size are all passed to cellular_read_status */
    /* setup */
    item_to_get = expected_item_to_get = 4567;  /* any old unlikely to stumble upon number */
    index_3g = expected_index_3g_value = 1234;  /* any old unlikely to stumble upon number */
    buffer_passed = NULL;
    buffer_size_passed = 0;
    index_3g_passed = 0;
    item_passed = 0;
    value_to_return = "test value";
    total_sleep_time = 0;
    cellular_open_status_return_value = 0;
    cellular_read_status_return_value = 0;
    buffer[0] = '\0';
    /* perform test */
    result = get_cellular_item(index_3g, item_to_get, buffer, sizeof buffer);
    /* check result*/
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_EQUAL(index_3g_passed, expected_index_3g_value);
    CU_ASSERT_PTR_EQUAL(buffer_passed, buffer);
    CU_ASSERT_EQUAL(item_passed, expected_item_to_get);
    CU_ASSERT_STRING_EQUAL(buffer, value_to_return);
    CU_ASSERT_EQUAL(total_sleep_time, 0);

    /* test that the function doesn't wait forever if the requested item isn't available */
    /* setup */
    item_to_get = expected_item_to_get = 4567;  /* any old unlikely to stumble upon number */
    index_3g = expected_index_3g_value = 1234;  /* any old unlikely to stumble upon number */
    buffer_passed = NULL;
    buffer_size_passed = 0;
    index_3g_passed = 0;
    item_passed = 0;
    value_to_return = "";   /* return empty string, which is unexpected in normal cases */
    total_sleep_time = 0;
    cellular_open_status_return_value = 0;
    cellular_read_status_return_value = 0;
    buffer[0] = '\0';
    /* perform test */
    result = get_cellular_item(index_3g, item_to_get, buffer, sizeof buffer);
    /* check result*/
    CU_ASSERT_EQUAL(result, 0);
    printf("total_sleep_time = %d\n", total_sleep_time);
    CU_ASSERT_FALSE(total_sleep_time < MAX_SECONDS_TO_WAIT_FOR_CELLULAR);
    CU_ASSERT_STRING_EQUAL(buffer, VALUE_UNAVAILABLE);

    /* test that the function doesn't wait forever if the requested item isn't available */
    /* setup */
    item_to_get = expected_item_to_get = 4567;  /* any old unlikely to stumble upon number */
    index_3g = expected_index_3g_value = 1234;  /* any old unlikely to stumble upon number */
    buffer_passed = NULL;
    buffer_size_passed = 0;
    index_3g_passed = 0;
    item_passed = 0;
    value_to_return = " ";   /* return string with single space, which is unexpected in normal cases */
    total_sleep_time = 0;
    cellular_open_status_return_value = 0;
    cellular_read_status_return_value = 0;
    buffer[0] = '\0';
    /* perform test */
    result = get_cellular_item(index_3g, item_to_get, buffer, sizeof buffer);
    /* check result*/
    CU_ASSERT_EQUAL(result, 0);
    printf("total_sleep_time = %d\n", total_sleep_time);
    CU_ASSERT_FALSE(total_sleep_time < MAX_SECONDS_TO_WAIT_FOR_CELLULAR);
    CU_ASSERT_STRING_EQUAL(buffer, VALUE_UNAVAILABLE);

    /* test failed cellular_open_status */
    /* setup */
    buffer[0] = '\0';
    total_sleep_time = 0;
    cellular_open_status_return_value = -1;
    cellular_read_status_return_value = 0;
    /* perform test */
    result = get_cellular_item(index_3g, item_to_get, buffer, sizeof buffer);
    /* check result*/
    CU_ASSERT_EQUAL(result, 0);
    printf("total_sleep_time = %d\n", total_sleep_time);
    CU_ASSERT_FALSE(total_sleep_time < MAX_SECONDS_TO_WAIT_FOR_CELLULAR);
    CU_ASSERT_STRING_EQUAL(buffer, VALUE_UNAVAILABLE);

    /* test failed cellular_read_status */
    /* setup */
    buffer[0] = '\0';
    total_sleep_time = 0;
    cellular_open_status_return_value = 0;
    cellular_read_status_return_value = -1;
    /* perform test */
    result = get_cellular_item(index_3g, item_to_get, buffer, sizeof buffer);
    /* check result*/
    CU_ASSERT_EQUAL(result, 0);
    printf("total_sleep_time = %d\n", total_sleep_time);
    CU_ASSERT_FALSE(total_sleep_time < MAX_SECONDS_TO_WAIT_FOR_CELLULAR);
    CU_ASSERT_STRING_EQUAL(buffer, VALUE_UNAVAILABLE);
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
    pSuite = CU_add_suite("show_manufacture_test_suite", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ( 0
         || (CU_add_test(pSuite, "test_get_cellular_item", test_get_cellular_item) == NULL)
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



