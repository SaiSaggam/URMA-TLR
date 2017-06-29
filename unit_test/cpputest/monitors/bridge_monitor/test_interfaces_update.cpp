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
    #include "bridge_interfaces_update.h"
    #include "bridge_monitor_config.h"
    #include "bridge_monitor.h"
    #include "bridge_monitor_init.h"
    #include "bridge_monitor_config.h"
    #include "bridge_monitor_config_commands.h"
    #include "bridge_monitor_timers.h"
    #include "bridge_monitor_event_handlers.h"
    #include "bridge_interfaces.h"
    #include "messages.h"
    #include "config_def.h"
    #include "config_utils.h"
    #include "interfaces.h"
    #include "tlr_common.h"

    /* include helper code header files */
}

bridge_context_st * bridge_get_context(size_t const instance)
{
    mock()
        .actualCall("bridge_get_context")
        .withParameter("instance", (int)instance);
    if (mock().hasReturnValue())
    {
        return (bridge_context_st *)mock().returnValue().getPointerValue();
    }
    return NULL;
}

interfaces_t interface_get_type_from_name(char const * const name)
{
    mock()
        .actualCall("interface_get_type_from_name")
        .withParameter("name", name);

    if (mock().hasReturnValue())
    {
        return (interfaces_t)mock().returnValue().getIntValue();
    }
    return bridge_interface;
}

int send_message_to_interface_monitor(interfaces_t interface_type, size_t instance, message_codes_t const message_code, char const * const str)
{
    mock()
        .actualCall("send_message_to_interface_monitor")
        .withParameter("interface_type", (int)interface_type)
        .withParameter("instance", (int)instance)
        .withParameter("message_code", (int)message_code)
        .withParameter("str", str);

    if (mock().hasReturnValue())
    {
        return mock().returnValue().getIntValue();
    }
    return 0;
}

bool bridge_interface_add_child_interfaces(size_t const instance, char const * const interface_names)
{
    mock()
        .actualCall("bridge_interface_add_interfaces")
        .withParameter("instance", (int)instance)
        .withParameter("interface_names", interface_names);

    if (mock().hasReturnValue())
    {
        return mock().returnValue().getIntValue();
    }
    return true;
}

bool bridge_interface_delete_child_interfaces(size_t const instance, char const * const interface_names)
{
    mock()
        .actualCall("bridge_interface_delete_interfaces")
        .withParameter("instance", (int)instance)
        .withParameter("interface_names", interface_names);

    if (mock().hasReturnValue())
    {
        return mock().returnValue().getIntValue();
    }
    return true;
}


