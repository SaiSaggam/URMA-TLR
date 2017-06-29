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
/* This function searches for the next valid request_id and leaves it in connector_ptr->last_request_id */
STATIC connector_status_t sm_get_request_id(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr)
{
    connector_status_t result = connector_pending;
    unsigned long const request_id = connector_ptr->last_request_id;

    do
    {
        connector_sm_session_t * session = sm_ptr->session.head;

        connector_ptr->last_request_id++;
        connector_ptr->last_request_id &= SM_REQUEST_ID_MASK;

        while (session != NULL)
        {
            /* already used? */
            if (session->request_id == connector_ptr->last_request_id)
                break;

            session = session->next;
        }

        if (session == NULL)
            break;

    } while (request_id != connector_ptr->last_request_id);

    ASSERT_GOTO(request_id != connector_ptr->last_request_id, error);
    result = connector_working;

error:
    return result;
}

STATIC connector_sm_session_t * get_sm_session(connector_sm_data_t * const sm_ptr, uint32_t const transcation_id, connector_bool_t const client_originated)
{
    connector_sm_session_t * session = sm_ptr->session.head;

    while (session != NULL)
    {
        if ((session->request_id == transcation_id) && (SmIsClientOwned(session->flags) == client_originated))
            break;

        session = session->next;
    }

    return session;
}

STATIC connector_sm_session_t * sm_create_session(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr, connector_bool_t const client_originated)
{
    connector_sm_session_t * session = NULL;
    void * ptr = NULL;
    connector_status_t result;
    size_t const active_sessions = client_originated ? sm_ptr->session.active_client_sessions : sm_ptr->session.active_cloud_sessions;
    static connector_bool_t print_once = connector_true;

    if (active_sessions >= sm_ptr->session.max_sessions)
    {
        if (print_once)
        {
            connector_debug_line("Active %s sessions reached the limit %" PRIsize, client_originated ? "client" : "cloud", active_sessions);
            print_once = connector_false;
        }

        goto done;
    }

    print_once = connector_true;
    result = malloc_data_buffer(connector_ptr, sizeof *session, named_buffer_id(sm_session), &ptr);
    if (result != connector_working)
        goto error;

    session = ptr;
    result = get_system_time(connector_ptr, &session->start_time);
    ASSERT_GOTO(result == connector_working, error);

    session->flags = 0;
    session->error = connector_sm_error_none;
    session->in.data = NULL;
    session->in.bytes = 0;
    session->bytes_processed = 0;
    session->user.header = NULL;
    session->user.context = NULL;
    session->segments.processed = 0;
    session->segments.count = 0;

    session->transport = sm_ptr->network.transport;
    #if (defined CONNECTOR_TRANSPORT_SMS)
    if (session->transport == connector_transport_sms)
        SmSetEncoded(session->flags);
    #endif

    if (client_originated)
    {
        result = sm_copy_user_request(sm_ptr, session);
        ASSERT_GOTO(result == connector_working, error);
        SmSetClientOwned(session->flags);

        session->request_id = sm_ptr->pending.request_id;
        sm_ptr->session.active_client_sessions++;
    }
    else
    {
        session->timeout_in_seconds = sm_ptr->rx_timeout_in_seconds;
        session->sm_state = connector_sm_state_receive_data;
        sm_ptr->session.active_cloud_sessions++;
    }

    add_list_node(&sm_ptr->session.head, &sm_ptr->session.tail, session);
    goto done;

error:
    if (session != NULL)
    {
        result = free_data_buffer(connector_ptr, named_buffer_id(sm_session), session);
        ASSERT(result == connector_working);
        session = NULL;
    }

done:
    return session;
}

