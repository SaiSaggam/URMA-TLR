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
#ifndef __INTERFACE_MAC_ADDRESSES_H__
#define __INTERFACE_MAC_ADDRESSES_H__

#include <config_mgr.h>
#include <stdio.h>

typedef struct interface_mac_address_st interface_mac_address_st;

/**
 * get_interface_mac_addresses
 *
 * Gets all MAC addresses for the specified config group.
 *
 * @param config_group - The config group we're trying to get MAC addresses from
 *
 * @return interface_mac_address_st * - Pass this to the other API functions. Should be freed by a
 *         call to interface_mac_addresses_free() when finished with.
 */
interface_mac_address_st * interface_mac_addresses_alloc(config_group_t const config_group);

/**
 *
 *  interface_mac_addresses_free
 *
 *  Frees resources obtained using get_interface_mac_addresses.
 *
 * @param interface_mac_addresses - The MAC address object to be freed
 */
void interface_mac_addresses_free(interface_mac_address_st * const interface_mac_addresses);

/**
 * write_interface_mac_addresses
 *
 * Write all the MAC addresses relating to mac_addresses to the file pointer FP in the format
 * expected for the 'show manufacture' command.
 *
 * @param fp - The file to write the output to
 * @param mac_addresses - The MAC addresses for a particualr interface type obtained using
 *                      get_interface_mac_addresses().
 * @param label_to_print - Pointer to the initial label to print. Updated to point to variable
 *                       'empty_label' once the first MAC address has been printed.
 * @param empty_label - The label to use once the first MAC address has been printed.
 * @param interface_label - The label we want to use for the interface name (e.g. for ETH, use label
 *                        "LAN").
 * @param sub_label - Some interface types (e.g. Wi-FI 2.4G) need an additional sub-label to the
 *                  right of the interface name.
*/
void write_interface_mac_addresses(FILE * const fp,
                                   interface_mac_address_st const * const mac_addresses,
                                   char const * * const label_to_print,
                                   char const * const empty_label,
                                   char const * const interface_label,
                                   char const * const sub_label);

/**
 * get_mac_address_from_mac_addresses
 *
 * Get a single MAC address from the mac_addresses.
 *
 * @param mac_addresses - The set of MAC addresses to get the MAC address from.
 * @param instance  - The interface instance (not index) to get the MAC address for.
 *
 * @return char const* - The MAC address in question as a string.
 */
char const * get_mac_address_from_mac_addresses(interface_mac_address_st const * const mac_addresses,
                                                unsigned int const instance);

#endif /* __INTERFACE_MAC_ADDRESSES_H__ */
