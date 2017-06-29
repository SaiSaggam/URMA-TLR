/*
 * @file Member ID Definition of Group PPPOE.
 *
 * Moderator: GeorgeWang
 * Group ID: 0x00040000/0x80040000
 */

#ifndef _CSID_PPPOE_H
#define _CSID_PPPOE_H

#include "csid_gid.h"

#define _CFG_PPPOE(x)		(CSID_GIDC_PPPOE|(x))
#define _STA_PPPOE(x)		(CSID_GIDS_PPPOE|(x))

/* Extra Definitions */

#define PPPOE_CNT_TYPE_ONDEMAND         0x00
#define PPPOE_CNT_TYPE_AUTORECONNECT    0x01
#define PPPOE_CNT_TYPE_MANUAL           0x02

#define PPPOE_AUTHTYPE_AUTO             0x00
#define PPPOE_AUTHTYPE_PAP              0x01
#define PPPOE_AUTHTYPE_CHAP             0x02
#define PPPOE_AUTHTYPE_MSCHAP           0x04
#define PPPOE_AUTHTYPE_MSCHAPV2         0x08
#define PPPOE_AUTHTYPE_EAP              0x10

#define PPPOE_CNT_STATUS_DISCONNECT     0x00
#define PPPOE_CNT_STATUS_CONNECTING     0x01
#define PPPOE_CNT_STATUS_CONNECT        0x02
#define PPPOE_CNT_STATUS_DISCONNECTING  0x03
#define PPPOE_CNT_STATUS_WAIT_TRAFFIC	0x05
#define PPPOE_CNT_STATUS_AUTH_FAIL		0x06

/* Configuration Items */
#define CSID_C_PPPOE_NAT_DISABLE     	    _CFG_PPPOE(0x000f)	//T=u32, PPPoE wantype NAT disable
#define CSID_C_PPPOE_MULTIWAN_NAT_DISABLE	_CFG_PPPOE(0x0010)	//T=u32, PPPoE wantype NAT disable, for MULTIWAN
#define CSID_C_PPPOE_RELAY_ENABLE		    _CFG_PPPOE(0x0020)	//T=u32, D=0, PPPoE Relay enable 0:disable, 1:enable
#define CSID_C_PPPOE_VOIPWAN_NAT_ENABLE		_CFG_PPPOE(0x0021)	//T=u32, D=0, PPPoE wantype NAT disable, for VoIP WAN
#define CSID_C_PPPOE_LCS_ENABLE		        _CFG_PPPOE(0x0022)	//T=u8,  D=0, PPPoE for LCS Server enable/disable
#define CSID_C_PPPOE_LCS_SERVER		        _CFG_PPPOE(0x0023)	//T=str, PPPoE for LCS Server enable/disable
#define CSID_C_PPPOE_LCS_NAT_DISABLE		_CFG_PPPOE(0x0024)	//T=u8,  Enable/Disable NAT for LCS PPPoE 
#define CSID_C_PPPOE_LCS_CHECK		        _CFG_PPPOE(0x0025)	//T=u8,  Enable/Disable to Do LCS Server check
#define CSID_C_PPPOE_PADI_PACKETS	        _CFG_PPPOE(0x0026)	//T=u8,  PPPoE Discover sendPADI packets
#define CSID_C_PPPOE_IPV6_PASSTHROUGH	    _CFG_PPPOE(0x0027)	//T=u32, Enable/Disable IPv6 Passthrought for PPPoE

/* 16 Rules for LCS Policy Routing */
#define CSID_C_PPPOE_LCS_PRT_ENABLE	        _CFG_PPPOE(0x0030)	//T=u8,  Enable/Disable LCS PPPoE Policy Routing
#define CSID_C_PPPOE_LCS_PRT_ADDR		    _CFG_PPPOE(0x0040)	//T=str, LCS PPPoE Policy Routing Address

// Begin of 256 account --->
/* Reserved 16 items for VoIP-WAN */
/* Reserved  1 items for LCS PPPoE*/
#define CSID_C_PPPOE_USER           	    _CFG_PPPOE(0x0100)	//T=str, user name
#define CSID_C_PPPOE_MULTIWAN_USER			_CFG_PPPOE(0x0101)	//T=str, user name for wan1			
#define CSID_C_PPPOE_VOIPWAN_USER			_CFG_PPPOE(0x0111)	//T=str, user name for VoIP WAN			
#define CSID_C_PPPOE_LCS_USER			    _CFG_PPPOE(0x0121)	//T=str, user name for LCS PPPoE 
#define CSID_C_PPPOE_PEER_ETH               _CFG_PPPOE(0x0131)	//T=mac, Peer's MAC address
#define CSID_C_PPPOE_SESSION                _CFG_PPPOE(0x0141)	//T=u32, Session ID

