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
    #include "bridge_dns.h"
    #include "messages.h"
    #include "config_def.h"
    #include "config_utils.h"

    /* include helper code header files */
}

bool bridge_interface_set_mtu(size_t const instance, char const * const mtu_string)
{
    mock()
        .actualCall("bridge_interface_set_mtu")
        .withParameter("instance", (int)instance)
        .withParameter("mtu_string", mtu_string);
    if (mock().hasReturnValue())
    {
        return (bool)mock().returnValue().getIntValue();
    }
    return true;
}

bool bridge_interface_set_ip_address_and_mask(size_t const instance, char const * ip_address_string, char const * const mask_string)
{
    mock()
        .actualCall("bridge_interface_set_ip_address_and_mask")
        .withParameter("instance", instance)
        .withParameter("ip_address_string", ip_address_string)
        .withParameter("mask_string", mask_string);
    if (mock().hasReturnValue())
    {
        return mock().returnValue().getIntValue();
    }
    return true;
}

bool bridge_interfaces_changed(size_t const instance, char const * const old_value, char const * const new_value)
{
    mock()
        .actualCall("bridge_interfaces_changed")
        .withParameter("instance", instance)
        .withParameter("old_value", old_value)
        .withParameter("new_value", new_value);
    if (mock().hasReturnValue())
    {
        return mock().returnValue().getIntValue();
    }

    return true;
}

bool bridge_is_connected(size_t const instance)
{
	mock()
    	.actualCall("bridge_is_connected")
		.withParameter("instance", (int)instance);
	if (mock().hasReturnValue())
	{
		int x= mock().returnValue().getIntValue();
		return x;
	}
	return true;
}

void config_set_element_value(config_st * const config, size_t const index, char const * const value)
{
    mock()
        .actualCall("config_set_element_value")
        .withParameter("config", (void *)config)
        .withParameter("index", index)
        .withParameter("value", value);
}

bool bridge_interface_set_dhcp_client_to_desired_state(size_t const instance, char const * desired_state)
{
    return true;
}

bool bridge_set_dns_servers(size_t const instance, char const * const dns1, char const * const dns2)
{
    return true;
}

