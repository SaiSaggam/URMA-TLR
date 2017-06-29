
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
#define     USE_AUTOMATED_STYLE   0
#define     USE_CONSOLE_STYLE     0

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
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#if !defined(NO_TIMERFD_SUPPORT)
#include <sys/timerfd.h>
#endif
#include <pthread.h>
#include <sys/queue.h>
#include <semaphore.h>
#include <signal.h>
#include <errno.h>
#include <stdint.h>


#include "config_def.h"
#include "cellular_monitor.h"
#include "cellular_modem.h"
#include "cellular_status.h"
#include "cellular_config.h"
#include "monitor_debug.h"
#include "context.h"

#include "interfaces.h"
#include "event_queue.h"

bool init_3g_initialized = true;

cellular_context_t cellularContextData[2];
cellular_context_t * cellular_contexts = cellularContextData;

size_t stop_timer_call_count = 0;
size_t start_timer_call_count = 0;
size_t send_disconnected_event_to_interface_manager_call_count = 0;
size_t send_connected_event_to_interface_manager_call_count = 0;

bool cellular_get_usb_ready_value = true;
bool cellular_get_usb_ready(int const fd, size_t const index_3g)
{
    return cellular_get_usb_ready_value;
}

int send_connected_event_to_interface_manager(size_t interface_instance)
{
    send_connected_event_to_interface_manager_call_count++;
    return 0;
}


int send_disconnected_event_to_interface_manager(size_t interface_instance)
{
    send_disconnected_event_to_interface_manager_call_count++;
    return 0;
}

config_cellular_state_t get_cellular_state(size_t const instance)
{
    return config_cellular_state_on;
}

int cellular_set_active_sim(int const fd, size_t const index_3g, size_t const active_sim)
{
    return 0;
}

size_t system_command_call_count;
int system_command(char const * const format, ...)
{
    system_command_call_count++;
    return 0;
}


cellular_config_status_t get_cellular_data_value = cellular_config_no_changed;
cellular_config_status_t get_cellular_data(size_t const instance, cellular_config_t * const config)
{
    return get_cellular_data_value;
}

void state_disconnecting_handler(cellular_context_t * const context, void * const data);

void test_state_disconnecting_handler(void)
{
    cellular_modem_status_t modem_status = {0};
    cellular_event_t event;
    size_t i;

    {
        unsigned int event_code[] = {
            connect_message,
            disconnect_message,
            connected_message,
            disconnected_message,
            update_message,
            ping_message,
            pong_message,
            query_connect,
            cellular_message_initialize,
            cellular_message_connect_timeout,
            cellular_message_disconnect_timeout,
            CELL_PING_RESPONSE_TMR_EVENT,
        };

        for (i = 0 ; i < ARRAY_SIZE(event_code); i++)
        {
            event.code = event_code[i];
            system_command_call_count = 0;
            cellular_contexts[0].csman_fd = 0;
            cellular_contexts[0].desired_state = false;
            cellular_contexts[0].current_state = state_disconnecting;
            cellular_contexts[0].modem_status = &modem_status;
            state_disconnecting_handler(&cellular_contexts[0], &event);

            switch (event_code[i])
            {
                case connect_message:
                    CU_ASSERT_TRUE(cellular_contexts[0].desired_state);
                    break;
                case disconnect_message:
                    CU_ASSERT_FALSE(cellular_contexts[0].desired_state);
                    CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_idle);
                    break;
                case disconnected_message:
                    CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_idle);
                    break;
                case cellular_message_disconnect_timeout:
                    CU_ASSERT_EQUAL(system_command_call_count, 1);
                    /* no break; */
                default:
                    /* should not do anything for other state */
                    CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_disconnecting);
                    break;
            }
        }
    }

    {
        unsigned event_code[] = {
                cellular_message_disconnect_timeout,
                disconnected_message,
                disconnect_message
        };


         for (i = 0 ; i < ARRAY_SIZE(event_code); i++)
         {
             event.code = event_code[i];
             cellular_contexts[0].desired_state = false;
             cellular_contexts[0].disconnecting_timer_running = true;
             cellular_contexts[0].current_state = state_disconnecting;
             cellular_contexts[0].had_sent_act_in = true;
             stop_timer_call_count = 0;
             send_disconnected_event_to_interface_manager_call_count = 0;
             state_disconnecting_handler(&cellular_contexts[0], &event);

             switch (event_code[i])
             {
                 case connect_message:
                     CU_ASSERT_TRUE(cellular_contexts[0].desired_state);
                     break;
                 case disconnect_message:
                     CU_ASSERT_FALSE(cellular_contexts[0].desired_state);
                     /* no break; */
                 case disconnected_message:
                     CU_ASSERT_FALSE(cellular_contexts[0].disconnecting_timer_running);
                     CU_ASSERT_FALSE(cellular_contexts[0].desired_state);
    //                 CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_idle);
                     CU_ASSERT_EQUAL(stop_timer_call_count, 1);
                     break;
                 case cellular_message_disconnect_timeout:
                     CU_ASSERT_TRUE(cellular_contexts[0].disconnecting_timer_running);
                     CU_ASSERT_EQUAL(system_command_call_count, 1);
                     /* no break; */
                 default:
                     /* should not do anything for other state */
                     CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_disconnecting);
                     break;
             }
         }
    }

}

