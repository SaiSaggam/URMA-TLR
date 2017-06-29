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

/* Example of adding a STATIC buffer (single structure) or static buffer array named my_data
 *  
 * In os_intf.h: 
 * 1. Add to connector_static_buffer_id_t:
 *      named_buffer_id(my_data)
 * - Single buffers share a msg_facility map, and their id's should go up from the bottom (28, 27, etc.)
 * - All arrays should be before single buffers
 *  
 * In this file:
 * 2. - If there is a type, add:
 *         typedef my_buffer_type_t named_buffer_type(my_data);
 *  
 *    - If you know size in bytes instead (for example 100 bytes), add:
 *         define_sized_buffer_type(my_data, 100);
 *  
 * For a single static buffer:
 * 3. Add to connector_static_mem: 
 *      named_buffer_define(my_data);
 *  
 * 4. Add to malloc_static_data(): 
 *      case named_buffer_id(my_data):
 *         malloc_named_static_buffer(my_data, size, ptr, status);
 *         break;
 *  
 * 5. Add to free_static_data(): 
 *      case named_buffer_id(my_data):
 *           free_named_static_buffer(my_data);
 *           break;
 *  
 * For an array of 20 elements (must be <= 32): 
 * 3. Add: 
 *      #define my_data_buffer_cnt 20
 *  
 * 4. Add to connector_static_mem: 
 *      named_buffer_array_define(my_data);
 *      named_buffer_map_define(my_data);
 *
 * 5. Add to malloc_static_data(): 
 *      case named_buffer_id(my_data):
 *         malloc_named_array_element(my_data, size, ptr, status);
 *         break;
 * 
 * 6. Add to free_static_data(): 
 *      case named_buffer_id(my_data):
 *           free_named_array_element(my_data, ptr);
 *           break;
 * 
 */


#define named_buffer_type(name)         name##_static_buffer_t
#define named_buffer_decl(name)         name
#define named_buffer_map_decl(name)     name##_map
#define named_buffer_cnt(name)          name##_buffer_cnt
#define named_buffer_define(name)       named_buffer_type(name) named_buffer_decl(name)
#define named_buffer_array_define(name) named_buffer_define(name)[named_buffer_cnt(name)]
#define named_buffer_map_define(name)   uint32_t named_buffer_map_decl(name)

#define named_buffer_storage(name)      connector_static_mem.named_buffer_decl(name)
#define named_buffer_map(name)          connector_static_mem.named_buffer_map_decl(name)

#define define_sized_buffer_type(name, size) typedef struct { char field[(size)]; } named_buffer_type(name)

/*** Define types used by static memory system ***/

#if defined CONNECTOR_TRANSPORT_TCP

#if (defined CONNECTOR_DATA_SERVICE) || (defined CONNECTOR_FILE_SYSTEM) || defined (CONNECTOR_RCI_SERVICE)
#ifndef CONNECTOR_MSG_MAX_TRANSACTION
#define CONNECTOR_MSG_MAX_TRANSACTION   1
#endif

#if CONNECTOR_MSG_MAX_TRANSACTION > 32
#error "CONNECTOR_MSG_MAX_TRANSACTION must be <= 32"
#endif

#define CONNECTOR_MSG_SERVICE

#if defined CONNECTOR_RCI_SERVICE
#define RCI_SERVICE_CNT     1
#define DATA_BLOCKS_PER_MSG 2
#else
#define RCI_SERVICE_CNT     0
#define DATA_BLOCKS_PER_MSG 1
#endif

#if defined CONNECTOR_DATA_SERVICE
#define DATA_SERVICE_CNT 1
#else
#define DATA_SERVICE_CNT 0
#endif

#if defined CONNECTOR_FILE_SYSTEM
#define FILE_SYSTEM_CNT 1
#else
#define FILE_SYSTEM_CNT 0
#endif

#define msg_facility_buffer_cnt (DATA_SERVICE_CNT + FILE_SYSTEM_CNT + RCI_SERVICE_CNT)

