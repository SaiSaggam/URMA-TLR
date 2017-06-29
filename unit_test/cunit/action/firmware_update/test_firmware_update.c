#define     USE_BASIC_STYLE     1
#define     USE_AUTOMATED_STYLE     0
#define     USE_CONSOLE_STYLE     0

#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "tlr_print.h"
#include "flashbuf.h"

#if USE_BASIC_STYLE
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#elif USE_AUTOMATED_STYLE
#include "CUnit/Automated.h"
#else
#include "CUnit/Console.h"
#endif

#include "update_firmware.h"
#include <config_def.h>

// Flags used to force error conditions
bool action_parse_args_fail;
bool asprintf_fail;
bool exec_upgfile_fail;
bool parse_command_fail;
bool getline_fail;

typedef enum
{
    check_firmware_crc_error,
    check_firmware_open_error,
    check_firmware_bootloader,
    check_firmware_rootfs,
    check_firmware_unknown
} check_firmware_type;

static check_firmware_type check_firmware_result;

static FILE * cunit_fopen_return_value;
static char actual_filename[PATH_MAX];
static FILE * cunit_fclose_fp;

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

static void test_verify_firmware_file(void)
{
    image_type_t result;
	extern image_type_t verify_firmware_file(char * const file_path);

	check_firmware_result =  check_firmware_unknown;
	result = verify_firmware_file("/tmp/firmware.bin");
	CU_ASSERT_EQUAL(result, image_type_unknown);

	check_firmware_result =  check_firmware_crc_error;
    result = verify_firmware_file("/tmp/firmware.bin");
    CU_ASSERT_EQUAL(result, image_type_unknown);

    check_firmware_result =  check_firmware_open_error;
    result = verify_firmware_file("/tmp/firmware.bin");
    CU_ASSERT_EQUAL(result, image_type_unknown);

    check_firmware_result =  check_firmware_bootloader;
    result = verify_firmware_file("/tmp/firmware.bin");
    CU_ASSERT_EQUAL(result, image_type_bootloader);

    check_firmware_result =  check_firmware_rootfs;
    result = verify_firmware_file("/tmp/firmware.bin");
    CU_ASSERT_EQUAL(result, image_type_firmware);
}

static void test_update_firmware_main(void)
{
	bool result;
	bool update_firmware_main(int argc, char *argv[]);
	char * good_user_argv[] = {"update_firmware", "-u", "test_user", "-f", "ascii", "test.bin", NULL};
	int const good_user_argc = 6;
	char * bad_user_argv[] = {"update_firmware", "-u", "test_user", "-f", "ascii", NULL};
	int const bad_user_argc = 1;

	result = update_firmware_main(bad_user_argc, bad_user_argv);
	CU_ASSERT_EQUAL(result, false);

	action_parse_args_fail =  true;
	result = update_firmware_main(good_user_argc, good_user_argv);
	CU_ASSERT_EQUAL(result, false);
	action_parse_args_fail =  false;

	asprintf_fail =  true;
	result = update_firmware_main(good_user_argc, good_user_argv);
	CU_ASSERT_EQUAL(result, false);
	asprintf_fail =  false;

	check_firmware_result =  check_firmware_crc_error;
	result = update_firmware_main(good_user_argc, good_user_argv);
	CU_ASSERT_EQUAL(result, image_type_unknown);
	check_firmware_result = check_firmware_rootfs;

	exec_upgfile_fail =  true;
	result = update_firmware_main(good_user_argc, good_user_argv);
	CU_ASSERT_EQUAL(result, false);
	exec_upgfile_fail =  false;
}

#define MAGIC_FILE_POINTER_VALUE (FILE *)0x35649700

static void test_set_bootbit(void)
{
	bool result;
	bool set_bootbit(void);

	cunit_fopen_return_value = NULL;
	result = set_bootbit();
	CU_ASSERT_EQUAL(result, false);
	cunit_fopen_return_value = MAGIC_FILE_POINTER_VALUE;

	getline_fail =  true;
	result = set_bootbit();
	CU_ASSERT_EQUAL(result, false);
	getline_fail =  false;
}



int main(void)
{
    unsigned int number_of_failures;

    CU_pSuite pSuite = NULL;

	/* initialize the CUnit test registry */
	if ( CUE_SUCCESS != CU_initialize_registry() )
		return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite( "firmware_update_test_suite", init_suite, clean_suite );
	if ( NULL == pSuite ) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	   /* add the tests to the suite */
	   if ( (CU_add_test(pSuite, "test_verify_firmware_file", test_verify_firmware_file) == NULL) ||
			(CU_add_test(pSuite, "update_firmware_main", test_update_firmware_main) == NULL) ||
			(CU_add_test(pSuite, "test_set_bootbit", test_set_bootbit) == NULL))
	   {
	      CU_cleanup_registry();
	      return CU_get_error();
	   }

	#if USE_BASIC_STYLE
	   // Run all tests using the basic interface
	   CU_basic_set_mode(CU_BRM_VERBOSE);
	   CU_basic_run_tests();
	   printf("\n");
	   CU_basic_show_failures(CU_get_failure_list());
	   printf("\n\n");

	#elif USE_AUTOMATED_STYLE
	   // Run all tests using the automated interface
	   CU_automated_run_tests();
	   CU_list_tests_to_file();

	   // Run all tests using the console interface
	   CU_console_run_tests();
	#endif

       number_of_failures = CU_get_number_of_failures();

       /* Clean up registry */
	   CU_cleanup_registry();

       // Ensure we return an error if any tests failed
       if (number_of_failures)
           return -1;

       /* What does CU_get_error() actually give us? It seems to return 0 here even after some
        * tests fail. We want tests to halt if there is ever an error, which is why we have the
        * test for number_of_failures just above.
        */
       return CU_get_error();
}

/* stubs */
bool mocked_tlr_printf(tlr_response_format_t const type, char const * const format, ...)
{
    return true;
}

bool action_parse_args(int argc, char * const argv[], tlr_response_format_t * const format, char const ** const user)
{
	if (action_parse_args_fail)
		return false;
	else
		return true;
}

int check_firmware(const char* filepath, const int flags)
{
    int result;

    switch (check_firmware_result)
    {
        case check_firmware_crc_error:
            result = 0;
            break;

        case check_firmware_open_error:
            result = -1;
            break;

        case check_firmware_bootloader:
            result = 1;
            break;

        case check_firmware_rootfs:
            result = 2;
            break;

        default:
            result = -2;
            break;
    }

    return result;
}

int asprintf(char **strp, const char *fmt, ...)
{
	if (asprintf_fail)
		return -1;
	else
		return 0;
}

int exec_upgfile(const char *filename, const int flags)
{
	if (exec_upgfile_fail)
		return false;
	else
		return true;
}
ssize_t getline(char **a, size_t *b, FILE *c)
{
	if (getline_fail)
		return 0;
	else
		return 5;
}

FILE * cunit_fopen(const char * __restrict __filename, const char * __restrict __modes)
{
	strncpy(actual_filename, __filename, sizeof actual_filename);
	actual_filename[sizeof actual_filename - 1] = '\0';
	return cunit_fopen_return_value;
}

int cunit_fclose(FILE * __stream)
{
	cunit_fclose_fp = __stream;
	(void)__stream;
	return 0;
}




