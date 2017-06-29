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

#if (defined CONNECTOR_COMPRESSION)
#include "zlib.h"
#endif

#define MSG_FACILITY_VERSION  0x01
#define MSG_COMPRESSION_NONE  0x00
#define MSG_COMPRESSION_LIBZ  0xFF

#define MSG_INVALID_CLIENT_SESSION  0xFFFF

#define MSG_FLAG_REQUEST      UINT32_C(0x01)
#define MSG_FLAG_LAST_DATA    UINT32_C(0x02)
#define MSG_FLAG_SENDER       UINT32_C(0x04)

#define MSG_FLAG_CLIENT_OWNED UINT32_C(0x20)
#define MSG_FLAG_RECEIVING    UINT32_C(0x40)
#define MSG_FLAG_START        UINT32_C(0x80)
#define MSG_FLAG_ACK_PENDING  UINT32_C(0x100)
#define MSG_FLAG_COMPRESSED   UINT32_C(0x200)
#define MSG_FLAG_INFLATED     UINT32_C(0x400)
#define MSG_FLAG_DEFLATED     UINT32_C(0x800)
#define MSG_FLAG_SEND_NOW     UINT32_C(0x1000)
#define MSG_FLAG_DOUBLE_BUF   UINT32_C(0x2000)

#define MsgIsBitSet(flag, bit)   (connector_bool(((flag) & (bit)) == (bit)))
#define MsgIsBitClear(flag, bit) (connector_bool(((flag) & (bit)) == 0))
#define MsgBitSet(flag, bit)    ((flag) |= (bit))
#define MsgBitClear(flag, bit)  ((flag) &= ~(bit))

#define MsgIsRequest(flag)      MsgIsBitSet((flag), MSG_FLAG_REQUEST)
#define MsgIsLastData(flag)     MsgIsBitSet((flag), MSG_FLAG_LAST_DATA)
#define MsgIsSender(flag)       MsgIsBitSet((flag), MSG_FLAG_SENDER)
#define MsgIsReceiving(flag)    MsgIsBitSet((flag), MSG_FLAG_RECEIVING)
#define MsgIsCompressed(flag)   MsgIsBitSet((flag), MSG_FLAG_COMPRESSED)
#define MsgIsStart(flag)        MsgIsBitSet((flag), MSG_FLAG_START)
#define MsgIsAckPending(flag)   MsgIsBitSet((flag), MSG_FLAG_ACK_PENDING)
#define MsgIsClientOwned(flag)  MsgIsBitSet((flag), MSG_FLAG_CLIENT_OWNED)
#define MsgIsInflated(flag)     MsgIsBitSet((flag), MSG_FLAG_INFLATED)
#define MsgIsDeflated(flag)     MsgIsBitSet((flag), MSG_FLAG_DEFLATED)
#define MsgIsSendNow(flag)      MsgIsBitSet((flag), MSG_FLAG_SEND_NOW)
#define MsgIsDoubleBuf(flag)    MsgIsBitSet((flag), MSG_FLAG_DOUBLE_BUF)

#define MsgIsNotRequest(flag)      MsgIsBitClear((flag), MSG_FLAG_REQUEST)
#define MsgIsNotLastData(flag)     MsgIsBitClear((flag), MSG_FLAG_LAST_DATA)
#define MsgIsNotSender(flag)       MsgIsBitClear((flag), MSG_FLAG_SENDER)
#define MsgIsNotReceiving(flag)    MsgIsBitClear((flag), MSG_FLAG_RECEIVING)
#define MsgIsNotClientOwned(flag)  MsgIsBitClear((flag), MSG_FLAG_CLIENT_OWNED)
#define MsgIsNotInflated(flag)     MsgIsBitClear((flag), MSG_FLAG_INFLATED)
#define MsgIsNotDeflated(flag)     MsgIsBitClear((flag), MSG_FLAG_DEFLATED)

#define MsgSetRequest(flag)     MsgBitSet((flag), MSG_FLAG_REQUEST)
#define MsgSetLastData(flag)    MsgBitSet((flag), MSG_FLAG_LAST_DATA)
#define MsgSetSender(flag)      MsgBitSet((flag), MSG_FLAG_SENDER)
#define MsgSetReceiving(flag)   MsgBitSet((flag), MSG_FLAG_RECEIVING)
#define MsgSetCompression(flag) MsgBitSet((flag), MSG_FLAG_COMPRESSED)
#define MsgSetStart(flag)       MsgBitSet((flag), MSG_FLAG_START)
#define MsgSetAckPending(flag)  MsgBitSet((flag), MSG_FLAG_ACK_PENDING)
#define MsgSetClientOwned(flag) MsgBitSet((flag), MSG_FLAG_CLIENT_OWNED)
#define MsgSetInflated(flag)    MsgBitSet((flag), MSG_FLAG_INFLATED)
#define MsgSetDeflated(flag)    MsgBitSet((flag), MSG_FLAG_DEFLATED)
#define MsgSetSendNow(flag)     MsgBitSet((flag), MSG_FLAG_SEND_NOW)
#define MsgSetDoubleBuf(flag)   MsgBitSet((flag), MSG_FLAG_DOUBLE_BUF)

#define MsgClearRequest(flag)     MsgBitClear((flag), MSG_FLAG_REQUEST)
#define MsgClearLastData(flag)    MsgBitClear((flag), MSG_FLAG_LAST_DATA)
#define MsgClearReceiving(flag)   MsgBitClear((flag), MSG_FLAG_RECEIVING)
#define MsgClearStart(flag)       MsgBitClear((flag), MSG_FLAG_START)
#define MsgClearAckPending(flag)  MsgBitClear((flag), MSG_FLAG_ACK_PENDING)
#define MsgClearCompression(flag) MsgBitClear((flag), MSG_FLAG_COMPRESSED)
#define MsgClearInflated(flag)    MsgBitClear((flag), MSG_FLAG_INFLATED)
#define MsgClearDeflated(flag)    MsgBitClear((flag), MSG_FLAG_DEFLATED)
#define MsgClearSendNow(flag)     MsgBitClear((flag), MSG_FLAG_SEND_NOW)

typedef enum
{
    msg_service_id_none,
    msg_service_id_data,
    msg_service_id_file,
    msg_service_id_rci,
    msg_service_id_brci,
    msg_service_id_count
} msg_service_id_t;

typedef enum
{
    msg_opcode_capability,
    msg_opcode_start,
    msg_opcode_data,
    msg_opcode_ack,
    msg_opcode_error
} msg_opcode_t;

typedef enum
{
    msg_block_state_send_request,
    msg_block_state_recv_request,
    msg_block_state_send_response,
    msg_block_state_recv_response
} msg_block_state_t;

typedef enum
{
    msg_state_init,
    msg_state_get_data,
    msg_state_compress,
    msg_state_send_data,
    msg_state_wait_send_complete,
    msg_state_receive,
    msg_state_decompress,
    msg_state_process_decompressed,
    msg_state_send_ack,
    msg_state_send_error,
    msg_state_delete
} msg_state_t;

typedef enum
{
    msg_service_type_need_data,
    msg_service_type_have_data,
    msg_service_type_error,
    msg_service_type_free,
    msg_service_type_pending_request
} msg_service_type_t;

typedef enum
{
    msg_capability_cloud,
    msg_capability_client,
    msg_capability_count
} msg_capability_type_t;

enum msg_capability_packet_t
{
    field_define(capability_packet, opcode, uint8_t),
    field_define(capability_packet, flags, uint8_t),
    field_define(capability_packet, version, uint8_t),
    field_define(capability_packet, max_transactions, uint8_t),
    field_define(capability_packet, window_size, uint32_t),
    field_define(capability_packet, compression_count, uint8_t),
    record_end(capability_packet)
};

enum msg_start_packet_t
{
    field_define(start_packet, opcode, uint8_t),
    field_define(start_packet, flags, uint8_t),
    field_define(start_packet, transaction_id, uint16_t),
    field_define(start_packet, service_id, uint16_t),
    field_define(start_packet, compression_id, uint8_t),
    record_end(start_packet)
};

enum msg_data_packet_t
{
    field_define(data_packet, opcode, uint8_t),
    field_define(data_packet, flags, uint8_t),
    field_define(data_packet, transaction_id, uint16_t),
    record_end(data_packet)
};

enum msg_ack_packet_t
{
    field_define(ack_packet, opcode, uint8_t),
    field_define(ack_packet, flags, uint8_t),
    field_define(ack_packet, transaction_id, uint16_t),
    field_define(ack_packet, ack_count, uint32_t),
    field_define(ack_packet, window_size, uint32_t),
    record_end(ack_packet)
};

enum msg_error_packet_t
{
    field_define(error_packet, opcode, uint8_t),
    field_define(error_packet, flags, uint8_t),
    field_define(error_packet, transaction_id, uint16_t),
    field_define(error_packet, error_code, uint8_t),
    record_end(error_packet)
};

