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
    #include "bridge_monitor.h"
    #include "bridge_monitor_init.h"
    #include "bridge_monitor_config.h"
    #include "bridge_monitor_config_commands.h"
    #include "bridge_monitor_timers.h"
    #include "bridge_monitor_event_handlers.h"
    #include "bridge_dns.h"
    #include "messages.h"

    /* include helper code header files */
}

bool bridge_connect_interface(size_t instance)
{
    mock()
        .actualCall("bridge_connect_interface")
        .withParameter("instance",(int)instance);
    if (mock().hasReturnValue())
    {
        return (bool)mock().returnValue().getIntValue();
    }
    return true;
}

bool bridge_disconnect_interface(size_t instance)
{
    mock()
        .actualCall("bridge_disconnect_interface")
        .withParameter("instance",(int)instance);

    if (mock().hasReturnValue())
    {
        return (bool)mock().returnValue().getIntValue();
    }
    return true;
}

bool bridge_config_add_interfaces(size_t const instance)
{
    mock()
        .actualCall("bridge_config_add_interfaces")
        .withParameter("instance",(int)instance);

    if (mock().hasReturnValue())
    {
        return (bool)mock().returnValue().getIntValue();
    }
    return true;
}

bool bridge_config_delete_interfaces(size_t const instance)
{
    mock()
        .actualCall("bridge_config_delete_interfaces")
        .withParameter("instance",(int)instance);

    if (mock().hasReturnValue())
    {
        return (bool)mock().returnValue().getIntValue();
    }
    return true;
}

char const * bridge_interface_name_get(void)
{
    mock()
        .actualCall("bridge_interface_name_get");

    if (mock().hasReturnValue())
    {
        return mock().stringReturnValue();
    }
    return "default";
}

int send_message(message_destination_t const destination, size_t instance, message_codes_t const message_code,
                 char const * const additional_info, char ** const response_buffer)
{
    mock()
        .actualCall("send_message")
        .withParameter("destination", destination)
        .withParameter("instance", (int)instance)
        .withParameter("message_code", (int)message_code)
        .withParameter("additional_info", additional_info);
    if (mock().hasReturnValue())
    {
        return mock().returnValue().getIntValue();
    }
    return 0;
}

bool bridge_interface_set_dhcp_client_off(size_t const instance)
{
    return false;
}

void bridge_refresh_dns_configuration(size_t const instance)
{
}

bool bridge_check_if_dhcp_client_needed(size_t const instance)
{
    return true;
}

/* mocked functions */
size_t bridge_get_num_contexts(void)
{
    mock()
        .actualCall("bridge_get_num_contexts");

    return (size_t)mock().returnValue().getIntValue();

}

bridge_context_st * bridge_get_context(size_t const instance)
{
    mock()
        .actualCall("bridge_get_context")
        .withParameter("instance", (int)instance);

    return (bridge_context_st *)mock().pointerReturnValue();
}

config_st * config_alloc(config_group_t group_id)
{
    mock()
        .actualCall("config_alloc")
        .withParameter("group_id", (int)group_id);

    if (mock().hasReturnValue())
    {
        return (config_st *)mock().returnValue().getPointerValue();
    }

    return NULL;
}

bool bridge_config_check_for_change(size_t const instance, config_st * const config, config_st * const new_config)
{
    return false;
}

int config_update_and_compare_with_previous(config_st * * const config,
                                            size_t const instance,
                                            config_compare_fn compare_fn,
                                            bool * const config_has_changed)
{
    mock()
        .actualCall("config_update_and_compare_with_previous")
        .withParameter("config", (void *)config)
        .withParameter("instance", (int)instance)
        .withParameter("compare_fn", (void *)compare_fn);

    return 0;
}

int send_pong_message(message_destination_t const destination, unsigned int message_id)
{
    mock()
        .actualCall("send_pong_message")
        .withParameter("destination", destination)
        .withParameter("message_id", message_id);

    return 0;
}

void bridge_ping_response_timer_expired(bridge_context_st * const context)
{
    mock()
        .actualCall("bridge_ping_response_timer_expired")
        .withParameter("context", (void *)context);
}

