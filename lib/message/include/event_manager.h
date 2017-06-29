#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

#include <stddef.h>

/* prototype for the event handler function */
typedef void (*event_handler)(void * const user_context, void * const data);

/* the event manager structure */
struct event_manager_st;
typedef struct event_manager_st event_manager_st;

event_manager_st * new_event_manager(void const * const user_context, size_t event_queue_size, event_handler event_handler_cb);
int send_event_to_event_manager(event_manager_st * const event_manager, void * const event);

#endif /* __EVENT_MANAGER_H__ */
