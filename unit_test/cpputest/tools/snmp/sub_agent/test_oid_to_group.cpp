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
config_mgr_group_t const * tlr_get_group_ptr(size_t const group_id, size_t const instance, config_mgr_group_type_t const table_index)
{
    mock()
        .actualCall("tlr_get_group_ptr")
        .withParameter("group_id", (int)group_id)
        .withParameter("instance", (int)instance)
        .withParameter("table_index", (int)table_index);

    if (mock().hasReturnValue())
    {
        return (config_mgr_group_t const *)mock().returnValue().getPointerValue();
    }

    return (config_mgr_group_t const *)NULL;
}

size_t tlr_get_num_groups(config_mgr_group_type_t const table_index)
{
    mock()
          .actualCall("tlr_get_num_groups")
          .withParameter("table_index", (int)table_index);

    if (mock().hasReturnValue())
    {
        return mock().returnValue().getIntValue();
    }

    return 0;
}

TEST_GROUP(oid_to_group)
{
    void setup()
    {
    }
    void teardown()
    {
        mock().clear();
    }
};


TEST(oid_to_group, snmp_get_next_group_returns_first_group_in_mib)
{
    oid test_oid[] = {1, 3, 6, 1, 4, 1, 332, 1};
    uint32_t oid_len = ARRAY_SIZE(test_oid);
    config_mgr_group_t const *expected_group;
    config_mgr_group_t const *group_found;
    config_mgr_group_t next_group;
    config_mgr_group_type_t group_type=config_mgr_set_group;
    size_t group_id = 0;
    size_t instance = 1;

    next_group.instances = 4;
    next_group.in_mib = true;
    next_group.name = "test next group";

    expected_group   = &next_group;

    mock()
        .expectOneCall("tlr_get_group_ptr")
        .withParameter("group_id", (int)group_id)
        .withParameter("instance", (int)instance)
        .withParameter("table_index", (int)group_type)
        .andReturnValue((void *)&next_group);

    /* perform test */
    group_found =  snmp_get_next_group(test_oid, oid_len, &group_type);

    /* check results */
    CHECK_EQUAL(expected_group, group_found);
    CHECK_EQUAL(group_type, config_mgr_set_group);

    mock().checkExpectations();
}

TEST(oid_to_group, snmp_get_next_group_returns_correct_group_in_mib)
{
    oid test_oid[] = {1, 3, 6, 1, 4, 1, 332, 1, 1, 1, 9};

    uint32_t oid_len = ARRAY_SIZE(test_oid);
    config_mgr_group_t const *expected_group;
    config_mgr_group_t const *group_found;
    config_mgr_group_t next_group;
    config_mgr_group_type_t group_type=config_mgr_set_group;
    size_t group_id = 0;
    size_t instance = 1;

    next_group.instances = 4;
    next_group.in_mib = true;
    next_group.name = "test next group";
    next_group.id = 0;

    expected_group   = &next_group;

    mock()
           .expectOneCall("tlr_get_num_groups")
           .withParameter("table_index", (int)group_type)
           .andReturnValue(5);

    mock()
        .expectOneCall("tlr_get_group_ptr")
        .withParameter("group_id", (int)group_id)
        .withParameter("instance", (int)instance)
        .withParameter("table_index", (int)group_type)
        .andReturnValue((void *)&next_group);


    // Next call gets the second group
    mock()
         .expectOneCall("tlr_get_group_ptr")
         .withParameter("group_id", (int)group_id+1)
         .withParameter("instance", (int)instance)
         .withParameter("table_index", (int)group_type)
         .andReturnValue((void *)&next_group);

    /* perform test */
    group_found =  snmp_get_next_group(test_oid, oid_len, &group_type);

    /* check results */
    CHECK_EQUAL(expected_group, group_found);
    CHECK_EQUAL(group_type, config_mgr_set_group);

    mock().checkExpectations();
}


TEST(oid_to_group, snmp_get_group_returns_correct_group_in_mib)
{
    oid test_oid[] = {1, 3, 6, 1, 4, 1, 332, 1, 1, 3, 1, 0};

    uint32_t oid_len = ARRAY_SIZE(test_oid);
    config_mgr_group_t const *expected_group;
    config_mgr_group_t const *group_found;
    config_mgr_group_t next_group;
    config_mgr_group_type_t group_type=config_mgr_set_group;
    size_t group_id = 2;
    size_t instance = 1;

    next_group.instances = 4;
    next_group.in_mib = true;
    next_group.name = "test group";
    next_group.id = 0;

    expected_group   = &next_group;

    mock()
        .expectOneCall("tlr_get_group_ptr")
        .withParameter("group_id", (int)group_id)
        .withParameter("instance", (int)instance)
        .withParameter("table_index", (int)group_type)
        .andReturnValue((void *)&next_group);

    /* perform test */
    group_found =  snmp_get_group(test_oid, oid_len, &group_type);

    /* check results */
    CHECK_EQUAL(expected_group, group_found);
    CHECK_EQUAL(group_type, config_mgr_set_group);

    mock().checkExpectations();
}

