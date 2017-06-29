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

#include <interfaces.h>
#include <config_def.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#define NB_ELEMENTS(array)  (sizeof((array))/sizeof((array[0])))

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
static void test_get_interface_name_from_type(void)
{
    char const * interface_name;
    interfaces_t interface_type;

    for (interface_type = first_valid_interface; interface_type <= last_valid_interface; interface_type++)
    {
        /* setup */
        /* perform test */
        interface_name = interface_get_name_from_type(interface_type);
        /* check result */
        CU_ASSERT_PTR_NOT_NULL(interface_name);
    }
    /* test invalid interface type */
    /* setup */
    interface_type = invalid_interface;
    /* perform test */
    interface_name = interface_get_name_from_type(interface_type);
    /* check result */
    CU_ASSERT_PTR_NULL(interface_name);
}

static void test_get_interface_type_from_name(void)
{
    struct test_cases_st
    {
        char * interface_name;
        interfaces_t expected_interface;
    } test_cases[] =
    {
        {
            .interface_name = "eth",
            .expected_interface = eth_interface
        },
        {
            .interface_name = "dsl",
            .expected_interface = dsl_interface
        },
        {
            .interface_name = "cellular",
            .expected_interface = cellular_interface
        },
        {
            .interface_name = "lan",
            .expected_interface = bridge_interface
        },
        {
            .interface_name = "invalid_interface_name",
            .expected_interface = invalid_interface
        }
    };
    int index;
    interfaces_t interface_type;

    for (index = 0; index < NB_ELEMENTS(test_cases); index++)
    {
        /* setup */
        /* perform test */
        interface_type = interface_get_type_from_name(test_cases[index].interface_name);
        /* check result */
        CU_ASSERT_EQUAL(interface_type, test_cases[index].expected_interface);
    }


}

static void test_get_interface_type_from_config_group(void)
{
    struct test_cases_st
    {
        config_group_t config_group;
        interfaces_t expected_interface;
    } test_cases[] =
    {
        {
            .config_group = config_eth,
            .expected_interface = eth_interface
        },
        {
            .config_group = config_dsl,
            .expected_interface = dsl_interface
        },
        {
            .config_group = config_cellular,
            .expected_interface = cellular_interface
        },
        {
            .config_group = config_lan,
            .expected_interface = bridge_interface
        },
        {
            .config_group = config_serial,
            .expected_interface = invalid_interface
        }
    };
    int index;
    interfaces_t interface_type;

    for (index = 0; index < NB_ELEMENTS(test_cases); index++)
    {
        /* setup */
        /* perform test */
        interface_type = interface_get_type_from_config_group(test_cases[index].config_group);
        /* check result */
        CU_ASSERT_EQUAL(interface_type, test_cases[index].expected_interface);
    }


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
    pSuite = CU_add_suite("libmessage_interfaces_test_suite", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ( 0
         || (CU_add_test(pSuite, "test_get_interface_name_from_type", test_get_interface_name_from_type) == NULL)
         || (CU_add_test(pSuite, "test_get_interface_type_from_name", test_get_interface_type_from_name) == NULL)
         || (CU_add_test(pSuite, "test_get_interface_type_from_config_group", test_get_interface_type_from_config_group) == NULL)
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



