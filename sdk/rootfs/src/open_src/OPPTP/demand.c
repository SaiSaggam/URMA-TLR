#include <stdio.h>
#include <string.h>
// added for dialondemand start 
#include <sys/socket.h>			/* socket()	*/
#include <sys/ioctl.h>			/* ioctl()	*/
#include <net/if.h>				/* struct ifreq	*/

/* "struct sockaddr"    is defined in linux/socket.h	*/
/* "struct sockaddr_in" is defined in netinet/in.h	    */
/* "struct in_addr"     is defined in netinet/in.h      */
#ifdef NEW_SOCKET
#include <linux/if_packet.h>	/* struct sockaddr_ll	*/
#else
#include <net/if_packet.h> 		/* struct sockaddr_pkt	*/
#endif
#include <netinet/in.h>			/* htons()	*/

/* "struct sockaddr" is defined in linux/socket.h	*/
/* "struct sockaddr_in" is defined in netinet/in.h	*/
/* "struct in_addr" is defined in netinet/in.h		*/

// select()
#include <sys/time.h>
#include <unistd.h>             /* select()             */
#include <arpa/inet.h>          /* inet_ntoa()          */
#include <net/ethernet.h>       /* "struct ether_header" is defined in net/ethernet.h   */
#include <linux/ip.h>           /* "struct iphdr" is defined in linux/ip.h              */
#include <signal.h>

#define IFACE "br0"

