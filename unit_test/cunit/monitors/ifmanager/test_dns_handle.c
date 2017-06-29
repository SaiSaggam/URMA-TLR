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
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define     USE_BASIC_STYLE     1
#define     USE_AUTOMATED_STYLE     0
#define     USE_CONSOLE_STYLE     0

#if USE_BASIC_STYLE
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#elif USE_AUTOMATED_STYLE
#include "CUnit/Automated.h"
#else
#include "CUnit/Console.h"
#endif

#include "dns_handle.h"
#include "dns_handle_private.h"
#include <stdlib.h>

/* Test Suite setup and cleanup functions: */

static int init_suite(void) { return 0; }
static int clean_suite(void) { return 0; }

/************* Test case functions ****************/
static void do_trim_trailing_white_test(char const * const string_to_test, char const * const expected_result)
{
    char * test_string;
    /* setup */
    test_string = strdup(string_to_test);
    CU_ASSERT_PTR_NOT_NULL_FATAL(test_string);

    /* perform test */
    trim_trailing_white(test_string);

    /* check results*/
    CU_ASSERT_STRING_EQUAL(test_string, expected_result);

    /* cleanup */
    free(test_string);
}

static void test_trim_trailing_white_with_no_trailing_whitepsace(void)
{
    do_trim_trailing_white_test("no_trailing_whitespace", "no_trailing_whitespace");
}

static void test_trim_trailing_white_with_trailing_whitepsace(void)
{
    do_trim_trailing_white_test("with_trailing_whitespace ", "with_trailing_whitespace");
}

static void test_trim_trailing_white_all_whitepsace(void)
{
    do_trim_trailing_white_test("  ", "");
}

static void test_trim_trailing_empty_string(void)
{
    do_trim_trailing_white_test("", "");
}

static void test_trim_trailing_leading_and_trailing_whitespace_just_has_trailing_removed(void)
{
    do_trim_trailing_white_test(" some_string ", " some_string");
}

static void do_get_interface_and_metric_from_line_test(char const * const string_to_test,
                                                       ROUTE_TYPE route_type,
                                                       bool const expected_result,
                                                       int expected_metric,
                                                       char const * const expected_ifname)
{
    char * test_string;
    bool actual_result;
    int metric;
    char * ifname = NULL;

    /* setup */
    test_string = strdup(string_to_test);
    CU_ASSERT_PTR_NOT_NULL_FATAL(test_string);

    /* perform test */
    actual_result = get_interface_and_metric_from_line(test_string, route_type, &ifname, &metric);

    /* check result */
    CU_ASSERT_EQUAL(actual_result, expected_result);
    if (expected_result)
    {
        CU_ASSERT_EQUAL(metric, expected_metric);
        CU_ASSERT_STRING_EQUAL(ifname, expected_ifname);
    }

    /* cleanup */
    free(ifname);
    free(test_string);
}

static void get_interface_and_metric_from_line_normal_default_route(void)
{
    do_get_interface_and_metric_from_line_test("0.0.0.0         0.0.0.0         0.0.0.0         U     3      0        0 eth1",
                                               DEFAULT_ROUTE,
                                               true,
                                               3,
                                               "eth1");
}

static void get_interface_and_metric_from_line_default_route_not_matched(void)
{
    do_get_interface_and_metric_from_line_test("1.2.3.4         0.0.0.0         0.0.0.0         U     3      0        0 eth1",
                                               DEFAULT_ROUTE,
                                               false,
                                               0,
                                               NULL);
}

static void get_interface_and_metric_from_line_non_default_route_matches_default_route(void)
{
    do_get_interface_and_metric_from_line_test("0.0.0.0         0.0.0.0         0.0.0.0         U     3      0        0 eth1",
                                               ANYTHING_ROUTE,
                                               true,
                                               3,
                                               "eth1");
}

static void get_interface_and_metric_from_line_normal_non_default_route(void)
{
    do_get_interface_and_metric_from_line_test("1.2.3.4         0.0.0.0         0.0.0.0         U     3      0        0 eth1",
                                               ANYTHING_ROUTE,
                                               true,
                                               3,
                                               "eth1");
}

static void get_interface_and_metric_from_line_metric_not_int(void)
{
    do_get_interface_and_metric_from_line_test("0.0.0.0         0.0.0.0         0.0.0.0         U     metric_not_an_int      0        0 eth1",
                                               DEFAULT_ROUTE,
                                               false,
                                               0,
                                               NULL);

}

