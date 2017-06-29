/*
 * Copyright (c) 2014 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */
#include "main.h"
#include "platform.h"
#include "os_support.h"
#include "connector_config.h"

connector_error_t connector_config(void);
int mac_parse(_enet_address enet_address, const char *str);
int get_line(char **lineptr, size_t limit);

TASK_TEMPLATE_STRUCT MQX_template_list[] =
{ 
/*  Task number, Entry point, Stack, Pri, String, Auto? */
   {MAIN_TASK, Main_task, 2048, 9, "main", MQX_AUTO_START_TASK},
   {CONNECTOR_TASK, connector_thread, CONNECTOR_THREAD_STACK_SIZE, 10, "Cloud Connector", 0},
#ifdef APPLICATION_FILE_SYSTEM_SDCARD
   {SDCARD_TASK, sdcard_task,  2048, 11, "SDcard Task", MQX_AUTO_START_TASK},
#endif
#ifdef APPLICATION_FILE_SYSTEM_USB
   {USB_TASK, USB_task, 2048, 12, "USB Task", MQX_AUTO_START_TASK},
#endif
#if (defined APPLICATION_FIRMWARE_SERVICE_FULL)
    {FLASH_TASK, flash_task, 4096, 10, "Connector_flash", 0},
#endif
   {0,           0,           0,     0,   0,      0,                 }
};

static uint_32 network_start(_enet_address const * mac_addr)
{
    IPCFG_IP_ADDRESS_DATA ip_data;
    uint_32 result = RTCS_create();

    if (result != RTCS_OK) 
    {
        APP_DEBUG("RTCS failed to initialize, error = %X", result);
        goto error;
    }

    result = ipcfg_init_device (ENET_DEVICE, (uint8_t *)mac_addr);
    if (result != RTCS_OK)
    {
        APP_DEBUG("Failed to initialize Ethernet device, error = %X", result);
        goto error;
    }

    APP_DEBUG("Waiting for Ethernet cable plug in ... ");
    while(!ipcfg_get_link_active(ENET_DEVICE)) {};
    APP_DEBUG("Cable connected\n");

#if (defined CONNECTOR_USE_STATIC_IP)
    ip_data.ip = CONNECTOR_DEVICE_IPADDR;
    ip_data.mask = CONNECTOR_DEVICE_IPMASK;
    ip_data.gateway = CONNECTOR_DEVICE_GATEWAY;

    ipcfg_add_dns_ip(ENET_DEVICE, CONNECTOR_DNS_SERVER_IPADDR);
    APP_DEBUG("Setting static IP address ... ");
    result = ipcfg_bind_staticip (ENET_DEVICE, &ip_data);
#else
    APP_DEBUG("Contacting DHCP server ... ");
    result = ipcfg_bind_dhcp_wait(ENET_DEVICE, FALSE, &ip_data);
#endif
    if (result != IPCFG_ERROR_OK) 
    {
        APP_DEBUG("\nRTCS failed to bind interface with IPv4, error = %X", result);
        goto error;
    }
    else
    {
        APP_DEBUG("Done\n");
    }

    ipcfg_get_ip(ENET_DEVICE, &ip_data);
    APP_DEBUG("\nIP Address      : %d.%d.%d.%d\n",IPBYTES(ip_data.ip));
    APP_DEBUG("\nSubnet Address  : %d.%d.%d.%d\n",IPBYTES(ip_data.mask));
    APP_DEBUG("\nGateway Address : %d.%d.%d.%d\n",IPBYTES(ip_data.gateway));
    APP_DEBUG("\nDNS Address     : %d.%d.%d.%d\n",IPBYTES(ipcfg_get_dns_ip(ENET_DEVICE,0)));
    result = RTCS_OK;
    

#if (defined USE_SSL)
    if (SNTP_oneshot(IPADDR(84,77,40,132), 3000) != RTCS_OK) {
    	printf("SNTP_oneshot failed!\n");
    	printf("Setting time to 1/1/2013\n");
		{
			TIME_STRUCT time;
			time.SECONDS =  1356998400; /* 01/01/2013 00:00:00 */
			time.MILLISECONDS = 0;
			_time_set(&time);
		}
    }
#endif
    
error:
    return result;
}

/* Following hook will be called during application initialization if 
   CONNECTOR_MAC_ADDRESS is not defined in connector_config.h
   It requests the Device MAC Address through the serial port and uses
   it to configure the Ethernet Stack.
*/

#define MAC_ADDR_LENGTH     6
#define MAX_MAC_ADDR_STR   20


#if defined(CONNECTOR_MAC_ADDRESS)
_enet_address device_mac_addr = CONNECTOR_MAC_ADDRESS;

_enet_address const *read_mac(void)
{
	return &device_mac_addr; 
}
#else
_enet_address device_mac_addr;

_enet_address *read_mac(void)
{
	char linebuffer[MAX_MAC_ADDR_STR];
	char *p_linebuffer = linebuffer;
	uint8_t got_mac = 0;

	do {
		printf("Enter the MAC address (aabbcc:ddeeffgg): ");
		get_line(&p_linebuffer, sizeof (linebuffer));
		putchar('\n');
		got_mac = mac_parse(device_mac_addr, linebuffer);
	} while (!got_mac);
	
	return &device_mac_addr;
}
#endif

/*TASK*-----------------------------------------------------
* 
* Task Name    : Main_task
* Comments     :
*    This starts Device Cloud Connector
*
*END*-----------------------------------------------------*/

void Main_task(uint_32 initial_data)
{
    uint_32 result;
    _enet_address const * mac_addr;
    
    mac_addr = read_mac();
    connector_config();
    result = network_start(mac_addr);
  
    if (result == RTCS_OK)
    {
        int const status = application_start();

        if (status != 0)
        {
            APP_DEBUG("application_start failed %d\n", status);
        }
    }

    _task_block();
}

/* EOF */
