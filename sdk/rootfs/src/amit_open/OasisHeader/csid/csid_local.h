/* vi: set sw=4 ts=4: */

/*
 * @file Member ID Definition of Group LOCAL.
 *
 * Moderator: GeorgeWang/RupertLi
 * Group ID: 0x00010000/0x80010000
 */

#ifndef _CSID_LOCAL_H
#define _CSID_LOCAL_H

#include "csid_gid.h"

#define _CFG_LOCAL(x)       (CSID_GIDC_LOCAL|(x))
#define _STA_LOCAL(x)       (CSID_GIDS_LOCAL|(x))

#define LOCAL_WANTYPE_DHCP              0x00
#define LOCAL_WANTYPE_NULL              0x01
#define LOCAL_WANTYPE_BRIDGE            0x02
#define LOCAL_WANTYPE_3G				0x10
#define LOCAL_WANTYPE_IBURST			0x11
#define LOCAL_WANTYPE_WIBRO             0x12
#define LOCAL_WANTYPE_WISP              0x13
#define LOCAL_WANTYPE_WLANCLI           0x14
#define LOCAL_WANTYPE_FLASHOFDM			0x15
#define LOCAL_WANTYPE_S3G             	0x16
#define LOCAL_WANTYPE_FIXED             0x20
#define LOCAL_WANTYPE_PPPOE             0x40
#define LOCAL_WANTYPE_PPPOE_MULTI       0x41
#define LOCAL_WANTYPE_PPPOE_RUSSIAN     0x42
#define LOCAL_WANTYPE_PPPOA             0x50
#define LOCAL_WANTYPE_IPOA_DHCP         0x51
#define LOCAL_WANTYPE_IPOA_FIXED        0x52
#define LOCAL_WANTYPE_PPTP              0x60
#define LOCAL_WANTYPE_L2TP              0x80
#define LOCAL_WANTYPE_DIALUP			0x90
#define LOCAL_WANTYPE_DHCPD 			0x70
#define LOCAL_WANTYPE_AUTO	 			0x110
#define LOCAL_WANTYPE_AZLINK            0x17


#define LOCAL_WAN_STATUS_DISCONNECT     0x00
#define LOCAL_WAN_STATUS_CONNECTING     0x01
#define LOCAL_WAN_STATUS_CONNECT        0x02
#define LOCAL_WAN_STATUS_DISCONNECTING  0x03
#define LOCAL_WAN_STATUS_LEASEFAIL      0x04
#define LOCAL_WAN_STATUS_WAIT_TRAFFIC	0x05
#define LOCAL_WAN_STATUS_AUTH_FAIL		0x06

#define LOCAL_WANMODE_MAIN				0x01
#define LOCAL_WANMODE_FAILOVER			0x02
#define LOCAL_WANMODE_STANDBY_FAILOVER	0x03
	

/* Define maximum httpd auth username password length*/
#define LOCAL_HTTP_USER_PASSWD_LENGTH 32

/* Configuration Items */
#define CSID_C_LOCAL_HOSTNAME           _CFG_LOCAL(0x0001) //T=str, local host name
#define CSID_C_LOCAL_PASSWORD           _CFG_LOCAL(0x0002) //T=str, D="admin", administrator password
#define CSID_C_LOCAL_WANTYPE            _CFG_LOCAL(0x0003) //T=u32, D=0, wan type (default is DHCP)
#define CSID_C_LOCAL_LANIP              _CFG_LOCAL(0x0004) //T=ipv4, D={192.168.123.254}, lan ip
#define CSID_C_LOCAL_LANNM              _CFG_LOCAL(0x0005) //T=ipv4, D={255.255.255.0}, lan subnet mask
#define CSID_C_LOCAL_ROUTER_TYPE		_CFG_LOCAL(0x0006) //T=u32, D=0, router type, 0:normal(1WAN+4LAN) 1:single ethernet
#define CSID_C_LOCAL_SIG_ETHER_PORT     _CFG_LOCAL(0x0007) //T=u32, D=2, For the "only one" ethernet port exists 0:no function 1:WAN 2:LAN
#define CSID_C_LOCAL_HOMECOM_ADMIN_USERNAME		_CFG_LOCAL(0x0008) //T=str, D="admin", UI username
#define CSID_C_LOCAL_HOMECOM_ADMIN_PASSWORD		_CFG_LOCAL(0x0009) //T=str, D="admin", UI password
#define CSID_C_LOCAL_ETHER_AUTO_DETECT	_CFG_LOCAL(0x000A) //T=u32, D=0, 0:stop auto-detection of DHCP, 1: start it.
#define CSID_C_LOCAL_USERNAME			_CFG_LOCAL(0x000B) //T=str, D="admin", Login username	
#define CSID_C_LOCAL_WAN_WEIGHT			_CFG_LOCAL(0x000C) //T=u8, wan weight
#define CSID_C_LOCAL_LOADBALANCE_POLICY	_CFG_LOCAL(0x000D) //T=u32, D=100, load balance policy
#define CSID_C_LOCAL_LOADBALANCE_MODE	_CFG_LOCAL(0x000E) //T=u32, D=0, load balance mode. 0: disable, 1: smart weight, 2: priority, 3: user policy
#define CSID_C_LOCAL_WAN_MAC            _CFG_LOCAL(0x000F) //T=mac, MAC of WAN port

#define CSID_C_LOCAL_WLANENABLE         _CFG_LOCAL(0x0010) //T=u8, D=1, WLAN enable/disable (default is enable)
#define CSID_C_LOCAL_3G_ENABLE			_CFG_LOCAL(0x0011) //T=u32, D=0, 3G wantype enable/disable for UI page showing (default is disable)
#define CSID_C_LOCAL_LANWAN_CHANGE      _CFG_LOCAL(0x0012) //T=u8, D=0, 0:LAN Port 1:WAN Port
#define CSID_C_LOCAL_GUEST_PASSWORD     _CFG_LOCAL(0x0013) //T=str, D="guest", guest password
#define CSID_C_LOCAL_PASSWORD_TYPE      _CFG_LOCAL(0x0014) //T=u8, D=0, password type 0:admin 1:guest
#define CSID_C_LOCAL_INFO_ROM_ID		_CFG_LOCAL(0x0015) //T=str, romid check use
#define CSID_C_LOCAL_PREV_ETHER_WANTYPE		_CFG_LOCAL(0x0016) //T=u32, D=0, previous wan type (default is DHCP)
#define CSID_C_LOCAL_PREV_WIRELESS_WANTYPE	_CFG_LOCAL(0x0017) //T=u32, D=16, previous wan type (default is 3G)
#define CSID_C_LOCAL_WLAN2ENABLE         _CFG_LOCAL(0x0018) //T=u8, D=1, WLAN2 enable/disable (default is enable)
#define CSID_C_LOCAL_GUEST_USERNAME      _CFG_LOCAL(0x0019) //T=str, D="guest", guest username
#define CSID_C_LOCAL_SAVE_USERNAME       _CFG_LOCAL(0x001a) //T=str, save login username for mutil login
#define CSID_C_LOCAL_GET_IFNAME       	 _CFG_LOCAL(0x001b) //T=str, get interface name
#define CSID_C_LOCAL_BONJOUR_DISABLE     _CFG_LOCAL(0x001c) //T=u8, D=0, 0:Enable 1:Disable
#define CSID_C_LOCAL_DYNAMIC_LANIP       _CFG_LOCAL(0x001d) //T=ipv4, D={192.168.123.254}, dynamic LAN IP 
#define CSID_C_LOCAL_EN_DYNAMIC_LANIP    _CFG_LOCAL(0x001e) //T=u8, D=0, enable/disable dynamic LAN IP 
#define CSID_C_LOCAL_INET_SWITCH	     _CFG_LOCAL(0x001f) //T=u32,D=0, enable/disable inet switch 
#define CSID_C_LOCAL_DEVICENAME		     _CFG_LOCAL(0x002C) //T=str, Router's Device Name
#define CSID_C_LOCAL_BPS_RECORD_TIME     _CFG_LOCAL(0x002D) //T=u32, bps's record time for traffic_stat(Unit: Sec)
#define CSID_C_LOCAL_BPS_RECORD_MAX_TIME     _CFG_LOCAL(0x002E) //T=u32, bps's max record time for traffic_stat(Unit: Sec)
#define CSID_C_LOCAL_DNRD_ENABLE         _CFG_LOCAL(0x002F) //T=u8, dnrd enable

