#ifndef __VPN_CONFIG_H__
#define __VPN_CONFIG_H__

#include "debug.h"
#include "config_mgr.h"
#include "config_def.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct config_st;
typedef struct config_st config_st;

int config_init(config_st * * const config, size_t const instance, config_group_t config_group);
int config_update(config_st * * const config, size_t const instance, bool * const config_has_changed);


#endif /* __VPN_CONFIG_H__ */
