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
    #include "get_config_value.h"
    #include "config_table.h"
    #include "config_def.h"
    #include "config_mgr.h"
    #include "find_element.h"
    #include "oid_to_group.h"
    #include "index_element.h"
    #include "tlr_common.h"
}
config_mgr_group_t const * snmp_get_next_group(oid const * const oid, uint32_t const oid_len,
        config_mgr_group_type_t * const group_type)
{
    mock()
        .actualCall("snmp_get_next_group")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", (int)oid_len)
        .withParameter("group_type", (void *)group_type);

    if (mock().hasReturnValue())
    {
        return (config_mgr_group_t const *)mock().returnValue().getPointerValue();
    }

    return (config_mgr_group_t const *)NULL;
}
config_mgr_element_t * snmp_get_index_config_element(void)
{
    mock().actualCall("snmp_get_index_config_element");
    if (mock().hasReturnValue())
    {
        return (config_mgr_element_t *)mock().returnValue().getPointerValue();
    }

    return (config_mgr_element_t *)NULL;
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

TEST_GROUP(find_element)
{
    void setup()
    {
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(find_element, get_next_element_in_table_goes_to_next_instance)
{
    oid test_oid[] = {1, 3, 6, 1, 4, 1, 332, 1, 1, 2, 1, 1 ,2, 1};
    uint32_t oid_len = ARRAY_SIZE(test_oid);
    config_mgr_group_t const *expected_group;
    config_mgr_group_t const *group_found;
    config_mgr_group_t test_group;
    config_mgr_element_t test_elements[5];
    config_mgr_group_type_t group_type=config_mgr_set_group;
    config_mgr_element_t *expected_element;
    config_mgr_element_t *element_found;
    uint32_t instance_found;
    uint32_t expected_instance;
    size_t group_id=1;

    // Setup the test group and elements
    test_group.instances = 5;
    test_group.elements.count = 5;
    test_group.elements.element = test_elements;
    test_group.elements.element[0].name = "test name";
    test_group.name = "test group";

    expected_group   = &test_group;
    expected_element = &test_elements[0];
    expected_instance = 2;
    group_found = expected_group;

    /* perform test */
    element_found = snmp_find_next_element_in_table(test_oid, oid_len,
                                                &group_found,
                                                &group_type,
                                                &instance_found);

    /* check results */
    CHECK_EQUAL(expected_element, element_found);
    CHECK_EQUAL(expected_group, group_found);
    CHECK_EQUAL(expected_instance, instance_found);

    mock().checkExpectations();
}

TEST(find_element, get_next_element_in_table_goes_to_next_element)
{
    oid test_oid[] = {1, 3, 6, 1, 4, 1, 332, 1, 1, 2, 1, 1 ,2, 4};
    uint32_t oid_len = ARRAY_SIZE(test_oid);
    config_mgr_group_t const *expected_group;
    config_mgr_group_t const *group_found;
    config_mgr_group_t test_group;
    config_mgr_element_t test_elements[5];
    config_mgr_group_type_t group_type=config_mgr_set_group;
    config_mgr_element_t *expected_element;
    config_mgr_element_t *element_found;
    uint32_t instance_found;
    uint32_t expected_instance;
    size_t group_id=1;

    // Setup the test group and elements
    test_group.instances = 4;
    test_group.elements.count = 5;
    test_group.elements.element = test_elements;
    test_group.elements.element[1].name = "test name";
    test_group.name = "test group";

    expected_group   = &test_group;
    expected_element = &test_elements[1];
    expected_instance = 1;
    group_found = expected_group;

    /* perform test */
    element_found = snmp_find_next_element_in_table(test_oid, oid_len,
                                                &group_found,
                                                &group_type,
                                                &instance_found);

    /* check results */
    CHECK_EQUAL(expected_element, element_found);
    CHECK_EQUAL(expected_group, group_found);
    CHECK_EQUAL(expected_instance, instance_found);

    mock().checkExpectations();
}

TEST(find_element, get_next_element_in_table_for_last_element_goes_to_next_tables_index_element)
{
    oid test_oid[] = {1, 3, 6, 1, 4, 1, 332, 1, 1, 2, 1, 1 , 5, 4};
    uint32_t oid_len = ARRAY_SIZE(test_oid);
    config_mgr_group_t const *expected_group;
    config_mgr_group_t const *group_found;
    config_mgr_group_t test_group;
    config_mgr_element_t test_elements[5];
    config_mgr_group_type_t group_type=config_mgr_set_group;
    config_mgr_element_t *expected_element;
    config_mgr_element_t *element_found;
    uint32_t instance_found;
    uint32_t expected_instance;
    size_t group_id=1;
    config_mgr_group_t next_group;

    // Setup the test group and elements
    test_group.instances = 4;
    test_group.elements.count = 4;
    test_group.elements.element = test_elements;
    test_group.elements.element[4].name = "test name";
    test_group.name = "first group";

    next_group.instances = 4;
    next_group.elements.count = 5;
    next_group.elements.element = test_elements;
    next_group.elements.element[0].name = "index element";
    next_group.name = "test next group";

    expected_group   = &next_group;
    expected_element = &test_elements[0];
    expected_instance = 1;
    group_found = &test_group;

    mock().strictOrder();
    mock()
        .expectOneCall("snmp_get_next_group")
        .withParameter("oid", test_oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group_type", &group_type)
        .andReturnValue(&next_group);

    mock()
        .expectOneCall("is_a_table")
        .withParameter("pgroup", &next_group);

    mock().expectOneCall("snmp_get_index_config_element")
                .andReturnValue(&test_elements[0]);

    /* perform test */
    element_found = snmp_find_next_element_in_table(test_oid, oid_len,
                                                &group_found,
                                                &group_type,
                                                &instance_found);

    /* check results */
    CHECK_EQUAL(expected_element, element_found);
    CHECK_EQUAL(expected_group, group_found);
    CHECK_EQUAL(expected_instance, instance_found);

    mock().checkExpectations();
}

// Tests for elements which are in groups which are not tables
TEST(find_element, get_next_element_in_group_goes_to_next_element_in_group)
{
    oid test_oid[] = {1, 3, 6, 1, 4, 1, 332, 1, 1, 3, 2, 0}; // dsl-config description
    uint32_t oid_len = ARRAY_SIZE(test_oid);
    config_mgr_group_t const *expected_group;
    config_mgr_group_t const *group_found;
    config_mgr_group_t test_group;
    config_mgr_element_t test_elements[5];
    config_mgr_group_type_t group_type=config_mgr_set_group;
    config_mgr_element_t *expected_element;
    config_mgr_element_t *element_found;
    uint32_t instance_found;
    uint32_t expected_instance;
    size_t group_id=1;

    // Setup the test group and elements
    test_group.instances = 1; // Not a table
    test_group.elements.count = 5;
    test_group.elements.element = test_elements;
    test_group.elements.element[1].name = "current element";
    test_group.elements.element[2].name = "next element";
    test_group.name = "test group";

    expected_group   = &test_group;
    expected_element = &test_elements[2];
    expected_instance = 1;
    group_found = expected_group;

    /* perform test */
    element_found = snmp_find_next_element_in_group(test_oid, oid_len,
                                                &group_found,
                                                &group_type,
                                                &instance_found);

    /* check results */
    CHECK_EQUAL(expected_element, element_found);
    CHECK_EQUAL(expected_group, group_found);
    CHECK_EQUAL(expected_instance, instance_found);

    mock().checkExpectations();
}

TEST(find_element, get_next_element_in_group_for_last_element_goes_to_next_tables_index_element)
{
    oid test_oid[] = {1, 3, 6, 1, 4, 1, 332, 1, 1, 3, 4, 0};
    uint32_t oid_len = ARRAY_SIZE(test_oid);
    config_mgr_group_t const *expected_group;
    config_mgr_group_t const *group_found;
    config_mgr_group_t test_group;
    config_mgr_element_t test_elements[5];
    config_mgr_group_type_t group_type=config_mgr_set_group;
    config_mgr_element_t *expected_element;
    config_mgr_element_t *element_found;
    uint32_t instance_found;
    uint32_t expected_instance;
    size_t group_id=1;
    config_mgr_group_t next_group;

    // Setup the test group and elements
    test_group.instances = 1; // Not a table
    test_group.elements.count = 4;
    test_group.elements.element = test_elements;
    test_group.elements.element[4].name = "test name";
    test_group.name = "first group";

    next_group.instances = 4;
    next_group.elements.count = 5;
    next_group.elements.element = test_elements;
    next_group.elements.element[0].name = "index element";
    next_group.name = "test next group";

    expected_group   = &next_group;
    expected_element = &test_elements[0];
    expected_instance = 1;
    group_found = &test_group;

    mock().strictOrder();
    mock()
        .expectOneCall("snmp_get_next_group")
        .withParameter("oid", test_oid)
        .withParameter("oid_len", oid_len)
        .withParameter("group_type", &group_type)
        .andReturnValue(&next_group);

    mock()
        .expectOneCall("is_a_table")
        .withParameter("pgroup", &next_group);

    mock().expectOneCall("snmp_get_index_config_element")
                .andReturnValue(&test_elements[0]);

    /* perform test */
    element_found = snmp_find_next_element_in_group(test_oid, oid_len,
                                                &group_found,
                                                &group_type,
                                                &instance_found);

    /* check results */
    CHECK_EQUAL(expected_element, element_found);
    CHECK_EQUAL(expected_group, group_found);
    CHECK_EQUAL(expected_instance, instance_found);

    mock().checkExpectations();
}


