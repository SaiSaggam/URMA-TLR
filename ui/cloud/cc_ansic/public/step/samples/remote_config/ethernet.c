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
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <net/if.h>
#include <sys/socket.h>

#include "connector_config.h"
#include "connector_api.h"
#include "platform.h"
#include "remote_config_cb.h"

#define ETHERNET_IPV4_STRING_LENGTH 16
#define ETHERNET_DNS_FQDN_LENGTH    128


typedef struct {
    in_addr_t ip_address;
    in_addr_t subnet;
    in_addr_t gateway;
    connector_setting_ethernet_duplex_id_t duplex;
    char dns[ETHERNET_DNS_FQDN_LENGTH];

    connector_bool_t dhcp_enabled;
} ethernet_config_data_t;

typedef struct {
    char ip_address[ETHERNET_IPV4_STRING_LENGTH];
    char subnet[ETHERNET_IPV4_STRING_LENGTH];
    char gateway[ETHERNET_IPV4_STRING_LENGTH];
    char dns[ETHERNET_DNS_FQDN_LENGTH];
    connector_setting_ethernet_duplex_id_t  duplex;
    connector_bool_t dhcp_enabled;
} ethernet_connector_data_t;

ethernet_config_data_t ethernet_config_data = {0, 0, 0, connector_setting_ethernet_duplex_auto, "\0", connector_true};

int ethernet_configuration_init(void)
{
    #define MAX_INTERFACES      5

    int             fd = -1;
    int             status=-1;
    char            *buf = malloc(MAX_INTERFACES*sizeof(struct ifreq));
    struct ifconf   conf;

    ethernet_config_data_t * const ethernet_ptr = &ethernet_config_data;

    if (buf == NULL)
    {
        APP_DEBUG("get_ip_address malloc failed\n");
        goto error;
    }

    conf.ifc_len = MAX_INTERFACES*sizeof(struct ifreq);
    conf.ifc_buf = buf;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
    {
        perror("socket");
        goto error;
    }

    if( ioctl(fd, SIOCGIFCONF , &conf) == -1)
    {
        APP_DEBUG("get_ip_address: Error using ioctl SIOCGIFCONF.\n");
        goto error;
    }

    {
        unsigned int entries = conf.ifc_len / sizeof(struct ifreq);
        unsigned int i;

        APP_DEBUG("get_ip_address: Looking for current device IP address: found [%d] entries\n", entries);

        if (entries == 0)
        {
            goto error;
        }
        for( i = 0; i < entries; i++)
        {
            struct ifreq * req = &conf.ifc_req[i];
            struct sockaddr_in * const sa = cast_for_alignment(struct sockaddr_in *, &req->ifr_addr);

            APP_DEBUG("get_ip_address: %d: Interface name [%s]\tIP Address [%s]\n", i+1, req->ifr_name, inet_ntoa(sa->sin_addr));
            if (sa->sin_addr.s_addr != htonl(INADDR_LOOPBACK))
            {
                ethernet_ptr->ip_address = sa->sin_addr.s_addr;
                status = 0;
                break;
            }
        }

        {
            struct ifreq * req = &conf.ifc_req[i];
            struct sockaddr_in * const sa = cast_for_alignment(struct sockaddr_in *, &req->ifr_addr);

            if( ioctl(fd, SIOCGIFNETMASK , req) == -1)
            {
                perror("get_ip_address: Error using ioctl SIOCGIFNETMASK");
                status = -1;
                goto error;
            }

            APP_DEBUG("get_ip_address: IP Address [%s]\n", inet_ntoa(sa->sin_addr));
            ethernet_ptr->subnet = sa->sin_addr.s_addr;
        }
    }

error:
    if (fd != -1)
    {
        close(fd);
    }
    if (buf != NULL)
    {
        free(buf);
    }
    return status;
}

void ethernet_get_ip_address(unsigned char ** addr, size_t * size)
{
    *addr = (unsigned char *) &ethernet_config_data.ip_address;
    *size = sizeof ethernet_config_data.ip_address;
}

connector_callback_status_t app_ethernet_group_init(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    void * ptr;
    ethernet_connector_data_t * ethernet_ptr = NULL;

    ASSERT(session_ptr != NULL);


    ptr = malloc(sizeof *ethernet_ptr);
    if (ptr == NULL)
    {
        remote_config->error_id = connector_global_error_memory_fail;
        goto done;
    }

    ethernet_ptr = ptr;
    inet_ntop(AF_INET, &ethernet_config_data.ip_address,    ethernet_ptr->ip_address,   sizeof ethernet_ptr->ip_address);
    inet_ntop(AF_INET, &ethernet_config_data.subnet,        ethernet_ptr->subnet,       sizeof ethernet_ptr->subnet);
    inet_ntop(AF_INET, &ethernet_config_data.gateway,       ethernet_ptr->gateway,      sizeof ethernet_ptr->gateway);

    memcpy(ethernet_ptr->dns, ethernet_config_data.dns, sizeof ethernet_ptr->dns);
    ethernet_ptr->dhcp_enabled = ethernet_config_data.dhcp_enabled;
    ethernet_ptr->duplex = ethernet_config_data.duplex;

done:
    session_ptr->group_context = ethernet_ptr;
    return status;
}

