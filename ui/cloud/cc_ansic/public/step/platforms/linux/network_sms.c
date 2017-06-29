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
/**
 * @file
 *  @brief Routines which implement Cloud Connector network interface for
 *  @ref CONNECTOR_TRANSPORT_SMS.
 */

#include "connector_api.h"
#include "platform.h"

#if defined CONNECTOR_TRANSPORT_SMS
#include <gammu.h>

#define MAX_TELEPHONE_NUMBER_LENGTH     32 /* TODO: define it better. */

typedef struct gammu_sms_handler {
    char server_telephone[MAX_TELEPHONE_NUMBER_LENGTH];
    GSM_SMSC phone_SMSC;
    GSM_StateMachine *state_machine;
    GSM_Error sms_send_status;
    connector_bool_t send_sms_callback_asserted;
} gammu_sms_handler_t;

gammu_sms_handler_t g_sms_handle;

/* Function to handle errors */
connector_callback_status_t gammu_error_handler(GSM_Error error)
{
    if (error != ERR_NONE)
    {
        APP_DEBUG("GAMMU error: %s\n", GSM_ErrorString(error));
        return connector_callback_error;
    }
    return connector_callback_continue;
}

/* Handler for SMS send reply */
void send_sms_callback(GSM_StateMachine *state_machine, int status, int MessageReference, void * user_data)
{
    UNUSED_ARGUMENT(state_machine);
    UNUSED_ARGUMENT(user_data);
    
    g_sms_handle.send_sms_callback_asserted = connector_true;
    
    APP_DEBUG("Sent SMS");
    if (status == 0)
    {
        APP_DEBUG("..OK");
        g_sms_handle.sms_send_status = ERR_NONE;
    }
    else
    {
        APP_DEBUG("..error %i", status);
        g_sms_handle.sms_send_status = ERR_UNKNOWN;
    }
    APP_DEBUG(", message reference=%d\n", MessageReference);
}

gboolean sms_inbox_is_empty(GSM_StateMachine *state_machine)
{
    GSM_SMSMemoryStatus SMSStatus;
    GSM_Error error;

    error = GSM_GetSMSStatus(state_machine, &SMSStatus);
    gammu_error_handler(error);

    return !SMSStatus.SIMUsed && !SMSStatus.PhoneUsed;
}

GSM_Error get_first_sms(GSM_StateMachine *state_machine, GSM_SMSMessage *sms)
{
    GSM_MultiSMSMessage multisms;
    gboolean const start = 1;
    GSM_Error error;

    multisms.Number = 0;
    multisms.SMS[0].Folder = 0;
    multisms.SMS[0].Location = 0;
    error = GSM_GetNextSMS(state_machine, &multisms, start);
    if (error != ERR_NONE)
        goto done;

    if (multisms.SMS[0].Location < 0 || multisms.SMS[0].Location == GSM_PHONE_MAXSMSINFOLDER)
         multisms.SMS[0].Location = 0; /* Some Modems do not set the Location properly. */

    memcpy(sms, &multisms.SMS[0], sizeof *sms);
done:
    return error;
}

/**
 * @brief   Open a network to communicate Device Cloud
 *
 * @param data @ref connector_network_open_t
 *  <ul>
 *   <li><b><i>device_cloud.phone</i></b> - For SMS transport it's the Device Cloud phone number where to send SMSs.</li>
 *   <li><b><i>handle</i></b> - This is filled in with the value
 *   of a network handle, passed to subsequent networking calls,
 *   @ref connector_network_handle_t "connector_network_handle_t"
 *      is defined in public\include\connector_types.h.
 *  </li>
 * </ul>
 *
 * @retval connector_callback_continue  The routine has successfully opened a socket and resolved Device Cloud's name.
 * @retval connector_callback_busy              The routine will be called again to complete open.
 * @retval connector_callback_error     The operation failed, Cloud Connector
 *                                  will exit @ref connector_run "connector_run()" or @ref connector_step "connector_step()".
 * @retval connector_callback_abort     The application aborts Cloud Connector.
 *
 * @see @ref open "Network API callback Open"
 */
