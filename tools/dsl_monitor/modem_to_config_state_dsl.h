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

#ifndef __MODEM_TO_CONFIG_STATE_DSL__
#define __MODEM_TO_CONFIG_STATE_DSL__

#include "config_mgr.h"

config_state_dsl_line_mode_t modem_to_config_state_dsl_line_mode(char const * const modem_line_mode);
config_state_dsl_line_status_t modem_to_config_state_dsl_line_status(char const * const modem_line_status);
config_state_dsl_downstream_channel_type_t modem_to_config_state_dsl_channel_type(char const * const modem_channel_type);

#endif // __MODEM_TO_CONFIG_STATE_DSL__