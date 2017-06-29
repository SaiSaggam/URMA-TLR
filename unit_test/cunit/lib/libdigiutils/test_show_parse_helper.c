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

#include "test_show_parse_helper.h"
#include "show_parse_helper.h"
#include "tlr_common.h"
#include "CUnit/CUnit.h"
#include <stdlib.h>
#include <string.h>

void test_parse_show_options_format_values() {
    #define GOOD_STATUS_VALUE 0
    #define BAD_STATUS_VALUE -1
    #define MAX_ARGS 3
    typedef struct test_items_st
    {
        char * arguments[MAX_ARGS];
        int expected_status;
        tlr_response_format_t expected_format;
    } test_items_st;

    test_items_st test_items[] =
    {
        {
            .arguments = {"./show_route"},
            .expected_status = GOOD_STATUS_VALUE,
            .expected_format = tlr_response_format_ascii
        },
        {
            .arguments = {"./show_route", "-fwhatever"},
            .expected_status = BAD_STATUS_VALUE,
            .expected_format = tlr_response_format_ascii
        },
        {
            .arguments = {"./show_route", "-fascii"},
            .expected_status = GOOD_STATUS_VALUE,
            .expected_format = tlr_response_format_ascii
        },
        {
            .arguments = {"./show_route", "whatever", "-fjson"},
            .expected_status = GOOD_STATUS_VALUE,
            .expected_format = tlr_response_format_json
        },
        {   /* test that we get no error when instance == 0 */
            .arguments = {"./show_route", "-i0", "-fjson"},
            .expected_status = GOOD_STATUS_VALUE,
            .expected_format = tlr_response_format_json
        },
    };
	int instance = 0;
	tlr_response_format_t format;
    size_t number_of_tests = ARRAY_SIZE(test_items);

    for (int i = 0; i < number_of_tests; i++) {
        char ** argv = calloc(sizeof(char *), MAX_ARGS);
		int argc = 0;
		for (int j = 0; j < MAX_ARGS; j++) {
			if (test_items[i].arguments[j] != NULL) {
				argv[j] = strdup(test_items[i].arguments[j]);
				argc++;
			}
		}
		int status = parse_show_options(argc, argv, &format, &instance, 0, 0);
		CU_ASSERT_EQUAL(status, test_items[i].expected_status);
        CU_ASSERT_EQUAL(format, test_items[i].expected_format);

        /* cleanup */
        for (int i = 0; i < MAX_ARGS; i++)
        {
            free(argv[i]);
        }
        free(argv);
    }
}

void test_parse_show_options_instance_values() {
    #define GOOD_STATUS_VALUE 0
    #define BAD_STATUS_VALUE -1
    #define MAX_ARGS 3
    #define MIN_INSTANCE 1
    #define MID_INSTANCE 2
    #define MAX_INSTANCE 3
    #define BAD_INSTANCE 4
    #define STRINGIFY(x) #x
    #define XSTRINGIFY(x) STRINGIFY(x)

    #define MIN_INSTANCE_STR XSTRINGIFY(MIN_INSTANCE)
    #define MID_INSTANCE_STR XSTRINGIFY(MID_INSTANCE)
    #define MAX_INSTANCE_STR XSTRINGIFY(MAX_INSTANCE)
    #define BAD_INSTANCE_STR XSTRINGIFY(BAD_INSTANCE)

    typedef struct test_items_st
    {
        char * arguments[MAX_ARGS];
        int min_instance;
        int max_instance;
        int expected_status;
        int expected_instance;
    } test_items_st;

    test_items_st test_items[] =
    {
        {   /* instance not specified should return min_instance */
            .arguments = {"./show_route"},
            .min_instance = MIN_INSTANCE,
            .max_instance = MAX_INSTANCE,
            .expected_status = GOOD_STATUS_VALUE,
            .expected_instance = MIN_INSTANCE
        },
        {   /* test instance == minimum instance is OK */
            .arguments = {"./show_route", "-i"MIN_INSTANCE_STR, "-fascii"},
            .min_instance = MIN_INSTANCE,
            .max_instance = MAX_INSTANCE,
            .expected_status = GOOD_STATUS_VALUE,
            .expected_instance = MIN_INSTANCE
        },
        {   /* test MIN_INSTANCE < instance < MAX_INSTANCE is OK */
            .arguments = {"./show_route", "-i"MID_INSTANCE_STR, "-fascii"},
            .min_instance = MIN_INSTANCE,
            .max_instance = MAX_INSTANCE,
            .expected_status = GOOD_STATUS_VALUE,
            .expected_instance = MID_INSTANCE
        },
        {   /* test instance == maximum instance is OK */
            .arguments = {"./show_route", "-i"MAX_INSTANCE_STR, "-fascii"},
            .min_instance = MIN_INSTANCE,
            .max_instance = MAX_INSTANCE,
            .expected_status = GOOD_STATUS_VALUE,
            .expected_instance = MAX_INSTANCE
        },
        {   /* test instance <==> maximum instance is not OK */
            .arguments = {"./show_route", "-i"BAD_INSTANCE_STR, "-fascii"},
            .min_instance = MIN_INSTANCE,
            .max_instance = MAX_INSTANCE,
            .expected_status = BAD_STATUS_VALUE,
            .expected_instance = BAD_INSTANCE
        },
    };
	int instance = 0;
	tlr_response_format_t format;
    size_t number_of_tests = ARRAY_SIZE(test_items);

    for (int i = 0; i < number_of_tests; i++) {
        char ** argv = calloc(sizeof(char *), MAX_ARGS);
		int argc = 0;
		for (int j = 0; j < MAX_ARGS; j++) {
			if (test_items[i].arguments[j] != NULL) {
				argv[j] = strdup(test_items[i].arguments[j]);
				argc++;
			}
		}
        instance = -1;
		int status = parse_show_options(argc, argv, &format, &instance, test_items[i].min_instance, test_items[i].max_instance);
		CU_ASSERT_EQUAL(status, test_items[i].expected_status);
        CU_ASSERT_EQUAL(instance, test_items[i].expected_instance);

        /* cleanup */
        for (int i = 0; i < MAX_ARGS; i++)
        {
            free(argv[i]);
        }
        free(argv);
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
    CU_TEST_INFO(test_parse_show_options_format_values),
    CU_TEST_INFO(test_parse_show_options_instance_values),
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo test_parse_show_options_suites[] =
{
    { "test_parse_show_options", init_suite, clean_suite, tests },
    CU_SUITE_INFO_NULL,
};
