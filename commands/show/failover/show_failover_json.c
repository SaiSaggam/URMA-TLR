/*****************************************************************************
* Copyright (c) 2016 Digi International Inc., All Rights Reserved
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

#include "show_failover_work.h"
#include "show_failover_json.h"
#include "tlr_print.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>
#include <config_mgr.h>
#include <strlcpy.h>
#include <json.h>
#include "unittest_support.h"
#include <assert.h>

static void add_element_value_to_show_object(tlr_show_object_t * const show_object,
                                             failover_status_t const * const pstatus,
                                             config_state_failover_t const element_id)
{
    void const * value = NULL;
    config_state_failover_status_t  status;
    config_state_failover_primary_interface_status_t primary_if_status;
    config_state_failover_secondary_interface_status_t  second_if_status;
    char const empty_space[] = "";
    static char up_string[] = "Up";
    char temp[MAX_FIELD_LENGTH*2];
    
    switch (element_id)
    {
        case config_state_failover_description:
            value = pstatus->description;
            break;

        case config_state_failover_status:
            status = (pstatus->status) ?  config_state_failover_status_up : config_state_failover_status_down;
            value = &status;
            break;

        case config_state_failover_primary_interface:
            value = pstatus->primary_if;
            break;

        case config_state_failover_primary_interface_status:
            primary_if_status = (!memcmp(pstatus->primary_if_status, up_string, sizeof up_string)) ?  config_state_failover_primary_interface_status_up :       config_state_failover_primary_interface_status_down;
            value = &primary_if_status;
            break;

        case config_state_failover_secondary_interface:
            value = pstatus->secondary_if;
            break;

        case config_state_failover_secondary_interface_status:
            second_if_status = (!memcmp(pstatus->secondary_if_status,up_string, sizeof up_string)) ?  config_state_failover_secondary_interface_status_up : config_state_failover_secondary_interface_status_down;
            value = &second_if_status;
            break;

        case config_state_failover_using_interface:
            value = pstatus->using_if;
            break;

        case config_state_failover_detection_method:
            value = pstatus->detection_method;
            break;

        case config_state_failover_last_ping:
            if (pstatus->using_ping == true)
            {
                snprintf(temp, sizeof temp, "%s seconds ago", pstatus->probe_received_time);
                value = temp;
            }
            else
                value = empty_space;
            break;

        case config_state_failover_failing_over:
            if (pstatus->show_failing_over)
            {
                snprintf(temp, sizeof temp, "in %s seconds", pstatus->failing_over_time);
                value = temp;
            }
            else
                value = empty_space;

            break;
        case config_state_failover_switching_back:
            if (pstatus->show_switch_back)
            {
                snprintf(temp, sizeof temp, "in %s seconds", pstatus->switching_back_time);
                value = temp;
            }
            else
                value = empty_space;
            break;
    }

    tlr_show_object_add_element(show_object, element_id, value);
}

static void add_element_to_show_object(tlr_show_object_t * const show_object, failover_status_t const * const pstatus, config_mgr_element_t const * const element)
{
    size_t const element_id = element->id;

    add_element_value_to_show_object(show_object, pstatus, element_id);
}

static void add_elements_to_show_object(tlr_show_object_t * const show_object, failover_status_t const * const pstatus, int const instance)
{
    config_mgr_group_t const * const group = tlr_get_group_ptr(config_state_failover, instance, config_mgr_get_group);
    int i;

    assert(group != NULL);

    for (i = 0; i < group->elements.count; i++)
    {
        //printf("adding element %s\n", group->elements.element[i].name);
        add_element_to_show_object(show_object, pstatus, &group->elements.element[i]);
    }
}

int show_failover_json(failover_status_t const * const pstatus, int const instance)
{

    int result = -1;

    tlr_show_object_t * const show_object = tlr_show_object_create(config_state_failover, instance);

    if (show_object == NULL)
    {
        tlr_debug("failed to create json show object\n");
        goto error;
    }

    add_elements_to_show_object(show_object, pstatus, instance);

    printf("%s\n", tlr_show_object_to_json_string(show_object));
    tlr_show_object_destroy(show_object);

    result = 0;
error:
    return result;

}

