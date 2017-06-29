#ifndef __EVENTS_HELPER_H__
#define __EVENTS_HELPER_H__

#include "event_manager.h"
#include "bridge_monitor.h"
#include "bridge_monitor_event.h"

#include <stddef.h>
#include <stdbool.h>

void event_spy_set_send_event_to_event_manager_result(int result);
void * event_spy_get_event_sent_to_event_manager(void);
void event_spy_init(void);
void event_spy_cleanup(void);
void event_spy_set_bridge_context(bridge_context_st * context);
size_t event_spy_get_event_free_call_count(void);
bridge_monitor_event_t * event_spy_get_freed_event(void);
size_t event_spy_get_bridge_handle_initialise_event_call_count(void);
size_t event_spy_get_bridge_handle_ping_response_timeout_event_call_count(void);
size_t event_spy_get_bridge_handle_update_event_call_count(void);
size_t event_spy_get_bridge_handle_connect_event_call_count(void);
size_t event_spy_get_bridge_handle_disconnect_event_call_count(void);
size_t event_spy_get_bridge_handle_connected_event_call_count(void);
size_t event_spy_get_bridge_handle_disconnected_event_call_count(void);
size_t event_spy_get_bridge_handle_ping_message_event_call_count(void);
bool event_spy_free_called_before_handler(void);
void event_spy_set_event_alloc_event(bridge_monitor_event_t * const event);
void event_spy_set_new_event_manager_result(event_manager_st * result);
size_t event_spy_get_event_queue_size_passed_to_new_event_manager(void);


#endif /* __EVENTS_HELPER_H__ */
