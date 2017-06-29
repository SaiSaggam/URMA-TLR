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
    #include "wifi_monitor.h"
    #include "wifi_monitor_event.h"
    #include "wifi_monitor_event_alloc.h"
    #include "wifi_monitor_event_handlers.h"
    #include "event_manager.h"

}

event_manager_st * new_event_manager(void const * const user_context, size_t event_queue_size, event_handler event_handler_cb)
{
    mock()
        .actualCall("new_event_manager")
        .withParameter("user_context", (void *)user_context)
        .withParameter("event_queue_size", event_queue_size);

    return (event_manager_st *)mock().returnPointerValueOrDefault(NULL);
}

wifi_monitor_event_t * wifi_monitor_event_alloc(int const code, size_t const instance, void * * const info, size_t const info_size)
{
    mock()
        .actualCall("wifi_monitor_event_alloc")
        .withParameter("code", code)
        .withParameter("instance", (int)instance)
        .withParameter("info", info)
        .withParameter("info_size", (int)info_size);

    return (wifi_monitor_event_t *)mock().returnPointerValueOrDefault(NULL);
}

int send_event_to_event_manager(event_manager_st * const event_manager, void * const event)
{
    mock()
        .actualCall("send_event_to_event_manager")
        .withParameter("event", (void *)event);

    return mock().returnIntValueOrDefault(-1);
}

void wifi_monitor_event_free(wifi_monitor_event_t * const event)
{
    mock()
        .actualCall("wifi_monitor_event_free")
        .withParameter("event", (void *)event);
}

void wifi_handle_ping_response_timeout_event(wifi_context_st * const context)
{
    mock()
        .actualCall("wifi_handle_ping_response_timeout_event")
        .withParameter("context", (void *)context);
}

void wifi_handle_update_event(wifi_context_st * const context)
{
    mock()
        .actualCall("wifi_handle_update_event")
        .withParameter("context", (void *)context);
}

void wifi_handle_connect_event(wifi_context_st * const context)
{
    mock()
        .actualCall("wifi_handle_connect_event")
        .withParameter("context", (void *)context);
}

void wifi_handle_disconnect_event(wifi_context_st * const context)
{
    mock()
        .actualCall("wifi_handle_disconnect_event")
        .withParameter("context", (void *)context);
}

void wifi_handle_connected_event(wifi_context_st * const context)
{
    mock()
        .actualCall("wifi_handle_connected_event")
        .withParameter("context", (void *)context);
}

void wifi_handle_disconnected_event(wifi_context_st * const context)
{
    mock()
        .actualCall("wifi_handle_disconnected_event")
        .withParameter("context", (void *)context);
}

void wifi_handle_ping_message_event(wifi_context_st * const context, wifi_monitor_event_t * const event)
{
    mock()
        .actualCall("wifi_handle_ping_message_event")
        .withParameter("context", (void *)context)
        .withParameter("event", (void *)event);
}

wifi_context_st * wifi_get_context(size_t const instance)
{
    mock()
        .actualCall("wifi_get_context")
        .withParameter("instance", (int)instance);

    return (wifi_context_st *)mock().pointerReturnValue();
}

/* tests */
TEST_GROUP(send_event)
{

    void setup()
    {
    }

    void teardown()
    {
        mock().clear();
    }
};

TEST(send_event, failed_event_alloc_returns_failure)
{
    wifi_monitor_event_t * event;
    size_t instance;
    int event_code;
    void * info;
    size_t info_size;
    int result;
    int expected_result;

    /* setup */
    instance = 1;
    event_code = 2;
    info = (void *)3;
    info_size = 4;
    event = NULL;
    expected_result = -1;

    mock()
        .expectOneCall("wifi_monitor_event_alloc")
        .withParameter("code", event_code)
        .withParameter("instance", (int)instance)
        .withParameter("info", (void * *)&info)
        .withParameter("info_size", info_size)
        .andReturnValue((void *)event);
    mock()
        .expectOneCall("wifi_monitor_event_free")
        .withParameter("event", (void *)event);


    /* perform test */
    result = send_event_to_wifi_monitor(instance, event_code, &info, (int)info_size);

    /* check results */
    CHECK_EQUAL(expected_result, result);
    mock().checkExpectations();
}

