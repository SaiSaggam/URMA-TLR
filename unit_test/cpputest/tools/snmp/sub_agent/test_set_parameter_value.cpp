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
	#include "set_parameter_value.h"
	#include "snmp_utils.h"
	#include "snmp_agent_debug.h"
	#include "get_config_value.h"
	#include "get_show_value.h"
    #include "index_element.h"
}


void snmp_print_parameter_debug_info(parameter_t  const * const parameter)
{
    mock().actualCall("snmp_print_parameter_debug_info")
        .withParameter("parameter", (void *)parameter);

}

config_status_t tlr_set_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      char const * const value,
                                      config_mgr_access_level_t const access)
{
    mock()
        .actualCall("tlr_set_element_value")
        .withParameter("group_id", (int)group_id)
        .withParameter("instance", (int)instance)
        .withParameter("element_id", (int)element_id)
        .withParameter("access", (int)access);


    if (mock().hasReturnValue())
    {
        return (config_status_t)mock().returnValue().getIntValue();
    }

    return config_status_success;
}

tlr_handle_t tlr_config_open(size_t const group_id, size_t const instance, config_mgr_access_level_t const access)
{
    mock()
        .actualCall("tlr_config_open")
        .withParameter("group_id", (int)group_id)
        .withParameter("instance", (int)instance)
        .withParameter("access", (int)access);

    if (mock().hasReturnValue())
    {
        return (tlr_handle_t)mock().returnValue().getConstPointerValue();
    }

    return (tlr_handle_t)NULL;
}

config_status_t tlr_config_write_native(tlr_handle_t const handle, size_t const element_id, void const * const value)
{
    mock()
        .actualCall("tlr_config_write_native")
        .withParameter("handle", (void *)handle)
        .withParameter("element_id", (int)element_id)
        .withParameter("value", (void *)value);

    if (mock().hasReturnValue())
    {
        return (config_status_t)mock().returnValue().getIntValue();
    }

    return (config_status_t)NULL;
}