#define msg_session_buffer_cnt  (CONNECTOR_MSG_MAX_TRANSACTION * msg_facility_buffer_cnt)
#define msg_service_buffer_cnt  msg_session_buffer_cnt

#if defined CONNECTOR_DATA_SERVICE

#ifndef CONNECTOR_NO_MALLOC_MAX_SEND_SESSIONS
#define CONNECTOR_NO_MALLOC_MAX_SEND_SESSIONS   1
#endif

#define msg_session_client_buffer_cnt  CONNECTOR_NO_MALLOC_MAX_SEND_SESSIONS
#define put_request_buffer_cnt      msg_session_client_buffer_cnt
typedef data_service_context_t      named_buffer_type(put_request);

#if defined CONNECTOR_DATA_POINTS
#define data_point_block_buffer_cnt msg_session_client_buffer_cnt
typedef data_point_info_t           named_buffer_type(data_point_block);
#endif
#endif

typedef struct
{
    connector_facility_t fc;
    connector_msg_data_t msg;

} named_buffer_type(msg_facility);

typedef union
{
#if defined CONNECTOR_DATA_SERVICE
    data_service_context_t ds;
#endif
#if defined CONNECTOR_FILE_SYSTEM
    fs_context_t  fs;
#endif

#if defined CONNECTOR_RCI_SERVICE
    rci_service_data_t     rci;
#endif
} named_buffer_type(msg_service);

#if defined CONNECTOR_COMPRESSION
#define MSG_SEND_SIZE   0
#else
#define MSG_SEND_SIZE   MSG_MAX_SEND_PACKET_SIZE
#endif
#define MSG_BLOCK_SIZE          (sizeof(msg_data_block_t) + sizeof(msg_service_data_t))
#define MSG_SESSION_CLIENT_SIZE (sizeof(msg_session_t) + MSG_BLOCK_SIZE + MSG_SEND_SIZE)
#define MSG_SESSION_SIZE        (sizeof(msg_session_t) + DATA_BLOCKS_PER_MSG * MSG_BLOCK_SIZE + MSG_SEND_SIZE)

define_sized_buffer_type(msg_session, MSG_SESSION_SIZE);
define_sized_buffer_type(msg_session_client, MSG_SESSION_CLIENT_SIZE);
#endif

typedef struct
{
    connector_facility_t fc;
    connector_cc_data_t  cc;

} named_buffer_type(cc_facility);

#if defined CONNECTOR_FIRMWARE_SERVICE || defined CONNECTOR_RCI_SERVICE
typedef struct
{
    connector_facility_t fc;
    connector_firmware_data_t  fw;

} named_buffer_type(fw_facility);
#endif
#endif /* CONNECTOR_TRANSPORT_TCP */

#if (defined CONNECTOR_SHORT_MESSAGE)

#if (defined CONNECTOR_TRANSPORT_UDP)
#ifndef CONNECTOR_SM_UDP_MAX_RX_SEGMENTS
#error "You have to define CONNECTOR_SM_UDP_MAX_RX_SEGMENTS for CONNECTOR_NO_MALLOC configuration"
#endif
#else
#undef CONNECTOR_SM_UDP_MAX_RX_SEGMENTS
#define CONNECTOR_SM_UDP_MAX_RX_SEGMENTS 0
#endif
#if (defined CONNECTOR_TRANSPORT_SMS)
#ifndef CONNECTOR_SM_SMS_MAX_RX_SEGMENTS
#error "You have to define CONNECTOR_SM_SMS_MAX_RX_SEGMENTS for CONNECTOR_NO_MALLOC configuration"
#endif
#else
#undef CONNECTOR_SM_SMS_MAX_RX_SEGMENTS
#define CONNECTOR_SM_SMS_MAX_RX_SEGMENTS 0
#endif

