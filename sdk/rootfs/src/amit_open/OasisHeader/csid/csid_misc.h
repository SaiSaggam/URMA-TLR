/*
 * @file Member ID Definition of Group Miscellaneous.
 *
 * Moderator: JasonJiang
 * Group ID: 0x00150000/0x80150000
 */

#ifndef _CSID_MISC_H
#define _CSID_MISC_H

#include "csid_gid.h"

#define _CFG_MISC(x)         (CSID_GIDC_MISC|(x))
#define _STA_MISC(x)         (CSID_GIDS_MISC|(x))


#define MISC_PING_STOP	0
#define MISC_PING_START	1


/* Configuration Items */
#define CSID_C_MISC_REMOTE_ADMIN_ENABLE		_CFG_MISC(0x0001)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MISC_REMOTE_ADMIN_IP		    _CFG_MISC(0x0002)	//T=ipv4, the ip of administrators
#define CSID_C_MISC_REMOTE_ADMIN_PORT		_CFG_MISC(0x0003)	//T=u16, the port of adminitrators 
#define CSID_C_MISC_REMOTE_ADMIN_NM		    _CFG_MISC(0x0004)	//T=u16, the netmask of adminitrators 

#define CSID_C_MISC_DISCARD_PING			_CFG_MISC(0x0005)	//T=u8, whether discarding ping from wan, 1:yes 0:no
#define CSID_C_MISC_SPI_MODE				_CFG_MISC(0x0006)	//T=u8, D=0, whether enabling SPI mode, 1:yes 0:no
#define CSID_C_MISC_DOS_ATTACK				_CFG_MISC(0x0007)	//T=u8, whether protecting DoS attackment, 1:yes 0:no

#define CSID_C_MISC_DMZ_ENABLE				_CFG_MISC(0x0008)	//T=u8, DMZ enable, 1:yes 0:no
#define CSID_C_MISC_DMZ_HOST_IP				_CFG_MISC(0x0009)	//T=ipv4, the ip of DMZ host
#define CSID_C_MISC_NONSTANDARD_FTP_PORT	_CFG_MISC(0x000A)	//T=u16, the port of non-standard FTP port
#define CSID_C_MISC_UPNP_ENABLE				_CFG_MISC(0x000B)	//T=u8, UPnP enable, 1:yes 0:no
#define CSID_C_MISC_IGMP_ENABLE				_CFG_MISC(0x000C)	//T=u8,	 IGMP enable, 1:yes 0:no
#define CSID_C_MISC_QOS_SMALL_PKT_ENABLE	_CFG_MISC(0x000D)	//T=u8,	D=0, QoS for small packets enable, 1:yes 0:no
#define CSID_C_MISC_STEALTH_EABLE	        _CFG_MISC(0x000E)	//T=u8,	D=0, Stealth Mode, 1:yes 0:no
#define CSID_C_MISC_ENABLE_WEBRB	        _CFG_MISC(0x000F)	//T=u8,	D=0, Web Rebinding, 1:yes 0:no

#define CSID_C_MISC_DISABLE_PPTP_ALG        _CFG_MISC(0x0010)   //T=u8,	 Whether to disable PPTP Passthrough, 1:yes 0:no 
#define CSID_C_MISC_DISABLE_L2TP_ALG        _CFG_MISC(0x0011)   //T=u8,	 Whether to disable L2TP Passthrough, 1:yes 0:no 
#define CSID_C_MISC_DISABLE_IPSEC_ALG       _CFG_MISC(0x0012)   //T=u8,	 Whether to disable IPSec Passthrough, 1:yes 0:no 
#define CSID_C_MISC_ENABLE_NAT_LOOPBACK     _CFG_MISC(0x0013)   //T=u8,	 D=0, Whether to enable NAT Loopback, 1:yes 0:no 
#define CSID_C_MISC_DONT_REPLY_CONF_REQ     _CFG_MISC(0x0014)   //T=u8,	 D=0, Whether to response Configure-Request
#define CSID_C_MISC_DISABLE_TELNETD         _CFG_MISC(0x0015)   //T=u8,	 D=0, Whether to start telnetd (Need reboot)
#define CSID_C_MISC_TELNETD_ALLOW_WAN       _CFG_MISC(0x0016)   //T=u8,	 D=0, Whether to allow conect to telnetd from WAN side (Need reboot)
#define CSID_C_MISC_TELNETD_BIND_PORT       _CFG_MISC(0x0017)   //T=u32, D=23, Change the bind port of telnetd (Need reboot)
#define CSID_C_MISC_WEBRB_URL               _CFG_MISC(0x0018)	//T=str, Redefine URL for Web-Rebinding

#define CSID_C_MISC_TELNETD_CUSTOMIZE_ENABLE 	_CFG_MISC(0x0019)	//T=u8, D=0, telnet is customize or not
#define CSID_C_MISC_TELNETD_ACCESS_STATUS 		_CFG_MISC(0x001a)	//T=u8, D=0, allow access from LAN, WAN or BOTH
#define CSID_C_MISC_TELNETD_ACCESS_ALLOW_ALL 	_CFG_MISC(0x001b)	//T=u8, D=1, allow everyone to access
#define CSID_C_MISC_TELNETD_ACCESS_IP 			_CFG_MISC(0x001c)	//T=str, only allow special ip

#define CSID_C_MISC_TELNETD_DENY_LAN       _CFG_MISC(0x001d)   //T=u8,	 D=0, Whether to deny conect to telnetd from LAN side (Need reboot)

#define CSID_C_MISC_PPTP_ALG_ALL_IF_DISABLE       _CFG_MISC(0x001e)   //T=u8,	 D=0, drop all interface pptp pass through port, 1:yes 0:no

#define CSID_C_MISC_SYSLOG_ENABLE			_CFG_MISC(0x0020)	//T=u8, enable to send log to collector
#define CSID_C_MISC_SYSLOG_COLLECTOR_IP		_CFG_MISC(0x0021)	//T=ipv4, syslog collector's ip
#define CSID_C_MISC_SYSLOG_COLLECTOR_PORT	_CFG_MISC(0x0022)	//T=u16, syslog collector's port
#define CSID_C_MISC_SYSLOG_TYPE_SYSTEM      _CFG_MISC(0x0023)	//T=u8, D=0, enable system activities type of syslog
#define CSID_C_MISC_SYSLOG_TYPE_ATTACKS     _CFG_MISC(0x0024)	//T=u8, D=0, enable attacks type of syslog
#define CSID_C_MISC_SYSLOG_TYPE_NOTI_DROP   _CFG_MISC(0x0025)	//T=u8, D=0, enable dropped and notices type of syslog
#define CSID_C_MISC_SYSLOG_TYPE_DEBUG       _CFG_MISC(0x0026)	//T=u8, D=0, enable debug type of syslog
#define CSID_C_MISC_SYSLOG_COLLECTOR_INDEX  _CFG_MISC(0x0027)   //T=u8, D=99, syslog server index from external server list

#define CSID_C_MISC_NTT_SYSLOG_ENABLE       _CFG_MISC(0x002A)	//T=u8, enable to send log to collector (For NTT) (Hidden syslog option)
 
