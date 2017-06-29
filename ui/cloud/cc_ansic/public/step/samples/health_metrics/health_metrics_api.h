#ifndef _HEALTH_METRICS_API_H_
#define _HEALTH_METRICS_API_H_

#include "connector_api.h"

#if !(defined connector_bool)
#define connector_bool(cond)    ((cond) ? connector_true : connector_false)
#endif

#if !(defined ASSERT_GOTO)
#define ASSERT_GOTO(condition, label)    do{if (!(condition)) {goto label;}}while(0)
#endif

#if !(defined UNUSED_VARIABLE)
#define UNUSED_VARIABLE(a)  (void)(a)
#endif

#if !(defined ASSERT)
#define ASSERT  assert
#endif

#define MIN_VALUE(x,y)        (((x) < (y))? (x): (y))
#define MAX_VALUE(x,y)        (((x) > (y))? (x): (y))

#define DEV_HEALTH_MAX_STREAM_ID_LEN               64

typedef enum {
    dev_health_root_eth,
    dev_health_root_mobile,
    dev_health_root_sys,
    dev_health_root_COUNT
} dev_health_root_t;

typedef struct {
    connector_bool_t metrics;
    unsigned long sample_rate;
} dev_health_simple_metric_t;

typedef struct {
    dev_health_simple_metric_t eth;
    dev_health_simple_metric_t mobile;
    dev_health_simple_metric_t sys;
    unsigned long report_rate;
} health_metrics_config_t;

typedef struct {
    unsigned int upper;
    unsigned int lower;
} connector_indexes_t;

typedef enum {
  NETWORK_TECH_2G,
  NETWORK_TECH_3G,
  NETWORK_TECH_4G
} mobile_network_tech_t;

typedef struct {
        struct dev_health_info {
            struct {
                char string[DEV_HEALTH_MAX_STREAM_ID_LEN];
                unsigned int len;
            } stream_id;

            struct {
                char * data;
                char const * next_header;
                unsigned int total_size;
                unsigned int free_bytes;
                unsigned int data_points_count;
                enum {
                    DEV_HEALTH_CSV_STATUS_PROCESSING,
                    DEV_HEALTH_CSV_STATUS_READY_TO_SEND,
                    DEV_HEALTH_CSV_STATUS_SENDING,
                    DEV_HEALTH_CSV_STATUS_SENT
                } status;
            } csv;
        } info;

        struct {
            unsigned long report_at;
            unsigned long sample_at[dev_health_root_COUNT];
        } simple_metrics;

        unsigned int last_check;
} health_metrics_data_t;

typedef struct {
    char const * p_csv;
    size_t bytes_available;
    connector_request_data_service_send_t send_request;
    health_metrics_data_t * health_metrics_data;
} dev_health_data_push_t;

size_t dp_process_string(char const * const string, char * const buffer, size_t const bytes_available, size_t * bytes_used_ptr, connector_bool_t need_quotes, connector_bool_t first_chunk);
connector_bool_t string_needs_quotes(char const * const string);

connector_status_t health_metrics_report_step(health_metrics_config_t const * const config, health_metrics_data_t * const health_metrics_data, connector_handle_t connector_handle);

void hm_print_line(char const * const format, ...);
int hm_realloc_data(size_t const old_length, size_t const new_length, void ** ptr);
int hm_malloc_data(size_t const length, void ** ptr);
int hm_free_data(void * const ptr);
int hm_get_system_time(unsigned long * const uptime);

connector_callback_status_t dev_health_handle_data_callback(connector_data_service_send_data_t * const data_ptr);
connector_callback_status_t dev_health_handle_response_callback(connector_data_service_send_response_t * const data_ptr);
connector_callback_status_t dev_health_handle_status_callback(connector_data_service_status_t * const data_ptr);
#if (defined CONNECTOR_SHORT_MESSAGE)
connector_callback_status_t dev_health_handle_length_callback(connector_data_service_length_t * const data_ptr);
#endif

char * cc_dev_health_malloc_string(size_t size);
void cc_dev_health_free_string(char const * const string);

uint32_t cc_dev_health_get_posix_time(void);

unsigned int cc_dev_health_get_mobile_instances(unsigned int const upper_index);
unsigned int cc_dev_health_get_mobile_net_instances(unsigned int const upper_index);

void cc_dev_health_get_mobile_module_imei(connector_indexes_t const * const indexes, char * * const value);
void cc_dev_health_get_mobile_module_modem_id(connector_indexes_t const * const indexes, char * * const value);
void cc_dev_health_get_mobile_module_sims(connector_indexes_t const * const indexes, unsigned int * const value);

void cc_dev_health_get_mobile_net_info_iccid(connector_indexes_t const * const indexes, char * * const value);
void cc_dev_health_get_mobile_net_info_imsi(connector_indexes_t const * const indexes, char * * const value);
void cc_dev_health_get_mobile_net_info_phone_num(connector_indexes_t const * const indexes, char * * const value);

void cc_dev_health_get_mobile_net_status_registration(connector_indexes_t const * const indexes, char * * const value);
void cc_dev_health_get_mobile_net_status_attachment(connector_indexes_t const * const indexes, char * * const value);