#undef CONNECTOR_SM_MAX_RX_SEGMENTS /* In case of old define is still present in connector_config.h */
#define CONNECTOR_SM_MAX_RX_SEGMENTS (CONNECTOR_SM_UDP_MAX_RX_SEGMENTS + CONNECTOR_SM_SMS_MAX_RX_SEGMENTS)


#ifndef CONNECTOR_SM_MAX_DATA_POINTS_SEGMENTS
#define CONNECTOR_SM_MAX_DATA_POINTS_SEGMENTS   1
#endif

#define CONNECTOR_MAX_SM_SEGMENTS   (CONNECTOR_SM_MAX_RX_SEGMENTS + CONNECTOR_SM_MAX_DATA_POINTS_SEGMENTS)

#if (defined CONNECTOR_TRANSPORT_UDP)
#ifndef CONNECTOR_SM_UDP_MAX_SESSIONS
#error "You have to define CONNECTOR_SM_UDP_MAX_SESSIONS for CONNECTOR_NO_MALLOC configuration"
#endif
#else
#undef CONNECTOR_SM_UDP_MAX_SESSIONS
#define CONNECTOR_SM_UDP_MAX_SESSIONS 0
#endif
#if (defined CONNECTOR_TRANSPORT_SMS)
#ifndef CONNECTOR_SM_SMS_MAX_SESSIONS
#error "You have to define CONNECTOR_SM_SMS_MAX_SESSIONS for CONNECTOR_NO_MALLOC configuration"
#endif
#else
#undef CONNECTOR_SM_SMS_MAX_SESSIONS
#define CONNECTOR_SM_SMS_MAX_SESSIONS 0
#endif

#undef CONNECTOR_SM_MAX_SESSIONS /* In case of old define is still present in connector_config.h */
#define CONNECTOR_SM_MAX_SESSIONS (CONNECTOR_SM_UDP_MAX_SESSIONS + CONNECTOR_SM_SMS_MAX_SESSIONS)

#if CONNECTOR_SM_MAX_SESSIONS > 32
#error "CONNECTOR_SM_MAX_SESSIONS must be <= 32"
#endif

#if defined (CONNECTOR_TRANSPORT_UDP) && defined (CONNECTOR_TRANSPORT_SMS)
#define sm_session_buffer_cnt   (4 * CONNECTOR_SM_MAX_SESSIONS)
#else
#define sm_session_buffer_cnt   (2 * CONNECTOR_SM_MAX_SESSIONS)
#endif

#define sm_packet_buffer_cnt        sm_session_buffer_cnt
#define sm_data_block_buffer_cnt    sm_session_buffer_cnt

#if defined CONNECTOR_TRANSPORT_UDP 
#define SM_MAX_MTU  SM_PACKET_SIZE_UDP
#else
#define SM_MAX_MTU  SM_PACKET_SIZE_SMS_ENCODED
#endif

typedef connector_sm_session_t named_buffer_type(sm_session);

define_sized_buffer_type(sm_packet, 2 * SM_MAX_MTU);
define_sized_buffer_type(sm_data_block, CONNECTOR_MAX_SM_SEGMENTS * SM_MAX_MTU);

#endif

typedef connector_data_t named_buffer_type(connector_data);


/*** Declare static memory structure ***/
static struct
{
    named_buffer_define(connector_data);
    named_buffer_map_define(msg_facility);

#if defined CONNECTOR_TRANSPORT_TCP
    named_buffer_define(cc_facility);

#if defined CONNECTOR_FIRMWARE_SERVICE || defined CONNECTOR_RCI_SERVICE
    named_buffer_define(fw_facility);
#endif

#if defined CONNECTOR_MSG_SERVICE
    named_buffer_array_define(msg_facility);
    named_buffer_array_define(msg_session);
    named_buffer_array_define(msg_service);