typedef struct msg_data_block_t
{
    size_t total_bytes;
    size_t available_window;
    size_t ack_count;
    unsigned int status_flag;
#if (defined CONNECTOR_COMPRESSION)
    uint8_t  buffer_in[MSG_MAX_RECV_PACKET_SIZE];
    uint8_t  buffer_out[MSG_MAX_RECV_PACKET_SIZE];
    size_t   bytes_out;
    int      z_flag;
    z_stream zlib;
#endif
} msg_data_block_t;

typedef struct
{
    void * data_ptr;
    size_t length_in_bytes;
    unsigned int flags;
} msg_service_data_t;

#if (defined CONNECTOR_DATA_SERVICE)
typedef enum {
    connector_send_data_initiator_user,
#if (defined CONNECTOR_DATA_POINTS)
    connector_send_data_initiator_data_point,
#endif
    connector_send_data_initiator_unknown
} connector_send_data_initiator_t;
#endif
typedef struct
{
    void * session;
    msg_service_type_t service_type;
    msg_service_data_t * need_data;
    msg_service_data_t * have_data;
    connector_session_error_t error_value;
#if (defined CONNECTOR_DATA_SERVICE)
    connector_send_data_initiator_t send_data_initiator;
#endif
} msg_service_request_t;

typedef struct msg_session_t
{
    unsigned int session_id;
    unsigned int service_id;
    void * service_context;
    msg_state_t current_state;
    msg_state_t saved_state;
    uint8_t * send_data_ptr;
    size_t send_data_bytes;
    msg_data_block_t * in_dblock;
    msg_data_block_t * out_dblock;
    connector_session_error_t error;
    unsigned int error_flag;
    msg_service_request_t service_layer_data;
    struct msg_session_t * next;
    struct msg_session_t * prev;
} msg_session_t;

typedef connector_status_t connector_msg_callback_t(connector_data_t * const connector_ptr, msg_service_request_t * const service_request);

typedef struct
{
    uint32_t window_size;
    connector_bool_t compression_supported;
    uint8_t active_transactions;
    uint8_t max_transactions;
} msg_capabilities_t;

typedef struct
{
    msg_capabilities_t capabilities[msg_capability_count];
    connector_msg_callback_t * service_cb[msg_service_id_count];
    connector_bool_t session_locked;
    struct
    {
        msg_session_t * head;
        msg_session_t * tail;
        msg_session_t * current;
    } session;
    unsigned int last_assigned_id;
    struct {
        void const * user;
        void const * internal;
    } pending_service_request;
} connector_msg_data_t;

STATIC msg_session_t * msg_find_session(connector_msg_data_t const * const msg_ptr, unsigned int const id, connector_bool_t const client_owned)
{
    msg_session_t * session = msg_ptr->session.head;

    while (session != NULL)
    {
        if (session->session_id == id)
        {
            unsigned int const status = (session->in_dblock != NULL) ? session->in_dblock->status_flag : session->out_dblock->status_flag;

            if (MsgIsClientOwned(status) == client_owned)
                break;
        }

        session = session->next;
    }

    return session;
}

static unsigned int msg_find_next_available_id(connector_msg_data_t * const msg_ptr)
{
    unsigned int new_id = MSG_INVALID_CLIENT_SESSION;
    unsigned int const last_id = msg_ptr->last_assigned_id;

    do
    {
        connector_bool_t const client_owned = connector_true;

        msg_ptr->last_assigned_id++;
        if (msg_ptr->last_assigned_id >= MSG_INVALID_CLIENT_SESSION)
        {
            msg_ptr->last_assigned_id = 0;
            continue;
        }

        if (msg_find_session(msg_ptr, msg_ptr->last_assigned_id, client_owned) == NULL)
        {
            new_id = msg_ptr->last_assigned_id;
            break;
        }

    } while (msg_ptr->last_assigned_id != last_id);

    return new_id;
}

STATIC void msg_set_error(msg_session_t * const session, connector_session_error_t const error_code)
{
    msg_data_block_t * const dblock = (session->in_dblock != NULL) ? session->in_dblock : session->out_dblock;
    connector_bool_t const client_request_error = connector_bool(MsgIsClientOwned(dblock->status_flag) && MsgIsNotReceiving(dblock->status_flag));
    connector_bool_t const client_response_error = connector_bool(MsgIsNotClientOwned(dblock->status_flag) && MsgIsNotReceiving(dblock->status_flag));
    connector_bool_t const cloud_request_error = connector_bool(MsgIsNotClientOwned(dblock->status_flag) && MsgIsReceiving(dblock->status_flag));

    if (client_request_error && MsgIsStart(dblock->status_flag))
    {
        /* no need to send an error. just delete since nothing has been sent to Device Cloud */
        session->current_state = msg_state_delete;
        goto done;
    }

    session->error = error_code;
    session->current_state = msg_state_send_error;

    if (client_response_error && MsgIsStart(dblock->status_flag))
    {
        /* canceling Device Cloud request since no response data has been sent yet */
        MsgSetRequest(session->error_flag);
        goto done;
    }

    if (client_request_error || cloud_request_error)
        MsgSetRequest(session->error_flag);

    if (client_request_error || client_response_error)
        MsgSetSender(session->error_flag);

done:
    return;
}

STATIC connector_status_t msg_call_service_layer(connector_data_t * const connector_ptr, msg_session_t * const session, msg_service_type_t const type)
{
    connector_status_t status = connector_init_error;
    msg_service_request_t * const service_ptr = &session->service_layer_data;
    connector_msg_data_t * const msg_ptr = get_facility_data(connector_ptr, E_MSG_FAC_MSG_NUM);
    connector_msg_callback_t * cb_fn;

    ASSERT_GOTO(msg_ptr != NULL, error);
    cb_fn = msg_ptr->service_cb[session->service_id];
    ASSERT_GOTO(cb_fn != NULL, error);

    service_ptr->service_type = type;
    service_ptr->session = session;

    status = cb_fn(connector_ptr, service_ptr);
    if ((status == connector_working) && (service_ptr->service_type == msg_service_type_error))
    {
        msg_set_error(session, service_ptr->error_value);
        status = connector_unavailable;
    }

error:
    return status;
}

STATIC connector_status_t msg_inform_error(connector_data_t * const connector_ptr, msg_session_t * const session, connector_session_error_t error_code)
{
    session->service_layer_data.error_value = error_code;

    return msg_call_service_layer(connector_ptr, session, msg_service_type_error);
}

STATIC msg_session_t * msg_create_session(connector_data_t * const connector_ptr, connector_msg_data_t * const msg_ptr, unsigned int const service_id,
                                          connector_bool_t const client_owned, connector_status_t * const status)
{
    unsigned int session_id = MSG_INVALID_CLIENT_SESSION;
    msg_capability_type_t const capability_id = client_owned == connector_true ? msg_capability_cloud : msg_capability_client;
    msg_session_t * session = NULL;
    unsigned int flags = 0;

    *status = connector_pending;
    ASSERT_GOTO(msg_ptr != NULL, done);

    {
        uint8_t const max_transactions = msg_ptr->capabilities[capability_id].max_transactions;
        uint8_t const active_transactions = msg_ptr->capabilities[capability_id].active_transactions;
        uint8_t const unlimited_transactions = 0;

        if ((max_transactions != unlimited_transactions) && (active_transactions >= max_transactions))
        {
            connector_debug_line("msg_create_session: active transactions reached the limit [%d], service id [%d]", active_transactions, max_transactions, service_id);
            goto done;
        }
    }

    if (client_owned == connector_true)
    {
        session_id = msg_find_next_available_id(msg_ptr);
        if (session_id == MSG_INVALID_CLIENT_SESSION) goto done;
        MsgSetClientOwned(flags);
    }

    MsgSetRequest(flags);
    #if (defined CONNECTOR_COMPRESSION)
    MsgSetCompression(flags);
    #endif

    switch (service_id)
    {
    case msg_service_id_rci:
    case msg_service_id_brci:
        MsgSetDoubleBuf(flags);
        break;

    default:
        break;
    }

    {
        void * ptr;
        uint8_t * data_ptr;
        size_t const bytes_in_block = sizeof(msg_data_block_t);
        size_t const bytes_in_service_data = sizeof(msg_service_data_t);
        size_t const bytes_in_session = sizeof *session;
        size_t const single_buffer_bytes = bytes_in_block + bytes_in_service_data;
        size_t const double_buffer_bytes = MsgIsCompressed(flags) ? 2 * single_buffer_bytes : (2 * single_buffer_bytes) + MSG_MAX_SEND_PACKET_SIZE;
        size_t const total_bytes = bytes_in_session + (MsgIsDoubleBuf(flags) ? double_buffer_bytes : single_buffer_bytes);
        connector_static_buffer_id_t buffer_id = client_owned == connector_true ? named_buffer_id(msg_session_client) : named_buffer_id(msg_session);

        *status = malloc_data_buffer(connector_ptr, total_bytes, buffer_id, &ptr);
        if (*status != connector_working) goto done;

        data_ptr = ptr;
        session = ptr;
        data_ptr += bytes_in_session;

        if (MsgIsDoubleBuf(flags))
        {
            session->out_dblock = (msg_data_block_t *)(session + 1);
            session->in_dblock = session->out_dblock + 1;
            data_ptr += (2 * bytes_in_block);
            session->service_layer_data.have_data = (msg_service_data_t *)(session->in_dblock + 1);
            session->service_layer_data.need_data = session->service_layer_data.have_data + 1;
            data_ptr += (2 * bytes_in_service_data);
            session->send_data_ptr = MsgIsCompressed(flags) ? NULL : data_ptr;
        }
        else
        {
            msg_data_block_t ** const current_dblock_ptr = client_owned == connector_true ? &session->out_dblock : &session->in_dblock;
            msg_data_block_t ** const other_dblock_ptr = client_owned == connector_true ? &session->in_dblock : &session->out_dblock;

            *current_dblock_ptr = (msg_data_block_t *)(session + 1);
            *other_dblock_ptr = NULL;
            data_ptr += bytes_in_block;
            session->service_layer_data.need_data = (msg_service_data_t *)(client_owned == connector_true ? (*current_dblock_ptr) + 1 : NULL);
            session->service_layer_data.have_data = (msg_service_data_t *)(client_owned == connector_true ? NULL : (*current_dblock_ptr) + 1);
            session->send_data_ptr = NULL;
        }
    }

    session->session_id = session_id;
    session->service_id = service_id;
    session->error = connector_session_error_none;
    session->service_layer_data.error_value = connector_session_error_none;
#if (defined CONNECTOR_DATA_SERVICE)
    session->service_layer_data.send_data_initiator = connector_send_data_initiator_unknown;
#endif
    session->error_flag = 0;
    session->send_data_bytes = 0;
    session->service_context = NULL;
    session->current_state = msg_state_init;
    session->saved_state = msg_state_init;

    if (session->out_dblock != NULL)
        session->out_dblock->status_flag = flags;

    if (session->in_dblock != NULL)
        session->in_dblock->status_flag = flags;

    if (msg_ptr->session_locked) goto error;
    msg_ptr->session_locked = connector_true;
    add_list_node(&msg_ptr->session.head, &msg_ptr->session.tail, session);
    msg_ptr->session_locked = connector_false;

    msg_ptr->capabilities[capability_id].active_transactions++;
    *status = connector_working;
    goto done;

error:
    {
        connector_status_t const result = free_data_buffer(connector_ptr, named_buffer_id(msg_session), session);

        *status = (result == connector_abort) ? connector_abort : connector_pending;
        session = NULL;
    }

done:
    return session;
}

