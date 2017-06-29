/* vi: set sw=4 ts=4: */

/*
 * @file Member ID Definition of Group DHCP. 
 *
 * DHCP Sever (DHCPD) & DHCP Client (DHCPC)
 *
 * Moderator: WumingWu
 * Group ID: 0x00030000/0x80030000
 *
 * // Configuration Items   (64K)
 *   // DHCP Daemon 0x0001 ~ 0x4FFF
 *   // DHCP Client 0x5001 ~ 0x6FFF
 *   // DHCP Probe  0x7001 ~ 0x7FFF
 *   // Reserved    0x8000 ~ 0xFFFF
 * // Status Items			(64K)
 *   // DHCP Daemon 0x0001 ~ 0x4FFF
 *   // DHCP Client 0x5001 ~ 0x6FFF
 *   // DHCP Probe  0x7001 ~ 0x7FFF
 *   // Reserved    0x8000 ~ 0xFFFF
 */

#ifndef _CSID_DHCP_H
#define _CSID_DHCP_H

#include "csid_gid.h"

#define _CFG_DHCP(x)		(CSID_GIDC_DHCP|(x))
#define _STA_DHCP(x)		(CSID_GIDS_DHCP|(x))

/* Extra Definitions */

#define DHCP_CNT_TYPE_ONDEMAND          0x00
#define DHCP_CNT_TYPE_AUTORECONNECT     0x01
#define DHCP_CNT_TYPE_MANUAL            0x02

#define DHCP_CNT_STATUS_DISCONNECT      0x00
#define DHCP_CNT_STATUS_CONNECTING      0x01
#define DHCP_CNT_STATUS_CONNECT         0x02
#define DHCP_CNT_STATUS_DISCONNECTING   0x03
#define DHCP_CNT_STATUS_LEASEFAIL		0x04
#define DHCP_CNT_STATUS_WAIT_TRAFFIC	0x05

/* Configuration Items */

/* DHCP Daemon */
/* DHCP Server (DHCPD) Basic Setting */
#define CSID_C_DHCP_SVR_ENABLE				_CFG_DHCP(0x0001)	//T=u32, D=1, Enable/Disable
#define CSID_C_DHCP_SVR_LEASETIME			_CFG_DHCP(0x0002)	//T=u32, D=86400, Lease Time (seconds)
#define CSID_C_DHCP_SVR_IPPOOL_START		_CFG_DHCP(0x0003)	//T=ipv4, D={192.168.123.100},  IP Pool Starting Address
#define CSID_C_DHCP_SVR_IPPOOL_END			_CFG_DHCP(0x0004)	//T=ipv4, D={192.168.123.200}, IP Pool Ending Address
#define CSID_C_DHCP_SVR_DOMAIN				_CFG_DHCP(0x0005)	//T=str, Domain Name
#define CSID_C_DHCP_SVR_PRIDNS				_CFG_DHCP(0x0006)	//T=ipv4, Primary DNS  
#define CSID_C_DHCP_SVR_SECDNS				_CFG_DHCP(0x0007)	//T=ipv4, Second DNS  
#define CSID_C_DHCP_SVR_PRIWINS				_CFG_DHCP(0x0008)	//T=ipv4, Primary WINS  
#define CSID_C_DHCP_SVR_SECWINS				_CFG_DHCP(0x0009)	//T=ipv4, Second WINS
#define CSID_C_DHCP_SVR_GATEWAY				_CFG_DHCP(0x000a)	//T=ipv4, Gateway default is LAN ip
#define CSID_C_DHCP_SVR_MAXLEASE			_CFG_DHCP(0x000b)	//T=u32, D=101, the Max. number of lease ip 
#define CSID_C_DHCP_SVR_OPTION66_ENABLE		_CFG_DHCP(0x000c)	//T=u8, D=0, DHCP option 66 Enable/Disable
#define CSID_C_DHCP_SVR_OPTION66_PATH		_CFG_DHCP(0x000d)	//T=str, tftp server name
#define CSID_C_DHCP_SVR_INDEX				_CFG_DHCP(0x000e)	//T=u32, 0:server 1, 1:server 2,...
#define CSID_C_DHCP_SVR_NAME				_CFG_DHCP(0x000f)	//T=str, Server Name


