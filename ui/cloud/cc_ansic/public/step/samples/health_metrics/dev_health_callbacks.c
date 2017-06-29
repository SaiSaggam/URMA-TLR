#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include "health_metrics_api.h"

/* #define DEBUG */

#if !(defined UNUSED_ARGUMENT)
#define UNUSED_ARGUMENT(a)  (void)(a)
#endif

#if (defined DEBUG)
#define PRINT_FUNCTION_NAME()   do{printf("Function: %s\n", __FUNCTION__);}while(0)
#define PRINT_INDEXES(i)         do{printf("Mobile %d Net %d\n", (i)->upper, (i)->lower);}while(0)
#define PRINT_TECH(t)            do{printf("Mobile tech %dG\n", (int)(t) + 2);}while(0)
#else
#define PRINT_FUNCTION_NAME()
#define PRINT_INDEXES(i)         UNUSED_ARGUMENT((i))
#define PRINT_TECH(t)            UNUSED_ARGUMENT((t))
#endif

void hm_print_line(char const * const format, ...)
{
    va_list args;

    printf("HealthMetrics: ");

    va_start(args, (format));
    vprintf(format, args);
    va_end(args);

    printf("\n");

    fflush(stdout);
}

int hm_realloc_data(size_t const old_length, size_t const new_length, void ** ptr)
{
    void * const old_ptr = *ptr;

    UNUSED_ARGUMENT(old_length);
    *ptr = realloc(old_ptr, new_length);
    if (*ptr == NULL)
    {
        free(old_ptr);
    }
    return 0;
}

int hm_malloc_data(size_t const length, void ** ptr)
{
    *ptr = malloc(length);
    return 0;
}

int hm_free_data(void * const ptr)
{
    free(ptr);
    return 0;
}

int hm_get_system_time(unsigned long * const uptime)
{
    *uptime = time(NULL);
    return 0;
}

char * cc_dev_health_malloc_string(size_t size)
{
    return malloc(size);
}

void cc_dev_health_free_string(char const * const string)
{
    free((void *)string);
}

uint32_t cc_dev_health_get_posix_time(void)
{
    time_t const posix_time = time(NULL);

    return posix_time;
}

unsigned int cc_dev_health_get_mobile_instances(unsigned int const upper_index)
{
    PRINT_FUNCTION_NAME();
    UNUSED_ARGUMENT(upper_index);

    return 2;
}

unsigned int cc_dev_health_get_mobile_net_instances(unsigned int const upper_index)
{
    PRINT_FUNCTION_NAME();
    UNUSED_ARGUMENT(upper_index);

    return 2;
}

#define MAX_STRING  64

void cc_dev_health_get_mobile_module_modem_id(connector_indexes_t const * const indexes, char * * const value)
{
    *value = cc_dev_health_malloc_string(MAX_STRING);
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    strcpy(*value, "My \"Modem\" \\ID\\");
}

void cc_dev_health_get_mobile_module_imei(connector_indexes_t const * const indexes, char * * const value)
{
    *value = cc_dev_health_malloc_string(MAX_STRING);
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    strcpy(*value, "123456-789-0");
}

connector_bool_t cc_dev_health_get_mobile_net_cell_id(connector_indexes_t const * const indexes, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);

    *value = 32;
    return connector_true;
}

connector_bool_t cc_dev_health_get_mobile_net_network(connector_indexes_t const * const indexes, char * * const value)
{
    *value = cc_dev_health_malloc_string(MAX_STRING);
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    strcpy(*value, "AT&T");
    return connector_true;
}

void cc_dev_health_get_mobile_module_sims(connector_indexes_t const * const indexes, unsigned int * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 2;
}

connector_bool_t cc_dev_health_get_mobile_active_sim(connector_indexes_t const * const indexes, unsigned int * const value)
{
    UNUSED_ARGUMENT(indexes);
    PRINT_FUNCTION_NAME();
    *value = 1;
    return connector_true;
}

connector_bool_t cc_dev_health_mobile_module_present(connector_indexes_t const * const indexes)
{
    UNUSED_ARGUMENT(indexes);
    PRINT_FUNCTION_NAME();
    return connector_true;
}


connector_bool_t cc_dev_health_mobile_module_net_status_present(connector_indexes_t const * const indexes)
{
    UNUSED_ARGUMENT(indexes);
    PRINT_FUNCTION_NAME();
    return connector_true;
}

connector_bool_t cc_dev_health_get_mobile_net_info_present(connector_indexes_t const * const indexes)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);
    return connector_true;
}

connector_bool_t cc_dev_health_get_mobile_net_rssi(connector_indexes_t const * const indexes, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);

    *value = 32;
    return connector_true;
}


