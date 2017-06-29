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
	#include "get_parameter_value.h"
	#include "snmp_utils.h"
	#include "snmp_agent_debug.h"
	#include "get_config_value.h"
	#include "get_show_value.h"
    #include "index_element.h"
}

size_t tlr_get_element_size(config_mgr_element_t const * const element)
{
    mock().actualCall("tlr_get_element_size")
		.withParameter("element", (void *)element);

	if (mock().hasReturnValue())
	{
		return (int)mock().returnValue().getIntValue();
	}
	return 4;
}

void snmp_print_parameter_debug_info(parameter_t  const * const parameter)
{
    mock().actualCall("snmp_print_parameter_debug_info")
        .withParameter("parameter", (void *)parameter);

}


u_char snmp_convert_tlr_type_to_asn_type(config_mgr_element_type_t element_type)
{
    mock().actualCall("snmp_convert_tlr_type_to_asn_type")
		.withParameter("element_type", (int)element_type);

	if (mock().hasReturnValue())
	{
		return (int)mock().returnValue().getIntValue();
	}
	return 0;
}

int snmp_set_var_typed_value(netsnmp_variable_list * newvar, u_char type,
                         const void * val_str, size_t val_len)
{
    mock().actualCall("snmp_set_var_typed_value")
	        .withParameter("newvar", (void *)newvar)
	        .withParameter("type", (int)type)
	        .withParameter("val_str", (void *)val_str)
	        .withParameter("val_len", (int)val_len);

	if (mock().hasReturnValue())
	{
		return (int)mock().returnValue().getIntValue();
	}
	return 0;
}


uint8_t * snmp_get_index_element(int const instance)
{
    mock().actualCall("snmp_get_index_element")
	      .withParameter("instance", instance);

	if (mock().hasReturnValue())
	{
		return (uint8_t *)mock().returnValue().getPointerValue();
	}
	return 0;
}

bool snmp_is_index_element(char const * const name, u_char const asn_type)
{
    mock().actualCall("snmp_is_index_element")
	      .withParameter("name", (void *)name)
	      .withParameter("asn_type", (int)asn_type);

	if (mock().hasReturnValue())
	{
		return (bool)mock().returnValue().getIntValue();
	}
	return false;
}

int get_config_group_element_value(config_mgr_group_t const * const group,
                                   size_t instance,
                                   config_mgr_element_t const * const element,
                                   u_char const asn_type,
                                   void * * const buffer,
                                   size_t * const element_size)
{
	mock()
		.actualCall("get_config_group_element_value")
	    .withParameter("group", (void *)group)
        .withParameter("instance", (int)instance)
        .withParameter("element", (void *)element)
	    .withParameter("asn_type", (int)asn_type)
		.withOutputParameter("buffer", (void *)buffer)
	    .withOutputParameter("element_size", (void *)element_size);

    if (mock().hasData("buffer"))
    {
        *buffer = mock().getData("buffer").getPointerValue();
    }

	if (mock().hasReturnValue())
	{
		return mock().returnValue().getIntValue();
	}
	return SNMP_ERR_NOERROR;
}

int get_show_group_element_value(config_mgr_group_t const * const group,
                                 size_t instance,
                                 config_mgr_element_t const * const element,
                                 u_char const asn_type,
                                 void * * const buffer,
                                 size_t * const element_size)
{
	mock()
		.actualCall("get_show_group_element_value")
        .withParameter("group", (void *)group)
        .withParameter("instance", (int)instance)
        .withParameter("element", (void *)element)
	    .withParameter("asn_type", (int)asn_type)
	    .withOutputParameter("buffer", (void *)buffer)
	    .withOutputParameter("element_size", (void *)element_size);

	if (mock().hasReturnValue())
	{
		return mock().returnValue().getIntValue();
	}
	return SNMP_ERR_NOERROR;
}


