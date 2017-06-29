/*
 * Copyright (c) 2013 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */
#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

#include "bele.h"
#include "connector_config.h"
#include "connector_debug.h"
#include "connector_api.h"
#include "platform.h"
#include "application.h"

static bool verbose_mode;

typedef struct
{
    int adapter_index;
    struct in_addr my_ip_addr;
    struct in_addr cloud_ip_addr;
    uint8_t my_mac_addr[CONNECTOR_MAC_ADDR_LENGTH];
    uint8_t vendor_id[CONNECTOR_VENDOR_ID_LENGTH];
    char * cloud_url;

} app_config_t;
static app_config_t app_config;

typedef struct
{
    unsigned long ping_failed_cnt;
    unsigned long missed_ka_cnt;
    unsigned long missed_ka_time;
    unsigned long open_conn_cnt;
    unsigned long open_conn_fail_cnt;
    unsigned long closed_conn_cnt;
    unsigned long dns_error_cnt;
    connector_status_t close_reason;
    connector_status_t error_status;

} app_statistics_t;
static app_statistics_t app_stats;

unsigned long app_get_stats(app_stats_id_t const id)
{
    unsigned long val = 0;

    switch(id)
    {
        case ping_failed_id:
            val = app_stats.ping_failed_cnt;
            break;
        case missed_ka_id:
            val = app_stats.missed_ka_cnt;
            break;
        case missed_ka_time_id:
            val = app_stats.missed_ka_time;
            break;
        case open_conn_id:
            val = app_stats.open_conn_cnt;
            break;
        case open_conn_fail_id:
            val = app_stats.open_conn_fail_cnt;
            break;
        case closed_conn_id:
            val = app_stats.closed_conn_cnt;
            break;
        case dns_error_id:
            val = app_stats.dns_error_cnt;
            break;
        case close_reason_id:
            val = (unsigned long) app_stats.close_reason;
            break;
        case error_status_id:
            val = (unsigned long) app_stats.error_status;
            break;
        default:
            break;
    }
    return val;
}

void app_inc_stats(app_stats_id_t const id)
{
    if (id != close_reason_id)
    {
        app_set_stats(id, 1 + app_get_stats(id));
    }
}

void app_set_stats(app_stats_id_t const id, unsigned long val)
{
    switch(id)
    {
        case ping_failed_id:
            app_stats.ping_failed_cnt = val;
            break;
        case missed_ka_id:
            app_stats.missed_ka_cnt = val;
            break;
        case missed_ka_time_id:
            app_stats.missed_ka_time = val;
            break;
        case open_conn_id:
            app_stats.open_conn_cnt = val;
            break;
        case open_conn_fail_id:
            app_stats.open_conn_fail_cnt = val;
            break;
        case closed_conn_id:
            app_stats.closed_conn_cnt = val;
            break;
        case dns_error_id:
            app_stats.dns_error_cnt = val;
            break;
        case close_reason_id:
            app_stats.close_reason = (connector_status_t) val;
            break;
        case error_status_id:
            app_stats.error_status = (connector_status_t) val;;
            break;
        default:
            break;
    }
}

static bool match_mac_pattern(char const * mac, char const * pattern)
{
    unsigned i;
    bool result = false;

    size_t len = strlen(pattern);
    if (len != strlen(mac))
        goto error;

    for (i = 0; i < len; i++)
    {
        if (pattern[i] == ':' || pattern[i] == '-')
        {
            if (pattern[i] != mac[i])
                goto error;
        }
        else
        if (!isxdigit(mac[i]))
            goto error;
    }
    result = true;

error:
    return result;
}

static  void StoreBE24(void * const array, uint32_t const val)
{
    ((uint8_t *)(array))[0] = BYTE32_2(val);
    ((uint8_t *)(array))[1] = BYTE32_1(val);
    ((uint8_t *)(array))[2] = BYTE32_0(val);
}

