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

#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <json.h>

#include "config_def.h"
#include "config_table.h"
#include "config_mgr.h"
#include "config_status.h"
#include "validate.h"
#include <tlr_common.h>
#include "getlinefd.h"
#include "tlr_popen.h"


tlr_handle_t tlr_show_open(char const * const format, int const id, size_t const instance, char const * const parameter, config_mgr_access_level_t const access)
{
    FILE * fp = NULL;
    char * command = NULL;
    char const * const argument = (parameter != NULL) ? parameter : "";
    int length;

    config_mgr_group_t const * const group_ptr = tlr_get_group_ptr(id, instance, config_mgr_get_group);

    if (group_ptr == NULL) goto ret;

    if (tlr_verify_access(group_ptr->access, config_mgr_read_operation, access) != config_status_success)
    {
        tlr_config_error("Permission denied\n");
        goto ret;
    }

    length = asprintf(&command, DESTDIR_STR "/usr/bin/show_%s -f %s -i %zd %s", group_ptr->name, format, instance, argument);
    if (length == -1)
    {
        tlr_config_error("Internal error insufficient command length for %s show command\n", group_ptr->name);
        goto ret;
    }

    tlr_config_debug("executing \"%s\"\n", command);
    fp = tlr_popen(command, "r");
    if (fp == NULL) goto ret;

ret:
    free(command);
    return fp;
}

ssize_t tlr_show_read(tlr_handle_t const handle, char * const data, size_t const data_length, size_t const timeout)
{
    ssize_t rc = 0;
    FILE  * fp = (FILE *)handle;

    if (timeout != WAIT_FOREVER)
    {
        struct timeval tv;
        fd_set fds;
        int status;
        int fd = fileno(fp);

        tv.tv_sec = timeout/1000;
        tv.tv_usec = (timeout % 1000) * 1000;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        status = select(fd+1, &fds, NULL, NULL, &tv);

        if (status <= 0)
        {
            rc = status;
            goto ret;
        }
    }

    *data = '\0';
    fgets((char *)data, data_length, fp);
    rc = strlen(data);

ret:
    return rc;
}

char * tlr_show_read_line_alloc(tlr_handle_t const handle, size_t const timeout_millisecs)
{
    FILE  * fp = (FILE *)handle;
    char * line = NULL;
    size_t line_size = 0;
    int fd = fileno(fp);
    struct timeval * timeout_ptr;
    struct timeval tv;
    int getline_result;

    if (timeout_millisecs != WAIT_FOREVER)
    {
        tv.tv_sec = timeout_millisecs / 1000;
        tv.tv_usec = (timeout_millisecs % 1000) * 1000;
        timeout_ptr = &tv;
    }
    else
    {
        timeout_ptr = NULL;
    }

    getline_result = getlinefd_timeout(&line, &line_size, fd, timeout_ptr);
    if (getline_result <= 0)
    {
        free(line);
        line = NULL;
        goto ret;
    }

ret:
    return line;
}

config_status_t tlr_show_close(tlr_handle_t const handle)
{
    FILE * fp = (FILE *)handle;
    return (tlr_pclose(fp) == 0) ? config_status_success : config_status_system_error;
}
