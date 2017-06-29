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
#include <net.h>
#include <errno.h>
#include <connector_api.h>
#include <connector_types.h>
#include <platform.h>
#include <connector_debug.h>
#include "os_support.h"

#if (APP_CFG_DNS_EN == DEF_ENABLED)
#include <dns-c.h>
#else
#warning "Your project seems to lack DNS:
#warning "Then you should fill CONNECTOR_CLOUD_URL define with Etherios Cloud IP instead of URL
#warning "If your project has DNS, please, define APP_CFG_DNS_EN  to DEF_ENABLED in your app_cfg.h file
#endif

extern connector_callback_status_t app_os_get_system_time(unsigned long * const uptime);
extern connector_callback_status_t app_os_delay(unsigned short const timeout_in_milliseconds);

int idigi_network_receive_failures = 0;
int idigi_network_send_failures = 0;
int idigi_connect_to_idigi_successes = 0;
int idigi_connect_to_idigi_failures = 0;

int nNET_SOCK_ERR_LINK_DOWN = 0;
int nNET_SOCK_ERR_CONN_SIGNAL_TIMEOUT = 0;
int nNET_DNS_ERR = 0;

static NET_SOCK_ID socket_fd = NET_SOCK_ID_NONE;

connector_bool_t app_connector_reconnect(connector_class_id_t const class_id, connector_close_status_t const status)
{
    UNUSED_ARGUMENT(class_id);


    switch (status)
    {
           /* if either the server or our application cuts the connection, don't reconnect */
        case connector_close_status_device_terminated:
        case connector_close_status_device_stopped:
        case connector_close_status_abort:
             return connector_false;

       /* otherwise it's an error and we want to retry */
       default:
    	   return connector_true;
    }
}

#if (APP_CFG_DNS_EN == DEF_ENABLED)
static int app_dns_resolve_name(char const * const domain_name, NET_IP_ADDR * const ip_addr)
{
    int ret = -1;
    
    NET_IP_ADDR   res_addr_ip;
    
    res_addr_ip = DNSc_GetHostByName((CPU_CHAR *)domain_name);
    *ip_addr = res_addr_ip;

    if (*ip_addr != 0)
        ret = 0;    

    return ret;
}
#endif

