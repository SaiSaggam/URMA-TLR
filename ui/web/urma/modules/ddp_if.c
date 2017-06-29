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

#include "ddp_if.h"
#include "ddp_device.h"
#include "ddp_firmware.h"
#include "tlr_print.h"
#include "unittest_support.h"
#include <pythread.h>

typedef enum
{
    ddp_group_firmware_file,
    ddp_group_firmware_status,
    ddp_group_COUNT
} ddp_group_t;

static char const * const ddp_groups[ddp_group_COUNT] =
{
    "firmware_files",
    "firmware_status"
};

static size_t const first_group_id = 0;
static size_t const first_instance = 1;

static ddp_thread_t * ddp_thread_head = NULL;
static PyThread_type_lock thread_lock = NULL;

static void acquire_lock(void)
{
    if (thread_lock != NULL)
    {
        PyThread_acquire_lock(thread_lock, 1);
    }
}

static void release_lock(void)
{
    if (thread_lock != NULL)
    {
        PyThread_release_lock(thread_lock);
    }
}

static int get_group_index(char const * const group_name)
{
    int index;
    int result = -1;

    for (index = 0; index < ddp_group_COUNT; index++)
    {
        if (strcmp(group_name, ddp_groups[index]) == 0)
        {
            result = index;
            break;
        }
    }

    return result;
}

static bool run_python_callback(PyObject * const callback, const char *xml_str)
{
    bool success;
    PyObject *arglist = NULL;
    PyObject *returnVal = NULL;
    PyGILState_STATE gstate = PyGILState_Ensure();

    arglist = Py_BuildValue("(s)", xml_str);
    returnVal = PyEval_CallObject(callback, arglist);
    Py_XDECREF(arglist);

    success = (returnVal != NULL);

    if (success)
    {
        Py_XDECREF(returnVal);
    }

    PyGILState_Release(gstate);

    return success;
}

static bool send_xml_data(PyObject * const callback, char * const xml_str)
{
    bool success = false;

    if (xml_str != NULL)
    {
        success = run_python_callback(callback, xml_str);
        free(xml_str);
    }

    return success;
}

static bool prepare_instance(PyObject * const callback, size_t const group_id, size_t const instance, bool const is_config, bool const at_init)
{
    bool success;
    char * const xml_str = ddp_read_group(group_id, instance, is_config, at_init);

    if (xml_str == NULL)
    {
        success = true; /* group not supported? skip it */ 
        goto done;
    }

    success = send_xml_data(callback, xml_str);

done:
    return success;
}

static char * read_exceptions(char const * const group_name)
{
    char * result = NULL;
    int const index = get_group_index(group_name);

    if (index < 0) goto error;

    switch (index)
    {
        case ddp_group_firmware_file:
            result = ddp_get_firmware_files();
            break;

        case ddp_group_firmware_status:
            result = ddp_get_firmware_status();
            break;

        default:
            break;
    }

error:
    return result;
}

static bool handle_exceptions(PyObject * const callback, char const * const group_name)
{
    bool success;
    char * const xml_str = read_exceptions(group_name);

    if (xml_str == NULL)
    {
        success = true;
        goto done;
    }

    success = send_xml_data(callback, xml_str);

done:
    return success;
}

static bool prepare_group(PyObject * const callback, size_t const group_id, bool const is_config, bool const at_init)
{
    size_t instance;
    size_t const last_instance = ddp_get_group_instance(group_id, is_config);
    bool success = (last_instance == 0);

    if (success) goto done;

    for (instance = first_instance; instance <= last_instance; instance++)
    {
        success = prepare_instance(callback, group_id, instance, is_config, at_init);

        if (!success) break;
    }

done:
    return success;
}

static bool push_group(PyObject *callback, char const * const group_name, size_t const instance, bool const is_config)
{
    ssize_t const group_id = ddp_get_group_id(group_name, is_config);
    bool result = (group_id >= 0);
    bool const at_init = false;

    if (!result)
    {
        result = handle_exceptions(callback, group_name);
        goto error;
    }

    if (instance == 0) /* send entire group if the instance is 0 */
    {
        result = prepare_group(callback, group_id, is_config, at_init);
    }
    else
    {
         result = prepare_instance(callback, group_id, instance, is_config, at_init);
    }

error:
    return result;
}