/* DHCP Server (DHCPD) Basic Setting For AP mode , refer to CDM532RP */
#define CSID_C_DHCP_AP_SVR_ENABLE			_CFG_DHCP(0x0011)	//T=u32, D=1, Enable/Disable
#define CSID_C_DHCP_AP_SVR_LEASETIME		_CFG_DHCP(0x0012)	//T=u32, D=86400, Lease Time (seconds)
#define CSID_C_DHCP_AP_SVR_IPPOOL_START		_CFG_DHCP(0x0013)	//T=ipv4, D={192.168.123.100},  IP Pool Starting Address
#define CSID_C_DHCP_AP_SVR_IPPOOL_END		_CFG_DHCP(0x0014)	//T=ipv4, D={192.168.123.200}, IP Pool Ending Address
#define CSID_C_DHCP_AP_SVR_DOMAIN			_CFG_DHCP(0x0015)	//T=str, Domain Name
#define CSID_C_DHCP_AP_SVR_PRIDNS			_CFG_DHCP(0x0016)	//T=ipv4, Primary DNS  
#define CSID_C_DHCP_AP_SVR_SECDNS			_CFG_DHCP(0x0017)	//T=ipv4, Second DNS  
#define CSID_C_DHCP_AP_SVR_PRIWINS			_CFG_DHCP(0x0018)	//T=ipv4, Primary WINS  
#define CSID_C_DHCP_AP_SVR_SECWINS			_CFG_DHCP(0x0019)	//T=ipv4, Second WINS
#define CSID_C_DHCP_AP_SVR_GATEWAY			_CFG_DHCP(0x001a)	//T=ipv4, Gateway default is LAN ip
#define CSID_C_DHCP_AP_SVR_MAXLEASE			_CFG_DHCP(0x001b)	//T=u32, D=101, the Max. number of lease ip 

/* DHCP Server (DHCPD) Basic Setting For STA mode , refer to CDM532RP */
#define CSID_C_DHCP_STA_SVR_ENABLE           _CFG_DHCP(0x0021)   //T=u32, D=1, Enable/Disable
#define CSID_C_DHCP_STA_SVR_LEASETIME        _CFG_DHCP(0x0022)   //T=u32, D=86400, Lease Time (seconds)
#define CSID_C_DHCP_STA_SVR_IPPOOL_START     _CFG_DHCP(0x0023)   //T=ipv4, D={192.168.123.100},  IP Pool Starting Address
#define CSID_C_DHCP_STA_SVR_IPPOOL_END       _CFG_DHCP(0x0024)   //T=ipv4, D={192.168.123.200}, IP Pool Ending Address
#define CSID_C_DHCP_STA_SVR_DOMAIN           _CFG_DHCP(0x0025)   //T=str, Domain Name
#define CSID_C_DHCP_STA_SVR_PRIDNS           _CFG_DHCP(0x0026)   //T=ipv4, Primary DNS
#define CSID_C_DHCP_STA_SVR_SECDNS           _CFG_DHCP(0x0027)   //T=ipv4, Second DNS
#define CSID_C_DHCP_STA_SVR_PRIWINS          _CFG_DHCP(0x0028)   //T=ipv4, Primary WINS
#define CSID_C_DHCP_STA_SVR_SECWINS          _CFG_DHCP(0x0029)   //T=ipv4, Second WINS
#define CSID_C_DHCP_STA_SVR_GATEWAY          _CFG_DHCP(0x002a)   //T=ipv4, Gateway default is LAN ip
#define CSID_C_DHCP_STA_SVR_MAXLEASE         _CFG_DHCP(0x002b)   //T=u32, D=101, the Max. number of lease ip

