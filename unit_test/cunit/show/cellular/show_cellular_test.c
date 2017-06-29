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
#define     USE_BASIC_STYLE     1
#define     USE_AUTOMATED_STYLE     0
#define     USE_CONSOLE_STYLE     0

#if USE_BASIC_STYLE
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#elif USE_AUTOMATED_STYLE
#include "CUnit/Automated.h"
#else
#include "CUnit/Console.h"
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <json.h>

#if USE_BASIC_STYLE
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#elif USE_AUTOMATED_STYLE
#include "CUnit/Automated.h"
#else
#include "CUnit/Console.h"
#endif

#include "cellular.h"
#include "csman.h"
#include "csid/csid_3g.h"

#include "strlcpy.h"
#include "config_mgr.h"
#include "cellular_work.h"
#include "tlr_common.h"
#include "config_table.h"

static int init_suite(void)
{
    return 0;
}
static int clean_suite(void)
{
    return 0;
}
static int cellular_read_status_failure = 0;
static int cellular_open_status_failure = 0;
static cellular_status_t cellular_status;
static char interface_name[20] = "eth0";

/* Show cellular test case routines*/
static void test_parse_options(void)
{
    char * argv[][3] =
    {
        { "./show_cellular" },
        { "./show_cellular", "-fwhatever" },
        { "./show_cellular", "-fascii" },
        { "./show_cellular", "whatever", "-fjson" },
        { "./show_cellular", "-i0", "-fjson" }
    };
    int argc[] = { 1, 2, 2, 3, 3 };
    size_t len = (sizeof argv / sizeof argv[0]);
    int status_expected[] = { EXIT_SUCCESS, EXIT_FAILURE, EXIT_SUCCESS,
        EXIT_SUCCESS, EXIT_SUCCESS };
    /* setting cellular_status */
    cellular_status.active_sim = 2;
    strcpy(cellular_status.firmware, "MyFirmware");
    strcpy(cellular_status.imei, "MyImei");
    strcpy(cellular_status.ipaddr, "192.168.213.156");
    strcpy(cellular_status.netmask, "255.255.255.0");
    strcpy(cellular_status.gateway, "192.168.1.1");
    strcpy(cellular_status.dns_servers, "222.255.114.215, 167.187.105.199");

    for (int i = 0; i < len; i++)
    {
        int status = cellular_main(argc[i], argv[i]);
        printf("status %d expected %d i=%d\n", status, status_expected[i], i);
        CU_ASSERT_EQUAL(status, status_expected[i]);
    }
}

config_status_t tlr_get_element_value_return = config_status_success;
config_status_t tlr_get_element_value(size_t const group_id,
                                      size_t const instance,
                                      size_t const element_id,
                                      void * const value,
                                      size_t const bytes,
                                      config_mgr_element_type_t const type,
                                      config_mgr_access_level_t const access)
{
    if ((value != NULL) && (bytes > 0))
    {
        char * const data = value;
        memcpy(data, cellular_status.description, bytes);
    }

    return tlr_get_element_value_return;
}


int cellular_open_status(void)
{
    return cellular_open_status_failure;
}

char * cellular_get_interface_name(int const fd, size_t index_3g)
{
    return interface_name;
}

int get_rx_packets(char const * const interface, uint64_t * const value)
{
    return 0;
}

int get_tx_packets(char const * const interface, uint64_t * const value)
{
    return 0;
}

int get_rx_bytes(char const * const interface, uint64_t * const value)
{
    return 0;
}

int get_tx_bytes(char const * const interface, uint64_t * const value)
{
    return 0;
}
config_cellular_state_t get_cellular_state(size_t const instance)
{
    return config_cellular_state_on;
}