static void get_interface_and_metric_from_line_too_few_fields_before_metric_results_in_failure(void)
{
    do_get_interface_and_metric_from_line_test("0.0.0.0         0.0.0.0         U     3      0        0 eth1",
                                               DEFAULT_ROUTE,
                                               false,
                                               0,
                                               NULL);

}

static void get_interface_and_metric_from_line_too_few_fields_before_interface_results_in_failure(void)
{
    do_get_interface_and_metric_from_line_test("0.0.0.0         0.0.0.0    0.0.0.0     U     3      0 eth1",
                                               DEFAULT_ROUTE,
                                               false,
                                               0,
                                               NULL);

}

static void get_interface_and_metric_from_line_additional_fields_after_interfaces_ignored(void)
{
    do_get_interface_and_metric_from_line_test("0.0.0.0         0.0.0.0    0.0.0.0     U     3   0   0 eth1 something_additional",
                                               DEFAULT_ROUTE,
                                               true,
                                               3,
                                               "eth1");

}

static void get_interface_and_metric_from_line_one_word_string_results_in_failure(void)
{
    do_get_interface_and_metric_from_line_test("single_word",
                                               DEFAULT_ROUTE,
                                               false,
                                               0,
                                               NULL);

}

/************* Test Runner Code goes here **************/

int add_dns_handle_tests(void)
{
    CU_pSuite pSuite = NULL;

    /* add a suite to the registry */
    pSuite = CU_add_suite( "dns_handle_test_suite", init_suite, clean_suite );
    if (pSuite == NULL)
        goto error;

    /* add the tests to the suite */
    if (CU_add_test(pSuite, "test_trim_trailing_white_wtih_no_trailing_whitepsace", test_trim_trailing_white_with_no_trailing_whitepsace) == NULL)
    {
        goto error;
    }
    if (CU_add_test(pSuite, "test_trim_trailing_white_wtih_trailing_whitepsace", test_trim_trailing_white_with_trailing_whitepsace) == NULL)
    {
        goto error;
    }
    if (CU_add_test(pSuite, "test_trim_trailing_white_all_whitepsace", test_trim_trailing_white_all_whitepsace) == NULL)
    {
        goto error;
    }
    if (CU_add_test(pSuite, "test_trim_trailing_empty_string", test_trim_trailing_empty_string) == NULL)
    {
        goto error;
    }
    if (CU_add_test(pSuite, "test_trim_trailing_leading_and_trailing_whitespace_just_has_trailing_removed", test_trim_trailing_leading_and_trailing_whitespace_just_has_trailing_removed) == NULL)
    {
        goto error;
    }

    if (CU_add_test(pSuite, "get_interface_and_metric_from_line_normal_default_route", get_interface_and_metric_from_line_normal_default_route) == NULL)
    {
        goto error;
    }
    if (CU_add_test(pSuite, "get_interface_and_metric_from_line_normal_non_default_route", get_interface_and_metric_from_line_normal_non_default_route) == NULL)
    {
        goto error;
    }
    if (CU_add_test(pSuite, "get_interface_and_metric_from_line_metric_not_int", get_interface_and_metric_from_line_metric_not_int) == NULL)
    {
        goto error;
    }
    if (CU_add_test(pSuite, "get_interface_and_metric_from_line_too_few_fields_before_metric_results_in_failure", get_interface_and_metric_from_line_too_few_fields_before_metric_results_in_failure) == NULL)
    {
        goto error;
    }
    if (CU_add_test(pSuite, "get_interface_and_metric_from_line_too_few_fields_before_interface_results_in_failure", get_interface_and_metric_from_line_too_few_fields_before_interface_results_in_failure) == NULL)
    {
        goto error;
    }
    if (CU_add_test(pSuite, "get_interface_and_metric_from_line_additional_fields_after_interfaces_ignored", get_interface_and_metric_from_line_additional_fields_after_interfaces_ignored) == NULL)
    {
        goto error;
    }
    if (CU_add_test(pSuite, "get_interface_and_metric_from_line_one_word_string_results_in_failure", get_interface_and_metric_from_line_one_word_string_results_in_failure) == NULL)
    {
        goto error;
    }
    if (CU_add_test(pSuite, "get_interface_and_metric_from_line_default_route_not_matched", get_interface_and_metric_from_line_default_route_not_matched) == NULL)
    {
        goto error;
    }
    if (CU_add_test(pSuite, "get_interface_and_metric_from_line_non_default_route_matches_default_route", get_interface_and_metric_from_line_non_default_route_matches_default_route) == NULL)
    {
        goto error;
    }

    goto done;

error:
    CU_cleanup_registry();

done:
    return CU_get_error();

}