void bridge_ping_response_timer_start(bridge_context_st * const context, size_t millisecs)
{
    mock()
        .actualCall("bridge_ping_response_timer_start")
        .withParameter("context", (void *)context)
        .withParameter("millisecs", (int)millisecs);
}

/* tests */
TEST_GROUP(event_handlers)
{
    bridge_context_st context;

    void setup()
    {
        memset(&context, 0, sizeof context);
    }
    void teardown()
    {
        mock()
            .clear();
    }

};

TEST(event_handlers, initialise_event_calls_get_context_correct_number_of_times)
{
    size_t const instances = 2;
    size_t index;

    mock()
        .strictOrder();   /* all calls must occur in the expected order */
    mock()
        .expectOneCall("bridge_get_num_contexts")
        .andReturnValue((int)instances);
    for (index = 0; index < instances; index++)
    {
        size_t instance = index + 1;

        mock()
            .expectOneCall("bridge_get_context")
            .withParameter("instance", instance)
            .andReturnValue((void *)&context);
        mock()
            .expectOneCall("config_alloc")
            .withParameter("group_id", config_lan);
    }

    bridge_handle_initialise_event();

    mock()
        .checkExpectations();
}

TEST(event_handlers, initialise_event_config_assigned_to_the_context)
{
    size_t const instance = 1;
    int test_config;

    mock()
        .strictOrder();   /* all calls must occur in the expected order */
    mock()
        .expectOneCall("bridge_get_num_contexts")
        .andReturnValue((int)instance);

    mock()
        .expectOneCall("bridge_get_context")
        .withParameter("instance", instance)
        .andReturnValue((void *)&context);

    mock()
        .expectOneCall("config_alloc")
        .withParameter("group_id", config_lan)
        .andReturnValue((void *)&test_config);


    bridge_handle_initialise_event();

    POINTERS_EQUAL(&test_config, context.config);
    mock()
        .checkExpectations();
}

TEST(event_handlers, update_event_calls_config_update_and_compare_with_previous)
{
    context.instance = 1234;

    /* Not much to test here other than the update function gets called */
    mock()
        .expectOneCall("config_update_and_compare_with_previous")
        .withParameter("config", (void *)&context.config)
        .withParameter("instance", (int)context.instance)
        .withParameter("compare_fn", (void *)bridge_config_check_for_change);

    bridge_handle_update_event(&context);

    mock()
        .checkExpectations();
}

TEST(event_handlers, ping_response_timeout_event_calls_send_pong_if_response_info_not_null)
{
    ping_info_st * const ping_info = (ping_info_st *)calloc(1, sizeof *ping_info);

    ping_info->pong_destination = ethernet_monitor;
    ping_info->message_id = 5678;
    context.saved_ping_response_info = ping_info;

    mock()
        .expectOneCall("bridge_ping_response_timer_expired")
        .withParameter("context", (void *)&context);

    mock()
        .expectOneCall("send_pong_message")
        .withParameter("destination", ping_info->pong_destination)
        .withParameter("message_id", ping_info->message_id);

    bridge_handle_ping_response_timeout_event(&context);

    /*
     * Check that ping_response_info is now NULL.
     * XXX - How to test that free() was called? The leak detection doesn't seem to find such leaks,
     * although it whinges if mock().clear() isn't called.
     */
    POINTERS_EQUAL(NULL, context.saved_ping_response_info);

    mock()
        .checkExpectations();
}

TEST(event_handlers, ping_response_timeout_event_doesnt_call_send_pong_if_response_info_null)
{
    context.saved_ping_response_info = NULL;

    mock()
        .expectOneCall("bridge_ping_response_timer_expired")
        .withParameter("context", (void *)&context);

    bridge_handle_ping_response_timeout_event(&context);

    mock()
        .checkExpectations();
}

TEST(event_handlers, ping_message_event_response_if_please_respond_zero)
{
    bridge_monitor_event_t event;
    ping_info_st * const ping_info = (ping_info_st *)calloc(1, sizeof *ping_info);

    ping_info->pong_destination = ethernet_monitor;
    ping_info->message_id = 5678;
    ping_info->please_respond_time = 0;

    event.info = ping_info;

    mock()
        .expectOneCall("send_pong_message")
        .withParameter("destination", ping_info->pong_destination)
        .withParameter("message_id", ping_info->message_id);

    bridge_handle_ping_message_event(&context, &event);

    mock()
        .checkExpectations();
}

