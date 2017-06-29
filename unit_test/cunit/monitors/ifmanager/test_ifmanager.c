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

#include "daemonize.h"
#include "ifmwan.h"
#include "event_manager.h"
#include "messages.h"
#include "plain_socket.h"
#include "timer.h"
#include "ifmanager.h"
#include "test_ifmanager.h"

#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Test Suite setup and cleanup functions: */

static int init_suite(void) { return 0; }
static int clean_suite(void) { return 0; }

static bool bad_timer_context = false;
static bool bad_message_thread = false;
static bool bad_plain_socket = false;

/* configuration functions */
static void set_bad_timer_context(bool value)
{
    bad_timer_context = value;
}

static void set_bad_message_thread(bool value)
{
    bad_message_thread = value;
}

struct plain_socket_context_st
{
    char const * socket_name;
    bool use_abstract_namespace;
    pthread_t thread_id;
    plain_socket_callback_fn callback;
    void const * user_context;

};
static void set_bad_plain_socket(bool value)
{
    bad_plain_socket = value;
}

void daemon_confirm_is_running(void)
{
}

int wan_init(void)
{
    return 0;
}

int wan_handle_config_change(ifm_update_info_t const * const info)
{
    return 0;
}

int wan_handle_gateway_assignment(dhcp_assigned_gateway_st const * const gateway_info)
{
    return 0;
}
/************* Test case functions ****************/

static void test_if_manager_start(void)
{
    int result;

    /* test successful result */
    result = if_manager_start();
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);

    set_bad_plain_socket(true);
    result = if_manager_start();
    CU_ASSERT_EQUAL(result, EXIT_FAILURE);

    set_bad_timer_context(true);
    result = if_manager_start();
    CU_ASSERT_EQUAL(result, EXIT_FAILURE);

    set_bad_message_thread(true);
    result = if_manager_start();
    CU_ASSERT_EQUAL(result, EXIT_FAILURE);

    set_bad_timer_context(false);
    result = if_manager_start();
    CU_ASSERT_EQUAL(result, EXIT_FAILURE);

    set_bad_message_thread(false);
}

/************* Test Runner Code goes here **************/

int add_ifmanager_tests(void)
{
    CU_pSuite pSuite = NULL;

    /* add a suite to the registry */
    pSuite = CU_add_suite( "ifmanager_test_suite", init_suite, clean_suite );
    if (pSuite == NULL)
        goto error;

    /* add the tests to the suite */
    if (CU_add_test(pSuite, "test_if_manager_start", test_if_manager_start) == NULL)
        goto error;

    /* newly added function tests goes here */
    goto done;

error:
    CU_cleanup_registry();

done:
    return CU_get_error();

}

/* stubs */
timer_context_st * open_timer_context(void)
{
    static int timer_context;

    return (void *) (bad_timer_context ? NULL : &timer_context);
}

message_thread_st * open_messaging_context(message_destination_t const destination, message_callback_fn const message_callback)
{
    static int message_thread;
    (void)destination;
    (void)message_callback;

    return (void *)(bad_message_thread ? NULL : &message_thread);
}

plain_socket_context_st * open_plain_socket_context(char const * const socket_name, bool use_abstract_namespace, plain_socket_callback_fn callback, void const * const user_context)
{
    static int plain_socket;
    (void)use_abstract_namespace;
    (void)socket_name;
    (void)callback;
    (void)user_context;

    return (void *)(bad_plain_socket ? NULL : &plain_socket);
}

int create_dns_link(void)
{
    return -1;
}
