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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdint.h>
#include <sys/types.h>
#include "libifuptime_defs.h"

char *get_proc_uptime_file_name(void);

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

// We create a /tmp/proc/uptime file with known values which is then
// fed to the resource monitor's main look and the output file is checked.
// Two sets of readings are required to calculate CPU usage.
char const * good_connected_time = "2089.41 9120.85\n";
char const * good_proc_uptime = "2289.41 9120.85\n";

char const * bad_connected_time = "\n";
char const * bad_proc_uptime = "";

/* functions to be tested */
read_time_result_t read_time_from_file(char const * const filename, uint64_t * time);

/****************  Stubs   ***************************/

/************* Test case functions ****************/

// We create a /tmp/proc/uptime file used for testing
// instead of using the real /proc/uptime
static int create_proc_uptime(char const *buf)
{
	FILE *fp;
	int ret=-1;
	size_t len=strlen(buf);
	char const * const proc_filename = SYSTEM_UPTIME_FILENAME;

	if (mkdir("/tmp/proc", S_IRWXU))
	{
		if (errno != EEXIST)
		{
			perror("Could not create /tmp/proc\r\n");
		}
	}

	if ((fp = fopen(proc_filename, "w+")) == NULL)
	{
		fprintf(stderr, "fopen '%s' failed\r\n", proc_filename);
		goto error;
	}

	if ((fwrite(buf, 1, len, fp) != len))
	{
		fprintf(stderr, "fwrite failed for %s\n", proc_filename);
		goto error;
	}

	fclose(fp);

	ret = 0;
	error:

	return ret;
}

/******* Test Cases ******/
void test_valid_proc_uptime()
{
	read_time_result_t result;
	uint64_t time;

	/* test that reading a valid /proc/uptime gives an OK result */
	/* setup */
	create_proc_uptime(good_connected_time);
	/* perform test */
	result = read_time_from_file(SYSTEM_UPTIME_FILENAME, &time);
	/* check result */
	CU_ASSERT_EQUAL(result, read_time_ok);

}

void test_invalid_proc_uptime()
{
	read_time_result_t result;
	uint64_t time;
	char buffer [L_tmpnam];

	/* test that reading from a file that doesn't exist gives a 'no file' response */
	/* setup */
	tmpnam(buffer);	/* find the name of a file that doesn't exist */
	/* perform test */
	result = read_time_from_file(buffer, &time);
	/* check result */
	CU_ASSERT_EQUAL(result, read_time_no_file);

}


/************* Test Runner Code goes here **************/

int main( void )
{
	int number_of_failures;
	CU_pSuite pSuite = NULL;

	/* initialize the CUnit test registry */
	if ( CUE_SUCCESS != CU_initialize_registry() )
	  return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite( "libifuptime_test_suite", init_suite, clean_suite );
	if (NULL == pSuite)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if ((CU_add_test(pSuite, "test_valid_proc_uptime", test_valid_proc_uptime) == NULL) ||
		(CU_add_test(pSuite, "test_invalid_proc_uptime", test_invalid_proc_uptime) == NULL))
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

	//	Clean up registry and return
	number_of_failures = CU_get_number_of_failures();
	CU_cleanup_registry();

	//	Ensure we return an error if any tests failed
	if (number_of_failures)
		return -1;

	return CU_get_error();
}



