/*
 * @file Member ID Definition of Group REMOTE.
 *
 * Moderator: Aaron Wu
 * Group ID: 0x00760000/0x80760000
 */

#ifndef _CSID_REMOTE_H
#define _CSID_REMOTE_H

#include "csid_gid.h"

#define _CFG_REMOTE(x)       (CSID_GIDC_REMOTE|(x))
#define _STA_REMOTE(x)       (CSID_GIDS_REMOTE|(x))

/*TRAP Message sender*/
#define CSID_C_REMOTE_AP_CONTROLLER_ENABLE		_CFG_REMOTE(0x0000)  //T=u16, AP controller main enable, 0:disable, 1:enable
#define CSID_C_REMOTE_AP_TRAP_ENABLE                    _CFG_REMOTE(0x0001)  //T=u16, AP trap messgae enable, 0:disable, 1:enable
#define CSID_C_REMOTE_AP_TRAP_ALERT_L1_ENABLE           _CFG_REMOTE(0x0002)  //T=u16, AP trap alert Levet 1 enable or disable, 0: disable, 1:enable
#define CSID_C_REMOTE_AP_TRAP_ALERT_L2_ENABLE           _CFG_REMOTE(0x0003)  //T=u16, AP trap alert Levet 2 enable or disable, 0: disable, 1:enable
#define CSID_C_REMOTE_AP_TRAP_ALERT_L3_ENABLE           _CFG_REMOTE(0x0004)  //T=u16, AP trap alert Levet 3 enable or disable, 0: disable, 1:enable
#define CSID_C_REMOTE_AP_TRAP_ALERT_L4_ENABLE           _CFG_REMOTE(0x0005)  //T=u16, AP trap alert Levet 4 enable or disable, 0: disable, 1:enable
#define CSID_C_REMOTE_AP_TRAP_ALERT_L5_ENABLE           _CFG_REMOTE(0x0006)  //T=u16, AP trap alert Levet 5 enable or disable, 0: disable, 1:enable
#define CSID_C_REMOTE_AP_TRAP_AP_TRAFFIC_ENABLE         _CFG_REMOTE(0x0007)  //T=u16, AP trap Tx Rx Packet enable
#define CSID_C_REMOTE_AP_TRAP_USER_TRAFFIC_ENABLE       _CFG_REMOTE(0x0008)  //T=u16, AP trap users upload, download traffic enable,disable
#define CSID_C_REMOTE_AP_TRAP_USER_TXRX_ENABLE          _CFG_REMOTE(0x0009)  //T=u16, AP trap users Tx Rate, Rx Raite traffic enable,disable
#define CSID_C_REMOTE_AP_PROTOCOL_AP			_CFG_REMOTE(0x0010)  //T=u16, D=0, set AP support protocol, 0: udp, 1: tcp
#define CSID_C_REMOTE_AP_PROTOCOL_AC			_CFG_REMOTE(0x0011)  //T=u8, D=0, AC protoocl, 0:udp, 1:tcp
#define CSID_C_REMOTE_AP_TCPPORT_AP			_CFG_REMOTE(0x0012)  //T=u8, D=0, AP TCP socket default port
#define CSID_C_REMOTE_AP_TCPPORT_AC			_CFG_REMOTE(0x0013)  //T=u8, D=0, AC TCP socket default port
#define CSID_C_REMOTE_AP_SUPPORT_NUMBER			_CFG_REMOTE(0x0014)  //T=u8, D=0, Set Support AP Numbet, 0: default 128
#define CSID_C_REMOTE_AP_SUPPORT_COMMBUS		_CFG_REMOTE(0x0015)  //T=u8, D=0, Set if support communication bus, 0: disable, 1: enable

/*Trap Message sender Counter*/
#define CSID_C_REMOTE_AP_TRAP_ALERT_L1_COUNTER		_CFG_REMOTE(0x0100)  //T=u32, AP trap alert Level 1 counter
#define CSID_C_REMOTE_AP_TRAP_ALERT_L2_COUNTER          _CFG_REMOTE(0x0101)  //T=u32, AP trap alert Level 2 counter
#define CSID_C_REMOTE_AP_TRAP_ALERT_L3_COUNTER          _CFG_REMOTE(0x0102)  //T=u32, AP trap alert Level 3 counter
#define CSID_C_REMOTE_AP_TRAP_ALERT_L4_COUNTER          _CFG_REMOTE(0x0103)  //T=u32, AP trap alert Level 4 counter
#define CSID_C_REMOTE_AP_TRAP_ALERT_L5_COUNTER          _CFG_REMOTE(0x0104)  //T=u32, AP trap alert Level 5 counter
#define CSID_C_REMOTE_AP_TRAP_TRAFFIC_COUNTER		_CFG_REMOTE(0x0105)  //T=u32, D=5, AP trap traffic static counter

