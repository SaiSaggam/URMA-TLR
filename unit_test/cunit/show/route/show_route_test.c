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

#include "show_route.h"
#include "show_parse_helper.h"

/********************
	Stub Functions
********************/

void print_error(char * message) {}
void print_message(char * message) {}
void print_header(tlr_response_format_t const format) {}
void print_ascii(char * const route[]) {}
void print_json(json_object * json_obj) {}

/********************
	Test Suite Setup
********************/

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/********************
	Test Case Functions
********************/


void show_test_1() {
	int instance = 1;
	FILE * test_file;
	char * test_filename = "test.table";
	test_file = fopen(test_filename, "r");
	tlr_response_format_t formats[] = {
		tlr_response_format_ascii,
		tlr_response_format_json
	};
	int status_expected[] = {0, 0};
	size_t size = sizeof formats / sizeof formats[0];
	for (int i = 0; i < size; i++) {
		rewind(test_file);
		int status = show(TEMP_FILE_NAME, test_file, formats[i], instance);
		CU_ASSERT_EQUAL(status, status_expected[i]);
	}
	if (test_file != NULL)
		fclose(test_file);
}

void parse_route_test_1() {
	char * values_expected[][value_count] = {
		{"0.0.0.0", "10.52.18.1", "0.0.0.0", "UG", "0", "0", "0", "eth0"},
		{"192.168.1.0", "192.168.5.1", "255.255.255.0", "U", "0", "0", "0", "eth1"},
		{"192.168.2.0", "192.168.5.2", "255.255.255.128", "D", "2", "0", "0", "eth0"},
		{"192.168.2.128", "192.168.5.3", "255.255.255.128", "U", "5", "0", "0", "eth2"},
		{"192.168.3.0", "192.168.5.4", "255.255.255.0", "U", "0", "0", "0", "eth0"}
	};
	FILE * test_file;
	char * test_filename = "test.table";
	test_file = fopen(test_filename, "r");
	char line[CMD_LEN];
	char ** values = calloc(sizeof(char *), value_count);
	size_t row = sizeof values_expected / sizeof values_expected[0];
	size_t col = sizeof values_expected[0] / sizeof values_expected[0][0];
	fgets(line, sizeof line, test_file);
	fgets(line, sizeof line, test_file);
	for (int i = 0; i < row; i++)
		if (fgets(line, sizeof line, test_file) != NULL) {
			int status = parse_route(line, values);
			CU_ASSERT_EQUAL(status, 0);
			for (int j = 0; j < col; j++)
				CU_ASSERT_STRING_EQUAL(values[j], values_expected[i][j]);
		}
	if (test_file != NULL)
		fclose(test_file);
	if (values != NULL) {
		for (int i = 0; i < value_count; i++)
			if (values[i] != NULL)
				free(values[i]);
		free(values);
	}
}

void process_route_test_1() {
	char * value_table[][value_count] = {
		{"0.0.0.0", "10.52.18.1", "0.0.0.0", "UG", "0", "0", "0", "eth0"},
		{"192.168.1.0", "192.168.5.1", "255.255.255.0", "U", "0", "0", "0", "eth1"},
		{"192.168.2.0", "192.168.5.2", "255.255.255.128", "D", "2", "0", "0", "eth0"},
		{"192.168.2.128", "192.168.5.3", "255.255.255.128", "U", "5", "0", "0", "eth2"},
		{"192.168.3.0", "192.168.5.4", "255.255.255.0", "U", "0", "0", "0", "eth0"}
	};
	char * route_expected[][route_count] = {
		{"default", "10.52.18.1", "0", "Static", "6", "eth0", "UP"},
		{"192.168.1.0/24", "192.168.5.1", "0", "Connected", "0", "eth1", "UP"},
		{"192.168.2.0/25", "192.168.5.2", "2", "Connected", "4", "eth0", "DOWN"},
		{"192.168.2.128/25", "192.168.5.3", "5", "Connected", "2", "eth2", "UP"},
		{"192.168.3.0/24", "192.168.5.4", "0", "Connected", "1", "eth0", "UP"},
	};
	char ** route  = calloc(sizeof(char *), route_count);
	char ** values  = calloc(sizeof(char *), value_count);
	size_t row = sizeof value_table / sizeof value_table[0];

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < value_count; j++) {
			values[j] = malloc(VAL_LEN * sizeof(char));
			strcpy(values[j], value_table[i][j]);
		}
		int status = process_route(TEMP_FILE_NAME, route, values);
		CU_ASSERT_EQUAL(status, 0);
		for (int j = 0; j < route_count; j++)
			CU_ASSERT_STRING_EQUAL(route[j], route_expected[i][j]);
	}
	if (values != NULL) {
		for (int i = 0; i < value_count; i++)
			if (values[i] != NULL)
				free(values[i]);
		free(values);
	}
	if (route != NULL) {
		for (int i = 0; i < route_count; i++)
			if (route[i] != NULL)
				free(route[i]);
		free(route);
	}
}

