/*****************************************************************************
* Copyright (c) 2016 Digi International Inc., All Rights Reserved
*
* This software contains proprietary and confidential information of Digi
* International Inc.  By accepting transfer of this copy, Recipient agrees
* to retain this software in confidence, to prevent disclosure to others,
* and to make no use of this software other than that for which it was
* delivered.  This is an unpublished copyrighted work of Digi International
* Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
* prohibited.
*
* Restricted Rights Legend
*
* Use, duplication, or disclosure by the Government is subject to
* restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
* Technical Data and Computer Software clause at DFARS 252.227-7031 or
* subparagraphs (c)(1) and (2) of the Commercial Computer Software -
* Restricted Rights at 48 CFR 52.227-19, as applicable.
*
* Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
*
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <stdint.h>
#include <inttypes.h>
#include <json.h>
#include <linux/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/sockios.h>
#include <termios.h>
#include <sys/sysinfo.h>
#include <ctype.h>

#include "ra_ioctl.h"
#include "ethphy.h"
#include "ethregs.h"
#include "config_mgr.h"
#include "config_table.h"
#include "config_def.h"
#include "ifuptime_lib.h"
#include "show_eth.h"
#include "tlr_print.h"
#include "ip_helper.h"
#include "show_parse_helper.h"
#include "unittest_support.h"
#include "messages.h"
#include "tlr_common.h"
#include "tlr_assert.h"
#include "show_status_helper.h"

#define     ACCUMULATED_STATS_FILE      "/tmp/acc_esw_cnt"
#define     PROC_STATS_FILE             "/proc/mt7621/esw_cnt"

STATIC ethstat_t ethstat[NB_SCAN_PORTS];
STATIC ethstat_t acc_ethstat[NB_SCAN_PORTS]; // Will contain accumulated totals.

typedef struct
{
    char const * const stat_name;
    size_t member_off;
} stat_desc_t;

STATIC stat_desc_t const stat_desc[] =
{
    {"Rx Unicast Packet   ", offsetof(ethstat_t, rx_unicast_pak)},
    {"Tx Unicast Packet   ", offsetof(ethstat_t, tx_unicast_pak)},
    {"Rx Broadcast Packet ", offsetof(ethstat_t, rx_bcast_pak)},
    {"Tx Broadcast Packet ", offsetof(ethstat_t, tx_bcast_pak)},
    {"Rx Multicast Packet ", offsetof(ethstat_t, rx_multicast_pak)},
    {"Tx Multicast Packet ", offsetof(ethstat_t, tx_multicast_pak)},
    {"Rx CRC Error        ", offsetof(ethstat_t, rx_crc_err)},
    {"Tx CRC Error        ", offsetof(ethstat_t, tx_crc_error)},
    {"Rx Drop Packet      ", offsetof(ethstat_t, rx_drop_pak)},
    {"Tx Drop Packet      ", offsetof(ethstat_t, tx_drop_pak)},
    {"Rx Pause Packet     ", offsetof(ethstat_t, rx_pause_pak)},
    {"Tx Pause Packet     ", offsetof(ethstat_t, tx_pause_pak)},
    {"Rx Filtering Packet ", offsetof(ethstat_t, rx_filt_pak)},
    {"Tx Collision Event  ", offsetof(ethstat_t, tx_coll_pak)},
    {"Rx Alignment Error  ", offsetof(ethstat_t, rx_align_err)},
    {"Rx Undersize Error  ", offsetof(ethstat_t, rx_und_err)},
    {"Rx Fragment Error   ", offsetof(ethstat_t, rx_frag_err)},
    {"Rx Oversize Error   ", offsetof(ethstat_t, rx_oversize_err)},
    {"Rx Jabber Error     ", offsetof(ethstat_t, rx_jabber_err)},
};

char const   *duplexs[] = { "Half", "Full" };
char const   *link_state[] = { "Down", "Up" };
typedef enum  { HALF, FULL} DUPLEX_T;
typedef enum  { DOWN, UP} LINK_STATE_T;


typedef struct
{
    char const   *link_up;
    int          speed;
    char const   *duplex;
} phy_state_t;

STATIC phy_state_t phy_states[NB_SCAN_PORTS];

typedef struct
{
    size_t instance;
    char const * description;
    config_state_eth_admin_status_t admin_status;
    config_state_eth_oper_status_t oper_status;
    uint32_t uptime;
    char mac_address[MACADDR_STRING_LENGTH];
    char const * link_status;
    int link_speed;
    char const * link_duplex;
    ethstat_t const * stats;
} eth_status_t;


static int max_instance;

static uint64_t * get_element_value(void const * const pstat, size_t const offset)
{
    uint8_t const * const ptr = pstat;
    void * const pmember = (void * const)(ptr + offset);  /* pointer to element value which is writable */

    return pmember;
}