/*AP Function config setting*/
#define CSID_C_REMOTE_AP_TRUSTAP_CUSTOM_NAME		_CFG_REMOTE(0x0106)  //T=str, set trust AP custom name
#define CSID_C_REMOTE_AP_TRUSTAP_LOCK_MAC		_CFG_REMOTE(0x0107)  //T=mac, trust AP lock mac

/*AP Alert Setting*/
#define CSID_C_REMOTE_AP_TRAP_ALERT_L1_RECORE_ENABLE	_CFG_REMOTE(0x0200)  //T=u8, D=0, Alter 1 enable,disable, 0:disable, 1:enable
#define CSID_C_REMOTE_AP_TRAP_ALERT_L2_RECORE_ENABLE    _CFG_REMOTE(0x0201)  //T=u8, D=0, Alter 2 enable,disable, 0:disable, 1:enable
#define CSID_C_REMOTE_AP_TRAP_ALERT_L3_RECORE_ENABLE    _CFG_REMOTE(0x0202)  //T=u8, D=0, Alter 3 enable,disable, 0:disable, 1:enable
#define CSID_C_REMOTE_AP_TRAP_ALERT_L4_RECORE_ENABLE    _CFG_REMOTE(0x0203)  //T=u8, D=0, Alter 3 enable,disable, 0:disable, 1:enable
#define CSID_C_REMOTE_AP_TRAP_ALERT_L5_RECORE_ENABLE    _CFG_REMOTE(0x0204)  //T=u8, D=0, Alter 5 enable,disable, 0:disable, 1:enable

#define CSID_C_REMOTE_AP_TRAP_ALERT_L1_RECORE_TYPE	_CFG_REMOTE(0x0205)  //T=u16, D=0, record Alert level 1 Type, 1:WebUI,2:Email,3:Syslog,4:SMS, using bit for enable
#define CSID_C_REMOTE_AP_TRAP_ALERT_L2_RECORE_TYPE	_CFG_REMOTE(0x0206)  //T=u16, D=0, record Alert level 1 Type, 1:WebUI,2:Email,3:Syslog,4:SMS, using bit for enable
#define CSID_C_REMOTE_AP_TRAP_ALERT_L3_RECORE_TYPE	_CFG_REMOTE(0x0207)  //T=u16, D=0, record Alert level 1 Type, 1:WebUI,2:Email,3:Syslog,4:SMS, using bit for enable
#define CSID_C_REMOTE_AP_TRAP_ALERT_L4_RECORE_TYPE	_CFG_REMOTE(0x0208)  //T=u16, D=0, record Alert level 1 Type, 1:WebUI,2:Email,3:Syslog,4:SMS, using bit for enable
#define CSID_C_REMOTE_AP_TRAP_ALERT_L5_RECORE_TYPE	_CFG_REMOTE(0x0209)  //T=u16, D=0, record Alert level 1 Type, 1:WebUI,2:Email,3:Syslog,4:SMS, using bit for enable
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_REBOOT		_CFG_REMOTE(0x0220)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_RESET             _CFG_REMOTE(0x0221)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_CPU		_CFG_REMOTE(0x0222)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_MEMORY            _CFG_REMOTE(0x0223)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_CONFIG		_CFG_REMOTE(0x0224)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_PROPOSAL		_CFG_REMOTE(0x0225)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5

#define CSID_C_REMOTE_AP_ALERT_NOTIFY_APMODULE24	_CFG_REMOTE(0x0230)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_APMODULE50	_CFG_REMOTE(0x0231)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_APOPMODE24	_CFG_REMOTE(0x0232)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_APOPMODE50	_CFG_REMOTE(0x0233)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_APSTA24		_CFG_REMOTE(0x0234)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_APSTA50		_CFG_REMOTE(0x0235)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_APCHANNEL24	_CFG_REMOTE(0x0236)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_APCHANNEL50	_CFG_REMOTE(0x0237)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_APTXPOWER24	_CFG_REMOTE(0x0238)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_APTXPOWER50	_CFG_REMOTE(0x0239)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_APCLICONN24	_CFG_REMOTE(0x0240)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_APCLICONN50	_CFG_REMOTE(0x0241)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_APWIDS24		_CFG_REMOTE(0x0242)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5
#define CSID_C_REMOTE_AP_ALERT_NOTIFY_APWIDS50		_CFG_REMOTE(0x0243)  //T=u8, D=0, enable notify alert, 0:disable, 1:alert 1, 2alert 2, 3: alert 3, 4: alert 4, 5: alert 5

