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
#if USE_BASIC_STYLE
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#elif USE_AUTOMATED_STYLE
#include "CUnit/Automated.h"
#else
#include "CUnit/Console.h"
#endif
#include "cellular_led.h"
#include "lr54leds.h"
#include "tlr_common.h"
#include "tlr_print.h"
//#include "CUnit/CUnit.h"

extern void update_cellular_sim_leds(int const fd, const int active_sim);
extern void update_cellular_service_leds(int const fd, const int active_sim, cellular_context_t * const context);
extern void update_cellular_signal_strength_leds(int const fd);
#define ACTIVE_SIM_0 0
#define ACTIVE_SIM_1 1
#define ACTIVE_SIM_2 2

static cellular_context_t test_context;
static led_t expected_led_sim1;
static led_state_t expected_led_state_sim1;
static led_t expected_led_sim2;
static led_state_t expected_led_state_sim2;
static led_t expected_led_service_green;
static led_state_t expected_led_state_service_green;
static led_t expected_led_service_yellow;
static led_state_t expected_led_state_service_yellow;
static led_t expected_led_signal_green;
static led_state_t expected_led_state_signal_green;
static led_t expected_led_signal_yellow;
static led_state_t expected_led_state_signal_yellow;
static int expected_active_sim;
static config_cellular_state_t expected_cellular_state;
static cellular_connect_status_t expected_connection_status;
static cellular_connection_type_t expected_connection_type;
static int32_t expected_signal_strength;

int led_control(led_t const led, led_state_t const led_state)
{
    printf("led = %d led_state = %d\n", led, led_state);
    switch(led)
    {
        case led_sim1:
            CU_ASSERT(led == expected_led_sim1);
            CU_ASSERT(led_state == expected_led_state_sim1);
            break;
       case led_sim2:
           printf("led = %d led_state = %d\n", led, led_state);
            printf("expected led = %d led_state = %d\n", expected_led_sim2, expected_led_state_sim2);
            CU_ASSERT(led == expected_led_sim2);
            CU_ASSERT(led_state == expected_led_state_sim2);
            break;
       case led_service_green:
            CU_ASSERT(led == expected_led_service_green);
            CU_ASSERT(led_state == expected_led_state_service_green);
            break;
       case led_service_yellow:
            CU_ASSERT(led == expected_led_service_yellow);
            CU_ASSERT(led_state == expected_led_state_service_yellow);
            break;
       case led_signal_green:
            CU_ASSERT(led == expected_led_signal_green);
            CU_ASSERT(led_state == expected_led_state_signal_green);
            break;
       case led_signal_yellow:
            CU_ASSERT(led == expected_led_signal_yellow);
            CU_ASSERT(led_state == expected_led_state_signal_yellow);
            break;
       case led_wifi_24:
       case led_wifi_5:
       case led_power:
       case led_dsl:
       case MAX_LED:
           CU_ASSERT(0);
           break;
    }

    return 0;
}

int cellular_get_active_sim(int const fd, size_t const index_3g, size_t * const active_sim)
{
    *active_sim = 1;
    return 0;
}

tlr_log_t cellular_debug_priority;

void tlr_event_debug(tlr_log_t const priority, char * fmt, ...)
{

}

bool is_current_context_active_sim(cellular_context_t const * const context, size_t const active_sim)
{
    return (context->instance == active_sim);
}

int cellular_open_status(void)
{
    return 1;
}

void cellular_close_status(int fd)
{
    return;
}

config_cellular_state_t get_cellular_state(size_t const instance)
{
    return expected_cellular_state;
}

int cellular_get_connection_type(int const fd, size_t const index_3g, cellular_connection_type_t * const value)
{
    *value = expected_connection_type;
    return 0;
}

int cellular_get_rssi(int const fd, size_t const index_3g, int32_t * const value)
{
    *value = expected_signal_strength;
    return 0;
}

cellular_connect_status_t cellular_get_connection_status(int const fd, size_t const index_3g)
{
    return expected_connection_status;
}

static void setup_expected_led_state_sim1(led_state_t const led_state)
{
    expected_led_state_sim1 = led_state;
}

static void setup_expected_led_sim1(led_t const led)
{
    expected_led_sim1 = led;
}

static void setup_expected_led_state_sim2(led_state_t const led_state)
{
    expected_led_state_sim2 = led_state;
}

static void setup_expected_led_sim2(led_t const led)
{
    expected_led_sim2 = led;
}

static void setup_expected_led_state_service_green(led_state_t const led_state)
{
    expected_led_state_service_green = led_state;
}

static void setup_expected_led_service_green(led_t const led)
{
    expected_led_service_green = led;
}

