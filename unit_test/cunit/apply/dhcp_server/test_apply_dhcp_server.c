#define USE_BASIC_STYLE     1
#define USE_AUTOMATED_STYLE 0
#define USE_CONSOLE_STYLE   0

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
#include "config_mgr.h"
#include "config_def.h"
#include <stdlib.h>

extern int apply_dhcp_server(size_t const instance);
static int system_command_return_value;
static config_mgr_group_t group;
static bool test_null_group_prt;
static unsigned int state;
static char ip_address_start[] = "192.168.155.100";
static char ip_address_end[] = "192.168.155.255";
static unsigned int    * p_state = &state;
static char * p_ip_address_start = ip_address_start;
static char * p_ip_address_end = ip_address_end;
static config_status_t tlr_get_element_alloc_value_return_value_state;
static config_status_t tlr_get_element_alloc_value_return_value_ipstart;
static config_status_t tlr_get_element_alloc_value_return_value_ipend;
static int system_command_called = 0;

/********************
    Test Suite Setup
********************/

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/********************
    Test Case Functions
********************/
static void test_null_group_pointer()
{
    int status;
    int instance = 1;

    test_null_group_prt = true;
    status = apply_dhcp_server(instance);
    CU_ASSERT_EQUAL(status, EXIT_FAILURE);
}

static void test_get_state_fail()
{
    int status;
    int instance = 1;

    test_null_group_prt = false;
    //system_command_return_value = -1;
    tlr_get_element_alloc_value_return_value_state =  config_status_success + 1;
    status = apply_dhcp_server(instance);
    CU_ASSERT_EQUAL(status, EXIT_FAILURE);
}

static void test_get_ip_start_fail()
{
    int status;
    int instance = 1;

    test_null_group_prt = false;
    tlr_get_element_alloc_value_return_value_state =  config_status_success;
    tlr_get_element_alloc_value_return_value_ipstart =  config_status_success+1;
    status = apply_dhcp_server(instance);
    CU_ASSERT_EQUAL(status, EXIT_FAILURE);
}

static void test_get_ip_end_fail()
{
    int status;
    int instance = 1;

    test_null_group_prt = false;
    tlr_get_element_alloc_value_return_value_state =  config_status_success;
    tlr_get_element_alloc_value_return_value_ipstart =  config_status_success;
    tlr_get_element_alloc_value_return_value_ipend =  config_status_success+1;
    status = apply_dhcp_server(instance);
    CU_ASSERT_EQUAL(status, EXIT_FAILURE);
}

static void test_state_is_not_set_call_system_command()
{
    int status;
    int instance = 1;

    system_command_called = 0;
    test_null_group_prt = false;
    state = 0;
    tlr_get_element_alloc_value_return_value_state =  config_status_success;
    tlr_get_element_alloc_value_return_value_ipstart =  config_status_success;
    tlr_get_element_alloc_value_return_value_ipend =  config_status_success;
    status = apply_dhcp_server(instance);
    CU_ASSERT_EQUAL(system_command_called, 1);
    CU_ASSERT_EQUAL(status, EXIT_SUCCESS);
}

static void test_state_is_set_with_startip_endip_call_system_command()
{
    int status;
    int instance = 1;

    system_command_called = 0;
    test_null_group_prt = false;
    state = 1;
    tlr_get_element_alloc_value_return_value_state =  config_status_success;
    tlr_get_element_alloc_value_return_value_ipstart =  config_status_success;
    tlr_get_element_alloc_value_return_value_ipend =  config_status_success;
    status = apply_dhcp_server(instance);
    CU_ASSERT_EQUAL(system_command_called, 1);
    CU_ASSERT_EQUAL(status, EXIT_SUCCESS);
}

static void test_state_is_set_with_endip_not_call_system_command()
{
    int status;
    int instance = 1;

    system_command_called = 0;
    test_null_group_prt = false;
    state = 1;
    p_ip_address_start = "";
    p_ip_address_end = ip_address_end;
    tlr_get_element_alloc_value_return_value_state =  config_status_success;
    tlr_get_element_alloc_value_return_value_ipstart =  config_status_success;
    tlr_get_element_alloc_value_return_value_ipend =  config_status_success;
    status = apply_dhcp_server(instance);
    CU_ASSERT_EQUAL(system_command_called, 0);
    CU_ASSERT_EQUAL(status, EXIT_SUCCESS);
}

static void test_state_is_set_with_startip_not_call_system_command()
{
    int status;
    int instance = 1;

    system_command_called = 0;
    test_null_group_prt = false;
    state = 1;
    p_ip_address_start = ip_address_start;
    p_ip_address_end = "";
    tlr_get_element_alloc_value_return_value_state =  config_status_success;
    tlr_get_element_alloc_value_return_value_ipstart =  config_status_success;
    tlr_get_element_alloc_value_return_value_ipend =  config_status_success;
    status = apply_dhcp_server(instance);
    CU_ASSERT_EQUAL(system_command_called, 0);
    CU_ASSERT_EQUAL(status, EXIT_SUCCESS);
}