//Trust AP Items	
#define CSID_S_REMOTE_AP_TRUSTAP_NAME                   _STA_REMOTE(0x0000)  //T=str, trust AP name
#define CSID_S_REMOTE_AP_TRUSTAP_IP                     _STA_REMOTE(0x0080)  //T=ipv4, trust AP IP Address
#define CSID_S_REMOTE_AP_TRUSTAP_MAC                    _STA_REMOTE(0x0100)  //T=mac, trust AP MAC Address
#define CSID_S_REMOTE_AP_TRUSTAP_SSID_AP1               _STA_REMOTE(0x0180)  //T=str, trust AP 2.4G SSID name
#define CSID_S_REMOTE_AP_TRUSTAP_SSID_AP2               _STA_REMOTE(0x0200)  //T=str, trust AP 5G SSID name
#define CSID_S_REMOTE_AP_TRUSTAP_STATUS                 _STA_REMOTE(0x0280)  //T=u8, trust AP status, 0:offline , 1:online, 2:denined, 3: Can Config, 4: Can't Config, 5:reboot success, 6:reboot fail, 7:reset success, 8:reset fail
#define CSID_S_REMOTE_AP_TRUSTAP_TRAFFIC                _STA_REMOTE(0x0300)  //T=u32, trust AP traffic statistic
#define CSID_S_REMOTE_AP_TRUSTAP_VID_LOCATION_AP1       _STA_REMOTE(0x0380)  //T=u16, trust AP VID location
#define CSID_S_REMOTE_AP_TRUSTAP_VID_LOCATION_AP2       _STA_REMOTE(0x0400)  //T=u16, trust AP VID location
#define CSID_S_REMOTE_AP_TRUSTAP_CHANNEL_AP1            _STA_REMOTE(0x0480)  //T=u16, trust AP 2.4G channel
#define CSID_S_REMOTE_AP_TRUSTAP_CHANNEL_AP2            _STA_REMOTE(0x0500)  //T=u16, trust AP 5G channel
#define CSID_S_REMOTE_AP_TRUSTAP_ASSOICATED_AP1         _STA_REMOTE(0x0580)  //T=u32, trust AP 2.4G Assoicated station number
#define CSID_S_REMOTE_AP_TRUSTAP_ASSOICATED_AP2         _STA_REMOTE(0x0600)  //T=u32, trust AP 5G Assoicated station number
#define CSID_S_REMOTE_AP_TRUSTAP_CPU_USED               _STA_REMOTE(0x0680)  //T=str, trust AP CPU Used
#define CSID_S_REMOTE_AP_TRUSTAP_MEMORY_USED            _STA_REMOTE(0x0700)  //T=u32, trust AP Memory Used
#define CSID_S_REMOTE_AP_TRUSTAP_ROM_ID			_STA_REMOTE(0x0780)  //T=str, trust AP ROM ID
#define CSID_S_REMOTE_AP_TRUSTAP_PARSED			_STA_REMOTE(0x0800)  //T=u16, record which trust AP is get
#define CSID_S_REMOTE_AP_TRUSTAP_TX_MBPS		_STA_REMOTE(0x0880)  //T=u32, record trust AP 2.4G+5G Upload Mbps
#define CSID_S_REMOTE_AP_TRUSTAP_RX_MBPS		_STA_REMOTE(0x0900)  //T=u32, record trust AP 2.4G+5G Download Mbps
#define CSID_S_REMOTE_AP_TRUSTAP_TXRX_MBPS		_STA_REMOTE(0x0980)  //T=u32, record trust AP 2.4G+5G Totoal Mbps
#define CSID_S_REMOTE_AP_TRUSTAP_UPTIME			_STA_REMOTE(0x0a00)  //T=str, record trust AP Working time
#define CSID_S_REMOTE_AP_TRUSTAP_TRAP_TIME		_STA_REMOTE(0x0a80)  //T=str, record trust AP sending trap message time
#define CSID_S_REMOTE_AP_TRUSTAP_TRAP_STIME		_STA_REMOTE(0x0b00)  //T=str, record trust AP sending trap message time for string format, e.g. Wed Jul 7 15:37:53 2010
#define CSID_S_REMOTE_AP_TRUSTAP_ALLOW_DENY		_STA_REMOTE(0x0b80)  //T=u32, show AP If allow or deny
#define CSID_S_REMOTE_AP_TRUSTAP_HAVE5G			_STA_REMOTE(0x0c00)  //T=u32, show AP If have 5G
#define CSID_S_REMOTE_AP_TRUSTAP_PROPO_STATUS		_STA_REMOTE(0x0c80)  //T=u32, D=0, show proposal status, 1: receive, 2:fail, 3: success
#define CSID_S_REMOTE_AP_TRUSTAP_FWUPGRADE_BAR		_STA_REMOTE(0x0d00)  //T=u32, Show AP FW Upgrade Bar, 3: 33%, 4:66%, 5:100%
#define CSID_S_REMOTE_AP_TRUSTAP_CONFIGRESTORE		_STA_REMOTE(0x0d80)  //T=u32, show AP config restore status, 1: success , 2:fail
#define CSID_S_REMOTE_AP_TRUSTAP_PROPOSALREGET		_STA_REMOTE(0x0e00)  //T=u32, show AP if need reget proposal file, 0: default, 1: need
#define CSID_S_REMOTE_AP_TRUSTAP_STATUS_REBOOT		_STA_REMOTE(0x0e80)  //T=u32, D=0, show AP reboot status, 1: success, 2:fail
#define CSID_S_REMOTE_AP_TRUSTAP_STATUS_RESET		_STA_REMOTE(0x0f00)  //T=u32, D=0, show AP reset status, 1:success, 2:fail
#define CSID_S_REMOTE_AP_TRUSTAP_STATUS_FWUPGRADE	_STA_REMOTE(0x0f80)  //T=u32, D=0, Show AP FW Upgrade status, 1: success, 2:fail
#define CSID_S_REMOTE_AP_TRUSTAP_ALLOW_DENY_STATUS	_STA_REMOTE(0x1000)  //T=u32, D=0, Show AP list set allow or deny if success, 0: def, 1: success, 2:fail
#define CSID_S_REMOTE_AP_TRUSTAP_PROTOCOL		_STA_REMOTE(0x1080)  //T=u32, D=0, Show AP protocol, 0: UDP, 1:TCP 
#define CSID_S_REMOTE_AP_TRUSTAP_DEVICE_NAME		_STA_REMOTE(0x1100)  //T=str, shwo AP default device name
#define CSID_S_REMOTE_AP_TRUSTAP_GROUP_SETSTATUS_AP1	_STA_REMOTE(0x1180)  //T=u8, show AP Load Balacing Set Status, 1: successed, 2:fail
#define CSID_S_REMOTE_AP_TRUSTAP_GROUP_SETSTATUS_AP2	_STA_REMOTE(0x1200)  //T=u8, show AP Load Balacing Set Status, 1: successed, 2:fail

