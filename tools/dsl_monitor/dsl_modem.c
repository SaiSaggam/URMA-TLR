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

#include "dsl_modem.h"
#include "dsl_event.h"
#include "dsl_log.h"
#include "dsl_monitor.h"
#include "modem_to_config_state_dsl.h"
#include "config_dsl_to_modem.h"
#include "get_field.h"
#include "strip_whitespace.h"
#include "config_mgr.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

dsl_modem_config_t * dsl_modem_config;
dsl_modem_status_t * dsl_modem_status;

typedef void (*parse_func_t)(char * const response);

static void null_parse_func(char * const response)
{
	// Do nothing.
}

static void parse_sys_version(char * const response)
{
  	char * field;

	pthread_mutex_lock(&dsl_modem_status->lock);

	if (strstr(response, "DSL FW Version") == response)
	{
		char version_format[100];
		snprintf(version_format, sizeof version_format, "DSL FW Version : FwVer:%%%zds HwVer:%%%zds", sizeof dsl_modem_status->fw_version - 1, sizeof dsl_modem_status->hw_version - 1);
		sscanf(response, version_format, dsl_modem_status->fw_version, dsl_modem_status->hw_version);
	}
	else if (strstr(response, "RAS F/W Version") == response)
	{
		if ((field = strchr(response, ':')) != NULL)
		{
            field = strip_whitespace(field+1);
            strlcpy(dsl_modem_status->ras_fw_version, field, sizeof dsl_modem_status->ras_fw_version);
		}
	}
	else if (strstr(response, "System  F/W  ID") == response)
	{
		if ((field = strchr(response, ':')) != NULL)
		{
            field = strip_whitespace(field+1);
            strlcpy(dsl_modem_status->system_fw_id, field, sizeof dsl_modem_status->system_fw_id);
		}
	}
	else if (strstr(response, "Standard") == response)
	{
		if ((field = get_field(response, ":", 2)) != NULL)
		{
			dsl_modem_status->line_mode = modem_to_config_state_dsl_line_mode(field);
		}
	}

	pthread_mutex_unlock(&dsl_modem_status->lock);
}

static void parse_wan_adsl_status(char * const response)
{
	pthread_mutex_lock(&dsl_modem_status->lock);

	if (strstr(response, "current modem status") == response)
	{
		char * field;

		if ((field = get_field(response, ":", 2)) != NULL)
		{
			dsl_modem_status->line_status = modem_to_config_state_dsl_line_status(field);
		}
	}

	pthread_mutex_unlock(&dsl_modem_status->lock);
}

static void parse_wan_adsl_chandata(char * const response)
{
	pthread_mutex_lock(&dsl_modem_status->lock);

	if (strstr(response, "adsl modem not up") == response)
	{
		dsl_modem_status->downstream.speed = 0;
		dsl_modem_status->upstream.speed = 0;
		dsl_modem_status->downstream.channel_type = config_state_dsl_downstream_channel_type_unknown;
		dsl_modem_status->upstream.channel_type = config_state_dsl_upstream_channel_type_unknown;
	}
	else
	{
		char * field;

		if ((field = get_field(response, ":", 2)) != NULL)
		{
			unsigned long speed;

			speed = strtoul(field, NULL, 0);

			if (speed > 0)
			{
				if (strstr(response, "near-end") == response)		// near-end is downstream
				{
					dsl_modem_status->downstream.speed = speed;
					if ((field = get_field(response, " ", 2)) != NULL)
					{
						dsl_modem_status->downstream.channel_type = modem_to_config_state_dsl_channel_type(field);
					}
				}
				else if (strstr(response, "far-end") == response)	// far-end is upstream
				{
					dsl_modem_status->upstream.speed = speed;
					if ((field = get_field(response, " ", 2)) != NULL)
					{
						dsl_modem_status->upstream.channel_type = modem_to_config_state_dsl_channel_type(field);
					}
				}
			}
		}
	}

	pthread_mutex_unlock(&dsl_modem_status->lock);
}

static void parse_wan_adsl_linedata(char * const response, dsl_modem_dir_t * const pdir)
{
	if (strstr(response, "adsl modem not up") == response)
	{
		pdir->relative_capacity[0] = 0;
		pdir->noise_margin[0] = 0;
		pdir->output_power[0] = 0;
		pdir->attenuation[0] = 0;
	}
	else
	{
		char * field;

		if (strstr(response, "relative capacity occupation") == response)
		{
			if ((field = get_field(response, " %", 4)) != NULL)
				strlcpy(pdir->relative_capacity, field, sizeof pdir->relative_capacity);
		}
		else if (strstr(response, "noise margin") == response)
		{
			if ((field = get_field(response, " ", 4)) != NULL)
				strlcpy(pdir->noise_margin, field, sizeof pdir->noise_margin);
		}
		else if (strstr(response, "output power") == response)
		{
			if ((field = get_field(response, " ", 4)) != NULL)
				strlcpy(pdir->output_power, field, sizeof pdir->output_power);
		}
		else if (strstr(response, "attenuation") == response)
		{
			if ((field = get_field(response, " ", 3)) != NULL)
				strlcpy(pdir->attenuation, field, sizeof pdir->attenuation);
		}
	}
}

