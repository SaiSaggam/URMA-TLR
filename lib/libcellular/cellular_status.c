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
****************************************************************************/

#include "cellular_status.h"
#include "cellular_def.h"
#include "csman.h"
#include "csid/csid_3g.h"
#include "csid/csid_usb.h"
#include "tlr_common.h"

#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>



typedef struct {
    unsigned int mcc;
    char * name;
} cellular_mcc_country_t;

typedef struct {
    unsigned int base;
    struct {
        cellular_mcc_country_t const * country;
        size_t count;
    } countries;

} cellular_mcc_t;

static cellular_mcc_country_t const mcc_200_country_table[] = {
     {204, "Netherlands"},
     {208, "France"},
     {214, "Spain"},
     {216, "Hungary"},
     {222, "Italy"},
     {228, "Switzerland"},
     {234, "UK"},
     {240, "Sweden"},
     {262, "Germany"},
     {286, "Turkey"}
};

static cellular_mcc_country_t const mcc_300_country_table[] = {
     {302, "Canada"},
     {310, "USA"},
     {311, "USA"},
     {316, "USA"},
     {334, "Mexico"}
};

static cellular_mcc_country_t const mcc_400_country_table[] = {
     {466, "Taiwan"},
};

static cellular_mcc_country_t const mcc_500_country_table[] = {
     {505, "Australia"},
     {530, "New Zealand"},
};
static cellular_mcc_country_t const mcc_600_country_table[] = {
     {623, "Central African Republic"},
     {655, "South Africa"},
};
static cellular_mcc_country_t const mcc_700_country_table[] = {
     {724, "Brazil"},
};

static cellular_mcc_t const cellular_mcc_table[] = {
        {0, {NULL, 0}},
        {1, {NULL, 0}},
        {2, {mcc_200_country_table, asizeof(mcc_200_country_table)}},
        {3, {mcc_300_country_table, asizeof(mcc_300_country_table)}},
        {4, {mcc_400_country_table, asizeof(mcc_400_country_table)}},
        {5, {mcc_500_country_table, asizeof(mcc_500_country_table)}},
        {6, {mcc_600_country_table, asizeof(mcc_600_country_table)}},
        {7, {mcc_700_country_table, asizeof(mcc_700_country_table)}},
        {8, {NULL, 0}},
        {9, {NULL, 0}},

};

cellular_modem_t const cellular_modem_table[] = {
    {cellular_module_unknown, "Unkown", CELLULAR_SUPPORTED},
    {cellular_module_HE910, "HE910", CELLULAR_NOT_SUPPORTED},
    {cellular_module_MC7455, "MC7455", CELLULAR_SUPPORTED}
};


static size_t cellular_mcc_table_count = asizeof(cellular_mcc_table);

static int get_module(int const fd, size_t const index_3g, char * const name, size_t const name_size)
{
    int ret;
    /* concatenate manufacturer and model */
    char * ptr = name;
    size_t avail_length = name_size;
    ret = get_manufacturer(fd, index_3g, ptr, avail_length);
    if (ret == 0)
    {
        size_t length = strlen(ptr);
        *(ptr + length) = ' ';
        length++;
        ptr += length;
        avail_length -= length;
        ret = get_model(fd, index_3g, ptr, avail_length);
    }

    return ret;
}

static int get_network_provider(int const fd, size_t const index_3g, char * const name, size_t const name_size)
{
    int ret;
    char * ptr = name;
    size_t avail_length = name_size;

    /* concatenate network name and country name */
    ret = get_network_name(fd, index_3g, ptr, avail_length);

    if (ret == 0)
    {
        unsigned int mcc;
        unsigned int mcc_base;

        size_t length = strlen(ptr);
        if (length == 0) goto done;

        ptr += length;
        avail_length -= length;

        ret = get_mcc(fd, index_3g, &mcc);
        if (ret < 0) goto done;

        mcc_base = mcc/100;
        length = 0;

        if (mcc_base < cellular_mcc_table_count)
        {
            cellular_mcc_t const * mcc_table = &cellular_mcc_table[mcc_base];

            if (mcc_table->countries.count > 0)
            {
                size_t i;

                for (i=0; i < mcc_table->countries.count; i++)
                {
                    cellular_mcc_country_t const * country = &mcc_table->countries.country[i];
                    if (country->mcc == mcc)
                    {
                        length = snprintf(ptr, avail_length, ", %s", country->name);
                        *(ptr + length) = '\0';
                        break;
                    }
                }
            }
        }
        if ((length == 0) && (mcc != 0))
        {
            length = snprintf(ptr, avail_length, ", %03d", mcc);
            *(ptr + length) = '\0';
        }

    }
done:
    return ret;
}

