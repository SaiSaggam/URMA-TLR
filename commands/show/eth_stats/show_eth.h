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

#ifndef __SHOW_ETH_H__
#define __SHOW_ETH_H__

#include "unittest_support.h"
#include <stdint.h>

typedef struct
{
    uint64_t  tx_drop_pak;
    uint64_t  tx_crc_error;
    uint64_t  tx_unicast_pak;
    uint64_t  tx_multicast_pak;
    uint64_t  tx_bcast_pak;
    uint64_t  tx_coll_pak;
    uint64_t  tx_pause_pak;

    uint64_t  rx_drop_pak;
    uint64_t  rx_filt_pak;
    uint64_t  rx_unicast_pak;
    uint64_t  rx_multicast_pak;
    uint64_t  rx_bcast_pak;
    uint64_t  rx_align_err;
    uint64_t  rx_crc_err;
    uint64_t  rx_und_err;
    uint64_t  rx_frag_err;
    uint64_t  rx_oversize_err;
    uint64_t  rx_jabber_err;
    uint64_t  rx_pause_pak;
} ethstat_t;

/* @TODO: This would be better as an array with an enumerator for indexing, that way
 * we won't need to use offsetof() when retrieving values */

#define NB_SCAN_PORTS   6   // number of ports we'll scan in and save.

#if defined(UNIT_TEST)
STATIC_FUNCTION int parse_eth_stat(ethstat_t const * const pstats, char const * const file);
STATIC_FUNCTION void add_in_accumulated(void);
STATIC_FUNCTION void element_walk_init(int const port);
STATIC_FUNCTION int walk_next_element(char const ** const elename, uint64_t * const elevalue);

extern ethstat_t ethstat[NB_SCAN_PORTS];
extern ethstat_t acc_ethstat[NB_SCAN_PORTS];    // will contain accumulated totals
#endif

#endif

