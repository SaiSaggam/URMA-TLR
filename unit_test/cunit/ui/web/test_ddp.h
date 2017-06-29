#ifndef _TEST_DDP_H_
#define _TEST_DDP_H_

typedef struct
{
    char const * name;
    size_t id;
    size_t instance;
    char * value;
} test_group_t;

typedef struct
{
    long thread_id;
    char const * sys_command;
    test_group_t config;
    test_group_t status;
    int error;
} ddp_test_info_t;

extern ddp_test_info_t ddp_test_info;

#endif /* _TEST_DDP_TLR_H_ */