#if defined CONNECTOR_TRANSPORT_TCP
static connector_callback_status_t app_network_tcp_open(connector_network_open_t * const data)
{
#define APP_CONNECT_TIMEOUT 30
  
    static NET_SOCK_ADDR_IP addr;
    NET_SOCK_ADDR_LEN addr_size;
    connector_callback_status_t status = connector_callback_abort;
    NET_ERR err_net;
    CPU_INT08U sock_cfg08;
    CPU_INT32U sock_cfg32;
            
    static unsigned long connect_time;
    
    if (socket_fd == NET_SOCK_ID_NONE)
    {
        NET_IP_ADDR ip_addr;
        {
#if (APP_CFG_DNS_EN == DEF_ENABLED)
            if (app_dns_resolve_name(data->device_cloud.url, &ip_addr) != 0)
#else
            ip_addr = NetASCII_Str_to_IP((CPU_CHAR *) data->device_cloud.url,
                                          &err_net);
            ip_addr =  NET_UTIL_HOST_TO_NET_32(ip_addr);
            
            if (err_net !=  NET_ASCII_ERR_NONE)
#endif
            {
                nNET_DNS_ERR++;
                idigi_connect_to_idigi_failures++;
                APP_DEBUG("network_connect: Can't resolve DNS for %s. LinkDown?\n", data->device_cloud.url);
                status = connector_callback_busy;
                goto error;
            }
        }

        socket_fd = NetSock_Open( NET_SOCK_ADDR_FAMILY_IP_V4,
                                  NET_SOCK_TYPE_STREAM,
                                  NET_SOCK_PROTOCOL_TCP,
                                  &err_net);
            
        if (err_net != NET_SOCK_ERR_NONE) 
        {
            APP_DEBUG("Could not open socket\n");
            goto done;
        }
       
        addr_size = sizeof(addr);
        Mem_Clr((void *)&addr,
        (CPU_SIZE_T) addr_size);
        addr.AddrFamily = NET_SOCK_ADDR_FAMILY_IP_V4;
        addr.Addr = ip_addr; // already in network order
        addr.Port = NET_UTIL_HOST_TO_NET_16(CONNECTOR_PORT);
                
        // Override NET_SOCK_CFG_BLOCK_SEL in net_cfg.h
        sock_cfg08 = NET_SOCK_BLOCK_SEL_BLOCK;
        NetSock_CfgBlock((NET_SOCK_ID) socket_fd, sock_cfg08, &err_net);
        if (err_net != NET_SOCK_ERR_NONE)
        {
            APP_DEBUG("NetSock_CfgBlock error %d...\n", err_net);
            goto error;
        }
        
        /* 
         * Override here NET_SOCK_CFG_TIMEOUT_CONN_REQ_MS in net_cfg.h which is
         * normally set to NET_TMR_TIME_INFINITE.
         */
        sock_cfg32 = 5000;      // in mS
        NetSock_CfgTimeoutConnReqSet((NET_SOCK_ID) socket_fd, sock_cfg32, &err_net);
        if (err_net != NET_SOCK_ERR_NONE)
        {
            APP_DEBUG("NetSock_CfgTimeoutConnReqSet error %d...\n", err_net);
            goto error;
        }
        sock_cfg32 = NetSock_CfgTimeoutConnReqGet_ms((NET_SOCK_ID) socket_fd, &err_net);
        
        app_os_get_system_time(&connect_time);
        
        APP_DEBUG("Connecting to %s...\n", data->device_cloud.url);
    }

    {
        /* Connect to device */
        NET_SOCK_RTN_CODE ret= NetSock_Conn((NET_SOCK_ID) socket_fd,
                                            (NET_SOCK_ADDR *) &addr,
                                            (NET_SOCK_ADDR_LEN) (sizeof (addr)),
                                            (NET_ERR *) &err_net);          
        switch (err_net)
        {
            case NET_SOCK_ERR_NONE:
                break;
                
            case NET_SOCK_ERR_LINK_DOWN:
                nNET_SOCK_ERR_LINK_DOWN++;
                idigi_connect_to_idigi_failures++;
                APP_DEBUG("Link is down...\n");
                status = connector_callback_busy;
                
                if (socket_fd != NET_SOCK_ID_NONE)
                {
                    NetSock_Close(socket_fd, &err_net);
                    socket_fd = NET_SOCK_ID_NONE;
                }
                goto error;
              
            case NET_SOCK_ERR_CONN_SIGNAL_TIMEOUT:
                nNET_SOCK_ERR_CONN_SIGNAL_TIMEOUT++;
                idigi_connect_to_idigi_failures++;
                status = connector_callback_busy;
                goto error;
                                
            default:
                idigi_connect_to_idigi_failures ++;
                goto error;
        }
    }

    idigi_connect_to_idigi_successes ++;

    data->handle = &socket_fd;
    status = connector_callback_continue;
    APP_DEBUG("network_connect: connected to [%s] server.\n", data->device_cloud.url);
    APP_DEBUG("network_connect: %d connect() failures\n", 
               idigi_connect_to_idigi_failures); 
    APP_DEBUG("network_connect: %d LinkDown,%d TimeOut,%d Dns connect() failures\n", 
               nNET_SOCK_ERR_LINK_DOWN,
               nNET_SOCK_ERR_CONN_SIGNAL_TIMEOUT,
               nNET_DNS_ERR);
    goto done;

error:
    if (status == connector_callback_busy)
    {
        unsigned long elapsed_time;

        app_os_get_system_time(&elapsed_time);
        elapsed_time -= connect_time;

        if (elapsed_time >= APP_CONNECT_TIMEOUT)
            status = connector_callback_error;
        
        // Don't be too demandingg if link is still down.
        app_os_delay(1000);
    }
    if (status == connector_callback_error)
    {
        if (socket_fd != NET_SOCK_ID_NONE)
        {
            NetSock_Close(socket_fd, &err_net);
            socket_fd = NET_SOCK_ID_NONE;
        }
    }
done:
    return status;
}
#endif

/*
 * Send data to the iDigi Device Cloud, this routine must not block.  If it encounters
 * EAGAIN or EWOULDBLOCK error, 0 bytes must be returned and iDigi Connector will continue
 * calling this function.
 */

