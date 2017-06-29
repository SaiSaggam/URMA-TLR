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

#include "test_cc.h"
#include "connector_api.h"
#include "platform.h"
#include "cc_status.h"
#include "plain_socket.h"
#include "config_mgr.h"
#include "tlr_print.h"
#include "ip_helper.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

/* Test Suite setup and cleanup functions: */
static int init_suite(void) { return 0; }
static int clean_suite(void) { return 0; }
extern void cc_show_callback(int const fd, void * const user_context);

static char dummy_url[] = "tlr.unittest.com";
static char dummy_platform[] = "LR54ABC";

/************* Test case functions ****************/
static void test_cc_get_cloud_status(void)
{
    cloud_status_t * const status_ptr = cc_get_cloud_status();

    CU_ASSERT_PTR_NOT_NULL(status_ptr);
    CU_ASSERT_FALSE(status_ptr->connected);
    status_ptr->connected = true;
    strcpy(status_ptr->server_name, dummy_url);
}

static void test_cc_clear_cloud_status(void)
{
    cloud_status_t const * const status_ptr = cc_get_cloud_status();

    cc_clear_cloud_status();

    CU_ASSERT_FALSE(status_ptr->connected);
    CU_ASSERT_STRING_EQUAL(status_ptr->server_name, dummy_url);
}

static void verify_tcp_events(connector_tcp_status_t const event)
{
    connector_request_id_status_t const request = connector_request_id_status_tcp;
    connector_status_tcp_event_t data = {.status = event};
    connector_callback_status_t status;

    status = app_status_handler(request, &data);
    CU_ASSERT_EQUAL(status, connector_callback_continue);
}

static void test_app_status_handler(void)
{
    verify_tcp_events(connector_tcp_communication_started);
    verify_tcp_events(connector_tcp_keepalive_missed);
    verify_tcp_events(connector_tcp_keepalive_restored);
}

/************* Test Runner Code goes here **************/
int add_cc_status_tests(void)
{
    CU_pSuite pSuite = NULL;

    /* add a suite to the registry */
    pSuite = CU_add_suite( "cc_status_test_suite", init_suite, clean_suite );
    if (pSuite == NULL) goto error;

    /* add the tests to the suite */
    if (CU_add_test(pSuite, "test_cc_get_cloud_status", test_cc_get_cloud_status) == NULL) goto error;
    if (CU_add_test(pSuite, "test_cc_clear_cloud_status", test_cc_clear_cloud_status) == NULL) goto error;
    if (CU_add_test(pSuite, "test_app_status_handler", test_app_status_handler) == NULL) goto error;

    /* newly added function tests goes here */
    goto done;

error:
    CU_cleanup_registry();

done:
    return CU_get_error();

}

/* ----------------------------------------------------------------------------  */
/* stub functions */
bool get_ifr(char const * const ifname, int const request, struct ifreq * const ifr)
{
    CU_ASSERT_STRING_EQUAL(ifname, "eth1");
    CU_ASSERT_PTR_NOT_NULL(ifr);

    return true;
}

config_status_t tlr_get_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      void * const value,
                                      size_t const bytes,
                                      config_mgr_element_type_t const type,
                                      config_mgr_access_level_t const access)
{
    CU_ASSERT_EQUAL(group_id, config_cloud);
    CU_ASSERT_EQUAL(instance, 1);
    CU_ASSERT_PTR_NOT_NULL(value);

    switch (element_id)
    {
        case config_cloud_server:
            CU_ASSERT(bytes > sizeof dummy_url);
            strcpy(value, dummy_url);
            break;

        default:
            break;
    }

    return config_status_success;
}

int get_tlr_platform_name(char const * * name_ptr)
{
    CU_ASSERT_PTR_NOT_NULL(name_ptr);
    *name_ptr = dummy_platform;
    return 0;
}
