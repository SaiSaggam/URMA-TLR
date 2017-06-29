/*
 * @file Member ID Definition of Group FIX.
 *
 * Moderator: GeorgeWang
 * Group ID: 0x00110000/0x80110000
 */

#ifndef _CSID_FIX_H
#define _CSID_FIX_H

#include "csid_gid.h"

#define _CFG_FIX(x)		(CSID_GIDC_FIX|(x))
#define _STA_FIX(x)		(CSID_GIDS_FIX|(x))

#define FIX_CNT_TYPE_ONDEMAND          0x00
#define FIX_CNT_TYPE_AUTORECONNECT     0x01
#define FIX_CNT_TYPE_MANUAL            0x02

#define FIX_CNT_STATUS_DISCONNECT      0x00
#define FIX_CNT_STATUS_CONNECTING      0x01                                          
#define FIX_CNT_STATUS_CONNECT         0x02
#define FIX_CNT_STATUS_DISCONNECTING   0x03
#define FIX_CNT_STATUS_WAIT_TRAFFIC    0x05

/* Configuration Items */
#define CSID_C_FIX_IF_IP	      _CFG_FIX(0x0001)	//T=ipv4, fix ip
#define CSID_C_FIX_IF_NM	      _CFG_FIX(0x0002)	//T=ipv4, fix netmask
#define CSID_C_FIX_IF_GW	      _CFG_FIX(0x0003)	//T=ipv4, fix gateway
#define CSID_C_FIX_IF_PRIDNS	  _CFG_FIX(0x0004)	//T=ipv4, fix primary dns
#define CSID_C_FIX_IF_SECDNS	  _CFG_FIX(0x0005)	//T=ipv4, fix secondary dns
#define CSID_C_FIX_MTU            _CFG_FIX(0x0009)  //T=u16, D=0, MTU
#define CSID_C_FIX_NAT_DISABLE	  _CFG_FIX(0x000f)	//T=u32, Static IP wantype NAT disable
#define CSID_C_FIX_AP_IF_DEFAULT_IP	    _CFG_FIX(0x000a)	//T=ipv4, default fix ip

/* When device is in AP Mode. e.g. CDM532RP ... */
#define CSID_C_FIX_AP_IF_IP	      _CFG_FIX(0x0011)	//T=ipv4, fix ip for AP mode
#define CSID_C_FIX_AP_IF_NM	      _CFG_FIX(0x0012)	//T=ipv4, fix netmask for AP mode
#define CSID_C_FIX_AP_IF_GW	      _CFG_FIX(0x0013)	//T=ipv4, fix gateway for AP mode
#define CSID_C_FIX_AP_IF_PRIDNS	  _CFG_FIX(0x0014)	//T=ipv4, fix primary dns for AP mode
#define CSID_C_FIX_AP_IF_SECDNS	  _CFG_FIX(0x0015)	//T=ipv4, fix secondary dns for AP mode
#define CSID_C_FIX_AP_MTU         _CFG_FIX(0x0019)  //T=u16,  MTU for AP mode
#define CSID_C_FIX_AP_NAT_DISABLE _CFG_FIX(0x001f)	//T=u32,  Static IP wantype NAT disable for AP mode

/* When device is in Client/STA Mode. e.g. CDM532RP ... */
#define CSID_C_FIX_STA_IF_IP	   _CFG_FIX(0x0021)	//T=ipv4, fix ip for Client mode
#define CSID_C_FIX_STA_IF_NM	   _CFG_FIX(0x0022)	//T=ipv4, fix netmask for Client mode
#define CSID_C_FIX_STA_IF_GW	   _CFG_FIX(0x0023)	//T=ipv4, fix gateway for Client mode
#define CSID_C_FIX_STA_IF_PRIDNS   _CFG_FIX(0x0024)	//T=ipv4, fix primary dns for Client mode
#define CSID_C_FIX_STA_IF_SECDNS   _CFG_FIX(0x0025)	//T=ipv4, fix secondary dns for Client mode
#define CSID_C_FIX_STA_MTU         _CFG_FIX(0x0029) //T=u16,  MTU for Client mode
#define CSID_C_FIX_STA_NAT_DISABLE _CFG_FIX(0x002f)	//T=u32,  Static IP wantype NAT disable for Client mode

