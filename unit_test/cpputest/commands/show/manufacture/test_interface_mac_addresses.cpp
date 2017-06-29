/*****************************************************************************
* Copyright (c) 2015 Digi International Inc., All Rights Reserved
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
#include "CppUTestExt/MockSupport.h"
#include <stdio.h>

extern "C"
{
    #include "interface_mac_addresses.h"
    #include "interface_mac_addresses_private.h"
    #include "show_manufacture_mem.h"
    #include "mac_address.h"
    #include "interfaces.h"

    #include <stdlib.h>
    #include "string.h"
}

void show_manufacture_free(void * mem)
{
    mock().actualCall("show_manufacture_free").withParameter("mem", mem);
    free(mem);
}

void * show_manufacture_calloc(size_t const nmemb, size_t const size)
{
    mock()
    .actualCall("show_manufacture_calloc")
    .withParameter("nmemb", nmemb)
    .withParameter("size", size);

    return calloc(nmemb, size);
}

char * get_mac_address(char const * const interface_name, size_t const instance)
{
    mock().actualCall("get_mac_address")
        .withParameter("interface_name", interface_name)
        .withParameter("instance", instance);

    return (char *)mock().returnPointerValueOrDefault(NULL);
}

char const * interface_get_name_from_config_group(config_group_t const config_group)
{
    mock().actualCall("interface_get_name_from_config_group").withParameter("config_group", config_group);

    return (char const *)mock().returnPointerValueOrDefault(NULL);
}

TEST_GROUP(interface_mac_addresses_free)
{
    void setup(void)
    {
    }

    void teardown(void)
    {
        mock().clear();
    }
};

TEST(interface_mac_addresses_free, no_calls_to_free_with_null_pointer)
{
    interface_mac_address_st * mac_addresses;

    /* setup */
    mac_addresses = NULL;
    /* no calls expected at all */

    /* perform test */
    interface_mac_addresses_free(mac_addresses);

    /* check results */
    mock().checkExpectations();
}

TEST(interface_mac_addresses_free, mac_addresses_is_freed)
{
    interface_mac_address_st * mac_addresses;

    /* setup */
    mac_addresses = (interface_mac_address_st *)calloc(1, sizeof *mac_addresses);
    mock().expectOneCall("show_manufacture_free").withParameter("mem", (void *)mac_addresses);

    /* perform test */
    interface_mac_addresses_free(mac_addresses);

    /* check results */
    mock().checkExpectations();
}

TEST(interface_mac_addresses_free, free_called_for_each_mac_address)
{
    interface_mac_address_st * mac_addresses;
    unsigned int const num_interfaces_to_free = 2;
    unsigned int index;

    /* setup */
    mock().strictOrder();
    mac_addresses = (interface_mac_address_st *)calloc(1, sizeof *mac_addresses);
    mac_addresses->num_interfaces = num_interfaces_to_free;
    /* Set up a dummy pointer for each address and check that it gets passed to
     * show_manufacture_free.
     */
    mac_addresses->mac_addresses = (char * *)calloc(num_interfaces_to_free, sizeof *mac_addresses->mac_addresses);
    for (index = 0; index < num_interfaces_to_free; index++)
    {
        mac_addresses->mac_addresses[index] = strdup("test");
        mock().expectOneCall("show_manufacture_free").withParameter("mem", (void *)mac_addresses->mac_addresses[index]);
    }
    mock().expectOneCall("show_manufacture_free").withParameter("mem", (void *)mac_addresses->mac_addresses);
    mock().expectOneCall("show_manufacture_free").withParameter("mem", (void *)mac_addresses);

    /* perform test */
    interface_mac_addresses_free(mac_addresses);

    /* check results */
    mock().checkExpectations();
}

