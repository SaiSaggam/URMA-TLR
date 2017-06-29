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
*****************************************************************************/

#ifndef __DSL_MODEM_H__
#define __DSL_MODEM_H__

#include "config_mgr.h"
#include <pthread.h>
#include <stdbool.h>

typedef struct
{
	pthread_mutex_t lock;	// to serialise access to this struct
	bool is_ppp_up;
	config_dsl_mode_t mode;
	bool changed_mode;
	uint32_t vpi;
	uint32_t vci;
	config_dsl_encapsulation_t encapsulation;
	bool changed_vpi_vci_encapsulation;
} dsl_modem_config_t;

extern dsl_modem_config_t * dsl_modem_config;

#define MAX_FIELD_LENGTH	100	// longest expected parameter value to output

typedef struct
{
	unsigned long speed;
	config_state_dsl_downstream_channel_type_t channel_type;
	char relative_capacity[MAX_FIELD_LENGTH];
	char noise_margin[MAX_FIELD_LENGTH];
	char output_power[MAX_FIELD_LENGTH];
	char attenuation[MAX_FIELD_LENGTH];
	unsigned fec_error;
	unsigned crc_error;
	unsigned hec_error;
} dsl_modem_dir_t;

typedef struct
{
	pthread_mutex_t lock;	// to serialise access to this struct
	char fw_version[MAX_FIELD_LENGTH];
	char hw_version[MAX_FIELD_LENGTH];
	char ras_fw_version[MAX_FIELD_LENGTH];
	char system_fw_id[MAX_FIELD_LENGTH];
    config_state_dsl_line_mode_t line_mode;
	char remote_vendor_id[MAX_FIELD_LENGTH];
    config_state_dsl_line_status_t line_status;
	dsl_modem_dir_t downstream;
	dsl_modem_dir_t upstream;
	unsigned uptime_secs;
	unsigned errored_secs_15min;
	unsigned errored_secs_24hr;
	unsigned errored_secs_powerup;
} dsl_modem_status_t;

extern dsl_modem_status_t * dsl_modem_status;

void * dsl_modem_thread(void * data);

#endif /* __DSL_MODEM_H__ */

