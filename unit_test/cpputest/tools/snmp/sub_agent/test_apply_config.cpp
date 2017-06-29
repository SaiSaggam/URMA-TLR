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
	#include "config_table.h"
	#include "config_def.h"
	#include "config_status.h"
	#include "config_mgr.h"
	#include "snmp_utils.h"
	#include "snmp_agent_debug.h"
    #include "apply_config.h"
    #include "digiutils.h"
}

tlr_handle_t tlr_apply_open(size_t const id, size_t const instance, char const * const user, config_mgr_access_level_t const access)
{
    mock()
        .actualCall("tlr_apply_open")
        .withParameter("id", (int)id)
        .withParameter("instance", (int)instance)
        .withParameter("access", (int)access);

    if (mock().hasReturnValue())
    {
        return (tlr_handle_t)mock().returnValue().getConstPointerValue();
    }

    return (tlr_handle_t)NULL;
}

config_status_t tlr_apply_read(tlr_handle_t const handle, char * const data, size_t const data_length, size_t const timeout)
{
    mock()
        .actualCall("tlr_apply_read")
        .withParameter("handle", (void *)handle)
        .withParameter("data_length", (int)data_length)
        .withParameter("timeout", (int)timeout);


    if (mock().hasReturnValue())
    {
        return (config_status_t)mock().returnValue().getIntValue();
    }

    return config_status_success;
}

int tlr_apply_close(tlr_handle_t const handle)
{
    mock()
        .actualCall("tlr_config_close")
        .withParameter("handle", (const void*)handle);
}

int system_command(char const * const format, ...)
{
    mock()
        .actualCall("system_command");

    if (mock().hasReturnValue())
    {
        return (config_status_t)mock().returnValue().getIntValue();
    }

    return 0;
}

int convert_ip_address_to_string_from_binary(char const * const binary_ip_address, char * const ip_address_string_buffer,
                                             size_t const str_len)
{
    mock()
        .actualCall("convert_ip_address_to_string_from_binary");

    if (mock().hasReturnValue())
    {
        return (config_status_t)mock().returnValue().getIntValue();
    }

    return 0;
}

TEST_GROUP(apply_config)
{
    void setup()
    {
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(apply_config, snmp_apply_config_returns_zero_when_all_is_ok)
{
    int actual_value;
    size_t group_id = 1;
    size_t instance = 1;
    tlr_handle_t test_handle = (tlr_handle_t)1234;

    mock().strictOrder();

    mock()
        .expectOneCall("tlr_apply_open")
        .withParameter("id", (int)group_id)
        .withParameter("instance", (int)instance)
        .withParameter("access", (int)config_mgr_access_level_super)
        .andReturnValue(test_handle);


    mock()
        .expectOneCall("tlr_apply_read")
        .withParameter("handle", (void *)test_handle)
        .withParameter("data_length", (int)256)
        .withParameter("timeout", (int)WAIT_FOREVER);

    mock().expectOneCall("system_command");

    mock()
        .expectOneCall("tlr_config_close")
        .withParameter("handle", test_handle);

    /* perform test */
    actual_value = snmp_apply_config(group_id, instance);

    /* check result */
    CHECK_EQUAL(SNMP_ERR_NOERROR, actual_value);
    mock().checkExpectations();
}

TEST(apply_config, snmp_apply_config_fails_when_tlr_apply_open_returns_null)
{
    int actual_value;
    int expected_value = -1;
    size_t group_id = 1;
    size_t instance = 1;
    tlr_handle_t test_handle = (tlr_handle_t)NULL;

    mock().strictOrder();

    mock()
        .expectOneCall("tlr_apply_open")
        .withParameter("id", (int)group_id)
        .withParameter("instance", (int)instance)
        .withParameter("access", (int)config_mgr_access_level_super)
        .andReturnValue(test_handle);

    /* perform test */
    actual_value = snmp_apply_config(group_id, instance);

    /* check result */
    CHECK_EQUAL(expected_value, actual_value);
    mock().checkExpectations();
}

TEST(apply_config, snmp_apply_config_fails_when_tlr_apply_read_returns_error)
{
    int actual_value;
    size_t group_id = 1;
    size_t instance = 1;
    tlr_handle_t test_handle = (tlr_handle_t)1234;
    int expected_value = -1;

    mock().strictOrder();

    mock()
        .expectOneCall("tlr_apply_open")
        .withParameter("id", (int)group_id)
        .withParameter("instance", (int)instance)
        .withParameter("access", (int)config_mgr_access_level_super)
        .andReturnValue(test_handle);


    mock()
        .expectOneCall("tlr_apply_read")
        .withParameter("handle", (void *)test_handle)
        .withParameter("data_length", (int)256)
        .withParameter("timeout", (int)WAIT_FOREVER)
        .andReturnValue(config_status_invalid_parameter);

    // Verify the handle is closed
    mock()
        .expectOneCall("tlr_config_close")
        .withParameter("handle", test_handle);

    /* perform test */
    actual_value = snmp_apply_config(group_id, instance);

    /* check result */
    CHECK_EQUAL(expected_value, actual_value);
    mock().checkExpectations();
}

TEST(apply_config, snmp_apply_config_fails_when_system_command_fails)
{
    int actual_value;
    int expected_value = -1;
    size_t group_id = 1;
    size_t instance = 1;
    tlr_handle_t test_handle = (tlr_handle_t)1234;

    mock().strictOrder();

    mock()
        .expectOneCall("tlr_apply_open")
        .withParameter("id", (int)group_id)
        .withParameter("instance", (int)instance)
        .withParameter("access", (int)config_mgr_access_level_super)
        .andReturnValue(test_handle);


    mock()
        .expectOneCall("tlr_apply_read")
        .withParameter("handle", (void *)test_handle)
        .withParameter("data_length", (int)256)
        .withParameter("timeout", (int)WAIT_FOREVER);

    mock().expectOneCall("system_command")
           .andReturnValue(-1);

    mock()
        .expectOneCall("tlr_config_close")
        .withParameter("handle", test_handle);

    /* perform test */
    actual_value = snmp_apply_config(group_id, instance);

    /* check result */
    CHECK_EQUAL(expected_value, actual_value);
    mock().checkExpectations();
}