#define CSID_C_MISC_SYSLOG_EMAIL_TYPE_SYSTEM      _CFG_MISC(0x002B)	//T=u8, D=0, enable system activities type of syslog for email alert
#define CSID_C_MISC_SYSLOG_EMAIL_TYPE_ATTACKS     _CFG_MISC(0x002C)	//T=u8, D=0, enable attacks type of syslog for email alert
#define CSID_C_MISC_SYSLOG_EMAIL_TYPE_NOTI_DROP   _CFG_MISC(0x002D)	//T=u8, D=0, enable dropped and notices type of syslog for email alert
#define CSID_C_MISC_SYSLOG_EMAIL_TYPE_DEBUG       _CFG_MISC(0x002E)	//T=u8, D=0, enable debug type of syslog for email alert

#define CSID_C_MISC_TX_RATE_CONTROL_ENABLE	_CFG_MISC(0x0030)	//T=u8,	 TX Rate Control enable, 1:yes 0:no
#define CSID_C_MISC_TX_NEW_TCP_RATE		    _CFG_MISC(0x0031)	//T=u16, Number of New TCP connections can be created (per seconed)
#define CSID_C_MISC_TX_NEW_UDP_RATE		    _CFG_MISC(0x0032)	//T=u16, Number of New UDP connections can be created (per seconed)
#define CSID_C_MISC_TX_SMALL_UDP_RATE		_CFG_MISC(0x0033)	//T=u16, Number of small size UDP packets can be sent (per seconed)
#define CSID_C_MISC_TX_SMALL_UDP_PKTSZ		_CFG_MISC(0x0034)	//T=u16, Define the length of TX packet is small 

#define CSID_C_MISC_RX_RATE_CONTROL_ENABLE	_CFG_MISC(0x0035)	//T=u8,	 RX Rate Control enable, 1:yes 0:no
#define CSID_C_MISC_RX_NEW_TCP_RATE		    _CFG_MISC(0x0036)	//T=u16, Number of New TCP connections can be accepted (per seconed)
#define CSID_C_MISC_RX_NEW_UDP_RATE		    _CFG_MISC(0x0037)	//T=u16, Number of New UDP connections can be accepted (per seconed)
#define CSID_C_MISC_RX_SMALL_UDP_RATE		_CFG_MISC(0x0038)	//T=u16, Number of small size UDP packets can be received (per seconed)
#define CSID_C_MISC_RX_SMALL_UDP_PKTSZ		_CFG_MISC(0x0039)	//T=u16, Define the length of RX packet is small 
#define CSID_C_MISC_IGMP_SNOOP_ENABLE		_CFG_MISC(0x003A)	//T=u8,	D=0, IGMP Snoop enable, 1:yes 0:no  
#define CSID_C_MISC_IGMP_QUICKLEAVE			_CFG_MISC(0x003B)	//T=u8,	 IGMP quick leave enable, 1:yes 0:no 
#define CSID_C_MISC_ENABLE_STP			    _CFG_MISC(0x003C)	//T=u8,	 Enable/Disable Spanning Tree, 1:yes 0:no 
#define CSID_C_MISC_DEVICE_NAME			    _CFG_MISC(0x003D)	//T=str, device name
#define CSID_C_MISC_LAN_WAN_BRIDGE_ENABLE          _CFG_MISC(0x003E)    //T=u32, Lan with wan brideg function,1:yes 0:no
#define CSID_C_MISC_LAN_WAN_SELECT                 _CFG_MISC(0x003F)    //T=u32, Lan with wan brideg select.0:LAN4,1:LAN3,2:LAN2,3:LAN1

// Remote admin support more rules
#define CSID_C_MISC_REMOTE_ADMIN_ENABLE_2	_CFG_MISC(0x0040)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MISC_REMOTE_ADMIN_IP_2		_CFG_MISC(0x0050)	//T=ipv4, the ip of administrators
#define CSID_C_MISC_REMOTE_ADMIN_PORT_2		_CFG_MISC(0x0060)	//T=u16, the port of adminitrators 
#define CSID_C_MISC_REMOTE_ADMIN_NM_2		_CFG_MISC(0x0070)	//T=u16, the netmask of adminitrators 
  
#define CSID_C_MISC_PING_TARGET				_CFG_MISC(0x0080)	//T=str, default target for ping test
#define CSID_C_MISC_WAKE_TARGET				_CFG_MISC(0x0081)	//T=mac, default target for wake on lan
#define CSID_C_MISC_MAX_LOW_BATTERY			_CFG_MISC(0x0082)	//T=u8,  Max number of Low Battery/Power
#define CSID_C_MISC_BTY_DETECT_INTERVAL		_CFG_MISC(0x0083)	//T=u8,  Detect Interval of  Low Battery/Power
#define CSID_C_MISC_M1LED_ALWAYS_ON			_CFG_MISC(0x0084)	//T=u8,  M1 Status LED, 0: twinkling 1:always on
#define CSID_C_MISC_DIM_LED					_CFG_MISC(0x0085)	//T=u8,  DIM LED, 0:brighten led, 1:dim led
#define CSID_C_MISC_DIM_RULE_NO				_CFG_MISC(0x0086)	//T=u16, D=0, rule number for scheduling dimming leds
#define CSID_C_MISC_PWR_SAVING_ENABLE       _CFG_MISC(0x0087)   //T=u8, D=0, power saving enable
#define CSID_C_MISC_WIFI_PWR_SAVING         _CFG_MISC(0x0088)   //T=u8, D=0, wifi power saving
#define CSID_C_MISC_HSPA_PWR_SAVING         _CFG_MISC(0x0089)   //T=u8, D=0, hspa power saving
#define CSID_C_MISC_CPU_PWR_SAVING          _CFG_MISC(0x008A)   //T=u8, D=0, cpu power saving
#define CSID_C_MISC_LOGIN_PROMPT            _CFG_MISC(0x008B)	//T=str, default login prompt for customization login RT305XL:
#define CSID_C_MISC_TRACEROUTE_TARGET		_CFG_MISC(0x008C)	//T=str, default target for Traceroute test
#define CSID_C_MISC_PROVISIONING_SERVER     _CFG_MISC(0x008D)   //T=str, covia provisioning server
#define CSID_C_MISC_CENTRAL_SERVER          _CFG_MISC(0x008E)   //T=str, covia central server
#define CSID_C_MISC_NTP_SERVER_PRI          _CFG_MISC(0x008F)   //T=str, covia ntp provisioning server primary
#define CSID_C_MISC_NTP_SERVER_SEC          _CFG_MISC(0x0090)   //T=str, covia ntp server secondary
#define CSID_C_MISC_FIRMWARE_SERVER         _CFG_MISC(0x0091)   //T=str, covia firmware server
#define CSID_C_MISC_UPNP_SETTINGS           _CFG_MISC(0x0092)   //T=u8, covia upnp settings
#define CSID_C_MISC_UPNP_PUBLIC_ADDRESS     _CFG_MISC(0x0093)   //T=ipv4, covia upnp public ip address
#define CSID_C_MISC_UPNP_PUBLIC_PORT        _CFG_MISC(0x0094)   //T=u32, covia upnp public port number
#define CSID_C_MISC_DLNA_AUTO_RESCAN        _CFG_MISC(0x0095)   //T=u8, D=1, DLNA auto rescan Enable/Disable
#define CSID_C_MISC_DLNA_ENABLE             _CFG_MISC(0x0096)   //T=u8, D=1, DLNA Enable/Disable