int cellular_read_status(int const fd, size_t const index_3g, config_state_cellular_t const sid, void * value, size_t const value_size)
{
    int ret = cellular_read_status_failure;
    //printf("cellular_read_status_failure=%d, sid=%d\n", cellular_read_status_failure, sid);
    if (cellular_read_status_failure == 0)
    {

        if (value == NULL)
        {
            ret = -1;
            goto done;
        }

        switch (sid)
        {
        case config_state_cellular_description:
            /* call get */
            break;
        case config_state_cellular_module:
            if (value_size <= sizeof cellular_status.module)
            {
                char * const dest = value;
                memcpy(dest, cellular_status.module, value_size);
            }
            break;

        case config_state_cellular_firmware_version:
            if (value_size <= sizeof cellular_status.firmware)
            {
                char * const dest = value;
                memcpy(dest, cellular_status.firmware, value_size);
            }
            break;
        case config_state_cellular_hardware_version:
            if (value_size <= sizeof cellular_status.hardware)
            {
                char * const dest = value;
                memcpy(dest, cellular_status.hardware, value_size);
            }
            break;
        case config_state_cellular_imei:
            if (value_size <= sizeof cellular_status.imei)
            {
                char * const dest = value;
                memcpy(dest, cellular_status.imei, value_size);
            }
            break;
        case config_state_cellular_sim_status:
            if (value_size == sizeof cellular_status.active_sim)
            {
                int * dest = value;
                *dest = cellular_status.active_sim;
            }
            break;
        case config_state_cellular_ip_address:
            if (value_size == sizeof cellular_status.ipaddr)
            {
                int * dest = value;
                memcpy(dest, cellular_status.ipaddr, value_size);
            }
            break;

        case config_state_cellular_signal_strength:
            if (value_size == sizeof cellular_status.signal_strength)
            {
                int * const dest = value;
                *dest = cellular_status.signal_strength;
            }
            break;

        case config_state_cellular_signal_quality:
            if (value_size == sizeof cellular_status.signal_quality)
            {
                int * const dest = value;
                *dest = cellular_status.signal_quality;
            }
            break;

        case config_state_cellular_mask:
            if (value_size == sizeof cellular_status.netmask)
            {
                int * dest = value;
                memcpy(dest, cellular_status.netmask, value_size);
            }
            break;
        case config_state_cellular_gateway:
            if (value_size == sizeof cellular_status.gateway)
            {
                int * dest = value;
                memcpy(dest, cellular_status.gateway, value_size);
            }
            break;
        case config_state_cellular_dns_servers:
            if (value_size == sizeof cellular_status.dns_servers)
            {
                int * dest = value;
                memcpy(dest, cellular_status.dns_servers, value_size);
            }
            break;
        case config_state_cellular_registration_status:
            if (value_size == sizeof cellular_status.registration_status)
            {
                cellular_registration_status_t * const dest = value;
                *dest = cellular_status.registration_status;
            }
            break;

        case config_state_cellular_network_provider:
            if (value_size == sizeof cellular_status.network_provider)
            {
                char * dest = value;
                memcpy(dest, cellular_status.network_provider, value_size);
            }
            break;
        case config_state_cellular_connection_type:
            if (value_size == sizeof(cellular_connection_type_t))
            {
                cellular_connection_type_t * const dest = value;
                *dest = cellular_status.connection_type;
            }
            break;
        case config_state_cellular_radio_band:
            if (value_size == sizeof cellular_status.radio_band)
            {
                char * dest = value;
                memcpy(dest, cellular_status.radio_band, value_size);
            }
            break;
        case config_state_cellular_channel:
            if (value_size == sizeof cellular_status.channel)
            {
                int * dest = value;

                *dest = cellular_status.channel;
            }
            break;

        case config_state_cellular_temperature:
            if (value_size == sizeof cellular_status.temperature)
            {
                int * dest = value;

                *dest = cellular_status.temperature;
            }
            break;

        case config_state_cellular_pdp_context:
            if (value_size == sizeof cellular_status.pdp_context)
            {
                char * dest = value;
                memcpy(dest, cellular_status.pdp_context, value_size);
            }
            break;

        default:
            printf("missing id = %d in the switch case\n", sid);
            ret = -1;
            break;

        }
    }
done:
    return ret;
}

#if defined(INCLUDE_TEST_READ_STATUS)
static void test_read_status(void)
{
    char * argv[1] =	{ "./show_cellular" };
    int status;

    /* open failure */
    cellular_open_status_failure = -1;
    status = cellular_main(1, argv);
    CU_ASSERT_EQUAL(status, EXIT_FAILURE);

    /* read failure */
    cellular_open_status_failure = 0;
    cellular_read_status_failure = -1;
    status = cellular_main(1, argv);
    CU_ASSERT_EQUAL(status, EXIT_FAILURE);
}
#endif

extern int get_cellular_stats(cellular_status_t * const pstatus);

