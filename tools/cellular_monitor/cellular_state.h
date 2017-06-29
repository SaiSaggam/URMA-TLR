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
#ifndef __CELLULAR_STATE_H__
#define __CELLULAR_STATE_H__

#include "context.h"

void set_state(cellular_context_t * const context, cellular_state_t const new_state);
void state_initialize_handler(cellular_context_t * const context, void * const data);
void state_idle_handler(cellular_context_t * const context, void * const data);
void state_connect_handler(cellular_context_t * const context, void * const data);
void state_connecting_handler(cellular_context_t * const context, void * const data);
void state_connected_handler(cellular_context_t * const context, void * const data);
void state_disconnect_handler(cellular_context_t * const context, void * const data);
void state_disconnecting_handler(cellular_context_t * const context, void * const data);
void state_disconnected_handler(cellular_context_t * const context, void * const data);
void state_wait_modem_handler(cellular_context_t * const context, void * const data);

void restart_usb_control(void);

extern char const * cellular_state_strings[];

#endif /* __CELLULAR_STATE_H__ */