/* Customize for ZyXEL */
#define CSID_C_DHCP_SVR_CUSTOMIZE_DNS 		_CFG_DHCP(0x0031)   //T=u8, D=0, customize flag, 0:no ; 1:yes 
#define CSID_C_DHCP_SVR_PRIDNS_TYPE 		_CFG_DHCP(0x0032)   //T=u8, D=0, Primary DNS type, 0: from ISP ; 1: user defined ; 2: dns relay
#define CSID_C_DHCP_SVR_SECDNS_TYPE 		_CFG_DHCP(0x0033)   //T=u8, D=0, Second DNS type, 0: from ISP ; 1: user defined ; 2: dns relay

/* Fixed Mapping Main Switch */
#define CSID_C_DHCP_SVR_FIXMAP_MAIN_DISABLE  _CFG_DHCP(0x0040)   //T=u8, DHCP server fixed mapping main switch, 0:Enable; 1:Disable

/* Fixed Mapping */
#define CSID_C_DHCP_SVR_FIXMAP_ENABLE		_CFG_DHCP(0x1000)	//T=u8, D=0, Enable/Disable
#define CSID_C_DHCP_SVR_FIXMAP_MAC			_CFG_DHCP(0x1200)	//T=mac, Mapped MAC
#define CSID_C_DHCP_SVR_FIXMAP_IP			_CFG_DHCP(0x1400)	//T=ipv4, Mapped IP
#define CSID_C_DHCP_SVR_FIXMAP_NAME			_CFG_DHCP(0x1600)	//T=str, Mapped IP Name

/* 512 items are reserved */
#define CSID_C_DHCP_SVR_FIXMAP_SVR_IDX		_CFG_DHCP(0x2000)	//T=u32, Fixed Mapping rule belong to which DHCP Server, 0: None, 1~4: DHCP 1~4

/* Basic Setting end */

/* DHCP Client */
#define CSID_C_DHCP_CLI_ENABLE				_CFG_DHCP(0x5001)	//T=u8, Client's Enable/Disable
#define CSID_C_DHCP_CLI_HOSTNAME			_CFG_DHCP(0x5002)	//T=str, Client's Host Name

#define CSID_C_DHCP_CLI_IF_IP				_CFG_DHCP(0x5004)	//T=ipv4, static interface ip
#define CSID_C_DHCP_CLI_IF_NM				_CFG_DHCP(0x5005)	//T=ipv4, static interface netmask
#define CSID_C_DHCP_CLI_IF_GW				_CFG_DHCP(0x5006)	//T=ipv4, static interface gateway
#define CSID_C_DHCP_CLI_IF_PRIDNS		    _CFG_DHCP(0x5007)	//T=ipv4, static interface Primary DNS
#define CSID_C_DHCP_CLI_IF_SECDNS		    _CFG_DHCP(0x5008)	//T=ipv4, static interface Second DNS
#define CSID_C_DHCP_CLI_MTU					_CFG_DHCP(0x5009)	//T=u32, D=0, MTU
#define CSID_C_DHCP_CLI_CNT_TYPE			_CFG_DHCP(0x500a)	//T=u8, Connect Type
#define CSID_C_DHCP_CLI_CHGTTL				_CFG_DHCP(0x500b)	//T=u8, Enable/Disable , Change the TTL value
#define CSID_C_DHCP_CLI_AUTOBKP				_CFG_DHCP(0x500c)	//T=u8, Enable/Disable , Auto Backup
#define CSID_C_DHCP_CLI_IDLETIME        	_CFG_DHCP(0x500d)	//T=u16, D=600, max idle time
#define CSID_C_DHCP_CLI_CNT_TYPE_UI			_CFG_DHCP(0x500e)	//T=u8, Connect Type
#define CSID_C_DHCP_CLI_NAT_DISABLE        	_CFG_DHCP(0x500f)	//T=u32, Dynamic IP wantype NAT disable
#define CSID_C_DHCP_CLI_VENDORCLASSID		_CFG_DHCP(0x5010)	//T=str, option vendor class identifier