STATIC connector_status_t msg_delete_session(connector_data_t * const connector_ptr, connector_msg_data_t * const msg_ptr, msg_session_t * const session)
{
    connector_status_t status = connector_working;

    ASSERT_GOTO(msg_ptr != NULL, error);
    ASSERT_GOTO(session != NULL, error);
    ASSERT_GOTO((session->in_dblock != NULL) || (session->out_dblock != NULL), error);

    if (msg_ptr->session_locked) goto error;
    msg_ptr->session_locked = connector_true;
    remove_list_node(&msg_ptr->session.head, &msg_ptr->session.tail, session);
    if (msg_ptr->session.current == session)
        msg_ptr->session.current = (session->prev != NULL) ? session->prev : msg_ptr->session.tail;
    msg_ptr->session_locked = connector_false;

    #if (defined CONNECTOR_COMPRESSION)
    {
        if ((session->in_dblock != NULL) && MsgIsInflated(session->in_dblock->status_flag))
            inflateEnd(&session->in_dblock->zlib);

        if ((session->out_dblock != NULL) && MsgIsDeflated(session->out_dblock->status_flag))
            deflateEnd(&session->out_dblock->zlib);
    }
    #endif

    {
        unsigned int const flag = (session->in_dblock != NULL) ? session->in_dblock->status_flag : session->out_dblock->status_flag;
        msg_capability_type_t const capability_id = MsgIsClientOwned(flag) == connector_true ? msg_capability_cloud : msg_capability_client;

        if (msg_ptr->capabilities[capability_id].active_transactions > 0)
        {
            msg_ptr->capabilities[capability_id].active_transactions--;
        }
        else
        {
            ASSERT(connector_false);
        }
    }

    status = msg_call_service_layer(connector_ptr, session, msg_service_type_free);

    {
        connector_status_t const free_status = free_data_buffer(connector_ptr, named_buffer_id(msg_session), session);

        if (status != connector_abort)
            status = free_status;
    }

error:
    return status;
}

STATIC void msg_default_data_block(msg_data_block_t * dblock, uint32_t const window_size)
{
    #if (defined CONNECTOR_COMPRESSION)
    dblock->bytes_out = 0;
    dblock->z_flag = Z_NO_FLUSH;
    #endif

    dblock->available_window = window_size;
    dblock->ack_count = 0;
    dblock->total_bytes = 0;
    MsgSetStart(dblock->status_flag);
    MsgClearLastData(dblock->status_flag);
    MsgClearAckPending(dblock->status_flag);
}

STATIC connector_session_error_t msg_initialize_data_block(msg_session_t * const session, uint32_t const window_size, msg_block_state_t state)
{
    connector_session_error_t result = connector_session_error_none;

    ASSERT_GOTO(session != NULL, error);

    switch(state)
    {
    case msg_block_state_send_response:
        if (session->out_dblock == NULL)
        {
            #if (defined CONNECTOR_COMPRESSION)
            ASSERT_GOTO(session->in_dblock != NULL, compression_error);
            inflateEnd(&session->in_dblock->zlib);
            MsgClearInflated(session->in_dblock->status_flag);
            #endif
            session->out_dblock = session->in_dblock;
            ASSERT_GOTO(session->service_layer_data.have_data != NULL, error);
            session->service_layer_data.need_data = session->service_layer_data.have_data;
            session->in_dblock = NULL;
            session->service_layer_data.have_data = NULL;
        }
        MsgClearRequest(session->out_dblock->status_flag);
        /* intentional fall through */

    case msg_block_state_send_request:
        msg_default_data_block(session->out_dblock, window_size);
        MsgClearReceiving(session->out_dblock->status_flag);
        session->current_state = msg_state_get_data;
        session->service_layer_data.need_data->data_ptr = NULL;
        #if (defined CONNECTOR_COMPRESSION)
        if (MsgIsNotDeflated(session->out_dblock->status_flag) == connector_true)
        {
            memset(&session->out_dblock->zlib, 0, sizeof session->out_dblock->zlib);
            ASSERT_GOTO(deflateInit(&session->out_dblock->zlib, Z_DEFAULT_COMPRESSION) == Z_OK, compression_error);
            MsgSetDeflated(session->out_dblock->status_flag);
        }
        #endif
        break;

    case msg_block_state_recv_response:
        if (session->in_dblock == NULL)
        {
            #if (defined CONNECTOR_COMPRESSION)
            ASSERT_GOTO(session->out_dblock != NULL, compression_error);
            deflateEnd(&session->out_dblock->zlib);
            MsgClearDeflated(session->out_dblock->status_flag);
            #endif
            session->in_dblock = session->out_dblock;
            ASSERT_GOTO(session->service_layer_data.need_data != NULL, error);
            session->service_layer_data.have_data = session->service_layer_data.need_data;
            MsgClearRequest(session->in_dblock->status_flag);
            session->out_dblock = NULL;
            session->service_layer_data.need_data = NULL;
        }
        /* intentional fall through */

    case msg_block_state_recv_request:
        msg_default_data_block(session->in_dblock, window_size);
        MsgSetReceiving(session->in_dblock->status_flag);
        session->current_state = msg_state_receive;
        #if (defined CONNECTOR_COMPRESSION)
        if (MsgIsNotInflated(session->in_dblock->status_flag) == connector_true)
        {
            memset(&session->in_dblock->zlib, 0, sizeof session->in_dblock->zlib);
            ASSERT_GOTO(inflateInit(&session->in_dblock->zlib) == Z_OK, compression_error);
            MsgSetInflated(session->in_dblock->status_flag);
        }
        #endif
        break;

    default:
        ASSERT_GOTO(connector_false, error);
        break;
    }

    goto done;

#if (defined CONNECTOR_COMPRESSION)
compression_error:
    result = connector_session_error_compression_failure;
#endif

error:
done:
    return result;
}

STATIC connector_status_t msg_send_error(connector_data_t * const connector_ptr, connector_msg_data_t * const msg_ptr, msg_session_t * const session, uint16_t const session_id, connector_session_error_t const error_value, uint8_t const flag)
{
    connector_status_t status = connector_working;
    uint8_t * error_packet;
    uint8_t * const edp_header = tcp_get_packet_buffer(connector_ptr, E_MSG_FAC_MSG_NUM, &error_packet, NULL);

    if (edp_header == NULL)
    {
        status = connector_pending;
        goto done;
    }

    message_store_u8(error_packet, opcode, msg_opcode_error);
    message_store_u8(error_packet, flags, flag);
    message_store_be16(error_packet, transaction_id, session_id);
    message_store_u8(error_packet, error_code, error_value);

    status = tcp_initiate_send_facility_packet(connector_ptr, edp_header, record_end(error_packet), E_MSG_FAC_MSG_NUM, tcp_release_packet_buffer, NULL);


    if ((status != connector_pending) && (session != NULL))
        status = msg_delete_session(connector_ptr, msg_ptr, session);

done:
    return status;
}