TEST(oid_to_group, snmp_get_group_fails_with_unsupported_oid)
{
    oid test_oid[] = {1, 3, 6, 1, 4, 1, 332, 1, 1, 3, 1, 0};

    uint32_t oid_len = ARRAY_SIZE(test_oid);
    config_mgr_group_t const *group_found;
    config_mgr_group_type_t group_type=config_mgr_set_group;
    size_t group_id = 2;
    size_t instance = 1;

    // tlr_get_group_ptr will return NULL if the group is
    // not supported.
    mock()
        .expectOneCall("tlr_get_group_ptr")
        .withParameter("group_id", (int)group_id)
        .withParameter("instance", (int)instance)
        .withParameter("table_index", (int)group_type)
        .andReturnValue((void *)NULL);

    /* perform test */
    group_found =  snmp_get_group(test_oid, oid_len, &group_type);

    /* check results */
    POINTERS_EQUAL(NULL, group_found);

    mock().checkExpectations();
}


// Last OID in group skips to the next group
TEST(oid_to_group, snmp_get_next_group_goes_to_next_group)
{
    oid test_oid[] = {1, 3, 6, 1, 4, 1, 332, 1, 1, 1, 9};

    uint32_t oid_len = ARRAY_SIZE(test_oid);
    config_mgr_group_t const *expected_group;
    config_mgr_group_t const *group_found;
    config_mgr_group_t next_group;
    config_mgr_group_type_t group_type=config_mgr_set_group;
    size_t group_id = 0;
    size_t instance = 1;

    next_group.instances = 4;
    next_group.in_mib = true;
    next_group.mib_indexed = true;
    next_group.name = "test next group";
    next_group.id = 0;

    expected_group   = &next_group;

    mock()
           .expectOneCall("tlr_get_num_groups")
           .withParameter("table_index", (int)group_type)
           .andReturnValue(5);

    mock()
        .expectOneCall("tlr_get_group_ptr")
        .withParameter("group_id", (int)group_id)
        .withParameter("instance", (int)instance)
        .withParameter("table_index", (int)group_type)
        .andReturnValue((void *)&next_group);


    // Next call gets the second group
    mock()
         .expectOneCall("tlr_get_group_ptr")
         .withParameter("group_id", (int)group_id+1)
         .withParameter("instance", (int)instance)
         .withParameter("table_index", (int)group_type)
         .andReturnValue((void *)&next_group);

    /* perform test */
    group_found =  snmp_get_next_group(test_oid, oid_len, &group_type);

    /* check results */
    CHECK_EQUAL(expected_group, group_found);
    CHECK_EQUAL(group_type, config_mgr_set_group);

    mock().checkExpectations();
}

TEST(oid_to_group, snmp_get_next_group_skips_over_unsupported_group)
{
    oid test_oid[] = {1, 3, 6, 1, 4, 1, 332, 1, 1, 1, 9};

    uint32_t oid_len = ARRAY_SIZE(test_oid);
    config_mgr_group_t const *expected_group;
    config_mgr_group_t const *group_found;
    config_mgr_group_t next_group;
    config_mgr_group_type_t group_type=config_mgr_set_group;
    size_t group_id = 0;
    size_t instance = 1;

    next_group.instances = 4;
    next_group.in_mib = true;
    next_group.mib_indexed = true;
    next_group.name = "test next group";
    next_group.id = 0;

    expected_group   = &next_group;

    mock()
           .expectOneCall("tlr_get_num_groups")
           .withParameter("table_index", (int)group_type)
           .andReturnValue(5);

    mock()
        .expectOneCall("tlr_get_group_ptr")
        .withParameter("group_id", (int)group_id)
        .withParameter("instance", (int)instance)
        .withParameter("table_index", (int)group_type)
        .andReturnValue((void *)&next_group);

    // return NULL which indicates the group is not supported
    // The code should then get the next group
    mock()
         .expectOneCall("tlr_get_group_ptr")
         .withParameter("group_id", (int)group_id+1)
         .withParameter("instance", (int)instance)
         .withParameter("table_index", (int)group_type)
         .andReturnValue((void *)NULL);

    mock()
         .expectOneCall("tlr_get_group_ptr")
         .withParameter("group_id", (int)group_id+2)
         .withParameter("instance", (int)instance)
         .withParameter("table_index", (int)group_type)
         .andReturnValue((void *)&next_group);

    /* perform test */
    group_found =  snmp_get_next_group(test_oid, oid_len, &group_type);

    /* check results */
    CHECK_EQUAL(expected_group, group_found);
    CHECK_EQUAL(group_type, config_mgr_set_group);

    mock().checkExpectations();
}



