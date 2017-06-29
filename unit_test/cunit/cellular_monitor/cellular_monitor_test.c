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

#define CELLULAR_INTERFACE_NAME "cellular"
bool init_3g_initialized = false;
unsigned int cellular_alloc_event_count = 0;

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
cellular_state_t state_handler_call;
int send_pong_message_call_count = 0;

struct event_queue_st
{
    void * queue;
};

struct timer_id_st
{
    timer_t timer_id;
    void (*callback)(void * data);
    void * data;
};

struct timer_context_st
{
    pthread_t timer_thread_id;
    int next_unique_id;
    sigset_t original_signal_mask;
    pthread_mutex_t   unique_id_lock;

    TAILQ_HEAD(, active_timer_st) active_list;
    sem_t active_list_sem;
};

struct event_manager_st
{
    event_handler event_handler;
    event_queue_st * event_queue;
    void const * user_context;

    pthread_t user_thread_id; /* the worker thread ID. This is the thread the user code runs in */
};


/******* Test Cases ******/

config_mgr_group_t config_groups[config_cellular+1];

config_mgr_group_table_t config_table = {
        .count = sizeof config_groups / sizeof config_groups[0],
        .groups = &config_groups[0],
};

timer_context_st timer_context;

extern int start_cellular_monitor(void);

char * my_interface_name;
char const * interface_get_name_from_type(interfaces_t type)
{
    return my_interface_name;
}

timer_id_st * timer;
timer_id_st * create_timer(timer_context_st * const context, void (*callback)(void * data), void * data)
{
    return timer;
}

event_manager_st * event_manager;
event_manager_st * new_event_manager(void const * const user_context, size_t event_queue_size, event_handler event_handler_cb)
{
    return event_manager;
}

int free_event_count = 0;
void free_event(cellular_event_t * handler_event)
{
    free_event_count++;

}

static int send_monitor_event_return = 0;
int send_monitor_event(cellular_context_t * const context, int event, void * * const info, size_t const info_size)
{
    return send_monitor_event_return;
}

int cellular_get_active_sim_value = 1;
int cellular_get_active_sim(int const fd, size_t const index_3g, size_t * const active_sim)
{
    *active_sim = cellular_get_active_sim_value;

    return 0;
}


void modem_status_handler(cellular_context_t * const context, size_t const active_sim)
{
    int status;
    event_manager_st active_event_manager;

    event_manager = NULL;
    status = start_cellular_monitor();
    CU_ASSERT_EQUAL(status, -1);

    event_manager = &active_event_manager;

}

void test_start_cellular_monitor()
{
    int status;
    timer_id_st active_timer;
    event_manager_st active_event_manager;

    event_manager = NULL;
    status = start_cellular_monitor();
    CU_ASSERT_EQUAL(status, -1);

    cellular_contexts = NULL;
    status = start_cellular_monitor();
    CU_ASSERT_EQUAL(status, -1);

    config_groups[config_cellular].instances = 1;
    my_interface_name = CELLULAR_INTERFACE_NAME;
    timer = &active_timer;
    event_manager = &active_event_manager;
    status = init_cellular_monitor(&timer_context);
    CU_ASSERT_EQUAL(status, 0);

    timer = &active_timer;
    event_manager = &active_event_manager;
    status = start_cellular_monitor();
    CU_ASSERT_EQUAL(status, 0);
}

extern int send_message_to_cellular_monitor(size_t const instance, int const event, void * * const info, size_t const info_size);

void test_send_message_to_cellular_monitor(void)
{
    int status;

    /* return error */
    send_monitor_event_return = -1;
    status = send_message_to_cellular_monitor(1, update_message, NULL, 0);
    CU_ASSERT_EQUAL(status, -1);

    /* invalid instance */
    send_monitor_event_return = 0;
    status = send_message_to_cellular_monitor(0, update_message, NULL, 0);
    CU_ASSERT_EQUAL(status, -1);

    send_monitor_event_return = 0;
    status = send_message_to_cellular_monitor(3, update_message, NULL, 0);
    CU_ASSERT_EQUAL(status, -1);

}

extern int init_cellular_monitor(timer_context_st * const timer_context);

void test_init_cellular_monitor(void)
{
    int status;
    timer_id_st active_timer;
    //timer_context_st timer_context;
    event_manager_st active_event_manager;

    config_groups[config_cellular].instances = 0;
    status = init_cellular_monitor(&timer_context);
    CU_ASSERT_EQUAL(status, -1);

    config_groups[config_cellular].instances = 1;
    my_interface_name = NULL;
    status = init_cellular_monitor(&timer_context);
    CU_ASSERT_EQUAL(status, -1);

    config_groups[config_cellular].instances = 1;
    my_interface_name = CELLULAR_INTERFACE_NAME;
    timer = NULL;
    status = init_cellular_monitor(&timer_context);
    CU_ASSERT_EQUAL(status, -1);

    timer = &active_timer;
    event_manager = &active_event_manager;
    status = init_cellular_monitor(&timer_context);
    CU_ASSERT_EQUAL(status, 0);
}

extern void monitor_event_handler(void * const user_context, void * const data);