static connector_callback_status_t app_network_tcp_send(connector_network_send_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    
    NET_SOCK_RTN_CODE tx_size;
    NET_ERR err_net;
    
    NET_SOCK_ID *fd = data->handle;
        
    tx_size = NetSock_TxData( *fd,
                              (void *)data->buffer,
                              data->bytes_available,
                              NET_SOCK_FLAG_NONE,
                              &err_net);
    switch (err_net)
    {
        case NET_SOCK_ERR_NONE:
            data->bytes_used = tx_size;
            break;
                  
        case NET_SOCK_ERR_NOT_USED:
        case NET_SOCK_ERR_CLOSED:
            APP_DEBUG("network_send: socket closed\n");
            status = connector_callback_error;
            break;
        case NET_ERR_TX:
            /* I'm having this error when running out of 'device's large transmit buffers' */
            APP_DEBUG("network_send: NET_ERR_TX -> busy\n");
            status = connector_callback_busy;
            break;
        default:
            APP_DEBUG("network_send: failed, code = %d\n", err_net);
            status = connector_callback_error;
            break;
    }

    return status;
}

/*
 * This routine reads a specified number of bytes from the iDigi Device Cloud.  This
 * function must not block. If it encounters EAGAIN or EWOULDBLOCK error, 0
 * bytes must be returned and iDigi Connector will continue calling this function.
 */
static connector_callback_status_t app_network_tcp_receive(connector_network_receive_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;  
    NET_SOCK_RTN_CODE rx_size;
    NET_ERR err_net;
    
    NET_SOCK_ID *fd = data->handle;
    
    data->bytes_used = 0;
        
    // Use the NET_SOCK_FLAG_RX_NO_BLOCK so it does not block while reading independently of the NetSock_CfgTimeoutRxQ configuration
    rx_size = NetSock_RxData( *fd,
                              (void *)data->buffer,
                              (int)data->bytes_available,
                              NET_SOCK_FLAG_RX_NO_BLOCK,
                              &err_net);
    switch (err_net)
    {
        case NET_SOCK_ERR_NONE:
            data->bytes_used = rx_size;
            break;
                  
        case NET_SOCK_ERR_RX_Q_EMPTY:
            status = connector_callback_busy;
            break;
            
        case NET_SOCK_ERR_NOT_USED:
        case NET_SOCK_ERR_CLOSED:
            APP_DEBUG("network_receive: NET_SOCK_ERR_CLOSED\n");
            status = connector_callback_error;
            break;

       case NET_SOCK_ERR_INVALID_TYPE:
            APP_DEBUG("network_receive: NET_SOCK_ERR_INVALID_TYPE\n");
            status = connector_callback_abort;
            break;

        default:
            APP_DEBUG("network_receive: failed, code = %d\n", err_net);
            status = connector_callback_error;
            break;
    }
    
    return status;
}

static connector_callback_status_t app_network_tcp_close(connector_network_close_t * const data)
{
    connector_callback_status_t status;
    NET_SOCK_RTN_CODE rtn_code;
    NET_ERR err_net;

    NET_SOCK_ID *fd = data->handle;
  
    ASSERT(*fd == socket_fd);
    
    rtn_code = NetSock_Close(*fd, &err_net);
    
    switch (err_net)
    {
        case NET_SOCK_ERR_CLOSED:
        case NET_SOCK_ERR_FAULT:
        case NET_SOCK_ERR_NONE:
		case NET_SOCK_ERR_CONN_SIGNAL_TIMEOUT:
            status = connector_callback_continue;
            socket_fd = NET_SOCK_ID_NONE;
            break;
 
        default:
            APP_DEBUG("network_close: failed, code = %d,%d\n", err_net, rtn_code);
            status = connector_callback_continue;
            *fd = NET_SOCK_ID_NONE;
            break;
    }
    
    data->reconnect = app_connector_reconnect(connector_class_id_network_tcp, data->status);
    
    return status;
}

/*
 *  Callback routine to handle all networking related calls.
 */
connector_callback_status_t app_network_tcp_handler(connector_request_id_network_t const request_id,
                                                    void * const data)
{
    connector_callback_status_t status;

    switch (request_id)
    {
    case connector_request_id_network_open:
        status = app_network_tcp_open(data);
        break;

    case connector_request_id_network_send:
        status = app_network_tcp_send(data);
        break;

    case connector_request_id_network_receive:
        status = app_network_tcp_receive(data);
        break;

    case connector_request_id_network_close:
        status = app_network_tcp_close(data);
        break;

    default:
        APP_DEBUG("app_network_tcp_handler: unrecognized callback request_id [%d]\n", request_id);
        status = connector_callback_unrecognized;
        break;

    }

    return status;
}