/* Time control */
#define CSID_C_LOCAL_RESET_TIME         _CFG_LOCAL(0x0020) //T=u32, D=6, press RESET button time
#define CSID_C_LOCAL_RESET_TIME_MAX     _CFG_LOCAL(0x0029) //T=u32, D=6, press RESET button time
#define CSID_C_LOCAL_WPS_LOCK_TIME      _CFG_LOCAL(0x0021) //T=u32, D=120, WPS enable time
#define CSID_C_LOCAL_WIFI_ONOFF_TIME    _CFG_LOCAL(0x0022) //T=u32, D=3, press WiFi on/off button time

/* WAN interface selection*/ /*refer to CDD531 */
#define CSID_C_LOCAL_WAN_INTERFACE		_CFG_LOCAL(0x0023) //T=u32, D=0, 0:ADSL 1:Ethernet 2:3G 3:VDSL

/* USB type selection*/
#define CSID_C_LOCAL_USBTYPE            _CFG_LOCAL(0x0024) //T=u32, D=0, 0:Storage 1:Printer 2:3G

/* WPS button time control */
#define CSID_C_LOCAL_WPS_TIME			_CFG_LOCAL(0x0025) //T=u32, D=3, press WPS button time (MIN time)
#define CSID_C_LOCAL_WPS_TIME_MAX	    _CFG_LOCAL(0x002a) //T=u32, D=6, press WPS button time (MAC time)

#define CSID_C_LOCAL_WANTYPE_RES        _CFG_LOCAL(0x0026) //T=u32, D=0, wan type (default is DHCP)

/* DIP Switch mode */
#define CSID_C_LOCAL_WANTYPE_DSL_MODE   _CFG_LOCAL(0x0027) //T=u32, D=0, wan type (default is DHCP)
#define CSID_C_LOCAL_WANTYPE_3G_MODE    _CFG_LOCAL(0x0028) //T=u32, D=0, wan type (default is 3G)

/* Russian mode (Dual access) switch */
#define CSID_C_LOCAL_WANTYPE_RUSSIAN_MODE_DISABLE    _CFG_LOCAL(0x002B) //T=u32, D=1, wan type Russian mode (dual access)

/*Device is in AP Mode, refer to CDM532RP */
#define CSID_C_LOCAL_AP_IP				_CFG_LOCAL(0x0030) //T=ipv4, D={192.168.123.254}
#define CSID_C_LOCAL_AP_NM				_CFG_LOCAL(0x0031) //T=ipv4, D={255.255.255.0}
#define CSID_C_LOCAL_AP_GW				_CFG_LOCAL(0x0032) //T=ipv4, AP's GW
#define CSID_C_LOCAL_AP_PRIDNS			_CFG_LOCAL(0x0033) //T=ipv4, AP's primary dns
#define CSID_C_LOCAL_AP_SECDNS			_CFG_LOCAL(0x0034) //T=ipv4, AP's secondary dns
#define CSID_C_LOCAL_AP_CNT_TYPE		_CFG_LOCAL(0x0035) //T=u32, D=1,  0:Dynamic (DHCP Client) , 1:Fixed IP
#define CSID_C_LOCAL_AP_HOSTNAME		_CFG_LOCAL(0x0036) //T=str, AP's Hostname
#define CSID_C_LOCAL_AP_DEVICENAME		_CFG_LOCAL(0x0037) //T=str, AP's Device Name

/* Handle min_free_kbytes */
#define CSID_C_LOCAL_MIN_FREE_SWITCH	_CFG_LOCAL(0x0038) //T=u32, D=0, handle min_free_kbytes

/* Handle MiiiCaSa Switch */
#define CSID_C_LOCAL_MIIICASA_ENABLE	_CFG_LOCAL(0x0039) //T=u32, D=0, 0:disable, 1:enable

/* Handle  Status LED */
#define CSID_C_LOCAL_STATUS_LED_SWITCH	_CFG_LOCAL(0x003a) //T=u32, D=0, 0:disable, 1:enable

/*ONO username/password*/
#define CSID_C_LOCAL_ONO_USERNAME      _CFG_LOCAL(0x003B) //T=str, D="onoemp", ono username
#define CSID_C_LOCAL_ONO_PASSWORD      _CFG_LOCAL(0x003C) //T=str, D="d3sc0n0", ono password

/*Load balance smart weight check interval*/
#define CSID_C_LOCAL_LOADBALANCE_CHECK_INTERVAL _CFG_LOCAL(0x003D)  //T=u32, D=60, load balance smart weight check interval

/*Used to check if default password has been changed by UI*/
#define CSID_C_LOCAL_DEFAULT_PASSWORD  _CFG_LOCAL(0x003E) //T=str, D="admin", default password in reset.txt
#define CSID_C_LOCAL_IINET_TR_PASSWORD _CFG_LOCAL(0x003F) //T=str, D="Netcomm-iiNet", password to access TR-069

/*Device is in STA Mode, refer to CDM532RP */
#define CSID_C_LOCAL_STA_IP				_CFG_LOCAL(0x0040) //T=ipv4, D={192.168.123.254}
#define CSID_C_LOCAL_STA_NM				_CFG_LOCAL(0x0041) //T=ipv4, D={255.255.255.0}
#define CSID_C_LOCAL_STA_GW				_CFG_LOCAL(0x0042) //T=ipv4, STA's GW
#define CSID_C_LOCAL_STA_PRIDNS			_CFG_LOCAL(0x0043) //T=ipv4, STA's primary dns
#define CSID_C_LOCAL_STA_SECDNS			_CFG_LOCAL(0x0044) //T=ipv4, STA's secondary dns
#define CSID_C_LOCAL_STA_CNT_TYPE		_CFG_LOCAL(0x0045) //T=u32, D=1,  0:Dynamic (DHCP Client) , 1:Fixed IP
#define CSID_C_LOCAL_STA_HOSTNAME		_CFG_LOCAL(0x0046) //T=str, STA's Hostname
#define CSID_C_LOCAL_STA_DEVICENAME		_CFG_LOCAL(0x0047) //T=str, STA's Device Name

/* Royalty Time */
#define CSID_C_LOCAL_ROYALTY_TIME		_CFG_LOCAL(0x0048) //T=u32, D=1800, Royalty Time, default sec.

#define CSID_C_LOCAL_DEVICENAME_SUFFIX	_CFG_LOCAL(0x0049) //T=str, Suffix of Device Name for Host Resolution

/* for Mutil & Single WAN Switch*/
#define CSID_C_LOCAL_MUTIL_SINGLE_WAN_SWITCH	_CFG_LOCAL(0x004a) //T=u32, D=0, Mutil & Single WAN Switch, 0:Mutil 1:Single

