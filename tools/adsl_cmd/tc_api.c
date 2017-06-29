#include "net_util.h"
#include "ci_cmd.h"
#include "parse_str.h"
//#include "unilog.h"

/*	function: Search CPE on wan interface
 *	input:	fd-> bind socket fd
 *	return:	0:not found, 1:found CPE device, -1:socket error
 */
int search_CPE(int fd, struct sockaddr_ll *remote)
{
	int broadcast=1, retval;
	int retry=0, cnt=0;
	int cpe_found=0;
	socklen_t remote_len;
	struct bss_data send_pkt, recv_pkt;
	//struct sockaddr_ll remote;
	fd_set rfds;
    struct timeval tv;

    //printf("Search CPE.....");
	while((cpe_found==0) && (retry<4))
	{
		/* fill cmd data of broadcast packet */
		memset(&send_pkt, 0x00, sizeof(send_pkt));
        send_pkt.cmd = MAC_RTS_START;	// CPE search start

#if defined(_AMIT_CDD531AM)
		if(retry==0)
		{
			int value;
			int csman_fd=open_csman(NULL,0);
			if(csman_fd<0)
			{
				perror("open csman failed!!");
				return -1;
			}
			if(read_csman(csman_fd, CSID_C_LOCAL_WANTYPE,&value, sizeof(value), CSM_R_ZERO)<0)
			{
				LPRINTF(PRI_I, "======CSID_C_LOCAL_WANTYPE return -1======\n");
			}
			else
			{
				if((value == LOCAL_WANTYPE_IPOA_DHCP) || (value == LOCAL_WANTYPE_IPOA_FIXED))
				{
					LPRINTF(PRI_I, "======LOCAL_WANTYPE_IPOA_DHCP/LOCAL_WANTYPE_IPOA_FIXED==value=%d====\n",value);
					if( send_data( fd, &send_pkt, sizeof(send_pkt), (struct sockaddr_ll*)remote, 2 ) < 0)
					{
						//printf("Fail -- socket fail\n");
			     		return -1;
			    	}
				}
				else
				{
					LPRINTF(PRI_I, "======not LOCAL_WANTYPE_IPOA_DHCP/LOCAL_WANTYPE_IPOA_FIXED==value=%d====\n",value);
					if( send_data( fd, &send_pkt, sizeof(send_pkt), (struct sockaddr_ll*)remote, broadcast ) < 0)
					{
						//printf("Fail -- socket fail\n");
			     		return -1;
			    	}
				}
			}
		  close_csman(csman_fd);
		}else
#endif
		{
		/* send data */
		if( send_data( fd, &send_pkt, sizeof(send_pkt), (struct sockaddr_ll*)remote, broadcast ) < 0)
		{
			//printf("Fail -- socket fail\n");
     		return -1;
    	}
	    }

		do{
			FD_ZERO(&rfds);
    		FD_SET(fd, &rfds);
			tv.tv_sec = 0;
			tv.tv_usec = 300000;
			/* receive data */
			retval =  (select( (fd+1), &rfds, NULL, NULL, &tv));
			if( retval < 0)
			{
				//printf("Fail -- select fail\n");
				return -1;
			}else if( retval == 0)
			{
				cnt++;
				continue;
			}else
    		{
        		if( FD_ISSET(fd,&rfds))
        		{
					remote_len = sizeof(struct sockaddr_ll);
					recvfrom(fd, (void *)&recv_pkt, sizeof(recv_pkt), 0, (struct sockaddr*)remote, &remote_len);
					if((recv_pkt.cmd==MAC_RTS_RESPONSE) && (recv_pkt.data[0]==MAC_RTS_START))
					{
						// if recv by from struct, you need use from struct when unicast
						cpe_found=1;
						break;
					}
					cnt++;
        		}
			}
		}while( (cnt<3) && (cpe_found==0) );
		retry++;
	};


	if(cpe_found)
	{
		//printf("Found\nReceive from MAC: ");
		//print_mac(remote->sll_addr);
	}else
	{
		//printf("Not found\n");
	}
	return cpe_found;
}

/*	function: open CPE console mode
 *	input:	fd-> bind socket fd
 *	return:	0:fail, 1:success
 */
