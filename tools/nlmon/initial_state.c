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

/* get the initial state of each interface. Called when the application first starts */
typedef struct nl_req_s nl_req_t;

struct nl_req_s {
  struct nlmsghdr hdr;
  struct rtgenmsg gen;
};

/*
    Send off a netlink message requesting the names of all interfaces.
    (I think we just get the connected ones).
*/
int get_initial_interface_states(void)
{
	int result = -1;
	struct sockaddr_nl addr;
	struct sockaddr_nl kernel;
	struct msghdr rtnl_msg;
	struct iovec io;
	nl_req_t req;
	pid_t pid = getpid();

	int nl_socket = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
	if (nl_socket < 0)
	{
		DEBUG_PRINT("Socket Open Error!");
		goto done;
	}

	memset((void *)&addr, 0, sizeof(addr));
	addr.nl_family = AF_NETLINK;

	addr.nl_pid = pid;
	addr.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR | RTMGRP_IPV6_IFADDR;

	if (bind(nl_socket, (struct sockaddr *) &addr, sizeof(addr)) < 0)
	{
		DEBUG_PRINT("Socket bind failed!");
		goto done;
	}

	memset(&rtnl_msg, 0, sizeof(rtnl_msg));
	memset(&kernel, 0, sizeof(kernel));
	memset(&req, 0, sizeof(req));

	/* construct the netlink message to send */
	kernel.nl_family = AF_NETLINK; /* fill-in kernel address (destination) */

	req.hdr.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtgenmsg));
	req.hdr.nlmsg_type = RTM_GETLINK;
	req.hdr.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
	req.hdr.nlmsg_seq = 1;
	req.hdr.nlmsg_pid = pid;
	req.gen.rtgen_family = AF_PACKET; /*  no preferred AF, we will get *all* interfaces */

	io.iov_base = &req;
	io.iov_len = req.hdr.nlmsg_len;
	rtnl_msg.msg_iov = &io;
	rtnl_msg.msg_iovlen = 1;
	rtnl_msg.msg_name = &kernel;
	rtnl_msg.msg_namelen = sizeof(kernel);

	DEBUG_PRINT("Send getlink request\n");
	if (sendmsg(nl_socket, (struct msghdr *)&rtnl_msg, 0) <= 0)
	{
		result = -1;
		goto done;
	}

	/*
	   handily, the response message is formatted exactly the same way as when getting notified
	   about interface state changes.
	   Note that a complete message may span multiple recvmsg calls, so keep select()ing until
	   we get an indication that the 'done' message has been received.
	*/
	while (1)
	{
		fd_set rfds;
		int retval;

		FD_ZERO(&rfds);
		FD_CLR(nl_socket, &rfds);
		FD_SET(nl_socket, &rfds);

		DEBUG_PRINT("Wait for response\n");
		retval = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);
		if (retval == -1)
		{
			DEBUG_PRINT("Error select()\n");
			syslog(LOG_ERR, "error select\n");
			result = -1;
			goto done;
		}
		else if (retval)
		{
			DEBUG_PRINT("New NETLINK message ready");
			retval = read_event(nl_socket, true);
			if (retval < 0)
			{
				syslog(LOG_ERR, "read_event error %d reading initial states\n", retval);
				result = -1;
				goto done;
			}
			else if (retval == 1)	/* done reading the whole message */
			{
				result = 0;
				break;
			}
		}
	}
done:
	if (nl_socket >= 0)
		close(nl_socket);

	return result;
}
