#ifndef	__NET_UTIL_H__
#define __NET_UTIL_H__

#include <sys/socket.h>
#include <sys/types.h> 
#include <sys/ioctl.h>
#include <net/if.h>
#include "ci_cmd.h"

int send_data(int fd, struct bss_data *send_pkt, int send_size, struct sockaddr_ll *remote, int is_broadcast);
void print_mac(unsigned char * mac_addr);

#endif	//__NET_UTIL_H__