#define CSID_C_MISC_SYSLOG_EXTERNAL_TYPE_SYSTEM      _CFG_MISC(0x0097)	//T=u8, D=0, enable system activities type of syslog for external syslogd
#define CSID_C_MISC_SYSLOG_EXTERNAL_TYPE_ATTACKS     _CFG_MISC(0x0098)	//T=u8, D=0, enable attacks type of syslog for external syslogd
#define CSID_C_MISC_SYSLOG_EXTERNAL_TYPE_NOTI_DROP   _CFG_MISC(0x0099)	//T=u8, D=0, enable dropped and notices type of syslog for external syslogd
#define CSID_C_MISC_SYSLOG_EXTERNAL_TYPE_DEBUG       _CFG_MISC(0x009A)	//T=u8, D=0, enable debug type of syslog for external syslogd
#define CSID_C_MISC_SYSLOG_STORAGE_TYPE_SYSTEM       _CFG_MISC(0x009B)	//T=u8, D=0, enable system activities type of syslog for storage
#define CSID_C_MISC_SYSLOG_STORAGE_TYPE_ATTACKS      _CFG_MISC(0x009C)	//T=u8, D=0, enable attacks type of syslog for storage
#define CSID_C_MISC_SYSLOG_STORAGE_TYPE_NOTI_DROP    _CFG_MISC(0x009D)	//T=u8, D=0, enable dropped and notices type of syslog for storage
#define CSID_C_MISC_SYSLOG_STORAGE_TYPE_DEBUG        _CFG_MISC(0x009E)	//T=u8, D=0, enable debug type of syslog for storage

#define CSID_C_MISC_HANDLE_MEM_DISABLE      _CFG_MISC(0x00a0)   //T=u8, D=0, commander handle_memory disable, 0: enable 1: disable
#define CSID_C_MISC_MULTIWAN_DMZ_MODE		_CFG_MISC(0x00a1)	//T=u8, D=0, DMZ mode, 1:MULTI mode 0:single mode
#define CSID_C_MISC_MULTIWAN_DMZ_ENABLE		_CFG_MISC(0x00b0)	//T=u8, D=0, DMZ enable, 1:yes 0:no
#define CSID_C_MISC_MULTIWAN_DMZ_HOST_IP	_CFG_MISC(0x00c0)	//T=ipv4, the ip of DMZ host
/*reserve 16 space for multiwan*/

#define CSID_C_MISC_SYSLOG_STORAGE_ENABLE        	_CFG_MISC(0x00E0)	//T=u8, D=0, enable syslog to storage
#define CSID_C_MISC_SYSLOG_STORAGE_DEVICE        	_CFG_MISC(0x00E1)	//T=u8, D=0, select syslog to storage device, 0:internal, 1:external
#define CSID_C_MISC_SYSLOG_STORAGE_FILENAME      	_CFG_MISC(0x00E2)	//T=str, syslog to storage file name
#define CSID_C_MISC_SYSLOG_STORAGE_SPLIT_ENABLE        	_CFG_MISC(0x00E3)	//T=u8, D=0, enable syslog to storage split file
#define CSID_C_MISC_SYSLOG_STORAGE_SPLIT_SIZE        	_CFG_MISC(0x00E4)	//T=u32, D=0, syslog to storage split file size in KB
#define CSID_C_MISC_SYSLOG_TYPE_LOGIN      		_CFG_MISC(0x00E5)	//T=u8, D=0, enable login activities type of syslog
#define CSID_C_MISC_SYSLOG_EMAIL_TYPE_LOGIN      	_CFG_MISC(0x00E6)	//T=u8, D=0, enable login activities type of syslog for email alert
#define CSID_C_MISC_SYSLOG_EXTERNAL_TYPE_LOGIN      	_CFG_MISC(0x00E7)	//T=u8, D=0, enable login activities type of syslog for external syslogd
#define CSID_C_MISC_SYSLOG_STORAGE_TYPE_LOGIN        	_CFG_MISC(0x00E8)	//T=u8, D=0, enable login type of syslog for storage
#define CSID_C_MISC_SYSLOG_TYPE_MOBILE      		_CFG_MISC(0x00E9)	//T=u8, D=0, enable mobile data usage type of syslog
#define CSID_C_MISC_SYSLOG_EMAIL_TYPE_MOBILE      	_CFG_MISC(0x00EA)	//T=u8, D=0, enable mobile data usage type of syslog for email alert
#define CSID_C_MISC_SYSLOG_EXTERNAL_TYPE_MOBILE      	_CFG_MISC(0x00EB)	//T=u8, D=0, enable mobile data usage type of syslog for external syslogd
#define CSID_C_MISC_SYSLOG_STORAGE_TYPE_MOBILE        	_CFG_MISC(0x00EC)	//T=u8, D=0, enable mobile data usage type of syslog for storage
#define CSID_C_MISC_SYSLOG_STORAGE_SPLIT_UNIT        	_CFG_MISC(0x00ED)	//T=u8, D=0, syslog to storage split file size unit, 0:KB, 1:MB

#define CSID_C_MISC_MYDLINK_REGISTER_ST			_CFG_MISC(0x0100)	//T=u8, D=0,  mydlink registered status of the device.
#define CSID_C_MISC_MYDLINK_DEV_NAME    		_CFG_MISC(0x0101)  //T=str, Device name (user set).
#define CSID_C_MISC_DLNA_SERVER_NAME    		_CFG_MISC(0x0102)  //T=str, DLNA Server name (user set).
#define CSID_C_MISC_MYDLINK_USERNAME            _CFG_MISC(0x0103)  //T=str, mydlink user name.
#define CSID_C_MISC_MYDLINK_PASSWORD            _CFG_MISC(0x0104)  //T=str, mydlink password.
#define CSID_C_MISC_MYDLINK_FIRST_NAME          _CFG_MISC(0x0105)  //T=str, first name when sign-up mydlink.
#define CSID_C_MISC_MYDLINK_LAST_NAME           _CFG_MISC(0x0106)  //T=str, last name when sign-up mydlink.
#define CSID_C_MISC_MYDLINK_ENABLE			    _CFG_MISC(0x0107)	//T=u8, D=0,  mydlink enalbe, 0: disable 1: enable.
#define CSID_C_MISC_MYDLINK_LOG_ENABLE			_CFG_MISC(0x0108)	//T=u8, D=0,  mydlink log enalbe, 0: disable 1: enable.
#define CSID_C_MISC_MYDLINK_USERLOGINFO_ENABLE	_CFG_MISC(0x0109)	//T=u8, D=0,  mydlink userloginfo enalbe, 0: disable 1: enable.
#define CSID_C_MISC_MYDLINK_WIRELESSWARN_ENABLE	_CFG_MISC(0x010A)	//T=u8, D=0,  mydlink wirelesswarn enalbe, 0: disable 1: enable.
#define CSID_C_MISC_MYDLINK_DISABLE	            _CFG_MISC(0x010B)	//T=u8, D=0,  flag to disable mydlink function
#define CSID_C_MISC_MYDLINK_ATTR_0	            _CFG_MISC(0x010C)	//T=str,  mdb attr0
#define CSID_C_MISC_MYDLINK_ATTR_1	            _CFG_MISC(0x010D)	//T=str,  mdb attr1
#define CSID_C_MISC_MYDLINK_ATTR_2	            _CFG_MISC(0x010E)	//T=str,  mdb attr2
#define CSID_C_MISC_MYDLINK_ATTR_3	            _CFG_MISC(0x010F)	//T=str,  mdb attr3
#define CSID_C_MISC_MYDLINK_ATTR_4	            _CFG_MISC(0x0110)	//T=str,  mdb attr4
#define CSID_C_MISC_MYDLINK_ATTR_5	            _CFG_MISC(0x0111)	//T=str,  mdb attr5
#define CSID_C_MISC_MYDLINK_ATTR_6	            _CFG_MISC(0x0112)	//T=str,  mdb attr6
#define CSID_C_MISC_MYDLINK_ATTR_7	            _CFG_MISC(0x0113)	//T=str,  mdb attr7
#define CSID_C_MISC_MYDLINK_ATTR_8	            _CFG_MISC(0x0114)	//T=str,  mdb attr8
#define CSID_C_MISC_MYDLINK_ATTR_9	            _CFG_MISC(0x0115)	//T=str,  mdb attr9
#define CSID_C_MISC_MYDLINK_RX	            	_CFG_MISC(0x0116)	//T=u32,  received traffic for mydlink
#define CSID_C_MISC_MYDLINK_TX	            	_CFG_MISC(0x0117)	//T=u32,  transmitted traffic for mydlink
#define CSID_C_MISC_MYDLINK_REGION	            _CFG_MISC(0x0118)	//T=str,  region of this device(e.g., AU, EU)
#define CSID_C_MISC_MYDLINK_EV_HISTORY_ENABLE   _CFG_MISC(0x0119)   //T=u32, D=0,  mydlink TriggedEventHistory enable, 0: disable 1: enable.
#define CSID_C_MISC_MYDLINK_NUMBER              _CFG_MISC(0x011a)   //T=str   mydlink number of DUT from the device agent.
#define CSID_C_MISC_MYDLINK_FOOTPRINT           _CFG_MISC(0x011b)   //T=str,  The footprint value from device agent.
#define CSID_C_MISC_MYDLINK_REGISTER_ENABLE     _CFG_MISC(0x011c)   //T=u32, D=0,  mydlink registration processing.
#define CSID_C_MISC_MYDLINK_REGISTER_BINDING    _CFG_MISC(0x011d)   //T=u32, D=0,  mydlink binding processing.
#define CSID_C_MISC_MYDLINK_REGISTER_USERNAME   _CFG_MISC(0x011e)   //T=str,  mydlink registration user account.
#define CSID_C_MISC_MYDLINK_REGISTER_PASSWORD   _CFG_MISC(0x011f)   //T=str,  mydlink registration user password.
#define CSID_C_MISC_MYDLINK_REGISTER_LASTNAME   _CFG_MISC(0x0120)   //T=str,  mydlink registration user last name.
#define CSID_C_MISC_MYDLINK_REGISTER_FIRSTNAME  _CFG_MISC(0x0121)   //T=str,  mydlink registration user first name.

