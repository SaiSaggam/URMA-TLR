/*****************************************************************************
* Copyright (c) 2015, 2016 Digi International Inc., All Rights Reserved
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

#ifndef _CONFIG_MGR_VALIDATE_H_
#define _CONFIG_MGR_VALIDATE_H_

#include "config_mgr.h"

typedef enum
{
    config_mgr_read_operation,
    config_mgr_write_operation,
} config_mgr_operation_t;

config_status_t tlr_verify_access(config_mgr_access_t const access, config_mgr_operation_t const operation, config_mgr_access_level_t const user_access_level);
config_status_t tlr_verify_range(config_mgr_valid_function_t validation, void * value_limit, config_mgr_element_type_t const type, char const * const value);

#endif /* _CONFIG_MGR_VALIDATE_H_ */
