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
    #include "index_element.h"
    #include "oid_to_element.h"
    #include "tlr_common.h"
}
config_mgr_element_t *snmp_find_next_element_in_table(oid const * const oid, uint32_t const oid_len,
                                            config_mgr_group_t const **group,
                                            config_mgr_group_type_t * const group_type,
                                            uint32_t * const instance)
{
    mock()
        .actualCall("snmp_find_next_element_in_table")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", (int)oid_len)
        .withParameter("group", (void *)group)
        .withParameter("group_type", (void *)group_type)
        .withParameter("instance", (void *)instance);

    if (mock().hasReturnValue())
    {
        return (config_mgr_element_t *)mock().returnValue().getPointerValue();
    }

    return (config_mgr_element_t *)NULL;
}

config_mgr_element_t *snmp_find_next_element_in_group(oid const * const oid, uint32_t const oid_len,
                                            config_mgr_group_t const **group,
                                            config_mgr_group_type_t * const group_type,
                                            uint32_t * const instance)
{
    mock()
        .actualCall("snmp_find_next_element_in_group")
        .withParameter("oid", (void *)oid)
        .withParameter("oid_len", (int)oid_len)
        .withParameter("group", (void *)group)
        .withParameter("group_type", (void *)group_type)
        .withParameter("instance", (void *)instance);

    if (mock().hasReturnValue())
    {
        return (config_mgr_element_t *)mock().returnValue().getPointerValue();
    }

    return (config_mgr_element_t *)NULL;
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

TEST_GROUP(oid_to_element)
{
    void setup()
    {
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(oid_to_element, snmp_get_next_element_returns_next_element_in_table)
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
    uint32_t instance_found=1;
    uint32_t expected_instance;
    size_t group_id=1;

    // Setup the test group and elements
    test_group.instances = 5;
    test_group.elements.count = 5;
    test_group.elements.element = test_elements;
    test_group.elements.element[0].name = "test name";
    test_group.name = "test group";

    expected_group   = &test_group;
    expected_element = &test_elements[2];
    expected_instance = 1;
    group_found = expected_group;

    mock()
        .expectOneCall("is_a_table")
        .withParameter("pgroup", &test_group);

    mock()
        .expectOneCall("snmp_find_next_element_in_table")
        .withParameter("oid", (void *)test_oid)
        .withParameter("oid_len", (int)oid_len)
        .withParameter("group", (void *)&group_found)
        .withParameter("group_type", (void *)&group_type)
        .withParameter("instance", (void *)&instance_found)
        .andReturnValue(expected_element);

    /* perform test */
    element_found = snmp_get_next_element(test_oid, oid_len,
                                          &group_found,
                                          &group_type,
                                          &instance_found);

    /* check results */
    CHECK_EQUAL(expected_element, element_found);
    CHECK_EQUAL(expected_group, group_found);
    CHECK_EQUAL(expected_instance, instance_found);

    mock().checkExpectations();
}

TEST(oid_to_element, snmp_get_next_element_returns_index_element_on_short_oid)
{
    oid test_oid[] = {1, 3, 6, 1, 4, 1, 332, 1, 1 };
    uint32_t oid_len = ARRAY_SIZE(test_oid);
    config_mgr_group_t const *expected_group;
    config_mgr_group_t const *group_found;
    config_mgr_group_t test_group;
    config_mgr_element_t test_elements[5];
    config_mgr_group_type_t group_type=config_mgr_set_group;
    config_mgr_element_t *expected_element;
    config_mgr_element_t *element_found;
    uint32_t instance_found=1;
    uint32_t expected_instance;
    size_t group_id=1;

    // Setup the test group and elements
    test_group.instances = 5;
    test_group.elements.count = 5;
    test_group.elements.element = test_elements;
    test_group.elements.element[0].name = "test name";
    test_group.name = "test group";

    expected_group   = &test_group;
    expected_element = &test_elements[2];
    expected_instance = 1;
    group_found = expected_group;

    mock()
        .expectOneCall("is_a_table")
        .withParameter("pgroup", &test_group);

    mock()
        .expectOneCall("snmp_get_index_config_element")
        .andReturnValue(expected_element);

    /* perform test */
    element_found = snmp_get_next_element(test_oid, oid_len,
                                          &group_found,
                                          &group_type,
                                          &instance_found);

    /* check results */
    CHECK_EQUAL(expected_element, element_found);
    CHECK_EQUAL(expected_group, group_found);
    CHECK_EQUAL(expected_instance, instance_found);

    mock().checkExpectations();
}

TEST(oid_to_element, snmp_get_next_element_returns_next_element_in_group)
{
    oid test_oid[] = {1, 3, 6, 1, 4, 1, 332, 1, 1, 3, 1, 0};
    uint32_t oid_len = ARRAY_SIZE(test_oid);
    config_mgr_group_t const *expected_group;
    config_mgr_group_t const *group_found;
    config_mgr_group_t test_group;
    config_mgr_element_t test_elements[5];
    config_mgr_group_type_t group_type=config_mgr_set_group;
    config_mgr_element_t *expected_element;
    config_mgr_element_t *element_found;
    uint32_t instance_found=1;
    uint32_t expected_instance;
    size_t group_id=1;

    // Setup the test group and elements
    test_group.instances = 1;
    test_group.elements.count = 5;
    test_group.elements.element = test_elements;
    test_group.elements.element[0].name = "test name";
    test_group.name = "test group";

    expected_group   = &test_group;
    expected_element = &test_elements[2];
    expected_instance = 1;
    group_found = expected_group;

    mock()
        .expectOneCall("is_a_table")
        .withParameter("pgroup", &test_group)
        .andReturnValue(false);

    mock()
        .expectOneCall("snmp_find_next_element_in_group")
        .withParameter("oid", (void *)test_oid)
        .withParameter("oid_len", (int)oid_len)
        .withParameter("group", (void *)&group_found)
        .withParameter("group_type", (void *)&group_type)
        .withParameter("instance", (void *)&instance_found)
        .andReturnValue(expected_element);

    /* perform test */
    element_found = snmp_get_next_element(test_oid, oid_len,
                                          &group_found,
                                          &group_type,
                                          &instance_found);

    /* check results */
    CHECK_EQUAL(expected_element, element_found);
    CHECK_EQUAL(expected_group, group_found);
    CHECK_EQUAL(expected_instance, instance_found);

    mock().checkExpectations();
}

TEST(oid_to_element, snmp_get_next_element_returns_first_element_in_group_with_short_oid)
{
    oid test_oid[] = {1, 3, 6, 1, 4, 1, 332, 1, 1, 3};
    uint32_t oid_len = ARRAY_SIZE(test_oid);
    config_mgr_group_t const *expected_group;
    config_mgr_group_t const *group_found;
    config_mgr_group_t test_group;
    config_mgr_element_t test_elements[5];
    config_mgr_group_type_t group_type=config_mgr_set_group;
    config_mgr_element_t *expected_element;
    config_mgr_element_t *element_found;
    uint32_t instance_found=1;
    uint32_t expected_instance;
    size_t group_id=1;

    // Setup the test group and elements
    test_group.instances = 1;
    test_group.elements.count = 5;
    test_group.elements.element = test_elements;
    test_group.elements.element[0].name = "test name";
    test_group.name = "test group";

    expected_group   = &test_group;
    expected_element = &test_elements[0]; // Should return the first element in the group.
    expected_instance = 1;
    group_found = expected_group;

    mock()
        .expectOneCall("is_a_table")
        .withParameter("pgroup", &test_group)
        .andReturnValue(false);

    /* perform test */
    element_found = snmp_get_next_element(test_oid, oid_len,
                                          &group_found,
                                          &group_type,
                                          &instance_found);

    /* check results */
    CHECK_EQUAL(expected_element, element_found);
    CHECK_EQUAL(expected_group, group_found);
    CHECK_EQUAL(expected_instance, instance_found);

    mock().checkExpectations();
}

TEST(oid_to_element, snmp_get_element_returns_correct_element_in_table)
{
    oid test_oid[] = {1, 3, 6, 1, 4, 1, 332, 1, 1, 2, 1, 1 ,2, 1};
    uint32_t oid_len = ARRAY_SIZE(test_oid);
    config_mgr_group_t test_group;
    config_mgr_element_t test_elements[5];
    config_mgr_group_type_t group_type=config_mgr_set_group;
    config_mgr_element_t *expected_element;
    config_mgr_element_t *element_found;
    uint32_t instance_found=1;
    uint32_t expected_instance;
    size_t group_id=1;

    // Setup the test group and elements
    test_group.instances = 5;
    test_group.elements.count = 5;
    test_group.elements.element = test_elements;
    test_group.elements.element[0].name = "test name";
    test_group.name = "test group";

    expected_element = &test_elements[0];
    expected_instance = 1;

    mock()
        .expectOneCall("is_a_table")
        .withParameter("pgroup", &test_group);

    /* perform test */
    element_found = snmp_get_element(test_oid, oid_len,
                                          &test_group,
                                          &instance_found);

    /* check results */
    CHECK_EQUAL(expected_element, element_found);
    CHECK_EQUAL(expected_instance, instance_found);

    mock().checkExpectations();
}

TEST(oid_to_element, snmp_get_element_returns_NULL_with_short_oid)
{
    oid test_oid[] = {1, 3, 6, 1, 4, 1, 332, 1, 1, 3};
    uint32_t oid_len = ARRAY_SIZE(test_oid);
    config_mgr_group_t const *expected_group;
    config_mgr_group_t const *group_found;
    config_mgr_group_t test_group;
    config_mgr_element_t test_elements[5];
    config_mgr_group_type_t group_type=config_mgr_set_group;
    config_mgr_element_t *expected_element;
    config_mgr_element_t *element_found;
    uint32_t instance_found=1;
    uint32_t expected_instance;
    size_t group_id=1;

    // Setup the test group and elements
    test_group.instances = 1;
    test_group.elements.count = 5;
    test_group.elements.element = test_elements;
    test_group.elements.element[0].name = "test name";
    test_group.name = "test group";

    expected_group   = &test_group;
    expected_element = NULL;  // No element should be found.
    expected_instance = 1;
    group_found = expected_group;

    mock()
        .expectOneCall("is_a_table")
        .withParameter("pgroup", &test_group)
        .andReturnValue(false);

    /* perform test */
    element_found = snmp_get_element(test_oid, oid_len,
                                          &test_group,
                                          &instance_found);

    /* check results */
    CHECK_EQUAL(expected_element, element_found);
    CHECK_EQUAL(expected_group, group_found);
    CHECK_EQUAL(expected_instance, instance_found);

    mock().checkExpectations();
}


