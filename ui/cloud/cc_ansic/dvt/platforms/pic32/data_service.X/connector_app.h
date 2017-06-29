// Include all headers for any enabled TCPIP Stack functions
#define DEBUG_PRINTF(...)   DBPRINTF(__VA_ARGS__)

#include "TCPIP Stack/TCPIP.h"
#include "TCPIP Stack/Tick.h"

#include "connector_api.h"
#include "connector_config.h"

#define DEVICE_ID_LENGTH    16
#define VENDOR_ID_LENGTH    4
#define MAC_ADDR_LENGTH     6

typedef int bool;
#define true 1
#define false 0

#define RED_LED             BIT_0
#define YELLOW_LED          BIT_1
#define GREEN_LED           BIT_2

#define SW1                 BIT_6
#define SW2                 BIT_7
#define SW3                 BIT_13

typedef struct __attribute__((__packed__))
{
    uint32_t                start_time;
    uint8_t                 vendor_id[VENDOR_ID_LENGTH];
    uint16_t                tx_keepalive;
    uint16_t                rx_keepalive;
    uint16_t                wait_count;
    uint8_t                 phone_number;
    uint32_t                link_speed;
    char *                  device_type;
    char *                  server_url;
    connector_connection_type_t connection_type;
    connector_network_handle_t  socket_fd;
    int                     connected;
} CONNECTOR_CONFIG;

extern CONNECTOR_CONFIG connector_config;

void sleep(unsigned int);

connector_callback_status_t connector_config_callback(
        connector_config_request_t const request_id,
        const void * const request_data, void * response_data,
        size_t * const response_length);

connector_callback_status_t connector_os_callback(connector_os_request_t const request_id, 
        const void * const request_data, void * response_data);

connector_callback_status_t connector_network_callback(connector_network_request_t request,
        const void * const request_data, void * response_data,
        size_t * response_length);

connector_callback_status_t connector_data_service_callback(
        connector_data_service_request_t request, const void * const request_data,
        void * response_data, size_t * response_length);