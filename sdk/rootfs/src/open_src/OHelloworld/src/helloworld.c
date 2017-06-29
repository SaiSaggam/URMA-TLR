#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h> 	
#include <arpa/inet.h> 		
#include <sys/socket.h> 	
#include "unilog.h"
#include "csman.h"
#include "csid/csid_helloworld.h"


#define MAC_ADDR_LEN    6

int main(int argc, char **argv)
{
	int linecount = 0;
	int csmanfd;

	if((csmanfd = open_csman(NULL,0)) < 0)
    {
        LPRINTF(PRI_INFO, "Can't Open CSMAN\n");
        return -1;
    }

	while(1)
	{
		char buff[256];
		int i;
		unsigned char mac[MAC_ADDR_LEN]={0, 0x50, 0x18, 0, 0x20, 0x1};
		unsigned char bin[]={5, 4, 11, 12, 10, 24, 5, 30};
		unsigned char bin_rd[1024];

		LPRINTF(PRI_INFO, "[%s][%d] ------- hello world restart -------\n", __func__, __LINE__);
		read_csman(csmanfd, CSID_C_HELLOWORLD_ROM_STRING, buff, sizeof(buff), CSM_R_ZERO);
		LPRINTF(PRI_INFO, "[%s][%d] buff[%s]\n", __func__, __LINE__, buff);

		write_csman(csmanfd, CSID_S_HELLOWORLD_RAM_MAC, mac, MAC_ADDR_LEN, 0);
		read_csman(csmanfd, CSID_S_HELLOWORLD_RAM_MAC, mac, MAC_ADDR_LEN, CSM_R_0xFF);
		LPRINTF(PRI_INFO, "[%s][%d] MAC (%02X:%02X:%02X:%02X:%02X:%02X)\n", __func__, __LINE__, 
				mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
			
		write_csman(csmanfd, CSID_S_HELLOWORLD_RAM_BIN, bin, sizeof(bin), CSM_W_NOLOG);
		read_csman(csmanfd, CSID_S_HELLOWORLD_RAM_BIN, bin_rd, 1024, CSM_R_NOLOG);
		LPRINTF(PRI_INFO, "[%s][%d] BIN (%d-%d-%d-%d-%d-%d-%d-%d-%d-%d)\n", __func__, __LINE__, 
				bin_rd[0], bin_rd[1], bin_rd[2], bin_rd[3], bin_rd[4], bin_rd[5], bin_rd[6], bin_rd[7], bin_rd[8], bin_rd[9]);
				

		for(i=0;i<8;i++)
		{
			char str[256];
			struct in_addr ip;
			int id, enable;
				
			read_csman(csmanfd, CSID_C_HELLOWORLD_ROM_RULE_NAME+i, str, sizeof(str), CSM_R_ZERO);
   			read_csman(csmanfd, CSID_C_HELLOWORLD_ROM_RULE_IP+i, &ip, sizeof(struct in_addr), CSM_R_ZERO);
   			read_csman(csmanfd, CSID_C_HELLOWORLD_ROM_RULE_ID+i, &id, sizeof(int), CSM_R_ZERO);
   			read_csman(csmanfd, CSID_C_HELLOWORLD_ROM_RULE_ENABLE+i, &enable, sizeof(int), CSM_R_ZERO);
			LPRINTF(PRI_INFO, "[%s][%d] str[%s] ip[0x%08x] id[%d] enable[%d]\n", __func__, __LINE__, str, ntohl(ip.s_addr), id, enable);
		}
		sleep(10);
	}
	close_csman(csmanfd);
}
