#include "net_util.h"

#define ADSL_DEBUG 0

void print_mac(unsigned char * mac_addr)
{
	printf("%02X:%02X:%02X:%02X:%02X:%02x\n",
		mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
}


int send_data(int fd, struct bss_data *send_pkt, int send_size, struct sockaddr_ll *remote, int is_broadcast)
{
	int broadcast = is_broadcast;
	int ret;
	struct ifreq ifr;
	unsigned int ADSL_TAGE=0;

#if defined(P3_WAN_ENABLE)
	int if_wan, csman_fd;

  	csman_fd=open_csman(NULL,0);
	if(csman_fd<0)
	{
		perror("open csman failed!!");
		return -1;
	}

	int i;
	for (i=START_INDEX; i<END_INDEX; i++)
	{
		read_csman(csman_fd, CSID_C_LOCAL_MULTIWAN_ENABLE+i, &if_wan, 4, CSM_R_ZERO);
		if (if_wan)	break;
	}

	if (if_wan)
	{
		int enable=0;
		read_csman(csman_fd,CSID_C_VLAN_TAG_ENABLE,&enable,4,CSM_R_ZERO);
		if(enable){
			char wan_if[IFNAMSIZ];

			read_csman(csman_fd,CSID_S_VLAN_ADSL_IF,wan_if,sizeof(wan_if),CSM_R_ZSTR);
			if(strlen(wan_if)<=0)
				strcpy(wan_if,WAN_IF_NAME2);
			//	sprintf(ifr.ifr_name,"%s", wan_if);
			strncpy(ifr.ifr_name, wan_if, IFNAMSIZ);
		}else{
			strncpy(ifr.ifr_name, WAN_IF_NAME2, IFNAMSIZ);

		}
	}
	else
	{
		strncpy(ifr.ifr_name, WAN_IF_NAME3, IFNAMSIZ);
	}

	ADSL_TAGE=0;
	if(read_csman(csman_fd, CSID_S_LOCAL_ADSL_VLANNUM, &ADSL_TAGE, 4, CSM_R_ZERO)<0) //Jase add tmp
	{
		close_csman(csman_fd);
		return -1; //aaron add
	}

	if(ADSL_TAGE)
		ADSL_TAGE = 1<<8;

#if ADSL_DEBUG
	printf("********ADSL_TAGE %d\n",ADSL_TAGE);
#endif
	close_csman(csman_fd);
#else
	{
		//	determine WAN interface name
		strncpy(ifr.ifr_name, WAN_IF_NAME1, IFNAMSIZ);
	}
#endif
	ret = ioctl( fd, SIOCGIFINDEX, &ifr);
	if(ret < 0)
	{
#if ADSL_DEBUG
		printf("ioctl(SIOCGIFINDEX) error\n");
#endif
		return -1;
	}else
	{
		remote->sll_ifindex	= ifr.ifr_ifindex;	// interface index
	}

	if((broadcast == 1)||broadcast == 2)
	{
		/* fill broadcast header */
		remote->sll_family	= AF_PACKET;
		if(broadcast == 1)
			remote->sll_protocol= htons(TC_PROTO|ADSL_TAGE);
		else if(broadcast == 2)
			remote->sll_protocol= htons(TC_PROTO_B|ADSL_TAGE);//Jase add, for IPOA
		remote->sll_hatype	= ARPHRD_ETHER;
		remote->sll_halen	= 6;
		remote->sll_pkttype	= PACKET_BROADCAST;
		memset(remote->sll_addr, 0xFF, remote->sll_halen);	// set broadcast address
		if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
		{
#if ADSL_DEBUG
			printf("%s %d -- setsocketopt fail\n",  __FILE__, __LINE__);
#endif
			return -1;
		}
	}

	/* send data */
	if ( sendto(fd, (void *)send_pkt, send_size, 0, (struct sockaddr*)remote, sizeof(struct sockaddr_ll) ) < 0 )
	{
#if ADSL_DEBUG
		printf("%s %d -- send packet fail\n",  __FILE__, __LINE__);
#endif
		//perror("send packet");
		return -1;
	}
	return 0;
}