#define CSID_C_PPPOE_PASSWORD       	    _CFG_PPPOE(0x0200)	//T=str, user password
#define CSID_C_PPPOE_MULTIWAN_PASSWORD     	_CFG_PPPOE(0x0201)	//T=str, user password for wan1
#define CSID_C_PPPOE_VOIPWAN_PASSWORD     	_CFG_PPPOE(0x0211)	//T=str, user password for VoIP WAN
#define CSID_C_PPPOE_LCS_PASSWORD     	    _CFG_PPPOE(0x0221)	//T=str, user password for VoIP WAN

#define CSID_C_PPPOE_IF_IP          	    _CFG_PPPOE(0x0300)	//T=ipv4, static interface ip
#define CSID_C_PPPOE_MULTIWAN_IF_IP         _CFG_PPPOE(0x0301)	//T=ipv4, static interface ip
#define CSID_C_PPPOE_VOIPWAN_IF_IP          _CFG_PPPOE(0x0311)	//T=ipv4, static interface ip VoIP WAN

#define CSID_C_PPPOE_SERVICENAME    	    _CFG_PPPOE(0x0600)	//T=str, service name
#define CSID_C_PPPOE_MULTIWAN_SERVICENAME   _CFG_PPPOE(0x0601)	//T=str, service name
#define CSID_C_PPPOE_VOIPWAN_SERVICENAME    _CFG_PPPOE(0x0611)	//T=str, service name for VoIP WAN
#define CSID_C_PPPOE_LCS_SERVICENAME        _CFG_PPPOE(0x0621)	//T=str, service name for LCS PPPoE

#define CSID_C_PPPOE_IF_PRIDNS      	    _CFG_PPPOE(0x0700)	//T=ipv4, static interface primary DNS
#define CSID_C_PPPOE_MULTIWAN_IF_PRIDNS     _CFG_PPPOE(0x0701)	//T=ipv4, static interface primary DNS
#define CSID_C_PPPOE_VOIPWAN_IF_PRIDNS      _CFG_PPPOE(0x0711)	//T=ipv4, static interface primary DNS for VoIP WAN
#define CSID_C_PPPOE_LCS_IF_PRIDNS          _CFG_PPPOE(0x0721)	//T=ipv4, static interface primary DNS for LCS PPPoE

#define CSID_C_PPPOE_IF_SECDNS       	    _CFG_PPPOE(0x0800)	//T=ipv4, static interface secondary DNS
#define CSID_C_PPPOE_MULTIWAN_IF_SECDNS     _CFG_PPPOE(0x0801)	//T=ipv4, static interface secondary DNS
#define CSID_C_PPPOE_VOIPWAN_IF_SECDNS      _CFG_PPPOE(0x0811)	//T=ipv4, static interface secondary DNS for VoIP WAN
#define CSID_C_PPPOE_LCS_IF_SECDNS          _CFG_PPPOE(0x0821)	//T=ipv4, static interface secondary DNS for LCS PPPoE

#define CSID_C_PPPOE_CNT_TYPE        	    _CFG_PPPOE(0x0900)	//T=u32, connect type, Dial-on-Demand, autoconnect...
#define CSID_C_PPPOE_MULTIWAN_CNT_TYPE      _CFG_PPPOE(0x0901)	//T=u32, connect type, Dial-on-Demand, autoconnect...
#define CSID_C_PPPOE_VOIPWAN_CNT_TYPE       _CFG_PPPOE(0x0911)	//T=u32, connect type, Dial-on-Demand, autoconnect... for VoIP WAN
#define CSID_C_PPPOE_CNT_TYPE_UI            _CFG_PPPOE(0x0912)	//T=u8,  connect type, Dial-on-Demand, autoconnect... for LCS PPPoE
#define CSID_C_PPPOE_LCS_CNT_TYPE           _CFG_PPPOE(0x0921)	//T=u8,  connect type, Dial-on-Demand, autoconnect... 
#define CSID_C_PPPOE_MULTIWAN_CNT_TYPE_UI   _CFG_PPPOE(0x0940)  //T=u32, connect type, Dial-on-Demand, autoconnect...