/* AP Controller */
#define CSID_C_MISC_AP_CONTROLLER_AP   			_CFG_MISC(0x0130)   //T=u8, D=0, enable AP function, 0: disable 1: enable.
#define CSID_C_MISC_AP_CONTROLLER_AC   			_CFG_MISC(0x0131)   //T=u8, D=0, enable AC(AP Controller) function, 0: disable 1: enable.
#define CSID_C_MISC_IP_CONFLICT_DETECTION   		_CFG_MISC(0x0132)   //T=u8, D=0, enable IP conflict detection function, 0: disable 1: enable.

/* DCH */
#define CSID_C_MISC_DCH_MODULE_ID   			_CFG_MISC(0x0140)   //T=str, DCH module ID
#define CSID_C_MISC_DCH_SUPPORT_TYPES 			_CFG_MISC(0x0141)   //T=u32, D=0, DCH support type, 1:monthly 3:daily 5:hourly 
#define CSID_C_MISC_DCH_POWER_INDEX 			_CFG_MISC(0x0142)   //T=u32, D=0, DCH power usage list index
#define CSID_C_MISC_PUR_TMP_ELECTRICITY_INDEX   _CFG_MISC(0x0143)   //T=u32, D=0, dch ac over the 
#define CSID_C_MISC_APCLIENT_TRIGGER            _CFG_MISC(0x0144)   //T=u32, D=0, when 3g up ap client still up, 1:disable 0:enable 

#define CSID_C_MISC_DCH_AC_ENABLE 	          	_CFG_MISC(0x0150)   //T=u32, DCH ACTimerOPMode enable
#define CSID_C_MISC_DCH_AC_MODE 	          	_CFG_MISC(0x0160)   //T=u32, DCH ACTimerOPMode mode
#define CSID_C_MISC_DCH_AC_LOUVER 	          	_CFG_MISC(0x0170)   //T=u32, DCH ACTimerOPMode louver
#define CSID_C_MISC_DCH_AC_FAN   	          	_CFG_MISC(0x0180)   //T=u32, DCH ACTimerOPMode fan
#define CSID_C_MISC_DCH_AC_TEMP   	          	_CFG_MISC(0x0190)   //T=u32, DCH ACTimerOPMode temp
#define CSID_C_MISC_DCH_AC_REPEAT_ENABLE      	_CFG_MISC(0x01a0)   //T=u32, DCH ACTimerOPMode repeat enable
#define CSID_C_MISC_DCH_AC_REPEAT_DAYS         	_CFG_MISC(0x01b0)   //T=u32, DCH ACTimerOPMode repeat days
#define CSID_C_MISC_DCH_AC_STARTTIME         	_CFG_MISC(0x01c0)   //T=u32, DCH ACTimerOPMode starttime

/* Budget Quota*/
#define CSID_C_MISC_QUOTA_ENABLE			    	_CFG_MISC(0x0200)  //T=u8, D=0, budget quota enable.
#define CSID_C_MISC_QUOTA_INTERFACE_NAME    			_CFG_MISC(0x0201)  //T=str, interface name (user set), enumeration of br0, vlan1, ...
#define CSID_C_MISC_QUOTA_LIMIT_TIME    			_CFG_MISC(0x0202)  //T=u16, quota limit time(days).
#define CSID_C_MISC_QUOTA_DOWNLOAD_ENABLE                       _CFG_MISC(0x0203)  //T=u8, D=0, enable download quota.
#define CSID_C_MISC_QUOTA_DOWNLOAD_MAX                          _CFG_MISC(0x0204)  //T=u32, max download quota(MB).
#define CSID_C_MISC_QUOTA_UPLOAD_ENABLE                         _CFG_MISC(0x0205)  //T=u8, D=0, enable upload quota.
#define CSID_C_MISC_QUOTA_UPLOAD_MAX                            _CFG_MISC(0x0206)  //T=u32, max upload quota(MB).
#define CSID_C_MISC_QUOTA_INTERFACE_OPTION    			_CFG_MISC(0x0207)  //T=str, interface selection option, enumeration of br0, vlan1, ...
/*reserve 5 space for interface option*/
#define CSID_C_MISC_QUOTA_INTERFACE_RX                          _CFG_MISC(0x0210)  //T=u32, received traffic of current interface(KB).
#define CSID_C_MISC_QUOTA_INTERFACE_TX                          _CFG_MISC(0x0211)  //T=u32, transmitted traffic of current interface(KB).


/* DCH */
#define CSID_C_MISC_DCH_DH_ENABLE               _CFG_MISC(0x0250)   //T=u32, DCH DHTimerOPMode enable
#define CSID_C_MISC_DCH_DH_MODE                 _CFG_MISC(0x0260)   //T=u32, DCH DHTimerOPMode mode
#define CSID_C_MISC_DCH_DH_FAN               _CFG_MISC(0x0270)   //T=u32, DCH DHCTimerOPMode fan
#define CSID_C_MISC_DCH_DH_REPEAT_ENABLE        _CFG_MISC(0x0280)   //T=u32, DCH DHTimerOPMode repeat enable
#define CSID_C_MISC_DCH_DH_REPEAT_DAYS          _CFG_MISC(0x0290)   //T=u32, DCH DHTimerOPMode repeat days
#define CSID_C_MISC_DCH_DH_STARTTIME            _CFG_MISC(0x02a0)   //T=u32, DCH DHTimerOPMode starttime