/* Reserved 16 items for Multi-WAN,  X is from 1 .... */
#define CSID_C_DHCP_MULTIWAN_ENABLE				_CFG_DHCP(0x5100)	//T=u8, Client's Enable/Disable
#define CSID_C_DHCP_MULTIWAN_HOSTNAME			_CFG_DHCP(0x5110)	//T=str, Client's Host Name
#define CSID_C_DHCP_MULTIWAN_IF_IP				_CFG_DHCP(0x5120)	//T=ipv4, static interface ip
#define CSID_C_DHCP_MULTIWAN_IF_NM				_CFG_DHCP(0x5130)	//T=ipv4, static interface netmask
#define CSID_C_DHCP_MULTIWAN_IF_GW				_CFG_DHCP(0x5140)	//T=ipv4, static interface gateway
#define CSID_C_DHCP_MULTIWAN_IF_PRIDNS		    _CFG_DHCP(0x5150)	//T=ipv4, static interface Primary DNS
#define CSID_C_DHCP_MULTIWAN_IF_SECDNS		    _CFG_DHCP(0x5160)	//T=ipv4, static interface Second DNS
#define CSID_C_DHCP_MULTIWAN_MTU				_CFG_DHCP(0x5170)	//T=u32, D=0, MTU
#define CSID_C_DHCP_MULTIWAN_CNT_TYPE			_CFG_DHCP(0x5180)	//T=u32, Connect Type
#define CSID_C_DHCP_MULTIWAN_CHGTTL				_CFG_DHCP(0x5190)	//T=u32, Enable/Disable , Change the TTL value
#define CSID_C_DHCP_MULTIWAN_AUTOBKP			_CFG_DHCP(0x51A0)	//T=u32, Enable/Disable , Auto Backup
#define CSID_C_DHCP_MULTIWAN_IDLETIME        	_CFG_DHCP(0x51B0)	//T=u32, D=600, max idle time
#define CSID_C_DHCP_MULTIWAN_NAT_DISABLE        _CFG_DHCP(0x51C0)	//T=u32, Dynamic IP wantype NAT disable
#define CSID_C_DHCP_MULTIWAN_CNT_TYPE_UI        _CFG_DHCP(0x51D0)   //T=u32, Connect Type
#define CSID_C_DHCP_MULTIWAN_VENDORCLASSID		_CFG_DHCP(0x5400)	//T=str, option vendor class identifier

/* DHCP Relay */
#define CSID_C_DHCP_RELAY_ENABLE 				_CFG_DHCP(0x51E0)	//T=u8, Agent's Enable/Disable
#define CSID_C_DHCP_RELAY_SERVER_IP 			_CFG_DHCP(0x51E1)	//T=ipv4, static Agent server ip