#define CSID_C_PPPOE_MTU             	    _CFG_PPPOE(0x0a00)	//T=u32, D=0, max transmission unit
#define CSID_C_PPPOE_MULTIWAN_MTU          	_CFG_PPPOE(0x0a01)	//T=u32, D=0, max transmission unit
#define CSID_C_PPPOE_VOIPWAN_MTU          	_CFG_PPPOE(0x0a11)	//T=u32, D=0, max transmission unit for VoIP WAN
#define CSID_C_PPPOE_LCS_MTU          	    _CFG_PPPOE(0x0a21)	//T=u32, D=0, max transmission unit for LCS PPPoE

#define CSID_C_PPPOE_IDLETIME        	    _CFG_PPPOE(0x0b00)	//T=u32, D=600, max idle time
#define CSID_C_PPPOE_MULTIWAN_IDLETIME      _CFG_PPPOE(0x0b01)	//T=u32, D=600, max idle time
#define CSID_C_PPPOE_VOIPWAN_IDLETIME       _CFG_PPPOE(0x0b11)	//T=u32, D=600, max idle time for VoIP WAN
#define CSID_C_PPPOE_LCS_IDLETIME           _CFG_PPPOE(0x0b21)	//T=u32, D=600, max idle time for LCS PPPoE, 0 for disable

#define CSID_C_PPPOE_HOLDOFFTIME     	    _CFG_PPPOE(0x0c00)	//T=u32, D=300, wait time before rebuild
#define CSID_C_PPPOE_MULTIWAN_HOLDOFFTIME   _CFG_PPPOE(0x0c01)	//T=u32, D=300, wait time before rebuild
#define CSID_C_PPPOE_VOIPWAN_HOLDOFFTIME    _CFG_PPPOE(0x0c11)	//T=u32, D=300, wait time before rebuild for VoIP WAN

#define CSID_C_PPPOE_AUTHTYPE        	    _CFG_PPPOE(0x0d00)	//T=u32, auth type
#define CSID_C_PPPOE_MULTIWAN_AUTHTYPE      _CFG_PPPOE(0x0d01)	//T=u32, auth type
#define CSID_C_PPPOE_VOIPWAN_AUTHTYPE       _CFG_PPPOE(0x0d11)	//T=u32, auth type for VoIP WAN

#define CSID_C_PPPOE_KA_LCP_INTERVAL   			_CFG_PPPOE(0x0e00) //T=u16, D=20, lcp interval for pppoe keep alive
#define CSID_C_PPPOE_MULTIWAN_KA_LCP_INTERVAL   _CFG_PPPOE(0x0e01) //T=u16, D=20, lcp interval for pppoe keep alive
#define CSID_C_PPPOE_VOIPWAN_KA_LCP_INTERVAL    _CFG_PPPOE(0x0e11) //T=u16, D=20, lcp interval for pppoe keep alive for VoIP WAN
#define CSID_C_PPPOE_LCS_KA_LCP_INTERVAL        _CFG_PPPOE(0x0e21) //T=u16, D=20, lcp interval for pppoe keep alive for LCS PPPoE

#define CSID_C_PPPOE_KA_LCP_TIMES               _CFG_PPPOE(0x0f00) //T=u16, D=9, lcp times for pppoe keep alive
#define CSID_C_PPPOE_MULTIWAN_KA_LCP_TIMES      _CFG_PPPOE(0x0f01) //T=u16, D=9, lcp times for pppoe keep alive
#define CSID_C_PPPOE_VOIPWAN_KA_LCP_TIMES       _CFG_PPPOE(0x0f11) //T=u16, D=9, lcp times for pppoe keep alive for VoIP WAN
#define CSID_C_PPPOE_LCS_KA_LCP_TIMES           _CFG_PPPOE(0x0f21) //T=u16, D=9, lcp times for pppoe keep alive for LCS PPPoE

/* Multisession Items (start fromn 0x2000) */
#define CSID_C_PPPOE_ML_MASTER	            _CFG_PPPOE(0x2000) //T=u8, master session, 0: slave 1:master
#define CSID_C_PPPOE_ML_ENABLE	            _CFG_PPPOE(0x2100) //T=u8, enable this session, 0:no 1:yes