/*user group for pptp/l2tp   */
#define CSID_C_MISC_GROUPUSER_NAME         			_CFG_MISC(0x0300) //T=str,  User name
#define CSID_C_MISC_GROUPUSER_PASSWORD         			_CFG_MISC(0x0400) //T=str,  User password
#define CSID_C_MISC_GROUPUSER_GROUP1         			_CFG_MISC(0x0500) //T=str,  User group1
#define CSID_C_MISC_GROUPUSER_GROUP2         			_CFG_MISC(0x0501) //T=str,  User group2
#define CSID_C_MISC_GROUPUSER_GROUP3         			_CFG_MISC(0x0502) //T=str,  User group3
#define CSID_C_MISC_GROUPUSER_GROUP4         			_CFG_MISC(0x0503) //T=str,  User group4
#define CSID_C_MISC_GROUPUSER_GROUP1_NULL         		_CFG_MISC(0x0504) //T=u8, D=0,User group1 null,0:empty,1:full
#define CSID_C_MISC_GROUPUSER_GROUP2_NULL         		_CFG_MISC(0x0505) //T=u8, D=0,User group2 null,0:empty,1:full
#define CSID_C_MISC_GROUPUSER_GROUP3_NULL         		_CFG_MISC(0x0506) //T=u8, D=0,User group3 null,0:empty,1:full
#define CSID_C_MISC_GROUPUSER_GROUP4_NULL         		_CFG_MISC(0x0507) //T=u8, D=0,User group4 null,0:empty,1:full

#define CSID_C_MISC_TRACERT_TARGET         		_CFG_MISC(0x0508) //T=str, default target for traceroute test
#define CSID_C_MISC_FIREWALL_FUNCTION_DISABLE	_CFG_MISC(0x0509) //T=u8, D=0, firewall function disable, 0: Function enable 1: Function disable

#define CSID_C_MISC_DMZ_WAN_LIST                _CFG_MISC(0x050A) //T=u32, WAN List for DMZ Host, 0: All, 1: WAN1, 2: WAN2, 3: WAN1~2, etc...

/*Dos item*/
#define CSID_C_MISC_DOS_ENABLE	         		_CFG_MISC(0x0600) //T=u8, whether protecting DoS is enable or not, 1:yes 0:no
#define CSID_C_MISC_DOS_SYN_FLOOD         		_CFG_MISC(0x0601) //T=u8, whether protecting DoS attack-syn flood, 1:yes 0:no
#define CSID_C_MISC_DOS_SYN_FLOOD_THRESHHOLD	_CFG_MISC(0x0602) //T=u32, D=300, syn flood threshold (packet/second)
#define CSID_C_MISC_DOS_SYN_FLOOD_TIMEOUT  		_CFG_MISC(0x0603) //T=u32, D=10, syn flood timeout
#define CSID_C_MISC_DOS_ICMP_FLOOD         		_CFG_MISC(0x0604) //T=u8, whether protecting DoS attack-ping flood, 1:yes 0:no
#define CSID_C_MISC_DOS_ICMP_FLOOD_THRESHHOLD	_CFG_MISC(0x0605) //T=u32, D=300, icmp flood threshold (packet/second)
#define CSID_C_MISC_DOS_ICMP_FLOOD_TIMEOUT  	_CFG_MISC(0x0606) //T=u32, D=10, icmp flood timeout
#define CSID_C_MISC_DOS_UDP_FLOOD         		_CFG_MISC(0x0607) //T=u8, whether protecting DoS attack-udp flood, 1:yes 0:no
#define CSID_C_MISC_DOS_UDP_FLOOD_THRESHHOLD	_CFG_MISC(0x0608) //T=u32, D=300, udp flood threshold (packet/second)
#define CSID_C_MISC_DOS_UDP_FLOOD_TIMEOUT  		_CFG_MISC(0x0609) //T=u32, D=10, udp flood timeout
#define CSID_C_MISC_DOS_PORT_SCAN	 			_CFG_MISC(0x060a) //T=u8, whether protecting DoS attack port scan, 1:yes 0:no
#define CSID_C_MISC_DOS_PORT_SCAN_THRESHHOLD	_CFG_MISC(0x060b) //T=u32, D=200, port scan threshold (packet/second)
#define CSID_C_MISC_DOS_SMURF        			_CFG_MISC(0x060c) //T=u8, whether protecting DoS attack-sumrf, 1:yes 0:no
#define CSID_C_MISC_DOS_SNORK         			_CFG_MISC(0x060d) //T=u8, whether protecting DoS attack-snork, 1:yes 0:no
#define CSID_C_MISC_DOS_LAND         			_CFG_MISC(0x060e) //T=u8, whether protecting DoS attack-land, 1:yes 0:no
#define CSID_C_MISC_DOS_FRAGGLE        			_CFG_MISC(0x060f) //T=u8, whether protecting DoS attack-fraggle, 1:yes 0:no
#define CSID_C_MISC_DOS_ICMP_DEFENDER	  		_CFG_MISC(0x0610) //T=u8, whether protecting DoS attack-icmp defender, 1:yes 0:no
#define CSID_C_MISC_DOS_UNKNOWN_PROTOCOL		_CFG_MISC(0x0611) //T=u8, whether protecting DoS attack-unknown protocol, 1:yes 0:no
#define CSID_C_MISC_DOS_PING_DEATH		  		_CFG_MISC(0x0612) //T=u8, whether protecting DoS attack-ping death, 1:yes 0:no
#define CSID_C_MISC_DOS_ICMP_FRAGMENT	  		_CFG_MISC(0x0613) //T=u8, whether protecting DoS attack-icmp fragment, 1:yes 0:no
#define CSID_C_MISC_DOS_SYN_FRAGMENT	  		_CFG_MISC(0x0614) //T=u8, whether protecting DoS attack-syn fragment, 1:yes 0:no
#define CSID_C_MISC_DOS_TRACE_ROUTE		  		_CFG_MISC(0x0615) //T=u8, whether protecting DoS attack-trace route, 1:yes 0:no
#define CSID_C_MISC_DOS_TEAR_DROP		  		_CFG_MISC(0x0616) //T=u8, whether protecting DoS attack-tear drop, 1:yes 0:no
#define CSID_C_MISC_DOS_TCP_ABNORMAL	  		_CFG_MISC(0x0617) //T=u8, whether protecting DoS attack-tcp abnormal, 1:yes 0:no
#define CSID_C_MISC_DOS_IP_SPOOF		  		_CFG_MISC(0x0618) //T=u8, whether protecting DoS attack-ip spoofing, 1:yes 0:no
#define CSID_C_MISC_DOS_ARP_SPOOFING		  _CFG_MISC(0x0619) //T=u8, whether protecting DoS attack-arp spoofing, 1:yes 0:no
#define CSID_C_MISC_DOS_ARP_SPOOFING_THRESHHOLD   _CFG_MISC(0x061A) //T=u32, D=300, arp spoofing threshold (packet/second)
#define CSID_C_MISC_DOS_ARP_SPOOFING_TIMEOUT  		_CFG_MISC(0x061B) //T=u32, D=10, arp spoofing timeout