//Known AP Items
#define CSID_S_REMOTE_AP_KNOWNAP_IP                	_STA_REMOTE(0x2000)  //T=ipv4, known AP IP Address
#define CSID_S_REMOTE_AP_KNOWNAP_MAC                	_STA_REMOTE(0x2180)  //T=mac, known AP MAC Address
#define CSID_S_REMOTE_AP_KNOWNAP_SSID_AP1               _STA_REMOTE(0x2200)  //T=str, known AP 2.4G SSID name
#define CSID_S_REMOTE_AP_KNOWNAP_SSID_AP2               _STA_REMOTE(0x2280)  //T=str, known AP 5G SSID name
#define CSID_S_REMOTE_AP_KNOWNAP_STATUS               	_STA_REMOTE(0x2300)  //T=u8, known AP status, 0:offline , 1:online, 2:denined
#define CSID_S_REMOTE_AP_KNOWNAP_VID_LOCATION_AP1       _STA_REMOTE(0x2380)  //T=u16, known AP VID location
#define CSID_S_REMOTE_AP_KNOWNAP_VID_LOCATION_AP2       _STA_REMOTE(0x2400)  //T=u16, known AP VID location
#define CSID_S_REMOTE_AP_KNOWNAP_CHANNEL_AP1            _STA_REMOTE(0x2480)  //T=u16, known AP 2.4G channel
#define CSID_S_REMOTE_AP_KNOWNAP_CHANNEL_AP2            _STA_REMOTE(0x2500)  //T=u16, known AP 5G channel
//Rouge AP Items
#define CSID_S_REMOTE_AP_ROUGEAP_MAC                	_STA_REMOTE(0x3000)  //T=mac, rouge AP MAC Address
#define CSID_S_REMOTE_AP_ROUGEAP_SSID_AP1              	_STA_REMOTE(0x3080)  //T=str, rouge AP 2.4G SSID name
#define CSID_S_REMOTE_AP_ROUGEAP_SSID_AP2              	_STA_REMOTE(0x3100)  //T=str, rouge AP 5G SSID name
#define CSID_S_REMOTE_AP_ROUGEAP_STATUS              	_STA_REMOTE(0x3180)  //T=u8, rouge AP status, 0:offline , 1:online, 2:denined
#define CSID_S_REMOTE_AP_ROUGEAP_VID_LOCATION_AP1       _STA_REMOTE(0x3200)  //T=u16, rouge AP VID location
#define CSID_S_REMOTE_AP_ROUGEAP_VID_LOCATION_AP2       _STA_REMOTE(0x3280)  //T=u16, rouge AP VID location
#define CSID_S_REMOTE_AP_ROUGEAP_CHANNEL_AP1            _STA_REMOTE(0x3300)  //T=u16, known AP 2.4G channel
#define CSID_S_REMOTE_AP_ROUGEAP_CHANNEL_AP2            _STA_REMOTE(0x3380)  //T=u16, known AP 5G channel
#define CSID_S_REMOTE_AP_ROUGEAP_IP                	_STA_REMOTE(0x3400)  //T=ipv4, rouge AP IP Address