int cellular_open_status(void)
{

    int fd = open_csman(NULL, 0);

    if (fd < 0)
    {
        cellular_debug("open_csman failed");
    }
    else
    {
        cellular_debug("open_csman fd = %d\n", fd);
    }
    return fd;
}

void cellular_close_status(int fd)
{
    if (fd >= 0)
    {
        if (close_csman(fd) < 0)
        {
            cellular_debug("cellular_close_status failed\n");
        }
        else
        {
            cellular_debug("close_csman fd = %d\n", fd);
        }

    }
}

int cellular_read_status(int const fd, size_t const index_3g, config_state_cellular_t const sid, void * value, size_t const value_size)
{
    int ret= -1;
    size_t index_usb;
    bool usb_ready;

    if ((value == NULL) || (value_size <= 0))
    {
        cellular_debug("Invalid parameter for cellular status  (incorrect size %zu)\n", value_size);
        goto done;
    }

    usb_ready = is_usb_ready(fd, index_3g);

    if (usb_ready)
    {
        ret = get_usb_index(fd, index_3g, &index_usb);
        if (ret < 0)
        {
            goto done;
        }
    }

    switch (sid)
    {
        case config_state_cellular_description:
            /* read from UCI which is handled in show cellular process */
            break;
        case config_state_cellular_module:
            ret= get_module(fd, index_3g, value, value_size);
            break;

        case config_state_cellular_sim_status:
            if (value_size == sizeof(size_t))
            {
                ret = get_active_sim(fd, index_3g, value);
            }
            else
            {
                cellular_debug("Invalid parameter for active sim  (incorrect size %zu)\n", value_size);
            }

            break;

        case config_state_cellular_firmware_version:
            ret = get_firmware_version(fd, index_3g, value, value_size);
            break;

        case config_state_cellular_hardware_version:
            ret = get_hardware_version(fd, index_3g, value, value_size);
            break;

        case config_state_cellular_imei:
            ret = get_imei(fd, index_3g, value, value_size);
            break;

        case config_state_cellular_signal_strength:
            if (value_size == sizeof(int32_t))
                ret = get_rssi(fd, index_3g, value);
            else
            {
                ret = -1;
                cellular_debug("Invalid parameter for RSSI  (incorrect size %zu)\n", value_size);
            }
            break;

        case config_state_cellular_signal_quality:
            if (value_size == sizeof(int32_t))
                ret = get_ecio(fd, index_3g, value);
            else
            {
                ret = -1;
                cellular_debug("Invalid parameter for EC/IO (incorrect size %zu)\n", value_size);
            }
            break;
        case config_state_cellular_temperature:
            if (value_size == sizeof(int32_t))
                ret = get_temperature(fd, index_3g, value);
            else
            {
                ret = -1;
                cellular_debug("Invalid parameter for temperature (incorrect size %zu)\n", value_size);
            }
            break;
        case config_state_cellular_registration_status:
            if (value_size == sizeof(cellular_registration_status_t))
                ret = get_registration_status(fd, index_3g, value);
            else
            {
                ret = -1;
                cellular_debug("Invalid parameter for registration status (incorrect size %zu)\n", value_size);
            }
            break;

        case config_state_cellular_network_provider:
            ret = get_network_provider(fd, index_3g, value, value_size);
            break;

        case config_state_cellular_connection_type:
            if (value_size == sizeof(cellular_connection_type_t))
            {
                ret = get_connection_type(fd, index_3g, value);
            }
            else
            {
                ret = -1;
                cellular_debug("Invalid parameter for connection type (incorrect size %zu)\n", value_size);
            }
            break;
        case config_state_cellular_radio_band:
            ret = get_radio_band(fd, index_3g, value, value_size);
            break;

        case config_state_cellular_channel:
            if (value_size == sizeof(unsigned int))
            {
                ret = get_channel(fd, index_3g, value);
            }
            else
            {
                ret = -1;
                cellular_debug("Invalid parameter for channel (incorrect size %zu)\n", value_size);
            }
            break;
        case config_state_cellular_pdp_context:
            if (value_size > 0)
            {
                char * ptr = value;
                int len;
                size_t avail_len = value_size;

                uint32_t pid;
                ret = get_pdp_context_pid(fd, index_3g, &pid);
                if (ret == 0)
                {
                    len = snprintf(ptr, avail_len, "Context %u: ", pid);
                    ptr += len;
                    avail_len -= len;

                    ret = get_pdp_context_apn(fd, index_3g, ptr, avail_len);
                }
            }
            break;

        case config_state_cellular_ip_address:
            if (usb_ready)
            {
                struct in_addr ipaddr;
                ipaddr.s_addr = 0;
                ret = get_ip_address(fd, index_usb, &ipaddr);
                if (ret == 0)
                {
                    unsigned long *p = (unsigned long *)&ipaddr;
                    if (*p != 0)
                    {
                        snprintf(value, value_size, "%s", (char *) inet_ntoa(ipaddr));
                    }
                }
            }
            break;

        case config_state_cellular_mask:
            if (usb_ready)
            {
                struct in_addr netmask;
                netmask.s_addr = 0;
                ret = get_netmask(fd, index_usb, &netmask);
                if (ret == 0)
                {
                    unsigned long *p = (unsigned long *)&netmask;
                    if( *p != 0)
                    {
                        snprintf(value, value_size, "%s", (char *) inet_ntoa(netmask));
                    }
                }
            }
            break;

        case config_state_cellular_gateway:
            if (usb_ready)
            {
                struct in_addr gateway;
                gateway.s_addr = 0;
                ret = get_gateway(fd, index_usb, &gateway);
                if (ret == 0)
                {
                    unsigned long *p = (unsigned long *)&gateway;
                    if( *p != 0)
                    {
                        snprintf(value, value_size, "%s", (char *) inet_ntoa(gateway));
                    }
                }
            }
            break;

        case config_state_cellular_dns_servers:
            if (usb_ready)
            {
                struct in_addr dns1, dns2;
                dns1.s_addr = dns2.s_addr = 0;
                if (get_primary_dns(fd, index_usb, &dns1) == 0 && get_secondary_dns(fd, index_usb, &dns2) == 0)
                {
                    unsigned long *p1 = (unsigned long *)&dns1;
                    unsigned long *p2 = (unsigned long *)&dns2;
                    ret = 0;
                    if ( *p1 != 0 && *p2 != 0)
                    {
                        snprintf(value, value_size, "%s", (char *)inet_ntoa(dns1));
                        if(strlen(value) < value_size)
                        {
                            char * temp = (char *) value;
                            snprintf(&temp[strlen(value)], value_size - strlen(value), ", %s", (char *)inet_ntoa(dns2));
                        }
                        else
                        {
                            ret = -1;
                            cellular_debug("Invalid parameter for dns_servers  (incorrect size %zu)\n", value_size);
                            goto done;
                        }
                    }
                    else if (*p1 != 0)
                    {
                        snprintf(value, value_size, "%s", (char *) inet_ntoa(dns1));
                    }
                    else if (*p2 != 0)
                    {
                        snprintf(value, value_size, "%s", (char *) inet_ntoa(dns2));
                    }
                }
            }
            break;

        case config_state_cellular_rx_packets:
        case config_state_cellular_tx_packets:
        case config_state_cellular_rx_bytes:
        case config_state_cellular_tx_bytes:
            /* These are not from CSID */
            break;
    }

done:
    return ret;
}

