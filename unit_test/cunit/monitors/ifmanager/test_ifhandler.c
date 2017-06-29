
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

#include "ifmwan.h"
#include "event_manager.h"
#include "messages.h"
#include "timer.h"
#include "test_ifhandler.h"
#include "tlr_print.h"
#include "ifmanager.h"
#include "interfaces.h"
#include "parsing.h"

#include <stdlib.h>
#include <syslog.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

extern int ifm_unit_test_result;

/* Test Suite setup and cleanup functions: */

static int init_suite(void) { return 0; }
static int clean_suite(void) { return 0; }

/************* Test case functions ****************/
typedef enum
{
    /* All cases that expect no error go below here */
    test_ifmh_event_first,
    test_ifmh_event_update_interface_from,
    test_ifmh_event_update_interface_to,
    test_ifmh_event_update_failover,
    test_ifmh_event_connected_good,
    test_ifmh_event_disconnected_good,
    test_ifmh_event_timeout_good,
    test_ifmh_event_show_good_socket,
    test_ifmh_event_show_good_socket_0,
    test_ifmh_event_last_good,
    /* All cases that expect an error result go below here */
    test_ifmh_event_connected_bad_instance,
    test_ifmh_event_connected_bad_interface,
    test_ifmh_event_disconnected_bad_instance,
    test_ifmh_event_disconnected_bad_interface,
    test_ifmh_event_update_bad_instance,
    test_ifmh_event_update_bad_group,
    test_ifmh_event_timeout_bad_instance,
    test_ifmh_event_timeout_bad_interface,
    test_ifmh_event_show_bad_socket,
    test_ifmh_event_last_bad
} test_ifm_handler_case_t;

/* stubs */
static int failover_timeout_timer_timeout_result;
int failover_timeout_timer_timeout(failover_info_t * const failover)
{
    return failover_timeout_timer_timeout_result;
}

int failover_probe_interval_timeout(failover_info_t * const failover)
{
    return 0;
}

int failover_probe_timeout(failover_info_t * const failover)
{
    return 0;
}

int failover_probe_success(probe_success_info_st * const success_info)
{
    return 0;
}

ifm_config_t * ifm_get_context(void)
{
    static ifm_config_t context;

    return &context;
}

void send_failover_status(ifm_config_t const * const context, int const socket_fd)
{
}

int wan_handle_config_change(ifm_update_info_t const * const info)
{
    return 0;
}

static ifm_if_config_t interface;
static ifm_if_config_t * ifm_get_interface_entry_return_value;

ifm_if_config_t * ifm_get_interface_entry(ifm_if_info_t const * const info)
{
    return ifm_get_interface_entry_return_value;
}

void failover_handler(ifm_if_info_t const * const iface)
{
}

bool failover_inhibit_routes(ifm_if_info_t const * const iface)
{
    return false;
}

int ifm_apply_routes(char const * const ifname)
{
    return 0;
}

int wan_handle_gateway_assignment(dhcp_assigned_gateway_st const * const gateway_info)
{
    return 0;
}

static int failover_handle_config_change_result;

int failover_handle_config_change(ifm_update_info_t const * const info)
{
    return failover_handle_config_change_result;
}

char const * const failover_get_interface_name(ifm_if_info_t const * const info)
{
    return NULL;
}

int ifm_handle_config_change(ifm_update_info_t const * const info)
{
    return 0;
}

int ifm_update_route(ifm_update_info_t const * const info)
{
    return 0;
}

int failover_switch_primary_up_timeout(failover_info_t * const failover)
{
    return 0;
}

/* functions used by tests */

