/* vi: set sw=4 ts=4: */

/*
 * @file Member ID Definition of Group VLANDHCP. 
 *
 * VLANDHCP Sever (VLANDHCPD)
 *
 * Moderator:
 * Group ID: 0x00930000/0x80930000
 *
 * // Configuration Items   (64K)
 *   // VLANDHCP Daemon 0x0001 ~ 0x4FFF
 *   // VLANDHCP Client 0x5001 ~ 0x6FFF
 *   // VLANDHCP Probe  0x7001 ~ 0x7FFF
 *   // Reserved    0x8000 ~ 0xFFFF
 * // Status Items			(64K)
 *   // VLANDHCP Daemon 0x0001 ~ 0x4FFF
 *   // VLANDHCP Client 0x5001 ~ 0x6FFF
 *   // DHCP Probe  0x7001 ~ 0x7FFF
 *   // Reserved    0x8000 ~ 0xFFFF
 */

#ifndef _CSID_VLAN_H
#define _CSID_VLAN_H

#include "csid_gid.h"

#define _CFG_VLAN(x)		(CSID_GIDC_VLAN|(x))
#define _STA_VLAN(x)		(CSID_GIDS_VLAN|(x))

#define VLAN_SET_UNTAG 0
#define VLAN_SET_TAG 1

#define LAN_SET_NAT     0
#define LAN_SET_VLAN    1
#define LAN_SET_BRIDGE  2

/* Configuration Items */

/* DHCP Daemon */
/* DHCP Server (DHCPD) Basic Setting */
#define CSID_C_VLAN_ENABLE              	_CFG_VLAN(0x0001)   //T=u32, D=0, Enable/Disable
#define CSID_C_VLAN_SVR_ENABLE				_CFG_VLAN(0x0010)	//T=u32, D=0, Enable/Disable
#define CSID_C_VLAN_SVR_LEASETIME			_CFG_VLAN(0x0020)	//T=u32, D=86400, Lease Time (seconds)
#define CSID_C_VLAN_SVR_IPPOOL_START		_CFG_VLAN(0x0030)	//T=ipv4, IP Pool Starting Address
#define CSID_C_VLAN_SVR_IPPOOL_END			_CFG_VLAN(0x0040)	//T=ipv4, IP Pool Ending Address
#define CSID_C_VLAN_SVR_DOMAIN				_CFG_VLAN(0x0050)	//T=str, Domain Name
#define CSID_C_VLAN_SVR_PRIDNS				_CFG_VLAN(0x0060)	//T=ipv4, Primary DNS  
#define CSID_C_VLAN_SVR_SECDNS				_CFG_VLAN(0x0070)	//T=ipv4, Second DNS  
#define CSID_C_VLAN_SVR_PRIWINS				_CFG_VLAN(0x0080)	//T=ipv4, Primary WINS  
#define CSID_C_VLAN_SVR_SECWINS				_CFG_VLAN(0x0090)	//T=ipv4, Second WINS
#define CSID_C_VLAN_SVR_GATEWAY				_CFG_VLAN(0x00a0)	//T=ipv4, Gateway default is LAN ip
#define CSID_C_VLAN_SVR_MAXLEASE			_CFG_VLAN(0x00b0)	//T=u32, D=101, the Max. number of lease ip 
#define CSID_C_VLAN_SVR_IFNAME				_CFG_VLAN(0x00c0)	//T=str, ifname
#define CSID_C_VLAN_SVR_IP           		_CFG_VLAN(0x00d0)	//T=ipv4, IP Address
#define CSID_C_VLAN_NAT_ENABLE				_CFG_VLAN(0x00e0)	//T=u32, D=1, Enable/Disable


#define CSID_C_VLAN_START_PORT				_CFG_VLAN(0x00f0)	//T=u32, D=4, PORT 1,2,3......

