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

#include "dsl_config.h"
#include "dsl_modem.h"
#include "dsl_log.h"
#include "config_mgr.h"
#include "iptables.h"
#include "interfaces.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void free_config(dsl_element_t * config)
{
	config_mgr_group_table_t const * const table = tlr_get_table(config_mgr_table_type_config);
	config_mgr_group_t const * const group = &table->groups[config_dsl];
	size_t const count = group->elements.count;
	size_t i;

	if (config != NULL)
	{
		for (i = 0; i < count; i++)
			if (config[i].value != NULL)
				free(config[i].value);
		free(config);
	}
}

static dsl_element_t * alloc_config(void)
{
	config_mgr_group_table_t const * const table = tlr_get_table(config_mgr_table_type_config);
	config_mgr_group_t const * const group = &table->groups[config_dsl];
	size_t const count = group->elements.count;
	dsl_element_t * config = NULL;
	size_t i;

	config = calloc(sizeof *config, count);
	if (config == NULL)
	{
		dsl_error("calloc(%d, %d) failed\n", sizeof(dsl_element_t), count);
		goto error;
	}

	for (i = 0; i < count; i++)
	{
		size_t element_size = tlr_get_element_size(&group->elements.element[i]);
		if (element_size == 0)
		{
			dsl_error("element size zero on #%d\n", i);
			goto error;
		}
		config[i].value = calloc(sizeof(char), element_size);
		if (config[i].value == NULL)
		{
			dsl_error("calloc(%d, %d) failed\n", sizeof(char), element_size);
			goto error;
		}
		config[i].size = element_size;
	}

	goto done;

error:
	free_config(config);
    config = NULL;

done:
	return config;
}

static int read_config(dsl_element_t * const config, int const instance)
{
	config_mgr_group_table_t const * const table = tlr_get_table(config_mgr_table_type_config);
	config_mgr_group_t const * const group = &table->groups[config_dsl];
	size_t const count = group->elements.count;
	tlr_handle_t tlr_handle = NULL;
	size_t i;
	int result = EXIT_FAILURE;

	tlr_handle = tlr_config_open(config_dsl, instance, config_mgr_access_level_super);
	if (tlr_handle == NULL)
	{
		dsl_error("tlr_config_open failed\n");
		goto done;
	}

	for (i = 0; i < count; i++)
	{
		config_status_t config_status;

		config_status = tlr_config_read(tlr_handle, i, config[i].value, config[i].size);
		if (config_status != config_status_success)
		{
			dsl_error("tlr_config_read(%p, %d) failed\n", tlr_handle, i);
			goto done;
		}
	}

	result = EXIT_SUCCESS;

done:
	if (tlr_handle != NULL)
		tlr_config_close(tlr_handle);

	return result;
}