TEST_GROUP(config)
{
    void setup()
    {

    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(config, bridge_config_check_for_change_calls_update_functions)
{
    size_t instance = 1;
    bool actual_value;
    config_st new_config;
    config_st old_config;
    char old_value[] = "Old Value";
    char new_value[] = "New Value";
    element_t old_element;
    element_t new_element;

    old_element.value = old_value;
    new_element.value = new_value;

    // Test setting all configuration items

    old_config.count = config_lan_mask+1;
    new_config.count = config_lan_mask+1;

    old_config.elements = &old_element;
    new_config.elements = &new_element;

    mock()
    	.expectOneCall("bridge_interface_set_mtu")
		.withParameter("instance", instance)
		.withParameter("mtu_string", "New Value")
		.andReturnValue(true);

    mock()
        .expectOneCall("bridge_interfaces_changed")
        .withParameter("instance", instance)
        .withParameter("old_value", "Old Value")
        .withParameter("new_value", "New Value");

    mock().expectNCalls(2, "bridge_interface_set_ip_address_and_mask")
	    .withParameter("instance", instance)
	    .withParameter("ip_address_string", "New Value")
	    .withParameter("mask_string", "New Value")
		.andReturnValue(true);

    mock().expectNCalls(2, "bridge_is_connected")
	    .withParameter("instance", instance)
		.andReturnValue(true);

    actual_value = bridge_config_check_for_change(instance, &old_config, &new_config);

    CHECK_TRUE(actual_value);

    mock().checkExpectations();

}

TEST(config, bridge_config_check_for_change_not_handled)
{
    size_t instance = 1;
    bool actual_value;
    config_st new_config;
    config_st old_config;
    char old_value[] = "Old Value";
    char new_value[] = "New Value";
    element_t old_element;
    element_t new_element;

    old_element.value = old_value;
    new_element.value = new_value;

    // Test setting all configuration items

    old_config.count = config_lan_mask+1;
    new_config.count = config_lan_mask+1;

    old_config.elements = &old_element;
    new_config.elements = &new_element;

    // Have bridge_interface_set_mtu return false, this will
    // trigger a call to config_set_element_value
    mock()
    	.expectOneCall("bridge_interface_set_mtu")
		.withParameter("instance", instance)
		.withParameter("mtu_string", "New Value")
		.andReturnValue(false);

    mock()
        .expectOneCall("bridge_interfaces_changed")
        .withParameter("instance", instance)
        .withParameter("old_value", "Old Value")
        .withParameter("new_value", "New Value");

    mock().expectNCalls(2, "bridge_interface_set_ip_address_and_mask")
	    .withParameter("instance", instance)
	    .withParameter("ip_address_string", "New Value")
	    .withParameter("mask_string", "New Value")
		.andReturnValue(true);

    mock().expectNCalls(2, "bridge_is_connected")
	    .withParameter("instance", instance)
		.andReturnValue(true);

    mock()
    	.expectOneCall("config_set_element_value")
	    .withParameter("config", (void *)&new_config)
	    .withParameter("index", config_lan_mtu)
	    .withParameter("value", "Old Value");

    actual_value = bridge_config_check_for_change(instance, &old_config, &new_config);

    CHECK_TRUE(actual_value);

    mock().checkExpectations();

}

TEST(config, bridge_config_check_for_change_bridge_not_connected)
{
    size_t instance = 1;
    bool actual_value;
    config_st new_config;
    config_st old_config;
    char old_value[] = "Old Value";
    char new_value[] = "New Value";
    element_t old_element;
    element_t new_element;

    old_element.value = old_value;
    new_element.value = new_value;

    // Test setting all configuration items

    old_config.count = config_lan_mask+1;
    new_config.count = config_lan_mask+1;

    old_config.elements = &old_element;
    new_config.elements = &new_element;

    mock()
    	.expectOneCall("bridge_interface_set_mtu")
		.withParameter("instance", instance)
		.withParameter("mtu_string", "New Value")
		.andReturnValue(true);

    mock()
        .expectOneCall("bridge_interfaces_changed")
        .withParameter("instance", instance)
        .withParameter("old_value", "Old Value")
        .withParameter("new_value", "New Value");

    // Return false to indicate the bridge is not connected.
    mock()
        .expectNCalls(2, "bridge_is_connected")
	    .withParameter("instance", instance)
		.andReturnValue(false);

    mock()
    	.expectOneCall("config_set_element_value")
	    .withParameter("config", (void *)&new_config)
	    .withParameter("index", config_lan_ip_address)
	    .withParameter("value", "Old Value");

    mock()
    	.expectOneCall("config_set_element_value")
	    .withParameter("config", (void *)&new_config)
	    .withParameter("index", config_lan_mask)
	    .withParameter("value", "Old Value");


    actual_value = bridge_config_check_for_change(instance, &old_config, &new_config);

    CHECK_TRUE(actual_value);

    mock().checkExpectations();

}

#if defined(NEEDS_FIXING)
TEST(config, bridge_config_check_for_change_returns_false_when_bridge_interface_set_mtu_fails)
{
    size_t instance = 1;
    bool actual_value;
    config_st new_config;
    config_st old_config;
    char old_value[] = "Old Value";
    char new_value[] = "New Value";
    element_t old_element;
    element_t new_element;

    old_element.value = old_value;
    new_element.value = new_value;

    // Test setting all configuration items

    old_config.count = config_lan_mtu+1;
    new_config.count = config_lan_mtu+1;

    old_config.elements = &old_element;
    new_config.elements = &new_element;

    // Return false
    mock()
    	.expectOneCall("bridge_interface_set_mtu")
		.withParameter("instance", instance)
		.withParameter("mtu_string", "New Value")
		.andReturnValue(false);
    mock()
        .expectNCalls(2, "bridge_is_connected")
	    .withParameter("instance", instance)
		.andReturnValue(true);

    mock()
    	.expectOneCall("config_set_element_value")
	    .withParameter("config", (void *)&new_config)
	    .withParameter("index", config_lan_mtu)
	    .withParameter("value", "Old Value");

    actual_value = bridge_config_check_for_change(instance, &old_config, &new_config);

    CHECK_FALSE(actual_value);

    mock().checkExpectations();

}
#endif















