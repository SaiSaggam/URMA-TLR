#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "messages.h"
#include "parsing.h"
#include "config_mgr.h"
#include "interfaces.h"
#include "ifmanager.h"
#include "failover_config.h"

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

#include <event_manager.h>
#include <messages.h>
#include <timer.h>
#include "test_ifmanager.h"
#include <stdlib.h>
#include <config_def.h>

/* Test Suite setup and cleanup functions: */

static int init_suite(void) { return 0; }
static int clean_suite(void) { return 0; }

// Flags used to indicate failure cases.
bool tlr_get_element_value_bad = false;
bool tlr_config_string_to_value_bad = false;
static bool send_message_fail = false;
static ifm_config_state_t failover_state_value = ifm_config_state_off;
static ifm_config_state_t config_state_value = ifm_config_state_off;
static int failover_from_value = config_failover_from_dsl;
static int failover_to_value = config_failover_to_cellular1;
static size_t failover_timeout_value = 30;

/************* Test case functions ****************/

void test_set_interface_state(ifm_config_state_t state)
{
    config_state_value = state;
}

void test_set_failover_state(ifm_config_state_t state)
{
    config_state_value = state;
}

static failover_config_status_t failover_get_config_result;

failover_config_status_t failover_get_config(size_t const instance, failover_config_t * const info)
{
    return failover_get_config_result;
}

static void test_failover_handle_config_change_valid_instance(void)
{
    int result;
    ifm_update_info_t const valid_info = {.instance = 1, .config_group = config_failover, .name = "failover"};
    ifm_update_info_t const invalid_info = {.instance = 11, .config_group = config_failover, .name = "failover"};
    failover_info_t failover;

    /* check valid call */
    /* setup */
    memset(&failover, 0, sizeof failover);
    failover_set_number_of_contexts(1);
    failover_set_contexts(&failover);
    failover_get_config_result = failover_config_status_unchanged;

    /* perform test*/
    result = failover_handle_config_change(&valid_info);
    /* check result */
    CU_ASSERT_EQUAL(result, 0);

    /* check for error when no failover contexts are defined */
    /* setup */
    memset(&failover, 0, sizeof failover);
    failover_set_number_of_contexts(1);
    failover_set_contexts(&failover);
    failover_get_config_result = failover_config_status_unchanged;

    /* perform test */
    result = failover_handle_config_change(&invalid_info);

    /* check result */
    CU_ASSERT_EQUAL(result, -1);
}

static void test_failover_handle_config_change_invalid_instance(void)
{
    int result;
    ifm_update_info_t const invalid_info = {.instance = 2, .config_group = config_failover, .name = "failover"};
    failover_info_t failover;

    /* check for error when no failover contexts are defined */
    /* setup */
    memset(&failover, 0, sizeof failover);
    failover_set_number_of_contexts(1);
    failover_set_contexts(&failover);
    failover_get_config_result = failover_config_status_unchanged;

    /* perform test */
    result = failover_handle_config_change(&invalid_info);

    /* check result */
    CU_ASSERT_EQUAL(result, -1);
}

static void test_failover_handle_config_change_no_contexts(void)
{
    int result;
    ifm_update_info_t const valid_info = {.instance = 1, .config_group = config_failover, .name = "failover"};

    /* setup */
    failover_set_number_of_contexts(0);
    failover_set_contexts(NULL);

    /* perform test*/
    result = failover_handle_config_change(&valid_info);
    /* check result */
    CU_ASSERT_EQUAL(result, -1);

}

static void test_failover_handle_config_change_config_error_results_in_error(void)
{
    int result;
    ifm_update_info_t const valid_info = {.instance = 1, .config_group = config_failover, .name = "failover"};
    failover_info_t failover;

    /* check that a config error results in an error */
    /* setup */
    memset(&failover, 0, sizeof failover);
    failover.instance = 1;
    failover_set_number_of_contexts(1);
    failover_set_contexts(&failover);
    failover_get_config_result = failover_config_status_error;

    /* perform test*/
    result = failover_handle_config_change(&valid_info);
    /* check result */
    CU_ASSERT_EQUAL(result, -1);

}

/************* Test Runner Code goes here **************/