extern void state_disconnected_handler(cellular_context_t * const context, void * const data);

void test_state_disconnected_handler(void)
{
    cellular_event_t event;
    size_t i;


    {
        unsigned int event_code[] = {
            disconnect_message,
            disconnected_message,
            cellular_message_disconnect_timeout,
        };

        for (i = 0 ; i < ARRAY_SIZE(event_code); i++)
        {
            event.code = event_code[i];

            cellular_contexts[0].desired_state = false;
            cellular_contexts[0].current_state = state_disconnected;
            state_disconnected_handler(&cellular_contexts[0], &event);

            CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_idle);
            CU_ASSERT_FALSE(cellular_contexts[0].desired_state);
        }
    }
}

void state_connected_handler(cellular_context_t * const context, void * const data);

void test_state_connected_handler(void)
{
    cellular_event_t event;
    cellular_modem_status_t modem_status = {0};
    size_t i;


    {
        unsigned int event_code[] = {
            connect_message,
            disconnect_message,
            connected_message,
            disconnected_message,
            update_message,
            ping_message,
            pong_message,
            query_connect,
            cellular_message_initialize,
            cellular_message_connect_timeout,
            cellular_message_disconnect_timeout,
            CELL_PING_RESPONSE_TMR_EVENT,
        };

        for (i = 0 ; i < ARRAY_SIZE(event_code); i++)
        {
            event.code = event_code[i];
            cellular_contexts[0].desired_state = true;
            cellular_contexts[0].had_sent_act_in = true;  // should be set when connect message is received
            cellular_contexts[0].current_state = state_connected;
            cellular_contexts[0].instance = 1;
            modem_status.active_sim = 1;
            modem_status.connect_status = cellular_connected;
            cellular_contexts[0].modem_status = &modem_status;
            system_command_call_count = 0;
            send_connected_event_to_interface_manager_call_count = 0;
            send_disconnected_event_to_interface_manager_call_count = 0;

            get_cellular_data_value = cellular_config_changed;

            state_connected_handler(&cellular_contexts[0], &event);

            switch (event.code)
            {
                case disconnect_message:
                    /* desired_state should be clear so that it will not reconnect */
                    CU_ASSERT_FALSE(cellular_contexts[0].desired_state);
                    CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_disconnect);
                    break;

                case connect_message:
                    /* desired state should be set so that it will reconnect */
                    CU_ASSERT_TRUE(cellular_contexts[0].desired_state);
                    CU_ASSERT_TRUE(cellular_contexts[0].had_sent_act_in);
                    CU_ASSERT_EQUAL(send_connected_event_to_interface_manager_call_count, 1);
                    break;

                case disconnected_message:
                    /* interface link gone down so need to restart
                     * connecting without notifying interface manager
                     */
                    CU_ASSERT_EQUAL(send_connected_event_to_interface_manager_call_count, 0);
                    CU_ASSERT_EQUAL(send_disconnected_event_to_interface_manager_call_count, 0);
                    CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_connecting);
                    break;

                case update_message:
                    /* if configuration changed, expected disconnecting the connection
                     */
                    CU_ASSERT_EQUAL(send_connected_event_to_interface_manager_call_count, 0);
                    CU_ASSERT_EQUAL(send_disconnected_event_to_interface_manager_call_count, 0);
                    CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_disconnecting);

                    break;

                default:
                    break;
            }

        }
    }
}

void state_connecting_handler(cellular_context_t * const context, void * const data);

