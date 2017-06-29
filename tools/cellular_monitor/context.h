/*****************************************************************************
* Copyright (c) 2015 Digi International Inc., All Rights Reserved
*
* This software contains proprietary and confidential information of Digi
* International Inc.  By accepting transfer of this copy, Recipient agrees
* to retain this software in confidence, to prevent disclosure to others,
* and to make no use of this software other than that for which it was
* delivered.  This is an unpublished copyrighted work of Digi International
* Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
* prohibited.
*
* Restricted Rights Legend
*
* Use, duplication, or disclosure by the Government is subject to
* restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
* Technical Data and Computer Software clause at DFARS 252.227-7031 or
* subparagraphs (c)(1) and (2) of the Commercial Computer Software -
* Restricted Rights at 48 CFR 52.227-19, as applicable.
*
* Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
*
*****************************************************************************/

#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "cellular_config.h"
#include "parsing.h"
#include "event_manager.h"
#include "message_codes.h"
#include "timer.h"
#include "cellular_status.h"
#include "tlr_common.h"

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>

#define CELLULAR_MODEM_STATUS_TIMER

#define MONITOR_EVENT_QUEUE_SIZE        300

#define WAIT_MODEM_TIME_IN_MILLISECS        1000    /* checking modem status */
#define MAX_CONNECTING_TIME_MILLISECS       (4 * (60 * 1000)) /* waiting connection */
#define MAX_DISCONNECTING_TIME_MILLISECS    (2 * (60 * 1000))
#define MAX_STATUS_TIME_MILLISECS           1000
#define MODEM_STATUS_TIME_IN_MILLISECS 1000

/*
 * The events that can affect the cellular state machine
 * Note that application event values must not exceed the first global message code
*/
#define CELLULAR_FOREACH_MESSAGE(MESSAGE) \
        MESSAGE(cellular_message_initialize)   \
        MESSAGE(cellular_message_connect)  \
        MESSAGE(cellular_message_disconnect)  \
        MESSAGE(cellular_message_connect_timeout)  \
        MESSAGE(cellular_message_disconnect_timeout)  \
        MESSAGE(cellular_message_modem_status_timeout)  \
        MESSAGE(CELL_PING_RESPONSE_TMR_EVENT) \
        MESSAGE(cellular_message_passthrough_enabled) \
        MESSAGE(cellular_message_passthrough_disabled)

typedef enum
{
    CELLULAR_FOREACH_MESSAGE(GENERATE_ENUM)
} cellular_message_code_t;


#define CELLULAR_FOREACH_STATE(STATE) \
        STATE(state_initialize)   \
        STATE(state_idle)  \
        STATE(state_connect) \
        STATE(state_connecting)   \
        STATE(state_connected)  \
        STATE(state_disconnect)  \
        STATE(state_disconnecting)  \
        STATE(state_disconnected)  \
        STATE(state_wait_modem)  \

#define CELLULAR_GENERATE_FUNC(FUNC) FUNC##_handler,

typedef enum
{
    CELLULAR_FOREACH_STATE(GENERATE_ENUM)
} cellular_state_t;

typedef struct
{
    size_t active_sim;
    cellular_connect_status_t connect_status;
    cellular_connect_status_t desired_status;
    size_t heartbeat;
} cellular_modem_status_t;

typedef struct cellular_context_t
{
    size_t instance;    /* which monitor instance is this. NB This is 1-based, not 0-based */
    size_t index_3g;
    size_t index_usb;

    event_manager_st * event_manager;

    bool desired_state;        /* true for UP, false for down. Set by interface manager task */
    bool had_sent_act_in;    /* true if we've previously sent and ACT_IN to interface manager */

    /* timers owned by the monitor */
    timer_id_st * connecting_timer_id;
    bool connecting_timer_running;

    timer_id_st * disconnecting_timer_id;
    bool disconnecting_timer_running;

#ifdef CELLULAR_MODEM_STATUS_TIMER
    timer_id_st * modem_status_timer_id;
    bool modem_status_timer_running;
#endif

    timer_id_st *   cell_pingmsg_reponse_timer_id;
    timer_context_st * timer_context;
    cellular_config_t config_data;

    cellular_state_t current_state; /* points to the current state handler for this context */
    bool connection_established;
    size_t restart_count;
    size_t status_timer_count;
    size_t connecting_timer_count;
    cellular_modem_status_t * modem_status;
    ping_info_st       const *p_response_info;

    int csman_fd;

} cellular_context_t;

#define CELLULAR_VALID_CSMAN_FD(fd)   ((fd) >= 0)

/* prototype for the event handler function */
typedef void (*state_handler_fn)(cellular_context_t * const context, void * const data);

extern char const * interface_name;    // the name of the interface we're monitoring
extern size_t interface_instance_max;
extern cellular_context_t * cellular_contexts;
extern bool init_3g_initialized;


#endif /* __CONTEXT_H__ */
