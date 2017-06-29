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
    #include <ipsec.h>
    #include <digiutils.h>
    #include <vpn_config.h>
    #include <config_mgr.h>
}

/* mocked functions */
void const * tlr_get_table(config_mgr_table_type_t const type)
{
    mock().actualCall("tlr_get_table");

    if (mock().hasReturnValue())
    {
        return (void *)mock().returnValue().getPointerValue();
    }

    return NULL;
}

size_t tlr_get_element_size(config_mgr_element_t const * const element)
{
    mock().actualCall("tlr_get_element_size");
    if (mock().hasReturnValue())
    {
        return mock().returnValue().getIntValue();
    }

    return 0;
}

tlr_handle_t tlr_config_open(size_t const group_id, size_t const instance, config_mgr_access_level_t const access)
{
    mock().actualCall("tlr_config_open");

    if (mock().hasReturnValue())
    {
        return (void *)mock().returnValue().getPointerValue();
    }

    return NULL;
}

config_status_t tlr_config_read(tlr_handle_t const handle, size_t const element_id, char * const data, size_t const bytes)
{
    mock().actualCall("tlr_config_read");
    if (mock().hasReturnValue())
    {
        return (config_status_t)mock().returnValue().getIntValue();
    }

    return config_status_success;
}

void tlr_config_close(tlr_handle_t const handle)
{
    mock().actualCall("tlr_config_close");
}

/* tests */
TEST_GROUP(config_init)
{
    void setup()
    {
        mock().clear();
    }
    void teardown()
    {
        mock().clear();
    }
};

TEST(config_init, normal)
{
    config_mgr_group_table_t config_table;
    config_mgr_group_t ipsec_group;
    config_st * config;
    int actual_result;
    int expected_result;
    config_group_t config_group;
    size_t instance;
    tlr_handle_t tlr_handle = (tlr_handle_t)1;

    /* setup */
    expected_result = 0;
    memset(&config_table, 0, sizeof config_table);
    memset(&ipsec_group, 0, sizeof ipsec_group);
    ipsec_group.elements.count = 0;
    mock().expectOneCall("tlr_get_table").andReturnValue(&config_table);
    mock().expectOneCall("tlr_config_open").andReturnValue((void *)tlr_handle);
    mock().expectOneCall("tlr_config_close");
    config_group = (config_group_t)0;
    config_table.count = 1;
    config_table.groups = &ipsec_group;
    instance = 0;
    /* perform test */
    actual_result = config_init(&config, instance, config_group);

    /* check results */
    LONGS_EQUAL(expected_result, actual_result);
    CHECK_TRUE(config != NULL);
    mock().checkExpectations();

}

TEST(config_init, one_element_is_allocated)
{
    config_mgr_group_table_t config_table;
    config_mgr_group_t ipsec_group;
    config_st * config;
    int actual_result;
    int expected_result;
    config_group_t config_group;
    int expected_element_size;
    config_mgr_element_t element;
    size_t instance;
    tlr_handle_t tlr_handle = (tlr_handle_t)1;
    int number_of_elements;

    /* setup */
    expected_result = 0;
    memset(&config_table, 0, sizeof config_table);
    memset(&ipsec_group, 0, sizeof ipsec_group);
    memset(&element, 0, sizeof element);
    number_of_elements = 1;
    ipsec_group.elements.count = number_of_elements;
    ipsec_group.elements.element = &element;
    expected_element_size = 1;
    mock().expectOneCall("tlr_get_table").andReturnValue(&config_table);
    mock().expectNCalls(ipsec_group.elements.count, "tlr_get_element_size").andReturnValue(expected_element_size);
    mock().expectOneCall("tlr_config_open").andReturnValue((void *)tlr_handle);
    mock().expectNCalls(ipsec_group.elements.count, "tlr_config_read").andReturnValue(config_status_success);
    mock().expectOneCall("tlr_config_close");

    config_group = (config_group_t)0;
    config_table.count = 1;
    config_table.groups = &ipsec_group;
    instance = 0;

    /* perform test */
    actual_result = config_init(&config, instance, config_group);

    /* check results */
    /* config is an opaque pointer, meaning that we can't check fields in it here */
    LONGS_EQUAL(expected_result, actual_result);
    CHECK_TRUE(config != NULL);
    mock().checkExpectations();

}

TEST(config_init, many_elements_are_allocated)
{
    config_mgr_group_table_t config_table;
    config_mgr_group_t ipsec_group;
    config_st * config;
    int actual_result;
    int expected_result;
    config_group_t config_group;
    int expected_element_size;
    config_mgr_element_t element;
    int number_of_elements_to_allocate;
    size_t instance;
    tlr_handle_t tlr_handle = (tlr_handle_t)1;

    /* setup */
    expected_result = 0;
    memset(&config_table, 0, sizeof config_table);
    memset(&ipsec_group, 0, sizeof ipsec_group);
    memset(&element, 0, sizeof element);
    number_of_elements_to_allocate = 10;
    ipsec_group.elements.count = number_of_elements_to_allocate;
    ipsec_group.elements.element = &element;
    expected_element_size = 1;
    mock().expectOneCall("tlr_get_table").andReturnValue(&config_table);
    mock().expectNCalls(ipsec_group.elements.count, "tlr_get_element_size").andReturnValue(expected_element_size);
    mock().expectOneCall("tlr_config_open").andReturnValue((void *)tlr_handle);
    mock().expectNCalls(ipsec_group.elements.count, "tlr_config_read").andReturnValue(config_status_success);
    mock().expectOneCall("tlr_config_close");
    config_group = (config_group_t)0;
    config_table.count = number_of_elements_to_allocate;
    config_table.groups = &ipsec_group;
    instance = 0;

    /* perform test */
    actual_result = config_init(&config, instance, config_group);

    /* check results */
    /* config is an opaque pointer, meaning that we can't check fields in it here */
    LONGS_EQUAL(expected_result, actual_result);
    CHECK_TRUE(config != NULL);
    mock().checkExpectations();

}

