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
    #include "bridge_monitor_timers.h"
    #include "bridge_monitor.h"
    #include "bridge_monitor_init.h"
    #include "bridge_monitor_config.h"
    #include "bridge_monitor_config_commands.h"
    #include "bridge_monitor_timers.h"
    #include "bridge_monitor_event_handlers.h"
    #include "messages.h"

    /* include helper code header files */
}

int send_event_to_bridge_monitor(size_t const instance, int const event_code, void * * const info, size_t const info_size)
{
    mock()
        .actualCall("send_event_to_bridge_monitor")
        .withParameter("instance",(int)instance)
        .withParameter("event_code",(int)event_code)
    	.withParameter("info", (void *)info)
		.withParameter("info_size", (int)info_size);
    if (mock().hasReturnValue())
    {
        return mock().returnValue().getIntValue();
    }
    return 0;
}

timer_id_st * create_timer(timer_context_st * const context, void (*callback)(void * data), void * data)
{
    mock()
        .actualCall("create_timer")
        .withParameter("context", context)
        .withParameter("callback",(void *)callback);
    if (mock().hasReturnValue())
    {
        return (timer_id_st *)mock().returnValue().getPointerValue();
    }
    return NULL;
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
    if (mock().hasReturnValue())
    {
        return mock().returnValue().getIntValue();
    }
    return 0;
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

TEST(timers, bridgemon_ping_response_timer_callback_calls_send_event_to_bridge_monitor)
{
    bridge_context_st context = { .instance = 1};

    mock()
        .expectOneCall("send_event_to_bridge_monitor")
        .withParameter("instance", context.instance)
        .withParameter("event_code", PING_RESPONSE_TIMER_EVENT)
        .withParameter("info", (void *)NULL)
        .withParameter("info_size", 0);

    ping_response_timer_callback(&context);

    mock().checkExpectations();
}

TEST(timers, bridge_timer_init_calls_create_timer)
{
         bridge_timer_info_st timer_info;

         mock()
             .expectOneCall("create_timer")
             .withParameter("context", (void *)NULL)
             .withParameter("callback", (void *)NULL);

         bridge_timer_init(&timer_info, NULL, NULL, NULL);

         CHECK_FALSE(timer_info.timer_running);

         mock().checkExpectations();
}

TEST(timers, bridge_timer_start_calls_start_timer)
{
    size_t millisecs=10;
    bridge_timer_info_st timer_info;

    timer_info.timer_running = false;
    timer_info.timer_id = NULL;

    mock()
        .expectOneCall("start_timer")
        .withParameter("timer_id", (void *)NULL)
        .withParameter("millisecs", millisecs);

    bridge_timer_start(&timer_info, millisecs);

    CHECK_TRUE(timer_info.timer_running);

    mock().checkExpectations();
}

TEST(timers, bridge_timer_stop_calls_stop_timer)
{
    bridge_timer_info_st timer_info;

    timer_info.timer_running = true;
    timer_info.timer_id = NULL;

    mock()
        .expectOneCall("stop_timer")
        .withParameter("timer_id", (void *)NULL);

    bridge_timer_stop(&timer_info);

    CHECK_FALSE(timer_info.timer_running);

    mock().checkExpectations();
}

TEST(timers, bridge_timer_stop_timer_not_running)
{
    bridge_timer_info_st timer_info;

    timer_info.timer_running = false;
    timer_info.timer_id = NULL;

    bridge_timer_stop(&timer_info);

    CHECK_FALSE(timer_info.timer_running);

    mock().checkExpectations();
}

