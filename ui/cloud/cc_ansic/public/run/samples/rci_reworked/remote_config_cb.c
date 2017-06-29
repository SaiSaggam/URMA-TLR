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
#include  "remote_config.h"
#define  UNUSED_PARAMETER(a) (void)(a)


#define CONST const 
#define FIRMWARE_TARGET_ZERO_VERSION  0x1000000

#define CONNECTOR_RCI_ERROR_BAD_COMMAND (connector_remote_all_strings+0)
#define CONNECTOR_RCI_ERROR_BAD_DESCRIPTOR (connector_remote_all_strings+12)
#define SETTING_SERIAL_ERROR_INVALID_BAUD (connector_remote_all_strings+30)
#define SETTING_SERIAL_ERROR_INVALID_DATABITS (connector_remote_all_strings+49)
#define SETTING_SERIAL_ERROR_INVALID_PARITY (connector_remote_all_strings+67)
#define SETTING_SERIAL_ERROR_INVALID_XBREAK (connector_remote_all_strings+83)
#define SETTING_SERIAL_ERROR_INVALID_DATABITS_PARITY (connector_remote_all_strings+106)
#define SETTING_ETHERNET_ERROR_INVALID_DUPLEX (connector_remote_all_strings+150)
#define SETTING_ETHERNET_ERROR_INVALID_IP (connector_remote_all_strings+182)
#define SETTING_ETHERNET_ERROR_INVALID_SUBNET (connector_remote_all_strings+201)
#define SETTING_ETHERNET_ERROR_INVALID_GATEWAY (connector_remote_all_strings+221)
#define SETTING_ETHERNET_ERROR_INVALID_DNS (connector_remote_all_strings+245)
#define SETTING_DEVICE_TIME_ERROR_INVALID_TIME (connector_remote_all_strings+265)
#define STATE_DEVICE_STATE_ERROR_INVALID_INTEGER (connector_remote_all_strings+278)
#define CONNECTOR_GLOBAL_ERROR_LOAD_FAIL (connector_remote_all_strings+300)
#define CONNECTOR_GLOBAL_ERROR_SAVE_FAIL (connector_remote_all_strings+310)
#define CONNECTOR_GLOBAL_ERROR_MEMORY_FAIL (connector_remote_all_strings+320)

char CONST connector_remote_all_strings[] = {
 11,'B','a','d',' ','c','o','m','m','a','n','d',
 17,'B','a','d',' ','c','o','n','f','i','g','u','r','a','t','i','o','n',
 18,'I','n','v','a','l','i','d',' ','b','a','u','d',' ','r','a','t','e',' ',
 17,'I','n','v','a','l','i','d',' ','d','a','t','a',' ','b','i','t','s',
 15,' ','I','n','v','a','l','i','d',' ','p','a','r','i','t','y',
 22,'I','n','v','a','l','i','d',' ','x','b','r','e','a','k',' ','s','e','t','t','i','n','g',
 43,'I','n','v','a','l','i','d',' ','c','o','m','b','i','n','a','t','i','o','n',' ','o','f',' ','d','a','t','a',' ','b','i','t','s',' ','a','n','d',' ','p','a','r','i','t','y',
 31,'I','n','v','a','l','i','d',' ','e','t','h','e','r','n','e','t',' ','d','u','p','l','e','x',' ','s','e','t','t','i','n','g',
 18,'I','n','v','a','l','i','d',' ','I','P',' ','a','d','d','r','e','s','s',
 19,'I','n','v','a','l','i','d',' ','s','u','b','n','e','t',' ','m','a','s','k',
 23,'I','n','v','a','l','i','d',' ','g','a','t','e','w','a','y',' ','a','d','d','r','e','s','s',
 19,'I','n','v','a','l','i','d',' ','D','N','S',' ','a','d','d','r','e','s','s',
 12,'I','n','v','a','l','i','d',' ','t','i','m','e',
 21,'I','n','v','a','l','i','d',' ','i','n','t','e','g','e','r',' ','v','a','l','u','e',
 9,'L','o','a','d',' ','f','a','i','l',
 9,'S','a','v','e',' ','f','a','i','l',
 19,'I','n','s','u','f','f','i','c','i','e','n','t',' ','m','e','m','o','r','y'
};

