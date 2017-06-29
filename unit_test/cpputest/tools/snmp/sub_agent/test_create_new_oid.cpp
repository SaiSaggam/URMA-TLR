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
    #include "oid_to_parameter.h"
    #include "oid_to_group.h"
    #include "oid_to_element.h"
    #include "create_new_oid.h"
}


TEST_GROUP(create_new_oid)
{
    void setup()
    {
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(create_new_oid, short_oid_prefix_results_in_error)
{
    oid prefix_oid[MAX_OID_LEN];
    uint32_t prefix_oid_length;
    int actual_result;
    int expected_result;
    size_t const minimum_oid_prefix_length = 8;

    /* setup */
    prefix_oid_length = minimum_oid_prefix_length - 1;
    expected_result = -1;

    /* perform test */
    actual_result = create_new_oid((oid const *)prefix_oid, prefix_oid_length, NULL, NULL, (config_mgr_group_type_t)0, 0, false, 0, 0, false);

    /* check result */
    CHECK_EQUAL(expected_result, actual_result);
}

TEST(create_new_oid, minimum_oid_prefix_length_results_in_success)
{
    oid prefix_oid[MAX_OID_LEN];
    uint32_t prefix_oid_length;
    oid output_oid[MAX_OID_LEN];
    uint32_t output_oid_length;
    int actual_result;
    int expected_result;
    size_t const minimum_oid_prefix_length = 8;

    /* setup */
    prefix_oid_length = minimum_oid_prefix_length;
    expected_result = 0;

    /* perform test */
    actual_result = create_new_oid((oid const *)prefix_oid, prefix_oid_length, output_oid, &output_oid_length, (config_mgr_group_type_t)0, 0, false, 0, 0, false);

    /* check result */
    CHECK_EQUAL(expected_result, actual_result);
}

TEST(create_new_oid, prefix_copied_into_new_oid)
{
    oid prefix_oid[MAX_OID_LEN] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    uint32_t prefix_oid_length;
    oid output_oid[MAX_OID_LEN];
    uint32_t output_oid_length;
    bool oid_prefix_matches;
    size_t const minimum_oid_prefix_length = 8;

    /* setup */
    prefix_oid_length = minimum_oid_prefix_length;

    /* perform test */
    (void)create_new_oid((oid const *)prefix_oid, prefix_oid_length, output_oid, &output_oid_length, (config_mgr_group_type_t)0, 0, false, 0, 0, false);

    /* check result */
    oid_prefix_matches = memcmp(prefix_oid, output_oid, minimum_oid_prefix_length * sizeof(oid[0])) == 0;

    CHECK_TRUE(oid_prefix_matches);
}

TEST(create_new_oid, check_table_group_index_element_oid)
{
    oid prefix_oid[MAX_OID_LEN] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    uint32_t prefix_oid_length;
    oid output_oid[MAX_OID_LEN];
    uint32_t output_oid_length;
    oid expected_oid[MAX_OID_LEN] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    uint32_t expected_oid_length;
    size_t const minimum_oid_prefix_length = 8;
    size_t instance = 2;
    size_t group_type = 5;
    size_t group_id = 10;
    bool oids_match;
    bool is_table_group;
    bool is_index_element;

    /* setup */
    prefix_oid_length = minimum_oid_prefix_length;
    expected_oid[8] = group_type + 1;
    expected_oid[9] = group_id + 1;
    expected_oid[10] = 1;
    expected_oid[11] = 1;
    expected_oid[12] = 1;
    expected_oid[13] = instance;
    expected_oid_length = 14;
    is_table_group = true;
    is_index_element = true;

    /* perform test */
    (void)create_new_oid((oid const *)prefix_oid, prefix_oid_length, output_oid, &output_oid_length, (config_mgr_group_type_t)0, 0, is_table_group, 0, 0, is_index_element);

    /* check result */
    CHECK_EQUAL(expected_oid_length, output_oid_length);
    oids_match = memcmp(output_oid, expected_oid, expected_oid_length) == 0;
    CHECK_TRUE(oids_match);
}

TEST(create_new_oid, check_table_group_non_index_element_oid)
{
    oid prefix_oid[MAX_OID_LEN] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    uint32_t prefix_oid_length;
    oid output_oid[MAX_OID_LEN];
    uint32_t output_oid_length;
    oid expected_oid[MAX_OID_LEN] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    uint32_t expected_oid_length;
    size_t const minimum_oid_prefix_length = 8;
    size_t instance = 2;
    size_t group_type = 5;
    size_t group_id = 10;
    size_t element_id = 15;
    bool oids_match;
    bool is_table_group;
    bool is_index_element;


    /* setup */
    prefix_oid_length = minimum_oid_prefix_length;
    expected_oid[8] = group_type + 1;
    expected_oid[9] = group_id + 1;
    expected_oid[10] = 1;
    expected_oid[11] = 1;
    expected_oid[12] = element_id + 2;
    expected_oid[13] = instance;
    expected_oid_length = 14;
    is_table_group = true;
    is_index_element = false;

    /* perform test */
    (void)create_new_oid((oid const *)prefix_oid, prefix_oid_length, output_oid, &output_oid_length, (config_mgr_group_type_t)0, 0, is_table_group, 0, 0, is_index_element);

    /* check result */
    CHECK_EQUAL(expected_oid_length, output_oid_length);
    oids_match = memcmp(output_oid, expected_oid, expected_oid_length) == 0;
    CHECK_TRUE(oids_match);
}

TEST(create_new_oid, check_non_table_group_oid)
{
    oid prefix_oid[MAX_OID_LEN] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    uint32_t prefix_oid_length;
    oid output_oid[MAX_OID_LEN];
    uint32_t output_oid_length;
    oid expected_oid[MAX_OID_LEN] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    uint32_t expected_oid_length;
    size_t const minimum_oid_prefix_length = 8;
    size_t instance = 2;
    size_t group_type = 5;
    size_t group_id = 10;
    size_t element_id = 15;
    bool oids_match;
    bool is_table_group;
    bool is_index_element;


    /* setup */
    prefix_oid_length = minimum_oid_prefix_length;
    expected_oid[8] = group_type + 1;
    expected_oid[9] = group_id + 1;
    expected_oid[10] = element_id + 1;
    expected_oid[11] = 0;
    expected_oid_length = 12;
    is_table_group = false;
    is_index_element = false;

    /* perform test */
    (void)create_new_oid((oid const *)prefix_oid, prefix_oid_length, output_oid, &output_oid_length, (config_mgr_group_type_t)0, 0, is_table_group, 0, 0, is_index_element);

    /* check result */
    CHECK_EQUAL(expected_oid_length, output_oid_length);
    oids_match = memcmp(output_oid, expected_oid, expected_oid_length) == 0;
    CHECK_TRUE(oids_match);
}

TEST(create_new_oid, check_non_table_group_oid_is_index_element_has_no_effect)
{
    oid prefix_oid[MAX_OID_LEN] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    uint32_t prefix_oid_length;
    oid output_oid[MAX_OID_LEN];
    uint32_t output_oid_length;
    oid expected_oid[MAX_OID_LEN] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    uint32_t expected_oid_length;
    size_t const minimum_oid_prefix_length = 8;
    size_t instance = 2;
    size_t group_type = 5;
    size_t group_id = 10;
    size_t element_id = 15;
    bool oids_match;
    bool is_table_group;
    bool is_index_element;


    /* setup */
    prefix_oid_length = minimum_oid_prefix_length;
    expected_oid[8] = group_type + 1;
    expected_oid[9] = group_id + 1;
    expected_oid[10] = element_id + 1;
    expected_oid[11] = 0;
    expected_oid_length = 12;
    is_table_group = false;
    is_index_element = true;

    /* perform test */
    (void)create_new_oid((oid const *)prefix_oid, prefix_oid_length, output_oid, &output_oid_length, (config_mgr_group_type_t)0, 0, is_table_group, 0, 0, is_index_element);

    /* check result */
    CHECK_EQUAL(expected_oid_length, output_oid_length);
    oids_match = memcmp(output_oid, expected_oid, expected_oid_length) == 0;
    CHECK_TRUE(oids_match);
}