/* Reserved 16 items for Multi-WAN,  X is from 1 .... */
#define CSID_C_FIX_MULTIWAN_IF_IP	      _CFG_FIX(0x0110)	//T=ipv4, fix ip
#define CSID_C_FIX_MULTIWAN_IF_NM	      _CFG_FIX(0x0120)	//T=ipv4, fix netmask
#define CSID_C_FIX_MULTIWAN_IF_GW	      _CFG_FIX(0x0130)	//T=ipv4, fix gateway
#define CSID_C_FIX_MULTIWAN_IF_PRIDNS	  _CFG_FIX(0x0140)	//T=ipv4, fix primary dns
#define CSID_C_FIX_MULTIWAN_IF_SECDNS	  _CFG_FIX(0x0150)	//T=ipv4, fix secondary dns
#define CSID_C_FIX_MULTIWAN_NAT_DISABLE	  _CFG_FIX(0x0160)	//T=u32, Static IP wantype NAT disable
#define CSID_C_FIX_MULTIWAN_MTU            _CFG_FIX(0x0170)  //T=u16, D=0, MTU

/* Reserved 16 items for VoIP-WAN */
#define CSID_C_FIX_VOIPWAN_IF_IP	      _CFG_FIX(0x0180)	//T=ipv4, fix ip
#define CSID_C_FIX_VOIPWAN_IF_NM	      _CFG_FIX(0x0190)	//T=ipv4, fix netmask
#define CSID_C_FIX_VOIPWAN_IF_GW	      _CFG_FIX(0x01a0)	//T=ipv4, fix gateway
#define CSID_C_FIX_VOIPWAN_IF_PRIDNS	  _CFG_FIX(0x01b0)	//T=ipv4, fix primary dns
#define CSID_C_FIX_VOIPWAN_IF_SECDNS	  _CFG_FIX(0x01c0)	//T=ipv4, fix secondary dns
#define CSID_C_FIX_VOIPWAN_MTU	          _CFG_FIX(0x01d0)	//T=u16, MTU
#define CSID_C_FIX_VOIPWAN_NAT_DISABLE	  _CFG_FIX(0x01e0)	//T=u32, Static IP wantype NAT disable

/* Status Items */

#define CSID_S_FIX_PKTIN_NUM              _STA_FIX(0x0001)	//T=u32, number of packet input
#define CSID_S_FIX_PKTOUT_NUM             _STA_FIX(0x0002)	//T=u32, number of packet output
#define CSID_S_FIX_ALTERED		          _STA_FIX(0x0003)	//T=u32, ui altered
#define CSID_S_FIX_CNT_STATUS	          _STA_FIX(0x0004)	//T=u32, FIX IP connection status 

#define CSID_S_FIX_CMD_STATUS	          _STA_FIX(0x0010)	//T=u32, FIX IP cmd status 

/* Reserved 16 items for Multi-WAN,  X is from 1 .... */
#define CSID_S_FIX_MULTIWAN_PKTIN_NUM   _STA_FIX(0x0100)	//T=u32,   number of packet input
#define CSID_S_FIX_MULTIWAN_PKTOUT_NUM  _STA_FIX(0x0110)	//T=u32,   number of packet output
#define CSID_S_FIX_MULTIWAN_ALTERED		_STA_FIX(0x0120)	//T=u32, ui altered

/*multiwan*/
#define CSID_S_FIX_MULTIWAN_CMD_STATUS  _STA_FIX(0x0150)	//T=u32, FIX IP multiwan cmd status

#endif //ifndef _CSID_FIX_H
