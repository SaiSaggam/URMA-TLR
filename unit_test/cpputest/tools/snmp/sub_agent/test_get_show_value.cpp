/*****************************************************************************
 * Copyright (c) 2016 Digi International Inc., All Rights Reserved
 *
 * This software contains proprietary and confidential information of Digi
 * International Inc.  By accepting transfer of this copy, Recipient agrees
 * to retain this software in confidence, to prevent disclosure to others,
 * and to make no use of this software other than that for which it was
 * delivered.  This is an unpublished copyrighted work of Digi International
 * Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
 * prohibited.
 *
 * Restricted Rights Legend
 *
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7031 or
 * subparagraphs (c)(1) and (2) of the Commercial Computer Software -
 * Restricted Rights at 48 CFR 52.227-19, as applicable.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 *
 *****************************************************************************/
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
    #include "get_show_value.h"
    #include "config_mgr.h"
    #include "read_show_element.h"
    #include "snmp_utils.h"
}

int snmp_adjust_element(u_char const asn_type, size_t * const element_size, void * * const value_buffer)
{
    mock("happy")
        .actualCall("snmp_adjust_element")
        .withParameter("asn_type", asn_type);

    if (mock("happy").hasReturnValue())
    {
        return mock("happy").returnValue().getIntValue();
    }

    /* default is to return success */
    return 0;
}

tlr_handle_t tlr_status_open(size_t const group_id, size_t const instance)
{
    mock("happy").actualCall("tlr_status_open");
    mock("failed_read").actualCall("tlr_status_open");
    mock("check_group")
        .actualCall("tlr_status_open")
        .withParameter("group_id", group_id)
        .withParameter("instance", instance);

    if (mock("happy").hasReturnValue())
    {
        return (tlr_handle_t)mock("happy").returnValue().getPointerValue();
    }
    if (mock("check_group").hasReturnValue())
    {
        return (tlr_handle_t)mock("check_group").returnValue().getPointerValue();
    }
    if (mock("failed_read").hasReturnValue())
    {
        return (tlr_handle_t)mock("failed_read").returnValue().getPointerValue();
    }

    /* default is to return NULL */
    return (tlr_handle_t)NULL;
}

void tlr_status_close(tlr_handle_t handle)
{
    mock("happy")
        .actualCall("tlr_status_close")
        .withParameter("handle", (void *)handle);
}

int read_show_element_as_string(tlr_handle_t handle, size_t element_id, void * * value_buffer)
{
    mock("happy")
        .actualCall("read_show_element_as_string")
        .withParameter("handle", (void *)handle)
        .withParameter("element_id", (int)element_id);
    mock("buffer")
        .actualCall("read_show_element_as_string")
        .withParameter("value_buffer", (void *)value_buffer);
    mock("failed_read")
        .actualCall("read_show_element_as_string");

    if (mock("happy").hasReturnValue())
    {
        return mock("happy").returnValue().getIntValue();
    }
    if (mock("failed_read").hasReturnValue())
    {
        return mock("failed_read").returnValue().getIntValue();
    }

    return SNMP_ERR_NOERROR;
}

int read_show_element(tlr_handle_t handle, config_mgr_element_t const * const element, void * * value_buffer)
{
    mock("happy")
        .actualCall("read_show_element")
        .withParameter("handle", (void *)handle)
        .withParameter("element", (void *)element);
    mock("buffer")
        .actualCall("read_show_element")
        .withParameter("value_buffer", (void *)value_buffer);
    mock("failed_read")
        .actualCall("read_show_element");

    if (mock("happy").hasReturnValue())
    {
        return mock("happy").returnValue().getIntValue();
    }
    if (mock("failed_read").hasReturnValue())
    {
        return mock("failed_read").returnValue().getIntValue();
    }

    return SNMP_ERR_NOERROR;
}

