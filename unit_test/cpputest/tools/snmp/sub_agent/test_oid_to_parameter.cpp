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
    #include "snmp_utils.h"
    #include "snmp_agent_debug.h"
    #include "oid_to_parameter.h"
    #include "oid_to_group.h"
    #include "oid_to_element.h"
    #include "get_parameter_value.h"
    #include "create_new_oid.h"
    #include "index_element.h"
}

config_mgr_group_t const * snmp_get_group(oid const * const oid, uint32_t const oid_len,
                                          config_mgr_group_type_t * const group_type)
{
    mock()
        .actualCall("snmp_get_group")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group_type", (void *)group_type);

    return (config_mgr_group_t *)mock().returnPointerValueOrDefault(NULL);
}

config_mgr_element_t * snmp_get_element(oid const * const oid, uint32_t const oid_len,
                                        config_mgr_group_t const * pgroup,
                                        uint32_t * instance)
{
    mock()
        .actualCall("snmp_get_element")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("pgroup", (void *)pgroup)
        .withParameter("instance", (void *)instance);
    if (mock().hasData("instance"))
    {
        *instance = (uint32_t)mock().getData("instance").getUnsignedIntValue();
    }

    return (config_mgr_element_t *)mock().returnPointerValueOrDefault(NULL);
}

config_mgr_element_t * snmp_get_index_config_element(void)
{
    static config_mgr_element_t index_element;

    return &index_element;
}

config_mgr_element_t *  snmp_get_next_element(oid const * const oid, uint32_t const oid_len,
                                             config_mgr_group_t const ** group,
                                             config_mgr_group_type_t * const group_type,
                                             uint32_t * instance)
{
    mock()
        .actualCall("snmp_get_next_element")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group", (void *)group)
        .withParameter("group_type", (void *)group_type)
        .withParameter("instance", (void *)instance);

    if (mock().hasData("instance"))
    {
        *instance = (uint32_t)mock().getData("instance").getUnsignedIntValue();
    }
    if (mock().hasData("group_type"))
    {
        *group_type = (config_mgr_group_type_t)mock().getData("group_type").getIntValue();
    }
    if (mock().hasData("group"))
    {
        *group = (config_mgr_group_t *)mock().getData("group").getPointerValue();
    }

    return (config_mgr_element_t *)mock().returnPointerValueOrDefault(NULL);
}

int create_new_oid(oid const * const orig_oid,
                    uint32_t fixed_oid_prefix_length,
                    oid * const new_oid,
                    uint32_t * const new_oid_length,
                    config_mgr_group_type_t const group_type,
                    size_t const group_id,
                    bool const is_table_group,
                    size_t const instance,
                    size_t const element_id,
                    bool const is_index_element)
{
    mock()
        .actualCall("create_new_oid")
        .withParameter("orig_oid", (void *)orig_oid)
        .withParameter("fixed_oid_prefix_length", fixed_oid_prefix_length)
        .withParameter("new_oid", new_oid)
        .withParameter("new_oid_length", new_oid_length)
        .withParameter("group_type", group_type)
        .withParameter("group_id", group_id)
        .withParameter("is_table_group", is_table_group)
        .withParameter("instance", instance)
        .withParameter("element_id", element_id)
        .withParameter("is_index_element", is_index_element);

    return 0;
}

bool is_a_table(config_mgr_group_t const * const pgroup)
{
    mock().actualCall("is_a_table")
        .withParameter("pgroup", (void *)pgroup);
    if (mock().hasReturnValue())
    {
        return (bool)mock().returnValue().getIntValue();
    }
    return true;
}


