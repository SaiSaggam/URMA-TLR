/*
 * @file Member ID Definition of Group VRRP.
 *
 * Moderator: Eric Chen
 * Group ID: 0x003A0000/0x803A0000
 */
#ifndef _CSID_VRRP_H
#define _CSID_VRRP_H

#include "csid_gid.h"

#define _CFG_VRRP(x)         (CSID_GIDC_VRRP|(x))
#define _STA_VRRP(x)         (CSID_GIDS_VRRP|(x))

/* Configuration Items */
#define CSID_C_VRRP_ENABLE						_CFG_VRRP(0x0001)	//T=u32, VRRPd enable, 1:yes 0:no
#define	CSID_C_VRRP_VIRTUAL_SERVER_ID			_CFG_VRRP(0x0010)	//T=u32, ID of Virtual Server [1-255]
#define	CSID_C_VRRP_VIRTUAL_SERVER_PRIORITY		_CFG_VRRP(0x0011)	//T=u32, the priority of this host in the virtual server  
#define CSID_C_VRRP_VIRTUAL_SERVER_IP			_CFG_VRRP(0x0020)	//T=ipv4, the ip address of the virtual server

/* Status Items */
#define CSID_S_VRRP_ALTERED						_STA_VRRP(0x0001)	//T=u32, ui altered

#endif //ifndef _CSID_VRRP_H
