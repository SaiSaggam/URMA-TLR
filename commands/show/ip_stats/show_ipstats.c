#define __USE_GNU
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <json.h>
#include <stdint.h>
#include <inttypes.h>
#include "show_parse_helper.h"
#include "config_mgr.h"

static int parse_net_stat(void);
static int parse_net_snmp(void);
static int get_ip_stats(void);
static int format_json(void);
static int format_ascii(void);

int main (int argc, char *argv[]) {
	int ret= EXIT_FAILURE;
    tlr_response_format_t format;
    int port;
    int max_instance = tlr_get_max_instance(config_state_ipstats, config_mgr_table_type_show);

    if (max_instance <= 0)
        goto error;

    if (parse_show_options(argc, argv, &format, &port, FIRST_INSTANCE, max_instance) != 0)
        goto error;

	if (get_ip_stats() < 0) {
		goto error;
	}

	switch (format)
    {
        case tlr_response_format_json:
            format_json();
            break;

        case tlr_response_format_ascii:
            format_ascii();
            break;

        default:
            goto error;
    }

	ret = EXIT_SUCCESS;
error:
	return ret;
}

// TLR IP stats as defined in command definition file
struct ip_stats_t {
	uint64_t rx_bytes;
	uint64_t rx_packets;
	uint64_t rx_multicast_packets;
	uint64_t rx_multicast_bytes;
	uint64_t rx_broadcast_packets;
	uint64_t rx_forward_datagrams;
	uint64_t rx_delivers;
	uint64_t rx_reasm_requireds;
	uint64_t rx_reasm_oks;
	uint64_t rx_reasm_fails;
	uint64_t rx_discards;
	uint64_t rx_no_routes;
	uint64_t rx_address_errors;
	uint64_t rx_unknown_protos;
	uint64_t rx_truncated_packets;

	uint64_t tx_bytes;
	uint64_t tx_packets;
	uint64_t tx_multicast_packets;
	uint64_t tx_multicast_bytes;
	uint64_t tx_broadcast_packets;
	uint64_t tx_forward_datagrams;
	uint64_t tx_frag_requireds;
	uint64_t tx_frag_oks;
	uint64_t tx_frag_fails;
	uint64_t tx_frag_creates;
	uint64_t tx_discards;
	uint64_t tx_no_routes;
} ip_stats;

// values from /proc/net/netstat
static struct netstat_t {
	uint64_t InNoRoutes;
	uint64_t InTruncatedPkts;
	uint64_t InMcastPkts;
	uint64_t OutMcastPkts;
	uint64_t InBcastPkts;
	uint64_t OutBcastPkts;
	uint64_t InOctets;
	uint64_t OutOctets;
	uint64_t InMcastOctets;
	uint64_t OutMcastOctets;
	uint64_t InBcastOctets;
	uint64_t OutBcastOctets;
	uint64_t InCsumErrors;
	uint64_t InNoECTPkts;
	uint64_t InECT1Pkts;
	uint64_t InECT0Pkts;
	uint64_t InCEPkts;
} netstat;

// values from /proc/net/snmp
static struct snmp_t {
	struct ip_t {
		uint64_t Forwarding;
		uint64_t DefaultTTL;
		uint64_t InReceives;
		uint64_t InHdrErrors;
		uint64_t InAddrErrors;
		uint64_t ForwDatagrams;
		uint64_t InUnknownProtos;
		uint64_t InDiscards;
		uint64_t InDelivers;
		uint64_t OutRequests;
		uint64_t OutDiscards;
		uint64_t OutNoRoutes;
		uint64_t ReasmTimeout;
		uint64_t ReasmReqds;
		uint64_t ReasmOKs;
		uint64_t ReasmFails;
		uint64_t FragOKs;
		uint64_t FragFails;
		uint64_t FragCreates;
	} ip;
	struct udp_t {
		uint64_t InDatagrams;
		uint64_t NoPorts;
		uint64_t InErrors;
		uint64_t OutDatagrams;
		uint64_t RcvbufErrors;
		uint64_t SndbufErrors;
		uint64_t InCsumErrors;
	} udp;
} snmp;

static int skip_lines(FILE * fp, int num_skip)
{
    char *line = NULL;
    size_t len=0;
    int ret = -1;
	while (num_skip-- > 0) {
		if (getline(&line, &len, fp) <= 0) {
            goto error;
		}
	}
	ret = 0;
error:
    free(line);
	return ret;
}