static void parse_wan_adsl_linedata_near(char * const response)
{
	pthread_mutex_lock(&dsl_modem_status->lock);
	parse_wan_adsl_linedata(response, &dsl_modem_status->downstream);
	pthread_mutex_unlock(&dsl_modem_status->lock);
}

static void parse_wan_adsl_linedata_far(char * const response)
{
	pthread_mutex_lock(&dsl_modem_status->lock);
	parse_wan_adsl_linedata(response, &dsl_modem_status->upstream);
	pthread_mutex_unlock(&dsl_modem_status->lock);
}

static void parse_wan_adsl_perfdata(char * const response)
{
	unsigned hrs, mins, secs, errors;

	pthread_mutex_lock(&dsl_modem_status->lock);

	if (strstr(response, "adsl modem not up") == response)
	{
		// Do nothing.
	}
	else if (sscanf(response, "ADSL uptime %u:%u:%u", &hrs, &mins, &secs) == 3)
	{
		dsl_modem_status->uptime_secs = hrs*3600 + mins*60 + secs;
	}
	else if (sscanf(response, "Error second in 15min : %u", &dsl_modem_status->errored_secs_15min) == 1)
	{
		// Do nothing.
	}
	else if (sscanf(response, "Error second in 24hr : %u", &dsl_modem_status->errored_secs_24hr) == 1)
	{
		// Do nothing.
	}
	else if (sscanf(response, "Error second after power-up : %u", &dsl_modem_status->errored_secs_powerup) == 1)
	{
		// Do nothing.
	}
	else if (sscanf(response, "near-end FEC error fast: %u", &errors) == 1)
	{
		dsl_modem_status->downstream.fec_error += errors;
	}
	else if (sscanf(response, "near-end FEC error interleaved: %u", &errors) == 1)
	{
		dsl_modem_status->downstream.fec_error += errors;
	}
	else if (sscanf(response, "near-end CRC error fast: %u", &errors) == 1)
	{
		dsl_modem_status->downstream.crc_error += errors;
	}
	else if (sscanf(response, "near-end CRC error interleaved: %u", &errors) == 1)
	{
		dsl_modem_status->downstream.crc_error += errors;
	}
	else if (sscanf(response, "near-end HEC error fast: %u", &errors) == 1)
	{
		dsl_modem_status->downstream.hec_error += errors;
	}
	else if (sscanf(response, "near-end HEC error interleaved: %u", &errors) == 1)
	{
		dsl_modem_status->downstream.hec_error += errors;
	}
	else if (sscanf(response, "far-end FEC error fast: %u", &errors) == 1)
	{
		dsl_modem_status->upstream.fec_error += errors;
	}
	else if (sscanf(response, "far-end FEC error interleaved: %u", &errors) == 1)
	{
		dsl_modem_status->upstream.fec_error += errors;
	}
	else if (sscanf(response, "far-end CRC error fast: %u", &errors) == 1)
	{
		dsl_modem_status->upstream.crc_error += errors;
	}
	else if (sscanf(response, "far-end CRC error interleaved: %u", &errors) == 1)
	{
		dsl_modem_status->upstream.crc_error += errors;
	}
	else if (sscanf(response, "far-end HEC error fast: %u", &errors) == 1)
	{
		dsl_modem_status->upstream.hec_error += errors;
	}
	else if (sscanf(response, "far-end HEC error interleaved: %u", &errors) == 1)
	{
		dsl_modem_status->upstream.hec_error += errors;
	}

	pthread_mutex_unlock(&dsl_modem_status->lock);
}

static void parse_wan_adsl_farituid(char * const response)
{
	unsigned vendor_id[8];

	pthread_mutex_lock(&dsl_modem_status->lock);

	if (sscanf(response, "far end itu identification: %x %x %x %x %x %x %x %x",
		&vendor_id[0], &vendor_id[1], &vendor_id[2], &vendor_id[3], &vendor_id[4], &vendor_id[5], &vendor_id[6], &vendor_id[7]) == 8)
	{
		snprintf(dsl_modem_status->remote_vendor_id, sizeof dsl_modem_status->remote_vendor_id, "%02x%02x%02x%02x%02x%02x%02x%02x (%c%c%c%c)",
			vendor_id[0], vendor_id[1], vendor_id[2], vendor_id[3], vendor_id[4], vendor_id[5], vendor_id[6], vendor_id[7],
			vendor_id[2], vendor_id[3], vendor_id[4], vendor_id[5]);
	}

	pthread_mutex_unlock(&dsl_modem_status->lock);
}

