#include "messages.h"
#include "messages_private.h"
#include "tlr_socket.h"
#include "args.h"
#include "parsing.h"
#include <create_worker_thread.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdbool.h>
#include "tlr_print.h"
#include "tlr_common.h"
#include "getlinefd.h"
#include "unittest_support.h"
#include <semaphore.h>

#define GET_MESSAGE_TIMEOUT_SECONDS 5
#define WORKER_THREAD_CREATE_TRIES    3
#define WORKER_THREAD_RETRY_DELAY_MILLISECONDS   1

struct message_thread_st
{
	pthread_t thread_id;
};

struct message_thread_info_st
{
	char const * socket_name;
	message_callback_fn message_callback;
    sem_t confirmation_sem;
};
typedef struct message_thread_info_st message_thread_info_st;

typedef struct socket_mappings_st
{
	char const * const monitor_name;
	char const * const socket_name;
} socket_mappings_st;

static socket_mappings_st const socket_mappings[] =
{
    [ethernet_monitor] =
	{
		.monitor_name = "ethmon",
		.socket_name = "ETH_MONITOR_MESSAGING_SOCKET"
	},
    [dsl_monitor] =
	{
		.monitor_name = "dslmon",
		.socket_name = "DSL_MONITOR_MESSAGING_SOCKET"
	},
    [cellular_monitor] =
	{
		.monitor_name = "cellmon",
		.socket_name = "CELL_MONITOR_MESSAGING_SOCKET"
	},
    [vpn_monitor] =
    {
        .monitor_name = "vpnmon",
        .socket_name = "VPN_MONITOR_MESSAGING_SOCKET"
    },
    [interface_manager] =
	{
		.monitor_name = "ifman",
		.socket_name = "IFACE_MGR_MESSAGING_SOCKET"
	},
    [bridge_monitor] =
    {
        .monitor_name = "bridgemon",
        .socket_name = "BRIDGE_MONITOR_MESSAGING_SOCKET"
    },
    [wifi_monitor] =
    {
        .monitor_name = "wifimon",
        .socket_name = "WIFI_MONITOR_MESSAGING_SOCKET"
    },
    [wifi5g_monitor] =
    {
        .monitor_name = "wifi5gmon",
        .socket_name = "WIFI5G_MONITOR_MESSAGING_SOCKET"
    },

    [message_test_app] =
    {
        .monitor_name = "msgtest",
        .socket_name = "MESSAGE_TEST_SOCKET"
    }
};
#define NUM_SOCKET_NAMES ((sizeof socket_mappings)/(sizeof socket_mappings[0]))

message_destination_t get_message_destination_from_monitor_name(char const * const monitor_name)
{
	message_destination_t destination = invalid_message_destination;
    int index;

	for (index = 0; index < NUM_SOCKET_NAMES; index++)
	{
		if (strcmp(monitor_name, socket_mappings[index].monitor_name) == 0)
		{
            destination = index;
			break;
		}
	}

	return destination;
}