/**
 * Given the ETHSTAT structure and the name of the element return pointer to actual element value.
 *
 */
static uint64_t * get_element_ptr(ethstat_t const * const pstat, char const * const stat_name)
{
    int idx;
    uint64_t * result = NULL;

    for (idx=0; idx < ARRAY_SIZE(stat_desc); idx++)
    {
        stat_desc_t const * const pdesc = &stat_desc[idx];

        if (strstr(pdesc->stat_name, stat_name) != NULL)
        {
            result = get_element_value(pstat, pdesc->member_off);
            break;
        }
    }

    return result;
}

/* @TODO: please put them in a structure and use it, because they are all related */
static int walk_idx;
static int walk_port;
static stat_desc_t const * pdesc_walk;
static int walk_port_idx;
static ethstat_t * pstat_walk;

/**
 * Given the port; get ready for a walk through all statistical elements for that port.
 *
 */
STATIC_FUNCTION void element_walk_init(int const port)
{
    walk_idx = 0;
    walk_port = port;
}


STATIC_FUNCTION int walk_next_element(char const ** const elename, uint64_t * const elevalue)
{
    int result;
    stat_desc_t const * pdesc = &stat_desc[0];
    ethstat_t const * const pstat = &ethstat[walk_port];

    if (walk_idx < ARRAY_SIZE(stat_desc))
    {
        pdesc += walk_idx++;
        *elename = pdesc->stat_name;

        {
            uint64_t const * const value_ptr = get_element_value(pstat, pdesc->member_off);

            *elevalue = *value_ptr;
        }

        result = 0;
    }
    else
    {
        *elename = NULL;
        *elevalue = 0;
        result = -1;
    }

    return result;
}


STATIC_FUNCTION void port_walk_init(stat_desc_t const * const pdesc)
{
    walk_port_idx = 0;
    pdesc_walk = pdesc;
    pstat_walk = &ethstat[0];
}

STATIC_FUNCTION int walk_next_port ( uint64_t *elevalue )
{
// Get the same element but for the next port.
    if ( walk_port_idx >= NB_SCAN_PORTS )
        {
        *elevalue = 0;
        return -1;
        }
    *elevalue = * (uint64_t *)((unsigned char *)pstat_walk+pdesc_walk->member_off);
    pstat_walk++;
    walk_port_idx++;

    return 0;
}


STATIC_FUNCTION void add_in_accumulated(void)
{
    // Performs ethstat += acc_ethstat for each individual statistic
    int port;
    ethstat_t const * pto = &ethstat[0];
    ethstat_t const * pfrom = &acc_ethstat[0];

    for (port = 0; port < NB_SCAN_PORTS; port++)
    {
        int idx;

        for (idx = 0; idx < ARRAY_SIZE(stat_desc); idx++)
        {
             stat_desc_t const * const pdesc = &stat_desc[idx];
             uint64_t * const value_to = (uint64_t *)((unsigned char *)pto+pdesc->member_off);
             uint64_t const * const value_from = (uint64_t *)((unsigned char *)pfrom+pdesc->member_off);

            *value_to += *value_from;
        }

        pto++;
        pfrom++;
    }
}

static int skip_lines_until(FILE * fp, char *match)
{
    int result;
    char * line = NULL;
    size_t len = 0;

    while (getline(&line, &len, fp) > 0)
    {
        if (strstr(line, match) != NULL)
        {
            result = 0;
            goto done;
        }
    }

    result = -1;

done:
    free(line);
    return result;
}

