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

#include "firmware_checksum.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdarg.h>

static int const dummy_file;
static FILE * fopen_return_value = (FILE *)&dummy_file;
static char * getline_lineptr_value;
static ssize_t getline_return_value;
static FILE * file_ptr_passed_to_getline;
static char const * filename_requested_to_open;
static FILE * file_pointer_passed_to_fclose;

static int init_suite(void)
{
    return 0;
}

static int clean_suite(void)
{
    return 0;
}

/* stubs */

FILE *fopen(const char  * fn, const char  * m)
{
    filename_requested_to_open = strdup(fn);
	return fopen_return_value;
}

int fclose(FILE *fp)
{
    file_pointer_passed_to_fclose = fp;
    return 0;
}

ssize_t getline (char **lineptr, size_t *n, FILE *stream)
{
    file_ptr_passed_to_getline = stream;
    *lineptr = strdup(getline_lineptr_value);

    return getline_return_value;
}


/************* Test case functions ****************/

/******* Test Cases ******/
static void test_get_tlr_firmware_checksum(void)
{
    #define TEMP_BUFFER_SIZE 100
    int result;
    size_t const buf_size = TEMP_BUFFER_SIZE;
    char buf[TEMP_BUFFER_SIZE];
    #define EXPECTED_FIRMWARE_CHECKSUM_STRING "the firmware checksum"
    char * string_with_newline = EXPECTED_FIRMWARE_CHECKSUM_STRING "\n";
    char * string_without_newline = EXPECTED_FIRMWARE_CHECKSUM_STRING;
    char * firmware_checksum_returned;

    /* all tests ensure that the file opened was also closed */

    /* test that get_tlr_firmware_checksum returns the expected string, and that the returned string
     * has no newline as expected when line read has a newline. Test that the file is closed and
     * that the correct file pointer is passed to getline.
     */
    /* setup */
    file_ptr_passed_to_getline = NULL;
    file_pointer_passed_to_fclose = NULL;
    filename_requested_to_open = NULL;
    snprintf(buf, buf_size, "%s", string_with_newline);
    getline_return_value = strlen(buf);
    getline_lineptr_value = buf;
    firmware_checksum_returned = NULL;
    /* perform test */
    result = get_tlr_firmware_checksum(&firmware_checksum_returned);
    /* check results */
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_STRING_EQUAL(firmware_checksum_returned, EXPECTED_FIRMWARE_CHECKSUM_STRING);
    CU_ASSERT_PTR_EQUAL(file_ptr_passed_to_getline, fopen_return_value);
    CU_ASSERT_PTR_EQUAL(file_pointer_passed_to_fclose, fopen_return_value);
    /* cleanup */
    free((void *)filename_requested_to_open);
    free(firmware_checksum_returned);

    /* test that get_tlr_firmware_checksum returns the expected string, and that the returned string
     * has no newline as expected when line read has no newline. Test that the file is closed and
     * that the correct file pointer is passed to getline.
     */
    /* setup */
    file_ptr_passed_to_getline = NULL;
    file_pointer_passed_to_fclose = NULL;
    filename_requested_to_open = NULL;
    snprintf(buf, buf_size, "%s", string_without_newline);
    getline_return_value = strlen(buf);
    getline_lineptr_value = buf;
    firmware_checksum_returned = NULL;
    /* perform test */
    result = get_tlr_firmware_checksum(&firmware_checksum_returned);
    /* check results */
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_STRING_EQUAL(firmware_checksum_returned, EXPECTED_FIRMWARE_CHECKSUM_STRING);
    CU_ASSERT_PTR_EQUAL(file_ptr_passed_to_getline, fopen_return_value);
    CU_ASSERT_PTR_EQUAL(file_pointer_passed_to_fclose, fopen_return_value);
    /* cleanup */
    free((void *)filename_requested_to_open);
    free(firmware_checksum_returned);

    /* test bad getline return value
     */
    /* setup */
    file_ptr_passed_to_getline = NULL;
    file_pointer_passed_to_fclose = NULL;
    filename_requested_to_open = NULL;
    snprintf(buf, buf_size, "%s", string_with_newline);
    getline_return_value = 0;   /* indicates 0 bytes returned to caller */
    getline_lineptr_value = buf;
    firmware_checksum_returned = NULL;
    /* perform test */
    result = get_tlr_firmware_checksum(&firmware_checksum_returned);
    /* check results */
    CU_ASSERT_EQUAL(result, -1);
    CU_ASSERT_PTR_EQUAL(file_ptr_passed_to_getline, fopen_return_value);
    CU_ASSERT_PTR_EQUAL(file_pointer_passed_to_fclose, fopen_return_value);
    /* cleanup */
    free((void *)filename_requested_to_open);
    free(firmware_checksum_returned);
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
         || (CU_add_test(pSuite, "test_get_tlr_firmware_checksum", test_get_tlr_firmware_checksum) == NULL)
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



