/*
 * pscan.c
 *
 *   TCP/UDP/NIS/RPC scanner..
 *     o scans TCP ports and prints the services running
 *     o scans UDP ports and prints the services running (remote hosts only)
 *     o dumps portmappers listing of RPC services
 *     o prints available NIS maps 
 *
 *   UDP port scanning is kinda flakey.. but it works.. with the exception 
 *   of on your own host (netstat -a for christs sake).. anyway.. here 
 *   it is..
 *
 *                  - pluvius@dhp.com
 *
 * tested on SunOS 4.1.3_U1 and Linux 1.1.85
 * Also tested on OSF/1, courtesy of IETF.
 * compile: cc -o pscan -s pscan.c
 *
 * NOTE: when you do a NIS listing.. it MUST be the domain name that
 *       you pass as the remote host.. otherwise this will not work.
 *
 * H* mods: use a normally refused TCP port to determine UDP RTT delay.
 * Much faster, and prevents the target from logging opened connections.
 * May behave oddly if some TCP ports are dropped by a filter, and doesn't
 * work on solaris cuz ICMP is *not* delivered to connected-UDP sockets.
 * See udp_scan.c in Satan for more info, and a solaris version that works
 * but needs root.
 *
 * If only one port arg given, just hit that port and report state.
 *
 * Added settable connect() timeout for tcp scan, with appropriate errors.
 * Useful for determining what TCP ports are being filtered out -- scan a
 * known machine behind a packet filter, and anything that wasn't refused
 * or open is being dropped.  Ping first to get an idea of the right
 * timeout to use; otherwise defaults to 3 sec.
 *
 * -->
 *   connect time outs is broke on linux.. ripped it out. <p.
 *   also yanked the --wait command thingy
 *
 *
 * TODO: fix domainname shit, maybe rip code from satan yp_chk
 *
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <setjmp.h>

#ifdef ENABLE_RPC
#include <rpc/rpc.h>
#include <rpc/xdr.h>
#include <rpc/pmap_prot.h>
#include <rpc/pmap_clnt.h>
#include <rpcsvc/yp_prot.h>
#include <rpcsvc/ypclnt.h>
#endif

/* xxx: these are sometimes defined by .h files as below for getopt.  I'll
   fix it later... */
extern int errno;
#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif

/* this is part of the GNU C lib */
#ifdef __GNU_LIBRARY__    
#include <getopt.h>
#else
extern int optind;
extern char *optarg;
#endif

#define PSCAN_RESULT "/var/run/pscan-result"
#define DEFAULT_LOW_PORT 1
#define DEFAULT_HIGH_PORT 10240
#define RTT_PORT 3		/* for UDP-RTT hack */

#define MAJOR_VERSION 1
#define MINOR_VERSION 4

static char sccsid[] = "@(#) pscan.c	1.4	(pluvius+hobbit) 04/15/95";

typedef enum {
   false,
   true
} my_bool;

my_bool Single = false;	/* use booleans for all if-ed vars */

typedef enum {
   s_none,
   s_tcp,
   s_udp,
   s_rpc,
   s_nis
} scan_t;

#ifdef __GNU_LIBRARY__
static struct option long_options[] = {
   {"tcp", 0, 0, 0},
   {"udp", 0, 0, 0},
   {"rpc", 0, 0, 0},
   {"nis", 0, 0, 0},
   {"help", 0, 0, 0},
   {"version", 0, 0, 0},
   {0,0,0,0}
};
#endif

struct {
   char    *alias;
   char    *mapname;
   my_bool  inuse;
} yp_maps[] = {
   {"passwd",    "passwd.byname", false},
   {"group",     "group.byname", false},
   {"networks",  "networks.byaddr", false},
   {"hosts",     "hosts.byaddr", false},
   {"protocols", "protocols.bynumber", false},
   {"services",  "services.byname", false},
   {"aliases",   "mail.aliases", false},
   {"ethers",    "ethers.byname", false},
   {NULL,        NULL, false}
};

scan_t scan_type;
char remote_host[200];
char remote_ip[20];
char remote_domain[128];
int low_port;
int high_port;
short debug = 0;

void print_version(char *s)
{
   fprintf(stderr,"%s version %d.%d\n",s,MAJOR_VERSION,MINOR_VERSION);
   exit(0);
}

void print_usage(char *s)
{
   fprintf(stderr,"usage %s: <scan type> <host> [low port] [high port]\n",s);
   fprintf(stderr,"where scan type is one of:\n");
#ifdef __GNU_LIBRARY__
   fprintf(stderr,"   --tcp, -t       - TCP port scan\n");
   fprintf(stderr,"   --udp, -u       - UDP port scan\n");
   fprintf(stderr,"   --rpc, -r       - RPC service list\n");
   fprintf(stderr,"   --nis, -n       - NIS map listing\n");
   fprintf(stderr,"   --version, -v   - Print version information\n");
   fprintf(stderr,"   --help, -h      - Print usage information\n");
#else
   fprintf(stderr,"   -t              - TCP port scan\n");
   fprintf(stderr,"   -u              - UDP port scan\n");
   fprintf(stderr,"   -r              - RPC service list\n");
   fprintf(stderr,"   -n              - NIS map listing\n");
   fprintf(stderr,"   -v              - Print version information\n");
   fprintf(stderr,"   -h              - Print usage information\n");
#endif
   fprintf(stderr,"\n");
   exit(0);
}

