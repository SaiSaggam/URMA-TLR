/*
 * @file Member ID Definition of Group PPTP.
 *
 * Moderator: GeorgeWang
 * Group ID: 0x00050000/0x80050000
 */

#ifndef _CSID_PPTP_H
#define _CSID_PPTP_H

#include "csid_gid.h"

#define _CFG_PPTP(x)		(CSID_GIDC_PPTP|(x))
#define _STA_PPTP(x)		(CSID_GIDS_PPTP|(x))

/* Extra Definitions */

#define PPTP_CNT_TYPE_ONDEMAND			0x00
#define PPTP_CNT_TYPE_AUTORECONNECT		0x01
#define PPTP_CNT_TYPE_MANUAL			0x02
#define PPTP_CNT_TYPE_ONFAILOVER		0x03

#define PPTP_AUTHTYPE_AUTO				0x00
#define PPTP_AUTHTYPE_PAP				0x01
#define PPTP_AUTHTYPE_CHAP				0x02
#define PPTP_AUTHTYPE_MSCHAP			0x04
#define PPTP_AUTHTYPE_MSCHAPV2			0x08
#define PPTP_AUTHTYPE_EAP				0x10

#define PPTP_PHASE1_DHCP				0x00
#define PPTP_PHASE1_FIXED				0x01

#define PPTP_CNT_STATUS_DISCONNECT		0x00
#define PPTP_CNT_STATUS_CONNECTING		0x01
#define PPTP_CNT_STATUS_CONNECT			0x02
#define PPTP_CNT_STATUS_DISCONNECTING	0x03
#define PPTP_CNT_STATUS_WAIT_TRAFFIC	0x05
#define PPTP_CNT_STATUS_AUTH_FAIL		0x06

/* Configuration Items */
/* Total 256 account */
#define CSID_C_PPTP_PASSTHROUGH					_CFG_PPTP(0x0001)   //T=u32, D=1, pptp passthrough
#define CSID_C_PPTP_NAT_DISABLE					_CFG_PPTP(0x0002)   //T=u8,  D=0, NAT disable for WAN type PPTP
#define CSID_C_PPTP_KERNEL_MODE					_CFG_PPTP(0x0003)   //T=u8,  D=0, Whether or not to enable Kernle Mode (Accel-PPTP)
#define CSID_C_PPTP_MULTIWAN_NAT_DISABLE		_CFG_PPTP(0x0020)   //T=u8,  D=0, NAT disable for WAN type PPTP in Multiwan
#define CSID_C_PPTP_USER						_CFG_PPTP(0x0100)	//T=str, user name
#define CSID_C_PPTP_MULTIWAN_USER				_CFG_PPTP(0x0101)	//T=str, user name
#define CSID_C_PPTP_PASSWORD					_CFG_PPTP(0x0200)	//T=str, user password
#define CSID_C_PPTP_MULTIWAN_PASSWORD			_CFG_PPTP(0x0201)	//T=str, user password
#define CSID_C_PPTP_P1FIXED_IP					_CFG_PPTP(0x0300)	//T=ipv4, fixed interface ip
#define CSID_C_PPTP_MULTIWAN_P1FIXED_IP        	_CFG_PPTP(0x0301)	//T=ipv4, fixed interface ip
#define CSID_C_PPTP_P1FIXED_NM					_CFG_PPTP(0x0400)	//T=ipv4, fixed interface netmask
#define CSID_C_PPTP_MULTIWAN_P1FIXED_NM        	_CFG_PPTP(0x0401)	//T=ipv4, fixed interface netmask
#define CSID_C_PPTP_P1FIXED_GW					_CFG_PPTP(0x0500)	//T=ipv4, fixed interface gateway
#define CSID_C_PPTP_MULTIWAN_P1FIXED_GW         _CFG_PPTP(0x0501)	//T=ipv4, fixed interface gateway
#define CSID_C_PPTP_PNS_SERVER					_CFG_PPTP(0x0600)	//T=str, PNS server name/ip
#define CSID_C_PPTP_MULTIWAN_PNS_SERVER			_CFG_PPTP(0x0601)	//T=str, PNS server name/ip
#define CSID_C_PPTP_P1FIXED_PRIDNS				_CFG_PPTP(0x0700)	//T=ipv4, fixed primary DNS
#define CSID_C_PPTP_MULTIWAN_P1FIXED_PRIDNS     _CFG_PPTP(0x0701)	//T=ipv4, fixed primary DNS
#define CSID_C_PPTP_P1FIXED_SECDNS				_CFG_PPTP(0x0800)	//T=ipv4, fixed secondary DNS
#define CSID_C_PPTP_MULTIWAN_P1FIXED_SECDNS     _CFG_PPTP(0x0801)	//T=ipv4, fixed secondary DNS
#define CSID_C_PPTP_CNT_TYPE					_CFG_PPTP(0x0900)	//T=u16, connect type
#define CSID_C_PPTP_MULTIWAN_CNT_TYPE			_CFG_PPTP(0x0901)	//T=u16, connect type
#define CSID_C_PPTP_CNT_TYPE_UI					_CFG_PPTP(0x0911)	//T=u8, connect type
#define CSID_C_PPTP_MULTIWAN_CNT_TYPE_UI        _CFG_PPTP(0x0920)   //T=u16, connect type
#define CSID_C_PPTP_MTU							_CFG_PPTP(0x0a00)	//T=u16, D=0, max transmission unit
#define CSID_C_PPTP_MULTIWAN_MTU				_CFG_PPTP(0x0a01)	//T=u16, D=0, max transmission unit
#define CSID_C_PPTP_IDLETIME					_CFG_PPTP(0x0b00)	//T=u16, D=600, max idle time
#define CSID_C_PPTP_MULTIWAN_IDLETIME			_CFG_PPTP(0x0b01)	//T=u16, D=600, max idle time
#define CSID_C_PPTP_HOLDOFFTIME					_CFG_PPTP(0x0c00)	//T=u16, D=300, wait time before rebuild
#define CSID_C_PPTP_MULTIWAN_HOLDOFFTIME		_CFG_PPTP(0x0c01)	//T=u16, D=300, wait time before rebuild
#define CSID_C_PPTP_AUTHTYPE					_CFG_PPTP(0x0d00)	//T=u16, auth type
#define CSID_C_PPTP_MULTIWAN_AUTHTYPE			_CFG_PPTP(0x0d01)	//T=u16, auth type
#define CSID_C_PPTP_PHASE1_TYPE					_CFG_PPTP(0x0e00)	//T=u8, D=0, connect to server in phase1 stage
#define CSID_C_PPTP_MULTIWAN_PHASE1_TYPE		_CFG_PPTP(0x0e01)	//T=u8, D=0, connect to server in phase1 stage
#define CSID_C_PPTP_CNT_ID						_CFG_PPTP(0x0f00)	//T=str, Connection ID
#define CSID_C_PPTP_MULTIWAN_CNT_ID				_CFG_PPTP(0x0f01)	//T=str, Connection ID
/* 255 account reserverd */ 