STATIC_FUNCTION int parse_eth_stat(ethstat_t const * const pstats, char const * const file)
{
	FILE *fp = NULL;
	char *line = NULL;
	size_t len=0;
	int ret=-1;
    int scanned, port;
	char    buf1[40], buf2[40], buf3[40];
    char    id[120];
    uint64_t  cnt_port[6];

	if((fp = fopen(file, "r")) == NULL)
	{
//		printf ("fopen %s fail\n", file);
		goto esw_cnt_error;
	}

	if (skip_lines_until (fp, "===================") < 0)
		goto esw_cnt_error;

	while ( getline(&line, &len, fp) != -1)
        {
        ethstat_t const * pwrite_stats = pstats;
		scanned = sscanf(line, "%39s %39s %39s : %" PRIu64 "%" PRIu64 "%" PRIu64 "%" PRIu64 "%" PRIu64 "%" PRIu64, buf1, buf2, buf3,
            &cnt_port[0], &cnt_port[1], &cnt_port[2], &cnt_port[3], &cnt_port[4], &cnt_port[5] );
        if ( scanned == 9)
            {
            snprintf (id, sizeof id, "%s %s %s", buf1, buf2, buf3 );
            id[sizeof id -1] = 0;   // ensure we are NULL terminated.
            // Right we'll save all the ports and just display the ones we want.
            // This is because the mere reading of the file clears all the counts
            // and we need to maintain a running accumulated total file.
            for ( port = 0; port < NB_SCAN_PORTS; port++ )
                {
                uint64_t  * const element = get_element_ptr(pwrite_stats, id);

                if ( element != NULL )
                    *element = cnt_port[port];

                pwrite_stats++;
                }
            }
	    }

	ret = 0;
esw_cnt_error:
    if ( line!=NULL)
	    free(line);
	if (fp != NULL) {
		fclose(fp);
	}
	return ret;
}


/**
 * Write a tmp file with the given pstas..
 * Uses same format as proc file so we can utilise same parsing format.
 */
STATIC_FUNCTION void write_tmp_ethstat(ethstat_t const * const pstats)
{
    FILE *fp = NULL;
    int     idx;
    uint64_t  value;

    if ((fp = fopen(ACCUMULATED_STATS_FILE, "w")) == NULL)
        {
        goto error;
        }

    fputs ( "Accumulated Stats from Switch in MT7621..\n", fp );
	fputs ( "=====================    Port0    Port1    Port2    Port3    Port4    Port5    6\n", fp );

    for (idx=0; idx<ARRAY_SIZE(stat_desc); idx++)
        {
        stat_desc_t const * const pdesc = &stat_desc[idx];

        fprintf(fp, "%s : ", pdesc->stat_name );
        port_walk_init(pdesc);

        while ( walk_next_port ( &value ) == 0 )
            {
            fprintf ( fp, " %" PRIu64, value );
            }
        fputs ( "\n", fp );
        }

error:
    if (fp != NULL)
        {
        fclose(fp);
        }
}

/**
 * Gets the current ethernet speed and duplex of the given port.
 *
 */
STATIC_FUNCTION int get_eth_speed_duplex(int port, int *speed, DUPLEX_T *duplex)
{
    int     phy_stat, part_1000, adv_1000;
    int     adv,partner,phy_con;

    if (reg_read(port, PHY_STAT, &phy_stat) < 0 ||
        reg_read(port, PHY_AUTOADVERT, &adv) < 0 ||
        reg_read(port, PHY_AUTOPART, &partner) < 0 ||
        reg_read(port, PHY_CON, &phy_con) < 0 ||
        reg_read(port, PHY_1000BASET_CTRL, &adv_1000) < 0 ||
        reg_read(port, PHY_1000BASET_STAT, &part_1000) < 0 )
        return -1;

    if ( (phy_stat & PHY_STAT_LINKUP) == 0 )
        {
        // not connected
        *speed = 0;
        *duplex = HALF;
        return 0;
        }

    if ( (phy_con & PHY_CON_AUTONEG ) == 0 )
    	{
    	// Unlikely but just in case...
    	if ( phy_con & PHY_CON_100MB )
            *speed = 100;
        if ( phy_con & PHY_CON_FULLDUPLEX )
            *duplex = FULL;
        return 0;
    	}

    adv &= partner;
    adv_1000 <<= 2;
    adv_1000 &= part_1000;

    if ( adv_1000 & (PHY_STAT_1000HALF|PHY_STAT_1000FULL) )
        {
        *speed = 1000;
        if ( adv_1000 & PHY_STAT_1000FULL )
          *duplex = FULL;
        else
           *duplex = HALF;
        return 0;
        }
    else if ( adv & (PHY_AUTOADV_100HALFDUP|PHY_AUTOADV_100FULLDUP) )
        *speed = 100;
    else
    	*speed = 10;
    if ( adv & (PHY_AUTOADV_10FULLDUP|PHY_AUTOADV_100FULLDUP) )
       *duplex = FULL;
    else
       *duplex = HALF;
    return 0;

}


