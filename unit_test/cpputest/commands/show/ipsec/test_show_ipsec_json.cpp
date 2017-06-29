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
    #include "show_ipsec_json.h"
    #include "show_ipsec.h"
    #include "utils.h"

    #include "config_mgr.h"
}

tlr_show_object_t * tlr_show_object_create(size_t const group_id, size_t const instance)
{
    mock().actualCall("tlr_show_object_create");

    if (mock().hasReturnValue())
    {
        return (tlr_show_object_t *)mock().pointerReturnValue();
    }
    return NULL;
}

void tlr_show_object_destroy(tlr_show_object_t * const show_object)
{
    mock().actualCall("tlr_show_object_destroy");
}

char const * tlr_show_object_to_json_string(tlr_show_object_t const * const show_object)
{
    mock().actualCall("tlr_show_object_to_json_string");

    if (mock().hasReturnValue())
    {
        return (char const *)mock().pointerReturnValue();
    }
    return NULL;
}

void tunnel_status_context_init(tunnel_status_t * ptr_tunnel, size_t instance)
{
    mock().actualCall("tunnel_status_context_init");
}

void tunnel_status_context_cleanup(tunnel_status_t * ptr_tunnel)
{
    mock().actualCall("tunnel_status_context_cleanup");
}

bool read_all_tunnel_status_data(tunnel_status_t * ptr_tunnel)
{
    bool return_value;

    mock().actualCall("read_all_tunnel_status_data");

    if (mock().hasReturnValue())
    {
        return_value = (bool)mock().returnValue().getIntValue();
    }
    else
    {
        return_value = true;
    }

    return return_value;
}

config_mgr_group_t const * tlr_get_group_ptr(size_t const group_id, size_t const instance, config_mgr_group_type_t const table_index)
{
    config_mgr_group_t * group;

    mock().actualCall("tlr_get_group_ptr");

    if (mock().hasReturnValue())
    {
        group = (config_mgr_group_t *)mock().pointerReturnValue();
    }
    else
    {
        group = NULL;
    }
    return group;
}

void tlr_show_object_add_element(tlr_show_object_t * const show_object, size_t const element_id, void const * const value)
{
    mock()
        .actualCall("tlr_show_object_add_element")
        .withParameter("element_id", element_id);
    mock("network_string")
        .actualCall("tlr_show_object_add_element")
        .withParameter("value", (void *)value);
}

uint32_t parse_ipsec_uptime_seconds(char const * const original_uptime_str)
{
    return 0;
}

char * parse_network(char const * const cidr_network)
{
    char * return_value;

    mock("network_string")
        .actualCall("parse_network");
    if (mock("network_string").hasReturnValue())
    {
        return_value = (char *)mock("network_string").pointerReturnValue();
    }
    else
    {
        return_value = NULL;
    }
    return return_value;
}

char * parse_netmask(char const * const cidr_network)
{
    char * return_value;

    mock("network_string")
        .actualCall("parse_netmask");
    if (mock("network_string").hasReturnValue())
    {
        return_value = (char *)mock("network_string").pointerReturnValue();
    }
    else
    {
        return_value = NULL;
    }
    return return_value;
}

