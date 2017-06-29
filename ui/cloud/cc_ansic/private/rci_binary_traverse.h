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

STATIC void traverse_rci_command(rci_t * const rci)
{
    trigger_rci_callback(rci, connector_request_id_remote_config_action_start);

    set_rci_output_state(rci, rci_output_state_command_id);
    state_call(rci, rci_parser_state_output);

}
STATIC void traverse_group_id(rci_t * const rci)
{
    trigger_rci_callback(rci, connector_request_id_remote_config_group_start);

    set_rci_output_state(rci, rci_output_state_group_id);
    state_call(rci, rci_parser_state_output);
}

STATIC void traverse_element_id(rci_t * const rci)
{

    connector_group_element_t const * const element = get_current_element(rci);

    if ((rci->shared.callback_data.action == connector_remote_action_query) &&
        (element->access == connector_element_access_write_only))
    {
        goto done;
    }

    trigger_rci_callback(rci, connector_request_id_remote_config_group_process);
    set_rci_output_state(rci, rci_output_state_field_id);
    state_call(rci, rci_parser_state_output);

done:
    return;
}

#if (defined RCI_LEGACY_COMMANDS)
STATIC void traverse_command_do_command(rci_t * const rci)
{
    trigger_rci_callback(rci, connector_request_id_remote_config_do_command);

    set_rci_output_state(rci, rci_output_state_do_command_payload);
    state_call(rci, rci_parser_state_output);

    return;
}

STATIC void traverse_command_reboot(rci_t * const rci)
{
    trigger_rci_callback(rci, connector_request_id_remote_config_reboot);

    state_call(rci, rci_parser_state_input);

    return;
}

STATIC void traverse_command_set_factory_default(rci_t * const rci)
{
    trigger_rci_callback(rci, connector_request_id_remote_config_set_factory_def);

    state_call(rci, rci_parser_state_input);

    return;
}
#endif

STATIC void traverse_element_end(rci_t * const rci)
{
    trigger_rci_callback(rci, connector_request_id_remote_config_group_end);

    set_rci_output_state(rci, rci_output_state_field_terminator);
    state_call(rci, rci_parser_state_output);
}

STATIC void traverse_group_end(rci_t * const rci)
{
    trigger_rci_callback(rci, connector_request_id_remote_config_action_end);
    set_rci_output_state(rci, rci_output_state_group_terminator);
    state_call(rci, rci_parser_state_output);

}

STATIC connector_bool_t traverse_all_elements(rci_t * const rci)
{
    connector_bool_t done = connector_false;

    if (!have_element_id(rci))
    {
        /* all fields */
        set_element_id(rci, 0);
        traverse_element_id(rci);
    }
    else
    {
        connector_group_t const * const group =  get_current_group(rci);
        unsigned int const id = get_element_id(rci) + 1;

        if (id < group->elements.count)
        {
            set_element_id(rci, id);
            traverse_element_id(rci);
        }
        else
        {
            traverse_element_end(rci);
            done = connector_true;
        }
    }
    return done;
}

STATIC connector_bool_t traverse_all_group_instances(rci_t * const rci)
{
    connector_bool_t done = connector_false;

    switch (rci->traverse.process_state)
    {
        case rci_traverse_process_group:
        case rci_traverse_process_element:
            if (traverse_all_elements(rci))
                rci->traverse.process_state = rci_traverse_process_next_instance;
            break;
        case rci_traverse_process_next_instance:
            {
                connector_group_t const * const group =  get_current_group(rci);
                if (group->instances > 1 && group->instances > get_group_index(rci))
                {
                    /* next instances */
                    increment_group_index(rci);
                    traverse_group_id(rci);
                    rci->traverse.process_state = rci_traverse_process_element;
                }
                else
                {
                    /* no more instances */
                    done = connector_true;
                }
            }
            break;
    }

    return done;
}

STATIC connector_bool_t traverse_all_groups(rci_t * const rci)
{
    connector_bool_t done = connector_false;

    switch (rci->traverse.process_state)
    {
        case rci_traverse_process_group:
        {
            connector_remote_config_data_t const * const rci_data = rci->service_data->connector_ptr->rci_data;
            connector_remote_group_table_t const * const table = rci_data->group_table + rci->shared.callback_data.group.type;

            if (table->count == 0)
            {
                traverse_group_end(rci);
            }
            else if (!have_group_id(rci))
            {
                set_group_id(rci, 0);
                set_group_index(rci, 1);
                traverse_group_id(rci);
                rci->traverse.process_state = rci_traverse_process_element;
            }
            else
            {
                connector_group_t const * const group =  get_current_group(rci);
                if (group->instances > 1 && group->instances > get_group_index(rci))
                {
                    /* next instances */
                    increment_group_index(rci);

                    traverse_group_id(rci);
                    rci->traverse.process_state = rci_traverse_process_element;
                }
                else
                {
                    size_t const id = get_group_id(rci) + 1;
                    if (id == table->count)
                    {
                        /* done all groups */
                        traverse_group_end(rci);
                        done = connector_true;
                    }
                    else
                    {
                        /* next group */
                        increment_group_id(rci);
                        set_group_index(rci, 1);

                        traverse_group_id(rci);
                        rci->traverse.process_state = rci_traverse_process_element;
                    }
                }
            }
            break;
        }
        case rci_traverse_process_element:
            if (traverse_all_elements(rci))
                rci->traverse.process_state = rci_traverse_process_group;
            break;

        case rci_traverse_process_next_instance:
            break;
    }

    return done;
}

STATIC void rci_traverse_data(rci_t * const rci)
{
    connector_bool_t done_state = connector_true;

#if (defined RCI_DEBUG)
    connector_debug_line("traverse: %s", rci_traverse_state_t_as_string(rci->traverse.state));
#endif

    switch (rci->traverse.state)
    {
        case rci_traverse_state_none:
            if (get_rci_input_state(rci) == rci_input_state_done)
                state_call(rci, rci_parser_state_output);
            else
                state_call(rci, rci_parser_state_input);
            break;

        case rci_traverse_state_command_id:
            traverse_rci_command(rci);
            break;

        case rci_traverse_state_all_groups:
            done_state = traverse_all_groups(rci);
            break;

        case rci_traverse_state_group_id:
            traverse_group_id(rci);
            break;

        case rci_traverse_state_all_group_instances:
            done_state = traverse_all_group_instances(rci);
            break;

        case rci_traverse_state_element_id:
            traverse_element_id(rci);
            break;

        case rci_traverse_state_all_elements:
            done_state = traverse_all_elements(rci);
            break;

        case rci_traverse_state_element_end:
            traverse_element_end(rci);
            break;

        case rci_traverse_state_group_end:
            traverse_group_end(rci);
            break;
#if (defined RCI_LEGACY_COMMANDS)
        case rci_traverse_state_command_do_command:
            traverse_command_do_command(rci);
            break;
        case rci_traverse_state_command_reboot:
            traverse_command_reboot(rci);
            break;
        case rci_traverse_state_command_set_factory_default:
            traverse_command_set_factory_default(rci);
            break;
#endif
    }

    if (done_state)
    {
        rci->traverse.process_state = rci_traverse_process_group;
        set_rci_traverse_state(rci, rci_traverse_state_none);
    }

    return;
}