#define CSID_C_VLAN_BLOCK0_ENABLE			_CFG_VLAN(0x0100)	//T=u32, D=0, 0:Enable/1:Disable other lan to access to br0
#define CSID_C_VLAN_BLOCK_ENABLE			_CFG_VLAN(0x0101)	//T=u32, D=0, 0:Enable/1:Disable other lan access to this subnet
#define CSID_C_VLAN_UI_DISABLE				_CFG_VLAN(0x0110)	//T=u32, D=0, 0:Enable/1:Disable access to UI
#define CSID_C_VLAN_SVR_MASK           		_CFG_VLAN(0x0120)	//T=ipv4, D={255.255.255.0}, vlan subnet mask
#define CSID_C_VLAN_SVR_NAME				_CFG_VLAN(0x0130)	//T=str, Server Name


/* Customize for ZyXEL */
#define CSID_C_VLAN_SVR_PRIDNS_TYPE			_CFG_VLAN(0x0200)	//T=u8, D=0, Primary DNS type
#define CSID_C_VLAN_SVR_SECDNS_TYPE			_CFG_VLAN(0x0210)	//T=u8, D=0, Secondary DNS type
 
/* Fixed Mapping */
#define CSID_C_VLAN_SVR_FIXMAP_ENABLE		_CFG_VLAN(0x1000)	//T=u8, D=0, Enable/Disable
#define CSID_C_VLAN_SVR_FIXMAP_MAC			_CFG_VLAN(0x1300)	//T=mac, Mapped MAC
#define CSID_C_VLAN_SVR_FIXMAP_IP			_CFG_VLAN(0x1600)	//T=ipv4, Mapped IP
/* Basic Setting end */

/* VLAN TAG */
#define CSID_C_VLAN_TAG_ENABLE				_CFG_VLAN(0x2000)	//T=u8, D=0, Disable/Enable

#define CSID_C_VLAN_PORT1_WAN_PVID			_CFG_VLAN(0x2001)	//T=u32, D=0, PORT1 WAN Tag
#define CSID_C_VLAN_PORT2_WAN_PVID			_CFG_VLAN(0x2002)	//T=u32, D=0, PORT2 WAN Tag
#define CSID_C_VLAN_PORT3_WAN_PVID			_CFG_VLAN(0x2003)	//T=u32, D=0, PORT3 WAN Tag
#define CSID_C_VLAN_PORT4_WAN_PVID			_CFG_VLAN(0x2004)	//T=u32, D=0, PORT4
#define CSID_C_VLAN_PORT5_WAN_PVID			_CFG_VLAN(0x2005)	//T=u32, D=0, PORT5

#define CSID_C_VLAN_PORT1_LAN_PVID  		_CFG_VLAN(0x2011)	//T=u32, D=1, PORT1 LAN side VLAN tag
#define CSID_C_VLAN_PORT2_LAN_PVID  		_CFG_VLAN(0x2012)	//T=u32, D=1, PORT2
#define CSID_C_VLAN_PORT3_LAN_PVID  		_CFG_VLAN(0x2013)	//T=u32, D=1, PORT3
#define CSID_C_VLAN_PORT4_LAN_PVID  		_CFG_VLAN(0x2014)	//T=u32, D=1, PORT4
#define CSID_C_VLAN_PORT5_LAN_PVID  		_CFG_VLAN(0x2015)	//T=u32, D=1, PORT5

#define CSID_C_VLAN_VOIPWAN_TAG_ENABLE		_CFG_VLAN(0x201f)	//T=u8, D=0, Disable

/* Reserved 16 items for VoIP-WAN PVID */
#define CSID_C_VLAN_VOIPWAN_PVID			_CFG_VLAN(0x2020)	//T=u8, VoIP WAN PVID


#define CSID_C_VLAN_PORT1_VLAN              _CFG_VLAN(0x2031)   //T=u8, D=0, 0:NAT 1:VLAN(Bridge)
#define CSID_C_VLAN_PORT2_VLAN              _CFG_VLAN(0x2032)   //T=u8, D=0, NAT
#define CSID_C_VLAN_PORT3_VLAN              _CFG_VLAN(0x2033)   //T=u8, D=0, NAT
#define CSID_C_VLAN_PORT4_VLAN              _CFG_VLAN(0x2034)   //T=u8, D=0, NAT
#define CSID_C_VLAN_PORT5_VLAN              _CFG_VLAN(0x2035)   //T=u8, D=0, NAT
#define CSID_C_VLAN_PORT6_VLAN              _CFG_VLAN(0x2036)   //T=u8, D=0, NAT


