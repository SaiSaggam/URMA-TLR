
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
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <termios.h>

#include "apply_lan.h"


// Functions we will unit test.
bool lan_is_in_interface_list(config_lan_interfaces_t const bridgeint, config_mgr_enum_multi_value_t  const * const bridge_list);
bool is_interface_listed_in_lan_config(size_t const bridge_instance, config_mgr_enum_multi_value_t  const * const bridge_list);

/* Test Suite setup and cleanup functions: */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

static config_mgr_enum_multi_value_t * current_interfaces_element = NULL;
static unsigned int current_state_element = 0;

void simulate_interfaces_element_value(config_mgr_enum_multi_value_t  *pmulti)
{
    current_interfaces_element = pmulti;
}

void simulate_state_element_value(unsigned int state_value)
{
    current_state_element = state_value;
}

config_status_t tlr_get_element_alloc_value(size_t const group_id,
                                            size_t const instance,
                                            size_t const element_id,
                                            void ** value,
                                            config_mgr_element_type_t const type,
                                            config_mgr_access_level_t const access)
{
    config_status_t rc = config_status_element_not_found;

    if (value != NULL)
    {
        if (element_id == config_lan_interfaces && type == config_mgr_element_type_enum_multi)
        {
            *value = current_interfaces_element;
            current_interfaces_element = NULL;
            rc = config_status_success;
        }
        else if (element_id == config_lan_state && type == config_mgr_element_type_on_off)
        {
            unsigned int    * pint = malloc(sizeof*pint);

            if (pint != NULL)
            {
                *pint = current_state_element;
                *value = pint;
                rc = config_status_success;
            }
        }
    }
    return rc;
}


config_mgr_enum_multi_value_t * create_config_mgr_enum_multi_value_t(size_t cnt,  ...)
{
    int     i;
    va_list args;
    config_mgr_enum_multi_value_t     * pmulti;

    pmulti = (config_mgr_enum_multi_value_t *)malloc(sizeof *pmulti + (sizeof(int) * cnt));
    if (pmulti == NULL)
        goto done;
    pmulti->count = cnt;

    va_start(args, cnt);
    for (i = 0; i < cnt; i++)
    {
        pmulti->value[i] = va_arg(args, int);
    }
    va_end(args);
done:
    return pmulti;

}


/************* Test case functions ****************/
void test_bridge_isin_standard_interface_list(void)
{
    config_mgr_enum_multi_value_t   * test_case_list = create_config_mgr_enum_multi_value_t(3,    1, 2, 3);

    if (test_case_list != NULL)
    {
        CU_ASSERT_FALSE(lan_is_in_interface_list(4, test_case_list));
        CU_ASSERT_TRUE(lan_is_in_interface_list(3, test_case_list));
        CU_ASSERT_TRUE(lan_is_in_interface_list(2, test_case_list));
        CU_ASSERT_TRUE(lan_is_in_interface_list(1, test_case_list));
        CU_ASSERT_FALSE(lan_is_in_interface_list(0, test_case_list));
    }

    free(test_case_list);
}


void test_bridge_isin_small_interface_list(void)
{
    config_mgr_enum_multi_value_t   * test_case_list = create_config_mgr_enum_multi_value_t(1,    2);

    if (test_case_list != NULL)
    {
        CU_ASSERT_FALSE(lan_is_in_interface_list(4, test_case_list));
        CU_ASSERT_FALSE(lan_is_in_interface_list(3, test_case_list));
        CU_ASSERT_TRUE(lan_is_in_interface_list(2, test_case_list));
        CU_ASSERT_FALSE(lan_is_in_interface_list(1, test_case_list));
        CU_ASSERT_FALSE(lan_is_in_interface_list(0, test_case_list));
    }

    free(test_case_list);
}