void test_state_connecting_handler(void)
{
    cellular_event_t event;
    cellular_modem_status_t modem_status = {0};
    size_t i;


    {
        unsigned int event_code[] = {
            connect_message,
            disconnect_message,
            connected_message,
            disconnected_message,
            update_message,
            ping_message,
            pong_message,
            query_connect,
            cellular_message_initialize,
            cellular_message_connect_timeout,
            cellular_message_disconnect_timeout,
            CELL_PING_RESPONSE_TMR_EVENT,
        };

        for (i = 0 ; i < ARRAY_SIZE(event_code); i++)
        {
            event.code = event_code[i];
            cellular_contexts[0].desired_state = true;
            cellular_contexts[0].had_sent_act_in = true;  // should be set when connect message is received
            cellular_contexts[0].current_state = state_connecting;
            cellular_contexts[0].instance = 1;
            modem_status.active_sim = 1;
            modem_status.connect_status = cellular_connected;
            cellular_contexts[0].connecting_timer_running = true;
            cellular_contexts[0].modem_status = &modem_status;
            system_command_call_count = 0;
            send_connected_event_to_interface_manager_call_count = 0;
            send_disconnected_event_to_interface_manager_call_count = 0;

            get_cellular_data_value = cellular_config_changed;

            state_connecting_handler(&cellular_contexts[0], &event);


            switch (event.code)
            {
                case cellular_message_connect_timeout:
                    /* keep waiting connection */
                    CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_connecting);
                    CU_ASSERT_TRUE(cellular_contexts[0].connecting_timer_running);
                    break;
                case disconnect_message:

                    CU_ASSERT_FALSE(cellular_contexts[0].connecting_timer_running);
                    /* desired_state should be clear for not to reconnect */
                    CU_ASSERT_FALSE(cellular_contexts[0].desired_state);
                    CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_disconnect);
                    break;

                case connected_message:
                    /* interface now connected. connecting timer should be clear */
                    CU_ASSERT_FALSE(cellular_contexts[0].connecting_timer_running);
                    CU_ASSERT_TRUE(cellular_contexts[0].desired_state);
                    CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_connected);
                    break;
                case update_message:
                    /* if configuration changed, expected disconnecting the connection
                     */
                    CU_ASSERT_EQUAL(send_connected_event_to_interface_manager_call_count, 0);
                    CU_ASSERT_EQUAL(send_disconnected_event_to_interface_manager_call_count, 0);
                    CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_disconnecting);
                    break;

                default:
                    break;
            }
        }

    }
}


void state_wait_modem_handler(cellular_context_t * const context, void * const data);

void test_state_wait_modem_handler(void)
{
    cellular_event_t event;
    cellular_modem_status_t modem_status = {0};
    size_t i;


    {
        unsigned int event_code[] = {
            connect_message,
            disconnect_message,
            connected_message,
            disconnected_message,
            update_message,
            ping_message,
            pong_message,
            query_connect,
            cellular_message_initialize,
            cellular_message_connect_timeout,
            cellular_message_disconnect_timeout,
            CELL_PING_RESPONSE_TMR_EVENT,
            cellular_message_disconnect
        };

        for (i = 0 ; i < ARRAY_SIZE(event_code); i++)
        {
            event.code = event_code[i];
            cellular_contexts[0].desired_state = true;
            cellular_contexts[0].had_sent_act_in = true;  // should be set when connect message is received
            cellular_contexts[0].current_state = state_wait_modem;
            cellular_contexts[0].instance = 1;
            modem_status.active_sim = 2;
            modem_status.connect_status = cellular_disconnected;
            cellular_contexts[0].connecting_timer_running = true;
            cellular_contexts[0].modem_status = &modem_status;
            stop_timer_call_count = 0;
            start_timer_call_count = 0;
            system_command_call_count = 0;
            send_disconnected_event_to_interface_manager_call_count = 0;
            cellular_get_usb_ready_value = false;
            state_wait_modem_handler(&cellular_contexts[0], &event);

            switch (event.code)
            {
                case cellular_message_connect_timeout:
                    CU_ASSERT_EQUAL(system_command_call_count, 2);
                    break;
                case disconnect_message:
                    CU_ASSERT_EQUAL(send_disconnected_event_to_interface_manager_call_count, 1);
                    CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_idle);
                    break;
                case cellular_message_disconnect:
                    CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_idle);
                    break;

                default:
                    break;
            }

        }
    }

}

void state_idle_handler(cellular_context_t * const context, void * const data);

