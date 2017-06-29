#ifndef __FIREWALL_UTILS_H__
#define __FIREWALL_UTILS_H__

#include <stdbool.h>

bool firewall_line_is_start_tag(char const * const line);
bool firewall_line_is_end_tag(char const * const line);


#endif /* __FIREWALL_UTILS_H__ */
