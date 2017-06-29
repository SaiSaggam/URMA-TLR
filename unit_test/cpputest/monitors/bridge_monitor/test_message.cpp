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
    #include "bridge_monitor_message.h"
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
    	.withParameter("info", info)
		.withParameter("info_size", (int)info_size);
    if (mock().hasReturnValue())
    {
        return mock().returnValue().getIntValue();
    }
    return 0;
}

message_thread_st * open_messaging_context(message_destination_t const destination, message_callback_fn const message_callback)
{
    mock()
        .actualCall("open_messaging_context")
        .withParameter("destination",(int)destination)
        .withParameter("message_callback",(void *)message_callback);
    if (mock().hasReturnValue())
    {
        return (message_thread_st *)mock().returnValue().getPointerValue();
    }
    return NULL;
}

TEST_GROUP(messaging)
{
    void setup()
    {

    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(messaging, bridgemon_message_callback_calls_send_event_to_bridge_monitor)
{
    int instance = 1;
    message_codes_t code = connect_message;

    mock()
        .expectOneCall("send_event_to_bridge_monitor")
        .withParameter("instance", (int)instance)
        .withParameter("event_code", (int)code)
        .withParameter("info", (void *)NULL)
        .withParameter("info_size", 1);

    bridgemon_message_callback(0, instance, code, NULL, 1);

    mock().checkExpectations();
}

TEST(messaging, bridge_messaging_setup_calls_open_messaging_context_specifying_bridge_monitor)
{
    int test_message;
    bool actual_result;

    mock()
        .expectOneCall("open_messaging_context")
        .withParameter("destination", (int)bridge_monitor)
        .withParameter("message_callback", (void *)bridgemon_message_callback)
		.andReturnValue((void *)&test_message);

    actual_result = bridge_messaging_setup();

    CHECK_TRUE(actual_result);

    mock().checkExpectations();
}

TEST(messaging, bridge_messaging_setup_returns_false_on_null_context)
{
    bool actual_result;

    mock()
        .expectOneCall("open_messaging_context")
        .withParameter("destination", (int)bridge_monitor)
        .withParameter("message_callback", (void *)bridgemon_message_callback)
		.andReturnValue((void *)NULL);

    actual_result = bridge_messaging_setup();

    CHECK_FALSE(actual_result);

    mock().checkExpectations();
}







