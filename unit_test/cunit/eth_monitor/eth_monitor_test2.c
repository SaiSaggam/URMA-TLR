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

#include "dhcp_client.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include "config_mgr.h"
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

/****************  Stubs   ***************************/

// Simulations

static int      want_PHY_AUTOADVERT;
static int      want_PHY_1000BASET_CTRL;
static int      want_PHY_CON;


void expect_PHY_AUTOADVERT(int exp)
{
    want_PHY_AUTOADVERT = exp;
}
void expect_PHY_1000BASET_CTRL(int exp)
{
    want_PHY_1000BASET_CTRL = exp;
}
void expect_PHY_CON(int exp)
{
    want_PHY_CON = exp;
}

pid_t dhcp_client_get_pid(interfaces_t const interface, size_t const instance)
{
    return (pid_t)0;
}

int dhcp_client_start(interfaces_t const interface, size_t const instance)
{
    return 0;
}

int dhcp_client_stop(interfaces_t const interface, size_t const instance)
{
    return 0;
}

/************* Test case functions ****************/


/******* Test Cases ******/
void test_100_full(void)
{
    int adv_want =  PHY_AUTOADV_100FULLDUP;
    int ctrl_1000_want = 0;
    expect_PHY_AUTOADVERT(adv_want);
    expect_PHY_1000BASET_CTRL(ctrl_1000_want);
    expect_PHY_CON(PHY_CON_RESTARTNEG);

    eth_apply_speed_duplex(1, "100", "full");

    CU_ASSERT_EQUAL(want_PHY_AUTOADVERT, 0);
    CU_ASSERT_EQUAL(want_PHY_1000BASET_CTRL, 0);
    CU_ASSERT_EQUAL(want_PHY_CON, 0);

}

void test_100_half(void)
{
    int adv_want =  PHY_AUTOADV_100HALFDUP;
    int ctrl_1000_want = 0;
    expect_PHY_AUTOADVERT(adv_want);
    expect_PHY_1000BASET_CTRL(ctrl_1000_want);
    expect_PHY_CON(PHY_CON_RESTARTNEG);

    eth_apply_speed_duplex(1, "100", "half");

    CU_ASSERT_EQUAL(want_PHY_AUTOADVERT, 0);
    CU_ASSERT_EQUAL(want_PHY_1000BASET_CTRL, 0);
    CU_ASSERT_EQUAL(want_PHY_CON, 0);
}


void test_10_full(void)
{
    int adv_want =  PHY_AUTOADV_10FULLDUP;
    int ctrl_1000_want = 0;
    expect_PHY_AUTOADVERT(adv_want);
    expect_PHY_1000BASET_CTRL(ctrl_1000_want);
    expect_PHY_CON(PHY_CON_RESTARTNEG);

    eth_apply_speed_duplex(1, "10", "full");

    CU_ASSERT_EQUAL(want_PHY_AUTOADVERT, 0);
    CU_ASSERT_EQUAL(want_PHY_1000BASET_CTRL, 0);
    CU_ASSERT_EQUAL(want_PHY_CON, 0);

}

void test_10_half(void)
{
    int adv_want =  PHY_AUTOADV_10HALFDUP;
    int ctrl_1000_want = 0;
    expect_PHY_AUTOADVERT(adv_want);
    expect_PHY_1000BASET_CTRL(ctrl_1000_want);
    expect_PHY_CON(PHY_CON_RESTARTNEG);

    eth_apply_speed_duplex(1, "10", "half");

    CU_ASSERT_EQUAL(want_PHY_AUTOADVERT, 0);
    CU_ASSERT_EQUAL(want_PHY_1000BASET_CTRL, 0);
    CU_ASSERT_EQUAL(want_PHY_CON, 0);
}

void test_1000_full(void)
{
    int adv_want =  0;
    int ctrl_1000_want = PHY_CTRL_1000FULL;
    expect_PHY_AUTOADVERT(adv_want);
    expect_PHY_1000BASET_CTRL(ctrl_1000_want);
    expect_PHY_CON(PHY_CON_RESTARTNEG);

    eth_apply_speed_duplex(1, "1000", "full");

    CU_ASSERT_EQUAL(want_PHY_AUTOADVERT, 0);
    CU_ASSERT_EQUAL(want_PHY_1000BASET_CTRL, 0);
    CU_ASSERT_EQUAL(want_PHY_CON, 0);

}