void test_state_idle_handler(void)
{
    cellular_event_t event;
    cellular_modem_status_t modem_status = {0};
    size_t i;


    {
        unsigned int event_code[] = {
            connect_message,
            disconnect_message,
            connected_message,
            disconnected_message,
            update_message,
            ping_message,
            pong_message,
            query_connect,
            cellular_message_initialize,
            cellular_message_connect_timeout,
            cellular_message_disconnect_timeout,
            CELL_PING_RESPONSE_TMR_EVENT,
            cellular_message_connect
        };

        for (i = 0 ; i < ARRAY_SIZE(event_code); i++)
        {
            event.code = event_code[i];
            cellular_contexts[0].desired_state = true;
            cellular_contexts[0].had_sent_act_in = true;  // should be set when connect message is received
            cellular_contexts[0].current_state = state_idle;
            cellular_contexts[0].instance = 1;
            modem_status.active_sim = 1;
            modem_status.connect_status = cellular_disconnected;
            cellular_contexts[0].connecting_timer_running = true;
            cellular_contexts[0].modem_status = &modem_status;
            stop_timer_call_count = 0;
            start_timer_call_count = 0;
            system_command_call_count = 0;
            send_disconnected_event_to_interface_manager_call_count = 0;
            cellular_get_usb_ready_value = true;
            state_idle_handler(&cellular_contexts[0], &event);

            switch (event.code)
            {
                case connect_message:
                    CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_connect);
                    CU_ASSERT_TRUE(cellular_contexts[0].desired_state);
                    break;
                case disconnect_message:
                    CU_ASSERT_EQUAL(send_disconnected_event_to_interface_manager_call_count, 1);
                    break;
                case cellular_message_connect:
                    CU_ASSERT_EQUAL(start_timer_call_count, 1);
                    CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_wait_modem);
                    CU_ASSERT_TRUE(cellular_contexts[0].desired_state);
                    break;
                default:
                    break;
            }

        }
    }

}

void state_initialize_handler(cellular_context_t * const context, void * data);

void test_state_initialize_handler(void)
{
    cellular_event_t event;
    cellular_modem_status_t modem_status = {0};

    event.code = cellular_message_initialize;
    cellular_contexts[0].desired_state = false;
    cellular_contexts[0].had_sent_act_in = false;  // should be set when connect message is received
    cellular_contexts[0].current_state = state_initialize;
    cellular_contexts[0].instance = 1;
    modem_status.active_sim = 1;
    modem_status.connect_status = cellular_disconnected;
    cellular_contexts[0].connecting_timer_running = false;
    cellular_contexts[0].modem_status = &modem_status;
    stop_timer_call_count = 0;
    start_timer_call_count = 0;
    system_command_call_count = 0;
    send_disconnected_event_to_interface_manager_call_count = 0;
    cellular_get_usb_ready_value = true;
    state_initialize_handler(&cellular_contexts[0], &event);

    CU_ASSERT_EQUAL(cellular_contexts[0].current_state, state_idle);
    CU_ASSERT_FALSE(cellular_contexts[0].desired_state);
}

/* Test Suite setup and cleanup functions: */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/************* Test Runner Code goes here **************/

#define CU_TEST_INFO(test) { #test, test }

static CU_TestInfo tests[] =
{
    CU_TEST_INFO(test_state_disconnecting_handler),
    CU_TEST_INFO(test_state_disconnected_handler),
    CU_TEST_INFO(test_state_connected_handler),
    CU_TEST_INFO(test_state_connecting_handler),
    CU_TEST_INFO(test_state_wait_modem_handler),
    CU_TEST_INFO(test_state_idle_handler),
    CU_TEST_INFO(test_state_initialize_handler),
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo test_cellular_state_suites[] =
{
    { "cellular_state_test", init_suite, clean_suite, tests },
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

    if (CU_register_nsuites(1, test_cellular_state_suites) != CUE_SUCCESS)
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

/****************  Stubs   ***************************/


int cellular_get_usb_index(int const fd, size_t const index_3g, size_t * const index_usb)
{
    *index_usb = 0;
    return 0;
}

int send_monitor_event(cellular_context_t * const context, int event, void * * const info, size_t const info_size)
{
    return 0;
}
void start_timer(timer_id_st * const timer_id, unsigned int millisecs)
{
    start_timer_call_count++;
}
int stop_timer(timer_id_st * timer_id)
{
    stop_timer_call_count++;
    return 0;
}

tlr_log_t cellular_debug_priority;

void tlr_event_debug(tlr_log_t const priority, char * fmt, ...)
{

}
int cellular_get_active_sim(int const fd, size_t const index_3g, size_t * const active_sim)
{
    *active_sim = 1;
    return 0;
}
bool is_current_context_active_sim(cellular_context_t const * const context, size_t const active_sim)
{
    return (context->instance == active_sim);
}

size_t cellular_get_switch_sim_card(int const fd, size_t const index_3g)
{
    return 1;
}

