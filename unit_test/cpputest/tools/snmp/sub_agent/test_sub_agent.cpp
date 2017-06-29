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
    #include "sub_agent.h"
    #include "request_handlers.h"

    /* include helper code header files */
}

int handle_get_request(netsnmp_variable_list * var)
{
    int retval;

    mock()
        .actualCall("handle_get_request")
        .withParameter("var", (void *)var);

    retval = SNMP_ERR_NOERROR;

    return retval;
}

int handle_get_next_request(netsnmp_variable_list * var)
{
    int retval;

    mock()
        .actualCall("handle_get_next_request")
        .withParameter("var", (void *)var);

    retval = SNMP_ERR_NOERROR;

    return retval;
}

int handle_set_request_begin(netsnmp_variable_list const * const var)
{
    int retval;

    mock()
        .actualCall("handle_set_request")
        .withParameter("var", (void *)var);

    retval = SNMP_ERR_NOERROR;

    return retval;
}

int handle_set_request_commit(netsnmp_variable_list const * const var)
{
    int retval;

    mock()
        .actualCall("handle_set_request")
        .withParameter("var", (void *)var);

    retval = SNMP_ERR_NOERROR;

    return retval;
}


int netsnmp_request_set_error(netsnmp_request_info *request, int error_value)
{
    int retval;

    mock()
        .actualCall("netsnmp_request_set_error")
        .withParameter("request", (void *)request)
        .withParameter("error_value", error_value);

    retval = SNMP_ERR_NOERROR;

return retval;
}


netsnmp_handler_registration * netsnmp_create_handler_registration(
                                    const char * name, Netsnmp_Node_Handler * handler_access_method,
                                    const oid * reg_oid,
                                    size_t reg_oid_len,
                                    int modes)
{
    mock()
        .actualCall("netsnmp_create_handler_registration");

    return (netsnmp_handler_registration *)mock().pointerReturnValue();
}

int netsnmp_register_handler(netsnmp_handler_registration * reginfo)
{
    mock()
        .actualCall("netsnmp_register_handler")
        .withParameter("reginfo", (void *)reginfo);

    return SNMPERR_SUCCESS;
}

TEST_GROUP(init_sub_agent)
{
    void setup()
    {

    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(init_sub_agent, init_sub_agent_passed_return_value_from_netsnmp_create_handler_registration)
{
    int dummy_pointer;

    mock().strictOrder();

    mock()
        .expectOneCall("netsnmp_create_handler_registration")
        .andReturnValue((void *)&dummy_pointer);

    mock()
        .expectOneCall("netsnmp_register_handler")
        .withParameter("reginfo", (void *)&dummy_pointer);
    init_sub_agent();
    mock().checkExpectations();
}

TEST_GROUP(sub_agent_handler)
{
    void setup()
    {

    }
    void teardown()
    {
        mock().clear();
    }

};

TEST(sub_agent_handler, null_request_returns_success)
{
    int actual_result;
    int const expected_result = SNMP_ERR_NOERROR;

    actual_result = sub_agent_handler(NULL, NULL, NULL, NULL);

    CHECK_EQUAL(expected_result, actual_result);
}

TEST(sub_agent_handler, mode_set_begin_returns_success)
{
    int actual_result;
    int const expected_result = SNMP_ERR_NOERROR;
    netsnmp_agent_request_info request_info;
    netsnmp_request_info request;

    /* setup */
    request_info.mode = MODE_SET_BEGIN;
    request.next = NULL;

    mock()
        .expectOneCall("handle_set_request")
        .withParameter("var", (void *)request.requestvb);

    actual_result = sub_agent_handler(NULL, NULL, &request_info, &request);

    CHECK_EQUAL(expected_result, actual_result);
}

TEST(sub_agent_handler, mode_get_calls_handle_get_request_returns_success)
{
    int actual_result;
    int const expected_result = SNMP_ERR_NOERROR;
    netsnmp_agent_request_info request_info;
    netsnmp_request_info request;

    /* setup */
    request.next = NULL;
    request_info.mode = MODE_GET;
    request.requestvb = NULL;
    mock()
        .expectOneCall("handle_get_request")
        .withParameter("var", (void *)request.requestvb);

    actual_result = sub_agent_handler(NULL, NULL, &request_info, &request);

    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();
}

TEST(sub_agent_handler, mode_get_varbindings_passed_along)
{
    netsnmp_agent_request_info request_info;
    netsnmp_request_info request;
    netsnmp_variable_list variable;

    /* setup */
    request.next = NULL;
    request.requestvb = &variable;
    request_info.mode = MODE_GET;

    mock()
        .expectOneCall("handle_get_request")
        .withParameter("var", (void *)request.requestvb);

    (void)sub_agent_handler(NULL, NULL, &request_info, &request);

    mock().checkExpectations();
}

TEST(sub_agent_handler, mode_getnext_calls_handle_get_next_request_returns_success)
{
    int actual_result;
    int const expected_result = SNMP_ERR_NOERROR;
    netsnmp_agent_request_info request_info;
    netsnmp_request_info request;

    /* setup */
    request.next = NULL;
    request.requestvb = NULL;
    request_info.mode = MODE_GETNEXT;
    mock()
        .expectOneCall("handle_get_next_request")
        .withParameter("var", (void *)request.requestvb);

    actual_result = sub_agent_handler(NULL, NULL, &request_info, &request);

    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();
}

TEST(sub_agent_handler, mode_getnext_varbindings_passed_along)
{
    netsnmp_agent_request_info request_info;
    netsnmp_request_info request;
    netsnmp_variable_list variable;

    /* setup */
    request.next = NULL;
    request.requestvb = &variable;
    request_info.mode = MODE_GETNEXT;
    mock()
        .expectOneCall("handle_get_next_request")
        .withParameter("var", (void *)request.requestvb);

    (void)sub_agent_handler(NULL, NULL, &request_info, &request);

    mock().checkExpectations();
}

TEST(sub_agent_handler, mode_get_multiple_calls)
{
    netsnmp_agent_request_info request_info;
    netsnmp_request_info request;
    netsnmp_request_info request2;
    netsnmp_variable_list variable;
    netsnmp_variable_list variable2;

    /* setup */
    request.next = &request2;
    request.requestvb = &variable;
    request2.requestvb = &variable2;
    request2.next = NULL;

    request_info.mode = MODE_GET;
    mock()
        .expectOneCall("handle_get_request")
        .withParameter("var", (void *)request.requestvb);
    mock()
        .expectOneCall("handle_get_request")
        .withParameter("var", (void *)request2.requestvb);

    (void)sub_agent_handler(NULL, NULL, &request_info, &request);

    mock().checkExpectations();
}

