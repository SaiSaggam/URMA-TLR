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

#define	USE_BASIC_STYLE		1
#define	USE_AUTOMATED_STYLE	0
#define	USE_CONSOLE_STYLE	0

#define TEMP_FILE_NAME "test.log"

#if USE_BASIC_STYLE
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#elif USE_AUTOMATED_STYLE
#include "CUnit/Automated.h"
#else
#include "CUnit/Console.h"
#endif

/********************
	Header Files
********************/
#include "show_failover_work.h"
#include "tlr_common.h"
#include "config_mgr.h"
#include "config_status.h"
#include "show_parse_helper.h"
#include "interfaces.h"

#include <stdlib.h>
#include <dirent.h>

/********************
	Stub Functions
********************/
static int sock_num = -1;
static int max_instance = -1;
extern bool update_failover_status(failover_status_t * pstatus, int instance);
static failover_status_t failover_status;
#define GOOD_SHOW_FAILOVER_FILE 0
#define BAD_SHOW_FAILOVER_FILE 1
#define NON_SHOW_FAILOVER_FILE 2
#define good_show_filename "good_show_failover.txt"
#define bad_show_filename "bad_show_failover.txt"
#define non_show_filename "non_show_failover.txt"
static int which_failover_file;
static char const * filename_requested_to_open;
FILE *fdopen(int fd, const char *mode)
{
    switch (which_failover_file)
    {
        case GOOD_SHOW_FAILOVER_FILE:
            filename_requested_to_open = good_show_filename;
            break;
            
        case BAD_SHOW_FAILOVER_FILE:
            filename_requested_to_open = bad_show_filename;
            break;
         
        case NON_SHOW_FAILOVER_FILE:
            filename_requested_to_open = non_show_filename;
            break;      
    }
    //printf("mock fdopen()\n");
    return fopen(filename_requested_to_open, mode);
}

/** more Stubs    */
int show_failover_json(failover_status_t  const * const pstatus, int const instance)
{
    return 0;
}
size_t tlr_get_max_instance(size_t const group_id, config_mgr_table_type_t const type)
{
    return max_instance;
}
int connect_to_plain_socket(char const * const socket_name, bool const use_abstract_namespace)
{
    return sock_num;
}

/********************
	Test Suite Setup
********************/

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/********************
	Test Case Functions
********************/
static void test_update_failover_status()
{
    bool status;

    {
        /* test can't open socket */
        int instance = 1;
        failover_status_t * pstatus = &failover_status;
        sock_num = -1;
        which_failover_file = GOOD_SHOW_FAILOVER_FILE;
        status = update_failover_status(pstatus, instance);
        CU_ASSERT_EQUAL(status, false);
    }

    {
        /* test can't open a file */
        int instance = 1;
        failover_status_t * pstatus = &failover_status;
        sock_num = -1;
        which_failover_file = NON_SHOW_FAILOVER_FILE;
        status = update_failover_status(pstatus, instance);
        CU_ASSERT_EQUAL(status, false);
    }

    {
        /* test good a file and correct instance, using passive*/
        int instance = 1;
        failover_status_t * pstatus = &failover_status;
        memset(pstatus, 0, sizeof(*pstatus));
        sock_num = 8;
        which_failover_file = GOOD_SHOW_FAILOVER_FILE;
        status = update_failover_status(pstatus, instance);
        CU_ASSERT_EQUAL(status, true);
        CU_ASSERT_STRING_EQUAL(pstatus->primary_if, "eth4")
        CU_ASSERT_STRING_EQUAL(pstatus->primary_if_status, "Up")
        CU_ASSERT_STRING_EQUAL(pstatus->secondary_if, "cellular1")
        CU_ASSERT_STRING_EQUAL(pstatus->secondary_if_status, "Up")
        CU_ASSERT_STRING_EQUAL(pstatus->using_if, "eth4")
        CU_ASSERT_STRING_EQUAL(pstatus->detection_method, "passive")
        CU_ASSERT_EQUAL(pstatus->using_ping, false);
    }

    {
        /* test good a file and correct instance, using probe*/
        int instance = 3;
        failover_status_t * pstatus = &failover_status;
        memset(pstatus, 0, sizeof(*pstatus));
        sock_num = 8;
        which_failover_file = GOOD_SHOW_FAILOVER_FILE;
        status = update_failover_status(pstatus, instance);
        CU_ASSERT_EQUAL(status, true);
        CU_ASSERT_EQUAL(pstatus->using_ping, true);
        CU_ASSERT_EQUAL(pstatus->show_failing_over, true);
        CU_ASSERT_EQUAL(pstatus->show_switch_back, true);
        CU_ASSERT_STRING_EQUAL(pstatus->primary_if, "eth1")
        CU_ASSERT_STRING_EQUAL(pstatus->primary_if_status, "Up")
        CU_ASSERT_STRING_EQUAL(pstatus->secondary_if, "eth4")
        CU_ASSERT_STRING_EQUAL(pstatus->secondary_if_status, "Up")
        CU_ASSERT_STRING_EQUAL(pstatus->using_if, "eth4")
        CU_ASSERT_STRING_EQUAL(pstatus->detection_method, "probe")
    }

    {
        /* test good a file and correct instance, using probe*/
        int instance = 5;
        failover_status_t * pstatus = &failover_status;
        memset(pstatus, 0, sizeof(*pstatus));
        sock_num = 8;
        which_failover_file = GOOD_SHOW_FAILOVER_FILE;
        status = update_failover_status(pstatus, instance);
        CU_ASSERT_EQUAL(status, true);
        CU_ASSERT_EQUAL(pstatus->using_ping, true);
        CU_ASSERT_EQUAL(pstatus->show_failing_over, true);
        CU_ASSERT_EQUAL(pstatus->show_switch_back, false);
    }

    {
        /* test good a file and instance does not match */
        int instance = 7;
        failover_status_t * pstatus = &failover_status;
        sock_num = 99;
        which_failover_file = GOOD_SHOW_FAILOVER_FILE;
        status = update_failover_status(pstatus, instance);
        CU_ASSERT_EQUAL(status, false);
    }

    {
        /* test good a file and correct instance */
        int instance = 9;
        failover_status_t * pstatus = &failover_status;
        sock_num = 99;
        which_failover_file = GOOD_SHOW_FAILOVER_FILE;
        status = update_failover_status(pstatus, instance);
        CU_ASSERT_EQUAL(status, true);
        CU_ASSERT_EQUAL(pstatus->using_ping, true);
        CU_ASSERT_EQUAL(pstatus->show_failing_over, true);
        CU_ASSERT_EQUAL(pstatus->show_switch_back, true);
    }

    {
        /* test a bad file and correct instance */
        int instance = 9;
        failover_status_t * pstatus = &failover_status;
        sock_num = 99;
        which_failover_file = BAD_SHOW_FAILOVER_FILE;
        status = update_failover_status(pstatus, instance);
        CU_ASSERT_EQUAL(status, false);
    }
}