static connector_callback_status_t app_network_sms_open(connector_network_open_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    INI_Section *cfg;
    GSM_Error error;
    GSM_StateMachine *state_machine;

    GSM_InitLocales(NULL);

    state_machine = GSM_AllocStateMachine();
    error = GSM_FindGammuRC(&cfg, NULL);
    status = gammu_error_handler(error);
    error = GSM_ReadConfig(cfg, GSM_GetConfig(state_machine, 0), 0);
    status = gammu_error_handler(error);
    INI_Free(cfg);
    GSM_SetConfigNum(state_machine, 1);
    error = GSM_InitConnection(state_machine, 1);
    status = gammu_error_handler(error);

    g_sms_handle.phone_SMSC.Location = 1;
    error = GSM_GetSMSC(state_machine, &g_sms_handle.phone_SMSC);
    status = gammu_error_handler(error);
    if (status != connector_callback_continue)
        goto done;

    /* Delete all SMS in SIM and Device */
    while (!sms_inbox_is_empty(state_machine) && status == connector_callback_continue)
    {
        GSM_SMSMessage sms;
        GSM_Error error = get_first_sms(state_machine, &sms);
        status = gammu_error_handler(error);
        if (status != connector_callback_continue)
            goto done;
        error = GSM_DeleteSMS(state_machine, &sms);
        status = gammu_error_handler(error);
        if (status != connector_callback_continue)
            goto done;
    }
    
    /* Set the SMS sent callback, for send function. */

    GSM_SetSendSMSStatusCallback(state_machine, send_sms_callback, NULL);

    g_sms_handle.state_machine = state_machine;
    strcpy(g_sms_handle.server_telephone, "+");
    strcat(g_sms_handle.server_telephone, data->device_cloud.phone);

    data->handle = &g_sms_handle;



done:
    return status;
}

/**
 * @brief   Send data to Device Cloud
 *
 * This routine sends data to Device Cloud. This
 * function must not block. If it encounters EAGAIN or
 * EWOULDBLOCK error, 0 bytes must be returned and
 * Cloud Connector will continue calling this function. If
 * successful connector_callback_continue is returned. If the
 * data could not be sent connector_callback_busy is returned,
 * otherwise connector_callback_abort is returned.
 *
 * @param data @ref connector_network_send_t
 *  <ul>
 *   <li><b><i>handle</i></b> - Network handle </li>
 *   <li><b><i>buffer</i></b> - Data to be sent </li>
 *   <li><b><i>bytes_available</i></b> - Number of bytes to send
 *  </li>
 *   <li><b><i>bytes_used</i></b> - Number of bytes sent </li>
 * </ul>
 *
 * @retval connector_callback_continue  The routine has sent
 *         some data.
 * @retval connector_callback_busy  No data was sent, the
 *                                  routine has encountered
 *                                  EAGAIN or EWOULDBLOCK error.
 *                                  It will be called again to
 *                                  send data.
 * @retval connector_callback_error     An irrecoverable error has occurred,  Cloud Connector will call
 *                                  @ref app_network_sms_close.
 * @retval connector_callback_abort     The application aborts Cloud Connector.
 *
 * @see @ref send API Network Callback
 */
static connector_callback_status_t app_network_sms_send(connector_network_send_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    gammu_sms_handler_t *sms_handle = data->handle;
    GSM_SMSMessage sms;
    GSM_Error error;

    data->bytes_used = data->bytes_available; /* All bytes sent */
    GSM_SetDefaultSMSData(&sms);

    EncodeUnicode(sms.Number, sms_handle->server_telephone, strlen(sms_handle->server_telephone));
    EncodeUnicode(sms.Text, data->buffer, data->bytes_available);
    CopyUnicodeString(sms.SMSC.Number, sms_handle->phone_SMSC.Number);

    /* Submit message */
    sms.PDU = SMS_Submit;
    /* No UDH, just a plain message */
    sms.UDH.Type = UDH_NoUDH;
    /* Default coding for text */
    sms.Coding = SMS_Coding_Default_No_Compression;
    /* Class 1 message (normal) */
    sms.Class = 1;

    sms_handle->send_sms_callback_asserted = connector_false;

    error = GSM_SendSMS(sms_handle->state_machine, &sms);

    status = gammu_error_handler(error);
    for (;;)
    {
        GSM_ReadDevice(sms_handle->state_machine, TRUE);

        if (sms_handle->send_sms_callback_asserted)
        {
            status = sms_handle->sms_send_status == ERR_NONE ? connector_callback_continue : connector_callback_error;
            break;
        }
        /* TODO, add here a sleep so it is not too demanding... */
    }

    data->bytes_used = data->bytes_available; /* All bytes sent */
    return status;
}

