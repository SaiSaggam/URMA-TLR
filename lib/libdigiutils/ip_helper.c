#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <stdint.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/sockios.h>
#include <termios.h>
#include <sys/sysinfo.h>
#include "tlr_print.h"
#include "ip_helper.h"
#include "strlcpy.h"
#include <sys/ioctl.h>

static char const * const ip_items[] = {
        [ip_rx_packets] = "rx_packets",
        [ip_tx_packets] = "tx_packets",
        [ip_rx_bytes] = "rx_bytes",
        [ip_tx_bytes] = "tx_bytes"
};

bool get_ifr(char const * const ifname, int const request, struct ifreq * const ifr)
{
    bool success = false;

    if (ifname == NULL) goto done;

    {
        int const fd = socket(AF_INET, SOCK_DGRAM, 0);

        if (fd < 0) goto done;

        ifr->ifr_addr.sa_family = AF_INET;
        strlcpy(ifr->ifr_name, ifname, sizeof ifr->ifr_name);
        success = (ioctl(fd, request, ifr) == 0);
        close(fd);
    }

done:
    return success;
}

void get_ipaddr (char const * const ifname, char * const buf, size_t const bufsize)
{
    if (buf == NULL || bufsize == 0) goto done;

    {
        struct ifreq ifr;
        char const * const ipaddr = get_ifr(ifname, SIOCGIFADDR, &ifr) ? inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr) : NOT_ASSIGNED_STRING;

        strlcpy (buf, ipaddr, bufsize);
    }

done:
    return;
}

void get_hwaddr (char const * const ifname, char * const buf, size_t const bufsize)
{
    struct ifreq ifr;

    if (buf == NULL || bufsize == 0) goto done;

    if (get_ifr(ifname, SIOCGIFHWADDR, &ifr))
    {
        unsigned char const * const mac = (unsigned char *)&ifr.ifr_hwaddr.sa_data;

        snprintf(buf, bufsize, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }
    else
        strlcpy(buf, NOT_ASSIGNED_STRING, bufsize);

done:
    return;
}

void get_newmaskaddr (char const * const ifname, char * const buf, size_t const bufsize )
{
    if (buf == NULL || bufsize == 0) goto done;

    {
        struct ifreq ifr;
        char const * const mask = get_ifr(ifname, SIOCGIFNETMASK, &ifr) ? inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr) : NOT_ASSIGNED_STRING;

        strlcpy (buf, mask, bufsize);
    }

done:
    return;
}

/* Get the destination address of a point-to-point device */
void get_destaddr (char const * const ifname, char * const buf, size_t const bufsize )
{
    if (buf == NULL || bufsize == 0) goto done;

    {
        struct ifreq ifr;
        char const * const dest = get_ifr(ifname, SIOCGIFDSTADDR, &ifr) ? inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr) : NOT_ASSIGNED_STRING;

        strlcpy (buf, dest, bufsize);
    }

done:
    return;
}

/* Get MTU of an interface */
void get_mtu (char const * const ifname, int * const buf)
{
    struct ifreq ifr;

    if (buf == NULL) goto done;
    *buf = get_ifr(ifname, SIOCGIFMTU, &ifr) ? ifr.ifr_mtu : 0;

done:
    return;
}


static int parse_ip_statistics(char const * const interface, char const * const item, uint64_t * const value)
{
    char const statistics_string[] = "statistics";
    char *line = NULL;
    int ret=-1;

    if (interface == NULL || item == NULL || value == NULL)
        goto done;

    *value = 0;

    {
        char file_name[strlen(item) + sizeof statistics_string + 1];
        snprintf(file_name, sizeof file_name, "statistics/%s", item);

        line = tlr_get_sys_class_net(interface, file_name);
        if (line == NULL)
        {
            goto done;
        }
    }

    *value = strtoull(line, NULL, 10);
    ret = 0;

done:
    free(line);
    return ret;
}


int get_rx_packets(char const * const interface, uint64_t * const value)
{
    return parse_ip_statistics(interface, ip_items[ip_rx_packets], value);
}

int get_tx_packets(char const * const interface, uint64_t * const value)
{
    return parse_ip_statistics(interface, ip_items[ip_tx_packets], value);
}

int get_rx_bytes(char const * const interface, uint64_t * const value)
{
    return parse_ip_statistics(interface, ip_items[ip_rx_bytes], value);
}

int get_tx_bytes(char const * const interface, uint64_t * const value)
{
    return parse_ip_statistics(interface, ip_items[ip_tx_bytes], value);
}

char const * tlr_get_mac_address(char const * const interface_name)
{
    char const * line = tlr_get_sys_class_net(interface_name, "address");
    if (line == NULL)
    {
        tlr_debug("error getting MAC address for %s\n", interface_name);
    }

    return line;
}

/* This function returns allocated memory containing the content of the object.
 * Caller must call free on the returned pointer.
 */
char * tlr_get_sys_class_net(char const * const interface_name, char const * const object)
{
    int result;
    int temp_result;
    FILE * fp = NULL;
    char * line = NULL;
    size_t line_len = 0;
    char * filename = NULL;

    if ((interface_name == NULL)  || (asprintf(&filename, "/sys/class/net/%s/%s", interface_name, object) < 0) || (filename == NULL))
    {
        result = -1;
        goto done;
    }

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        result = -1;
        goto done;
    }

    temp_result = getline( &line, &line_len, fp);
    if (temp_result <= 0)
    {
        result = -1;
        goto done;
    }
    if (line[temp_result - 1] == '\n')
    {
        line[temp_result - 1] = '\0';
    }
    result = 0;

done:
    if (fp != NULL)
    {
        fclose(fp);
    }
    if (result != 0)
    {
        tlr_debug("error getting %s\n", filename);
    }
    free(filename);
    return line;
}
