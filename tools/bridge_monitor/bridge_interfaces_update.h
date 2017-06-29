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
*****************************************************************************/

#ifndef __BRIDGE_INTERFACE_UPDATE_H__
#define __BRIDGE_INTERFACE_UPDATE_H__

#include <stdbool.h>
#include <sys/types.h>

bool bridge_interfaces_changed(size_t const instance, char const * const old_value, char const * const new_value);
bool bridge_is_connected(size_t const instance);

#if defined(_CUNIT_) || defined(UNIT_TEST)
#include "interfaces.h"
bool parse_interface_name(char const * const interface_name, interfaces_t * const interface_type, size_t * const instance);
#endif

#endif /* __BRIDGE_MONITOR_CONFIG_H__ */
