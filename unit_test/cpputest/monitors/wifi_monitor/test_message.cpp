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
    #include "wifi_monitor_message.h"
    #include "wifi_monitor_event.h"
    #include "wifi_monitor.h"
    #include "messages.h"

    /* include helper code header files */
}

int send_event_to_wifi_monitor(size_t const instance, int const event_code, void * * const info, size_t const info_size)
{
    mock()
        .actualCall("send_event_to_wifi_monitor")
        .withParameter("instance", (int)instance)
        .withParameter("event_code", (int)event_code)
    	.withParameter("info", info)
		.withParameter("info_size", (int)info_size);
    return mock().returnIntValueOrDefault(0);
}

message_thread_st * open_messaging_context(message_destination_t const destination, message_callback_fn const message_callback)
{
    mock()
        .actualCall("open_messaging_context")
        .withParameter("destination",(int)destination)
        .withParameter("message_callback",(void *)message_callback);

    return (message_thread_st *)mock().returnPointerValueOrDefault(NULL);
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

TEST(messaging, wifimon_message_callback_calls_send_event_to_wifi_monitor)
{
    int instance = 1;
    message_codes_t code = connect_message;
    void * * dummy_info = (void * *)1234;

    /* setup */
    mock()
        .expectOneCall("send_event_to_wifi_monitor")
        .withParameter("instance", instance)
        .withParameter("event_code", (int)code)
        .withParameter("info", dummy_info)
        .withParameter("info_size", 1);

    /* perform test */
    wifi_monitor_message_callback(0, instance, code, dummy_info, 1);

    /* check results */
    mock().checkExpectations();
}

TEST(messaging, wifi_messaging_setup_calls_open_messaging_context)
{
    int test_message;
    bool actual_result;

    /* setup */
    mock()
        .expectOneCall("open_messaging_context")
        .withParameter("destination", (int)wifi_monitor)
        .withParameter("message_callback", (void *)wifi_monitor_message_callback)
		.andReturnValue((void *)&test_message);

    /* perform test */
    actual_result = wifi_messaging_setup(wifi_interface);

    /* check results */
    CHECK_TRUE(actual_result);

    mock().checkExpectations();
}

TEST(messaging, wifi_messaging_setup_returns_false_on_null_messaging_context)
{
    bool actual_result;

    /* setup */
    mock()
        .expectOneCall("open_messaging_context")
        .withParameter("destination", (int)wifi_monitor)
        .withParameter("message_callback", (void *)wifi_monitor_message_callback)
		.andReturnValue((void *)NULL);

    /* perform test */
    actual_result = wifi_messaging_setup(wifi_interface);

    /* check results */
    CHECK_FALSE(actual_result);

    mock().checkExpectations();
}