static bool parse_mac_address(char * mac, uint8_t addr[])
{
    const char * pat1 = "xxxxxx:xxxxxx";
    const char * pat2 = "xx:xx:xx:xx:xx:xx";
    const char * pat3 = "xx-xx-xx-xx-xx-xx";
    char * ptr = mac - 1;
    bool result = false;

    if (match_mac_pattern(mac, pat1))
    {
        unsigned long val1 = strtoul(++ptr, &ptr, 16);
        unsigned long val2 = strtoul(++ptr, &ptr, 16);
        if ((val1 | val2) == 0)
            goto error;

        StoreBE24(addr, val1);
        StoreBE24(&addr[3], val2);
        result = true;
    }
    else
    if (match_mac_pattern(mac, pat2) || match_mac_pattern(mac, pat3))
    {
        int i;
        uint8_t val = 0;

        for (i = 0; i < CONNECTOR_MAC_ADDR_LENGTH; i++)
        {
            addr[i] = (uint8_t) strtoul(++ptr, &ptr, 16);
            val |= addr[i];
        }
        result = val != 0;
    }
error:
    return result;
}

static bool parse_adapter_index(char * const str, int * adapter_index)
{
    *adapter_index = 0;

    if (isdigit(*str))
    {
       *adapter_index = atoi(str);
    }
    return *adapter_index > 0;
}

static bool parse_vendor_id(char const * vendor_str, uint8_t vendor_id[])
{
    const char * ptr;
    bool result = false;
    uint32_t val = 0;
    size_t len = strlen(vendor_str);

    if (len <= 2 && len > (2 * CONNECTOR_VENDOR_ID_LENGTH + 2))
      goto error;

    ptr = vendor_str;

    if (*ptr++ != '0' || *ptr++ != 'x')
        goto error;

    for (; *ptr; ptr++)
    {
        if (!isxdigit(*ptr))
            goto error;
    }
    val = (uint32_t) strtoul(vendor_str, (char **) NULL, 16);
    StoreBE32(vendor_id, val);

    if (val == 0)
        goto error;

    result = true;
error:
    return result;
}





static void print_device_config(app_config_t * const config)
{
    uint8_t *ptr;
    APP_DEBUG("\nUsing device configuration:\n");
    APP_DEBUG("iDigi Device Cloud URL: %s\n", config->cloud_url);

    ptr = config->vendor_id;
    APP_DEBUG("Vendor ID:   0x%.2x%.2x%.2x%.2x\n",ptr[0], ptr[1], ptr[2], ptr[3]);

    ptr = config->my_mac_addr;
    APP_DEBUG("MAC address: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
                ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);

    APP_DEBUG("IP Address:  %s\n\n", inet_ntoa(config->my_ip_addr));
}

static bool ping_connector_server(app_config_t * const config)
{
    char ping_cmd[sizeof("ping 255.255.255.255 -n 2 > nul:")];
    bool result = false;
    unsigned long server_ip_addr;

    APP_DEBUG("Resolving IP address for %s...\n", config->cloud_url);

    if (app_dns_resolve_name(config->cloud_url, &server_ip_addr) < 0)
    {
       app_inc_stats(dns_error_id);
       APP_ALT_PRINTF("Could not resolve DNS name for %s\n", config->cloud_url);
       goto done;
    }

    app_show_progress();

    config->server_ip_addr.s_addr = server_ip_addr;
    APP_DEBUG("Pinging for %s...\n", inet_ntoa(config->server_ip_addr));
    sprintf(ping_cmd, "ping %s -n 3 > nul:", inet_ntoa(config->server_ip_addr));
    result = system(ping_cmd) == 0;
    app_show_progress();

    if (!result)
    {
        APP_PRINTF("Ping request timed out for %s\n",config->cloud_url );
        app_inc_stats(ping_failed_id);
    }
done:
    return result;
}

static bool connect_to_idigi(app_config_t * const config)
{
    int result = SOCKET_ERROR, namelen;
    struct sockaddr_in sin, name;
    SOCKET fd = socket(AF_INET, SOCK_STREAM, 0);

    APP_DEBUG("Connecting to %s\n", config->cloud_url);

    if (fd == INVALID_SOCKET)
    {
        APP_PRINTF("connect_to_idigi: Could not open socket %d\n", WSAGetLastError());
        goto done;
    }

    memset((char *)&sin, 0, sizeof(sin));
    sin.sin_addr = config->cloud_ip_addr;
    sin.sin_port   = htons(CONNECTOR_PORT);
    sin.sin_family = AF_INET;
    result = connect(fd, (struct sockaddr *)&sin, sizeof(sin));
    if (result == SOCKET_ERROR)
    {
		int const error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK)
        {
            APP_PRINTF("connect_to_idigi: connect() failed for Device Cloud %s, error %d \n", config->cloud_url, error);
            goto done;
        }
    }
    name.sin_family = AF_INET;
    namelen = sizeof name;
    result = getsockname(fd, (struct sockaddr *) &name, &namelen);
    if (result == SOCKET_ERROR)
    {
		int const error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK)
        {
            APP_PRINTF("connect_to_idigi: getsockname() failed %d\n", error);
            goto done;
        }
    }
    config->my_ip_addr = name.sin_addr;

