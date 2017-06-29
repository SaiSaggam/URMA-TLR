/*
 * @file Member ID Definition of Group L2TP.
 *
 * Moderator: GeorgeWang
 * Group ID: 0x00060000/0x80060000
 */

#ifndef _CSID_L2TP_H
#define _CSID_L2TP_H

#include "csid_gid.h"

#define _CFG_L2TP(x)		(CSID_GIDC_L2TP|(x))
#define _STA_L2TP(x)		(CSID_GIDS_L2TP|(x))

/* Extra Definitions */
#define L2TP_CNT_TYPE_ONDEMAND			0x00
#define L2TP_CNT_TYPE_AUTORECONNECT		0x01
#define L2TP_CNT_TYPE_MANUAL			0x02

#define L2TP_AUTHTYPE_AUTO				0x00
#define L2TP_AUTHTYPE_PAP				0x01
#define L2TP_AUTHTYPE_CHAP				0x02
#define L2TP_AUTHTYPE_MSCHAP			0x04
#define L2TP_AUTHTYPE_MSCHAPV2			0x08
#define L2TP_AUTHTYPE_EAP				0x10

#define L2TP_PHASE1_DHCP				0x00
#define L2TP_PHASE1_FIXED				0x01

#define L2TP_CNT_STATUS_DISCONNECT		0x00
#define L2TP_CNT_STATUS_CONNECTING		0x01
#define L2TP_CNT_STATUS_CONNECT			0x02
#define L2TP_CNT_STATUS_DISCONNECTING	0x03
#define L2TP_CNT_STATUS_WAIT_TRAFFIC	0x05
#define L2TP_CNT_STATUS_AUTH_FAIL		0x06

#define L2TP_CLI_MAX_TUN        		22

/* Configuration Items for WAN Type L2TP */
#define CSID_C_L2TP_PASSTHROUGH          	_CFG_L2TP(0x0001)   //T=u32, D=1, l2tp passthrough
#define CSID_C_L2TP_CNT_TYPE_UI				_CFG_L2TP(0x0002)	//T=u8, connect type
#define CSID_C_L2TP_NAT_DISABLE			_CFG_L2TP(0x0003)	//T=u8, D=0, NAT disable for WAN type L2TP
#define CSID_C_L2TP_MULTIWAN_NAT_DISABLE	_CFG_L2TP(0x0030)	//T=u8, D=0, NAT disable for WAN type L2TP
#define CSID_C_L2TP_USER					_CFG_L2TP(0x0100)	//T=str, user name
#define CSID_C_L2TP_PASSWORD				_CFG_L2TP(0x0200)	//T=str, user password
#define CSID_C_L2TP_P1FIXED_IP				_CFG_L2TP(0x0300)	//T=ipv4, fixed interface ip
#define CSID_C_L2TP_P1FIXED_NM				_CFG_L2TP(0x0400)	//T=ipv4, fixed interface netmask
#define CSID_C_L2TP_P1FIXED_GW				_CFG_L2TP(0x0500)	//T=ipv4, fixed interface gateway
#define CSID_C_L2TP_LNS_SERVER				_CFG_L2TP(0x0600)	//T=str, LNS server name/ip
#define CSID_C_L2TP_P1FIXED_PRIDNS			_CFG_L2TP(0x0700)	//T=ipv4, fixed interface primary DNS
#define CSID_C_L2TP_P1FIXED_SECDNS			_CFG_L2TP(0x0800)	//T=ipv4, fixed interface secondary DNS
#define CSID_C_L2TP_CNT_TYPE				_CFG_L2TP(0x0900)	//T=u16, connect type
#define CSID_C_L2TP_MTU						_CFG_L2TP(0x0a00)	//T=u16, D=0,  max transmission unit
#define CSID_C_L2TP_IDLETIME				_CFG_L2TP(0x0b00)	//T=u16, D=600, max idle time
#define CSID_C_L2TP_HOLDOFFTIME				_CFG_L2TP(0x0c00)	//T=u16, D=300, wait time before rebuild
#define CSID_C_L2TP_AUTHTYPE				_CFG_L2TP(0x0d00)	//T=u16, auth type
#define CSID_C_L2TP_PHASE1_TYPE				_CFG_L2TP(0x0e00)	//T=u8, D=0, connect to server in phase1 stage
#define CSID_C_L2TP_PORT					_CFG_L2TP(0x0f00)	//T=u16, D=1702, LNS server port
#define CSID_C_L2TP_WAN_PORT 					_CFG_L2TP(0x0f01)	//T=u16, D=1702, LNS server port
#define CSID_C_L2TP_WAN_TUN_PASSWORD 					_CFG_L2TP(0x0f02)	//T=str, tunnel password

