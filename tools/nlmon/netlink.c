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

#include <asm/types.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <syslog.h>
#include <libgen.h>
#include "nlmon_debug.h"
#include "notify_system.h"

#define MAX_INTERFACE_NAME_LENGTH	128	/* central repo for these sorts of values? */
#define NETLINK_RX_BUF_SIZE (4*1024)

/*
	:rtnl_get_interface_name
    obtain the name of the interface this message pertains to (e.g. eth1)
    h: the start of the message
    buf: the destination buffer for the interface name
    bufsize: the size of the buffer
    returns:
    0: interface name was fuond
    -1: interface name wasn't found, or error etc
*/
static int rtnl_get_interface_name(struct nlmsghdr *h, char * const buf, size_t bufsize)
{
	int result = -1;
	struct ifinfomsg *iface;
	struct rtattr *attribute;
	int len;

	iface = NLMSG_DATA(h);
	len = h->nlmsg_len - NLMSG_LENGTH(sizeof(*iface));

	for (attribute = IFLA_RTA(iface); RTA_OK(attribute, len); attribute = RTA_NEXT(attribute, len))
	{
		switch (attribute->rta_type)
		{
			case IFLA_IFNAME:
				snprintf(buf, bufsize, "%s", (char *)RTA_DATA(attribute));
				DEBUG_PRINT("iface: %s\n", buf);
				result = 0;
				break;
			default:
				break;
		}
	}

	return result;
}

/*
    process_rtm_newlink_msg
    obtain interface state and attempt to obtain interface name from the message
    if both are found, notifiy the system of the state.
    h: a header to a struct nlmsghdr received from a netlink socket
*/
static void process_rtm_newlink_msg(struct nlmsghdr *h)
{
	int interface_state;
	char interface_name[MAX_INTERFACE_NAME_LENGTH+1];
	struct ifinfomsg *ifi;

	ifi = NLMSG_DATA(h);
	interface_state = (ifi->ifi_flags & IFF_RUNNING) != 0;
	DEBUG_PRINT("interface  state: %s\n", interface_state ? "Up" : "Down");
	if (rtnl_get_interface_name(h, interface_name, sizeof interface_name) == 0)
	{
		/* got the interface name and state. Let the system know */
		inform_system_of_interface_state_change(interface_name, interface_state);
	}
}

/*
   read_event
   read messages from the supplied netlink socket
   This application is interested in RTM_NEWLINK messages, which contain
   interface state and name.
   A complete netlink message may span multiple recvmsg calls
   returns:
   < 0: error
   0: more data expected. Call recvmsg again to get the rest of the message
   1: no more data expected.
*/
int read_event (int sockint)
{
	int status;
	int ret = 0;
	char buf[NETLINK_RX_BUF_SIZE];	/* some nice big buffer to read message into (they appear to be about 3.5KB max) */
	struct iovec iov = { buf, sizeof buf };
	struct sockaddr_nl snl;
	struct msghdr msg = { (void *) &snl, sizeof snl, &iov, 1, NULL, 0, 0 };
	struct nlmsghdr *h;

	status = recvmsg(sockint, &msg, 0);
	if (status < 0)
	{
		/* Socket non-blocking so bail out once we have read everything */
		if (errno == EWOULDBLOCK || errno == EAGAIN)
		{
			return ret;
		}

		/* Anything else is an error */
		DEBUG_PRINT("read_netlink: Error recvmsg: %d\n", status);
		return status;
	}

	if (status == 0)
	{
		DEBUG_PRINT("read_netlink: EOF\n");
	}

	// We need to handle more than one message per 'recvmsg'
	for (h = (struct nlmsghdr *)buf; NLMSG_OK(h, (unsigned int)status); h = NLMSG_NEXT(h, status))
	{
		if (h->nlmsg_type == NLMSG_DONE)        //Finished reading
		{
			ret = 1;	/* +ve value to indicate completion */
			break;
		}
		else if (h->nlmsg_type == NLMSG_ERROR)	// Message is some kind of error
		{
			DEBUG_PRINT("read_netlink: Message is an error - decode TBD\n");
			ret = -1;
			break;
		}
		else if (h->nlmsg_type == RTM_NEWLINK)	// this is the one we're interested in
		{
			process_rtm_newlink_msg(h);
		}
	}

	return ret;
}