// get_monitor_names_string returns a string containing a list of all the monitor names.
// This string needs to be freed afterwards.
char * get_monitor_names_string(void)
{
    char    * str = NULL;
    char    const * name;
    int     index;
    int     cur_allocsize = 0, cur_size = 0, name_len;
    int     first = 1, newlinecnt = 0, donl=0;

    for (index = 0; index < NUM_SOCKET_NAMES; index++)
    {
        name = socket_mappings[index].monitor_name;
        name_len = strlen(name);
        if (strcmp("msgtest", name) == 0)
        {
            // Don't worry about msgtest...
            continue;
        }
        while (cur_allocsize < (cur_size + name_len + 3))
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


char const * get_monitor_name_from_message_destination(message_destination_t const destination)
{
    char const * monitor_name = NULL;

	if (destination < NUM_SOCKET_NAMES)
	{
        monitor_name = socket_mappings[destination].monitor_name;
	}

	return monitor_name;
}

static char const * get_socket_name_from_monitor_name(char const * const monitor_name)
{
	char const * socket_name = NULL;
	int index;

	for (index = 0; index < NUM_SOCKET_NAMES; index++)
	{
		if (strcmp(monitor_name, socket_mappings[index].monitor_name) == 0)
		{
			socket_name = socket_mappings[index].socket_name;
			break;
		}
	}

	return socket_name;
}

static char const * get_socket_name_from_destination(message_destination_t const destination)
{
    char const * socket_name = NULL;

    if (destination < NUM_SOCKET_NAMES)
    {
        socket_name = socket_mappings[destination].socket_name;
    }

    return socket_name;
}

static connection_handle * open_connection_to_socket(char const * const socket_name)
{
	connection_handle 	* conn;
	int 	sock;

	if ((conn = malloc(sizeof *conn)) == NULL)
	{
        tlr_debug("messages: open_connection_to_socket() failed, socket_name %s\n", socket_name);
		goto done;
	}
	if ((sock = connect_to_socket(socket_name, true)) < 0)
	{
        tlr_debug("messages: connect_to_socket() failed, socket_name %s\n", socket_name);
		free(conn);
		conn = NULL;
		goto done;
	}
	conn->sock = sock;
done:
	return conn;

}

static bool parse_message_info(message_codes_t const message_code, int const argc, char ** const argv, void ** const info, size_t * const info_size)
{
    bool result = false;

    *info = NULL;
    *info_size = 0;

    switch (message_code)
    {
        case update_message:
            /* the update message may optionally include config group details */
            if (argc < 2)
            {
                result = true;
                break;
            }
            if (get_update_info(argc, argv, info, info_size) == 0)
                result = true;
            break;
        case query_connect:
        case joined_bridge_message:
        case left_bridge_message:
            if (argc < 2)
            {
                result = true;
                break;
            }
            /* fall thru as these messages can also optionally include interface details */
        case connected_message:
        case disconnected_message:
            /* these messages require an interface and instance */
            if (get_interface_info(argc, argv, info, info_size) == 0)
                result = true;
            break;
        case ping_message:
            if (get_ping_message_info(argc, argv, info, info_size) == 0)
                result = true;
            break;
        case pong_message:
            if (get_pong_message_info(argc, argv, info, info_size) == 0)
                result = true;
            break;
        case assigned_to_wan_message:
            /* fall through is intended */
        case removed_from_wan_message:
            if (get_wan_message_info(argc, argv, info, info_size) == 0)
                result = true;
            break;
        case dhcp_assigned_gateway:
            if (get_dhcp_assigned_gateway_info(argc, argv, info, info_size) == 0)
                result = true;
            break;
        default:
            /* nothing to do for these other messages */
            result = true;
            break;
    }

    return result;
}

STATIC_FUNCTION void parse_message(int const fd, char * const line, message_handler_info_st const * const info)
{
	char * token;
	char * save_ptr;
    char * linebuf = line;
	char * response = "OK";
    args_st * args = malloc_args_st(1);
    char buf[200];

    if (args == NULL)
    {
        response = "ERROR: memory shortage";
        goto done;
    }
    while ((token = strtok_r(linebuf, " ", &save_ptr)) != NULL)
    {
        add_arg(args, token);
        linebuf = NULL;
    }

    if (args->argc >= 2)    /* there should always be at least two args. 1st for instance, second for message code */
    {
        size_t instance;
        message_codes_t message_code;
        void * additional_info = NULL;
        size_t additional_info_size = 0;

        if (!get_instance_from_string(args->argv[0], &instance))
        {
            snprintf(buf, sizeof buf, "ERROR: invalid instance number (%s)", args->argv[0]);
            response = buf;
            goto done;
        }

        /* note that further validation of the instance value will be required by the application */
        message_code = get_global_message_code_from_name(args->argv[1]);
        if (message_code == invalid_message_code)
        {
            response = "ERROR: unknown message";
            goto done;
        }
        /* depending on the message code there may be more information in the command */
        if (!parse_message_info(message_code, args->argc - 2, &args->argv[2], &additional_info, &additional_info_size))
        {
            response = "ERROR: invalid message info";
            goto done;
        }

        /* now deliver this to the application */
        if (info->message_callback(fd, instance, message_code, &additional_info, additional_info_size) == -1)
        {
            response = "ERROR: unable to deliver message";
        }
        /* if the callback hasn't claimed the memory, we free it here */
        if (additional_info != NULL)
        {
            free(additional_info);
        }
    }
    else
    {
        response = "ERROR: instance and code fields missing";
    }
done:
    dprintf(fd, "%s\n", response);

    free_args_st(args);
}

static void * message_thread(void * const data)
{
	message_handler_info_st * const info = data;
	int fd = info->sock;
	int rval;
	char * line = NULL;
	size_t len = 0;
	bool blank_line = false;

    do
	{
        struct timeval timeout;

        timeout.tv_sec = GET_MESSAGE_TIMEOUT_SECONDS;
        timeout.tv_usec = 0;

        rval = getlinefd_timeout(&line, &len, fd, &timeout);
		if (rval > 0)
		{
            char * const last_char = line + rval - 1;

			if (*last_char == '\n')
				*last_char = '\0';

			blank_line = (line[0] == '\0');
            if (!blank_line)
            {
                parse_message(fd, line, info);
            }
		}
	}
	while (rval > 0 && !blank_line);

    free(data);
    free(line);
	close(fd);

    pthread_exit(NULL);

}

static void wait_for_messages(char const * const socket_name, message_callback_fn message_callback, sem_t * const confirmation_semaphore)
{
	int sock = listen_on_socket(socket_name, true);

    /* Can let the parent thread know that we're listening on the socket now.
     */
    sem_post(confirmation_semaphore);

    if (sock < 0)
    {
        goto done;
    }

	for (;;)  // XXX when to break out?
	{
		int msgsock;
		pthread_t thread_id;
		message_handler_info_st * info;
        int create_worker_thread_result;

		msgsock = accept(sock, 0, 0);
        if (msgsock == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            break;  // TODO: what's the best thing to do on error?
            /*
             * Shutting down the socket will result in the process not processing any new global
             * messages. If we ever create an over-arching monitor process, it will detect that this
             * process has become unresponsive, and can restart it.
            */
        }
		/* process each socket in its own thread so as not to block other connections */
		info = malloc(sizeof *info);
		if (info == NULL)
		{
            close(msgsock);
			perror("message thread");
            goto done;
		}
		info->sock = msgsock;
		info->message_callback = message_callback;

        create_worker_thread_result = create_worker_thread(&thread_id,
                                                           true,
                                                           WORKER_THREAD_CREATE_TRIES,
                                                           WORKER_THREAD_RETRY_DELAY_MILLISECONDS,
                                                           message_thread,
                                                           info);

        /*
         * If the thread wasn't created we need to close the socket here so that the client side
         * isn't left hanging.
         */
        if (create_worker_thread_result != 0)
        {
            free(info);
            close(msgsock);
            tlr_debug("failed to create message handling thread\n");
        }

    }
	tlr_debug("messaging thread ending\n");

done:
    if (sock >= 0)
    {
    	close(sock);
    }
	unlink(socket_name);

    return;
}

static void * main_message_thread(void * data)
{
	message_thread_info_st * info = data;

	/* get down to the business of waiting for new messages */
	wait_for_messages(info->socket_name, info->message_callback, &info->confirmation_sem);

	free(data);
	pthread_exit(NULL);
}

message_thread_st * open_messaging_context(message_destination_t const destination, message_callback_fn const message_callback)
{
	int rc;
	struct message_thread_st * messaging_context = NULL;
	struct message_thread_info_st * info = NULL;
    char const * socket_name;

    socket_name = get_socket_name_from_destination(destination);
    if (socket_name == NULL)
    {
        goto error;
    }
    unlink(socket_name);

    if ((messaging_context = malloc(sizeof *messaging_context)) == NULL)
    {
		goto error;
    }

    if ((info = malloc(sizeof *info)) == NULL)
    {
        goto error;
    }

	info->socket_name = socket_name;
	info->message_callback = message_callback;
    sem_init(&info->confirmation_sem, 0, 0);

	rc = pthread_create(&messaging_context->thread_id, NULL, main_message_thread, info);

    if (rc != 0)
    {
		goto error;
    }

    /* Wait for the messaging thread to indicate that it's listening for messages. */
    sem_wait(&info->confirmation_sem);

	goto done;

error:
    free(info);
    free(messaging_context);
    messaging_context = NULL;

done:
	return messaging_context;

}

// TODO: need a matching close_messaging_context() function

connection_handle * open_connection(char const * const monitor_name)
{
    connection_handle * conn = NULL;
    char const * const socket_name = get_socket_name_from_monitor_name(monitor_name);

    if (socket_name == NULL)
    {
        tlr_debug("unknown socket name for monitor %s\n", monitor_name);
        goto done;
    }
    conn = open_connection_to_socket(socket_name);
done:
    return conn;
}

int transact_with_connection(connection_handle const * const conn, char const * const str, char ** const response_buffer)
{
	size_t len = 0;
	int rval;
	int return_val;
	struct timeval timeout;

	if (*str == '\0' || *str == '\n')
		return 0;   // We won't bother with empty strings.. but will just return OK so keep going.
	if (response_buffer == NULL)
	{   // Must have a pointer to response buffer -- mandatory for this call.
		return_val = -1;
		goto done;
	}

        while ( 1 )
        {
            int sts;
            // Send the message over the connection
            sts=send(conn->sock, str, strlen(str), 0);
            if ( sts < 0 && EINTR == errno ) continue;
            if ( sts < 0 )
            {
	      tlr_debug("messages: send() failed with errno = %d",errno);
	      return_val = -1;
	      goto done;
            }
            break;
        }

	timeout.tv_sec = GET_MESSAGE_TIMEOUT_SECONDS;
	timeout.tv_usec = 0;
	rval = getlinefd_timeout(response_buffer, &len, conn->sock, &timeout);
	if (rval <= 0)
	{
		return_val = -1;
		goto done;
	}
	{
		if (strlen(*response_buffer) == 0)
		{
			return_val = -1;
			goto done;
		}

		return_val = 0;
	}

done:

	return return_val;
}



void close_connection(connection_handle const * const conn)
{
	if (conn != NULL)
	{
		close(conn->sock);
		free((void *)conn);
	}
}

/*
 * Sends a message to the given monitor. Opens, transacts the message and closes the connection.
 * This can be used if caller isn't particularly interested in keeping the connection open
 * Returns 0 if all ok.  If a response_buffer is provided (NON NULL). Then a pointer to a malloced string
 * representing the response is provided.  This will need to be freed after it has been used.
*/
int send_message_to_socket(char const * const socket_name, size_t instance, message_codes_t const message_code,
				 char const * const additional_info, char ** const response_buffer)
{
	connection_handle * conn = NULL;
	int 	retval;
	char 	* response = NULL;
	char * sendbuf = NULL;
	char const * message_code_str;
	int temp;

	message_code_str = get_global_message_name_from_code(message_code);
	if (message_code_str == NULL)
	{
		tlr_debug("messages: send_message() failed: message_code_str == NULL \n");
		retval = -1;
		goto done;
	}

    temp = asprintf(&sendbuf, "%zd %s %s\n", instance, message_code_str, additional_info != NULL ? additional_info : "");
    if (temp == -1 || sendbuf == NULL)
    {
        tlr_debug("messages: send_message() failed: temp == -1 || sendbuf == NULL \n");
        retval = -1;
        goto done;
    }

    if ((conn = open_connection_to_socket(socket_name)) == NULL)
	{
		tlr_debug("messages: send_message() failed: open_connection_to_destination() failed\n");
		retval = -1;
		goto done;
	}

	if (transact_with_connection(conn, sendbuf, &response) != 0 || response == NULL)
	{
		tlr_debug("messages: send_message() failed: transact_with_connection() failed \n");
		retval = -1;
		goto done;
	}

	// check response ends with "OK"
	retval =  (strstr(response, "OK\n") == NULL) ? -1 : 0;

	// check if user want the response back..
	if (response_buffer != NULL)
	{
		*response_buffer = response;
		response = NULL;
	}

	if (retval == -1)
		tlr_debug("messages: send_message() failed: response is not OK \n");

done:
	if (sendbuf != NULL)
		free(sendbuf);
	if (response != NULL)
		free(response);
	if (conn != NULL)
		close_connection(conn);
	return retval;
}

/*
 * Sends a message to the given monitor. Opens, transacts the message and closes the connection.
 * This can be used if caller isn't particularly interested in keeping the connection open
 * Returns 0 if all ok.
*/
int send_message(message_destination_t const destination, size_t instance, message_codes_t const message_code,
				 char const * const additional_info, char ** const response_buffer)
{
    char const * const socket_name = get_socket_name_from_destination(destination);
    int result;

    if (socket_name == NULL)
    {
        tlr_debug("messages: send_message() failed: socket_name == NULL \n");
        result = -1;
        goto done;
    }
    result = send_message_to_socket(socket_name, instance, message_code, additional_info, response_buffer);

done:
    return result;
}

/*
 * given an interface type, return the destination for messages relating to that interface
 * so for ethernet messages, we send to the ethernet monitor, etc.
 * For unknown/unsupported interfaces, we return invalid_message_destination
*/
static message_destination_t get_message_destination_from_interface_type(interfaces_t const interface_type)
{
    message_destination_t destination;

    switch (interface_type)
    {
        case cellular_interface:
            destination = cellular_monitor;
            break;
        case eth_interface:
            destination = ethernet_monitor;
            break;
        case dsl_interface:
            destination = dsl_monitor;
            break;
        case vpn_interface:
            destination = vpn_monitor;
            break;
        case bridge_interface:
            destination = bridge_monitor;
            break;
        case wifi_interface:
            destination = wifi_monitor;
            break;
        case wifi5g_interface:
            destination = wifi5g_monitor;
            break;
        default:
            destination = invalid_message_destination;
            break;
    }

    return destination;
}

int send_message_to_interface_monitor(interfaces_t interface_type, size_t instance, message_codes_t const message_code, char const * const str)
{
    int result;
    message_destination_t destination;

    destination = get_message_destination_from_interface_type(interface_type);
    if (destination == invalid_message_destination)
    {
        result = -1;
        goto done;
    }
    result = send_message(destination, instance, message_code, str, NULL);

done:
    return result;
}

int send_pong_message(message_destination_t const destination, unsigned int message_id)
{
    char buf[40];

    // XXX pong messages are currently always sent to instance 0
    snprintf(buf, sizeof buf, "%u", message_id);
    return send_message(destination, 0, pong_message, buf, NULL);
}

