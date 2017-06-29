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
#include "csman.h"
#include "csid/csid_3g.h"
#include "csid/csid_usb.h"
#include "tlr_common.h"
#include "tlr_format_string.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>



static uint32_t const amit_sim_mode[] = {DUALSIMCARD_MODE_DISABLE, DUALSIMCARD_MODE_SIMAONLY, DUALSIMCARD_MODE_SIMBONLY};

bool is_usb_ready(int const fd, size_t const index_3g)
{

    uint32_t is_ready = 0;

    if (read_csman(fd, CSID_S_3G_MAPPING_STATUS+index_3g, &is_ready, sizeof is_ready, CSM_R_ZERO) < 0)
    {
        cellular_error("Could not read CSID_S_3G_MAPPING_STATUS usb ready %d\n", fd);
    }
    else
    {
        cellular_printf("usb 3G mapping status = %zd\n", is_ready);
    }


    return (is_ready == 1);
}

int get_usb_index(int const fd, size_t const index_3g, size_t * const index_usb)
{
    int ret = -1;

    uint32_t usb_idx;

    if (read_csman(fd, CSID_S_3G_MAPPING_INDEX+index_3g, &usb_idx, sizeof usb_idx, CSM_R_ZERO) < 0)
        cellular_error("Could not read CSID_S_3G_MAPPING_INDEX usb index %d\n", fd);
    else
    {
        ret = 0;
        *index_usb = (size_t)usb_idx;
        //cellular_debug("usb index = %zd\n", usb_idx);
    }

    return ret;
}

int get_connect_state(int const fd, int const index_usb, cellular_connect_status_t * const state)
{
    int ret = 0;
    uint32_t status;

    if (state == NULL)
    {
        ret = -1;
        cellular_debug("Invalid connected state parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_USB_CNT_STATUS+index_usb, &status, sizeof status, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read connected state usb index %ul %d\n", index_usb, fd);
        goto done;
    }

    switch (status)
    {
    default:
        *state = cellular_disconnected;
        break;
    case CNT_STATUS_3G_DISCONNECTING:
        *state = cellular_disconnecting;
        break;
    case CNT_STATUS_3G_CONNECTING:
        *state = cellular_connecting;
        break;
    case CNT_STATUS_3G_CONNECT:
        *state = cellular_connected;
        break;
    }

done:
    return ret;
}

int get_interface_name(int const fd, int const index_usb, char * const name, size_t const name_length)
{
    int ret = 0;

    if ((name == NULL) || (name_length == 0))
    {
        ret = -1;
        cellular_debug("Invalid interface name parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_USB_CNT_IF_NAME + index_usb, name, name_length, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read connected interface name %d\n", fd);
    }

done:
    return ret;
}


int get_model(int const fd, size_t const index_3g, char * const name, size_t const name_length)
{
    int ret = 0;

    if ((name == NULL) || (name_length == 0))
    {
        ret = -1;
        cellular_debug("Invalid model parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_3G_MULTIWAN_CARD_INFO+index_3g, name, name_length, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read model name %d\n", fd);
    }

    tlr_string_strip_trailing_newline(name);

done:
    return ret;
}

