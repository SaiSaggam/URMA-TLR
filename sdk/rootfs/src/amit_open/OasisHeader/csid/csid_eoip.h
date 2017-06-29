/*
 * CSID Definition of Group EOIP.
 *
 * Moderator: Fish
 * Group ID: 0x00B30000/0x80B30000
 */

#ifndef _CSID_EOIP_H
#define _CSID_EOIP_H

#include "csid_gid.h"

#define _CFG_EOIP(x)		(CSID_GIDC_EOIP|(x))
#define _STA_EOIP(x)		(CSID_GIDS_EOIP|(x))


/* Extra Definitions */
#define MAX_EOIP_TUNNEL               32

/* Configuration Items */
#define	CSID_C_EOIP_ENABLE			_CFG_EOIP(0x0001) //T=u32, D=0, enable/disable eoip,0:diable,1:enable
#define	CSID_C_EOIP_MAX_TUNNEL			_CFG_EOIP(0x0002) //T=u32, D=32, Maximum numbers of EoIP tunnel
#define	CSID_C_EOIP_TUN_ENABLE			_CFG_EOIP(0x0100) //T=u32, D=0, Enable/Disable this EoIP tunnel, 0:disable,1:enable
#define	CSID_C_EOIP_TUN_NAME			_CFG_EOIP(0x0200) //T=str, EoIP tunnel name
#define	CSID_C_EOIP_TUN_ID			_CFG_EOIP(0x0300) //T=u32, D=0, EoIP tunnel id
#define	CSID_C_EOIP_TUN_L_IP			_CFG_EOIP(0x0400) //T=ipv4, local EoIP tunnel IP
#define	CSID_C_EOIP_TUN_R_GATEWAY		_CFG_EOIP(0x0500) //T=ipv4, the IP address of remote router
#define	CSID_C_EOIP_TUN_R_IP			_CFG_EOIP(0x0600) //T=ipv4, remote EoIP tunnel IP
#define	CSID_C_EOIP_TUN_R_SUBNET		_CFG_EOIP(0x0700) //T=ipv4, domain of the remote LAN
#define	CSID_C_EOIP_TUN_R_MASK			_CFG_EOIP(0x0800) //T=u32, D=0, mask of the remote LAN
#define CSID_C_EOIP_TUN_DEFAULT_GW_EN           _CFG_EOIP(0x0900) //T=u32, D=0, Tunnel as default gateway,0:disable,1:Enable
#define	CSID_C_EOIP_TUN_BRIDGING_EN     _CFG_EOIP(0x0A00) //T=u32, D=0, Enable/Disable tunnel bridging, 0:Disable 1:Enable
#define	CSID_C_EOIP_TUN_BRIDGING_IF     _CFG_EOIP(0x0B00) //T=u32, D=0, Tunnel bridging interface, 0:br0/LAN,1:VLAN1,2:VLAN2,3:VLAN3 
#define CSID_C_EOIP_TUN_L_MASK			_CFG_EOIP(0x0C00) //T=ipv4, local EOIP tunnel IP subnet netmask

/* Status Items */ 
#define CSID_S_EOIP_ALTERED		 	_STA_EOIP(0x0001) //T=u32, D=0, UI altered 
#define CSID_S_EOIP_ALTERED_IDX			_STA_EOIP(0x0002) //T=u32, D=0, UI altered idx
#define CSID_S_EOIP_TUN_ACTION			_STA_EOIP(0x0100) //T=u32, D=0, EoIP tunnel Action, 0: no active,1:active
#define CSID_S_EOIP_TUN_ALTERED                 _STA_EOIP(0x0200) //T=u32, D=0, EoIP tunnel alter



#endif