#define CSID_C_MISC_DOS_LOG_ENABLE	       		_CFG_MISC(0x0620) //T=u8, whether protecting DoS log is enable or not, 1:yes 0:no

/*NAT TTL adjust*/
#define CSID_C_MISC_NAT_TTL_ENABLE              _CFG_MISC(0x0700) //T=u32, D=0, Adjust TTL number

/*JFFS2 Storage*/
#define CSID_C_MISC_JFFS2_ENABLE                _CFG_MISC(0x0710) //T=u32, D=0, Create jffs2 storage file system
#define CSID_C_MISC_JFFS2_STATUS                _CFG_MISC(0x0711) //T=u32, D=0, 0:not ready, 1:formated

/*IPV6 MLD proxy*/
#define CSID_C_MISC_MLD_ENABLE				_CFG_MISC(0x0712)	//T=u8,	 MLD enable, 1:yes 0:no

/* UPnP clear UDP port cache flag */
#define CSID_C_MISC_UPNP_CLEAR_UDP_CONNTRACK    _CFG_MISC(0x0713)   //T=u8, D=0, clear conntrack cache after deleting UDP port mapping

/*IPV6 Configuration Items*/
#define CSID_C_MISC_REMOTE_ADMIN_IPv6_ENABLE		_CFG_MISC(0x1001)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MISC_REMOTE_ADMIN_IPv6_IP		    _CFG_MISC(0x1002)	//T=str, the ipv6 of administrators
#define CSID_C_MISC_REMOTE_ADMIN_IPv6_PORT		_CFG_MISC(0x1003)	//T=u16, the port of adminitrators 
#define CSID_C_MISC_REMOTE_ADMIN_IPv6_NM		    _CFG_MISC(0x1004)	//T=u16, the prefix of ipv6 address
#define CSID_C_MISC_MLD_SNOOP_ENABLE		_CFG_MISC(0x1010)	//T=u8,	D=0, MLD Snoop enable, 1:yes 0:no  

/* IPv4 Remote Admin for HTTPS */
#define CSID_C_MISC_HTTPS_REMOTE_ADMIN_ENABLE		_CFG_MISC(0x1020)	//T=u8, rule enable, 1:yes 0:no
#define CSID_C_MISC_HTTPS_REMOTE_ADMIN_IP		    _CFG_MISC(0x1030)	//T=ipv4, the ip of administrators
#define CSID_C_MISC_HTTPS_REMOTE_ADMIN_PORT		_CFG_MISC(0x1040)	//T=u16, the port of adminitrators 
#define CSID_C_MISC_HTTPS_REMOTE_ADMIN_NM		    _CFG_MISC(0x1050)	//T=u16, the netmask of adminitrators 
/* reserve 16 space */

/* UPnP port mapping */
#define CSID_C_MISC_UPNP_PORT_MAPPING_ENABLE            _CFG_MISC(0x1100)   //T=u8, D=0
#define CSID_C_MISC_UPNP_PORT_MAPPING_PROTOCOL          _CFG_MISC(0x1110)   //T=str
#define CSID_C_MISC_UPNP_PORT_MAPPING_EXTERNAL_PORT     _CFG_MISC(0x1120)   //T=u32
#define CSID_C_MISC_UPNP_PORT_MAPPING_INTERNAL_IP       _CFG_MISC(0x1130)   //T=str
#define CSID_C_MISC_UPNP_PORT_MAPPING_INTERNAL_PORT     _CFG_MISC(0x1140)   //T=u32
#define CSID_C_MISC_UPNP_PORT_MAPPING_DESCRIPTION       _CFG_MISC(0x1150)   //T=str
#define CSID_C_MISC_UPNP_PORT_MAPPING_REMOTE_HOST       _CFG_MISC(0x1160)   //T=str
#define CSID_C_MISC_UPNP_PORT_MAPPING_LEASE_DURATION    _CFG_MISC(0x1170)   //T=u32
/* reserve 16 space */

/* Static ARP mapping */
#define CSID_C_MISC_STATIC_ARP_MAPPING_ENABLE		_CFG_MISC(0x1200)	//T=u8, the rule enable of static arp mapping
#define CSID_C_MISC_STATIC_ARP_MAPPING_IP		_CFG_MISC(0x1300)	//T=ipv4, the IP address on static arp mapping
#define CSID_C_MISC_STATIC_ARP_MAPPING_MAC		_CFG_MISC(0x1400)	//T=mac, the MAC address on static arp mapping 
/* reserve 256 space */

/* TECO */
#define CSID_C_MISC_DCH_POWER_RUSAGE 			_CFG_MISC(0x1500)   //T=u32, D=0, DCH power usage list 
#define CSID_C_MISC_DCH_TIME_STAMP 	          	_CFG_MISC(0x1700)   //T=u32, D=0, DCH time stamp list 
#define CSID_C_MISC_PUR_TMP_ELECTRICITY         _CFG_MISC(0x1900)   //T=u32, D=0, dch ac over the enable setup

/*Remote link 3 for IODATA*/
#define CSID_C_MISC_IODATA_RL3_MANUAL			_CFG_MISC(0x2000)	//T=u8, D=0, auto or manual
#define CSID_C_MISC_IODATA_RL3_PIN				_CFG_MISC(0x2010)	//T=str, PIN code
#define CSID_C_MISC_IODATA_RL3_DEFAULT_PIN		_CFG_MISC(0x201F)	//T=str, PIN code
#define CSID_C_MISC_IODATA_RL3_IP				_CFG_MISC(0x2020)	//T=str, the IP manual
#define CSID_C_MISC_IODATA_RL3_SRC_PORT0		_CFG_MISC(0x2030)	//T=u32, mapping source port
#define CSID_C_MISC_IODATA_RL3_DST_PORT0		_CFG_MISC(0x2040)	//T=u32, mapping dst port
#define CSID_C_MISC_IODATA_RL3_SRC_PORT1		_CFG_MISC(0x2050)	//T=u32, mapping source port
#define CSID_C_MISC_IODATA_RL3_DST_PORT1		_CFG_MISC(0x2060)	//T=u32, mapping dst port
#define CSID_C_MISC_IODATA_RL3_SRC_PORT2		_CFG_MISC(0x2070)	//T=u32, mapping source port
#define CSID_C_MISC_IODATA_RL3_DST_PORT2		_CFG_MISC(0x2080)	//T=u32, mapping dst port
#define CSID_C_MISC_IODATA_RL3_SRC_PORT3		_CFG_MISC(0x2090)	//T=u32, mapping source port
#define CSID_C_MISC_IODATA_RL3_DST_PORT3		_CFG_MISC(0x20a0)	//T=u32, mapping dst port
#define CSID_C_MISC_IODATA_RL3_SRC_PORT4		_CFG_MISC(0x20b0)	//T=u32, mapping source port
#define CSID_C_MISC_IODATA_RL3_DST_PORT4		_CFG_MISC(0x20c0)	//T=u32, mapping dst port

#define CSID_C_MISC_IODATA_RL3_DEVICE_MAC		_CFG_MISC(0x20F0)	//T=mac, record remote device mac
#define CSID_C_MISC_IODATA_RL3_DEVICE_METHOD	_CFG_MISC(0x20F1)	//T=str, record remote device method
#define CSID_C_MISC_IODATA_RL3_DEVICE_NAME 		_CFG_MISC(0x20F2)	//T=str, record remote device name