STATIC connector_status_t msg_send_capabilities(connector_data_t * const connector_ptr, connector_msg_data_t const * const msg_ptr, uint8_t const flag)
{
    connector_status_t status = connector_pending;
    uint8_t * capability_packet = NULL;
    uint8_t * const edp_header = tcp_get_packet_buffer(connector_ptr, E_MSG_FAC_MSG_NUM, &capability_packet, NULL);
    uint8_t * variable_data_ptr = capability_packet + record_bytes(capability_packet);

    if ((edp_header == NULL) || (capability_packet == NULL))
        goto error;

    /*
     * ------------------------------------------------------------------------------------------
     * |   0    |    1  |     2   |    3    |   4-7  |     8    | 9-(8+n) |     9+n   | s bytes |
     *  -----------------------------------------------------------------------------------------
     * | opcode | flags | version | Max Trx | Window | Count(n) | list    | Count (s) |  list   |
     * |        |       |         |         |  size  |  compression algo  |     Service Info    |
     *  -----------------------------------------------------------------------------------------
     */
    message_store_u8(capability_packet, opcode, msg_opcode_capability);
    message_store_u8(capability_packet, flags, flag);
    message_store_u8(capability_packet, version, MSG_FACILITY_VERSION);
    message_store_u8(capability_packet, max_transactions, msg_ptr->capabilities[msg_capability_client].max_transactions);
    message_store_be32(capability_packet, window_size, msg_ptr->capabilities[msg_capability_client].window_size);

    /* append compression algorithms supported */
    {
        uint8_t const compression_length = (msg_ptr->capabilities[msg_capability_client].compression_supported) ? 1 : 0;

        message_store_u8(capability_packet, compression_count, compression_length);
        if (compression_length > 0)
            *variable_data_ptr++ = MSG_COMPRESSION_LIBZ;
    }

    /* append service IDs of all listeners */
    {
        uint16_t services = 0;
        uint16_t service_id;

        for (service_id = 0; service_id < msg_service_id_count; service_id++)
            if (msg_ptr->service_cb[service_id] !=  NULL) services++;

        StoreBE16(variable_data_ptr, services);
        variable_data_ptr += sizeof services;

        for (service_id = 0; service_id < msg_service_id_count; service_id++)
        {
            if (msg_ptr->service_cb[service_id] !=  NULL)
            {
                StoreBE16(variable_data_ptr, service_id);
                variable_data_ptr += sizeof service_id;
            }
        }
    }

    {
        size_t const packet_len = (size_t)(variable_data_ptr - capability_packet);

        status = tcp_initiate_send_facility_packet(connector_ptr, edp_header, packet_len, E_MSG_FAC_MSG_NUM, tcp_release_packet_buffer, NULL);
    }

error:
    return status;
}

STATIC void msg_fill_msg_header(msg_session_t * const session, void * ptr)
{
    uint8_t flag = 0;

    if MsgIsRequest(session->out_dblock->status_flag)
        MsgSetRequest(flag);

    if (MsgIsLastData(session->out_dblock->status_flag))
        MsgSetLastData(flag);

    if (MsgIsStart(session->out_dblock->status_flag))
    {
        uint8_t * const start_packet = ptr;

        MsgClearStart(session->out_dblock->status_flag);

        /*
        * -----------------------------------------------------------------
        * |   0    |    1  |   2-3   |    4-5     |    6    |     7 ... n |
        *  ----------------------------------------------------------------
        * | opcode | flags |   Xid   | Service ID | Comp ID |     Data    |
        *  ----------------------------------------------------------------
        */
        message_store_u8(start_packet, opcode, msg_opcode_start);
        message_store_u8(start_packet, flags, flag);
        {
            uint16_t const session_id = (uint16_t)session->session_id;
            uint16_t const service_id = (uint16_t)session->service_id;

            message_store_be16(start_packet, transaction_id, session_id);
            message_store_be16(start_packet, service_id, service_id);
        }
        message_store_u8(start_packet, compression_id, (MsgIsCompressed(session->out_dblock->status_flag) ? MSG_COMPRESSION_LIBZ : MSG_COMPRESSION_NONE));
    }
    else
    {
        uint8_t * const data_packet = ptr;

        /*
         * ---------------------------------------
         * |   0    |    1  |   2-3   |  4 ... n |
         *  --------------------------------------
         * | opcode | flags |   Xid   |  Data    |
         *  --------------------------------------
         */
        message_store_u8(data_packet, opcode, msg_opcode_data);
        message_store_u8(data_packet, flags, flag);
        {
            uint16_t const session_id = (uint16_t)session->session_id;

            message_store_be16(data_packet, transaction_id, session_id);
        }
    }
}

STATIC connector_status_t msg_send_complete(connector_data_t * const connector_ptr, uint8_t const * const packet, connector_status_t const status, void * const user_data)
{
    connector_status_t return_status = connector_working;

    msg_session_t * const session = user_data;
    msg_data_block_t * const dblock = session->out_dblock;

    ASSERT_GOTO(session != NULL, error);
    ASSERT_GOTO(dblock != NULL, error);

    if ((MsgIsDoubleBuf(dblock->status_flag) == connector_false) && (MsgIsCompressed(dblock->status_flag) == connector_false))
    {
        return_status = tcp_release_packet_buffer(connector_ptr, packet, connector_success, NULL);
        if (return_status != connector_working) goto error;
    }

    switch (status)
    {
        case connector_service_busy:
            session->current_state = msg_state_send_data;
            break;

        case connector_success:
            /* update session state */
            if (MsgIsLastData(dblock->status_flag))
            {
                if (MsgIsRequest(dblock->status_flag))
                {
                    session->current_state = msg_state_receive;
                }
                else
                {
                    connector_msg_data_t * const msg_ptr = get_facility_data(connector_ptr, E_MSG_FAC_MSG_NUM);

                    return_status = msg_delete_session(connector_ptr, msg_ptr, session);
                }

                goto done;
            }

            #if (defined CONNECTOR_COMPRESSION)
            dblock->bytes_out = 0;
            if (dblock->zlib.avail_out == 0)
            {
                session->current_state = msg_state_compress;
                goto done;
            }

            dblock->z_flag = Z_NO_FLUSH;
            dblock->zlib.avail_out = 0;
            #else
            session->send_data_bytes = 0;
            #endif

            session->service_layer_data.need_data->data_ptr = NULL;
            session->current_state = session->saved_state;

            break;

        default:
            msg_inform_error(connector_ptr, session, connector_session_error_send);
            break;
    }

error:
done:
    return return_status;
}

STATIC connector_status_t msg_send_data(connector_data_t * const connector_ptr, msg_session_t * const session)
{
    connector_status_t status = connector_working;
    uint8_t * buffer;
    size_t bytes;

    #if (defined CONNECTOR_COMPRESSION)
    {
        msg_data_block_t * const dblock = session->out_dblock;

        bytes = dblock->bytes_out;
        buffer = dblock->buffer_out;
    }
    #else
    bytes = session->send_data_bytes;
    buffer = session->send_data_ptr;
    #endif

    ASSERT_GOTO(bytes > 0, error);
    status = tcp_initiate_send_facility_packet(connector_ptr, buffer, bytes, E_MSG_FAC_MSG_NUM, msg_send_complete, session);
    if (status != connector_working)
    {
        connector_status_t result = status;

        if (status == connector_pending) result = connector_service_busy;

        msg_send_complete(connector_ptr, buffer, result, session);
    }
    else
        session->current_state = msg_state_wait_send_complete;

error:
    return status;
}

#if (defined CONNECTOR_COMPRESSION)
STATIC connector_status_t msg_compress_data(connector_data_t * const connector_ptr, msg_session_t * const session)
{
    connector_status_t status = connector_working;
    msg_data_block_t * const dblock = session->out_dblock;
    uint8_t * const msg_buffer = GET_PACKET_DATA_POINTER(dblock->buffer_out, PACKET_EDP_FACILITY_SIZE);
    size_t const frame_bytes = sizeof dblock->buffer_out - PACKET_EDP_FACILITY_SIZE;
    z_streamp zlib_ptr = &dblock->zlib;
    int zret;

    if (zlib_ptr->avail_out == 0)
    {
        size_t const header_length = MsgIsStart(dblock->status_flag) ? record_end(start_packet) : record_end(data_packet);

        zlib_ptr->next_out = msg_buffer + header_length;
        zlib_ptr->avail_out = frame_bytes - header_length;
    }

    zret = deflate(zlib_ptr, dblock->z_flag);
    switch (zret)
    {
    case Z_OK:
        if ((dblock->z_flag != Z_SYNC_FLUSH) && (zlib_ptr->avail_out > 0))
            goto done;
        break;

    case Z_STREAM_END:
        MsgSetLastData(dblock->status_flag);
        break;

    default:
        status = msg_inform_error(connector_ptr, session, connector_session_error_compression_failure);
        goto done;
    }

    msg_fill_msg_header(session, msg_buffer);
    dblock->bytes_out = frame_bytes - zlib_ptr->avail_out;
    status = msg_send_data(connector_ptr, session);

done:
    return status;
}