/*pppoe duall access*/
#define CSID_C_PPPOE_DUAL_ACCESS            	_CFG_PPPOE(0x3000) //T=u8, dual access control 0:disable, 1: dynamic, 2 :static
#define CSID_C_PPPOE_DUAL_ACCESS_FIXED_IP       _CFG_PPPOE(0x3100) //T=ipv4, dual access fixed ip address
#define CSID_C_PPPOE_DUAL_ACCESS_FIXED_NM	    _CFG_PPPOE(0x3200) //T=ipv4, dual access fixed netmask
#define CSID_C_PPPOE_DUAL_ACCESS_FIXED_GW	    _CFG_PPPOE(0x3300) //T=ipv4, dual access fixed getway ip address
#define CSID_C_PPPOE_DUAL_ACCESS_FIXED_PRIDNS	_CFG_PPPOE(0x3400) //T=ipv4, dual access fixed primary DNS
#define CSID_C_PPPOE_DUAL_ACCESS_FIXED_SECDNS	_CFG_PPPOE(0x3500) //T=ipv4, dual access fixed secondary DNS
//<--- End of 256 account

/* pppoe unnumbered ip */
#define CSID_C_PPPOE_UNNUMBERED_ENABLE        _CFG_PPPOE(0x3600) //T=u8, pppoe unnumbered ip enable
#define CSID_C_PPPOE_UNNUMBERED_IP            _CFG_PPPOE(0x3700) //T=ipv4, pppoe unnumbered ip address
#define CSID_C_PPPOE_UNNUMBERED_NETMASK       _CFG_PPPOE(0x3800) //T=ipv4, pppoe unnumbered ip netmask

#define CSID_C_PPPOE_DUAL_ACCESS_NEW          _CFG_PPPOE(0x3900) //T=u8, dual access fixed secondary DNS

/* Status Items */
// Begin of 256 account --->
#define CSID_S_PPPOE_ACNAME    	            _STA_PPPOE(0x0001)	//T=str, ac name
#define CSID_S_PPPOE_LCS_ACNAME             _STA_PPPOE(0x0002)	//T=str, ac name for LCS PPPoE 

#define CSID_S_PPPOE_CNT_TIME        	    _STA_PPPOE(0x0100)	//T=u32, connect time
#define CSID_S_PPPOE_MULTIWAN_CNT_TIME      _STA_PPPOE(0x0101)	//T=u32, connect time
#define CSID_S_PPPOE_VOIPWAN_CNT_TIME       _STA_PPPOE(0x0111)	//T=u32, connect time
#define CSID_S_PPPOE_LCS_CNT_TIME           _STA_PPPOE(0x0121)	//T=u32, connect time

#define CSID_S_PPPOE_IF_IP           	    _STA_PPPOE(0x0200)	//T=ipv4, dynamic interface ip
#define CSID_S_PPPOE_MULTIWAN_IF_IP         _STA_PPPOE(0x0201)	//T=ipv4, dynamic interface ip
#define CSID_S_PPPOE_VOIPWAN_IF_IP          _STA_PPPOE(0x0211)	//T=ipv4, dynamic interface ip
#define CSID_S_PPPOE_LCS_IF_IP              _STA_PPPOE(0x0221)	//T=ipv4, dynamic interface ip

#define CSID_S_PPPOE_IF_NM           	    _STA_PPPOE(0x0300)	//T=ipv4, dynamic interface netmask
#define CSID_S_PPPOE_MULTIWAN_IF_NM         _STA_PPPOE(0x0301)	//T=ipv4, dynamic interface netmask
#define CSID_S_PPPOE_VOIPWAN_IF_NM          _STA_PPPOE(0x0311)	//T=ipv4, dynamic interface netmask
#define CSID_S_PPPOE_LCS_IF_NM              _STA_PPPOE(0x0321)	//T=ipv4, dynamic interface netmask

#define CSID_S_PPPOE_IF_GW           	    _STA_PPPOE(0x0400)	//T=ipv4, dynamic interface gateway
#define CSID_S_PPPOE_MULTIWAN_IF_GW         _STA_PPPOE(0x0401)	//T=ipv4, dynamic interface gateway
#define CSID_S_PPPOE_VOIPWAN_IF_GW          _STA_PPPOE(0x0411)	//T=ipv4, dynamic interface gateway
#define CSID_S_PPPOE_LCS_IF_GW              _STA_PPPOE(0x0421)	//T=ipv4, dynamic interface gateway

#define CSID_S_PPPOE_IF_PRIDNS       	    _STA_PPPOE(0x0500)	//T=ipv4, dynamic interface primary DNS
#define CSID_S_PPPOE_MULTIWAN_IF_PRIDNS     _STA_PPPOE(0x0501)	//T=ipv4, dynamic interface primary DNS
#define CSID_S_PPPOE_VOIPWAN_IF_PRIDNS      _STA_PPPOE(0x0511)	//T=ipv4, dynamic interface primary DNS
#define CSID_S_PPPOE_LCS_IF_PRIDNS          _STA_PPPOE(0x0521)	//T=ipv4, dynamic interface primary DNS

