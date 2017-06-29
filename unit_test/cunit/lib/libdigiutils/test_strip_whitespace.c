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

#include "test_strip_whitespace.h"
#include "strip_whitespace.h"
#include "tlr_common.h"
#include "CUnit/CUnit.h"
#include <string.h>

static void test_strip_whitespace(void)
{
    char * expected_value;
    char * value_to_test;
    char test_string[20];

    /* test empty string */
    /* prepare */
    strcpy(test_string, "");
    expected_value = "";
    /* test */
    value_to_test = strip_whitespace(test_string);
    /* check result */
    CU_ASSERT_STRING_EQUAL(value_to_test, expected_value);

    /* test string with no whitespace at beginning or end */
    /* prepare */
    strcpy(test_string, "123");
    expected_value = "123";
    /* test */
    value_to_test = strip_whitespace(test_string);
    /* check result */
    CU_ASSERT_STRING_EQUAL(value_to_test, expected_value);

    /* test string with whitespace at beginning */
    /* prepare */
    strcpy(test_string, " \t123");
    expected_value = "123";
    /* test */
    value_to_test = strip_whitespace(test_string);
    /* check result */
    CU_ASSERT_STRING_EQUAL(value_to_test, expected_value);

    /* test string with whitespace at end */
    /* prepare */
    strcpy(test_string, "123 \t");
    expected_value = "123";
    /* test */
    value_to_test = strip_whitespace(test_string);
    /* check result */
    CU_ASSERT_STRING_EQUAL(value_to_test, expected_value);

    /* test string with whitespace at beginning and end */
    /* prepare */
    strcpy(test_string, " \t123\t ");
    expected_value = "123";
    /* test */
    value_to_test = strip_whitespace(test_string);
    /* check result */
    CU_ASSERT_STRING_EQUAL(value_to_test, expected_value);
}

static int init_suite(void)
{
    return 0;
}

static int clean_suite(void)
{
    return 0;
}

#define CU_TEST_INFO(test) { #test, test }

static CU_TestInfo tests[] =
{
    CU_TEST_INFO(test_strip_whitespace),
    CU_TEST_INFO_NULL,
};
CU_SuiteInfo test_strip_whitespace_suites[] =
{
    { "strip_whitespace", init_suite, clean_suite, tests },
    CU_SUITE_INFO_NULL,
};