STATIC connector_status_t msg_prepare_send_data(connector_data_t * const connector_ptr, msg_session_t * const session)
{
    connector_status_t status = connector_no_resource;
    msg_data_block_t * const dblock = session->out_dblock;

    UNUSED_PARAMETER(connector_ptr);
    ASSERT_GOTO(dblock != NULL, error);
    ASSERT_GOTO(dblock->zlib.avail_in == 0, error);

    {
        unsigned int const flag = (dblock->total_bytes == 0) ? MSG_FLAG_START : 0;
        msg_service_data_t * const service_data = session->service_layer_data.need_data;

        ASSERT_GOTO(service_data != NULL, error);
        service_data->data_ptr = dblock->buffer_in;
        service_data->length_in_bytes = sizeof dblock->buffer_in;
        service_data->flags = flag;
        status = connector_working;
    }

error:
    return status;
}

STATIC connector_status_t msg_process_send_data(connector_data_t * const connector_ptr, msg_session_t * const session)
{
    connector_status_t status = connector_no_resource;
    msg_data_block_t * const dblock = session->out_dblock;

    ASSERT_GOTO(dblock != NULL, error);
    ASSERT_GOTO(dblock->zlib.avail_in == 0, error);

    {
        z_streamp const zlib_ptr = &dblock->zlib;
        msg_service_data_t * const service_data = session->service_layer_data.need_data;

        zlib_ptr->next_in = dblock->buffer_in;
        zlib_ptr->avail_in = service_data->length_in_bytes;
        dblock->total_bytes += service_data->length_in_bytes;
        if (MsgIsLastData(service_data->flags))
            dblock->z_flag = Z_FINISH;
    }

    if (dblock->z_flag == Z_NO_FLUSH)
    {
        size_t const unacked_bytes = dblock->total_bytes - dblock->ack_count;
        size_t const safe_window_size = dblock->available_window - sizeof dblock->buffer_in;

        if (unacked_bytes > safe_window_size)
        {
            dblock->z_flag = Z_SYNC_FLUSH;
            MsgSetAckPending(dblock->status_flag);
        }
    }

    status = msg_compress_data(connector_ptr, session);

error:
    return status;
}

#else

STATIC connector_status_t msg_prepare_send_data(connector_data_t * const connector_ptr, msg_session_t * const session)
{
    uint8_t * msg_buffer;
    connector_status_t status = connector_no_resource;
    msg_data_block_t * const dblock = session->out_dblock;

    ASSERT_GOTO(dblock != NULL, error);
    if (MsgIsDoubleBuf(dblock->status_flag))
    {
        msg_buffer = GET_PACKET_DATA_POINTER(session->send_data_ptr, PACKET_EDP_FACILITY_SIZE);
        session->send_data_bytes = MSG_MAX_SEND_PACKET_SIZE - PACKET_EDP_FACILITY_SIZE;
    }
    else
    {
        session->send_data_ptr = tcp_get_packet_buffer(connector_ptr, E_MSG_FAC_MSG_NUM, &msg_buffer, &session->send_data_bytes);
        if (session->send_data_ptr == NULL)
        {
            status = connector_pending;
            goto done;
        }
    }

    {
        size_t const header_bytes = MsgIsStart(dblock->status_flag) == connector_true ? record_end(start_packet) : record_end(data_packet);
        msg_service_data_t * const service_data = session->service_layer_data.need_data;
        unsigned int const flag = MsgIsStart(dblock->status_flag) ? MSG_FLAG_START : 0;

        ASSERT_GOTO(MsgIsNotLastData(dblock->status_flag), error);
        ASSERT_GOTO(session->send_data_bytes > header_bytes, error);
        ASSERT_GOTO(service_data != NULL, error);

        service_data->data_ptr = msg_buffer + header_bytes;
        service_data->length_in_bytes = session->send_data_bytes - header_bytes;
        service_data->flags = flag;
        status = connector_working;
    }

error:
done:
    return status;
}

STATIC connector_status_t msg_process_send_data(connector_data_t * const connector_ptr, msg_session_t * const session)
{
    connector_status_t status = connector_no_resource;
    msg_data_block_t * const dblock = session->out_dblock;
    msg_service_data_t * const service_data = session->service_layer_data.need_data;

    ASSERT_GOTO(dblock != NULL, error);
    if (MsgIsLastData(service_data->flags))
        MsgSetLastData(dblock->status_flag);

    {
        size_t const header_bytes = MsgIsStart(dblock->status_flag) == connector_true ? record_end(start_packet) : record_end(data_packet);
        uint8_t * const msg_buffer = GET_PACKET_DATA_POINTER(session->send_data_ptr, PACKET_EDP_FACILITY_SIZE);

        session->send_data_bytes = service_data->length_in_bytes + header_bytes;
        msg_fill_msg_header(session, msg_buffer);
        status = msg_send_data(connector_ptr, session);
        if (status != connector_working)
            goto error;
    }

    dblock->total_bytes += service_data->length_in_bytes;
    if ((dblock->total_bytes - dblock->ack_count) > (dblock->available_window - MSG_MAX_SEND_PACKET_SIZE))
        MsgSetAckPending(dblock->status_flag);

error:
    return status;
}

#endif

STATIC connector_status_t msg_get_service_data(connector_data_t * const connector_ptr, msg_session_t * const session)
{
    connector_status_t status = connector_no_resource;
    msg_data_block_t * const dblock = session->out_dblock;

    ASSERT_GOTO(dblock != NULL, error);
    ASSERT_GOTO(MsgIsNotLastData(dblock->status_flag), done);

    status = msg_prepare_send_data(connector_ptr, session);
    if (status != connector_working)
        goto error;

    status = msg_call_service_layer(connector_ptr, session, msg_service_type_need_data);
    if (status != connector_working)
    {
        /* release the buffer */
        if ((MsgIsDoubleBuf(dblock->status_flag) == connector_false) && (MsgIsCompressed(dblock->status_flag) == connector_false))
            tcp_release_packet_buffer(connector_ptr, session->send_data_ptr, connector_success, NULL);

        goto done;
    }

    session->saved_state = session->current_state;
    status = msg_process_send_data(connector_ptr, session);

error:
done:
    return status;
}

#if (defined CONNECTOR_DATA_SERVICE)
typedef enum {
    MSG_REQUEST_USER,
    MSG_REQUEST_INTERNAL
} msg_request_initiator_t;

STATIC connector_bool_t msg_initiate_request(connector_data_t * const connector_ptr, void const * const service_context, msg_request_initiator_t initiator)
{
    connector_bool_t success = connector_false;
    connector_msg_data_t * const msg_ptr = get_facility_data(connector_ptr, E_MSG_FAC_MSG_NUM);

    ASSERT_GOTO(msg_ptr != NULL, error);
    switch (initiator)
    {
        case MSG_REQUEST_USER:
            if (msg_ptr->pending_service_request.user == NULL)
            {
                msg_ptr->pending_service_request.user = service_context;
                success = connector_true;
            }
            break;
        case MSG_REQUEST_INTERNAL:
            if (msg_ptr->pending_service_request.internal == NULL)
            {
                msg_ptr->pending_service_request.internal = service_context;
                success = connector_true;
            }
            break;
    }

error:
    return success;
}

STATIC connector_status_t msg_handle_pending_requests(connector_data_t * const connector_ptr, connector_msg_data_t * const msg_ptr, msg_session_t * const session, connector_session_error_t const result)
{
    connector_status_t status = connector_working;
    connector_msg_callback_t * const cb_fn = msg_ptr->service_cb[msg_service_id_data];
    void const * * pending_request = NULL;

    if (msg_ptr->pending_service_request.user != NULL)
    {
        pending_request = &msg_ptr->pending_service_request.user;
    }
    else if (msg_ptr->pending_service_request.internal != NULL)
    {
        pending_request = &msg_ptr->pending_service_request.internal;
    }

    if (cb_fn != NULL)
    {
        msg_service_request_t service_data;

        service_data.session = session;
        service_data.service_type = msg_service_type_pending_request;
        service_data.error_value = result;
        service_data.have_data = pending_request != NULL ? (void *)*pending_request : NULL;
#if (defined CONNECTOR_DATA_SERVICE)
        service_data.send_data_initiator = connector_send_data_initiator_unknown;
#endif
        cb_fn(connector_ptr, &service_data);
        if ((service_data.error_value != connector_session_error_none) && (session != NULL))
        {
            status = msg_delete_session(connector_ptr, msg_ptr, session);
        }
#if (defined CONNECTOR_DATA_SERVICE)
        else if (session != NULL)
        {
            msg_service_request_t * const session_service_data = &session->service_layer_data;

            session_service_data->send_data_initiator = service_data.send_data_initiator;
        }
#endif
    }

    if (pending_request != NULL)
    {
        *pending_request = NULL;
    }

    return status;
}

