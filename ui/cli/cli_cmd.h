/*****************************************************************************
* Copyright (c) 2015 Digi International Inc., All Rights Reserved
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

#ifndef CLI_CMD_H_
#define CLI_CMD_H_

#include "cli.h"

cli_status_t cli_cmd_set_element(cli_t * const context, config_mgr_group_t const * const group, size_t const instance, size_t const element_id, char const * const element_value, char const ** const error_string);
cli_status_t cli_cmd_get_element(cli_t * const context, config_mgr_group_t const * const group, size_t const instance,
                                    size_t const element_id, char ** element_value);
int process_show_command(cli_t * const context, size_t const group_id, size_t const instance, char const * const parameter);
int process_action_command(cli_t * const context, size_t const action_id, char const * const parameter);
int cli_apply_config(cli_t * const context, size_t const group_id, size_t const instance);

cli_status_t cli_get_config(cli_t * context, char const * const group_name, size_t const instance,
                            char const * const element_name, char ** const value);

#endif /* CLI_CMD_H_ */
