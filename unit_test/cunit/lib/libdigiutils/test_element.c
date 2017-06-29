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

#include "test_element.h"
#include "element.h"
#include "tlr_common.h"
#include "CUnit/CUnit.h"
#include <string.h>

static int init_suite(void)
{
    return 0;
}

static int clean_suite(void)
{
    return 0;
}

static void test_element_needs_quoting(void)
{
	extern bool element_needs_quoting(config_mgr_element_type_t element_type);
	bool result, expected_value;
	config_mgr_element_type_t type_to_test;

	/*test that integer types return false, and others return true*/
	/*setup*/
	expected_value = false;
	type_to_test = config_mgr_element_type_uint32;
	/*perform test*/
	result = element_needs_quoting(type_to_test);
	/*check result*/
	CU_ASSERT_EQUAL(result, expected_value);

	/*setup*/
	expected_value = false;
	type_to_test = config_mgr_element_type_int32;
	/*perform test*/
	result = element_needs_quoting(type_to_test);
	/*check result*/
	CU_ASSERT_EQUAL(result, expected_value);

	/*setup*/
	expected_value = false;
	type_to_test = config_mgr_element_type_index;
	/*perform test*/
	result = element_needs_quoting(type_to_test);
	/*check result*/
	CU_ASSERT_EQUAL(result, expected_value);

	/*setup*/
	expected_value = true;
	type_to_test = config_mgr_element_type_string;
	/*perform test*/
	result = element_needs_quoting(type_to_test);
	/*check result*/
	CU_ASSERT_EQUAL(result, expected_value);

	/*setup*/
	expected_value = true;
	type_to_test = config_mgr_element_type_password;
	/*perform test*/
	result = element_needs_quoting(type_to_test);
	/*check result*/
	CU_ASSERT_EQUAL(result, expected_value);

	/*setup*/
	expected_value = true;
	type_to_test = config_mgr_element_type_enum;
	/*perform test*/
	result = element_needs_quoting(type_to_test);
	/*check result*/
	CU_ASSERT_EQUAL(result, expected_value);

	/*setup*/
	expected_value = true;
	type_to_test = config_mgr_element_type_on_off;
	/*perform test*/
	result = element_needs_quoting(type_to_test);
	/*check result*/
	CU_ASSERT_EQUAL(result, expected_value);

	/*I can't be bothered testing all cases as the function's task is pretty obvious*/
}

static void test_get_element_type(void)
{
	extern bool get_element_type(config_mgr_group_t const * const group, char const * const digi_parameter_name, config_mgr_element_type_t * const element_type);
	config_mgr_element_t elements[3];
	config_mgr_group_t group;
	config_mgr_element_type_t expected_element_type, unexpected_element_type, actual_element_type;
	char * digi_parameter_name, * element_name;
	bool result;

	/*setup to match on first index*/
	/*test matching name returns true along with expected element type*/
	/*setup*/
	expected_element_type = config_mgr_element_type_index;
	unexpected_element_type = config_mgr_element_type_uint32;
	digi_parameter_name = element_name = "test";
	elements[0].type = expected_element_type;
	elements[0].name = element_name;
	/* ensure we don't match on indexes other than the one we're interested in*/
	elements[1].type = unexpected_element_type;
	elements[1].name = "non-matching";
	elements[2].type = unexpected_element_type;
	elements[2].name = "non-matching";

	group.elements.count = ARRAY_SIZE(elements);
	group.elements.element = &elements[0];
	/*perform test*/
	result = get_element_type(&group, digi_parameter_name, &actual_element_type);
	/*check result*/
	CU_ASSERT_TRUE(result);
	CU_ASSERT_EQUAL(actual_element_type, expected_element_type);

	/*setup to match on last index*/
	/*setup*/
	expected_element_type = config_mgr_element_type_index;
	unexpected_element_type = config_mgr_element_type_uint32;
	digi_parameter_name = element_name = "test";
	elements[2].type = expected_element_type;
	elements[2].name = element_name;
	/* ensure we don't match on indexes other than the one we're interested in*/
	elements[1].type = unexpected_element_type;
	elements[1].name = "non-matching";
	elements[0].type = unexpected_element_type;
	elements[0].name = "non-matching";

	group.elements.count = ARRAY_SIZE(elements);
	group.elements.element = &elements[0];
	/*perform test*/
	result = get_element_type(&group, digi_parameter_name, &actual_element_type);
	/*check result*/
	CU_ASSERT_TRUE(result);
	CU_ASSERT_EQUAL(actual_element_type, expected_element_type);

	/*setup to match on index that isn't first or last*/
	/*setup*/
	expected_element_type = config_mgr_element_type_index;
	unexpected_element_type = config_mgr_element_type_uint32;
	digi_parameter_name = element_name = "test";
	elements[1].type = expected_element_type;
	elements[1].name = element_name;
	/* ensure we don't match on indexes other than the one we're interested in*/
	elements[2].type = unexpected_element_type;
	elements[2].name = "non-matching";
	elements[0].type = unexpected_element_type;
	elements[0].name = "non-matching";

	group.elements.count = ARRAY_SIZE(elements);
	group.elements.element = &elements[0];
	/*perform test*/
	result = get_element_type(&group, digi_parameter_name, &actual_element_type);
	/*check result*/
	CU_ASSERT_TRUE(result);
	CU_ASSERT_EQUAL(actual_element_type, expected_element_type);

	/*ensure that the actual value follows the expected value by selecting a different element type*/
	/*setup*/
	expected_element_type = config_mgr_element_type_string;
	unexpected_element_type = config_mgr_element_type_uint32;
	digi_parameter_name = element_name = "test";
	elements[1].type = expected_element_type;
	elements[1].name = element_name;
	/* ensure we don't match on indexes other than the one we're interested in*/
	elements[2].type = unexpected_element_type;
	elements[2].name = "non-matching";
	elements[0].type = unexpected_element_type;
	elements[0].name = "non-matching";

	group.elements.count = ARRAY_SIZE(elements);
	group.elements.element = &elements[0];
	/*perform test*/
	result = get_element_type(&group, digi_parameter_name, &actual_element_type);
	/*check result*/
	CU_ASSERT_TRUE(result);
	CU_ASSERT_EQUAL(actual_element_type, expected_element_type);

	/*setup to ensure failure result with non-matching element name*/
	/*setup*/
	expected_element_type = config_mgr_element_type_index;
	unexpected_element_type = config_mgr_element_type_uint32;
	digi_parameter_name = "unique-name";	/*something that won't match on any index*/
	elements[1].type = expected_element_type;
	elements[1].name = "non-matching";
	/* ensure we don't match on indexes other than the one we're interested in*/
	elements[2].type = unexpected_element_type;
	elements[2].name = "non-matching";
	elements[0].type = unexpected_element_type;
	elements[0].name = "non-matching";

	group.elements.count = ARRAY_SIZE(elements);
	group.elements.element = &elements[0];
	/*perform test*/
	result = get_element_type(&group, digi_parameter_name, &actual_element_type);
	/*check result*/
	CU_ASSERT_FALSE(result);
}

#define CU_TEST_INFO(test) { #test, test }

static CU_TestInfo tests[] =
{
    CU_TEST_INFO(test_element_needs_quoting),
    CU_TEST_INFO(test_get_element_type),
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo test_element_suites[] =
{
    { "element", init_suite, clean_suite, tests },
    CU_SUITE_INFO_NULL,
};

