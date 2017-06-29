#ifndef __CI_CMD_H__
#define __CI_CMD_H__
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include <net/if.h>
#include <net/if_arp.h>

#include <sys/epoll.h>
#include <pthread.h>
#include <errno.h>

#include <asm/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>

#include <sys/ioctl.h>
//#include <netpacket/packet.h>

/* defined Cmd */
#define	MAC_RTS_START			0x01
#define	MAC_RTS_RESPONSE		0x02
#define	MAC_RTS_STOP			0x03
#define	MAC_RTS_ON				0x04
#define MAC_RTS_CONSOLE_ON		0x05
#define	MAC_RTS_CONSOLE_CMD		0x06
#define	MAC_RTS_CONSOLE_DATA	0x07

/* defind initial param */
#define PORT					8003
#define BUF_LENGTH				50
#define TC_PROTO				0xaaaa
#define TC_PROTO_B				0xaaab

#define WAN_IF_NAME1				"eth5"

#define WAN_IF_NAME2				"eth2"

#define WAN_IF_NAME3				"eth3"

#define START_INDEX   5
#define END_INDEX     13

/* packet structure define
 * At APRouter Doc, packet frame include: DA|SA|Type|Cmd|Data
 *                                        6  6  2    1   >49
 * Packet size: 64 or small than 1536
 */

#if 0
struct eth_hdr {
  	unsigned char eth_da[6]; //ethernet dest mac address
  	unsigned char eth_sa[6]; //ethernet source mac address
  	unsigned short eth_type; //packet type
};
#endif
struct bss_data {
    unsigned char cmd;
    unsigned char data[299];
};

#if 1
struct eth_pkt {
    //struct eth_hdr hdr;
    struct bss_data data;
};
#endif


// function proto type
int search_CPE(int fd, struct sockaddr_ll *remote);
int open_console_mode( int fd, struct sockaddr_ll *remote);
int do_auth( int fd, char *passwd, struct sockaddr_ll *remote);
int send_tc_cmd(int fd , int cmd, char *data, struct sockaddr_ll *remote);
int recv_tc_msg(int fd , int cmd, char *data, struct sockaddr_ll *remote);
int get_result(int fd, struct sockaddr_ll *remote);

//int proc_cliconsol_data(struct bss_data *skb);
#endif	// end __CI_CMD_H__