static char const * const connector_rci_errors[] = {
 CONNECTOR_RCI_ERROR_BAD_COMMAND, /*bad_command*/
 CONNECTOR_RCI_ERROR_BAD_DESCRIPTOR, /*bad_descriptor*/
 CONNECTOR_GLOBAL_ERROR_LOAD_FAIL, /*load_fail*/
 CONNECTOR_GLOBAL_ERROR_SAVE_FAIL, /*save_fail*/
 CONNECTOR_GLOBAL_ERROR_MEMORY_FAIL /*memory_fail*/
};

static connector_group_element_t CONST setting_serial_elements[] = {
 {  /*baud*/
   connector_element_access_read_write,
   connector_element_type_enum,
   (rci_function_t)rci_setting_serial_baud_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_serial_baud_get  /* cast is mandatory*/
 },
 {  /*parity*/
   connector_element_access_read_write,
   connector_element_type_enum,
   (rci_function_t)rci_setting_serial_parity_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_serial_parity_get  /* cast is mandatory*/
 },
 {  /*databits*/
   connector_element_access_read_write,
   connector_element_type_uint32,
   (rci_function_t)rci_setting_serial_databits_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_serial_databits_get  /* cast is mandatory*/
 },
 {  /*xbreak*/
   connector_element_access_read_write,
   connector_element_type_on_off,
   (rci_function_t)rci_setting_serial_xbreak_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_serial_xbreak_get  /* cast is mandatory*/
 },
 {  /*txbytes*/
   connector_element_access_read_only,
   connector_element_type_uint32,
   NULL,
   (rci_function_t)rci_setting_serial_txbytes_get  /* cast is mandatory*/
 }
};

static char CONST * CONST setting_serial_errors[] = {
 SETTING_SERIAL_ERROR_INVALID_BAUD, /*invalid_baud*/
 SETTING_SERIAL_ERROR_INVALID_DATABITS, /*invalid_databits*/
 SETTING_SERIAL_ERROR_INVALID_PARITY, /*invalid_parity*/
 SETTING_SERIAL_ERROR_INVALID_XBREAK, /*invalid_xbreak*/
 SETTING_SERIAL_ERROR_INVALID_DATABITS_PARITY /*invalid_databits_parity*/
};

static connector_group_element_t CONST setting_ethernet_elements[] = {
 {  /*ip*/
   connector_element_access_read_write,
   connector_element_type_ipv4,
   (rci_function_t)rci_setting_ethernet_ip_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_ethernet_ip_get  /* cast is mandatory*/
 },
 {  /*subnet*/
   connector_element_access_read_write,
   connector_element_type_ipv4,
   (rci_function_t)rci_setting_ethernet_subnet_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_ethernet_subnet_get  /* cast is mandatory*/
 },
 {  /*gateway*/
   connector_element_access_read_write,
   connector_element_type_ipv4,
   (rci_function_t)rci_setting_ethernet_gateway_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_ethernet_gateway_get  /* cast is mandatory*/
 },
 {  /*dhcp*/
   connector_element_access_read_write,
   connector_element_type_boolean,
   (rci_function_t)rci_setting_ethernet_dhcp_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_ethernet_dhcp_get  /* cast is mandatory*/
 },
 {  /*dns*/
   connector_element_access_read_write,
   connector_element_type_fqdnv4,
   (rci_function_t)rci_setting_ethernet_dns_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_ethernet_dns_get  /* cast is mandatory*/
 },
 {  /*duplex*/
   connector_element_access_read_write,
   connector_element_type_enum,
   (rci_function_t)rci_setting_ethernet_duplex_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_ethernet_duplex_get  /* cast is mandatory*/
 }
};

static char CONST * CONST setting_ethernet_errors[] = {
 SETTING_ETHERNET_ERROR_INVALID_DUPLEX, /*invalid_duplex*/
 SETTING_ETHERNET_ERROR_INVALID_IP, /*invalid_ip*/
 SETTING_ETHERNET_ERROR_INVALID_SUBNET, /*invalid_subnet*/
 SETTING_ETHERNET_ERROR_INVALID_GATEWAY, /*invalid_gateway*/
 SETTING_ETHERNET_ERROR_INVALID_DNS /*invalid_dns*/
};