static void setup_expected_led_state_service_yellow(led_state_t const led_state)
{
    expected_led_state_service_yellow = led_state;
}

static void setup_expected_led_service_yellow(led_t const led)
{
    expected_led_service_yellow = led;
}

static void setup_expected_led_state_signal_green(led_state_t const led_state)
{
    expected_led_state_signal_green = led_state;
}

static void setup_expected_led_signal_green(led_t const led)
{
    expected_led_signal_green = led;
}

static void setup_expected_led_state_signal_yellow(led_state_t const led_state)
{
    expected_led_state_signal_yellow = led_state;
}

static void setup_expected_led_signal_yellow(led_t const led)
{
    expected_led_signal_yellow = led;
}

static void setup_test_context(cellular_state_t const current_state, size_t instance)
{
    memset(&test_context, 0, sizeof test_context);
    test_context.current_state = current_state;
    test_context.instance = instance;
}

static void setup_expected_active_sim(int const active_sim)
{
    expected_active_sim = active_sim;
}

static void setup_expected_cellular_state(int const cellular_state)
{
    expected_cellular_state = cellular_state;
}

static void setup_expected_connection_status(cellular_connect_status_t const connection_status)
{
    expected_connection_status = connection_status;
}

static void setup_expected_connection_type(cellular_connection_type_t const connection_type)
{
    expected_connection_type = connection_type;
}

static void setup_expected_signal_strength(uint32_t signal)
{
    expected_signal_strength = signal;
}

static void perform_sim_test(led_t const led_sim1, led_state_t const led_state_sim1,
                         led_t const led_sim2, led_state_t const led_state_sim2,
                         int const active_sim,
                         config_cellular_state_t const config_state,
                         cellular_connect_status_t const connection_status
                        )
{
    int const fd = 0;
    printf("%d %d %d %d %d %d %d\n", led_sim1, led_state_sim1, led_sim2, led_state_sim2, active_sim, config_state, connection_status);

    setup_expected_led_sim1(led_sim1);
    setup_expected_led_state_sim1(led_state_sim1);
    setup_expected_led_sim2(led_sim2);
    setup_expected_led_state_sim2(led_state_sim2);
    setup_expected_active_sim(active_sim);
    setup_expected_cellular_state(config_state);
    setup_expected_connection_status(connection_status);
    update_cellular_sim_leds(fd, expected_active_sim);
}

static void perform_service_test(led_t const led_service_green, led_state_t const led_state_service_green,
                         led_t const led_service_yellow, led_state_t const led_state_service_yellow,
                         cellular_state_t const current_state, size_t instance,
                          cellular_connection_type_t const connection_type,
                         int const active_sim
                         )
{
    int const fd = 0;

    setup_expected_led_service_green(led_service_green);
    setup_expected_led_state_service_green(led_state_service_green);
    setup_expected_led_service_yellow(led_service_yellow);
    setup_expected_led_state_service_yellow(led_state_service_yellow);
    setup_test_context(current_state, instance);
    setup_expected_active_sim(active_sim);
    setup_expected_connection_type(connection_type);
    update_cellular_service_leds(fd, expected_active_sim, &test_context);
}

static void perform_signal_test(led_t const led_signal_green, led_state_t const led_state_signal_green,
                         led_t const led_signal_yellow, led_state_t const led_state_signal_yellow,
                         cellular_connection_type_t const connection_type,
                         uint32_t signal_strength
                        )
{
    int const fd = 0;
    setup_expected_led_signal_green(led_signal_green);
    setup_expected_led_state_signal_green(led_state_signal_green);
    setup_expected_led_signal_yellow(led_signal_yellow);
    setup_expected_led_state_signal_yellow(led_state_signal_yellow);
    setup_expected_connection_type(connection_type);
    setup_expected_signal_strength(signal_strength);
    update_cellular_signal_strength_leds(fd);
}

static void test_sim_leds_no_sim(void)
{
    perform_sim_test(led_sim1, led_state_off, led_sim2, led_state_off, ACTIVE_SIM_0, config_cellular_state_off, cellular_disconnected);
    perform_sim_test(led_sim1, led_state_off, led_sim2, led_state_off, ACTIVE_SIM_0, config_cellular_state_on, cellular_disconnected);
    perform_sim_test(led_sim1, led_state_off, led_sim2, led_state_off, ACTIVE_SIM_0, config_cellular_state_on_demand, cellular_disconnected);
}