int open_console_mode( int fd, struct sockaddr_ll *remote)
{
	int err=0;
	//printf("OPEN_CONSOLE_ON...");
	if( send_tc_cmd( fd, MAC_RTS_CONSOLE_ON, NULL, remote) == 1)
	{
		if(recv_tc_msg(  fd, MAC_RTS_CONSOLE_ON, NULL, remote) <=0)
		{	err++;	}
	}else
	{	err++;	}
	if(err)
	{
		//printf("fail\n");
		return 0;
	}
	//printf("success\n");
	return 1;
}


/*	function: do authetication with CPE
 *	input:	fd-> bind socket fd
 *	return:	0:fail, 1:success
 */
int do_auth( int fd, char *passwd, struct sockaddr_ll *remote)
{
	char tmp_str[100];
	int retry=0, need_auth=0;

	if( send_tc_cmd( fd, MAC_RTS_CONSOLE_CMD, "", remote) == 1)
	{
		//printf("MAC_RTS_CONSOLE_CMD...do CPE auth\n");
		do
		{
			recv_tc_msg(  fd, MAC_RTS_CONSOLE_ON, tmp_str, remote);
			if(strstr(tmp_str,"Password")!=NULL)
			{
				//printf("Got password prompt:\n");
				need_auth=1;
				break;
			}
			else if(strstr(tmp_str,"tc>")!=NULL)
			{
				//printf("Got tc prompt:\n");
				need_auth=0;
				break;
			}
		}while(retry++ < 5);
	}
	if(need_auth)
	{
		if( send_tc_cmd( fd, MAC_RTS_CONSOLE_CMD, passwd, remote) == 1)
		{
			//printf("MAC_RTS_CONSOLE_cmd...send password\n");
			if (get_result(fd, remote) <=0)
			{	return 0;	}
		}
	}
	return 1;
}


int send_tc_cmd(int fd , int cmd, char *data, struct sockaddr_ll *remote)
{

	struct bss_data send_pkt;

	memset(&send_pkt, 0x00, sizeof(send_pkt));
    send_pkt.cmd = cmd;	// CPE search start
	if(data!=NULL)
	{
		sprintf((char *)send_pkt.data,"%s\r\n", data);
		//printf("send tc cmd->%s", data);
	}
	/* send data */
	if( send_data( fd, &send_pkt, sizeof(send_pkt), (struct sockaddr_ll*)remote, 0/*unicast*/ ) < 0)
	{
		//printf("%s %d -- send packet fail\n",  __FILE__, __LINE__);
 		return -1;
	}

	return 1;
}



int recv_tc_msg(int fd , int cmd, char *data, struct sockaddr_ll *remote)
{
	int retval, cnt=0;
	int got_response=0;
	socklen_t remote_len;
	int recv_byte = 0;
	fd_set rfds;
    struct timeval tv;
	struct bss_data recv_pkt;


	do{
		FD_ZERO(&rfds);
    	FD_SET(fd, &rfds);
		tv.tv_sec = 0;
		tv.tv_usec = 500000;
		/* receive data */
		retval =  (select( (fd+1), &rfds, NULL, NULL, &tv));
		if( retval < 0)
		{
			//perror("select");
			//printf("select error\n");
			return -1;
		}else if( retval == 0)
		{
			//printf("%s %d -- Time Out\n", __FILE__, __LINE__);
			cnt++;
		}else
    	{
       		if( FD_ISSET(fd,&rfds))
       		{
				//printf("%s %d -- receiving data...\n", __FILE__, __LINE__);
				remote_len = sizeof(struct sockaddr_ll);
				memset(&recv_pkt, 0x00 , sizeof(recv_pkt));

				recv_byte=recvfrom(fd, (void *)&recv_pkt, sizeof(recv_pkt), 0, (struct sockaddr*)remote, &remote_len);
				if(recv_byte>0)
				{
					if((recv_pkt.cmd==MAC_RTS_RESPONSE) && (recv_pkt.data[0]==cmd))
					{
						got_response=1;
						break;
					}else if(recv_pkt.cmd==MAC_RTS_CONSOLE_DATA)
					{

						if((memcmp(recv_pkt.data, "OK",2)==0) || (memcmp(recv_pkt.data, "FAIL",4)==0) )
						{
#if 0 // removed by FS; may need some alternative when we want OAM F5 loopback support
							int cs_fd;

							cs_fd = open_csman(NULL,0);

							if(cs_fd<0)
							{
								return -1;
							}

	        				write_csman(cs_fd, CSID_S_XDSL_PVC0_F5_LOOP_STATUS, recv_pkt.data, strlen(recv_pkt.data), 0);

	        				close_csman(cs_fd);
#endif
							continue;

						}

						if( strstr((char *)recv_pkt.data, "\r\n")!=NULL )
						{
							cnt++;
							continue;
						}
						//printf("get ret_msg len=%d, recv_byte=%d\n", strlen(recv_pkt.data), recv_byte);

						memcpy(data, recv_pkt.data, strlen((char *)recv_pkt.data));
						//printf("retmsg->%s\n",recv_pkt.data);
						got_response=1;
						break;
					}else
					{
						//printf("unhandle cmd=0x%02d\n",recv_pkt.cmd);
						break;
					}
				}
       		}
		}
		//printf("cnt=%d,response=%d\n",cnt, got_response);
		cnt++;
	}while( cnt<3 );


	return recv_byte;
}

