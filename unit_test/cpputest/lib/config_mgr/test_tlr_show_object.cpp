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
    #include "config_mgr.h"
    #include "tlr_common.h"
}

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <stdint.h>

enum
{
    element_id_string,
    element_id_password,
    element_id_uint32,
    element_id_int32,
    element_id_enum,
    element_id_on_off,
    element_id_boolean,
    element_id_ipaddr,
    element_id_fqdn,
    element_id_mac_addr,
    element_id_counter32,
    element_id_counter64,
    element_id_index,
    element_id_interface,
    element_id_encpassword,
    element_id_enum_multi,
    element_id_none,
};

config_mgr_group_t const * tlr_get_group_ptr(size_t const group_id, size_t const instance, config_mgr_group_type_t const table_index)
{
    static config_mgr_element_t elements[] =
    {
        [element_id_string] =
        {
            .name = "string",
            .description = "",
            .error = "",
            .default_value = "",
            .access = config_mgr_access_readwrite,
            .type = config_mgr_element_type_string,
        },
        [element_id_password] =
        {
            .name = "password",
            .description = "",
            .error = "",
            .default_value = "",
            .access = config_mgr_access_readwrite,
            .type = config_mgr_element_type_password,
        },
        [element_id_uint32] =
        {
            .name = "uint32",
            .description = "",
            .error = "",
            .default_value = "",
            .access = config_mgr_access_readwrite,
            .type = config_mgr_element_type_uint32,
        },
        [element_id_int32] =
        {
            .name = "int32",
            .description = "",
            .error = "",
            .default_value = "",
            .access = config_mgr_access_readwrite,
            .type = config_mgr_element_type_int32,
        },
        [element_id_enum] =
        {
            .name = "enum",
            .description = "",
            .error = "",
            .default_value = "",
            .access = config_mgr_access_readwrite,
            .type = config_mgr_element_type_enum,
        },
        [element_id_on_off] =
        {
            .name = "on_off",
            .description = "",
            .error = "",
            .default_value = "",
            .access = config_mgr_access_readwrite,
            .type = config_mgr_element_type_on_off,
        },
        [element_id_boolean] =
        {
            .name = "boolean",
            .description = "",
            .error = "",
            .default_value = "",
            .access = config_mgr_access_readwrite,
            .type = config_mgr_element_type_boolean,
        },
        [element_id_ipaddr] =
        {
            .name = "ipaddr",
            .description = "",
            .error = "",
            .default_value = "",
            .access = config_mgr_access_readwrite,
            .type = config_mgr_element_type_ipaddr,
        },
        [element_id_fqdn] =
        {
            .name = "fqdn",
            .description = "",
            .error = "",
            .default_value = "",
            .access = config_mgr_access_readwrite,
            .type = config_mgr_element_type_fqdn,
        },
        [element_id_mac_addr] =
        {
            .name = "mac_addr",
            .description = "",
            .error = "",
            .default_value = "",
            .access = config_mgr_access_readwrite,
            .type = config_mgr_element_type_mac_addr,
        },
        [element_id_counter32] =
        {
            .name = "counter32",
            .description = "",
            .error = "",
            .default_value = "",
            .access = config_mgr_access_readwrite,
            .type = config_mgr_element_type_counter32,
        },
        [element_id_counter64] =
        {
            .name = "counter64",
            .description = "",
            .error = "",
            .default_value = "",
            .access = config_mgr_access_readwrite,
            .type = config_mgr_element_type_counter64,
        },
        [element_id_index] =
        {
            .name = "index",
            .description = "",
            .error = "",
            .default_value = "",
            .access = config_mgr_access_readwrite,
            .type = config_mgr_element_type_index,
        },
        [element_id_interface] =
        {
            .name = "interface",
            .description = "",
            .error = "",
            .default_value = "",
            .access = config_mgr_access_readwrite,
            .type = config_mgr_element_type_interface,
        },
        [element_id_encpassword] =
        {
            .name = "encpassword",
            .description = "",
            .error = "",
            .default_value = "",
            .access = config_mgr_access_readwrite,
            .type = config_mgr_element_type_encpassword,
        },
        [element_id_enum_multi] =
        {
            .name = "enum_multi",
            .description = "",
            .error = "",
            .default_value = "",
            .access = config_mgr_access_readwrite,
            .type = config_mgr_element_type_enum_multi,
        },
        [element_id_none] =
        {
            .name = "none",
            .description = "",
            .error = "",
            .default_value = "",
            .access = config_mgr_access_readwrite,
            .type = config_mgr_element_type_none,
        },
    };

    static config_mgr_group_t const group =
    {
        .name = "",
        .filename = "",
        .description = "",
        .id = 1,
        .instances = 1,
        .in_mib = false,
        .mib_indexed = false,
        .access = config_mgr_access_readwrite,
        .elements =
        {
            .element = elements,
            .count = ARRAY_SIZE(elements),
        },
    };

    return &group;
}

