
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


#include "eth_status_helper.h"
#include "eth_status.h"


/* Test Suite setup and cleanup functions: */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

char const *normal_ifconfig =
"eth0      Link encap:Ethernet  HWaddr 90:b1:1c:9c:3c:5b  \r\n" \
"          inet addr:10.52.18.103  Bcast:10.52.18.255  Mask:255.255.255.0\r\n" \
"          inet6 addr: fe80::92b1:1cff:fe9c:3c5b/64 Scope:Link\r\n" \
"          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1\r\n" \
"          RX packets:1800462 errors:0 dropped:0 overruns:0 frame:0\r\n" \
"          TX packets:1094922 errors:0 dropped:0 overruns:0 carrier:0\r\n" \
"          collisions:0 txqueuelen:1000 \r\n" \
"          RX bytes:912015566 (912.0 MB)  TX bytes:211296176 (211.2 MB)\r\n";

char const *disconnected_ifconfig =
"eth0      Link encap:Ethernet  HWaddr 90:b1:1c:9c:3c:5b    \r\n" \
"          inet6 addr: fe80::92b1:1cff:fe9c:3c5b/64 Scope:Link  \r\n" \
"          UP BROADCAST MULTICAST  MTU:1500  Metric:1  \r\n" \
"          RX packets:1839873 errors:0 dropped:0 overruns:0 frame:0  \r\n" \
"          TX packets:1108813 errors:0 dropped:0 overruns:0 carrier:0  \r\n" \
"          collisions:0 txqueuelen:1000   \r\n" \
"          RX bytes:934115551 (934.1 MB)  TX bytes:213239979 (213.2 MB)  \r\n";

char const *invalid_if_ifconfig =
"eth0: error fetching interface information: Device not found\r\n";

char const *link_1000baset =
"eth0: negotiated 1000baseTx-FD flow-control, link ok\r\n";

char const *link_100baset =
"eth0: negotiated 100baseTx-FD flow-control, link ok\r\n";

char const *link_10baset =
"eth0: negotiated 10baseTx-FD flow-control, link ok\r\n";

char const *link_down =
"eth0: no link\r\n";

char const *link_invalid_if =
"SIOCGMIIPHY on 'eth1' failed: No such device\r\n";



char * get_malloced_buffer(char const *const_buffer)
{
    char * result = (char *) malloc(strlen(const_buffer) + 1);

    if (result) {
        strcpy(result, const_buffer);
    } else {
        printf("Out of memory");
        exit(1);
    }

    return result;
}


char    *simulated_if_config_return = NULL;

void simulate_ifconfig ( char const *str )
{
    simulated_if_config_return = NULL;
    if ( str != NULL )
        {
        simulated_if_config_return = get_malloced_buffer ( str );
        }
}


char    *simulated_mii_info_return = NULL;

void simulate_mii_info ( char const *str )
{
    simulated_mii_info_return = NULL;
    if ( str != NULL )
        {
        simulated_mii_info_return = get_malloced_buffer ( str );
        }
}


/************* Test case functions ****************/

/******* IP Address Test Cases ******/
void get_ip_address_normal()
{
    char buffer[30];
    int     retv;

    simulate_ifconfig ( normal_ifconfig );
    retv = get_ip_address("eth0", buffer, sizeof buffer);
    CU_ASSERT_EQUAL_FATAL(retv, 0);
    CU_ASSERT_STRING_EQUAL(buffer, "10.52.18.103" );
}
void get_ip_address_overflow()
{
    char buffer[3];
    int     retv;

    simulate_ifconfig ( normal_ifconfig );
    retv = get_ip_address("eth0", buffer, sizeof buffer);
    CU_ASSERT_EQUAL_FATAL(retv, -1);
    CU_ASSERT_STRING_EQUAL(buffer, "" );
}
void get_ip_address_nullbuffer()
{
    int     retv;

    simulate_ifconfig ( normal_ifconfig );
    retv = get_ip_address("eth0", NULL, 0);
    CU_ASSERT_EQUAL_FATAL(retv, -1);

}
void get_ip_address_disconnected()
{
    char buffer[30];
    int     retv;

    simulate_ifconfig ( disconnected_ifconfig );
    retv = get_ip_address("eth0", buffer, sizeof buffer);
    CU_ASSERT_EQUAL_FATAL(retv, -1);
    CU_ASSERT_STRING_EQUAL(buffer, "" );
}

