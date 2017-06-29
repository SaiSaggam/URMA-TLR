/*
 * @file Member ID Definition of Group PPPOA.
 *
 * Moderator: GeorgeWang
 * Group ID: 0x00370000/0x80370000
 */

#ifndef _CSID_PPPOA_H
#define _CSID_PPPOA_H

#include "csid_gid.h"

#define _CFG_PPPOA(x)		(CSID_GIDC_PPPOA|(x))
#define _STA_PPPOA(x)		(CSID_GIDS_PPPOA|(x))

/* Extra Definitions */

#define PPPOA_CNT_TYPE_ONDEMAND         0x00
#define PPPOA_CNT_TYPE_AUTORECONNECT    0x01
#define PPPOA_CNT_TYPE_MANUAL           0x02

#define PPPOA_AUTHTYPE_AUTO             0x00
#define PPPOA_AUTHTYPE_PAP              0x01
#define PPPOA_AUTHTYPE_CHAP             0x02
#define PPPOA_AUTHTYPE_MSCHAP           0x04
#define PPPOA_AUTHTYPE_MSCHAPV2         0x08
#define PPPOA_AUTHTYPE_EAP              0x10

#define PPPOA_CNT_STATUS_DISCONNECT     0x00
#define PPPOA_CNT_STATUS_CONNECTING     0x01
#define PPPOA_CNT_STATUS_CONNECT        0x02
#define PPPOA_CNT_STATUS_DISCONNECTING  0x03
#define PPPOA_CNT_STATUS_WAIT_TRAFFIC	0x05
#define PPPOA_CNT_STATUS_AUTH_FAIL		0x06

/* Configuration Items */
#define CSID_C_PPPOA_NAT_DISABLE     	_CFG_PPPOA(0x000f)	//T=u32, PPPoA wantype NAT disable
#define CSID_C_PPPOA_WAN1_NAT_DISABLE	_CFG_PPPOA(0x0010)	//T=u32, PPPoA wantype NAT disable, for WAN1	
// Begin of 256 account --->
#define CSID_C_PPPOA_USER           	_CFG_PPPOA(0x0100)	//T=str, user name
#define CSID_C_PPPOA_WAN1_USER			_CFG_PPPOA(0x0101)	//T=str, user name for wan1			

#define CSID_C_PPPOA_PASSWORD       	_CFG_PPPOA(0x0200)	//T=str, user password
#define CSID_C_PPPOA_WAN1_PASSWORD     	_CFG_PPPOA(0x0201)	//T=str, user password for wan1

#define CSID_C_PPPOA_IF_IP          	_CFG_PPPOA(0x0300)	//T=ipv4, static interface ip
#define CSID_C_PPPOA_WAN1_IF_IP         _CFG_PPPOA(0x0301)	//T=ipv4, static interface ip

#define CSID_C_PPPOA_SERVICENAME    	_CFG_PPPOA(0x0600)	//T=str, service name
#define CSID_C_PPPOA_WAN1_SERVICENAME   _CFG_PPPOA(0x0601)	//T=str, service name

#define CSID_C_PPPOA_IF_PRIDNS      	_CFG_PPPOA(0x0700)	//T=ipv4, static interface primary DNS
#define CSID_C_PPPOA_WAN1_IF_PRIDNS     _CFG_PPPOA(0x0701)	//T=ipv4, static interface primary DNS

#define CSID_C_PPPOA_IF_SECDNS       	_CFG_PPPOA(0x0800)	//T=ipv4, static interface secondary DNS
#define CSID_C_PPPOA_WAN1_IF_SECDNS     _CFG_PPPOA(0x0801)	//T=ipv4, static interface secondary DNS

#define CSID_C_PPPOA_CNT_TYPE        	_CFG_PPPOA(0x0900)	//T=u32, connect type, Dial-on-Demand, autoconnect...
#define CSID_C_PPPOA_WAN1_CNT_TYPE      _CFG_PPPOA(0x0901)	//T=u32, connect type, Dial-on-Demand, autoconnect...
#define CSID_C_PPPOA_CNT_TYPE_UI        _CFG_PPPOA(0x0902)	//T=u8, connect type, Dial-on-Demand, autoconnect...

#define CSID_C_PPPOA_MTU             	_CFG_PPPOA(0x0a00)	//T=u32, D=0, max transmission unit
#define CSID_C_PPPOA_WAN1_MTU          	_CFG_PPPOA(0x0a01)	//T=u32, D=0, max transmission unit

