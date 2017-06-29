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
#include "test_ddp_if.h"
#include <stdlib.h>

/* Test Suite setup and cleanup functions: */

static int init_suite(void) { return 0; }
static int clean_suite(void) { return 0; }

static void test_callback_fn(PyObject * args)
{
    char * xml_data;
    int const retVal = PyArg_ParseTuple(args, "s", &xml_data);

    printf("%s\n", xml_data);
}

/************* Test case functions ****************/
static void test_ddpif_init(void)
{

}

static void test_ddpif_set(void)
{

}

static void test_ddpif_apply(void)
{

}

static void test_ddpif_save(void)
{

}

static void test_ddpif_poll(void)
{

}

static void test_ddpif_cancel_poll(void)
{

}

static void test_ddpif_push_config_change(void)
{
    bool result;

    ddp_test_info.config.name = "eth";
    ddp_test_info.config.id = config_eth;
    ddp_test_info.config.instance = 1;

    result = ddpif_push_config_change(group_name, instance);
    CU_ASSERT_TRUE(result);
}

/************* Test Runner Code goes here **************/

int add_ddp_if_tests(void)
{
    CU_pSuite pSuite = NULL;

    /* add a suite to the registry */
    pSuite = CU_add_suite("ddp_if_test_suite", init_suite, clean_suite);
    if (pSuite == NULL) goto error;

    ddp_callback = test_callback_fn;

    /* add the tests to the suite */
    if (CU_add_test(pSuite, "test_ddpif_init", test_ddpif_init) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddpif_set", test_ddpif_set) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddpif_apply", test_ddpif_apply) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddpif_save", test_ddpif_save) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddpif_poll", test_ddpif_poll) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddpif_cancel_poll", test_ddpif_cancel_poll) == NULL) goto error;
    if (CU_add_test(pSuite, "test_ddpif_push_config_change", test_ddpif_push_config_change) == NULL) goto error;

    /* newly added function tests goes here */
    goto done;

error:
    CU_cleanup_registry();

done:
    return CU_get_error();

}

/* stubs */