static int dial_on_demand(char *interface, char* target, int is_sub)
{
	int 					o = 1;
	int 					ethfd;
	struct ifreq 			ifr;
	#ifdef NEW_SOCKET
	  struct sockaddr_ll 	sap;
	#else
	  struct sockaddr_pkt 	sap;
	#endif
	//struct timeval 		tv;
	struct in_addr 			ip, ip2, netmask, netid;
	struct in_addr 			temp_ip;
	unsigned char 			*uptr1, *uptr2;	
	fd_set 					fdread;
	struct 					ether_header *eth_hdr;
	struct iphdr 			*ip_hdr;
	char 					str[100];
	char 					des_ip_str[16], src_ip_str[16]; //EX: "255:255:255:255\0"
	
	// 1. --- Check parameters
#if 0		//programmer must be careful of this
	if(strlen(interface) == 0)
	{
		printf("dialondemand: please fill in LAN interface\r\n");
		return -1;
	}
#endif

	// 2. --- Function entrance
	printf("pptp: dialondemand: Starting to monitor LAN interface %s.\r\n", interface);

	
	// 3. --- Function initialization
	memset(&ifr, 0, sizeof(struct ifreq));
	memcpy(ifr.ifr_name, interface, strlen(interface));

	// 4. --- Create socket
	printf("pptp: dialondemand: Create socket: ");

#ifdef NEW_SOCKET
    ethfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
#else
    ethfd = socket(AF_PACKET, SOCK_PACKET, htons(ETH_P_ALL));
#endif

	if(ethfd == -1)
	{
		printf("failed to create socket.\r\n");
		exit(1);
	} else {
		printf("OK!\r\n");
	}

	// 5. --- get hardware address
	printf("pptp: dialondemand: Get hardware address: ");
	if(ioctl(ethfd, SIOCGIFHWADDR, &ifr))
	{
		printf("failed to get hardware address.\r\n");
		exit(1);
	} else  {
		printf("%02X:%02X:%02X:%02X:%02X:%02X\r\n", \
				ifr.ifr_hwaddr.sa_data[0], ifr.ifr_hwaddr.sa_data[1], \
				ifr.ifr_hwaddr.sa_data[2], ifr.ifr_hwaddr.sa_data[3], \
				ifr.ifr_hwaddr.sa_data[4], ifr.ifr_hwaddr.sa_data[5] );
	}


	// 6. --- is this interface a ethernet device or not
#if 0
	printf("dhcpcd: Check interface: ");
	if(ifr.ifr_hwaddr.sa_family != ARPHRD_ETHER)
	{
		printf("%s is not an ethernet device.\r\n", ifr.ifr_name);
		exit(1);
	} else {
		printf("%s is an ethernet device.\r\n", ifr.ifr_name);
	}
#endif

	// 7. --- get interface ip
	printf("pptp: dialondemand: Get IP address: ");
	if(ioctl(ethfd, SIOCGIFADDR, &ifr) < 0)
	{
		printf("failed to get IP address.\r\n");
		exit(1);
	}
	ip = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;
	printf("%s\r\n", inet_ntoa(ip));

	// 8. --- get interface netmask
	printf("pptp: dialondemand: Get Netmask: ");
	if(ioctl(ethfd, SIOCGIFNETMASK, &ifr) < 0)
	{
		printf("failed to get netmask.\r\n");
		exit(1);
	}
	netmask = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;
	printf("%s\r\n", inet_ntoa(netmask));

	// 9. --- get network id
	printf("pptp: dialondemand: Get netword id: ");
	uptr1 = (unsigned char *)&ip;
	uptr2 = (unsigned char *)&netmask;
	uptr1[0] &= uptr2[0]; 
	uptr1[1] &= uptr2[1];	
	uptr1[2] &= uptr2[2]; 
	uptr1[3] &= uptr2[3];
	memcpy((void *)&netid, uptr1, sizeof(ip));
	printf("%s\r\n", inet_ntoa(netid));

	// 10. --- get or send broadcast packet
	//if(setsockopt(ethfd, SOL_SOCKET, SO_BROADCAST, &o, sizeof(o)) == -1)
	//	exit(1);

	// set flags
	// IFF_UP: interface is up
	// IFF_BROADCAST: broadcast address valid
	// IFF_NOTRAILERS: avoid use of trailers
	// IFF_RUNNING: resource allocated
	//ifr.ifr_flags = IFF_UP | IFF_BROADCAST | IFF_NOTRAILERS | IFF_RUNNING;
	//if(ioctl(ethfd, SIOCSIFFLAGS, &ifr))
	//	exit(1);

	// 11. --- binding
	printf("pptp: dialondemand: Bind packet socket to %s: ", ifr.ifr_name);

#ifdef NEW_SOCKET
	memset(&sap, 0, sizeof(sap));
	sap.sll_family = AF_PACKET;
	sap.sll_protocol = htons(ETH_P_ALL);
	//sap.sll_ifindex = 0;	//atoi(interface + 3);
	if (ioctl(ethfd, SIOCGIFINDEX, &ifr) < 0) {
		printf("pptp: dialondemand: ioctl(SIOCFIGINDEX): Could not get interface index");
		exit(1);
   	}
    sap.sll_ifindex = ifr.ifr_ifindex;
	if(bind(ethfd, (struct sockaddr *)&sap, sizeof(sap)) == -1)
#else
	memset(&sap, 0, sizeof(sap));
	sap.spkt_family = AF_PACKET;
	memcpy(sap.spkt_device, interface, strlen(interface));
	if(bind(ethfd, (void *)&sap, sizeof(struct sockaddr)) == -1)
#endif
	{
		printf("failed to bind.\r\n");
		exit(1);
	} 
	else 
	{
#ifdef NEW_SOCKET
		printf("OK! and ifindex=%d\r\n",sap.sll_ifindex);
#else
		printf("OK!\r\n");
#endif
	}

	//12. --- Wait for traffic
	printf("pptp: dialondemand: Start waiting input packet: \r\n");
		
	//tv.tv_sec = 5; tv.tv_usec = 0;
	for(;;)
	{
		FD_ZERO(&fdread);
		FD_SET(ethfd,&fdread);
		select(ethfd + 1, &fdread, NULL, NULL, NULL);
		
		if(FD_ISSET(ethfd, &fdread))
		{
			o = read(ethfd, (void *)str, sizeof(str)-1);
			eth_hdr = (struct ether_header *)str;
		//	printf("pptp: dialondemand: received a packet, type = %04X\r\n", ntohs(eth_hdr->ether_type));
			if(ntohs(eth_hdr->ether_type) == ETHERTYPE_IP)
			{
		//		printf("                     its ether type = ETHERTYPE_IP\r\n");
		//		usleep(300);
				ip_hdr = (struct iphdr *)((char *)eth_hdr + sizeof(struct ether_header));
				ip.s_addr = ip_hdr->daddr; 
				ip2.s_addr = ip_hdr->saddr;
				temp_ip = ip; 
				strcpy(des_ip_str,inet_ntoa(temp_ip));
				temp_ip = ip2; 
				strcpy(src_ip_str,inet_ntoa(temp_ip));
		//		printf("                     source ip=%s, destination ip=%s\r\n",src_ip_str,des_ip_str);
				uptr1 = (unsigned char *)&ip; 
				uptr2 = (unsigned char *)&netmask;
				uptr1[0] &= uptr2[0]; 
				uptr1[1] &= uptr2[1]; 
				uptr1[2] &= uptr2[2]; 
				uptr1[3] &= uptr2[3];
				if ((ip.s_addr != netid.s_addr) 					&&
					(ip.s_addr != inet_addr("224.0.0.0"))		 	&&
					(ip_hdr->daddr != inet_addr("255.255.255.255")) &&
					(ip_hdr->daddr != inet_addr("239.255.255.250"))	   )
				{
                    char cmd[512];
					printf("received a packet from LAN %s (%s),and send to WAN (%s)\r\n" \
							, ifr.ifr_name, src_ip_str, des_ip_str);
					//unsigned char protocol_str[5];
					//if (ip_hdr->protocol == 6) {
					//	Strcpy(protocol_str,"TCP");
					//}
                    if(!strcmp(target, "")){
                        sprintf(cmd, "logger -p 8.7 -t \"pptpc\" \"received a packet from LAN %s (%s),and send to WAN (%s)\"" \
                                , ifr.ifr_name, src_ip_str, des_ip_str);
                        system(cmd);

                        printf("WAN matched \r\n");
    					close(ethfd);
	    				return 1;  
                    }
                                
                    if(!is_sub){
                        if(!strcmp(des_ip_str, target)){
                            sprintf(cmd, "logger -p 8.7 -t \"pptpc\" \"received a packet from LAN %s (%s),and send to WAN (%s)\"" \
                                    , ifr.ifr_name, src_ip_str, des_ip_str);
                            system(cmd);

                            printf("matched target IP %s \r\n", target);
                            close(ethfd);
                            return 1;                     
                        }
                    }
                    else {
                        int ip_[4];
                        int mask;
                        sscanf(des_ip_str, "%d.%d.%d.%d", &ip_[0], &ip_[1], &ip_[2], &ip_[4]);
                        int dest_ip_int = (ip_[0]<<24) + (ip_[1]<<16) + (ip_[2]<<8) + ip_[3];

                        sscanf(target, "%d.%d.%d.%d/%d", &ip_[0], &ip_[1], &ip_[2], &ip_[3], &mask);
                        int subnet = (ip_[0]<<24) + (ip_[1]<<16) + (ip_[2]<<8) + ip_[3];
                        if(subnet == 0) // for 0.0.0.0/0
                           return 1; 
                         
                        if((dest_ip_int>>(32-mask)) == (subnet>>(32-mask))){
                            sprintf(cmd, "logger -p 8.7 -t \"pptpc\" \"received a packet from LAN %s (%s),and send to WAN (%s)\"" \
                                    , ifr.ifr_name, src_ip_str, des_ip_str);
                            system(cmd);

                            printf("matched target subnet, ip %s(%x) in %s(%x) \r\n", 
                                    des_ip_str, (dest_ip_int>>(32-mask)), target, (subnet>>(32-mask)));
                            close(ethfd);
                            return 1;                     
                        }
                    }
				}
			}
		}
	}	// end for loop
	close(ethfd);
	return 0;
}

