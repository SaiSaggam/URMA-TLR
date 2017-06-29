/* PPTP Client */
#include <stdio.h>
#include <string.h>
#include <netdb.h>


int get_host_ip(char *ips,char *hos)
{
    char *ptr,**pptr;
    char str[16];
    struct hostent *hptr;

    ptr = ips;
    if( ( hptr = gethostbyname(ptr) ) == NULL )
    {
        return -1;
    }
    pptr=hptr->h_addr_list;
    sprintf(hos,"%s", inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
    return 1;
}

int main(int argc, char** argv)
{
    char host[80], ip_str[16];
	int get=0, i=0, TIMEOUT=5;

    if(argc != 2)
	{
        printf("Usage:\n  getHostIP [Host Domain Name/IP]\n");
        return -1;
    }
    sprintf(host,"%s",argv[1]);
    get = get_host_ip(host,ip_str);
	if (get == 1)
	{
    	printf("%s\n",ip_str);
		return 0;
	}
}