void tlr_config_close(tlr_handle_t const handle)
{
    mock()
        .actualCall("tlr_config_close")
        .withParameter("handle", (const void*)handle);
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

TEST_GROUP(set_parameter_value)
{
    void setup()
    {
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(set_parameter_value, returns_success_when_all_goes_well_for_type_string)
{
    int actual_value;
	parameter_t parameter;
	config_mgr_group_t group;
	config_mgr_element_t element;
    netsnmp_variable_list var;
    tlr_handle_t test_handle = (tlr_handle_t)1234;
    u_char test_string[] = "this is a test string";

    element.id = 1;
    element.type = config_mgr_element_type_string;
    element.name = "test";

    group.id = 1;

    group.elements.element = &element;
    group.elements.count = 1;

    parameter.group = &group;
    parameter.group_type = config_mgr_set_group;
    parameter.element = &element;
    parameter.instance = 1;

    var.val.string = test_string;

    mock().strictOrder();

    mock().expectOneCall("snmp_print_parameter_debug_info")
        .withParameter("parameter", (void *)&parameter);

    mock()
        .expectOneCall("tlr_config_open")
        .withParameter("group_id", (int)group.id)
        .withParameter("instance", (int)parameter.instance)
        .withParameter("access", (int)config_mgr_access_level_super)
        .andReturnValue(test_handle);

    mock()
        .expectOneCall("tlr_config_write_native")
        .withParameter("handle", (void *)test_handle)
        .withParameter("element_id", (int)element.id)
        .withParameter("value", (void *)test_string);

    mock()
        .expectOneCall("tlr_config_close")
        .withParameter("handle", test_handle);

    /* perform test */
    actual_value = snmp_set_parameter_value(&parameter, &var);

    /* check result */
    CHECK_EQUAL(SNMP_ERR_NOERROR, actual_value);
    mock().checkExpectations();
}

TEST(set_parameter_value, failure_on_config_open_type_string)
{
    int actual_value;
    parameter_t parameter;
    config_mgr_group_t group;
    config_mgr_element_t element;
    netsnmp_variable_list var;
    tlr_handle_t test_handle = (tlr_handle_t)NULL;
    u_char test_string[] = "this is a test string";

    element.id = 1;
    element.type = config_mgr_element_type_string;
    element.name = "test";

    group.id = 1;

    group.elements.element = &element;
    group.elements.count = 1;

    parameter.group = &group;
    parameter.group_type = config_mgr_set_group;
    parameter.element = &element;
    parameter.instance = 1;

    var.val.string = test_string;

    mock().strictOrder();

    mock().expectOneCall("snmp_print_parameter_debug_info")
        .withParameter("parameter", (void *)&parameter);

    mock()
        .expectOneCall("tlr_config_open")
        .withParameter("group_id", (int)group.id)
        .withParameter("instance", (int)parameter.instance)
        .withParameter("access", (int)config_mgr_access_level_super)
        .andReturnValue(test_handle);

    /* perform test */
    actual_value = snmp_set_parameter_value(&parameter, &var);

    /* check result */
    CHECK_EQUAL(SNMP_ERR_GENERR, actual_value);
    mock().checkExpectations();

}

TEST(set_parameter_value, failure_on_config_write_type_string)
{
    int actual_value;
    parameter_t parameter;
    config_mgr_group_t group;
    config_mgr_element_t element;
    netsnmp_variable_list var;
    tlr_handle_t test_handle = (tlr_handle_t)1234;
    u_char test_string[] = "this is a test string";

    element.id = 1;
    element.type = config_mgr_element_type_string;
    element.name = "test";

    group.id = 1;

    group.elements.element = &element;
    group.elements.count = 1;

    parameter.group = &group;
    parameter.group_type = config_mgr_set_group;
    parameter.element = &element;
    parameter.instance = 1;

    var.val.string = test_string;

    mock().strictOrder();

    mock().expectOneCall("snmp_print_parameter_debug_info")
        .withParameter("parameter", (void *)&parameter);

    mock()
        .expectOneCall("tlr_config_open")
        .withParameter("group_id", (int)group.id)
        .withParameter("instance", (int)parameter.instance)
        .withParameter("access", (int)config_mgr_access_level_super)
        .andReturnValue(test_handle);

    mock()
        .expectOneCall("tlr_config_write_native")
        .withParameter("handle", (void *)test_handle)
        .withParameter("element_id", (int)element.id)
        .withParameter("value", (void *)test_string)
        .andReturnValue(config_status_invalid_parameter);

    mock()
        .expectOneCall("tlr_config_close")
        .withParameter("handle", test_handle);

    /* perform test */
    actual_value = snmp_set_parameter_value(&parameter, &var);

    /* check result */
    CHECK_EQUAL(SNMP_ERR_GENERR, actual_value);
    mock().checkExpectations();
}

TEST(set_parameter_value, returns_success_when_all_goes_well_for_type_enum)
{
    int actual_value;
    parameter_t parameter;
    config_mgr_group_t group;
    config_mgr_element_t element;
    netsnmp_variable_list var;

    config_mgr_element_enum_t const enum_values[] = {
        {"enum1", NULL, 10, 0, false, config_mgr_access_readwrite},
        {"enum2", NULL, 11, 1, false, config_mgr_access_readwrite}
    };

    config_mgr_element_enum_limit_t enum_limit = {
      enum_values,
      sizeof enum_values/sizeof enum_values[0]
    };

    long test_enum_integer = enum_values[0].value;

    element.id = 1;
    element.type = config_mgr_element_type_enum;
    element.name = "test";
    element.value_limit = (config_mgr_element_limit_t) &enum_limit;


    group.id = 1;

    group.elements.element = &element;
    group.elements.count = 1;

    parameter.group = &group;
    parameter.group_type = config_mgr_set_group;
    parameter.element = &element;
    parameter.instance = 1;

    var.val.integer = &test_enum_integer;

    mock().strictOrder();

    mock().expectOneCall("snmp_print_parameter_debug_info")
        .withParameter("parameter", (void *)&parameter);

    mock()
        .expectOneCall("tlr_set_element_value")
        .withParameter("group_id", (int)group.id)
        .withParameter("instance", (int)parameter.instance)
        .withParameter("element_id", (int)element.id)
        .withParameter("access", (int)config_mgr_access_level_super);

    /* perform test */
    actual_value = snmp_set_parameter_value(&parameter, &var);


    /* check result */
    CHECK_EQUAL(SNMP_ERR_NOERROR, actual_value);
    mock().checkExpectations();
}

TEST(set_parameter_value, failure_invalid_enum_on_type_enum)
{
    int actual_value;
    parameter_t parameter;
    config_mgr_group_t group;
    config_mgr_element_t element;
    netsnmp_variable_list var;

    config_mgr_element_enum_t const enum_values[] = {
        {"enum1", NULL, 10, 0, false, config_mgr_access_readwrite},
        {"enum2", NULL, 11, 1, false, config_mgr_access_readwrite}
    };

    config_mgr_element_enum_limit_t enum_limit = {
      enum_values,
      sizeof enum_values/sizeof enum_values[0]
    };

    long test_enum_integer = enum_values[0].value + 10;

    element.id = 1;
    element.type = config_mgr_element_type_enum;
    element.name = "test";
    element.value_limit = (config_mgr_element_limit_t) &enum_limit;


    group.id = 1;

    group.elements.element = &element;
    group.elements.count = 1;

    parameter.group = &group;
    parameter.group_type = config_mgr_set_group;
    parameter.element = &element;
    parameter.instance = 1;

    var.val.integer = &test_enum_integer;

    mock().strictOrder();

    mock().expectOneCall("snmp_print_parameter_debug_info")
        .withParameter("parameter", (void *)&parameter);

    /* perform test */
    actual_value = snmp_set_parameter_value(&parameter, &var);


    /* check result */
    CHECK_EQUAL(SNMP_ERR_GENERR, actual_value);
    mock().checkExpectations();
}

TEST(set_parameter_value, failure_on_set_element_on_type_enum)
{
    int actual_value;
    parameter_t parameter;
    config_mgr_group_t group;
    config_mgr_element_t element;
    netsnmp_variable_list var;

    config_mgr_element_enum_t const enum_values[] = {
        {"enum1", NULL, 10, 0, false, config_mgr_access_readwrite},
        {"enum2", NULL, 11, 1, false, config_mgr_access_readwrite}
    };

    config_mgr_element_enum_limit_t enum_limit = {
      enum_values,
      sizeof enum_values/sizeof enum_values[0]
    };

    long test_enum_integer = enum_values[0].value;

    element.id = 1;
    element.type = config_mgr_element_type_enum;
    element.name = "test";
    element.value_limit = (config_mgr_element_limit_t) &enum_limit;


    group.id = 1;

    group.elements.element = &element;
    group.elements.count = 1;

    parameter.group = &group;
    parameter.group_type = config_mgr_set_group;
    parameter.element = &element;
    parameter.instance = 1;

    var.val.integer = &test_enum_integer;

    mock().strictOrder();

    mock().expectOneCall("snmp_print_parameter_debug_info")
        .withParameter("parameter", (void *)&parameter);

    mock()
        .expectOneCall("tlr_set_element_value")
        .withParameter("group_id", (int)group.id)
        .withParameter("instance", (int)parameter.instance)
        .withParameter("element_id", (int)element.id)
        .withParameter("access", (int)config_mgr_access_level_super)
        .andReturnValue(config_status_invalid_parameter);

    /* perform test */
    actual_value = snmp_set_parameter_value(&parameter, &var);


    /* check result */
    CHECK_EQUAL(SNMP_ERR_GENERR, actual_value);
    mock().checkExpectations();
}

TEST(set_parameter_value, returns_success_when_all_goes_well_for_type_ip_address)
{
    int actual_value;
    parameter_t parameter;
    config_mgr_group_t group;
    config_mgr_element_t element;
    netsnmp_variable_list var;

    u_char test_ip_address[] = {0x01, 0x02, 0x03, 0x04};

    element.id = 1;
    element.type = config_mgr_element_type_ipaddr;
    element.name = "test";

    group.id = 1;

    group.elements.element = &element;
    group.elements.count = 1;

    parameter.group = &group;
    parameter.group_type = config_mgr_set_group;
    parameter.element = &element;
    parameter.instance = 1;

    var.val.string = test_ip_address;

    mock().strictOrder();

    mock().expectOneCall("snmp_print_parameter_debug_info")
        .withParameter("parameter", (void *)&parameter);

    mock()
         .expectOneCall("convert_ip_address_to_string_from_binary");


    mock()
        .expectOneCall("tlr_set_element_value")
        .withParameter("group_id", (int)group.id)
        .withParameter("instance", (int)parameter.instance)
        .withParameter("element_id", (int)element.id)
        .withParameter("access", (int)config_mgr_access_level_super);

    /* perform test */
    actual_value = snmp_set_parameter_value(&parameter, &var);


    /* check result */
    CHECK_EQUAL(SNMP_ERR_NOERROR, actual_value);
    mock().checkExpectations();
}

TEST(set_parameter_value, failure_on_convert_ip_address_to_string_on_type_ip_address)
{
    int actual_value;
    parameter_t parameter;
    config_mgr_group_t group;
    config_mgr_element_t element;
    netsnmp_variable_list var;

    u_char test_ip_address[] = {0x01, 0x02, 0x03, 0x04};

    element.id = 1;
    element.type = config_mgr_element_type_ipaddr;
    element.name = "test";

    group.id = 1;

    group.elements.element = &element;
    group.elements.count = 1;

    parameter.group = &group;
    parameter.group_type = config_mgr_set_group;
    parameter.element = &element;
    parameter.instance = 1;

    var.val.string = test_ip_address;

    mock().strictOrder();

    mock().expectOneCall("snmp_print_parameter_debug_info")
        .withParameter("parameter", (void *)&parameter);

    mock()
         .expectOneCall("convert_ip_address_to_string_from_binary")
         .andReturnValue(-1);

    /* perform test */
    actual_value = snmp_set_parameter_value(&parameter, &var);


    /* check result */
    CHECK_EQUAL(SNMP_ERR_GENERR, actual_value);
    mock().checkExpectations();
}

TEST(set_parameter_value, failure_on_set_element_on_type_ip_address)
{
    int actual_value;
    parameter_t parameter;
    config_mgr_group_t group;
    config_mgr_element_t element;
    netsnmp_variable_list var;

    u_char test_ip_address[] = {0x01, 0x02, 0x03, 0x04};

    element.id = 1;
    element.type = config_mgr_element_type_ipaddr;
    element.name = "test";

    group.id = 1;

    group.elements.element = &element;
    group.elements.count = 1;

    parameter.group = &group;
    parameter.group_type = config_mgr_set_group;
    parameter.element = &element;
    parameter.instance = 1;

    var.val.string = test_ip_address;

    mock().strictOrder();

    mock().expectOneCall("snmp_print_parameter_debug_info")
        .withParameter("parameter", (void *)&parameter);

    mock()
         .expectOneCall("convert_ip_address_to_string_from_binary");


    mock()
        .expectOneCall("tlr_set_element_value")
        .withParameter("group_id", (int)group.id)
        .withParameter("instance", (int)parameter.instance)
        .withParameter("element_id", (int)element.id)
        .withParameter("access", (int)config_mgr_access_level_super)
        .andReturnValue(config_status_invalid_parameter);

    /* perform test */
    actual_value = snmp_set_parameter_value(&parameter, &var);


    /* check result */
    CHECK_EQUAL(SNMP_ERR_GENERR, actual_value);
    mock().checkExpectations();
}