void test_bridge_isin_null_interface_list(void)
{
    config_mgr_enum_multi_value_t   * test_case_list = create_config_mgr_enum_multi_value_t(0);

    if (test_case_list != NULL)
    {
        CU_ASSERT_FALSE(lan_is_in_interface_list(4, test_case_list));
        CU_ASSERT_FALSE(lan_is_in_interface_list(3, test_case_list));
        CU_ASSERT_FALSE(lan_is_in_interface_list(2, test_case_list));
        CU_ASSERT_FALSE(lan_is_in_interface_list(1, test_case_list));
        CU_ASSERT_FALSE(lan_is_in_interface_list(0, test_case_list));
    }

    free(test_case_list);
    test_case_list = NULL;

    CU_ASSERT_FALSE(lan_is_in_interface_list(4, test_case_list));
    CU_ASSERT_FALSE(lan_is_in_interface_list(3, test_case_list));
    CU_ASSERT_FALSE(lan_is_in_interface_list(2, test_case_list));
    CU_ASSERT_FALSE(lan_is_in_interface_list(1, test_case_list));
    CU_ASSERT_FALSE(lan_is_in_interface_list(0, test_case_list));
}



void test_listed_in_bridge_config_with_test_case(config_mgr_enum_multi_value_t   * test_case_list)
{
    simulate_interfaces_element_value(NULL);
    simulate_state_element_value(0);
    CU_ASSERT_FALSE(is_interface_listed_in_lan_config(1, test_case_list));

    {
        // TEST with a valid config now.
        config_mgr_enum_multi_value_t   * config_case_out_list = create_config_mgr_enum_multi_value_t(3,   4, 5, 6);
        config_mgr_enum_multi_value_t   * config_case_in_list = create_config_mgr_enum_multi_value_t(3,   4, 1, 6);

        simulate_interfaces_element_value(config_case_out_list);
        simulate_state_element_value(1);
        CU_ASSERT_FALSE(is_interface_listed_in_lan_config(1, test_case_list));

        simulate_interfaces_element_value(config_case_in_list);
        simulate_state_element_value(1);
        CU_ASSERT_TRUE(is_interface_listed_in_lan_config(1, test_case_list));

    }

    {
        config_mgr_enum_multi_value_t   * config_case_in_pos1_list = create_config_mgr_enum_multi_value_t(3,   1, 4, 6);
        config_mgr_enum_multi_value_t   * config_case_in_pos2_list = create_config_mgr_enum_multi_value_t(3,   4, 1, 6);
        config_mgr_enum_multi_value_t   * config_case_in_pos3_list = create_config_mgr_enum_multi_value_t(3,   4, 6, 1);
        config_mgr_enum_multi_value_t   * config_case_in_pos4_list = create_config_mgr_enum_multi_value_t(4,   5, 4, 6, 1);
        config_mgr_enum_multi_value_t   * config_case_in_pos5_list = create_config_mgr_enum_multi_value_t(5,   5, 4, 6, 5, 1);
        config_mgr_enum_multi_value_t   * config_case_in_NOpos5_list = create_config_mgr_enum_multi_value_t(5,   5, 4, 6, 8, 7);

        simulate_interfaces_element_value(config_case_in_pos1_list);
        simulate_state_element_value(1);
        CU_ASSERT_TRUE(is_interface_listed_in_lan_config(1, test_case_list));

        simulate_interfaces_element_value(config_case_in_pos2_list);
        CU_ASSERT_TRUE(is_interface_listed_in_lan_config(1, test_case_list));

        simulate_interfaces_element_value(config_case_in_pos3_list);
        CU_ASSERT_TRUE(is_interface_listed_in_lan_config(1, test_case_list));

        simulate_interfaces_element_value(config_case_in_pos4_list);
        CU_ASSERT_TRUE(is_interface_listed_in_lan_config(1, test_case_list));

        simulate_interfaces_element_value(config_case_in_pos5_list);
        CU_ASSERT_TRUE(is_interface_listed_in_lan_config(1, test_case_list));

        simulate_interfaces_element_value(config_case_in_NOpos5_list);
        CU_ASSERT_FALSE(is_interface_listed_in_lan_config(1, test_case_list));

    }
}

void test_listed_in_bridge_config(void)
{
    // Vary the position of the '1' to make sure all ok no matter what position the match is.
    config_mgr_enum_multi_value_t   * test_case1_list = create_config_mgr_enum_multi_value_t(3,   1, 2, 3);
    test_listed_in_bridge_config_with_test_case(test_case1_list);
    free(test_case1_list);

    config_mgr_enum_multi_value_t   * test_case2_list = create_config_mgr_enum_multi_value_t(3,   2, 1, 3);
    test_listed_in_bridge_config_with_test_case(test_case2_list);
    free(test_case2_list);

    config_mgr_enum_multi_value_t   * test_case3_list = create_config_mgr_enum_multi_value_t(3,   3, 2, 1);
    test_listed_in_bridge_config_with_test_case(test_case3_list);
    free(test_case3_list);
}