static void test_get_cellular_stats(void)
{
    int status;
    cellular_status_t this_status;

    /* setting cellular_status */
    cellular_open_status_failure = 0;
    cellular_read_status_failure = 0;
    tlr_get_element_value_return = config_status_success;
    cellular_status.active_sim = 0;
    memset(cellular_status.firmware, '1', sizeof cellular_status.firmware);
    memset(cellular_status.imei, '2', sizeof cellular_status.imei);
    memset(cellular_status.ipaddr, '3', sizeof cellular_status.ipaddr);
    memset(cellular_status.netmask, '4', sizeof cellular_status.netmask);
    memset(cellular_status.gateway, '5', sizeof cellular_status.gateway);
    memset(cellular_status.dns_servers, '6', sizeof cellular_status.dns_servers);
    memset(cellular_status.description, '7', sizeof cellular_status.description);
    memset(cellular_status.module, '8', sizeof cellular_status.module);
    memset(cellular_status.network_provider, '9', sizeof cellular_status.network_provider);
    memset(cellular_status.radio_band, '0', sizeof cellular_status.radio_band);
    cellular_status.registration_status = cellular_registration_status_idle;
    cellular_status.connection_type = cellular_conenction_type_2g;
    cellular_status.channel = 123;

    cellular_status.rx_packets = this_status.rx_packets = 0;
    cellular_status.tx_packets = this_status.tx_packets = 0;
    cellular_status.rx_bytes = this_status.rx_bytes = 0;
    cellular_status.tx_bytes = this_status.tx_bytes = 0;

    status = get_cellular_stats(&this_status);
    CU_ASSERT_FALSE(status);

    CU_ASSERT_EQUAL(this_status.active_sim, 0);
    CU_ASSERT_EQUAL(this_status.registration_status, cellular_registration_status_idle);
    CU_ASSERT_EQUAL(this_status.connection_type, cellular_conenction_type_2g);
    CU_ASSERT_EQUAL(this_status.channel, 123);
    CU_ASSERT_TRUE(this_status.rx_packets >= cellular_status.rx_packets);
    CU_ASSERT_TRUE(this_status.tx_packets >= cellular_status.tx_packets);
    CU_ASSERT_TRUE(this_status.rx_bytes >= cellular_status.rx_bytes);
    CU_ASSERT_TRUE(this_status.tx_bytes >= cellular_status.tx_bytes);

    for (size_t i=0; i < sizeof cellular_status.firmware; i++)
    {
        if (this_status.firmware[i] != '1')
        {
            CU_FAIL("Invalid firmware");
            break;
        }
    }

    for (size_t i=0; i < sizeof cellular_status.imei; i++)
    {
        if (this_status.imei[i] != '2')
        {
            CU_FAIL("Invalid IMEI");
            break;
        }
    }

    for (size_t i=0; i < sizeof cellular_status.ipaddr; i++)
    {
        if (this_status.ipaddr[i] != '3')
        {
            CU_FAIL("Invalid IP address");
            break;
        }
    }

    for (size_t i=0; i < sizeof cellular_status.netmask; i++)
    {
        if (this_status.netmask[i] != '4')
        {
            CU_FAIL("Invalid Mask");
            break;
        }
    }

    for (size_t i=0; i < sizeof cellular_status.gateway; i++)
    {
        if (this_status.gateway[i] != '5')
        {
            CU_FAIL("Invalid Gateway");
            break;
        }
    }

    for (size_t i=0; i < sizeof cellular_status.dns_servers; i++)
    {
        if (this_status.dns_servers[i] != '6')
        {
            CU_FAIL("Invalid DNS servers");
            break;
        }
    }

    /* should be empty since no active sim */
    if (strlen(this_status.description) > 0)
    {
        printf("read description: %s\n", this_status.description);
        printf("set description: %s\n", cellular_status.description);
        CU_FAIL("Invalid description");
    }

    for (size_t i=0; i < sizeof cellular_status.module; i++)
    {
        if (this_status.module[i] != '8')
        {
            CU_FAIL("Invalid module");
            break;
        }
    }

    for (size_t i=0; i < sizeof cellular_status.network_provider; i++)
    {
        if (this_status.network_provider[i] != '9')
        {
            CU_FAIL("Invalid network provider");
            break;
        }
    }

#if !defined(NO_RADIO_BAND_TEST)
    /* production code isn't currently filling in radio band info */
    for (size_t i=0; i < sizeof cellular_status.radio_band; i++)
    {
        if (this_status.radio_band[i] != '0')
        {
            CU_FAIL("Invalid radio band");
            break;
        }
    }
#endif

    /* setting cellular_status */
    cellular_status.active_sim = 1;
    strcpy(cellular_status.firmware, "");
    strcpy(cellular_status.imei, "");
    strcpy(cellular_status.ipaddr, "");
    strcpy(cellular_status.netmask, "");
    strcpy(cellular_status.gateway, "");
    strcpy(cellular_status.dns_servers, "");
    strcpy(cellular_status.description, "");
    strcpy(cellular_status.module, "");
    strcpy(cellular_status.network_provider, "");
    strcpy(cellular_status.radio_band, "");
    cellular_status.registration_status = cellular_registration_status_registered;
    cellular_status.connection_type = cellular_conenction_type_3g;
    cellular_status.rx_packets = this_status.rx_packets;
    cellular_status.tx_packets = this_status.tx_packets;
    cellular_status.rx_bytes = this_status.rx_bytes;
    cellular_status.tx_bytes = this_status.tx_bytes;
    this_status.rx_packets = 0;
    this_status.tx_packets = 0;
    this_status.rx_bytes = 0;
    this_status.tx_bytes = 0;
    //sleep(2);  //so the statistics would change

    status = get_cellular_stats(&this_status);
    CU_ASSERT_FALSE(status);

    CU_ASSERT_EQUAL(this_status.active_sim, 1);
    CU_ASSERT_TRUE(this_status.rx_packets >= cellular_status.rx_packets);
    CU_ASSERT_TRUE(this_status.tx_packets >= cellular_status.tx_packets);
    CU_ASSERT_TRUE(this_status.rx_bytes >= cellular_status.rx_bytes);
    CU_ASSERT_TRUE(this_status.tx_bytes >= cellular_status.tx_bytes);
    CU_ASSERT_STRING_EQUAL(this_status.firmware, "");
    CU_ASSERT_STRING_EQUAL(this_status.imei, "");
    CU_ASSERT_STRING_EQUAL(this_status.ipaddr, "");
    CU_ASSERT_STRING_EQUAL(this_status.netmask, "");
    CU_ASSERT_STRING_EQUAL(this_status.gateway, "");
    CU_ASSERT_STRING_EQUAL(this_status.dns_servers, "");
    CU_ASSERT_STRING_EQUAL(this_status.description, "");
    CU_ASSERT_STRING_EQUAL(this_status.module, "");
    CU_ASSERT_STRING_EQUAL(this_status.network_provider, "");
#if !defined(NO_RADIO_BAND_TEST)
    CU_ASSERT_STRING_EQUAL(this_status.radio_band, "");
#endif
    CU_ASSERT_EQUAL(this_status.registration_status, cellular_registration_status_registered);
    CU_ASSERT_EQUAL(this_status.connection_type, cellular_conenction_type_3g);

    /* setting cellular_status */
    cellular_status.active_sim = 2;
    strcpy(cellular_status.firmware, "MyFirmware");
    strcpy(cellular_status.imei, "MyImei");
    strcpy(cellular_status.ipaddr, "192.168.213.156");
    strcpy(cellular_status.netmask, "255.255.255.0");
    strcpy(cellular_status.gateway, "192.168.1.1");
    strcpy(cellular_status.dns_servers, "222.255.114.215, 167.187.105.199");

    cellular_status.rx_packets = this_status.rx_packets;
    cellular_status.tx_packets = this_status.tx_packets;
    cellular_status.rx_bytes = this_status.rx_bytes;
    cellular_status.tx_bytes = this_status.tx_bytes;
    this_status.rx_packets = 0;
    this_status.tx_packets = 0;
    this_status.rx_bytes = 0;
    this_status.tx_bytes = 0;

    //sleep(2);//so the statistics would change

    strcpy(cellular_status.description, "MyDescription");
    strcpy(cellular_status.module, "MyModule");
    strcpy(cellular_status.network_provider, "MyProvider");
    strcpy(cellular_status.radio_band, "MyBand");
    cellular_status.registration_status = cellular_registration_status_search;
    cellular_status.connection_type = cellular_conenction_type_4g;

    status = get_cellular_stats(&this_status);
    CU_ASSERT_FALSE(status);

    CU_ASSERT_EQUAL(this_status.active_sim, 2);
    CU_ASSERT_TRUE(this_status.rx_packets >= cellular_status.rx_packets);
    CU_ASSERT_TRUE(this_status.tx_packets >= cellular_status.tx_packets);
    CU_ASSERT_TRUE(this_status.rx_bytes >= cellular_status.rx_bytes);
    CU_ASSERT_TRUE(this_status.tx_bytes >= cellular_status.tx_bytes);
    CU_ASSERT_STRING_EQUAL(this_status.firmware, "MyFirmware");
    CU_ASSERT_STRING_EQUAL(this_status.imei, "MyImei");
    CU_ASSERT_STRING_EQUAL(this_status.ipaddr, "192.168.213.156");
    CU_ASSERT_STRING_EQUAL(this_status.netmask, "255.255.255.0");
    CU_ASSERT_STRING_EQUAL(this_status.gateway, "192.168.1.1");
    CU_ASSERT_STRING_EQUAL(this_status.dns_servers, "222.255.114.215, 167.187.105.199");
    CU_ASSERT_STRING_EQUAL(this_status.description, "MyDescription");
    CU_ASSERT_STRING_EQUAL(this_status.module, "MyModule");
    CU_ASSERT_STRING_EQUAL(this_status.network_provider, "MyProvider");
#if !defined(NO_RADIO_BAND_TEST)
    CU_ASSERT_STRING_EQUAL(this_status.radio_band, "MyBand");
#endif
    CU_ASSERT_EQUAL(this_status.registration_status, cellular_registration_status_search);
    CU_ASSERT_EQUAL(this_status.connection_type, cellular_conenction_type_4g);

#if 0 // TODO: Need to revisit this test
    /* setting cellular_status */
    snprintf(interface_name, sizeof interface_name, "%s", "ppp-0"); //this interface name does not exist on the host machine
    this_status.rx_packets = 0;
    this_status.tx_packets = 0;
    this_status.rx_bytes = 0;
    this_status.tx_bytes = 0;

    status = get_cellular_stats(&this_status);
    CU_ASSERT_TRUE(status);
    CU_ASSERT_EQUAL(this_status.rx_packets, 0);
    CU_ASSERT_EQUAL(this_status.tx_packets, 0);
    CU_ASSERT_EQUAL(this_status.rx_bytes, 0);
    CU_ASSERT_EQUAL(this_status.tx_bytes, 0);
#endif

}