#define CSID_C_VLAN_PHY_PORT1               _CFG_VLAN(0x2041)   //T=u8, D=0, 0:wan 1:lan
#define CSID_C_VLAN_PHY_PORT2               _CFG_VLAN(0x2042)   //T=u8, D=1, 0:wan 1:lan
#define CSID_C_VLAN_PHY_PORT3               _CFG_VLAN(0x2043)   //T=u8, D=1, 0:wan 1:lan
#define CSID_C_VLAN_PHY_PORT4               _CFG_VLAN(0x2044)   //T=u8, D=1, 0"wan 1:lan
#define CSID_C_VLAN_PHY_PORT5               _CFG_VLAN(0x2045)   //T=u8, D=1, 0:wan 1:lan
#define CSID_C_VLAN_PHY_PORT6               _CFG_VLAN(0x2046)   //T=u8, D=1, 0:wan 1:lan

#define CSID_C_VLAN_PORT1_BIND              _CFG_VLAN(0x2051)   //T=u8, D=1, bind/bridge to wan x , x=1~MAX_PORT.
#define CSID_C_VLAN_PORT2_BIND              _CFG_VLAN(0x2052)   //T=u8, D=1, bind/bridge to wan x , x=1~MAX_PORT.
#define CSID_C_VLAN_PORT3_BIND              _CFG_VLAN(0x2053)   //T=u8, D=1, bind/bridge to wan x , x=1~MAX_PORT.
#define CSID_C_VLAN_PORT4_BIND              _CFG_VLAN(0x2054)   //T=u8, D=1, bind/bridge to wan x , x=1~MAX_PORT.
#define CSID_C_VLAN_PORT5_BIND              _CFG_VLAN(0x2055)   //T=u8, D=1, bind/bridge to wan x , x=1~MAX_PORT.
#define CSID_C_VLAN_PORT6_BIND              _CFG_VLAN(0x2056)   //T=u8, D=1, bind/bridge to wan x , x=1~MAX_PORT.


#define CSID_C_VLAN_PORT1_LAN_GID           _CFG_VLAN(0x2061)   //T=u8, D=0, if lan is nat port, choice it's DHCP Server by this csid
#define CSID_C_VLAN_PORT2_LAN_GID           _CFG_VLAN(0x2062)   //T=u8, D=0, if lan is nat port, choice it's DHCP Server by this csid
#define CSID_C_VLAN_PORT3_LAN_GID           _CFG_VLAN(0x2063)   //T=u8, D=0, if lan is nat port, choice it's DHCP Server by this csid
#define CSID_C_VLAN_PORT4_LAN_GID           _CFG_VLAN(0x2064)   //T=u8, D=0, if lan is nat port, choice it's DHCP Server by this csid
#define CSID_C_VLAN_PORT5_LAN_GID           _CFG_VLAN(0x2065)   //T=u8, D=0, if lan is nat port, choice it's DHCP Server by this csid
#define CSID_C_VLAN_PORT6_LAN_GID           _CFG_VLAN(0x2066)   //T=u8, D=0, if lan is nat port, choice it's DHCP Server by this csid