connector_bool_t cc_dev_health_mobile_module_present(connector_indexes_t const * const indexes);
connector_bool_t cc_dev_health_mobile_module_net_status_present(connector_indexes_t const * const indexes);
connector_bool_t cc_dev_health_get_mobile_net_info_present(connector_indexes_t const * const indexes);

connector_bool_t cc_dev_health_get_mobile_temperature(connector_indexes_t const * const indexes, float * const value);
connector_bool_t cc_dev_health_get_mobile_active_sim(connector_indexes_t const * const indexes, unsigned int * const value);
connector_bool_t cc_dev_health_get_mobile_net_cell_id(connector_indexes_t const * const indexes, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_network(connector_indexes_t const * const indexes, char * * const value);
connector_bool_t cc_dev_health_get_mobile_net_rssi(connector_indexes_t const * const indexes, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_ecio(connector_indexes_t const * const indexes, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_rsrp(connector_indexes_t const * const indexes, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_rsrq(connector_indexes_t const * const indexes, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_sinr(connector_indexes_t const * const indexes, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_snr(connector_indexes_t const * const indexes, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_lac(connector_indexes_t const * const indexes, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_tac(connector_indexes_t const * const indexes, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_session(connector_indexes_t const * const indexes, int32_t * const value);

connector_bool_t cc_dev_health_get_mobile_net_rxbytes(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_txbytes(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_latency_min(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_latency_avg(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_latency_max(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_transactions_count(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_fdrop_count(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_losspercent(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_drop_count(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_oos_count(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value);
connector_bool_t cc_dev_health_get_mobile_net_uptime(connector_indexes_t const * const indexes, mobile_network_tech_t tech, int32_t * const value);

unsigned int cc_dev_health_get_eth_instances(unsigned int const upper_index);

connector_bool_t cc_dev_health_get_eth_tx_bytes(connector_indexes_t const * const indexes, uint64_t * const value);
connector_bool_t cc_dev_health_get_eth_tx_packets(connector_indexes_t const * const indexes, uint64_t * const value);
connector_bool_t cc_dev_health_get_eth_tx_dropped(connector_indexes_t const * const indexes, uint64_t * const value);
connector_bool_t cc_dev_health_get_eth_tx_overruns(connector_indexes_t const * const indexes, uint64_t * const value);
connector_bool_t cc_dev_health_get_eth_rx_bytes(connector_indexes_t const * const indexes, uint64_t * const value);
connector_bool_t cc_dev_health_get_eth_rx_packets(connector_indexes_t const * const indexes, uint64_t * const value);
connector_bool_t cc_dev_health_get_eth_rx_dropped(connector_indexes_t const * const indexes, uint64_t * const value);

connector_bool_t cc_dev_health_get_eth_link_down_count(connector_indexes_t const * const indexes, uint64_t * const value);
connector_bool_t cc_dev_health_get_eth_link_down_duration(connector_indexes_t const * const indexes, uint64_t * const value);

unsigned int cc_dev_health_get_wifi_instances(unsigned int const upper_index);

void cc_dev_health_get_wifi_radio_mode(connector_indexes_t const * const indexes, char * * const value);
void cc_dev_health_get_wifi_radio_ssid(connector_indexes_t const * const indexes, char * * const value);
void cc_dev_health_get_wifi_radio_channel(connector_indexes_t const * const indexes, unsigned int * const value);
void cc_dev_health_get_wifi_radio_protocol(connector_indexes_t const * const indexes, char * * const value);
connector_bool_t cc_dev_health_get_wifi_radio_present(connector_indexes_t const * const indexes);

connector_bool_t cc_dev_health_get_wifi_status(connector_indexes_t const * const indexes, char * * const value);
connector_bool_t cc_dev_health_get_wifi_rssi(connector_indexes_t const * const indexes, int32_t * const value);
connector_bool_t cc_dev_health_get_wifi_rate(connector_indexes_t const * const indexes, float * const value);
connector_bool_t cc_dev_health_get_wifi_clients(connector_indexes_t const * const indexes, int32_t * const value);

connector_bool_t cc_dev_health_get_system_mem_available(connector_indexes_t const * const indexes, uint64_t * const value);
connector_bool_t cc_dev_health_get_system_mem_free(connector_indexes_t const * const indexes, uint64_t * const value);
connector_bool_t cc_dev_health_get_system_reboots(connector_indexes_t const * const indexes, int32_t * const value);
connector_bool_t cc_dev_health_get_system_msg_free(connector_indexes_t const * const indexes, int32_t * const value);
connector_bool_t cc_dev_health_get_system_msg_min(connector_indexes_t const * const indexes, int32_t * const value);
connector_bool_t cc_dev_health_get_system_buf_free(connector_indexes_t const * const indexes, int32_t * const value);
connector_bool_t cc_dev_health_get_system_buf_min(connector_indexes_t const * const indexes, int32_t * const value);
connector_bool_t cc_dev_health_get_system_bigbuf_free(connector_indexes_t const * const indexes, int32_t * const value);
connector_bool_t cc_dev_health_get_system_bigbuf_min(connector_indexes_t const * const indexes, int32_t * const value);
connector_bool_t cc_dev_health_get_system_cpu_used(connector_indexes_t const * const indexes, int32_t * const value);

#endif