void test_monitor_event_handler(void)
{
    int status;
    timer_id_st active_timer;
    cellular_event_t event;
    //timer_context_st timer_context;

    timer = &active_timer;
    status = init_cellular_monitor(&timer_context);
    CU_ASSERT_EQUAL(status, 0);
#if 0

    {
        ping_info_st ping_info;
        event.code = ping_message;
        event.info = &ping_info;
        monitor_event_handler(NULL, &event);
    }
#endif

    {
        /* send_pong_message_call should not be called */
        event.code = CELL_PING_RESPONSE_TMR_EVENT;
        event.instance = 1;
        event.info = NULL;
        event.info_size = 0;
        cellular_contexts[0].p_response_info = NULL;
        send_pong_message_call_count = 0;
        monitor_event_handler(NULL, &event);
        CU_ASSERT_EQUAL(send_pong_message_call_count, 0);
    }

    {
        /* send_pong_message_call should be called */
        event.code = CELL_PING_RESPONSE_TMR_EVENT;
        event.instance = 1;
        event.info = NULL;
        event.info_size = 0;
        cellular_contexts[0].p_response_info = malloc(sizeof *cellular_contexts[0].p_response_info);
        send_pong_message_call_count = 0;
        monitor_event_handler(NULL, &event);
        CU_ASSERT_EQUAL(send_pong_message_call_count, 1);

    }

    {
        cellular_state_t state;
        cellular_message_code_t message;

        for (message = cellular_message_initialize; message <= cellular_message_disconnect_timeout; message++)
        {
            for (state = state_initialize; state <= state_wait_modem; state++)
            {
                //printf("monitor_event_handler: %s message in %s state\n",
                  //      cellular_message_strings[message], cellular_state_strings[state]);
                event.code = message;
                event.instance = 1;
                event.info = NULL;
                event.info_size = 0;
                cellular_contexts[0].current_state = state;
                cellular_contexts[0].instance = 1;
                monitor_event_handler(NULL, &event);
                CU_ASSERT_EQUAL(state_handler_call, state);
            }
        }
    }

    {
        cellular_state_t state;
        for (state = state_initialize; state <= state_wait_modem; state++)
        {
            //printf("monitor_event_handler: update_message in %s state\n", cellular_state_strings[state]);
            event.code = update_message;
            event.instance = 1;
            event.info = NULL;
            event.info_size = 0;
            cellular_contexts[0].current_state = state;
            cellular_contexts[0].instance = 1;
            monitor_event_handler(NULL, &event);
            CU_ASSERT_EQUAL(state_handler_call, state);
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
    CU_TEST_INFO(test_init_cellular_monitor),
    CU_TEST_INFO(test_start_cellular_monitor),
    CU_TEST_INFO(test_send_message_to_cellular_monitor),
    CU_TEST_INFO(test_monitor_event_handler),
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo test_cellular_monitor_suites[] =
{
    { "cellular_monitor_test", init_suite, clean_suite, tests },
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

/****************  Stubs   ***************************/

#if 0
int open_csman(const char * config, int flags)
{
    return 0;
}

void tlr_update_table_tag(config_tag_t const tag_id, unsigned int value)
{

}
int close_csman(int fd)
{
    return 0;
}
platform_type_t tlr_get_platform(void)
{
    return platform_lr54;

}
#endif

void const * tlr_get_table(config_mgr_table_type_t const type)
{
    return &config_table;
}

void start_timer(timer_id_st * const timer_id, unsigned int millisecs)
{

}

void state_initialize_handler(cellular_context_t * const context, void * const data)
{
    state_handler_call = state_initialize;
}
void state_idle_handler(cellular_context_t * const context, void * const data)
{
    state_handler_call = state_idle;

}
void state_connect_handler(cellular_context_t * const context, void * const data)
{
    state_handler_call = state_connect;
}
void state_connecting_handler(cellular_context_t * const context, void * const data)
{
    state_handler_call = state_connecting;
}
void state_connected_handler(cellular_context_t * const context, void * const data)
{
    state_handler_call = state_connected;

}
void state_disconnecting_handler(cellular_context_t * const context, void * const data)
{
    state_handler_call = state_disconnecting;

}
void state_disconnect_handler(cellular_context_t * const context, void * const data)
{
    state_handler_call = state_disconnect;
}

void state_disconnected_handler(cellular_context_t * const context, void * const data)
{
    state_handler_call = state_disconnected;

}

void state_wait_modem_handler(cellular_context_t * const context, void * const data)
{
    state_handler_call = state_wait_modem;

}
int destroy_timer(timer_id_st * timer_id)
{
    return 0;
}

char const * get_global_message_name_from_code(int code)
{
    return NULL;
}

int send_pong_message(message_destination_t const destination, unsigned int message_id)
{
    return ++send_pong_message_call_count;
}

cellular_connect_status_t cellular_get_connection_status(int const fd, size_t const index_3g)
{
    return 0;
}

int cellular_get_usb_index(int const fd, size_t const index_3g, size_t * const index_usb)
{
    return 0;
}

void tlr_event_debug(tlr_log_t const priority, char * fmt, ...)
{

}
void cellular_update_leds(const int active_sim, cellular_context_t * const context)
{
    return;
}
void cellular_all_leds_off(void)
{

}
int cellular_open_status(void)
{
    return 0;
}
void cellular_close_status(int fd)
{

}

