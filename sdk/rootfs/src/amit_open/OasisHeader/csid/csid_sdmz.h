/*
 * @file Member ID Definition of Group SDMZ.
 *
 * Moderator: JackChiang
 * Group ID: 0x001d0000/0x801d0000
 */

#ifndef _CSID_SDMZ_H
#define _CSID_SDMZ_H

#include "csid_gid.h"

#define _CFG_SDMZ(x)		(CSID_GIDC_SDMZ|(x))
#define _STA_SDMZ(x)		(CSID_GIDS_SDMZ|(x))

/* Configuration Items */

#define CSID_C_SDMZ_ENABLE	_CFG_SDMZ(0x0001)	//T=u8, Enable/Disable
#define CSID_C_SDMZ_MAC	      	_CFG_SDMZ(0x0002)	//T=mac, mac address of sdmz
#define CSID_C_SDMZ_RELEASE_ENABLE	  	_CFG_SDMZ(0x0003)	//T=u32, dhcp release time enable of sdmz

#define CSID_C_SDMZ_MULTIWAN_ENABLE	  _CFG_SDMZ(0x0020)	//T=u8, multiwan Enable/Disable
#define CSID_C_SDMZ_MULTIWAN_MAC      _CFG_SDMZ(0x0030)	//T=mac, multiwan mac address of sdmz

/* Status Items */

#define CSID_S_SDMZ_ALTERED	_STA_SDMZ(0x0001)	//T=u8, ui altered
#define CSID_S_SDMZ_ENABLE	_STA_SDMZ(0x0002)	//T=u8, Enable/Disable
#define CSID_S_SDMZ_RELEASE_ENABLE	  	_STA_SDMZ(0x0003)	//T=u32, dhcp release time enable of sdmz

#endif //ifndef _CSID_SDMZ_H