void mask_to_cidr_test_1() {
	char * mask[] = {
		"0.0.0.0",
		"128.0.0.0",
		"192.0.0.0",
		"224.0.0.0",
		"240.0.0.0",
		"248.0.0.0",
		"252.0.0.0",
		"254.0.0.0",
		"255.0.0.0",
		"255.128.0.0",
		"255.192.0.0",
		"255.224.0.0",
		"255.240.0.0",
		"255.248.0.0",
		"255.252.0.0",
		"255.254.0.0",
		"255.255.0.0",
		"255.255.128.0",
		"255.255.192.0",
		"255.255.224.0",
		"255.255.240.0",
		"255.255.248.0",
		"255.255.252.0",
		"255.255.254.0",
		"255.255.255.0",
		"255.255.255.128",
		"255.255.255.192",
		"255.255.255.224",
		"255.255.255.240",
		"255.255.255.248",
		"255.255.255.252",
		"255.255.255.254",
		"255.255.255.255"
	};
	size_t size = sizeof mask / sizeof mask[0];
	for (int i = 0; i < size; i++) {
		int cidr;
		int status = mask_to_cidr(&cidr, mask[i]);
		CU_ASSERT_EQUAL(status, 0);
		CU_ASSERT_EQUAL(cidr, i);
	}
}

void get_route_index_test_1() {
	char * value_table[][value_count] = {
		{"0.0.0.0", "10.52.18.1", "0.0.0.0", "UG", "0", "0", "0", "eth0"},
		{"192.168.1.0", "192.168.5.1", "255.255.255.0", "U", "0", "0", "0", "eth1"},
		{"192.168.2.0", "192.168.5.2", "255.255.255.128", "D", "2", "0", "0", "eth0"},
		{"192.168.2.128", "192.168.5.3", "255.255.255.128", "U", "5", "0", "0", "eth2"},
		{"192.168.3.0", "192.168.5.4", "255.255.255.0", "U", "0", "0", "0", "eth0"}
	};
	int index_expected[] = {6, 0, 4, 2, 1};
	int index;
	char ** values  = calloc(sizeof(char *), value_count);
	size_t row = sizeof value_table / sizeof value_table[0];

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < value_count; j++) {
			values[j] = malloc(VAL_LEN * sizeof(char));
			strcpy(values[j], value_table[i][j]);
		}
		int status = get_route_index(TEMP_FILE_NAME, &index, values);
		CU_ASSERT_EQUAL(status, 0);
		CU_ASSERT_EQUAL(index, index_expected[i]);
	}
	if (values != NULL) {
		for (int i = 0; i < value_count; i++)
			if (values[i] != NULL)
				free(values[i]);
		free(values);
	}
}

void get_route_index_test_2() {
	int index;
	char * values[] = {"whatever", "invalid", "255.255.255.0", "U", "0", "0", "0", "eth0"};
	int status = get_route_index(TEMP_FILE_NAME, &index, values);
	CU_ASSERT_EQUAL(status, -1);
	CU_ASSERT_EQUAL(index, -1);
}

