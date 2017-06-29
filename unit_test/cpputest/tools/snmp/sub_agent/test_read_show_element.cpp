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
    #include <net-snmp/net-snmp-config.h>
    #include <net-snmp/net-snmp-includes.h>
    #include "read_show_element.h"
    #include "config_mgr.h"
}

config_status_t tlr_status_get_string_ptr(tlr_handle_t const handle, size_t const element_id, char const ** const string_ptr)
{
    mock()
        .actualCall("tlr_status_get_string_ptr");
    mock("args_passed")
        .actualCall("tlr_status_get_string_ptr")
        .withParameter("handle", handle)
        .withParameter("element_id", element_id);
    if (mock("args_passed").hasReturnValue())
    {
        void * data = mock("args_passed").getData("element_value").getPointerValue();
        *string_ptr = (char const *)data;

        return (config_status_t)mock("args_passed").intReturnValue();
    }
    {
        void * data = mock().getData("element_value").getPointerValue();
        *string_ptr = (char const *)data;

        return (config_status_t)mock().intReturnValue();
    }
}

TEST_GROUP(read_show_element_as_string)
{
    void * value_buffer;

    void setup()
    {
        value_buffer = NULL;
    }
    void teardown()
    {
        free(value_buffer);
        mock().clear();
    }
};

TEST(read_show_element_as_string, tlr_status_get_string_ptr_success_results_in_no_error)
{
    char element_value[] = "some string";
    tlr_handle_t handle = NULL;
    int actual_result;
    int expected_result;

    /* setup */
    mock().setData("element_value", (void *)element_value);
    mock()
        .expectOneCall("tlr_status_get_string_ptr")
        .andReturnValue((int)config_status_success);
    mock("args_passed").ignoreOtherCalls();
    expected_result = SNMP_ERR_NOERROR;

    /* perform test */
    actual_result = read_show_element_as_string(handle, 0, &value_buffer);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();

    /* cleanup */
}

TEST(read_show_element_as_string, tlr_status_get_string_ptr_failure_results_in_no_such_name)
{
    char element_value[] = "some string";
    tlr_handle_t handle = NULL;
    int actual_result;
    int expected_result;

    /* setup */
    mock().setData("element_value", (void *)element_value);
    mock()
        .expectOneCall("tlr_status_get_string_ptr")
        .andReturnValue((int)config_status_action_not_found);
    mock("args_passed").ignoreOtherCalls();
    expected_result = SNMP_ERR_NOSUCHNAME;

    /* perform test */
    actual_result = read_show_element_as_string(handle, 0, &value_buffer);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);

    /* cleanup */
}

TEST(read_show_element_as_string, element_value_duplicated_into_result_buffer)
{
    char element_value[] = "some string";
    tlr_handle_t handle = NULL;

    /* setup */
    mock().setData("element_value", (void *)element_value);
    mock()
        .expectOneCall("tlr_status_get_string_ptr")
        .andReturnValue((int)config_status_success);
    mock("args_passed").ignoreOtherCalls();

    /* perform test */
    (void)read_show_element_as_string(handle, 0, &value_buffer);

    /* check results */
    STRCMP_EQUAL(element_value, (char *)value_buffer);

    /* cleanup */
}

TEST(read_show_element_as_string, handle_and_element_id_passed_to_get_string_ptr)
{
    char element_value[] = "some string";
    tlr_handle_t handle;
    size_t element_id;

    /* setup */
    handle =  (void *)5678;
    element_id = 1234;
    mock("args_passed")
        .expectOneCall("tlr_status_get_string_ptr")
        .withParameter("handle", handle)
        .withParameter("element_id", element_id)
        .andReturnValue((int)config_status_success);
    mock("args_passed").setData("element_value", (void *)element_value);
    mock().ignoreOtherCalls();

    /* perform test */
    (void)read_show_element_as_string(handle, element_id, &value_buffer);

    /* check results */
    mock("args_passed").checkExpectations();

    /* cleanup */
}

size_t tlr_get_element_size(config_mgr_element_t const * const element)
{
    mock()
        .actualCall("tlr_get_element_size");
    mock("element_size")
        .actualCall("tlr_get_element_size");
    mock("element")
        .actualCall("tlr_get_element_size")
        .withParameter("element", (void *)element);

    if (mock("element_size").hasReturnValue())
    {
        return (size_t)mock("element_size").intReturnValue();
    }

    if (mock().hasReturnValue())
    {
        return (size_t)mock().intReturnValue();
    }

    return 1;   /* some positive value */
}