STATIC_FUNCTION void get_physical_states(void)
{
    int     port;
    int     speed;
    DUPLEX_T    duplex;
    LINK_STATE_T    state;
    phy_state_t * ps = &phy_states[0];

    for (port=0; port < NB_SCAN_PORTS; port++, ps++ )
        {
        state = DOWN;
        duplex = HALF;
        speed = 0;
        memset ( ps, 0, sizeof *ps );
        if ( is_linkup(port) )
            state = UP;
        ps->link_up = link_state[state];
        ps->duplex = duplexs[duplex];
        if (get_eth_speed_duplex(port, &speed, &duplex) >= 0 )
            {
            ps->duplex = duplexs[duplex];
            ps->speed = speed;
            }
        }
}

static char * get_description(size_t const instance)
{
    char * description = NULL;
    config_status_t const status = tlr_get_element_alloc_value(config_eth, instance, config_eth_description, (void **)&description, config_mgr_element_type_string, config_mgr_access_level_read_only);

    if (status != config_status_success)
    {
        tlr_debug("unable to get description configuration (%d)\n", status);
    }

    return description;
}

static config_state_eth_admin_status_t get_admin_status(int const instance)
{
    config_state_eth_admin_status_t admin_status = config_state_eth_admin_status_down;
    config_eth_state_t * state = NULL;
    config_status_t const status = tlr_get_element_alloc_value(config_eth, instance, config_eth_state, (void **)&state, config_mgr_element_type_enum, config_mgr_access_level_read_only);

    if (status != config_status_success)
    {
        tlr_debug("unable to get state configuration (%d)\n", status);
        goto done;
    }

    if (*state == config_eth_state_off)
    {
        goto done;
    }

    admin_status = config_state_eth_admin_status_up;

done:
    free(state);

    return admin_status;
}

static config_state_eth_oper_status_t get_oper_status(int const instance)
{
    config_state_eth_oper_status_t oper_status = config_state_eth_oper_status_down;
	char * response = NULL;

	if (send_message(ethernet_monitor, instance, query_connect, NULL, &response) != 0)
	{
		tlr_printf(tlr_response_format_ascii, "show_eth: send_message() to eth monitor failed\n");
		tlr_event_log(tlr_log_error, "show_eth", "show_eth: send_message() to eth monitor failed");
        goto done;
	}

	if (response != NULL)
	{
		const char s[2] = ".";
		char * fword;
        char * save;

		/* get the first word (before the ".OK i.e. result should be either "connect.OK" or "disconnect.OK") */
		fword = strtok_r(response, s, &save);
		if (strcmp("connected", fword) == 0)
		{
			oper_status = config_state_eth_oper_status_up;
		}
	}

done:
    free(response);

	return oper_status;
}

static uint32_t get_uptime(char const * const interface_name)
{
    uint64_t uptime_in_seconds;
    uint32_t uptime_as_uint32 = 0;

    if (get_ifuptime(interface_name, &uptime_in_seconds) != ifuptime_ok)
    {
        goto done;
    }

    uptime_as_uint32 = uptime_in_seconds;

done:
    return uptime_as_uint32;
}

static void get_eth_status(int const instance, eth_status_t * const eth_status)
{
    phy_state_t const * const phy = &phy_states[instance];
    char const * const name = interface_get_name_from_type(eth_interface);
    char interface_name[strlen(name) + INTEGER_STRING_LENGTH];

    snprintf(interface_name, sizeof interface_name, "%s%u", name, instance);

    eth_status->instance = instance;

    eth_status->description = get_description(instance);
    eth_status->admin_status = get_admin_status(instance);
    eth_status->oper_status = get_oper_status(instance);
    eth_status->uptime = get_uptime(interface_name);
    get_hwaddr(interface_name, eth_status->mac_address, sizeof eth_status->mac_address);
    eth_status->link_status = phy->link_up;
    eth_status->link_speed = phy->speed;
    eth_status->link_duplex = phy->duplex;
    eth_status->stats = &ethstat[instance];
}

