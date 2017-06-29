#include "tlr_socket.h"
#include "tlr_common.h"
#include "tlr_print.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

static int set_socket_name(char * dest, size_t maxsize, char const * const name, bool const use_abstract_namepsace)
{
	char const nul = '\0';
    int len;
    int result;

    if (name == NULL || *name == nul)
    {
        result = -1;
        goto done;
	}

	len = strlen(name);

    if (use_abstract_namepsace == true)
    {
        maxsize--;
        dest[0] = nul;
        dest++;
        len++;
    }

	if (maxsize < len + 1)
	{
		result = -1;
		goto done;
	}
	strncpy(dest, name, maxsize);

    result = len;

done:
	return result;
}

int connect_to_socket(char const * const socket_name, bool use_abstract_namespace)
{
	int sock, len;
	struct sockaddr_un remote;

	if ((sock = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
        goto done;
	}

	memset(&remote, 0, sizeof remote);
	remote.sun_family = PF_LOCAL;
	len = set_socket_name(remote.sun_path, sizeof remote.sun_path, socket_name, use_abstract_namespace);
	if (len < 0)
	{
		perror("set_socket_name");
        close(sock);
        sock = -1;
        goto done;
	}
	len += sizeof remote.sun_family;

	if (connect(sock, (struct sockaddr *)&remote, len) == -1)
	{
		perror("connect");
        close(sock);
        sock = -1;
        goto done;
	}

done:
	return sock;
}

int listen_on_socket(char const * const socket_name, bool use_abstract_namespace)
{
    int sock;
    struct sockaddr_un server;
    int len;

    tlr_debug("listen on socket '%s'\n", socket_name);

    sock = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("opening stream socket");
        goto error;
    }
    memset(&server, 0, sizeof server);
    server.sun_family = PF_LOCAL;

    len = set_socket_name(server.sun_path, sizeof server.sun_path, socket_name, use_abstract_namespace);
    if (len < 0)
    {
        perror("pathname");
        close(sock);
        sock = -1;
        goto error;
    }
    len += sizeof server.sun_family;

    unlink(socket_name);    /* remove any previous instances of the socket (non-abstract namespace only?) */

    if (bind(sock, (struct sockaddr *)&server, len))
    {
        perror("binding socket");
        close(sock);
        sock = -1;
        goto error;
    }

    if (listen(sock, 5) == -1)
    {
        close(sock);
        sock = -1;
        goto error;
    }

error:
    return sock;
}


