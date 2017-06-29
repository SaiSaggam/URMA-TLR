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

#include "mac_address.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdarg.h>

static char * asprintf_buffer_ptr;
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
int asprintf(char ** ptr, const char * fmt, ...)
{
    va_list va_args;
    size_t const buf_size = 100;

    *ptr = malloc(buf_size);
    va_start(va_args, fmt);
    vsnprintf(*ptr, buf_size, fmt, va_args);
    va_end(va_args);

    return 1;   /* just some positive value */
}

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
    *lineptr = getline_lineptr_value;

    return getline_return_value;
}

#define xstr(s) tostr(s)
#define tostr(s) #s

/************* Test case functions ****************/

/******* Test Cases ******/
static void test_get_mac_address(void)
{
    #define TEMP_BUFFER_SIZE 100
    char * result;
    size_t instance_to_get;
    size_t const buf_size = TEMP_BUFFER_SIZE;
    char buf[TEMP_BUFFER_SIZE];
    #define EXPECTED_MAC_STRING "00:01:02:03:04:05"
    char * string_with_no_newline = EXPECTED_MAC_STRING;
    char * string_with_newline = EXPECTED_MAC_STRING"\n";
    char const interface_name_to_use[] = "eth";
    #define INSTANCE_1  1
    #define INSTANCE_2  2

    /* test that get mac for eth 1 requests from the correct /sys file, and that the returned
     * string has no newline as expected when line read has no newline.
     * Test that the file is closed and that the correct file pointer is passed to getline.
     */
    /* setup */
    instance_to_get = INSTANCE_1;
    file_ptr_passed_to_getline = NULL;
    file_pointer_passed_to_fclose = NULL;
    asprintf_buffer_ptr = malloc(buf_size);
    filename_requested_to_open = NULL;
    snprintf(buf, buf_size, "%s", string_with_no_newline);
    getline_return_value = strlen(buf);
    getline_lineptr_value = buf;
    /* perform test */
    result = get_mac_address(interface_name_to_use, instance_to_get);

    /* check results */
    CU_ASSERT_STRING_EQUAL(filename_requested_to_open, "/sys/class/net/eth" xstr(INSTANCE_1) "/address");
    CU_ASSERT_STRING_EQUAL(result, EXPECTED_MAC_STRING);
    CU_ASSERT_PTR_EQUAL(file_ptr_passed_to_getline, fopen_return_value);
    CU_ASSERT_PTR_EQUAL(file_pointer_passed_to_fclose, fopen_return_value);
    /* cleanup */
    free(asprintf_buffer_ptr);
    free((void *)filename_requested_to_open);

    /* test that get mac for different instance requests from the correct /sys file, and that the
     * returned string has no newline as expected when line read has newline.
     */
    /* setup */
    instance_to_get = INSTANCE_2;
    asprintf_buffer_ptr = malloc(buf_size);
    filename_requested_to_open = NULL;
    snprintf(buf, buf_size, "%s", string_with_newline);
    getline_return_value = strlen(buf);
    getline_lineptr_value = buf;
    /* perform test */
    result = get_mac_address(interface_name_to_use, instance_to_get);

    /* check results */
    CU_ASSERT_STRING_EQUAL(filename_requested_to_open, "/sys/class/net/eth" xstr(INSTANCE_2) "/address");
    CU_ASSERT_STRING_EQUAL(result, EXPECTED_MAC_STRING);
    /* cleanup */
    free(asprintf_buffer_ptr);
    free((void *)filename_requested_to_open);

    /* test bad getline return value
     */
    /* setup */
    instance_to_get = INSTANCE_2;
    asprintf_buffer_ptr = malloc(buf_size);
    snprintf(buf, buf_size, "%s", string_with_newline);
    getline_return_value = 0;
    getline_lineptr_value = NULL;
    /* perform test */
    result = get_mac_address(interface_name_to_use, instance_to_get);

    /* check results */
    CU_ASSERT_PTR_EQUAL(result, NULL);
    /* cleanup */
    free(asprintf_buffer_ptr);
    free((void *)filename_requested_to_open);

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
         || (CU_add_test(pSuite, "test_get_mac_address", test_get_mac_address) == NULL)
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



