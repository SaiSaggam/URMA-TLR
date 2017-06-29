#include "connector_app.h"

static void get_mac_addr(uint8_t ** addr, size_t * size){
    *addr = AppConfig.MyMACAddr.v;
    *size = sizeof AppConfig.MyMACAddr.v;
}

static void get_device_id(uint8_t ** device_id, size_t * size){

    static uint8_t device_id_arr[DEVICE_ID_LENGTH] = { 0 };
    device_id_arr[8] = AppConfig.MyMACAddr.v[0];
    device_id_arr[9] = AppConfig.MyMACAddr.v[1];
    device_id_arr[10]= AppConfig.MyMACAddr.v[2];
    device_id_arr[11]= 0xFF;
    device_id_arr[12]= 0xFF;
    device_id_arr[13]= AppConfig.MyMACAddr.v[3];
    device_id_arr[14]= AppConfig.MyMACAddr.v[4];
    device_id_arr[15]= AppConfig.MyMACAddr.v[5];

    *device_id = device_id_arr;
    *size = sizeof device_id_arr;
}

static void get_ip_addr(uint8_t ** ip_addr, size_t * size){

    *ip_addr = AppConfig.MyIPAddr.v;
    *size = sizeof AppConfig.MyIPAddr.v;
}

static void get_connection_type(connector_connection_type_t ** type){
    static connector_connection_type_t device_connection_type;
    device_connection_type = connector_config.connection_type;
    *type = &device_connection_type;
}

static connector_callback_status_t app_start_network_tcp(connector_auto_connect_type_t * const connect_type)
{
    *connect_type = connector_auto_connect;

    return connector_callback_continue;
}

connector_callback_status_t connector_config_callback(connector_config_request_t const request_id,
                                            const void * const request_data,
                                            void * response_data,
                                            size_t * const response_length){

    connector_error_status_t * error_data;
    
    switch(request_id){
        case connector_config_device_id:
            get_device_id((uint8_t **)response_data, response_length);
            break;
        case connector_config_vendor_id:
            *(uint8_t **)response_data = (uint8_t *)connector_config.vendor_id;
            *response_length = sizeof(connector_config.vendor_id);
            break;
        case connector_config_device_type:
            *(char **)response_data = connector_config.device_type;
            *response_length = strlen(connector_config.device_type);
            break;
        case connector_config_server_url:
            *(char **)response_data = (char *)connector_config.server_url;
            *response_length = strlen(connector_config.server_url);
            break;
        case connector_config_connection_type:
            get_connection_type((connector_connection_type_t **)response_data);
            break;
        case connector_config_mac_addr:
            get_mac_addr((uint8_t **)response_data, response_length);
            break;
        case connector_config_link_speed:
            *(uint32_t **)response_data = &connector_config.link_speed;
            *response_length = sizeof(connector_config.link_speed);
            break;
        case connector_config_phone_number:
            *(uint8_t **)response_data = &connector_config.phone_number;
            *response_length = sizeof(connector_config.phone_number);
            break;
        case connector_config_tx_keepalive:
            *(uint16_t **)response_data = &connector_config.tx_keepalive;
            *response_length = sizeof(connector_config.tx_keepalive);
            break;
        case connector_config_rx_keepalive:
            *(uint16_t **)response_data = &connector_config.rx_keepalive;
            *response_length = sizeof(connector_config.rx_keepalive);
            break;
        case connector_config_wait_count:
            *(uint16_t **)response_data = &connector_config.wait_count;
            *response_length = sizeof(connector_config.wait_count);
            break;
        case connector_config_ip_addr:
            get_ip_addr((uint8_t **)response_data, response_length);
            break;
        case connector_config_error_status:
            error_data = (connector_error_status_t *)request_data;
            DEBUG_PRINTF("Error Encountered Class: [%d], Status: [%d]!\n", error_data->class_id, error_data->status);
            break;
        case connector_config_firmware_facility:
            *((bool *)response_data) = false;
            break;
        case connector_config_data_service:
            #if (defined CONNECTOR_DATA_SERVICE)
            {
                *((bool *)response_data) = true;
            }
            #else
            {
                *((bool *)response_data) = false;
            }
            #endif
            break;
    }
    return connector_callback_continue;
}