TEST_GROUP(get_parameter_value)
{
    void setup()
    {
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(get_parameter_value, returns_success_when_all_goes_well_set_group)
{
	parameter_t parameter;
	config_mgr_group_t group;
	config_mgr_element_t element;
    int actual_value;
    netsnmp_variable_list variable;
    u_char asn_type=ASN_OCTET_STR;
    char value[]="test value";
    char *buffer;
    size_t element_size=strlen(value);

    buffer = (char *)malloc(strlen(value+1));
    strcpy(buffer, value);

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

    mock().strictOrder();

    mock()
        .expectOneCall("snmp_print_parameter_debug_info")
	    .withParameter("parameter", (void *)&parameter);

    mock()
        .expectOneCall("tlr_get_element_size")
	    .withParameter("element", (void *)&element);

    mock()
        .expectOneCall("snmp_convert_tlr_type_to_asn_type")
	    .withParameter("element_type", element.type)
		.andReturnValue((int)asn_type);

    mock()
        .expectOneCall("snmp_is_index_element")
	    .withParameter("name", (void *)element.name)
	    .withParameter("asn_type", (int)asn_type);

    mock()
    	.expectOneCall("get_config_group_element_value")
        .withParameter("group", (void *)parameter.group)
        .withParameter("instance", (int)parameter.instance)
        .withParameter("element", (void *)parameter.element)
        .withParameter("asn_type", (int)asn_type)
		.withOutputParameterReturning("buffer", &buffer, sizeof(buffer))
		.withOutputParameterReturning("element_size", &element_size, sizeof(element_size));


    mock()
    	.expectOneCall("snmp_set_var_typed_value")
	    .withParameter("newvar", (void *)&variable)
	    .withParameter("type", (int)asn_type)
	    .withParameter("val_str", (void *)buffer)
	    .withParameter("val_len", (int)element_size);

    /* perform test */
    actual_value = snmp_get_parameter_value(&parameter, &variable);

    /* check result */
    CHECK_EQUAL(SNMP_ERR_NOERROR, actual_value);
    mock().checkExpectations();
}

TEST(get_parameter_value, returns_success_when_all_goes_well_config_group)
{
    parameter_t parameter;
    config_mgr_group_t group;
    size_t instance;
    config_mgr_element_t element;
    int actual_value;
    netsnmp_variable_list variable;
    u_char asn_type=ASN_OCTET_STR;
    char value[]="test value";
    char *buffer;
    size_t element_size=strlen(value);

    buffer = (char *)malloc(strlen(value+1));
    strcpy(buffer, value);

    element.id = 1;
    element.type = config_mgr_element_type_string;
    element.name = "test";

    group.id = 1;

    group.elements.element = &element;
    group.elements.count = 1;

    instance = 1;
    parameter.group = &group;
    parameter.group_type = config_mgr_get_group;
    parameter.element = &element;
    parameter.instance = instance;

    mock().strictOrder();

    mock()
        .expectOneCall("snmp_print_parameter_debug_info")
        .withParameter("parameter", (void *)&parameter);

    mock()
        .expectOneCall("tlr_get_element_size")
        .withParameter("element", (void *)&element);

    mock()
        .expectOneCall("snmp_convert_tlr_type_to_asn_type")
        .withParameter("element_type", element.type)
        .andReturnValue((int)asn_type);

    mock()
        .expectOneCall("snmp_is_index_element")
        .withParameter("name", (void *)element.name)
        .withParameter("asn_type", (int)asn_type);

    mock()
        .expectOneCall("get_show_group_element_value")
        .withParameter("group", (void *)&group)
        .withParameter("instance", (int)instance)
        .withParameter("element", (void *)&element)
        .withParameter("asn_type", (int)asn_type)
        .withOutputParameterReturning("buffer", &buffer, sizeof(buffer))
        .withOutputParameterReturning("element_size", &element_size, sizeof(element_size));


    mock()
        .expectOneCall("snmp_set_var_typed_value")
        .withParameter("newvar", (void *)&variable)
        .withParameter("type", (int)asn_type)
        .withParameter("val_str", (void *)buffer)
        .withParameter("val_len", (int)element_size);

    /* perform test */
    actual_value = snmp_get_parameter_value(&parameter, &variable);

    /* check result */
    CHECK_EQUAL(SNMP_ERR_NOERROR, actual_value);
    mock().checkExpectations();
}


TEST(get_parameter_value, returns_error_with_bad_element_size)
{
	parameter_t parameter;
	config_mgr_group_t group;
	config_mgr_element_t element;
    int actual_value;
    netsnmp_variable_list variable;

    element.id = 1;
    element.type = config_mgr_element_type_string;
    element.name = "test";

    group.id = 1;

    group.elements.element = &element;
    group.elements.count = 1;

    parameter.group = &group;
    parameter.group_type = config_mgr_set_group;
    parameter.element = &element;

    mock().strictOrder();

    mock()
        .expectOneCall("snmp_print_parameter_debug_info")
	    .withParameter("parameter", (void *)&parameter);

    mock()
        .expectOneCall("tlr_get_element_size")
	    .withParameter("element", (void *)&element)
		.andReturnValue(0);

    /* perform test */
    actual_value = snmp_get_parameter_value(&parameter, &variable);

    /* check result */
    CHECK_EQUAL(SNMP_ERR_BADVALUE, actual_value);
    mock().checkExpectations();
}

TEST(get_parameter_value, returns_error_when_snmp_convert_tlr_type_to_asn_type_fails)
{
	parameter_t parameter;
	config_mgr_group_t group;
	config_mgr_element_t element;
    int actual_value;
    netsnmp_variable_list variable;

    element.id = 1;
    element.type = config_mgr_element_type_string;
    element.name = "test";

    group.id = 1;

    group.elements.element = &element;
    group.elements.count = 1;

    parameter.group = &group;
    parameter.group_type = config_mgr_set_group;
    parameter.element = &element;

    mock().strictOrder();

    mock()
        .expectOneCall("snmp_print_parameter_debug_info")
	    .withParameter("parameter", (void *)&parameter);

    mock()
        .expectOneCall("tlr_get_element_size")
	    .withParameter("element", (void *)&element);

    mock()
        .expectOneCall("snmp_convert_tlr_type_to_asn_type")
	    .withParameter("element_type", element.type)
		.andReturnValue((int)MAX_SUBID);


    /* perform test */
    actual_value = snmp_get_parameter_value(&parameter, &variable);

    /* check result */
    CHECK_EQUAL(SNMP_ERR_BADVALUE, actual_value);
    mock().checkExpectations();
}

TEST(get_parameter_value, returns_success_when_element_is_index)
{
	parameter_t parameter;
	config_mgr_group_t group;
	config_mgr_element_t element;
    int actual_value;
    netsnmp_variable_list variable;
    u_char asn_type=ASN_INTEGER;
    int *ptr_int = (int *)malloc(sizeof(int));
    size_t element_size = sizeof(int);

    element.id = 1;
    element.type = config_mgr_element_type_string;
    element.name = "index";

    group.id = 1;

    group.elements.element = &element;
    group.elements.count = 1;

    parameter.group = &group;
    parameter.group_type = config_mgr_set_group;
    parameter.element = &element;
    parameter.instance = 1;

    mock().strictOrder();

    mock()
        .expectOneCall("snmp_print_parameter_debug_info")
	    .withParameter("parameter", (void *)&parameter);

    mock()
        .expectOneCall("tlr_get_element_size")
	    .withParameter("element", (void *)&element)
	    .andReturnValue((int)element_size);

    mock()
        .expectOneCall("snmp_convert_tlr_type_to_asn_type")
	    .withParameter("element_type", element.type)
		.andReturnValue((int)asn_type);

    mock()
        .expectOneCall("snmp_is_index_element")
	    .withParameter("name", (void *)element.name)
	    .withParameter("asn_type", (int)asn_type)
		.andReturnValue((int)true);

    mock().expectOneCall("snmp_get_index_element")
	      .withParameter("instance", parameter.instance)
	      .andReturnValue(ptr_int);


    mock()
     	.expectOneCall("snmp_set_var_typed_value")
 	    .withParameter("newvar", (void *)&variable)
 	    .withParameter("type", (int)asn_type)
 	    .withParameter("val_str", (void *)ptr_int)
 	    .withParameter("val_len", (int)element_size);


    /* perform test */
    actual_value = snmp_get_parameter_value(&parameter, &variable);

    /* check result */
    CHECK_EQUAL(SNMP_ERR_NOERROR, actual_value);
    mock().checkExpectations();
}

TEST(get_parameter_value, returns_error_when_snmp_set_var_typed_value_fails)
{
    parameter_t parameter;
    config_mgr_group_t group;
    config_mgr_element_t element;
    int actual_value;
    netsnmp_variable_list variable;
    u_char asn_type=ASN_INTEGER;
    int *ptr_int = (int *)malloc(sizeof(int));
    size_t element_size = sizeof(int);

    element.id = 1;
    element.type = config_mgr_element_type_string;
    element.name = "index";

    group.id = 1;

    group.elements.element = &element;
    group.elements.count = 1;

    parameter.group = &group;
    parameter.group_type = config_mgr_set_group;
    parameter.element = &element;
    parameter.instance = 1;

    mock().strictOrder();

    mock()
        .expectOneCall("snmp_print_parameter_debug_info")
        .withParameter("parameter", (void *)&parameter);

    mock()
        .expectOneCall("tlr_get_element_size")
        .withParameter("element", (void *)&element)
        .andReturnValue((int)element_size);

    mock()
        .expectOneCall("snmp_convert_tlr_type_to_asn_type")
        .withParameter("element_type", element.type)
        .andReturnValue((int)asn_type);

    mock()
        .expectOneCall("snmp_is_index_element")
        .withParameter("name", (void *)element.name)
        .withParameter("asn_type", (int)asn_type)
        .andReturnValue((int)true);

    mock().expectOneCall("snmp_get_index_element")
          .withParameter("instance", parameter.instance)
          .andReturnValue(ptr_int);

    mock()
        .expectOneCall("snmp_set_var_typed_value")
        .withParameter("newvar", (void *)&variable)
        .withParameter("type", (int)asn_type)
        .withParameter("val_str", (void *)ptr_int)
        .withParameter("val_len", (int)element_size)
        .andReturnValue(-1);

    /* perform test */
    actual_value = snmp_get_parameter_value(&parameter, &variable);

    /* check result */
    CHECK_EQUAL(SNMP_ERR_GENERR, actual_value);
    mock().checkExpectations();
}