extern char * cellular_get_signal_strength_status_string(cellular_status_t const * const pstatus);

static void test_3g_get_signal_strength_status(void)
{

    cellular_status.connection_type = cellular_conenction_type_3g;
    cellular_status.signal_strength = -65;

    {
        char * excellent_signal = cellular_get_signal_strength_status_string(&cellular_status);
        if (strstr(excellent_signal, "Excellent") == NULL)
        {
            printf("Expect \"Excellent\" signal strength but got %s\n", excellent_signal);
            CU_FAIL("Fail Excellent signal strength");
        }
    }

    {
        int test_good_signal_strength[] = { -70, -75, -85 };
        int i;

        for (i = 0; i < (sizeof test_good_signal_strength / sizeof test_good_signal_strength[0]); i++)
        {
            char * good_signal;

            cellular_status.signal_strength = test_good_signal_strength[i];
            good_signal = cellular_get_signal_strength_status_string(&cellular_status);

            if (strstr(good_signal, "Good") == NULL)
            {
                printf("Expect \"Good\" signal strength but got %s\n", good_signal);
                CU_FAIL("Fail Good signal strength");
            }
        }
    }

    {
        int test_fair_signal_strength[] = { -86, -90, -100 };
        int i;

        for (i = 0; i < (sizeof test_fair_signal_strength / sizeof test_fair_signal_strength[0]); i++)
        {
            char * fair_signal;

            cellular_status.signal_strength = test_fair_signal_strength[i];
            fair_signal = cellular_get_signal_strength_status_string(&cellular_status);

            if (strstr(fair_signal, "Fair") == NULL)
            {
                printf("Expect \"Fair\" signal strength but got %s\n", fair_signal);
                CU_FAIL("Fail fair signal strength");
            }
        }
    }

    {
        int test_poor_signal_strength[] = { -101, -109 };
        int i;

        for (i = 0; i < (sizeof test_poor_signal_strength / sizeof test_poor_signal_strength[0]); i++)
        {
            char * poor_signal;

            cellular_status.signal_strength = test_poor_signal_strength[i];
            poor_signal = cellular_get_signal_strength_status_string(&cellular_status);

            if (strstr(poor_signal, "Poor") == NULL)
            {
                printf("Expect \"Poor\" signal strength but got %s\n", poor_signal);
                CU_FAIL("Fail poor signal strength");
            }
        }
    }

    cellular_status.signal_strength = -110;

    {
        char * no_service_signal = cellular_get_signal_strength_status_string(&cellular_status);
        if (strstr(no_service_signal, "No service") == NULL)
        {
            printf("Expect \"No service\" signal strength but got %s\n", no_service_signal);
            CU_FAIL("Fail no service signal strength");
        }
    }

}

