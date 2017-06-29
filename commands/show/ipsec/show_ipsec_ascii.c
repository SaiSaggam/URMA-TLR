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
#include "show_ipsec_ascii.h"
#include "show_ipsec.h"
#include "show_status_helper.h"
#include "unittest_support.h"

static void print_tunnel_status_footer(void)
{
    putchar('\n');
}

STATIC_FUNCTION void print_tunnel_status(tunnel_status_t const * const ptr_tunnel)
{
    int const label_field_width = 17;
    printf_status_header(DIVIDER_WIDTH_AUTO, " IPsec Tunnel %d Status\n", ptr_tunnel->instance);

    printf(" %-*s: %s\n", label_field_width, "Description", ptr_tunnel->description);
    printf(" %-*s: %s\n", label_field_width, "Admin Status", (ptr_tunnel->is_tunnel_enabled) ? "Up" : "Down");
    printf(" %-*s: %s\n", label_field_width, "Oper Status", (ptr_tunnel->is_tunnel_up) ? "Up" : "Down");
    if (ptr_tunnel->is_tunnel_up)
    {
        printf(" %-*s: %s\n", label_field_width, "Uptime", ptr_tunnel->up_time);
        printf(" %-*s: %s\n", label_field_width, "Peer", ptr_tunnel->peer_ip);
        printf(" %-*s: %s\n", label_field_width, "Local Network", ptr_tunnel->local_cidr_network);
        printf(" %-*s: %s\n", label_field_width, "Remote Network", ptr_tunnel->remote_cidr_network);
        printf(" %-*s: %s\n", label_field_width, "Key Negotiation", ptr_tunnel->key_negotiation);
        printf(" %-*s: %s\n", label_field_width, "Rekeying In", ptr_tunnel->rekeying_in);
        printf(" %-*s: %s\n", label_field_width, "AH Cipher Suite", "Not Used");   // XXX Do we need this considering we don't support AH configuration?
        printf(" %-*s: %s\n", label_field_width, "ESP Cipher Suite", ptr_tunnel->esp_ciphers);
        printf(" %-*s: %s\n", label_field_width, "Renegotiating In", ptr_tunnel->renegotiating_in);
        printf(" %-*s: 0x%s\n", label_field_width, "Outbound ESP SA", ptr_tunnel->outbound_esp_sa);
        printf(" %-*s: 0x%s\n", label_field_width, "Inbound ESP SA", ptr_tunnel->inbound_esp_sa);
        printf(" %-*s: %s\n", label_field_width, "Bytes In", ptr_tunnel->bytes_in);
        printf(" %-*s: %s\n", label_field_width, "Bytes Out", ptr_tunnel->bytes_out);
    }

    print_tunnel_status_footer();
}

STATIC_FUNCTION void print_tunnel_summary_ascii(tunnel_status_t const * const ptr_tunnel)
{
    printf("%-1d  ", ptr_tunnel->instance);
    printf("%-6s  ", (ptr_tunnel->is_tunnel_up) ? "Up" : "Down");
    printf("%-20s  ", ptr_tunnel->local_cidr_network);
    printf("%-20s  ", ptr_tunnel->remote_cidr_network);
    printf("%s\r\n", ptr_tunnel->up_time);
}

static void print_tunnel_summary_footer(void)
{
    printf("\n");
}

static void print_tunnel_status_for_instance(size_t const instance)
{
    tunnel_status_t tunnel_status;
    tunnel_status_t * const ptr_tunnel = &tunnel_status;

    tunnel_status_context_init(ptr_tunnel, instance);

    read_all_tunnel_status_data(ptr_tunnel);

    print_tunnel_status(ptr_tunnel);

    tunnel_status_context_cleanup(ptr_tunnel);

}

static bool print_tunnel_instance_summary(size_t const instance)
{
    tunnel_status_t tunnel_status;
    tunnel_status_t * const ptr_tunnel = &tunnel_status;
    bool summary_printed;

    tunnel_status_context_init(ptr_tunnel, instance);

    if (get_admin_status(ptr_tunnel) < 0)
    {
        summary_printed = false;
        goto done;
    }
    if (ptr_tunnel->is_tunnel_enabled)
    {
        if (get_tunnel_status(ptr_tunnel) < 0)
        {
            summary_printed = false;
            goto done;
        }
        print_tunnel_summary_ascii(ptr_tunnel);
    }
    summary_printed = true;

done:
    tunnel_status_context_cleanup(ptr_tunnel);

    return summary_printed;
}

static bool print_tunnel_summary(size_t const num_ipsec_instances)
{
    bool summary_printed;

    printf_status_header(80, " Status    Local                 Remote                 Uptime\n");

    for (int summary_instance = 1; summary_instance <= num_ipsec_instances; summary_instance++)
    {
        if (!print_tunnel_instance_summary(summary_instance))
        {
            summary_printed = false;
            goto done;
        }
    }

    print_tunnel_summary_footer();
    summary_printed = true;

done:
    return summary_printed;
}

int show_ipsec_ascii(int const instance, size_t const num_ipsec_instances)
{
    int ret = -1;

    if (instance > 0)
    {
        print_tunnel_status_for_instance(instance);
    }
    else // print out a summary
    {
        if (!print_tunnel_summary(num_ipsec_instances))
        {
            goto error;
        }
    }

    ret = 0;

error:
    return ret;
}