connector_bool_t cc_dev_health_get_mobile_net_ecio(connector_indexes_t const * const indexes, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);

    *value = 32;
    return connector_true;
}


connector_bool_t cc_dev_health_get_mobile_net_rsrp(connector_indexes_t const * const indexes, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);

    *value = 32;
    return connector_true;
}


connector_bool_t cc_dev_health_get_mobile_net_rsrq(connector_indexes_t const * const indexes, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);

    *value = 32;
    return connector_true;
}


connector_bool_t cc_dev_health_get_mobile_net_sinr(connector_indexes_t const * const indexes, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);

    *value = 32;
    return connector_true;
}


connector_bool_t cc_dev_health_get_mobile_net_snr(connector_indexes_t const * const indexes, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);

    *value = 32;
    return connector_true;
}


void cc_dev_health_get_mobile_net_status_registration(connector_indexes_t const * const indexes, char * * const value)
{
    *value = cc_dev_health_malloc_string(MAX_STRING);
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    strcpy(*value, "<Registration>");
}

void cc_dev_health_get_mobile_net_status_attachment(connector_indexes_t const * const indexes, char * * const value)
{
    *value = cc_dev_health_malloc_string(MAX_STRING);
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    strcpy(*value, "<Attachment>");
}


connector_bool_t cc_dev_health_get_mobile_net_lac(connector_indexes_t const * const indexes, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);

    *value = 32;
    return connector_true;
}


connector_bool_t cc_dev_health_get_mobile_net_tac(connector_indexes_t const * const indexes, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);

    *value = 32;
    return connector_true;
}


connector_bool_t cc_dev_health_get_mobile_net_session(connector_indexes_t const * const indexes, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);

    *value = 32;
    return connector_true;
}


connector_bool_t cc_dev_health_get_mobile_net_rxbytes(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);
    PRINT_TECH(tech);

    *value = 32;
    return connector_true;
}

connector_bool_t cc_dev_health_get_mobile_net_txbytes(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);
    PRINT_TECH(tech);

    *value = 32;
    return connector_true;
}

connector_bool_t cc_dev_health_get_mobile_net_latency_min(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);
    PRINT_TECH(tech);

    *value = 32;
    return connector_true;
}

connector_bool_t cc_dev_health_get_mobile_net_latency_avg(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);
    PRINT_TECH(tech);

    *value = 32;
    return connector_true;
}

connector_bool_t cc_dev_health_get_mobile_net_latency_max(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);
    PRINT_TECH(tech);

    *value = 32;
    return connector_true;
}

connector_bool_t cc_dev_health_get_mobile_net_transactions_count(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);
    PRINT_TECH(tech);

    *value = 32;
    return connector_true;
}

connector_bool_t cc_dev_health_get_mobile_net_fdrop_count(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);
    PRINT_TECH(tech);

    *value = 32;
    return connector_true;
}

connector_bool_t cc_dev_health_get_mobile_net_losspercent(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);
    PRINT_TECH(tech);

    *value = 32;
    return connector_true;
}

connector_bool_t cc_dev_health_get_mobile_net_drop_count(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);
    PRINT_TECH(tech);

    *value = 32;
    return connector_true;
}

connector_bool_t cc_dev_health_get_mobile_net_oos_count(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);
    PRINT_TECH(tech);

    *value = 32;
    return connector_true;
}

connector_bool_t cc_dev_health_get_mobile_net_uptime(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);
    PRINT_TECH(tech);

    *value = 32;
    return connector_true;
}

connector_bool_t cc_dev_health_get_mobile_temperature(connector_indexes_t const * const indexes, float * const value)
{
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);

    *value = 25.3;
    return connector_true;
}

void cc_dev_health_get_mobile_net_info_iccid(connector_indexes_t const * const indexes, char * * const value)
{
    *value = cc_dev_health_malloc_string(MAX_STRING);
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);

    strcpy(*value, "My ICCID");
}

void cc_dev_health_get_mobile_net_info_imsi(connector_indexes_t const * const indexes, char * * const value)
{
    *value = cc_dev_health_malloc_string(MAX_STRING);
    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);

    strcpy(*value, "My IMSI");
}

void cc_dev_health_get_mobile_net_info_phone_num(connector_indexes_t const * const indexes, char * * const value)
{
    *value = cc_dev_health_malloc_string(MAX_STRING);

    PRINT_FUNCTION_NAME();
    PRINT_INDEXES(indexes);

    strcpy(*value, "My Phone Num");
}

unsigned int cc_dev_health_get_eth_instances(unsigned int const upper_index)
{
    PRINT_FUNCTION_NAME();
    UNUSED_ARGUMENT(upper_index);

    return 4;
}

connector_bool_t cc_dev_health_get_eth_tx_bytes(connector_indexes_t const * const indexes, uint64_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 64;
    return connector_true;
}


