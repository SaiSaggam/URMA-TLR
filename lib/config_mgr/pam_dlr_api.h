/*****************************************************************************
 * Copyright (c) 2013 Digi International Inc., All Rights Reserved
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
 *****************************************************************************/

#ifndef pam_dlr_api_h
#define pam_dlr_api_h

#include <stdbool.h>        /* for bool type*/
#include "config_mgr.h"

#ifdef __cplusplus
extern "C" {
#endif


#define PAM_DLR_API_CURRENT_USER    (NULL)

/** 	Determine if user is authorized for requested access level

	@param name     name of the user, or PAM_DLR_API_CURRENT_USER
	@param access   access level to check

	@retval true        user is authorized for this access level
	@retval false       user is not authorized for this access level
*/
bool pam_dlr_check_is_authorized(char const * const name, config_mgr_access_level_t requested_access);

/** 	Get a user's access level

	@param name     name of the user, or PAM_DLR_API_CURRENT_USER

	@retval config_mgr_access_level_read_only  user has read-only access
	@retval config_mgr_access_level_read_write user has read/write access
	@retval config_mgr_access_level_super      user has super access
*/
config_mgr_access_level_t pam_dlr_get_access_level(char const * const name);


#ifdef __cplusplus
}
#endif

#endif