done:
    if (fd != SOCKET_ERROR)
        closesocket(fd);

    if (result != 0)
        app_inc_stats(open_conn_fail_id);

    return result == 0;
}


static void print_adapter_name(PIP_ADAPTER_ADDRESSES aa, int aa_index)
{
	char buf[BUFSIZ];
	memset(buf, 0, BUFSIZ);
	WideCharToMultiByte(CP_ACP, 0, aa->FriendlyName, (int) wcslen(aa->FriendlyName), buf, BUFSIZ, NULL, NULL);
	APP_DEBUG("%d Adapter:%s\n", aa_index, buf);
}

static int is_ipv4_addr(PIP_ADAPTER_UNICAST_ADDRESS ua)
{
	return  (ua->Address.lpSockaddr->sa_family == AF_INET) ? 1 : 0;
}

static void print_ipv4_addr(PIP_ADAPTER_UNICAST_ADDRESS ua)
{
	char buf[BUFSIZ];

	if (ua->Address.lpSockaddr->sa_family == AF_INET)
	{
		APP_DEBUG("\t%s ",  "IPv4");

		memset(buf, 0, BUFSIZ);
		getnameinfo(ua->Address.lpSockaddr, ua->Address.iSockaddrLength, buf, sizeof(buf), NULL, 0,NI_NUMERICHOST);
		APP_DEBUG("%s\n", buf);
	}
}

static void print_physical_addr(PIP_ADAPTER_ADDRESSES aa)
{
	DWORD i;

	APP_DEBUG("\tPhysical address: ");
	for (i = 0; i < aa->PhysicalAddressLength; i++)
	{
		if (i == (aa->PhysicalAddressLength - 1))
			APP_DEBUG("%.2X\n",
				   (int) aa->PhysicalAddress[i]);
		else
            APP_DEBUG("%.2X:",
				   (int) aa->PhysicalAddress[i]);
	}
}

static int print_adapter_addresses(void)
{
	DWORD rv, size;
	PIP_ADAPTER_ADDRESSES adapter_addresses = NULL;
	PIP_ADAPTER_ADDRESSES aa;
	int aa_ipv4_cnt = 0;
	int aa_index = 0;

    APP_DEBUG("Device Network Adapters:\n\n");
	rv = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, NULL, &size);
	if (rv != ERROR_BUFFER_OVERFLOW)
	{
        APP_PRINTF("GetAdaptersAddresses() failed...");
		goto done;
	}
	adapter_addresses = (PIP_ADAPTER_ADDRESSES)malloc(size);

	rv = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, adapter_addresses, &size);
	if (rv != ERROR_SUCCESS)
	{
		APP_PRINTF("GetAdaptersAddresses() failed...");
		goto done;
	}

	for (aa = adapter_addresses; aa != NULL; aa = aa->Next)
	{
		if (aa->PhysicalAddressLength == CONNECTOR_MAC_ADDR_LENGTH)
		{
			aa_index++;
			print_adapter_name(aa, aa_index);
			print_physical_addr(aa);

			if (aa->FirstUnicastAddress != NULL)
			{
				PIP_ADAPTER_UNICAST_ADDRESS ua;
				int ipv4_cnt = 0;

				for (ua = aa->FirstUnicastAddress; ua != NULL; ua = ua->Next)
				{
					ipv4_cnt += is_ipv4_addr(ua);
				}

				if (ipv4_cnt > 0)
				{
					aa_ipv4_cnt++;

					for (ua = aa->FirstUnicastAddress; ua != NULL; ua = ua->Next)
					{
						print_ipv4_addr(ua);
					}
				}
			}
		}
	}

done:
	if (adapter_addresses != NULL)
		free(adapter_addresses);
	return aa_ipv4_cnt;
}