int get_manufacturer(int const fd, size_t const index_3g, char * const name, size_t const name_length)
{
    int ret = 0;

    if ((name == NULL) || (name_length == 0))
    {
        ret = -1;
        cellular_debug("Invalid manufacturer parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_3G_MULTIWAN_MANUFACTURER+index_3g, name,
                name_length, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read manufacturer name %d\n", fd);
    }

done:
    return ret;
}

int get_imei(int const fd, size_t const index_3g, char * const value, size_t const value_size)
{
    int ret = 0;

    if ((value == NULL) || (value_size == 0))
    {
        ret = -1;
        cellular_debug("Invalid imei parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_3G_MULTIWAN_IMEI+index_3g, value,
                value_size, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read cellular IMEI %d\n", fd);
    }

done:
    return ret;
}

int get_firmware_version(int const fd, size_t const index_3g, char * const name, size_t const name_length)
{
    int ret = 0;

    if ((name == NULL) || (name_length == 0) )
    {
        ret = -1;
        cellular_debug("Invalid firmware version parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_3G_MULTIWAN_CARD_VERSION+index_3g, name,
                    name_length, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read cellular firmware version %d\n", fd);
    }

done:
    return ret;
}

int get_hardware_version(int const fd, size_t const index_3g, char * const name, size_t const name_length)
{
    int ret = 0;

    if ((name == NULL) || (name_length == 0) )
    {
        ret = -1;
        cellular_debug("Invalid hardware version parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_3G_MULTIWAN_CARD_HW_VER+index_3g, name,
                    name_length, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read cellular hardwareware version %d\n", fd);
    }

done:
    return ret;
}


int get_ip_address(int const fd, int const index_usb, struct in_addr * ipaddr)
{
    int ret = 0;

    if (ipaddr == NULL)
    {
        ret = -1;
        cellular_debug("Invalid ip address parameter call\n");
        goto done;
    }

    cellular_debug("cellular ip address usb index = %d\n", index_usb);
    if (read_csman(fd, CSID_S_USB_IF_IP+index_usb, ipaddr, sizeof *ipaddr, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read cellular IP address %d\n", fd);
    }

done:
    return ret;
}

int get_netmask(int const fd, int const index_usb, struct in_addr * mask)
{
    int ret = 0;

    if (mask == NULL)
    {
        ret = -1;
        cellular_debug("Invalid netmask parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_USB_IF_NM+index_usb, mask, sizeof *mask, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read cellular netmask %d\n", fd);
    }

done:
    return ret;
}

int get_gateway(int const fd, int const index_usb, struct in_addr * gateway)
{
    int ret = 0;

    if ((gateway == NULL))
    {
        ret = -1;
        cellular_debug("Invalid gateway parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_USB_IF_GW+index_usb, gateway, sizeof *gateway, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read cellular gateway %d\n", fd);
    }

done:
    return ret;
}

int get_primary_dns(int const fd, int const index_usb, struct in_addr * dns)
{
    int ret = 0;

    if (dns == NULL)
    {
        ret = -1;
        cellular_debug("Invalid primary dns parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_USB_IF_PRIDNS+index_usb, dns, sizeof *dns, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read cellular primary DNS %d\n", fd);
    }

done:
    return ret;
}

int get_secondary_dns(int const fd, int const index_usb, struct in_addr * dns)
{
    int ret = 0;

    if (dns == NULL)
    {
        ret = -1;
        cellular_debug("Invalid secondary dns parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_USB_IF_SECDNS+index_usb, dns, sizeof *dns, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read cellular secondary DNS %d\n", fd);
    }

done:
    return ret;
}


int get_active_sim(int const fd, size_t const index_3g, size_t * const active_sim)
{
    int ret = -1;
    uint32_t sim_mode;
    size_t i;

    if (active_sim == NULL)
    {
        ret = -1;
        cellular_debug("Invalid active sim parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_C_3G_MULTIWAN_SIMCARD_MODE+index_3g, &sim_mode,
                    sizeof sim_mode, 0) < 0)
    {
        cellular_debug("Could not read active sim mode on %d fd\n", fd);
        goto done;
    }

    for (i = 0; i < (sizeof amit_sim_mode/sizeof amit_sim_mode[0]); i++)
    {
        if (sim_mode == amit_sim_mode[i])
        {
            *active_sim = i;
            ret = 0;
            break;
        }
    }

done:
    return ret;
}


int set_active_sim(int const fd, size_t const index_3g, size_t const active_sim)
{
    int rc = 0;

    if (active_sim >= ARRAY_SIZE(amit_sim_mode))
    {
        rc = -1;
        cellular_debug("Unable to set active sim. Invalid active sim %d\n", active_sim);
        goto done;
    }

    cellular_debug("set sim mode = %d\n", amit_sim_mode[active_sim]);

    if (write_csman(fd, CSID_C_3G_MULTIWAN_SIMCARD_MODE+index_3g, &amit_sim_mode[active_sim],
                    sizeof amit_sim_mode[active_sim], 0) < 0)
    {
        cellular_debug("Failed writing active sim %d\n", fd);
        rc = -1;
    }
    else
    {
        cellular_debug("Active SIM %d\n", active_sim);
    }

done:
    return rc;
}

int get_rssi(int const fd, size_t const index_3g, int32_t * const value)
{
    int ret = 0;

    if (value == NULL)
    {
        ret = -1;
        cellular_debug("Invalid rssi parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_3G_MULTIWAN_RSSI+index_3g , value, sizeof *value, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read RSSI %d\n", fd);
    }
    else
        *value *= -1;

done:
    return ret;
}

int get_ecio(int const fd, size_t const index_3g, int32_t * const value)
{
    int ret = 0;
    //char ecio_string[INTEGER_STRING_LENGTH];

    if (value == NULL)
    {
        ret = -1;
        cellular_debug("Invalid ec/io parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_3G_MULTIWAN_ECIO+index_3g, value, sizeof *value, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read EC/IO %d\n", fd);
    }
    else
        *value *= -1;

done:
    return ret;
}

int get_registration_status(int const fd, size_t const index_3g, cellular_registration_status_t * const value)
{
    int ret = 0;
    uint32_t reg_status;

    if (value == NULL)
    {
        ret = -1;
        cellular_debug("Invalid registration status parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_3G_MULTIWAN_REG_STATUS+index_3g, &reg_status,
                    sizeof reg_status, 0) < 0)
    {
        ret = -1;
        cellular_debug("Could not read registration status %d\n", fd);
        goto done;
    }

    switch (reg_status)
    {
    case REG_STATUS_IDLE:
        *value = cellular_registration_status_idle;
        break;
    case REG_STATUS_REGISTERED:
        *value = cellular_registration_status_registered;
        break;
    case REG_STATUS_SEARCH:
        *value = cellular_registration_status_search;
        break;
    case REG_STATUS_ROAMING:
        *value = cellular_registration_status_roaming;
        break;
    }

done:
    return ret;
}

int get_network_name(int const fd, size_t const index_3g, char * const name, size_t const name_length)
{
    int ret = 0;

    if ((name == NULL) || (name_length == 0))
    {
        ret = -1;
        cellular_debug("Invalid network name parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_3G_MULTIWAN_NETWORK_NAME+index_3g, name,
                    name_length, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read cellular network name %d\n", fd);
    }

done:
    return ret;
}

int get_mcc(int const fd, size_t const index_3g, unsigned int * const mcc)
{
    int ret = 0;
    uint32_t value;

    if (mcc == NULL)
    {
        ret = -1;
        cellular_debug("Invalid MCC parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_3G_MULTIWAN_MCC+index_3g, &value,
                    sizeof value, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read MCC %d\n", fd);
    }
    else
    {
        *mcc = (unsigned int)value;
    }

done:
    return ret;
}

int get_mnc(int const fd, size_t const index_3g, unsigned int * const mnc)
{
    int ret = 0;
    uint32_t value;

    if (mnc == NULL)
    {
        ret = -1;
        cellular_debug("Invalid MCC parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_3G_MULTIWAN_MCC+index_3g, &value,
                    sizeof value, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read MCC %d\n", fd);
    }
    else
    {
        *mnc = (unsigned int)value;
    }

done:
    return ret;
}

int get_connection_type(int const fd, size_t const index_3g, cellular_connection_type_t * const value)
{
    int ret = 0;
    uint32_t type;

    if (value == NULL)
    {
        ret = -1;
        cellular_debug("Invalid connection type parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_3G_MULTIWAN_SERVICE_TYPE+index_3g, &type, sizeof type, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read connection type %d\n", fd);
    }
    else
    {
        /* 0:2G,1:none ,2:3G,3:3.5G,4~6:3.75G,7:LTE */
        switch (type)
        {
            case 0:
                *value = cellular_conenction_type_2g;
                break;
            case 2:
            case 3:
            case 4:
                *value = cellular_conenction_type_3g;
                break;
            case 7:
                *value = cellular_conenction_type_4g;
                break;
            default:
            case 1:
                *value = cellular_connection_type_unknown;
                break;
        }
    }

done:
    return ret;
}

int get_radio_band(int const fd, size_t const index_3g, char * const name, size_t const name_length)
{
    int ret = 0;

    if ((name == NULL) || (name_length == 0))
    {
        ret = -1;
        cellular_debug("Invalid radio band parameter call\n");
        goto done;
    }


    if (read_csman(fd, CSID_S_3G_MULTIWAN_ACTIVE_BAND+index_3g, name,
                    name_length, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read radio band %d\n", fd);
    }
    else if (strcmp(name, "UNKNOWN") == 0)
    {
        name[0]='\0';
    }

    cellular_printf("radio band = %s\n", name);

done:
    return ret;
}

/* TODO: Remove when AMIT has channel CSID available KEEP THIS FOR NOW FOR TEST */
#if 0
static int get_rfsts_channel(int const fd, size_t const index_3g, uint32_t * const channel)
{
    static char buffer[128];
    static char *rfsts_items[23];
    int ret=-1;
    char at_device[16]="";
    FILE *fp = NULL;
    size_t index_usb;

    if (get_usb_index(fd, index_3g, &index_usb) < 0)
    {
        goto done;
    }

    if (read_csman(fd, CSID_S_USB_MODEM2+index_usb, at_device,
                    sizeof at_device, CSM_R_ZERO) < 0)
    {
        cellular_debug("Could not read radio band\n");
        goto done;
    }

    if (*at_device == '\0')
    {
        cellular_debug("No AT device yet\n");
        goto done;
    }

    snprintf(buffer, sizeof buffer, "send_ATcmd /dev/%s AT#RFSTS", at_device);

    fp = popen(buffer, "r");
    if (fp == NULL) goto done;

    {
        char * ptr = buffer;
        size_t available_length = sizeof buffer;
        ssize_t length;

        do
        {
            length = fread(ptr, sizeof(char), available_length, fp);

            if (length > 0)
            {
                ptr += length;
                available_length -= length;
            }
        } while (length > 0);

        if (available_length == sizeof buffer)
            goto done;

    }

    {
        char * ptr = strstr(buffer, "RFSTS:");
        size_t i;

        if (ptr == NULL)
        {
            cellular_debug("Unable to find RFSTS response: %s\n", buffer);
            goto done;
        }

        cellular_printf("AT#RFSTS: %s\n", buffer);

        for (i= 0; (i < (sizeof rfsts_items / sizeof rfsts_items[0])) && (ptr != NULL); i++)
        {
            rfsts_items[i] = ++ptr;

            ptr = strchr(ptr, ',');
            if (ptr != NULL)
            {
                *ptr = '\0';
            }

            cellular_printf("%d: %s\n", i, rfsts_items[i]);
        }

        cellular_printf("AT#RSTS contains %zd items\n", i);

        if (i == 23) /* GSM */
        {
            cellular_printf("GSM Channel = %s\n", rfsts_items[1]);
        }
        else if (i == 16)  /* LTE */
        {
            cellular_printf("LTE Channel = %s\n", rfsts_items[1]);
        }
        else
        {
            cellular_printf("WCDMA Channel = %s\n", rfsts_items[1]);
        }

        *channel = atoi(rfsts_items[1]);
        ret = 0;
    }

done:
    if (fp != NULL)
    {
        pclose(fp);
    }
    return ret;
}
#endif
int get_channel(int const fd, size_t const index_3g, unsigned int * const channel)
{
    int ret = 0;
    uint32_t value = 0;

    if (channel == NULL)
    {
        ret = -1;
        cellular_debug("Invalid channel parameter call\n");
        goto done;
    }

#if 1
    if (read_csman(fd, CSID_S_3G_MULTIWAN_ARFCN+index_3g, &value,
                    sizeof value, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read channel %d\n", fd);
        goto done;
    }
#else
    {
        /* testing only */
        ret = get_rfsts_channel(fd, index_3g, &value);
        if (ret < 0) goto done;
    }
#endif
    *channel = (unsigned int)value;

done:
    return ret;
}

int get_temperature(int const fd, size_t const index_3g, int32_t * temperature)
{
    int ret = 0;
    int32_t value;

    if (temperature == NULL)
    {
        ret = -1;
        cellular_debug("Invalid temperature parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_3G_MULTIWAN_TEMPERATURE+index_3g, &value,
                    sizeof value, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read temperature %d\n", fd);
    }
    else
    {
        *temperature = value;
    }

done:
    return ret;
}

int get_pdp_context_pid(int const fd, size_t const index_3g, uint32_t * const value)
{
    int ret = 0;
    uint32_t pid;

    if (value == NULL)
    {
        ret = -1;
        cellular_debug("Invalid PDP context pid parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_3G_APN_PID+index_3g, &pid,
                    sizeof pid, 0) < 0)
    {
        ret = -1;
        cellular_debug("Could not read PDP context pid %d\n", fd);
        goto done;
    }

    *value = pid;

done:
    return ret;
}


int get_pdp_context_apn(int const fd, size_t const index_3g, char * const name, size_t const name_length)
{
    int ret = 0;

    if ((name == NULL) || (name_length == 0))
    {
        ret = -1;
        cellular_debug("Invalid PDP context APN parameter call\n");
        goto done;
    }

    if (read_csman(fd, CSID_S_3G_APN_WTIH_MODULE+index_3g, name,
                    name_length, CSM_R_ZERO) < 0)
    {
        ret = -1;
        cellular_debug("Could not read cellular PDP context APN %d\n", fd);
    }

done:
    return ret;
}
