#ifndef eth_status_h
#define eth_status_h

int get_ip_address(char const *interface, char * buffer, int buffer_len);
int get_rx_packets(char const *interface);
int get_rx_errors(char const *interface);
int get_rx_dropped(char const *interface);
int get_rx_overruns(char const *interface);
int get_tx_packets(char const *interface);
int get_tx_errors(char const *interface);
int get_tx_dropped(char const *interface);
int get_tx_overruns(char const *interface);
int is_link_up(char const *interface);
int link_speed(char const *interface);

#endif