TEST_GROUP(show_ipsec_json)
{
    int show_object;
    config_mgr_group_t group;

    void setup()
    {
        memset(&group, 0, sizeof group);
    }
    void teardown()
    {
        mock().clear();
    }

    void parse_network_test(size_t element_id)
    {
        bool actual_result;
        static char json_string[] = "";
        config_mgr_element_t elements[1];
        char * network_string;
        network_string = strdup("network_string");

        /* setup */
        group.elements.element = elements;
        group.elements.count = 1;
        memset(elements, 0, sizeof elements);
        elements[0].id = element_id;
        mock().strictOrder();
        mock()
            .expectOneCall("tunnel_status_context_init");
        mock()
            .expectOneCall("tlr_show_object_create")
            .andReturnValue((void *)&show_object);
        mock()
            .expectOneCall("read_all_tunnel_status_data");

        mock()
            .expectOneCall("tlr_get_group_ptr")
            .andReturnValue((void *)&group);
        mock("network_string")
            .expectOneCall("parse_network")
            .andReturnValue((void *)network_string);
        mock("network_string")
            .expectOneCall("tlr_show_object_add_element")
            .withParameter("value", (void *)network_string);
        mock()
            .expectOneCall("tlr_show_object_to_json_string")
            .andReturnValue((void *)json_string);
        mock()
            .expectOneCall("tunnel_status_context_cleanup");
        mock()
            .expectOneCall("tlr_show_object_destroy");
        mock().ignoreOtherCalls();

        /* perform test */
        actual_result = show_ipsec_json(1);

        /* check results */
        CHECK_EQUAL(true, actual_result);
        mock().checkExpectations();

        /* cleanup */
    }

    void parse_netmask_test(size_t const element_id)
    {
        bool actual_result;
        static char json_string[] = "";
        config_mgr_element_t elements[1];
        char * netmask_string;
        netmask_string = strdup("netmask_string");

        /* setup */
        group.elements.element = elements;
        group.elements.count = 1;
        memset(elements, 0, sizeof elements);
        elements[0].id = element_id;
        mock().strictOrder();
        mock()
            .expectOneCall("tunnel_status_context_init");
        mock()
            .expectOneCall("tlr_show_object_create")
            .andReturnValue((void *)&show_object);
        mock()
            .expectOneCall("read_all_tunnel_status_data");

        mock()
            .expectOneCall("tlr_get_group_ptr")
            .andReturnValue((void *)&group);
        mock("network_string")
            .expectOneCall("parse_netmask")
            .andReturnValue((void *)netmask_string);
        mock("network_string")
            .expectOneCall("tlr_show_object_add_element")
            .withParameter("value", (void *)netmask_string);
        mock()
            .expectOneCall("tlr_show_object_to_json_string")
            .andReturnValue((void *)json_string);
        mock()
            .expectOneCall("tunnel_status_context_cleanup");
        mock()
            .expectOneCall("tlr_show_object_destroy");
        mock().ignoreOtherCalls();

        /* perform test */
        actual_result = show_ipsec_json(1);

        /* check results */
        CHECK_EQUAL(true, actual_result);
        mock().checkExpectations();

        /* cleanup */
    }


};

TEST(show_ipsec_json, instance_0_returns_failure)
{
    bool actual_result;
    bool expected_result;

    /* setup */
    expected_result = false;
    mock().strictOrder();
    mock()
        .expectOneCall("tunnel_status_context_init");
    mock()
        .expectOneCall("tunnel_status_context_cleanup");
    mock()
        .expectOneCall("tlr_show_object_destroy");
    mock("network_string")
        .ignoreOtherCalls();

    /* perform test */
    actual_result = show_ipsec_json(0);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();

    /* cleanup */
}

TEST(show_ipsec_json, instance_1_no_elements_returns_success)
{
    bool actual_result;
    static char json_string[] = "";
    bool expected_result;

    /* setup */
    expected_result = true;
    mock().strictOrder();
    mock()
        .expectOneCall("tunnel_status_context_init");
    mock()
        .expectOneCall("tlr_show_object_create")
        .andReturnValue((void *)&show_object);
    mock()
        .expectOneCall("read_all_tunnel_status_data");
    mock()
        .expectOneCall("tlr_get_group_ptr")
        .andReturnValue((void *)&group);
    mock()
        .expectOneCall("tlr_show_object_to_json_string")
        .andReturnValue((void *)json_string);
    mock()
        .expectOneCall("tunnel_status_context_cleanup");
    mock()
        .expectOneCall("tlr_show_object_destroy");
    mock("network_string")
        .ignoreOtherCalls();

    /* perform test */
    actual_result = show_ipsec_json(1);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();

    /* cleanup */
}