/* for HOMECOM normal user */
#define CSID_C_LOCAL_HOMECOM_NORMAL_USERNAME	_CFG_LOCAL(0x0050) //T=str, UI username
#define CSID_C_LOCAL_HOMECOM_NORMAL_PASSWORD	_CFG_LOCAL(0x0060) //T=str, UI password
#define CSID_C_LOCAL_HOMECOM_NORMAL_AUTHORITY	_CFG_LOCAL(0x0070) //T=str, D="0", User authority

/* for CLI login configuration */
#define CSID_C_LOCAL_CLI_ACCOUNT           _CFG_LOCAL(0x0075) //T=str, D="", CLI username
#define CSID_C_LOCAL_CLI_PASSWORD           _CFG_LOCAL(0x0076) //T=str, D="", CLI password
#define CSID_C_LOCAL_ADMIN_ALIAS           _CFG_LOCAL(0x0077) //T=str, D="admin", customize admin account name
#define CSID_C_LOCAL_SSH_ENABLE		   _CFG_LOCAL(0x0078) //T=u32, D=0,  0 disable dropbear ssh , 1 enable
#define CSID_C_LOCAL_SSH_PORT		   _CFG_LOCAL(0x0079) //T=u32, D=22,  ssh service port
#define CSID_C_LOCAL_SSH_ALLOW_WAN		   _CFG_LOCAL(0x007A) //T=u32, D=0, Whether to allow connect to sshd from WAN side (Need reboot)
#define CSID_C_LOCAL_SSH_DENY_LAN		   _CFG_LOCAL(0x007B) //T=u32, D=0, Whether to deny connect to sshd from LAN side (Need reboot)

/* For WISP TxRx bridge function */
#define CSID_C_LOCAL_TXRXBRIDGE_ENABLE          _CFG_LOCAL(0x0080) //T=u32, D=0, TxRx Bridge enable/disable
#define CSID_C_LOCAL_TXRXBRIDGE_ACCESS_UI_IP    _CFG_LOCAL(0x0081) //T=ipv4, access ui ip
#define CSID_C_LOCAL_TXRXBRIDGE_MODE            _CFG_LOCAL(0x0082) //T=u32, D=1, TxRx Client/Server mode

#define CSID_C_LOCAL_WAN_ENABLE				_CFG_LOCAL(0x0083) //T=u32, D=1, used for adsl pvc0 in Multiwan, 0 disable, 1 enable
#define CSID_C_LOCAL_MULTIWAN_MODE			_CFG_LOCAL(0x0084) //T=u32, D=1, Multiwan Mode, 0 normal, 1 mutual
#define CSID_C_LOCAL_MULTIWAN_VIRTUAL_WAN_ENABLE	_CFG_LOCAL(0x0085) //T=u32, D=1, Enable different vids used in one ethernet port, 0 disable, 1 enable
#define CSID_C_LOCAL_HAVE_ADSL				_CFG_LOCAL(0x0086) //T=u8, D=0, machine has adsl or not, 0 no adsl, 1 have adsl
#define CSID_C_LOCAL_HAVE_DUALBAND			_CFG_LOCAL(0x0087) //T=u8, D=0, machine has AP2(5G) or not, 0 no AP2, 1 have AP2
#define CSID_C_LOCAL_WAN_DISCOVER_DISABLE	_CFG_LOCAL(0x0088) //T=u8, D=0, wan discover, 1 disable, 0 enable
#define CSID_C_LOCAL_WAN_ETHER_AUTOBAK_ENABLE	_CFG_LOCAL(0x0089) //T=u8, D=0, adsl WAN failover to ether WAN, 1 enable, 0 disable

#define CSID_C_LOCAL_WAN_NAME				_CFG_LOCAL(0x0090) //T=str, WAN name, used by ZyXEL UI
#define CSID_C_LOCAL_MULTIWAN_NAME			_CFG_LOCAL(0x00a0) //T=str, WAN name, used by ZyXEL UI

#define CSID_C_LOCAL_WAN_WEIGHT_TMP         _CFG_LOCAL(0x00b0) //T=u8, local wan weight for UI to show when load balance mode=2(priority)
#define CSID_C_LOCAL_MULTIWAN_WEIGHT_TMP    _CFG_LOCAL(0x00c0) //T=u8, multiwan weight for UI to show when load balance mode=2(priority)

/* Reserved 16 items for Multi-WAN,  X is from 1 .... */
#define CSID_C_LOCAL_MULTIWAN_ENABLE		_CFG_LOCAL(0x0100) //T=u32, D=0, Multi WAN Enbale/Disable, Starting from MULTIWAN, Next will be WAN2 , ...
#define CSID_C_LOCAL_MULTIWAN_CLONE_MAC		_CFG_LOCAL(0x0110) //T=mac, Clone MAC of MULTIWAN port
#define CSID_C_LOCAL_MULTIWAN_WANTYPE       _CFG_LOCAL(0x0120) //T=u32, D=0, wan type (default is DHCP)
#define CSID_C_LOCAL_MULTIWAN_WEIGHT		_CFG_LOCAL(0x0130) //T=u32, define multiwan weight value

/* Half-Bridging Mode */
#define CSID_C_LOCAL_HALFBRIDGING_ENABLE    _CFG_LOCAL(0x0141) //T=u8,  Half-Bridging mode 1:enable, 0:disable

/* Reset Button */
#define CSID_C_LOCAL_RESETBUTTON_DISABLE    _CFG_LOCAL(0x0142) //T=u8,  Reset Button 1:enable, 0:disable

/* Reserved 16 items for VoIP-WAN */
#define CSID_C_LOCAL_VOIPWAN_WANTYPE       _CFG_LOCAL(0x0150) //T=u32, D=0, wan type (default is DHCP)

/* Reserved 16 items for Port Speed Setting */
#define CSID_C_LOCAL_PORT_NUM				_CFG_LOCAL(0x0160) //T=u32, D=0, Port Number (0, 1, 2, ...)
#define CSID_C_LOCAL_PORT_SPEED				_CFG_LOCAL(0x0167) //T=u32, D=0, Port Speed (10M, 100M, 1000M, ...)

/* Reserved 16 items for Ondemand  */
#define CSID_C_LOCAL_ONDEMAND_IGNO_PKT      _CFG_LOCAL(0x0170) //T=str, Record Ignore packet for ondemand (1.2.3.4/24,8080,6)

#define CSID_C_LOCAL_2ND_LANIP            _CFG_LOCAL(0x0190) //T=ipv4, D={192.168.2.1}, 2ndlan ip
#define CSID_C_LOCAL_2ND_LANNM            _CFG_LOCAL(0x0191) //T=ipv4, D={255.255.255.0}, 2ndlan subnet mask

#define CSID_C_LOCAL_BRIDGE_DHCP_ENABLE     _CFG_LOCAL(0x0192) //T=u32, D=0, Br0 DHCP Client Enable
#define CSID_C_LOCAL_LANGW                  _CFG_LOCAL(0x0193) //T=ipv4, D={0.0.0.0}, LAN Default Gateway for IO-DATA Setting
#define CSID_C_LOCAL_LANDNS                 _CFG_LOCAL(0x0194) //T=ipv4, D={0.0.0.0}, LAN DNS for IO-DATA Setting

/*second LAN IP*/
#define CSID_C_LOCAL_SECOND_LANIP_ENABLE     _CFG_LOCAL(0x0195) //T=u32, D=0, second LAN IP C_LOCAL_2ND_LANIP