static void test_sim_leds_sim1(void)
{
    perform_sim_test(led_sim1, led_state_off, led_sim2, led_state_off, ACTIVE_SIM_1, config_cellular_state_off, cellular_disconnected);
    perform_sim_test(led_sim1, led_state_on, led_sim2, led_state_off, ACTIVE_SIM_1, config_cellular_state_on, cellular_disconnected);
    perform_sim_test(led_sim1, led_state_off, led_sim2, led_state_off, ACTIVE_SIM_1, config_cellular_state_on_demand, cellular_disconnected);
    perform_sim_test(led_sim1, led_state_off, led_sim2, led_state_off, ACTIVE_SIM_1, config_cellular_state_on_demand, cellular_disconnecting);
    perform_sim_test(led_sim1, led_state_on, led_sim2, led_state_off, ACTIVE_SIM_1, config_cellular_state_on_demand, cellular_connecting);
    perform_sim_test(led_sim1, led_state_on, led_sim2, led_state_off, ACTIVE_SIM_1, config_cellular_state_on_demand, cellular_connected);
}

static void test_sim_leds_sim2(void)
{
    perform_sim_test(led_sim1, led_state_off, led_sim2, led_state_off, ACTIVE_SIM_2, config_cellular_state_off, cellular_disconnected);
    perform_sim_test(led_sim1, led_state_off, led_sim2, led_state_on, ACTIVE_SIM_2, config_cellular_state_on, cellular_disconnected);
    perform_sim_test(led_sim1, led_state_off, led_sim2, led_state_off, ACTIVE_SIM_2, config_cellular_state_on_demand, cellular_disconnected);
    perform_sim_test(led_sim1, led_state_off, led_sim2, led_state_off, ACTIVE_SIM_2, config_cellular_state_on_demand, cellular_disconnecting);
    perform_sim_test(led_sim1, led_state_off, led_sim2, led_state_on, ACTIVE_SIM_2, config_cellular_state_on_demand, cellular_connecting);
    perform_sim_test(led_sim1, led_state_off, led_sim2, led_state_on, ACTIVE_SIM_2, config_cellular_state_on_demand, cellular_connected);
}

static void test_service_leds_off(void)
{
    perform_service_test(led_service_green, led_state_off, led_service_yellow, led_state_off, state_idle, 1, cellular_conenction_type_3g, ACTIVE_SIM_1);
    perform_service_test(led_service_green, led_state_off, led_service_yellow, led_state_off, state_initialize, 1, cellular_conenction_type_3g, ACTIVE_SIM_1);
    perform_service_test(led_service_green, led_state_off, led_service_yellow, led_state_off, state_disconnect, 1, cellular_conenction_type_3g, ACTIVE_SIM_1);
    perform_service_test(led_service_green, led_state_off, led_service_yellow, led_state_off, state_disconnecting, 1, cellular_conenction_type_3g, ACTIVE_SIM_1);
    perform_service_test(led_service_green, led_state_off, led_service_yellow, led_state_off, state_disconnected, 1, cellular_conenction_type_3g, ACTIVE_SIM_1);

    perform_service_test(led_service_green, led_state_off, led_service_yellow, led_state_off, state_idle, 2, cellular_conenction_type_3g, ACTIVE_SIM_2);
    perform_service_test(led_service_green, led_state_off, led_service_yellow, led_state_off, state_initialize, 2, cellular_conenction_type_3g, ACTIVE_SIM_2);
    perform_service_test(led_service_green, led_state_off, led_service_yellow, led_state_off, state_disconnect, 2, cellular_conenction_type_3g, ACTIVE_SIM_2);
    perform_service_test(led_service_green, led_state_off, led_service_yellow, led_state_off, state_disconnecting, 2, cellular_conenction_type_3g, ACTIVE_SIM_2);
    perform_service_test(led_service_green, led_state_off, led_service_yellow, led_state_off, state_disconnected, 2, cellular_conenction_type_3g, ACTIVE_SIM_2);
}

static void test_service_leds_blink(void)
{
    perform_service_test(led_service_green, led_state_blink_slow, led_service_yellow, led_state_off, state_connect, 1, cellular_conenction_type_3g, ACTIVE_SIM_1);
    perform_service_test(led_service_green, led_state_blink_slow, led_service_yellow, led_state_off, state_connecting, 1, cellular_conenction_type_3g, ACTIVE_SIM_1);
    perform_service_test(led_service_green, led_state_blink_slow, led_service_yellow, led_state_off, state_wait_modem, 1, cellular_conenction_type_3g, ACTIVE_SIM_1);

    perform_service_test(led_service_green, led_state_blink_slow, led_service_yellow, led_state_off, state_connect, 2, cellular_conenction_type_3g, ACTIVE_SIM_2);
    perform_service_test(led_service_green, led_state_blink_slow, led_service_yellow, led_state_off, state_connecting, 2, cellular_conenction_type_3g, ACTIVE_SIM_2);
    perform_service_test(led_service_green, led_state_blink_slow, led_service_yellow, led_state_off, state_wait_modem, 2, cellular_conenction_type_3g, ACTIVE_SIM_2);
}

