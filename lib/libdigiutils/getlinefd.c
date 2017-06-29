#include "getlinefd.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>

#define MIN_ALLOC 64

int getlinefd_timeout(char ** const output_buffer, size_t * n, int const fd, struct timeval * const timeout)
{
	size_t total_bytes_read;
	char ch;
	char * output_location;

	if (n == NULL || output_buffer == NULL || fd < 0)
	{
		errno = EINVAL;
		return -1;
	}

	/* allocate a buffer if none was supplied */
	if ( *output_buffer == NULL)
	{
		*n = MIN_ALLOC;
		*output_buffer = malloc(*n);
		if (*output_buffer == NULL)
		{
			errno = ENOMEM;
			return -1;
		}
	}

	output_location = *output_buffer;
	total_bytes_read = 0;

	for (;;)
	{
        int select_result;
		ssize_t bytes_read;
        fd_set read_fd_set;

        FD_ZERO(&read_fd_set);
        FD_SET(fd, &read_fd_set);

        select_result = select(fd + 1, &read_fd_set, NULL, NULL, timeout);
        if (select_result < 0)
        {
            if (errno == EINTR)
                continue;
            if (total_bytes_read > 0)	/* read something prior to this error */
                break;
            return -1;
        }
        else if (select_result == 0)
        {
            // timeout.
            errno = ETIMEDOUT;
            return -1;
        }

        bytes_read = read(fd, &ch, 1);
		if (bytes_read == -1)
		{
			perror("read error");
			if (errno == EINTR)
				continue;
			if (total_bytes_read > 0)	/* read something prior to this error */
				break;
			return -1;
		}

		/* No error. Was anything read? */
		if (bytes_read == 0)      /* EOF */
		{
			if (total_bytes_read > 0)
				break;
			return -1;
		}
		/* read a character */

		/* Ensure we have space to write it to the output buffer */
		if (total_bytes_read >= *n - 1)	/* must be space for the new char and the NUL terminator */
		{
			*n *= 2;
			*output_buffer = realloc(*output_buffer, *n);
			if (*output_buffer == NULL)
			{
				errno = ENOMEM;
				return -1;
			}
			output_location = *output_buffer + total_bytes_read;
		}

		/* add it to the output buffer */
		*output_location++ = ch;
		total_bytes_read++;

		if (ch == '\n')
			break;
	}

	*output_location = '\0';	/* NUL terminate the string */

	return (int)total_bytes_read;
}

#if defined(TEST_GETLINEFD)
int main (int argc, char *argv[])
{
	int fd;
	char * line = NULL;
	size_t n = 0;

	fd = open("./getlinefd.c", O_RDONLY);
	if (fd >= 0)
	{
		int bytes_read;
        struct timeval timeout;

		do
		{
			bytes_read = getlinefd_timeout(&line, &n, fd, NULL);
			if (bytes_read > 0)
			{
				fprintf(stdout, "%s", line);
			}
		}
		while (bytes_read != -1);
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        bytes_read = getlinefd_timeout(&line, &n, STDIN_FILENO, &timeout);
        if (bytes_read < 0)
        {
            fprintf(stderr, "failed\n");
        }
    }
	close(fd);
	free(line);

	return 0;
}
#endif