/* IGMP snooping Index for Multiwan appliance */
#define CSID_C_LOCAL_MULTIWAN_IGMP_INTERFACE		_CFG_LOCAL(0x0199) //T=u32, define multiwan igmp index .99 localwan. 0-15 
/*  Reserved 16 items for specify each Muti-Wan interface IGMP version support */
#define CSID_C_LOCAL_MULTIWAN_IGMP_VERSION	_CFG_LOCAL(0x0200) //T=u32, define multiwan igmp version 0 auto. 1,2,3
#define CSID_C_LOCAL_WAN_IGMP_VERSION		_CFG_LOCAL(0x0210) //T=u32, define local wan IGMP version
#define CSID_C_LOCAL_WAN_ALIAS_ENABLE		_CFG_LOCAL(0x0220) //T=u8, D=0, wan alias enable/disable
#define CSID_C_LOCAL_WAN_ALIAS_IP			_CFG_LOCAL(0x0221) //T=ipv4, D={10.0.0.1}, wan alias ip

/* Wireless Mode */
#define CSID_C_LOCAL_WAN_TYPE_DISABLE       _CFG_LOCAL(0x0230) //T=u8, disable all WAN type, 0:enable WAN type 1:disable WAN type
#define CSID_C_LOCAL_WAN_BECOMES_LAN        _CFG_LOCAL(0x0231) //T=u8, WAN port becomes LAN port, 0:disable 1:enable
#define CSID_C_LOCAL_NAT_DISABLE            _CFG_LOCAL(0x0232) //T=u8, disable NAT, 0:enable NAT 1:disable NAT
#define CSID_C_LOCAL_HW_NAT_DISABLE         _CFG_LOCAL(0x0233) //T=u8, disable HW NAT, 0:enable 1:disable
#define CSID_C_LOCAL_WAN_BROADCAST_DISABLE  _CFG_LOCAL(0x0234) //T=u8, D=0, disable wan to recieve Broadcast pkt, 0:enable 1:disable
#define CSID_C_LOCAL_SINGLE_ETH_PORT_AUTOSEL _CFG_LOCAL(0x0235) //T=u8, auto select LAN/WAN for single ether port
#define CSID_C_LOCAL_HW_NAT_ENABLE           _CFG_LOCAL(0x0236) //T=u8, enble HW NAT, 1:enable 0:disable
#define CSID_C_LOCAL_NAT_SESSION_MAX         _CFG_LOCAL(0x0237) //T=u8, D=0, nat nf conntrack max
#define CSID_C_LOCAL_NAT_FUNCTION_DISABLE    _CFG_LOCAL(0x0238) //T=u8, disable NAT function, 0:enable NAT 1:disable NAT

//IGMP For PPP
#define CSID_C_LOCAL_WAN_IGMP_VIP_DISABLE		_CFG_LOCAL(0x0240) //T=u8, D=0, Disable/Enable virtual IP for IGMP interface
#define CSID_C_LOCAL_WAN_IGMP_VIP		_CFG_LOCAL(0x0241) //T=ipv4, D={10.166.211.32}, virtual IP for IGMP interface

#define CSID_C_LOCAL_VOD_RTSP_ENABLE		_CFG_LOCAL(0x0250) //T=u8, D=0, Enable/Disable RTSP ALG
#define CSID_C_LOCAL_VOD_RTSP_PORTS		_CFG_LOCAL(0x0251) //T=str, D="554,5544", Ports for RTSP ALG and it is VOD feature

//Recent
#define CSID_C_LOCAL_RECENT_ENABLE		_CFG_LOCAL(0x0252) //T=u32, D=0, Disable BT Recent
#define CSID_C_LOCAL_RECENT_TCP_CHECK_SECOND		_CFG_LOCAL(0x0253) //T=u32, D=0, Recent tcp check second
#define CSID_C_LOCAL_RECENT_TCP_HITCOUNT		_CFG_LOCAL(0x0254) //T=u32, D=0, Recent tcp hitcount
#define CSID_C_LOCAL_RECENT_UDP_CHECK_SECOND		_CFG_LOCAL(0x0255) //T=u32, D=0, Recent udp check second
#define CSID_C_LOCAL_RECENT_UDP_HITCOUNT		_CFG_LOCAL(0x0256) //T=u32, D=0, Recent udp hitcount
#define CSID_C_LOCAL_HANDLE_RECENT_ENABLE		_CFG_LOCAL(0x0257) //T=u32, D=0, Disable handle_recent

/* Share port User management */
#define CSID_C_LOCAL_SP_USERNAME      _CFG_LOCAL(0x0260) //T=str, Share port account user name, from 0 ~ 15
#define CSID_C_LOCAL_SP_PASSWORD      _CFG_LOCAL(0x0270) //T=str, Share port account password, from 0 ~ 15
#define CSID_C_LOCAL_SP_ACCESS_PATH1      _CFG_LOCAL(0x0280) //T=str, Share port user access path 1, from 0 ~ 15
#define CSID_C_LOCAL_SP_ACCESS_PATH2      _CFG_LOCAL(0x0290) //T=str, Share port user access path 2, from 0 ~ 15
#define CSID_C_LOCAL_SP_ACCESS_PATH3      _CFG_LOCAL(0x02a0) //T=str, Share port user access path 3, from 0 ~ 15
#define CSID_C_LOCAL_SP_ACCESS_PATH4      _CFG_LOCAL(0x02b0) //T=str, Share port user access path 4, from 0 ~ 15
#define CSID_C_LOCAL_SP_ACCESS_PATH5      _CFG_LOCAL(0x02c0) //T=str, Share port user access path 5, from 0 ~ 15
#define CSID_C_LOCAL_SP_ACCESS_PATH1_PERMISSION      _CFG_LOCAL(0x02d0) //T=u8, D=0, Share port user access path 1 permission, 0:read 1:read/write
#define CSID_C_LOCAL_SP_ACCESS_PATH2_PERMISSION      _CFG_LOCAL(0x02e0) //T=u8, D=0, Share port user access path 2 permission, 0:read 1:read/write
#define CSID_C_LOCAL_SP_ACCESS_PATH3_PERMISSION      _CFG_LOCAL(0x02f0) //T=u8, D=0, Share port user access path 3 permission, 0:read 1:read/write
#define CSID_C_LOCAL_SP_ACCESS_PATH4_PERMISSION      _CFG_LOCAL(0x0300) //T=u8, D=0, Share port user access path 4 permission, 0:read 1:read/write
#define CSID_C_LOCAL_SP_ACCESS_PATH5_PERMISSION      _CFG_LOCAL(0x0310) //T=u8, D=0, Share port user access path 5 permission, 0:read 1:read/write

#define CSID_C_LOCAL_SP_WEB_ACCESS_ENABLE      _CFG_LOCAL(0x0320) //T=u8, D=0, Share port web access enable, 0:disable 1:enable
#define CSID_C_LOCAL_SP_REMOTE_ACCESS_ENABLE      _CFG_LOCAL(0x0321) //T=u8, D=0, Share port remote access enable, 0:disable 1:enable
#define CSID_C_LOCAL_SP_HTTP_ACCESS_PORT      _CFG_LOCAL(0x0322) //T=u32, Share port http access port
#define CSID_C_LOCAL_SP_HTTPS_ACCESS_PORT      _CFG_LOCAL(0x0323) //T=u32, Share port https access port

/* HNAP */
#define CSID_C_LOCAL_HNAP_ENABLE            _CFG_LOCAL(0x0324) 	//T=u8, D=0, HNAP enable
#define CSID_C_LOCAL_HNAP_ACCESS_PORT       _CFG_LOCAL(0x0325) 	//T=u32, access port of hnap