TEST(event_handlers, ping_message_event_start_timer_if_please_respond_non_zero)
{
    bridge_monitor_event_t event;
    ping_info_st * const ping_info = (ping_info_st *)calloc(1, sizeof *ping_info);

    ping_info->pong_destination = ethernet_monitor;
    ping_info->message_id = 5678;
    ping_info->please_respond_time = 1234;

    event.info = ping_info;

    mock()
        .expectOneCall("bridge_ping_response_timer_start")
        .withParameter("context", &context)
        .withParameter("millisecs", ping_info->please_respond_time);

    bridge_handle_ping_message_event(&context, &event);

    /* ensure the ping info has been removed from the event */
    POINTERS_EQUAL(NULL, event.info);

    mock()
        .checkExpectations();
}

TEST(event_handlers, bridge_handle_connect_desired_state_set_to_connected)
{
    context.desired_state = bridge_state_disconnected;

    mock().disable();

    bridge_handle_connect_event(&context);

    LONGS_EQUAL(bridge_state_connected, context.desired_state);

}

TEST(event_handlers, bridge_handle_connect_updates_ifman_if_already_connected)
{
    context.current_state = bridge_state_connected;
    context.instance = 1234;

    mock()
        .expectOneCall("bridge_interface_name_get")
        .andReturnValue((char const *)"bridge_name");

    mock()
        .expectOneCall("send_message")
        .withParameter("destination", interface_manager)
        .withParameter("message_code", connected_message)
        .withParameter("instance", (int)INTERFACE_MANAGER_INSTANCE)
        .withParameter("additional_info", "bridge_name 1234");

    bridge_handle_connect_event(&context);

    LONGS_EQUAL(bridge_state_connected, context.current_state);

    mock()
        .checkExpectations();

}

TEST(event_handlers, bridge_handle_connect_connects_iface_if_not_already_connected)
{
    context.current_state = bridge_state_disconnected;
    context.instance = 1234;

    mock().strictOrder();

    mock()
        .expectOneCall("bridge_connect_interface")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((int)true);

    mock()
        .expectOneCall("bridge_config_add_interfaces")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((int)true);

    mock()
        .expectOneCall("config_update_and_compare_with_previous")
        .withParameter("config", (void *)&context.config)
        .withParameter("instance", (int)context.instance)
        .withParameter("compare_fn", (void *)bridge_config_check_for_change);

    mock()
        .expectOneCall("bridge_interface_name_get")
        .andReturnValue((char const *)"bridge_name");

    mock()
        .expectOneCall("send_message")
        .withParameter("destination", interface_manager)
        .withParameter("message_code", connected_message)
        .withParameter("instance", (int)INTERFACE_MANAGER_INSTANCE)
        .withParameter("additional_info", "bridge_name 1234");

    bridge_handle_connect_event(&context);

    LONGS_EQUAL(bridge_state_connected, context.current_state);

    mock()
        .checkExpectations();

}

TEST(event_handlers, bridge_handle_connect_stays_disconnected_if_connect_fails)
{
    context.current_state = bridge_state_disconnected;
    context.instance = 1234;

    mock()
        .expectOneCall("bridge_connect_interface")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((int)false);


    bridge_handle_connect_event(&context);

    LONGS_EQUAL(bridge_state_disconnected, context.current_state);

    mock()
        .checkExpectations();

}

TEST(event_handlers, bridge_handle_connect_stays_disconnected_if_add_interfaces_fails)
{
    context.current_state = bridge_state_disconnected;
    context.instance = 1234;

    mock()
        .expectOneCall("bridge_connect_interface")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((int)true);

    mock()
        .expectOneCall("bridge_config_add_interfaces")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((int)false);


    bridge_handle_connect_event(&context);

    LONGS_EQUAL(bridge_state_disconnected, context.current_state);

    mock()
        .checkExpectations();

}

