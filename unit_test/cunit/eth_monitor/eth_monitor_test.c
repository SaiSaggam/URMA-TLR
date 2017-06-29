/*****************************************************************************
 * Copyright (c) 2015 Digi International Inc., All Rights Reserved
 *
 * This software contains proprietary and confidential information of Digi
 * International Inc.  By accepting transfer of this copy, Recipient agrees
 * to retain this software in confidence, to prevent disclosure to others,
 * and to make no use of this software other than that for which it was
 * delivered.  This is an unpublished copyrighted work of Digi International
 * Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
 * prohibited.
 *
 * Restricted Rights Legend
 *
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7031 or
 * subparagraphs (c)(1) and (2) of the Commercial Computer Software -
 * Restricted Rights at 48 CFR 52.227-19, as applicable.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 *
 *****************************************************************************/
#define     USE_BASIC_STYLE     1
#define     USE_AUTOMATED_STYLE     0
#define     USE_CONSOLE_STYLE     0

#include "eth_monitor.h"
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
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "ethphy.h"

// Simulation functions
void simulate_line(bool stat);


int init_suite(void)
{
    return 0;
}

int clean_suite(void)
{
    return 0;
}


static  eth_context_t   test_context;
static state_t  expect_state;
static int      expecting_do_connect;
static int      expecting_do_disconnect;

void expect_do_connect(int exp)
{
    expecting_do_connect = exp;
}
void expect_do_disconnect(int exp)
{
    expecting_do_disconnect = exp;
}
void expect_set_eth_interface(state_t state)
{
    expect_state = state;
}

/************* Test case functions ****************/



/******* Test Cases ******/
void test_eth_mon_linkup(void)
{
    eth_context_t   *context = &test_context;

    // check we take correct action when physical link comes up.
    context->link_state = false;
    context->desired_eth_state = true;
    simulate_line(true);    // So poll function reads it as on.

    // Setup expectations.
    expect_do_connect(1);
    expect_do_disconnect(0);
    expect_set_eth_interface(state_eth_up);

    // and now test this function..
    poll_eth_status(context);
    CU_ASSERT_EQUAL(expecting_do_connect, -1);
    CU_ASSERT_EQUAL(expect_state, state_init);
}


void test_eth_mon_linkdown(void)
{
    eth_context_t   *context = &test_context;

    // check we take correct action when physical link goes down
    context->link_state = true;
    context->desired_eth_state = false;
    context->state = state_eth_up;
    simulate_line(false);    // So poll function reads it as off

    // Setup expectations.
    expect_do_connect(0);
    expect_do_disconnect(1);
    expect_set_eth_interface(state_eth_down);

    // and now test this function..
    poll_eth_status(context);
    CU_ASSERT_EQUAL(expecting_do_disconnect, -1);
    CU_ASSERT_EQUAL(expect_state, state_init);
}


void test_eth_mon_linkup_but_cannot(void)
{
    eth_context_t   *context = &test_context;

    // check we take correct action when physical link goes down but it is desired to be up
    context->link_state = true;
    context->desired_eth_state = true;
    context->state = state_eth_up;
    simulate_line(false);    // So poll function reads it as off

    // Setup expectations.
    expect_do_connect(0);
    expect_do_disconnect(1);
    expect_set_eth_interface(state_eth_down);

    // and now test this function..
    poll_eth_status(context);
    CU_ASSERT_EQUAL(expecting_do_disconnect, -1);
    CU_ASSERT_EQUAL(expect_state, state_init);
}


/************* Test Runner Code goes here **************/

int main(void)
{
    int number_of_failures;
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("eth_monitor_test_suite", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if (0
        || (CU_add_test(pSuite, "test_eth_mon_linkup", test_eth_mon_linkup) == NULL)
        || (CU_add_test(pSuite, "test_eth_mon_linkdown", test_eth_mon_linkdown) == NULL)
        || (CU_add_test(pSuite, "test_eth_mon_linkup_but_cannot", test_eth_mon_linkup_but_cannot) == NULL)

       )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

#if USE_BASIC_STYLE
    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
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

    //	Clean up registry and return
    number_of_failures = CU_get_number_of_failures();
    CU_cleanup_registry();

    //	Ensure we return an error if any tests failed
    if (number_of_failures)
        return -1;
    return CU_get_error();
}



/****************  Stubs   ***************************/
static bool     line_status;
void simulate_line(bool stat)
{
    line_status = stat;
}

int reg_read(int port, int phyreg, int * value)
{

    if (phyreg == PHY_STAT)
    {
        if (line_status == true)
            *value = PHY_STAT_LINKUP;
        else
            *value = 0;
        return 0;
    }
    else
        return -1;
}


int eth_do_connect_action(eth_context_t * const context)
{
    if (expecting_do_connect == 1)
    {
        CU_PASS("do connect");
    }
    else
    {
        CU_FAIL("do connect not expected");
    }
    expecting_do_connect = -1;

    return 0;
}

int eth_do_disconnect_action(eth_context_t * const context)
{
    if (expecting_do_disconnect == 1)
    {
        CU_PASS("do disconnect");
    }
    else
    {
        CU_FAIL("do disconnect not expected");
    }
    expecting_do_disconnect = -1;
    return 0;
}

void set_eth_interface_state(eth_context_t * const context, state_t new_state)
{
    CU_ASSERT_EQUAL(expect_state, new_state);
    expect_state = state_init;
}