static int parse_net_stat(void)
{
	FILE *fp = NULL;
	char *line = NULL; // getline() mallocs this buffer
	size_t len=0;
	ssize_t read;
	int ret=-1;
	char buf[1024];

	if((fp = fopen( "/proc/net/netstat", "r" )) == NULL)
	{
		perror("fopen");
		goto net_stat_error;
	}

	if (skip_lines(fp, 3) < 0) {
		goto net_stat_error;
	}

	if ((read = getline(&line, &len, fp)) != -1) {
		sscanf(line, " %s %" PRIu64 "%" PRIu64 "%" PRIu64 "%" PRIu64 "%" PRIu64 "%" PRIu64 "%" PRIu64 "%" PRIu64 "%" PRIu64\
				"%" PRIu64 "%" PRIu64 "%" PRIu64 "%" PRIu64 "%" PRIu64 "%" PRIu64 "%" PRIu64 "%" PRIu64,
				buf, &netstat.InNoRoutes, &netstat.InTruncatedPkts, &netstat.InMcastPkts, &netstat.OutMcastPkts,
				&netstat.InBcastPkts, &netstat.OutBcastPkts, &netstat.InOctets, &netstat.OutOctets, &netstat.InMcastOctets,
				&netstat.OutMcastOctets, &netstat.InBcastOctets, &netstat.OutBcastOctets, &netstat.InCsumErrors,
				&netstat.InNoECTPkts, &netstat.InECT1Pkts, &netstat.InECT0Pkts, &netstat.InCEPkts);
	} else {
		goto net_stat_error;
	}

	ret = 0;

net_stat_error:
	if (line) free(line);
	if (fp != NULL) {
		fclose(fp);
	}
	return ret;
}

static int parse_net_snmp(void)
{
	FILE *fp = NULL;
	char *line = NULL;
	size_t len=0;
	ssize_t read;
	int ret=-1;
	char buf[16];

	if((fp = fopen("/proc/net/snmp", "r")) == NULL)
	{
		perror("fopen");
		goto net_snmp_error;
	}

	if (skip_lines(fp, 1) < 0) {
		goto net_snmp_error;
	}

	if ((read = getline(&line, &len, fp)) != -1) {
		sscanf(line, " %s %llu %llu %llu %llu %llu %llu %llu %llu %llu\
				%llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
				buf, &snmp.ip.Forwarding, &snmp.ip.DefaultTTL, &snmp.ip.InReceives,
				&snmp.ip.InHdrErrors, &snmp.ip.InAddrErrors, &snmp.ip.ForwDatagrams,
				&snmp.ip.InUnknownProtos, &snmp.ip.InDiscards, &snmp.ip.InDelivers,
				&snmp.ip.OutRequests, &snmp.ip.OutDiscards, &snmp.ip.OutNoRoutes,
				&snmp.ip.ReasmTimeout, &snmp.ip.ReasmReqds, &snmp.ip.ReasmOKs,
				&snmp.ip.ReasmFails, &snmp.ip.FragOKs, &snmp.ip.FragFails,
				&snmp.ip.FragCreates);
	} else {
		goto net_snmp_error;
	}

	if (skip_lines(fp, 7) < 0) {
		goto net_snmp_error;
	}

	if ((read = getline(&line, &len, fp)) != -1) {
		sscanf(line, "%s %llu %llu %llu %llu %llu %llu %llu",
				buf, &snmp.udp.InDatagrams,
				&snmp.udp.NoPorts, &snmp.udp.InErrors,
				&snmp.udp.OutDatagrams, &snmp.udp.RcvbufErrors,
				&snmp.udp.SndbufErrors,	&snmp.udp.InCsumErrors);
	} else {
		goto net_snmp_error;
	}

	ret = 0;
net_snmp_error:
	if (line) free(line);
	if (fp != NULL) {
		fclose(fp);
	}
	return ret;
}