static void free_eth_status(eth_status_t const * const eth_status)
{
    free((void *)eth_status->description);
}

STATIC_FUNCTION bool format_ascii(int const instance)
{
    int idx;

    for (idx = FIRST_INSTANCE; idx <= max_instance; idx++)
    {
        if (instance == -1 || instance == idx)
        {
            eth_status_t eth_status_object = { 0 };
            eth_status_t * const eth_status = &eth_status_object;
            char uptime_str[60];
            char const *name;
            uint64_t value;

            get_eth_status(idx, eth_status);

            printf_status_header(DIVIDER_WIDTH_AUTO, " Eth Status and Statistics Port %d\n", idx);

            printf(" Description     : %s\n", eth_status->description );
            printf(" Admin Status    : %s\n", eth_status->admin_status == config_state_eth_admin_status_up ? link_state[UP] : link_state[DOWN]);
            printf(" Oper Status     : %s\n", eth_status->oper_status == config_state_eth_oper_status_up ? link_state[UP] : link_state[DOWN]);

            format_ifuptime(eth_status->uptime, uptime_str, sizeof uptime_str);
            printf(" Up Time         : %s\n", uptime_str);

            printf("\n" );
            printf(" MAC Address     : %s\n", eth_status->mac_address);

            printf(" Link            : ");
            if (strcmp(eth_status->link_status, "Down") != 0)
            {
                printf("%dBase-T %s-Duplex\n", eth_status->link_speed, eth_status->link_duplex);
            }
            else
            {
                printf("No connection\n");
            }

            printf("\n" );
            printf(" Received                              Sent\n");
            printf(" --------                              ----\n");

            element_walk_init(idx);
            while (walk_next_element(&name, &value) == 0)
            {
                static char const txmsk[] = "Tx";
                char const *name2;
                uint64_t value2;

                walk_next_element(&name2, &value2);      // We are going to print two at a time

                if (name2 != NULL && strncmp(name2, txmsk, sizeof txmsk - 1) == 0)
                {
                    printf(" %s : %-15" PRIu64  "%s : %-15" PRIu64 "\n", name, value, name2, value2);
                }
                else
                {
                    printf(" %s : %-15" PRIu64 "\n", name, value);
                    if (name2 != NULL)
                    {
                        printf(" %s : %-15" PRIu64 "\n", name2, value2);
                    }
                }
            }

            printf("\n" );

            free_eth_status(eth_status);
        }
    }

    return true;
}