/*Proposal Items*/
#define CSID_S_REMOTE_AP_PROPOSAL_ROM_ID		_STA_REMOTE(0x4000)  //T=str, Save rom id for create proposal file
#define CSID_S_REMOTE_AP_PROPOSAL_SAVE_NAME		_STA_REMOTE(0x4100)  //T=str, Save all proposal file name
#define CSID_S_REMOTE_AP_PROPOSAL_SAVE_NAME_APUSING	_STA_REMOTE(0x4200)  //T=str, Show Which AP using this Proposal file
#define CSID_S_REMOTE_AP_PROPOSAL_GET_SAVE_NAME		_STA_REMOTE(0x4300)  //T=str, Save get AP Proposal name
#define CSID_S_REMOTE_AP_PROPOSAL_SAVE_ROM_ID		_STA_REMOTE(0x4400)  //T=str, Save rom id for PROPOSAL_SAVE_NAME 
#define CSID_S_REMOTE_AP_PROPOSAL_EDIT_TIME		_STA_REMOTE(0x4500)  //T=str, record proposal edit time
#define CSID_S_REMOTE_AP_PROPOSAL_CUSTOM_NAME		_STA_REMOTE(0x4700)  //T=str, custom proposal name	
/* SNMP Setting Items */
//Basic	
#define CSID_S_REMOTE_AP_SNMP_GET_COMMUNITY         	_STA_REMOTE(0x5000)  //T=str, AP SNMP Get Community
#define CSID_S_REMOTE_AP_SNMP_SET_COMMUNITY         	_STA_REMOTE(0x5080)  //T=str, AP SNMP Set Community
#define CSID_S_REMOTE_AP_SNMP_VERSION_V1            	_STA_REMOTE(0x5100)  //T=u16, AP SNMP V1 Enable
#define CSID_S_REMOTE_AP_SNMP_VERSION_V2C           	_STA_REMOTE(0x5180)  //T=u16, AP SNMP V2C Enable
#define CSID_S_REMOTE_AP_SNMP_VERSION_V3            	_STA_REMOTE(0x5200)  //T=u16, AP SNMP V3 Enable
//SNMP V3
#define CSID_S_REMOTE_AP_SNMP_V3_USER_ENABLE        	_STA_REMOTE(0x5280)  //T=u16, AP SNMP v3 Enable Multi Account, 1:yes,0:no  
#define CSID_S_REMOTE_AP_SNMP_V3_USER              	_STA_REMOTE(0x5300)  //T=str, AP SNMP v3 User Account
#define CSID_S_REMOTE_AP_SNMP_V3_PASSWORD           	_STA_REMOTE(0x5380)  //T=str, AP SNMP v3 User Password
#define CSID_S_REMOTE_AP_SNMP_V3_AUTHTYPE          	_STA_REMOTE(0x5400)  //T=u16, AP SNMP v3 Auth Type, 0:MD5,1:SHA 
#define CSID_S_REMOTE_AP_SNMP_V3_PRIVACYMODE        	_STA_REMOTE(0x5480)  //T=u16, AP SNMP v3 Privacy Mode, 0:noAuthNoPriv,1:authNoPriv,2:authPriv
#define CSID_S_REMOTE_AP_SNMP_V3_PRIVACY           	_STA_REMOTE(0x5500)  //T=str, AP SNMP v3 DES passphrase 
#define CSID_S_REMOTE_AP_SNMP_V3_RO_RW              	_STA_REMOTE(0x5580)  //T=str, AP SNMP v3 User RO/RW Enable, 0:read only,1:read write

