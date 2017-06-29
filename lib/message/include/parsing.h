#ifndef __PARSING_H__
#define __PARSING_H__

#include "interfaces.h"
#include "message_codes.h"
#include "messages.h"
#include "update_message.h"
#include <stddef.h>
#include <stdbool.h>
#include <ip_helper.h>


/* one of these structures is passed along with assigned/removed from wan message codes */
struct wan_info_st
{
    size_t wan_instance;
};
typedef struct wan_info_st wan_info_st;

/* one of these structures is passed along with connected/disconnected message codes */
struct interface_info_st
{
    interfaces_t interface;
    size_t instance;
};
typedef struct interface_info_st interface_info_st;

/* one of these structures is passed along with ping message codes */
struct ping_info_st
{
    message_destination_t pong_destination; /* desired destination of the pong response message */
    unsigned int message_id;                /* unique ID for this message. Included in the pong response message */
    unsigned int please_respond_time;
};
typedef struct ping_info_st ping_info_st;

/* one of these structures is passed along with pong message codes */
struct pong_info_st
{
    unsigned int message_id;                /* ID copied over from ping message */
};
typedef struct pong_info_st pong_info_st;

struct dhcp_assigned_gateway_t
{
    interfaces_t interface;
    unsigned int instance;
    char gateway[IP_ADDRESS_SIZE];
};
typedef struct dhcp_assigned_gateway_t dhcp_assigned_gateway_st;

bool get_instance_from_string(char const * const str, size_t * const instance);
int get_interface_info(int const argc, char ** const argv, void ** const interface_info_result, size_t * const interface_info_size);
int get_ping_message_info(int const argc, char ** const argv, void ** const info_result, size_t * const info_size);
int get_pong_message_info(int const argc, char ** const argv, void ** const info_result, size_t * const info_size);
int get_wan_message_info(int const argc, char ** const argv, void ** const info_result, size_t * const info_size);
int get_dhcp_assigned_gateway_info(int const argc, char ** const argv, void ** const info_result, size_t * const info_size);

#endif /* __PARSING_H__ */
