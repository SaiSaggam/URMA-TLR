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

#include <args.h>
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

/************* Test case functions ****************/

/******* Test Cases ******/
static void test_add_args(void)
{
    args_st * args;
    static char const * const string1 = "string1";
    static char const * const string2 = "string2";
    int result;

    /* test adding a single arg */
    /* setup */
    args = malloc_args_st(1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(args);
    /* perform test */
    result = add_arg(args, string1);
    /* check result */
    CU_ASSERT_EQUAL_FATAL(result, 0);
    CU_ASSERT_EQUAL(args->argc, 1);
    CU_ASSERT_STRING_EQUAL(args->argv[0], string1);
    /* cleanup */
    free_args_st(args);

    /* test adding multiple args */
    /* setup */
    args = malloc_args_st(1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(args);
    /* perform test */
    result = add_arg(args, string1);
    result += add_arg(args, string2);
    /* check result */
    CU_ASSERT_EQUAL_FATAL(result, 0);
    CU_ASSERT_EQUAL(args->argc, 2);
    CU_ASSERT_STRING_EQUAL(args->argv[0], string1);
    CU_ASSERT_STRING_EQUAL(args->argv[1], string2);
    /* cleanup */
    free_args_st(args);

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
    pSuite = CU_add_suite("libmessage_args_test_suite", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ( 0
         || (CU_add_test(pSuite, "test_add_args", test_add_args) == NULL)
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