TEST(show_ipsec_json, check_single_element)
{
    bool actual_result;
    static char json_string[] = "";
    config_mgr_element_t elements[1];
    size_t element_id;
    bool expected_result;

    /* setup */
    expected_result = true;
    group.elements.element = elements;
    group.elements.count = 1;
    memset(elements, 0, sizeof elements);
    element_id = config_state_ipsec_admin_status;
    elements[0].id = element_id;
    mock().strictOrder();
    mock()
        .expectOneCall("tunnel_status_context_init");
    mock()
        .expectOneCall("tlr_show_object_create")
        .andReturnValue((void *)&show_object);
    mock()
        .expectOneCall("read_all_tunnel_status_data");

    mock()
        .expectOneCall("tlr_get_group_ptr")
        .andReturnValue((void *)&group);
    mock()
        .expectOneCall("tlr_show_object_add_element")
        .withParameter("element_id", element_id);
    mock()
        .expectOneCall("tlr_show_object_to_json_string")
        .andReturnValue((void *)json_string);
    mock()
        .expectOneCall("tunnel_status_context_cleanup");
    mock()
        .expectOneCall("tlr_show_object_destroy");
    mock("network_string")
        .ignoreOtherCalls();

    /* perform test */
    actual_result = show_ipsec_json(1);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();

    /* cleanup */
}

TEST(show_ipsec_json, check_multiple_elements)
{
    bool actual_result;
    static char json_string[] = "";
    config_mgr_element_t elements[2];
    size_t element_id[2];
    bool expected_result;

    /* setup */
    expected_result = true;
    group.elements.element = elements;
    group.elements.count = 2;
    memset(elements, 0, sizeof elements);
    element_id[0] = config_state_ipsec_admin_status;
    elements[0].id = element_id[0];
    element_id[1] = config_state_ipsec_esp_ciphers;
    elements[1].id = element_id[1];
    mock().strictOrder();
    mock()
        .expectOneCall("tunnel_status_context_init");
    mock()
        .expectOneCall("tlr_show_object_create")
        .andReturnValue((void *)&show_object);
    mock()
        .expectOneCall("read_all_tunnel_status_data");

    mock()
        .expectOneCall("tlr_get_group_ptr")
        .andReturnValue((void *)&group);
    mock()
        .expectOneCall("tlr_show_object_add_element")
        .withParameter("element_id", element_id[0]);
    mock()
        .expectOneCall("tlr_show_object_add_element")
        .withParameter("element_id", element_id[1]);
    mock()
        .expectOneCall("tlr_show_object_to_json_string")
        .andReturnValue((void *)json_string);
    mock()
        .expectOneCall("tunnel_status_context_cleanup");
    mock()
        .expectOneCall("tlr_show_object_destroy");
    mock("network_string")
        .ignoreOtherCalls();

    /* perform test */
    actual_result = show_ipsec_json(1);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();

    /* cleanup */
}

TEST(show_ipsec_json, failed_show_object_create)
{
    bool actual_result;
    bool expected_result;

    /* setup */
    expected_result = false;
    mock().strictOrder();
    mock()
        .expectOneCall("tunnel_status_context_init");
    mock()
        .expectOneCall("tlr_show_object_create")
        .andReturnValue((void *)NULL);
    mock()
        .expectOneCall("tunnel_status_context_cleanup");
    mock()
        .expectOneCall("tlr_show_object_destroy");
    mock("network_string")
        .ignoreOtherCalls();

    /* perform test */
    actual_result = show_ipsec_json(1);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();

    /* cleanup */
}

TEST(show_ipsec_json, failed_read_tunnel_status)
{
    bool actual_result;
    bool expected_result;

    /* setup */
    expected_result = false;
    mock().strictOrder();
    mock()
        .expectOneCall("tunnel_status_context_init");
    mock()
        .expectOneCall("tlr_show_object_create")
        .andReturnValue((void *)&show_object);
    mock()
        .expectOneCall("read_all_tunnel_status_data")
        .andReturnValue(false);

    mock()
        .expectOneCall("tunnel_status_context_cleanup");
    mock()
        .expectOneCall("tlr_show_object_destroy");
    mock("network_string")
        .ignoreOtherCalls();

    /* perform test */
    actual_result = show_ipsec_json(1);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();

    /* cleanup */
}

TEST(show_ipsec_json, local_network_passes_value_on_to_add_element)
{
    parse_network_test(config_state_ipsec_local_network);
}

TEST(show_ipsec_json, remote_network_passes_value_on_to_add_element)
{
    parse_network_test(config_state_ipsec_remote_network);
}

TEST(show_ipsec_json, local_netmask_passes_value_on_to_add_element)
{
    parse_netmask_test(config_state_ipsec_local_mask);
}

TEST(show_ipsec_json, remote_netmask_passes_value_on_to_add_element)
{
    parse_netmask_test(config_state_ipsec_remote_mask);
}