TEST_GROUP(get_show_group_element_value)
{
    void setup()
    {
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(get_show_group_element_value, happy_string_path_results_in_no_error)
{
    config_mgr_group_t group;
    config_mgr_element_t element;
    int tlr_handle;
    int actual_result;
    int expected_result;
    int asn_type;

    /* setup */
    asn_type = ASN_OCTET_STR;
    element.id = 1234;
    mock("happy").strictOrder();
    mock("happy").expectOneCall("tlr_status_open").andReturnValue((void *)&tlr_handle);
    mock("happy")
        .expectOneCall("read_show_element_as_string")
        .withParameter("handle", (void *)&tlr_handle)
        .withParameter("element_id", element.id)    /* also test that element.id is passed */
        .andReturnValue(SNMP_ERR_NOERROR);
    mock("happy").expectOneCall("snmp_adjust_element").withParameter("asn_type", asn_type).andReturnValue(0);
    mock("happy").expectOneCall("tlr_status_close").withParameter("handle", (void *)&tlr_handle);
    mock("check_group").ignoreOtherCalls();
    mock("failed_read").ignoreOtherCalls();
    mock("buffer").ignoreOtherCalls();
    expected_result = SNMP_ERR_NOERROR;

    /* perform test */
    actual_result = get_show_group_element_value(&group, 0, &element, asn_type, NULL, NULL);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock("happy").checkExpectations();

    /* cleanup */
}

TEST(get_show_group_element_value, happy_integer_path_results_in_no_error)
{
    config_mgr_group_t group;
    config_mgr_element_t element;
    int tlr_handle;
    int actual_result;
    int expected_result;
    int asn_type;

    /* setup */
    asn_type = ASN_INTEGER;
    mock("happy").strictOrder();
    mock("happy").expectOneCall("tlr_status_open").andReturnValue((void *)&tlr_handle);
    mock("happy")
        .expectOneCall("read_show_element")
        .withParameter("handle", (void *)&tlr_handle)
        .withParameter("element", (void *)&element) /* also test that element is passed */
        .andReturnValue(SNMP_ERR_NOERROR);
    mock("happy").expectOneCall("snmp_adjust_element").withParameter("asn_type", asn_type).andReturnValue(0);
    mock("happy").expectOneCall("tlr_status_close").withParameter("handle", (void *)&tlr_handle);
    mock("check_group").ignoreOtherCalls();
    mock("failed_read").ignoreOtherCalls();
    mock("buffer").ignoreOtherCalls();
    expected_result = SNMP_ERR_NOERROR;

    /* perform test */
    actual_result = get_show_group_element_value(&group, 0, &element, asn_type, NULL, NULL);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock("happy").checkExpectations();

    /* cleanup */
}

TEST(get_show_group_element_value, failed_tlr_status_open_results_in_gen_error)
{
    config_mgr_group_t group;
    config_mgr_element_t element;
    int actual_result;
    int expected_result;

    /* setup */
    mock("happy").strictOrder();
    mock("happy").expectOneCall("tlr_status_open").andReturnValue((void *)NULL);
    mock("happy").expectOneCall("tlr_status_close").withParameter("handle", (void *)NULL);
    mock("check_group").ignoreOtherCalls();
    mock("failed_read").ignoreOtherCalls();
    mock("buffer").ignoreOtherCalls();
    expected_result = SNMP_ERR_GENERR;

    /* perform test */
    actual_result = get_show_group_element_value(&group, 0, &element, ASN_INTEGER, NULL, NULL);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock("happy").checkExpectations();

    /* cleanup */
}

TEST(get_show_group_element_value, failed_read_string_results_in_error_from_read_string)
{
    config_mgr_group_t group;
    config_mgr_element_t element;
    int actual_result;
    int expected_result;
    int read_string_error = 32;
    int tlr_handle;
    u_char asn_type;

    /* setup */
    mock("failed_read").strictOrder();
    mock("failed_read").expectOneCall("tlr_status_open").andReturnValue((void *)&tlr_handle);
    mock("failed_read").expectOneCall("read_show_element_as_string").andReturnValue(read_string_error);
    mock("happy").ignoreOtherCalls();
    mock("check_group").ignoreOtherCalls();
    mock("buffer").ignoreOtherCalls();
    expected_result = read_string_error;
    asn_type = ASN_OCTET_STR;

    /* perform test */
    actual_result = get_show_group_element_value(&group, 0, &element, asn_type, NULL, NULL);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock("happy").checkExpectations();

    /* cleanup */
}

TEST(get_show_group_element_value, failed_read_int_results_in_error_from_read)
{
    config_mgr_group_t group;
    config_mgr_element_t element;
    int actual_result;
    int expected_result;
    int read_error = 32;
    int tlr_handle;
    u_char asn_type;

    /* setup */
    mock("failed_read").strictOrder();
    mock("failed_read").expectOneCall("tlr_status_open").andReturnValue((void *)&tlr_handle);
    mock("failed_read").expectOneCall("read_show_element").andReturnValue(read_error);
    mock("happy").ignoreOtherCalls();
    mock("check_group").ignoreOtherCalls();
    mock("buffer").ignoreOtherCalls();
    expected_result = read_error;
    asn_type = ASN_INTEGER;

    /* perform test */
    actual_result = get_show_group_element_value(&group, 0, &element, asn_type, NULL, NULL);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock("happy").checkExpectations();

    /* cleanup */
}

TEST(get_show_group_element_value, failed_adjust_results_in_gen_error)
{
    config_mgr_group_t group;
    config_mgr_element_t element;
    int tlr_handle;
    int actual_result;
    int expected_result;
    int asn_type;

    /* setup */
    asn_type = ASN_OCTET_STR;
    element.id = 1234;
    mock("happy").strictOrder();
    mock("happy").expectOneCall("tlr_status_open").andReturnValue((void *)&tlr_handle);
    mock("happy").expectOneCall("read_show_element_as_string")
        .withParameter("handle", (void *)&tlr_handle)
        .withParameter("element_id", element.id)
        .andReturnValue(SNMP_ERR_NOERROR);
    mock("happy").expectOneCall("snmp_adjust_element").withParameter("asn_type", asn_type).andReturnValue(-1);
    mock("happy").expectOneCall("tlr_status_close").withParameter("handle", (void *)&tlr_handle);
    mock("check_group").ignoreOtherCalls();
    mock("failed_read").ignoreOtherCalls();
    mock("buffer").ignoreOtherCalls();
    expected_result = SNMP_ERR_GENERR;

    /* perform test */
    actual_result = get_show_group_element_value(&group, 0, &element, asn_type, NULL, NULL);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock("happy").checkExpectations();

    /* cleanup */
}

TEST(get_show_group_element_value, group_id_and_instance_passed_to_tlr_open)
{
    config_mgr_group_t group;
    size_t instance;

    /* setup */
    group.id = 1234;
    instance = 5678;
    mock("check_group").strictOrder();
    mock("check_group")
        .expectOneCall("tlr_status_open")
        .withParameter("group_id", group.id)
        .withParameter("instance", instance)
        .andReturnValue((void *)NULL);
    mock("happy").ignoreOtherCalls();
    mock("failed_read").ignoreOtherCalls();
    mock("buffer").ignoreOtherCalls();

    /* perform test */
    (void)get_show_group_element_value(&group, instance, NULL, ASN_INTEGER, NULL, NULL);

    /* check results */
    mock("check_group").checkExpectations();

    /* cleanup */
}

TEST(get_show_group_element_value, unknown_asn_type_results_in_gen_error)
{
    config_mgr_group_t group;
    config_mgr_element_t element;
    int tlr_handle;
    int actual_result;
    int expected_result;
    int asn_type;

    /* setup */
    asn_type = ASN_TIMETICKS;
    mock("happy").strictOrder();
    mock("happy").expectOneCall("tlr_status_open").andReturnValue((void *)&tlr_handle);
    mock("happy").expectOneCall("tlr_status_close").withParameter("handle", (void *)&tlr_handle);
    mock("check_group").ignoreOtherCalls();
    mock("failed_read").ignoreOtherCalls();
    mock("buffer").ignoreOtherCalls();
    expected_result = SNMP_ERR_GENERR;

    /* perform test */
    actual_result = get_show_group_element_value(&group, 0, &element, asn_type, NULL, NULL);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock("happy").checkExpectations();

    /* cleanup */
}

TEST(get_show_group_element_value, buffer_passed_to_read_show_element_as_string)
{
    config_mgr_group_t group;
    config_mgr_element_t element;
    int tlr_handle;
    int actual_result;
    int expected_result;
    int asn_type;
    int temp;
    void * value_buffer = &temp;

    /* setup */
    asn_type = ASN_OCTET_STR;
    mock("buffer").strictOrder();
    mock("buffer")
        .expectOneCall("read_show_element_as_string")
        .withParameter("value_buffer", &value_buffer);
    mock("buffer").expectOneCall("snmp_adjust_element")
        .withParameter("value_buffer", &value_buffer);

    mock("happy").ignoreOtherCalls();
    mock("check_group").ignoreOtherCalls();
    mock("failed_read").ignoreOtherCalls();

    /* perform test */
    (void)get_show_group_element_value(&group, 0, &element, asn_type, &value_buffer, NULL);

    /* check results */
    mock("happy").checkExpectations();

    /* cleanup */
}

TEST(get_show_group_element_value, buffer_passed_to_read_show_element)
{
    config_mgr_group_t group;
    config_mgr_element_t element;
    int tlr_handle;
    int actual_result;
    int expected_result;
    int asn_type;
    int temp;
    void * value_buffer = &temp;

    /* setup */
    asn_type = ASN_INTEGER;
    mock("buffer").strictOrder();
    mock("buffer")
        .expectOneCall("read_show_element")
        .withParameter("value_buffer", &value_buffer);
    mock("buffer").expectOneCall("snmp_adjust_element")
        .withParameter("value_buffer", &value_buffer);

    mock("happy").ignoreOtherCalls();
    mock("check_group").ignoreOtherCalls();
    mock("failed_read").ignoreOtherCalls();

    /* perform test */
    (void)get_show_group_element_value(&group, 0, &element, asn_type, &value_buffer, NULL);

    /* check results */
    mock("happy").checkExpectations();

    /* cleanup */
}

