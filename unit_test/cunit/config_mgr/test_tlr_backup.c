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

#include "tlr_backup.h"
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
#include <config_status.h>

#define tlr_debug(x...) do { } while (0)
/*data*/
static config_mgr_element_t config_mgr_set_group_system_elements[] = {
 {
   "prompt",
   "CLI prompt",
   NULL,
   "digi.router>",
   config_mgr_access_readwrite, 
   1,
   NULL,
   NULL,
   1,
 },
 {
   "prompt2",
   "CLI prompt2",
   NULL,
   "digi.router2>",
   config_mgr_access_readwrite, 
   2,
   NULL,
   NULL,
   2,
 }
};

static config_mgr_group_t group[] = {
 {
   "system",
   "system",
   "System configuration",
   config_system,
   1,
   true,
   true,
   config_mgr_access_readwrite,
   {
   config_mgr_set_group_system_elements,
   config_mgr_asizeof(config_mgr_set_group_system_elements)
   }
 }
};
static bool test_null_group_prt;
static bool test_set_element_fail;
static bool test_tlr_get_last_config_fail;
static bool test_tlr_set_last_config_fail;
static bool test_tlr_get_element_value_fail;

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

static void test_config_restore_default(void)
{
    int result;
    size_t group_id;
    size_t instance;

    /* test group_prt null path operation*/
    /*setup*/
    test_null_group_prt = true;
    test_set_element_fail = false;
    group_id = 1;
    instance = 1;
    /*perform test*/
    result = config_restore_default(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, -1);

    /* test set_element fail path operation, count is 0, 1, 2*/
    /*setup*/
    test_null_group_prt = false;
    test_set_element_fail = true;
    group_id = 1;
    instance = 1;
    group[0].elements.count = 0;
    /*perform test*/
    result = config_restore_default(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, 0);

    test_null_group_prt = false;
    test_set_element_fail = true;
    group_id = 1;
    instance = 1;
    group[0].elements.count = 1;
    /*perform test*/
    result = config_restore_default(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, -1);

    /*setup*/
    test_null_group_prt = false;
    test_set_element_fail = true;
    group_id = 1;
    instance = 1;
    group[0].elements.count = 2;
    /*perform test*/
    result = config_restore_default(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, -1);

     /* test set_element good path operation, count is 1, 2*/
    /*setup*/
    test_null_group_prt = false;
    test_set_element_fail = false;
    group_id = 1;
    instance = 1;
    group[0].elements.count = 1;
    /*perform test*/
    result = config_restore_default(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, 0);

    /*setup*/
    test_null_group_prt = false;
    test_set_element_fail = false;
    group_id = 1;
    instance = 1;
    group[0].elements.count = 2;
    /*perform test*/
    result = config_restore_default(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, 0);
}

static void test_config_restore_backup(void)
{
    int result;
    size_t group_id;
    size_t instance;

    /* test group_prt null path operation*/
    /*setup*/
    test_null_group_prt = true;
    test_set_element_fail = false;
    group_id = 1;
    instance = 1;
    /*perform test*/
    result = config_restore_backup(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, -1);

    /* test group_prt good path but tlr_get_last_config fails operation*/
    /*setup*/
    test_null_group_prt = true;
    test_set_element_fail = false;
    test_tlr_get_last_config_fail = true;
    group_id = 1;
    instance = 1;
    /*perform test*/
    result = config_restore_backup(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, -1);

    /* test set_element fail path operation, count is 0, 1, 2*/
    /*setup*/
    test_null_group_prt = false;
    test_set_element_fail = true;
    test_tlr_get_last_config_fail = false;
    group_id = 1;
    instance = 1;
    group[0].elements.count = 0;
    /*perform test*/
    result = config_restore_backup(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, 0);

    test_null_group_prt = false;
    test_set_element_fail = true;
    test_tlr_get_last_config_fail = false;
    group_id = 1;
    instance = 1;
    group[0].elements.count = 1;
    /*perform test*/
    result = config_restore_backup(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, -1);

    /*setup*/
    test_null_group_prt = false;
    test_set_element_fail = true;
    group_id = 1;
    instance = 1;
    group[0].elements.count = 2;
    /*perform test*/
    result = config_restore_backup(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, -1);

     /* test set_element good path operation, count is 1, 2*/
    /*setup*/
    test_null_group_prt = false;
    test_set_element_fail = false;
    test_tlr_get_last_config_fail = false;
    group_id = 1;
    instance = 1;
    group[0].elements.count = 1;
    /*perform test*/
    result = config_restore_backup(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, 0);

    /*setup*/
    test_null_group_prt = false;
    test_set_element_fail = false;
    test_tlr_get_last_config_fail = false;
    group_id = 1;
    instance = 1;
    group[0].elements.count = 2;
    /*perform test*/
    result = config_restore_backup(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, 0);
}