void get_ip_address_null_return()
{
    char buffer[30];
    int     retv;

    simulate_ifconfig ( NULL );     // if config call will return NULL
    retv = get_ip_address("eth0", buffer, sizeof buffer);
    CU_ASSERT_EQUAL(retv, -1);
}


void get_ip_address_tests()
{
    get_ip_address_normal();
    get_ip_address_overflow();
    get_ip_address_nullbuffer();
    get_ip_address_disconnected();
    get_ip_address_null_return();
}



/******* RX Packets Test Cases ******/

void get_rx_packets_normal()
{
    simulate_ifconfig ( normal_ifconfig );
    CU_ASSERT_EQUAL ( get_rx_packets("eth0"), 1800462 );
}

void get_rx_packets_disconnected()
{
    simulate_ifconfig(disconnected_ifconfig);
    CU_ASSERT_EQUAL(get_rx_packets("eth0"), 1839873);
}

void get_rx_packets_invalid()
{
    simulate_ifconfig(invalid_if_ifconfig);
    CU_ASSERT_EQUAL(get_rx_packets("eth0"), 0);
}


void get_rx_packets_tests()
{
    get_rx_packets_normal();
    get_rx_packets_disconnected();
    get_rx_packets_invalid();
}


/******* Link Speed Test Cases ******/

void link_speed_1000Mbits()
{
    simulate_mii_info(link_1000baset);
    CU_ASSERT_EQUAL(link_speed("eth0"), 1000);
}
void link_speed_100Mbits()
{
    simulate_mii_info(link_100baset);
    CU_ASSERT_EQUAL(link_speed("eth0"), 100);
}
void link_speed_10Mbits()
{
    simulate_mii_info(link_10baset);
    CU_ASSERT_EQUAL(link_speed("eth0"), 10);
}
void link_speed_down()
{
    simulate_mii_info(link_down);
    CU_ASSERT_EQUAL(link_speed("eth0"), 0);
}

void link_speed_null_return()
{
    simulate_mii_info ( NULL );
    CU_ASSERT_EQUAL(link_speed("eth0"), 0);
}

void link_speed_tests()
{
    link_speed_1000Mbits();
    link_speed_100Mbits();
    link_speed_10Mbits();
    link_speed_down();
    link_speed_null_return();
}


/************* Test Runner Code goes here **************/

int main ( void )
{
    int result;
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if ( CUE_SUCCESS != CU_initialize_registry() )
    {
        result = CU_get_error();
        goto error;
    }

    /* add a suite to the registry */
    pSuite = CU_add_suite( "eth_status_test_suite", init_suite, clean_suite );
    if ( NULL == pSuite )
    {
        result = CU_get_error();
        goto error;
    }

    /* add the tests to the suite */
    if ((CU_add_test(pSuite, "get_ip_address", get_ip_address_tests)==NULL) ||
        (CU_add_test(pSuite, "get_rx_packets", get_rx_packets_tests)==NULL) ||
        (CU_add_test(pSuite, "link_speed", link_speed_tests)==NULL))
    {
        result = CU_get_error();
        goto error;
    }

#if USE_BASIC_STYLE
    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
	if (CU_basic_run_tests() != CUE_SUCCESS)
	{
        result = CU_get_error();
        goto error;
	}
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

    result = CU_get_number_of_failures();

error:
	CU_cleanup_registry();
	return result;
}



/****************  Stubs   ***************************/


char const *ifconfig(char const *interface)
{
    char    *retv;
    CU_ASSERT_STRING_EQUAL_FATAL(interface, "eth0" ); // All our tests are using eth0..if anything else.. thats bad
    retv = simulated_if_config_return;
    simulated_if_config_return = NULL;
    return retv;
}


char const *mii_tool(char const *interface)
{
    char    *retv;
    CU_ASSERT_STRING_EQUAL_FATAL(interface, "eth0" ); // All our tests are using eth0..if anything else.. thats bad
    retv = simulated_mii_info_return;
    simulated_mii_info_return = NULL;
    return retv;
}