static void usage()
{
    printf("Usage:\n"
           "pptp-ondemand -wan\n"
           "pptp-ondemand -ip x.x.x.x -tun x\n"
           "pptp-ondemand -sub x.x.x.x/x -tun x\n"
           );
}

static void init(char *tun)
{
	int pid = 0;
	FILE *fd = NULL;

    char PID_FILE[64] = {0};
    if(tun != NULL)
        snprintf(PID_FILE, 64, "/var/run/pptpc-ondemand-%s.pid", tun);
    else
        snprintf(PID_FILE, 64, "/var/run/pptpc-ondemand-wan.pid", tun);
	
    //kill the previous process
	if ((fd = fopen(PID_FILE, "r")) != NULL) {
		fscanf(fd,"%d",&pid);
		fclose(fd);
        remove(PID_FILE);
		kill(pid,SIGTERM);
		sleep(1);
	}   

	if ((fd = fopen(PID_FILE, "w")) != NULL) {
		fprintf(fd,"%d",getpid());
		fclose(fd);
	}    
}


int main(int argc, char** argv) 
{
    if(!(argc == 2 || argc == 5)){
        usage();
        return 0;
    }

    if(argc == 2){
        if(!strcmp(argv[1], "-wan")){
             init(NULL);
             if(dial_on_demand(IFACE, "", 0) == 1)
                system("pptp-action start &");	
        }
        else
            printf("invalid parameter: %s\n", argv[1]);

        return 0;
    }

    if(strcmp(argv[1], "-ip") && strcmp(argv[1], "-sub")){
        printf("invalid parameter: %s\n", argv[1]);
        return 0;
    }
    
    if(strcmp(argv[3], "-tun")){
        printf("invalid parameter: %s\n", argv[3]);
        return 0;
    }
    init(argv[4]);

    char cmd[16];
    if(!strcmp(argv[1], "-ip")){
        if(dial_on_demand(IFACE, argv[2], 0) == 1){
            snprintf(cmd, 16, "pptpc-start %s", argv[4]);
            system(cmd);         
            return 0;
        }
    }
    else if(!strcmp(argv[1], "-sub")){
        if(dial_on_demand(IFACE, argv[2], 1) == 1){
            snprintf(cmd, 16, "pptpc-start %s", argv[4]);
            system(cmd);         
            return 0;
        }
    }
    return 0;
}



