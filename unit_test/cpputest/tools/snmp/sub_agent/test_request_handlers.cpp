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
    #include "request_handlers.h"
    #include "snmp_utils.h"
    #include "snmp_agent_debug.h"
    #include "oid_to_parameter.h"
    #include "get_parameter_value.h"
}

static bool oid_to_parameter_should_do_withParameter_calls;
static bool oid_to_next_parameter_should_do_withParameter_calls;


void snmp_print_get_request(netsnmp_variable_list const * const var)
{
}

void snmp_print_get_next_request(netsnmp_variable_list const * const var)
{
}

int oid_to_parameter(oid const * const oid, uint32_t const oid_len, parameter_t * const parameter)
{
    if (oid_to_parameter_should_do_withParameter_calls)
    {
        mock().actualCall("oid_to_parameter")
                .withParameter("oid", (void *)oid)
                .withParameter("oid_len", (int)oid_len);
    }
    else
    {
        mock().actualCall("oid_to_parameter");
    }
    return mock().intReturnValue();
}

int snmp_get_parameter_value(parameter_t * const parameter, netsnmp_variable_list * const var)
{
    mock().actualCall("snmp_get_parameter_value");
    return mock().intReturnValue();
}

int oid_to_next_parameter(oid const * const orig_oid, uint32_t const oid_len, parameter_t * const parameter)
{
    if (oid_to_next_parameter_should_do_withParameter_calls)
    {
        mock().actualCall("oid_to_next_parameter")
                .withParameter("orig_oid", (void *)orig_oid)
                .withParameter("oid_len", (int)oid_len);
    }
    else
    {
        mock().actualCall("oid_to_next_parameter");
    }
    return mock().intReturnValue();
}

int snmp_set_var_objid(netsnmp_variable_list * var, const oid * name, size_t name_length)
{
    mock().actualCall("snmp_set_var_objid");
    return mock().intReturnValue();
}

