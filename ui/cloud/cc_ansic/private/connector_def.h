/*
 * Copyright (c) 2014 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */

#ifndef CONNECTOR_DEF_H_
#define CONNECTOR_DEF_H_

#ifndef CONNECTOR_MAX_VENDOR_ID_NUMBER
#define CONNECTOR_MAX_VENDOR_ID_NUMBER 0xFE000000
#endif

#define UNUSED_VARIABLE(x)      ((void) (x))
#define UNUSED_PARAMETER(x)     UNUSED_VARIABLE(x)

#define ON_FALSE_DO_(cond, code)        do { if (!(cond)) {code;} } while (0)

#if (defined CONNECTOR_DEBUG)
#define ON_ASSERT_DO_(cond, code, output)   ON_FALSE_DO_((cond), {ASSERT(cond); code;})
#else
#define ON_ASSERT_DO_(cond, code, output)   ON_FALSE_DO_((cond), {code})
#endif

#define ASSERT_GOTO(cond, label)    ON_ASSERT_DO_((cond), {goto label;}, {})
#define CONFIRM(cond)               do { switch(0) {case 0: case (cond):;} } while (0)

#define COND_ELSE_GOTO(cond, label)    ON_FALSE_DO_((cond), {goto label;})


#define DEVICE_ID_LENGTH    16
#define CLOUD_URL_LENGTH   64

#define CONNECTOR_GSM_IMEI_LENGTH   8
#define CONNECTOR_ESN_HEX_LENGTH    4
#define CONNECTOR_MEID_HEX_LENGTH   7

#define MAC_ADDR_LENGTH     6

#define MIN_VALUE(x,y)        (((x) < (y))? (x): (y))
#define MAX_VALUE(x,y)        (((x) > (y))? (x): (y))

#define MAX_RECEIVE_TIMEOUT_IN_SECONDS  1
#define MIN_RECEIVE_TIMEOUT_IN_SECONDS  0

#define FW_VERSION_NUMBER(version)  (MAKE32_4(version.major, version.minor, version.revision, version.build))

#if !(defined CONNECTOR_TRANSPORT_RECONNECT_AFTER)
#define CONNECTOR_TRANSPORT_RECONNECT_AFTER     30
#endif

typedef enum {
#if (defined CONNECTOR_TRANSPORT_TCP)
    connector_network_tcp,
#endif
#if (defined CONNECTOR_TRANSPORT_UDP)
    connector_network_udp,
#endif
#if (defined CONNECTOR_TRANSPORT_SMS)
    connector_network_sms,
#endif
    connector_network_count
} connector_network_type_t;

typedef enum {
    connector_transport_idle,
    connector_transport_open,
    connector_transport_send,
    connector_transport_receive,
    connector_transport_close,
    connector_transport_terminate,
    connector_transport_redirect,
    connector_transport_wait_for_reconnect
} connector_transport_state_t;

#define connector_bool(cond)    ((cond) ? connector_true : connector_false)

#define add_list_node(head, tail, node) \
    do { \
        ASSERT(node != NULL); \
        if (*head != NULL) \
        {\
            *head->prev = node;\
        }\
        node->next = *head;\
        node->prev = NULL;\
        *head = node;\
        if ((tail != NULL) && (*tail == NULL)) \
        { \
            *tail = node; \
        } \
   } while (0)

#define remove_list_node(head, tail, node) \
    do { \
        ASSERT(node != NULL); \
        if (node->next != NULL) \
        {\
            node->next->prev = node->prev;\
        }\
        if (node->prev != NULL) \
        {\
            node->prev->next = node->next;\
        }\
        if (node == *head)\
        {\
            *head = node->next;\
        }\
        if ((tail != NULL) && (node == *tail))\
        {\
            *tail = node->prev;\
        }\
    } while (0)

#define add_node(head, node) \
    do { \
        ASSERT(node != NULL); \
        if (*head != NULL) \
        {\
            *head->prev = node;\
        }\
        node->next = *head;\
        node->prev = NULL;\
        *head = node;\
    } while (0)

#define remove_node(head, node) \
    do { \
        ASSERT(node != NULL); \
        if (node->next != NULL) \
        {\
            node->next->prev = node->prev;\
        }\
        if (node->prev != NULL) \
        {\
            node->prev->next = node->next;\
        }\
        if (node == *head)\
        {\
            *head = node->next;\
        }\
    } while (0)

struct connector_data;

#if (defined CONNECTOR_TRANSPORT_TCP)
#include "connector_edp_def.h"
#endif

#if defined(CONNECTOR_SHORT_MESSAGE)
#include "connector_sm_def.h"
#endif

typedef struct connector_data {

    uint8_t device_id[DEVICE_ID_LENGTH];
    uint8_t * mac_addr;
    uint8_t * wan_id;
    size_t wan_id_length;

    char const * device_cloud_url;
    size_t device_cloud_url_length;

    char const * device_cloud_phone;
    size_t device_cloud_phone_length;

    char const * device_cloud_service_id;
    size_t device_cloud_service_id_length;

    connector_connection_type_t connection_type;

#if !(defined CONNECTOR_WAN_LINK_SPEED_IN_BITS_PER_SECOND)
    uint32_t link_speed;
#endif

#if !(defined CONNECTOR_WAN_PHONE_NUMBER_DIALED)
    char const * phone_dialed;
    size_t phone_dialed_length;
#endif

    connector_bool_t connector_got_device_id;
#if (CONNECTOR_TRANSPORT_COUNT > 1)
    connector_network_type_t first_running_network;
#endif

    connector_callback_t callback;
    connector_status_t error_code;

#if (defined CONNECTOR_TRANSPORT_UDP || defined CONNECTOR_TRANSPORT_SMS)
    uint32_t last_request_id;
#endif

#if (defined CONNECTOR_TRANSPORT_UDP)
    connector_sm_data_t sm_udp;
#endif

#if (defined CONNECTOR_TRANSPORT_SMS)
    connector_sm_data_t sm_sms;
#endif

#if (defined CONNECTOR_TRANSPORT_TCP)
    connector_edp_data_t edp_data;
#endif

#if (defined CONNECTOR_RCI_SERVICE)
    connector_remote_config_data_t const * rci_data;
    struct rci * rci_internal_data;
#endif

#if (defined CONNECTOR_DATA_POINTS)
    connector_bool_t process_csv;
#endif

    struct {
        enum {
            connector_state_running,
            connector_state_stop_by_initiate_action,
            connector_state_terminate_by_initiate_action,
            connector_state_abort_by_callback
        } state;
        connector_stop_condition_t condition;
        void * user_context;
    } stop;
    void * context;

} connector_data_t;

#endif
