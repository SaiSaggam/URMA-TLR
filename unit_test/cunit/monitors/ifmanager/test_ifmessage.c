#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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
#include "tlr_print.h"
#include <stdlib.h>
#include <syslog.h>
#include "ifmanager.h"
#include <interfaces.h>
#include <parsing.h>
#include <stdarg.h>

/* Test Suite setup and cleanup functions: */

static int init_suite(void) { return 0; }
static int clean_suite(void) { return 0; }

static bool event_send_success = true;
static bool send_pong_message_called;
static unsigned int actual_message_id;
static message_destination_t actual_message_destination;

/************* Test case functions ****************/
static void test_ifm_message_callback(void)
{
    int result;
    int unused_fd = 1;
    static interface_info_st if_info = {.instance = 1, .interface = eth_interface};
    interface_info_st * pifinfo = &if_info;
    static update_info_st config_info = {.instance = 1, .config_group = config_failover};
    update_info_st * pinfo = &config_info;
    interface_info_st * emptyinfo = NULL;

    /* test successful result */
    result = ifm_message_callback(unused_fd, 1, connected_message, (void **)&pifinfo, sizeof if_info);
    CU_ASSERT_EQUAL(result, 0);

    result = ifm_message_callback(unused_fd, 1, disconnected_message, (void **)&pifinfo, sizeof if_info);
    CU_ASSERT_EQUAL(result, 0);

    result = ifm_message_callback(unused_fd, 1, update_message, (void **)&pinfo, sizeof config_info);
    CU_ASSERT_EQUAL(result, 0);

    event_send_success = false;
    result = ifm_message_callback(unused_fd, 1, update_message, (void **)&pinfo, sizeof config_info);
    CU_ASSERT_EQUAL(result, -1);
    event_send_success = true;

    result = ifm_message_callback(unused_fd, 1, connect_message, (void **)&emptyinfo, sizeof if_info);
    CU_ASSERT_EQUAL(result, -1);

    result = ifm_message_callback(unused_fd, 1, disconnected_message, NULL, sizeof if_info);
    CU_ASSERT_EQUAL(result, -1);

    result = ifm_message_callback(unused_fd, 1, connected_message, (void **)&pifinfo, 0);
    CU_ASSERT_EQUAL(result, -1);
}

static void test_ifm_message_callback_handles_ping(void)
{
    int result;
    int unused_fd = 1;
    ping_info_st ping_info;
    ping_info_st * ping_info_ptr = &ping_info;
    unsigned int expected_message_id;
    message_destination_t expected_message_destination;

    /* setup */
    expected_message_id = 1234;
    expected_message_destination = ethernet_monitor;
    actual_message_id = 2345;
    actual_message_destination = vpn_monitor;
    ping_info.message_id = expected_message_id;
    ping_info.pong_destination = expected_message_destination;
    send_pong_message_called = false;
    /* perform test */
    result = ifm_message_callback(unused_fd, 1, ping_message, (void **)&ping_info_ptr, sizeof *ping_info_ptr);
    /* check results */
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_TRUE(send_pong_message_called);
    CU_ASSERT_EQUAL(actual_message_id, expected_message_id);
    CU_ASSERT_EQUAL(actual_message_destination, expected_message_destination);
}

/************* Test Runner Code goes here **************/

int add_ifmessage_tests(void)
{
    CU_pSuite pSuite = NULL;

    /* add a suite to the registry */
    pSuite = CU_add_suite( "ifmessage_test_suite", init_suite, clean_suite );
    if (pSuite == NULL)
        goto error;

    /* add the tests to the suite */
    if (0
        || CU_add_test(pSuite, "test_ifm_message_callback", test_ifm_message_callback) == NULL
        || CU_add_test(pSuite, "test_ifm_message_callback_handles_ping", test_ifm_message_callback_handles_ping) == NULL
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
char const * interface_get_name_from_type(interfaces_t type)
{
    char * name = NULL;

    switch(type)
    {
        case dsl_interface:
            name = "dsl";
            break;

        case eth_interface:
            name = "eth";
            break;

        case bridge_interface:
            name = "lan";
            break;

        case cellular_interface:
            name = "cellular";
            break;

        default:
            break;
    }

    return name;
}

interfaces_t interface_get_type_from_config_group(config_group_t config_group)
{
    interfaces_t type;

    switch(config_group)
    {
        case config_dsl:
            type = dsl_interface;
            break;

        case config_eth:
            type = eth_interface;
            break;

        case config_lan:
            type = bridge_interface;
            break;

        case config_cellular:
            type = cellular_interface;
            break;

        default:
            type = invalid_interface;
            break;
    }

    return type;
}

int send_event_to_event_manager(event_manager_st * const event_manager, void * const event)
{
    (void) event_manager;
    (void) event;
    int result = event_send_success ? 0 : -1;

    return result;
}

int send_pong_message(message_destination_t const destination, unsigned int message_id)
{
    send_pong_message_called = true;
    actual_message_destination = destination;
    actual_message_id = message_id;
    return 0;  // @TODO
}

char const * get_config_group_name_from_id(config_group_t const group_id)
{
    char * name;

    switch(group_id)
    {
        case config_dsl:
            name = "dsl";
            break;

        case config_eth:
            name = "eth";
            break;

        case config_cellular:
            name = "cellular";
            break;

        case config_failover:
            name = "failover";
            break;

        default:
            break;
    }

    return name;
}