static connector_group_element_t CONST setting_device_time_elements[] = {
 {  /*curtime*/
   connector_element_access_read_write,
   connector_element_type_datetime,
   (rci_function_t)rci_setting_device_time_curtime_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_device_time_curtime_get  /* cast is mandatory*/
 }
};

static char CONST * CONST setting_device_time_errors[] = {
 SETTING_DEVICE_TIME_ERROR_INVALID_TIME /*invalid_time*/
};

static connector_group_element_t CONST setting_device_info_elements[] = {
 {  /*version*/
   connector_element_access_read_only,
   connector_element_type_0x_hex32,
   NULL,
   (rci_function_t)rci_setting_device_info_version_get  /* cast is mandatory*/
 },
 {  /*product*/
   connector_element_access_read_write,
   connector_element_type_string,
   (rci_function_t)rci_setting_device_info_product_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_device_info_product_get  /* cast is mandatory*/
 },
 {  /*model*/
   connector_element_access_read_write,
   connector_element_type_string,
   (rci_function_t)rci_setting_device_info_model_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_device_info_model_get  /* cast is mandatory*/
 },
 {  /*company*/
   connector_element_access_read_write,
   connector_element_type_string,
   (rci_function_t)rci_setting_device_info_company_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_device_info_company_get  /* cast is mandatory*/
 },
 {  /*desc*/
   connector_element_access_read_write,
   connector_element_type_multiline_string,
   (rci_function_t)rci_setting_device_info_desc_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_device_info_desc_get  /* cast is mandatory*/
 }
};

static connector_group_element_t CONST setting_system_elements[] = {
 {  /*description*/
   connector_element_access_read_write,
   connector_element_type_string,
   (rci_function_t)rci_setting_system_description_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_system_description_get  /* cast is mandatory*/
 },
 {  /*contact*/
   connector_element_access_read_write,
   connector_element_type_string,
   (rci_function_t)rci_setting_system_contact_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_system_contact_get  /* cast is mandatory*/
 },
 {  /*location*/
   connector_element_access_read_write,
   connector_element_type_string,
   (rci_function_t)rci_setting_system_location_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_system_location_get  /* cast is mandatory*/
 }
};

static connector_group_element_t CONST setting_devicesecurity_elements[] = {
 {  /*identityVerificationForm*/
   connector_element_access_read_write,
   connector_element_type_enum,
   (rci_function_t)rci_setting_devicesecurity_identityVerificationForm_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_devicesecurity_identityVerificationForm_get  /* cast is mandatory*/
 },
 {  /*password*/
   connector_element_access_write_only,
   connector_element_type_password,
   (rci_function_t)rci_setting_devicesecurity_password_set,  /* cast is mandatory*/
   (rci_function_t)rci_setting_devicesecurity_password_get  /* cast is mandatory*/
 }
};

static connector_group_t CONST connector_setting_groups[] = {
 {  /*serial*/
   2 , /* instances */
   { ARRAY_SIZE(setting_serial_elements),
     setting_serial_elements
   },
   { ARRAY_SIZE(setting_serial_errors),
     setting_serial_errors
   },  /* errors*/
   (rci_function_t)rci_setting_serial_start, /* start callback*/
   (rci_function_t)rci_setting_serial_end  /* end callback*/
 }
,
 {  /*ethernet*/
   1 , /* instances */
   { ARRAY_SIZE(setting_ethernet_elements),
     setting_ethernet_elements
   },
   { ARRAY_SIZE(setting_ethernet_errors),
     setting_ethernet_errors
   },  /* errors*/
   (rci_function_t)rci_setting_ethernet_start, /* start callback*/
   (rci_function_t)rci_setting_ethernet_end  /* end callback*/
 }
,
 {  /*device_time*/
   1 , /* instances */
   { ARRAY_SIZE(setting_device_time_elements),
     setting_device_time_elements
   },
   { ARRAY_SIZE(setting_device_time_errors),
     setting_device_time_errors
   },  /* errors*/
   (rci_function_t)rci_setting_device_time_start, /* start callback*/
   (rci_function_t)rci_setting_device_time_end  /* end callback*/
 }
,
 {  /*device_info*/
   1 , /* instances */
   { ARRAY_SIZE(setting_device_info_elements),
     setting_device_info_elements
   },
   { 0,
     NULL
   },  /* errors*/
   (rci_function_t)rci_setting_device_info_start, /* start callback*/
   (rci_function_t)rci_setting_device_info_end  /* end callback*/
 }
,
 {  /*system*/
   1 , /* instances */
   { ARRAY_SIZE(setting_system_elements),
     setting_system_elements
   },
   { 0,
     NULL
   },  /* errors*/
   (rci_function_t)rci_setting_system_start, /* start callback*/
   (rci_function_t)rci_setting_system_end  /* end callback*/
 }
,
 {  /*devicesecurity*/
   1 , /* instances */
   { ARRAY_SIZE(setting_devicesecurity_elements),
     setting_devicesecurity_elements
   },
   { 0,
     NULL
   },  /* errors*/
   (rci_function_t)rci_setting_devicesecurity_start, /* start callback*/
   (rci_function_t)rci_setting_devicesecurity_end  /* end callback*/
 }

};

