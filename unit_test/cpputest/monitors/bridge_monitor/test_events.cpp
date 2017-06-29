#include <CppUTest/TestHarness.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CppUTestExt/MockSupport.h"

/*
 * This is where we include the header files of the C code under test
 */
extern "C"
{
    /* include C header files of code under test */
    #include "bridge_monitor_event.h"

    /* include helper code header files */
    #include "events_helper.h"
}

typedef struct expectations_st
{
    size_t event_free_call_count;
    size_t bridge_handle_initialise_event_call_count;
    size_t bridge_handle_ping_response_timeout_ev;
    size_t bridge_handle_update_event_call_count;
    size_t bridge_handle_connect_event_call_count;
    size_t bridge_handle_disconnect_event_call_count;
    size_t bridge_handle_connected_event_call_count;
    size_t bridge_handle_disconnected_event_call_count;
    size_t bridge_handle_ping_message_event_call_count;
} expectations_st;


/* tests */
TEST_GROUP(event_handler)
{
    void setup()
    {
        event_spy_init();
    }
    void teardown()
    {
        event_spy_cleanup();
    }

    void check_expectations(expectations_st * const expectations)
    {
        LONGS_EQUAL(expectations->event_free_call_count, event_spy_get_event_free_call_count());
        LONGS_EQUAL(expectations->bridge_handle_initialise_event_call_count, event_spy_get_bridge_handle_initialise_event_call_count());
        LONGS_EQUAL(expectations->bridge_handle_ping_response_timeout_ev, event_spy_get_bridge_handle_ping_response_timeout_event_call_count());
        LONGS_EQUAL(expectations->bridge_handle_update_event_call_count, event_spy_get_bridge_handle_update_event_call_count());
        LONGS_EQUAL(expectations->bridge_handle_connect_event_call_count, event_spy_get_bridge_handle_connect_event_call_count());
        LONGS_EQUAL(expectations->bridge_handle_disconnect_event_call_count, event_spy_get_bridge_handle_disconnect_event_call_count());
        LONGS_EQUAL(expectations->bridge_handle_connected_event_call_count, event_spy_get_bridge_handle_connected_event_call_count());
        LONGS_EQUAL(expectations->bridge_handle_disconnected_event_call_count, event_spy_get_bridge_handle_disconnected_event_call_count());
        LONGS_EQUAL(expectations->bridge_handle_ping_message_event_call_count, event_spy_get_bridge_handle_ping_message_event_call_count());
        /* check that the event free function was called after the handlers */
        CHECK_FALSE(event_spy_free_called_before_handler());
    }
};

TEST(event_handler, null_context_calls_event_free)
{
    bridge_monitor_event_t event;
    expectations_st expectations =
    {
        .event_free_call_count = 1,
        .bridge_handle_initialise_event_call_count = 0,
        .bridge_handle_ping_response_timeout_ev = 0,
        .bridge_handle_update_event_call_count = 0,
        .bridge_handle_connect_event_call_count = 0,
        .bridge_handle_disconnect_event_call_count = 0,
        .bridge_handle_connected_event_call_count = 0,
        .bridge_handle_disconnected_event_call_count = 0,
        .bridge_handle_ping_message_event_call_count = 0
    };
    event_spy_set_bridge_context(NULL);

    bridgemon_event_handler(NULL, &event);

    check_expectations(&expectations);
}

TEST(event_handler, initialise_event)
{
    bridge_monitor_event_t event;
    bridge_context_st context;
    expectations_st expectations =
    {
        .event_free_call_count = 1,
        .bridge_handle_initialise_event_call_count = 1,
        .bridge_handle_ping_response_timeout_ev = 0,
        .bridge_handle_update_event_call_count = 0,
        .bridge_handle_connect_event_call_count = 0,
        .bridge_handle_disconnect_event_call_count = 0,
        .bridge_handle_connected_event_call_count = 0,
        .bridge_handle_disconnected_event_call_count = 0,
        .bridge_handle_ping_message_event_call_count = 0
    };

    event.code = INITIALISE_EVENT;
    event_spy_set_bridge_context(&context);

    bridgemon_event_handler(NULL, &event);

    check_expectations(&expectations);
}