static void test_config_backup_current(void)
{
    int result;
    size_t group_id;
    size_t instance;

    /* test group_prt null path operation*/
    /*setup*/
    test_null_group_prt = true;
    group_id = 1;
    instance = 1;
    /*perform test*/
    result = config_backup_current(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, -1);

    /* test group_prt good path but tlr_get_element_value fails operation, count is 0, 1, 2*/
    /*setup*/
    test_null_group_prt = false;
    test_tlr_get_element_value_fail = true;
    test_tlr_set_last_config_fail = false;
    group_id = 1;
    instance = 1;
    group[0].elements.count = 0;
    /*perform test*/
    result = config_backup_current(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, 0);

    /*setup*/
    test_null_group_prt = false;
    test_tlr_get_element_value_fail = true;
    test_tlr_set_last_config_fail = false;
    group_id = 1;
    instance = 1;
    group[0].elements.count = 1;
    /*perform test*/
    result = config_backup_current(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, -1);

    /*setup*/
    test_null_group_prt = false;
    test_tlr_get_element_value_fail = true;
    test_tlr_set_last_config_fail = false;
    group_id = 1;
    instance = 1;
    group[0].elements.count = 2;
    /*perform test*/
    result = config_backup_current(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, -1);

    /* test get_elment_value good path operation, count is 1, 2*/
    /*setup*/
    test_null_group_prt = false;
    test_tlr_get_element_value_fail = false;
    test_tlr_set_last_config_fail = true;
    group_id = 1;
    instance = 1;
    group[0].elements.count = 1;
    /*perform test*/
    result = config_backup_current(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, -1);

    /*setup*/
    test_null_group_prt = false;
    test_tlr_get_element_value_fail = false;
    test_tlr_set_last_config_fail = true;
    group_id = 1;
    instance = 1;
    group[0].elements.count = 2;
    /*perform test*/
    result = config_backup_current(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, -1);

    /* test get_elment_value good path operation, count is 1, 2*/
    /*setup*/
    test_null_group_prt = false;
    test_tlr_get_element_value_fail = false;
    test_tlr_set_last_config_fail = false;
    group_id = 1;
    instance = 1;
    group[0].elements.count = 1;
    /*perform test*/
    result = config_backup_current(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, 0);

    /*setup*/
    test_null_group_prt = false;
    test_tlr_get_element_value_fail = false;
    test_tlr_set_last_config_fail = false;
    group_id = 1;
    instance = 1;
    group[0].elements.count = 2;
    /*perform test*/
    result = config_backup_current(group_id, instance);
    /*check result*/
    CU_ASSERT_EQUAL(result, 0);
}

/************* Test Runner Code goes here **************/

static int add_tests(void)
{
	CU_pSuite pSuite = NULL;

	/* add a suite to the registry */
	pSuite = CU_add_suite( "backup_restore_config", init_suite, clean_suite );
	if ( NULL == pSuite )
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if ( 0
		 || CU_add_test(pSuite, "test_config_restore_default", test_config_restore_default)==NULL
		 || CU_add_test(pSuite, "test_config_restore_backup", test_config_restore_backup)==NULL
		 || CU_add_test(pSuite, "test_config_backup_current", test_config_backup_current)==NULL
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
config_mgr_group_t const * tlr_get_group_ptr(size_t const group_id, size_t const instance, config_mgr_group_type_t const table_index)
{
    if (test_null_group_prt)
        return NULL;
    else
        return &group[0];
}

config_status_t tlr_set_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      char const * const value,
                                      config_mgr_access_level_t const access)
{
    if (test_set_element_fail)
        return config_status_success+1;  // config_status_success is 0, all other errors are >=1
    else
        return config_status_success;
}

size_t tlr_get_config_value_max_length(config_mgr_table_type_t const type, size_t const config_id, size_t const element_id)
{
    return 1;
}

config_status_t tlr_get_last_config(size_t const group_id, size_t const instance, void * const ptr, size_t const size)
{
    if (test_tlr_get_last_config_fail)
        return config_status_success+1;  // config_status_success is 0, all other errors are >=1
    else
        return config_status_success;
}

config_status_t tlr_get_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      void * const value,
                                      size_t const bytes,
                                      config_mgr_element_type_t const type,
                                      config_mgr_access_level_t const access)
{
    if (test_tlr_get_element_value_fail)
        return config_status_success+1;  // config_status_success is 0, all other errors are >=1
    else
        return config_status_success;
}

config_status_t tlr_set_last_config(size_t const group_id, size_t const instance, void const * const ptr, size_t const size)
{
    if (test_tlr_set_last_config_fail)
        return config_status_success+1;  // config_status_success is 0, all other errors are >=1
    else
        return config_status_success;
}