static void test_4g_get_signal_strength_status(void)
{

    cellular_status.connection_type = cellular_conenction_type_4g;
    cellular_status.signal_strength = -85;

    {
        char * excellent_signal = cellular_get_signal_strength_status_string(&cellular_status);
        if (strstr(excellent_signal, "Excellent") == NULL)
        {
            printf("Expect \"Excellent\" signal strength but got %s\n", excellent_signal);
            CU_FAIL("Fail Excellent signal strength");
        }
    }

    {
        int test_good_signal_strength[] = { -90, -95, -105 };
        int i;

        for (i = 0; i < (sizeof test_good_signal_strength / sizeof test_good_signal_strength[0]); i++)
        {
            char * good_signal;

            cellular_status.signal_strength = test_good_signal_strength[i];
            good_signal = cellular_get_signal_strength_status_string(&cellular_status);

            if (strstr(good_signal, "Good") == NULL)
            {
                printf("Expect \"Good\" signal strength but got %s\n", good_signal);
                CU_FAIL("Fail Good signal strength");
            }
        }
    }

    {
        int test_fair_signal_strength[] = { -106, -110, -115 };
        int i;

        for (i = 0; i < (sizeof test_fair_signal_strength / sizeof test_fair_signal_strength[0]); i++)
        {
            char * fair_signal;

            cellular_status.signal_strength = test_fair_signal_strength[i];
            fair_signal = cellular_get_signal_strength_status_string(&cellular_status);

            if (strstr(fair_signal, "Fair") == NULL)
            {
                printf("Expect \"Fair\" signal strength but got %s\n", fair_signal);
                CU_FAIL("Fail fair signal strength");
            }
        }
    }

    {
        int test_poor_signal_strength[] = { -116, -118, -120 };
        int i;

        for (i = 0; i < (sizeof test_poor_signal_strength / sizeof test_poor_signal_strength[0]); i++)
        {
            char * poor_signal;

            cellular_status.signal_strength = test_poor_signal_strength[i];
            poor_signal = cellular_get_signal_strength_status_string(&cellular_status);

            if (strstr(poor_signal, "Poor") == NULL)
            {
                printf("Expect \"Poor\" signal strength but got %s\n", poor_signal);
                CU_FAIL("Fail poor signal strength");
            }
        }
    }

    cellular_status.signal_strength = -125;

    {
        char * no_service_signal = cellular_get_signal_strength_status_string(&cellular_status);
        if (strstr(no_service_signal, "No service") == NULL)
        {
            printf("Expect \"No service\" signal strength but got %s\n", no_service_signal);
            CU_FAIL("Fail no service signal strength");
        }
    }

}

