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

extern "C"
{
   #include "wan_nat.h"
   #include "iptables.h"
   #include "config_mgr.h"
}
#include <CppUTest/TestHarness.h>
#include <cstring>
#include "CppUTestExt/MockSupport.h"

config_status_t tlr_config_string_to_value(size_t const group_id, size_t const element_id, char const * const buffer, void * const value)
{
    mock().actualCall("tlr_config_string_to_value")
        .withParameter("group_id", group_id)
        .withParameter("element_id", element_id)
        .withParameter("buffer", buffer)
        .withOutputParameter("value", (void *)value);

    return (config_status_t)mock().intReturnValue();
}

config_status_t tlr_get_element_alloc_value(size_t const group_id,
                                            size_t const instance,
                                            size_t const element_id,
                                            void ** value,
                                            config_mgr_element_type_t const type,
                                            config_mgr_access_level_t const access)
{
    mock().actualCall("tlr_get_element_alloc_value")
          .withParameter("group_id", group_id)
          .withParameter("instance", instance)
          .withParameter("element_id", element_id)
          .withOutputParameter("value", (void *)value)
          .withParameter("type", type)
          .withParameter("access", access);

    return (config_status_t)mock().intReturnValue();
}

config_status_t tlr_get_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      void * const value,
                                      size_t const bytes,
                                      config_mgr_element_type_t const type,
                                      config_mgr_access_level_t const access)
{
    mock().actualCall("tlr_get_element_value")
          .withParameter("group_id", group_id)
          .withParameter("instance", instance)
          .withParameter("element_id", element_id)
          .withOutputParameter("value", value)
          .withParameter("bytes", bytes)
          .withParameter("type", type)
          .withParameter("access", access);

    return (config_status_t)mock().intReturnValue();
}

void iptables_nat_on(char const * const ifname)
{
    mock().actualCall("iptables_nat_on")
          .withParameter("ifname", ifname);
}

void iptables_nat_off(char const * const ifname)
{
    mock().actualCall("iptables_nat_off")
          .withParameter("ifname", ifname);
}

TEST_GROUP(wan_nat)
{
    char * element_value;

    void setup()
    {
        element_value = NULL;
    }
    void teardown()
    {
    }

};

TEST(wan_nat, apply_wan_nat_settings_turns_on_nat_when_enabled)
{
    int const expected_result = 0;
    int actual_result;
    size_t const instance = 1;
    unsigned int const on_off_value = 1;
    char const interface_name[] = "eth1";
    config_wan_interface_t wan_interface = config_wan_interface_eth1;

    element_value = strdup(interface_name);

    mock().expectOneCall("tlr_get_element_value")
            .withParameter("group_id", config_wan)
            .withParameter("instance", instance)
            .withParameter("element_id", config_wan_nat)
            .withOutputParameterReturning("value", &on_off_value, sizeof on_off_value)
            .withParameter("bytes", sizeof on_off_value)
            .withParameter("type", config_mgr_element_type_on_off)
            .withParameter("access", config_mgr_access_level_super)
            .andReturnValue(config_status_success);

    mock().expectOneCall("tlr_get_element_alloc_value")
          .withParameter("group_id", config_wan)
          .withParameter("instance", instance)
          .withParameter("element_id", config_wan_interface)
          .withOutputParameterReturning("value", &element_value, sizeof(char *))
          .withParameter("type", config_mgr_element_type_string)
          .withParameter("access", config_mgr_access_level_super)
          .andReturnValue(config_status_success);

    mock().expectOneCall("tlr_config_string_to_value")
        .withParameter("group_id", config_wan)
        .withParameter("element_id", config_wan_interface)
        .withParameter("buffer", element_value)
        .withOutputParameterReturning("value", &wan_interface, sizeof(wan_interface))
        .andReturnValue(config_status_success);

    mock().expectOneCall("iptables_nat_on")
          .withParameter("ifname", interface_name);

    actual_result = apply_wan_nat_settings(instance);

    LONGS_EQUAL(expected_result, actual_result);
}

