#include <CppUTest/TestHarness.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CppUTestExt/MockSupport.h"
#include "eth_status_helper.h"
#include "eth_status.h"

#define FREE_BUFFER(x)  if (x != NULL) free((void *) x)

int get_ip_address(char const *interface, char *buffer, int buffer_len)
{
    char const *if_info = ifconfig(interface);
    int result = -1;
    
    if ((buffer) && (buffer_len > 1)) {
        *buffer = 0;
        if (if_info) {
            char const *p = strstr(if_info, "inet addr:");
                
            if (p) {
                char const *end;
                
                p = strstr(p, ":");
                p++;
                end = strstr(p, " ");
                if (buffer_len > (end - p)) {
                    strncpy(buffer, p, (end - p));
                    buffer[(end - p)] = 0;
                    result = 0;
                }
            }
        }
    }
    FREE_BUFFER(if_info);
    
    return result;
}

int get_rx_packets(char const *interface)
{
    char const *if_info = ifconfig(interface);
    char const *p = strstr(if_info, "RX packets:");
    int result = 0;
    
    if (p) {
        p = strstr(p, ":");
        p++;
        result = atoi(p);
    }
    
    FREE_BUFFER(if_info);
    
    return result;
}

int get_rx_errors(char const *interface)
{
    char const *if_info = ifconfig(interface);
    char const *p = strstr(if_info, "RX packets:");
    int result = 0;
    
    if (p) {
        p = strstr(p, "errors:");
        p = strstr(p, ":");
        p++;
        result = atoi(p);
    }
    
    FREE_BUFFER(if_info);
    
    return result;
}

int get_rx_dropped(char const *interface)
{
    char const *if_info = ifconfig(interface);
    char const *p = strstr(if_info, "RX packets:");
    int result = 0;
    
    if (p) {
        p = strstr(p, "dropped:");
        p = strstr(p, ":");
        p++;
        result = atoi(p);
    }
    
    FREE_BUFFER(if_info);
    
    return result;
}

int get_rx_overruns(char const *interface)
{
    char const *if_info = ifconfig(interface);
    char const *p = strstr(if_info, "RX packets:");
    int result = 0;
    
    if (p) {
        p = strstr(p, "overruns:");
        p = strstr(p, ":");
        p++;
        result = atoi(p);
    }
    
    FREE_BUFFER(if_info);
    
    return result;
}

int get_tx_packets(char const *interface)
{
    char const *if_info = ifconfig(interface);
    char const *p = strstr(if_info, "TX packets:");
    int result = 0;
    
    if (p) {
        p = strstr(p, ":");
        p++;
        result = atoi(p);
    }
    
    FREE_BUFFER(if_info);
    
    return result;
}

int get_tx_errors(char const *interface)
{
    char const *if_info = ifconfig(interface);
    char const *p = strstr(if_info, "TX packets:");
    int result = 0;
    
    if (p) {
        p = strstr(p, "errors:");
        p = strstr(p, ":");
        p++;
        result = atoi(p);
    }
    
    FREE_BUFFER(if_info);
    
    return result;
}

int get_tx_dropped(char const *interface)
{
    char const *if_info = ifconfig(interface);
    char const *p = strstr(if_info, "TX packets:");
    int result = 0;
    
    if (p) {
        p = strstr(p, "dropped:");
        p = strstr(p, ":");
        p++;
        result = atoi(p);
    }
    
    FREE_BUFFER(if_info);
    
    return result;
}

int get_tx_overruns(char const *interface)
{
    char const *if_info = ifconfig(interface);
    char const *p = strstr(if_info, "TX packets:");
    int result = 0;
    
    if (p) {
        p = strstr(p, "overruns:");
        p = strstr(p, ":");
        p++;
        result = atoi(p);
    }
    
    FREE_BUFFER(if_info);
    
    return result;
}


int is_link_up(char const *interface)
{
    char const *link_info = mii_tool(interface);
    int result = 0;
    
    if (link_info) {
        if (strstr(link_info, "link ok")) {
            result = 1;
        }
    }
    
    FREE_BUFFER(link_info);
    
    return result;
}

int link_speed(char const *interface)
{
    char const *link_info = mii_tool(interface);
    int result = 0;
    
    if (link_info) {
        if (strstr(link_info, "1000base")) {
            result = 1000;
        } else if (strstr(link_info, "100base")) {
            result = 100;
        } else if (strstr(link_info, "10base")) {
            result = 10;
        }
    }
    
    FREE_BUFFER(link_info);
    
    return result;
}


            