extern char * cellular_get_signal_quality_status_string(cellular_status_t const * const pstatus);

static void test_3g_get_signal_quality_status(void)
{

    cellular_status.connection_type = cellular_conenction_type_3g;

    {
        int test_good_signal_quality[] = { -3, -6 }; // XXX was { 0, -3, -6 } but get_signal_quality_status changed so that 0 returns empty string
        int i;

        for (i = 0; i < (sizeof test_good_signal_quality / sizeof test_good_signal_quality[0]); i++)
        {
            char * excellent_quality;
            cellular_status.signal_quality = test_good_signal_quality[i];
            cellular_status.connection_type = cellular_connection_type_unknown;
            excellent_quality = cellular_get_signal_quality_status_string(&cellular_status);

            if (strstr(excellent_quality, "Excellent") == NULL)
            {
                printf("Expect \"Excellent\" quality but got %s\n", excellent_quality);
                CU_FAIL("Fail Excellent signal quality");
            }
        }
    }

    {
        int test_good_signal_quality[] = { -7, -8, -10 };
        int i;

        for (i = 0; i < (sizeof test_good_signal_quality / sizeof test_good_signal_quality[0]); i++)
        {
            char * good_quality;

            cellular_status.signal_quality = test_good_signal_quality[i];
            good_quality = cellular_get_signal_quality_status_string(&cellular_status);

            if (strstr(good_quality, "Good") == NULL)
            {
                printf("Expect \"Good\" quality but got %s\n", good_quality);
                CU_FAIL("Fail Good signal quality");
            }
        }
    }

    {
        int test_fair_signal_quality[] = { -11, -18, -20 };
        int i;

        for (i = 0; i < (sizeof test_fair_signal_quality / sizeof test_fair_signal_quality[0]); i++)
        {
            char * fair_quality;

            cellular_status.signal_quality = test_fair_signal_quality[i];
            fair_quality = cellular_get_signal_quality_status_string(&cellular_status);

            if (strstr(fair_quality, "Fair") == NULL)
            {
                printf("Expect \"Fair\" quality but got %s\n", fair_quality);
                CU_FAIL("Fail fair signal quality");
            }
        }
    }

}

