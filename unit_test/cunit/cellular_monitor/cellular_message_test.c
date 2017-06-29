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

int send_message_to_cellular_monitor_value = 0;
int send_message_to_cellular_monitor_event_code;

int send_event_to_event_manager_event_code;
int send_event_to_event_manager_value = 0;

char const * interface_name = "Cellular";
size_t interface_instance_max = 1;

cellular_context_t cellularContextData[2];
cellular_context_t * cellular_contexts = cellularContextData;

/******* Test Cases ******/

config_mgr_group_t config_groups[config_cellular+1];

config_mgr_group_table_t config_table = {
        .count = sizeof config_groups / sizeof config_groups[0],
        .groups = &config_groups[0],
};


int monitor_message_callback(int fd, size_t const instance, message_codes_t const message_code, void * * const info, size_t const info_size);

void test_monitor_message_callback()
{
    int status;
    size_t i;
    cellular_modem_status_t modem_status = {0};

    message_codes_t event_code[] = {
        connect_message,
        disconnect_message,
        connected_message,
        disconnected_message,
        update_message,
        ping_message,
        pong_message,
    };

    status =  monitor_message_callback(0, interface_instance_max+1, query_connect, NULL, 0);
    CU_ASSERT_EQUAL(status, -1);

    cellular_contexts[0].instance = 1;
    cellular_contexts[0].modem_status = &modem_status;

    status =  monitor_message_callback(0, interface_instance_max, query_connect, NULL, 0);
    CU_ASSERT_EQUAL(status, 0);

    for (i=0; i < ARRAY_SIZE(event_code); i++)
    {
        status =  monitor_message_callback(0, interface_instance_max, event_code[i], NULL, 0);
        CU_ASSERT_EQUAL(status, 0);
        CU_ASSERT_EQUAL(send_message_to_cellular_monitor_event_code, event_code[i]);
    }
}


/* Test Suite setup and cleanup functions: */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/************* Test Runner Code goes here **************/

#define CU_TEST_INFO(test) { #test, test }

static CU_TestInfo tests[] =
{
    CU_TEST_INFO(test_monitor_message_callback),
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo test_cellular_message_suites[] =
{
    { "cellular_message_test", init_suite, clean_suite, tests },
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

    if (CU_register_nsuites(1, test_cellular_message_suites) != CUE_SUCCESS)
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

int send_message_to_cellular_monitor(size_t const instance, int const event, void * * const info, size_t const info_size)
{
    send_message_to_cellular_monitor_event_code = event;

    return send_message_to_cellular_monitor_value;
}

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

tlr_log_t cellular_debug_priority;

void tlr_event_debug(tlr_log_t const priority, char * fmt, ...)
{

}

//cellular_connect_status_t cellular_get_connection_status(size_t const index_3g)
//{
    //return cellular_get_connection_status_value;
//}

unsigned int tlr_get_group_loglevel(size_t const group_id)
{
    return 0;
}

void show_cellular_debug(size_t const instance)
{

}
bool is_current_context_active_sim(cellular_context_t const * const context, size_t const active_sim)
{
    return (context->instance == active_sim);
}