static void test_state_is_set_with_not_call_system_command()
{
    int status;
    int instance = 1;

    system_command_called = 0;
    test_null_group_prt = false;
    state = 1;
    p_ip_address_start = "";
    p_ip_address_end = "";
    tlr_get_element_alloc_value_return_value_state =  config_status_success;
    tlr_get_element_alloc_value_return_value_ipstart =  config_status_success;
    tlr_get_element_alloc_value_return_value_ipend =  config_status_success;
    status = apply_dhcp_server(instance);
    CU_ASSERT_EQUAL(system_command_called, 0);
    CU_ASSERT_EQUAL(status, EXIT_SUCCESS);
}

static void test_state_is_set_with_startip_with_endif_again_call_system_command()
{
    int status;
    int instance = 1;

    system_command_called = 0;
    test_null_group_prt = false;
    state = 1;
    p_ip_address_start = ip_address_start;
    p_ip_address_end = ip_address_end;
    tlr_get_element_alloc_value_return_value_state =  config_status_success;
    tlr_get_element_alloc_value_return_value_ipstart =  config_status_success;
    tlr_get_element_alloc_value_return_value_ipend =  config_status_success;
    status = apply_dhcp_server(instance);
    CU_ASSERT_EQUAL(system_command_called, 1);
    CU_ASSERT_EQUAL(status, EXIT_SUCCESS);
}

void test_apply_dhcp_server()
{
    test_null_group_pointer();

    test_get_state_fail();
    test_get_ip_start_fail();
    test_get_ip_end_fail();

    test_state_is_not_set_call_system_command();

    test_state_is_set_with_startip_endip_call_system_command();
    test_state_is_set_with_endip_not_call_system_command();
    test_state_is_set_with_startip_not_call_system_command();
    test_state_is_set_with_not_call_system_command();
    test_state_is_set_with_startip_with_endif_again_call_system_command();
}

/********************
    Test Runner
********************/

int main() {
    unsigned int number_of_failures;

    //  Initialize the CUnit test registry
    if ( CUE_SUCCESS != CU_initialize_registry() )
        return CU_get_error();

    //  Add a suite to the registry
    CU_pSuite pSuite1 = CU_add_suite( "test_apply_dhcp_server_suite", init_suite, clean_suite );
    if ( pSuite1 == NULL ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add the tests to the suite
    if (
        (CU_add_test(pSuite1, "test_apply_dhcp_server", test_apply_dhcp_server)==NULL)
        )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    #if USE_BASIC_STYLE
        //  Run all tests using the basic interface
        CU_basic_set_mode(CU_BRM_SILENT);
        CU_basic_run_tests();
        CU_basic_show_failures(CU_get_failure_list());
    #elif USE_AUTOMATED_STYLE
        //  Run all tests using the automated interface
        CU_automated_run_tests();
        CU_list_tests_to_file();
    #else
        //  Run all tests using the console interface
        CU_console_run_tests();
    #endif

    //  Clean up registry and return
    number_of_failures = CU_get_number_of_failures();
    CU_cleanup_registry();

    //  Ensure we return an error if any tests failed
    if (number_of_failures)
        return -1;

    return CU_get_error();
}


/* stub functions */

/********************
    Function Signatures
********************/

config_status_t tlr_get_element_alloc_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      void ** value,
                                      config_mgr_element_type_t const type,
                                      config_mgr_access_level_t const access)
{
    switch (element_id)
    {
        case config_dhcp_server_state:
            * value = p_state;
            break;
        case config_dhcp_server_ip_address_start:
            *value = p_ip_address_start;
            break;
        case config_dhcp_server_ip_address_end:
            *value = p_ip_address_end;
            break;
        default:
            break;
    }

    switch (element_id)
    {
        case config_dhcp_server_state:
            return tlr_get_element_alloc_value_return_value_state;
            break;
        case config_dhcp_server_ip_address_start:
            return tlr_get_element_alloc_value_return_value_ipstart;
            break;
        case config_dhcp_server_ip_address_end:
            return tlr_get_element_alloc_value_return_value_ipend;
            break;
        default:
            return config_status_success;
            break;
    }
}


int system_command(char const * const format, ...)
{
    system_command_called++;
    return system_command_return_value;
}

config_mgr_group_t const * tlr_get_group_ptr(size_t const group_id, size_t const instance, config_mgr_group_type_t const table_index)
{
    if (test_null_group_prt)
        return NULL;
    else
        return &group;
}

void free(void *ptr)
{
}