static void test_4g_get_signal_quality_status(void)
{

    cellular_status.connection_type = cellular_conenction_type_4g;

    {
        int test_good_signal_quality[] = { -8 };
        int i;

        for (i = 0; i < (sizeof test_good_signal_quality / sizeof test_good_signal_quality[0]); i++)
        {
            char * excellent_quality;

            cellular_status.signal_quality = test_good_signal_quality[i];
            excellent_quality = cellular_get_signal_quality_status_string(&cellular_status);

            if (strstr(excellent_quality, "Excellent") == NULL)
            {
                printf("Expect \"Excellent\" quality but got %s\n", excellent_quality);
                CU_FAIL("Fail Excellent signal quality");
            }
        }
    }

    {
        int test_good_signal_quality[] = { -9, -10, -12 };
        int i;

        for (i = 0; i < (sizeof test_good_signal_quality / sizeof test_good_signal_quality[0]); i++)
        {
            char * good_quality;

            cellular_status.signal_quality = test_good_signal_quality[i];
            good_quality = cellular_get_signal_quality_status_string(&cellular_status);

            if (strstr(good_quality, "Good") == NULL)
            {
                printf("Expect \"Good\" quality but got %s\n", good_quality);
                CU_FAIL("Fail Good signal quality");
            }
        }
    }

    {
        int test_fair_signal_quality[] = { -13, -18 };
        int i;

        for (i = 0; i < (sizeof test_fair_signal_quality / sizeof test_fair_signal_quality[0]); i++)
        {
            char * fair_quality;

            cellular_status.signal_quality = test_fair_signal_quality[i];
            fair_quality = cellular_get_signal_quality_status_string(&cellular_status);

            if (strstr(fair_quality, "Fair") == NULL)
            {
                printf("Expect \"Fair\" quality but got %s\n", fair_quality);
                CU_FAIL("Fail fair signal quality");
            }
        }
    }

}
extern int get_cellular_description(size_t const active_sim, char * const description, size_t const length);

static int tlr_handle;
tlr_handle_t tlr_config_open(size_t const group_id, size_t const instance, config_mgr_access_level_t const access)
{
    return &tlr_handle;
}

config_status_t tlr_config_read(tlr_handle_t const handle, size_t const element_id, char * const data, size_t const bytes)
{
    config_status_t status = config_status_success;

    if ((element_id != config_cellular_description) || (data == NULL))
    {
        printf("Not unit test on %zd element id or invalid data argument\n", element_id);
        status = config_status_invalid_parameter;
    }
    else
    {
        memcpy(data, cellular_status.description, bytes);
    }

    return status;
}

static void test_get_cellular_description(void)
{
    int status;
    char description[CELLULAR_DESCRIPTION_LENGTH * 2] = "";
    size_t const length = sizeof description;


    status = get_cellular_description(1, NULL, length);
    CU_ASSERT_NOT_EQUAL(status, 0);

    tlr_get_element_value_return = config_status_invalid_parameter;
    strcpy(cellular_status.description, "MyDescription");
    status = get_cellular_description(1, description, 2);
    CU_ASSERT_NOT_EQUAL(status, 0);

    tlr_get_element_value_return = config_status_success;
    status = get_cellular_description(1, description, 2);
    CU_ASSERT_FALSE(status);
    CU_ASSERT_EQUAL(strlen(description), 2);
    CU_ASSERT_STRING_EQUAL(description, "My");

    status = get_cellular_description(1, description, length);
    CU_ASSERT_FALSE(status);
    CU_ASSERT_STRING_EQUAL(description, "MyDescription");

}

extern char const * get_registration_status(cellular_status_t const * const pstatus);

static void test_get_registration_status(void)
{
    char const * status;

    tlr_get_element_value_return = config_status_success;
    cellular_status.registration_status = cellular_registration_status_idle;
    status = cellular_get_registration_status_string(&cellular_status);
    CU_ASSERT_STRING_EQUAL(status, "Not registered");

    tlr_get_element_value_return = config_status_success;
    cellular_status.registration_status = cellular_registration_status_registered;
    status = cellular_get_registration_status_string(&cellular_status);
    CU_ASSERT_STRING_EQUAL(status, "Registered");

    cellular_status.registration_status = cellular_registration_status_search;
    status = cellular_get_registration_status_string(&cellular_status);
    CU_ASSERT_STRING_EQUAL(status, "Searching");

    cellular_status.registration_status = cellular_registration_status_roaming;
    status = cellular_get_registration_status_string(&cellular_status);
    CU_ASSERT_STRING_EQUAL(status, "Roaming");

    cellular_status.registration_status = 10;
    status = cellular_get_registration_status_string(&cellular_status);
    CU_ASSERT_PTR_NULL(status);

}

