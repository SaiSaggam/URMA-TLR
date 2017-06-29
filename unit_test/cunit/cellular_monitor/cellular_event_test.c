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
#define _GNU_SOURCE
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
#include <stdarg.h>


#include "config_def.h"
#include "cellular_monitor.h"
#include "cellular_status.h"
#include "config_mgr.h"

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/******* Test Cases ******/
void test_process_event_status_in_state_initialize(void)
{
    cellular_context_t context;
    cellular_connect_status_t connect_status

    context->instance      = 0;
    context->modem_status  = 0;
    context->current_state = 0;

    cellular_get_usb_ready_return = true;

    connect_status = cellular_connected;
    process_event_status_in_state_connecting(context, connect_status);
    CU_ASSERT_EQUAL(context->desired_state, true);
    CU_ASSERT_EQUAL(context->current_state, state_connected);

    connect_status = cellular_connecting;
    process_event_status_in_state_connecting(context, connect_status);
    CU_ASSERT_EQUAL(context->status_desired, true);
    CU_ASSERT_EQUAL(context->current_state, cellular_connecting);

    connect_status = cellular_disconnecting;
    context->status_desired = true;
    process_event_status_in_state_connecting(context, connect_status);
    CU_ASSERT_EQUAL(context->status_desired, false);

    connect_status = cellular_disconnected;
    context->status_desired = true;
    process_event_status_in_state_connecting(context, connect_status);
    CU_ASSERT_EQUAL(context->status_desired, false);
}





/************* Test Runner Code goes here **************/

int main( void )
{
    int result;
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if ( CUE_SUCCESS != CU_initialize_registry() )
    {
        result = CU_get_error();
        goto error;
    }

    /* add a suite to the registry */
    pSuite = CU_add_suite( "cellular_restart_test_suite", init_suite, clean_suite );
    if ( NULL == pSuite )
    {
        result = CU_get_error();
        goto error;
    }

    /* add the tests to the suite */
    if ( (CU_add_test(pSuite, "test_process_event_status_in_state_initialize", test_process_event_status_in_state_initialize) == NULL))
    {
        result = CU_get_error();
        goto error;
    }

#if USE_BASIC_STYLE
    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
	if (CU_basic_run_tests() != CUE_SUCCESS)
	{
        result = CU_get_error();
        goto error;
	}
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");

#elif USE_AUTOMATED_STYLE
    // Run all tests using the automated interface
    CU_automated_run_tests();
    CU_list_tests_to_file();

#elif USE_CONSOLE_STYLE
    // Run all tests using the console interface
    CU_console_run_tests();
#endif

    result = CU_get_number_of_failures();

error:
	CU_cleanup_registry();
	return result;
}



/****************  Stubs   ***************************/


char * cellular_get_interface_name(size_t const index_3g)
{
    return "ppp0";
}


uint32_t cellular_get_fail_restart_count(size_t index_3g)
{
    return 0;
}

int set_timer(timer_id_t const id, size_t const interval_in_seconds, timer_flag_t const flag,
              timer_cb_t const callback, void * const user_context)
{
    return 0;
}

int cellular_set_active_sim(size_t const index_3g, size_t const active_sim)
{
    return 0;
}