STATIC connector_status_t msg_start_session(connector_data_t * const connector_ptr, connector_msg_data_t * const msg_ptr)
{
    connector_status_t status = connector_working;
    static connector_bool_t const client_owned = connector_true;
    msg_session_t * const session = msg_create_session(connector_ptr, msg_ptr, msg_service_id_data, client_owned, &status);

    if (session == NULL) goto error;

    {
        connector_session_error_t const result = msg_initialize_data_block(session, msg_ptr->capabilities[msg_capability_cloud].window_size, msg_block_state_send_request);

        status = msg_handle_pending_requests(connector_ptr, msg_ptr, session, result);
    }

error:
    return status;
}
#endif

STATIC connector_status_t msg_send_ack(connector_data_t * const connector_ptr, connector_msg_data_t * const msg_ptr, msg_session_t * const session)
{
    connector_status_t status = connector_pending;
    uint8_t * ack_packet;
    uint8_t * const edp_packet = tcp_get_packet_buffer(connector_ptr, E_MSG_FAC_MSG_NUM, &ack_packet, NULL);
    msg_data_block_t * const dblock = session->in_dblock;

    if (edp_packet == NULL)
        goto done;

    ASSERT_GOTO(dblock != NULL, error);
    message_store_u8(ack_packet, opcode, msg_opcode_ack);
    {
        uint8_t const flag = MsgIsClientOwned(dblock->status_flag) ? 0 : MSG_FLAG_REQUEST;

        message_store_u8(ack_packet, flags, flag);
    }
    {
        uint16_t const session_id = (uint16_t)session->session_id;

        message_store_be16(ack_packet, transaction_id, session_id);
    }
    {
        uint32_t const val32 = (uint32_t) dblock->total_bytes;

        ASSERT(dblock->total_bytes <= UINT32_MAX);
        message_store_be32(ack_packet, ack_count, val32);
    }

    message_store_be32(ack_packet, window_size, msg_ptr->capabilities[msg_capability_client].window_size);

    status = tcp_initiate_send_facility_packet(connector_ptr, edp_packet, record_end(ack_packet),
                                           E_MSG_FAC_MSG_NUM, tcp_release_packet_buffer, NULL);
    if (status != connector_pending)
    {
        dblock->ack_count = dblock->total_bytes;
        MsgClearAckPending(dblock->status_flag);
        session->current_state = msg_state_receive;
    }

error:
done:
    return status;
}

STATIC connector_status_t msg_process_capabilities(connector_data_t * const connector_ptr, connector_msg_data_t * const msg_fac, uint8_t * const ptr)
{
    connector_status_t status = connector_working;
    uint8_t * capability_packet = ptr;
    connector_bool_t const request_capabilities = connector_bool((message_load_u8(capability_packet, flags) & MSG_FLAG_REQUEST) == MSG_FLAG_REQUEST);
    uint8_t const version = message_load_u8(capability_packet, version);

    ASSERT_GOTO(version == MSG_FACILITY_VERSION, error);
    msg_fac->capabilities[msg_capability_cloud].max_transactions = message_load_u8(capability_packet, max_transactions);
    msg_fac->capabilities[msg_capability_cloud].window_size = message_load_be32(capability_packet, window_size);

    {
        uint8_t const comp_count = message_load_u8(capability_packet, compression_count);
        uint8_t const * compression_list = capability_packet + record_bytes(capability_packet);
        int i;

        msg_fac->capabilities[msg_capability_cloud].compression_supported = connector_false;
        for (i = 0; i < comp_count; i++)
        {
            uint8_t const compression_id = *compression_list++;

            if (compression_id == MSG_COMPRESSION_LIBZ)
            {
                msg_fac->capabilities[msg_capability_cloud].compression_supported = connector_true;
                break;
            }
        }
    }

    if (request_capabilities)
    {
        uint8_t const capability_flag = 0;

        status = msg_send_capabilities(connector_ptr, msg_fac, capability_flag);
    }

error:
    return status;
}

STATIC connector_status_t msg_pass_service_data(connector_data_t * const connector_ptr, msg_session_t * const session, void * data, size_t const bytes)
{
    connector_status_t status = connector_working;
    msg_data_block_t * const dblock = session->in_dblock;

    ASSERT_GOTO(session != NULL, error);
    ASSERT_GOTO(dblock != NULL, error);

    {
        unsigned int service_flag = 0;
        msg_service_data_t * const service_data = session->service_layer_data.have_data;

        ASSERT_GOTO(service_data != NULL, error);
        if (MsgIsStart(dblock->status_flag))
            MsgSetStart(service_flag);

        if (MsgIsLastData(dblock->status_flag))
            MsgSetLastData(service_flag);

        service_data->data_ptr = data;
        service_data->length_in_bytes = bytes;
        service_data->flags = service_flag;
        if ((MsgIsDoubleBuf(dblock->status_flag)) && (session->service_layer_data.need_data->data_ptr == NULL))
        {
            status = msg_prepare_send_data(connector_ptr, session);
            ASSERT_GOTO(status == connector_working, error);
        }
        status = msg_call_service_layer(connector_ptr, session, msg_service_type_have_data);
    }

    if (status != connector_working && status != connector_idle && status != connector_pending && status != connector_active)
        goto error;

    if (status == connector_working)
    {
        MsgClearStart(dblock->status_flag);

        if (MsgIsLastData(dblock->status_flag))
        {
            if (MsgIsRequest(dblock->status_flag))
            {
                connector_msg_data_t * const msg_ptr = get_facility_data(connector_ptr, E_MSG_FAC_MSG_NUM);
                connector_session_error_t result;

                ASSERT_GOTO(msg_ptr != NULL, error);
                if (MsgIsDoubleBuf(dblock->status_flag) == connector_false)
                {
                    result = msg_initialize_data_block(session, msg_ptr->capabilities[msg_capability_cloud].window_size, msg_block_state_send_response);
                    if (result != connector_session_error_none)
                        status = msg_inform_error(connector_ptr, session, result);
                }
                else
                {
                    MsgClearRequest(dblock->status_flag);
                    MsgClearRequest(session->out_dblock->status_flag);
                }
            }
            else
                session->current_state = msg_state_delete;
        }
        else
        {
            dblock->total_bytes += bytes;

            if ((dblock->total_bytes - dblock->ack_count) > (dblock->available_window/2))
            {
                MsgSetAckPending(dblock->status_flag);
                session->current_state = msg_state_send_ack;
            }
            else
                session->current_state = msg_state_receive;
        }
    }

    if (MsgIsDoubleBuf(dblock->status_flag))
    {
        ASSERT_GOTO(session->service_layer_data.need_data != NULL, error);
        session->saved_state = session->current_state;

        if (MsgIsSendNow(session->service_layer_data.need_data->flags))
        {
            connector_status_t const send_status = msg_process_send_data(connector_ptr, session);

            ASSERT_GOTO(((send_status == connector_working) || (send_status == connector_pending) || (send_status == connector_active)), error);
            session->service_layer_data.need_data->data_ptr = NULL;
        }
    }

error:
    return status;
}

#if (defined CONNECTOR_COMPRESSION)
STATIC connector_status_t msg_process_decompressed_data(connector_data_t * const connector_ptr, msg_session_t * const session)
{
    connector_status_t status = connector_working;
    msg_data_block_t * const dblock = session->in_dblock;

    ASSERT_GOTO(dblock != NULL, error);
    ASSERT_GOTO(dblock->bytes_out > 0, error);

    status = msg_pass_service_data(connector_ptr, session, dblock->buffer_out, dblock->bytes_out);
    if (status ==  connector_working)
    {
        dblock->bytes_out = 0;
        if (session->current_state != msg_state_get_data)
        {
            if (dblock->zlib.avail_out == 0)
                session->current_state = msg_state_decompress;
            else
            {
                if (MsgIsAckPending(dblock->status_flag))
                    session->current_state = msg_state_send_ack;
            }
        }
    }
    else if ((status == connector_active) || (status == connector_pending))
    {
        session->current_state = msg_state_process_decompressed;
        status = connector_working;
    }

error:
    return status;
}

STATIC connector_status_t msg_decompress_data(connector_data_t * const connector_ptr, msg_session_t * const session)
{
    connector_status_t status = connector_working;
    msg_data_block_t * const dblock = session->in_dblock;
    z_streamp zlib_ptr;

    ASSERT_GOTO(dblock != NULL, error);
    zlib_ptr = &dblock->zlib;

    if (zlib_ptr->avail_out == 0)
    {
        zlib_ptr->next_out = dblock->buffer_out;
        zlib_ptr->avail_out = sizeof dblock->buffer_out;
    }

    {
        int const zret = inflate(zlib_ptr, Z_NO_FLUSH);

        session->current_state = MsgIsAckPending(dblock->status_flag) ? msg_state_send_ack : msg_state_receive;
        switch(zret)
        {
        case Z_BUF_ERROR:
            if (zlib_ptr->avail_in > 0)
                goto error;
            goto done;

        case Z_OK:
            if (zlib_ptr->avail_out > 0)
                goto done;
            break;

        case Z_STREAM_END:
            MsgSetLastData(dblock->status_flag);
            break;

        default:
            goto error;
        }

        dblock->bytes_out = sizeof dblock->buffer_out - zlib_ptr->avail_out;
        status = msg_process_decompressed_data(connector_ptr, session);
    }

    goto done;

error:
    status = msg_inform_error(connector_ptr, session, connector_session_error_decompression_failure);

done:
    return status;
}