/* Configuration Items for VPN - L2TP Client */
#define CSID_C_L2TP_CLI_ENABLE              _CFG_L2TP(0x1000)   //T=u8, D=0, enable l2tp client
#define CSID_C_L2TP_CLI_QOS_ENABLE          _CFG_L2TP(0x1001)   //T=u8, D=0, enable l2tp client QoS for quick Echo Req/Res
#define CSID_C_L2TP_CLI_DETECT_LNS          _CFG_L2TP(0x1002)   //T=u8, D=0, type to detect the existence of server
#define CSID_C_L2TP_CLI_USER_ENABLE         _CFG_L2TP(0x1100)   //T=u8, D=0, enable l2tp client user
#define CSID_C_L2TP_CLI_TUNLNAME            _CFG_L2TP(0x1200)   //T=str, tunnel name
#define CSID_C_L2TP_CLI_PEERIP              _CFG_L2TP(0x1300)   //T=ipv4, peer ip
#define CSID_C_L2TP_CLI_USER                _CFG_L2TP(0x1400)   //T=str, user name
#define CSID_C_L2TP_CLI_PASSWORD            _CFG_L2TP(0x1500)   //T=str, user password
#define CSID_C_L2TP_CLI_ROUTE               _CFG_L2TP(0x1600)   //T=str, route
#define CSID_C_L2TP_CLI_CONNECT_TYPE        _CFG_L2TP(0x1700)   //T=u8, D=0, connect type
#define CSID_C_L2TP_CLI_MPPE                _CFG_L2TP(0x1800)   //T=u8, D=0, enable mppe
#define CSID_C_L2TP_CLI_NAT                 _CFG_L2TP(0x1900)   //T=u8, D=0, enable nat
#define CSID_C_L2TP_CLI_CCP_ENABLE	        _CFG_L2TP(0x1a00)	//T=u8, D=0, enable supported CCP for l2tp client
#define CSID_C_L2TP_CLI_PEERIP_STR	        _CFG_L2TP(0x1b00)	//T=str, peer ip