#define CSID_C_LOCAL_SP_ACCESS_PATH1_ALIAS      _CFG_LOCAL(0x0330) //T=u8, D=0, Share port user access path 1 alias
#define CSID_C_LOCAL_SP_ACCESS_PATH2_ALIAS      _CFG_LOCAL(0x0340) //T=u8, D=0, Share port user access path 2 alias
#define CSID_C_LOCAL_SP_ACCESS_PATH3_ALIAS      _CFG_LOCAL(0x0350) //T=u8, D=0, Share port user access path 3 alias
#define CSID_C_LOCAL_SP_ACCESS_PATH4_ALIAS      _CFG_LOCAL(0x0360) //T=u8, D=0, Share port user access path 4 alias
#define CSID_C_LOCAL_SP_ACCESS_PATH5_ALIAS      _CFG_LOCAL(0x0370) //T=u8, D=0, Share port user access path 5 alias

#define CSID_C_UPNP_LOOSE_IPRANGE_ENABLE      _CFG_LOCAL(0x03b0) //T=u32, D=0, miniupnpd port range 1024~65536

#define CSID_C_LOCAL_MULTIWAN_HALFBRIDGING_ENABLE    _CFG_LOCAL(0x03f0) //T=u32, D=0, Half-Bridging mode 1:enable, 0:disable
/* Phy Port link setting */

//#define CSID_C_LOCAL_PORT_LINKMENABLE	_CFG_LOCAL(0x0400) //T=u8, D=0, default disable
//#define CSID_C_LOCAL_PORT_NUMBER			_CFG_LOCAL(0x0401) //T=u8, D=5, default 5(0-->4)
//using CSID_S_SYS_PORT_COUNT_ETH to judge NUMBER
/* Reserved 24 items  for 0 --> 8 Physical Port mapping */
#define CSID_C_LOCAL_PORT0_ENABLE     _CFG_LOCAL(0x0402) //T=u8, D=0, default disable
#define CSID_C_LOCAL_PORT0_SPEEDMODE  _CFG_LOCAL(0x0403) //T=u8, D=0, 0:10 1:100 2:1000 Mbps 3:auto
#define CSID_C_LOCAL_PORT0_DUPLEXMODE _CFG_LOCAL(0x0404) //T=u8, D=0, 0:half 1:full mode 2:auto

#define CSID_C_LOCAL_MULTIWAN_ONDEMAND_IGNO_PKT   _CFG_LOCAL(0x0500) //T=str, Record Ignore packet for ondemand (1.2.3.4/24,8080,6) reserved for 16 WANs, each WAN has 16 records, from 0x0500 to 0x05ff

#define CSID_C_LOCAL_WAN1_INDEX	_CFG_LOCAL(0x0600) //T=u8, D=0, record WAN index picked to first WAN
#define CSID_C_LOCAL_WAN2_INDEX	_CFG_LOCAL(0x0601) //T=u8, D=0, record WAN index picked to second WAN
#define CSID_C_LOCAL_WAN3_INDEX	_CFG_LOCAL(0x0602) //T=u8, D=0, record WAN index picked to third WAN

#define CSID_C_LOCAL_WAN1_OP_MODE _CFG_LOCAL(0x0610) //T=u8, D=0, record WAN index operation mode to first WAN
#define CSID_C_LOCAL_WAN2_OP_MODE _CFG_LOCAL(0x0611) //T=u8, D=0, record WAN index operation mode to second WAN
#define CSID_C_LOCAL_WAN3_OP_MODE _CFG_LOCAL(0x0612) //T=u8, D=0, record WAN index operation mode to third WAN

/* multiwan IP alias */
#define CSID_C_LOCAL_MULTIWAN_ALIAS_ENABLE  _CFG_LOCAL(0x0620) //T=u8, D=0, multiwan alias enable/disable
#define CSID_C_LOCAL_MULTIWAN_ALIAS_IP      _CFG_LOCAL(0x0630) //T=ipv4, D={10.0.0.1}, multiwan alias ip

#define CSID_C_LOCAL_WIFI1_VLAN_ENABLE	_CFG_LOCAL(0x0700) //T=u8, D=0, WIFI1 VLAN ID enable?
#define CSID_C_LOCAL_WIFI2_VLAN_ENABLE	_CFG_LOCAL(0x0710) //T=u8, D=0, WIFI2 VLAN ID enable?
#define CSID_C_LOCAL_WIFI1_VLAN_ID			_CFG_LOCAL(0x0720) //T=u32, D=3, WIFI1_VLAN ID, 
#define CSID_C_LOCAL_WIFI2_VLAN_ID			_CFG_LOCAL(0x0730) //T=u32, D=3, WIFI2_VLAN ID

#define CSID_C_LOCAL_WANCHK_ENABLE		    _CFG_LOCAL(0x0800) //T=u32, D=0, wanchk enable 0:disable 1:enable
#define CSID_C_LOCAL_WANCHK_IDLE		    _CFG_LOCAL(0x0801) //T=u32, D=0, wanchk idle 0:disable 1:enable
#define CSID_C_LOCAL_HAVE_MULTIWAN2         _CFG_LOCAL(0x0802) //T=u8, D=0, HAVE_MULTIWAN2? 0: no, 1: yes
#define CSID_C_LOCAL_HAVE_ANTENNA_SWITCH	_CFG_LOCAL(0x0803) //T=u8, D=0, AP have antenna switch, 0: no, 1:yes
#define CSID_C_LOCAL_MULTIWANCHK_ENABLE		_CFG_LOCAL(0x0810) //T=u32, D=0, multiwanchk enable 0:disable 1:enable
#define CSID_C_LOCAL_MULTIWANCHK_IDLE		_CFG_LOCAL(0x0820) //T=u32, D=0, multiwanchk idle 0:disable 1:enable
#define CSID_C_LOCAL_MULTIWAN_MAPPING_INDEX		_CFG_LOCAL(0x0830) //T=u32, D=0, multiwanchk wan mapping wantype index
#define CSID_C_LOCAL_HAVE_PREFERRED_ANTENNA     _CFG_LOCAL(0x0840) //T=u32, D=0, 0:No 3G preferred antenna, 1:Have 3G preferred antenna 

/*C_LOCAL_PASSWORD customize*/
#define CSID_C_LOCAL_PASSWORD_CUSTOMIZE_FLAG	_CFG_LOCAL(0x0900) //T=u32, D=0, password customize

/*IPV6 MLD*/
#define CSID_C_LOCAL_WAN_MLD_VERSION		_CFG_LOCAL(0x6010) //T=u32, define local wan MLD version

/* Status Items */
#define CSID_S_LOCAL_LOGIN_IP           _STA_LOCAL(0x0001) //T=ipv4, ip address of logged in user
#define CSID_S_LOCAL_WAN_IP             _STA_LOCAL(0x0002) //T=ipv4, wan ip
#define CSID_S_LOCAL_WAN_NM             _STA_LOCAL(0x0003) //T=ipv4, wan netmask
#define CSID_S_LOCAL_WAN_GW             _STA_LOCAL(0x0004) //T=ipv4, wan gateway
#define CSID_S_LOCAL_WAN_PRIDNS         _STA_LOCAL(0x0005) //T=ipv4, wan primary dns
#define CSID_S_LOCAL_WAN_SECDNS         _STA_LOCAL(0x0006) //T=ipv4, wan secondary dns
#define CSID_S_LOCAL_WAN_STATUS         _STA_LOCAL(0x0007) //T=u16, wan status
#define CSID_S_LOCAL_WANTYPE_ALTERED    _STA_LOCAL(0x0008) //T=u32, ui altered
#define CSID_S_LOCAL_WANSET_ALTERED     _STA_LOCAL(0x0009) //T=u32, ui altered
#define CSID_S_LOCAL_LAN_ALTERED        _STA_LOCAL(0x000a) //T=u32, ui altered
#define CSID_S_LOCAL_WAN_IF_NAME        _STA_LOCAL(0x000b) //T=str, the interface name of wan, ppp0, eth1, ixp1
#define CSID_S_LOCAL_CNT_TIME           _STA_LOCAL(0x000c) //T=u32, the connect time
#define CSID_S_LOCAL_DETECT_WAN			_STA_LOCAL(0x000d) //T=u32, auto wantype detect result,0=no detect,1=dhcp,2=pppoe
#define CSID_S_LOCAL_WAN_CNT_FORCE		_STA_LOCAL(0x000e) //T=u32, Force WAN trigger conncet,0=disable,1=enable
#define CSID_S_LOCAL_SEP_CFG_ALTERED    _STA_LOCAL(0x000f) //T=u32, D=0, single ethernet port configuration changing, 0:no change, 1:

