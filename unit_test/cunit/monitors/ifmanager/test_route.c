#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "messages.h"
#include "parsing.h"
#include "config_mgr.h"
#include "interfaces.h"
#include "failover.h"
#include "route.h"
#include "ifmanager.h"

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
#include "route.h"

/* Test Suite setup and cleanup functions: */

static int init_suite(void) { return 0; }
static int clean_suite(void) { return 0; }

extern bool tlr_get_element_value_bad;
extern bool tlr_config_string_to_value_bad;

bool popen_fail;
bool system_command_fail;
bool fopen_fail;
bool tlr_config_open_fail;

/************* Test case functions ****************/

static void test_add_default_routes(void)
{
    int result;
    char ifname[64];
    extern int add_default_routes(char const * const ifname);

    strcpy(ifname, "eth1");
    tlr_get_element_value_bad = true;
    result = add_default_routes(ifname);
    CU_ASSERT_EQUAL(result, -1);
    tlr_get_element_value_bad = false;
}

static void test_add_static_routes(void)
{
    int result;
    char ifname[64];
    extern int add_static_routes(char const * const ifname);

    strcpy(ifname, "eth1");
    tlr_get_element_value_bad = true;
    result = add_static_routes(ifname);
    CU_ASSERT_EQUAL(result, -1);
    tlr_get_element_value_bad = false;

}

static void test_ifm_update_route(void)
{
    int result;
    extern int ifm_update_route(ifm_update_info_t const * const info);
    ifm_update_info_t info;

    info.instance = 1;
    info.name = NULL;
    info.config_group = 0;

    result = ifm_update_route(&info);
    CU_ASSERT_EQUAL(result, -1);
}

static void test_is_interface_up(void)
{
    int result;
    extern bool is_interface_up(char * const ifname);

    popen_fail = true;
    result = is_interface_up("eth0");
    popen_fail = false;
    CU_ASSERT_EQUAL(result, false);
}

static void test_route_remove(void)
{
    int result;
    extern int route_remove(char * const command);

    system_command_fail = true;
    result = route_remove("ip route add");
    CU_ASSERT_EQUAL(result, -1);
    system_command_fail = false;
}

#define VAL_LEN 128
static void test_read_default_route_configuration(void)
{
    extern int read_default_route_configuration(config_defroute_t * const parameters, size_t const instance, char values[][VAL_LEN]);
    config_defroute_t parameters[5];
    char values[5][VAL_LEN];
    int result;

    tlr_get_element_value_bad = true;
    result = read_default_route_configuration(parameters,1 , values);
    CU_ASSERT_EQUAL(result, -1);
    tlr_get_element_value_bad = false;
}

static void test_apply_default_route(void)
{
   extern int apply_default_route(int const instance);
   int result;

   fopen_fail = true;
   result = apply_default_route(1);
   CU_ASSERT_EQUAL(result, -1);
   fopen_fail = false;

   tlr_config_open_fail = true;
   result = apply_default_route(1);
   CU_ASSERT_EQUAL(result, -1);
   tlr_config_open_fail = false;

}

static void test_update_default_route(void)
{
    extern int update_default_route(int const instance);
    int result;

    fopen_fail = true;
    result = update_default_route(1);
    CU_ASSERT_EQUAL(result, -1);
    fopen_fail = false;

    tlr_config_open_fail = true;
    result = update_default_route(1);
    CU_ASSERT_EQUAL(result, -1);
    tlr_config_open_fail = false;
}

static void test_apply_static_route(void)
{
    extern int apply_static_route(int const instance);
    int result;

    fopen_fail = true;
    result = apply_static_route(1);
    CU_ASSERT_EQUAL(result, -1);
    fopen_fail = false;

    tlr_config_open_fail = true;
    result = apply_static_route(1);
    CU_ASSERT_EQUAL(result, -1);
    tlr_config_open_fail = false;
}


static void test_update_static_route(void)
{
    extern int update_static_route(int const instance);
    int result;

    fopen_fail = true;
    result = update_static_route(1);
    CU_ASSERT_EQUAL(result, -1);
    fopen_fail = false;

    tlr_config_open_fail = true;
    result = update_static_route(1);
    CU_ASSERT_EQUAL(result, -1);
    tlr_config_open_fail = false;
}