#define CSID_C_MISC_IODATA_RL3_DEVICE1_MAC		_CFG_MISC(0x20F4)	//T=mac, record remote device mac
#define CSID_C_MISC_IODATA_RL3_DEVICE1_METHOD	_CFG_MISC(0x20F5)	//T=str, record remote device method
#define CSID_C_MISC_IODATA_RL3_DEVICE1_NAME 	_CFG_MISC(0x20F6)	//T=str, record remote device name

#define CSID_C_MISC_IODATA_RL3_DEVICE2_MAC		_CFG_MISC(0x20F8)	//T=mac, record remote device mac
#define CSID_C_MISC_IODATA_RL3_DEVICE2_METHOD	_CFG_MISC(0x20F9)	//T=str, record remote device method
#define CSID_C_MISC_IODATA_RL3_DEVICE2_NAME 	_CFG_MISC(0x20FA)	//T=str, record remote device name

#define CSID_C_MISC_IODATA_RL3_DEVICE3_MAC		_CFG_MISC(0x20FC)	//T=mac, record remote device mac
#define CSID_C_MISC_IODATA_RL3_DEVICE3_METHOD	_CFG_MISC(0x20FD)	//T=str, record remote device method
#define CSID_C_MISC_IODATA_RL3_DEVICE3_NAME 	_CFG_MISC(0x20FE)	//T=str, record remote device name

#define CSID_C_MISC_IODATA_RL3_DEFAULT_SET		_CFG_MISC(0x2100)	//T=u32, D=1, record it dufault

#define CSID_C_MISC_IODATA_RL3_AUTO_SRC_PORT0		_CFG_MISC(0x2110)	//T=u32, mapping source port
#define CSID_C_MISC_IODATA_RL3_AUTO_DST_PORT0		_CFG_MISC(0x2120)	//T=u32, mapping dst port
#define CSID_C_MISC_IODATA_RL3_AUTO_SRC_PORT1		_CFG_MISC(0x2130)	//T=u32, mapping source port
#define CSID_C_MISC_IODATA_RL3_AUTO_DST_PORT1		_CFG_MISC(0x2140)	//T=u32, mapping dst port
#define CSID_C_MISC_IODATA_RL3_AUTO_SRC_PORT2		_CFG_MISC(0x2150)	//T=u32, mapping source port
#define CSID_C_MISC_IODATA_RL3_AUTO_DST_PORT2		_CFG_MISC(0x2160)	//T=u32, mapping dst port
#define CSID_C_MISC_IODATA_RL3_AUTO_SRC_PORT3		_CFG_MISC(0x2170)	//T=u32, mapping source port
#define CSID_C_MISC_IODATA_RL3_AUTO_DST_PORT3		_CFG_MISC(0x2180)	//T=u32, mapping dst port
#define CSID_C_MISC_IODATA_RL3_AUTO_SRC_PORT4		_CFG_MISC(0x2190)	//T=u32, mapping source port
#define CSID_C_MISC_IODATA_RL3_AUTO_DST_PORT4		_CFG_MISC(0x21a0)	//T=u32, mapping dst port


/* Status Items */
#define CSID_S_MISC_ALTERED					_STA_MISC(0x0002)	//T=u8, ui altered
#define CSID_S_MISC_PING_PID				_STA_MISC(0x0003)	//T=u32, the pid of ping
#define CSID_S_MISC_PING_STATUS				_STA_MISC(0x0004) 	//T=u32, the status of ping 
#define CSID_S_MISC_PING_TARGET_TMP			_STA_MISC(0x0005)	//T=str, default target for ping test

#define CSID_S_MISC_UPNP_CHK				_STA_MISC(0x0006)	//T=u32, default ,upnp_chk 1 cannot run start or upnp_chk 2 cannot run stop
#define CSID_S_MISC_WEBRB_ALTERED			_STA_MISC(0x0007)	//T=u8, ui altered
#define CSID_S_MISC_WEBRB_ISSUEID			_STA_MISC(0x0008)	//T=u8, issue ID recorded 
#define CSID_S_MISC_DLNA_STATUS				_STA_MISC(0x0009)	//T=u8, D=0, DLNA 0:Off , 1: On
#define CSID_S_MISC_DLNA_RESCAN				_STA_MISC(0x000A)	//T=u8, D=0, DLNA 1: Rescan all files
#define CSID_S_MISC_RESTART				    _STA_MISC(0x000B)	//T=u8, D=0, restart commander
#define CSID_S_MISC_INTERNET_BREAK          _STA_MISC(0x000C)	//T=u8, internet break for manual
#define CSID_S_MISC_IGMP_SNOOP_ALTERED		_STA_MISC(0x000D)	//T=u8, ui igmp snoop altered
#define CSID_S_MISC_WEBRB_PAUSE			    _STA_MISC(0x000E)	//T=u8, pause time for webrb


/* statistics */
#define CSID_S_MISC_TRACERT_TARGET_TMP		_STA_MISC(0x000F)	//T=str, default target for traceroute test
#define CSID_S_MISC_TRACERT_STATUS			_STA_MISC(0x0010)	//T=u32, the status of traceroute
#define CSID_S_MISC_TRACERT_PID			    _STA_MISC(0x0011)	//T=u32, the pid of ping
#define CSID_S_MISC_WAN_PKT_IN			    _STA_MISC(0x0012)	//T=u32, number of incoming packet of wan
#define CSID_S_MISC_WAN_PKT_OUT			    _STA_MISC(0x0013)	//T=u32, number of outgoing packet of wan
#define CSID_S_MISC_LAN_PKT_IN			    _STA_MISC(0x0013)	//T=u32, number of incoming packet of lan
#define CSID_S_MISC_LAN_PKT_OUT			    _STA_MISC(0x0014)	//T=u32, number of outgoing packet of lan
#define CSID_S_MISC_WAN_BYTE_IN			    _STA_MISC(0x0015)	//T=u32, number of incoming bytes of wan
#define CSID_S_MISC_WAN_BYTE_OUT			_STA_MISC(0x0016)	//T=u32, number of outgoing bytes of wan
#define CSID_S_MISC_LAN_BYTE_IN			    _STA_MISC(0x0017)	//T=u32, number of incoming bytes of lan
#define CSID_S_MISC_LAN_BYTE_OUT			_STA_MISC(0x0018)	//T=u32, number of outgoing bytes of lan

/*dos*/
#define CSID_S_MISC_DOS_BLOCK_ALTER				_STA_MISC(0x0020)	//T=u8, ui altered
#define CSID_S_MISC_DOS_ALTERED			    _STA_MISC(0x0021)	//T=u8, ui altered for DoS

#define CSID_S_MISC_TELNETD_SETTING_ALTERED _STA_MISC(0x0022) //T=u8, Telnetd setting is altered

#define CSID_S_MISC_FIREWALL_FUNCTION_ALTERED	_STA_MISC(0x0023) //T=u8, firewall function setting is altered

#define CSID_S_MISC_STANDBY_LED_ON		_STA_MISC(0x0024) //T=u32, standymode led control

#define CSID_S_MISC_MYDLINK_RX	            	_STA_MISC(0x0030)	//T=u32,  received traffic for mydlink
#define CSID_S_MISC_MYDLINK_TX	            	_STA_MISC(0x0031)	//T=u32,  transmitted traffic for mydlink
#define CSID_S_MISC_MYDLINK_DISABLE             _STA_MISC(0x0032)   //T=u32, default value is 0 always on