    named_buffer_map_define(msg_session);
    named_buffer_map_define(msg_service);
#endif

#if defined CONNECTOR_DATA_SERVICE
    named_buffer_array_define(msg_session_client);
    named_buffer_array_define(put_request);
    named_buffer_map_define(msg_session_client);
    named_buffer_map_define(put_request);
#if defined CONNECTOR_DATA_POINTS
    named_buffer_array_define(data_point_block);
    named_buffer_map_define(data_point_block);
#endif
#endif
#endif

#if defined CONNECTOR_TRANSPORT_UDP || defined CONNECTOR_TRANSPORT_SMS
    named_buffer_array_define(sm_session);
    named_buffer_array_define(sm_packet);
    named_buffer_array_define(sm_data_block);

    named_buffer_map_define(sm_session);
    named_buffer_map_define(sm_packet);
    named_buffer_map_define(sm_data_block);
#endif

} connector_static_mem;

/*** Static memory operations ***/

#define static_buffer_release(pmap, idx) (*(pmap) &= (~(1 << (idx))))

STATIC int static_buffer_reserve(uint32_t * const pmap, uint32_t const idx)
{
    int result = -1;

    if ((*pmap & (1 << idx)) == 0)
    {
        *pmap |= 1 << idx;
        result = idx;
    }
    return result;
}

STATIC int static_buffer_get_avail(uint32_t * const pmap, int max_cnt)
{
    int idx = -1;
    int i;

    for (i = 0; i < max_cnt; i++)
    {
        idx = static_buffer_reserve(pmap, i);
        if (idx >= 0)
            break;
    }
    return idx;
}

#define static_buffer_malloc(pmap, name) static_buffer_reserve(pmap, named_buffer_id(name)) >= 0 ? &named_buffer_storage(name) : NULL
#define static_buffer_free(pmap, name) static_buffer_release(pmap, named_buffer_id(name))

#define free_named_array_element(name, ptr) \
{   \
    int idx = ((named_buffer_type(name) *) ptr - (named_buffer_type(name) *) &named_buffer_storage(name)[0]); \
    ASSERT(idx >= 0 && idx < named_buffer_cnt(name)); \
    static_buffer_release(&named_buffer_map(name), idx); \
}

#define malloc_named_static_buffer(name, size, ptr, status) \
{ \
    ASSERT(size == sizeof(named_buffer_type(name))); \
    *ptr = static_buffer_malloc(&named_buffer_map(msg_facility), name); \
    status = *ptr != NULL ? connector_working : connector_pending; \
}


#define free_named_static_buffer(name) static_buffer_free(&named_buffer_map(msg_facility), name)

#define malloc_named_array_element(name, size, ptr, status) \
{   \
    int idx; \
    ASSERT(size <= sizeof(named_buffer_type(name))); \
    idx = static_buffer_get_avail(&named_buffer_map(name), named_buffer_cnt(name)); \
    *ptr = idx >= 0 ? &named_buffer_storage(name)[idx] : NULL; \
    status = *ptr != NULL ? connector_working : connector_pending; \
}