connector_bool_t cc_dev_health_get_eth_tx_packets(connector_indexes_t const * const indexes, uint64_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 64;
    return connector_true;
}

connector_bool_t cc_dev_health_get_eth_tx_dropped(connector_indexes_t const * const indexes, uint64_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 64;
    return connector_true;
}

connector_bool_t cc_dev_health_get_eth_tx_overruns(connector_indexes_t const * const indexes, uint64_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 64;
    return connector_true;
}

connector_bool_t cc_dev_health_get_eth_rx_bytes(connector_indexes_t const * const indexes, uint64_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 64;
    return connector_true;
}

connector_bool_t cc_dev_health_get_eth_rx_packets(connector_indexes_t const * const indexes, uint64_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 64;
    return connector_true;
}

connector_bool_t cc_dev_health_get_eth_rx_dropped(connector_indexes_t const * const indexes, uint64_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 64;
    return connector_true;
}

connector_bool_t cc_dev_health_get_eth_link_down_count(connector_indexes_t const * const indexes, uint64_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 64;
    return connector_true;
}

connector_bool_t cc_dev_health_get_eth_link_down_duration(connector_indexes_t const * const indexes, uint64_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 64;
    return connector_true;
}


unsigned int cc_dev_health_get_wifi_instances(unsigned int const upper_index)
{
    PRINT_FUNCTION_NAME();
    UNUSED_ARGUMENT(upper_index);

    return 1;
}

connector_bool_t cc_dev_health_get_wifi_radio_present(connector_indexes_t const * const indexes)
{
    UNUSED_ARGUMENT(indexes);
    return connector_true;
}

void cc_dev_health_get_wifi_radio_mode(connector_indexes_t const * const indexes, char * * const value)
{
    *value = cc_dev_health_malloc_string(MAX_STRING);
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    strcpy(*value, "client");
}

void cc_dev_health_get_wifi_radio_ssid(connector_indexes_t const * const indexes, char * * const value)
{
    *value = cc_dev_health_malloc_string(MAX_STRING);

    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    strcpy(*value, "SSID");
}

void cc_dev_health_get_wifi_radio_channel(connector_indexes_t const * const indexes, unsigned int * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 3;
}

void cc_dev_health_get_wifi_radio_protocol(connector_indexes_t const * const indexes, char * * const value)
{
    *value = cc_dev_health_malloc_string(MAX_STRING);

    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    strcpy(*value, "Wifi Protocol");
}


connector_bool_t cc_dev_health_get_wifi_status(connector_indexes_t const * const indexes, char * * const value)
{
    *value = cc_dev_health_malloc_string(MAX_STRING);
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    strcpy(*value, "wifi status");
    return connector_true;
}


connector_bool_t cc_dev_health_get_wifi_rssi(connector_indexes_t const * const indexes, int32_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 32;
    return connector_true;
}


connector_bool_t cc_dev_health_get_wifi_rate(connector_indexes_t const * const indexes, float * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 1.159;
    return connector_true;
}


connector_bool_t cc_dev_health_get_wifi_clients(connector_indexes_t const * const indexes, int32_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 32;
    return connector_true;
}



connector_bool_t cc_dev_health_get_system_mem_available(connector_indexes_t const * const indexes, uint64_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 64;
    return connector_true;
}


connector_bool_t cc_dev_health_get_system_mem_free(connector_indexes_t const * const indexes, uint64_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 64;
    return connector_true;
}


connector_bool_t cc_dev_health_get_system_reboots(connector_indexes_t const * const indexes, int32_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 32;
    return connector_true;
}


connector_bool_t cc_dev_health_get_system_msg_free(connector_indexes_t const * const indexes, int32_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 32;
    return connector_true;
}


connector_bool_t cc_dev_health_get_system_msg_min(connector_indexes_t const * const indexes, int32_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 32;
    return connector_true;
}


connector_bool_t cc_dev_health_get_system_buf_free(connector_indexes_t const * const indexes, int32_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 32;
    return connector_true;
}


connector_bool_t cc_dev_health_get_system_buf_min(connector_indexes_t const * const indexes, int32_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 32;
    return connector_true;
}


connector_bool_t cc_dev_health_get_system_bigbuf_free(connector_indexes_t const * const indexes, int32_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 32;
    return connector_true;
}


connector_bool_t cc_dev_health_get_system_bigbuf_min(connector_indexes_t const * const indexes, int32_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 32;
    return connector_true;
}

connector_bool_t cc_dev_health_get_system_cpu_used(connector_indexes_t const * const indexes, int32_t * const value)
{
    UNUSED_ARGUMENT(indexes);

    PRINT_FUNCTION_NAME();
    *value = 32;
    return connector_true;
}
