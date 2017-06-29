#include "events_helper.h"
#include "bridge_monitor.h"
#include "bridge_monitor_event_alloc.h"
#include "event_manager.h"

static int send_event_to_event_manager_result;
static void * event_sent_to_event_manager;
static bridge_context_st * bridge_monitor_context;
static size_t bridgemon_event_free_called;

static size_t bridge_handle_initialise_event_called;
static size_t bridge_handle_ping_response_timeout_event_called;
static size_t bridge_handle_update_event_called;
static size_t bridge_handle_connect_event_called;
static size_t bridge_handle_disconnect_event_called;
static size_t bridge_handle_connected_event_called;
static size_t bridge_handle_disconnected_event_called;
static size_t bridge_handle_ping_message_event_called;
static size_t free_called_out_of_order;
static bridge_monitor_event_t * event_alloc_result;
static bridge_monitor_event_t * event_freed;
static event_manager_st * new_event_manager_result;
static size_t event_queue_size_passed_to_new_event_manager;

void event_spy_init(void)
{
    event_sent_to_event_manager = NULL;
    send_event_to_event_manager_result = 0;
    bridgemon_event_free_called = 0;
    bridge_handle_initialise_event_called = 0;
    bridge_handle_ping_response_timeout_event_called = 0;
    bridge_handle_update_event_called = 0;
    bridge_handle_connect_event_called = 0;
    bridge_handle_disconnect_event_called = 0;
    bridge_handle_connected_event_called = 0;
    bridge_handle_disconnected_event_called = 0;
    bridge_handle_ping_message_event_called = 0;
    free_called_out_of_order = 0;
    event_alloc_result = NULL;
    event_freed = NULL;
    new_event_manager_result = NULL;
    event_queue_size_passed_to_new_event_manager = 0;
}

void event_spy_set_send_event_to_event_manager_result(int result)
{
    send_event_to_event_manager_result = result;
}

void * event_spy_get_event_sent_to_event_manager(void)
{
    return event_sent_to_event_manager;
}

void event_spy_cleanup(void)
{
    event_sent_to_event_manager = NULL;
}

int send_event_to_event_manager(event_manager_st * const event_manager, void * const event)
{
    (void)event_manager;

    event_sent_to_event_manager = event;

    return send_event_to_event_manager_result;
}

void event_spy_set_bridge_context(bridge_context_st * context)
{
    bridge_monitor_context = context;
}

bridge_context_st * bridge_get_context(size_t const instance)
{
    return bridge_monitor_context;
}

size_t event_spy_get_event_free_call_count(void)
{
    return bridgemon_event_free_called;
}

void event_spy_set_event_alloc_event(bridge_monitor_event_t * const event)
{
    event_alloc_result = event;
}

bridge_monitor_event_t * bridgemon_event_alloc(int code, size_t instance, void * * const info, size_t const info_size)
{
    return event_alloc_result;
}

bridge_monitor_event_t * event_spy_get_freed_event(void)
{
    return event_freed;
}

void bridgemon_event_free(bridge_monitor_event_t * event)
{
    event_freed = event;
    bridgemon_event_free_called++;
}

size_t event_spy_get_bridge_handle_initialise_event_call_count(void)
{
    return bridge_handle_initialise_event_called;
}

void bridge_handle_initialise_event(void)
{
    if (bridgemon_event_free_called > 0)
    {
        free_called_out_of_order++;
    }
    bridge_handle_initialise_event_called++;
}

size_t event_spy_get_bridge_handle_ping_response_timeout_event_call_count(void)
{
    return bridge_handle_ping_response_timeout_event_called;
}

void bridge_handle_ping_response_timeout_event(bridge_context_st * const context)
{
    if (bridgemon_event_free_called > 0)
    {
        free_called_out_of_order++;
    }
    bridge_handle_ping_response_timeout_event_called++;
}

size_t event_spy_get_bridge_handle_update_event_call_count(void)
{
    return bridge_handle_update_event_called;
}

void bridge_handle_update_event(bridge_context_st * const context)
{
    if (bridgemon_event_free_called > 0)
    {
        free_called_out_of_order++;
    }
    bridge_handle_update_event_called++;
}

size_t event_spy_get_bridge_handle_connect_event_call_count(void)
{
    return bridge_handle_connect_event_called;
}

void bridge_handle_connect_event(bridge_context_st * const context)
{
    if (bridgemon_event_free_called > 0)
    {
        free_called_out_of_order++;
    }
    bridge_handle_connect_event_called++;
}

size_t event_spy_get_bridge_handle_disconnect_event_call_count(void)
{
    return bridge_handle_disconnect_event_called;
}

void bridge_handle_disconnect_event(bridge_context_st * const context)
{
    if (bridgemon_event_free_called > 0)
    {
        free_called_out_of_order++;
    }
    bridge_handle_disconnect_event_called++;
}

size_t event_spy_get_bridge_handle_connected_event_call_count(void)
{
    return bridge_handle_connected_event_called;
}

void bridge_handle_connected_event(bridge_context_st * const context)
{
    if (bridgemon_event_free_called > 0)
    {
        free_called_out_of_order++;
    }
    bridge_handle_connected_event_called++;
}

size_t event_spy_get_bridge_handle_disconnected_event_call_count(void)
{
    return bridge_handle_disconnected_event_called;
}

void bridge_handle_disconnected_event(bridge_context_st * const context)
{
    if (bridgemon_event_free_called > 0)
    {
        free_called_out_of_order++;
    }
    bridge_handle_disconnected_event_called++;
}

size_t event_spy_get_bridge_handle_ping_message_event_call_count(void)
{
    return bridge_handle_ping_message_event_called;
}

void bridge_handle_ping_message_event(bridge_context_st * const context, bridge_monitor_event_t * const event)
{
    if (bridgemon_event_free_called > 0)
    {
        free_called_out_of_order++;
    }
    bridge_handle_ping_message_event_called++;
}

bool event_spy_free_called_before_handler(void)
{
    return free_called_out_of_order > 0;
}

void event_spy_set_new_event_manager_result(event_manager_st * result)
{
    new_event_manager_result = result;
}

size_t event_spy_get_event_queue_size_passed_to_new_event_manager(void)
{
    return event_queue_size_passed_to_new_event_manager;
}

event_manager_st * new_event_manager(void const * const user_context, size_t event_queue_size, event_handler event_handler_cb)
{
    event_queue_size_passed_to_new_event_manager = event_queue_size;
    return new_event_manager_result;
}

