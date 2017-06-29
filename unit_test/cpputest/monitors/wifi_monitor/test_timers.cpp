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
    #include "wifi_monitor_timers.h"
    #include "wifi_monitor_event.h"
}

int send_event_to_wifi_monitor(size_t const instance, int const event_code, void * * const info, size_t const info_size)
{
    mock()
        .actualCall("send_event_to_wifi_monitor")
        .withParameter("instance",(int)instance)
        .withParameter("event_code",(int)event_code)
    	.withParameter("info", (void *)info)
		.withParameter("info_size", (int)info_size);

    return mock().returnIntValueOrDefault(0);
}

timer_id_st * create_timer(timer_context_st * const context, void (*callback)(void * data), void * data)
{
    mock()
        .actualCall("create_timer")
        .withParameter("context", context)
        .withParameter("arg", data)
        .withParameter("callback",(void *)callback);

    return (timer_id_st *)mock().returnPointerValueOrDefault(NULL);
}

void start_timer(timer_id_st * const timer_id, unsigned int millisecs)
{
    mock()
        .actualCall("start_timer")
        .withParameter("timer_id", (void *)timer_id)
        .withParameter("millisecs", millisecs);

}

int stop_timer(timer_id_st * timer_id)
{
    mock()
        .actualCall("stop_timer")
        .withParameter("timer_id", (void *)timer_id);
    return mock().returnIntValueOrDefault(0);
}


TEST_GROUP(timers)
{
    void setup()
    {

    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(timers, wifimon_ping_response_timer_callback_calls_send_event_to_wifi_monitor)
{
    wifi_context_st context = { .instance = 1};

    /* setup */
    mock()
        .expectOneCall("send_event_to_wifi_monitor")
        .withParameter("instance", context.instance)
        .withParameter("event_code", PING_RESPONSE_TIMER_EVENT)
        .withParameter("info", (void *)NULL)
        .withParameter("info_size", 0);

    /* perform test */
    ping_response_timer_callback(&context);

    /* check results */
    mock().checkExpectations();
}

TEST(timers, wifi_timers_init_calls_create_timer_for_ping_timer)
{
    wifi_context_st context;
    timer_context_st * timer_context;
    timer_id_st * expected_timer_id_value;

    /* setup */
    timer_context = (timer_context_st *)1234;
    expected_timer_id_value = (timer_id_st *)5678;

    mock()
        .expectOneCall("create_timer")
        .withParameter("context", (void *)timer_context)
        .withParameter("callback", (void *)ping_response_timer_callback)
        .withParameter("arg", &context)
        .andReturnValue((void *)expected_timer_id_value);

    /* perform test */
    wifi_timers_init(&context, timer_context);

    /* check result */
    POINTERS_EQUAL(expected_timer_id_value, context.ping_response_timer.timer_id);
    mock().checkExpectations();
}

TEST(timers, wifi_ping_response_timer_start_calls_start_timer_with_correct_parameters)
{
    size_t millisecs = 10;
    wifi_context_st context;

    /* setup */
    context.ping_response_timer.timer_id = (timer_id_st *)1234;

    mock()
        .expectOneCall("start_timer")
        .withParameter("timer_id", (void *)context.ping_response_timer.timer_id)
        .withParameter("millisecs", millisecs);

    /* perform test */
    wifi_ping_response_timer_start(&context, millisecs);

    /* check results */
    mock().checkExpectations();
}

TEST(timers, wifi_ping_response_timer_stop_calls_stop_timer_with_correct_parameters)
{
    wifi_context_st context;

    /* setup */
    context.ping_response_timer.timer_id = (timer_id_st *)1234;

    mock()
        .expectOneCall("stop_timer")
        .withParameter("timer_id", (void *)context.ping_response_timer.timer_id);

    /* perform test */
    wifi_ping_response_timer_stop(&context);

    /* check results */
    mock().checkExpectations();
}


