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

#include "config_file.h"
#include "config_file_helper.h"
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <uci.h>

#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include <config_mgr.h>

/*data*/
static config_mgr_group_table_t table;
static config_mgr_group_t group;
static struct uci_context uci_context;

static struct uci_context * uci_alloc_return_value;
static struct uci_context * uci_context_passed_to_uci_set_confdir;
static struct uci_context * uci_context_passed_to_write_group_commands_to_file;
static FILE * fp_passed_to_write_group_commands_to_file;
static config_mgr_group_t * group_passed_to_write_group_commands_to_file;
static struct uci_context * uci_context_passed_to_uci_free_context;
static int write_group_commands_to_file_result;

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

static void test_config_file_write_commands(void)
{
	FILE file;
	int result;

	/* test normal operation*/
	/*setup*/
	table.count = 1;
	table.groups = &group;
	uci_context_passed_to_uci_set_confdir = NULL;
	uci_context_passed_to_write_group_commands_to_file = NULL;
	fp_passed_to_write_group_commands_to_file = NULL;
	group_passed_to_write_group_commands_to_file = NULL;
	uci_context_passed_to_uci_free_context = NULL;
	uci_alloc_return_value = &uci_context;
	write_group_commands_to_file_result = 0;
	/*perform test*/
	result = config_file_write_commands(&file);
	/*check result*/
	CU_ASSERT_EQUAL(result, 0);
	CU_ASSERT_PTR_EQUAL(uci_context_passed_to_uci_set_confdir, &uci_context);
	CU_ASSERT_PTR_EQUAL(uci_context_passed_to_write_group_commands_to_file, &uci_context);
	CU_ASSERT_PTR_EQUAL(fp_passed_to_write_group_commands_to_file, &file);
	CU_ASSERT_PTR_EQUAL(group_passed_to_write_group_commands_to_file, &group);
	CU_ASSERT_PTR_EQUAL(uci_context_passed_to_uci_free_context, &uci_context);	/* ensures the UCI context is freed*/

	/*test failed UCI alloc*/
	/*setup*/
	table.count = 1;
	table.groups = &group;
	uci_context_passed_to_uci_set_confdir = NULL;
	uci_context_passed_to_write_group_commands_to_file = NULL;
	fp_passed_to_write_group_commands_to_file = NULL;
	group_passed_to_write_group_commands_to_file = NULL;
	uci_context_passed_to_uci_free_context = NULL;
	uci_alloc_return_value = NULL;
	write_group_commands_to_file_result = 0;
	/*perform test*/
	result = config_file_write_commands(&file);
	/*check result*/
	CU_ASSERT_EQUAL(result, -1);

	/*test failed write_group_commands_to_file*/
	/*setup*/
	table.count = 1;
	table.groups = &group;
	uci_context_passed_to_uci_set_confdir = NULL;
	uci_context_passed_to_write_group_commands_to_file = NULL;
	fp_passed_to_write_group_commands_to_file = NULL;
	group_passed_to_write_group_commands_to_file = NULL;
	uci_context_passed_to_uci_free_context = NULL;
	uci_alloc_return_value = &uci_context;
	write_group_commands_to_file_result = -1;
	/*perform test*/
	result = config_file_write_commands(&file);
	/*check result*/
	CU_ASSERT_EQUAL(result, -1);
	CU_ASSERT_PTR_EQUAL(uci_context_passed_to_uci_free_context, &uci_context);	/* ensures the UCI context is freed*/
}

/************* Test Runner Code goes here **************/

static int add_tests(void)
{
	CU_pSuite pSuite = NULL;

	/* add a suite to the registry */
	pSuite = CU_add_suite( "config_file", init_suite, clean_suite );
	if ( NULL == pSuite )
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if ( 0
		 || CU_add_test(pSuite, "test_config_file_write_commands", test_config_file_write_commands)==NULL
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

	// Run all tests using the basic interface
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	printf("\n");
	CU_basic_show_failures(CU_get_failure_list());
	printf("\n\n");


	/* Clean up registry and return */
	number_of_failures = CU_get_number_of_failures();
	CU_cleanup_registry();

	// Ensure we return an error if any tests failed.
	if (number_of_failures)
		return -1;

	return CU_get_error();
}

/* stubs */

void const * tlr_get_table(config_mgr_table_type_t const type)
{
	return &table;
}

struct uci_context * uci_alloc_context(void)
{
	return uci_alloc_return_value;
}

int uci_set_confdir(struct uci_context * ctx, const char * dir)
{
	uci_context_passed_to_uci_set_confdir = ctx;
	return 0;
}

int write_group_commands_to_file(struct uci_context * const uci_context, FILE * fp, config_mgr_group_t const * const group)
{
	uci_context_passed_to_write_group_commands_to_file = uci_context;
	fp_passed_to_write_group_commands_to_file = fp;
	group_passed_to_write_group_commands_to_file = (config_mgr_group_t * const)group;
	return write_group_commands_to_file_result;
}

void uci_free_context(struct uci_context * ctx)
{
	uci_context_passed_to_uci_free_context = ctx;
}