static ifm_event_t * update_event(test_ifm_handler_case_t caseId, ifm_if_config_t * * interface_entry)
{
    ifm_event_t * event = malloc(sizeof *event);

    *interface_entry = &interface;  /* good tests return a valid pointer, bad tests return NULL */
    failover_handle_config_change_result = 0;
    failover_timeout_timer_timeout_result = 0;

    switch (caseId)
    {
        case test_ifmh_event_connected_good:
        {
            static ifm_if_info_t if_info = { .instance = 1, .interface = dsl_interface, .name = "dsl" };

            event->code = ifm_event_code_connected;
            event->info = &if_info;
            break;
        }

        case test_ifmh_event_disconnected_good:
        {
            static ifm_if_info_t if_info = { .instance = 1, .interface = cellular_interface, .name = "cellular" };

            event->code = ifm_event_code_disconnected;
            event->info = &if_info;
            break;
        }

        case test_ifmh_event_update_interface_from:
        {
            static ifm_update_info_t config_info = { .instance = 1, .config_group = config_dsl, .name = "dsl" };

            event->code = ifm_event_code_update;
            event->info = &config_info;
            break;
        }

        case test_ifmh_event_update_interface_to:
        {
            static ifm_update_info_t config_info = { .instance = 1, .config_group = config_cellular, .name = "cellular" };

            event->code = ifm_event_code_update;
            event->info = &config_info;
            break;
        }

        case test_ifmh_event_update_failover:
        {
            static ifm_update_info_t config_info = { .instance = 1, .config_group = config_failover, .name = "failover" };

            event->code = ifm_event_code_update;
            event->info = &config_info;
            break;
        }

        case test_ifmh_event_timeout_good:
        {
            static failover_info_t failover;
            static failover_event_info_t event_info = { .instance = 1, .failover = &failover };

            event->code = ifm_event_code_timer_timeout;
            event->info = &event_info;
            break;
        }

        case test_ifmh_event_show_good_socket:
        {
            static int socket = 5;

            event->code = ifm_event_code_show;
            event->info = &socket;
            break;
        }

        case test_ifmh_event_show_good_socket_0:
        {
            static int socket = 0;

            event->code = ifm_event_code_show;
            event->info = &socket;
            break;
        }

        case test_ifmh_event_connected_bad_instance:
        {
            static ifm_if_info_t if_info = { .instance = 2, .interface = dsl_interface, .name = "dsl" };

            *interface_entry = NULL;

            event->code = ifm_event_code_connected;
            event->info = &if_info;
            break;
        }

        case test_ifmh_event_connected_bad_interface:
        {
            static ifm_if_info_t if_info = { .instance = 1, .interface = invalid_interface, .name = "invalid" };

            *interface_entry = NULL;

            event->code = ifm_event_code_connected;
            event->info = &if_info;
            break;
        }

        case test_ifmh_event_disconnected_bad_instance:
        {
            static ifm_if_info_t if_info = { .instance = 3, .interface = cellular_interface, .name = "cellular" };

            *interface_entry = NULL;

            event->code = ifm_event_code_disconnected;
            event->info = &if_info;
            break;
        }

        case test_ifmh_event_disconnected_bad_interface:
        {
            static ifm_if_info_t if_info = { .instance = 1, .interface = invalid_interface, .name = "invalid" };

            *interface_entry = NULL;

            event->code = ifm_event_code_disconnected;
            event->info = &if_info;
            break;
        }

        case test_ifmh_event_update_bad_instance:
        {
            static ifm_update_info_t config_info = { .instance = 0, .config_group = config_failover, .name = "failover" };

            failover_handle_config_change_result = -1;

            event->code = ifm_event_code_update;
            event->info = &config_info;
            break;
        }

        case test_ifmh_event_update_bad_group:
        {
            static ifm_update_info_t config_info = { .instance = 1, .config_group = config_system, .name = "system" };

            event->code = ifm_event_code_update;
            event->info = &config_info;
            break;
        }

        case test_ifmh_event_timeout_bad_instance:
        {
            static failover_info_t failover;
            static failover_event_info_t event_info = { .instance = 0, .failover = &failover };

            failover_timeout_timer_timeout_result = -1;
            event->code = ifm_event_code_timer_timeout;
            event->info = &event_info;
            break;
        }

        case test_ifmh_event_timeout_bad_interface:
        {
            static failover_info_t failover;
            static failover_event_info_t event_info = { .instance = 1, .failover = &failover };

            failover_timeout_timer_timeout_result = -1;
            event->code = ifm_event_code_timer_timeout;
            event->info = &event_info;
            break;
        }

        case test_ifmh_event_show_bad_socket:
        {
            static int socket = -1;

            event->code = ifm_event_code_show;
            event->info = &socket;
            break;
        }

        case test_ifmh_event_first:
        case test_ifmh_event_last_good:
        case test_ifmh_event_last_bad:
            free(event);
            event = NULL;
            break;
    }

    return event;
}

/* tests */
static void test_ifm_event_handler(void)
{
    int expected_result = 0;
    ifm_event_t *event;
    test_ifm_handler_case_t case_id;

    for (case_id = test_ifmh_event_first; case_id < test_ifmh_event_last_bad; case_id++)
    {
        switch (case_id)
        {
            case test_ifmh_event_first:
                expected_result = 0;
                break;

            case test_ifmh_event_last_good:
                expected_result = -1;
                break;

            default:
                event = update_event(case_id, &ifm_get_interface_entry_return_value);
                if (event != NULL)  /* deal with cases where no event is returned */
                {
                    ifm_event_handler(NULL, event);
                    if (ifm_unit_test_result != expected_result)
                    {
                        fprintf(stderr, "ifmanager: failed test case %d\n", case_id);
                    }
                    CU_ASSERT_EQUAL(ifm_unit_test_result, expected_result);
                }
                break;
        }
    }
}

/************* Test Runner Code goes here **************/

int add_ifhandler_tests(void)
{
    CU_pSuite pSuite = NULL;

    /* add a suite to the registry */
    pSuite = CU_add_suite( "ifhandler_test_suite", init_suite, clean_suite );
    if (pSuite == NULL)
        goto error;

    /* add the tests to the suite */
    if (CU_add_test(pSuite, "test_ifm_event_handler", test_ifm_event_handler) == NULL)
        goto error;

    /* newly added function tests goes here */
    goto done;

error:
    CU_cleanup_registry();

done:
    return CU_get_error();

}

int create_dns_link(void)
{
    return -1;
}
