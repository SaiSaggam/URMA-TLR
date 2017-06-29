#ifndef __TLR_SOCKET_H__
#define __TLR_SOCKET_H__

#include <stddef.h>
#include <stdbool.h>

int connect_to_socket(char const * const socket_name, bool use_abstract_namespace);
int listen_on_socket(char const * const socket_name, bool use_abstract_namespace);


#endif /* __TLR_SOCKET_H__ */
