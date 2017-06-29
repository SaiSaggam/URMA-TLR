#ifndef __MESSAGES_PRIVATE_H__
#define __MESSAGES_PRIVATE_H__

#include <messages.h>

struct message_handler_info_st
{
	int sock;
	message_callback_fn message_callback;
};
typedef struct message_handler_info_st message_handler_info_st;

#endif /* __MESSAGES_PRIVATE_H__ */