#define CSID_S_PPPOE_IF_SECDNS       	    _STA_PPPOE(0x0600)	//T=ipv4, dynamic interface secondary DNS
#define CSID_S_PPPOE_MULTIWAN_IF_SECDNS     _STA_PPPOE(0x0601)	//T=ipv4, dynamic interface secondary DNS
#define CSID_S_PPPOE_VOIPWAN_IF_SECDNS      _STA_PPPOE(0x0611)	//T=ipv4, dynamic interface secondary DNS
#define CSID_S_PPPOE_LCS_IF_SECDNS          _STA_PPPOE(0x0621)	//T=ipv4, dynamic interface secondary DNS

#define CSID_S_PPPOE_CNT_STATUS      	    _STA_PPPOE(0x0700)	//T=u32, connect status
#define CSID_S_PPPOE_MULTIWAN_CNT_STATUS    _STA_PPPOE(0x0701)	//T=u32, connect status
#define CSID_S_PPPOE_VOIPWAN_CNT_STATUS     _STA_PPPOE(0x0711)	//T=u32, connect status
#define CSID_S_PPPOE_LCS_CNT_STATUS         _STA_PPPOE(0x0721)	//T=u32, connect status

#define CSID_S_PPPOE_PKTIN_NUM       	    _STA_PPPOE(0x0800)	//T=u32, number of packet input
#define CSID_S_PPPOE_MULTIWAN_PKTIN_NUM     _STA_PPPOE(0x0801)	//T=u32, number of packet input
#define CSID_S_PPPOE_VOIPWAN_PKTIN_NUM      _STA_PPPOE(0x0811)	//T=u32, number of packet input

#define CSID_S_PPPOE_PKTOUT_NUM      	    _STA_PPPOE(0x0900)	//T=u32, number of packet output
#define CSID_S_PPPOE_MULTIWAN_PKTOUT_NUM    _STA_PPPOE(0x0901)	//T=u32, number of packet output
#define CSID_S_PPPOE_VOIPWAN_PKTOUT_NUM     _STA_PPPOE(0x0911)	//T=u32, number of packet output

#define CSID_S_PPPOE_ALTERED         	    _STA_PPPOE(0x0A00)	//T=u32, ui altered
#define CSID_S_PPPOE_MULTIWAN_ALTERED       _STA_PPPOE(0x0A01)	//T=u32, ui altered
#define CSID_S_PPPOE_VOIPWAN_ALTERED        _STA_PPPOE(0x0A11)	//T=u32, ui altered
#define CSID_S_PPPOE_LCS_ALTERED            _STA_PPPOE(0x0A21)	//T=u32, ui altered

#define CSID_S_PPPOE_CMD_STATUS         	_STA_PPPOE(0x0B00)	//T=u32, check whether pppoe-action need to call or not
#define CSID_S_PPPOE_MULTIWAN_CMD_STATUS    _STA_PPPOE(0x0B01)	//T=u32, check whether pppoe-action need to call or not 
#define CSID_S_PPPOE_VOIPWAN_CMD_STATUS     _STA_PPPOE(0x0B11)	//T=u32, check whether pppoe-action need to call or not 

#define CSID_S_PPPOE_IF_NAME      	    	_STA_PPPOE(0x0C00)	//T=str, the interface name of pppoe, ppp0, ppp1, ppp2
#define CSID_S_PPPOE_LCS_IF_NAME            _STA_PPPOE(0x0C01)	//T=u32, the interface name of lcs pppoe, ppp0, ppp1 ...
#define CSID_S_PPPOE_MULTIWAN_IF_NAME		_STA_PPPOE(0x0C10)	//T=str, the interface name of pppoe, ppp0, ppp1, ppp2

#define CSID_S_PPPOE_DUAL_ACCESS_DEFAULT    _STA_PPPOE(0x0D00)	//T=str, the interface name of pppoe, ppp0, ppp1, ppp2

//<--- End of 256 account

#define CSID_S_PPPOE_RELAY_ALTERED			_STA_PPPOE(0x0020)	//T=u32, ui altered

/* Multisession Items (start fromn 0x2000) */
#define CSID_S_PPPOE_ML_ALTERED      _STA_PPPOE(0x2000)	//T=u8, ui altered

//<--- End of 256 account

#endif //ifndef _CSID_PPPOE_H
