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

#include "dsl_version.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdarg.h>

//static int const dummy_file;
//static FILE * popen_return_value = (FILE *)&dummy_file;
//static char * getline_lineptr_value;
//static ssize_t getline_return_value;
//static FILE * file_ptr_passed_to_getline;
static char const * filename_requested_to_open;
//static FILE * file_pointer_passed_to_pclose;
#define GOOD_SHOW_DSL_FILE 0
#define BAD_SHOW_DSL_FILE 1
#define NON_SHOW_DSL_FILE 3
#define good_show_filename "good_show_dsl.txt"
#define bad_show_filename "bad_show_dsl.txt"
#define non_show_filename "non_show_dsl.txt"
static int which_dsl_file;

static int init_suite(void)
{
    return 0;
}

static int clean_suite(void)
{
    return 0;
}

/* stubs */

FILE *popen(const char  * fn, const char  * m)
{
    switch (which_dsl_file)
    {
        case GOOD_SHOW_DSL_FILE:
            filename_requested_to_open = good_show_filename;
            break;
            
        case BAD_SHOW_DSL_FILE:
            filename_requested_to_open = bad_show_filename;
            break;
         
        case NON_SHOW_DSL_FILE:
            filename_requested_to_open = non_show_filename;
            break;      
    }
    
	return fopen(filename_requested_to_open, m);
}

int pclose(FILE *fp)
{
    //file_pointer_passed_to_pclose = fp;
    fclose(fp);
    return 0;
}

#if 0
ssize_t getline (char **lineptr, size_t *n, FILE *stream)
{
    file_ptr_passed_to_getline = stream;
    *lineptr = strdup(getline_lineptr_value);

    return getline_return_value;
}
#endif

/************* Test case functions ****************/

/******* Test Cases ******/
static void test_get_tlr_dsl_version(void)
{
    #define TEMP_BUFFER_SIZE 100
    int result;
//    size_t const buf_size = TEMP_BUFFER_SIZE;
//    char buf[TEMP_BUFFER_SIZE];
    #define EXPECTED_DSL_VERSION_STRING "3.22.14.0_A60394"
 //   char * string_with_newline = "FW Version       :" EXPECTED_DSL_VERSION_STRING "\n";
//    char * string_without_newline = "FW Version       :" EXPECTED_DSL_VERSION_STRING;
    
    #define EXPECTED_FAILED_DSL_VERSION_STRING "Not available"
//    char * failed_string_with_newline = "FW Version       :" "\n";
 //   char * failed_string_without_newline = "FW Version       :";
    
    char const * dsl_version_returned;
    printf("\n");

    /* setup */
    which_dsl_file = GOOD_SHOW_DSL_FILE;
    dsl_version_returned = NULL;
    /* perform test */
    result = get_tlr_dsl_version(&dsl_version_returned);
    //printf("result = %d, %s\n", result, dsl_version_returned);
    /* check results */
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_STRING_EQUAL(dsl_version_returned, EXPECTED_DSL_VERSION_STRING);
    /* cleanup */
    filename_requested_to_open = NULL;
    
    
    /* setup */
    which_dsl_file = BAD_SHOW_DSL_FILE;
    dsl_version_returned = NULL;
    /* perform test */
    result = get_tlr_dsl_version(&dsl_version_returned);
    //printf("result = %d, %s\n", result, dsl_version_returned);
    /* check results */
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_STRING_EQUAL(dsl_version_returned, EXPECTED_FAILED_DSL_VERSION_STRING);
    /* cleanup */
    filename_requested_to_open = NULL;
    
    /* setup */
    which_dsl_file = NON_SHOW_DSL_FILE;
    dsl_version_returned = NULL;
    /* perform test */
    result = get_tlr_dsl_version(&dsl_version_returned);
    //printf("result = %d, %s\n", result, dsl_version_returned);
    /* check results */
    CU_ASSERT_EQUAL(result, -1);
    CU_ASSERT_EQUAL(dsl_version_returned, NULL);
    /* cleanup */
    filename_requested_to_open = NULL;
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
         || (CU_add_test(pSuite, "test_get_tlr_dsl_version", test_get_tlr_dsl_version) == NULL)
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