TEST_GROUP(tlr_show_object_add_element)
{
    tlr_show_object_t * show_object;

    void setup()
    {
        show_object = tlr_show_object_create(0, 0);
    }

    void teardown()
    {
        tlr_show_object_destroy(show_object);
    }
};

TEST(tlr_show_object_add_element, element_string_is_ordinary_string)
{
    tlr_show_object_add_element(show_object, element_id_string, "ordinary string");
    STRCMP_EQUAL("{\n  \"string\":\"ordinary string\"\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_string_is_empty)
{
    tlr_show_object_add_element(show_object, element_id_string, "");
    STRCMP_EQUAL("{\n  \"string\":\"\"\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_string_is_null)
{
    tlr_show_object_add_element(show_object, element_id_string, NULL);
    STRCMP_EQUAL("{\n  \"string\":null\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_password_is_ordinary_password)
{
    tlr_show_object_add_element(show_object, element_id_password, "ordinary password");
    STRCMP_EQUAL("{\n  \"password\":\"ordinary password\"\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_password_is_empty)
{
    tlr_show_object_add_element(show_object, element_id_password, "");
    STRCMP_EQUAL("{\n  \"password\":\"\"\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_uint32_is_uint32_min)
{
    uint32_t const value = 0;
    tlr_show_object_add_element(show_object, element_id_uint32, &value);
    STRCMP_EQUAL("{\n  \"uint32\":\"0\"\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_uint32_is_uint32_max)
{
    uint32_t const value = UINT32_MAX;
    tlr_show_object_add_element(show_object, element_id_uint32, &value);
    STRCMP_EQUAL("{\n  \"uint32\":\"4294967295\"\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_int32_is_int32_min)
{
    int32_t const value = INT32_MIN;
    tlr_show_object_add_element(show_object, element_id_int32, &value);
    STRCMP_EQUAL("{\n  \"int32\":-2147483648\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_int32_is_zero)
{
    int32_t const value = 0;
    tlr_show_object_add_element(show_object, element_id_int32, &value);
    STRCMP_EQUAL("{\n  \"int32\":0\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_int32_is_int32_max)
{
    int32_t const value = INT32_MAX;
    tlr_show_object_add_element(show_object, element_id_int32, &value);
    STRCMP_EQUAL("{\n  \"int32\":2147483647\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_enum)
{
    enum
    {
        enum_member_0,
        enum_member_1,
        enum_member_2,
    } const value = enum_member_1;
    tlr_show_object_add_element(show_object, element_id_enum, &value);
    STRCMP_EQUAL("{\n  \"enum\":1\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_on_off_is_off)
{
    enum
    {
        on_off_enum_off,
        on_off_enum_on,
    } const value = on_off_enum_off;
    tlr_show_object_add_element(show_object, element_id_on_off, &value);
    STRCMP_EQUAL("{\n  \"on_off\":false\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_on_off_is_on)
{
    enum
    {
        on_off_enum_off,
        on_off_enum_on,
    } const value = on_off_enum_on;
    tlr_show_object_add_element(show_object, element_id_on_off, &value);
    STRCMP_EQUAL("{\n  \"on_off\":true\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_boolean_is_false)
{
    enum
    {
        boolean_enum_false,
        boolean_enum_true,
    } const value = boolean_enum_false;
    tlr_show_object_add_element(show_object, element_id_boolean, &value);
    STRCMP_EQUAL("{\n  \"boolean\":false\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_boolean_is_true)
{
    enum
    {
        boolean_enum_false,
        boolean_enum_true,
    } const value = boolean_enum_true;
    tlr_show_object_add_element(show_object, element_id_boolean, &value);
    STRCMP_EQUAL("{\n  \"boolean\":true\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_ipaddr_is_0_0_0_0)
{
    tlr_show_object_add_element(show_object, element_id_ipaddr, "0.0.0.0");
    STRCMP_EQUAL("{\n  \"ipaddr\":\"0.0.0.0\"\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_ipaddr_is_255_255_255_255)
{
    tlr_show_object_add_element(show_object, element_id_ipaddr, "255.255.255.255");
    STRCMP_EQUAL("{\n  \"ipaddr\":\"255.255.255.255\"\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_fqdn)
{
    tlr_show_object_add_element(show_object, element_id_fqdn, "aaa.bbb.ccc.ddd");
    STRCMP_EQUAL("{\n  \"fqdn\":\"aaa.bbb.ccc.ddd\"\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_mac_address)
{
    tlr_show_object_add_element(show_object, element_id_mac_addr, "00:11:22:33:44:55");
    STRCMP_EQUAL("{\n  \"mac_addr\":\"00:11:22:33:44:55\"\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_counter32_is_uint32_min)
{
    uint32_t const value = 0;
    tlr_show_object_add_element(show_object, element_id_counter32, &value);
    STRCMP_EQUAL("{\n  \"counter32\":\"0\"\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_counter32_is_uint32_max)
{
    uint32_t const value = UINT32_MAX;
    tlr_show_object_add_element(show_object, element_id_counter32, &value);
    STRCMP_EQUAL("{\n  \"counter32\":\"4294967295\"\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_counter64_is_uint64_min)
{
    uint64_t const value = 0;
    tlr_show_object_add_element(show_object, element_id_counter64, &value);
    STRCMP_EQUAL("{\n  \"counter64\":\"0\"\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_counter64_is_uint64_max)
{
    uint64_t const value = UINT64_MAX;
    tlr_show_object_add_element(show_object, element_id_counter64, &value);
    STRCMP_EQUAL("{\n  \"counter64\":\"18446744073709551615\"\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_index_is_int32_min)
{
    int32_t const value = INT32_MIN;
    tlr_show_object_add_element(show_object, element_id_index, &value);
    STRCMP_EQUAL("{\n  \"index\":-2147483648\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_index_is_zero)
{
    int32_t const value = 0;
    tlr_show_object_add_element(show_object, element_id_index, &value);
    STRCMP_EQUAL("{\n  \"index\":0\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_index_is_int32_max)
{
    int32_t const value = INT32_MAX;
    tlr_show_object_add_element(show_object, element_id_index, &value);
    STRCMP_EQUAL("{\n  \"index\":2147483647\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_interface)
{
    tlr_show_object_add_element(show_object, element_id_interface, "eth1");
    STRCMP_EQUAL("{\n  \"interface\":\"eth1\"\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_encpassword)
{
    tlr_show_object_add_element(show_object, element_id_encpassword, "encrypted password");
    STRCMP_EQUAL("{\n  \"encpassword\":\"encrypted password\"\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_enum_multi)
{
    tlr_show_object_add_element(show_object, element_id_enum_multi, "aes128,aes192,aes256");
    STRCMP_EQUAL("{\n  \"enum_multi\":\"aes128,aes192,aes256\"\n}", tlr_show_object_to_json_string(show_object));
}

TEST(tlr_show_object_add_element, element_none)
{
    tlr_show_object_add_element(show_object, element_id_none, "some random string");
    STRCMP_EQUAL("{\n  \"none\":null\n}", tlr_show_object_to_json_string(show_object));
}