TEST(event_handler, ping_response_timeout)
{
    bridge_monitor_event_t event;
    bridge_context_st context;
    expectations_st expectations =
    {
        .event_free_call_count = 1,
        .bridge_handle_initialise_event_call_count = 0,
        .bridge_handle_ping_response_timeout_ev = 1,
        .bridge_handle_update_event_call_count = 0,
        .bridge_handle_connect_event_call_count = 0,
        .bridge_handle_disconnect_event_call_count = 0,
        .bridge_handle_connected_event_call_count = 0,
        .bridge_handle_disconnected_event_call_count = 0,
        .bridge_handle_ping_message_event_call_count = 0
    };

    event.code = PING_RESPONSE_TIMER_EVENT;
    event_spy_set_bridge_context(&context);

    bridgemon_event_handler(NULL, &event);

    check_expectations(&expectations);
}

TEST(event_handler, update)
{
    bridge_monitor_event_t event;
    bridge_context_st context;
    expectations_st expectations =
    {
        .event_free_call_count = 1,
        .bridge_handle_initialise_event_call_count = 0,
        .bridge_handle_ping_response_timeout_ev = 0,
        .bridge_handle_update_event_call_count = 1,
        .bridge_handle_connect_event_call_count = 0,
        .bridge_handle_disconnect_event_call_count = 0,
        .bridge_handle_connected_event_call_count = 0,
        .bridge_handle_disconnected_event_call_count = 0,
        .bridge_handle_ping_message_event_call_count = 0
    };

    event.code = UPDATE_MESSAGE_EVENT;
    event_spy_set_bridge_context(&context);

    bridgemon_event_handler(NULL, &event);

    check_expectations(&expectations);
}

TEST(event_handler, connect)
{
    bridge_monitor_event_t event;
    bridge_context_st context;
    expectations_st expectations =
    {
        .event_free_call_count = 1,
        .bridge_handle_initialise_event_call_count = 0,
        .bridge_handle_ping_response_timeout_ev = 0,
        .bridge_handle_update_event_call_count = 0,
        .bridge_handle_connect_event_call_count = 1,
        .bridge_handle_disconnect_event_call_count = 0,
        .bridge_handle_connected_event_call_count = 0,
        .bridge_handle_disconnected_event_call_count = 0,
        .bridge_handle_ping_message_event_call_count = 0
    };

    event.code = CONNECT_MESSAGE_EVENT;
    event_spy_set_bridge_context(&context);

    bridgemon_event_handler(NULL, &event);

    check_expectations(&expectations);
}

TEST(event_handler, disconnect)
{
    bridge_monitor_event_t event;
    bridge_context_st context;
    expectations_st expectations =
    {
        .event_free_call_count = 1,
        .bridge_handle_initialise_event_call_count = 0,
        .bridge_handle_ping_response_timeout_ev = 0,
        .bridge_handle_update_event_call_count = 0,
        .bridge_handle_connect_event_call_count = 0,
        .bridge_handle_disconnect_event_call_count = 1,
        .bridge_handle_connected_event_call_count = 0,
        .bridge_handle_disconnected_event_call_count = 0,
        .bridge_handle_ping_message_event_call_count = 0
    };

    event.code = DISCONNECT_MESSAGE_EVENT;
    event_spy_set_bridge_context(&context);

    bridgemon_event_handler(NULL, &event);

    check_expectations(&expectations);
}

TEST(event_handler, connected)
{
    bridge_monitor_event_t event;
    bridge_context_st context;
    expectations_st expectations =
    {
        .event_free_call_count = 1,
        .bridge_handle_initialise_event_call_count = 0,
        .bridge_handle_ping_response_timeout_ev = 0,
        .bridge_handle_update_event_call_count = 0,
        .bridge_handle_connect_event_call_count = 0,
        .bridge_handle_disconnect_event_call_count = 0,
        .bridge_handle_connected_event_call_count = 1,
        .bridge_handle_disconnected_event_call_count = 0,
        .bridge_handle_ping_message_event_call_count = 0
    };

    event.code = CONNECTED_MESSAGE_EVENT;
    event_spy_set_bridge_context(&context);

    bridgemon_event_handler(NULL, &event);

    check_expectations(&expectations);
}

TEST(event_handler, disconnected)
{
    bridge_monitor_event_t event;
    bridge_context_st context;
    expectations_st expectations =
    {
        .event_free_call_count = 1,
        .bridge_handle_initialise_event_call_count = 0,
        .bridge_handle_ping_response_timeout_ev = 0,
        .bridge_handle_update_event_call_count = 0,
        .bridge_handle_connect_event_call_count = 0,
        .bridge_handle_disconnect_event_call_count = 0,
        .bridge_handle_connected_event_call_count = 0,
        .bridge_handle_disconnected_event_call_count = 1,
        .bridge_handle_ping_message_event_call_count = 0
    };

    event.code = DISCONNECTED_MESSAGE_EVENT;
    event_spy_set_bridge_context(&context);

    bridgemon_event_handler(NULL, &event);

    check_expectations(&expectations);
}

