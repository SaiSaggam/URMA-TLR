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

#include "odm_specific.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdarg.h>

static char const * platform_name_to_return;
static char const * rom_id_to_return;
static int get_odm_ROMID_result;
static int get_odm_ModelName_result;

static int init_suite(void)
{
    return 0;
}

static int clean_suite(void)
{
    return 0;
}

/* stubs */
int get_odm_ModelName(char *name, int len)
{
    int result;
    if (platform_name_to_return != NULL)
        result = snprintf(name, len, "%s", platform_name_to_return);
    else
        result = get_odm_ModelName_result;
    return result;

}

int get_odm_ROMID(char * romid, int len)
{
    snprintf(romid, len, "%s", rom_id_to_return);
    return get_odm_ROMID_result;
}

/************* Test case functions ****************/

/******* Test Cases ******/
static void test_get_tlr_platform_name(void)
{
    char const * expected_platform_name = "expected platform name";
    char result;
    char const * model_name;

    int const successful_get_odm_ModelName = 0;
    int const unsuccessful_get_odm_ModelName = -1;

    platform_name_to_return = expected_platform_name;
    get_odm_ModelName_result = successful_get_odm_ModelName;
    /* perform test */
    result = get_tlr_platform_name(&model_name);

    /* check result */
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_STRING_EQUAL(model_name, expected_platform_name);

    platform_name_to_return = NULL;
    get_odm_ModelName_result = unsuccessful_get_odm_ModelName;
    /* perform test */
    result = get_tlr_platform_name(&model_name);
    /* check result */
    CU_ASSERT_EQUAL(result, -1);


}

static void test_get_tlr_rom_id(void)
{
    char const * expected_rom_id = "expected ROM ID";
    char const * rom_id;
    int result;
    int const successful_get_odm_ROMID = 1;
    int const unsuccessful_get_odm_ROMID = 0;

    /* test normal operation */
    /* setup */
    rom_id_to_return = expected_rom_id;
    get_odm_ROMID_result = successful_get_odm_ROMID;
    /* perform test */
    result = get_tlr_rom_id(&rom_id);
    /* check result */
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_STRING_EQUAL(rom_id, expected_rom_id);

    /* test failed get_odm_ROMID() call */
    /* setup */
    rom_id_to_return = expected_rom_id;
    get_odm_ROMID_result = unsuccessful_get_odm_ROMID;
    /* perform test */
    result = get_tlr_rom_id(&rom_id);
    /* check result */
    CU_ASSERT_EQUAL(result, -1);
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
    pSuite = CU_add_suite("odm_specific", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ( 0
         || (CU_add_test(pSuite, "test_get_tlr_platform_name", test_get_tlr_platform_name) == NULL)
         || (CU_add_test(pSuite, "test_get_tlr_rom_id", test_get_tlr_rom_id) == NULL)
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



