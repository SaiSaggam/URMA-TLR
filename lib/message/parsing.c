#include "parsing.h"
#include "messages.h"
#include "tlr_print.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "unittest_support.h"

STATIC_FUNCTION bool get_unsigned_integer_from_string(char const * const str, unsigned int * const unsigned_int)
{
    bool result;
    unsigned long value;

    if (str[0] == '-')
    {
        result = false;
        goto done;
    }
    errno = 0;
    value = strtoul(str, NULL, 10);

    if (errno == EINVAL || errno == ERANGE || value > SIZE_MAX)
    {
        result = false;
        goto done;
    }
    /*
     * If we don't clear errno once strtoul sets it, we see that the calling process will exit at
     * some later point. As I'm not certain that we've just checked all values that strtoul will set
     * errno to, I'll clear it no matter what value it might have.
     */
    if (errno != 0)
    {
        errno = 0;
    }
    *unsigned_int = (unsigned int)value;
    result = true;

done:
    return result;
}

bool get_instance_from_string(char const * const str, size_t * const instance)
{
    unsigned int value;
    bool const result = get_unsigned_integer_from_string(str, &value);

    if (result)
    {
        *instance = (size_t)value;
    }
    return result;
}

int get_interface_info(int const argc, char ** const argv, void ** const interface_info_result, size_t * const interface_info_size)
{
    int result;
    interface_info_st * interface_info = NULL;

    if (argc < 2)
    {
        tlr_debug("%s: too few args to get interface info\n", __func__);
        goto error;
    }
    interface_info = calloc(1, sizeof *interface_info);
    if (interface_info == NULL)
        goto error;

    interface_info->interface = interface_get_type_from_name(argv[0]);
    if (interface_info->interface == invalid_interface)
    {
        tlr_debug("%s: failed to get interface enum from name %s\n", __func__, argv[0]);
        goto error;
    }

    if (!get_instance_from_string(argv[1], &interface_info->instance))
    {
        tlr_debug("%s: failed to get interface instance from %s\n", __func__, argv[1]);
        goto error;
    }

    /* everything is OK if we get here */
    *interface_info_result = interface_info;
    *interface_info_size = sizeof * interface_info;
    result = 0;
    goto done;

error:
    free(interface_info);
    result = -1;
done:
    return result;
}

int get_ping_message_info(int const argc, char ** const argv, void ** const info_result, size_t * const info_size)
{
    int result;
    ping_info_st * ping_info = NULL;

    if (argc < 2)
    {
        tlr_debug("%s: too few args to get ping info\n", __func__);
        goto error;
    }
    ping_info = calloc(1, sizeof *ping_info);
    if (ping_info == NULL)
        goto error;

    ping_info->pong_destination = get_message_destination_from_monitor_name(argv[0]);
    if (ping_info->pong_destination == invalid_message_destination)
    {
        tlr_debug("%s: failed to get message destination from name %s\n", __func__, argv[0]);
        goto error;
    }

    if (!get_unsigned_integer_from_string(argv[1], &ping_info->message_id))
    {
        tlr_debug("%s: failed to get message ID from %s\n", __func__, argv[1]);
        goto error;
    }

    if (argc > 2)
    {
        char    const * const search = "time=";
        char    * str;

        if ((str = strstr(argv[2], search)) != NULL)
        {
            str += strlen(search);
            if (!get_unsigned_integer_from_string(str, &ping_info->please_respond_time))
            {
                tlr_debug("%s: failed to get response time from %s\n", __func__, str);
                goto error;
            }
        }

    }

    /* everything is OK if we get here */
    *info_result = ping_info;
    *info_size = sizeof * ping_info;
    result = 0;
    goto done;

error:
    free(ping_info);
    result = -1;
done:
    return result;
}

int get_pong_message_info(int const argc, char ** const argv, void ** const info_result, size_t * const info_size)
{
    int result;
    pong_info_st * pong_info = NULL;

    if (argc < 1)
    {
        tlr_debug("%s: too few args to get pong info\n", __func__);
        goto error;
    }
    pong_info = calloc(1, sizeof *pong_info);
    if (pong_info == NULL)
        goto error;

    if (!get_unsigned_integer_from_string(argv[0], &pong_info->message_id))
    {
        tlr_debug("%s: failed to get message ID from '%s'\n", __func__, argv[0]);
        goto error;
    }

    /* everything is OK if we get here */
    *info_result = pong_info;
    *info_size = sizeof * pong_info;
    result = 0;
    goto done;

error:
    free(pong_info);
    result = -1;
done:
    return result;
}

int get_dhcp_assigned_gateway_info(int const argc, char ** const argv, void ** const info_result, size_t * const info_size)
{
#define MAX_INTERFACE_LENGTH    sizeof("CELLULAR")
    int result;
    dhcp_assigned_gateway_st *gateway_info = NULL;
    char interface[MAX_INTERFACE_LENGTH + 1];
    unsigned int interface_index;
    char *interface_param;

    if (argc < 3)
    {
        tlr_debug("%s: too few args to get DHCP assigned gateway info\n", __func__);
        goto error;
    }
    gateway_info = malloc(sizeof *gateway_info);
    if (gateway_info == NULL)
    {
        tlr_debug("%s: unable to allocate memory for dhcp_assigned_gateway_st\n", __func__);
        goto error;
    }

    interface_param = argv[1];
    for (interface_index = 0; interface_index < MAX_INTERFACE_LENGTH; interface_index++)
    {
        if (isdigit(interface_param[interface_index]))
        {
            break;
        }
        else
        {
            interface[interface_index] = interface_param[interface_index];
        }
    }
    if (!isdigit(interface_param[interface_index]))
    {
        tlr_debug("%s: failed to parse interface from %s\n", __func__, argv[1]);
        goto error;
    }
    interface[interface_index] = '\0';
    gateway_info->interface = interface_get_type_from_name(interface);
    if (invalid_interface == gateway_info->interface)
    {
        tlr_debug("%s: failed to get interface type from %s\n", __func__, argv[1]);
        goto error;
    }
    if (!get_unsigned_integer_from_string(&interface_param[interface_index], &gateway_info->instance))
    {
        tlr_debug("%s: failed to get interface instance from %s\n", __func__, argv[1]);
        goto error;
    }
    if (strlen(argv[2]) < sizeof(gateway_info->gateway))
    {
        strcpy(gateway_info->gateway, argv[2]);
    }
    else
    {
        tlr_debug("%s: gateway %s is too long\n", __func__, argv[1]);
        goto error;
    }

    *info_size = sizeof *gateway_info;
    *info_result = gateway_info;
    result = 0;
    goto done;

    error:
    free(gateway_info);
    result = -1;

done:
    return result;
}