/**
 * @brief   Receive data from Device Cloud
 *
 * This routine receives data from Device Cloud. This
 * function must not block.
 *
 * @param data @ref connector_network_receive_t
 *  <ul>
 *   <li><b><i>handle</i></b> - Network handle </li>
 *   <li><b><i>buffer</i></b> - Buffer to place received data
 *   </li>
 *   <li><b><i>bytes_available</i></b> - Buffer size in bytes
 *  </li>
 *   <li><b><i>bytes_used</i></b> - Number of bytes received
 *   </li>
 * </ul>
 *
 * @retval connector_callback_continue  The routine has received some data.
 * @retval connector_callback_busy      No data is pending, the routine has encountered EAGAIN or
 *                                  EWOULDBLOCK error. It will be called again to receive data.
 * @retval connector_callback_error     An irrecoverable error has occurred,  Cloud Connector will call
 *                                  @ref app_network_sms_close.
 * @retval connector_callback_abort     The application aborts Cloud Connector.
 *
 * @see @ref receive API Network Callback
 */
static connector_callback_status_t app_network_sms_receive(connector_network_receive_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    gammu_sms_handler_t *sms_handle = data->handle;
    GSM_Error error;

    if (sms_inbox_is_empty(sms_handle->state_machine))
    {
        status = connector_callback_busy;
        goto done;
    }

    {
        GSM_SMSMessage sms;

        error = get_first_sms(sms_handle->state_machine, &sms);

        status = gammu_error_handler(error);
        if (status != connector_callback_continue)
            goto done;

        if (data->bytes_available < (size_t)sms.Length)
        {
            APP_DEBUG("app_network_sms_receive: buffer is not long enough to store received SMS\n");
            status = connector_callback_error;
            goto done;
        }

        data->bytes_used = sms.Length;
        memcpy(data->buffer, DecodeUnicodeConsole(sms.Text), data->bytes_used);

        error = GSM_DeleteSMS(sms_handle->state_machine, &sms);
        status = gammu_error_handler(error);
        if (status != connector_callback_continue)
            goto done;

        status = connector_callback_continue;
        goto done;
    }

done:
    return status;
}

/**
 * @brief   Close the network
 *
 * This callback requests an application to close it's network handle.
 *
 * @param data @ref connector_network_close_t
 *  <ul>
 *   <li><b><i>handle</i></b> - Network handle </li>
 *   <li><b><i>reconnect</i></b> - Set to connector_true to
 *   reconnect, without exiting connector_run(). Set
 *   to connector_false otherwise.
 *   </li>
 * </ul>
 *
 * @retval connector_callback_continue  The callback has successfully closed the connection.
 * @retval connector_callback_busy      The network device is busy, the routine will be called again to complete close.
 * @retval connector_callback_abort     The application aborts Cloud Connector.
 *
 * @see @ref connector_callback_status_t
 * @see @ref close API Network Callback
 */
static connector_callback_status_t app_network_sms_close(connector_network_close_t * const data)
{
    connector_callback_status_t status = connector_callback_continue;
    gammu_sms_handler_t *sms_handle = data->handle;
    GSM_Error error;

    /* app_dns_set_redirected(connector_class_id_network_sms, data->status == connector_close_status_cloud_redirected); */

    data->reconnect = app_connector_reconnect(connector_class_id_network_sms, data->status);

    /* Terminate connection */
    error = GSM_TerminateConnection(sms_handle->state_machine);
    status = gammu_error_handler(error);
    if (status != connector_callback_continue)
        goto done;

    GSM_FreeStateMachine(sms_handle->state_machine);
done:
    return status;
}

/*
 *  Callback routine to handle all networking related calls.
 */
connector_callback_status_t app_network_sms_handler(connector_request_id_network_t const request_id,
                                                    void * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request_id)
    {
    case connector_request_id_network_open:
        status = app_network_sms_open(data);
        break;

    case connector_request_id_network_send:
        status = app_network_sms_send(data);
        break;

    case connector_request_id_network_receive:
        status = app_network_sms_receive(data);
        break;

    case connector_request_id_network_close:
        status = app_network_sms_close(data);
        break;

    default:
        APP_DEBUG("app_network_sms_handler: unrecognized callback request_id [%d]\n", request_id);
        status = connector_callback_unrecognized;
        break;

    }

    return status;
}
#endif
