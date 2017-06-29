#ifndef _FAILOVER_H_
#define _FAILOVER_H_

#include "probing.h"
#include "tlr_common.h"
#include <tlr_print.h>

#include <parsing.h>
#include <tlr_print.h>
#include <stdbool.h>
#include <timer.h>
#include <pthread.h>
#include <stdint.h>

#include "config_mgr.h"
#include "unittest_support.h"

#define MAX_FQDN_LENGTH 256

#define FOREACH_FAILOVER_STATE(STATE) \
        STATE(failover_state_idle) \
        STATE(failover_state_start) \
        STATE(failover_state_switch) \
        STATE(failover_state_no_switch) \
        STATE(failover_state_done) \

typedef enum
{
    FOREACH_FAILOVER_STATE(GENERATE_ENUM)

} failover_state_t;

typedef struct
{
    size_t instance;
    interfaces_t interface;
    char const * name;
} ifm_if_info_t;

typedef struct
{
    size_t instance;
    config_group_t config_group;
    char const * name;
} ifm_update_info_t;

typedef struct
{
    bool enabled;
    ifm_if_info_t from;
    ifm_if_info_t to;
    config_failover_use_t use_type;
    size_t timeout_after;
    char probe_host[MAX_FQDN_LENGTH];
    size_t probe_size;
    size_t probe_timeout;
    size_t probe_interval;
    size_t switch_primary_up;
} failover_config_t;

typedef struct failover_info
{
    size_t instance;
    ifm_if_info_t * current_iface;
    timer_context_st * timer_context;
    failover_state_t state;
    timer_id_st * timer_id;
    timer_id_st * timer_probe_id;
    timer_id_st * timer_probe_interval_id;
    timer_id_st * timer_switch_primary_up_id;
    probe_context_st * probe_handle;
    uint32_t last_probe_request_id;
    struct timeval last_successful_probe_timestamp;   // time the probe response was received.

    bool valid;
    bool is_started;
    bool timer_probe_running;
    bool timer_probe_interval_running;

//    pthread_t failover_thread_id;
    failover_config_t config;
} failover_info_t;

typedef struct probe_success_info_st
{
    failover_info_t * failover;
    struct timeval timestamp;   // time the probe response was received.
    uint32_t probe_id;          // used to match the success message with the most recent probe request
} probe_success_info_st;

typedef struct
{
    failover_info_t * failover;
    size_t instance;
} failover_event_info_t;

int failover_handle_config_change(ifm_update_info_t const * const info);
//void failover_event_send(ifm_if_info_t const * const info);

int failover_probe_interval_timeout(failover_info_t * const failover);
int failover_probe_timeout(failover_info_t * const failover);
int failover_probe_success(probe_success_info_st * const success_info);
int failover_timeout_timer_timeout(failover_info_t * const failover);
int failover_switch_primary_up_timeout(failover_info_t * const failover);
//void failover_start(failover_info_t * const failover);
//void failover_stop(failover_info_t * const failover);

int failover_init(timer_context_st * const timer_context);
void failover_close(void);

void failover_handler(ifm_if_info_t const * const iface);
bool failover_inhibit_routes(ifm_if_info_t const * const iface);

failover_info_t * const failover_get_context(size_t const instance);
bool is_interface_info(ifm_if_info_t const * const src, ifm_if_info_t const * const dest);
bool get_failover_interface(size_t interface_id, ifm_if_info_t * const if_info);
size_t const failover_get_number_of_contexts(void);

#if defined(_CUNIT_) || defined(UNIT_TEST)
void failover_set_number_of_contexts(size_t const contexts_count);
void failover_set_contexts(failover_info_t * const contexts);
#endif

#endif /* _FAILOVER_H_ */
