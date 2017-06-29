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
    #include "snmp_utils.h"
    #include "config_mgr.h"
}

TEST_GROUP(snmp_convert_tlr_type_to_asn_type)
{
    void setup()
    {
    }
    void teardown()
    {
        mock().clear();
    }
    void do_test(config_mgr_element_type_t element_type, u_char expected_result)
    {
        u_char const actual_result =  snmp_convert_tlr_type_to_asn_type(element_type);

        CHECK_EQUAL(expected_result, actual_result);
    }
};

TEST(snmp_convert_tlr_type_to_asn_type, dummy)
{
    do_test(config_mgr_element_type_string, ASN_OCTET_STR);
    do_test(config_mgr_element_type_password, ASN_OCTET_STR);
    do_test(config_mgr_element_type_encpassword, ASN_OCTET_STR);
    do_test(config_mgr_element_type_fqdn, ASN_OCTET_STR);
    do_test(config_mgr_element_type_mac_addr, ASN_OCTET_STR);
    do_test(config_mgr_element_type_interface, ASN_OCTET_STR);
    do_test(config_mgr_element_type_enum_multi, ASN_OCTET_STR);
    do_test(config_mgr_element_type_uint32, ASN_UNSIGNED);
    do_test(config_mgr_element_type_int32, ASN_INTEGER);
    do_test(config_mgr_element_type_enum, ASN_INTEGER);
    do_test(config_mgr_element_type_on_off, ASN_INTEGER);
    do_test(config_mgr_element_type_boolean, ASN_INTEGER);
    do_test(config_mgr_element_type_index, ASN_INTEGER);
    do_test(config_mgr_element_type_ipaddr, ASN_IPADDRESS);
    do_test(config_mgr_element_type_counter32, ASN_COUNTER);
    do_test(config_mgr_element_type_counter64, ASN_COUNTER64);
    do_test(config_mgr_element_type_none, 255);

}

TEST_GROUP(get_element_size)
{
    void setup()
    {
    }
    void teardown()
    {
        mock().clear();
    }
    void test_regular_asn_types(u_char const asn_type, size_t const expected_element_size, int const expected_result)
    {
        int actual_result;

        /* setup */
        size_t element_size;

        /* perform test */
        actual_result = snmp_adjust_element(asn_type, &element_size, NULL);

        /* check result */
        CHECK_EQUAL(expected_result, actual_result);
        if (expected_result == 0)
        {
            CHECK_EQUAL(expected_element_size, element_size);
        }
    }
};

TEST(get_element_size, octet_string_returns_string_length)
{
    int actual_result;
    /* setup */
    size_t element_size;
    char some_string[] = "some_string";
    void * buffer = some_string;
    size_t expected_size = strlen(some_string);
    int expected_result = 0;

    /* perform test */
    actual_result = snmp_adjust_element(ASN_OCTET_STR, &element_size, &buffer);

    /* check result */
    CHECK_EQUAL(expected_result, actual_result);
    CHECK_EQUAL(expected_size, element_size);
}

TEST(get_element_size, ip_address_string_returns_element_size_4_and_ip_in_buffer)
{
    int actual_result;
    /* setup */
    size_t element_size;
    void * buffer = strdup("1.2.3.4");
    size_t const expected_size = 4;
    char const expected_buffer_contents[expected_size] = { 1, 2, 3, 4 };
    int const expected_result = 0;

    /* perform test */
    actual_result = snmp_adjust_element(ASN_IPADDRESS, &element_size, &buffer);

    /* check result */
    CHECK_EQUAL(expected_result, actual_result);
    CHECK_EQUAL(expected_size, element_size);
    CHECK_EQUAL(0, memcmp(buffer, expected_buffer_contents, expected_size));
    /* cleanup */
    free(buffer);
}

TEST(get_element_size, empty_ip_address_string_returns_element_size_4_and_ip_in_buffer)
{
    /*
     * The empty IP address is handled a little differently in the production code, so let's ensure
     * that it works.
     */
    int actual_result;
    /* setup */
    size_t element_size;
    void * buffer = strdup("");
    size_t const expected_size = 4;
    char const expected_buffer_contents[expected_size] = { 0, 0, 0, 0 };
    int const expected_result = 0;

    /* perform test */
    actual_result = snmp_adjust_element(ASN_IPADDRESS, &element_size, &buffer);

    /* check result */
    CHECK_EQUAL(expected_result, actual_result);
    CHECK_EQUAL(expected_size, element_size);
    CHECK_EQUAL(0, memcmp(buffer, expected_buffer_contents, expected_size));
    /* cleanup */
    free(buffer);
}

TEST(get_element_size, int64_words_are_swapped)
{
    int actual_result;
    /* setup */
    size_t element_size;
    uint64_t value = 0x0102030405060708ULL;
    uint64_t expected_value = 0x0506070801020304ULL;
    void * buffer = &value;
    size_t expected_size;

    /* perform test */
    (void)snmp_adjust_element(ASN_COUNTER64, &element_size, &buffer);

    /* check result */
    CHECK_EQUAL(expected_value, value);
}

TEST(get_element_size, test_regular_asn_types)
{
    test_regular_asn_types(ASN_INTEGER, 4, 0);
    test_regular_asn_types(ASN_BOOLEAN, 4, 0);
    test_regular_asn_types(ASN_UNSIGNED, 4, 0);
    test_regular_asn_types(ASN_COUNTER, 4, 0);
    test_regular_asn_types(ASN_COUNTER64, 8, 0);
    test_regular_asn_types(ASN_TIMETICKS, 0, -1);
}
