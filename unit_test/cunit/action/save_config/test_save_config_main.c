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

#include "save_config_work.h"
#include "save_config_process.h"
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
#include <string.h>

#define NB_ELEMENTS(array)  (sizeof((array))/sizeof((array[0])))
#define READ_BUFFER_SIZE	100		/*ensure is large enough to contain all expected results*/
#define USER_SIZE			100

/*data*/
static int config_save_process_result;
static char actual_user_passed[USER_SIZE];

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
static void test_save_config_main(void)
{
	bool result;
	char expected_user[USER_SIZE];
	char * good_user_argv[] = {"", "-u", "test_user", NULL};
	int const good_user_argc = 3;
	char * missing_user_argv[] = {"programname", "-i", "1", NULL};
	int const missing_user_argc = 3;
#if defined(WHEN_GETOPT_FIXED)	/*results in crash in getopt*/
	char * too_few_argv[] = {"programname", NULL};
	int const too_few_argc = 1;
#endif

	/*test normal operation*/
	/*setup*/
	actual_user_passed[0] = '\0';
	config_save_process_result = 0;
	strncpy(expected_user, "test", sizeof expected_user);
	expected_user[sizeof(expected_user) - 1] = '\0';
	/*perform test*/
	result = save_config_main(good_user_argc, good_user_argv);
	/*check result*/
	CU_ASSERT_EQUAL(result, true);
	CU_ASSERT_STRING_EQUAL(actual_user_passed, "test_user");

	/*test missing user arg*/
	/*setup*/
	actual_user_passed[0] = '\0';
	config_save_process_result = 0;
	strncpy(expected_user, "test", sizeof expected_user);
	expected_user[sizeof(expected_user) - 1] = '\0';
	/*perform test*/
	result = save_config_main(missing_user_argc, missing_user_argv);
	/*check result*/
	CU_ASSERT_EQUAL(result, false);

#if defined(WHEN_GETOPT_FIXED)	/*results in crash in getopt*/
	/*test too few args*/
	/*setup*/
	actual_user_passed[0] = '\0';
	config_save_process_result = 0;
	strncpy(expected_user, "test", sizeof expected_user);
	expected_user[sizeof(expected_user) - 1] = '\0';
	/*perform test*/
	result = save_config_main(too_few_argc, too_few_argv);
	/*check result*/
	CU_ASSERT_EQUAL(result, false);
#endif

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
		 || CU_add_test(pSuite, "test_save_config_main", test_save_config_main)==NULL
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
	if (add_tests())
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

int config_save_process(char const * const user)
{
	/*save the user string somewhere so that we can compare with the expected value*/
	strncpy(actual_user_passed, user, sizeof actual_user_passed);
	actual_user_passed[sizeof actual_user_passed - 1] = '\0';
	return config_save_process_result;
}