extern char const * get_connection_type(cellular_status_t const * const pstatus);

static void test_get_connection_type(void)
{
    char const * type;

    tlr_get_element_value_return = config_status_success;
    cellular_status.connection_type = cellular_conenction_type_2g;
    type = cellular_get_connection_type_string(&cellular_status);
    CU_ASSERT_STRING_EQUAL(type, "2G");

    tlr_get_element_value_return = config_status_success;
    cellular_status.connection_type = cellular_conenction_type_3g;
    type = cellular_get_connection_type_string(&cellular_status);
    CU_ASSERT_STRING_EQUAL(type, "3G");

    cellular_status.connection_type = cellular_conenction_type_4g;
    type = cellular_get_connection_type_string(&cellular_status);
    CU_ASSERT_STRING_EQUAL(type, "4G");

    cellular_status.connection_type = cellular_connection_type_unknown;
    type = cellular_get_connection_type_string(&cellular_status);
    CU_ASSERT_STRING_EQUAL(type, "");

    cellular_status.connection_type = 4;
    type = cellular_get_connection_type_string(&cellular_status);
    CU_ASSERT_STRING_EQUAL(type, "");

}

/************* Test Runner Code goes here **************/

int main(void)
{
    int number_of_failures;
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("show_cellular_test_suite", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((CU_add_test(pSuite, "test_parse_options", test_parse_options) == NULL)
        || (CU_add_test(pSuite, "test_get_cellular_stats", test_get_cellular_stats) == NULL)
        || (CU_add_test(pSuite, "test_3g_get_signal_strength_status", test_3g_get_signal_strength_status) == NULL)
        || (CU_add_test(pSuite, "test_4g_get_signal_strength_status", test_4g_get_signal_strength_status) == NULL)
        || (CU_add_test(pSuite, "test_3g_get_signal_quality_status", test_3g_get_signal_quality_status) == NULL)
        || (CU_add_test(pSuite, "test_4g_get_signal_quality_status", test_4g_get_signal_quality_status) == NULL)
        || (CU_add_test(pSuite, "test_get_cellular_description", test_get_cellular_description) == NULL)
        || (CU_add_test(pSuite, "test_get_registration_status", test_get_registration_status) == NULL)
        || (CU_add_test(pSuite, "test_get_connection_type", test_get_connection_type) == NULL)
#if defined(INCLUDE_TEST_READ_STATUS)
        || (CU_add_test(pSuite, "test_read_status", test_read_status) == NULL)
#endif
       )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

#if USE_BASIC_STYLE
    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");

#elif USE_AUTOMATED_STYLE
    // Run all tests using the automated interface
    CU_automated_run_tests();
    CU_list_tests_to_file();

    // Run all tests using the console interface
    CU_console_run_tests();
#endif

    //	Clean up registry and return
    number_of_failures = CU_get_number_of_failures();
    CU_cleanup_registry();

    //	Ensure we return an error if any tests failed
    if (number_of_failures)
        return -1;

    return CU_get_error();
}

/*** Stubs ****/

void cellular_close_status(int const fd)
{
    return;
}

void tlr_config_close(tlr_handle_t const handle)
{

}

bool cellular_get_usb_ready(int const fd, size_t const index_3g)
{
    return true;
}
cellular_connect_status_t cellular_get_connection_status(int const fd, size_t const index_3g)
{
    return cellular_connected;
}
size_t tlr_get_max_instance(size_t const group_id, config_mgr_table_type_t const type)
{
    return 1;
}

struct tlr_show_object_t // TODO FIX THIS!!!
{
    config_mgr_group_t const * group;
    json_object * json_parent;
};

tlr_show_object_t * tlr_show_object_create(size_t const group_id, size_t const instance)
{
    tlr_show_object_t * show_object;

    show_object = malloc(sizeof *show_object);

    return show_object;
}

void tlr_show_object_add_element(tlr_show_object_t * const show_object, size_t const element_id, void const * const value)
{

}
config_mgr_group_t const * tlr_get_group_ptr(size_t const group_id, size_t const instance, config_mgr_group_type_t const table_index)
{
    static config_mgr_group_t group;

    return &group;
}
char const * tlr_show_object_to_json_string(tlr_show_object_t const * const show_object)
{
    static char string[] ="test json string";

    return string;
}

void tlr_show_object_destroy(tlr_show_object_t * const show_object)
{
    free(show_object);
}