char const * const cellular_get_model(int const fd, size_t const index_3g)
{
    static char cellular_model[CELLULAR_MODEL_LENGTH] = "";

    char const * model = NULL;

    if (strlen(cellular_model) == 0)
    {
        if (get_model(fd, index_3g, cellular_model, sizeof cellular_model) == 0)
        {
            cellular_info("Model = %s\n", cellular_model);
            model = cellular_model;
        }
    }
    else
        model = cellular_model;

    return model;
}

cellular_connect_status_t cellular_get_connection_status(int const fd, size_t const index_3g)
{
    cellular_connect_status_t connect_status = cellular_disconnected;

    if (cellular_get_usb_ready(fd, index_3g))
    {
        size_t index_usb;
        if (get_usb_index(fd, index_3g, &index_usb) == 0)
        {
            get_connect_state(fd, index_usb, &connect_status);
        }
    }

    return connect_status;
}


char * cellular_get_interface_name(int const fd, size_t const index_3g)
{
    static char cellular_interface_name[CELLULAR_INTERFACE_NAME_LENGTH] = "";
    char * name = NULL;
    size_t index_usb;

    if ((get_usb_index(fd, index_3g, &index_usb) == 0) &&
        (get_interface_name(fd, index_usb, cellular_interface_name, sizeof cellular_interface_name) == 0))
    {
        cellular_printf("interface name = %s\n", cellular_interface_name);
        name = cellular_interface_name;
    }

    return name;
}