static int get_ip_stats(void) {
	int ret=-1;

	if (parse_net_stat() < 0) {
		goto get_ip_stats_error;
	}

	if (parse_net_snmp() < 0) {
		goto get_ip_stats_error;
	}

	ip_stats.rx_bytes	= netstat.InOctets;
	ip_stats.rx_packets 	= snmp.ip.InReceives;
	ip_stats.rx_multicast_packets 	= netstat.InMcastPkts;
	ip_stats.rx_multicast_bytes 	= netstat.InMcastOctets;
	ip_stats.rx_broadcast_packets	= netstat.InBcastPkts;
	ip_stats.rx_forward_datagrams	= snmp.ip.ForwDatagrams;
	ip_stats.rx_delivers	= snmp.ip.InDelivers;
	ip_stats.rx_reasm_requireds	= snmp.ip.ReasmReqds;
	ip_stats.rx_reasm_oks	= snmp.ip.ReasmOKs;
	ip_stats.rx_reasm_fails	= snmp.ip.ReasmFails;
	ip_stats.rx_discards	= snmp.ip.InDiscards;
	ip_stats.rx_no_routes	= netstat.InNoRoutes;
	ip_stats.rx_address_errors	= snmp.ip.InAddrErrors;
	ip_stats.rx_unknown_protos	= snmp.ip.InUnknownProtos;
	ip_stats.rx_truncated_packets	= netstat.InTruncatedPkts;

	ip_stats.tx_bytes	= netstat.OutOctets;
	ip_stats.tx_packets	= snmp.ip.OutRequests;
	ip_stats.tx_multicast_packets	= netstat.OutMcastPkts;
	ip_stats.tx_multicast_bytes	= netstat.OutMcastOctets;
	ip_stats.tx_broadcast_packets	= netstat.OutBcastPkts;
	ip_stats.tx_forward_datagrams	= snmp.ip.ForwDatagrams; // check this
	ip_stats.tx_frag_requireds	= snmp.ip.FragCreates;   // and this one
	ip_stats.tx_frag_oks	= snmp.ip.FragOKs;
	ip_stats.tx_frag_fails	= snmp.ip.FragFails;
	ip_stats.tx_frag_creates	= snmp.ip.FragCreates;
	ip_stats.tx_discards	= snmp.ip.OutDiscards;
	ip_stats.tx_no_routes	= snmp.ip.OutNoRoutes;

	ret = 0;
get_ip_stats_error:
	return ret;
}

static int format_json(void) {
	int ret=0;

	// Create the parent json object
	json_object *parent_obj = json_object_new_object();

	// Created the received json object
	json_object *received_obj = json_object_new_object();

	// Add Received to the parent
	json_object_object_add(parent_obj,"Received", received_obj);
	json_object_object_add(received_obj,"Bytes", json_object_new_int64(ip_stats.rx_bytes));
	json_object_object_add(received_obj,"Packets", json_object_new_int64(ip_stats.rx_packets));

	json_object *multicast_rx_obj = json_object_new_object();
	json_object_object_add(received_obj,"Multicast", multicast_rx_obj);
	json_object_object_add(multicast_rx_obj,"Bytes", json_object_new_int64(ip_stats.rx_multicast_bytes));
	json_object_object_add(multicast_rx_obj,"Packets", json_object_new_int64(ip_stats.rx_multicast_packets));

	json_object_object_add(received_obj,"Broadcast_Packets", json_object_new_int64(ip_stats.rx_broadcast_packets));
	json_object_object_add(received_obj,"Forward_Datagrams", json_object_new_int64(ip_stats.rx_forward_datagrams));
	json_object_object_add(received_obj,"Delivers", json_object_new_int64(ip_stats.rx_delivers));

	json_object *reasm_obj = json_object_new_object();
	json_object_object_add(received_obj,"Reasm", reasm_obj);
	json_object_object_add(reasm_obj,"Requireds", json_object_new_int64(ip_stats.rx_reasm_requireds));
	json_object_object_add(reasm_obj,"OKs", json_object_new_int64(ip_stats.rx_reasm_oks));
	json_object_object_add(reasm_obj,"Fails", json_object_new_int64(ip_stats.rx_reasm_fails));

	json_object_object_add(received_obj,"Discards", json_object_new_int64(ip_stats.rx_discards));
	json_object_object_add(received_obj,"No_Routes", json_object_new_int64(ip_stats.rx_no_routes));
	json_object_object_add(received_obj,"Address_Errors", json_object_new_int64(ip_stats.rx_address_errors));
	json_object_object_add(received_obj,"Unknown_Protos", json_object_new_int64(ip_stats.rx_unknown_protos));
	json_object_object_add(received_obj,"Truncated_Packets", json_object_new_int64(ip_stats.rx_truncated_packets));

	// Create sent json object
	json_object *sent_obj = json_object_new_object();

	json_object_object_add(parent_obj,"Sent", sent_obj);
	json_object_object_add(sent_obj,"Bytes", json_object_new_int64(ip_stats.tx_bytes));
	json_object_object_add(sent_obj,"Packets", json_object_new_int64(ip_stats.tx_packets));

	json_object *multicast_tx_obj = json_object_new_object();
	json_object_object_add(sent_obj,"Multicast", multicast_tx_obj);
	json_object_object_add(multicast_tx_obj,"Bytes", json_object_new_int64(ip_stats.tx_multicast_bytes));
	json_object_object_add(multicast_tx_obj,"Packets", json_object_new_int64(ip_stats.tx_multicast_packets));

	json_object_object_add(sent_obj,"Broadcast_Packets", json_object_new_int64(ip_stats.tx_broadcast_packets));
	json_object_object_add(sent_obj,"Forward_Datagrams", json_object_new_int64(ip_stats.tx_forward_datagrams));

	json_object *frag_obj = json_object_new_object();
	json_object_object_add(sent_obj,"Frag", frag_obj);
	json_object_object_add(frag_obj,"Requireds", json_object_new_int64(ip_stats.tx_frag_requireds));
	json_object_object_add(frag_obj,"OKs", json_object_new_int64(ip_stats.tx_frag_oks));
	json_object_object_add(frag_obj,"Fails", json_object_new_int64(ip_stats.tx_frag_fails));
	json_object_object_add(frag_obj,"Creates", json_object_new_int64(ip_stats.tx_frag_creates));

	json_object_object_add(sent_obj,"Discards", json_object_new_int64(ip_stats.rx_discards));
	json_object_object_add(sent_obj,"No_Routes", json_object_new_int64(ip_stats.rx_no_routes));

	printf ("%s", json_object_to_json_string(parent_obj));
	json_object_put(parent_obj);	/* frees the json object */

	return ret;
}