void build_json_route_test_1() {
	char const * string;
	char * routes[][route_count] = {
		{"0.0.0.0/0", "10.52.18.1", "0", "Static", "6", "eth0", "UP"},
		{"192.168.1.0/24", "192.168.5.1", "0", "Connected", "0", "eth1", "UP"},
		{"192.168.2.0/25", "192.168.5.2", "2", "Connected", "4", "eth0", "DOWN"},
		{"192.168.2.128/25", "192.168.5.3", "5", "Connected", "2", "eth2", "UP"},
		{"192.168.3.0/24", "192.168.5.4", "0", "Connected", "1", "eth0", "UP"},
	};
	char * string_expected[] = {
		"{ \"0\": { \"Destination\": \"0.0.0.0\\/0\", \"Gateway\": \"10.52.18.1\", \"Metric\": \"0\", \"Protocol\": \"Static\", \"Index\": \"6\", \"Interface\": \"eth0\", \"Status\": \"UP\" } }",
		"{ \"1\": { \"Destination\": \"192.168.1.0\\/24\", \"Gateway\": \"192.168.5.1\", \"Metric\": \"0\", \"Protocol\": \"Connected\", \"Index\": \"0\", \"Interface\": \"eth1\", \"Status\": \"UP\" } }",
		"{ \"2\": { \"Destination\": \"192.168.2.0\\/25\", \"Gateway\": \"192.168.5.2\", \"Metric\": \"2\", \"Protocol\": \"Connected\", \"Index\": \"4\", \"Interface\": \"eth0\", \"Status\": \"DOWN\" } }",
		"{ \"3\": { \"Destination\": \"192.168.2.128\\/25\", \"Gateway\": \"192.168.5.3\", \"Metric\": \"5\", \"Protocol\": \"Connected\", \"Index\": \"2\", \"Interface\": \"eth2\", \"Status\": \"UP\" } }",
		"{ \"4\": { \"Destination\": \"192.168.3.0\\/24\", \"Gateway\": \"192.168.5.4\", \"Metric\": \"0\", \"Protocol\": \"Connected\", \"Index\": \"1\", \"Interface\": \"eth0\", \"Status\": \"UP\" } }"
	};

	char ** route  = calloc(sizeof(char *), route_count);
	size_t row = sizeof routes / sizeof routes[0];

	for (int i = 0; i < row; i++) {
		json_object * json_obj = json_object_new_object();
		for (int j = 0; j < route_count; j++) {
			route[j] = malloc(VAL_LEN * sizeof(char));
			strcpy(route[j], routes[i][j]);
		}
		build_json_route(route, json_obj, i);
		string = json_object_to_json_string(json_obj);
		CU_ASSERT_STRING_EQUAL(string, string_expected[i]);
	}
	if (route != NULL) {
		for (int i = 0; i < route_count; i++)
			if (route[i] != NULL)
				free(route[i]);
		free(route);
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
	CU_pSuite pSuite = CU_add_suite( "show_route_test_suite", init_suite, clean_suite );
	if ( pSuite == NULL ) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	// Add the tests to the suite
	if ( 0 ||
#if defined(MOVE_TO_LIBDIGIUTILS)
		(CU_add_test(pSuite, "get_format_test_1", get_format_test_1)==NULL) ||
#endif
		(CU_add_test(pSuite, "show_test_1", show_test_1)==NULL) ||
		(CU_add_test(pSuite, "parse_route_test_1", parse_route_test_1)==NULL) ||
		(CU_add_test(pSuite, "process_route_test_1", process_route_test_1)==NULL) ||
		(CU_add_test(pSuite, "mask_to_cidr_test_1", mask_to_cidr_test_1)==NULL) ||
		(CU_add_test(pSuite, "get_route_index_test_1", get_route_index_test_1)==NULL) ||
		(CU_add_test(pSuite, "get_route_index_test_2", get_route_index_test_2)==NULL) ||
		(CU_add_test(pSuite, "build_json_route_test_1", build_json_route_test_1)==NULL)
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
