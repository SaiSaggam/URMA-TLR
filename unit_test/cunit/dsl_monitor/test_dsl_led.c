/*****************************************************************************
* Copyright (c) 2016 Digi International Inc., All Rights Reserved
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

#include "test_dsl_led.h"
#include "dsl_led.h"
#include "dsl_state.h"
#include "lr54leds.h"
#include "tlr_common.h"
#include "CUnit/CUnit.h"

static dsl_context_t test_context;
static led_state_t expected_led_state;

int led_control(led_t const led, led_state_t const led_state)
{
    CU_ASSERT(led == led_dsl);
    CU_ASSERT(led_state == expected_led_state);

    return 0;
}

static void setup_expected_led_state(led_state_t const led_state)
{
    expected_led_state = led_state;
}

static void setup_test_context(bool const line_is_up, dsl_state_t const desired_state, dsl_state_t const current_state)
{
    memset(&test_context, 0, sizeof test_context);
    test_context.is_line_up = line_is_up;
    test_context.desired_state = desired_state;
    test_context.current_state = current_state;
}

static void perform_test(led_state_t const led_state, bool const line_is_up, dsl_state_t const desired_state, dsl_state_t const current_state)
{
    setup_expected_led_state(led_state);
    setup_test_context(line_is_up, desired_state, current_state);
    dsl_led_update_state(&test_context);
}

static void test_led_is_off_when_dsl_is_off(void)
{
    perform_test(led_state_off, false, dsl_state_disconnected, dsl_state_disconnected);
    perform_test(led_state_off, true, dsl_state_disconnected, dsl_state_disconnected);
    perform_test(led_state_off, false, dsl_state_disconnected, dsl_state_connecting);
    perform_test(led_state_off, true, dsl_state_disconnected, dsl_state_connecting);
    perform_test(led_state_off, false, dsl_state_disconnected, dsl_state_connected);
    perform_test(led_state_off, true, dsl_state_disconnected, dsl_state_connected);
    perform_test(led_state_off, false, dsl_state_disconnected, dsl_state_disconnecting);
    perform_test(led_state_off, true, dsl_state_disconnected, dsl_state_disconnecting);
}

static void test_led_is_blink_slow_when_line_is_attempting_to_train(void)
{
    perform_test(led_state_blink_slow, false, dsl_state_connected, dsl_state_disconnected);
    perform_test(led_state_blink_slow, false, dsl_state_connected, dsl_state_connecting);
    perform_test(led_state_blink_slow, false, dsl_state_connected, dsl_state_disconnecting);
}

static void test_led_is_blink_fast_when_line_is_up_and_ppp_is_down(void)
{
    perform_test(led_state_blink_fast, true, dsl_state_connected, dsl_state_disconnected);
    perform_test(led_state_blink_fast, true, dsl_state_connected, dsl_state_connecting);
    perform_test(led_state_blink_fast, true, dsl_state_connected, dsl_state_disconnecting);
}

static void test_led_is_on_when_ppp_is_up(void)
{
    perform_test(led_state_on, false, dsl_state_connected, dsl_state_connected);
    perform_test(led_state_on, true, dsl_state_connected, dsl_state_connected);
}

static int init_suite(void)
{
    return 0;
}

static int clean_suite(void)
{
    return 0;
}

#define CU_TEST_INFO(test) { #test, test }

static CU_TestInfo tests[] =
{
    CU_TEST_INFO(test_led_is_off_when_dsl_is_off),
    CU_TEST_INFO(test_led_is_blink_slow_when_line_is_attempting_to_train),
    CU_TEST_INFO(test_led_is_blink_fast_when_line_is_up_and_ppp_is_down),
    CU_TEST_INFO(test_led_is_on_when_ppp_is_up),
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo test_dsl_led_suites[] =
{
    { "dsl_led", init_suite, clean_suite, tests },
    CU_SUITE_INFO_NULL,
};