connector_callback_status_t app_ethernet_group_get(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;

    remote_group_session_t * const session_ptr = remote_config->user_context;
    ethernet_connector_data_t * ethernet_ptr;

    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    ethernet_ptr = session_ptr->group_context;

    printf("app_ethernet_group_get: id = %d\n", remote_config->element.id);

    switch (remote_config->element.id)
    {
    case connector_setting_ethernet_dhcp:
        ASSERT(remote_config->element.type == connector_element_type_boolean);
        remote_config->response.element_value->boolean_value = ethernet_ptr->dhcp_enabled;
        break;

    case connector_setting_ethernet_dns:
        ASSERT(remote_config->element.type == connector_element_type_fqdnv4);
        remote_config->response.element_value->string_value = ethernet_ptr->dns;
        break;

    case connector_setting_ethernet_duplex:
        ASSERT(remote_config->element.type == connector_element_type_enum);
        remote_config->response.element_value->enum_value = ethernet_ptr->duplex;
        break;

    case connector_setting_ethernet_ip:
    case connector_setting_ethernet_subnet:
    case connector_setting_ethernet_gateway:
    {
        char * config_data[3];

        config_data[0] = ethernet_ptr->ip_address;
        config_data[1] = ethernet_ptr->subnet;
        config_data[2] = ethernet_ptr->gateway;

        ASSERT(remote_config->element.type == connector_element_type_ipv4);
        remote_config->response.element_value->string_value = config_data[remote_config->element.id];
        break;
    }

    default:
        ASSERT(0);
        break;
    }

    return status;
}

connector_callback_status_t app_ethernet_group_set(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;

    remote_group_session_t * const session_ptr = remote_config->user_context;
    ethernet_connector_data_t * ethernet_ptr;
    size_t length;

    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);
    ASSERT(remote_config->element.value != NULL);

    ethernet_ptr = session_ptr->group_context;

    switch (remote_config->element.id)
    {
    case connector_setting_ethernet_dhcp:
        ASSERT(remote_config->element.type == connector_element_type_boolean);
        ethernet_ptr->dhcp_enabled = remote_config->element.value->boolean_value;
        break;

    case connector_setting_ethernet_dns:
        ASSERT(remote_config->element.type == connector_element_type_fqdnv4);
        length = strlen(remote_config->element.value->string_value);

        ASSERT(length < sizeof ethernet_ptr->dns);
        memcpy(ethernet_ptr->dns, remote_config->element.value->string_value, length);
        ethernet_ptr->dns[length] = '\0';
        break;

    case connector_setting_ethernet_duplex:
        ASSERT(remote_config->element.type == connector_element_type_enum);
        ethernet_ptr->duplex = (connector_setting_ethernet_duplex_id_t)remote_config->element.value->enum_value;
        break;

    case connector_setting_ethernet_ip:
    case connector_setting_ethernet_subnet:
    case connector_setting_ethernet_gateway:
    {
        struct {
            char * data;
            size_t max_length;
        } config_data[3];

        config_data[0].data = ethernet_ptr->ip_address;
        config_data[0].max_length = sizeof ethernet_ptr->ip_address;

        config_data[1].data = ethernet_ptr->subnet;
        config_data[1].max_length = sizeof ethernet_ptr->subnet;

        config_data[2].data = ethernet_ptr->gateway;
        config_data[2].max_length = sizeof ethernet_ptr->gateway;

        ASSERT(remote_config->element.type == connector_element_type_ipv4);

        length = strlen(remote_config->element.value->string_value);

        ASSERT(length <= config_data[remote_config->element.id].max_length);
        memcpy(config_data[remote_config->element.id].data, remote_config->element.value->string_value, length);
        config_data[remote_config->element.id].data[length] = '\0';
        break;
    }
    default:
        ASSERT(0);
        break;
    }

    return status;
}

connector_callback_status_t app_ethernet_group_end(connector_remote_config_t * const remote_config)
{
    connector_callback_status_t status = connector_callback_continue;
    remote_group_session_t * const session_ptr = remote_config->user_context;
    ethernet_connector_data_t * ethernet_ptr = NULL;

    /* save the data */

    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    ethernet_ptr = session_ptr->group_context;

    if (remote_config->action == connector_remote_action_set)
    {
        int ccode;

        if (inet_aton(ethernet_ptr->ip_address, (struct in_addr *)&ethernet_config_data.ip_address) == 0)
        {
            remote_config->error_id = connector_global_error_load_fail;
            remote_config->response.error_hint = "IP address";
            goto done;
        }

        if (inet_aton(ethernet_ptr->subnet, (struct in_addr *)&ethernet_config_data.subnet) == 0)
        {
            remote_config->error_id = connector_setting_ethernet_error_invalid_subnet;
            remote_config->response.error_hint = "Subnet";
            goto done;
        }

        ccode = inet_aton(ethernet_ptr->gateway, (struct in_addr *)&ethernet_config_data.gateway);
        if (ccode == 0)
        {
            remote_config->error_id = connector_setting_ethernet_error_invalid_gateway;
            remote_config->response.error_hint = "Gateway";
            goto done;
        }

        memcpy(ethernet_config_data.dns, ethernet_ptr->dns, sizeof ethernet_config_data.dns);
        ethernet_config_data.dhcp_enabled = ethernet_ptr->dhcp_enabled;
        ethernet_config_data.duplex = ethernet_ptr->duplex;

    }

done:
    if (ethernet_ptr != NULL)
    {
        free(ethernet_ptr);
    }
    return status;
}

void app_ethernet_group_cancel(connector_remote_config_cancel_t * const remote_config)
{
    remote_group_session_t * const session_ptr = remote_config->user_context;

    if (session_ptr != NULL)
    {

        free(session_ptr->group_context);
    }

}

