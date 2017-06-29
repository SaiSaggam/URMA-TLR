#ifndef __CELLULAR_WORK_H__
#define __CELLULAR_WORK_H__

#include "cellular_status.h"

#define CELLULAR_FIRMWARE_CSID_LENGTH 128
#define CELLULAR_FIRMWARE_CSID_IMEI 32
#define CELLULAR_DESCRIPTION_LENGTH 64
#define CELLULAR_NETWORK_PROVIDER_LENGTH 64
#define CELLULAR_HARDWARE_CSID_LENGTH 64

typedef struct
{
    size_t index_3g;
    size_t active_sim;
    char module[CELLULAR_MANUFACTURER_LENGTH + CELLULAR_MODEL_LENGTH + 1];
    char firmware[CELLULAR_FIRMWARE_CSID_LENGTH];
    char hardware[CELLULAR_HARDWARE_CSID_LENGTH];
    char imei[CELLULAR_FIRMWARE_CSID_IMEI];
    char description[CELLULAR_DESCRIPTION_LENGTH];
    char network_provider[CELLULAR_NETWORK_PROVIDER_LENGTH];
    char radio_band[CELLULAR_FIRMWARE_CSID_LENGTH];
    char pdp_context[CELLULAR_NETWORK_PROVIDER_LENGTH];
    int32_t signal_strength;
    int32_t signal_quality;
    int32_t temperature;
    cellular_registration_status_t registration_status;
    cellular_connection_type_t connection_type;
    size_t channel;

    char ipaddr[sizeof "255.255.255.255"];
    char netmask[sizeof "255.255.255.255"];
    char gateway[sizeof "255.255.255.255"];
    char dns_servers[sizeof "255.255.255.255, 255.255.255.255"];
    uint64_t  rx_packets;
    uint64_t  tx_packets;
    uint64_t  rx_bytes;
    uint64_t  tx_bytes;
} cellular_status_t;

char * cellular_get_signal_strength_status_string(cellular_status_t const * const pstatus);
char * cellular_get_signal_quality_status_string(cellular_status_t const * const pstatus);
char const * cellular_get_registration_status_string(cellular_status_t const * const pstatus);
char const * cellular_get_connection_type_string(cellular_status_t const * const pstatus);
char const * cellular_get_sim_status_string(int const index);
int cellular_get_sim_status_strings_count(void);

#endif