TEST(send_event, failed_send_event_returns_failure)
{
    wifi_monitor_event_t * event;
    size_t instance;
    int event_code;
    void * info;
    size_t info_size;
    int result;
    int expected_result;

    /* setup */
    instance = 1;
    event_code = 2;
    info = (void *)3;
    info_size = 4;
    event = (wifi_monitor_event_t *)1234;
    expected_result = -1;

    mock()
        .expectOneCall("wifi_monitor_event_alloc")
        .withParameter("code", event_code)
        .withParameter("instance", (int)instance)
        .withParameter("info", (void * *)&info)
        .withParameter("info_size", info_size)
        .andReturnValue((void *)event);

    mock()
        .expectOneCall("send_event_to_event_manager")
        .withParameter("event", (void *)event)
        .andReturnValue(-1);
    mock()
        .expectOneCall("wifi_monitor_event_free")
        .withParameter("event", (void *)event);

    /* perform test */
    result = send_event_to_wifi_monitor(instance, event_code, &info, (int)info_size);

    /* check results */
    CHECK_EQUAL(expected_result, result);
    mock().checkExpectations();
}

TEST(send_event, successful_call_to_send_event_returns_success)
{
    wifi_monitor_event_t * event;
    size_t instance;
    int event_code;
    void * info;
    size_t info_size;
    int result;
    int expected_result;

    /* setup */
    instance = 1;
    event_code = 2;
    info = (void *)3;
    info_size = 4;
    event = (wifi_monitor_event_t *)1234;
    expected_result = 0;

    mock()
        .expectOneCall("wifi_monitor_event_alloc")
        .withParameter("code", event_code)
        .withParameter("instance", (int)instance)
        .withParameter("info", (void * *)&info)
        .withParameter("info_size", info_size)
        .andReturnValue((void *)event);

    mock()
        .expectOneCall("send_event_to_event_manager")
        .withParameter("event", (void *)event)
        .andReturnValue(0);

    /* perform test */
    result = send_event_to_wifi_monitor(instance, event_code, &info, (int)info_size);

    /* check results */
    CHECK_EQUAL(expected_result, result);
    mock().checkExpectations();
}

TEST_GROUP(event_init)
{
    void setup()
    {
    }

    void teardown()
    {
        mock().clear();
    }
};

TEST(event_init, failed_init_returns_false)
{
    size_t event_queue_size;

    /* setup */
    event_queue_size = 1234;
    mock()
        .expectOneCall("new_event_manager")
        .withParameter("user_context", (void *)NULL)
        .withParameter("event_queue_size", (int)event_queue_size)
        .andReturnValue((void *)NULL);

    /* perform test */
    wifi_monitor_event_init(event_queue_size);

    /* check results */
    mock().checkExpectations();
}

TEST(event_init, successful_init_returns_true)
{
    size_t event_queue_size;
    void * some_non_null_pointer = (void *)1234;

    /* setup */
    event_queue_size = 1234;
    mock()
        .expectOneCall("new_event_manager")
        .withParameter("user_context", (void *)NULL)
        .withParameter("event_queue_size", (int)event_queue_size)
        .andReturnValue(some_non_null_pointer);

    /* perform test */
    wifi_monitor_event_init(event_queue_size);

    /* check results */
    mock().checkExpectations();
}

TEST_GROUP(event_handler)
{
    wifi_monitor_event_t event;
    wifi_context_st context;

    void setup()
    {
        memset((void *)&event, 0, sizeof event);
        memset((void *)&context, 0, sizeof context);
    }

    void teardown()
    {
        mock().clear();
    }
};

TEST(event_handler, instance_passed_to_get_context_and_quits_when_null_context_returned)
{
    /* setup */
    event.instance = 1234;
    mock()
        .expectOneCall("wifi_get_context")
        .withParameter("instance", event.instance)
        .andReturnValue((void *)NULL);
    mock()
        .expectOneCall("wifi_monitor_event_free")
        .withParameter("event", &event);

    /* perform test */
    wifi_monitor_event_handler(NULL, &event);

    /* check results */
    mock().checkExpectations();
}

TEST(event_handler, ping_timeout_handler_called_with_ping_timeout_event)
{
    /* setup */
    event.instance = 1234;
    event.code = PING_RESPONSE_TIMER_EVENT;

    mock().strictOrder();
    mock()
        .expectOneCall("wifi_get_context")
        .withParameter("instance", event.instance)
        .andReturnValue((void *)&context);
    mock()
        .expectOneCall("wifi_handle_ping_response_timeout_event")
        .withParameter("context", &context);

    mock()
        .expectOneCall("wifi_monitor_event_free")
        .withParameter("event", &event);

    /* perform test */
    wifi_monitor_event_handler(NULL, &event);

    /* check results */
    mock().checkExpectations();
}