static bool get_adapter_address(int const aa_index, struct in_addr * const ip_address, uint8_t * const mac_address)
{
	DWORD rv, size;
	PIP_ADAPTER_ADDRESSES adapter_addresses = NULL;
	PIP_ADAPTER_ADDRESSES aa;
	int aa_cnt = 0;
	bool result = false;

    ip_address->s_addr = 0;

	rv = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, NULL, &size);
	if (rv != ERROR_BUFFER_OVERFLOW)
	{
		APP_PRINTF("GetAdaptersAddresses() failed...");
		goto done;
	}
	adapter_addresses = (PIP_ADAPTER_ADDRESSES)malloc(size);

	rv = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, adapter_addresses, &size);
	if (rv != ERROR_SUCCESS)
	{
		APP_PRINTF("GetAdaptersAddresses() failed...");
		goto done;
	}

	for (aa = adapter_addresses; aa != NULL; aa = aa->Next)
	{
		if (aa->PhysicalAddressLength == CONNECTOR_MAC_ADDR_LENGTH)
		{
			if (++aa_cnt == aa_index)
			{
				if (aa->FirstUnicastAddress != NULL)
				{
					PIP_ADAPTER_UNICAST_ADDRESS ua;
					int ipv4_cnt = 0;

					for (ua = aa->FirstUnicastAddress; ua != NULL; ua = ua->Next)
					{
						ipv4_cnt += is_ipv4_addr(ua);
					}

					if (ipv4_cnt > 0)
					{
						for (ua = aa->FirstUnicastAddress; ua != NULL; ua = ua->Next)
						{
							if (ua->Address.lpSockaddr->sa_family == AF_INET)
							{
								struct sockaddr_in * addr_in = (struct sockaddr_in *)ua->Address.lpSockaddr;
                                if (addr_in != NULL)
								    *ip_address = addr_in->sin_addr;
								break;
							}
						}
						if (ip_address->s_addr != 0)
						{
							memcpy(mac_address, aa->PhysicalAddress, aa->PhysicalAddressLength);
							result = true;
						}
					}
				}
				break;
			}
		}
	}
done:
	if (adapter_addresses != NULL)
		free(adapter_addresses);

	return result;
}

#define is_vendor(str) (*(str) == '0' && *((str)+1) == 'x')
#define is_url(str) (strchr((str), '.'))
#define is_mac(str) (strchr((str),':') || strchr((str),'-'))


static bool parse_cmd_line(int argc, char **argv, app_config_t * const config)
{
    bool result = false;
    int i, num_args = argc;
    char * vendor_str = NULL;
    char * url_str = NULL;
    char * mac_str = NULL;
    char * adapter_str = NULL;


    for (i = 1; i < num_args; i++)
    {
        if (strcmp("verbose", argv[i]) == 0)
            verbose_mode = true;
        else
        if (vendor_str == NULL && is_vendor(argv[i]))
            vendor_str = argv[i];
        else
        if (url_str == NULL && is_url(argv[i]))
            url_str = argv[i];
        else
        if (mac_str == NULL && is_mac(argv[i]))
            mac_str = argv[i];
        else
            adapter_str = argv[i];
    }
    if (verbose_mode)
        num_args--;

    if ((vendor_str == NULL || url_str == NULL) ||
        ((num_args > 3) && (mac_str == NULL && vendor_str == NULL))
        )
    {
        print_adapter_addresses();

        APP_PRINTF("Usage  : %s <iDigi Device Cloud Server URL> <Vendor ID> [<MAC Address>]\n\n", APP_NAME);
        APP_PRINTF("<iDigi Device Cloud Server URL> -  login.etherios.com or my.idigi.co.uk\n\n");
        APP_PRINTF("<Vendor ID>   - Unique code identifying the manufacturer of a device,\n");
        APP_PRINTF("                presented as hexidecimal number, for example 0x0300000a\n\n");
        APP_PRINTF("<MAC Address> - MAC address of the device, used to identify the device to Etherios Device Cloud,\n");
        APP_PRINTF("                for example 01:02:03:04:0a:0b\n\n");
        goto done;
    }

    APP_PRINTF("\nAttempting to connect and maintain a connection to %s:%d\n", url_str, CONNECTOR_PORT);
    APP_PRINTF("Please wait one minute to complete\n\n");

    {
        size_t cloud_url_size = strlen(url_str) + 1;
        config->cloud_url = malloc(cloud_url_size);
        if (config->cloud_url == NULL)
        {
            APP_PRINTF("Could not allocate mamory\n");
            goto done;
        }
        memcpy(config->cloud_url, url_str, cloud_url_size);
    }
    if (!parse_vendor_id(vendor_str, config->vendor_id))
    {
        APP_PRINTF("Invalid vendor ID %s\n", vendor_str);
        goto done;
    }
    if (num_args == 3)
    {
        config->adapter_index = 1;
    }
    else
    {
        if (mac_str != NULL)
        {
            if (!parse_mac_address(mac_str, config->my_mac_addr))
            {
                APP_PRINTF("Invalid MAC address id %s\n", mac_str);
                goto done;
            }
        }
        else
        if (!parse_adapter_index(adapter_str, &config->adapter_index))
        {
            APP_PRINTF("Invalid adapter index id %s\n", adapter_str);
            goto done;
        }
    }
    if (config->adapter_index > 0)
    {
    	if (!get_adapter_address(config->adapter_index, &config->my_ip_addr, config->my_mac_addr))
        {
            APP_PRINTF("Could not obtain MAC or IP address for adapter # %d\n", config->adapter_index);
            goto done;
        }
    }

    result = true;

done:
    return result;
}