STATIC connector_status_t malloc_static_data(connector_data_t * const connector_ptr, size_t const size, connector_static_buffer_id_t const buffer_id, void ** const ptr)
{
    connector_status_t status = connector_working;

    UNUSED_PARAMETER(connector_ptr);
    UNUSED_PARAMETER(size);

    if (buffer_id != named_buffer_id(connector_data))
    {
        ASSERT(connector_ptr == &named_buffer_storage(connector_data));
    }

    switch(buffer_id)
    {
    case named_buffer_id(connector_data):
        malloc_named_static_buffer(connector_data, size, ptr, status);
        break;

#if defined CONNECTOR_TRANSPORT_TCP
    case named_buffer_id(cc_facility):
        malloc_named_static_buffer(cc_facility, size, ptr, status);
        break;

#if defined CONNECTOR_FIRMWARE_SERVICE || defined CONNECTOR_RCI_SERVICE
    case named_buffer_id(fw_facility):
        malloc_named_static_buffer(fw_facility, size, ptr, status);
        break;
#endif

#if defined CONNECTOR_MSG_SERVICE
    case named_buffer_id(msg_facility):
        malloc_named_array_element(msg_facility, size, ptr, status);
        break;

    case named_buffer_id(msg_session):
        malloc_named_array_element(msg_session, size, ptr, status);
        break;

    case named_buffer_id(msg_service):
        malloc_named_array_element(msg_service, size, ptr, status);
        break;

#if defined CONNECTOR_DATA_SERVICE
    case named_buffer_id(msg_session_client):
        malloc_named_array_element(msg_session_client, size, ptr, status);
        break;

    case named_buffer_id(put_request):
        malloc_named_array_element(put_request, size, ptr, status);
        break;

#if defined CONNECTOR_DATA_POINTS
    case named_buffer_id(data_point_block):
        malloc_named_array_element(data_point_block, size, ptr, status);
        break;
#endif
#endif
#endif
#endif

#if defined CONNECTOR_TRANSPORT_UDP || defined CONNECTOR_TRANSPORT_SMS
    case named_buffer_id(sm_session):
        malloc_named_array_element(sm_session, size, ptr, status);
        break;

    case named_buffer_id(sm_packet):
        malloc_named_array_element(sm_packet, size, ptr, status);
        break;

    case named_buffer_id(sm_data_block):
        malloc_named_array_element(sm_data_block, size, ptr, status);
        break;
#endif

    default:
        ASSERT(connector_false);
        *ptr = NULL;
        break;
    }
    return status;
}

void free_static_data(connector_data_t * const connector_ptr, connector_static_buffer_id_t const buffer_id, void * const ptr)
{
    UNUSED_PARAMETER(connector_ptr);
    UNUSED_PARAMETER(ptr);
    ASSERT(connector_ptr == &named_buffer_storage(connector_data));

    switch(buffer_id)
    {
    case named_buffer_id(connector_data):
        free_named_static_buffer(connector_data);
        break;

#if defined CONNECTOR_TRANSPORT_TCP
    case named_buffer_id(cc_facility):
        free_named_static_buffer(cc_facility);
        break;

#if defined CONNECTOR_FIRMWARE_SERVICE || defined CONNECTOR_RCI_SERVICE
    case named_buffer_id(fw_facility):
        free_named_static_buffer(fw_facility);
        break;
#endif

#if defined CONNECTOR_MSG_SERVICE
    case named_buffer_id(msg_facility):
        free_named_array_element(msg_facility, ptr);
        break;

    case named_buffer_id(msg_service):
        free_named_array_element(msg_service, ptr);
        break;

#if defined CONNECTOR_DATA_SERVICE
    case named_buffer_id(put_request):
        free_named_array_element(put_request, ptr);
        break;

#if defined CONNECTOR_DATA_POINTS
    case named_buffer_id(data_point_block):
        free_named_array_element(data_point_block, ptr);
        break;
#endif
#endif

    case named_buffer_id(msg_session):
        {
#if defined CONNECTOR_DATA_SERVICE
            named_buffer_type(msg_session_client) * data_ptr = ptr;

            if ((data_ptr >= &named_buffer_storage(msg_session_client)[0]) &&
                (data_ptr <= &named_buffer_storage(msg_session_client)[named_buffer_cnt(msg_session_client)]))
            {
                free_named_array_element(msg_session_client, ptr);
            }
            else
#endif
            free_named_array_element(msg_session, ptr);
        }
        break;
#endif
#endif

#if defined CONNECTOR_TRANSPORT_UDP || defined CONNECTOR_TRANSPORT_SMS
    case named_buffer_id(sm_session):
        free_named_array_element(sm_session, ptr);
        break;

    case named_buffer_id(sm_packet):
        free_named_array_element(sm_packet, ptr);
        break;

    case named_buffer_id(sm_data_block):
        free_named_array_element(sm_data_block, ptr);
        break;
#endif

    default:
        break;
    }
}
