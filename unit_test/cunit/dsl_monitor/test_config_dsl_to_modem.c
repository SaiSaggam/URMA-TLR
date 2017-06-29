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

#include "test_config_dsl_to_modem.h"
#include "config_dsl_to_modem.h"
#include "tlr_common.h"
#include "CUnit/CUnit.h"

static void test_config_dsl_mode_to_modem(void)
{
	struct
	{
		config_dsl_mode_t mode;
		char const * expected_result;
	} const test_cases[] =
	{
		{
			.mode = config_dsl_mode_auto,
			.expected_result = "adsl2plus_multi",
		},
		{
			.mode = config_dsl_mode_adsl2_plus,
			.expected_result = "adsl2plus",
		},
		{
			.mode = config_dsl_mode_adsl2,
			.expected_result = "adsl2",
		},
		{
			.mode = config_dsl_mode_gdmt,
			.expected_result = "adsl1",
		},
		{
			.mode = config_dsl_mode_glite,
			.expected_result = "lite",
		},
	};
	size_t i;

	for (i = 0; i < ARRAY_SIZE(test_cases); i++)
	{
		int const mode = test_cases[i].mode;
		char const * const expected_result = test_cases[i].expected_result;
		char const * const result = config_dsl_mode_to_modem(mode);
		CU_ASSERT_STRING_EQUAL(result, expected_result);
	}
}

static void test_config_dsl_encapsulation_to_modem_encap(void)
{
	struct
	{
		config_dsl_encapsulation_t encapsulation;
		int expected_result;
	} const test_cases[] =
	{
		{
			.encapsulation = config_dsl_encapsulation_pppoa_vcmux,
			.expected_result = 1,
		},
		{
			.encapsulation = config_dsl_encapsulation_pppoa_llc,
			.expected_result = 0,
		},
		{
			.encapsulation = config_dsl_encapsulation_pppoe_vcmux,
			.expected_result = 1,
		},
		{
			.encapsulation = config_dsl_encapsulation_pppoe_llc,
			.expected_result = 0,
		},
	};
	size_t i;

	for (i = 0; i < ARRAY_SIZE(test_cases); i++)
	{
		int const encapsulation = test_cases[i].encapsulation;
		int const expected_result = test_cases[i].expected_result;
		int const result = config_dsl_encapsulation_to_modem_encap(encapsulation);
		CU_ASSERT_EQUAL(result, expected_result);
	}
}

static void test_config_dsl_encapsulation_to_modem_mode(void)
{
	struct
	{
		config_dsl_encapsulation_t encapsulation;
		int expected_result;
	} const test_cases[] =
	{
		{
			.encapsulation = config_dsl_encapsulation_pppoa_vcmux,
			.expected_result = 1,
		},
		{
			.encapsulation = config_dsl_encapsulation_pppoa_llc,
			.expected_result = 1,
		},
		{
			.encapsulation = config_dsl_encapsulation_pppoe_vcmux,
			.expected_result = 0,
		},
		{
			.encapsulation = config_dsl_encapsulation_pppoe_llc,
			.expected_result = 0,
		},
	};
	size_t i;

	for (i = 0; i < ARRAY_SIZE(test_cases); i++)
	{
		int const encapsulation = test_cases[i].encapsulation;
		int const expected_result = test_cases[i].expected_result;
		int const result = config_dsl_encapsulation_to_modem_mode(encapsulation);
		CU_ASSERT_EQUAL(result, expected_result);
	}
}

static int init_suite(void)
{
    return 0;
}

static int clean_suite(void)
{
    return 0;
}

#define CU_TEST_INFO(test) { #test, test }

static CU_TestInfo tests[] =
{
    CU_TEST_INFO(test_config_dsl_mode_to_modem),
    CU_TEST_INFO(test_config_dsl_encapsulation_to_modem_encap),
    CU_TEST_INFO(test_config_dsl_encapsulation_to_modem_mode),
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo test_config_dsl_to_modem_suites[] =
{
    { "config_dsl_to_modem", init_suite, clean_suite, tests },
    CU_SUITE_INFO_NULL,
};

