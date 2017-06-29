
#define     USE_BASIC_STYLE     1
#define     USE_AUTOMATED_STYLE     0
#define     USE_CONSOLE_STYLE     0

#if USE_BASIC_STYLE
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#elif USE_AUTOMATED_STYLE
#include "CUnit/Automated.h"
#else
#include "CUnit/Console.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <json.h>

#include "config_table.h"
#include "config_mgr.h"
#include "strlcpy.h"

/* Test Suite setup and cleanup functions: */
int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

void status_test_set_show_handle(tlr_handle_t handle);

static tlr_handle_t status_handle;

static void test_status_open(void)
{
    tlr_handle_t show_handle = NULL;

    status_test_set_show_handle(show_handle);
    status_handle = tlr_status_open(config_state_eth, 1);
    CU_ASSERT_PTR_NULL(status_handle);

    show_handle = malloc(sizeof(FILE));
    status_test_set_show_handle(show_handle);

    status_handle = tlr_status_open(config_state_eth, 1);
    CU_ASSERT_PTR_NOT_NULL(status_handle);
    free((void *)show_handle);
}

static void test_status_read(void)
{
    char buffer[24];
    size_t invalid_element = 100;
    config_status_t status = tlr_status_read(status_handle, invalid_element, buffer, sizeof buffer);

    CU_ASSERT_EQUAL(status, config_status_element_not_found);

    status = tlr_status_read(status_handle, config_state_eth_mac_address, buffer, sizeof buffer);
    CU_ASSERT_EQUAL(status, config_status_success);
}

static void test_status_get_string_ptr(void)
{
    char const * buffer;
    size_t invalid_element = 100;
    config_status_t status = tlr_status_get_string_ptr(status_handle, invalid_element, &buffer);

    CU_ASSERT_EQUAL(status, config_status_element_not_found);

    status = tlr_status_get_string_ptr(status_handle, config_state_eth_mac_address, &buffer);
    CU_ASSERT_EQUAL(status, config_status_success);
}

static void test_status_close(void)
{
    tlr_status_close(status_handle);
}

/************* Test Runner Code goes here **************/

int main(void)
{
    CU_pSuite pSuite = NULL;
    unsigned int number_of_failures;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("status_test_suite", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ( CU_add_test(pSuite, "test_status_open", test_status_open) == NULL
        || CU_add_test(pSuite, "test_status_read", test_status_read) == NULL
        || CU_add_test(pSuite, "test_status_get_string_ptr", test_status_get_string_ptr) == NULL
        || CU_add_test(pSuite, "test_status_close", test_status_close) == NULL)
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

    /* Clean up registry and return */
    number_of_failures = CU_get_number_of_failures();
    CU_cleanup_registry();

	// Ensure we return an error if any tests failed.
    if (number_of_failures)
    	return -1;

    return CU_get_error();
}