int cellular_set_active_sim(int const fd, size_t const index_3g, size_t const active_sim)
{
    int ret = set_active_sim(fd, index_3g, active_sim);

    return ret;
}

int cellular_get_active_sim(int const fd, size_t const index_3g, size_t * const active_sim)
{
    int ret = -1;

    if (get_active_sim(fd, index_3g, active_sim) == 0)
    {
        cellular_printf("Active SIM %d\n", active_sim);
        ret = 0;
    }

    return ret;
}

size_t cellular_get_switch_sim_card(int const fd, size_t const index_3g)
{

    uint32_t cardstatus = 0;

    if (read_csman(fd, CSID_S_3G_DUALSIM_CARDSTATUS+index_3g, &cardstatus, sizeof cardstatus, CSM_R_ZERO) < 0)
    {
        cellular_error("Could not read CSID_S_3G_DUALSIM_CARDSTATUS %d\n", fd);
    }


    return cardstatus;
}



int cellular_get_usb_index(int const fd, size_t const index_3g, size_t * const index_usb)
{
    int ret = get_usb_index(fd, index_3g, index_usb);

    return ret;
}


bool cellular_get_usb_ready(int const fd, size_t const index_3g)
{

    bool is_ready = is_usb_ready(fd, index_3g);
    return is_ready;
}


static cellular_modem_t const * get_cellular_default_modem(void)
{
    return &cellular_modem_table[0];
}


cellular_modem_t const * cellular_get_modem(int const fd, size_t const index_3g)
{
    char const * const model_name = cellular_get_model(fd, index_3g);

    cellular_modem_t const * modem = NULL;

    if ((model_name != NULL) && (strlen(model_name) > 0))
    {
        size_t i;
        for (i=0; i < ARRAY_SIZE(cellular_modem_table); i++)
        {
            if (strcmp(model_name, cellular_modem_table[i].model) == 0)
            {
                modem = &cellular_modem_table[i];
                break;
            }
        }

        if (modem == NULL)
        {
            /* no match modem in modem_table
             * set default modem.
             */
            modem = get_cellular_default_modem();
        }
    }

    return modem;
}

bool cellular_sim_ready(int const fd, size_t const index_3g)
{
    uint32_t status = SIM_STATUS_ABSENT;

    if (read_csman(fd, CSID_S_3G_MULTIWAN_SIM_STATUS+index_3g, &status, sizeof status, CSM_R_ZERO) < 0)
    {
        cellular_debug("Could not read SIM status %d\n", fd);
    }

    return (status != SIM_STATUS_ABSENT);

}

config_cellular_state_t get_cellular_state(size_t const instance)
{
    config_cellular_state_t state = config_cellular_state_off;
    void * value;

    if (tlr_get_element_alloc_value(config_cellular, instance, config_cellular_state, &value, config_mgr_element_type_enum,
                                    config_mgr_access_level_read_only) == config_status_success)
    {
        unsigned int * state_enum = value;
        state = *state_enum;
        free(value);
    }

    return state;
}

int cellular_get_connection_type(int const fd, size_t const index_3g, cellular_connection_type_t * const value)
{
    return get_connection_type(fd, index_3g, value);
}
int cellular_get_rssi(int const fd, size_t const index_3g, int32_t * const value)
{
    return get_rssi(fd, index_3g, value);
}

unsigned int cellular_get_pass_through_port_number(void)
{
    unsigned int port_number=0;

    config_status_t const status = tlr_get_element_value(config_system, 1, config_system_passthrough,
                                                         &port_number, sizeof port_number,
                                                         config_mgr_element_type_uint32, config_mgr_access_level_super);

    if (status != config_status_success)
    {
        cellular_error("Passthrough could not read port number\n");
        port_number = 0;
    }
    return port_number;
}

bool cellular_is_pass_through_enabled(void)
{
    return cellular_get_pass_through_port_number() > 0;
}