/* WANType PPTP MPPE and AUTHTYPE */
#define CSID_C_PPTP_ENABLE_MPPE                      _CFG_PPTP(0x4100)   	//T=u8, D=0, Enable/Disable
#define CSID_C_PPTP_ENABLE_MPPE_40                   _CFG_PPTP(0x4101)   	//T=u8, D=0, Enable/Disable
#define CSID_C_PPTP_ENABLE_MPPE_56                   _CFG_PPTP(0x4102)   	//T=u8, D=0, Enable/Disable
#define CSID_C_PPTP_ENABLE_MPPE_128                  _CFG_PPTP(0x4103)   	//T=u8, D=0, Enable/Disable



/* PPTP Client Items */
#define CSID_C_PPTP_CLI_ENABLE          _CFG_PPTP(0x1000)   //T=u8, D=0, enable pptp client
#define CSID_C_PPTP_CLI_QOS_ENABLE      _CFG_PPTP(0x1001)   //T=u8, D=0, enable pptp client QoS for quick Echo Req/Res
#define CSID_C_PPTP_CLI_DETECT_PNS      _CFG_PPTP(0x1002)   //T=u8, D=0, type to detect the existence of server
#define CSID_C_PPTP_CLI_USER_ENABLE     _CFG_PPTP(0x1100)   //T=u8, D=0, enable pptp client user
#define CSID_C_PPTP_CLI_TUNLNAME        _CFG_PPTP(0x1200)   //T=str, tunnel name
#define CSID_C_PPTP_CLI_PEERIP          _CFG_PPTP(0x1300)   //T=ipv4, peer ip (replace by CLI_PEERIP_STR)
#define CSID_C_PPTP_CLI_USER            _CFG_PPTP(0x1400)   //T=str, user name
#define CSID_C_PPTP_CLI_PASSWORD        _CFG_PPTP(0x1500)   //T=str, user password
#define CSID_C_PPTP_CLI_ROUTE           _CFG_PPTP(0x1600)   //T=str, route
#define CSID_C_PPTP_CLI_CONNECT_TYPE    _CFG_PPTP(0x1700)   //T=u8, D=0, connect type
#define CSID_C_PPTP_CLI_MPPE            _CFG_PPTP(0x1800)   //T=u8, D=0, enable mppe
#define CSID_C_PPTP_CLI_NAT             _CFG_PPTP(0x1900)   //T=u8, D=0, enable nat
#define CSID_C_PPTP_CLI_ACTIVE          _CFG_PPTP(0x1a00)   //T=u8, D=0, client active
#define CSID_C_PPTP_CLI_PEERIP_STR		_CFG_PPTP(0x1b00)	//T=str, peer ip
#define CSID_C_PPTP_CLI_DO_AUTH         _CFG_PPTP(0x1c00)   //T=u8, D=0, 0: NoAuth,  1: Auth
#define CSID_C_PPTP_CLI_AUTH_PAP        _CFG_PPTP(0x1d00)   //T=u8, D=0, 0: Default, 1: Accept PAP        2: Reject PAP
#define CSID_C_PPTP_CLI_AUTH_CHAP       _CFG_PPTP(0x1e00)   //T=u8, D=0, 0: Default, 1: Accept CHAP       2: Reject CHAP  
#define CSID_C_PPTP_CLI_AUTH_MSCHAP     _CFG_PPTP(0x1f00)   //T=u8, D=0, 0: Default, 1: Accept MSCHAP     2: Reject MSCHAP
#define CSID_C_PPTP_CLI_AUTH_MSCHAPV2   _CFG_PPTP(0x2000)   //T=u8, D=0, 0: Default, 1: Accept MSCHAP-V2  2: Reject MACHAP-V2
#define CSID_C_PPTP_CLI_LOCAL_IP        _CFG_PPTP(0x2100)   //T=ipv4, Local IP
#define CSID_C_PPTP_CLI_REMOTE_IP       _CFG_PPTP(0x2200)   //T=ipv4, Remote IP
#define CSID_C_PPTP_CLI_LCP_TYPE        _CFG_PPTP(0x2300)   //T=u8, D=0,  LCP Echo Type. 0: Auto, 1: Manual, 2: Disable
#define CSID_C_PPTP_CLI_LCP_INTERVAL    _CFG_PPTP(0x2400)   //T=u8, D=30, LCP Echo Interval
#define CSID_C_PPTP_CLI_LCP_FAILURE     _CFG_PPTP(0x2500)   //T=u8, D=6,  LCP Max Failures
#define CSID_C_PPTP_CLI_IDLETIME		_CFG_PPTP(0x2600)	//T=u16, D=600, max idle time for cli
#define CSID_C_PPTP_CLI_FAILOVER        _CFG_PPTP(0x2700)   //T=u32, D=0, Operation mode,0:Always,1:Failover,2:Load Balance
#define CSID_C_PPTP_CLI_TYPE_FAILOVER   _CFG_PPTP(0x2800)   //T=u32,D=0,the major tunnel type which use this failover tunnel, 0:IPsec,1:PPTP,2:L2TP,3:GRE
#define CSID_C_PPTP_CLI_FAILOVER_IDX    _CFG_PPTP(0x2900)   //T=u32, D=0,the tunnel idx use this failover tunnel
/* 255 account reserverd */

