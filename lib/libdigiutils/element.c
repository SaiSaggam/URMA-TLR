#include "element.h"
#include <string.h>
#include "config_mgr.h"

bool element_needs_quoting(config_mgr_element_type_t element_type)
{
    bool needs_quoting = false;

    /*
        I'm not sure that any value types actually _require_ wrapping in quotes.
        The CLI seems to strip them off if they're present when running commands
        The original version of this code indicated that quotes were required on all but
        integer style types, so that's what I'll stay with here.
    */
    switch (element_type)
    {
        case config_mgr_element_type_uint32:
        case config_mgr_element_type_index:
        case config_mgr_element_type_int32:
            needs_quoting = false;
            break;
        case config_mgr_element_type_enum_multi:
        case config_mgr_element_type_encpassword:
        case config_mgr_element_type_string:
        case config_mgr_element_type_password:
        case config_mgr_element_type_enum:
        case config_mgr_element_type_on_off:
        case config_mgr_element_type_boolean:
        case config_mgr_element_type_ipaddr:
        case config_mgr_element_type_fqdn:
        case config_mgr_element_type_mac_addr:
        case config_mgr_element_type_counter32:
        case config_mgr_element_type_counter64:
        case config_mgr_element_type_interface:
        case config_mgr_element_type_none:
            needs_quoting = true;
            break;
    }

    return needs_quoting;
}

/*
    get_element_type:
    given a config group and parameter name in Digi format, attempt to locate the Digi parameter type
    return true if the element type was found, else return false
    if found, the element type is written into element_type
    XXX should this function be put into a library somewhere. It looks like the sort of thing that might be
    needed frequently.
*/
bool get_element_type(config_mgr_group_t const * const group, char const * const digi_parameter_name, config_mgr_element_type_t * const element_type)
{
    bool element_type_found = false;
    size_t i;

    for (i = 0; i < group->elements.count; i++)
    {
        config_mgr_element_t const * const element = &group->elements.element[i];

        if (strcmp(element->name, digi_parameter_name) == 0)
        {
            *element_type = element->type;
            element_type_found = true;
            break;
        }

    }
    return element_type_found;
}


