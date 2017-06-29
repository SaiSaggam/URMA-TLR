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

#include "test_modem_to_config_state_dsl.h"
#include "modem_to_config_state_dsl.h"
#include "config_mgr.h"
#include "tlr_common.h"
#include "CUnit/CUnit.h"

static void test_modem_to_config_state_dsl_line_mode(void)
{
	struct
	{
		char const * line_mode;
		config_state_dsl_line_mode_t expected_result;
	} const test_cases[] =
	{
		{
			.line_mode = "Multi-Mode",
			.expected_result = config_state_dsl_line_mode_unknown,
		},
		{
			.line_mode = "ADSL2 PLUS",
			.expected_result = config_state_dsl_line_mode_adsl2_plus,
		},
		{
			.line_mode = "ADSL2",
			.expected_result = config_state_dsl_line_mode_adsl2,
		},
		{
			.line_mode = "G.DMT",
			.expected_result = config_state_dsl_line_mode_gdmt,
		},
		{
			.line_mode = "G.LITE",
			.expected_result = config_state_dsl_line_mode_glite,
		},
		{
			.line_mode = "",
			.expected_result = config_state_dsl_line_mode_unknown,
		},
		{
			.line_mode = "some unexpected line mode",
			.expected_result = config_state_dsl_line_mode_unknown,
		},
		{
			.line_mode = "line mode\nwith newline",
			.expected_result = config_state_dsl_line_mode_unknown,
		},
	};
	size_t i;

	for (i = 0; i < ARRAY_SIZE(test_cases); i++)
	{
		char const * const line_mode = test_cases[i].line_mode;
		config_state_dsl_line_mode_t const expected_result = test_cases[i].expected_result;
		config_state_dsl_line_mode_t const result = modem_to_config_state_dsl_line_mode(line_mode);
		CU_ASSERT_EQUAL(result, expected_result);
	}
}

static void test_modem_to_config_state_dsl_line_status(void)
{
	struct
	{
		char const * line_status;
		config_state_dsl_line_status_t expected_result;
	} const test_cases[] =
	{
		{
			.line_status = "up",
			.expected_result = config_state_dsl_line_status_up,
		},
		{
			.line_status = "down",
			.expected_result = config_state_dsl_line_status_down,
		},
		{
			.line_status = "wait for initialization",
			.expected_result = config_state_dsl_line_status_waiting_for_initialisation,
		},
		{
			.line_status = "initializing",
			.expected_result = config_state_dsl_line_status_initialising,
		},
		{
			.line_status = "",
			.expected_result = config_state_dsl_line_status_unknown,
		},
		{
			.line_status = "some unexpected line status",
			.expected_result = config_state_dsl_line_status_unknown,
		},
		{
			.line_status = "line status\nwith newline",
			.expected_result = config_state_dsl_line_status_unknown,
		},
	};
	size_t i;

	for (i = 0; i < ARRAY_SIZE(test_cases); i++)
	{
		char const * const line_status = test_cases[i].line_status;
		config_state_dsl_line_status_t const expected_result = test_cases[i].expected_result;
		config_state_dsl_line_status_t const result = modem_to_config_state_dsl_line_status(line_status);
		CU_ASSERT_EQUAL(result, expected_result);
	}
}

static void test_modem_to_config_state_dsl_channel_type(void)
{
	struct
	{
		char const * channel_type;
		config_state_dsl_downstream_channel_type_t expected_result;
	} const test_cases[] =
	{
		{
			.channel_type = "fast",
			.expected_result = config_state_dsl_downstream_channel_type_fast,
		},
		{
			.channel_type = "interleaved",
			.expected_result = config_state_dsl_downstream_channel_type_interleaved,
		},
		{
			.channel_type = "",
			.expected_result = config_state_dsl_downstream_channel_type_unknown,
		},
		{
			.channel_type = "some unexpected channel type",
			.expected_result = config_state_dsl_downstream_channel_type_unknown,
		},
		{
			.channel_type = "channel type\nwith newline",
			.expected_result = config_state_dsl_downstream_channel_type_unknown,
		},
	};
	size_t i;

	for (i = 0; i < ARRAY_SIZE(test_cases); i++)
	{
		char const * const channel_type = test_cases[i].channel_type;
		config_state_dsl_downstream_channel_type_t const expected_result = test_cases[i].expected_result;
		config_state_dsl_downstream_channel_type_t const result = modem_to_config_state_dsl_channel_type(channel_type);
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
    CU_TEST_INFO(test_modem_to_config_state_dsl_line_mode),
    CU_TEST_INFO(test_modem_to_config_state_dsl_line_status),
    CU_TEST_INFO(test_modem_to_config_state_dsl_channel_type),
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo test_modem_to_config_state_dsl_suites[] =
{
    { "modem_to_config_state_dsl", init_suite, clean_suite, tests },
    CU_SUITE_INFO_NULL,
};

config_mgr_element_enum_limit_t const * tlr_get_element_enum_limit(config_mgr_table_type_t const type, size_t const group_id, size_t const element_id)
{
    config_mgr_group_table_t const * const table_ptr = &config_mgr_group_table[config_mgr_get_group];
    config_mgr_group_t const * const group_ptr = &table_ptr->groups[group_id];
    config_mgr_element_t const * const element_ptr = &group_ptr->elements.element[element_id];
    config_mgr_element_enum_limit_t const * const enum_limit_ptr = (config_mgr_element_enum_limit_t *)element_ptr->value_limit;

    return enum_limit_ptr;
}