config_status_t tlr_status_read(tlr_handle_t const handle, size_t const element_id, void * const data, size_t const bytes)
{
    mock()
        .actualCall("tlr_status_read");
    mock("element_size")
        .actualCall("tlr_status_read")
        .withParameter("bytes", (int)bytes);
    mock("handle")
        .actualCall("tlr_status_read")
        .withParameter("handle", (void *)handle)
        .withParameter("element_id", (int)element_id);

    if (mock("element_size").hasReturnValue())
    {
        return (config_status_t)mock("tlr_status_read").intReturnValue();
    }

    if (mock().hasReturnValue())
    {
        return (config_status_t)mock().intReturnValue();
    }

    return config_status_success;
}

TEST_GROUP(read_show_element)
{
    void * value_buffer;

    void setup()
    {
        value_buffer = NULL;
    }
    void teardown()
    {
        free(value_buffer);
        mock().clear();
    }
};

TEST(read_show_element, tlr_status_read_success_results_in_no_error)
{
    tlr_handle_t handle = NULL;
    config_mgr_element_t element;
    int actual_result;
    int expected_result;

    /* setup */
    mock().strictOrder();
    mock()
        .expectOneCall("tlr_get_element_size")
        .andReturnValue(1);
    mock()
        .expectOneCall("tlr_status_read")
        .andReturnValue((int)config_status_success);
    mock("element_size").ignoreOtherCalls();
    mock("element").ignoreOtherCalls();
    mock("handle").ignoreOtherCalls();

    /* perform test */
    actual_result = read_show_element(handle, &element, &value_buffer);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    /* check that value_buffer is also non-NULL (should be malloced memory) */
    CHECK_TRUE(value_buffer != NULL);
    mock().checkExpectations();

    /* cleanup */
}

TEST(read_show_element, tlr_status_read_failure_results_in_no_such_name)
{
    tlr_handle_t handle = NULL;
    config_mgr_element_t element;
    int actual_result;
    int expected_result;

    /* setup */
    mock().strictOrder();
    mock()
        .expectOneCall("tlr_get_element_size")
        .andReturnValue(1);
    mock()
        .expectOneCall("tlr_status_read")
        .andReturnValue((int)config_status_action_not_found);
    mock("element_size").ignoreOtherCalls();
    mock("element").ignoreOtherCalls();
    mock("handle").ignoreOtherCalls();
    expected_result = SNMP_ERR_NOSUCHNAME;

    /* perform test */
    actual_result = read_show_element(handle, &element, &value_buffer);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);

    /* cleanup */
}

TEST(read_show_element, element_size_passed_to_status_read)
{
    tlr_handle_t handle = NULL;
    config_mgr_element_t element;
    size_t expected_element_size;

    /* setup */
    expected_element_size = 123;
    mock("element_size").strictOrder();
    mock("element_size")
        .expectOneCall("tlr_get_element_size")
        .andReturnValue((int)expected_element_size);

    mock("element_size")
        .expectOneCall("tlr_status_read")
        .withParameter("bytes", (int)expected_element_size)
        .andReturnValue((int)config_status_success);
    mock().ignoreOtherCalls();
    mock("element").ignoreOtherCalls();
    mock("handle").ignoreOtherCalls();

    /* perform test */
    (void)read_show_element(handle, &element, &value_buffer);

    /* check results */
    mock("element_size").checkExpectations();

    /* cleanup */
}

TEST(read_show_element, element_passed_to_get_size)
{
    tlr_handle_t handle = NULL;
    config_mgr_element_t element;

    /* setup */
    mock("element")
        .expectOneCall("tlr_get_element_size")
        .withParameter("element", (void *)&element);
    mock().ignoreOtherCalls();
    mock("element_size").ignoreOtherCalls();
    mock("handle").ignoreOtherCalls();

    /* perform test */
    (void)read_show_element(handle, &element, &value_buffer);

    /* check results */
    mock("element").checkExpectations();

    /* cleanup */
}

TEST(read_show_element, handle_and_element_id_passed_to_status_read)
{
    tlr_handle_t handle;
    config_mgr_element_t element;

    /* setup */
    handle = (tlr_handle_t)1234;
    element.id = 5678;
    mock("handle")
        .expectOneCall("tlr_status_read")
        .withParameter("handle", (void *)handle)
        .withParameter("element_id", (int)element.id)
        .andReturnValue((int)config_status_success);
    mock().ignoreOtherCalls();
    mock("element_size").ignoreOtherCalls();
    mock("element").ignoreOtherCalls();

    /* perform test */
    (void)read_show_element(handle, &element, &value_buffer);

    /* check results */
    mock("handle").checkExpectations();

    /* cleanup */
}

