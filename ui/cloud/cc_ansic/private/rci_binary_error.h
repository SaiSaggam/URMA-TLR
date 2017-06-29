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

STATIC void rci_output_error_id(rci_t * const rci)
{

    connector_remote_config_t const * const remote_config = &rci->shared.callback_data;
    uint32_t value;

    if (rci->error.command_error)
    {
        #define BINARY_RCI_COMMAND_LOWER_ERROR_ID_MASK  UINT32_C(0x7FF)
        #define BINARY_RCI_COMMAND_UPPER_ERROR_ID_MASK   (~UINT32_C(0x1FFF))
        /* Command Error ID is [:13][10:0] */
        value = (remote_config->error_id & BINARY_RCI_COMMAND_LOWER_ERROR_ID_MASK);
        value |= ((remote_config->error_id << UINT32_C(2)) & BINARY_RCI_COMMAND_UPPER_ERROR_ID_MASK);
    }
    else
    {
        /* Error Id is [:13][10:7][5:0] */
        #define BINARY_RCI_LOWER_ERROR_ID_MASK UINT32_C(0x3F)
        #define BINARY_RCI_MIDDLE_ERROR_ID_MASK UINT32_C(0x780)
        #define BINARY_RCI_UPPER_ERROR_ID_MASK  ()~UINT32_C(0x1FFF))

        value = remote_config->error_id & BINARY_RCI_LOWER_ERROR_ID_MASK;
        value |= (remote_config->error_id << 1) & BINARY_RCI_MIDDLE_ERROR_ID_MASK;
        value |= (remote_config->error_id << 3) & BINARY_RCI_MIDDLE_ERROR_ID_MASK;

    }

    value |= BINARY_RCI_ERROR_INDICATOR_BIT;

    {
        connector_bool_t const overflow = rci_output_uint32(rci, value);

        if (!overflow)
            set_rci_error_state(rci, rci_error_state_description);
    }
}

STATIC void rci_output_error_description(rci_t * const rci)
{
#if defined RCI_PARSER_USES_ERROR_DESCRIPTIONS
    char const * const description = rci->error.description+1;
    size_t const length = (size_t)*rci->error.description;
#else
    char * description = NULL;
    size_t const length = 0;
#endif

    connector_bool_t const overflow = rci_output_string(rci, description,  length);

    if (!overflow)
        set_rci_error_state(rci, rci_error_state_hint);
}

STATIC void rci_output_error_hint(rci_t * const rci)
{
    connector_remote_config_t const * const remote_config = &rci->shared.callback_data;
    size_t const description_length = (remote_config->response.error_hint == NULL) ? 0 : strlen(remote_config->response.error_hint);
    connector_bool_t const overflow = rci_output_string(rci, remote_config->response.error_hint,  description_length);

    if (!overflow)
    {
        set_rci_error_state(rci, rci_error_state_callback);
    }
}


STATIC void rci_generate_error(rci_t * const rci)
{
    rci_buffer_t * const output = &rci->buffer.output;

    if (pending_rci_callback(rci))
    {
        if (!rci_callback(rci))
            goto done;
    }

    if (rci_buffer_remaining(output) != 0)
    {
        switch (rci->error.state)
        {
            case rci_error_state_id:
                rci_output_error_id(rci);
                break;

            case rci_error_state_description:
                rci_output_error_description(rci);
                break;

            case rci_error_state_hint:
                rci_output_error_hint(rci);
                break;

            case rci_error_state_callback:
            {
                connector_request_id_remote_config_t const remote_config_request = rci->callback.request.remote_config_request;
                connector_remote_config_t const * const remote_config = &rci->shared.callback_data;

                switch (remote_config_request)
                {
                    case connector_request_id_remote_config_action_start:
                    case connector_request_id_remote_config_group_end:
                        trigger_rci_callback(rci, connector_request_id_remote_config_action_end);
                        break;
                    case connector_request_id_remote_config_session_start:
                        trigger_rci_callback(rci, connector_request_id_remote_config_session_end);
                        break;
                    case connector_request_id_remote_config_action_end:
                        {
                            connector_bool_t const overflow = rci_output_terminator(rci);
                            if (overflow) goto done;
                        }
                        trigger_rci_callback(rci, connector_request_id_remote_config_session_end);
                        break;
                    case connector_request_id_remote_config_group_start:
                        {
                            connector_bool_t const overflow = rci_output_terminator(rci);
                            if (overflow) goto done;
                        }
                        if (remote_config->error_id < connector_rci_error_COUNT)
                        {
                            trigger_rci_callback(rci, connector_request_id_remote_config_group_end);
                        }
                        else
                        {
                            rci->output.group_skip = connector_true;
                            set_rci_error_state(rci, rci_error_state_id);
                            state_call(rci, rci_parser_state_traverse);
                        }
                        break;

                    case connector_request_id_remote_config_group_process:
#if (defined RCI_LEGACY_COMMANDS)
                    case connector_request_id_remote_config_do_command:
                    case connector_request_id_remote_config_reboot:
                    case connector_request_id_remote_config_set_factory_def:
#endif
                        if (remote_config->error_id < connector_rci_error_COUNT)
                        {
                            {
                                connector_bool_t const overflow = rci_output_terminator(rci);
                                if (overflow) goto done;
                            }
                            trigger_rci_callback(rci, connector_request_id_remote_config_group_end);
                        }
                        else
                        {
                            set_rci_error_state(rci, rci_error_state_id);
                            state_call(rci, rci_parser_state_traverse);
                        }
                        break;

                    case connector_request_id_remote_config_session_cancel:
                        ASSERT(connector_false);
                        break;

                    case connector_request_id_remote_config_session_end:
                        rci->status = rci_status_complete;
                        break;
                }
                break;
            }
        }
    }

#if defined RCI_DEBUG
    {
        size_t const bytes = rci_buffer_used(&rci->buffer.output);
        if (bytes > 0)
        {
            connector_debug_print_buffer("Response", rci->buffer.output.start, bytes);
        }
    }
#endif

done:
    return;
}


