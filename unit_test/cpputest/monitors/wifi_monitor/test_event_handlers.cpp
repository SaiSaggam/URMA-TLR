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
    #include "wifi_monitor_event_handlers.h"
    #include "wifi_monitor.h"
    #include "wifi_monitor_init.h"
    #include "wifi_monitor_config.h"
    #include "wifi_monitor_timers.h"
    #include "tlr_common.h"
    #include "messages.h"
    #include "wifi_radio.h"
    #include "config_utils.h"
    #include "tlr_print.h"

}

char const * wifi_interface_name_get(void)
{
    mock().actualCall("wifi_interface_name_get");

    return mock().returnStringValueOrDefault("default");
}

bool wifi_radio_connect_24(size_t const instance)
{
    mock()
        .actualCall("wifi_radio_connect")
        .withParameter("instance",(int)instance);

    return (bool)mock().returnIntValueOrDefault(true);
}

bool wifi_radio_disconnect_24(size_t const instance)
{
    mock()
        .actualCall("wifi_radio_disconnect")
        .withParameter("instance",(int)instance);

    return (bool)mock().returnIntValueOrDefault(true);
}

/* mocked functions */
size_t wifi_get_num_contexts(void)
{
    mock()
        .actualCall("wifi_get_num_contexts");

    return (size_t)mock().returnLongIntValueOrDefault(0);

}

wifi_context_st * wifi_get_context(size_t const instance)
{
    mock()
        .actualCall("wifi_get_context")
        .withParameter("instance", (int)instance);

    return (wifi_context_st *)mock().pointerReturnValue();
}