TEST(wan_nat, iptables_nat_command_not_issued_if_interface_is_none)
{
    int const expected_result = 0;
    int actual_result;
    size_t const instance = 1;
    unsigned int const on_off_value = 1;
    char const interface_name[] = "none";
    config_wan_interface_t wan_interface = config_wan_interface_none;

    element_value = strdup(interface_name);

    mock().expectOneCall("tlr_get_element_value")
            .withParameter("group_id", config_wan)
            .withParameter("instance", instance)
            .withParameter("element_id", config_wan_nat)
            .withOutputParameterReturning("value", &on_off_value, sizeof on_off_value)
            .withParameter("bytes", sizeof on_off_value)
            .withParameter("type", config_mgr_element_type_on_off)
            .withParameter("access", config_mgr_access_level_super)
            .andReturnValue(config_status_success);

    mock().expectOneCall("tlr_get_element_alloc_value")
          .withParameter("group_id", config_wan)
          .withParameter("instance", instance)
          .withParameter("element_id", config_wan_interface)
          .withOutputParameterReturning("value", &element_value, sizeof(char *))
          .withParameter("type", config_mgr_element_type_string)
          .withParameter("access", config_mgr_access_level_super)
          .andReturnValue(config_status_success);

    mock().expectOneCall("tlr_config_string_to_value")
        .withParameter("group_id", config_wan)
        .withParameter("element_id", config_wan_interface)
        .withParameter("buffer", element_value)
        .withOutputParameterReturning("value", &wan_interface, sizeof(wan_interface))
        .andReturnValue(config_status_success);

    actual_result = apply_wan_nat_settings(instance);

    LONGS_EQUAL(expected_result, actual_result);
}

TEST(wan_nat, apply_wan_nat_settings_turns_off_nat_when_disabled)
{
    int const expected_result = 0;
    int actual_result;
    size_t const instance = 1;
    unsigned int const on_off_value = 0;
    char const interface_name[] = "eth1";
    config_wan_interface_t wan_interface = config_wan_interface_eth1;

    element_value = strdup(interface_name);

    mock().expectOneCall("tlr_get_element_value")
            .withParameter("group_id", config_wan)
            .withParameter("instance", instance)
            .withParameter("element_id", config_wan_nat)
            .withOutputParameterReturning("value", &on_off_value, sizeof on_off_value)
            .withParameter("bytes", sizeof on_off_value)
            .withParameter("type", config_mgr_element_type_on_off)
            .withParameter("access", config_mgr_access_level_super)
            .andReturnValue(config_status_success);

    mock().expectOneCall("tlr_get_element_alloc_value")
            .withParameter("group_id", config_wan)
            .withParameter("instance", instance)
            .withParameter("element_id", config_wan_interface)
            .withOutputParameterReturning("value", &element_value, sizeof(char *))
            .withParameter("type", config_mgr_element_type_string)
            .withParameter("access", config_mgr_access_level_super)
            .andReturnValue(config_status_success);

    mock().expectOneCall("tlr_config_string_to_value")
        .withParameter("group_id", config_wan)
        .withParameter("element_id", config_wan_interface)
        .withParameter("buffer", element_value)
        .withOutputParameterReturning("value", &wan_interface, sizeof(wan_interface))
        .andReturnValue(config_status_success);

    mock().expectOneCall("iptables_nat_off")
            .withParameter("ifname", interface_name);

    actual_result = apply_wan_nat_settings(instance);

    LONGS_EQUAL(expected_result, actual_result);
}

TEST(wan_nat, apply_wan_nat_settings_fails_when_tlr_get_element_value_fails)
{
    int const expected_result = -1;
    int actual_result;
    size_t const instance = 1;
    unsigned int const on_off_value = 0;


    mock().expectOneCall("tlr_get_element_value")
            .withParameter("group_id", config_wan)
            .withParameter("instance", instance)
            .withParameter("element_id", config_wan_nat)
            .withOutputParameterReturning("value", &on_off_value, sizeof on_off_value)
            .withParameter("bytes", sizeof on_off_value)
            .withParameter("type", config_mgr_element_type_on_off)
            .withParameter("access", config_mgr_access_level_super)
            .andReturnValue(config_status_element_not_found);

    actual_result = apply_wan_nat_settings(instance);

    LONGS_EQUAL(expected_result, actual_result);
}

TEST(wan_nat, apply_wan_nat_settings_fails_when_tlr_alloc_element_value_fails)
{
    int expected_result = -1;
    int actual_result;
    size_t instance = 1;
    unsigned int on_off_value = 0;

    mock().expectOneCall("tlr_get_element_value")
            .withParameter("group_id", config_wan)
            .withParameter("instance", instance)
            .withParameter("element_id", config_wan_nat)
            .withOutputParameterReturning("value", &on_off_value, sizeof on_off_value)
            .withParameter("bytes", sizeof on_off_value)
            .withParameter("type", config_mgr_element_type_on_off)
            .withParameter("access", config_mgr_access_level_super)
            .andReturnValue(config_status_success);

    mock().expectOneCall("tlr_get_element_alloc_value")
            .withParameter("group_id", config_wan)
            .withParameter("instance", instance)
            .withParameter("element_id", config_wan_interface)
            .withOutputParameterReturning("value", &element_value, sizeof(char *))
            .withParameter("type", config_mgr_element_type_string)
            .withParameter("access", config_mgr_access_level_super)
            .andReturnValue(config_status_no_memory);

    actual_result = apply_wan_nat_settings(instance);

    LONGS_EQUAL(expected_result, actual_result);
}