TEST_GROUP(handle_get_request)
{
    void setup()
    {
        oid_to_parameter_should_do_withParameter_calls = false;
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(handle_get_request, returns_success_when_all_goes_well)
{
    netsnmp_variable_list variable;
    int actual_value;

    /* setup */
    mock().strictOrder();
    mock()
        .expectOneCall("oid_to_parameter")
        .andReturnValue(0);
    mock()
        .expectOneCall("snmp_get_parameter_value")
        .andReturnValue(SNMP_ERR_NOERROR);

    /* perform test */
    actual_value = handle_get_request(&variable);

    /* check result */
    CHECK_EQUAL(SNMP_ERR_NOERROR, actual_value);
    mock().checkExpectations();
}

TEST(handle_get_request, returns_no_such_name_when_oid_to_parameter_fails)
{
    netsnmp_variable_list variable;
    int actual_value;

    /* setup */
    mock().strictOrder();
    mock()
        .expectOneCall("oid_to_parameter")
        .andReturnValue(-1);

    /* perform test */
    actual_value = handle_get_request(&variable);

    /* check result */
    CHECK_EQUAL(SNMP_ERR_NOSUCHNAME, actual_value);
    mock().checkExpectations();
}

TEST(handle_get_request, returns_error_result_from_snmp_get_parameter_value)
{
    netsnmp_variable_list variable;
    int actual_value;
    int const expected_value = SNMP_ERR_NOACCESS;

    /* setup */
    mock().strictOrder();
    mock()
        .expectOneCall("oid_to_parameter")
        .andReturnValue(0);
    mock()
        .expectOneCall("snmp_get_parameter_value")
        .andReturnValue(expected_value);

    /* perform test */
    actual_value = handle_get_request(&variable);

    /* check result */
    CHECK_EQUAL(expected_value, actual_value);
    mock().checkExpectations();
}

TEST(handle_get_request, var_name_and_length_get_passed_to_oid_to_parameter)
{
    netsnmp_variable_list variable;
    int const expected_value = SNMP_ERR_NOERROR;
    oid expected_name;
    size_t expected_name_length = 1234;

    /* setup */
    oid_to_parameter_should_do_withParameter_calls = true;
    variable.name = &expected_name;
    variable.name_length = expected_name_length;

    mock().strictOrder();
    mock()
        .expectOneCall("oid_to_parameter")
        .withParameter("oid", (void *)&expected_name)
        .withParameter("oid_len", (int) expected_name_length)
        .andReturnValue(-1);

    /* perform test */
    (void)handle_get_request(&variable);

    mock().checkExpectations();
}

TEST_GROUP(handle_get_next_request)
{
    void setup()
    {
        oid_to_next_parameter_should_do_withParameter_calls = false;
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(handle_get_next_request, returns_success_when_all_goes_well)
{
    int actual_value;
    netsnmp_variable_list variable;

    /* setup */
    mock().strictOrder();
    mock()
        .expectOneCall("oid_to_next_parameter")
        .andReturnValue(0);
    mock()
        .expectOneCall("snmp_set_var_objid")
        .andReturnValue(0);
    mock()
        .expectOneCall("snmp_get_parameter_value")
        .andReturnValue(SNMP_ERR_NOERROR);

    /* perform test */
    actual_value = handle_get_next_request(&variable);

    /* check result */
    CHECK_EQUAL(SNMP_ERR_NOERROR, actual_value);
    mock().checkExpectations();
}

TEST(handle_get_next_request, returns_no_such_name_when_oid_to_next_returns_minus_one)
{
    int actual_value;
    netsnmp_variable_list variable;

    /* setup */
    mock().strictOrder();
    mock()
        .expectOneCall("oid_to_next_parameter")
        .andReturnValue(-1);

    /* perform test */
    actual_value = handle_get_next_request(&variable);

    /* check result */
    CHECK_EQUAL(SNMP_ERR_NOSUCHNAME, actual_value);
    mock().checkExpectations();
}

TEST(handle_get_next_request, returns_no_error_when_oid_to_next_returns_something_not_minus_one)
{
    int actual_value;
    netsnmp_variable_list variable;

    /* setup */
    mock().strictOrder();
    mock()
        .expectOneCall("oid_to_next_parameter")
        .andReturnValue(1);

    /* perform test */
    actual_value = handle_get_next_request(&variable);

    /* check result */
    CHECK_EQUAL(SNMP_ERR_NOERROR, actual_value);
    mock().checkExpectations();
}

TEST(handle_get_next_request, returns_general_error_when_snmp_set_var_objid_fails)
{
    int actual_value;
    netsnmp_variable_list variable;

    /* setup */
    mock().strictOrder();
    mock()
        .expectOneCall("oid_to_next_parameter")
        .andReturnValue(0);
    mock()
        .expectOneCall("snmp_set_var_objid")
        .andReturnValue(-1);

    /* perform test */
    actual_value = handle_get_next_request(&variable);

    /* check result */
    CHECK_EQUAL(SNMP_ERR_GENERR, actual_value);
    mock().checkExpectations();
}

TEST(handle_get_next_request, returns_result_from_snmp_get_parameter_value)
{
    int actual_value;
    netsnmp_variable_list variable;
    int const expected_result = SNMP_ERR_GENERR;

    /* setup */
    mock().strictOrder();
    mock()
        .expectOneCall("oid_to_next_parameter")
        .andReturnValue(0);
    mock()
        .expectOneCall("snmp_set_var_objid")
        .andReturnValue(0);
    mock()
        .expectOneCall("snmp_get_parameter_value")
        .andReturnValue(expected_result);

    /* perform test */
    actual_value = handle_get_next_request(&variable);

    /* check result */
    CHECK_EQUAL(expected_result, actual_value);
    mock().checkExpectations();
}

TEST(handle_get_next_request, var_name_and_length_get_passed_to_oid_to_parameter)
{
    int actual_value;
    netsnmp_variable_list variable;
    oid expected_name;
    size_t expected_name_length = 1234;

    /* setup */
    oid_to_next_parameter_should_do_withParameter_calls = true;
    variable.name = &expected_name;
    variable.name_length = expected_name_length;

    mock().strictOrder();
    mock()
        .expectOneCall("oid_to_next_parameter")
        .withParameter("orig_oid", (void *)&expected_name)
        .withParameter("oid_len", (int) expected_name_length)
        .andReturnValue(-1);

    /* perform test */
    (void)handle_get_next_request(&variable);

    /* check result */
    mock().checkExpectations();
}


