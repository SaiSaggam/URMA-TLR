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

#include <CppUTest/CommandLineTestRunner.h>
#include "CppUTestExt/MockSupportPlugin.h"
#include <CppUTest/TestHarness.h>
#include "CppUTest/TestRegistry.h"
#include "CppUTestExt/MockSupport.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

extern "C"
{
#include "dhcp_client.h"
#include "tlr_print.h"
#include "config_mgr.h"
#include "config_def.h"
#include "interfaces.h"
};

extern "C"
{
    // some mock'd routines
    char const *interface_get_name_from_type(interfaces_t const type)
    {
	static char tab[32];
	  strcpy(tab, "eth");
	  return tab;
    }

    int system_command(char const *const format, ...)
    {
	char *command = NULL;
	va_list args;

	va_start(args, format);
	if (vasprintf(&command, format, args) > 0)
	  {
	      if (strcmp
		  (command,
		   "/sbin/udhcpc -i eth1 -p /ram/var/dns/eth1/udhcpc.pid -s /usr/share/udhcpc/tlr_dhcp_client_script.sh &")
		  == 0)
		{
		    // ok
		    LONGS_EQUAL(0, 0);
		}
	      else
		{
		    // fail
		    LONGS_EQUAL(0, -1);
		}

	      free(command);
	  }
	va_end(args);

	return 0;		// 0 means success
    }

    int kill(pid_t pid, int sig);

    int kill(pid_t pid, int sig)
    {
	return 0;
    }
};

TEST_GROUP(dhcp_client_test)
{
    void setup(void)
    {
    }
};

TEST(dhcp_client_test, test_dhcp_client_stop)
{
    int sts;
    interfaces_t interface = eth_interface;
    size_t instance = 1;

    sts = dhcp_client_stop(interface, instance);

    LONGS_EQUAL(0, sts);
};

TEST(dhcp_client_test, test_dhcp_client_start)
{
    int sts;
    interfaces_t interface = eth_interface;
    size_t instance = 1;

    sts = dhcp_client_start(interface, instance);

    LONGS_EQUAL(0, sts);
};

TEST(dhcp_client_test, test_dhcp_client_get_pid)
{
    pid_t sts;
    interfaces_t interface = eth_interface;
    size_t instance = 1;

    sts = dhcp_client_get_pid(interface, instance);

    LONGS_EQUAL(0, sts);
};

TEST(dhcp_client_test, test_dhcp_client_get_pid_filename)
{
    char *fil;

    interfaces_t interface = eth_interface;
    size_t instance = 1;

    fil = dhcp_client_get_pid_filename(interface, instance);

    if (strcmp(fil, "/ram/var/dns/eth1/udhcpc.pid") == 0)
      {
	  // ok
	  LONGS_EQUAL(0, 0);
      }
    else
      {
	  // fail
	  LONGS_EQUAL(0, -1);
      }

    // tidy up
    free(fil);
};


// main entry point
int
main(int argc, char *argv[])
{
    MockSupportPlugin mock_plugin;

    TestRegistry::getCurrentRegistry()->installPlugin(&mock_plugin);

    return CommandLineTestRunner::RunAllTests(argc, argv);
}
