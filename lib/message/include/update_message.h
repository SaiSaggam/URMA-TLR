#ifndef __UPDATE_MESSAGE_H__
#define __UPDATE_MESSAGE_H__

#include <config_def.h>
#include <sys/types.h>

/* one of these structures is passed along with update message codes */
struct update_info_st
{
    config_group_t config_group;
    size_t instance;
};
typedef struct update_info_st update_info_st;

int get_update_info(int const argc, char ** const argv, void ** const info_result, size_t * const info_size);
int get_config_group_type_from_name(char const * const name);
char const * get_config_group_name_from_id(config_group_t const group_id);


#endif /* __UPDATE_MESSAGE_H__ */