void get_args(int n, char *v[])
{
    int c;
    int opt_ind;

    memset (remote_domain, 0, 8);
    scan_type = s_none;

    while (true) {
#ifdef __GNU_LIBRARY__
        c = getopt_long(n,v,"turnhvd:",long_options,&opt_ind);
#else
        c = getopt(n,v,"turnhvd:");
#endif
        if (c == -1)
            break;
        switch(c) {
#ifdef __GNU_LIBRARY__
        case 0:
         opt_ind++; /* index's are one less than the scan type */
         if (opt_ind == 5)
            print_usage(v[0]);

         if (opt_ind == 6)
            print_version(v[0]);

         scan_type = opt_ind;
         break;
#endif
        case 't':
          scan_type = s_tcp;
          break;
        case 'u':
          scan_type = s_udp;
          break;
        case 'r':
          scan_type = s_rpc;
          break;
        case 'n':
          scan_type = s_nis;
          break;
        case 'd':
          strcpy (remote_domain, optarg);
          break;
        case 'v':
          print_version(v[0]);
          break;
        case 'h':
        case '?':
          print_usage(v[0]);
          break;
        }
    }

    low_port = DEFAULT_LOW_PORT;
    high_port = DEFAULT_HIGH_PORT;

    for (opt_ind = 0; optind < n; optind++) {
        switch(opt_ind++) {
        case 0: /* remote host */
            strncpy(remote_host,v[optind],199);
            break;
        case 1: /* low port */
            low_port = atoi(v[optind]);
            Single = true;
            break;
        case 2: /* high port */
            high_port = atoi(v[optind]);
            Single = false;
            break;
        }
    }

    if (Single)
        high_port = low_port;

    if ((opt_ind == 0) || (scan_type == s_none)) {
        fprintf(stderr,"error: you must specify a scan type and a host\n");
        print_usage(v[0]);
    }
}
void check_args()
{
    struct hostent *host;
    host = gethostbyname(remote_host);
    if (host == NULL) {
        unsigned long n;
        n = inet_addr(remote_host);
        if (n == INADDR_NONE) {
            fprintf(stderr,"error: host '%s' not found\n",remote_host);
            exit(1);
        }

        strcpy (remote_ip, remote_host);
        host = gethostbyaddr(remote_ip,4,AF_INET);
        if (host == NULL) {
            fprintf(stderr,"error: host '%s' not found\n",remote_host);
            exit(1);
        }
    } 
    else {  /* gethostbyname */
        sprintf(remote_ip,"%u.%u.%u.%u",
               (unsigned char) host->h_addr_list[0][0],
               (unsigned char) host->h_addr_list[0][1],
               (unsigned char) host->h_addr_list[0][2],
               (unsigned char) host->h_addr_list[0][3]);
    }
}

void print_args()
{
    static char *opt_table[] = {
       "tcp","udp","rpc","nis"
    };

    if (!Single) {
        fprintf(stdout,"scanning host %s's %s ports ",remote_host, opt_table[scan_type-1]);
        if (scan_type < 3) 
          fprintf(stdout,"%d through %d",low_port,high_port);
        
        fprintf(stdout,"\n");
    } /* single */
}

void ping_host() 
{
    int s,slen;
    struct sockaddr_in addr;
    char out_string[100];

   /* What p. didn't explain here is that this is a simple way of delaying for
   the expected RTT of the link, i.e. enough time for UDP out, ICMP back,
   causing the next UDP write to error out.  Turns out that trying to connect
   to a normally refused port is faster, although occasionally misses one. _H*/

   /* send some stuff and wait for it to come back... ping for us */
   /* non-root folks                                              */
   s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = inet_addr(remote_ip);
   addr.sin_port = htons(RTT_PORT);

   /* timeout code didn't work on linux.. scrap it. <p. */
   connect(s, (struct sockaddr*) &addr, sizeof(addr));
   close(s);
}