TEST(event_handler, ping_message)
{
    bridge_monitor_event_t event;
    bridge_context_st context;
    expectations_st expectations =
    {
        .event_free_call_count = 1,
        .bridge_handle_initialise_event_call_count = 0,
        .bridge_handle_ping_response_timeout_ev = 0,
        .bridge_handle_update_event_call_count = 0,
        .bridge_handle_connect_event_call_count = 0,
        .bridge_handle_disconnect_event_call_count = 0,
        .bridge_handle_connected_event_call_count = 0,
        .bridge_handle_disconnected_event_call_count = 0,
        .bridge_handle_ping_message_event_call_count = 1
    };

    event.code = PING_MESSAGE_EVENT;
    event_spy_set_bridge_context(&context);

    bridgemon_event_handler(NULL, &event);

    check_expectations(&expectations);
}

/*
 * XXX How to combine the common code in these tests while still able to reference the functions
 * defined in the group class
*/

TEST_GROUP(send_event_to_bridge_monitor)
{
    void setup()
    {
        event_spy_init();
    }
    void teardown()
    {
        event_spy_cleanup();
    }
};

TEST(send_event_to_bridge_monitor, failed_event_alloc_returns_error)
{
    int result;
    event_spy_set_event_alloc_event(NULL);

    result = send_event_to_bridge_monitor(0, 0, NULL, 0);

    LONGS_EQUAL(-1, result);
}

TEST(send_event_to_bridge_monitor, successful_alloc_calls_send_event_with_correct_pointer)
{
    bridge_monitor_event_t event;

    event_spy_set_event_alloc_event(&event);
    event_spy_set_send_event_to_event_manager_result(0);

    (void)send_event_to_bridge_monitor(0, 0, NULL, 0);

    POINTERS_EQUAL(&event, event_spy_get_event_sent_to_event_manager());
}

TEST(send_event_to_bridge_monitor, successful_send_message_does_not_call_free)
{
    bridge_monitor_event_t event;

    event_spy_set_event_alloc_event(&event);
    event_spy_set_send_event_to_event_manager_result(0);

    (void)send_event_to_bridge_monitor(0, 0, NULL, 0);

    LONGS_EQUAL(0, event_spy_get_event_free_call_count());
}

TEST(send_event_to_bridge_monitor, failed_send_message_calls_free)
{
    bridge_monitor_event_t event;

    event_spy_set_event_alloc_event(&event);
    event_spy_set_send_event_to_event_manager_result(-1);

    (void)send_event_to_bridge_monitor(0, 0, NULL, 0);

    LONGS_EQUAL(1, event_spy_get_event_free_call_count());
}

TEST(send_event_to_bridge_monitor, failed_send_message_calls_free_with_the_correct_event)
{
    bridge_monitor_event_t event;

    event_spy_set_event_alloc_event(&event);
    event_spy_set_send_event_to_event_manager_result(-1);

    (void)send_event_to_bridge_monitor(0, 0, NULL, 0);

    POINTERS_EQUAL(&event, event_spy_get_freed_event());
}

TEST(send_event_to_bridge_monitor, alloced_event_is_passed_to_send_event)
{
    bridge_monitor_event_t event;

    event_spy_set_event_alloc_event(&event);
    event_spy_set_send_event_to_event_manager_result(0);

    (void)send_event_to_bridge_monitor(0, 0, NULL, 0);

    POINTERS_EQUAL(&event, event_spy_get_event_sent_to_event_manager());
}

TEST_GROUP(init)
{
    void setup()
    {
        event_spy_init();
    }
    void teardown()
    {
        event_spy_cleanup();
    }
};

TEST(init, successful_call_to_new_event_manager_returns_true)
{
    bool result;
    int event_manager;

    event_spy_set_new_event_manager_result((event_manager_st *)&event_manager);

    result = bridge_event_init(0);

    CHECK_TRUE(result);
}

TEST(init, failed_call_to_new_event_manager_returns_false)
{
    bool result;

    event_spy_set_new_event_manager_result(NULL);

    result = bridge_event_init(0);

    CHECK_FALSE(result);
}

TEST(init, queue_size_passed_to_new_event_manager)
{
    int event_manager;
    size_t event_queue_size = 10;

    event_spy_set_new_event_manager_result((event_manager_st *)&event_manager);

    (void)bridge_event_init(event_queue_size);

    LONGS_EQUAL(event_queue_size, event_spy_get_event_queue_size_passed_to_new_event_manager());
}
