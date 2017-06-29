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

#include "save_config_process.h"
#include "config_file.h"
#include "config_mgr.h"

#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>


#if USE_BASIC_STYLE
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#elif USE_AUTOMATED_STYLE
#include "CUnit/Automated.h"
#else
#include "CUnit/Console.h"
#endif

#include <config_def.h>

#define NB_ELEMENTS(array)  (sizeof((array))/sizeof((array[0])))

/*data*/
static FILE * cunit_fopen_return_value;
static int write_commands_to_file_result;
static FILE * write_timestamp_to_file_fp;
static FILE * write_commands_to_file_fp;
static FILE * cunit_fclose_fp;
static char const * const expected_filename = "./tmpconfigfile";
static char actual_filename[PATH_MAX];
static bool popen_fail=false;

/* Test Suite setup and cleanup functions: */
static int init_suite(void)
{
	return 0;
}

static int clean_suite(void)
{
	return 0;
}

/************* Test case functions ****************/

static void test_config_save_process(void)
{
	int result;
	char const * const test_user = "test_user";
	#define MAGIC_FILE_POINTER_VALUE (FILE *)0x35649700

	/*test normal operation*/
	/*setup*/
	cunit_fopen_return_value = MAGIC_FILE_POINTER_VALUE;
	write_commands_to_file_result = 0;
	write_timestamp_to_file_fp = NULL;
	write_commands_to_file_fp = NULL;
	cunit_fclose_fp = NULL;
	/*perform test*/
	result = config_save_process(test_user);
	/*check result*/
	CU_ASSERT_EQUAL(result, 0);
	CU_ASSERT_PTR_EQUAL(write_timestamp_to_file_fp, MAGIC_FILE_POINTER_VALUE);
	CU_ASSERT_PTR_EQUAL(write_commands_to_file_fp, MAGIC_FILE_POINTER_VALUE);
	CU_ASSERT_PTR_EQUAL(cunit_fclose_fp, MAGIC_FILE_POINTER_VALUE);
	CU_ASSERT_STRING_EQUAL(actual_filename, expected_filename);
	/*test failed fopen*/
	/*setup*/
	cunit_fopen_return_value = NULL;
	write_commands_to_file_result = 0;
	/*perform test*/
	result = config_save_process(test_user);
	/*check result*/
	CU_ASSERT_EQUAL(result, -1);
	CU_ASSERT_STRING_EQUAL(actual_filename, expected_filename);

	/*test failed write_commands_to_file*/
	/*setup*/
	cunit_fopen_return_value = MAGIC_FILE_POINTER_VALUE;
	write_commands_to_file_result = -1;
	write_timestamp_to_file_fp = NULL;
	write_commands_to_file_fp = NULL;
	cunit_fclose_fp = NULL;
	/*perform test*/
	result = config_save_process(test_user);
	/*check result*/
	CU_ASSERT_EQUAL(result, -1);
	CU_ASSERT_STRING_EQUAL(actual_filename, expected_filename);
	CU_ASSERT_PTR_EQUAL(cunit_fclose_fp, MAGIC_FILE_POINTER_VALUE);
}

static void test_config_file_write_firewall(void)
{
    int expected_value;
    int actual_value;
    FILE *fp=NULL;

    expected_value = -1;
    popen_fail = true;
    actual_value = config_file_write_firewall(fp);
    CU_ASSERT_EQUAL(actual_value, expected_value);
    popen_fail = false;
}

/************* Test Runner Code goes here **************/

static int add_tests(void)
{
	CU_pSuite pSuite = NULL;

	/* add a suite to the registry */
	pSuite = CU_add_suite( "save_config_test_suite", init_suite, clean_suite );
	if ( NULL == pSuite )
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if ( 0
		 || CU_add_test(pSuite, "test_config_save_process", test_config_save_process)==NULL
		 || CU_add_test(pSuite, "test_config_file_write_firewall", test_config_file_write_firewall)==NULL
	)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	return 0;
}

int main(void)
{
	unsigned int number_of_failures;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add the tests to the suite */
	if (0
		|| add_tests()
	   )
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

#if USE_BASIC_STYLE
	// Run all tests using the basic interface
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	printf("\n");
	CU_basic_show_failures(CU_get_failure_list());
	printf("\n\n");

#elif USE_AUTOMATED_STYLE
	// Run all tests using the automated interface
	CU_automated_run_tests();
	CU_list_tests_to_file();

	// Run all tests using the console interface
	CU_console_run_tests();
#endif

	/* Clean up registry and return */
	number_of_failures = CU_get_number_of_failures();
	CU_cleanup_registry();

	// Ensure we return an error if any tests failed.
	if (number_of_failures)
		return -1;

	return CU_get_error();
}

/* stubs */

void tlr_get_config_file(char * cfg_file, int maxlen)
{
	strncpy(cfg_file, expected_filename, maxlen);
	cfg_file[maxlen - 1] = '\0';
}

config_mgr_element_enum_limit_t const * tlr_get_element_enum_limit(config_mgr_table_type_t const type, size_t const group_id, size_t const element_id)
{
    return NULL;
}

void config_file_write_timestamp(FILE * const fp, char const * const user)
{
	write_timestamp_to_file_fp = fp;
}

int config_file_write_commands(FILE * fp)
{
	write_commands_to_file_fp = fp;
	return write_commands_to_file_result;
}

FILE * cunit_fopen(const char * __restrict __filename, const char * __restrict __modes)
{
	strncpy(actual_filename, __filename, sizeof actual_filename);
	actual_filename[sizeof actual_filename - 1] = '\0';
	return cunit_fopen_return_value;
}

int cunit_fclose(FILE * __stream)
{
	cunit_fclose_fp = __stream;
	(void)__stream;
	return 0;
}
ssize_t getline(char **a, size_t *b, FILE *c)
{
    return -1;
}

void rewind(FILE *stream)
{
    (void)stream;
    return;
}

FILE *popen(const char *command, const char *type)
{
    static FILE fd;
    return (popen_fail) ? NULL : &fd;
}

int pclose(FILE *fp)
{
    return 0;
}

int fprintf(FILE *stream, const char *format, ...)
{
    return 1;
}