TEST_GROUP(get_mac_address_from_mac_addresses)
{
    void setup(void)
    {
    }

    void teardown(void)
    {
        mock().clear();
    }

    void do_test(unsigned int instance_to_get, char const * const expected_string, char const * const unexpected_string)
    {
        interface_mac_address_st * mac_addresses;
        unsigned int num_interfaces_to_create;
        unsigned int index;
        char const * actual_string;

        /* setup */
        num_interfaces_to_create = 2;
        mac_addresses = (interface_mac_address_st *)calloc(1, sizeof *mac_addresses);
        mac_addresses->num_interfaces = num_interfaces_to_create;
        mac_addresses->mac_addresses = (char * *)calloc(num_interfaces_to_create, sizeof *mac_addresses->mac_addresses);
        for (index = 0; index < num_interfaces_to_create; index++)
        {
            if (instance_to_get == 0 || index == (instance_to_get - 1))
            {
                mac_addresses->mac_addresses[index] = strdup(expected_string);
            }
            else
            {
                mac_addresses->mac_addresses[index] = strdup(unexpected_string);
            }
        }
        mock().disable();

        /* perform test */
        actual_string = get_mac_address_from_mac_addresses(mac_addresses, instance_to_get);

        /* check results */
        STRCMP_EQUAL(expected_string, actual_string);

        /* cleanup */
        interface_mac_addresses_free(mac_addresses);
    }
};

TEST(get_mac_address_from_mac_addresses, null_mac_addresses_returns_null)
{
    char const * expected_string;
    char const * actual_string;
    interface_mac_address_st * mac_addresses;

    /* setup */
    mac_addresses = NULL;
    expected_string = NULL;

    /* perform test */
    actual_string = get_mac_address_from_mac_addresses(mac_addresses, 0);

    /* check results */
    POINTERS_EQUAL(expected_string, actual_string);

    /* cleanup */
}

TEST(get_mac_address_from_mac_addresses, instance_0_returns_first_mac)
{
    do_test(0, "expected_string", "unexpected_string");
}

TEST(get_mac_address_from_mac_addresses, instance_1_returns_first_mac)
{
    do_test(1, "expected_string", "unexpected_string");
}

TEST(get_mac_address_from_mac_addresses, instance_3_returns_null)
{
    do_test(3, NULL, "unexpected_string");
}

TEST(get_mac_address_from_mac_addresses, instance_2_returns_second_mac)
{
    do_test(2, "expected_string", "unexpected_string");
}

TEST_GROUP(write_interface_mac_addresses)
{
    void setup(void)
    {
    }

    void teardown(void)
    {
        mock().clear();
    }
};

TEST(write_interface_mac_addresses, label_unchanged_if_mac_addresses_is_null)
{
    char const * label;
    char const expected_label[] = "expected_label";
    char const unexpected_label[] = "unexpected_label";
    FILE * memstream;
    interface_mac_address_st * mac_addresses;

    /* setup */
    memstream = NULL;
    mac_addresses = NULL;
    label = expected_label;

    /* perform test */
    write_interface_mac_addresses(memstream, mac_addresses, &label, unexpected_label,NULL, NULL);

    /* check results */
    POINTERS_EQUAL(label, expected_label);

    /* cleanup */
}

TEST(write_interface_mac_addresses, label_updated_if_one_mac_is_printed)
{
    char const * label;
    char const initial_label[] = "initial label";
    char const empty_label[] = "empty label";
    FILE * memstream;
    interface_mac_address_st * mac_addresses;
    char * output_buffer = NULL;
    size_t output_buffer_size;

    /* setup */
    mock().disable();
    memstream = open_memstream(&output_buffer, &output_buffer_size);
    label = initial_label;
    mac_addresses = (interface_mac_address_st *)calloc(1, sizeof *mac_addresses);
    mac_addresses->num_interfaces = 1;
    mac_addresses->mac_addresses = (char * *)calloc(1, sizeof *mac_addresses->mac_addresses);
    mac_addresses->mac_addresses[0] = strdup("00:01:02:03:04:05");

    /* perform test */
    write_interface_mac_addresses(memstream, mac_addresses, &label, empty_label, "interface label", "sub");

    /* check results */
    fclose(memstream);
    POINTERS_EQUAL(label, empty_label);

    /* cleanup */
    free(output_buffer);
    interface_mac_addresses_free(mac_addresses);
}

