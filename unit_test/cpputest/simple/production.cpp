#include <stdio.h>
#include <stdlib.h>
#include "eth_status.h"

int main(int argc, char *argv[])
{
    char const * const eth0 = "eth0";
    char buffer[30];
    
    if (is_link_up(eth0)) {
        if (get_ip_address(eth0, buffer, sizeof(buffer))) {
            printf (" ip address: none\n");
        } else {
            printf(" ip address: %s\n", buffer);
        }
        printf(" link speed: %d Mbits/s\n", link_speed(eth0));
        printf(" rx packets: %d\n", get_rx_packets(eth0));
        printf("  rx errors: %d\n", get_rx_errors(eth0));
        printf(" rx dropped: %d\n", get_rx_dropped(eth0));
        printf("rx_overruns: %d\n", get_rx_overruns(eth0));
        printf(" tx packets: %d\n", get_tx_packets(eth0));
        printf("  tx errors: %d\n", get_tx_errors(eth0));
        printf(" tx dropped: %d\n", get_tx_dropped(eth0));
        printf("tx_overruns: %d\n", get_tx_overruns(eth0));
    } else {
        printf("link is down\n");
    }
    return 0;
}

