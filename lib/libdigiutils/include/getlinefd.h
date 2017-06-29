#ifndef __GETLINEFD_H__
#define __GETLINEFD_H__

#include <stddef.h>
#include <sys/time.h>

int getlinefd_timeout(char ** const output_buffer, size_t * n, int const fd, struct timeval * const timeout);

#endif /* __GETLINEFD_H__ */
