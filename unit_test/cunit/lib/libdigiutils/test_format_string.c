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

#include "tlr_format_string.h"
#include "serial_number.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdarg.h>

static char * serial_number_to_use;
int get_serial_result;


static int init_suite(void)
{
    return 0;
}

static int clean_suite(void)
{
    return 0;
}

/* stubs */
int get_tlr_serial_number(char * * serial_number_ptr)
{
    *serial_number_ptr = serial_number_to_use;
    return get_serial_result;
}

/************* Test case functions ****************/

/******* Test Cases ******/
static void do_test(char const * const input_string,
                    char * const serial_number,
                    int const get_serial_result_to_use,
                    size_t const output_buffer_length,
                    char const * const expected_output_string,
                    int const expected_truncation_result)
{
    char *buffer;
    int actual_result;

    /* setup */
    buffer = malloc(output_buffer_length);
    CU_ASSERT_PTR_NOT_NULL_FATAL(buffer);
    serial_number_to_use = serial_number;
    get_serial_result = get_serial_result_to_use;

    /* perform test */
    actual_result = tlr_replace_percent_s_with_serial_number(input_string, buffer, output_buffer_length);
    fprintf(stderr, "output: %s\n", buffer);
    /* check result */
    CU_ASSERT_STRING_EQUAL(buffer, expected_output_string);
    CU_ASSERT_EQUAL(actual_result, expected_truncation_result);

    free(buffer);
}

static void test_empty_string_returns_empty_string(void)
{
    do_test("", "", 0, 10, "", 0);
    do_test("", "", 0, 1, "", 0);  /* ensure still OK when output length is only just long enough */
}

static void test_basic_string_returns_same_string(void)
{
    do_test("123", "", 0, 10, "123", 0);
    do_test("123", "", 0, 4, "123", 0);  /* ensure still OK when output length is only just long enough */
}

static void test_basic_string_truncated_if_output_too_small(void)
{
    do_test("1234", "", 0, 4, "123", 1);
}

static void test_string_with_percent_s_includes_serial_number(void)
{
    do_test("123%s", "serial", 0, 10, "123serial", 0);
    do_test("%s123", "serial", 0, 10, "serial123", 0);
    /* include an example of a 32 char string with serial number to simulate max length SSID */
    do_test("%s78901234567890123456789012", "serial", 0, 33, "serial78901234567890123456789012", 0);
}

static void test_string_with_percent_s_truncates_if_buffer_too_small(void)
{
    do_test("123%s", "serial", 0, 9, "123seria", 1);   /* part of serial number included */
    do_test("123%s", "serial", 0, 4, "123", 1);        /* no part of serial number should be included */
    do_test("%s", "serial", 0, 4, "ser", 1);           /* ensure truncation occurs correctly if buffer too small for serial number alone */
    do_test("%s123", "serial", 0, 8, "serial1", 1);    /* all of serial plus portion of trailing */
    /* include an example of truncating to a 32 char string to simulate max length SSID*/
    do_test("%s789012345678901234567890123456", "serial", 0, 33, "serial78901234567890123456789012", 1);
}

static void test_failed_get_serial_replaced_with_empty_string(void)
{
    do_test("123%s", "serial", -1, 9, "123", 0);
}

static void test_only_only_first_percent_s_replaced(void)
{
    do_test("123%s%s", "serial", 0, 12, "123serial%s", 0);
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
    pSuite = CU_add_suite("tlr_format_string", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ( 0
         || (CU_add_test(pSuite, "test_empty_string_returns_empty_string", test_empty_string_returns_empty_string) == NULL)
         || (CU_add_test(pSuite, "test_basic_string_returns_same_string", test_basic_string_returns_same_string) == NULL)
         || (CU_add_test(pSuite, "test_basic_string_truncated_if_output_too_small", test_basic_string_truncated_if_output_too_small) == NULL)
         || (CU_add_test(pSuite, "test_string_with_percent_s_includes_serial_number", test_string_with_percent_s_includes_serial_number) == NULL)
         || (CU_add_test(pSuite, "test_string_with_percent_s_truncates_if_buffer_too_small", test_string_with_percent_s_truncates_if_buffer_too_small) == NULL)
         || (CU_add_test(pSuite, "test_failed_get_serial_replaced_with_empty_string", test_failed_get_serial_replaced_with_empty_string) == NULL)
         || (CU_add_test(pSuite, "test_only_only_first_percent_s_replaced", test_only_only_first_percent_s_replaced) == NULL)
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



