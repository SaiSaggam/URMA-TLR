#include "ci_cmd.h"
#include "net_util.h"
#include "tlr_file.h"

void usage(char* prog)
{
	printf("%s \"tc cmd\"\n", prog);
}

int main(int argc, char *argv[])
{
	int fd = -1, lock_fd = -1, ret;
	char * const cpe_passwd = "admin";
	struct ifreq ifr;	// netdevice
	struct sockaddr_ll local, remote;
	unsigned int ADSL_TAGE=0;
	//printf("ci_cmd, build on %s %s\n",__DATE__,__TIME__);
	int result = 1;

	if( argc !=2 )
	{
		usage(argv[0]);
		goto error;
	}

    lock_fd = tlr_file_lock("adsl_cmd.lock", true);
    if (lock_fd < 0)
    {
        printf("%s: unable to obtain lock\n", argv[0]);
        goto error;
    }

	//	prepare socket
    if( (fd = socket(PF_PACKET, SOCK_DGRAM, 0) ) < 0)
	{
		//printf("%s %d -- Sockfd open fail!!\n", __FILE__, __LINE__);
		goto error;
	}

#if defined(P3_WAN_ENABLE)
	int if_wan = 0, csman_fd, i;

	csman_fd=open_csman(NULL,0);
	if(csman_fd<0)
	{
		perror("open csman failed!!");
		return -1;
	}

	for (i=START_INDEX; i<END_INDEX; i++)
	{
		read_csman(csman_fd, CSID_C_LOCAL_MULTIWAN_ENABLE+i, &if_wan, 4, CSM_R_ZERO);
		if (if_wan)	break;
	}

	if (if_wan)
	{
		//LPRINTF(PRI_I, "======CSID_C_LOCAL_WAN_INTERFACE=%d===WAN_IF_NAME=%s======\n",if_wan,WAN_IF_NAME2); //Jase add tmp
		int enable=0;
		read_csman(csman_fd,CSID_C_VLAN_TAG_ENABLE,&enable,4,CSM_R_ZERO);
		if(enable){
			char wan_if[IFNAMSIZ];
			read_csman(csman_fd,CSID_S_VLAN_ADSL_IF,wan_if,sizeof(wan_if),CSM_R_ZSTR);

			if(strlen(wan_if)<=0)
				strcpy(wan_if,WAN_IF_NAME2);
			//sprintf(ifr.ifr_name,"%s" ,wan_if);
			strncpy(ifr.ifr_name, wan_if, IFNAMSIZ);

		}else{
			strncpy(ifr.ifr_name, WAN_IF_NAME2, IFNAMSIZ);
		}
		//LPRINTF(PRI_I, "======CSID_C_LOCAL_WAN_INTERFACE=%d===WAN_IF_NAME=%s======\n",if_wan,ifr.ifr_name); //Jase add tmp
	}
	else
	{
		//	LPRINTF(PRI_I, "======CSID_C_LOCAL_WAN_INTERFACE=%d===WAN_IF_NAME=%s======\n",if_wan,WAN_IF_NAME3); //Jase add tmp
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

	close_csman(csman_fd);
	//	LPRINTF(PRI_I, "======ADSL_TAGE %x\n",ADSL_TAGE);
#else
	{
		//LPRINTF(PRI_I, "======CSID_C_LOCAL_WAN_INTERFACE=%d===WAN_IF_NAME=%s======\n",if_wan,WAN_IF_NAME1); //Jase add tmp
		//	determine WAN interface name
		strncpy(ifr.ifr_name, WAN_IF_NAME1, IFNAMSIZ);
	}
#endif

	ret = ioctl( fd, SIOCGIFINDEX, &ifr);
	if(ret < 0)
	{
		//printf("ioctl(SIOCGIFINDEX) error\n");
		goto error;
	}else
	{
		local.sll_ifindex	= ifr.ifr_ifindex;	// interface index
	}

	//	get interface HW address (MAC address)
	ret = ioctl(fd, SIOCGIFHWADDR, &ifr);
	if(ret < 0)
	{
		//printf("ioctl(SIOCGIFHWADDR) error\n");
		goto error;

	}else
	{
		local.sll_halen		= 6;	// length of address
		memcpy(local.sll_addr, ifr.ifr_hwaddr.sa_data, local.sll_halen);
	}
	// setup local socket parameter
	local.sll_family	= AF_PACKET;		// always AF_PACKET
	local.sll_protocol	= htons(TC_PROTO|ADSL_TAGE);	// TC command
	//LPRINTF(PRI_I, "======local.sll_protocol 0x%x ,0x%x\n",local.sll_protocol,TC_PROTO|ADSL_TAGE);
	local.sll_hatype	= ARPHRD_ETHER;		// Header type
	local.sll_pkttype	= PACKET_BROADCAST;	// Packet type

	ret = bind( fd, (struct sockaddr*)&local, sizeof(struct sockaddr_ll));
	if(ret < 0)
	{
		//perror("bind error");
		goto error;
	}

	/* Broadcast until receive ack packet or broadcast 3 times */
	if( search_CPE(fd, &remote) <= 0 )
	{
		goto error;
	}

	/* After broadcast stage, use unicast */
	if( open_console_mode(fd, &remote) <= 0 )
	{
		goto error;
	}

	if( do_auth(fd, cpe_passwd, &remote) <= 0 )
	{
		goto error;
	}else
	{
		;//sleep(1);
	}
	/*	Get TC command	*/
	char cmd_line[200], *str1, *str2;
	memset(cmd_line, 0x00, sizeof(cmd_line));
	for(str1=argv[1] ; ; str1=NULL)
	{
		str2 = strtok(str1, "\"");
		if(str2==NULL)
			break;
		if(strlen(str2)>0)
			sprintf(cmd_line, "%s", str2);
	}
	//(argv[1],"\"%s\"",cmd_line);
	/*	Paser Command line */
	char tc_cmd[50];
	for(str1=cmd_line ; ; str1=NULL)
	{
		str2 = strtok(str1, ";");
		if(str2==NULL)
			break;
		if(strlen(str2)>0)
		{
			memset(tc_cmd, 0x00, sizeof(tc_cmd));
			sprintf(tc_cmd, "%s", str2);
			if( send_tc_cmd( fd, MAC_RTS_CONSOLE_CMD, tc_cmd, &remote) == 1)
			{
				//printf("MAC_RTS_CONSOLE_cmd...send query\n");
				//add by aaron  2010.12.7
				if(memcmp(str2,"sys reboot",strlen("sys reboot")))
				{
					if(get_result(fd, &remote)<=0)
						goto error;

				}
			}
		}
	}

    result = 0;

error:
    if (fd >= 0)
    {
        close(fd);
    }
    if (lock_fd >= 0)
    {
        tlr_file_unlock(lock_fd);
    }

	return result;
}