static void add_element_to_show_object(tlr_show_object_t * const show_object, eth_status_t const * const eth_status, config_state_eth_t const element_id)
{
    void const * value = NULL;
    ethstat_t const * const stats = eth_status->stats;

    switch (element_id)
    {
        case config_state_eth_description:
            value = eth_status->description;
            break;
        case config_state_eth_admin_status:
            value = &eth_status->admin_status;
            break;
        case config_state_eth_oper_status:
            value = &eth_status->oper_status;
            break;
        case config_state_eth_uptime:
            value = &eth_status->uptime;
            break;
        case config_state_eth_mac_address:
            value = eth_status->mac_address;
            break;
        case config_state_eth_link_status:
            value = eth_status->link_status;
            break;
        case config_state_eth_link_speed:
            value = &eth_status->link_speed;
            break;
        case config_state_eth_link_duplex:
            value = eth_status->link_duplex;
            break;
        case config_state_eth_rx_unicast_packets:
            value = &stats->rx_unicast_pak;
            break;
        case config_state_eth_tx_unicast_packets:
            value = &stats->tx_unicast_pak;
            break;
        case config_state_eth_rx_broadcast_packets:
            value = &stats->rx_bcast_pak;
            break;
        case config_state_eth_tx_broadcast_packets:
            value = &stats->tx_bcast_pak;
            break;
        case config_state_eth_rx_multicast_packets:
            value = &stats->rx_multicast_pak;
            break;
        case config_state_eth_tx_multicast_packets:
            value = &stats->tx_multicast_pak;
            break;
        case config_state_eth_rx_crc_errors:
            value = &stats->rx_crc_err;
            break;
        case config_state_eth_tx_crc_errors:
            value = &stats->tx_crc_error;
            break;
        case config_state_eth_rx_drop_packets:
            value = &stats->rx_drop_pak;
            break;
        case config_state_eth_tx_drop_packets:
            value = &stats->tx_drop_pak;
            break;
        case config_state_eth_rx_pause_packets:
            value = &stats->rx_pause_pak;
            break;
        case config_state_eth_tx_pause_packets:
            value = &stats->tx_pause_pak;
            break;
        case config_state_eth_rx_filtering_packets:
            value = &stats->rx_filt_pak;
            break;
        case config_state_eth_tx_collisionss:
            value = &stats->tx_coll_pak;
            break;
        case config_state_eth_rx_alignment_error:
            value = &stats->rx_align_err;
            break;
        case config_state_eth_rx_undersize_error:
            value = &stats->rx_und_err;
            break;
        case config_state_eth_rx_fragment_error:
            value = &stats->rx_frag_err;
            break;
        case config_state_eth_rx_oversize_error:
            value = &stats->rx_oversize_err;
            break;
        case config_state_eth_rx_jabber_error:
            value = &stats->rx_jabber_err;
            break;
    }

    tlr_show_object_add_element(show_object, element_id, value);
}

static void add_elements_to_show_object(tlr_show_object_t * const show_object, eth_status_t const * const eth_status)
{
    config_mgr_group_t const * const group = tlr_get_group_ptr(config_state_eth, eth_status->instance, config_mgr_get_group);
    int element_idx;

    tlr_assert(group != NULL);

    for (element_idx = 0; element_idx < group->elements.count; element_idx++)
    {
        config_mgr_element_t * const element = &group->elements.element[element_idx];

        add_element_to_show_object(show_object, eth_status, element->id);
    }
}

STATIC_FUNCTION bool format_json(int const instance)
{
    int result = false;
    tlr_show_object_t * const show_object = tlr_show_object_create(config_state_eth, instance);
    eth_status_t eth_status_object = { 0 };
    eth_status_t * const eth_status = &eth_status_object;

    if (show_object == NULL)
    {
        tlr_debug("failed to create show object\n");
        goto done;
    }

    get_eth_status(instance, eth_status);
    add_elements_to_show_object(show_object, eth_status);
    free_eth_status(eth_status);

    printf("%s\n", tlr_show_object_to_json_string(show_object));

    result = true;

done:
    tlr_show_object_destroy(show_object);

    return result;
}

#ifndef _CUNIT_
int main (int argc, char * argv[])
{
    int result = EXIT_FAILURE;
  	char const * const progname = basename(argv[0]);
    tlr_response_format_t format;
    int instance;

    openlog(progname, 0, LOG_USER);

    max_instance = tlr_get_max_instance(config_state_eth, config_mgr_table_type_show);

    if (max_instance <= 0)
    {
        tlr_error("%s is not supported\n", progname);
        goto done;
    }

    if (parse_show_options(argc, argv, &format, &instance, 0, max_instance) != 0)
    {
		usage_show(progname);
		goto done;
    }

    if (instance <= 0)
    {
        instance = -1;
    }

    if (parse_eth_stat(&ethstat[0], PROC_STATS_FILE) != 0)
    {
        tlr_error("failed to parse eth stats\n");
        goto done;
    }

    parse_eth_stat(&acc_ethstat[0], ACCUMULATED_STATS_FILE);
    add_in_accumulated();
    write_tmp_ethstat(&ethstat[0]);

    get_physical_states();

    switch (format)
    {
        case tlr_response_format_json:
            if (instance != -1)
            {
                if (!format_json(instance))
                {
                    goto done;
                }
            }
            break;

        case tlr_response_format_ascii:
            if (!format_ascii(instance))
            {
                goto done;
            }
            break;

        default:
            goto done;
    }

    result = EXIT_SUCCESS;

done:
    return result;
}
#endif