static int apply_config(dsl_context_t * const context, dsl_element_t const * const new_config)
{
	int result = EXIT_FAILURE;
    dsl_element_t * const config = context->config;
	config_status_t config_status;

	if (strcmp(config[config_dsl_state].value, new_config[config_dsl_state].value) != 0)
	{
		dsl_debug("config_dsl_state changed\n");
		strcpy(config[config_dsl_state].value, new_config[config_dsl_state].value);
		// Nothing else to do; Interface Manager handles state changes.
	}

	if (strcmp(config[config_dsl_description].value, new_config[config_dsl_description].value) != 0)
	{
		dsl_debug("config_dsl_description changed\n");
		strcpy(config[config_dsl_description].value, new_config[config_dsl_description].value);
		// Nothing else to do.
	}

	if (strcmp(config[config_dsl_mode].value, new_config[config_dsl_mode].value) != 0)
	{
		dsl_debug("config_dsl_mode changed\n");

		config_status = tlr_config_string_to_value(config_dsl, config_dsl_mode, new_config[config_dsl_mode].value, &dsl_modem_config->mode);
		if (config_status != config_status_success)
			goto done;

		dsl_modem_config->changed_mode = true;

		strcpy(config[config_dsl_mode].value, new_config[config_dsl_mode].value);
	}

	if (strcmp(config[config_dsl_vpi].value, new_config[config_dsl_vpi].value) != 0 ||
		strcmp(config[config_dsl_vci].value, new_config[config_dsl_vci].value) != 0 ||
		strcmp(config[config_dsl_encapsulation].value, new_config[config_dsl_encapsulation].value) != 0)
	{
		dsl_debug("config_dsl_vpi/vci/encapsulation changed\n");

		config_status = tlr_config_string_to_value(config_dsl, config_dsl_vpi, new_config[config_dsl_vpi].value, &dsl_modem_config->vpi);
		if (config_status != config_status_success)
			goto done;
		config_status = tlr_config_string_to_value(config_dsl, config_dsl_vci, new_config[config_dsl_vci].value, &dsl_modem_config->vci);
		if (config_status != config_status_success)
			goto done;
		config_status = tlr_config_string_to_value(config_dsl, config_dsl_encapsulation, new_config[config_dsl_encapsulation].value, &dsl_modem_config->encapsulation);
		if (config_status != config_status_success)
			goto done;

		dsl_modem_config->changed_vpi_vci_encapsulation = true;

		strcpy(config[config_dsl_vpi].value, new_config[config_dsl_vpi].value);
		strcpy(config[config_dsl_vci].value, new_config[config_dsl_vci].value);
		strcpy(config[config_dsl_encapsulation].value, new_config[config_dsl_encapsulation].value);
	}

	if (strcmp(config[config_dsl_ppp_username].value, new_config[config_dsl_ppp_username].value) != 0 ||
		strcmp(config[config_dsl_ppp_password].value, new_config[config_dsl_ppp_password].value) != 0 ||
		strcmp(config[config_dsl_mtu].value, new_config[config_dsl_mtu].value) != 0)
	{
		dsl_debug("config_dsl_ppp_username/ppp_password/mtu changed\n");
		strcpy(config[config_dsl_ppp_username].value, new_config[config_dsl_ppp_username].value);
		strcpy(config[config_dsl_ppp_password].value, new_config[config_dsl_ppp_password].value);
		strcpy(config[config_dsl_mtu].value, new_config[config_dsl_mtu].value);
		// Nothing else to do; the PPP scripts will retrieve the new config
		// direct from UCI the next time a session is established.
	}

	if (strcmp(config[config_dsl_delay_up].value, new_config[config_dsl_delay_up].value) != 0)
	{
		dsl_debug("config_dsl_delay_up changed\n");

		config_status = tlr_config_string_to_value(config_dsl, config_dsl_delay_up, new_config[config_dsl_delay_up].value, &context->delay_up_timer_period);
		if (config_status != config_status_success)
			goto done;

		strcpy(config[config_dsl_delay_up].value, new_config[config_dsl_delay_up].value);
	}

	result = EXIT_SUCCESS;

done:
	return result;
}

bool dsl_config_initialise(dsl_context_t * const context)
{
    bool config_initialised = false;

	context->config = alloc_config();
	if (context->config == NULL)
	{
		dsl_error("could not alloc config\n");
		goto done;
	}

	config_initialised = true;

done:
	return config_initialised;
}

bool dsl_config_update(dsl_context_t * const context)
{
    bool config_updated = false;
	dsl_element_t * new_config;

	new_config = alloc_config();
	if (new_config == NULL)
	{
		dsl_error("could not alloc config\n");
		goto done;
	}

	if (read_config(new_config, context->instance) != EXIT_SUCCESS)
	{
		dsl_error("could not read config, instance %d\n", context->instance);
		goto done;
	}

	if (apply_config(context, new_config) != EXIT_SUCCESS)
	{
		dsl_error("could not apply config\n");
		goto done;
	}

	config_updated = true;

done:
    free_config(new_config);

	return config_updated;
}