void test_listed_state_bridge_config(void)
{
    // Tests in which we vary 'state' to make sure this is taken into account
    config_mgr_enum_multi_value_t   * test_case_list = create_config_mgr_enum_multi_value_t(3,   1, 2, 3);
    config_mgr_enum_multi_value_t   * config_case_list = create_config_mgr_enum_multi_value_t(3,   4, 1, 6);

    simulate_interfaces_element_value(config_case_list);
    simulate_state_element_value(1);
    CU_ASSERT_TRUE(is_interface_listed_in_lan_config(1, test_case_list));

    // Right now do the same thing but this time with state off.
    simulate_state_element_value(0);
    CU_ASSERT_FALSE(is_interface_listed_in_lan_config(1, test_case_list));

}

/************* Test Runner Code goes here **************/

int main ( void )
{
    int result;
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if ( CUE_SUCCESS != CU_initialize_registry() )
    {
        result = CU_get_error();
        goto error;
    }

    /* add a suite to the registry */
    pSuite = CU_add_suite( "bridge_apply_test_suite", init_suite, clean_suite );
    if ( NULL == pSuite )
    {
        result = CU_get_error();
        goto error;
    }

    /* add the tests to the suite */
    if (
        (CU_add_test(pSuite, "bridge_interface standard list check", test_bridge_isin_standard_interface_list)==NULL) ||
        (CU_add_test(pSuite, "bridge_interface small list check", test_bridge_isin_standard_interface_list)==NULL) ||
        (CU_add_test(pSuite, "bridge_interface null list check", test_bridge_isin_null_interface_list)==NULL) ||
        (CU_add_test(pSuite, "test_listed_in_bridge_config", test_listed_in_bridge_config)==NULL) ||
        (CU_add_test(pSuite, "test_listed_state_bridge_config", test_listed_state_bridge_config)==NULL)
        )
    {
        result = CU_get_error();
        goto error;
    }

#if USE_BASIC_STYLE
    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
	if (CU_basic_run_tests() != CUE_SUCCESS)
	{
        result = CU_get_error();
        goto error;
	}
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

    result = CU_get_number_of_failures();

error:
	CU_cleanup_registry();
	return result;
}



/****************  Stubs   ***************************/




int send_message(int const destination, size_t instance, int const message_code, char const * const str, char ** const response_buffer)
{
    return 0;
}
bool tlr_printf(int const type, char const * const format, ...)
{
    return true;
}
void tlr_event_log(int const priority, char const * const process_name, char const * const format, ...)
{
}
char const * interface_get_name_from_type(int const type)
{
    return NULL;
}



void const * tlr_get_table(config_mgr_table_type_t const type)
{
    return NULL;
}
config_status_t tlr_set_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      char const * const value,
                                      config_mgr_access_level_t const access)
{
    return config_status_success;
}

config_status_t tlr_config_write_native(tlr_handle_t const handle, size_t const element_id, void const * const value)
{
    return config_status_success;
}
config_status_t tlr_get_last_config(size_t const group_id, size_t const instance, void * const ptr, size_t const size)
{
    return config_status_success;
}
config_status_t tlr_set_last_config(size_t const group_id, size_t const instance, void const * const ptr, size_t const size)
{
    return config_status_success;
}


tlr_handle_t tlr_config_open(size_t const group_id, size_t const instance, config_mgr_access_level_t const access)
{
    static unsigned int fd;

    return ((tlr_handle_t) &fd);
}
void tlr_config_close(tlr_handle_t const handle)
{
    return;
}


config_status_t tlr_config_string_to_value(size_t const group_id, size_t const element_id, char const * const buffer, void * const value)
{
    return config_status_no_memory;
}

config_mgr_element_enum_limit_t const * tlr_get_element_enum_limit(config_mgr_table_type_t const type, size_t const group_id, size_t const element_id)
{
    return NULL;
}