void test_1000_half(void)
{
    int adv_want =  0;
    int ctrl_1000_want = PHY_CTRL_1000HALF;
    expect_PHY_AUTOADVERT(adv_want);
    expect_PHY_1000BASET_CTRL(ctrl_1000_want);
    expect_PHY_CON(PHY_CON_RESTARTNEG);

    eth_apply_speed_duplex(1, "1000", "half");

    CU_ASSERT_EQUAL(want_PHY_AUTOADVERT, 0);
    CU_ASSERT_EQUAL(want_PHY_1000BASET_CTRL, 0);
    CU_ASSERT_EQUAL(want_PHY_CON, 0);
}


void test_auto(void)
{
    int adv_want = PHY_AUTOADV_100HALFDUP | PHY_AUTOADV_100FULLDUP | PHY_AUTOADV_10FULLDUP | PHY_AUTOADV_10HALFDUP;
    int ctrl_1000_want = PHY_CTRL_1000HALF | PHY_CTRL_1000FULL;

    expect_PHY_AUTOADVERT(adv_want);
    expect_PHY_1000BASET_CTRL(ctrl_1000_want);
    expect_PHY_CON(PHY_CON_RESTARTNEG);

    eth_apply_speed_duplex(1, "auto", "auto");

    CU_ASSERT_EQUAL(want_PHY_AUTOADVERT, 0);
    CU_ASSERT_EQUAL(want_PHY_1000BASET_CTRL, 0);
    CU_ASSERT_EQUAL(want_PHY_CON, 0);
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
        || (CU_add_test(pSuite, "test_100_full;", test_100_full) == NULL)
        || (CU_add_test(pSuite, "test_100_half;", test_100_half) == NULL)
        || (CU_add_test(pSuite, "test_10_full;", test_10_full) == NULL)
        || (CU_add_test(pSuite, "test_10_half;", test_10_half) == NULL)
        || (CU_add_test(pSuite, "test_1000_full;", test_1000_full) == NULL)
        || (CU_add_test(pSuite, "test_1000_half;", test_1000_half) == NULL)
        || (CU_add_test(pSuite, "test_auto;", test_auto) == NULL)
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
unsigned int sleep(unsigned int __seconds)
{
    return 0;
}

int reg_read(int port, int phyreg, int * value)
{
    *value = 0;
    return 0;
}



int reg_write(int port, int phyreg, int value)
{
    switch (phyreg)
    {
    case PHY_AUTOADVERT:
        if (want_PHY_AUTOADVERT != value)
        {
            printf("expecting 0x%x but have %x autoadvert\n", want_PHY_AUTOADVERT, value);
        }
        CU_ASSERT_EQUAL(want_PHY_AUTOADVERT, value);
        want_PHY_AUTOADVERT = 0;
        break;
    case PHY_1000BASET_CTRL:
       if (want_PHY_1000BASET_CTRL != value)
        {
            printf("expecting 0x%x but have  %x 1000base\n", want_PHY_1000BASET_CTRL, value);
        }
        CU_ASSERT_EQUAL(want_PHY_1000BASET_CTRL, value);
        want_PHY_1000BASET_CTRL = 0;
        break;
    case PHY_CON:
       if (want_PHY_CON != value)
        {
            printf("expecting 0x%x but have %x PHY_CON\n", want_PHY_CON, value);
        }
        CU_ASSERT_EQUAL(want_PHY_CON, value);
        want_PHY_CON = 0;
        break;
    default:
        CU_FAIL("phy registger write not expected");
    }
    return 0;
}

config_status_t tlr_get_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      void * const value,
                                      size_t const bytes,
                                      config_mgr_element_type_t const type,
                                      config_mgr_access_level_t const access)
{
    return config_status_success;
}

void tlr_config_close(tlr_handle_t const handle)
{

}


config_status_t tlr_config_read(tlr_handle_t const handle, size_t const element_id, char * const data, size_t const bytes)
{
    return config_status_success;
}

tlr_handle_t tlr_config_open(size_t const group_id, size_t const instance, config_mgr_access_level_t const access)
{
    return NULL;
}

