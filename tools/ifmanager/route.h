#ifndef _ROUTE_H_
#define _ROUTE_H_

#include "ifmanager.h"
// Possible outcomes when we add a route
typedef enum {
    route_added_ok,
    route_already_exists,
    route_add_error
} execute_route_return_t;


int ifm_update_route(ifm_update_info_t const * const info);
int ifm_apply_routes(char const * const ifname);
int ifm_delete_routes(char const * const ifname);
int apply_default_route(int const instance);
int update_default_route(int const instance);
int apply_static_route(int const instance);
int update_static_route(int const instance);
execute_route_return_t ifm_execute_route_command(char * const cmd);
bool is_default_route_valid(int const instance);
bool is_static_route_valid(int const instance);
bool does_interface_match_static_route(char const * const ifname, int const instance);
bool does_interface_match_default_route(char const * const ifname, int const instance);

#endif /* _ROUTE_H_ */
