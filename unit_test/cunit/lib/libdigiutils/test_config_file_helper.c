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

#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include "config_file_helper.h"
#include "config_mgr.h"

#define READ_BUFFER_SIZE	100		/*ensure is large enough to contain all expected results*/

/*data*/
static bool element_needs_quoting_result;
static bool get_element_type_result;
static char temporary_filename[PATH_MAX] = "/tmp/test_write_command_XXXXXX";
static char read_buffer[READ_BUFFER_SIZE];
static int fd = -1;
static FILE * fp = NULL;


/* Test Suite setup and cleanup functions: */

static void open_temp_file(void)
{

	fd = mkstemp(temporary_filename);
	if (fd >= 0)
		fp = fdopen(fd, "w+");
	else
	{
		fprintf(stderr, "failed to open temp file\n");
		exit(EXIT_FAILURE);
	}
}

static void cleanup_temp_file(void)
{
	if (fd >= 0)
	{
		close(fd);
		remove(temporary_filename);
	}
}

static void reset_temp_file(void)
{
	read_buffer[0] = '\0';
	rewind(fp);
	ftruncate(fd, 0);
}

static char * get_value_written_to_file(void)
{
	int bytes_read = 0, total_bytes_read = 0;

	fseek(fp, 0, SEEK_SET);
	while ((bytes_read = fread(&read_buffer[total_bytes_read], 1, READ_BUFFER_SIZE - 1, fp)) > 0)
	{
		total_bytes_read += bytes_read;
	}
	read_buffer[total_bytes_read] = '\0';

	return read_buffer;
}

static int init_suite(void)
{
	open_temp_file();
	return 0;
}

static int clean_suite(void)
{
	cleanup_temp_file();
	return 0;
}

/************* Test case functions ****************/

static void test_parameter_should_be_quoted(void)
{
	bool result;
	extern bool parameter_should_be_quoted(config_mgr_group_t const * const group, char const * const digi_parameter_name);

	/*test element type found and element needs quoting*/
	/*setup*/
	get_element_type_result = true;
	element_needs_quoting_result = true;
	/*perform test*/
	result = parameter_should_be_quoted(NULL, NULL);
	/*check result*/
	CU_ASSERT_TRUE(result);

	/*test element type found and element doesn't need quoting*/
	/*setup*/
	get_element_type_result = true;
	element_needs_quoting_result = false;
	/*perform test*/
	result = parameter_should_be_quoted(NULL, NULL);
	/*check result*/
	CU_ASSERT_FALSE(result);

	/*test element type not found returns true*/
	/*setup*/
	get_element_type_result = false;
	element_needs_quoting_result = false;
	/*perform test*/
	result = parameter_should_be_quoted(NULL, NULL);
	/*check result*/
	CU_ASSERT_TRUE(result);
}

static void test_output_parameter_value(void)
{
	extern void output_parameter_value(config_mgr_group_t const * const group, char const * const digi_parameter_name, struct uci_option * o, FILE * fp);
	struct uci_option o;
	char * string_to_test = "test_string";
	char * expected_result_quotes = "\"test_string\"";
	char * expected_result_no_quotes = "test_string";

	/*setup*/
	/* get parameter _should_be_quoted() to return true*/
	get_element_type_result = true;
	element_needs_quoting_result = true;
	o.type = UCI_TYPE_STRING;
	o.v.string = string_to_test;
	reset_temp_file();
	/*perform test*/
	output_parameter_value(NULL, NULL, &o, fp);
	/*check result*/
	/* as we've indicated in the setup that the output requires quotes, the result should be surrounded with quotes*/
	CU_ASSERT_STRING_EQUAL(get_value_written_to_file(), expected_result_quotes);


	/*setup*/
	/* get parameter should _not_ be_quoted() to return true*/
	get_element_type_result = true;
	element_needs_quoting_result = false;
	o.type = UCI_TYPE_STRING;
	o.v.string = string_to_test;
	reset_temp_file();
	/*perform test*/
	output_parameter_value(NULL, NULL, &o, fp);
	/*check result*/
	/* as we've indicated in the setup that the output requires quotes, the result should be surrounded with quotes*/
	CU_ASSERT_STRING_EQUAL(get_value_written_to_file(), expected_result_no_quotes);

	// XXX how to test LIST type option

}

static void test_write_config_commands_to_file(void)
{
    extern void write_config_commands_to_file(config_mgr_group_t const * const group, char const * const section, struct uci_option * o, FILE * const fp);
	struct uci_option o;
	struct uci_section section;
	char * expected_result = "group 123 parameter value\n";
	config_mgr_group_t group;
	struct uci_package package;

	/*test when section name is longer than supplied string*/
	/*setup*/
	/* get parameter _should_be_quoted() to return true*/
	get_element_type_result = true;
	element_needs_quoting_result = false;
	o.type = UCI_TYPE_STRING;
	o.v.string = "value";
	o.e.name = "parameter";
	section.e.name = "group123";
	o.section = &section;
	package.e.name = "any old crud";
	section.package = &package;
	reset_temp_file();
	/*perform test*/
	write_config_commands_to_file(&group, "group", &o, fp);
	/*check result*/
	CU_ASSERT_STRING_EQUAL(get_value_written_to_file(), expected_result);

	/*test when section name is shorter than supplied string*/
	/*setup*/
	/* get parameter _should_be_quoted() to return true*/
	expected_result = "";
	get_element_type_result = true;
	element_needs_quoting_result = false;
	o.type = UCI_TYPE_STRING;
	o.v.string = "value";
	o.e.name = "parameter";
	section.e.name = "123";	/*shorter than supplied 'section' argument*/
	o.section = &section;
	package.e.name = "any old crud";
	section.package = &package;
	reset_temp_file();
	/*perform test*/
	write_config_commands_to_file(&group, "group", &o, fp);
	/*check result*/
	CU_ASSERT_STRING_EQUAL(get_value_written_to_file(), expected_result);

}
/************* Test Runner Code goes here **************/

static int add_tests(void)
{
	CU_pSuite pSuite = NULL;

	/* add a suite to the registry */
	pSuite = CU_add_suite( "config_file_helper", init_suite, clean_suite );
	if ( NULL == pSuite )
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if ( 0
		 || CU_add_test(pSuite, "test_parameter_should_be_quoted", test_parameter_should_be_quoted)==NULL
		 || CU_add_test(pSuite, "test_output_parameter_value", test_output_parameter_value)==NULL
		 || CU_add_test(pSuite, "test_write_config_commands_to_file", test_write_config_commands_to_file)==NULL
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

int strlcpy_uci_to_digi(char * const digi, char const * const uci, size_t size)
{
	strcpy(digi, uci);

	return strlen(digi);
}

bool element_needs_quoting(config_mgr_element_type_t element_type)
{
	return element_needs_quoting_result;
}

bool get_element_type(config_mgr_group_t const * const group, char const * const digi_parameter_name, config_mgr_element_type_t * const element_type)
{
	return get_element_type_result;
}