TEST(event_handler, update_handler_called_with_update_event)
{
    /* setup */
    event.instance = 1234;
    event.code = UPDATE_MESSAGE_EVENT;

    mock().strictOrder();
    mock()
        .expectOneCall("wifi_get_context")
        .withParameter("instance", event.instance)
        .andReturnValue((void *)&context);
    mock()
        .expectOneCall("wifi_handle_update_event")
        .withParameter("context", &context);

    mock()
        .expectOneCall("wifi_monitor_event_free")
        .withParameter("event", &event);

    /* perform test */
    wifi_monitor_event_handler(NULL, &event);

    /* check results */
    mock().checkExpectations();
}

TEST(event_handler, connected_handler_called_with_connected_event)
{
    /* setup */
    event.instance = 1234;
    event.code = CONNECTED_MESSAGE_EVENT;

    mock().strictOrder();
    mock()
        .expectOneCall("wifi_get_context")
        .withParameter("instance", event.instance)
        .andReturnValue((void *)&context);
    mock()
        .expectOneCall("wifi_handle_connected_event")
        .withParameter("context", &context);

    mock()
        .expectOneCall("wifi_monitor_event_free")
        .withParameter("event", &event);

    /* perform test */
    wifi_monitor_event_handler(NULL, &event);

    /* check results */
    mock().checkExpectations();
}

TEST(event_handler, disconnected_handler_called_with_disconnected_event)
{
    /* setup */
    event.instance = 1234;
    event.code = DISCONNECTED_MESSAGE_EVENT;

    mock().strictOrder();
    mock()
        .expectOneCall("wifi_get_context")
        .withParameter("instance", event.instance)
        .andReturnValue((void *)&context);
    mock()
        .expectOneCall("wifi_handle_disconnected_event")
        .withParameter("context", &context);

    mock()
        .expectOneCall("wifi_monitor_event_free")
        .withParameter("event", &event);

    /* perform test */
    wifi_monitor_event_handler(NULL, &event);

    /* check results */
    mock().checkExpectations();
}

TEST(event_handler, connect_handler_called_with_connect_event)
{
    /* setup */
    event.instance = 1234;
    event.code = CONNECT_MESSAGE_EVENT;

    mock().strictOrder();
    mock()
        .expectOneCall("wifi_get_context")
        .withParameter("instance", event.instance)
        .andReturnValue((void *)&context);
    mock()
        .expectOneCall("wifi_handle_connect_event")
        .withParameter("context", &context);

    mock()
        .expectOneCall("wifi_monitor_event_free")
        .withParameter("event", &event);

    /* perform test */
    wifi_monitor_event_handler(NULL, &event);

    /* check results */
    mock().checkExpectations();
}

TEST(event_handler, disconnect_handler_called_with_disconnect_event)
{
    /* setup */
    event.instance = 1234;
    event.code = DISCONNECT_MESSAGE_EVENT;

    mock().strictOrder();
    mock()
        .expectOneCall("wifi_get_context")
        .withParameter("instance", event.instance)
        .andReturnValue((void *)&context);
    mock()
        .expectOneCall("wifi_handle_disconnect_event")
        .withParameter("context", &context);

    mock()
        .expectOneCall("wifi_monitor_event_free")
        .withParameter("event", &event);

    /* perform test */
    wifi_monitor_event_handler(NULL, &event);

    /* check results */
    mock().checkExpectations();
}

TEST(event_handler, ping_handler_called_with_ping_event)
{
    /* setup */
    event.instance = 1234;
    event.code = PING_MESSAGE_EVENT;

    mock().strictOrder();
    mock()
        .expectOneCall("wifi_get_context")
        .withParameter("instance", event.instance)
        .andReturnValue((void *)&context);
    mock()
        .expectOneCall("wifi_handle_ping_message_event")
        .withParameter("context", &context)
        .withParameter("event", &event);

    mock()
        .expectOneCall("wifi_monitor_event_free")
        .withParameter("event", &event);

    /* perform test */
    wifi_monitor_event_handler(NULL, &event);

    /* check results */
    mock().checkExpectations();
}