#define CSID_C_L2TP_CLI_DO_AUTH             _CFG_L2TP(0x1d00)   //T=u8, D=0, 0: NoAuth,  1: Auth
#define CSID_C_L2TP_CLI_AUTH_PAP            _CFG_L2TP(0x1e00)   //T=u8, D=0, 0: Default, 1: Accept PAP        2: Reject PAP
#define CSID_C_L2TP_CLI_AUTH_CHAP           _CFG_L2TP(0x1f00)   //T=u8, D=0, 0: Default, 1: Accept CHAP       2: Reject CHAP  
#define CSID_C_L2TP_CLI_AUTH_MSCHAP         _CFG_L2TP(0x3000)   //T=u8, D=0, 0: Default, 1: Accept MSCHAP     2: Reject MSCHAP
#define CSID_C_L2TP_CLI_AUTH_MSCHAPV2       _CFG_L2TP(0x3100)   //T=u8, D=0, 0: Default, 1: Accept MSCHAP-V2  2: Reject MACHAP-V2
#define CSID_C_L2TP_CLI_LOCAL_IP            _CFG_L2TP(0x3200)   //T=ipv4, Local IP
#define CSID_C_L2TP_CLI_REMOTE_IP           _CFG_L2TP(0x3300)   //T=ipv4, Remote IP
#define CSID_C_L2TP_CLI_LCP_TYPE            _CFG_L2TP(0x3400)   //T=u8, D=0,  LCP Echo Type. 0: Auto, 1: Manaul, 2: Disable
#define CSID_C_L2TP_CLI_LCP_INTERVAL        _CFG_L2TP(0x3500)   //T=u8, D=30, LCP Echo Interval
#define CSID_C_L2TP_CLI_LCP_FAILURE         _CFG_L2TP(0x3600)   //T=u8, D=6,  LCP Max Failures
#define CSID_C_L2TP_CLI_IDLETIME			_CFG_L2TP(0x3700)	//T=u16, D=600, max idle time
#define CSID_C_L2TP_CLI_FAILOVER            _CFG_L2TP(0x3800)   //T=u32, D=0,Operation mode,0:Always,1:Failover,2:Load Balance
#define CSID_C_L2TP_CLI_TYPE_FAILOVER       _CFG_L2TP(0x3900)   //T=u32,D=0,the major tunnel type which use this failover tunnel, 0:IPsec,1:PPTP,2:L2TP,3:GRE
#define CSID_C_L2TP_CLI_FAILOVER_IDX        _CFG_L2TP(0x3A00)   //T=u32, D=0,the tunnel idx use this failover tunnel
#define CSID_C_L2TP_CLI_OVER_IPSEC_ENABLE   _CFG_L2TP(0x3B00)   //T=u32, D=0,0:disable, 1:enable
#define CSID_C_L2TP_CLI_PORT                _CFG_L2TP(0x3C00)   //T=u32, D=0,0:auto detect
#define CSID_C_L2TP_CLI_TUN_PASSWORD        _CFG_L2TP(0x3D00)   //T=str, tunnel password
#define CSID_C_L2TP_CLI_WAN                 _CFG_L2TP(0x3E00)   //T=u32, D=0, 0:auto,1:WAN1,2:WAN2,3:WAN3,30:br0/LAN,31:VLAN1,32:VLAN2,33:VLAN3
#define CSID_C_L2TP_CLI_LNS_PORT            _CFG_L2TP(0x3F00)   //T=u32, D=0, 0:use Default value 1701

/* Configuration Items for MULTIWAN Type L2TP */
#define CSID_C_L2TP_MULTIWAN_USER           _CFG_L2TP(0x2100)	//T=str, user name
#define CSID_C_L2TP_MULTIWAN_PASSWORD       _CFG_L2TP(0x2200)	//T=str, user password
#define CSID_C_L2TP_MULTIWAN_P1FIXED_IP		_CFG_L2TP(0x2300)	//T=ipv4, fixed interface ip
#define CSID_C_L2TP_MULTIWAN_P1FIXED_NM     _CFG_L2TP(0x2400)	//T=ipv4, fixed interface netmask
#define CSID_C_L2TP_MULTIWAN_P1FIXED_GW		_CFG_L2TP(0x2500)	//T=ipv4, fixed interface gateway
#define CSID_C_L2TP_MULTIWAN_LNS_SERVER 	_CFG_L2TP(0x2600)	//T=str, LNS server name/ip
#define CSID_C_L2TP_MULTIWAN_P1FIXED_PRIDNS _CFG_L2TP(0x2700)	//T=ipv4, fixed interface primary DNS
#define CSID_C_L2TP_MULTIWAN_P1FIXED_SECDNS _CFG_L2TP(0x2800)	//T=ipv4, fixed interface secondary DNS
#define CSID_C_L2TP_MULTIWAN_CNT_TYPE    	_CFG_L2TP(0x2900)	//T=u16, connect type
#define CSID_C_L2TP_MULTIWAN_CNT_TYPE_UI    _CFG_L2TP(0x2910)   //T=u16, connect type
#define CSID_C_L2TP_MULTIWAN_MTU           	_CFG_L2TP(0x2a00)	//T=u16, D=0,  max transmission unit
#define CSID_C_L2TP_MULTIWAN_IDLETIME      	_CFG_L2TP(0x2b00)	//T=u16, D=600, max idle time
#define CSID_C_L2TP_MULTIWAN_HOLDOFFTIME   	_CFG_L2TP(0x2c00)	//T=u16, D=300, wait time before rebuild
#define CSID_C_L2TP_MULTIWAN_AUTHTYPE      	_CFG_L2TP(0x2d00)	//T=u16, auth type
#define CSID_C_L2TP_MULTIWAN_PHASE1_TYPE   	_CFG_L2TP(0x2e00)	//T=u8, D=0, connect to server in phase1 stage
#define CSID_C_L2TP_MULTIWAN_PORT          	_CFG_L2TP(0x2f00)	//T=u16, D=1702, LNS server port
#define CSID_C_L2TP_MULTIWAN_WAN_PORT 					_CFG_L2TP(0x2f01)	//T=u16, D=1702, LNS server port
#define CSID_C_L2TP_MULTIWAN_TUN_PASSWORD 				_CFG_L2TP(0x2f02)	//T=str, tunnel password

