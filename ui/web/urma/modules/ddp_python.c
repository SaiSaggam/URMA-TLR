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
#include "unittest_support.h"
#include <pythread.h>

static char const ddpif_signature[] = "!digi#ddpv1!";
PyObject * ddp_callback = Py_None;

PyDoc_STRVAR(ddpif_init_doc,
"Populates the settings and state dictionary in DDP server\n\
it also populates the respective descriptors in DDP server\n\
\n\
It takes signature and callback function as input. \n\
It uses the provided callback function to send the settings and\n\
state values in xml format for every group/instance.\n\
\n\
This could results in a multiple callbacks, one for each group \n\
and instance. The callback will be called with obtained values in xml format \n\
Finally the function returns with the status. True for success.");
STATIC_FUNCTION PyObject * ddpif_init(PyObject * unused, PyObject * args)
{
    bool result = false;
    PyObject *callback = Py_None;
    (void)unused;

    {
        char const * signature = NULL;
        int const retVal = PyArg_ParseTuple(args, "sO", &signature, &callback);

        if (retVal < 0) goto error;
        if (strcmp(signature, ddpif_signature) != 0) goto error;
        if (!PyCallable_Check(callback)) goto error;

        ddp_callback = callback;
        Py_INCREF(callback);
    }

    result = ddp_get_config_groups(callback);
    if (!result) goto error;

    /* get the empty states for init */
    {
        bool const at_init = true;

        result = ddp_get_state_groups(callback, at_init);
        if (!result) goto error;
    }

    result = ddp_get_config_descriptor(callback);
    if (!result) goto error;

    result = ddp_get_state_descriptor(callback);

error:
    return PyBool_FromLong(result);
}

PyDoc_STRVAR(ddpif_set_doc,
"This method is called by the DDP server to set the device config.\n\
\n\
It expects signature, user, group name, group instance, element name, value\n\
and super user flag as input and returns true on success.");
STATIC_FUNCTION PyObject * ddpif_set(PyObject * unused, PyObject * args)
{
    bool result = false;
    ddp_group_info_t info;
    char const * signature;
    int const retVal = PyArg_ParseTuple(args, "sssissi", &signature,
                                        &info.user, &info.group_name,
                                        &info.instance, &info.element_name,
                                        &info.value, &info.access_level);

    (void)unused;
    if (retVal < 0) goto error;
    if (strcmp(signature, ddpif_signature) != 0) goto error;
    if (info.access_level == ddp_no_access) goto error;

    result = ddp_set_element(&info);

error:
    return PyBool_FromLong(result);
}

PyDoc_STRVAR(ddpif_apply_doc,
"This method is called by the DDP server to apply the device config.\n\
\n\
It expects signature, user, group name, group instance and super user flag\n\
as input and returns true on success.");
STATIC_FUNCTION PyObject * ddpif_apply(PyObject * unused, PyObject * args)
{
    bool result = false;
    ddp_group_info_t info;
    char const * signature;
    int const retVal = PyArg_ParseTuple(args, "sssii", &signature,
                                        &info.user, &info.group_name,
                                        &info.instance, &info.access_level);

    (void)unused;
    if (retVal < 0) goto error;
    if (strcmp(signature, ddpif_signature) != 0) goto error;
    if (info.access_level == ddp_no_access) goto error;

    result = ddp_apply_group(&info);

error:
    return PyBool_FromLong(result);
}

PyDoc_STRVAR(ddpif_save_doc,
"This method is called by the DDP server to save the device config.\n\
\n\
It expects signature and user as inputs and returns true on success.");
STATIC_FUNCTION PyObject * ddpif_save(PyObject * unused, PyObject *args)
{
    bool result = false;
    char const * user;
    char const * signature;
    int const retVal = PyArg_ParseTuple(args, "ss", &signature, &user);

    (void)unused;
    if (retVal < 0) goto error;
    if (strcmp(signature, ddpif_signature) != 0) goto error;

    result = ddp_save(user);

error:
    return PyBool_FromLong(result);
}

PyDoc_STRVAR(ddpif_poll_doc,
"This method is called by the DDP server to poll the device for a given status \n\
group and element.\n\
\n\
Theis method initiates a thread which polls the status periodically on a given rate.\n\
It expects signature, state group id, instance, polling interval and callback pointer as\n\
input. Returns a thread object which will be passed to cancel the polling when needed.");
STATIC_FUNCTION PyObject * ddpif_poll(PyObject * unused, PyObject * args)
{
    /* @TODO use the thread object */
    long result = -1;
    char const * signature;
    ddp_thread_t info;
    int const retVal = PyArg_ParseTuple(args, "ssiiO", &signature, &info.group_name, &info.instance, &info.rate, &info.callback);
    (void)unused;

    if (retVal < 0) goto error;
    if (strcmp(signature, ddpif_signature) != 0) goto error;
    if (!PyCallable_Check(info.callback)) goto error;

    Py_INCREF(info.callback);
    result = ddp_create_thread(&info);

error:
    return PyLong_FromLong(result);
}