static void test_service_leds_on(void)
{
    perform_service_test(led_service_green, led_state_off, led_service_yellow, led_state_on, state_connected, 1, cellular_conenction_type_3g, ACTIVE_SIM_1);
    perform_service_test(led_service_green, led_state_on, led_service_yellow, led_state_off, state_connected, 1, cellular_conenction_type_4g, ACTIVE_SIM_1);
    perform_service_test(led_service_green, led_state_off, led_service_yellow, led_state_on, state_connected, 1, cellular_connection_type_unknown, ACTIVE_SIM_1);
    perform_service_test(led_service_green, led_state_off, led_service_yellow, led_state_on, state_connected, 1, cellular_conenction_type_2g, ACTIVE_SIM_1);

    perform_service_test(led_service_green, led_state_off, led_service_yellow, led_state_on, state_connected, 2, cellular_conenction_type_3g, ACTIVE_SIM_2);
    perform_service_test(led_service_green, led_state_on, led_service_yellow, led_state_off, state_connected, 2, cellular_conenction_type_4g, ACTIVE_SIM_2);
    perform_service_test(led_service_green, led_state_off, led_service_yellow, led_state_on, state_connected, 2, cellular_connection_type_unknown, ACTIVE_SIM_2);
    perform_service_test(led_service_green, led_state_off, led_service_yellow, led_state_on, state_connected, 2, cellular_conenction_type_2g, ACTIVE_SIM_2);
}

static void test_signal_leds_4g(void)
{
    perform_signal_test(led_signal_green, led_state_on, led_signal_yellow, led_state_off, cellular_conenction_type_4g, -105);
    perform_signal_test(led_signal_green, led_state_off, led_signal_yellow, led_state_on, cellular_conenction_type_4g, -106);
    perform_signal_test(led_signal_green, led_state_off, led_signal_yellow, led_state_on, cellular_conenction_type_4g, -107);
    perform_signal_test(led_signal_green, led_state_off, led_signal_yellow, led_state_on, cellular_conenction_type_4g, -120);
    perform_signal_test(led_signal_green, led_state_off, led_signal_yellow, led_state_off, cellular_conenction_type_4g, -121);
    perform_signal_test(led_signal_green, led_state_off, led_signal_yellow, led_state_off, cellular_conenction_type_4g, -122);
}

static void test_signal_leds_3g(void)
{
    perform_signal_test(led_signal_green, led_state_on, led_signal_yellow, led_state_off, cellular_conenction_type_3g, -85);
    perform_signal_test(led_signal_green, led_state_off, led_signal_yellow, led_state_on, cellular_conenction_type_3g, -86);
    perform_signal_test(led_signal_green, led_state_off, led_signal_yellow, led_state_on, cellular_conenction_type_3g, -87);
    perform_signal_test(led_signal_green, led_state_off, led_signal_yellow, led_state_on, cellular_conenction_type_3g, -109);
    perform_signal_test(led_signal_green, led_state_off, led_signal_yellow, led_state_off, cellular_conenction_type_3g, -110);
    perform_signal_test(led_signal_green, led_state_off, led_signal_yellow, led_state_off, cellular_conenction_type_3g, -111);
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
    CU_TEST_INFO(test_sim_leds_no_sim),
    CU_TEST_INFO(test_sim_leds_sim1),
    CU_TEST_INFO(test_sim_leds_sim2),
    CU_TEST_INFO(test_service_leds_off),
    CU_TEST_INFO(test_service_leds_blink),
    CU_TEST_INFO(test_service_leds_on),
    CU_TEST_INFO(test_signal_leds_4g),
    CU_TEST_INFO(test_signal_leds_3g),
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo test_cellular_monitor_suites[] =
{
    { "cellular_led", init_suite, clean_suite, tests },
    CU_SUITE_INFO_NULL,
};


int main(void)
{
    int result;

    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        result = CU_get_error();
        goto error;
    }

    if (CU_register_nsuites(1, test_cellular_monitor_suites) != CUE_SUCCESS)
    {
        result = CU_get_error();
        goto error;
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    if (CU_basic_run_tests() != CUE_SUCCESS)
    {
        result = CU_get_error();
        goto error;
    }

    result = CU_get_number_of_failures();

error:
    CU_cleanup_registry();
    return result;
}