/*shareport UPNP*/
#define CSID_S_MISC_UPNP_UPPER_LAYER_IGD_ENABLE	    _STA_MISC(0x0040)	//T=u32, upnpc request upper layer upnp IGD 
#define CSID_S_MISC_UPNP_UPPER_LAYER_IGD_IP			_STA_MISC(0x0041)	//T=str, upper layer upnp IGD IP

#define CSID_S_MISC_SUCRE_STATUS               	 _STA_MISC(0x0050)       //T=u8, sucre status
#define CSID_S_MISC_SUCRE_RESET               	 _STA_MISC(0x0051)       //T=u8, sucre reset
#define CSID_S_MISC_SUCRE_WPS               	 _STA_MISC(0x0052)       //T=u8, sucre wps

#define CSID_S_LAN_WAN_BRIDGE_ALTERED			 _STA_MISC(0x0060)       //T=u8, Lan with Wan bridge altered
#define CSID_S_UI_PASSWORD_CHANGE_ALTERED		 _STA_MISC(0x0061)       //T=u8, ui password has been changed altered

/*Misc*/
#define CSID_S_MISC_JFFS2_SYNC                  _STA_MISC(0x0100) //T=u32, D=0, 0: not need, 1: need
 
#define CSID_S_MISC_QUOTA_INTERFACE_RX          _STA_MISC(0x0101)  //T=u32, received traffic of current interface(KB).
#define CSID_S_MISC_QUOTA_INTERFACE_TX          _STA_MISC(0x0102)  //T=u32, transmitted traffic of current interface(KB).

#define CSID_S_MISC_UPNP_DISCOVER          _STA_MISC(0x0103)  //T=u8, trigger MiniUPnP to send discover.

#define CSID_S_MISC_PING_FROM_SRC_IF_TMP	_STA_MISC(0x0104)	//T=u32, default ping from source interface, 0: Auto, 1: LAN, 2: WAN
#define CSID_S_MISC_PING_FROM_SRC_IF_IDX_TMP	_STA_MISC(0x0105)	//T=u32, D=0, 0: Auto, 1: LANWAN1

#define CSID_S_MISC_TRACERT_FROM_SRC_IF_TMP	_STA_MISC(0x0106)	//T=u32, default traceroute from source interface, 0: Auto, 1: LAN, 2: WAN
#define CSID_S_MISC_TRACERT_FROM_SRC_IF_IDX_TMP	_STA_MISC(0x0107)	//T=u32, default traceroute from source interface index 

#define CSID_S_MISC_TRACERT_PROTOCOL_TMP	_STA_MISC(0x0108)	//T=u32, default traceroute protocol, 0: UDP, 1: ICMP

#define CSID_S_MISC_STATIC_ARP_MAPPING_ALTERED  _STA_MISC(0x010A)   //T=u8, D=0, static arp mapping altered

/* UPnP port mapping altered */
#define CSID_S_MISC_UPNP_PORT_MAPPING_ALTERED   _STA_MISC(0x0200)   //T=u8, D=0
/* reserve 16 space */

#define CSID_S_MISC_BUSY_COUNTDOWN			_STA_MISC(0x0300)   //T=u8, D=0, for UI reload page countdown 

#define CSID_S_MISC_UPNPC_PID				_STA_MISC(0x0310)	//T=u32, the pid of UPNPC
#define CSID_S_MISC_UPNPC_STATUS			_STA_MISC(0x0311) 	//T=u32, the status of UPNPC 
#define CSID_S_MISC_UPNPC_A_PORT_TMP			_STA_MISC(0x0312)	//T=str, add maping port for UPNPC
#define CSID_S_MISC_UPNPC_A_PROTOL_TMP			_STA_MISC(0x0313)	//T=u8, D=0, add protocol for UPNPC 0:UDP 1:TCP
#define CSID_S_MISC_UPNPC_D_PORT_TMP			_STA_MISC(0x0314)	//T=str, del maping port for UPNPC
#define CSID_S_MISC_UPNPC_D_PROTOL_TMP			_STA_MISC(0x0315)	//T=u8, D=0, del protocol for UPNPC 0:UDP 1:TCP

#define CSID_S_MISC_JFFS2_SYNC_MODIFY_COUNT                 _STA_MISC(0x0316) //T=u32, D=0, 0: not need, 1: need
#define CSID_S_MISC_JFFS2_SYNC_TICK                  _STA_MISC(0x0317) //T=u32, D=0, 0: not need, 1: need

#define CSID_S_MISC_UPNPC_EXTERN_WAN_IP         _STA_MISC(0x0320)   //T=ipv4, D={0.0.0.0},extern WAN IP

#define CSID_S_MISC_MYDLINK_INDEX			_STA_MISC(0x0330)	//T=u32, D=0, 1:get mydlink status 2:mydlink sign up 3:sign in

#define CSID_S_MISC_PUR_SWITCH	       		_STA_MISC(0x0340)	//T=u32, D=0, 0:off 1:on
#define CSID_S_MISC_PUR_ON_POWER	       	_STA_MISC(0x0341)	//T=u32, D=0, pur on power
#define CSID_S_MISC_PUR_OFF_POWER	   		_STA_MISC(0x0342)	//T=u32, D=0, pur off power
#define CSID_S_MISC_DCH_RESET_TRIGGER       _STA_MISC(0x0343)   //T=u32, D=0, reset pur data
#define CSID_S_MISC_PUR_GET_TRIGGER         _STA_MISC(0x0344)   //T=u32, D=0, reset pur data
#define CSID_S_MISC_DCH_TYPE	       		_STA_MISC(0x0345)	//T=u32, D=0, DCH type 1:A/C 2:dehumidifier
#define CSID_S_MISC_PUR_TMP_ELECTRICITY		_STA_MISC(0x0350)	//T=u32, D=0, pur tmp electricity for one day

#define CSID_S_MISC_DCH_AC_DOWN        _STA_MISC(0x03a0)   //T=u32, D=0, dch ac over the enable setup 
#define CSID_S_MISC_DCH_DH_DOWN        _STA_MISC(0x03b0)   //T=u32, D=0, dch dh over the enable setup 

/*Remote link 3 for IODATA*/
#define CSID_S_MISC_IODATA_RL3_INPUT_PORT		        _STA_MISC(0x0400)	//T=u32, record list INPUT port
#define CSID_S_MISC_IODATA_RL3_OUTPUT_IP_PORT		    _STA_MISC(0x0410)	//T=str, record list OUTPUT IP:port
#define CSID_S_MISC_IODATA_RL3_PINCODE_CHANGE_ALTER	    _STA_MISC(0x0420)	//T=u32, uuidgen alter
#define CSID_S_MISC_IODATA_RL3_TCP_LISTEN_DEVICE1	    _STA_MISC(0x0421)	//T=u32, TCP_LISTEN_ADDR_DEVICE_1
#define CSID_S_MISC_IODATA_RL3_TCP_LISTEN_DEVICE2	    _STA_MISC(0x0422)	//T=u32, TCP_LISTEN_ADDR_DEVICE_2
#define CSID_S_MISC_IODATA_RL3_AUTO_IP	                _STA_MISC(0x0425)	//T=str, auto run IP
#define CSID_S_MISC_IODATA_RL3_PINCODE_STATE            _STA_MISC(0x0426)	//T=u32, D=0, Default PINCODE state

#define CSID_S_MISC_DASHBOARD_WRITING            _STA_MISC(0x0430)	//T=u8, D=0, indicate if dashboard file is writing

#endif //ifndef _CSID_MISC_H