PyDoc_STRVAR(ddpif_cancel_poll_doc,
"This method is called by the DDP server to cancel the running status \n\
\n\
It expects signature and the polling object to be cancelled as \n\
input. Returns true on success.\n");
STATIC_FUNCTION PyObject * ddpif_cancel_poll(PyObject * unused, PyObject *args)
{
    bool result = false;
    long thread_id;
    char const * signature;
    int const retVal = PyArg_ParseTuple(args, "sl", &signature, &thread_id);
    (void)unused;

    if (retVal < 0) goto error;
    if (strcmp(signature, ddpif_signature) != 0) goto error;

    result = ddp_delete_thread(thread_id);

error:
    return PyBool_FromLong(result);
}

PyDoc_STRVAR(ddpif_update_firmware_doc,
"This method is called by the DDP server to update the firmware \n\
\n\
It expects signature, user and file path as input and it returns True \n\
on success.\n");
STATIC_FUNCTION PyObject * ddpif_update_firmware(PyObject * unused, PyObject *args)
{
    bool result = false;
    char const * signature;
    char const * user;
    char const * file_path;
    bool remote = false;
    int const retVal = PyArg_ParseTuple(args, "sssi", &signature, &user, &file_path, &remote);
    (void)unused;

    if (retVal < 0) goto error;
    if (strcmp(signature, ddpif_signature) != 0) goto error;

    result = ddp_update_firmware(user, file_path, remote);

error:
    return PyBool_FromLong(result);
}

PyDoc_STRVAR(ddpif_reboot_doc,
"This method is called by the DDP server to reboot the device \n\
\n\
It expects signature, reboot after in minutes as input and it \n\
returns True on success.\n");
STATIC_FUNCTION PyObject * ddpif_reboot(PyObject * unused, PyObject *args)
{
    bool result = false;
    char const * signature;
    unsigned int reboot_after;
    int const retVal = PyArg_ParseTuple(args, "si", &signature, &reboot_after);
    (void)unused;

    if (retVal < 0) goto error;
    if (strcmp(signature, ddpif_signature) != 0) goto error;

    result = ddp_reboot(reboot_after);

error:
    return PyBool_FromLong(result);
}

PyDoc_STRVAR(ddpif_authenticate_doc,
"This method is called by the DDP server to authenticate the user \n\
\n\
It expects signature, username, password in clear text and access level \n\
in enum, 0:read_only (default), 1:read_write and 2: super. \n\
It returns access level set for the given user on success, else returns no_access.\n");
STATIC_FUNCTION PyObject * ddpif_authenticate(PyObject * unused, PyObject *args)
{
    ddp_access_t access_level = ddp_no_access;
    char const * signature;
    char const * username;
    char const * password;
    int const retVal = PyArg_ParseTuple(args, "sss", &signature, &username, &password);
    (void)unused;

    if (retVal < 0) goto error;
    if (strcmp(signature, ddpif_signature) != 0) goto error;

    access_level = ddp_authenticate(username, password);

error:
    return PyLong_FromLong(access_level);
}

PyMODINIT_FUNC PY_INIT_FUNC_DEF(PyDDP_MODULE_NAME)
{
    static PyMethodDef _ddp_methods[] = {
        {"init", (PyCFunction)ddpif_init, METH_VARARGS, ddpif_init_doc},
        {"set", (PyCFunction)ddpif_set, METH_VARARGS, ddpif_set_doc},
        {"apply", (PyCFunction)ddpif_apply, METH_VARARGS, ddpif_apply_doc},
        {"save", (PyCFunction)ddpif_save, METH_VARARGS, ddpif_save_doc},
        {"poll", (PyCFunction)ddpif_poll, METH_VARARGS, ddpif_poll_doc},
        {"cancel_poll", (PyCFunction)ddpif_cancel_poll, METH_VARARGS, ddpif_cancel_poll_doc},
        {"update_firmware", (PyCFunction)ddpif_update_firmware, METH_VARARGS, ddpif_update_firmware_doc},
        {"reboot", (PyCFunction)ddpif_reboot, METH_VARARGS, ddpif_reboot_doc},
        {"authenticate", (PyCFunction)ddpif_authenticate, METH_VARARGS, ddpif_authenticate_doc},
        {NULL, NULL, 0, NULL}
    };

    Py_InitModule3(MODULE_NAME(PyDDP_MODULE_NAME), _ddp_methods, "DDP module");
}
