#ifndef __SAVE_CONFIG_ELEMENT_H__
#define __SAVE_CONFIG_ELEMENT_H__

#include <config_mgr.h>

bool element_needs_quoting(config_mgr_element_type_t element_type);
bool get_element_type(config_mgr_group_t const * const group, char const * const digi_parameter_name, config_mgr_element_type_t * const element_type);

#endif /*__SAVE_CONFIG_ELEMENT_H__*/
