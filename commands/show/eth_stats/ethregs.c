#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if.h>
#include <linux/mii.h>
#include <linux/types.h>
#include "ethphy.h"


/**
 * Read a phy register.
 *
 */
int reg_read(int port, int phyreg, int * value)
{
    // port is in range of 0-4.
    // 0 = connection from cpu to dsl. (so you probably don't need to be reading this one).
    // port 1-4 for the Eth ports physically presented out the box.

    // Use the mii_mgr app to do this.. e.g. to read reg 1 of phy 1
    // mii_mgr -g -p 1 -r 1
    FILE *  fp;
    char    cmd_line[40];
    char    line[80];
    int     success = -1;

    snprintf(cmd_line, sizeof cmd_line, "mii_mgr -g -p %d -r %d", port, phyreg);

    fp = popen(cmd_line, "r");

    if (fp == NULL)
        goto error;

    if (fgets(line, sizeof line, fp) != NULL)
    {
        char *  word;
        char *  save;
        const char s[2] = "=";
        // should get something like "Get: phy[1].reg[1] = 7949"
        // We need to extract just the hex data after the '='

        /* get the first word (before the ".OK i.e. result should be either "connect.OK" or "disconnect.OK") */

        word = strtok_r(line, s, &save);
        if (word != NULL)
        {
            // And do this again to get the part after the '='
            word = strtok_r(NULL, s, &save);
            if (word != NULL)
            {
                if (sscanf(word, "%x", value) == 1)
                {
                    success = 1;
                }
            }
        }
    }

    pclose(fp);

error:
    return success;
}

/**
 * Write a phy register.
 *
 */
int reg_write(int port, int phyreg, int value)
{
    // port is in range of 0-4.
    // 0 = connection from cpu to switch. (so you probably don't need to be writing to this one).
    // port 1-4 for the Eth ports physically presented out the box.

    // Use the mii_mgr to set registers... eg..
    //mii_mgr -s -p 4 -r 1 -v 0xff11
    char    cmd_line[80];

    snprintf(cmd_line, sizeof cmd_line, "mii_mgr -s -p %d -r %d -v 0x%x", port, phyreg, value);

	if (system(cmd_line) != 0)
		return -1;
	return 0;

}

/**
 * Returns link connection state of the given port.
 *
 */
int is_linkup(int port)
{
    int     phy_stat;

    if ( reg_read(port, PHY_STAT, &phy_stat) < 0 )
        return 0;

    if (phy_stat & PHY_STAT_LINKUP)
        return 1;
    else
        return 0;
}



