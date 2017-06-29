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
****************************************************************************/

#include "iptables.h"
#include "digiutils.h"
#include "tlr_print.h"

#define IP_TABLES_COMMAND "/usr/sbin/iptables"

static void do_nat_command(char const * const ifname, char const command)
{
	system_command(IP_TABLES_COMMAND " -t nat -%c POSTROUTING -o %s -j MASQUERADE > /dev/null 2>&1", command, ifname);
}

static void append_nat_rule(char const * const ifname)
{
	do_nat_command(ifname, 'A');
}

static void delete_nat_rule(char const * const ifname)
{
	do_nat_command(ifname, 'D');
}

void iptables_nat_on(char const * const ifname)
{
    tlr_debug("Turn NAT on for interface [%s]\n", ifname);
	delete_nat_rule(ifname);	// ensure any existing rule is deleted
	append_nat_rule(ifname);
}

void iptables_nat_off(char const * const ifname)
{
    tlr_debug("Turn NAT off for interface [%s]\n", ifname);
	delete_nat_rule(ifname);
}

