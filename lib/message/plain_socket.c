#include "plain_socket.h"
#include "tlr_socket.h"
#include <create_worker_thread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "tlr_print.h"
#include "tlr_common.h"

#define WORKER_THREAD_CREATE_TRIES    3
#define WORKER_THREAD_RETRY_DELAY_MILLISECONDS   1

struct plain_socket_context_st
{
    char const * socket_name;
    bool use_abstract_namespace;
	pthread_t thread_id;
    plain_socket_callback_fn callback;
    void const * user_context;

};

struct plain_socket_info_st
{
    int socket_fd;
    plain_socket_callback_fn callback;
    void const * user_context;
};
typedef struct plain_socket_info_st plain_socket_info_st;

static void * plain_socket_worker_thread(void * data)
{
    plain_socket_info_st * plain_socket_info = data;

    /* Call the user callback. It's up to the user callback to close the socket when he's finished
     * with it. The user callback should NOT exit the thread as this function needs to clean up
     * resources.
     */
    plain_socket_info->callback(plain_socket_info->socket_fd, (void *)plain_socket_info->user_context);

    free(plain_socket_info);
	pthread_exit(NULL);
}

static void wait_for_new_plain_connections(plain_socket_context_st * const plain_socket_context)
{
	int sock = listen_on_socket(plain_socket_context->socket_name, plain_socket_context->use_abstract_namespace);

    if (sock < 0)
    {
        goto done;
    }

	for (;;)  // XXX when to break out?
	{
		int socket_fd;
		pthread_t thread_id;
		plain_socket_info_st * info;
        int create_worker_thread_result;

		socket_fd = accept(sock, 0, 0);
        if (socket_fd == -1)
        {
			perror("accept");
        }
		/* process each socket in its own thread so as not to block other connections */
		info = malloc(sizeof *info);
		if (info == NULL)
		{
            close(socket_fd);
			perror("plain socket thread");
            goto done;
		}
		info->socket_fd = socket_fd;
		info->callback = plain_socket_context->callback;
        info->user_context = plain_socket_context->user_context;

        create_worker_thread_result = create_worker_thread(&thread_id,
                                                           true,
                                                           WORKER_THREAD_CREATE_TRIES,
                                                           WORKER_THREAD_RETRY_DELAY_MILLISECONDS,
                                                           plain_socket_worker_thread,
                                                           info);
        /*
         * If the thread wasn't created we need to close the socket here so that the client side
         * isn't left hanging.
         */
        if (create_worker_thread_result != 0)
        {
            free(info);
            close(socket_fd);
            tlr_debug("failed to create worker thread\n");
        }

    }

done:
    if (sock >= 0)
    {
    	close(sock);
    }
	unlink(plain_socket_context->socket_name);

    return;
}

static void * plain_socket_main_worker_thread(void * data)
{
    plain_socket_context_st * plain_socket_context = data;

	/* get down to the business of waiting for new messages */
	wait_for_new_plain_connections(plain_socket_context);

	pthread_exit(NULL);
}

plain_socket_context_st * open_plain_socket_context(char const * const socket_name, bool use_abstract_namespace, plain_socket_callback_fn callback, void const * const user_context)
{
	int rc;
	plain_socket_context_st * plain_socket_context = NULL;

    if ((plain_socket_context = calloc(1, sizeof *plain_socket_context)) == NULL)
		goto error;

    unlink(socket_name);    /* remove any lingering remnants from a previous incarnation */

    plain_socket_context->socket_name = socket_name;
    plain_socket_context->use_abstract_namespace = use_abstract_namespace;
	plain_socket_context->callback = callback;
    plain_socket_context->user_context = user_context;

	rc = pthread_create(&plain_socket_context->thread_id, NULL, plain_socket_main_worker_thread, plain_socket_context);

	if (rc)
		goto error;

	goto done;

error:
    if (plain_socket_context != NULL)
        free(plain_socket_context);
    plain_socket_context = NULL;

done:
	return plain_socket_context;

}

int connect_to_plain_socket(char const * const socket_name, bool const use_abstract_namespace)
{
	int 	sock;

    sock = connect_to_socket(socket_name, use_abstract_namespace);

    return sock;
}

