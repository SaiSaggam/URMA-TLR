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

#include <Python.h>
#include "ddp_if.h"
#include "ddp_device.h"
#include "test_ddp.h"
#include "test_ddp_if.h"
#include <stdlib.h>

/* Test Suite setup and cleanup functions: */
static int init_suite(void) { return 0; }
static int clean_suite(void) { return 0; }

static void test_callback_fn(PyObject * args)
{
    CU_ASSERT_PTR_NOT_NULL(args);
}

/************* Test case functions ****************/
static void test_ddp_get_config_groups(void)
{
    bool result;

    memset(&ddp_test_info, 0, sizeof ddp_test_info);

    result =  = ddp_get_config_groups(test_callback_fn);
    CU_ASSERT_TRUE(result);
}

static void test_ddp_get_state_groups(void)
{
    bool empty = true;
    bool result;

    result = ddp_get_state_groups(test_callback_fn, empty);
    CU_ASSERT_TRUE(result);

    empty = false;
    result = ddp_get_state_groups(test_callback_fn, empty);
    CU_ASSERT_TRUE(result);

    memset(&ddp_test_info, 0, sizeof ddp_test_info);
}

static void test_ddp_create_thread(void)
{
    long tid;
    ddp_thread_t thread_info = {.group_name = "system"};

    ddp_test_info.group_name = thread_info.group_name;

    ddp_test_info.thread_id = -1;
    tid = ddp_create_thread(&thread_info);
    CU_ASSERT_EQUAL(tid, ddp_test_info.thread_id);

    ddp_test_info.thread_id = 222;
    tid = ddp_create_thread(&thread_info);
    CU_ASSERT_EQUAL(tid, ddp_test_info.thread_id);
}

static void test_ddp_delete_thread(void)
{
    bool result;

    ddp_test_info.thread_id = 222;
    result = ddp_delete_thread(ddp_test_info.thread_id);
    CU_ASSERT_TRUE(result);

    ddp_test_info.thread_id = -1;
    result = ddp_delete_thread(ddp_test_info.thread_id);
    CU_ASSERT_FALSE(result);
}

static void test_ddp_push_config_change(void)
{
    bool result;

    ddp_test_info.config.name = "lan";
    ddp_test_info.config.id = config_lan;
    ddp_test_info.config.instance = 1;

    result = ddp_push_config_change(group_name, instance);
    CU_ASSERT_TRUE(result);
}

/************* Test Runner Code goes here **************/

int add_ddp_if_tests(void)
{
    CU_pSuite pSuite = NULL;

    /* add a suite to the registry */
    pSuite = CU_add_suite( "ddp_if_test_suite", init_suite, clean_suite );
    if (pSuite == NULL) goto error;

    ddp_callback = test_callback_fn;

    /* add the tests to the suite */
    if (CU_add_test(pSuite, "test_ddp_get_config_groups", test_ddp_get_config_groups) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddp_get_state_groups", test_ddp_get_state_groups) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddp_create_thread", test_ddp_create_thread) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddp_delete_thread", test_ddp_delete_thread) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddp_push_config_change", test_ddp_push_config_change) == NULL) goto error;

    /* newly added function tests goes here */
    goto done;

error:
    CU_cleanup_registry();

done:
    return CU_get_error();

}

/* stubs */