STATIC connector_status_t msg_process_compressed_data(connector_data_t * const connector_ptr, msg_session_t * const session, uint8_t * data, size_t const bytes)
{
    connector_status_t status = connector_working;
    msg_data_block_t * const dblock = session->in_dblock;
    z_streamp zlib_ptr;

    ASSERT_GOTO(dblock != NULL, error);
    zlib_ptr = &dblock->zlib;

    if (zlib_ptr->avail_in > 0)
    {
        status = connector_pending;
        goto done;
    }

    ASSERT_GOTO(bytes < sizeof dblock->buffer_in, error);
    memcpy(dblock->buffer_in, data, bytes);

    zlib_ptr->next_in = dblock->buffer_in;
    zlib_ptr->avail_in = bytes;

    status = msg_decompress_data(connector_ptr, session);

error:
done:
    return status;
}
#endif

STATIC connector_status_t msg_process_service_data(connector_data_t * const connector_ptr, msg_session_t * const session, uint8_t * msg_data, size_t const frame_bytes, size_t const header_bytes, unsigned const flag)
{
    connector_status_t status = connector_working;
    uint8_t * buffer = msg_data + header_bytes;
    size_t const bytes = frame_bytes - header_bytes;
    msg_data_block_t * const dblock = session->in_dblock;

    ASSERT_GOTO(dblock != NULL, error);
    if (session->current_state != msg_state_receive)
    {
        status = ((session->current_state == msg_state_send_error) || (session->current_state == msg_state_delete)) ? connector_working : connector_active;
        goto done;
    }

    #if (defined CONNECTOR_COMPRESSION)
    if (MsgIsCompressed(dblock->status_flag))
    {
        status = msg_process_compressed_data(connector_ptr, session, buffer, bytes);
        goto done;
    }
    #endif

    if (MsgIsLastData(flag))
        MsgSetLastData(dblock->status_flag);

    status = msg_pass_service_data(connector_ptr, session, buffer, bytes);

error:
done:
    return status;
}

STATIC connector_status_t msg_process_start(connector_data_t * const connector_ptr, connector_msg_data_t * const msg_ptr, uint8_t * ptr, uint16_t const length)
{
    connector_status_t status = connector_working;
    connector_session_error_t result = connector_session_error_none;
    msg_session_t * session;
    uint8_t * start_packet = ptr;
    uint8_t flag = message_load_u8(start_packet, flags);
    uint16_t const session_id = message_load_be16(start_packet, transaction_id);
    connector_bool_t const client_owned = MsgIsNotRequest(flag);

    session = msg_find_session(msg_ptr, session_id, client_owned);
    if (session == NULL)
    {
        uint16_t const service_id = message_load_be16(start_packet, service_id);

        if (client_owned)
        {
             result = connector_session_error_unknown_session;
             goto error;
        }

        session = msg_create_session(connector_ptr, msg_ptr, service_id, client_owned, &status);
        if (session == NULL)
        {
            switch (status)
            {
                case connector_pending:
                    result = connector_session_error_busy;
                    status = connector_working;
                    break;

                default:
                    result = connector_session_error_fatal;
                    break;
            }

            goto error;
        }

        session->session_id = session_id;
        if (session->out_dblock != NULL)
        {
            result = msg_initialize_data_block(session, msg_ptr->capabilities[msg_capability_cloud].window_size, msg_block_state_send_response);
            if (result != connector_session_error_none)
                goto error;
        }

        result = msg_initialize_data_block(session, msg_ptr->capabilities[msg_capability_client].window_size, msg_block_state_recv_request);
        if (result != connector_session_error_none)
            goto error;
    }
    else
    {
        if (session->current_state == msg_state_send_error)
            goto done;

        if (client_owned)
        {
            result = msg_initialize_data_block(session, msg_ptr->capabilities[msg_capability_client].window_size, msg_block_state_recv_response);
            if (result != connector_session_error_none)
                goto error;
        }
    }

    {
        uint8_t const compression = message_load_u8(start_packet, compression_id);

        #if (defined CONNECTOR_COMPRESSION)
        if (compression == MSG_COMPRESSION_NONE)
            MsgClearCompression(session->in_dblock->status_flag);
        #else
        if (compression != MSG_COMPRESSION_NONE)
        {
            result = connector_session_error_decompression_failure;
            goto error;
        }
        #endif
    }

    MsgSetStart(flag);
    status = msg_process_service_data(connector_ptr, session, ptr, length, record_end(start_packet), flag);
    goto done;

error:
    {
        connector_status_t error_status = (session != NULL) ? msg_inform_error(connector_ptr, session, result) : msg_send_error(connector_ptr, msg_ptr, session, session_id, result, flag);

        if (error_status == connector_abort)
            status = connector_abort;
    }

done:
    return status;
}

STATIC connector_status_t msg_process_data(connector_data_t * const connector_ptr, connector_msg_data_t * const msg_ptr, uint8_t * ptr, uint16_t const length)
{
    connector_status_t status = connector_working;
    msg_session_t * session;
    uint8_t * const data_packet = ptr;
    uint8_t const flag = message_load_u8(data_packet, flags);
    uint16_t const session_id = message_load_be16(data_packet, transaction_id);
    connector_bool_t const client_owned = MsgIsNotRequest(flag);

    session = msg_find_session(msg_ptr, session_id, client_owned);
    if (session == NULL)
    {
        status = msg_send_error(connector_ptr, msg_ptr, session, session_id, connector_session_error_unknown_session, flag);
        goto done;
    }

    status = msg_process_service_data(connector_ptr, session, ptr, length, record_end(data_packet), flag);

done:
    return status;
}

STATIC connector_status_t msg_process_ack(connector_msg_data_t * const msg_fac, uint8_t const * ptr)
{
    msg_session_t * session;
    uint8_t const * const ack_packet = ptr;

    {
        uint16_t const session_id = message_load_be16(ack_packet, transaction_id);
        uint8_t const flag = message_load_u8(ack_packet, flags);
        connector_bool_t const client_owned = MsgIsRequest(flag);

        session = msg_find_session(msg_fac, session_id, client_owned);
        /* already closed? done sending all data */
        if (session == NULL)
            goto done;
    }

    {
        msg_data_block_t * const dblock = session->out_dblock;

        ASSERT_GOTO(dblock != NULL, error);
        dblock->available_window = message_load_be32(ack_packet, window_size);
        dblock->ack_count = message_load_be32(ack_packet, ack_count);

        if (dblock->available_window > 0)
        {
            if (MsgIsAckPending(dblock->status_flag))
            {
                MsgClearAckPending(dblock->status_flag);
            }
        }
    }

error:
done:
    return connector_working;
}

STATIC connector_status_t msg_process_error(connector_data_t * const connector_ptr, connector_msg_data_t * const msg_fac, uint8_t * const ptr)
{
    connector_status_t status = connector_working;
    uint8_t * const error_packet = ptr;
    uint8_t const flag = message_load_u8(error_packet, flags);
    connector_bool_t const client_request_error = connector_bool(MsgIsRequest(flag) && MsgIsNotSender(flag));
    connector_bool_t const cloud_response_error = connector_bool(MsgIsNotRequest(flag) && MsgIsSender(flag));
    connector_bool_t const client_owned = connector_bool(client_request_error || cloud_response_error);
    uint16_t const session_id = message_load_be16(error_packet, transaction_id);
    msg_session_t * const session = msg_find_session(msg_fac, session_id, client_owned);

    if (session != NULL)
    {
        if ((session->current_state != msg_state_delete) && (session->current_state != msg_state_send_error))
        {
            uint8_t const error_val = message_load_u8(error_packet, error_code);

            ASSERT_GOTO(error_val < connector_session_error_count, error);
            {
                connector_session_error_t const msg_error = (connector_session_error_t)error_val;

                status = msg_inform_error(connector_ptr, session, msg_error);
                if ((status != connector_pending) && (status != connector_abort))
                    status = msg_delete_session(connector_ptr, msg_fac, session);
            }
        }
    }
    else
    {
        connector_debug_line("msg_process_error: unable to find session id = %d", session_id);
    }

error:
    return status;
}

STATIC connector_status_t msg_discovery(connector_data_t * const connector_ptr, void * const facility_data, uint8_t * const packet, unsigned int * const receive_timeout)
{
    uint8_t const capability_flag = MSG_FLAG_REQUEST;

    UNUSED_PARAMETER(packet);
    UNUSED_PARAMETER(receive_timeout);

    return msg_send_capabilities(connector_ptr, facility_data, capability_flag);
}

STATIC void msg_switch_session(connector_msg_data_t * const msg_ptr, msg_session_t * const session)
{
    if (!msg_ptr->session_locked)
    {
        msg_ptr->session_locked = connector_true;
        msg_ptr->session.current = (session->prev != NULL) ? session->prev : msg_ptr->session.tail;
        msg_ptr->session_locked = connector_false;
    }
}

