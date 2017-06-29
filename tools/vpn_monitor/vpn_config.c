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

#include "vpn_config.h"
#include "config_mgr.h"
#include "debug.h"
#include <security.h>
#include <tlr_common.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct
{
	char * value;
	size_t size;
} element_t;

struct config_st
{
    config_group_t group_id;
    size_t count;
    element_t * elements;
};

static void free_config(config_st * config)
{
	if (config != NULL)
	{
        if (config->elements != NULL)
        {
            size_t i;

            for (i = 0; i < config->count; i++)
            {
                free(config->elements[i].value);
            }
            free(config->elements);
        }
		free(config);
	}
}

static config_st * alloc_config(config_group_t group_id)
{
	config_mgr_group_table_t const * const table = tlr_get_table(config_mgr_table_type_config);
	config_mgr_group_t const * const group = &table->groups[group_id];
	size_t const count = group->elements.count;
    config_st * config = NULL;
	size_t i;

    config = calloc(1, sizeof *config);
	if (config == NULL)
	{
		DEBUG("calloc(%d, %zd) failed\n", 1, sizeof(*config));
		goto error;
	}
    config->group_id = group_id;
    config->elements = calloc(count, sizeof *config->elements);
    if (config->elements == NULL)
    {
        DEBUG("calloc(%zd, %zd) failed\n", count, sizeof(*config->elements));
        goto error;
    }
    config->count = count;

	for (i = 0; i < count; i++)
	{
		size_t element_size = tlr_get_element_size(&group->elements.element[i]);

		if (element_size == 0)
		{
			DEBUG("element size zero on #%zd name: %s\n", i, group->elements.element[i].name);
			goto error;
		}
		config->elements[i].value = calloc(element_size, sizeof(char));
		if (config->elements[i].value == NULL)
		{
			DEBUG("calloc(%zd, %zd) failed\n", element_size, sizeof(char));
			goto error;
		}
		config->elements[i].size = element_size;
	}

	goto done;

error:
	if (config != NULL)
	{
		free_config(config);
		config = NULL;
	}

done:
	return config;
}

static int read_config(config_st * const config, int const instance)
{
	tlr_handle_t tlr_handle = NULL;
	size_t i;
	int result;

	tlr_handle = tlr_config_open(config->group_id, instance, config_mgr_access_level_super);
	if (tlr_handle == NULL)
	{
		DEBUG("tlr_config_open failed\n");
        result = -1;
		goto done;
	}

	for (i = 0; i < config->count; i++)
	{
		config_status_t config_status;

		config_status = tlr_config_read(tlr_handle, i, config->elements[i].value, config->elements[i].size);
		if (config_status != config_status_success)
		{
			DEBUG("tlr_config_read(%p, %zd) failed\n", tlr_handle, i);
            result = -1;
			goto done;
		}
	}

	result = 0;

done:
	if (tlr_handle != NULL)
		tlr_config_close(tlr_handle);

	return result;
}

static bool config_check_for_significant_change(config_st * const config, config_st const * const new_config)
{
    bool config_changed;
    size_t index;

    assert(config->count == new_config->count);

    /*
     * Done this way, we'lll get a compile time error whenever a new paramter is added, which is
     * much better than this function getting out of date as soon as a new parameter is added.
     * XXX if config_ipsec_psk (encpassword type) is set to the same value this function sees a
     * change because the encrypted vaule is regenerated each time.
     */
    for (index = 0; index < config->count; index++)
    {
        char old_clear_password[256];
        char new_clear_password[256];
        char * old_value = NULL;
        char * new_value = NULL;

        switch ((config_ipsec_t)index)
        {
            /* insignificant */
            case config_ipsec_state:
            case config_ipsec_description:
                continue;
            /* significant parameters that require ipsec.conf and tunnels to be updated */
            case config_ipsec_psk:
            {
                if (decrypt_password(config->elements[index].value, old_clear_password, sizeof old_clear_password)
                    && decrypt_password(new_config->elements[index].value, new_clear_password, sizeof new_clear_password))
                {
                    old_value = old_clear_password;
                    new_value = new_clear_password;
                }
                else    /* can't compare the values */
                {
                    continue;
                }
                break;
            }
            case config_ipsec_peer:
            case config_ipsec_local_network:
            case config_ipsec_local_mask:
            case config_ipsec_remote_network:
            case config_ipsec_remote_mask:
            case config_ipsec_esp_authentication:
            case config_ipsec_esp_encryption:
            case config_ipsec_esp_diffie_hellman:
            case config_ipsec_auth_by:
            case config_ipsec_local_id:
            case config_ipsec_remote_id:
            case config_ipsec_lifetime:
            case config_ipsec_lifebytes:
            case config_ipsec_margintime:
            case config_ipsec_marginbytes:
            case config_ipsec_random:
            case config_ipsec_ike:
            case config_ipsec_ike_mode:
            case config_ipsec_ike_encryption:
            case config_ipsec_ike_authentication:
            case config_ipsec_ike_diffie_hellman:
            case config_ipsec_ike_lifetime:
            case config_ipsec_ike_tries:
            case config_ipsec_dpddelay:
            case config_ipsec_dpdtimeout:
            case config_ipsec_dpd:
                old_value = config->elements[index].value;
                new_value = new_config->elements[index].value;
                break;
        }
        if (strcmp(old_value, new_value) != 0)
        {
            DEBUG("element id %zd has changed (%s->%s)\n",
                  index,
                  old_value,
                  new_value);
            config_changed = true;
            goto done;
        }

    }
    config_changed = false;

done:

    return config_changed;
}

int config_init(config_st * * const config, size_t const instance, config_group_t config_group)
{
	int result = -1;

	*config = alloc_config(config_group);
	if (*config == NULL)
	{
		DEBUG("could not alloc config\n");
		goto done;
	}
    if (read_config(*config, instance) != 0)
    {
        DEBUG("could not read config, instance %zd\n", instance);
        goto done;
    }

	result = 0;

done:
	return result;
}

int config_update(config_st * * const config, size_t const instance, bool * const config_has_changed)
{
	int result = -1;
	config_st * new_config;

	new_config = alloc_config((*config)->group_id);
	if (new_config == NULL)
	{
		DEBUG("could not alloc config\n");
		goto done;
	}

	if (read_config(new_config, instance) != 0)
	{
		DEBUG("could not read config, instance %zd\n", instance);
		goto done;
	}

    *config_has_changed = config_check_for_significant_change(*config, new_config);
    DEBUG("config has changed: %d\n", *config_has_changed);
    free_config(*config);
    *config = new_config;
    new_config = NULL;

	result = 0;

done:
	if (new_config != NULL)
		free_config(new_config);

	return result;
}


