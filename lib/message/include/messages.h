#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include "message_codes.h"
#include "interfaces.h"
#include "update_message.h"
#include <stdio.h>

struct message_thread_st;
typedef struct message_thread_st message_thread_st;
/*
 * message_callback_fn:
 * should return -1 on error, 0 on success
 * called whenever a message is read off a messaging context's queue
 * MUST free info if it is non-NULL and not returning -1.
 * If -1 is returned, the caller will free info
 */
typedef int (*message_callback_fn)(int fd, size_t const instance, message_codes_t const message_code, void * * const info, size_t const info_size);

typedef enum message_destination_t
{
    ethernet_monitor,
    dsl_monitor,
    cellular_monitor,
    vpn_monitor,
    interface_manager,
    bridge_monitor,
    wifi_monitor,
    wifi5g_monitor,
    message_test_app,
    invalid_message_destination
} message_destination_t;

message_thread_st * open_messaging_context(message_destination_t const destination, message_callback_fn const message_callback);

// API regarding outgoing connections; i.e. from the point of view of a client.

struct connection_handle
{
	int	sock;
};
typedef struct connection_handle  connection_handle;

message_destination_t get_message_destination_from_monitor_name(char const * const monitor_name);
char const * get_monitor_name_from_message_destination(message_destination_t const destination);
char * get_monitor_names_string(void);
connection_handle * open_connection(char const * const monitor_name);
int transact_with_connection(connection_handle const * const conn, char const * const str, char ** const response_buffer);
void close_connection(connection_handle const * const conn);
int send_message(message_destination_t const destination, size_t instance, message_codes_t const message_code, char const * const str, char ** const response_buffer);
int send_message_to_interface_monitor(interfaces_t interface_type, size_t instance, message_codes_t const message_code, char const * const str);
int send_pong_message(message_destination_t const destination, unsigned int message_id);

#endif /* __MESSAGES_H__ */
