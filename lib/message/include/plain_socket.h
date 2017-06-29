#ifndef __PLAIN_SOCKET_H__
#define __PLAIN_SOCKET_H__

#include <stdbool.h>

struct plain_socket_context_st;
typedef struct plain_socket_context_st plain_socket_context_st;
typedef void (*plain_socket_callback_fn)(int socket_fd, void * user_context);

plain_socket_context_st * open_plain_socket_context(char const * const socket_name, bool use_abstract_namespace, plain_socket_callback_fn callback, void const * const user_context);
int connect_to_plain_socket(char const * const socket_name, bool const use_abstract_namespace);
// TODO: close_plain_socket_context()

#endif /* __PLAIN_SOCKET_H__ */