static void polling_thread(void * context)
{
    ddp_thread_t * const info = context;

    do
    {
        bool const is_config = false;
        bool const result = push_group(info->callback, info->group_name, info->instance, is_config);

        if (!result) goto exit;
        if (info->rate == 0) goto exit;

        sleep(info->rate);

    } while (info->thread_id != -1);

exit:
    if (info->thread_id != -1)
    {
        if (!ddp_delete_thread(info->thread_id))
        {
            tlr_debug("failed to delete ddp polling thread\n");
        }
    }

    Py_CLEAR(info->callback);
    free(info->group_name);
    free(info);
    PyThread_exit_thread();
}

static bool prepare_all_groups(PyObject * const callback, bool const is_config, bool const at_init)
{
    bool success = false;
    size_t group_id;
    size_t const last_group_id = ddp_get_group_count(is_config);

    for (group_id = first_group_id; group_id < last_group_id; group_id++)
    {
        success = prepare_group(callback, group_id, is_config, at_init);

        if (!success) break;
    }

    return success;
}

bool ddp_get_config_groups(PyObject * const callback)
{
    bool const at_init = false; /* we need to populate config groups even at startup */
    bool const is_config = true;

    return prepare_all_groups(callback, is_config, at_init);
}

bool ddp_get_state_groups(PyObject * const callback, bool const at_init)
{
    bool const is_config = false;

    return prepare_all_groups(callback, is_config, at_init);
}

bool ddp_get_config_descriptor(PyObject * const callback)
{
    char * xml_str = ddp_read_config_descriptor();
    bool const success = send_xml_data(callback, xml_str);

    return success;
}

bool ddp_get_state_descriptor(PyObject * const callback)
{
    char * xml_str = ddp_read_state_descriptor();
    bool const success = send_xml_data(callback, xml_str);

    return success;
}

long ddp_create_thread(ddp_thread_t * const info)
{
    long result = -1;
    ddp_thread_t * const thread_ptr = malloc(sizeof *thread_ptr);

    if (thread_ptr == NULL) goto error;

    if (!PyEval_ThreadsInitialized())
    {
        PyEval_InitThreads();
        if (thread_lock == NULL)
        {
            thread_lock = PyThread_allocate_lock();
            if (thread_lock == NULL) goto error;
        }
    }

    *thread_ptr = *info;
    thread_ptr->group_name = strdup(info->group_name);
    thread_ptr->thread_id = PyThread_start_new_thread(polling_thread, thread_ptr);
    if (thread_ptr->thread_id == -1)
    {
        free(thread_ptr);
        goto error;
    }

    acquire_lock();
    thread_ptr->next = ddp_thread_head;
    ddp_thread_head = thread_ptr;
    release_lock();

    result = thread_ptr->thread_id;

error:
    return result;
}

bool ddp_delete_thread(long const thread_id)
{
    bool result = false;
    ddp_thread_t * pre_ptr = NULL;
    ddp_thread_t * cur_ptr;

    acquire_lock();
    cur_ptr = ddp_thread_head;

    while (cur_ptr != NULL)
    {
        if (cur_ptr->thread_id == thread_id)
        {
            if (pre_ptr == NULL)
            {
                ddp_thread_head = cur_ptr->next;
                ddp_thread_head = NULL;
            }
            else
            {
                pre_ptr->next = cur_ptr->next;
            }

            cur_ptr->thread_id = -1;
            cur_ptr->rate = 0;
            result = true;
            break;
        }

        pre_ptr = cur_ptr;
        cur_ptr = cur_ptr->next;
    }

    release_lock();
    return result;
}

bool ddp_push_config_change(char const * const group_name, size_t const instance)
{
    bool const is_config = true;

    return push_group(ddp_callback, group_name, instance, is_config);
}