int add_failover_tests(void)
{
    CU_pSuite pSuite = NULL;

    /* add a suite to the registry */
    pSuite = CU_add_suite( "failover_test_suite", init_suite, clean_suite );
    if (pSuite == NULL)
        goto error;

    /* add the tests to the suite */
    if (0
        || CU_add_test(pSuite,
                       "test_failover_handle_config_change_valid_instance",
                       test_failover_handle_config_change_valid_instance) == NULL
        || CU_add_test(pSuite,
                       "test_failover_handle_config_change_invalid_instance",
                       test_failover_handle_config_change_invalid_instance) == NULL
        || CU_add_test(pSuite,
                       "test_failover_handle_config_change_config_error_results_in_error",
                       test_failover_handle_config_change_config_error_results_in_error) == NULL
        || CU_add_test(pSuite,
                       "test_failover_handle_config_change_no_contexts",
                       test_failover_handle_config_change_no_contexts) == NULL
        )
        goto error;

    /* newly added function tests goes here */
    goto done;

error:
    CU_cleanup_registry();

done:
    return CU_get_error();

}

/* stubs */
config_status_t tlr_get_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      void * const value,
                                      size_t const bytes,
                                      config_mgr_element_type_t const type,
                                      config_mgr_access_level_t const access)
{
    config_status_t status = tlr_get_element_value_bad ? config_status_invalid_parameter : config_status_success;

    return status;
}

config_status_t tlr_config_string_to_value(size_t const group_id, size_t const element_id, char const * const buffer, void * const value)
{
    size_t * const ptr = value;
    config_status_t status = tlr_config_string_to_value_bad ? config_status_invalid_parameter : config_status_success;

    switch (element_id)
    {
        case config_failover_state:
            *ptr = (group_id == config_failover) ? failover_state_value : config_state_value;
            break;

        case config_failover_from:
            *ptr = failover_from_value;
            break;

        case config_failover_to:
            *ptr = failover_to_value;
            break;

        case config_failover_timeout:
            *ptr = failover_timeout_value;
            break;

        default:
            status = config_status_invalid_parameter;
            break;
    }

    return status;
}

int send_message(message_destination_t const destination, size_t instance, message_codes_t const message_code, char const * const str, char ** const response_buffer)
{
    if (send_message_fail)
        return -1;
    else
        return 0;
}

ssize_t tlr_get_group_id(char const * const group_name)
{
    ssize_t group_id = 0;

    return group_id;
}

ssize_t tlr_get_element_id(size_t const group_id, char const * const element_name)
{
    return 0; /* state ? */
}

int send_message_to_interface_monitor(interfaces_t interface_type, size_t instance, message_codes_t const message_code, char const * const str)
{
    return 0;
}

void const * tlr_get_table(config_mgr_table_type_t const type)
{
    return &config_mgr_group_table[config_mgr_set_group];
}

config_status_t tlr_get_group_instance(size_t const group_id,
                                       size_t const element_id,
                                       char const * const value,
                                       size_t const value_size,
                                       size_t * const instance)
{
    *instance = 1;

    return config_status_success;
}

int destroy_timer(timer_id_st * const timer_id)
{
    return 0;
}

timer_id_st * create_timer(timer_context_st * const context, void (*callback)(void * data), void * data)
{
    static char dummy_timer[32];

    return (timer_id_st *)&dummy_timer;
}

void start_timer(timer_id_st * const timer_id, unsigned int millisecs)
{
    return;
}

int stop_timer(timer_id_st * const timer_id)
{
    return 0;
}

event_manager_st * new_event_manager(void const * const user_context, size_t event_queue_size, event_handler event_handler_cb)
{
    static char dummy_event[32];

    return (event_manager_st *)&dummy_event;
}

void free_probe(probe_context_st * probe_context)
{
}

probe_context_st * send_probe(char const * const host, char const * const interface, uint32_t probe_id, size_t const size, void (*success_cb)(void * arg, uint32_t probe_id), void * const arg, size_t timeout_seconds)
{
    static int dummy_probe_context;

    return (probe_context_st *)&dummy_probe_context;
}

size_t tlr_get_config_value_max_length(config_mgr_table_type_t const type, size_t const config_id, size_t const element_id)
{
    return 100;
}

size_t tlr_get_max_instance(size_t const group_id, config_mgr_table_type_t const type)
{
    return 1;
}

int seconds_remaining(timer_id_st * const timer_id)
{
    return 0;
}

bool timer_is_running(timer_id_st * const timer_id)
{
    return false;
}
