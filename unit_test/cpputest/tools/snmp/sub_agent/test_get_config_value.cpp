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
    #include "snmp_agent_debug.h"
    #include "get_config_value.h"
    #include "config_mgr.h"
}
config_status_t tlr_get_element_alloc_value(size_t const group_id,
                                            size_t const instance,
                                            size_t const element_id,
                                            void ** value,
                                            config_mgr_element_type_t const type,
                                            config_mgr_access_level_t const access)
{
    mock()
        .actualCall("tlr_get_element_alloc_value")
        .withParameter("group_id", group_id)
        .withParameter("instance", instance)
        .withParameter("element_id", element_id)
        .withParameter("value", (void *)value)
        .withParameter("type", type);
        ;

    return (config_status_t)mock().intReturnValue();
}

int snmp_adjust_element(u_char const asn_type, size_t * const element_size, void * * const buffer)
{
    mock().actualCall("snmp_adjust_element");

    return mock().intReturnValue();
}

TEST_GROUP(get_config_value)
{
    void setup()
    {
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(get_config_value, happy_path_results_in_no_error)
{
    config_mgr_group_t group;
    size_t instance;
    config_mgr_element_t element;
    u_char asn_type;
    size_t element_size;
    int actual_result;
    int expected_result;
    config_mgr_element_type_t expected_type;

    /* setup */
    expected_type = config_mgr_element_type_string;
    element.type = expected_type;
    instance = 0;
    mock().strictOrder();
    mock()
        .expectOneCall("tlr_get_element_alloc_value")
        .withParameter("group_id", group.id)
        .withParameter("instance", 0)
        .withParameter("element_id", element.id)
        .withParameter("value", (void *)NULL)
        .withParameter("type", expected_type)
        .andReturnValue(config_status_success);
    mock()
        .expectOneCall("snmp_adjust_element")
        .andReturnValue(0);
    expected_result = SNMP_ERR_NOERROR;

    /* perform test */
    actual_result = get_config_group_element_value(&group, instance, &element, ASN_OCTET_STR, NULL, &element_size);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();
}

TEST(get_config_value, get_size_error_results_in_no_such_name)
{
    config_mgr_group_t group;
    size_t instance;
    config_mgr_element_t element;
    u_char asn_type;
    size_t element_size;
    int actual_result;
    int expected_result;
    config_mgr_element_type_t expected_type;

    /* setup */
    expected_type = config_mgr_element_type_string;
    element.type = expected_type;
    instance = 0;
    mock().strictOrder();
    mock()
        .expectOneCall("tlr_get_element_alloc_value")
        .withParameter("group_id", group.id)
        .withParameter("instance", 0)
        .withParameter("element_id", element.id)
        .withParameter("value", (void *)NULL)
        .withParameter("type", expected_type)
        .andReturnValue(config_status_success);
    mock()
        .expectOneCall("snmp_adjust_element")
        .andReturnValue(-1);
    expected_result = SNMP_ERR_NOSUCHNAME;

    /* perform test */
    actual_result = get_config_group_element_value(&group, instance, &element, ASN_OCTET_STR, NULL, &element_size);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();
}

TEST(get_config_value, group_not_found_results_in_no_such_name)
{
    config_mgr_group_t group;
    size_t instance;
    config_mgr_element_t element;
    u_char asn_type;
    size_t element_size;
    int actual_result;
    int expected_result;
    config_mgr_element_type_t expected_type;

    /* setup */
    expected_type = config_mgr_element_type_string;
    element.type = expected_type;
    instance = 0;
    mock().strictOrder();
    mock()
        .expectOneCall("tlr_get_element_alloc_value")
        .withParameter("group_id", group.id)
        .withParameter("instance", 0)
        .withParameter("element_id", element.id)
        .withParameter("value", (void *)NULL)
        .withParameter("type", expected_type)
        .andReturnValue(config_status_group_not_found);
    expected_result = SNMP_ERR_NOSUCHNAME;

    /* perform test */
    actual_result = get_config_group_element_value(&group, instance, &element, ASN_OCTET_STR, NULL, &element_size);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();
}

TEST(get_config_value, element_not_found_results_in_no_such_name)
{
    config_mgr_group_t group;
    size_t instance;
    config_mgr_element_t element;
    u_char asn_type;
    size_t element_size;
    int actual_result;
    int expected_result;
    config_mgr_element_type_t expected_type;

    /* setup */
    expected_type = config_mgr_element_type_string;
    element.type = expected_type;
    instance = 0;
    mock().strictOrder();
    mock()
        .expectOneCall("tlr_get_element_alloc_value")
        .withParameter("group_id", group.id)
        .withParameter("instance", 0)
        .withParameter("element_id", element.id)
        .withParameter("value", (void *)NULL)
        .withParameter("type", expected_type)
        .andReturnValue(config_status_element_not_found);
    expected_result = SNMP_ERR_NOSUCHNAME;

    /* perform test */
    actual_result = get_config_group_element_value(&group, instance, &element, ASN_OCTET_STR, NULL, &element_size);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();
}

TEST(get_config_value, instance_not_found_results_in_no_such_name)
{
    config_mgr_group_t group;
    size_t instance;
    config_mgr_element_t element;
    u_char asn_type;
    size_t element_size;
    int actual_result;
    int expected_result;
    config_mgr_element_type_t expected_type;

    /* setup */
    expected_type = config_mgr_element_type_string;
    element.type = expected_type;
    instance = 0;
    mock().strictOrder();
    mock()
        .expectOneCall("tlr_get_element_alloc_value")
        .withParameter("group_id", group.id)
        .withParameter("instance", 0)
        .withParameter("element_id", element.id)
        .withParameter("value", (void *)NULL)
        .withParameter("type", expected_type)
        .andReturnValue(config_status_instance_not_found);
    expected_result = SNMP_ERR_NOSUCHNAME;

    /* perform test */
    actual_result = get_config_group_element_value(&group, instance, &element, ASN_OCTET_STR, NULL, &element_size);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();
}

TEST(get_config_value, some_other_error_results_in_gen_error)
{
    config_mgr_group_t group;
    size_t instance;
    config_mgr_element_t element;
    u_char asn_type;
    size_t element_size;
    int actual_result;
    int expected_result;
    config_mgr_element_type_t expected_type;

    /* setup */
    expected_type = config_mgr_element_type_string;
    element.type = expected_type;
    instance = 0;
    mock().strictOrder();
    mock()
        .expectOneCall("tlr_get_element_alloc_value")
        .withParameter("group_id", group.id)
        .withParameter("instance", 0)
        .withParameter("element_id", element.id)
        .withParameter("value", (void *)NULL)
        .withParameter("type", expected_type)
        .andReturnValue(config_status_invalid_value);
    expected_result = SNMP_ERR_GENERR;

    /* perform test */
    actual_result = get_config_group_element_value(&group, instance, &element, ASN_OCTET_STR, NULL, &element_size);

    /* check results */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();
}

TEST(get_config_value, type_passed_to_alloc_value)
{
    config_mgr_group_t group;
    size_t instance;
    config_mgr_element_t element;
    u_char asn_type;
    size_t element_size;
    config_mgr_element_type_t expected_type;

    /* setup */
    expected_type = config_mgr_element_type_string;
    element.type = expected_type;
    instance = 0;
    mock().strictOrder();
    mock()
        .expectOneCall("tlr_get_element_alloc_value")
        .withParameter("group_id", group.id)
        .withParameter("instance", 0)
        .withParameter("element_id", element.id)
        .withParameter("value", (void *)NULL)
        .withParameter("type", expected_type)
        .andReturnValue(config_status_invalid_value);

    /* perform test */
    (void)get_config_group_element_value(&group, instance, &element, ASN_OCTET_STR, NULL, &element_size);

    /* check results */
    mock().checkExpectations();
}

TEST(get_config_value, enum_multi_converted_to_string)
{
    config_mgr_group_t group;
    size_t instance;
    config_mgr_element_t element;
    u_char asn_type;
    size_t element_size;
    config_mgr_element_type_t expected_type;

    /* setup */
    expected_type = config_mgr_element_type_string;
    element.type = config_mgr_element_type_enum_multi;
    instance = 0;
    mock().strictOrder();
    mock()
        .expectOneCall("tlr_get_element_alloc_value")
        .withParameter("group_id", group.id)
        .withParameter("instance", 0)
        .withParameter("element_id", element.id)
        .withParameter("value", (void *)NULL)
        .withParameter("type", expected_type)
        .andReturnValue(config_status_invalid_value);

    /* perform test */
    (void)get_config_group_element_value(&group, instance, &element, ASN_OCTET_STR, NULL, &element_size);

    /* check results */
    mock().checkExpectations();
}

TEST(get_config_value, group_id_passed_to_alloc_value)
{
    config_mgr_group_t group;
    size_t instance;
    config_mgr_element_t element;
    u_char asn_type;
    size_t element_size;
    config_mgr_element_type_t expected_type;
    size_t expected_group_id;


    /* setup */
    expected_type = config_mgr_element_type_string;
    element.type = config_mgr_element_type_enum_multi;
    expected_group_id = 1234;
    group.id = expected_group_id;
    instance = 0;
    mock().strictOrder();
    mock()
        .expectOneCall("tlr_get_element_alloc_value")
        .withParameter("group_id", expected_group_id)
        .withParameter("instance", 0)
        .withParameter("element_id", element.id)
        .withParameter("value", (void *)NULL)
        .withParameter("type", expected_type)
        .andReturnValue(config_status_invalid_value);

    /* perform test */
    (void)get_config_group_element_value(&group, instance, &element, ASN_OCTET_STR, NULL, &element_size);

    /* check results */
    mock().checkExpectations();
}

TEST(get_config_value, instance_passed_to_alloc_value)
{
    config_mgr_group_t group;
    size_t instance;
    config_mgr_element_t element;
    u_char asn_type;
    size_t element_size;
    config_mgr_element_type_t expected_type;
    size_t expected_group_id;
    size_t expected_instance;

    /* setup */
    expected_type = config_mgr_element_type_string;
    element.type = config_mgr_element_type_enum_multi;
    expected_group_id = 1234;
    group.id = expected_group_id;
    expected_instance = 45678;
    instance = expected_instance;
    mock().strictOrder();
    mock()
        .expectOneCall("tlr_get_element_alloc_value")
        .withParameter("group_id", expected_group_id)
        .withParameter("instance", expected_instance)
        .withParameter("element_id", element.id)
        .withParameter("value", (void *)NULL)
        .withParameter("type", expected_type)
        .andReturnValue(config_status_invalid_value);

    /* perform test */
    (void)get_config_group_element_value(&group, instance, &element, ASN_OCTET_STR, NULL, &element_size);

    /* check results */
    mock().checkExpectations();
}

TEST(get_config_value, element_id_passed_to_alloc_value)
{
    config_mgr_group_t group;
    size_t instance;
    config_mgr_element_t element;
    u_char asn_type;
    size_t element_size;
    config_mgr_element_type_t expected_type;
    size_t expected_group_id;
    size_t expected_instance;
    size_t expected_element_id;

    /* setup */
    expected_type = config_mgr_element_type_string;
    element.type = config_mgr_element_type_enum_multi;
    expected_group_id = 1234;
    group.id = expected_group_id;
    expected_instance = 45678;
    instance = expected_instance;
    expected_element_id = 2222;
    element.id = expected_element_id;

    mock().strictOrder();
    mock()
        .expectOneCall("tlr_get_element_alloc_value")
        .withParameter("group_id", expected_group_id)
        .withParameter("instance", expected_instance)
        .withParameter("element_id", expected_element_id)
        .withParameter("value", (void *)NULL)
        .withParameter("type", expected_type)
        .andReturnValue(config_status_invalid_value);

    /* perform test */
    (void)get_config_group_element_value(&group, instance, &element, ASN_OCTET_STR, NULL, &element_size);

    /* check results */
    mock().checkExpectations();
}

TEST(get_config_value, buffer_passed_to_alloc_value)
{
    config_mgr_group_t group;
    size_t instance;
    config_mgr_element_t element;
    u_char asn_type;
    void * buffer = &asn_type;    /* some non-NULL value */
    size_t element_size;
    config_mgr_element_type_t expected_type;
    size_t expected_group_id;
    size_t expected_instance;
    size_t expected_element_id;

    /* setup */
    expected_type = config_mgr_element_type_string;
    element.type = config_mgr_element_type_enum_multi;
    expected_group_id = 1234;
    group.id = expected_group_id;
    expected_instance = 45678;
    instance = expected_instance;
    expected_element_id = 2222;
    element.id = expected_element_id;

    mock().strictOrder();
    mock()
        .expectOneCall("tlr_get_element_alloc_value")
        .withParameter("group_id", expected_group_id)
        .withParameter("instance", expected_instance)
        .withParameter("element_id", expected_element_id)
        .withParameter("value", (void *)&buffer)
        .withParameter("type", expected_type)
        .andReturnValue(config_status_invalid_value);

    /* perform test */
    (void)get_config_group_element_value(&group, instance, &element, ASN_OCTET_STR, &buffer, &element_size);

    /* check results */
    mock().checkExpectations();
}