/* for L2TP wantype, user specified DNS information */
#define CSID_C_L2TP_PRIDNS					_CFG_L2TP(0x4100)	//T=ipv4, primary DNS
#define CSID_C_L2TP_MULTIWAN_PRIDNS			_CFG_L2TP(0x4101)	//T=ipv4, primary DNS
#define CSID_C_L2TP_SECDNS					_CFG_L2TP(0x4200)	//T=ipv4, secondary DNS
#define CSID_C_L2TP_MULTIWAN_SECDNS			_CFG_L2TP(0x4201)	//T=ipv4, secondary DNS


/* for L2TP wantype mppe*/
#define CSID_C_L2TP_ENABLE_MPPE                      _CFG_L2TP(0x4300)   //T=u8, D=0, Enable/Disable
#define CSID_C_L2TP_ENABLE_MPPE_40                   _CFG_L2TP(0x4301)   	//T=u8, D=0, Enable/Disable
#define CSID_C_L2TP_ENABLE_MPPE_56                   _CFG_L2TP(0x4302)   	//T=u8, D=0, Enable/Disable
#define CSID_C_L2TP_ENABLE_MPPE_128                  _CFG_L2TP(0x4303)   	//T=u8, D=0, Enable/Disable


/* for L2TPv3 items */
#define CSID_C_L2TPv3_LOCAL_IP              _CFG_L2TP(0x5000)   //T=ipv4, Local Addres
#define CSID_C_L2TPv3_REMOTE_IP             _CFG_L2TP(0x5010)   //T=ipv4, Remote Addres
#define CSID_C_L2TPv3_TUNNEL_ID             _CFG_L2TP(0x5020)   //T=u32, D=0, local Tunnel ID
#define CSID_C_L2TPv3_PEER_TUNNEL_ID        _CFG_L2TP(0x5030)   //T=u32, D=0, Peer Tunnel ID
#define CSID_C_L2TPv3_SPORT                 _CFG_L2TP(0x5040)   //T=u32, D=0, source port
#define CSID_C_L2TPv3_DPORT                 _CFG_L2TP(0x5050)   //T=u32, D=0, destation port
#define CSID_C_L2TPv3_ENC                   _CFG_L2TP(0x5060)   //T=str, D="udp", encapslation
#define CSID_C_L2TPv3_SESSION_ID            _CFG_L2TP(0x5070)   //T=u32, D=0, local Session id
#define CSID_C_L2TPv3_PEER_SESSION_ID       _CFG_L2TP(0x5080)   //T=u32, D=0, Peer session id
#define CSID_C_L2TPv3_VLAN_ID               _CFG_L2TP(0x5090)   //T=u32, D=0, VLAN ID
#define CSID_C_L2TPv3_ENABLE                _CFG_L2TP(0x50A0)    //T=u32, D=0, Enable / Disable
#define CSID_C_L2TPv3_TUNNEL_STATUS         _CFG_L2TP(0x50B0)    //T=u32, D=0, tunnel status
#define CSID_C_L2TPv3_REMOTE_ROUTE_ID       _CFG_L2TP(0x50C0)    //T=str, remote router id
#define CSID_C_L2TPv3_LOCAL_ROUTE_ID        _CFG_L2TP(0x50D0)    //T=str, local router id
#define CSID_C_L2TPv3_REMOTE_HOST_NAME      _CFG_L2TP(0x50E0)    //T=str, remote host name
#define CSID_C_L2TPv3_LOCAL_HOST_NAME       _CFG_L2TP(0x50F0)    //T=str, local host name
#define CSID_C_L2TPv3_SSID			        _CFG_L2TP(0x5100)    //T=u32, SSID, ex br0,br2 enable ==> 0x05
#define CSID_C_L2TPv3_MODE			        _CFG_L2TP(0x5110)    //T=u8, D=0, l2tpv3 only(0) or l2tpv3 over ipsec(1)
#define CSID_C_L2TPv3_END_ID	            _CFG_L2TP(0x5120)   //T=u32,  D=1, remote END id