bool wifi_config_check_for_change(size_t const instance, wifi_common_elements_t * const config, wifi_common_elements_t * const new_config)
{
    mock()
        .actualCall("wifi_config_check_for_change")
        .withParameter("instance", (int)instance)
        .withParameter("config", (void *)config)
        .withParameter("new_config", (void *)new_config);

    return (bool)mock().returnUnsignedIntValueOrDefault(false);

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

void wifi_ping_response_timer_start(wifi_context_st * const context, size_t millisecs)
{
    mock()
        .actualCall("wifi_ping_response_timer_start")
        .withParameter("context", (void *)context)
        .withParameter("millisecs", (int)millisecs);
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

    return mock().returnIntValueOrDefault(0);
}

void wifi_config_has_changed(wifi_context_st * const context, wifi_common_elements_t * const new_config)
{
    mock()
        .actualCall("wifi_config_has_changed")
        .withParameter("context", (void *)context)
        .withParameter("new_config", (void *)new_config);
}

wifi_common_elements_t * wifi_get_configuration_24(size_t const instance)
{
    mock()
        .actualCall("wifi_get_configuration_24")
        .withParameter("instance", (int)instance);

    return (wifi_common_elements_t *)mock().returnPointerValueOrDefault(NULL);
}

void wifi_free_configuration_24(wifi_common_elements_t * const wifi_config)
{
    mock()
        .actualCall("wifi_free_configuration_24")
        .withParameter("wifi_config", (void *)wifi_config);

}

void tlr_event_log(tlr_log_t const priority, char const * const process_name, char const * const format, ...)
{
}

/* tests */
TEST_GROUP(event_handlers)
{
    wifi_context_st context;

    void setup()
    {
        memset(&context, 0, sizeof context);
    }

    void teardown()
    {
        mock().clear();
    }
};

TEST(event_handlers, update_event_calls_wifi_config_has_changed_if_configs_differ)
{
    /* setup */
    context.instance = 1234;
    context.config = (wifi_common_elements_t *)8765;
    wifi_common_elements_t * const new_config = (wifi_common_elements_t *)5678;

    mock()
        .expectOneCall("wifi_get_configuration_24")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((void *)new_config);

    mock()
        .expectOneCall("wifi_config_check_for_change")
        .withParameter("instance", (int)context.instance)
        .withParameter("config", (void *)context.config)
        .withParameter("new_config", (void *)new_config)
        .andReturnValue(true);  /* Indicate that config has changed by returning true. */

    mock()
        .expectOneCall("wifi_free_configuration_24")
        .withParameter("wifi_config", (void *)context.config);

    mock()
        .expectOneCall("wifi_config_has_changed")
        .withParameter("context", (void *)&context)
        .withParameter("new_config", (void *)new_config);

    /* perform test */
    wifi_handle_update_event(&context);

    /* check results */
    /* Ensure that the new config has been assigned to the context. */
    POINTERS_EQUAL(new_config, context.config);
    mock().checkExpectations();
}

TEST(event_handlers, update_event_frees_new_config_if_configs_match)
{
    /* setup */
    wifi_common_elements_t * const original_config = (wifi_common_elements_t *)8765;
    context.instance = 1234;
    context.config = original_config;
    wifi_common_elements_t * const new_config = (wifi_common_elements_t *)5678;

    mock()
        .expectOneCall("wifi_get_configuration_24")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((void *)new_config);

    mock()
        .expectOneCall("wifi_config_check_for_change")
        .withParameter("instance", (int)context.instance)
        .withParameter("config", (void *)context.config)
        .withParameter("new_config", (void *)new_config)
        .andReturnValue(false);  /* Indicate that config has changed by returning true. */

    mock()
        .expectOneCall("wifi_free_configuration_24")
        .withParameter("wifi_config", (void *)new_config);

    /* perform test */
    wifi_handle_update_event(&context);

    /* check results */
    /* Ensure that the original config has beeen retained */
    POINTERS_EQUAL(original_config, context.config);
    mock().checkExpectations();
}

TEST(event_handlers, ping_response_timeout_event_calls_send_pong_if_response_info_not_null)
{
    ping_info_st * const ping_info = (ping_info_st *)calloc(1, sizeof *ping_info);

    /* setup */
    ping_info->pong_destination = ethernet_monitor;
    ping_info->message_id = 5678;
    context.saved_ping_response_info = ping_info;

    mock()
        .expectOneCall("send_pong_message")
        .withParameter("destination", ping_info->pong_destination)
        .withParameter("message_id", ping_info->message_id);

    /* perform test */
    wifi_handle_ping_response_timeout_event(&context);

    /* check results */
    /*
     * Check that ping_response_info is now NULL.
     * XXX - How to test that free() was called? The leak detection doesn't seem to find such leaks,
     * although it whinges if mock().clear() isn't called.
     */
    POINTERS_EQUAL(NULL, context.saved_ping_response_info);

    mock().checkExpectations();
}

TEST(event_handlers, ping_response_timeout_event_doesnt_call_send_pong_if_response_info_null)
{
    /* setup */
    context.saved_ping_response_info = NULL;

    /* perform test */
    wifi_handle_ping_response_timeout_event(&context);

    /* check results */
    mock().checkExpectations();
}

TEST(event_handlers, ping_message_event_response_if_please_respond_zero)
{
    wifi_monitor_event_t event;
    ping_info_st * const ping_info = (ping_info_st *)calloc(1, sizeof *ping_info);

    /* setup */
    ping_info->pong_destination = ethernet_monitor;
    ping_info->message_id = 5678;
    ping_info->please_respond_time = 0;

    event.info = ping_info;

    mock()
        .expectOneCall("send_pong_message")
        .withParameter("destination", ping_info->pong_destination)
        .withParameter("message_id", ping_info->message_id);

    /* perform test */
    wifi_handle_ping_message_event(&context, &event);

    /* check results */
    mock().checkExpectations();
}

TEST(event_handlers, ping_message_event_start_timer_if_please_respond_non_zero)
{
    wifi_monitor_event_t event;
    ping_info_st * const ping_info = (ping_info_st *)calloc(1, sizeof *ping_info);

    /* setup */
    ping_info->pong_destination = ethernet_monitor;
    ping_info->message_id = 5678;
    ping_info->please_respond_time = 1234;

    event.info = ping_info;

    mock()
        .expectOneCall("wifi_ping_response_timer_start")
        .withParameter("context", &context)
        .withParameter("millisecs", ping_info->please_respond_time);

    /* perform test */
    wifi_handle_ping_message_event(&context, &event);

    /* check results */
    /* ensure the ping info has been removed from the event */
    POINTERS_EQUAL(NULL, event.info);
    mock().checkExpectations();
}

TEST(event_handlers, wifi_handle_connect_desired_state_set_to_connected)
{
    /* setup */
    context.desired_state = wifi_state_disconnected;
    /* Set current state to connected. Cpputest gets upset if we use the disconnected state.
     */
    context.current_state = wifi_state_connected;

    mock().disable();

    /* perform test */
    wifi_handle_connect_event(&context);

    /* check results */
    LONGS_EQUAL(wifi_state_connected, context.desired_state);

}

TEST(event_handlers, wifi_handle_connect_updates_ifman_if_already_connected)
{
    /* setup */
    context.current_state = wifi_state_connected;
    context.instance = 1234;

    mock()
        .expectOneCall("wifi_interface_name_get")
        .andReturnValue((char const *)"wifi_name");

    mock()
        .expectOneCall("send_message")
        .withParameter("destination", interface_manager)
        .withParameter("message_code", connected_message)
        .withParameter("instance", (int)INTERFACE_MANAGER_INSTANCE)
        .withParameter("additional_info", "wifi_name 1234");

    /* perform test */
    wifi_handle_connect_event(&context);

    /* check results */
    LONGS_EQUAL(wifi_state_connected, context.current_state);
    mock().checkExpectations();
}

TEST(event_handlers, wifi_handle_connect_connects_iface_if_not_already_connected)
{
    /* setup */
    context.current_state = wifi_state_disconnected;
    context.instance = 1234;

    mock().strictOrder();
    mock()
        .expectOneCall("wifi_radio_connect")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((int)true);
    mock()
        .expectOneCall("wifi_interface_name_get")
        .andReturnValue((char const *)"wifi_name");
    mock()
        .expectOneCall("send_message")
        .withParameter("destination", interface_manager)
        .withParameter("message_code", connected_message)
        .withParameter("instance", (int)INTERFACE_MANAGER_INSTANCE)
        .withParameter("additional_info", "wifi_name 1234");

    /* perform test */
    wifi_handle_connect_event(&context);

    /* check results */
    LONGS_EQUAL(wifi_state_connected, context.current_state);
    mock()
        .checkExpectations();
}

TEST(event_handlers, wifi_handle_connect_stays_disconnected_if_connect_fails)
{
    /* setup */
    context.current_state = wifi_state_disconnected;
    context.instance = 1234;

    mock()
        .expectOneCall("wifi_radio_connect")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((int)false);

    /* perform test */
    wifi_handle_connect_event(&context);

    /* check results */
    LONGS_EQUAL(wifi_state_disconnected, context.current_state);
    mock().checkExpectations();

}

TEST(event_handlers, wifi_handle_disconnect_desired_state_set_to_disconnected)
{
    /* setup */
    context.desired_state = wifi_state_connected;

    mock().disable();

    /* perform test */
    wifi_handle_disconnect_event(&context);

    /* check results */
    LONGS_EQUAL(wifi_state_disconnected, context.desired_state);

}

TEST(event_handlers, wifi_handle_disconnect_updates_ifman_if_already_disconnected)
{
    /* setup */
    context.current_state = wifi_state_disconnected;
    context.instance = 1234;

    mock()
        .expectOneCall("wifi_interface_name_get")
        .andReturnValue((char const *)"wifi_name");

    mock()
        .expectOneCall("send_message")
        .withParameter("destination", interface_manager)
        .withParameter("message_code", disconnected_message)
        .withParameter("instance", (int)INTERFACE_MANAGER_INSTANCE)
        .withParameter("additional_info", "wifi_name 1234");

    /* perform test */
    wifi_handle_disconnect_event(&context);

    /* check results */
    LONGS_EQUAL(wifi_state_disconnected, context.current_state);

    mock()
        .checkExpectations();

}

TEST(event_handlers, wifi_handle_disconnect_disconnects_iface_if_not_already_disconnected)
{
    /* setup */
    context.current_state = wifi_state_connected;
    context.instance = 1234;

    mock().strictOrder();
    mock()
        .expectOneCall("wifi_radio_disconnect")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((int)true);
    mock()
        .expectOneCall("wifi_interface_name_get")
        .andReturnValue((char const *)"wifi_name");
    mock()
        .expectOneCall("send_message")
        .withParameter("destination", interface_manager)
        .withParameter("message_code", disconnected_message)
        .withParameter("instance", (int)INTERFACE_MANAGER_INSTANCE)
        .withParameter("additional_info", "wifi_name 1234");

    /* perform test */
    wifi_handle_disconnect_event(&context);

    /* check results */
    LONGS_EQUAL(wifi_state_disconnected, context.current_state);

    mock()
        .checkExpectations();

}

TEST(event_handlers, wifi_handle_disconnect_ignores_failed_send_message)
{
    /* setup */
    context.current_state = wifi_state_connected;
    context.instance = 1234;

    mock().strictOrder();

    mock()
        .expectOneCall("wifi_radio_disconnect")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((int)true);
    mock()
        .expectOneCall("wifi_interface_name_get")
        .andReturnValue((char const *)"wifi_name");

    mock()
        .expectOneCall("send_message")
        .withParameter("destination", interface_manager)
        .withParameter("message_code", disconnected_message)
        .withParameter("instance", (int)INTERFACE_MANAGER_INSTANCE)
        .withParameter("additional_info", "wifi_name 1234")
        .andReturnValue(-1);

    /* perform test */
    wifi_handle_disconnect_event(&context);

    /* check results */
    LONGS_EQUAL(wifi_state_disconnected, context.current_state);

    mock().checkExpectations();

}

TEST(event_handlers, wifi_handle_disconnect_goes_disconnected_if_disconnect_fails)
{
    /* setup */
    context.current_state = wifi_state_connected;
    context.instance = 1234;

    /* XXX - Should the monitor be ignoring failed disconnect attemtps? */
    mock()
        .expectOneCall("wifi_radio_disconnect")
        .withParameter("instance", (int)context.instance)
        .andReturnValue((int)false);
    mock()
        .expectOneCall("wifi_interface_name_get")
        .andReturnValue((char const *)"wifi_name");

    mock()
        .expectOneCall("send_message")
        .withParameter("destination", interface_manager)
        .withParameter("message_code", disconnected_message)
        .withParameter("instance", (int)INTERFACE_MANAGER_INSTANCE)
        .withParameter("additional_info", "wifi_name 1234")
        .andReturnValue(0);

    /* perform test */
    wifi_handle_disconnect_event(&context);

    /* check results */
    LONGS_EQUAL(wifi_state_disconnected, context.current_state);

    mock().checkExpectations();

}

TEST(event_handlers, handle_connected_event_updates_current_state_and_updates_if_manager_if_disconnected)
{
    /* setup */
    context.current_state = wifi_state_disconnected;
    context.instance = 1234;

    mock()
        .expectOneCall("wifi_interface_name_get")
        .andReturnValue((char const *)"wifi_name");

    mock()
        .expectOneCall("send_message")
        .withParameter("destination", interface_manager)
        .withParameter("message_code", connected_message)
        .withParameter("instance", (int)INTERFACE_MANAGER_INSTANCE)
        .withParameter("additional_info", "wifi_name 1234");

    /* perform test */
    wifi_handle_connected_event(&context);

    /* check results. */
    LONGS_EQUAL(wifi_state_connected, context.current_state);
    mock().checkExpectations();
}

TEST(event_handlers, handle_connected_event_ignored_if_already_connected)
{
    /* setup */
    context.current_state = wifi_state_connected;
    context.instance = 1234;

    /* perform test */
    wifi_handle_connected_event(&context);

    /* check results. */
    LONGS_EQUAL(wifi_state_connected, context.current_state);
    mock().checkExpectations();
}

TEST(event_handlers, handle_disconnected_event_updates_current_state_and_updates_if_manager_if_connected)
{
    /* setup */
    context.current_state = wifi_state_connected;
    context.instance = 1234;

    mock()
        .expectOneCall("wifi_interface_name_get")
        .andReturnValue((char const *)"wifi_name");

    mock()
        .expectOneCall("send_message")
        .withParameter("destination", interface_manager)
        .withParameter("message_code", disconnected_message)
        .withParameter("instance", (int)INTERFACE_MANAGER_INSTANCE)
        .withParameter("additional_info", "wifi_name 1234");

    /* perform test */
    wifi_handle_disconnected_event(&context);

    /* check results. */
    LONGS_EQUAL(wifi_state_disconnected, context.current_state);
    mock().checkExpectations();
}

TEST(event_handlers, handle_disconnected_event_ignored_if_already_disconnected)
{
    /* setup */
    context.current_state = wifi_state_disconnected;
    context.instance = 1234;

    /* perform test */
    wifi_handle_disconnected_event(&context);

    /* check results. */
    LONGS_EQUAL(wifi_state_disconnected, context.current_state);
    mock().checkExpectations();
}