static int format_ascii(void)
{
	int ret=0;

	printf("IP Statistics\r\n");
	printf("-------------\r\n");
	printf(" Received                                Sent\r\n");
	printf("-------------------------------------------------------------------------------\r\n");
	printf(" Bytes             : %-15llu     Bytes             : %-15llu\r\n", ip_stats.rx_bytes, ip_stats.tx_bytes);
	printf(" Packets           : %-15llu     Packets           : %-15llu\r\n", ip_stats.rx_packets, ip_stats.tx_packets);
	printf("\r\n");
	printf(" Multicast Bytes   : %-15llu     Multicast Bytes   : %-15llu\r\n", ip_stats.rx_multicast_bytes, ip_stats.tx_multicast_bytes);
	printf(" Multicast Packets : %-15llu     Multicast Packets : %-15llu\r\n", ip_stats.rx_multicast_packets, ip_stats.tx_multicast_packets);
	printf("\r\n");
	printf(" Broadcast Packets : %-15llu     Broadcast Packets : %-15llu\r\n", ip_stats.rx_broadcast_packets, ip_stats.tx_broadcast_packets);
	printf("\r\n");
	printf(" Forward Datagrams : %-15llu     Forward Datagrams : %-15llu\r\n", ip_stats.rx_forward_datagrams, ip_stats.tx_forward_datagrams);
	printf(" Delivers          : %-15llu\r\n", ip_stats.rx_delivers);
	printf("\r\n");
	printf(" Reasm Requireds   : %-15llu     Frag Requireds    : %-15llu\r\n", ip_stats.rx_reasm_requireds, ip_stats.tx_frag_requireds);
	printf(" Reasm OKs         : %-15llu     Frag OKs          : %-15llu\r\n", ip_stats.rx_reasm_oks, ip_stats.tx_frag_oks);
	printf(" Reasm Fails       : %-15llu     Frag Fails        : %-15llu\r\n", ip_stats.rx_reasm_fails, ip_stats.tx_frag_fails);
	printf("                                         Frag Creates      : %-15llu\r\n", ip_stats.tx_frag_creates);
	printf("\r\n");
	printf(" Discards          : %-15llu     Discards          : %-15llu\r\n", ip_stats.rx_discards, ip_stats.tx_discards);
	printf(" No Routes         : %-15llu     No Routes         : %-15llu\r\n", ip_stats.rx_no_routes, ip_stats.tx_no_routes);
	printf(" Address Errors    : %-15llu\r\n", ip_stats.rx_address_errors);
	printf(" Unknown Protos    : %-15llu\r\n", ip_stats.rx_unknown_protos);
	printf(" Truncated Packets : %-15llu\r\n", ip_stats.rx_truncated_packets);

	return ret;
}