STATIC connector_status_t msg_process_pending(connector_data_t * const connector_ptr, connector_msg_data_t * const msg_ptr, unsigned int * const receive_timeout)
{
    connector_status_t status = connector_idle;

    if (msg_ptr->session_locked) goto done;

#if (defined CONNECTOR_DATA_SERVICE)
    if (msg_ptr->pending_service_request.user != NULL || msg_ptr->pending_service_request.internal != NULL)
    {
        status = msg_start_session(connector_ptr, msg_ptr);
        if (status != connector_working) goto done;
    }
#endif

#if (defined CONNECTOR_DATA_POINTS)
    status = dp_process_request(connector_ptr, connector_transport_tcp);
    if ((status != connector_idle) && (status != connector_working))
        goto done;
#endif

    msg_ptr->session_locked = connector_true;
    if (msg_ptr->session.current == NULL)
        msg_ptr->session.current = msg_ptr->session.tail;
    msg_ptr->session_locked = connector_false;

    *receive_timeout = MAX_RECEIVE_TIMEOUT_IN_SECONDS;
    if (msg_ptr->session.current != NULL)
    {
        msg_session_t * const session = msg_ptr->session.current;

        status = connector_working;

        *receive_timeout = MIN_RECEIVE_TIMEOUT_IN_SECONDS;
        switch (session->current_state)
        {
        case msg_state_init:
        case msg_state_receive:
        case msg_state_wait_send_complete:
            msg_switch_session(msg_ptr, session);
            break;

        case msg_state_get_data:
            if ((session->out_dblock != NULL) && MsgIsAckPending(session->out_dblock->status_flag))
            {
                msg_switch_session(msg_ptr, session);
            }
            else
            {
                session->saved_state = msg_state_get_data;
                status = msg_get_service_data(connector_ptr, session);
                if (status == connector_pending)
                    msg_switch_session(msg_ptr, session);
            }
            break;

        case msg_state_send_data:
            status = msg_send_data(connector_ptr, session);
            break;

        #if (defined CONNECTOR_COMPRESSION)
        case msg_state_compress:
            status = msg_compress_data(connector_ptr, session);
            break;

        case msg_state_decompress:
            status = msg_decompress_data(connector_ptr, session);
            break;

        case msg_state_process_decompressed:
            status = msg_process_decompressed_data(connector_ptr, session);
            break;
        #endif

        case msg_state_send_ack:
            status = msg_send_ack(connector_ptr, msg_ptr, session);
            break;

        case msg_state_send_error:
        {
            uint8_t const flag = (uint8_t)session->error_flag;
            uint16_t const session_id = (uint16_t)session->session_id;

            status = msg_send_error(connector_ptr, msg_ptr, session, session_id, session->error, flag);
            break;
        }

        case msg_state_delete:
            status = msg_delete_session(connector_ptr, msg_ptr, session);
            break;

        default:
            status = connector_init_error;
            connector_debug_line("Failed %X, state %d", session, session->current_state);
            ASSERT_GOTO(connector_false, done);
            break;
        }
    }

done:
    return status;
}

STATIC connector_status_t msg_process(connector_data_t * const connector_ptr, void * const facility_data, uint8_t * const edp_header, unsigned int * const receive_timeout)
{
    connector_status_t status = connector_idle;
    connector_msg_data_t * const msg_ptr = facility_data;

    ASSERT_GOTO(connector_ptr != NULL, error);
    ASSERT_GOTO(msg_ptr != NULL, error);

    if (edp_header != NULL)
    {
        uint8_t * const data_ptr = GET_PACKET_DATA_POINTER(edp_header, PACKET_EDP_FACILITY_SIZE);
        uint16_t const length = message_load_be16(edp_header, length);
        uint8_t const opcode = *data_ptr;

        switch (opcode)
        {
            case msg_opcode_capability:
                status = msg_process_capabilities(connector_ptr, msg_ptr, data_ptr);
                break;

            case msg_opcode_start:
                status = msg_process_start(connector_ptr, msg_ptr, data_ptr, length);
                break;

            case msg_opcode_data:
                status = msg_process_data(connector_ptr, msg_ptr, data_ptr, length);
                break;

            case msg_opcode_ack:
                status = msg_process_ack(msg_ptr, data_ptr);
                break;

            case msg_opcode_error:
                status = msg_process_error(connector_ptr, msg_ptr, data_ptr);
                break;

            default:
                connector_debug_line("msg_process: Invalid opcode");
                break;
        }
    }

    {
        connector_status_t const pending_state =  msg_process_pending(connector_ptr, msg_ptr, receive_timeout);

        if ((pending_state == connector_pending) || (pending_state == connector_working) || (pending_state == connector_active))
        {
            if (status == connector_idle)
                status = pending_state;
        }
        else
        {
            if (pending_state != connector_idle)
                status = pending_state;
        }
    }

    if (status == connector_unavailable) status = connector_working;

error:
    return status;
}

STATIC connector_status_t msg_cleanup_all_sessions(connector_data_t * const connector_ptr, uint16_t const service_id)
{
    connector_status_t status = connector_working;
    connector_msg_data_t * const msg_ptr = get_facility_data(connector_ptr, E_MSG_FAC_MSG_NUM);
    msg_session_t * session;

    ASSERT_GOTO(msg_ptr != NULL, error);
    session= msg_ptr->session.head;

    while(session != NULL)
    {
        msg_session_t * next_session = session->next;

        if (session->service_id == service_id)
        {
            if (session->current_state != msg_state_delete && session->current_state != msg_state_send_error)
                msg_inform_error(connector_ptr, session, connector_session_error_cancel);

            status = msg_delete_session(connector_ptr, msg_ptr, session);
            if (status != connector_working) goto error;
        }

        session = next_session;
    }

#if (defined CONNECTOR_DATA_SERVICE)
    if (msg_ptr->pending_service_request.user != NULL || msg_ptr->pending_service_request.internal != NULL)
    {
        status = msg_handle_pending_requests(connector_ptr, msg_ptr, NULL, connector_session_error_cancel);
    }
#endif

error:
    return status;
}

STATIC connector_status_t msg_delete_facility(connector_data_t * const connector_ptr, uint16_t const service_id)
{
    connector_status_t status = connector_no_resource;
    connector_msg_data_t * const msg_ptr = get_facility_data(connector_ptr, E_MSG_FAC_MSG_NUM);

    if (msg_ptr == NULL)
    {
        status = connector_idle;
        goto error;
    }

    ASSERT_GOTO(service_id < msg_service_id_count, error);
    ASSERT_GOTO(msg_ptr->service_cb[service_id] != NULL, error);

    msg_ptr->service_cb[service_id] = NULL;

    {
        connector_bool_t is_empty = connector_true;
        int i;

        for(i = 0; i < msg_service_id_count; i++)
        {
            if (msg_ptr->service_cb[i] != NULL)
            {
                is_empty = connector_false;
                break;
            }
        }

        status = is_empty == connector_true ? del_facility_data(connector_ptr, E_MSG_FAC_MSG_NUM) : connector_working;
    }

error:
    return status;
}


STATIC connector_status_t msg_init_facility(connector_data_t * const connector_ptr, unsigned int const facility_index, uint16_t service_id, connector_msg_callback_t callback)
{
    connector_status_t status;
    connector_msg_data_t * msg_ptr = get_facility_data(connector_ptr, E_MSG_FAC_MSG_NUM);

    if (msg_ptr == NULL)
    {
        void * fac_ptr = NULL;
        connector_config_max_transaction_t config_max_transaction;

        status = add_facility_data(connector_ptr, facility_index, E_MSG_FAC_MSG_NUM, &fac_ptr, sizeof *msg_ptr);
        ASSERT_GOTO(status == connector_working, done);
        ASSERT_GOTO(fac_ptr != NULL, done);

        msg_ptr = fac_ptr;
        memset(msg_ptr, 0, sizeof *msg_ptr);

        #if (defined CONNECTOR_COMPRESSION)
        msg_ptr->capabilities[msg_capability_client].compression_supported = connector_true;
        #endif

        #if (defined CONNECTOR_MSG_MAX_TRANSACTION)
        config_max_transaction.count = CONNECTOR_MSG_MAX_TRANSACTION;
        #else
        {
            connector_request_id_t request_id;
            connector_callback_status_t callback_status;

            request_id.config_request = connector_request_id_config_max_transaction;
            callback_status = connector_callback(connector_ptr->callback, connector_class_id_config, request_id, &config_max_transaction, connector_ptr->context);
            if (callback_status != connector_callback_continue && callback_status != connector_callback_unrecognized)
            {
                status = connector_abort;
                goto done;
            }
        }
        #endif

        msg_ptr->capabilities[msg_capability_client].max_transactions = config_max_transaction.count;
        msg_ptr->capabilities[msg_capability_client].window_size = MSG_RECV_WINDOW_SIZE;
    }

    msg_ptr->service_cb[service_id] = callback;
    status = connector_working;

done:
    return status;
}

#if (defined CONNECTOR_RCI_SERVICE)

#include "rci_service.h"

#if (!defined CONNECTOR_FIRMWARE_SERVICE)
#include "rci_fwstub.h"
#endif
#endif

