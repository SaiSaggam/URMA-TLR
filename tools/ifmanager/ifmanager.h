#ifndef _IFMANAGER_H_
#define _IFMANAGER_H_

#include "failover.h"
#include <stdio.h>
#include <stddef.h>
#include <message_codes.h>
#include <event_manager.h>

#if !defined(IFMANAGER_DEBUG)
#define IFMANAGER_DEBUG 0
#endif

typedef enum
{
    ifm_config_state_off,
    ifm_config_state_on,
    ifm_config_state_on_demand
} ifm_config_state_t;

#define FOREACH_STATE(STATE) \
        STATE(ifm_state_init) \
        STATE(ifm_state_connecting) \
        STATE(ifm_state_connected) \
        STATE(ifm_state_disconnecting) \
        STATE(ifm_state_disconnected)

#define FOREACH_EVENT_CODE(EVENT_CODE) \
        EVENT_CODE(ifm_event_code_connected)   \
        EVENT_CODE(ifm_event_code_disconnected)  \
        EVENT_CODE(ifm_event_code_update)   \
        EVENT_CODE(ifm_event_code_timer_timeout)  \
        EVENT_CODE(ifm_event_code_probe_interval_timeout)  \
        EVENT_CODE(ifm_event_code_probe_timeout) \
        EVENT_CODE(ifm_event_code_probe_success) \
        EVENT_CODE(ifm_event_code_show) \
        EVENT_CODE(ifm_event_code_switch_primary_up_timeout) \
        EVENT_CODE(ifm_event_code_joined_bridge) \
        EVENT_CODE(ifm_event_code_left_bridge) \
        EVENT_CODE(ifm_event_code_dhcp_assigned_gateway)

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,
#define GENERATE_FUNC(FUNC) FUNC##_handler,

typedef enum
{
    FOREACH_STATE(GENERATE_ENUM)

} ifm_current_state_t;


typedef enum
{
    FOREACH_EVENT_CODE(GENERATE_ENUM)
} ifm_event_code_t;

typedef struct
{
    ifm_event_code_t code;
    void * info;
} ifm_event_t;

typedef struct ifm_if_config_t
{
    ifm_if_info_t info;
    ifm_config_state_t config_state;
    ifm_current_state_t current_state;
//    failover_info_t failover;
    struct ifm_if_config_t *pnext;
} ifm_if_config_t;

typedef struct ifm_config_t
{
    ifm_if_config_t * if_list;
//    timer_context_st * timer_context;
    event_manager_st * event_manager;
} ifm_config_t;

int ifm_init(timer_context_st * const context);
ifm_config_t * ifm_get_context(void); /* (size_t instance) */
int ifm_message_callback(int fd, size_t const instance, message_codes_t const message_code, void * * const info, size_t const info_size);
void ifm_event_handler(void * const user_context, void * const data);
void ifm_timeout_callback(void * context);
void ifm_show_failover_plain_socket_callback(int const socket_fd, void * const user_context);

void ifm_probe_timeout_callback(void * context);
void ifm_probe_interval_timeout_callback(void * context);

void ifm_switch_primary_up_timeout_callback(void * context);

int ifm_handle_config_change(ifm_update_info_t const * const info);
int ifm_handle_state_change(ifm_if_config_t * const ptr, ifm_config_state_t const new_state);
ifm_if_config_t * ifm_get_interface_entry(ifm_if_info_t const * const info);

char const * const failover_get_interface_name(ifm_if_info_t  const *  const ifptr);
int ifm_send_event(ifm_event_code_t code, void * const info, size_t info_size);

extern char const * state_strings[];

#define IFMANAGER "ifmanager"

#ifndef _CUNIT_

#if IFMANAGER_DEBUG
#define ifm_debug(x...)      do { printf("ifmanager: " x); } while (0)
#define ifm_error(x...)      do { printf("ifmanager: " x); } while (0)
#define ifm_info(x...)       do { printf("ifmanager: " x); } while (0)
#else
#define ifm_debug(x...)      do { tlr_debug("ifmanager: " x); } while (0)
#define ifm_error(x...)      do { tlr_event_log(tlr_log_error, IFMANAGER, x); } while (0)
#define ifm_info(x...)       do { tlr_event_log(tlr_log_info, IFMANAGER, x); } while (0)
#endif

#else

#if IFMANAGER_DEBUG
#define ifm_debug(x...)      do { printf("ifmanager: " x); } while (0)
#define ifm_error(x...)      do { printf("ifmanager: " x); } while (0)
#define ifm_info(x...)       do { printf("ifmanager: " x); } while (0)
#else
#define ifm_debug(x...)
#define ifm_error(x...)
#define ifm_info(x...)
#endif

#endif

#endif /* _IFMANAGER_H_ */