#define CSID_C_VLAN_PORT_VID			    _CFG_VLAN(0x2071)	//T=u8, D=0, VID of Port1
// reserve 16 space for VID
#define CSID_C_VLAN_PVC_VID_TAG_ENABLE	    _CFG_VLAN(0x2081)	//T=u8, D=0, enable tagging VID of PVC11
#define CSID_C_VLAN_PVC_VID				    _CFG_VLAN(0x2091)	//T=u16, D=1, VID of PVC1
// reserve 16 space for VID
#define CSID_C_VLAN_AP_VID_TAG_ENABLE		_CFG_VLAN(0x20a1)	//T=u8, D=0, enable tagging VID of wireless interface
#define CSID_C_VLAN_AP_VID					_CFG_VLAN(0x20b1)	//T=u16, D=1, VID of Wireless interface
// reserve 16 space for VID
#define CSID_C_VLAN_PORT1_TAG				_CFG_VLAN(0x20c1)	//T=u8, D=0, 0:UNTAG 1: TAG
#define CSID_C_VLAN_PORT2_TAG				_CFG_VLAN(0x20c2)	//T=u8, D=0, UNTAG/TAG
#define CSID_C_VLAN_PORT3_TAG				_CFG_VLAN(0x20c3)	//T=u8, D=0, UNTAG/TAG
#define CSID_C_VLAN_PORT4_TAG				_CFG_VLAN(0x20c4)	//T=u8, D=0, UNTAG/TAG
#define CSID_C_VLAN_PORT5_TAG				_CFG_VLAN(0x20c5)	//T=u8, D=0, UNTAG/TAG

/*AP VLAN settings, every item reserve 16 spaces*/
#define CSID_C_VLAN_AP_LAN_PVID				_CFG_VLAN(0x2100)	//T=u8, D=1, LAN PVID for AP
#define CSID_C_VLAN_AP_WAN_PVID				_CFG_VLAN(0x2110)	//T=u8, D=0, WAN PVID for AP
#define CSID_C_VLAN_AP_TAG					_CFG_VLAN(0x2120)	//T=u8, D=0, Tx TAG TAG/UNTAG
#define CSID_C_VLAN_AP_GID					_CFG_VLAN(0x2130)	//T=u8, D=0, for DHCP Select
#define CSID_C_VLAN_AP_BIND					_CFG_VLAN(0x2140)	//T=u8, D=0, Bind to WANx
#define CSID_C_VLAN_AP_VLAN              	_CFG_VLAN(0x2150)   //T=u8, D=0, 0:NAT 1:VLAN(Bridge)
/********************/
/*AP VLAN settings, every item reserve 16 spaces*/
#define CSID_C_VLAN_AP2_LAN_PVID			_CFG_VLAN(0x2160)	//T=u8, D=1, LAN PVID for AP2
#define CSID_C_VLAN_AP2_WAN_PVID			_CFG_VLAN(0x2170)	//T=u8, D=0, WAN PVID for AP2
#define CSID_C_VLAN_AP2_TAG					_CFG_VLAN(0x2180)	//T=u8, D=0, Tx TAG TAG/UNTAG
#define CSID_C_VLAN_AP2_GID					_CFG_VLAN(0x2190)	//T=u8, D=0, for DHCP Select
#define CSID_C_VLAN_AP2_BIND				_CFG_VLAN(0x21a0)	//T=u8, D=0, Bind to WANx
#define CSID_C_VLAN_AP2_VLAN              	_CFG_VLAN(0x21b0)   //T=u8, D=0, 0:NAT 1:VLAN(Bridge)
/********************/

/*VLAN Table (VLAN Member)*/
#define CSID_C_VLAN_TABLE_ENABLE			_CFG_VLAN(0x2200)	//T=u8, D=0, VLAN TABLE setting enable/disable
#define CSID_C_VLAN_TABLE_ID			    _CFG_VLAN(0x2201)	//T=u32, D=0, VLAN ID
//reserve 32 space
#define CSID_C_VLAN_TABLE_MEMBER			_CFG_VLAN(0x2221)	//T=u32, D=0, VLAN MEMBER bit0 assign to port 0
//reserve 32 space
/****************/

#define CSID_C_VLAN_GROUP_TABLE             _CFG_VLAN(0x22a0)   //T=u32, D=0, 
#define CSID_C_VLAN_GROUP_ID                _CFG_VLAN(0x2300)   //T=u32, D=0, 
#define CSID_C_VLAN_LAN_MEMBER              _CFG_VLAN(0x2320)   //T=u32, D=0, VLAN MEMBER bit0 assign to port 1
#define CSID_C_VLAN_AP_MEMBER               _CFG_VLAN(0x2340)   //T=u32, D=0, VLAN MEMBER bit0 assign to ap1
#define CSID_C_VLAN_AP2_MEMBER              _CFG_VLAN(0x2360)   //T=u32, D=0, VLAN MEMBER bit0 assign to ap2
#define CSID_C_VLAN_DENY_WAN                _CFG_VLAN(0x2380)   //T=u32, D=0, 


