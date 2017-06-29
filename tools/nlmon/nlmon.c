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
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <stdlib.h>
#include <sys/time.h>
#include <syslog.h>
#include "nlmon_debug.h"
#include "netlink.h"

int nlmon_worker(void)
{
	fd_set rfds;
	int retval;
	struct sockaddr_nl addr;
	int result;

	int nl_socket = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
	if (nl_socket < 0)
	{
		DEBUG_PRINT("Socket Open Error!\n");
		result = EXIT_FAILURE;
		syslog(LOG_ERR, "nlmon socket open serror\n");
		goto done;
	}

	memset ((void *)&addr, 0, sizeof (addr));

	addr.nl_family = AF_NETLINK;
	addr.nl_pid = getpid();
	addr.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR | RTMGRP_IPV6_IFADDR;

	if (bind(nl_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		DEBUG_PRINT("Socket bind failed! %s\n", strerror(errno));
		result = EXIT_FAILURE;
		syslog(LOG_ERR, "socket bind fail\n");
		goto done;
	}

	while (1)
	{
		FD_ZERO(&rfds);
		FD_CLR(nl_socket, &rfds);
		FD_SET(nl_socket, &rfds);

		/* block until a message is ready */
		DEBUG_PRINT("Wait for response\n");
		retval = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);
		if (retval == -1)
		{
			DEBUG_PRINT("Error select()\n");
			syslog(LOG_ERR, "select error\n");
			result = EXIT_FAILURE;
			goto done;
		}
		else if (retval)
		{
			DEBUG_PRINT("New NETLINK message ready\n");
			retval = read_event(nl_socket);
			if (retval < 0)
			{
				syslog(LOG_ERR, "read_event error %d\n", retval);
				result = EXIT_FAILURE;
				goto done;
			}
		}
	}

    result = EXIT_SUCCESS;

done:

	if (nl_socket >= 0)
		close(nl_socket);
	return result;
}