/* for PPTP wantype, user specified DNS information */
#define CSID_C_PPTP_PRIDNS						_CFG_PPTP(0x3100)	//T=ipv4, primary DNS
#define CSID_C_PPTP_MULTIWAN_PRIDNS				_CFG_PPTP(0x3101)	//T=ipv4, primary DNS
#define CSID_C_PPTP_SECDNS						_CFG_PPTP(0x3200)	//T=ipv4, secondary DNS
#define CSID_C_PPTP_MULTIWAN_SECDNS				_CFG_PPTP(0x3201)	//T=ipv4, secondary DNS
#define CSID_C_PPTP_CLI_WAN             _CFG_PPTP(0x3300)   //T=u32,D=0, 0:Auto,1:WAN1,2:WAN2,3:WAN3,30:br0/LAN,31:VLAN1,32:VLAN2,33:VLAN3
#define CSID_C_PPTP_CLI_ONDEMAND_MAX_IDLE_TIME                   _CFG_PPTP(0x3400)  //T=u32, D=0, PPTP client ondemand maximum idle time 
/* Status Items */
#define CSID_S_PPTP_CLI_ALTERED         _STA_PPTP(0x1001)   //T=u8, ui altered
#define CSID_S_PPTP_CLI_PEER_ALTERED    _STA_PPTP(0x1b00)   //T=u8, D=0, tunnel's configuration altered
//#define CSID_S_PPTP_CLI_ACTIVE_TUNNELS  _STA_PPTP(0x1002)   //T=u8, D=0, tunnel numbers
//#define CSID_S_PPTP_CLI_ACT_PEER_IDX    _STA_PPTP(0x1003)   //T=u8, D=0, peer index
//#define CSID_S_PPTP_CLI_ACT_BUTTON      _STA_PPTP(0x1004)   //T=u8, D=0, peer action button
/* Account status */
#define CSID_S_PPTP_CLI_PEER_USER       _STA_PPTP(0x2100)    //T=str, username
#define CSID_S_PPTP_CLI_PEER_REAL_IP    _STA_PPTP(0x2200)    //T=ipv4, peer real ip
#define CSID_S_PPTP_CLI_PEER_VIRTUAL_IP _STA_PPTP(0x2300)    //T=ipv4, peer virtual ip
#define CSID_S_PPTP_CLI_PEER_PID    	_STA_PPTP(0x2400)    //T=s32, D=0, peer` process id
#define CSID_S_PPTP_CLI_PEER_IFNAME     _STA_PPTP(0x2500)    //T=str, peer interface name
#define CSID_S_PPTP_CLI_PEER_REMOTE_IP  _STA_PPTP(0x2600)    //T=ipv4, remote server ip
#define CSID_S_PPTP_CLI_PEER_CONNT_ID   _STA_PPTP(0x2700)    //T=u8, D=0, connecter`s config list ID
#define CSID_S_PPTP_CLI_PEER_CONNT_TYPE _STA_PPTP(0x2800)    //T=u8, D=0, connect type
#define CSID_S_PPTP_CLI_PEER_CONNT_STAT _STA_PPTP(0x2900)    //T=u8, D=0, connect status
#define CSID_S_PPTP_CLI_TOTAL_PEER_SUBNET_LIST _STA_PPTP(0x3000)   //T=str, Total PPTP CLI Peer subnet list
/* Status Items */ 
/* Total 256 account */ 
#define CSID_S_PPTP_CNT_TIME				_STA_PPTP(0x0100)	//T=u32,   connect time
#define CSID_S_PPTP_MULTIWAN_CNT_TIME		_STA_PPTP(0x0101)	//T=u32,   connect time
#define CSID_S_PPTP_IF_IP					_STA_PPTP(0x0200)	//T=ipv4,  dynamic interface ip
#define CSID_S_PPTP_MULTIWAN_IF_IP			_STA_PPTP(0x0201)	//T=ipv4,  dynamic interface ip
#define CSID_S_PPTP_IF_NM					_STA_PPTP(0x0300)	//T=ipv4,  dynamic interface netmask
#define CSID_S_PPTP_MULTIWAN_IF_NM			_STA_PPTP(0x0301)	//T=ipv4,  dynamic interface netmask
#define CSID_S_PPTP_IF_GW					_STA_PPTP(0x0400)	//T=ipv4,  dynamic interface gateway
#define CSID_S_PPTP_MULTIWAN_IF_GW			_STA_PPTP(0x0401)	//T=ipv4,  dynamic interface gateway
#define CSID_S_PPTP_IF_PRIDNS				_STA_PPTP(0x0500)	//T=ipv4,  dynamic interface primary DNS
#define CSID_S_PPTP_MULTIWAN_IF_PRIDNS		_STA_PPTP(0x0501)	//T=ipv4,  dynamic interface primary DNS
#define CSID_S_PPTP_IF_SECDNS				_STA_PPTP(0x0600)	//T=ipv4,  dynamic interface secondary DNS
#define CSID_S_PPTP_MULTIWAN_IF_SECDNS		_STA_PPTP(0x0601)	//T=ipv4,  dynamic interface secondary DNS
#define CSID_S_PPTP_CNT_STATUS				_STA_PPTP(0x0700)	//T=u16,   connect status
#define CSID_S_PPTP_MULTIWAN_CNT_STATUS		_STA_PPTP(0x0701)	//T=u16,   connect status
#define CSID_S_PPTP_PKTIN_NUM				_STA_PPTP(0x0800)	//T=u32,   number of packet input
#define CSID_S_PPTP_MULTIWAN_PKTIN_NUM		_STA_PPTP(0x0801)	//T=u32,   number of packet input
#define CSID_S_PPTP_PKTOUT_NUM				_STA_PPTP(0x0900)	//T=u32,   number of packet output
#define CSID_S_PPTP_MULTIWAN_PKTOUT_NUM		_STA_PPTP(0x0901)	//T=u32,   number of packet output
#define CSID_S_PPTP_ALTERED					_STA_PPTP(0x0a00)	//T=u8,    ui altered
#define CSID_S_PPTP_MULTIWAN_ALTERED		_STA_PPTP(0x0a01)	//T=u8,    ui altered
#define CSID_S_PPTP_CMD_STATUS				_STA_PPTP(0x0b00)	//T=u32,   pptp command status
#define CSID_S_PPTP_MULTIWAN_CMD_STATUS		_STA_PPTP(0x0b01)	//T=u32,   pptp command status
#define CSID_S_PPTP_IF_NAME					_STA_PPTP(0x0c00)  	//T=str,   
#define CSID_S_PPTP_MULTIWAN_IF_NAME		_STA_PPTP(0x0c01)  	//T=str,   
/* 255 account reserverd */

#endif //ifndef _CSID_PPTP_H