#define CSID_C_L2TPv3_RADIUSCLIENT_ENABLE   _CFG_L2TP(0x5130)   //T=u8, D=0, Radius client enable for L2TPv3 (NTT)
#define CSID_C_L2TPv3_RADIUSCLIENT_IP       _CFG_L2TP(0x5140)   //T=ipv4, Radius client IP for L2TPv3 (NTT)
#define CSID_C_L2TPv3_RADIUSCLIENT_MASK     _CFG_L2TP(0x5150)   //T=ipv4, Radius client mask for L2TPv3 (NTT) 
#define CSID_C_L2TPv3_RADIUSCLIENT_VLAN_ID  _CFG_L2TP(0x5160)   //T=u32,D=0, Radius client vlan id for L2TPv3 (NTT)

/* Configuration Items for VPN - L2TP Client */
#define CSID_C_L2TP_CLI_HOLDOFFTIME            _CFG_L2TP(0x6000)   //T=u32, D=0, wait time before rebuild
#define CSID_C_L2TP_CLI_MAX_FAIL               _CFG_L2TP(0x6100)   //T=u32, D=0, Maximum reestablished times
#define CSID_C_L2TP_CLI_RWS                    _CFG_L2TP(0x6200)   //T=u32, D=0, Receive Window size
#define CSID_C_L2TP_CLI_ONDEMAND_MAX_IDLE_TIME      _CFG_L2TP(0x6300)  //T=u32, D=0, L2TP client ondemand maximum idle time

/* Status Items for WAN Type L2TP */
#define CSID_S_L2TP_CMD_STATUS				_STA_L2TP(0x0000)	//T=u8, cmd status
#define CSID_S_L2TP_LNS_SERVER_IP			_STA_L2TP(0x0010)	//T=str, LNS server name/ip
#define CSID_S_L2TP_CNT_TIME				_STA_L2TP(0x0100)	//T=u32, connect time
#define CSID_S_L2TP_IF_IP					_STA_L2TP(0x0200)	//T=ipv4, dynamic interface ip
#define CSID_S_L2TP_IF_NM					_STA_L2TP(0x0300)	//T=ipv4, dynamic interface netmask
#define CSID_S_L2TP_IF_GW					_STA_L2TP(0x0400)	//T=ipv4, dynamic interface gateway
#define CSID_S_L2TP_IF_PRIDNS				_STA_L2TP(0x0500)	//T=ipv4, dynamic interface primary DNS
#define CSID_S_L2TP_IF_SECDNS				_STA_L2TP(0x0600)	//T=ipv4, dynamic interface secondary DNS
#define CSID_S_L2TP_CNT_STATUS				_STA_L2TP(0x0700)	//T=u16, connect status
#define CSID_S_L2TP_PKTIN_NUM				_STA_L2TP(0x0800)	//T=u32, number of packet input
#define CSID_S_L2TP_PKTOUT_NUM				_STA_L2TP(0x0900)	//T=u32, number of packet output
#define CSID_S_L2TP_ALTERED					_STA_L2TP(0x0a00)	//T=u8, ui altered
#define CSID_S_L2TP_IF_NAME        			_STA_L2TP(0x3000)   //T=u32, 

/* Status Items for VPN - L2TP Client */
#define CSID_S_L2TP_CLI_ALTERED         _STA_L2TP(0x0011)   //T=u8, D=0, UI Altered
#define CSID_S_L2TP_CLI_PEER_CONNT_ID   _STA_L2TP(0x0012)   //T=u8, D=0, which tunnel try to connect/disconnect 
#define CSID_S_L2TP_CLI_PEER_PID        _STA_L2TP(0x0b00)   //T=u16, D=0, tunnels' pid
#define CSID_S_L2TP_CLI_PEER_VIRTUAL_IP _STA_L2TP(0x0c00)   //T=ipv4, D={0.0.0.0}, virtual IP address assigned from PPP Server
#define CSID_S_L2TP_CLI_PEER_REMOTE_IP  _STA_L2TP(0x0d00)   //T=ipv4, D={0.0.0.0}, virtual IP address of L2TP Server
#define CSID_S_L2TP_CLI_PEER_IFNAME     _STA_L2TP(0x0e00)   //T=str, interface name
#define CSID_S_L2TP_CLI_PEER_ALTERED    _STA_L2TP(0x0f00)   //T=u8, D=0, tunnel's configuration altered
#define CSID_S_L2TP_CLI_PEER_CNT_STATUS _STA_L2TP(0x1000)   //T=u8 connect status

