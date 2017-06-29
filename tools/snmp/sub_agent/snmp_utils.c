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
#include "snmp_agent_debug.h"

#include "snmp_utils.h"
#include "config_table.h"
#include "config_def.h"
#include "ip_helper.h"
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

bool is_a_table(config_mgr_group_t const * const pgroup)
{
    return pgroup->mib_indexed;
}

static bool ip_address_is_assigned(void ** const buffer)
{
    return strncmp(*buffer, NOT_ASSIGNED_STRING, sizeof NOT_ASSIGNED_STRING);
}

/* Given a string containing an IP (IPv4) address in *buffer, convert this into a 4 byte value */
static int convert_ip_address_string(size_t * const element_size, void * * const buffer)
{
    struct sockaddr_in ip_addr;
    int ret = -1;
    size_t ip_address_size;

    memset(&ip_addr, 0, sizeof(ip_addr)); // default to a 0.0.0.0 address
    ip_address_size = *element_size = sizeof(ip_addr.sin_addr);

    if ((strlen((char *)*buffer) > 0) && ip_address_is_assigned(buffer))
    {
        if (inet_aton((char *)*buffer, &ip_addr.sin_addr) == 0)
        {
            snmp_debug("Could not convert IP address string [%s]\n", (char *)*buffer);
        }
    }

    free(*buffer);
    *buffer = malloc(ip_address_size);
    if (*buffer == NULL)
    {
        goto error;
    }
    memcpy(*buffer, &ip_addr.sin_addr, ip_address_size);
    ret = 0;

error:
    return ret;
}

int convert_ip_address_to_string_from_binary(char const * const binary_ip_address, char * const ip_address_string_buffer,
                                             size_t const str_len)
{
    int ret = -1;
    struct sockaddr_in ipv4_addr;

    memset(&ipv4_addr, 0, sizeof ipv4_addr);
    memcpy(&ipv4_addr.sin_addr, binary_ip_address, sizeof ipv4_addr.sin_addr);

    if (inet_ntop(AF_INET, &ipv4_addr.sin_addr, ip_address_string_buffer, str_len) == NULL)
    {
        snmp_debug("could not convert IP address for SET request\n");
        goto error;
    }

    ret = 0;

error:
    return ret;
}

static uint64_t swap_words(uint64_t val)
{
    uint64_t result;

    result = ((val & 0xffffffff00000000ULL) >> 32)
        | ((val & 0x00000000ffffffffULL) << 32);

    return result;
}

/*
 *  Some element values need to be modified in some way before they are returned for delivery by
 *  SNMP.
 *  This function will make the necessary adjustments to the value and/or size of the element.
*/
int snmp_adjust_element(u_char const asn_type, size_t * const element_size, void * * const buffer)
{
	int ret = -1;

	switch (asn_type)
	{
		case ASN_OCTET_STR:
			*element_size = strlen((char *)*buffer);
			break;

		case ASN_IPADDRESS:
			if (convert_ip_address_string(element_size, buffer) < 0)
			{
				goto error;
			}
			break;
		case ASN_INTEGER:
		case ASN_BOOLEAN:
		case ASN_UNSIGNED:
        case ASN_COUNTER:
            /*
             *  XXX Not entirely certain if this should be sizeof(int) or sizeof(uint32_t).
             *  They're the same value on the target.
             */
			*element_size = sizeof(uint32_t);
			break;
        case ASN_COUNTER64:
        {
            *element_size = sizeof(uint64_t);
            /* unit tests sometimes pass in NULL, so we need to check for it here */
            if (buffer != NULL && *buffer != NULL)
            {
                uint64_t * const value = *buffer;
                *value = swap_words(*value);
            }
            break;
        }

		default:
			snmp_error("snmp_adjust_element: unrecongnized ASN type [%d]\n", asn_type);
			goto error;
	}

    ret = 0;
error:
    return ret;
}


u_char snmp_convert_tlr_type_to_asn_type(config_mgr_element_type_t const element_type)
{
    u_char asn_type;

    switch (element_type)
    {
        case config_mgr_element_type_string:
        case config_mgr_element_type_password:
        case config_mgr_element_type_encpassword:
        case config_mgr_element_type_fqdn:
        case config_mgr_element_type_mac_addr:
        case config_mgr_element_type_interface:  //TODO: is it string or int? used in cloud
        case config_mgr_element_type_enum_multi:
            asn_type = ASN_OCTET_STR;
            break;

        case config_mgr_element_type_uint32:
            asn_type = ASN_UNSIGNED;
            break;

        case config_mgr_element_type_int32:
        case config_mgr_element_type_enum:
        case config_mgr_element_type_on_off:
        case config_mgr_element_type_boolean:
        case config_mgr_element_type_index:
            asn_type = ASN_INTEGER;
            break;

        case config_mgr_element_type_ipaddr:
            asn_type = ASN_IPADDRESS;
            break;

        case config_mgr_element_type_counter32:
            asn_type = ASN_COUNTER;
            break;

        case config_mgr_element_type_counter64:
            asn_type = ASN_COUNTER64;
            break;

        case config_mgr_element_type_none:
            // XXX eh? Why assign to this value? What to do in default and 'none' cases?
            asn_type = (u_char)MAX_SUBID;
            break;

        default:
            snmp_debug("get_asn_type: unrecognized element type %d\n", element_type);
            asn_type = (u_char)MAX_SUBID;
            break;
    }

    return asn_type;
}




