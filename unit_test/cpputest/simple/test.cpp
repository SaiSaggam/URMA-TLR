#include <CppUTest/TestHarness.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CppUTestExt/MockSupport.h"
#include "eth_status_helper.h"
#include "eth_status.h"

char const *ifconfig(char const *interface)
{
    mock().actualCall("ifconfig").withParameter("interface", interface);
    return mock().returnValue().getStringValue();
}

char const *mii_tool(char const *interface)
{
    mock().actualCall("mii_tool").withParameter("interface", interface);
    return mock().returnValue().getStringValue();
}

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


TEST_GROUP(get_ip_address)
{
    void teardown()
    {
        mock().clear();
    }
};
TEST(get_ip_address, normal)
{
    char *ifconfig = get_malloced_buffer(normal_ifconfig);
    char buffer[30];
    
    
    mock().expectOneCall("ifconfig").withParameter("interface", "eth0").andReturnValue(ifconfig);
    LONGS_EQUAL(get_ip_address("eth0", buffer, sizeof buffer), 0);
    STRCMP_EQUAL(buffer, "10.52.18.103");
    mock().checkExpectations();
}
TEST(get_ip_address, overflow)
{
    char *ifconfig = get_malloced_buffer(normal_ifconfig);
    char buffer[3];
    
    
    mock().expectOneCall("ifconfig").withParameter("interface", "eth0").andReturnValue(ifconfig);
    LONGS_EQUAL(get_ip_address("eth0", buffer, sizeof buffer), -1);
    STRCMP_EQUAL(buffer, "");
    mock().checkExpectations();
}
TEST(get_ip_address, null_buffer)
{
    char *ifconfig = get_malloced_buffer(normal_ifconfig);
    
    
    mock().expectOneCall("ifconfig").withParameter("interface", "eth0").andReturnValue(ifconfig);
    LONGS_EQUAL(get_ip_address("eth0", NULL, 0), -1);
    mock().checkExpectations();
}
TEST(get_ip_address, disconnected)
{
    char *ifconfig = get_malloced_buffer(disconnected_ifconfig);
    char buffer[30];
    
    
    mock().expectOneCall("ifconfig").withParameter("interface", "eth0").andReturnValue(ifconfig);
    LONGS_EQUAL(get_ip_address("eth0", buffer, sizeof buffer), -1);
    STRCMP_EQUAL(buffer, "");
    mock().checkExpectations();
}
TEST(get_ip_address, invalid)
{
    char *ifconfig = get_malloced_buffer(invalid_if_ifconfig);
    char buffer[30];
    
    
    mock().expectOneCall("ifconfig").withParameter("interface", "eth0").andReturnValue(ifconfig);
    LONGS_EQUAL(get_ip_address("eth0", buffer, sizeof buffer), -1);
    STRCMP_EQUAL(buffer, "");
    mock().checkExpectations();
}




TEST_GROUP(get_rx_packets)
{
    void teardown()
    {
        mock().clear();
    }
};
TEST(get_rx_packets, normal)
{
    char *ifconfig = get_malloced_buffer(normal_ifconfig);
    
    mock().expectOneCall("ifconfig").withParameter("interface", "eth0").andReturnValue(ifconfig);
    LONGS_EQUAL(get_rx_packets("eth0"), 1800462);
    mock().checkExpectations();
}
TEST(get_rx_packets, disconnected)
{
    char *ifconfig = get_malloced_buffer(disconnected_ifconfig);
    
    mock().expectOneCall("ifconfig").withParameter("interface", "eth0").andReturnValue(ifconfig);
    LONGS_EQUAL(get_rx_packets("eth0"), 1839873);
    mock().checkExpectations();
}
TEST(get_rx_packets, invalid)
{
    char *ifconfig = get_malloced_buffer(invalid_if_ifconfig);
    
    mock().expectOneCall("ifconfig").withParameter("interface", "eth0").andReturnValue(ifconfig);
    LONGS_EQUAL(get_rx_packets("eth0"), 0);
    mock().checkExpectations();
}


TEST_GROUP(link_speed)
{
    void teardown()
    {
        mock().clear();
    }
};
TEST(link_speed, _1000Mbits)
{
    char *mii_info = get_malloced_buffer(link_1000baset);
    
    mock().expectOneCall("mii_tool").withParameter("interface", "eth0").andReturnValue(mii_info);
    LONGS_EQUAL(link_speed("eth0"), 1000);
    mock().checkExpectations();
}
TEST(link_speed, _100Mbits)
{
    char *mii_info = get_malloced_buffer(link_100baset);
    
    mock().expectOneCall("mii_tool").withParameter("interface", "eth0").andReturnValue(mii_info);
    LONGS_EQUAL(link_speed("eth0"), 100);
    mock().checkExpectations();
}
TEST(link_speed, _10Mbits)
{
    char *mii_info = get_malloced_buffer(link_10baset);
    
    mock().expectOneCall("mii_tool").withParameter("interface", "eth0").andReturnValue(mii_info);
    LONGS_EQUAL(link_speed("eth0"), 10);
    mock().checkExpectations();
}
TEST(link_speed, down)
{
    char *mii_info = get_malloced_buffer(link_down);
    
    mock().expectOneCall("mii_tool").withParameter("interface", "eth0").andReturnValue(mii_info);
    LONGS_EQUAL(link_speed("eth0"), 0);
    mock().checkExpectations();
}