TEST(write_interface_mac_addresses, single_mac_outputs_line_with_label)
{
    char const * label;
    char const initial_label[] = "LABEL";
    char const empty_label[] = "     ";
    FILE * memstream;
    interface_mac_address_st * mac_addresses;
    char * output_buffer = NULL;
    size_t output_buffer_size;

    /* setup */
    mock().disable();
    memstream = open_memstream(&output_buffer, &output_buffer_size);
    label = initial_label;
    mac_addresses = (interface_mac_address_st *)calloc(1, sizeof *mac_addresses);
    mac_addresses->num_interfaces = 1;
    mac_addresses->mac_addresses = (char * *)calloc(1, sizeof *mac_addresses->mac_addresses);
    mac_addresses->mac_addresses[0] = strdup("00:01:02:03:04:05");

    /* perform test */
    write_interface_mac_addresses(memstream, mac_addresses, &label, empty_label, "ETH", "sub");

    /* check results */
    fclose(memstream);  /* Close the memstream so that we can compare the buffer it writes to with the expected value. */
    STRCMP_EQUAL(" LABEL ETH1 sub MAC=00:01:02:03:04:05\n", output_buffer);

    /* cleanup */
    free(output_buffer);
    interface_mac_addresses_free(mac_addresses);
}

TEST(write_interface_mac_addresses, second_mac_outputs_line_with_empty_label)
{
    char const * label;
    char const initial_label[] = "LABEL";
    char const empty_label[] = "EMPTY";
    FILE * memstream;
    interface_mac_address_st * mac_addresses;
    char * output_buffer = NULL;
    size_t output_buffer_size;

    /* setup */
    mock().disable();
    memstream = open_memstream(&output_buffer, &output_buffer_size);
    label = initial_label;
    mac_addresses = (interface_mac_address_st *)calloc(1, sizeof *mac_addresses);
    mac_addresses->num_interfaces = 2;
    mac_addresses->mac_addresses = (char * *)calloc(1, sizeof *mac_addresses->mac_addresses);
    mac_addresses->mac_addresses[0] = strdup("00:01:02:03:04:05");
    mac_addresses->mac_addresses[1] = strdup("00:02:02:03:04:05");

    /* perform test */
    write_interface_mac_addresses(memstream, mac_addresses, &label, empty_label, "ETH", "sub");

    /* check results */
    fclose(memstream);  /* Close the memstream so that we can compare the buffer it writes to with the expected value. */
    STRCMP_EQUAL(" LABEL ETH1 sub MAC=00:01:02:03:04:05\n"
                 " EMPTY ETH2 sub MAC=00:02:02:03:04:05\n"
                 , output_buffer);

    /* cleanup */
    free(output_buffer);
    interface_mac_addresses_free(mac_addresses);
}

TEST(write_interface_mac_addresses, null_sub_label_not_printed)
{
    char const * label;
    char const initial_label[] = "LABEL";
    char const empty_label[] = "     ";
    FILE * memstream;
    interface_mac_address_st * mac_addresses;
    char * output_buffer = NULL;
    size_t output_buffer_size;

    /* setup */
    mock().disable();
    memstream = open_memstream(&output_buffer, &output_buffer_size);
    label = initial_label;
    mac_addresses = (interface_mac_address_st *)calloc(1, sizeof *mac_addresses);
    mac_addresses->num_interfaces = 1;
    mac_addresses->mac_addresses = (char * *)calloc(1, sizeof *mac_addresses->mac_addresses);
    mac_addresses->mac_addresses[0] = strdup("00:01:02:03:04:05");

    /* perform test */
    write_interface_mac_addresses(memstream, mac_addresses, &label, empty_label, "ETH", NULL);

    /* check results */
    fclose(memstream);  /* Close the memstream so that we can compare the buffer it writes to with the expected value. */
    STRCMP_EQUAL(" LABEL ETH1 MAC=00:01:02:03:04:05\n", output_buffer);

    /* cleanup */
    free(output_buffer);
    interface_mac_addresses_free(mac_addresses);
}