TEST_GROUP(interface_change)
{
    void setup()
    {

    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(interface_change, bridge_config_add_interfaces_calls_bridge_interface_add_interfaces)
{
    size_t instance = 1;
    bool actual_value;
    config_st config;
    element_t element;
    bridge_context_st bridge_context;
    char new_interface_name[] = "eth2";

    element.value = new_interface_name;
    config.count = config_lan_mask+1;
    config.elements = &element;

    bridge_context.current_state = bridge_state_connected;
    bridge_context.config = &config;

    mock()
    	.expectOneCall("bridge_get_context")
        .withParameter("instance", (int)instance)
        .andReturnValue(&bridge_context);

    mock()
         .expectOneCall("bridge_interface_add_interfaces")
         .withParameter("instance", (int)instance)
         .withParameter("interface_names", new_interface_name);

    mock()
         .expectOneCall("send_message_to_interface_monitor")
         .withParameter("interface_type", (int)bridge_interface)
         .withParameter("instance", (int)2) // For eth2
         .withParameter("message_code", (int)joined_bridge_message)
         .withParameter("str", (char *)NULL);

    mock()
    	 .expectOneCall("interface_get_type_from_name")
         .withParameter("name", "eth");

    actual_value = bridge_config_add_interfaces(instance);

    CHECK_TRUE(actual_value);

    mock().checkExpectations();

}

TEST(interface_change, bridge_config_add_interfaces_returns_false_with_bad_interface_name)
{
    size_t instance = 1;
    bool actual_value;
    config_st config;
    element_t element;
    bridge_context_st bridge_context;
    char new_interface_name[] = "garbage";


    element.value = new_interface_name;
    config.count = config_lan_mask+1;
    config.elements = &element;

    bridge_context.current_state = bridge_state_connected;
    bridge_context.config = &config;

    mock()
    	.expectOneCall("bridge_get_context")
        .withParameter("instance", (int)instance)
        .andReturnValue(&bridge_context);

    mock()
         .expectOneCall("bridge_interface_add_interfaces")
         .withParameter("instance", (int)instance)
         .withParameter("interface_names", new_interface_name)
         .andReturnValue(false);

    actual_value = bridge_config_add_interfaces(instance);

    CHECK_FALSE(actual_value);

    mock().checkExpectations();

}

TEST(interface_change, bridge_config_add_interfaces_does_not_add_interface_none)
{
    size_t instance = 1;
    bool actual_value;
    config_st config;
    element_t element;
    bridge_context_st bridge_context;
    char new_interface_name[] = "none";

    element.value = new_interface_name;
    config.count = config_lan_mask+1;
    config.elements = &element;

    bridge_context.current_state = bridge_state_connected;
    bridge_context.config = &config;

    mock()
    	.expectOneCall("bridge_get_context")
        .withParameter("instance", (int)instance)
        .andReturnValue(&bridge_context);

    actual_value = bridge_config_add_interfaces(instance);

    CHECK_TRUE(actual_value);

    mock().checkExpectations();
}

TEST(interface_change, bridge_config_delete_interfaces_calls_bridge_interface_delete_interfaces)
{
    size_t instance = 1;
    bool actual_value;
    config_st config;
    element_t element;
    bridge_context_st bridge_context;
    char new_interface_name[] = "eth2";

    element.value = new_interface_name;
    config.count = config_lan_mask+1;
    config.elements = &element;

    bridge_context.current_state = bridge_state_connected;
    bridge_context.config = &config;

    mock()
    	.expectOneCall("bridge_get_context")
        .withParameter("instance", (int)instance)
        .andReturnValue(&bridge_context);

    mock()
         .expectOneCall("bridge_interface_delete_interfaces")
         .withParameter("instance", (int)instance)
         .withParameter("interface_names", new_interface_name);

    mock()
         .expectOneCall("send_message_to_interface_monitor")
         .withParameter("interface_type", (int)bridge_interface)
         .withParameter("instance", (int)2) // For eth2
         .withParameter("message_code", (int)left_bridge_message)
         .withParameter("str", (char *)NULL);

    mock()
    	 .expectOneCall("interface_get_type_from_name")
         .withParameter("name", "eth");

    actual_value = bridge_config_delete_interfaces(instance);

    CHECK_TRUE(actual_value);

    mock().checkExpectations();
}

TEST(interface_change, bridge_interfaces_changed_detects_interface_changed)
{
    size_t instance = 1;
    bool actual_value;
    char old_interface_name[] = "eth1";
    char new_interface_name[] = "eth2";
    bridge_context_st bridge_context;

    bridge_context.current_state = bridge_state_connected;

    mock()
    	.expectOneCall("bridge_get_context")
        .withParameter("instance", (int)instance)
        .andReturnValue(&bridge_context);

    mock()
         .expectOneCall("bridge_interface_delete_interfaces")
         .withParameter("instance", (int)instance)
         .withParameter("interface_names", old_interface_name);

    mock()
         .expectOneCall("bridge_interface_add_interfaces")
         .withParameter("instance", (int)instance)
         .withParameter("interface_names", new_interface_name);

    mock()
         .expectOneCall("send_message_to_interface_monitor")
         .withParameter("interface_type", (int)bridge_interface)
         .withParameter("instance", (int)1)
         .withParameter("message_code", (int)left_bridge_message)
         .withParameter("str", (char *)NULL);

    mock()
         .expectOneCall("send_message_to_interface_monitor")
         .withParameter("interface_type", (int)bridge_interface)
         .withParameter("instance", (int)2)
         .withParameter("message_code", (int)joined_bridge_message)
         .withParameter("str", (char *)NULL);

    mock()
         .expectNCalls(2,"interface_get_type_from_name")
         .withParameter("name", "eth");

    actual_value = bridge_interfaces_changed(instance, old_interface_name, new_interface_name);

    CHECK_TRUE(actual_value);

    mock().checkExpectations();
}

TEST(interface_change, bridge_interfaces_changed_returns_false_when_disconnected)
{
    size_t instance = 1;
    bool actual_value;
    char old_interface_name[] = "eth1";
    char new_interface_name[] = "eth2";
    bridge_context_st bridge_context;

    bridge_context.current_state = bridge_state_disconnected;

    mock()
    	.expectOneCall("bridge_get_context")
        .withParameter("instance", (int)instance)
        .andReturnValue(&bridge_context);

    actual_value = bridge_interfaces_changed(instance, old_interface_name, new_interface_name);

    CHECK_FALSE(actual_value);

    mock().checkExpectations();
}

TEST(interface_change, bridge_interfaces_changed_returns_true_when_send_message_fails)
{
    size_t instance = 1;
    bool actual_value;
    char old_interface_name[] = "eth1";
    char new_interface_name[] = "eth2";
    bridge_context_st bridge_context;

    bridge_context.current_state = bridge_state_connected;

    mock()
    	.expectOneCall("bridge_get_context")
        .withParameter("instance", (int)instance)
        .andReturnValue(&bridge_context);

    mock()
         .expectOneCall("bridge_interface_delete_interfaces")
         .withParameter("instance", (int)instance)
         .withParameter("interface_names", old_interface_name);

    mock()
         .expectOneCall("bridge_interface_add_interfaces")
         .withParameter("instance", (int)instance)
         .withParameter("interface_names", new_interface_name);

    mock()
         .expectOneCall("send_message_to_interface_monitor")
         .withParameter("interface_type", (int)bridge_interface)
         .withParameter("instance", (int)1)
         .withParameter("message_code", (int)left_bridge_message)
         .withParameter("str", (char *)NULL);

    mock()
         .expectOneCall("send_message_to_interface_monitor")
         .withParameter("interface_type", (int)bridge_interface)
         .withParameter("instance", (int)2)
         .withParameter("message_code", (int)joined_bridge_message)
         .withParameter("str", (char *)NULL)
	     .andReturnValue(-1);

    mock()
         .expectNCalls(2,"interface_get_type_from_name")
         .withParameter("name", "eth");

    actual_value = bridge_interfaces_changed(instance, old_interface_name, new_interface_name);
    CHECK_TRUE(actual_value);

    mock().checkExpectations();
}



void check_parsing_names(char const * const * interfaces, size_t num_interfaces, char const * const expected_iface_name, interfaces_t const expected_iface)
{
    bool    result;
    size_t  idx;
    interfaces_t interface_type;
    size_t instance;

    for (idx = 0; idx < num_interfaces; idx++)
    {
        mock()
            .expectOneCall("interface_get_type_from_name")
            .withParameter("name", expected_iface_name)
            .andReturnValue(expected_iface);

        result = parse_interface_name(interfaces[idx], &interface_type, &instance);
        CHECK_TRUE(result);
        LONGS_EQUAL(expected_iface, interface_type);
        if (idx == 0)
        {
            LONGS_EQUAL(instance, 1);
        }
        else
        {
            LONGS_EQUAL(instance, idx);
        }
        mock().checkExpectations();
    }
}


TEST(interface_change, bridge_can_parse_interface_names)
{
    // Check Parse_interface_name function works.
    char    const * const eth_ifaces[] = { "eth", "eth1", "eth2", "eth3", "eth4", "eth5", "eth6", "eth7", "eth8" };
    char    const * const wifi_ifaces[] = { "wifi", "wifi1", "wifi2", "wifi3", "wifi4" };
    char    const * const wifi5g_ifaces[] = { "wifi5g", "wifi5g1", "wifi5g2", "wifi5g3", "wifi5g4" };

    check_parsing_names(wifi5g_ifaces, ARRAY_SIZE(wifi5g_ifaces), "wifi5g", wifi5g_interface);
    check_parsing_names(wifi_ifaces, ARRAY_SIZE(wifi_ifaces), "wifi", wifi_interface);
    check_parsing_names(eth_ifaces, ARRAY_SIZE(eth_ifaces), "eth", eth_interface);
}