#define CSID_C_PPPOA_IDLETIME        	_CFG_PPPOA(0x0b00)	//T=u32, D=600, max idle time
#define CSID_C_PPPOA_WAN1_IDLETIME      _CFG_PPPOA(0x0b01)	//T=u32, D=600, max idle time

#define CSID_C_PPPOA_HOLDOFFTIME     	_CFG_PPPOA(0x0c00)	//T=u32, D=300, wait time before rebuild
#define CSID_C_PPPOA_WAN1_HOLDOFFTIME   _CFG_PPPOA(0x0c01)	//T=u32, D=300, wait time before rebuild

#define CSID_C_PPPOA_AUTHTYPE        	_CFG_PPPOA(0x0d00)	//T=u32, auth type
#define CSID_C_PPPOA_WAN1_AUTHTYPE      _CFG_PPPOA(0x0d01)	//T=u32, auth type

/* Multisession Items (start fromn 0x2000) */
#define CSID_C_PPPOA_ML_MASTER	     _CFG_PPPOA(0x2000) //T=u8, master session, 0: slave 1:master
#define CSID_C_PPPOA_ML_ENABLE	     _CFG_PPPOA(0x2100) //T=u8, enable this session, 0:no 1:yes

//<--- End of 256 account

/* Status Items */

// Begin of 256 account --->

#define CSID_S_PPPOA_CNT_TIME        	_STA_PPPOA(0x0100)	//T=u32, connect time
#define CSID_S_PPPOA_WAN1_CNT_TIME      _STA_PPPOA(0x0101)	//T=u32, connect time

#define CSID_S_PPPOA_IF_IP           	_STA_PPPOA(0x0200)	//T=ipv4, dynamic interface ip
#define CSID_S_PPPOA_WAN1_IF_IP         _STA_PPPOA(0x0201)	//T=ipv4, dynamic interface ip

#define CSID_S_PPPOA_IF_NM           	_STA_PPPOA(0x0300)	//T=ipv4, dynamic interface netmask
#define CSID_S_PPPOA_WAN1_IF_NM         _STA_PPPOA(0x0301)	//T=ipv4, dynamic interface netmask

#define CSID_S_PPPOA_IF_GW           	_STA_PPPOA(0x0400)	//T=ipv4, dynamic interface gateway
#define CSID_S_PPPOA_WAN1_IF_GW         _STA_PPPOA(0x0401)	//T=ipv4, dynamic interface gateway

#define CSID_S_PPPOA_IF_PRIDNS       	_STA_PPPOA(0x0500)	//T=ipv4, dynamic interface primary DNS
#define CSID_S_PPPOA_WAN1_IF_PRIDNS     _STA_PPPOA(0x0501)	//T=ipv4, dynamic interface primary DNS

#define CSID_S_PPPOA_IF_SECDNS       	_STA_PPPOA(0x0600)	//T=ipv4, dynamic interface secondary DNS
#define CSID_S_PPPOA_WAN1_IF_SECDNS     _STA_PPPOA(0x0601)	//T=ipv4, dynamic interface secondary DNS

#define CSID_S_PPPOA_CNT_STATUS      	_STA_PPPOA(0x0700)	//T=u32, connect status
#define CSID_S_PPPOA_WAN1_CNT_STATUS    _STA_PPPOA(0x0701)	//T=u32, connect status

#define CSID_S_PPPOA_PKTIN_NUM       	_STA_PPPOA(0x0800)	//T=u32, number of packet input
#define CSID_S_PPPOA_WAN1_PKTIN_NUM     _STA_PPPOA(0x0801)	//T=u32, number of packet input

#define CSID_S_PPPOA_PKTOUT_NUM      	_STA_PPPOA(0x0900)	//T=u32, number of packet output
#define CSID_S_PPPOA_WAN1_PKTOUT_NUM    _STA_PPPOA(0x0901)	//T=u32, number of packet output

#define CSID_S_PPPOA_ALTERED         	_STA_PPPOA(0x0A00)	//T=u32, ui altered
#define CSID_S_PPPOA_WAN1_ALTERED       _STA_PPPOA(0x0A01)	//T=u32, ui altered


//<--- End of 256 account

/* Multisession Items (start fromn 0x2000) */
#define CSID_S_PPPOA_ML_ALTERED      _STA_PPPOA(0x2000)	//T=u8, ui altered

//<--- End of 256 account

#endif //ifndef _CSID_PPPOE_H
