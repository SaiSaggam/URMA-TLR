/*****************************************************************************
 * Copyright (c) 2015 Digi International Inc., All Rights Reserved
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

#ifndef apply_eth_h
#define apply_eth_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define VAL_LEN 64
#define CMD_LEN 64
#define MIN_INSTANCE 1
#define MAX_INSTANCE 4

typedef enum {
	eth_duplex,
	eth_speed,
	eth_mtu,
	eth_state,
	eth_count
} eth_t;

bool is_interface_up(int const instance);
int eth_apply_speed_duplex(int const instance, char const * const speed, char const * const duplex);

#endif /* apply_eth_h */