#define CSID_S_LOCAL_ROOT_DNS			_STA_LOCAL(0x0010) //T=ipv4, D={192.203.230.10}, ns.nasa.gov, 	Mountain View, California, U.S.
#define CSID_S_LOCAL_WAN_PORT			_STA_LOCAL(0x0020) //T=u8, WAN port cable connected, 0=disconnect, 1=connected
#define CSID_S_LOCAL_LAN_PORT			_STA_LOCAL(0x0030) //T=u8, LANx cable connected, 0=disconnect, 1=connected

/* for AGP Testing */
#define CSID_S_LOCAL_LOGINED			_STA_LOCAL(0x0040) //T=u8, logined or not?  0=not, 1=yes
/* for compare LAN MASK change */
#define CSID_S_LOCAL_LANNM              _STA_LOCAL(0x0041) //T=ipv4, lan subnet mask

#define CSID_S_LOCAL_RESETCNT			_STA_LOCAL(0x0042) //T=u16, for record reset_cnt press times
#define CSID_S_LOCAL_WPSCNT 			_STA_LOCAL(0x0044) //T=u16, for record wps_cnt press times

/*Used to check if default password has been changed by UI*/
#define CSID_S_LOCAL_CHG_PASSWORD		_STA_LOCAL(0x0043) //T=u8, D=0, 0: Default password, 1: Password changed

/* for on-demand */
#define CSID_S_LOCAL_ONDEMAND_TIMEOUT 	_STA_LOCAL(0x0045) //T=u32, record the current timeout value when ondemand

/* for IPv6 */
#define CSID_S_LOCAL_V6WANTYPE_ALTERED  _STA_LOCAL(0x0046) //T=u8, IPv6 config altered 

/* for WAN Discover Status */
#define CSID_S_LOCAL_WAN_DISCOVER_STATUS _STA_LOCAL(0x0047) //T=u8, D=0, 0: Not ready, 1: Processing, 2: Finish, 3: Disable, 4: WAN down

/* WAN Connect/Disconnect Alter */
#define CSID_S_WAN_CONNECTED_ALTERED     _STA_LOCAL(0x0048) //T=u8, D=0, WAN Connect alter
#define CSID_S_WAN_DISCONNECTED_ALTERED  _STA_LOCAL(0x0049) //T=u8, D=0, WAN Disconnect alter

/* Load balance Altered */
#define CSID_S_LOCAL_LOADBALANCE_ALTERED _STA_LOCAL(0x004a) //T=u8, D=0, load balance altered

/*Device is in AP Mode, refer to CDM532RP */
#define CSID_S_LOCAL_AP_IP				_STA_LOCAL(0x0050) //T=ipv4, AP's IP
#define CSID_S_LOCAL_AP_NM				_STA_LOCAL(0x0051) //T=ipv4, AP's Netmask
#define CSID_S_LOCAL_AP_GW				_STA_LOCAL(0x0052) //T=ipv4, AP's GW
#define CSID_S_LOCAL_AP_PRIDNS			_STA_LOCAL(0x0053) //T=ipv4, AP's primary dns
#define CSID_S_LOCAL_AP_SECDNS			_STA_LOCAL(0x0054) //T=ipv4, AP's secondary dns
#define CSID_S_LOCAL_AP_STATUS         	_STA_LOCAL(0x0055) //T=u32, AP's connection status
#define CSID_S_LOCAL_AP_ALTERED         _STA_LOCAL(0x0056) //T=u32, ui alter
#define CSID_S_LOCAL_APRESTART_ALTERED         _STA_LOCAL(0x0057) //T=u32, ui alter

/*Device is in STA Mode, refer to CDM532RP */
#define CSID_S_LOCAL_STA_IP				_STA_LOCAL(0x0060) //T=ipv4, STA's IP
#define CSID_S_LOCAL_STA_NM				_STA_LOCAL(0x0061) //T=ipv4, STA's Netmask
#define CSID_S_LOCAL_STA_GW				_STA_LOCAL(0x0062) //T=ipv4, STA's GW
#define CSID_S_LOCAL_STA_PRIDNS			_STA_LOCAL(0x0063) //T=ipv4, STA's primary dns
#define CSID_S_LOCAL_STA_SECDNS			_STA_LOCAL(0x0064) //T=ipv4, STA's secondary dns
#define CSID_S_LOCAL_STA_STATUS         _STA_LOCAL(0x0065) //T=u32, STA's connection status

/* For WISP TxRx bridge function */
#define CSID_S_LOCAL_TXRXBRIDGE_STATUS	_STA_LOCAL(0x0070) //T=u32, D=0, TxRx Bridge status
#define CSID_S_LOCAL_REBOOT_ALTERED     _STA_LOCAL(0x0080) //T=u32, reboot alter, 1=reboot, 0=nothing
#define CSID_S_LOCAL_FAILOVER_ALTERED   _STA_LOCAL(0x0081) //T=u32, ui altered

/* ADSL VLAN TAGE selection*/ /*refer to CDD531 */
#define CSID_S_LOCAL_ADSL_VLANNUM				_STA_LOCAL(0x0082) //T=u32, D=0, number of VLAN tage

#define CSID_S_LOCAL_WAN_TX_TRAFFIC             _STA_LOCAL(0x0083) //T=u32, Wan Tx traffic (Unit: Kbps)
#define CSID_S_LOCAL_WAN_RX_TRAFFIC             _STA_LOCAL(0x0084) //T=u32, Wan Rx traffic (Unit: Kbps)
#define CSID_S_LOCAL_WLAN0_TX_TRAFFIC            _STA_LOCAL(0x0085) //T=u32, Wlan0 Tx traffic (Unit: Kbps)
#define CSID_S_LOCAL_WLAN0_RX_TRAFFIC            _STA_LOCAL(0x0086) //T=u32, Wlan0 Rx traffic (Unit: Kbps)
#define CSID_S_LOCAL_WLAN1_TX_TRAFFIC            _STA_LOCAL(0x0087) //T=u32, wlan1 Tx traffic (Unit: Kbps)
#define CSID_S_LOCAL_WLAN1_RX_TRAFFIC            _STA_LOCAL(0x0088) //T=u32, Wlan1 Rx traffic (Unit: Kbps)
#define CSID_S_LOCAL_3G_TX_TRAFFIC            _STA_LOCAL(0x0089) //T=u32, 3G Tx traffic (Unit: Kbps)
#define CSID_S_LOCAL_3G_RX_TRAFFIC            _STA_LOCAL(0x008a) //T=u32, 3G Rx traffic (Unit: Kbps)
#define CSID_S_LOCAL_DIALUP_TX_TRAFFIC            _STA_LOCAL(0x008b) //T=u32, 3G Tx traffic (Unit: Kbps)
#define CSID_S_LOCAL_DIALUP_RX_TRAFFIC            _STA_LOCAL(0x008c) //T=u32, 3G Rx traffic (Unit: Kbps)