/* Reserved 16 items for VOIP-WAN,  X is from 1 .... */
#define CSID_C_DHCP_VOIPWAN_ENABLE				_CFG_DHCP(0x51F0)	//T=u8, Client's Enable/Disable
#define CSID_C_DHCP_VOIPWAN_HOSTNAME			_CFG_DHCP(0x5200)	//T=str, Client's Host Name
#define CSID_C_DHCP_VOIPWAN_IF_IP				_CFG_DHCP(0x5210)	//T=ipv4, static interface ip
#define CSID_C_DHCP_VOIPWAN_IF_NM				_CFG_DHCP(0x5220)	//T=ipv4, static interface netmask
#define CSID_C_DHCP_VOIPWAN_IF_GW				_CFG_DHCP(0x5230)	//T=ipv4, static interface gateway
#define CSID_C_DHCP_VOIPWAN_IF_PRIDNS		    _CFG_DHCP(0x5240)	//T=ipv4, static interface Primary DNS
#define CSID_C_DHCP_VOIPWAN_IF_SECDNS		    _CFG_DHCP(0x5250)	//T=ipv4, static interface Second DNS
#define CSID_C_DHCP_VOIPWAN_MTU					_CFG_DHCP(0x5260)	//T=u32, MTU
#define CSID_C_DHCP_VOIPWAN_CNT_TYPE			_CFG_DHCP(0x5270)	//T=u32, Connect Type
#define CSID_C_DHCP_VOIPWAN_CHGTTL				_CFG_DHCP(0x5280)	//T=u32, Enable/Disable , Change the TTL value
#define CSID_C_DHCP_VOIPWAN_AUTOBKP				_CFG_DHCP(0x5290)	//T=u32, Enable/Disable , Auto Backup
#define CSID_C_DHCP_VOIPWAN_IDLETIME        	_CFG_DHCP(0x52A0)	//T=u32, D=600, max idle time
#define CSID_C_DHCP_VOIPWAN_NAT_DISABLE        	_CFG_DHCP(0x52B0)	//T=u32, Dynamic IP wantype NAT disable

#define CSID_C_DHCP_OPTION66_ENABLE				_CFG_DHCP(0x6000)	//T=u8, D=0, DHCP option 66 enable, 0:disable, 1:enable.
#define CSID_C_DHCP_OPTION33_ENABLE	            _CFG_DHCP(0x6001)	//T=u8, D=0, DHCP option 33 enable, 0:disable, 1:enable.
#define CSID_C_DHCP_OPTION121_ENABLE	        _CFG_DHCP(0x6002)	//T=u8, D=0, DHCP option 121 enable, 0:disable, 1:enable.
#define CSID_C_DHCP_OPTION249_ENABLE	        _CFG_DHCP(0x6003)	//T=u8, D=0, DHCP option 249 enable, 0:disable, 1:enable.

#define CSID_C_DHCP_BROADCAST_ENABLE            _CFG_DHCP(0x6004)   //T=u8, D=1, DHCP broadcast enable, 0:disable, 1:enable

#define CSID_C_DHCP_SVR_NETCOMM                 _CFG_DHCP(0x6005)   //T=u8, D=0, router type, 0:other, 1:netcomm

#define CSID_C_DHCP_OPTION43_ENABLE	            _CFG_DHCP(0x6006)	//T=u8, D=0, DHCP option 43 enable, 0:disable, 1:enable.

/* DHCP Relay Agents Setting (Reserved 16 items) */
#define CSID_C_DHCP_RELAY_AGENT_ENABLE          _CFG_DHCP(0x6100)   //T=u8, Agent's Enable/Disable
#define CSID_C_DHCP_RELAY_AGENT_SVR_IP          _CFG_DHCP(0x6110)   //T=ipv4, static Agent server ip

/* Status Items */
/* DHCP Daemon */
#define CSID_S_DHCP_SVR_PID					_STA_DHCP(0x0001)	//T=u16, The PID of DHCP Server
#define CSID_S_DHCP_SVR_ALTERED			    _STA_DHCP(0x0002)	//T=u8, ui altered
#define CSID_S_DHCP_SVR_DNS			        _STA_DHCP(0x0003)	//T=u8, if assign lan ip as dns
#define CSID_S_DHCP_SVR_LEASE_TIME          _STA_DHCP(0x0004)   //T=u32, record lease time for dnrd
#define CSID_S_DHCP_STATUS          _STA_DHCP(0x0005)   //T=u32, 0:init 1:requesting ~ 5:released for tr-069 setting