// Send the command 'cmd_str' to the DSL modem and call 'parse_func' for
// each line of text that it responds with.
static void run_modem_cmd(char const * const cmd_str, parse_func_t parse_func)
{
	char * buffer = NULL;
	FILE * fp;
	char * response = NULL;
	size_t len = 0;

	if (asprintf(&buffer, "adsl_cmd \"%s\"", cmd_str) == -1 ||
		buffer == NULL)
        goto done;

	//dsl_debug("%s", buffer);

	fp = popen(buffer, "r");
	if (fp == NULL)
		goto done;

    while (getline(&response, &len, fp) != -1)
    	parse_func(response);

	free(response);
	pclose(fp);

done:
	if (buffer != NULL)
		free(buffer);
}

static void push_config(void)
{
	char * buffer = NULL;

	if (dsl_modem_config->changed_mode)
	{
		dsl_debug("pushing mode to dsl modem\n");
		if (asprintf(&buffer, "wan ghs set std %s", config_dsl_mode_to_modem(dsl_modem_config->mode)) == -1 ||
			buffer == NULL)
			goto error;
		run_modem_cmd(buffer, null_parse_func);
		free(buffer);
		buffer = NULL;
		dsl_modem_config->changed_mode = false;
	}

	if (dsl_modem_config->changed_vpi_vci_encapsulation)
	{
		dsl_debug("pushing vpi/vci/encapsulation to dsl modem\n");
        int const encap = config_dsl_encapsulation_to_modem_encap(dsl_modem_config->encapsulation);
        int const mode = config_dsl_encapsulation_to_modem_mode(dsl_modem_config->encapsulation);
		if (asprintf(&buffer, "wan atm pvc add 0 1 %u %u %d %d", dsl_modem_config->vpi, dsl_modem_config->vci, encap, mode) == -1 ||
			buffer == NULL)
			goto error;
		run_modem_cmd(buffer, null_parse_func);
		free(buffer);
		buffer = NULL;
		dsl_modem_config->changed_vpi_vci_encapsulation = false;
	}

error:
	if (buffer != NULL)
		free(buffer);
}

static void poll_status(void)
{
	static unsigned int counter = 0;
	static bool is_line_up = false;

	// Poll for line state every cycle, so we can react to it quickly.
	run_modem_cmd("wan adsl status", parse_wan_adsl_status);

	// Let the event handler know about changes in line state.
	if (!is_line_up && dsl_modem_status->line_status == config_state_dsl_line_status_up)
	{
		is_line_up = true;
		counter = 0;
		dsl_monitor_send_event(DSL_MONITOR_INSTANCE, dsl_event_line_up, NULL, 0);
	}
	else if (is_line_up && dsl_modem_status->line_status == config_state_dsl_line_status_down)
	{
		is_line_up = false;
		dsl_monitor_send_event(DSL_MONITOR_INSTANCE, dsl_event_line_down, NULL, 0);
	}

	// These stats don't change very frequently.
	if (is_line_up)
	{
		if ((counter % 31) == 0)
		{
			run_modem_cmd("sys version", parse_sys_version);
			run_modem_cmd("wan adsl chandata", parse_wan_adsl_chandata);
			run_modem_cmd("wan adsl perfdata", parse_wan_adsl_perfdata);
			run_modem_cmd("wan adsl farituid", parse_wan_adsl_farituid);
		}

		if ((counter % 79) == 0)
		{
			run_modem_cmd("wan adsl linedata near", parse_wan_adsl_linedata_near);
			run_modem_cmd("wan adsl linedata far", parse_wan_adsl_linedata_far);
		}
	}

	counter++;
}

void * dsl_modem_thread(void * data)
{
	dsl_modem_config = calloc(1, sizeof *dsl_modem_config);
	if (dsl_modem_config == NULL)
	{
		dsl_error("failed to alloc dsl_modem_config\n");
		goto done;
	}
    pthread_mutex_init(&dsl_modem_config->lock, NULL);

	dsl_modem_status = calloc(1, sizeof *dsl_modem_status);
	if (dsl_modem_status == NULL)
	{
		dsl_error("failed to alloc dsl_modem_status\n");
		goto done;
	}
    pthread_mutex_init(&dsl_modem_status->lock, NULL);

	run_modem_cmd("sys version", parse_sys_version);

	while (true)
	{
		sleep(1);
		if (!dsl_modem_config->is_ppp_up)
			push_config();
		poll_status();
	}

done:
	if (dsl_modem_config != NULL)
	{
		free(dsl_modem_config);
		dsl_modem_config = NULL;
	}
	if (dsl_modem_status != NULL)
	{
		free(dsl_modem_status);
		dsl_modem_status = NULL;
	}
	pthread_exit(NULL);
}

