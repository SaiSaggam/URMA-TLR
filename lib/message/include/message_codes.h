#ifndef __MESSAGE_CODES_H__
#define __MESSAGE_CODES_H__

/* we reserve a range of values for private use by the application */
#define INVALID_MESSAGE_CODE -1
#define NUM_PRIVATE_MESSAGES 1000

typedef enum message_codes_t
{
    invalid_message_code = INVALID_MESSAGE_CODE,
    first_private_message,
    last_private_message = first_private_message + NUM_PRIVATE_MESSAGES,
    first_global_message,

    connect_message = first_global_message,
    disconnect_message,
    connected_message,
    disconnected_message,
    update_message,
    ping_message,
    pong_message,
    joined_bridge_message,
    left_bridge_message,
    query_connect,
    assigned_to_wan_message,
    removed_from_wan_message,
    dhcp_assigned_gateway
} message_codes_t;

int get_global_message_code_from_name(char const * const name);
char const * get_global_message_name_from_code(int code);
char * get_global_msgs_string(void);

#endif /* __MESSAGE_CODES_H__ */