static void test_is_static_route_valid(void)
{
    extern bool is_static_route_valid(int const instance);
    bool result;

    tlr_config_open_fail = true;
    result = is_static_route_valid(1);
    CU_ASSERT_EQUAL(result, false);
    tlr_config_open_fail = false;
}

static void test_does_interface_match_default_route(void)
{
    extern bool does_interface_match_default_route(char const * const ifname, int const instance);
    bool result;

    tlr_get_element_value_bad = true;
    result = does_interface_match_default_route("eth1", 1);
    CU_ASSERT_EQUAL(result, false);
    tlr_get_element_value_bad = false;
}

static void test_does_interface_match_static_route(void)
{
    extern bool does_interface_match_static_route(char const * const ifname, int const instance);
    bool result;

    tlr_get_element_value_bad = true;
    result = does_interface_match_static_route("eth1", 1);
    CU_ASSERT_EQUAL(result, false);
    tlr_get_element_value_bad = false;
}

/************* Test Runner Code goes here **************/

int add_route_tests(void)
{
    CU_pSuite pSuite = NULL;

    /* add a suite to the registry */
    pSuite = CU_add_suite( "route_test_suite", init_suite, clean_suite );
    if (pSuite == NULL)
        goto error;

    /* add the tests to the suite */
    if ((CU_add_test(pSuite, "test_add_default_routes", test_add_default_routes) == NULL) ||
        (CU_add_test(pSuite, "test_add_static_routes", test_add_static_routes) == NULL) ||
        (CU_add_test(pSuite, "test_is_interface_up", test_is_interface_up) == NULL) ||
        (CU_add_test(pSuite, "test_route_remove", test_route_remove) == NULL) ||
        (CU_add_test(pSuite, "test_read_default_route_configuration", test_read_default_route_configuration) == NULL) ||
        (CU_add_test(pSuite, "test_apply_default_route", test_apply_default_route) == NULL) ||
        (CU_add_test(pSuite, "test_update_default_route", test_update_default_route) == NULL) ||
        (CU_add_test(pSuite, "test_apply_static_route", test_apply_static_route) == NULL) ||
        (CU_add_test(pSuite, "test_update_static_route", test_update_static_route) == NULL) ||
        (CU_add_test(pSuite, "test_is_static_route_valid", test_is_static_route_valid) == NULL) ||
        (CU_add_test(pSuite, "test_does_interface_match_default_route", test_does_interface_match_default_route) == NULL) ||
        (CU_add_test(pSuite, "test_does_interface_match_static_route", test_does_interface_match_static_route) == NULL) ||
        (CU_add_test(pSuite, "test_ifm_update_route", test_ifm_update_route) == NULL))
        goto error;

    /* newly added function tests goes here */
    goto done;

error:
    CU_cleanup_registry();

done:
    return CU_get_error();

}

tlr_handle_t tlr_config_open(size_t const group_id, size_t const instance, config_mgr_access_level_t const access)
{
    static tlr_handle_t handle;

    return (tlr_config_open_fail) ? NULL : &handle;
}

config_status_t tlr_config_read(tlr_handle_t const handle, size_t const element_id, char * const data, size_t const bytes)
{
    config_defroute_t parameter = (config_defroute_t) element_id;
    switch (parameter)
    {
        case config_defroute_gateway:
            strcpy(data, "defroute_gateway");
            break;
        case config_defroute_metric:
            strcpy(data, "defroute_metric");
            break;
        case config_defroute_interface:
            strcpy(data, "defroute_interface");
            break;
        default:
            goto ERROR;
    }
    return config_status_success;
ERROR:
    return config_status_invalid_parameter;
}

void tlr_config_close(tlr_handle_t const handle)
{
    return;
}

void tlr_event_log(tlr_log_t const priority, char const * const process_name, char const * const format, ...)
{
    return;
}

FILE *fopen(const char  * __restrict fn, const char  * __restrict m)
{
    static FILE fd;
    return (fopen_fail) ? NULL : &fd;
}

int fclose(FILE *fp)
{
    return 0;
}

FILE *popen(const char *command, const char *type)
{
    static FILE fd;
    return (popen_fail) ? NULL : &fd;
}

int pclose(FILE *fp)
{
    return 0;
}

int system_command (char const * const format, ...)
{
    return (system_command_fail) ? -1 : 0;
}