/* Status Items for MULTIWAN Type L2TP */
#define CSID_S_L2TP_MULTIWAN_CMD_STATUS		_STA_L2TP(0x2000)	//T=u8, multiwan cmd status
#define CSID_S_L2TP_MULTIWAN_LNS_SERVER_IP  _STA_L2TP(0x2010)	//T=str, LNS server name/ip
#define CSID_S_L2TP_MULTIWAN_CNT_TIME		_STA_L2TP(0x2100)	//T=u32, connect time
#define CSID_S_L2TP_MULTIWAN_IF_IP			_STA_L2TP(0x2200)	//T=ipv4, dynamic interface ip
#define CSID_S_L2TP_MULTIWAN_IF_NM			_STA_L2TP(0x2300)	//T=ipv4, dynamic interface netmask
#define CSID_S_L2TP_MULTIWAN_IF_GW			_STA_L2TP(0x2400)	//T=ipv4, dynamic interface gateway
#define CSID_S_L2TP_MULTIWAN_IF_PRIDNS		_STA_L2TP(0x2500)	//T=ipv4, dynamic interface primary DNS
#define CSID_S_L2TP_MULTIWAN_IF_SECDNS		_STA_L2TP(0x2600)	//T=ipv4, dynamic interface secondary DNS
#define CSID_S_L2TP_MULTIWAN_CNT_STATUS		_STA_L2TP(0x2700)	//T=u16, connect status
#define CSID_S_L2TP_MULTIWAN_PKTIN_NUM		_STA_L2TP(0x2800)	//T=u32, number of packet input
#define CSID_S_L2TP_MULTIWAN_PKTOUT_NUM		_STA_L2TP(0x2900)	//T=u32, number of packet output
#define CSID_S_L2TP_MULTIWAN_ALTERED		_STA_L2TP(0x2a00)	//T=u8, ui altered
#define CSID_S_L2TP_MULTIWAN_IF_NAME        _STA_L2TP(0x2b00)   //T=u32, 
#define CSID_S_L2TP_CLI_TOTAL_PEER_SUBNET_LIST  _STA_L2TP(0x2c00)       //T=str, L2TP total peer subnet list
#define CSID_S_L2TP_CLI_PORT_NUM                _STA_L2TP(0x2d00)       //T=u32, D=0, tunnel port

/* Status Items L2TPv3 */
#define CSID_S_L2TPv3_ALTER                 _STA_L2TP(0x5000)    //T=u8, alter
#define CSID_S_L2TPv3_STATUS                _STA_L2TP(0x5010)    //T=u8, L2tpv3 ipsec status
#define CSID_S_L2TPv3_TUNNEL_STATUS         _STA_L2TP(0x5020)    //T=u8, L2tpv3 tunnel status
#define CSID_S_L2TPv3_CONTROL_PID           _STA_L2TP(0x5030)    //T=u32,  D=0, l2tpv3_control process id
#define CSID_S_L2TPv3_TUNNEL_ID             _STA_L2TP(0x5040)   //T=u32, D=0, local Tunnel ID
#define CSID_S_L2TPv3_PEER_TUNNEL_ID        _STA_L2TP(0x5050)   //T=u32, D=0, Peer Tunnel ID
#define CSID_S_L2TPv3_SESSION_ID            _STA_L2TP(0x5060)   //T=u32, D=0, local Session id
#define CSID_S_L2TPv3_PEER_SESSION_ID       _STA_L2TP(0x5070)   //T=u32, D=0, Peer session id



#endif //ifndef _CSID_L2TP_H