/*
 *	function: get ready command from CPE
 *	return : 0->not ready, 1->get ready
 */
#define BUF_SIZE 	2048
int get_result(int fd, struct sockaddr_ll *remote)
{
	char msg_str[BUF_SIZE];
	char tmp_str[300], sData[150], *sLoc=NULL;
	int retry=0, recv_byte;
	unsigned int offset=0, nLoc;
	memset(msg_str, 0x00, BUF_SIZE);
	do
	{
		memset(tmp_str, 0x00, sizeof(tmp_str));
		memset(sData, 0x00, sizeof(sData));

		recv_byte = recv_tc_msg( fd, MAC_RTS_CONSOLE_DATA, tmp_str, remote);
		if( recv_byte <= 0)
		{
			retry++;
		}
		else
		{
			if( ( offset + recv_byte) < BUF_SIZE)
			{
				recv_byte = strlen(tmp_str);
				strlcpy(sData, tmp_str, sizeof sData);
				if (recv_byte > 4)
				{
					strcat(sData, "\n");

					if ((sLoc = strstr(tmp_str, "%")) != NULL)    //process serial string
					{
						strcpy(tmp_str, sData);
						nLoc = sLoc - tmp_str +1;
						memset(sData, 0x00, sizeof(sData));
						strncpy(sData, tmp_str, nLoc);
						strcat(sData, "\n");
						memcpy(msg_str+offset, sData, nLoc+1);
						offset += (nLoc+1);

						if ((sLoc = strstr(tmp_str, "db")) != NULL)
						{
							nLoc = sLoc - tmp_str - (offset) +1;
							memset(sData, 0x00, sizeof(sData));
							strncpy(sData, tmp_str+(offset+1), nLoc);
							strcat(sData, "\n");
							memcpy(msg_str+offset, sData, nLoc+1);
							offset += (nLoc+1);

							if ((sLoc = strstr(tmp_str, "dbm")) != NULL)
							{
								nLoc = sLoc - tmp_str -(offset) +2;
								memset(sData, 0x00, sizeof(sData));
								strncpy(sData, tmp_str+(offset+1), nLoc);
								strcat(sData, "\n");
								memcpy(msg_str+offset, sData, nLoc+1);
								offset += (nLoc+1);

								memset(sData, 0x00, sizeof(sData));
								strncpy(sData, tmp_str+offset+1, recv_byte-offset+1);
								recv_byte -= (offset-1);
							}
						}
					}
				}
				memcpy(msg_str+offset, sData, recv_byte);
				offset += recv_byte;
			}
			else
			{
				//printf("message overflow!\n");
				return 0;
			}
		}

		if(retry>3)
		{
			return 0;
		}
	}while(strstr(tmp_str,"tc>")==NULL);

	if(offset>0)
	{
		//printf("------message----- Len:%d\n %s \n", strlen(msg_str), msg_str);
		printf("%s\n", msg_str);
		//parse_str(msg_str);
	}


	return 1;
}
