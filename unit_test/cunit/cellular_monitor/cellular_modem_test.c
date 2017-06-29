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
//#define     USE_BASIC_STYLE     1
//#define     USE_AUTOMATED_STYLE   0
//#define     USE_CONSOLE_STYLE     0

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

#include "tlr_common.h"
#include "config_def.h"
#include "cellular_monitor.h"
#include "cellular_modem.h"
#include "cellular_status.h"
#include "monitor_debug.h"

#include "interfaces.h"
#include "event_queue.h"
#include "tlr_print.h"

//#include "csman.h"
//#include "csid/csid_3g.h"

char const * interface_name = "Cellular";
size_t interface_instance_max = 1;

cellular_context_t cellularContextData[2];
cellular_context_t * cellular_contexts = cellularContextData;

char const * cellular_state_strings[] =
{
    CELLULAR_FOREACH_STATE(GENERATE_STRING)
};

char const * cellular_message_strings[] =
{
    CELLULAR_FOREACH_MESSAGE(GENERATE_STRING)
};

size_t const cellular_state_strings_count = ARRAY_SIZE(cellular_state_strings);
size_t const cellular_message_strings_count = ARRAY_SIZE(cellular_message_strings);

bool init_3g_initialized = true;

int send_event_to_event_manager_value = 0;
cellular_connect_status_t cellular_get_connection_status_value;
int send_event_to_event_manager_event_code = 0;
/******* Test Cases ******/

config_mgr_group_t config_groups[config_cellular+1];

config_mgr_group_table_t config_table = {
        .count = sizeof config_groups / sizeof config_groups[0],
        .groups = &config_groups[0],
};


int send_monitor_event(cellular_context_t * const context, int event, void * * const info, size_t const info_size);

void test_send_monitor_event()
{
    int status;
    cellular_context_t context;

    context.instance = 1;

    send_event_to_event_manager_value = -1;
    status = send_monitor_event(&context, 1001, NULL, 0);
    CU_ASSERT_EQUAL(status, -1);

    send_event_to_event_manager_value = 0;
    status = send_monitor_event(&context, 1001, NULL, 0);
    CU_ASSERT_EQUAL(status, 0);


}

void test_modem_status_handler(void)
{
    cellular_context_t context;
    cellular_modem_status_t modem_status;


    size_t i;
    size_t n;
    cellular_state_t context_status[] = {
        state_initialize,
        state_idle,
        state_connecting,
        state_connected,
        state_disconnecting,
        state_disconnected,
        state_wait_modem
    };

    cellular_connect_status_t cellular_status[] = {
     cellular_disconnected,
     cellular_connecting,
     cellular_connected,
     cellular_disconnecting
    };

    for (i=0; i < ARRAY_SIZE(context_status); i++)
    {
        context.instance = 1;
        context.current_state = context_status[i];
        context.modem_status = &modem_status;
        context.connecting_timer_running = true;
        context.desired_state = true;
        modem_status.active_sim = 1;

        send_event_to_event_manager_value = 0;
        for (n=0; n < ARRAY_SIZE(cellular_status); n++)
        {
            cellular_get_connection_status_value = cellular_status[n];

            modem_status_handler(&context, modem_status.active_sim);

            switch (context.current_state)
            {
            case state_initialize:
                break;
            case state_connect:
                {

                    switch (cellular_get_connection_status_value)
                    {
                    case cellular_connected:
                        break;
                    case cellular_disconnected:
                        CU_ASSERT_EQUAL(send_event_to_event_manager_event_code, cellular_message_connect);
                        break;
                    case cellular_disconnecting:
                    case cellular_connecting:
                        break;
                    default:
                        break;
                    }

                }
                break;
            case state_disconnect:
                {
                    switch (cellular_get_connection_status_value)
                    {
                    case cellular_disconnected:
                        CU_ASSERT_EQUAL(send_event_to_event_manager_event_code, cellular_message_disconnect);
                        break;
                    case cellular_disconnecting:
                        //break;
                    case cellular_connected:
                    case cellular_connecting:
                        CU_ASSERT_EQUAL(send_event_to_event_manager_event_code, cellular_message_disconnect);
                        break;
                    }

                }
                break;
            case state_connected:
                break;
            default:
                break;
            }
        }
    }

}

/* Test Suite setup and cleanup functions: */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/************* Test Runner Code goes here **************/

#define CU_TEST_INFO(test) { #test, test }

static CU_TestInfo tests[] =
{
    CU_TEST_INFO(test_send_monitor_event),
    CU_TEST_INFO(test_modem_status_handler),
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo test_cellular_modem_suites[] =
{
    { "cellular_modem_test", init_suite, clean_suite, tests },
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

    if (CU_register_nsuites(1, test_cellular_modem_suites) != CUE_SUCCESS)
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
int send_event_to_event_manager(event_manager_st * const event_manager, void * const event)
{
    cellular_event_t * handler_event = event;

    send_event_to_event_manager_event_code = handler_event->code;

    if (send_event_to_event_manager_value == 0)
    {
        free(event);
    }
    return send_event_to_event_manager_value;
}

int send_message_to_cellular_monitor(size_t const instance, int const event, void * * const info, size_t const info_size)
{
    send_event_to_event_manager_event_code = event;
    return 0;
}

cellular_connect_status_t cellular_get_connection_status(int const fd, size_t const index_3g)
{
    return cellular_get_connection_status_value;
}

int asprintf(char **strp, const char *fmt, ...)
{
    return 0;
}
int send_message(message_destination_t const destination, size_t instance, message_codes_t const message_code,
                 char const * const additional_info, char ** const response_buffer)
{
    return 0;
}

bool cellular_get_usb_ready(int const fd, size_t const index_3g)
{
    return true;
}

tlr_log_t cellular_debug_priority;

void tlr_event_debug(tlr_log_t const priority, char * fmt, ...)
{

}
int cellular_read_status(int const fd, size_t const index_3g, config_state_cellular_t const sid, void * value, size_t const value_size)
{
    return 0;
}