/*** Public API ***/

bool app_verbose_mode(void)
{
    return verbose_mode;
}

bool application_get_ip_address(uint8_t ** const ip_address, size_t * const size)
{
    *ip_address = (uint8_t *) &app_config.my_ip_addr.s_addr;
    *size = sizeof app_config.my_ip_addr.s_addr;

	return true;
}

bool application_get_mac_addr(uint8_t ** const addr, size_t * const size)
{
    *addr = app_config.my_mac_addr;
    *size = sizeof app_config.my_mac_addr;

    return true;
}

bool application_get_vendor_id(uint8_t const ** id, size_t * const size)
{
    *id = app_config.vendor_id;
    *size = sizeof(app_config.vendor_id);

    return true;
}

bool application_get_cloud_url(char const ** url, size_t * const size)
{
    *url = app_config.cloud_url;
    *size = strlen(app_config.cloud_url);

    return true;
}

void application_cleanup(void)
{
    if (app_config.cloud_url != NULL)
        free(app_config.cloud_url);
}

bool application_init(int argc, char **argv)
{
    bool result = false;

    if (!parse_cmd_line(argc, argv, &app_config))
	    goto done;

    app_show_progress();
    if (!ping_connector_server(&app_config))
        goto done;
    app_show_progress();
    APP_DEBUG("\n");

    if (!connect_to_idigi(&app_config))
        goto done;

    app_show_progress();
    print_device_config(&app_config);
    result = true;

done:
    return result;
}

void print_test_result(int result)
{
    if (result == 0)
    {
        APP_PRINTF("\nSuccessfully connected to %s:%d\n\n", app_config.cloud_url, CONNECTOR_PORT);
        if (app_stats.open_conn_fail_cnt > 0)
            APP_DEBUG("Failed to establish connection %u times\n",app_stats.open_conn_fail_cnt);
        APP_DEBUG("Connection closed by Device Cloud %u times\n", app_stats.closed_conn_cnt);
        APP_DEBUG("Missed keepalive %u times\n",app_stats.missed_ka_cnt);
    }
    else
    {
        APP_PRINTF("\nFailed to connect to %s:%d\n\n", app_config.cloud_url, CONNECTOR_PORT);
        if (app_stats.open_conn_fail_cnt > 0)
        {
            APP_PRINTF("Failed to establish connection %u times\n",app_stats.open_conn_fail_cnt);
            if (app_stats.dns_error_cnt > 0)
                APP_PRINTF("Could not resolve DNS for %s\n", app_config.cloud_url);
        }
        APP_PRINTF("Connection closed by Device Cloud %u times\n", app_stats.closed_conn_cnt);
#ifdef CONNECTOR_DEBUG
        if (app_stats.closed_conn_cnt > 0)
        {
            if (app_stats.close_reason != connector_success)
                APP_DEBUG("Reason: %s\n", app_status_error_to_string(app_stats.close_reason));
            if (app_stats.error_status != connector_success)
                APP_DEBUG("Error: %s\n", app_status_error_to_string(app_stats.error_status));
        }
#endif
        APP_PRINTF("Missed keepalive %u times\n",app_stats.missed_ka_cnt);
    }
}
