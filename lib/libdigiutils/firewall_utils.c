#include <firewall_utils.h>
#include <config_mgr.h>
#include <string.h>

bool firewall_line_is_start_tag(char const * const line)
{
    return strstr(line, FIREWALL_TAG) != NULL;
}

bool firewall_line_is_end_tag(char const * const line)
{
    return strstr(line, FIREWALL_END_TAG) != NULL;
}