/* Record traffic for load balance */
#define CSID_S_LOCAL_LOADBALANCE_TX_TRAFFIC         _STA_LOCAL(0x008E) //T=u32, record local wan tx traffic for load balance
#define CSID_S_LOCAL_LOADBALANCE_RX_TRAFFIC         _STA_LOCAL(0x008F) //T=u32, record local wan rx traffic for load balance
#define CSID_S_LOCAL_LOADBALANCE_MULTI_TX_TRAFFIC   _STA_LOCAL(0x0090) //T=u32, record multi wan tx traffic for load balance
#define CSID_S_LOCAL_LOADBALANCE_MULTI_RX_TRAFFIC   _STA_LOCAL(0x00A0) //T=u32, record multi wan rx traffic for load balance

/* Reserved 16 items for Multi-WAN,  X is from 1 .... */
#define CSID_S_LOCAL_MULTIWAN_IP				  _STA_LOCAL(0x0110) //T=ipv4, wanx ip, ex wan1 ip
#define CSID_S_LOCAL_MULTIWAN_NM            	  _STA_LOCAL(0x0120) //T=ipv4, wanx netmask
#define CSID_S_LOCAL_MULTIWAN_GW            	  _STA_LOCAL(0x0130) //T=ipv4, wanx gateway
#define CSID_S_LOCAL_MULTIWAN_PRIDNS        	  _STA_LOCAL(0x0140) //T=ipv4, wanx primary dns
#define CSID_S_LOCAL_MULTIWAN_SECDNS        	  _STA_LOCAL(0x0150) //T=ipv4, wanx secondary dns
#define CSID_S_LOCAL_MULTIWAN_STATUS        	  _STA_LOCAL(0x0160) //T=u32, wanx status
#define CSID_S_LOCAL_MULTIWAN_WANTYPE_ALTERED     _STA_LOCAL(0x0170) //T=u32, ui altered
#define CSID_S_LOCAL_MULTIWAN_WANSET_ALTERED      _STA_LOCAL(0x0180) //T=u32, ui altered
#define CSID_S_LOCAL_MULTIWAN_IF_NAME             _STA_LOCAL(0x0190) //T=str, the interface name of wan, ppp0, eth1, ixp1
#define CSID_S_LOCAL_MULTIWAN_CNT_TIME      	  _STA_LOCAL(0x01A0) //T=u32, the connect time
#define CSID_S_LOCAL_MULTIWAN_DETECT_WAN		  _STA_LOCAL(0x01B0) //T=u32, auto wantype detect result,0=no detect,1=dhcp,2=pppoe
#define CSID_S_LOCAL_MULTIWAN_CNT_FORCE			  _STA_LOCAL(0x01C0) //T=u32, Force WAN trigger conncet,0=disable,1=enable
#define CSID_S_LOCAL_MULTIWAN_CONNECTED_ALERT     _STA_LOCAL(0x01D0) //T=u32, D=0, WAN trigger conncet,1=local wan,2=multi wan
#define CSID_S_LOCAL_MULTIWAN_DISCONNECTED_ALERT  _STA_LOCAL(0x01E1) //T=u32, D=0, WAN trigger disconncet,1=local wan,2=multi wan
#define CSID_S_LOCAL_MULTIWAN_DEFAULT_GATEWAY	  _STA_LOCAL(0x01F0) //T=u32, D=16, Multiwan default gateway index, 99:local WAN, 0~15:multi-WAN, 16: no default gateway


#define CSID_S_LOCAL_HOMECOM_USER_STATUS	_STA_LOCAL(0x0200) //T=u8, D=0, Homecom user status.
#define CSID_S_LOCAL_RT8651C_SPECIAL_SNAT	_STA_LOCAL(0x0210) //T=u8, D=0, support RT8651 to connect using br0 interface

/* Reserved 16 items for VoIP-WAN,  X is from 1 .... */
#define CSID_S_LOCAL_VOIPWAN_IP	            _STA_LOCAL(0x0220) //T=ipv4, wanx ip, ex wan1 ip
#define CSID_S_LOCAL_VOIPWAN_NM            	_STA_LOCAL(0x0230) //T=ipv4, wanx netmask
#define CSID_S_LOCAL_VOIPWAN_GW            	_STA_LOCAL(0x0240) //T=ipv4, wanx gateway
#define CSID_S_LOCAL_VOIPWAN_PRIDNS        	_STA_LOCAL(0x0250) //T=ipv4, wanx primary dns
#define CSID_S_LOCAL_VOIPWAN_SECDNS        	_STA_LOCAL(0x0260) //T=ipv4, wanx secondary dns
#define CSID_S_LOCAL_VOIPWAN_STATUS        	_STA_LOCAL(0x0270) //T=u32, wanx status
#define CSID_S_LOCAL_VOIPWAN_WANTYPE_ALTERED   _STA_LOCAL(0x0280) //T=u32, ui altered
#define CSID_S_LOCAL_VOIPWAN_WANSET_ALTERED    _STA_LOCAL(0x0290) //T=u32, ui altered
#define CSID_S_LOCAL_VOIPWAN_IF_NAME       	_STA_LOCAL(0x02A0) //T=str, the interface name of wan, ppp0, eth1, ixp1
#define CSID_S_LOCAL_VOIPWAN_CNT_TIME      	_STA_LOCAL(0x02B0) //T=u32, the connect time
#define CSID_S_LOCAL_VOIPWAN_DETECT_WAN	    _STA_LOCAL(0x02C0) //T=u32, auto wantype detect result,0=no detect,1=dhcp,2=pppoe
#define CSID_S_LOCAL_VOIPWAN_CNT_FORCE	    _STA_LOCAL(0x02D0) //T=u32, Force WAN trigger conncet,0=disable,1=enable
#define CSID_S_LOCAL_MULTIWAN_ONDEMAND_TIMEOUT    _STA_LOCAL(0x02E0) //T=u32, record the current timeout value when ondemand

#define CSID_S_LOCAL_WAN_IF_NAME_3G_FAILOVER	_STA_LOCAL(0x0300) //T=str, the interface name of wan used by autobak ping check, ppp0, eth1, ixp1

/* Share port User management */
#define CSID_S_LOCAL_SP_ALTERED             _STA_LOCAL(0x0310) //T=u8, D=0, Share port ui altered
#define CSID_S_LOCAL_SP_PASS_ENABLE         _STA_LOCAL(0x0311) //T=u8, D=0, Share port pass enable
#define CSID_S_LOCAL_SP_DEFAULT_CLIENT_PORT         _STA_LOCAL(0x0312) //T=u32, D=0, Share port default client port
#define CSID_S_LOCAL_SP_SSL_ENABLE         _STA_LOCAL(0x0313) //T=u32, D=0, Share port ssl enable, 0=8181, 1=4433 
#define CSID_S_LOCAL_SP_NAT_ENABLE         _STA_LOCAL(0x0314) //T=u32, D=0, Share port nat enable, 0=disable, 1=enable

#define CSID_S_LOCAL_SP_CLIENT_IP        _STA_LOCAL(0x0320) //T=str, Share port account client ip, from 0 ~ 31
#define CSID_S_LOCAL_SP_CLIENT_PORT      _STA_LOCAL(0x0340) //T=u32, Share port account client port, from 0 ~ 31

/*capcha */
#define CSID_S_LOCAL_CAPACHA_ANS            _STA_LOCAL(0x0400) //T=str, capcha answer

/* WAN Alias */
#define CSID_S_LOCAL_WAN_ALIAS_ALTERED      _STA_LOCAL(0x0410) //T=u32, wan alias ui altered