static connector_group_element_t CONST state_device_state_elements[] = {
 {  /*system_up_time*/
   connector_element_access_read_only,
   connector_element_type_uint32,
   NULL,
   (rci_function_t)rci_state_device_state_system_up_time_get  /* cast is mandatory*/
 },
 {  /*signed_integer*/
   connector_element_access_read_write,
   connector_element_type_int32,
   (rci_function_t)rci_state_device_state_signed_integer_set,  /* cast is mandatory*/
   (rci_function_t)rci_state_device_state_signed_integer_get  /* cast is mandatory*/
 }
};

static char CONST * CONST state_device_state_errors[] = {
 STATE_DEVICE_STATE_ERROR_INVALID_INTEGER /*invalid_integer*/
};

static connector_group_element_t CONST state_gps_stats_elements[] = {
 {  /*latitude*/
   connector_element_access_read_only,
   connector_element_type_string,
   NULL,
   (rci_function_t)rci_state_gps_stats_latitude_get  /* cast is mandatory*/
 },
 {  /*longitude*/
   connector_element_access_read_only,
   connector_element_type_string,
   NULL,
   (rci_function_t)rci_state_gps_stats_longitude_get  /* cast is mandatory*/
 }
};

static connector_group_t CONST connector_state_groups[] = {
 {  /*device_state*/
   1 , /* instances */
   { ARRAY_SIZE(state_device_state_elements),
     state_device_state_elements
   },
   { ARRAY_SIZE(state_device_state_errors),
     state_device_state_errors
   },  /* errors*/
   (rci_function_t)rci_state_device_state_start, /* start callback*/
   (rci_function_t)rci_state_device_state_end  /* end callback*/
 }
,
 {  /*gps_stats*/
   1 , /* instances */
   { ARRAY_SIZE(state_gps_stats_elements),
     state_gps_stats_elements
   },
   { 0,
     NULL
   },  /* errors*/
   (rci_function_t)rci_state_gps_stats_start, /* start callback*/
   (rci_function_t)rci_state_gps_stats_end  /* end callback*/
 }

};

static connector_remote_group_table_t CONST connector_group_table[] = {
 { connector_setting_groups,
   ARRAY_SIZE(connector_setting_groups)
 },
 { connector_state_groups,
   ARRAY_SIZE(connector_state_groups)
 }
};


connector_remote_config_data_t rci_desc_data = {
    connector_group_table,
    connector_rci_errors,
    connector_rci_error_COUNT,
    FIRMWARE_TARGET_ZERO_VERSION
};