/* Client-List, 256 items are reserved */
#define CSID_S_DHCP_SVR_CLILIST_IP			_STA_DHCP(0x0100)	//T=ipv4, The IP of the Client in Client List
#define CSID_S_DHCP_SVR_CLILIST_HOSTNAME	_STA_DHCP(0x0200)	//T=str, The hostname of the Client in the Client List
#define CSID_S_DHCP_SVR_CLILIST_MAC			_STA_DHCP(0x0300)	//T=mac, The Mac Address of the Client in Client List
#define CSID_S_DHCP_SVR_CLILIST_LEASETIME	_STA_DHCP(0x0400)	//T=u32, remaining lease time
#define CSID_S_DHCP_SVR_CLILIST_TYPE		_STA_DHCP(0x0500)	//T=u8, wired or wireless mode
#define CSID_S_DHCP_SVR_CLILIST_WLAN		_STA_DHCP(0x0600)	//T=str, backup wireless client for speical issue
/* Client-List end */

/* DHCP Client */
#define CSID_S_DHCP_CLI_PID					_STA_DHCP(0x5001)	//T=u16, The PID of DHCP Client 
#define CSID_S_DHCP_CLI_IF_IP				_STA_DHCP(0x5002)	//T=ipv4, dynamic interface ip
#define CSID_S_DHCP_CLI_IF_NM				_STA_DHCP(0x5003)	//T=ipv4, dynamic interface netmask
#define CSID_S_DHCP_CLI_IF_GW				_STA_DHCP(0x5004)	//T=ipv4, dynamic interface gateway
#define CSID_S_DHCP_CLI_IF_PRIDNS		    _STA_DHCP(0x5005)	//T=ipv4, dynamic interface Primary DNS
#define CSID_S_DHCP_CLI_IF_SECDNS		    _STA_DHCP(0x5006)	//T=ipv4, dynamic interface Second DNS
#define CSID_S_DHCP_CLI_CNT_STATUS          _STA_DHCP(0x5007)	//T=u32, connect status
#define CSID_S_DHCP_CLI_ALTERED			    _STA_DHCP(0x5008)	//T=u32, ui altered
#define CSID_S_DHCP_CLI_LEASE_TIME			_STA_DHCP(0x5009)	//T=u32, uptime + leasetime
#define CSID_S_DHCP_CLI_CMD_STATUS          _STA_DHCP(0x500a)   //T=u32, dhcp cmd status
#define CSID_S_DHCP_CLI_UP_TIME			_STA_DHCP(0x5010)	//T=u32, uptime

/* Reserved 16 items for Multi-WAN,  X is from 1 .... */
#define CSID_S_DHCP_MULTIWAN_PID				_STA_DHCP(0x5100)	//T=u16, The PID of DHCP Client 
#define CSID_S_DHCP_MULTIWAN_IF_IP				_STA_DHCP(0x5110)	//T=ipv4, dynamic interface ip
#define CSID_S_DHCP_MULTIWAN_IF_NM				_STA_DHCP(0x5120)	//T=ipv4, dynamic interface netmask
#define CSID_S_DHCP_MULTIWAN_IF_GW				_STA_DHCP(0x5130)	//T=ipv4, dynamic interface gateway
#define CSID_S_DHCP_MULTIWAN_IF_PRIDNS		    _STA_DHCP(0x5140)	//T=ipv4, dynamic interface Primary DNS
#define CSID_S_DHCP_MULTIWAN_IF_SECDNS		    _STA_DHCP(0x5150)	//T=ipv4, dynamic interface Second DNS
#define CSID_S_DHCP_MULTIWAN_CNT_STATUS         _STA_DHCP(0x5160)	//T=u32, connect status
#define CSID_S_DHCP_MULTIWAN_ALTERED			_STA_DHCP(0x5170)	//T=u32, ui altered
#define CSID_S_DHCP_MULTIWAN_LEASE_TIME			_STA_DHCP(0x5180)	//T=u32, uptime + leasetime
#define CSID_S_DHCP_MULTIWAN_CMD_STATUS			_STA_DHCP(0x5190)	//T=u32, multiwan command status
/* Reserved 16 items for VoIP-WAN,  X is from 1 .... */
#define CSID_S_DHCP_VOIPWAN_PID				_STA_DHCP(0x51a0)	//T=u16, The PID of DHCP Client 
#define CSID_S_DHCP_VOIPWAN_IF_IP				_STA_DHCP(0x51b0)	//T=ipv4, dynamic interface ip
#define CSID_S_DHCP_VOIPWAN_IF_NM				_STA_DHCP(0x51c0)	//T=ipv4, dynamic interface netmask
#define CSID_S_DHCP_VOIPWAN_IF_GW				_STA_DHCP(0x51d0)	//T=ipv4, dynamic interface gateway
#define CSID_S_DHCP_VOIPWAN_IF_PRIDNS		    _STA_DHCP(0x51e0)	//T=ipv4, dynamic interface Primary DNS
#define CSID_S_DHCP_VOIPWAN_IF_SECDNS		    _STA_DHCP(0x51f0)	//T=ipv4, dynamic interface Second DNS
#define CSID_S_DHCP_VOIPWAN_CNT_STATUS         _STA_DHCP(0x5200)	//T=u32, connect status
#define CSID_S_DHCP_VOIPWAN_ALTERED			_STA_DHCP(0x5210)	//T=u32, ui altered
#define CSID_S_DHCP_VOIPWAN_LEASE_TIME			_STA_DHCP(0x5220)	//T=u32, uptime + leasetime
#define CSID_S_DHCP_VOIPWAN_CMD_STATUS			_STA_DHCP(0x5230)	//T=u32, VOIPWAN command status