/* Ethernet PORT link status */
#define CSID_S_ETH_PORT0_LINK_STATUS        _STA_LOCAL(0x0500) //T=u32, Ethernet PORT 0 LINK status 0x0500-0x051F
#define CSID_S_ETH_PORT31_LINK_STATUS       _STA_LOCAL(0x051F) //T=u32, Ethernet PORT31 LINK status 0x0500-0x051F

#define CSID_S_LOCAL_WIFI_VLAN_ALTERED					_STA_LOCAL(0x0800) //T=u8, D=0, WiFi vlan altered
#define CSID_S_LOCAL_WIFI_INTERFACE_DOWN				_STA_LOCAL(0x0801) //T=u32, D=0, WiFi interface down or active, 1 : down, 0: active

#define CSID_S_LOCAL_RESET_ALTERED				        _STA_LOCAL(0x0802) //T=u32, D=0, Reset to Default, 1: do it


/*Except ROMID to allow to upgrade*/
#define CSID_S_LOCAL_EXCEPT_ROM_ID          _STA_LOCAL(0x0900)  //T=str, except ROMID to be allowed to upgrade , reserve 16 sets 0x0900 - 0x090F


/* ipv6 test */
#define CSID_S_LOCAL_IPV6_PREFIX			_STA_LOCAL(0x1000)	//T=str, ipv6 prefix
#define CSID_S_LOCAL_DEF_SYSTEM_TIME        _STA_LOCAL(0x2000)  //T=str, default system time

/*  Reserved 16 items traffic tx and rx */
#define CSID_S_LOCAL_TX_TRAFFIC				_STA_LOCAL(0x3000)  //T=u32, Tx traffic
#define CSID_S_LOCAL_RX_TRAFFIC				_STA_LOCAL(0x3010)  //T=u32, Rx traffic

/*Reserved 16 items VOIP WAN IP for vapp*/
#define CSID_S_LOCAL_VOIPWAN_IP_VAPP         _STA_LOCAL(0x3020) //T=str, voip wan ip

#define CSID_S_PORT_ALTERED      _STA_LOCAL(0x3030) //T=u8, D=0, port speed set ui altered

#define CSID_S_LOCAL_WAN_TX_PACKETS_TRAFFIC				_STA_LOCAL(0x3040)  //T=str, Wan Tx packets traffic
#define CSID_S_LOCAL_WAN_RX_PACKETS_TRAFFIC				_STA_LOCAL(0x3050)  //T=str, Wan Rx packets traffic
#define CSID_S_LOCAL_WAN_ERR_PACKETS_TRAFFIC			_STA_LOCAL(0x3060)  //T=str, Wan Error packets traffic
#define CSID_S_LOCAL_WAN_DOWNLINK_TRAFFIC				_STA_LOCAL(0x3070)  //T=str, Wan downlink traffic (Unit: Mbps)
#define CSID_S_LOCAL_WAN_UPLINK_TRAFFIC				_STA_LOCAL(0x3080)  //T=str, Wan Rx packets traffic (Unit: Mbps)

#define CSID_S_LOCAL_WAN_INTERNET_STATUS        _STA_LOCAL(0x3090)    //T=u32, Status of Internet, 0: internet break, 1: internet alive

/* For IO-Data Wizard Control */
#define CSID_S_LOCAL_WIZARD_TRIGGER                 _STA_LOCAL(0x3091)    //T=u32, D=0, Trigger for IO-Data Wizard
#define CSID_S_LOCAL_WIZARD_WANTYPE                 _STA_LOCAL(0x3092)    //T=u32, D=0, Test Wan for IO-Data Wizard
#define CSID_S_LOCAL_WIZARD_WANEXEC                 _STA_LOCAL(0x3093)    //T=u32, D=0, Test Wan Execution Status for IO-Data Wizard
#define CSID_S_LOCAL_WIZARD_USRACTION               _STA_LOCAL(0x3094)    //T=u32, D=0, Action for User, 1: Confirm, 2: Cancel
#define CSID_S_LOCAL_WIZARD_CONFIG                  _STA_LOCAL(0x3095)    //T=u32, D=0, Wizard Config Type
#define CSID_S_LOCAL_WIZARD_PPPOE_USER              _STA_LOCAL(0x3096)    //T=str, Wizard PPPoE Username
#define CSID_S_LOCAL_WIZARD_PPPOE_PWD               _STA_LOCAL(0x3097)    //T=str, Wizard PPPoE Password
#define CSID_S_LOCAL_WIZARD_KEEPALIVE               _STA_LOCAL(0x3098)    //T=u32, D=0, Wizard KeepAlive for UI, 1: alive
#define CSID_S_LOCAL_WIZARD_WANSTAT                 _STA_LOCAL(0x3099)    //T=u32, D=0, Test Wan Status for IO-Data Wizard


#define CSID_S_LOCAL_WANTYPE_INDEX                 _STA_LOCAL(0x5001) //T=u32, D=0, wan type index
#define CSID_S_LOCAL_WAN_MODE                      _STA_LOCAL(0x5002) //T=u32, D=0, wan mode, 1:main, 2:failover: 3:stay-by failover
#define CSID_S_LOCAL_XDSL_ENABLE                   _STA_LOCAL(0x5003) //T=u32, D=0, wan is xdsl enable 1:enable 0:disable
#define CSID_S_LOCAL_ROUTE_ENABLE                  _STA_LOCAL(0x5004) //T=u32, D=0, route enable 1:enable 0:disable
#define CSID_S_LOCAL_WAN_RESTART_IDEL              _STA_LOCAL(0x5005) //T=u32, D=0, wan restart idel time
#define CSID_S_LOCAL_CNT_TRIGGER				   _STA_LOCAL(0x5006) //T=u32, D=0, wan cnt by wan connect/disconnect alter 0:disconnect 1:connect
#define CSID_S_LOCAL_HALFBRIDGING_ENABLE		   _STA_LOCAL(0x5007) //T=u8, Half-Bridging mode 1:enable, 0:disable

#define CSID_S_LOCAL_MULTIWAN_CHANGE_INDEX                 _STA_LOCAL(0x5100) //T=u32, D=0, Reset to Default, 1: do
#define CSID_S_LOCAL_MULTIWAN_WANTYPE_INDEX                _STA_LOCAL(0x5110) //T=u32, D=0, multiwan type index
#define CSID_S_LOCAL_MULTIWAN_WAN_MODE                     _STA_LOCAL(0x5120) //T=u32, D=0, multiwan mode, 1:main, 2:failover: 3:stay-by failover
#define CSID_S_LOCAL_MULTIWAN_XDSL_ENABLE                  _STA_LOCAL(0x5130) //T=u32, D=0, multiwan is xdsl enable 1:enable 0:disable
#define CSID_S_LOCAL_MULTIWAN_ROUTE_ENABLE                 _STA_LOCAL(0x5140) //T=u32, D=0, multiwan route enable 1:enable 0:disable
#define CSID_S_LOCAL_MULTIWAN_WAN_RESTART_IDEL             _STA_LOCAL(0x5150) //T=u32, D=0, multiwan restart idel time
#define CSID_S_LOCAL_MULTIWAN_CNT_TRIGGER		           _STA_LOCAL(0x5160) //T=u32, D=0, multiwan cnt by wan connect/disconnect alter 0:disconnect 1:connect

#define CSID_S_LOCAL_WIFI_INTERFACE_ACTION		           _STA_LOCAL(0x5170) //T=u32, D=0, wifi interface action 1:disable 2:enable

#endif //ifndef _CSID_LOCAL_H