connector_callback_status_t rci_session_start_cb(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_session_end_cb(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_serial_start(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_serial_end(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_serial_baud_get(rci_info_t * const info, connector_setting_serial_baud_id_t * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = connector_setting_serial_baud_2400;
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_serial_baud_set(rci_info_t * const info, connector_setting_serial_baud_id_t const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_serial_parity_get(rci_info_t * const info, connector_setting_serial_parity_id_t * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = connector_setting_serial_parity_none;
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_serial_parity_set(rci_info_t * const info, connector_setting_serial_parity_id_t const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_serial_databits_get(rci_info_t * const info, uint32_t * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = 5;
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_serial_databits_set(rci_info_t * const info, uint32_t const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_serial_xbreak_get(rci_info_t * const info, connector_on_off_t * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = connector_on;
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_serial_xbreak_set(rci_info_t * const info, connector_on_off_t const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_serial_txbytes_get(rci_info_t * const info, uint32_t * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = 123;
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_ethernet_start(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_ethernet_end(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_ethernet_ip_get(rci_info_t * const info, char const * * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = "192.168.1.1";
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_ethernet_ip_set(rci_info_t * const info, char const * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_ethernet_subnet_get(rci_info_t * const info, char const * * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = "192.168.1.1";
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_ethernet_subnet_set(rci_info_t * const info, char const * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_ethernet_gateway_get(rci_info_t * const info, char const * * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = "192.168.1.1";
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_ethernet_gateway_set(rci_info_t * const info, char const * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_ethernet_dhcp_get(rci_info_t * const info, connector_bool_t * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = connector_true;
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_ethernet_dhcp_set(rci_info_t * const info, connector_bool_t const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_ethernet_dns_get(rci_info_t * const info, char const * * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = "192.168.1.1";
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_ethernet_dns_set(rci_info_t * const info, char const * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_ethernet_duplex_get(rci_info_t * const info, connector_setting_ethernet_duplex_id_t * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = connector_setting_ethernet_duplex_auto;
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_ethernet_duplex_set(rci_info_t * const info, connector_setting_ethernet_duplex_id_t const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_device_time_start(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_device_time_end(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_device_time_curtime_get(rci_info_t * const info, char const * * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = "2002-05-30T09:30:10-0600";;
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_device_time_curtime_set(rci_info_t * const info, char const * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_device_info_start(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_device_info_end(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_device_info_version_get(rci_info_t * const info, uint32_t * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = 0x20101010;
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_device_info_product_get(rci_info_t * const info, char const * * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = "String";
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_device_info_product_set(rci_info_t * const info, char const * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_device_info_model_get(rci_info_t * const info, char const * * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = "String";
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_device_info_model_set(rci_info_t * const info, char const * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_device_info_company_get(rci_info_t * const info, char const * * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = "String";
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_device_info_company_set(rci_info_t * const info, char const * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_device_info_desc_get(rci_info_t * const info, char const * * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = "String";
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_device_info_desc_set(rci_info_t * const info, char const * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_system_start(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_system_end(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_system_description_get(rci_info_t * const info, char const * * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = "String";
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_system_description_set(rci_info_t * const info, char const * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_system_contact_get(rci_info_t * const info, char const * * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = "String";
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_system_contact_set(rci_info_t * const info, char const * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_system_location_get(rci_info_t * const info, char const * * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = "String";
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_system_location_set(rci_info_t * const info, char const * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_devicesecurity_start(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_devicesecurity_end(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_devicesecurity_identityVerificationForm_get(rci_info_t * const info, connector_setting_devicesecurity_identityVerificationForm_id_t * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = connector_setting_devicesecurity_identityVerificationForm_simple;
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_devicesecurity_identityVerificationForm_set(rci_info_t * const info, connector_setting_devicesecurity_identityVerificationForm_id_t const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_setting_devicesecurity_password_set(rci_info_t * const info, char const * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_state_device_state_start(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_state_device_state_end(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_state_device_state_system_up_time_get(rci_info_t * const info, uint32_t * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = 123;
    return connector_callback_continue;
}

connector_callback_status_t rci_state_device_state_signed_integer_get(rci_info_t * const info, int32_t * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = -100;
    return connector_callback_continue;
}

connector_callback_status_t rci_state_device_state_signed_integer_set(rci_info_t * const info, int32_t const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    UNUSED_PARAMETER(value);
    return connector_callback_continue;
}

connector_callback_status_t rci_state_gps_stats_start(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_state_gps_stats_end(rci_info_t * const info)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    return connector_callback_continue;
}

connector_callback_status_t rci_state_gps_stats_latitude_get(rci_info_t * const info, char const * * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = "String";
    return connector_callback_continue;
}

connector_callback_status_t rci_state_gps_stats_longitude_get(rci_info_t * const info, char const * * const value)
{
    UNUSED_PARAMETER(info);
    printf("    Called '%s'\n", __FUNCTION__);
    *value = "String";
    return connector_callback_continue;
}