int scan()
{
 int soc;
 struct sockaddr_in addr;
 struct servent *serv;
 int port,rc,addr_len,opt;

   if (scan_type >= 3) /* this proc only does tcp and udp */
      return;

   errno = 0;
   for (port = low_port;port <= high_port;port++) {
        if (scan_type == s_tcp) 
            soc = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        else if (scan_type == s_udp) 
            soc = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
        else 
            return;

        if (soc < 0) {
            fprintf(stderr,"error: socket() failed\n");
            return;
        }

        /* this makes scan drop to a crawl on linux... */
        /*rc = setsockopt(soc,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));*/
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(remote_ip);
        addr.sin_port = htons(port);

        addr_len = sizeof(addr);
        rc = connect(soc, (struct sockaddr*) &addr, addr_len);
        if (scan_type == s_udp) {
            char out_text[100];
            strncpy(out_text,"k0ad kidz uv th3 WeRlD UN1t3\n",99); 
            rc = write(soc,out_text,strlen(out_text));
            ping_host(); /* wait for icmp's */
            rc = write(soc,out_text,strlen(out_text));
        }
        close(soc);

        /* for tcp, if anything other than refused, we want to know about it.  Always
        print status for a single-port run. */
        if (!Single)
            if ((rc < 0) && (scan_type == s_tcp) && (errno == ECONNREFUSED))
              continue;

        if ((rc < 0) && (scan_type == s_udp))
            continue;

        if (scan_type == s_tcp)
            serv = getservbyport(htons(port),"tcp");
        else if (scan_type == s_udp) {
            serv = getservbyport(htons(port),"udp");
            rc = 0;	/* fake out below msg */
        }
        else 
            return;

        FILE *fp = fopen(PSCAN_RESULT, "a");       
        fprintf(stdout,"port %d (%s) %s\n", 
                port, (serv == NULL)?"UNKNOWN": serv->s_name, 
                (rc == 0) ? "open" : strerror(errno)); 

        fprintf(fp,"port %d (%s) %s\n", 
                port, (serv == NULL)?"UNKNOWN": serv->s_name, 
                (rc == 0) ? "open" : strerror(errno)); 

        fclose(fp);
        fflush (stdout);
        
   }
}


#ifdef ENABLE_NIS 
int callback_proc(int is, char *ik, int ikl, char *iv, int ivl, char *id)
{
    if (is != YP_TRUE)
        return is;

    return 0;
}

void nis_dump()
{
    int i,rc;
    char *domainname;
    char *map;
    struct ypall_callback callback;

    if (remote_domain[0])
    else
        domainname = &remote_host[0];

    for (i = 0;yp_maps[i].mapname != NULL;i++) {
        callback.foreach = callback_proc;
        callback.data = NULL;
        map = yp_maps[i].mapname;
        rc = yp_all(domainname,map,&callback);
        switch(rc) {
        case 0:
            printf("%-10.10s is available\n",yp_maps[i].alias);
            break;
        case YPERR_YPBIND:
            fprintf(stderr,"error: server is not running ypbind\n");
            exit(1);
            break;
        default:
            fprintf(stderr,"error: %s in domain %s\n",
            yperr_string(rc), domainname);
            exit(1);
        }
    }
}
#endif

#ifdef ENABLE_RPC
/* this routine basically ripped from rpcinfo -p */
void rpc_scan()
{
	struct sockaddr_in server_addr;
	register struct hostent *hp;
	struct pmaplist *head = NULL;
	int socket = RPC_ANYSOCK;
	struct timeval minutetimeout;
	register CLIENT *client;
	struct rpcent *rpc;
	
	minutetimeout.tv_sec = 60;
	minutetimeout.tv_usec = 0;
    server_addr.sin_addr.s_addr = inet_addr(remote_ip);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(111);

	if ((client = clnttcp_create(&server_addr, PMAPPROG,
	    PMAPVERS, &socket, 50, 500)) == NULL) 
    {
		clnt_pcreateerror("rpcinfo: can't contact portmapper");
		exit(1);
	}

	if (clnt_call(client, PMAPPROC_DUMP, xdr_void, NULL,
	    xdr_pmaplist, &head, minutetimeout) != RPC_SUCCESS) 
    {
		fprintf(stderr, "rpcinfo: can't contact portmapper: ");
		clnt_perror(client, "rpcinfo");
		exit(1);
	}

	if (head == NULL) 
		printf("No remote programs registered.\n");
    else {
		printf("   program vers proto   port\n");
		for (; head != NULL; head = head->pml_next) {
			printf("%10ld%5ld", head->pml_map.pm_prog, head->pml_map.pm_vers);
			if (head->pml_map.pm_prot == IPPROTO_UDP)
				printf("%6s",  "udp");
			else if (head->pml_map.pm_prot == IPPROTO_TCP)
				printf("%6s", "tcp");
			else
				printf("%6ld",  head->pml_map.pm_prot);

			printf("%7ld",  head->pml_map.pm_port);

			rpc = getrpcbynumber(head->pml_map.pm_prog);
			if (rpc)
				printf("  %s\n", rpc->r_name);
			else
				printf("\n");
		}
	}
}
#endif

int main(int argc, char *argv[])
{
    get_args(argc,argv);
    check_args();
    print_args();
    remove(PSCAN_RESULT);

    /* this will only do tcp and udp, otherwise returns without doing anything */
    switch (scan_type) {
    case s_tcp:
        scan();
        break;

    case s_udp:
        scan();
        break;

#ifdef ENABLE_RPC
    case s_rpc:
        rpc_scan();
        break;
#endif

#ifdef ENABLE_NIS
    case s_nis:
        nis_dump();
        break;
#endif
    default:
        fprintf(stderr, "The scan type was not support!\n");
    }

    return 0;
}
    

