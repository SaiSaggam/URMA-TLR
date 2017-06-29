#ifndef __FAILOVER_CONFIG_H__
#define __FAILOVER_CONFIG_H__

#include "failover.h"
#include <stddef.h>

typedef enum {
    failover_config_status_unchanged,
    failover_config_status_changed,
    failover_config_status_error
} failover_config_status_t;

#define CONFIG_STATUS_MASK(x)   (0x01 << (x))

#define IS_CONFIG_STATUS_ERROR(x) ((x) & CONFIG_STATUS_MASK(failover_config_status_error))
#define IS_CONFIG_STATUS_CHANGED(x) ((x) & CONFIG_STATUS_MASK(failover_config_status_changed))
#define IS_CONFIG_STATUS_NO_CHANGED(x) ((x) & CONFIG_STATUS_MASK(failover_config_status_unchanged))

failover_config_status_t failover_get_config(size_t const instance, failover_config_t * const info);

#endif /* __FAILOVER_CONFIG_H__ */
