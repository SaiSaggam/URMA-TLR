/*
 * @file Member ID Definition for UI team 
 *
 * Moderator: 
 * Group ID: 0x00730000/0x80730000
 */

#ifndef _CSID_UI_H
#define _CSID_UI_H

#include "csid_gid.h"

#define _CFG_UI(x)         (CSID_GIDC_UI|(x))
#define _STA_UI(x)         (CSID_GIDS_UI|(x))

/* Configuration Items */
//#define CSID_C_UI_REMOTE_ADMIN_ENABLE		_CFG_UI(0x0000)	//T=u8, enable, 1:yes 0:no

/* Status Items */
//#define CSID_S_UI_ALTERED				    _STA_UI(0x0000)	//T=u8, ui altered

#endif //ifndef _CSID_MISC_H