#define CSID_C_VLAN_TAGBASE_VID             _CFG_VLAN(0x2400)   //T=u32, D=0, 
#define CSID_C_VLAN_TAGBASE_LAN_MEMBER      _CFG_VLAN(0x2500)   //T=u32, D=0, VLAN MEMBER bit0 assign to port 1
#define CSID_C_VLAN_TAGBASE_AP_MEMBER       _CFG_VLAN(0x2600)   //T=u32, D=0, VLAN MEMBER bit0 assign to ap2
#define CSID_C_VLAN_TAGBASE_AP2_MEMBER      _CFG_VLAN(0x2700)   //T=u32, D=0, VLAN MEMBER bit0 assign to ap1
#define CSID_C_VLAN_TAGBASE_SUBNET          _CFG_VLAN(0x2800)   //T=u32, D=0, subnet select
#define CSID_C_VLAN_TAGBASE_DENY_INT        _CFG_VLAN(0x2900)   //T=u32, D=0, deny internet access
#define CSID_C_VLAN_MODE                    _CFG_VLAN(0x2a00)   //T=u32, D=0, 0:port_base, 1: tagbase 
#define CSID_C_VLAN_TAGBASE_NAME            _CFG_VLAN(0x2a01)   //T=str, table name for Zyxel
#define CSID_C_VLAN_TAGBASE_8021P           _CFG_VLAN(0x2a10)   //T=u32, d=0, 8021p
#define CSID_C_VLAN_TAGBASE_PROFILE_ID      _CFG_VLAN(0x2a20)   //T=u32, d=0 , for Zyxel UI

#define CSID_C_VLAN_SVR_WAN_BIND            _CFG_VLAN(0x2a30)   //T=u32, d=0 ,bind WAN for Zyxel UI

#define CSID_C_VLAN_ARRANGEMENT			    _CFG_VLAN(0x3001)	//T=u32, D=0, 0:normal 1:reverse.
//VLAN PRI occupy 16~32 space
#define CSID_C_VLAN_LOCAL_PRIORITY          _CFG_VLAN(0x3010)   //T=u32, D=0, 802.1p.
#define CSID_C_VLAN_PRIORITY                _CFG_VLAN(0x3011)   //T=u32, D=0, 802.1p.
#define CSID_C_VLAN_VOIPWAN_PRIORITY        _CFG_VLAN(0x3021)   //T=u32, D=0, 802.1p.

//wan member control for CDW57CAM-003
#define CSID_C_VLAN_WAN_MEMBER_ENABLE       _CFG_VLAN(0x3040)   //T=u8, D=0, enable lan port member with wan control
#define CSID_C_VLAN_WAN_MEMBER_PORT         _CFG_VLAN(0x3041)   //T=u8, D=1, determine which port member with wan

#define CSID_C_VLAN_GROUP_ACTIVE            _CFG_VLAN(0x3042)   //T=u8, D=1, indicate active vlan group 

#define CSID_C_VLAN_WAN_FIX                 _CFG_VLAN(0x4001)   //T=u32, D=0, Set 1 to fix WAN

#define CSID_C_VLAN_PORT1_BR                _CFG_VLAN(0x4011)   //T=u8, D=0, bridge to wan x 
#define CSID_C_VLAN_PORT2_BR                _CFG_VLAN(0x4012)   //T=u8, D=0, bridge to wan x 
#define CSID_C_VLAN_PORT3_BR                _CFG_VLAN(0x4013)   //T=u8, D=0, bridge to wan x 
#define CSID_C_VLAN_PORT4_BR                _CFG_VLAN(0x4014)   //T=u8, D=0, bridge to wan x 
#define CSID_C_VLAN_PORT5_BR                _CFG_VLAN(0x4015)   //T=u8, D=0, bridge to wan x 
#define CSID_C_VLAN_PORT6_BR                _CFG_VLAN(0x4016)   //T=u8, D=0, bridge to wan x 