STATIC connector_status_t sm_delete_session(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr, connector_sm_session_t * const session)
{
    connector_status_t result = connector_working;

    ASSERT_GOTO(sm_ptr != NULL, error);
    ASSERT_GOTO(session != NULL, error);

    if (SmIsClientOwned(session->flags))
    {
        ASSERT(sm_ptr->session.active_client_sessions > 0);
        sm_ptr->session.active_client_sessions--;
    }
    else
    {
        ASSERT(sm_ptr->session.active_cloud_sessions > 0);
        sm_ptr->session.active_cloud_sessions--;
    }

    if (session->in.data != NULL)
    {
        result = free_data_buffer(connector_ptr, named_buffer_id(sm_data_block), session->in.data);
        session->in.data = NULL;
    }

    remove_list_node(&sm_ptr->session.head, &sm_ptr->session.tail, session);
    if (sm_ptr->session.current == session)
        sm_ptr->session.current = (session->next != NULL) ? session->next : sm_ptr->session.head;

    {
        connector_status_t const status = free_data_buffer(connector_ptr, named_buffer_id(sm_session), session);

        if (status != connector_working)
            result = connector_abort;
    }

    if (sm_ptr->close.stop_condition == connector_wait_sessions_complete)
    {
        if (sm_ptr->session.head == NULL)
            sm_ptr->transport.state = connector_transport_close;
    }

error:
    return result;
}

/* If request_id == NULL cancel ALL sessions. Else cancel the SM session whose Request ID matches the one passed as a parameter. */
STATIC connector_status_t sm_cancel_session(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr, uint32_t const * const request_id)
{
    connector_status_t result = connector_working;
    connector_sm_session_t * session = sm_ptr->session.head;
    connector_bool_t cancel_all = connector_bool(request_id == NULL);

    while (session != NULL)
    {
        connector_sm_session_t * next_session = NULL;
        uint32_t session_request_id = request_id != NULL ? *request_id : SM_INVALID_REQUEST_ID;

        if (cancel_all || (session_request_id == *request_id))
        {
            session->error = connector_sm_error_cancel;
            result = sm_inform_session_complete(connector_ptr, session);
            if (result != connector_working)
                break;
            next_session = session->next;
            result = sm_delete_session(connector_ptr, sm_ptr, session);
            if (result != connector_working)
                break;

            if (!cancel_all)
                break;
        }

        session = next_session != NULL ? next_session : session->next;
    }
#if (defined CONNECTOR_DATA_POINTS)
    dp_cancel_session(connector_ptr, session, request_id);
#endif
    return result;
}

STATIC connector_status_t sm_process_pending_data(connector_data_t * const connector_ptr, connector_sm_data_t * const sm_ptr)
{
    connector_status_t result = connector_idle;

    if (sm_ptr->pending.data == NULL)
        goto done;

    switch (sm_ptr->pending.request)
    {
        case connector_initiate_transport_start:
            switch(sm_ptr->transport.state)
            {
                case connector_transport_idle:
                    sm_ptr->transport.state = connector_transport_open;
                    break;

                case connector_transport_close:
                    /* wait for close to complete */
                    goto done;

                case connector_transport_terminate:
                    result = connector_device_terminated;
                    break;

                default:
                    break;
            }
            break;

        case connector_initiate_transport_stop:
            if ((sm_ptr->close.stop_condition == connector_wait_sessions_complete) && (sm_ptr->session.head != NULL))
                goto done;
            else
            {
                result = sm_cancel_session(connector_ptr, sm_ptr, NULL);
                sm_ptr->transport.state = connector_transport_close;
            }
            break;

        case connector_initiate_session_cancel:
        {
            connector_sm_cancel_request_t const * const request = sm_ptr->pending.data;
            result = sm_cancel_session(connector_ptr, sm_ptr, &request->request_id);
            break;
        }
        case connector_initiate_session_cancel_all:
        {
            result = sm_cancel_session(connector_ptr, sm_ptr, NULL);
            break;
        }
        default:
        {
            connector_bool_t const client_originated = connector_true;
            connector_sm_session_t * const session = sm_create_session(connector_ptr, sm_ptr, client_originated);

            if (session == NULL)
            {
                result = connector_pending;
                goto done;
            }

            result = connector_working;
            break;
        }
    }

    sm_ptr->pending.data = NULL;

done:
    return result;
}


