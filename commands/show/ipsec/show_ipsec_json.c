/*****************************************************************************
* Copyright (c) 2016 Digi International Inc., All Rights Reserved
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

#include "show_ipsec_json.h"
#include "show_ipsec.h"
#include "config_def.h"
#include "config_mgr.h"
#include "tlr_print.h"
#include "tlr_common.h"
#include "utils.h"
#include <get_field.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*
 * Ascii mode (for the CLI) allows for instance 0, which prints a summary. This is not supported in
 * JSON mode. Note that the upper limit has already been validated.
 */
#define INSTANCE_IS_VALID_FOR_JSON_OUTPUT(instance) ((instance) > 0)

static void add_element_value_to_show_object(tunnel_status_t const * const ptr_tunnel,
                                             tlr_show_object_t * const show_object,
                                             config_state_ipsec_t const element_id)
{
    switch (element_id)
    {
        case config_state_ipsec_description:
            tlr_show_object_add_element(show_object, element_id, ptr_tunnel->description);
            break;
        case config_state_ipsec_admin_status:
        {
            config_state_ipsec_admin_status_t const admin_status = ptr_tunnel->is_tunnel_enabled ? config_state_ipsec_admin_status_up : config_state_ipsec_admin_status_down;

            tlr_show_object_add_element(show_object, element_id, &admin_status);
            break;
        }
        case config_state_ipsec_oper_status:
        {
            config_state_ipsec_oper_status_t const oper_status = ptr_tunnel->is_tunnel_up ? config_state_ipsec_oper_status_up : config_state_ipsec_oper_status_down;

            tlr_show_object_add_element(show_object, element_id, &oper_status);
            break;
        }
        case config_state_ipsec_uptime:
        {
            uint32_t up_time_seconds = parse_ipsec_uptime_seconds(ptr_tunnel->up_time);

            tlr_show_object_add_element(show_object, element_id, &up_time_seconds);
            break;
        }
        case config_state_ipsec_peer_ip:
            tlr_show_object_add_element(show_object, element_id, ptr_tunnel->peer_ip);
            break;
        case config_state_ipsec_local_network:
        {
            char const * const local_network = parse_network(ptr_tunnel->local_cidr_network);

            tlr_show_object_add_element(show_object, element_id, local_network);
            free((void *)local_network);
            break;
        }
        case config_state_ipsec_local_mask:
        {
            char const * const local_netmask = parse_netmask(ptr_tunnel->local_cidr_network);

            tlr_show_object_add_element(show_object, element_id, local_netmask);
            free((void *)local_netmask);
            break;
        }
        case config_state_ipsec_remote_network:
        {
            char const * const remote_network = parse_network(ptr_tunnel->remote_cidr_network);

            tlr_show_object_add_element(show_object, element_id, remote_network);
            free((void *)remote_network);
            break;
        }
        case config_state_ipsec_remote_mask:
        {
            char const * const remote_netmask = parse_netmask(ptr_tunnel->remote_cidr_network);

            tlr_show_object_add_element(show_object, element_id, remote_netmask);
            free((void *)remote_netmask);
            break;
        }
        case config_state_ipsec_key_negotiation:
            tlr_show_object_add_element(show_object, element_id, ptr_tunnel->key_negotiation);
            break;
        case config_state_ipsec_rekeying_in:
            tlr_show_object_add_element(show_object, element_id, ptr_tunnel->rekeying_in);
            break;
        case config_state_ipsec_ah_ciphers:
            tlr_show_object_add_element(show_object, element_id, ptr_tunnel->ah_ciphers);
            break;
        case config_state_ipsec_esp_ciphers:
            tlr_show_object_add_element(show_object, element_id, ptr_tunnel->esp_ciphers);
            break;
        case config_state_ipsec_renegotiating_in:
            tlr_show_object_add_element(show_object, element_id, ptr_tunnel->renegotiating_in);
            break;
        case config_state_ipsec_outbound_esp_sa:
            tlr_show_object_add_element(show_object, element_id, ptr_tunnel->outbound_esp_sa);
            break;
        case config_state_ipsec_inbound_esp_sa:
            tlr_show_object_add_element(show_object, element_id, ptr_tunnel->inbound_esp_sa);
            break;
        case config_state_ipsec_rx_bytes:
        {
            uint64_t const bytes_in = strtoull(ptr_tunnel->bytes_in, NULL, 0);

            tlr_show_object_add_element(show_object, element_id, &bytes_in);
            break;
        }
        case config_state_ipsec_tx_bytes:
        {
            uint64_t const bytes_out = strtoull(ptr_tunnel->bytes_out, NULL, 0);

            tlr_show_object_add_element(show_object, element_id, &bytes_out);
            break;
        }
    }
}

static void add_element_to_show_object(tunnel_status_t const * const ptr_tunnel,
                                       tlr_show_object_t * const show_object,
                                       config_mgr_element_t const * const element)
{
    size_t const element_id = element->id;

    add_element_value_to_show_object(ptr_tunnel, show_object, element_id);
}

static void add_ipsec_elements_to_show_object(tunnel_status_t const * const ptr_tunnel, tlr_show_object_t * const show_object)
{
    config_mgr_group_t const * const group = tlr_get_group_ptr(config_state_ipsec, ptr_tunnel->instance, config_mgr_get_group);
    int i;

    assert(group != NULL);
    for (i = 0; i < group->elements.count; i++)
    {
        add_element_to_show_object(ptr_tunnel, show_object, &group->elements.element[i]);
    }
}

bool show_ipsec_json(int const instance)
{
    bool json_object_written;
    tlr_show_object_t * show_object = NULL;
    tunnel_status_t tunnel_status;
    tunnel_status_t * const ptr_tunnel = &tunnel_status;

    /* must do the init first so that the cleanup will not operate on uninitialised data */
    tunnel_status_context_init(ptr_tunnel, instance);

    if (!INSTANCE_IS_VALID_FOR_JSON_OUTPUT(instance))
    {
        json_object_written = false;
        goto done;
    }

    show_object = tlr_show_object_create(config_state_ipsec, instance);
    if (show_object == NULL)
    {
        json_object_written = false;
        goto done;
    }

    if (!read_all_tunnel_status_data(ptr_tunnel))
    {
        tlr_debug("failed to read ipsec %d status info\n", instance);
        json_object_written = false;
        goto done;
    }

    add_ipsec_elements_to_show_object(ptr_tunnel, show_object);

    printf("%s\n", tlr_show_object_to_json_string(show_object));

    json_object_written = true;

done:
    tunnel_status_context_cleanup(ptr_tunnel);
    tlr_show_object_destroy(show_object);
    return json_object_written;
}