TEST_GROUP(oid_to_parameter)
{
    parameter_t parameter;

    void setup()
    {
        parameter.group = NULL;
        parameter.element = NULL;
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(oid_to_parameter, group_and_element_assigned_to_parameter_successfully)
{
    config_mgr_group_t group_to_assign;
    config_mgr_element_t element_to_assign;
    oid const oid[MAX_OID_LEN] = { 0 };
    uint32_t oid_len = 12;
    int actual_result;

    /*
     * Note that this test also tests that oid and oid_len are passed to the called functions.
     * I'm not sure it's worth going to the effort of making specific tests for these checks.
     */
    /* setup */
    mock().strictOrder();
    mock()
        .expectOneCall("snmp_get_group")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group_type", (void *)&parameter.group_type)
        .andReturnValue((void *)&group_to_assign);

    mock()
        .expectOneCall("snmp_get_element")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("pgroup", (void *)&group_to_assign)
        .withParameter("instance", (void *)&parameter.instance)
        .andReturnValue((void *)&element_to_assign);

    /* perform the test */
    actual_result = oid_to_parameter(oid, oid_len, &parameter);

    /* check the result */
    CHECK_EQUAL(0, actual_result);
    /* also check that group and element have been assigned to parameter */
    POINTERS_EQUAL(&group_to_assign, parameter.group);
    POINTERS_EQUAL(&element_to_assign, parameter.element);
    mock().checkExpectations();

}

TEST(oid_to_parameter, failed_snmp_get_group_results_in_error)
{
    config_mgr_group_t group_to_assign;
    config_mgr_element_t element_to_assign;
    oid const oid[MAX_OID_LEN] = { 0 };
    uint32_t oid_len = 12;
    int actual_result;

    /* setup */
    mock()
        .expectOneCall("snmp_get_group")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group_type", (void *)&parameter.group_type)
        .andReturnValue((void *)NULL);


    /* perform the test */
    actual_result = oid_to_parameter(oid, oid_len, &parameter);

    /* check the result */
    CHECK_EQUAL(-1, actual_result);
    mock().checkExpectations();

}

TEST(oid_to_parameter, failed_snmp_get_element_results_in_error)
{
    config_mgr_group_t group_to_assign;
    config_mgr_element_t element_to_assign;
    oid const oid[MAX_OID_LEN] = { 0 };
    uint32_t oid_len = 12;
    int actual_result;

    /* setup */
    mock().strictOrder();
    mock()
        .expectOneCall("snmp_get_group")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group_type", (void *)&parameter.group_type)
        .andReturnValue((void *)&group_to_assign);

    mock()
        .expectOneCall("snmp_get_element")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("pgroup", (void *)&group_to_assign)
        .withParameter("instance", (void *)&parameter.instance)
        .andReturnValue((void *)NULL);

    /* perform the test */
    actual_result = oid_to_parameter(oid, oid_len, &parameter);

    /* check the result */
    CHECK_EQUAL(-1, actual_result);
    mock().checkExpectations();

}

TEST(oid_to_parameter, instance_is_assigned_to_parameter)
{
    config_mgr_group_t group_to_assign;
    config_mgr_element_t element_to_assign;
    oid const oid[MAX_OID_LEN] = { 0 };
    uint32_t const oid_len = 12;
    uint32_t const expected_instance = 5678;

    /* setup */
    mock().strictOrder();
    mock()
        .expectOneCall("snmp_get_group")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group_type", (void *)&parameter.group_type)
        .andReturnValue((void *)&group_to_assign);

    mock()
        .expectOneCall("snmp_get_element")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("pgroup", (void *)&group_to_assign)
        .withParameter("instance", (void *)&parameter.instance)
        .andReturnValue((void *)&parameter.instance);

    mock().setData("instance", expected_instance);

    /* perform the test */
    (void)oid_to_parameter(oid, oid_len, &parameter);

    /* check the result */
    CHECK_EQUAL(expected_instance, parameter.instance);
    mock().checkExpectations();

}

TEST_GROUP(oid_to_next_parameter)
{
    parameter_t parameter;

    void setup()
    {
        parameter.group = NULL;
        parameter.element = NULL;
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(oid_to_next_parameter, group_and_element_assigned_to_parameter_successfully)
{
    config_mgr_group_t group_to_assign;
    config_mgr_element_t element_to_assign;
    config_mgr_element_t * element = &element_to_assign;
    oid const oid[MAX_OID_LEN] = { 0 };
    uint32_t oid_len = 12;
    int actual_result;

    /*
     * Note that this test also tests that oid and oid_len are passed to the called functions.
     * I'm not sure it's worth going to the effort of making specific tests for these checks.
     */
    /* setup */
    group_to_assign.instances = 1;  /* shouldn't be a table */
    element_to_assign.id = 3333;
    group_to_assign.id = 4444;

    mock().strictOrder();
    mock()
        .expectOneCall("snmp_get_group")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group_type", (void *)&parameter.group_type)
        .andReturnValue((void *)&group_to_assign);

    mock()
        .expectOneCall("snmp_get_next_element")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group", (void *)&parameter.group)
        .withParameter("group_type", (void *)&parameter.group_type)
        .withParameter("instance", (void *)&parameter.instance)
        .andReturnValue((void *)element);

    mock()
        .expectOneCall("is_a_table")
        .withParameter("pgroup", &group_to_assign)
        .andReturnValue(false);

    mock()
        .expectOneCall("create_new_oid")
        .withParameter("orig_oid", (void *)oid)
        .withParameter("fixed_oid_prefix_length", oid_len)
        .withParameter("new_oid", (void *)parameter.oid)
        .withParameter("new_oid_length", (void *)&parameter.oid_length)
        .withParameter("group_type", parameter.group_type)
        .withParameter("group_id", group_to_assign.id)
        .withParameter("is_table_group", false)
        .withParameter("instance", parameter.instance)
        .withParameter("element_id", element_to_assign.id)
        .withParameter("is_index_element", element == snmp_get_index_config_element())
        ;

    /* perform the test */
    actual_result = oid_to_next_parameter(oid, oid_len, &parameter);

    /* check the result */
    CHECK_EQUAL(0, actual_result);
    /* also check that group and element have been assigned to parameter */
    POINTERS_EQUAL(&group_to_assign, parameter.group);
    POINTERS_EQUAL(&element_to_assign, parameter.element);
    mock().checkExpectations();

}

TEST(oid_to_next_parameter, failed_get_group_results_in_error)
{
    config_mgr_group_t group_to_assign;
    config_mgr_element_t element_to_assign;
    config_mgr_element_t * element = &element_to_assign;
    oid const oid[MAX_OID_LEN] = { 0 };
    uint32_t oid_len = 12;
    int actual_result;
    int expected_result;
    /*
     * Note that this test also tests that oid and oid_len are passed to the called functions.
     * I'm not sure it's worth going to the effort of making specific tests for these checks.
     */
    /* setup */
    group_to_assign.instances = 1;  /* shouldn't be a table */
    element_to_assign.id = 3333;
    group_to_assign.id = 4444;
    expected_result = -1;
    mock().strictOrder();
    mock()
        .expectOneCall("snmp_get_group")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group_type", (void *)&parameter.group_type)
        .andReturnValue((void *)NULL);


    /* perform the test */
    actual_result = oid_to_next_parameter(oid, oid_len, &parameter);

    /* check the result */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();

}

TEST(oid_to_next_parameter, failed_get_next_element_returns_error)
{
    config_mgr_group_t group_to_assign;
    config_mgr_element_t element_to_assign;
    config_mgr_element_t * element = &element_to_assign;
    oid const oid[MAX_OID_LEN] = { 0 };
    uint32_t oid_len = 12;
    int actual_result;
    int expected_result;

    /*
     * Note that this test also tests that oid and oid_len are passed to the called functions.
     * I'm not sure it's worth going to the effort of making specific tests for these checks.
     */
    /* setup */
    expected_result = -1;
    group_to_assign.instances = 1;  /* shouldn't be a table */
    element_to_assign.id = 3333;
    group_to_assign.id = 4444;

    mock().strictOrder();
    mock()
        .expectOneCall("snmp_get_group")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group_type", (void *)&parameter.group_type)
        .andReturnValue((void *)&group_to_assign);

    mock()
        .expectOneCall("snmp_get_next_element")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group", (void *)&parameter.group)
        .withParameter("group_type", (void *)&parameter.group_type)
        .withParameter("instance", (void *)&parameter.instance)
        .andReturnValue((void *)NULL);

    /* perform the test */
    actual_result = oid_to_next_parameter(oid, oid_len, &parameter);

    /* check the result */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();

}

TEST(oid_to_next_parameter, no_get_next_element_returns_positive)
{
    config_mgr_group_t group_to_assign;
    config_mgr_element_t element_to_assign;
    config_mgr_element_t * element = &element_to_assign;
    oid const oid[MAX_OID_LEN] = { 0 };
    uint32_t oid_len = 12;
    int actual_result;
    int expected_result;

    /*
     * Note that this test also tests that oid and oid_len are passed to the called functions.
     * I'm not sure it's worth going to the effort of making specific tests for these checks.
     */
    /* setup */
    expected_result = 1;
    group_to_assign.instances = 1;  /* shouldn't be a table */
    element_to_assign.id = 3333;
    group_to_assign.id = 4444;

    mock().strictOrder();
    mock()
        .expectOneCall("snmp_get_group")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group_type", (void *)&parameter.group_type)
        .andReturnValue((void *)&group_to_assign);

    mock()
        .expectOneCall("snmp_get_next_element")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group", (void *)&parameter.group)
        .withParameter("group_type", (void *)&parameter.group_type)
        .withParameter("instance", (void *)&parameter.instance)
        .andReturnValue((void *)NULL);
    mock().setData("group", (void *)NULL);

    /* perform the test */
    actual_result = oid_to_next_parameter(oid, oid_len, &parameter);

    /* check the result */
    CHECK_EQUAL(expected_result, actual_result);
    mock().checkExpectations();

}


TEST(oid_to_next_parameter, is_table_group_follows_group_instance)
{
    config_mgr_group_t group_to_assign;
    config_mgr_element_t element_to_assign;
    config_mgr_element_t * element = &element_to_assign;
    oid const oid[MAX_OID_LEN] = { 0 };
    uint32_t oid_len = 12;
    bool expected_is_table_group_value;

    /*
     * Note that this test also tests that oid and oid_len are passed to the called functions.
     * I'm not sure it's worth going to the effort of making specific tests for these checks.
     */
    /* setup */
    group_to_assign.instances = 2;  /* shouldn't be a table */
    expected_is_table_group_value = true;
    element_to_assign.id = 3333;
    group_to_assign.id = 4444;

    mock().strictOrder();
    mock()
        .expectOneCall("snmp_get_group")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group_type", (void *)&parameter.group_type)
        .andReturnValue((void *)&group_to_assign);

    mock()
        .expectOneCall("snmp_get_next_element")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group", (void *)&parameter.group)
        .withParameter("group_type", (void *)&parameter.group_type)
        .withParameter("instance", (void *)&parameter.instance)
        .andReturnValue((void *)element);

    mock()
        .expectOneCall("is_a_table")
        .withParameter("pgroup", &group_to_assign);

    mock()
        .expectOneCall("create_new_oid")
        .withParameter("orig_oid", (void *)oid)
        .withParameter("fixed_oid_prefix_length", oid_len)
        .withParameter("new_oid", (void *)parameter.oid)
        .withParameter("new_oid_length", (void *)&parameter.oid_length)
        .withParameter("group_type", parameter.group_type)
        .withParameter("group_id", group_to_assign.id)
        .withParameter("is_table_group", expected_is_table_group_value)
        .withParameter("instance", parameter.instance)
        .withParameter("element_id", element_to_assign.id)
        .withParameter("is_index_element", element == snmp_get_index_config_element())
        ;

    /* perform the test */
    (void)oid_to_next_parameter(oid, oid_len, &parameter);

    /* check the result */
    mock().checkExpectations();

}

TEST(oid_to_next_parameter, is_element_index)
{
    config_mgr_group_t group_to_assign;
    config_mgr_element_t * element = snmp_get_index_config_element();
    oid const oid[MAX_OID_LEN] = { 0 };
    uint32_t oid_len = 12;
    bool expected_is_table_group_value;

    /*
     * Note that this test also tests that oid and oid_len are passed to the called functions.
     * I'm not sure it's worth going to the effort of making specific tests for these checks.
     */
    /* setup */
    group_to_assign.instances = 2;  /* shouldn't be a table */
    expected_is_table_group_value = true;
    group_to_assign.id = 4444;

    mock().strictOrder();
    mock()
        .expectOneCall("snmp_get_group")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group_type", (void *)&parameter.group_type)
        .andReturnValue((void *)&group_to_assign);

    mock()
        .expectOneCall("snmp_get_next_element")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group", (void *)&parameter.group)
        .withParameter("group_type", (void *)&parameter.group_type)
        .withParameter("instance", (void *)&parameter.instance)
        .andReturnValue((void *)element);

    mock()
        .expectOneCall("is_a_table")
        .withParameter("pgroup", &group_to_assign);

    mock()
        .expectOneCall("create_new_oid")
        .withParameter("orig_oid", (void *)oid)
        .withParameter("fixed_oid_prefix_length", oid_len)
        .withParameter("new_oid", (void *)parameter.oid)
        .withParameter("new_oid_length", (void *)&parameter.oid_length)
        .withParameter("group_type", parameter.group_type)
        .withParameter("group_id", group_to_assign.id)
        .withParameter("is_table_group", expected_is_table_group_value)
        .withParameter("instance", parameter.instance)
        .withParameter("element_id", element->id)
        .withParameter("is_index_element", element == snmp_get_index_config_element());

    /* perform the test */
    (void)oid_to_next_parameter(oid, oid_len, &parameter);

    /* check the result */
    /* also check that group and element have been assigned to parameter */
    mock().checkExpectations();

}

TEST(oid_to_next_parameter, instance_assigned_to_parameter)
{
    config_mgr_group_t group_to_assign;
    config_mgr_element_t * element = snmp_get_index_config_element();
    oid const oid[MAX_OID_LEN] = { 0 };
    uint32_t oid_len = 12;
    bool expected_is_table_group_value = true;
    uint32_t expected_instance = 6666;

    /*
     * Note that this test also tests that oid and oid_len are passed to the called functions.
     * I'm not sure it's worth going to the effort of making specific tests for these checks.
     */
    /* setup */
    group_to_assign.instances = 2;  /* shouldn't be a table */
    group_to_assign.id = 4444;

    mock().strictOrder();
    mock()
        .expectOneCall("snmp_get_group")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group_type", (void *)&parameter.group_type)
        .andReturnValue((void *)&group_to_assign);

    mock()
        .expectOneCall("snmp_get_next_element")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group", (void *)&parameter.group)
        .withParameter("group_type", (void *)&parameter.group_type)
        .withParameter("instance", (void *)&parameter.instance)
        .andReturnValue((void *)element);

    mock()
        .expectOneCall("is_a_table")
        .withParameter("pgroup", &group_to_assign);

    mock()
        .expectOneCall("create_new_oid")
        .withParameter("orig_oid", (void *)oid)
        .withParameter("fixed_oid_prefix_length", oid_len)
        .withParameter("new_oid", (void *)parameter.oid)
        .withParameter("new_oid_length", (void *)&parameter.oid_length)
        .withParameter("group_type", parameter.group_type)
        .withParameter("group_id", group_to_assign.id)
        .withParameter("is_table_group", expected_is_table_group_value)
        .withParameter("instance", expected_instance)
        .withParameter("element_id", element->id)
        .withParameter("is_index_element", element == snmp_get_index_config_element());

    mock().setData("instance", expected_instance);

    /* perform the test */
    (void)oid_to_next_parameter(oid, oid_len, &parameter);

    /* check the result */
    CHECK_EQUAL(expected_instance, parameter.instance);
    /* also check that group and element have been assigned to parameter */
    mock().checkExpectations();

}

TEST(oid_to_next_parameter, group_type_assigned_to_parameter)
{
    config_mgr_group_t group_to_assign;
    config_mgr_element_t * element = snmp_get_index_config_element();
    oid const oid[MAX_OID_LEN] = { 0 };
    uint32_t oid_len = 12;
    bool expected_is_table_group_value = true;
    config_mgr_group_type_t expected_group_type = config_mgr_get_group;

    /*
     * Note that this test also tests that oid and oid_len are passed to the called functions.
     * I'm not sure it's worth going to the effort of making specific tests for these checks.
     */
    /* setup */
    group_to_assign.instances = 2;  /* shouldn't be a table */
    group_to_assign.id = 4444;

    mock().strictOrder();
    mock()
        .expectOneCall("snmp_get_group")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group_type", (void *)&parameter.group_type)
        .andReturnValue((void *)&group_to_assign);

    mock()
        .expectOneCall("snmp_get_next_element")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group", (void *)&parameter.group)
        .withParameter("group_type", (void *)&parameter.group_type)
        .withParameter("instance", (void *)&parameter.instance)
        .andReturnValue((void *)element);

    mock()
        .expectOneCall("is_a_table")
        .withParameter("pgroup", &group_to_assign);

    mock()
        .expectOneCall("create_new_oid")
        .withParameter("orig_oid", (void *)oid)
        .withParameter("fixed_oid_prefix_length", oid_len)
        .withParameter("new_oid", (void *)parameter.oid)
        .withParameter("new_oid_length", (void *)&parameter.oid_length)
        .withParameter("group_type", expected_group_type)
        .withParameter("group_id", group_to_assign.id)
        .withParameter("is_table_group", expected_is_table_group_value)
        .withParameter("instance", parameter.instance)
        .withParameter("element_id", element->id)
        .withParameter("is_index_element", element == snmp_get_index_config_element());

    mock().setData("group_type", expected_group_type);

    /* perform the test */
    (void)oid_to_next_parameter(oid, oid_len, &parameter);

    /* check the result */
    CHECK_EQUAL(expected_group_type, parameter.group_type);
    /* also check that group and element have been assigned to parameter */
    mock().checkExpectations();

}

TEST(oid_to_next_parameter, group_assigned_to_parameter)
{
    config_mgr_group_t group_to_assign;
    config_mgr_element_t * element = snmp_get_index_config_element();
    oid const oid[MAX_OID_LEN] = { 0 };
    uint32_t oid_len = 12;
    bool expected_is_table_group_value = true;
    config_mgr_group_type_t expected_group_type = config_mgr_get_group;

    /*
     * Note that this test also tests that oid and oid_len are passed to the called functions.
     * I'm not sure it's worth going to the effort of making specific tests for these checks.
     */
    /* setup */
    group_to_assign.instances = 2;  /* shouldn't be a table */
    group_to_assign.id = 4444;

    mock().strictOrder();
    mock()
        .expectOneCall("snmp_get_group")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group_type", (void *)&parameter.group_type)
        .andReturnValue((void *)&group_to_assign);

    mock()
        .expectOneCall("snmp_get_next_element")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group", (void *)&parameter.group)
        .withParameter("group_type", (void *)&parameter.group_type)
        .withParameter("instance", (void *)&parameter.instance)
        .andReturnValue((void *)NULL);


    mock().setData("group", &group_to_assign);

    /* perform the test */
    (void)oid_to_next_parameter(oid, oid_len, &parameter);

    /* check the result */
    CHECK_EQUAL(&group_to_assign, parameter.group);
    /* also check that group and element have been assigned to parameter */
    mock().checkExpectations();

}