/*IO DATA BX-VP1 port control*/
#define CSID_C_VLAN_BXVP1_PORT_EANBLE       _CFG_VLAN(0x4100)   //T=u32, D=0, BX VP1 EEE LAN/WAN enable.

//VLAN port-base setting
#define CSID_C_VLAN_NAME                    _CFG_VLAN(0x4200)   //T=str, Vlan Name
#define CSID_C_VLAN_PORTBASE_VID            _CFG_VLAN(0x4210)   //T=u32, Vlan ID
#define CSID_C_VLAN_PORTBASE_TAG            _CFG_VLAN(0x4220)   //T=u32, Vlan lan side Tx Tag or Untag
#define CSID_C_VLAN_PORTBASE_VLAN           _CFG_VLAN(0x4230)   //T=u32, D=0, 0:NAT 1:VLAN(Bridge) 
#define CSID_C_VLAN_PORTBASE_PORT_MEMBER    _CFG_VLAN(0x4240)   //T=u32, Port vlan member
#define CSID_C_VLAN_PORTBASE_AP_MEMBER      _CFG_VLAN(0x4250)   //T=u32, Vap vlan member 
#define CSID_C_VLAN_PORTBASE_AP2_MEMBER     _CFG_VLAN(0x4260)   //T=u32, Vap2 vlan member
#define CSID_C_VLAN_PORTBASE_BIND           _CFG_VLAN(0x4270)   //T=u32, D=0, bridge to wan x 
#define CSID_C_VLAN_PORTBASE_WAN_VID        _CFG_VLAN(0x4280)   //T=u32, D=0, WAN Tag
#define CSID_C_VLAN_PORTBASE_RULE_ENABLE    _CFG_VLAN(0x4290)   //T=u32, D=1,0:Disable/1:Enable

/* VLANDHCP Daemon */
#define CSID_S_VLAN_SVR_PID					_STA_VLAN(0x0010)	//T=u16, The PID of VLANDHCP Server
#define CSID_S_VLAN_SVR_ALTERED			    _STA_VLAN(0x0020)	//T=u8, ui altered
#define CSID_S_VLAN_SVR_DNS			        _STA_VLAN(0x0030)	//T=u8, if assign lan ip as dns
#define CSID_S_VLAN_ADSL_IF	              	_STA_VLAN(0x0040)	//T=str, The interface name for ADSL sync
#define CSID_S_VLAN_AP_BR					_STA_VLAN(0x0050)	//T=str, The bridge interface for AP
#define CSID_S_VLAN_AP2_BR					_STA_VLAN(0x0060)	//T=str, The bridge interface for AP2
//reserve 16 items for ap's bridge.
#define CSID_S_VLAN_ALTERED					_STA_VLAN(0x0070)	//T=u8, VLAN ALTER
#define CSID_S_VLAN_MEM_RESET				_STA_VLAN(0x0071)	//T=u8, VLAN TABLE reset to default

/* Client-List, 256 items are reserved */
#define CSID_S_VLAN_SVR_CLILIST_IP			_STA_VLAN(0x1001)	//T=ipv4, The IP of the Client in Client List
#define CSID_S_VLAN_SVR_CLILIST_HOSTNAME	_STA_VLAN(0x2001)	//T=str, The hostname of the Client in the Client List
#define CSID_S_VLAN_SVR_CLILIST_MAC			_STA_VLAN(0x3001)	//T=mac, The Mac Address of the Client in Client List
#define CSID_S_VLAN_SVR_CLILIST_LEASETIME	_STA_VLAN(0x4001)	//T=u32, remaining lease time
#define CSID_S_VLAN_SVR_CLILIST_TYPE		_STA_VLAN(0x5001)	//T=u8, wired or wireless mode
#define CSID_S_VLAN_SVR_CLILIST_WLAN		_STA_VLAN(0x6001)	//T=str, backup wireless client for speical issue
/* Client-List end */




#endif //ifndef _CSID_VLAN_H
