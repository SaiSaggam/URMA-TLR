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
****************************************************************************/
#ifndef __DDPIFMODULE_H__
#define __DDPIFMODULE_H__

#include <Python.h>
#include <stdbool.h>

// Python module and C API name 
#define PyDDP_MODULE_NAME	ddp_if

#define STR(name) #name
#define STRINGIFY(name) STR(name)
#define CAT(first,second) first ## second

// explicit naming convention required for extension imports, use the common macro
// Name should be 'initmodulename' with no spaces or underscore or other characters
// the module_name passed in should just be plain text and not a "string"
#define PY_INIT_FUNC_DEF(module_name) CAT(init, module_name)(void)

// provided to turn module_name used in the init above into a string for other Python API calls
#define MODULE_NAME(module_name) STRINGIFY(module_name)

typedef struct ddp_thread_t
{
    char * group_name;
    unsigned int instance;
    unsigned int rate;
    PyObject * callback;
    long thread_id;
    struct ddp_thread_t * next;
} ddp_thread_t;

extern PyObject * ddp_callback;

bool ddp_get_config_groups(PyObject * const callback);
bool ddp_get_state_groups(PyObject * const callback, bool const at_init);
bool ddp_get_config_descriptor(PyObject * const callback);
bool ddp_get_state_descriptor(PyObject * const callback);
long ddp_create_thread(ddp_thread_t * const info);
bool ddp_delete_thread(long const thread_id);
bool ddp_push_config_change(char const * const group_name, size_t const instance);

#endif // __DDPIFMODULE_H__