static void test_show_failover_main()
{
    int status;
    which_failover_file = GOOD_SHOW_FAILOVER_FILE;

    {
        char * argv[] = {"show_failover"};
        max_instance = 0;
        status = show_failover_main(ARRAY_SIZE(argv), argv);
        CU_ASSERT_EQUAL(status, EXIT_FAILURE);
        CU_ASSERT_EQUAL(max_instance, 0);
    }

    {
        char * argv[] = {"show_failover"};
        max_instance = -1;
        status = show_failover_main(ARRAY_SIZE(argv), argv);
        CU_ASSERT_EQUAL(status, EXIT_FAILURE);
        CU_ASSERT_EQUAL(max_instance, -1);
    }

    {
        char * argv[] = {"show_failover", "-i", "20"};
        
        max_instance = 20;
        status = show_failover_main(ARRAY_SIZE(argv), argv);

        CU_ASSERT_EQUAL(status, EXIT_SUCCESS);
        CU_ASSERT_EQUAL(max_instance, 20);
    }

    {
        char * argv[] = {"show_failover", "-i", "20"};
        
        max_instance = 10;
        status = show_failover_main(ARRAY_SIZE(argv), argv);

        CU_ASSERT_EQUAL(status, EXIT_FAILURE);
        CU_ASSERT_EQUAL(max_instance, 10);
    }

    {
        char * argv[] = {"show_failover", "-i", "8", "-f", "ascii"};

        max_instance = 10;
        status = show_failover_main(ARRAY_SIZE(argv), argv);
        CU_ASSERT_EQUAL(status, EXIT_SUCCESS);
        CU_ASSERT_EQUAL(max_instance, 10);
    }

    {
        char * argv[] = { "show_failover", "-i", "9", "-f", "json" };

        status = show_failover_main(ARRAY_SIZE(argv), argv);
        CU_ASSERT_EQUAL(status, EXIT_SUCCESS);
    }
}

/********************
	Test Runner
********************/

int main() {
	unsigned int number_of_failures;

	//	Initialize the CUnit test registry
	if ( CUE_SUCCESS != CU_initialize_registry() )
		return CU_get_error();

	//	Add a suite to the registry
	CU_pSuite pSuite = CU_add_suite( "show_failover_test_suite", init_suite, clean_suite );
	if ( pSuite == NULL ) {
		CU_cleanup_registry();
		return CU_get_error();
	}


	// Add the tests to the suite
	if ( 0 ||
        (CU_add_test(pSuite, "test_show_failover_main", test_show_failover_main)==NULL) ||
        (CU_add_test(pSuite, "test_update_failover_status", test_update_failover_status)==NULL)
		)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	#if USE_BASIC_STYLE
		//	Run all tests using the basic interface
		CU_basic_set_mode(CU_BRM_SILENT);
		CU_basic_run_tests();
		CU_basic_show_failures(CU_get_failure_list());
	#elif USE_AUTOMATED_STYLE
		//	Run all tests using the automated interface
		CU_automated_run_tests();
		CU_list_tests_to_file();
	#else
		//	Run all tests using the console interface
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