TEST_GROUP(interface_mac_addresses_alloc)
{
    void setup(void)
    {
    }

    void teardown(void)
    {
        mock().clear();
    }
};

config_mgr_group_t const * tlr_get_group_ptr(size_t const group_id, size_t const instance, config_mgr_group_type_t const table_index)
{
    mock().actualCall("tlr_get_group_ptr").withParameter("group_id", group_id);

    return (config_mgr_group_t const *)mock().returnPointerValueOrDefault(NULL);
}

TEST(interface_mac_addresses_alloc, failed_get_num_interfaces_returns_null)
{
    config_group_t config_group;
    interface_mac_address_st * mac_addresses;
    interface_mac_address_st * expected_result;

    /* setup */
    expected_result = NULL;
    config_group = config_eth;
    mock()
        .expectOneCall("tlr_get_group_ptr")
        .withParameter("group_id", config_group);

    /* perform test */
    mac_addresses = interface_mac_addresses_alloc(config_group);

    /* check results */
    POINTERS_EQUAL(expected_result, mac_addresses);
    mock().checkExpectations();

    /* cleanup */
}

TEST(interface_mac_addresses_alloc, failed_get_interface_name_returns_null)
{
    config_group_t config_group;
    config_mgr_group_t group;
    interface_mac_address_st * mac_addresses;
    interface_mac_address_st * expected_result;

    /* setup */
    expected_result = NULL;
    config_group = config_eth;
    group.instances = 1;

    mock()
        .expectOneCall("tlr_get_group_ptr")
        .withParameter("group_id", config_group)
        .andReturnValue((void *)&group);
    mock().expectOneCall("interface_get_name_from_config_group")
        .withParameter("config_group", config_group);
    mock().ignoreOtherCalls();

    /* perform test */
    mac_addresses = interface_mac_addresses_alloc(config_group);

    /* check results */
    POINTERS_EQUAL(expected_result, mac_addresses);
    mock().checkExpectations();

    /* cleanup */
}

TEST(interface_mac_addresses_alloc, successful_get_interface_name_returns_non_null)
{
    config_group_t config_group;
    config_mgr_group_t group;
    interface_mac_address_st * mac_addresses;

    /* setup */
    config_group = config_eth;
    group.instances = 1;

    mock()
        .expectOneCall("tlr_get_group_ptr")
        .withParameter("group_id", config_group)
        .andReturnValue((void *)&group);
    mock().expectOneCall("interface_get_name_from_config_group")
        .withParameter("config_group", config_group)
        .andReturnValue((void *)"eth");
    mock().ignoreOtherCalls();

    /* perform test */
    mac_addresses = interface_mac_addresses_alloc(config_group);

    /* check results */
    CHECK(mac_addresses != NULL);
    mock().checkExpectations();

    /* cleanup */
    interface_mac_addresses_free(mac_addresses);
}

TEST(interface_mac_addresses_alloc, get_mac_address_called_for_each_instance)
{
    config_group_t config_group;
    config_mgr_group_t group;
    interface_mac_address_st * mac_addresses;
    char const * const interface_name = "eth";
    size_t instance;

    /* setup */
    config_group = config_eth;
    group.instances = 2;

    mock()
        .expectOneCall("tlr_get_group_ptr")
        .withParameter("group_id", config_group)
        .andReturnValue((void *)&group);
    mock().expectOneCall("interface_get_name_from_config_group")
        .withParameter("config_group", config_group)
        .andReturnValue((void *)"eth");
    for (instance = 1; instance <= group.instances; instance++)
    {
        mock()
            .expectOneCall("get_mac_address")
            .withParameter("interface_name", interface_name)
            .withParameter("instance", instance);
    }
    mock().ignoreOtherCalls();

    /* perform test */
    mac_addresses = interface_mac_addresses_alloc(config_group);

    /* check results */
    CHECK(mac_addresses != NULL);
    mock().checkExpectations();

    /* cleanup */
    interface_mac_addresses_free(mac_addresses);
}

