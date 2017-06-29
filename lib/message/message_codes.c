#include "message_codes.h"
#include <string.h>
#include <stdlib.h>
#include "tlr_common.h"

struct message_code_mappings_st
{
    int code;
    char const * const name;
};

typedef struct message_code_mappings_st message_code_mappings_st;

static const message_code_mappings_st message_code_mappings[] =
{
    {
        .code = connect_message,
        .name = "connect"
    },
    {
        .code = disconnect_message,
        .name = "disconnect"
    },
    {
        .code = connected_message,
        .name = "connected"
    },
    {
        .code = disconnected_message,
        .name = "disconnected"
    },
    {
        .code = update_message,
        .name = "update"
    },
    {
        .code = ping_message,
        .name = "ping"
    },
    {
        .code = pong_message,
        .name = "pong"
    },
    {
        .code = joined_bridge_message,
        .name = "joined_bridge"
    },
    {
        .code = left_bridge_message,
        .name = "left_bridge"
    },
	{
        .code = query_connect,
        .name = "queryconnect"
	},
	{
        .code = assigned_to_wan_message,
        .name = "assigned_wan"
	},
	{
        .code = removed_from_wan_message,
        .name = "removed_wan"
	},
    {
        .code = dhcp_assigned_gateway,
        .name = "dhcp_assigned_gateway"
    }
};


// TODO: if we ever get more than a few of these codes it might be worth using a different lookup method.

int get_global_message_code_from_name(char const * const name)
{
    int i;
    int message_code = invalid_message_code;

    for (i = 0; i < ARRAY_SIZE(message_code_mappings); i++)
    {
        if (strcmp(name, message_code_mappings[i].name) == 0)
        {
            message_code = message_code_mappings[i].code;
            break;
        }
    }

    return message_code;
}

char const * get_global_message_name_from_code(int code)
{
    int i;
    char const * message_name = NULL;

    for (i = 0; i < ARRAY_SIZE(message_code_mappings); i++)
    {
        if (code == message_code_mappings[i].code)
        {
            message_name = message_code_mappings[i].name;
            break;
        }
    }

    return message_name;
}


// get_global_msgs_string returns a string containing a list of all the monitor names.
// This string needs to be freed afterwards.
char * get_global_msgs_string(void)
{
    char    * str = NULL;
    char    const * name;
    int     i;
    int     cur_allocsize = 0, cur_size = 0, name_len;
    int     first = 1, newlinecnt = 0, donl=0;

    for (i = 0; i < ARRAY_SIZE(message_code_mappings); i++)
    {
        name = message_code_mappings[i].name;
        name_len = strlen(name);
        while (cur_allocsize < (cur_size + name_len + 4))
        {
            cur_allocsize += 80;
            str = realloc(str, cur_allocsize);
            if (str == NULL)
                goto error;
        }
        if (first)
        {
            *str = 0;
            first = 0;
        }
        else
        {
            if (donl)
            {
                strcat(str, "\n");
                donl = 0;
            }
            strcat(str, ", ");
        }
        strcat(str, name);
        cur_size = strlen(str);
        newlinecnt++;
        if (newlinecnt >= 5)
        {
            donl = 1;
            newlinecnt = 0;
        }

    }
error:
    return str;

}