TEST(event_handlers, bridge_handle_disconnect_desired_state_set_to_disconnected)
{
    context.desired_state = bridge_state_connected;

    mock().disable();

    bridge_handle_disconnect_event(&context);

    LONGS_EQUAL(bridge_state_disconnected, context.desired_state);

}

TEST(event_handlers, bridge_handle_disconnect_updates_ifman_if_already_disconnected)
{
    context.current_state = bridge_state_disconnected;
    context.instance = 1234;

    mock()
        .expectOneCall("bridge_interface_name_get")
        .andReturnValue((char const *)"bridge_name");

    mock()
        .expectOneCall("send_message")
        .withParameter("destination", interface_manager)
        .withParameter("message_code", disconnected_message)
        .withParameter("instance", (int)INTERFACE_MANAGER_INSTANCE)
        .withParameter("additional_info", "bridge_name 1234");

    bridge_handle_disconnect_event(&context);

    LONGS_EQUAL(bridge_state_disconnected, context.current_state);

    mock()
        .checkExpectations();

}

TEST(event_handlers, bridge_handle_disconnect_disconnects_iface_if_not_already_disconnected)
{
    context.current_state = bridge_state_connected;
    context.instance = 1234;

    mock().strictOrder();
    mock()
        .expectOneCall("bridge_disconnect_interface")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((int)true);

    mock()
        .expectOneCall("bridge_config_delete_interfaces")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((int)true);

    mock()
        .expectOneCall("bridge_interface_name_get")
        .andReturnValue((char const *)"bridge_name");

    mock()
        .expectOneCall("send_message")
        .withParameter("destination", interface_manager)
        .withParameter("message_code", disconnected_message)
        .withParameter("instance", (int)INTERFACE_MANAGER_INSTANCE)
        .withParameter("additional_info", "bridge_name 1234");

    bridge_handle_disconnect_event(&context);

    LONGS_EQUAL(bridge_state_disconnected, context.current_state);

    mock()
        .checkExpectations();

}

TEST(event_handlers, bridge_handle_disconnect_ignores_failed_send_message)
{
    context.current_state = bridge_state_connected;
    context.instance = 1234;

    mock().strictOrder();

    mock()
        .expectOneCall("bridge_disconnect_interface")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((int)true);

    mock()
        .expectOneCall("bridge_config_delete_interfaces")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((int)true);

    mock()
        .expectOneCall("bridge_interface_name_get")
        .andReturnValue((char const *)"bridge_name");

    mock()
        .expectOneCall("send_message")
        .withParameter("destination", interface_manager)
        .withParameter("message_code", disconnected_message)
        .withParameter("instance", (int)INTERFACE_MANAGER_INSTANCE)
        .withParameter("additional_info", "bridge_name 1234")
        .andReturnValue(-1);

    bridge_handle_disconnect_event(&context);

    LONGS_EQUAL(bridge_state_disconnected, context.current_state);

    mock()
        .checkExpectations();

}

TEST(event_handlers, bridge_handle_disconnect_stays_connected_if_connect_fails)
{
    context.current_state = bridge_state_connected;
    context.instance = 1234;

    mock()
        .expectOneCall("bridge_disconnect_interface")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((int)false);


    bridge_handle_disconnect_event(&context);

    LONGS_EQUAL(bridge_state_connected, context.current_state);

    mock()
        .checkExpectations();

}

TEST(event_handlers, bridge_handle_disconnect_stays_connected_if_remove_interfaces_fails)
{
    context.current_state = bridge_state_connected;
    context.instance = 1234;

    mock()
        .expectOneCall("bridge_disconnect_interface")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((int)true);

    mock()
        .expectOneCall("bridge_config_delete_interfaces")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((int)false);

    bridge_handle_disconnect_event(&context);

    LONGS_EQUAL(bridge_state_connected, context.current_state);

    mock()
        .checkExpectations();

}


TEST(event_handlers, handle_connected_event_null_test)
{

    bridge_handle_connected_event(&context);
    mock()
        .checkExpectations();

}

TEST(event_handlers, handle_disconnected_event_null_test)
{

    bridge_handle_disconnected_event(&context);
    mock()
        .checkExpectations();

}

