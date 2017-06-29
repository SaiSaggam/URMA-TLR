#ifndef __INTERFACE_MAC_ADDRESSES_PRIVATE_H__
#define __INTERFACE_MAC_ADDRESSES_PRIVATE_H__

struct interface_mac_address_st
{
    char const * base_interface_name;
    unsigned int num_interfaces;
    char * * mac_addresses;
};


#endif /* __INTERFACE_MAC_ADDRESSES_PRIVATE_H__ */