/*AP Mode LAN */
#define CSID_S_REMOTE_AP_LAN_IP				_STA_REMOTE(0x6000)  //T=ipv4, AP mode LAN IP address			
#define CSID_S_REMOTE_AP_LAN_NM				_STA_REMOTE(0x6001)  //T=ipv4, AP mode LAN Subnet Mask
#define CSID_S_REMOTE_AP_CNT_TYPE			_STA_REMOTE(0x6002)  //T=u16, AP mode Connect type, 0:DHCP, 1:Static
#define CSID_S_REMOTE_AP_FIX_LAN_IP			_STA_REMOTE(0x6003)  //T=ipv4, AP mode FIX LAN IP
#define CSID_S_REMOTE_AP_FIX_LAN_NM			_STA_REMOTE(0x6004)  //T=ipv4, AP mode FIX LAN Subnet Mask
#define CSID_S_REMOTE_AP_FIX_GW				_STA_REMOTE(0x6005)  //T=ipv4, AP mode FIX LAN Gateway
#define CSID_S_REMOTE_AP_FIX_PRIDNS			_STA_REMOTE(0x6006)  //T=ipv4, AP mode FIX LAN Primary DNS
#define CSID_S_REMOTE_AP_FIX_SECDNS			_STA_REMOTE(0x6007)  //T=ipv4, AP mode FIX LAN Secondary DNS

/*AP Traffic 2.4G+5G Mbps*/
#define CSID_S_REMOTE_AP_TX_MBPS			_STA_REMOTE(0x6100)  //T=str, AP 2.4G+5G Traffic Tx Mbps
#define CSID_S_REMOTE_AP_RX_MBPS			_STA_REMOTE(0x6101)  //T=str, AP 2.4G+5G Traffic Rx Mbps
#define CSID_S_REMOTE_AP_TXRX_MBPS			_STA_REMOTE(0x6102)  //T=str, AP 2.4G+5G Traffic Tx+Rx Mbps
#define CSID_S_REMOTE_AP_HAVE_5G			_STA_REMOTE(0x6103)  //T=u32, Show AP if has 5G

#define CSID_S_REMOTE_AP_UPSTATUS			_STA_REMOTE(0x6200)  //T=u32, Trigger for Up AP status,0: default, 1: up
#define CSID_S_REMOTE_AP_TRUSTAP_FWUPGRADE_ROMID	_STA_REMOTE(0x6201)  //T=str, Record FW upgrade choice file ROMID
#define CSID_S_REMOTE_AP_TRUSTAP_FWUPGRADE_FNAME	_STA_REMOTE(0x6202)  //T=str, Record FW upgrade choice file name
#define CSID_S_REMOTE_AP_TRUSTAP_REGET_PROPOSAL		_STA_REMOTE(0x6203)  //T=u32, Trigger for apmgt for re-check proposal file
#define CSID_S_REMOTE_AP_TRUSTAP_UISET_FLAG		_STA_REMOTE(0x6204)  //T=u32, indicate if set by UI, 0:default, 1:set by UI
#define CSID_S_REMOTE_AP_TRUSTAP_UI_FUNCTION_SET	_STA_REMOTE(0x6205)  //T=u32, set for UI check if using FW upgrade, reset or reboot, 0:default
#define CSID_S_REMOTE_AP_TRUSTAP_PROPOSALDEL_STATUS	_STA_REMOTE(0x6206)  //T=u32, show delete proposal file if success, 0: def, 1: success
#endif //ifndef _CSID_REMOTE_H