/*CDS530AM*/
#define CSID_S_DHCP_CLIENT_TO_SERVER		_STA_DHCP(0x5280)	  //T=u8,D=0, client mode change to server mode(event)
#define CSID_S_DHCP_CLIENT_TO_SERVER_COMPLETE 	_STA_DHCP(0x5281) //T=u8,D=0, client mode change to server mode complete flag
#define CSID_S_DHCP_AUTO_MODE_GET_IP		_STA_DHCP(0x5282)	  //T=u8,D=0, The flag which router got IP first time at auto mode.
#define CSID_S_DHCP_CHECK_ARP		        _STA_DHCP(0x5283)	  //T=u8,D=0, 0: dissconnect 1:connect

#define CSID_S_DHCP_OPTION66_TRANSFER_MODE	_STA_DHCP(0x6000)	//T=u8, D=0, DHCP option 66 transfer mode, 0:tftp 1:http 2:https.
#define CSID_S_DHCP_OPTION66_SERVER_NAME	_STA_DHCP(0x6001)	//T=str, DHCP option 66 tftp server name.

#define CSID_S_DHCP_OPTION66_FILE_NAME		_STA_DHCP(0x6002)	//T=str, DHCP option 66 file name.

#define CSID_S_DHCP_OPTION33_DESTINATION        _STA_DHCP(0x6100)	//T=ipv4, DHCP option 33 destination
#define CSID_S_DHCP_OPTION33_GATEWAY	        _STA_DHCP(0x6180)	//T=ipv4, DHCP option 33 gateway

#define CSID_S_DHCP_OPTION121_DESTINATION       _STA_DHCP(0x6200)	//T=ipv4, DHCP option 121 destination
#define CSID_S_DHCP_OPTION121_GATEWAY	        _STA_DHCP(0x6240)	//T=ipv4, DHCP option 121 gateway
#define CSID_S_DHCP_OPTION121_PREFIX            _STA_DHCP(0x6280)	//T=u32, DHCP option 121 prefix


#define CSID_S_DHCP_OPTION249_DESTINATION       _STA_DHCP(0x6300)	//T=ipv4, DHCP option 249 destination
#define CSID_S_DHCP_OPTION249_GATEWAY	        _STA_DHCP(0x6340)	//T=ipv4, DHCP option 249 gateway
#define CSID_S_DHCP_OPTION249_PREFIX	        _STA_DHCP(0x6380)	//T=u32, DHCP option 249 prefix


#endif //ifndef _CSID_DHCP_H
