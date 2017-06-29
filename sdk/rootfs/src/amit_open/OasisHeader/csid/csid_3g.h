/*
 * @file Member ID Definition of Group 3G
 *
 * Moderator: StanleyChu
 * Group ID: 0x001e0000/0x801e0000
 */

#ifndef _CSID_3G_H
#define _CSID_3G_H

#include "csid_gid.h"

#define _CFG_3G(x)		(CSID_GIDC_3G|(x))
#define _STA_3G(x)		(CSID_GIDS_3G|(x))

/* Extra Definitions */

#define CNT_TYPE_3G_ONDEMAND				0x00
#define CNT_TYPE_3G_AUTORECONNECT			0x01
#define CNT_TYPE_3G_MANUAL					0x02

#define CNT_STATUS_3G_DISCONNECT			0x00
#define CNT_STATUS_3G_CONNECTING			0x01
#define CNT_STATUS_3G_CONNECT				0x02
#define CNT_STATUS_3G_DISCONNECTING			0x03
#define CNT_STATUS_3G_WAIT_TRAFFIC			0x05
#define CNT_STATUS_3G_AUTH_FAIL				0x06
#define CNT_STATUS_3G_BACKUP				0x07
#define CNT_STATUS_3G_NOT_INSCHEDULE		0x08

#define HOTPLUG_STATUS_3G_HOLDING     		0x00
#define HOTPLUG_STATUS_3G_ADD     			0x01
#define HOTPLUG_STATUS_3G_REMOVE        	0x02
#define HOTPLUG_STATUS_3G_NONE				0x03
#define HOTPLUG_STATUS_3G_NOT_READY			0x04

#define ONDEMAND_STATUS_DISCONNECT_NO_IP	0x00
#define ONDEMAND_STATUS_CONNECT_NO_IP		0x01
#define ONDEMAND_STATUS_CONNECT_GET_IP		0x02

#define AUTHTYPE_3G_AUTO					0x00
#define AUTHTYPE_3G_PAP						0x01
#define AUTHTYPE_3G_CHAP					0x02

#define KEEP_ALIVE_3G_DISABLE				0x00
#define KEEP_ALIVE_3G_PING					0x01
#define KEEP_ALIVE_3G_LCP					0x02
#define KEEP_ALIVE_3G_PS_STATE				0x03
#define KEEP_ALIVE_3G_PING_MONITOR			0x04
#define KEEP_ALIVE_3G_DNS_QUERY             0x05 
#define KEEP_ALIVE_3G_MORE_PING             0x06

#define ERR_CODE_3G_OK						0x00
#define ERR_CODE_3G_PIN_ERR					0x01
#define ERR_CODE_3G_PW_ERR					0x02
#define ERR_CODE_3G_NO_MODEM				0x03
#define ERR_CODE_3G_SETTING_ERR				0x05
#define ERR_CODE_3G_MODEM_NOT_READY			0x06
#define ERR_CODE_3G_NOT_INSCHEDULE			0x07
#define ERR_CODE_3G_SIM_NOT_INSERTED		0x08
#define ERR_CODE_3G_SIM_BLOCKED				0x09
#define ERR_CODE_3G_PIN_TRY_ERROR			0x0a
#define ERR_CODE_3G_AUTO_APN_ERROR			0x0b //no get imsi
#define ERR_CODE_3G_NO_SIGNAL    			0x0c //no signal
#define ERR_CODE_3G_OP_LOCK                         	0x0d //not allow to use this SIM
#define ERR_CODE_3G_INVALID_SIM_CARD      0x0e

#define SIM_STATUS_PIN_CODE       0x00
#define SIM_STATUS_READY          0x01
#define SIM_STATUS_ABSENT         0x02
#define SIM_STATUS_PIN_INCORRECT  0x03
#define SIM_STATUS_ABSENT_PIN     0x04
#define SIM_STATUS_PUK_CODE       0x05
#define SIM_STATUS_INVALID        0x06

#define AUTOBAK_STATUS_3G_DISCONNECT		0x00
#define AUTOBAK_STATUS_3G_CONNECTING		0x01
#define AUTOBAK_STATUS_3G_CONNECT			0x02
#define AUTOBAK_STATUS_3G_DISCONNECTING		0x03

#define REG_STATUS_IDLE						0x00
#define REG_STATUS_REGISTERED				0x01
#define REG_STATUS_SEARCH					0x02
#define REG_STATUS_ROAMING					0x05

#define PROFILE_MLW							0x02
#define PROFILE_MOBILX						0x04
#define PROFILE_LIGHTSPEED					0x05
#define PROFILE_HARIER						0x06
#define PROFILE_DLINK_TELECO				0x07
#define PROFILE_TELEXCELL					0x08
#define PROFILE_LT_SVC						0x09
#define PROFILE_INDIA_HARIER				0x0a
#define PROFILE_TELEWELL_FINLAND			0x0b
#define PROFILE_TELEWELL_GERMANY			0x0c
#define PROFILE_COMPAT						0x0d
#define PROFILE_RAPIDMAX					0x0e
#define PROFILE_DLINK_CHINA					0x0f
#define PROFILE_IDENTECH					0x10
#define PROFILE_CMOTECH						0x11
#define PROFILE_NETCOMM						0x12
#define PROFILE_WEL							0x13
#define PROFILE_DLINK_DWR					0x14
#define PROFILE_DATACELL					0x15
#define PROFILE_ZONBEST						0x16
#define PROFILE_TECHNOSERV					0x17
#define PROFILE_ZOOM						0x18
#define PROFILE_TRENDNET			0x19

#define CONNECT_TYPE_PPPD					0x00
#define CONNECT_TYPE_DIRECTIP				0x01
#define CONNECT_TYPE_HSO					0x02
#define CONNECT_TYPE_IPHETH					0x03
#define CONNECT_TYPE_RNDIS					0x04

#define PREFER_MODE_AUTO					0x00
#define PREFER_MODE_2G_ONLY					0x01
#define PREFER_MODE_3G_ONLY					0x02
#define PREFER_MODE_2G_PREFER				0x03
#define PREFER_MODE_3G_PREFER				0x04
#define PREFER_MODE_4G_ONLY					0x05
#define PREFER_MODE_4G_PREFER				0x06
#define PREFER_MODE_AUTO_3G_4G				0x07

#define DUALSIMCARD_MODE_DISABLE            0x00
#define DUALSIMCARD_MODE_SIMAFIRST          0x01
#define DUALSIMCARD_MODE_SIMBFIRST          0x02
#define DUALSIMCARD_MODE_SIMAONLY           0x03
#define DUALSIMCARD_MODE_SIMBONLY           0x04

#define POWRE_TYPE_GPIO                     0x00
#define POWRE_TYPE_I2C                      0x01

/* Configuration Items */
#define CSID_C_3G_USER          	_CFG_3G(0x0001)	//T=str, user name
#define CSID_C_3G_PASSWORD      	_CFG_3G(0x0002)	//T=str, user password
#define CSID_C_3G_APN	        	_CFG_3G(0x0003)	//T=str, APN
#define CSID_C_3G_PIN           	_CFG_3G(0x0004)	//T=str, PIN
#define CSID_C_3G_NUMBER        	_CFG_3G(0x0005)	//T=str, Dialed Number
#define CSID_C_3G_IF_IP         	_CFG_3G(0x0006)	//T=ipv4, static interface ip
#define CSID_C_3G_SERVICENAME   	_CFG_3G(0x0007)	//T=str, service name
#define CSID_C_3G_IF_PRIDNS     	_CFG_3G(0x0008)	//T=ipv4, dynamic interface primary DNS
#define CSID_C_3G_IF_SECDNS     	_CFG_3G(0x0009)	//T=ipv4, dynamic interface secondary DNS
#define CSID_C_3G_CNT_TYPE      	_CFG_3G(0x000a)	//T=u16, D=1, connect type, Dial-on-Demand, autoconnect...
#define CSID_C_3G_MTU           	_CFG_3G(0x000b)	//T=u16, D=0, max transmission unit
#define CSID_C_3G_IDLETIME      	_CFG_3G(0x000c)	//T=u16, D=600, max idle time
#define CSID_C_3G_AUTOBAK_ENABLE  	_CFG_3G(0x000d) //T=u16, 3g auto backup enable, 0:disable, 1:enable
#define CSID_C_3G_PING_HOST_IP		_CFG_3G(0x000e)	//T=ipv4, ping host IP for 3g auto backup
#define CSID_C_3G_AUTHTYPE			_CFG_3G(0x000f)	//T=u16, auth type, Auto, PAP, CHAP...
#define CSID_C_3G_KEEP_ALIVE		_CFG_3G(0x0010) //T=u16, 3g keep alive, disable, ping, lcp
#define CSID_C_3G_KA_PING_INTERVAL	_CFG_3G(0x0011)	//T=u16, D=60, ping interval for 3g keep alive
#define CSID_C_3G_KA_PING_IP		_CFG_3G(0x0012)	//T=ipv4, ping IP for 3g keep alive
#define CSID_C_3G_KA_LCP_INTERVAL	_CFG_3G(0x0013)	//T=u16, D=10, lcp interval for 3g keep alive 
#define CSID_C_3G_KA_LCP_TIMES		_CFG_3G(0x0014)	//T=u16, D=3, lcp times for 3g keep alive
#define CSID_C_3G_PREFER_MODE		_CFG_3G(0x0015)	//T=u16, D=0, 0:auto mode, 1:2g only, 2:3g only, 3:2g first, 4:3g first, 5:4g only, 6:4g first, 7: 3g/4g
#define CSID_C_3G_AUTO_APN          _CFG_3G(0x0016) //T=u16, D=1, 0:auto mode 1:Manual
#define CSID_C_3G_NAT_DISABLE		_CFG_3G(0x0017)	//T=u32, 3g wantype NAT disable
#define CSID_C_3G_USSD_RETURN		_CFG_3G(0x0018)	//T=str, ussd return str
#define CSID_C_3G_USSD				_CFG_3G(0x0019)	//T=str, ussd str
#define CSID_C_3G_PREFER_ENABLE		_CFG_3G(0x001a)	//T=u16, D=0, 3G prefer mode, 0:disable, 1:enable
#define CSID_C_3G_ROAMING_ENABLE	_CFG_3G(0x001B)	//T=u32, D=0, roaming switch, 0:disable, 1:enable
#define CSID_C_3G_PDP_TYPE          _CFG_3G(0x001c) //T=u16, D=0, 0:IP, 1=PPP
#define CSID_C_3G_CID               _CFG_3G(0x001D) //T=str, CID, D=1
#define CSID_C_3G_KA_PING_SIZE		_CFG_3G(0x001e)	//T=u16, D=56, ping packet size for 3g keep alive
#define CSID_C_3G_PING_REGULARLY_ENABLE		_CFG_3G(0x001f)	//T=u8, D=0, send ping regularly or not
#define CSID_C_3G_BDG_ETH_PORT		_CFG_3G(0x0020) //T=u16, 3g wan type bridge 2 ethernet port enable, 0:disable, 1:enable
#define CSID_C_3G_PING_INTERVAL     _CFG_3G(0x0021)	//T=u16, D=60, ping interval for 3g
#define CSID_C_3G_PING_IP		    _CFG_3G(0x0022)	//T=ipv4, ping IP for 3g
#define CSID_C_3G_PING_SIZE         _CFG_3G(0x0023)	//T=u16, D=56, ping packet size for 3g
#define CSID_C_3G_KEEP_ALIVE_FAIL_REBOOT    _CFG_3G(0x0024)	//T=u8, D=0, while keep alive fail: reboot or not
#define CSID_C_3G_KA_ERR_TIME       _CFG_3G(0x0025) //T=u16, D=4
#define CSID_C_3G_NOT_USE_DIRECTIP	_CFG_3G(0x0026)	//T=u16, D=0; 0:use direct IP; 1:not use direct IP
#define CSID_C_3G_PING_HOST			_CFG_3G(0x0027)	//T=str, ping domain or IP for 3g auto backup
#define CSID_C_3G_KA_PING_HOST		_CFG_3G(0x0028)	//T=str, ping domain or IP for 3g keep alive
#define CSID_C_3G_ROAMING_LED_ENABLE _CFG_3G(0x0029) //T=u32, D=0, roaming led switch, 0:disable, 1:enable
#define CSID_C_CHECK_RSSI_ENABLE 	_CFG_3G(0x002a) //T=u32, D=0, check rssi status 0:disable, 1:enable
#define CSID_C_3G_BAND_SELECT      	_CFG_3G(0x002b) //T=u32, D=0, 3g band select  0:auto, 1:850, 2:2100, 3:850/2100, 4:850/1900
#define CSID_C_3G_BAND_SELECT_ENABLE  _CFG_3G(0x002c) //T=u32, D=0, 3g band select switch, 0:disable, 1:enable
#define CSID_C_3G_PIN_TRYDEF_ENABLE _CFG_3G(0x002d) //T=u8,  D=0, try default pin code switch 1st:1234 2nd:0000, 0:disable, 1:enable
#define CSID_C_3G_SPN	        	_CFG_3G(0x002e)	//T=str, SPN
#define CSID_C_3G_PLMN_ENABLE      	_CFG_3G(0x002f)	//T=u8,  D=0,Enable/Disable PLMN
#define CSID_C_3G_PROFILE			_CFG_3G(0x0030) //T=u16, for customer
#define CSID_C_3G_PRE_COMMAND		_CFG_3G(0x0031) //T=str, pre_command ,before connecting fill at command
#define CSID_C_3G_CNT_TYPE_UI      	_CFG_3G(0x0032)	//T=u16, D=1, connect type, Dial-on-Demand, autoconnect...
#define CSID_C_3G_MODULE_PWR_GPIO   _CFG_3G(0x0033)	//T=u8,  D=255, Power GPIO for 3G Module
#define CSID_C_3G_IP_TYPE           _CFG_3G(0x0034)	//T=u8,  D=0, IP Type 0:IPv4 1:IPv6 2:IPV4/IPv6

#define CSID_C_3G_USE_RULE_NO		_CFG_3G(0x0040)	//T=u16, D=0,use_rule number
#define CSID_C_3G_SUPPORT_CARD_LIST	_CFG_3G(0x0050)	//T=str, 3g modem support list
#define CSID_C_3G_PIN_DEFAULT        _CFG_3G(0x0060) //T=str, PIN
#define CSID_C_3G_PIN_DEFAULT_RETRY  _CFG_3G(0x0061) //T=u32, D=0,
#define CSID_C_3G_SIERRA_NET_URB_SZ	 _CFG_3G(0x0062) //T=u32, D=0,
#define CSID_C_3G_PING_HOST_MODE	 _CFG_3G(0x0063)	//T=u32, D=0,

/* DNS query based Autoback */
#define CSID_C_3G_AUTOBAK_ETH_WAN_RETRY     _CFG_3G(0x0064) //T=u16, D=2,  Number of retries for Ethernet WAN
#define CSID_C_3G_AUTOBAK_PPP_WAN_RETRY     _CFG_3G(0x0065) //T=u16, D=4,  Number of retries for PPP WAN
#define CSID_C_3G_AUTOBAK_RETRY             _CFG_3G(0x0066) //T=u16, D=30, Wait time for Autobacking
#define CSID_C_3G_AUTOBAK_CHK_INTERVAL      _CFG_3G(0x0067) //T=u16, D=1,  Time for the next check
#define CSID_C_3G_AUTOBAK_BACK_WAN_RETRY    _CFG_3G(0x0068) //T=u16, D=2,  Numver of retries to back to WAN
#define CSID_C_3G_FIXED_AUTOBAK_ENABLE  	_CFG_3G(0x0069) //T=u8, D=0, Enable/Disable 3G - Static WAN Autoback 
#define CSID_C_3G_DHCPC_AUTOBAK_ENABLE  	_CFG_3G(0x006a) //T=u8, D=0, Enable/Disable 3G - DHCP WAN Autoback  
#define CSID_C_3G_PPPOE_AUTOBAK_ENABLE  	_CFG_3G(0x006b) //T=u8, D=0, Enable/Disable 3G - PPPoE WAN Autoback  
#define CSID_C_3G_PPTP_AUTOBAK_ENABLE  	    _CFG_3G(0x006c) //T=u8, D=0, Enable/Disable 3G - PPTP WAN Autoback  
#define CSID_C_3G_L2TP_AUTOBAK_ENABLE  	    _CFG_3G(0x006d) //T=u8, D=0, Enable/Disable 3G - L2TP WAN Autoback  
#define CSID_C_3G_WISP_AUTOBAK_ENABLE  	    _CFG_3G(0x006e) //T=u8, D=0, Enable/Disable 3G - WISP WAN Autoback  
#define CSID_C_3G_AUTOBAK_WANTYPE           _CFG_3G(0x006f) //T=u16, D=0, The WAN Type for Autoback

/* Ping Monitor*/
#define CSID_C_3G_PING_MONITOR_PRIADDR      _CFG_3G(0x0070) //T=str, Ping Monitor primary address
#define CSID_C_3G_PING_MONITOR_SECADDR      _CFG_3G(0x0071) //T=str, Ping Monitor secondary address
#define CSID_C_3G_PING_MONITOR_TIMER        _CFG_3G(0x0072) //T=u32, Ping Monitor Timer
#define CSID_C_3G_PING_MONITOR_ACC_TIMER    _CFG_3G(0x0073) //T=u32, Ping Monitor accelerated Timer
#define CSID_C_3G_PING_MONITOR_ERR_COUNT    _CFG_3G(0x0074) //T=u32, Ping Monitor Fail Count

/* Dual-SIM Setting*/
#define CSID_C_3G_DUAL_SIM_CURR              _CFG_3G(0x0075) //T=u32, switch flag, 1:SIM A, 2:SIM B,
#define CSID_C_3G_DUAL_SIM_3G1              _CFG_3G(0x0076) //T=u32, 3g1 sim select, 1:SIM A, 2:SIM B,
#define CSID_C_3G_DUAL_SIM_3G2              _CFG_3G(0x0077) //T=u32, 3g2 sim select, 1:SIM A, 2:SIM B,

/*autobak/failover active(seamless) mode*/
#define CSID_C_3G_FAILOVER_MODE 	_CFG_3G(0x0080) //T=u32, 0: standby-mode 1:active(seamless)-mode

//for DNS Check for 3G connect
#define CSID_C_3G_DNS_CHECK_ENABLE          _CFG_3G(0x0090) //T=u8, D=0, 0:disbale 1:enable Switch to enable DNS check
#define CSID_C_3G_DNS_CHECK_INTERVAL        _CFG_3G(0x0091) //T=u32, D=30, Interval to DNS check
#define CSID_C_3G_DNS_CHECK_TARGET          _CFG_3G(0x0092) //T=str, target to query DNS
#define CSID_C_3G_DNS_CHECK_IDLE_MODE       _CFG_3G(0x0093) //T=u8, D=0, 0:disable 1:enable switch for enable dns check when in idle_

//for modem monitor 
#define CSID_C_3G_MONITOR_ENABLE            _CFG_3G(0x0096) //T=u32, D=0, 0:disbale 1:enable Switch to enable modem monitor
#define CSID_C_3G_MONITOR_INTERVAL          _CFG_3G(0x0097) //T=u32, D=1, Interval to check modem status and rest it if modem is not ready 

//SW SIM detect
#define CSID_C_3G_SW_SIM_DETECT_SUPPORT     _CFG_3G(0x0098) //T=u32, D=0, 0:disable 1:enable, to support SW SIM detect

//ppp option
#define CSID_C_3G_PPP_OPTION		_CFG_3G(0x0100) //T=u32, ppp_option
#define CSID_C_3G_BAUDRATE			_CFG_3G(0x0101) //T=u32, D=0
#define CSID_C_3G_PEERDNS			_CFG_3G(0x0102) //T=str, D=0
#define CSID_C_3G_PPP_AUTHTYPE		_CFG_3G(0x0103)	//T=str, auth type, Auto, PAP, CHAP...
#define CSID_C_3G_PPP_LCP_INTERVAL	_CFG_3G(0x0104)	//T=str, lcp interval setting
#define CSID_C_3G_PPP_LCP_FAILURE	_CFG_3G(0x0105)	//T=str, lcp failure setting
#define CSID_C_3G_PPP_DEMAND		_CFG_3G(0x0106)	//T=str, demand option
#define CSID_C_3G_PPP_USERNAME		_CFG_3G(0x0107)	//T=str, username

#define CSID_C_3G_REMEBER_PIN		_CFG_3G(0x0200) //T=u16, 0:disable 1:enable
// 3g backup 3g 
#define CSID_C_3G_USE_PORT         	_CFG_3G(0x0800) //T=u16, 1:port1 2:port2
#define CSID_C_3G_S3G_ENABLE        _CFG_3G(0x0810) //T=u16, 0:disable 1:enable
#define CSID_C_3G_S3G_FAILOVER_MODE _CFG_3G(0x0820) //T=u32, 0: standby-mode 1:active-mode
//

#define CSID_C_3G_IMEI_NUMBER	_CFG_3G(0x0900) //T=str, IMEI for MEP Code, 8's word

//--- for card restrict function --//
#define CSID_C_3G_CARD_RESTRICT_ENABLE      _CFG_3G(0x0990) //T=u8, for Card Restrict Function, 0:disable, 1:enable
#define CSID_C_3G_CARD_RESTRICT_VENDOR      _CFG_3G(0x0a00) //T=u16, D=0, card restrict function vendor id list, reserve for 255 sets (0x0a00-0x0aff)
#define CSID_C_3G_CARD_RESTRICT_PRODUCT     _CFG_3G(0x0b00) //T=u16, D=0, card restrict function product list, reserve for 255 sets (0x0b00-0x0bff)
#define CSID_C_3G_MODEM_RESTRICT_ENABLE     _CFG_3G(0x0c00) //T=u8, Restrict Modem mode, 0:disable, 1:enable

#define CSID_C_3G_OP_LOCK_ENABLE	_CFG_3G(0x1000) //T=u16, for Operator Lock enable, 0:disable, 1:enable
#define CSID_C_3G_MCC0				_CFG_3G(0x1001) //T=str, for Mobile Contry Code
#define CSID_C_3G_MCC1				_CFG_3G(0x1002) //T=str, for Mobile Contry Code
#define CSID_C_3G_MCC2				_CFG_3G(0x1003) //T=str, for Mobile Contry Code
#define CSID_C_3G_MCC3				_CFG_3G(0x1004) //T=str, for Mobile Contry Code
#define CSID_C_3G_MCC4				_CFG_3G(0x1005) //T=str, for Mobile Contry Code
#define CSID_C_3G_MCC5				_CFG_3G(0x1006) //T=str, for Mobile Contry Code
#define CSID_C_3G_MCC6				_CFG_3G(0x1007) //T=str, for Mobile Contry Code
#define CSID_C_3G_MCC7				_CFG_3G(0x1008) //T=str, for Mobile Contry Code
#define CSID_C_3G_MNC0				_CFG_3G(0x1101) //T=str, for Mobile Network Code
#define CSID_C_3G_MNC1				_CFG_3G(0x1102) //T=str, for Mobile Network Code
#define CSID_C_3G_MNC2				_CFG_3G(0x1103) //T=str, for Mobile Network Code
#define CSID_C_3G_MNC3				_CFG_3G(0x1104) //T=str, for Mobile Network Code
#define CSID_C_3G_MNC4				_CFG_3G(0x1105) //T=str, for Mobile Network Code
#define CSID_C_3G_MNC5				_CFG_3G(0x1106) //T=str, for Mobile Network Code
#define CSID_C_3G_MNC6				_CFG_3G(0x1107) //T=str, for Mobile Network Code
#define CSID_C_3G_MNC7				_CFG_3G(0x1108) //T=str, for Mobile Network Code

//---- FW Upgrade Lock
#define CSID_C_3G_FW_UPGRADE_LOCK_ENABLE	_CFG_3G(0x1009) //T=u16, for FW Upgrade Lock enable, 0:disable, 1:enable

//--- Network scan config CSID
#define CSID_C_3G_NETWORK_SELECT_MODE           _CFG_3G(0x1109)	//T=u8, D=0, 3g network select mode 0:auto, 1:manual 
#define CSID_C_3G_NETWORK_PREFER_PROVIDER       _CFG_3G(0x110a)	//T=str, network prefer provider
#define CSID_C_3G_NETWORK_PREFER_TYPE	        _CFG_3G(0x110b)	//T=u8, network prefer type, 2:3G, 7:LTE
#define CSID_C_3G_NETWORK_SETAT_FORMAT	        _CFG_3G(0x110c)	//T=u8, set COPS format 0:long format alphanumeric 1:short format 2:numeric

//--- Multi 3G config CSID
#define CSID_C_3G_MULTIWAN_USER				 	_CFG_3G(0x2010)	//T=str, user name
#define CSID_C_3G_MULTIWAN_PASSWORD				_CFG_3G(0x2020)	//T=str, user password
#define CSID_C_3G_MULTIWAN_APN			    	_CFG_3G(0x2030)	//T=str, APN
#define CSID_C_3G_MULTIWAN_PIN				 	_CFG_3G(0x2040)	//T=str, PIN
#define CSID_C_3G_MULTIWAN_NUMBER				_CFG_3G(0x2050)	//T=str, Dialed Number
#define CSID_C_3G_MULTIWAN_IF_IP				_CFG_3G(0x2060)	//T=ipv4, static interface ip
#define CSID_C_3G_MULTIWAN_SERVICENAME		  	_CFG_3G(0x2070)	//T=str, service name
#define CSID_C_3G_MULTIWAN_IF_PRIDNS			_CFG_3G(0x2080)	//T=ipv4, dynamic interface primary DNS
#define CSID_C_3G_MULTIWAN_IF_SECDNS			_CFG_3G(0x2090)	//T=ipv4, dynamic interface secondary DNS
#define CSID_C_3G_MULTIWAN_CNT_TYPE				_CFG_3G(0x20a0)	//T=u16, D=1, connect type, Dial-on-Demand, autoconnect...
#define CSID_C_3G_MULTIWAN_MTU					_CFG_3G(0x20b0)	//T=u16, D=0, max transmission unit
#define CSID_C_3G_MULTIWAN_IDLETIME				_CFG_3G(0x20c0)	//T=u16, D=600, max idle time
#define CSID_C_3G_MULTIWAN_AUTOBAK_ENABLE		_CFG_3G(0x20d0) //T=u16, 3g auto backup enable, 0:disable, 1:enable
#define CSID_C_3G_MULTIWAN_PING_HOST_IP			_CFG_3G(0x20e0)	//T=ipv4, ping host IP for 3g auto backup
#define CSID_C_3G_MULTIWAN_AUTHTYPE				_CFG_3G(0x20f0)	//T=u16, auth type, Auto, PAP, CHAP...
#define CSID_C_3G_MULTIWAN_KEEP_ALIVE			_CFG_3G(0x2100) //T=u16, 3g keep alive, disable, ping, lcp, dns_query
#define CSID_C_3G_MULTIWAN_KA_PING_INTERVAL		_CFG_3G(0x2110)	//T=u16, D=60, ping interval for 3g keep alive
#define CSID_C_3G_MULTIWAN_KA_PING_IP			_CFG_3G(0x2120)	//T=ipv4, ping IP for 3g keep alive
#define CSID_C_3G_MULTIWAN_KA_LCP_INTERVAL		_CFG_3G(0x2130)	//T=u16, D=10, lcp interval for 3g keep alive 
#define CSID_C_3G_MULTIWAN_KA_LCP_TIMES			_CFG_3G(0x2140)	//T=u16, D=3, lcp times for 3g keep alive
#define CSID_C_3G_MULTIWAN_PREFER_MODE			_CFG_3G(0x2150)	//T=u16, D=0, 0:auto mode, 1:2g only, 2:3g only, 3:2g first, 4:3g first
#define CSID_C_3G_MULTIWAN_AUTO_APN				_CFG_3G(0x2160) //T=u16, D=1, 0:auto mode 1:Manual 2:profile
#define CSID_C_3G_MULTIWAN_NAT_DISABLE			_CFG_3G(0x2170)	//T=u32, 3g wantype NAT disable
#define CSID_C_3G_MULTIWAN_USSD_RETURN			_CFG_3G(0x2180)	//T=str, ussd return str
#define CSID_C_3G_MULTIWAN_USSD					_CFG_3G(0x2190)	//T=str, ussd str
#define CSID_C_3G_MULTIWAN_PREFER_ENABLE		_CFG_3G(0x21a0)	//T=u16, D=0, 3G prefer mode, 0:disable, 1:enable
#define CSID_C_3G_MULTIWAN_ROAMING_ENABLE		_CFG_3G(0x21b0)	//T=u32, D=0, roaming switch, 0:disable, 1:enable
#define CSID_C_3G_MULTIWAN_USE_PORT				_CFG_3G(0x21c0) //T=u32, 3g connect port
#define CSID_C_3G_MULTIWAN_CNT_TYPE_UI          _CFG_3G(0x21d0) //T=u16, D=1, connect type, Dial-on-Demand, autoconnect...
#define CSID_C_3G_MULTIWAN_SPN                  _CFG_3G(0x21e0)	//T=str, SPN
#define CSID_C_3G_MULTIWAN_PLMN_ENABLE      	_CFG_3G(0x21f0)	//T=u8,  D=0,Enable/Disable PLMN
#define CSID_C_3G_MULTIWAN_BDG_ETH_PORT			_CFG_3G(0x2200) //T=u16, 3g wan type bridge 2 ethernet port enable, 0:disable, 1:enable
#define CSID_C_3G_MULTIWAN_PROFILE				_CFG_3G(0x2210) //T=u16, for customer
#define CSID_C_3G_MULTIWAN_USE_RULE_NO			_CFG_3G(0x2220)	//T=u16, D=0,use_rule number
#define CSID_C_3G_MULTIWAN_SUPPORT_CARD_LIST	_CFG_3G(0x2230)	//T=str, 3g modem support list

#define CSID_C_3G_MULTIWAN_NETWORK_SELECT_MODE      _CFG_3G(0x2270)	//T=u8, D=0, 3g network select mode 0:auto, 1:manual 
#define CSID_C_3G_MULTIWAN_NETWORK_PREFER_PROVIDER  _CFG_3G(0x2280)	//T=str, network prefer provider
#define CSID_C_3G_MULTIWAN_NETWORK_PREFER_TYPE      _CFG_3G(0x2290)	//T=u8, network prefer type, 2:3G, 7:LTE
#define CSID_C_3G_MULTIWAN_NETWORK_MCC_MNC          _CFG_3G(0x22a0)	//T=str, network mcc_mnc
#define CSID_C_3G_MULTIWAN_IP_TYPE              _CFG_3G(0x22b0)     //T=u8, D=0, IP Type 0:IPv4 1:IPv6 2:IPV4/IPv6

/* for multi-3G mapping */
#define CSID_C_3G_MAPPING_LEVEL						_CFG_3G(0x2240)	//T=u16, level mapping for usb_index
#define CSID_C_3G_MAPPING_PORT						_CFG_3G(0x2250)	//T=u16, port mapping for usb_index

#define CSID_C_3G_MULTIWAN_KA_PING_HOST	        _CFG_3G(0x2260)	//T=str, ping domain or IP for 3g keep alive
#define CSID_C_3G_MULTIWAN_KA_PING_FAIL_TIMES	_CFG_3G(0x22c0)	//T=u16, D=3, retry fail times for 3g keep alive

/* For multi-3G Function config */
#define CSID_C_3G_MULTIWAN_PPP_ONLY             _CFG_3G(0x22d0) //T=u8, use PPP connected by force
#define CSID_C_3G_MULTIWAN_PRE_COMMAND          _CFG_3G(0x22e0) //T=str, pre_command ,before connecting fill at command
#define CSID_C_3G_MULTIWAN_PWR_GPIO             _CFG_3G(0x22f0)	//T=u8,  D=255, Power GPIO for 3G Module

#define CSID_C_3G_MULTIWAN_OP_LOCK_ENABLE		_CFG_3G(0x2300) //T=u16, for Operator Lock enable, 0:disable, 1:enable
#define CSID_C_3G_MULTIWAN_MCC0					_CFG_3G(0x2310) //T=str, for Mobile Contry Code
#define CSID_C_3G_MULTIWAN_MCC1					_CFG_3G(0x2320) //T=str, for Mobile Contry Code
#define CSID_C_3G_MULTIWAN_MCC2					_CFG_3G(0x2330) //T=str, for Mobile Contry Code
#define CSID_C_3G_MULTIWAN_MCC3					_CFG_3G(0x2340) //T=str, for Mobile Contry Code
#define CSID_C_3G_MULTIWAN_MCC4					_CFG_3G(0x2350) //T=str, for Mobile Contry Code
#define CSID_C_3G_MULTIWAN_MCC5					_CFG_3G(0x2360) //T=str, for Mobile Contry Code
#define CSID_C_3G_MULTIWAN_MCC6					_CFG_3G(0x2370) //T=str, for Mobile Contry Code
#define CSID_C_3G_MULTIWAN_MCC7					_CFG_3G(0x2380) //T=str, for Mobile Contry Code
#define CSID_C_3G_MULTIWAN_MNC0					_CFG_3G(0x2410) //T=str, for Mobile Network Code
#define CSID_C_3G_MULTIWAN_MNC1					_CFG_3G(0x2420) //T=str, for Mobile Network Code
#define CSID_C_3G_MULTIWAN_MNC2					_CFG_3G(0x2430) //T=str, for Mobile Network Code
#define CSID_C_3G_MULTIWAN_MNC3					_CFG_3G(0x2440) //T=str, for Mobile Network Code
#define CSID_C_3G_MULTIWAN_MNC4					_CFG_3G(0x2450) //T=str, for Mobile Network Code
#define CSID_C_3G_MULTIWAN_MNC5					_CFG_3G(0x2460) //T=str, for Mobile Network Code
#define CSID_C_3G_MULTIWAN_MNC6					_CFG_3G(0x2470) //T=str, for Mobile Network Code
#define CSID_C_3G_MULTIWAN_MNC7					_CFG_3G(0x2480) //T=str, for Mobile Network Code

#define CSID_C_3G_PCTL_SWITCH					_CFG_3G(0x2490) //T=u32, pctl switch, 0:disable/1:enable
#define CSID_C_3G_MULTIWAN_PCTL_SWITCH     		_CFG_3G(0x2491) //T=u32, pctl switch, 0:disable/1:enable
#define CSID_C_3G_MULTIWAN_APN_SELECT_MODE     			_CFG_3G(0x2493) //T=u32, D=0, 3G apn selest mode, 0:manual , 1:select enable
#define CSID_C_3G_SMS_SWITCH					_CFG_3G(0x24a0) //T=u32, sms switch , 0:disable/1:enable
#define CSID_C_3G_MULTIWAN_SMS_SWITCH     		_CFG_3G(0x24a1) //T=u32, sms switch , 0:disable/1:enable

#define CSID_C_3G_SAVE_PIN						_CFG_3G(0x24b0) //T=u32, save pin when pass , 0:disable/1:enable
#define CSID_C_3G_IMSI							_CFG_3G(0x24c0) //T=str, save imsi value, use for multiwan
#define CSID_C_3G_RDIDX1						_CFG_3G(0x24d0) //T=u32, save read idx from 0-29
#define CSID_C_3G_RDIDX2						_CFG_3G(0x24e0) //T=u32, save read idx from 30-59
#define CSID_C_3G_RDIDX3						_CFG_3G(0x24e1) //T=u32, save read idx from 60-89
#define CSID_C_3G_RDIDX4						_CFG_3G(0x24e2) //T=u32, save read idx from 90-119
#define CSID_C_3G_PROXY_ENABLE					_CFG_3G(0x24f0) //T=u32, D=0, Proxy Enable, 0:disable, 1:enable
#define CSID_C_3G_PROXY_HOSTNAME				_CFG_3G(0x24f1) //T=str, Proxy Address
#define CSID_C_3G_SMSC							_CFG_3G(0x2500) //T=str, SMSC obtained by AT+CSCA 
#define CSID_C_3G_MULTIWAN_SMSC         _CFG_3G(0x2501) //T=str, SMSC obtained by AT+CSCA
#define CSID_C_3G_IMEI							_CFG_3G(0x2510) //T=str, module IMEI
#define CSID_C_3G_PROXY_PORT					_CFG_3G(0x2521) //T=u16, Proxy Port (1-65535)
#define CSID_C_3G_BRIDGE_ENABLE					_CFG_3G(0x2531) //T=u32, D=0 ,0: 3G bridge disable ,1: 3G bridge enable
#define CSID_C_3G_CHECK_PREPAID_ENABLE          _CFG_3G(0x2532) //T=u32, D=0 ,0: Check SIM card prepaid value disable ,1: Check SIM card prepaid value enable
#define CSID_C_3G_SCHEDULE_REBOOT_DONE          _CFG_3G(0x2533) //T=u32, D=0 , 3G schedule reboot done 
#define CSID_C_3G_BRIDGE_SCHEDULE_REBOOT        _CFG_3G(0x2534) //T=u32, D=0 , 3G bridge mode schedule reboot
#define CSID_C_3G_BRIDGE_REBOOT_TIME            _CFG_3G(0x2535) //T=u32, D=0 , 3G bridge mode schedule reboot time
#define CSID_C_3G_SCHEDULE_REBOOT_ENABLE        _CFG_3G(0x2536) //T=u8, D=0 , reboot device by 3G schedule

#define CSID_C_3G_SMARTPHONE_DISABLE            _CFG_3G(0x2537) //T=u32, D=0 , disable modem swith for smart phone, 0: enable, 1: disable

#define CSID_C_3G_MODULE_SCHEDULE_REBOOT_ENABLE _CFG_3G(0x2538) //T=u8, D=1 , reboot 3G module enable
#define CSID_C_3G_MODULE_SCHEDULE_REBOOT_DONE   _CFG_3G(0x2539) //T=u32, D=0 , 3G module schedule reboot done 

#define CSID_C_3G_ATCOM_LOG_SWITCH     			_CFG_3G(0x253a) //T=u32, atcom log switch, 0:disable/1:enable

#define CSID_C_3G_DORMANCY_SWITCH     			_CFG_3G(0x253b) //T=u8, D=0, dormancy switch, 0:disable/1:enable
#define CSID_C_3G_DORMANCY_WAITING_TIME     	_CFG_3G(0x253c) //T=u32, D=10, go dormancy mode if no traffic for 10 seconds

#define CSID_C_3G_SMS_REMOTE_MG_PROFILE	                _CFG_3G(0x253f) //T=u8, D=0 , SMS remote management profile
#define CSID_C_3G_SMS_REMOTE_MG_ENABLE                  _CFG_3G(0x2540) //T=u8, D=0 , SMS remote management enable, 0: disable, 1: enable
#define CSID_C_3G_DELETE_REMOTE_MG_SMS_ENABLE           _CFG_3G(0x2541) //T=u8, D=0 , Delete remote management SMS enable, 0: disable, 1: enable
#define CSID_C_3G_SMS_REMOTE_MG_SECURITY_KEY            _CFG_3G(0x2542) //T=str, D=0 , SMS remote management security key
#define CSID_C_3G_SMS_REMOTE_MG_CMD_STATUS_ENABLE       _CFG_3G(0x2543) //T=u8, D=0 , SMS remote management command status enable, 0: disable, 1: enable
#define CSID_C_3G_SMS_REMOTE_MG_CMD_CONNECT_ENABLE      _CFG_3G(0x2544) //T=u8, D=0 , SMS remote management command connect enable, 0: disable, 1: enable
#define CSID_C_3G_SMS_REMOTE_MG_CMD_DISCONNECT_ENABLE   _CFG_3G(0x2545) //T=u8, D=0 , SMS remote management command disconnect enable, 0: disable, 1: enable
#define CSID_C_3G_SMS_REMOTE_MG_CMD_RECONNECT_ENABLE    _CFG_3G(0x2546) //T=u8, D=0 , SMS remote management command reconnect enable, 0: disable, 1: enable
#define CSID_C_3G_SMS_REMOTE_MG_CMD_REBOOT_ENABLE       _CFG_3G(0x2547) //T=u8, D=0 , SMS remote management command reboot enable, 0: disable, 1: enable
#define CSID_C_3G_SMS_REMOTE_MG_NOTIFY_WAN_UP_ENABLE            _CFG_3G(0x2548) //T=u8, D=0 , SMS remote management notification wan up enable, 0: disable, 1: enable
#define CSID_C_3G_SMS_REMOTE_MG_NOTIFY_WAN_DOWN_ENABLE          _CFG_3G(0x2549) //T=u8, D=0 , SMS remote management notification wan down enable, 0: disable, 1: enable
#define CSID_C_3G_SMS_REMOTE_MG_NOTIFY_SEC_WAN_UP_ENABLE        _CFG_3G(0x254a) //T=u8, D=0 , SMS remote management notification secondary wan up enable, 0: disable, 1: enable
#define CSID_C_3G_SMS_REMOTE_MG_NOTIFY_SEC_WAN_DOWN_ENABLE      _CFG_3G(0x254b) //T=u8, D=0 , SMS remote management notification secondary wan down enable, 0: disable, 1: enable
#define CSID_C_3G_SMS_REMOTE_MG_ACCESS_CONTROL_ENABLE           _CFG_3G(0x254c) //T=u8, D=0 , SMS remote management access control enable, 0: disable, 1: enable

#define CSID_C_3G_MODULE_SCHEDULE_REBOOT_FLAG                   _CFG_3G(0x254d) //T=u8, D=1 , 3G module weekly reboot auto select week day flag, 0: disable, 1: enable
#define CSID_C_3G_MODULE_SCHEDULE_DEV_REBOOT                    _CFG_3G(0x254e) //T=u8, D=0 , 0: disable, 1: enable, reboot the device instead of resetting the module

#define CSID_C_3G_SMS_REMOTE_MG_ACCESS_CONTROL_LIST             _CFG_3G(0x2550) //T=str, SMS remote management access control phone list
#define CSID_C_3G_SMS_REMOTE_MG_ACCESS_CONTROL_MG_ENABLE        _CFG_3G(0x2560) //T=u8, D=0, SMS remote management access control management enable, 0: disable, 1: enable
#define CSID_C_3G_SMS_REMOTE_MG_ACCESS_CONTROL_NOTIFY_ENABLE    _CFG_3G(0x2570) //T=u8, D=0, SMS remote management access control notification enable, 0: disable, 1: enable
#define CSID_C_3G_SMS_REMOTE_MG_CMD_PROCESS_IDX1          _CFG_3G(0x2580) //T=u32, D=0 , SMS remote management command process index from 0 ~ 29
#define CSID_C_3G_SMS_REMOTE_MG_CMD_PROCESS_IDX2          _CFG_3G(0x2581) //T=u32, D=0 , SMS remote management command process index from 30 ~ 59
#define CSID_C_3G_SMS_REMOTE_MG_CMD_PROCESS_IDX3          _CFG_3G(0x2582) //T=u32, D=0 , SMS remote management command process index from 60 ~ 89
#define CSID_C_3G_SMS_REMOTE_MG_CMD_PROCESS_IDX4          _CFG_3G(0x2583) //T=u32, D=0 , SMS remote management command process index from 90 ~ 119
#define CSID_C_3G_SMS_REMOTE_MG_ANY_PHONE_ENABLE          _CFG_3G(0x2590) //T=u8, D=0, SMS remote management any phone enable, 0: disable, 1: enable
#define CSID_C_3G_DELETE_ALL_SMS_ENABLE          _CFG_3G(0x2591) //T=u8, D=0, SMS remote management delete all sms enable, 0: disable, 1: enable

#define CSID_C_3G_MULTIWAN_RDIDX1						_CFG_3G(0x25a0) //T=u32, save read idx from 0-29
#define CSID_C_3G_MULTIWAN_RDIDX2						_CFG_3G(0x25b0) //T=u32, save read idx from 30-59
#define CSID_C_3G_MULTIWAN_RDIDX3						_CFG_3G(0x25c0) //T=u32, save read idx from 60-89
#define CSID_C_3G_MULTIWAN_RDIDX4						_CFG_3G(0x25d0) //T=u32, save read idx from 90-119

#define CSID_C_3G_QMI_CURRENT_CLIENT_ID          _CFG_3G(0x2600) //T=u16, D=0, Current QMI connection client id
#define CSID_C_3G_FAILOVER_CONNECTED             _CFG_3G(0x2601) //T=u8, D=0, Failover connected for VOIP UI
#define CSID_C_3G_RESET_MODEM_WHEN_ROAMING       _CFG_3G(0x2602) //T=u8, D=0, o:disable, 1: enable, reset module for reg status should not be roaming

#define CSID_C_3G_MULTIWAN_NOT_REG_RESTART_INTERVAL		_CFG_3G(0x2610) //T=u32, D=3600, not registered and the time interval (default one hour) to restart the modem

/* Dual SIM Items */
#define CSID_C_3G_SIM_UI_CHOICE                  _CFG_3G(0x2700) //T=u8, D=0 , Choice UI, 0: default, 1: always SIMA, 2: always SIMB
#define CSID_C_3G_APN_SIMA	        	         _CFG_3G(0x2701) //T=str, SIMA APN
#define CSID_C_3G_APN_SIMB	        	         _CFG_3G(0x2702) //T=str, SIMB APN
#define CSID_C_3G_PIN_SIMA           	         _CFG_3G(0x2703) //T=str, SIMA PIN
#define CSID_C_3G_PIN_SIMB           	         _CFG_3G(0x2704) //T=str, SIMB PIN
#define CSID_C_3G_NUMBER_SIMA        	         _CFG_3G(0x2705) //T=str, SIMA Dialed Number
#define CSID_C_3G_NUMBER_SIMB        	         _CFG_3G(0x2706) //T=str, SIMB Dialed Number
#define CSID_C_3G_USER_SIMA          	         _CFG_3G(0x2707) //T=str, SIMA user name
#define CSID_C_3G_USER_SIMB          	         _CFG_3G(0x2708) //T=str, SIMB user name
#define CSID_C_3G_PASSWORD_SIMA      	         _CFG_3G(0x2709) //T=str, SIMA user password
#define CSID_C_3G_PASSWORD_SIMB      	         _CFG_3G(0x270a) //T=str, SIMA user password
#define CSID_C_3G_AUTHTYPE_SIMA			         _CFG_3G(0x270b) //T=u16, SIMA auth type, Auto, PAP, CHAP...
#define CSID_C_3G_AUTHTYPE_SIMB			         _CFG_3G(0x270c) //T=u16, SIMB auth type, Auto, PAP, CHAP...
#define CSID_C_3G_IF_PRIDNS_SIMA               	 _CFG_3G(0x270d) //T=ipv4, SIMA dynamic interface primary DNS
#define CSID_C_3G_IF_PRIDNS_SIMB               	 _CFG_3G(0x270e) //T=ipv4, SIMB dynamic interface primary DNS
#define CSID_C_3G_IF_SECDNS_SIMA     	         _CFG_3G(0x270f) //T=ipv4, SIMA dynamic interface secondary DNS
#define CSID_C_3G_IF_SECDNS_SIMB     	         _CFG_3G(0x2710) //T=ipv4, SIMB dynamic interface secondary DNS
#define CSID_C_3G_CNT_TYPE_UI_SIMA      	     _CFG_3G(0x2711) //T=u16, D=1, SIMA connect type, Dial-on-Demand, autoconnect...
#define CSID_C_3G_CNT_TYPE_UI_SIMB      	     _CFG_3G(0x2712) //T=u16, D=1, SIMB connect type, Dial-on-Demand, autoconnect...
#define CSID_C_3G_IDLETIME_SIMA      	         _CFG_3G(0x2713) //T=u16, D=600, SIMA max idle time
#define CSID_C_3G_IDLETIME_SIMB      	         _CFG_3G(0x2714) //T=u16, D=600, SIMB max idle time
#define CSID_C_3G_FAILOVER_TIME					 _CFG_3G(0x2715) //T=u16, D=120, Dual-SIM failover time
#define CSID_C_3G_RESET_FLAG					 _CFG_3G(0x2716) //T=u16, D=0, Use IMEI to be Wireless key
#define CSID_C_3G_RM_DRIVER_BEFORE_PWR_OFF		 _CFG_3G(0x2717) //T=u16, D=0, 0:disable, 1:enable, rm driver before power off the module

/* Modem Restart setting for multiwan */
#define CSID_C_3G_MODEM_RESTART_ENABLE          _CFG_3G(0x2730) //T=u8, D=0, check modem restart enable, 0:disable, 1:enable  
#define CSID_C_3G_MODEM_RESTART_TIMES           _CFG_3G(0x2740) //T=u32, D=3, check modem fail times
#define CSID_C_3G_MODEM_FAIL_TIMES              _CFG_3G(0x2750) //T=u32, D=3, check Tele2 modem fail times for init check

/* 3G Power Control Setting for multiwan*/
#define CSID_C_3G_MULTIWAN_ACTIVE_HIGH          _CFG_3G(0x2760) //T=u8, D=0, GPIO High/low for 3G Power
#define CSID_C_3G_MODULE_PWR_TYPE               _CFG_3G(0x2770) //T=u8, D=0, 3G power type 0:gpio 1:I2C...
#define CSID_C_3G_SIM_SLOT_TYPE                 _CFG_3G(0x2780) //T=u8, D=0, 3G SIM slot power type 0:gpio 1:I2C...
#define CSID_C_3G_PWR_SIM_SLOT                  _CFG_3G(0x2790) //T=u8, D=0, Power GPIO for SIM slot

/* WNC MAL API */
#define CSID_C_3G_MAL_LOG_SWITCH     			_CFG_3G(0x2800) //T=u32, MAL log switch, 0:disable/1:enable

/* BAND SELECT */
#define CSID_C_3G_MULTIWAN_BAND_SELECT_ENABLE   _CFG_3G(0x2810) //T=u32, D=0, band select switch, 0:disable, 1:enable
#define CSID_C_3G_MULTIWAN_BAND_SELECT          _CFG_3G(0x2820) //T=u32, D=0, 3g band select(Mhz)  0:auto, 1:850, 2:2100, 3:850/2100, 4:850/1900, 5:1900
#define CSID_C_3G_MULTIWAN_BAND_SELECT_4G       _CFG_3G(0x2830) //T=u32, D=0, 4g band select(Mhz)  0:auto, 1:2600, 2: 700
#define CSID_C_3G_MULTIWAN_2G_DISABLE            _CFG_3G(0x2840) //T=u32, D=0, auto mode enable 2g, 0:enable 2g(GSM/WCDMA/LTE), 1:disable 2g(WCDMA/LTE)


/* USSD profile */
#define CSID_C_3G_MULTIWAN_USSD_PROFILE_NAME        _CFG_3G(0x2900) //T=str, USSD profile name, ex: 3g1_1 = 00, 3g1_17 = 10, 3g2_32 =3f
#define CSID_C_3G_MULTIWAN_USSD_PROFILE_CMD         _CFG_3G(0x2b00) //T=str, USSD profile cmd, ex: 3g1_1 = 00, 3g1_17 = 10, 3g2_32 =3f
#define CSID_C_3G_MULTIWAN_USSD_PROFILE_COMMENTS    _CFG_3G(0x2d00) //T=str, USSD profile comments, ex: 3g1_1 = 00, 3g1_17 = 10, 3g2_32 =3f


//SMS alert_rule
#define CSID_C_3G_MULTIWAN_SMSALERT_FROMPHONENUM    _CFG_3G(0x3000) //T=str, SMS alert_rule fromphonenum, ex: 3g1_1 = 00, 3g1_17 = 10, 3g2_32 =3f
#define CSID_C_3G_MULTIWAN_SMSALERT_APPROACH        _CFG_3G(0x3200) //T=u32, SMS alert_rule approach, ex: 3g1_1 = 00, 3g1_17 = 10, 3g2_32 =3f, autoforward = 0, email = 1, syslog = 2
#define CSID_C_3G_MULTIWAN_SMSALERT_DESTINATION     _CFG_3G(0x3400) //T=str, SMS alert_rule destination, ex: 3g1_1 = 00, 3g1_17 = 10, 3g2_32 =3f
#define CSID_C_3G_MULTIWAN_SMSALERT_ENABLE     		_CFG_3G(0x3600) //T=u8, SMS alert_rule enable, ex: 3g1_1 = 00, 3g1_17 = 10, 3g2_32 =3f, enable = 1, disable = 0

//SMS remote management phone number range
#define CSID_C_3G_SMS_REMOTE_MG_ACCESS_CTL_RANGE_START		_CFG_3G(0x3700) //T=str, SMS MG access control range start
#define CSID_C_3G_SMS_REMOTE_MG_ACCESS_CTL_RANGE_END		_CFG_3G(0x3710) //T=str, SMS MG access control range end
#define CSID_C_3G_SMS_REMOTE_MG_ACCESS_CTL_RANGE_MG_ENABLE	_CFG_3G(0x3720)	//T=u8, D=0, SMS MG access control mg enable
#define CSID_C_3G_SMS_REMOTE_MG_ACCESS_CTL_RANGE_NOTIFY_ENABLE	_CFG_3G(0x3730)	//T=u8, D=0, SMS MG access control notify enable

/* Multi 3G Dual SIM config CSID */
#define CSID_C_3G_MULTIWAN_SIMCARD_MODE         _CFG_3G(0x4000) //T=u8, D=0, 1:SIMA First 2:SIMB First 3:SIMA only 4:SIMB only
#define CSID_C_3G_MULTIWAN_APN1_SIMA            _CFG_3G(0x4010) //T=str, SIMA APN1
#define CSID_C_3G_MULTIWAN_APN2_SIMA            _CFG_3G(0x4020) //T=str, SIMA APN2 
#define CSID_C_3G_MULTIWAN_APN3_SIMA            _CFG_3G(0x4030) //T=str, SIMA APN3
#define CSID_C_3G_MULTIWAN_APN4_SIMA            _CFG_3G(0x4040) //T=str, SIMA APN4
#define CSID_C_3G_MULTIWAN_APN1_SIMB            _CFG_3G(0x4050) //T=str, SIMB APN1
#define CSID_C_3G_MULTIWAN_APN2_SIMB            _CFG_3G(0x4060) //T=str, SIMB APN2
#define CSID_C_3G_MULTIWAN_APN3_SIMB            _CFG_3G(0x4070) //T=str, SIMB APN3
#define CSID_C_3G_MULTIWAN_APN4_SIMB            _CFG_3G(0x4080) //T=str, SIMB APN4
#define CSID_C_3G_MULTIWAN_PIN_SIMA             _CFG_3G(0x4090) //T=str, SIMA PIN
#define CSID_C_3G_MULTIWAN_PIN_SIMB             _CFG_3G(0x40a0) //T=str, SIMB PIN
#define CSID_C_3G_MULTIWAN_NUMBER_SIMA          _CFG_3G(0x40b0) //T=str, SIMA Dialed Number
#define CSID_C_3G_MULTIWAN_NUMBER_SIMB          _CFG_3G(0x40c0) //T=str, SIMB Dialed Number
#define CSID_C_3G_MULTIWAN_USER_SIMA            _CFG_3G(0x40d0) //T=str, SIMA user name
#define CSID_C_3G_MULTIWAN_USER_SIMB            _CFG_3G(0x40e0) //T=str, SIMB user name
#define CSID_C_3G_MULTIWAN_PASSWORD_SIMA        _CFG_3G(0x40f0) //T=str, SIMA user password
#define CSID_C_3G_MULTIWAN_PASSWORD_SIMB        _CFG_3G(0x4110) //T=str, SIMA user password
#define CSID_C_3G_MULTIWAN_AUTHTYPE_SIMA        _CFG_3G(0x4120) //T=u16, SIMA auth type, Auto, PAP, CHAP...
#define CSID_C_3G_MULTIWAN_AUTHTYPE_SIMB        _CFG_3G(0x4130) //T=u16, SIMB auth type, Auto, PAP, CHAP...
#define CSID_C_3G_MULTIWAN_IF_PRIDNS_SIMA       _CFG_3G(0x4140) //T=ipv4, SIMA dynamic interface primary DNS
#define CSID_C_3G_MULTIWAN_IF_PRIDNS_SIMB       _CFG_3G(0x4150) //T=ipv4, SIMB dynamic interface primary DNS
#define CSID_C_3G_MULTIWAN_IF_SECDNS_SIMA       _CFG_3G(0x4160) //T=ipv4, SIMA dynamic interface secondary DNS
#define CSID_C_3G_MULTIWAN_IF_SECDNS_SIMB       _CFG_3G(0x4170) //T=ipv4, SIMB dynamic interface secondary DNS
#define CSID_C_3G_MULTIWAN_AUTO_APN_SIMA        _CFG_3G(0x4180) //T=u16, D=1, 0:auto mode 1:Manual 2:profile
#define CSID_C_3G_MULTIWAN_AUTO_APN_SIMB        _CFG_3G(0x4190) //T=u16, D=1, 0:auto mode 1:Manual 2:profile
#define CSID_C_3G_MULTIWAN_ROAMING_ENABLE_SIMA  _CFG_3G(0x41a0) //T=u32, D=0, SIMA roaming switch, 0:disable, 1:enable
#define CSID_C_3G_MULTIWAN_ROAMING_ENABLE_SIMB  _CFG_3G(0x41b0) //T=u32, D=0, SIMB roaming switch, 0:disable, 1:enable

/* dualsim failback */
#define CSID_C_3G_MULTIWAN_DUALSIM_FAILBACK_ENABLE        _CFG_3G(0x41e0) //T=u32, D=0, dualsim failback enable
#define CSID_C_3G_MULTIWAN_DUALSIM_FAILBACK_TIME          _CFG_3G(0x41f0) //T=u32, D=0, dualsim failback time

#define CSID_C_3G_MAPPING_DEVICE				_CFG_3G(0x4200)	//T=u16, mapping device for module or usb_3G, module=1, usb=0

/*use external Preferred Antenna*/
#define CSID_C_3G_PREFERRED_ANTENNA             _CFG_3G(0x4210)	//T=u8, D=0, 0:interanl 1:external

/* Force disable MAL API */
#define CSID_C_3G_MULTIWAN_FORCE_MALDO          _CFG_3G(0x4220)	//T=u8, D=0, 0:disable 1:enable for special modem

/* For check PDP Context status maxretry */
#define CSID_C_3G_MULTIWAN_CHKSTA_FAIL_TIMES    _CFG_3G(0x4230)	//T=u8, D=3, check PDP Context status max fail times 

/* SMS Remote Management */
#define CSID_C_3G_MULTIWAN_SMS_REMOTE_MG_CMD_PROCESS_IDX1          _CFG_3G(0x4300) //T=u32, D=0 , SMS remote management command process index from 0 ~ 29 , address 00:3G1 SIMA, 0f:3G16 SIMA, 10:3G2 SIMB
#define CSID_C_3G_MULTIWAN_SMS_REMOTE_MG_CMD_PROCESS_IDX2          _CFG_3G(0x4320) //T=u32, D=0 , SMS remote management command process index from 30 ~ 59 , address 20:3G1 SIMA, 2f:3G16 SIMA, 30:3G2 SIMB
#define CSID_C_3G_MULTIWAN_SMS_REMOTE_MG_CMD_PROCESS_IDX3          _CFG_3G(0x4340) //T=u32, D=0 , SMS remote management command process index from 60 ~ 89 , address 40:3G1 SIMA, 4f:3G16 SIMA, 50:3G2 SIMB
#define CSID_C_3G_MULTIWAN_SMS_REMOTE_MG_CMD_PROCESS_IDX4          _CFG_3G(0x4360) //T=u32, D=0 , SMS remote management command process index from 90 ~ 119 , address 60:3G1 SIMA, 6f:3G16 SIMA, 70:3G2 SIMB


/* For Card Permit Function  */
#define CSID_C_3G_MULTIWAN_CARD_PERMIT_ENABLE     _CFG_3G(0x5000) //T=u8, D=0, card permit function enable, 1: enable
#define CSID_C_3G_MULTIWAN_CARD_PERMIT_VENDOR     _CFG_3G(0x5100) //T=u8, D=0, card permit function vendor id list, reserve 255 sets (0x5100-0x51ff)
#define CSID_C_3G_MULTIWAN_CARD_PERMIT_PRODUCT    _CFG_3G(0x5200) //T=u8, D=0, card permit function product list, reserve 255 sets (0x5200-0x52ff)

/* Config Profile */
#define CSID_C_3G_PROFILE_ENABLE_SIMA           _CFG_3G(0x6000) //T=u8, D=0, 1:enable
#define CSID_C_3G_PROFILE_ENABLE_SIMB           _CFG_3G(0x6100) //T=u8, D=0, 1:enable
#define CSID_C_3G_PROFILE_APN_SIMA              _CFG_3G(0x6200) //T=str, SIMA APN
#define CSID_C_3G_PROFILE_APN_SIMB              _CFG_3G(0x6300) //T=str, SIMB APN
#define CSID_C_3G_PROFILE_NUMBER_SIMA           _CFG_3G(0x6400) //T=str, SIMA Dialed Number
#define CSID_C_3G_PROFILE_NUMBER_SIMB           _CFG_3G(0x6500) //T=str, SIMB Dialed Number
#define CSID_C_3G_PROFILE_USER_SIMA             _CFG_3G(0x6600) //T=str, SIMA user name
#define CSID_C_3G_PROFILE_USER_SIMB             _CFG_3G(0x6700) //T=str, SIMB user name
#define CSID_C_3G_PROFILE_PASSWORD_SIMA         _CFG_3G(0x6800) //T=str, SIMA user password
#define CSID_C_3G_PROFILE_PASSWORD_SIMB         _CFG_3G(0x6900) //T=str, SIMA user password
#define CSID_C_3G_PROFILE_AUTHTYPE_SIMA         _CFG_3G(0x6a00) //T=u16, SIMA auth type, Auto, PAP, CHAP...
#define CSID_C_3G_PROFILE_AUTHTYPE_SIMB         _CFG_3G(0x6b00) //T=u16, SIMB auth type, Auto, PAP, CHAP...
#define CSID_C_3G_PROFILE_NAME_SIMA             _CFG_3G(0x6c00) //T=str, SIMA Profile name
#define CSID_C_3G_PROFILE_NAME_SIMB             _CFG_3G(0x6d00) //T=str, SIMB Profile name

#define CSID_C_3G_MANAGEMENT_EVENT_LIST             _CFG_3G(0x6e00) //T=u32, sms remote managemet list
#define CSID_C_3G_MANAGEMENT_EVENT_ENABLE_LIST      _CFG_3G(0x6e20) //T=u32, sms remote managemet enable list
#define CSID_C_3G_NOTIFY_EVENT_LIST             		_CFG_3G(0x6e40) //T=u32, sms remote notify list
#define CSID_C_3G_NOTIFY_EVENT_ENABLE_LIST          _CFG_3G(0x6e60) //T=u32, sms remote notify enable list
#define CSID_C_3G_MANAGEMENT_EVENT_FUNCTION_ENABLE  _CFG_3G(0x6e80) //T=u32, sms remote managemet function enable/disable 
#define CSID_C_3G_NOTIFY_EVENT_FUNCTION_ENABLE      _CFG_3G(0x6e81) //T=u32, sms remote notify function enable/disable 
#define CSID_C_3G_MG_CURRENT_INTERFACE    	_CFG_3G(0x6e82) //T=u32, sms event management current 3G interface
#define CSID_C_3G_DELETE_ALL_RECIVE_SMS    	_CFG_3G(0x6e83) //T=u32, sms event management current 3G interface 
#define CSID_C_3G_CONFIRMED_SMS_ENABLE   	_CFG_3G(0x6e84) //T=u32, sms event management current 3G interface 
#define CSID_C_3G_SMS_REMOTE_MG_ACCESS_CONTROL_LIST_ENABLE 	_CFG_3G(0x6e90) //T=u32, sms event management list enable

#define CSID_C_3G_BRIDGE_FIXMAC      				_CFG_3G(0x6f00) //T=str, BRIDGE_FIXMAC
#define CSID_C_3G_BRIDGE_FIXIP      				_CFG_3G(0x6f01) //T=ipv4, BRIDGE_FIXIP

#define CSID_C_3G_SMS_REMOTE_MG_NOTIFY_DATA_USAGE_ENABLE	_CFG_3G(0x6f20) //T=u32, DATA_USAGE_ENABLE
#define CSID_C_3G_SMS_REMOTE_MG_NOTIFY_DATA_USAGE_80			_CFG_3G(0x6f40) //T=u32, DATA_USAGE_80%
#define CSID_C_3G_SMS_REMOTE_MG_NOTIFY_DATA_USAGE_100			_CFG_3G(0x6f60) //T=u32, DATA_USAGE_100%

#define CSID_C_3G_MULTIWAN_BAND_CONFIG_2G_SIMA			_CFG_3G(0x6f80) //T=u32, 3G1~3G16 use bit operator to set band
#define CSID_C_3G_MULTIWAN_BAND_CONFIG_2G_SIMB			_CFG_3G(0x6f90) //T=u32, 3G1~3G16 use bit operator to set band
#define CSID_C_3G_MULTIWAN_BAND_CONFIG_3G_FDD_SIMA			_CFG_3G(0x6fa0) //T=u32, 3G1~3G16 use bit operator to set band
#define CSID_C_3G_MULTIWAN_BAND_CONFIG_3G_FDD_SIMB			_CFG_3G(0x6fb0) //T=u32, 3G1~3G16 use bit operator to set band
#define CSID_C_3G_MULTIWAN_BAND_CONFIG_3G_TDD_SIMA			_CFG_3G(0x6fc0) //T=u32, 3G1~3G16 use bit operator to set band
#define CSID_C_3G_MULTIWAN_BAND_CONFIG_3G_TDD_SIMB			_CFG_3G(0x6fd0) //T=u32, 3G1~3G16 use bit operator to set band
#define CSID_C_3G_MULTIWAN_BAND_CONFIG_4G_FDD_SIMA			_CFG_3G(0x6fe0) //T=u32, 3G1~3G16 use bit operator to set band
#define CSID_C_3G_MULTIWAN_BAND_CONFIG_4G_FDD_SIMB			_CFG_3G(0x6ff0) //T=u32, 3G1~3G16 use bit operator to set band
#define CSID_C_3G_MULTIWAN_BAND_CONFIG_4G_TDD_SIMA			_CFG_3G(0x7000) //T=u32, 3G1~3G16 use bit operator to set band
#define CSID_C_3G_MULTIWAN_BAND_CONFIG_4G_TDD_SIMB			_CFG_3G(0x7010) //T=u32, 3G1~3G16 use bit operator to set band

#define CSID_C_3G_MULTIWAN_BAND_SELECTION_MODE_SIMA			_CFG_3G(0x7020) //T=u32, Auto or Manual
#define CSID_C_3G_MULTIWAN_BAND_SELECTION_MODE_SIMB			_CFG_3G(0x7030) //T=u32, Auto or Manual
#define CSID_C_3G_MULTIWAN_CTA_BAND_SELECT_ENABLE_SIMA			_CFG_3G(0x7040) //T=u32, CTA band select enable
#define CSID_C_3G_MULTIWAN_CTA_BAND_SELECT_ENABLE_SIMB			_CFG_3G(0x7050) //T=u32, CTA band select enable
#define CSID_C_3G_MULTIWAN_BAND_SINGLE_OR_MULTI_SELECT		_CFG_3G(0x7060) //T=u32, band singel or multi select base module

#define CSID_C_3G_MULTIWAN_PREFER_MODE_SIMA		_CFG_3G(0x7070) //T=u32, band singel or multi select base module
#define CSID_C_3G_MULTIWAN_PREFER_MODE_SIMB		_CFG_3G(0x7080) //T=u32, band singel or multi select base module
#define CSID_C_3G_MULTIWAN_NETWORK_SELECT_MODE_SIMA		_CFG_3G(0x7090) //T=u32, band singel or multi select base module
#define CSID_C_3G_MULTIWAN_NETWORK_SELECT_MODE_SIMB		_CFG_3G(0x70a0) //T=u32, band singel or multi select base module
#define CSID_C_3G_MULTIWAN_NETWORK_PREFER_TYPE_SIMA      _CFG_3G(0x70b0)	//T=u8, network prefer type, 2:3G, 7:LTE
#define CSID_C_3G_MULTIWAN_NETWORK_PREFER_TYPE_SIMB      _CFG_3G(0x70c0)	//T=u8, network prefer type, 2:3G, 7:LTE
#define CSID_C_3G_MULTIWAN_NETWORK_MCC_MNC_SIMA          _CFG_3G(0x70d0)	//T=str, network mcc_mnc
#define CSID_C_3G_MULTIWAN_NETWORK_MCC_MNC_SIMB          _CFG_3G(0x70e0)	//T=str, network mcc_mnc
#define CSID_C_3G_MULTIWAN_NETWORK_PREFER_PROVIDER_SIMA  _CFG_3G(0x70f0)	//T=str, network prefer provider
#define CSID_C_3G_MULTIWAN_NETWORK_PREFER_PROVIDER_SIMB  _CFG_3G(0x7100)	//T=str, network prefer provider
//temp CSID
#define CSID_C_3G_MULTIWAN_BAND_CONFIG_23G_1TO32			_CFG_3G(0x7150) //T=u32, 3G1~3G16 use bit operator to set band
#define CSID_C_3G_MULTIWAN_BAND_CONFIG_23G_33TO64			_CFG_3G(0x7170) //T=u32, 3G1~3G16 use bit operator to set band
#define CSID_C_3G_MULTIWAN_BAND_CONFIG_4G_FDD			_CFG_3G(0x7190) //T=u32, 3G1*2~3G16*2 use bit operator to set band
#define CSID_C_3G_MULTIWAN_BAND_CONFIG_4G_TDD			_CFG_3G(0x71b0) //T=u32, 3G1*2~3G16*2 use bit operator to set band
#define CSID_C_3G_MULTIWAN_CTA_BAND_SELECT_ENABLE			_CFG_3G(0x71d0) //T=u32, CTA band select enable
#define CSID_C_3G_MULTIWAN_BAND_TABLE_ENABLE			_CFG_3G(0x71e0) //T=u32, CTA band select enable

/* Config Profile (cont.)*/
#define CSID_C_3G_PROFILE_MCC_SIMA				_CFG_3G(0x7200) //T=str, SIMA MCC for APN profile
#define CSID_C_3G_PROFILE_MCC_SIMB				_CFG_3G(0x7300) //T=str, SIMB MCC for APN profile
#define CSID_C_3G_PROFILE_MNC_SIMA				_CFG_3G(0x7400) //T=str, SIMA MNC for APN profile
#define CSID_C_3G_PROFILE_MNC_SIMB				_CFG_3G(0x7500) //T=str, SIMB MNC for APN profile
#define CSID_C_3G_PROFILE_PRIORITY_SIMA			_CFG_3G(0x7600) //T=u16, SIMA profile priority for APN profile
#define CSID_C_3G_PROFILE_PRIORITY_SIMB			_CFG_3G(0x7700) //T=u16, SIMB profile priority for APN profile
#define CSID_C_3G_PROFILE_IP_TYPE_SIMA			_CFG_3G(0x7800) //T=u8, SIMA IP type(IPv4/v6) for APN profile
#define CSID_C_3G_PROFILE_IP_TYPE_SIMB			_CFG_3G(0x7900) //T=u8, SIMB IP type(IPv4/v6) for APN profile

/* IP mode */
#define CSID_C_3G_MULTIWAN_IP_MODE_SIMA			_CFG_3G(0x7a00) //T=u8, IP mode(dynamic/static IP address) for SIMA
#define CSID_C_3G_MULTIWAN_IP_MODE_SIMB			_CFG_3G(0x7a10) //T=u8, IP mode(dynamic/static IP address) for SIMB
#define CSID_C_3G_MULTIWAN_IF_FIXED_IP_SIMA		_CFG_3G(0x7a20) //T=ipv4, static IP address for SIMA
#define CSID_C_3G_MULTIWAN_IF_FIXED_IP_SIMB		_CFG_3G(0x7a30) //T=ipv4, static IP address for SIMB
#define CSID_C_3G_MULTIWAN_IF_FIXED_NM_SIMA		_CFG_3G(0x7a40) //T=ipv4, static netmask for SIMA
#define CSID_C_3G_MULTIWAN_IF_FIXED_NM_SIMB		_CFG_3G(0x7a50) //T=ipv4, static netmask for SIMB
#define CSID_C_3G_MULTIWAN_IF_FIXED_GW_SIMA		_CFG_3G(0x7a60) //T=ipv4, static gateway IP address for SIMA
#define CSID_C_3G_MULTIWAN_IF_FIXED_GW_SIMB		_CFG_3G(0x7a70) //T=ipv4, static gateway IP address for SIMB

#define CSID_C_3G_MULTIWAN_BAND_P300A_RESET_FLAG						_CFG_3G(0x7a80) //T=u32, P300A set band fro reset to default
//Just remind next start from 7a90

/* Status Items */
#define CSID_S_3G_CNT_TIME					_STA_3G(0x0100)	//T=u32, connect time
#define CSID_S_3G_MULTIWAN_CNT_TIME        	_STA_3G(0x0101)	//T=u32, connect time
#define CSID_S_3G_IF_IP						_STA_3G(0x0200)	//T=ipv4, dynamic interface ip
#define CSID_S_3G_MULTIWAN_IF_IP           	_STA_3G(0x0201)	//T=ipv4, dynamic interface ip
#define CSID_S_3G_IF_NM						_STA_3G(0x0300)	//T=ipv4, dynamic interface netmask
#define CSID_S_3G_MULTIWAN_IF_NM           	_STA_3G(0x0301)	//T=ipv4, dynamic interface netmask
#define CSID_S_3G_IF_GW						_STA_3G(0x0400)	//T=ipv4, dynamic interface gateway
#define CSID_S_3G_MULTIWAN_IF_GW           	_STA_3G(0x0401)	//T=ipv4, dynamic interface gateway
#define CSID_S_3G_IF_PRIDNS					_STA_3G(0x0500)	//T=ipv4, dynamic interface primary DNS
#define CSID_S_3G_MULTIWAN_IF_PRIDNS       	_STA_3G(0x0501)	//T=ipv4, dynamic interface primary DNS
#define CSID_S_3G_IF_SECDNS					_STA_3G(0x0600)	//T=ipv4, dynamic interface secondary DNS
#define CSID_S_3G_MULTIWAN_IF_SECDNS       	_STA_3G(0x0601)	//T=ipv4, dynamic interface secondary DNS
#define CSID_S_3G_CNT_STATUS				_STA_3G(0x0700)	//T=u16, D=0, connect status
#define CSID_S_3G_MULTIWAN_CNT_STATUS      	_STA_3G(0x0701)	//T=u16, D=0, connect status
#define CSID_S_3G_IPV6_CNT_STATUS           _STA_3G(0x0710)	//T=u16, D=0, connect status
#define CSID_S_3G_MULTIWAN_IPV6_CNT_STATUS  _STA_3G(0x0711)	//T=u16, D=0, connect status
#define CSID_S_3G_CNT_SUSPEND				_STA_3G(0x0780)	//T=u8, D=0, suspend connection action if necessary
#define CSID_S_3G_MULTIWAN_CNT_SUSPEND     	_STA_3G(0x0781)	//T=u8, D=0, suspend connection action if necessary
#define CSID_S_3G_PKTIN_NUM					_STA_3G(0x0800)	//T=u32, number of packet input
#define CSID_S_3G_MULTIWAN_PKTIN_NUM       	_STA_3G(0x0801)	//T=u32, number of packet input
#define CSID_S_3G_PKTOUT_NUM				_STA_3G(0x0900)	//T=u32, number of packet output
#define CSID_S_3G_MULTIWAN_PKTOUT_NUM      	_STA_3G(0x0901)	//T=u32, number of packet output
#define CSID_S_3G_HOTPLUG_STATUS			_STA_3G(0x0a00) //T=u16, 3g hotplug status for script so it don't care for UI
#define CSID_S_3G_MULTIWAN_HOTPLUG_STATUS  	_STA_3G(0x0a01) //T=u16, 3g hotplug status for script so it don't care for UI
#define CSID_S_3G_ONDEMAND_STATUS			_STA_3G(0x0b00) //T=u16, demend wan type status for script so it don't care for UI
#define CSID_S_3G_MULTIWAN_ONDEMAND_STATUS 	_STA_3G(0x0b01) //T=u16, demend wan type status for script so it don't care for UI
#define CSID_S_3G_ALTERED					_STA_3G(0x0c00)	//T=u8, ui altered
#define CSID_S_3G_MULTIWAN_ALTERED         	_STA_3G(0x0c01)	//T=u8, ui altered
#define CSID_S_3G_CARD_INFO					_STA_3G(0x0d00)	//T=str, 3G card info
#define CSID_S_3G_MULTIWAN_CARD_INFO		_STA_3G(0x0d01)	//T=str, 3G card info
#define CSID_S_3G_SIGNAL_STRENGTH			_STA_3G(0x0e00)	//T=u16, signal strength
#define CSID_S_3G_MULTIWAN_SIGNAL_STRENGTH	_STA_3G(0x0e01)	//T=u16, signal strength
#define CSID_S_3G_SIGNAL_QUALITY			_STA_3G(0x0f00)	//T=u16, signal quality
#define CSID_S_3G_MULTIWAN_SIGNAL_QUALITY	_STA_3G(0x0f01)	//T=u16, signal quality
#define CSID_S_3G_NETWORK_NAME				_STA_3G(0x1000)	//T=str, network name
#define CSID_S_3G_MULTIWAN_NETWORK_NAME		_STA_3G(0x1001)	//T=str, network name
#define CSID_S_3G_IMSI_NETWORK_NAME    		_STA_3G(0x1010) //T=str, network name, obtained by IMSI
#define CSID_S_3G_ERR_CODE					_STA_3G(0x1100) //T=u16, connect fail error code
#define CSID_S_3G_MULTIWAN_ERR_CODE			_STA_3G(0x1101) //T=u16, connect fail error code
#define CSID_S_3G_AUTOBAK_STATUS			_STA_3G(0x1200)	//T=u16, 3g failover status
#define CSID_S_3G_MULTIWAN_AUTOBAK_STATUS	_STA_3G(0x1201)	//T=u16, 3g failover status
#define CSID_S_3G_SERVICE_TYPE				_STA_3G(0x1300)	//T=u16, 3g service type, 0:2G,1:none ,2:3G,3:3.5G,4~6:3.75G,7:LTE
#define CSID_S_3G_MULTIWAN_SERVICE_TYPE		_STA_3G(0x1301)	//T=u16, 3g service type, 0:2G, 2:3G
#define CSID_S_3G_CARD_VENDORID				_STA_3G(0x1400)	//T=u16, 3G card vendor id
#define CSID_S_3G_MULTIWAN_CARD_VENDORID	_STA_3G(0x1401)	//T=u16, 3G card vendor id
#define CSID_S_3G_CARD_PRODUCTID			_STA_3G(0x1500)	//T=u16, 3G card product id
#define CSID_S_3G_MULTIWAN_CARD_PRODUCTID	_STA_3G(0x1501)	//T=u16, 3G card product id
#define CSID_S_3G_OP_LOCK_STATUS			_STA_3G(0x1600) //T=u16, for Operator Lock status, 0:unlock, 1:lock
#define CSID_S_3G_MULTIWAN_OP_LOCK_STATUS	_STA_3G(0x1601) //T=u16, for Operator Lock status, 0:unlock, 1:lock
#define CSID_S_3G_CARD_RESTRICT_STATUS      _STA_3G(0x1610) //T=u8, for card restrict status, 0:disable, 1:enable
#define CSID_S_3G_HOTPLUG_VERSION			_STA_3G(0x1700) //T=str, hotplug version
#define CSID_S_3G_MULTIWAN_HOTPLUG_VERSION	_STA_3G(0x1701) //T=str, hotplug version
#define CSID_S_3G_GET_STATUS				_STA_3G(0x1800) //T=u8, get_3g_satatus, 1:running
#define CSID_S_3G_MULTIWAN_GET_STATUS		_STA_3G(0x1801) //T=u8, get_3g_satatus, 1:running
#define CSID_S_3G_REG_STATUS				_STA_3G(0x1900) //T=u8, register status for 3G network
#define CSID_S_3G_MULTIWAN_REG_STATUS		_STA_3G(0x1901) //T=u8, register status for 3G network
#define CSID_S_3G_VOICE_REG_STATUS			_STA_3G(0x1a00) //T=u32, voice register status for 3G network
#define CSID_S_3G_MULTIWAN_VOICE_REG_STATUS	_STA_3G(0x1a01) //T=u32, voice register status for 3G network
#define CSID_S_3G_USSD_STATUS				_STA_3G(0x2300) //T=u8, D=0,get_ussd_satatus, 1:readable 0:unreadable
#define CSID_S_3G_MULTIWAN_USSD_STATUS		_STA_3G(0x2301) //T=u8, D=0,get_ussd_satatus, 1:readable 0:unreadable
#define CSID_S_3G_USSD_CMD                  _STA_3G(0x2310) //T=str, ussd send command str 
#define CSID_S_3G_MULTIWAN_USSD_CMD         _STA_3G(0x2311) //T=str, ussd send command str 
#define CSID_S_3G_USSD_RESULT               _STA_3G(0x2320) //T=str, the result after send USSD cmd
#define CSID_S_3G_MULTIWAN_USSD_RESULT      _STA_3G(0x2321) //T=str, the result after send USSD cmd
#define CSID_S_3G_USSD_START                _STA_3G(0x2330) //T=u8, ussd send cmd start
#define CSID_S_3G_MULTIWAN_USSD_START       _STA_3G(0x2331) //T=u8, ussd send cmd start
#define CSID_S_3G_USSD_RESULT_PHONE_NUM     _STA_3G(0x2340) //T=str, the result after send USSD cmd for DWR116-play
#define CSID_S_3G_MULTIWAN_USSD_RESULT_PHONE_NUM _STA_3G(0x2341) //T=str, the result after send USSD cmd for DWR116-play
#define CSID_S_3G_USSD_RESULT_REMAINING_VALUE     _STA_3G(0x2350) //T=str, the result after send USSD cmd for DWR116-play
#define CSID_S_3G_MULTIWAN_USSD_RESULT_REMAINING_VALUE _STA_3G(0x2351) //T=str, the result after send USSD cmd for DWR116-play
#define CSID_S_3G_PDP_CONTEXT_STATUS			_STA_3G(0x2360) //T=u16, 0:deeactivated, 1:activated, PDP context status
#define CSID_S_3G_MULTIWAN_PDP_CONTEXT_STATUS	_STA_3G(0x2361) //T=u16, 0:deeactivated, 1:activated, PDP context status

#define CSID_S_3G_USB_PORT					_STA_3G(0x2b00) //T=u32, 1:PCIE 2:USB
#define CSID_S_3G_MULTIWAN_USB_PORT         _STA_3G(0x2b01) //T=u32, 1:PCIE 2:USB
#define CSID_S_3G_CMD_STATUS				_STA_3G(0x2c00) //T=u32, multiwan cmd status
#define CSID_S_3G_MULTIWAN_CMD_STATUS       _STA_3G(0x2c01) //T=u32, multiwan cmd status
 
/* for 3G Failover ping check */
#define CSID_S_3G_WAN_IP					_STA_3G(0x2000) //T=ipv4, wan ip
#define CSID_S_3G_MULTIWAN_WAN_IP			_STA_3G(0x2001) //T=ipv4, wan ip
#define CSID_S_3G_WAN_GW					_STA_3G(0x2100) //T=ipv4, wan gateway
#define CSID_S_3G_MULTIWAN_WAN_GW			_STA_3G(0x2101) //T=ipv4, wan gateway
#define CSID_S_3G_WAN_IF_NAME				_STA_3G(0x2200) //T=str, the interface name of wan, ppp0, eth1, ixp1
#define CSID_S_3G_MULTIWAN_WAN_IF_NAME		_STA_3G(0x2201) //T=str, the interface name of wan, ppp0, eth1, ixp1
#define CSID_S_3G_IPSEC_ALTERED				_STA_3G(0x2b10) //T=u8, wan connect alter for vpn
#define CSID_S_3G_MULTIWAN_IPSEC_ALTERED    _STA_3G(0x2b11) //T=u8, wan connect alter for vpn

/* 3G status */
#define CSID_S_3G_PREFER_SERVICE_TYPE       _STA_3G(0x2b20) //T=str, AUTO/GSM only/WCDMA only
#define CSID_S_3G_MULTIWAN_PREFER_SERVICE_TYPE	_STA_3G(0x2b21) //T=str, AUTO/GSM only/WCDMA only/LTE only 
#define CSID_S_3G_PS_STATUS                 _STA_3G(0x2b30) //T=u8, PS status 0:detach 1:attach for 3G network
#define CSID_S_3G_MULTIWAN_PS_STATUS		_STA_3G(0x2b31) //T=u8, PS status 0:detach 1:attach for 3G network 
#define CSID_S_3G_MCC						_STA_3G(0x2b40) //T=u16, 3g MCC 
#define CSID_S_3G_MULTIWAN_MCC              _STA_3G(0x2b41) //T=u16, 3g MCC
#define CSID_S_3G_MNC						_STA_3G(0x2b50) //T=u16, 3g MNC 
#define CSID_S_3G_MULTIWAN_MNC              _STA_3G(0x2b51) //T=u16, 3g MNC
#define CSID_S_3G_LAC						_STA_3G(0x2b60) //T=u32, 3g LAC
#define CSID_S_3G_MULTIWAN_LAC              _STA_3G(0x2b61) //T=u32, 3g LAC
#define CSID_S_3G_SMSC						_STA_3G(0x2b70) //T=str, SMSC obtained by AT+CSCA 
#define CSID_S_3G_MULTIWAN_SMSC             _STA_3G(0x2b71) //T=str, SMSC obtained by AT+CSCA
#define CSID_S_3G_IMEI                      _STA_3G(0x2b80) //T=str, 3g IMEI
#define CSID_S_3G_MULTIWAN_IMEI             _STA_3G(0x2b81) //T=str, 3g IMEI
#define CSID_S_3G_CARD_VERSION              _STA_3G(0x2b90) //T=str, 3g modem version
#define CSID_S_3G_MULTIWAN_CARD_VERSION     _STA_3G(0x2b91) //T=str, 3g modem version 
#define CSID_S_3G_CELLID_DEC                _STA_3G(0x2ba0) //T=u32, Cell ID Decimal Type
#define CSID_S_3G_MULTIWAN_CELLID_DEC       _STA_3G(0x2ba1) //T=u32, Cell ID Decimal Type
#define CSID_S_3G_TAC                       _STA_3G(0x2bb0) //T=u32, TAC for LTE
#define CSID_S_3G_MULTIWAN_TAC              _STA_3G(0x2bb1) //T=u32, TAC for LTE
#define CSID_S_3G_CARD_HW_VER								_STA_3G(0x2bc0)	//T=str, 3G card HW version
#define CSID_S_3G_MULTIWAN_CARD_HW_VER			_STA_3G(0x2bc0)	//T=str, 3G card HW version
#define CSID_S_3G_BER                         _STA_3G(0x2bd0) //T=u32, Bit error rate
#define CSID_S_3G_MULTIWAN_BER              _STA_3G(0x2bd0) //T=u32, Bit error rate
#define CSID_S_3G_SINR                         _STA_3G(0x2be0) //T=u32, signal-to-interference-plus-noise ratio
#define CSID_S_3G_MULTIWAN_SINR              _STA_3G(0x2be0) //T=u32, signal-to-interference-plus-noise ratio
#define CSID_S_3G_MAC_ADDR              _STA_3G(0x2bf0) //T=str, modem mac address
#define CSID_S_3G_MULTIWAN_MAC_ADDR              _STA_3G(0x2bf1) //T=str, modem mac address

//---SMS function added by York 090916
#define CSID_S_3G_SMS_RECEIVED				_STA_3G(0x2400) //T=u8, D=0 ,numbers of reveived messages
#define CSID_S_3G_MULTIWAN_SMS_RECEIVED     _STA_3G(0x2401) //T=u8, D=0 ,numbers of reveived messages
#define CSID_S_3G_SMS_NEW					_STA_3G(0x2500) //T=u8, D=0 ,numbers of new messages
#define CSID_S_3G_MULTIWAN_SMS_NEW          _STA_3G(0x2501) //T=u8, D=0 ,numbers of new messages
#define CSID_S_3G_SMS_CAPACITY				_STA_3G(0x2600) //T=u8, D=0 ,numbers of SIM card messages capacity
#define CSID_S_3G_MULTIWAN_SMS_CAPACITY     _STA_3G(0x2601) //T=u8, D=0 ,numbers of SIM card messages capacity
#define CSID_S_3G_SMS_TOPHONE				_STA_3G(0x2700) //T=str, phone number of sending
#define CSID_S_3G_MULTIWAN_SMS_TOPHONE      _STA_3G(0x2701) //T=str, phone number of sending
#define CSID_S_3G_SMS_TEXT					_STA_3G(0x2800) //T=str, text content of sending
#define CSID_S_3G_MULTIWAN_SMS_TEXT         _STA_3G(0x2801) //T=str, text content of sending
#define CSID_S_3G_LONG_SMS_TEXT             _STA_3G(0x2810) //T=str, for long sms text content
#define CSID_S_3G_MULTIWAN_LONG_SMS_TEXT    _STA_3G(0x2811) //T=str, for long sms text content
#define CSID_S_3G_SMS_ERR_MSG				_STA_3G(0x2900) //T=u8,	send error messages 
#define CSID_S_3G_MULTIWAN_SMS_ERR_MSG      _STA_3G(0x2901) //T=u8,	send error messages 
#define CSID_S_3G_SMS_STATUS				_STA_3G(0x2a00) //T=u8, D=0 ,sms status , 0:querying, 1:query finished
#define CSID_S_3G_MULTIWAN_SMS_STATUS       _STA_3G(0x2a01) //T=u8, D=0 ,sms status , 0:querying, 1:query finished
//#define CSID_S_3G_SMS_QUERY_FLAG			_STA_3G(0x2c00) //T=u8, sms query flay, let commander know 3G connect or not
//#define CSID_S_3G_MULTIWAN_SMS_QUERY_FLAG   _STA_3G(0x2c01) //T=u8, sms query flay, let commander know 3G connect or not
//#define CSID_S_3G_SMS_FULL_ALERT			_STA_3G(0x2c02) //T=u8, D=0 ,sms full alert. 1:if sms full 
//#define CSID_S_3G_SMS_RETURN_STATUS			_STA_3G(0x2c03) //T=u8, D=0 ,sms return status. if send ok D=1, if send fail D=2
//#define CSID_S_3G_SMS_REMOTE_MG_PROCESS_CONNECTION			_STA_3G(0x2c04) //T=u8, D=0 ,sms remote management processing wan connection . 0: do nothing, 1: stop commander wan process
#define CSID_S_3G_MULTIWAN_SMS_FULL_ALERT			_STA_3G(0x2c10) //T=u8, D=0 ,sms full alert. 1:if sms full 
#define CSID_S_3G_MULTIWAN_SMS_RETURN_STATUS			_STA_3G(0x2c20) //T=u8, D=0 ,sms return status. if send ok D=1, if send fail D=2
#define CSID_S_3G_VOICE_MAIL_ALERT							_STA_3G(0x2c30) //T=u8, D=0 ,voice mail alert. 1:if receive a voice mail
#define CSID_S_3G_MULTIWAN_VOICE_MAIL_ALERT			_STA_3G(0x2c31) //T=u8, D=0 ,voice mail alert. 1:if receive a voice mail
#define CSID_S_3G_MULTIWAN_SMS_REMAINING_CAPACITY			_STA_3G(0x2c32) //T=u8, D=0 ,numbers of SIM card messages remaining capacity
#define CSID_S_3G_SEND_SMS_CURRENT_3G_INDEX			_STA_3G(0x2c50) //T=u8, D=0 ,need 3G index for dual module project
#define CSID_S_3G_SEND_SMS_CURRENT_STATUS				_STA_3G(0x2c51) //T=u8, D=0 ,0:idle, 1:sending

//---Pin Control Corresponding Parameters, added by York 091117
#define CSID_S_3G_PCTL_ACTION					_STA_3G(0x2d00) //T=u32, D=0 ,1:disable 2:enable 3:change 4:unlock_pin 5:unlock_puk
#define CSID_S_3G_MULTIWAN_PCTL_ACTION			_STA_3G(0x2d01) //T=u32, D=0 ,1:disable 2:enable 3:change 4:unlock_pin 5:unlock_puk
#define CSID_S_3G_PCTL_LOCK_PIN					_STA_3G(0x2e00) //T=str,SIM lock(pin code)
#define CSID_S_3G_MULTIWAN_PCTL_LOCK_PIN		_STA_3G(0x2e01) //T=str,SIM lock(pin code)
#define CSID_S_3G_PCTL_CHG_OLD_PIN				_STA_3G(0x2f00) //T=str,chagne function, old password
#define CSID_S_3G_MULTIWAN_PCTL_CHG_OLD_PIN		_STA_3G(0x2f01) //T=str,chagne function, old password
#define CSID_S_3G_PCTL_CHG_NEW_PIN				_STA_3G(0x3000) //T=str,change function, new password
#define CSID_S_3G_MULTIWAN_PCTL_CHG_NEW_PIN		_STA_3G(0x3001) //T=str,change function, new password
#define CSID_S_3G_PCTL_CHG_VRF_PIN				_STA_3G(0x3100) //T=str,change function, use to verify new password again
#define CSID_S_3G_MULTIWAN_PCTL_CHG_VRF_PIN		_STA_3G(0x3101) //T=str,change function, use to verify new password again
#define CSID_S_3G_PCTL_UNLOCK_PIN				_STA_3G(0x3200) //T=str,if SIM lock(pin), use to unlock
#define CSID_S_3G_MULTIWAN_PCTL_UNLOCK_PIN		_STA_3G(0x3201) //T=str,if SIM lock(pin), use to unlock
#define CSID_S_3G_PCTL_UNLOCK_PUK				_STA_3G(0x3300) //T=str,if SIM lock(puk), use to unlock
#define CSID_S_3G_MULTIWAN_PCTL_UNLOCK_PUK		_STA_3G(0x3301) //T=str,if SIM lock(puk), use to unlock
#define CSID_S_3G_PCTL_UNLOCK_NEW_PIN			_STA_3G(0x3400) //T=str,if SIM lock(puk), use to unlock
#define CSID_S_3G_MULTIWAN_PCTL_UNLOCK_NEW_PIN  _STA_3G(0x3401) //T=str,if SIM lock(puk), use to unlock
#define CSID_S_3G_PCTL_PIN_RETRY_TIMES			_STA_3G(0x3500) //T=s32, pin retry times, -1 means the module isn't supported
#define CSID_S_3G_MULTIWAN_PCTL_PIN_RETRY_TIMES _STA_3G(0x3501) //T=u8, pin retry times, -1 means the module isn't supported
#define CSID_S_3G_PCTL_PUK_RETRY_TIMES			_STA_3G(0x3600) //T=s32, puk retry times, -1 means the module isn't supported
#define CSID_S_3G_MULTIWAN_PCTL_PUK_RETRY_TIMES _STA_3G(0x3601) //T=u8, puk retry times, -1 means the module isn't supported
#define CSID_S_3G_PCTL_STATUS					_STA_3G(0x3700) //T=u32, 0:querying, 1:query finished
#define CSID_S_3G_MULTIWAN_PCTL_STATUS          _STA_3G(0x3701) //T=u32, 0:querying, 1:query finished
#define CSID_S_3G_PCTL_ERR_MSG					_STA_3G(0x3800) //T=s32, pin control error messages
#define CSID_S_3G_MULTIWAN_PCTL_ERR_MSG         _STA_3G(0x3801) //T=s32, pin control error messages
#define CSID_S_3G_PCTL_UI_SHOW					_STA_3G(0x3900) //T=s32, for UI for distinguish next step
#define CSID_S_3G_MULTIWAN_PCTL_UI_SHOW         _STA_3G(0x3901) //T=s32, for UI for distinguish next step
#define CSID_S_3G_PCTL_QUERY_FLAG				_STA_3G(0x3a00) //T=u32, pctl query flag, let commander know 3G connect or not
#define CSID_S_3G_MULTIWAN_PCTL_QUERY_FLAG      _STA_3G(0x3a01) //T=u32, pctl query flag, let commander know 3G connect or not
#define CSID_S_3G_PCTL_BTN						_STA_3G(0x3b00) //T=u32, pctl button flag, 1:disable/2:enable
#define CSID_S_3G_MULTIWAN_PCTL_BTN    			_STA_3G(0x3b01) //T=u32, pctl button flag, 1:disable/2:enable
#define CSID_S_3G_SIM_STATUS				    _STA_3G(0x3c00) //T=u32, sim card status, 0: SIM card with PIN code insert/1:SIM card ready/2:No SIM card
#define CSID_S_3G_MULTIWAN_SIM_STATUS			_STA_3G(0x3c01) //T=u32, sim card status for MULTIWAN, 0: SIM card with PIN code insert/1:SIM card ready/2:No SIM card
#define CSID_S_3G_NETWORK_TYPE					_STA_3G(0x3e00) //T=str, network type , NONE/GPRS/EDGE/UMTS/HSDPA/HSUPA
#define CSID_S_3G_MULTIWAN_NETWORK_TYPE    		_STA_3G(0x3e01) //T=str, network type , NONE/GPRS/EDGE/UMTS/HSDPA/HSUPA
#define CSID_S_3G_AUTOBAK_MODE					_STA_3G(0x4000) //T=u16, 0:local wan connected, 1:local wan disconnected
#define CSID_S_3G_CONNECT_TYPE					_STA_3G(0x4001) //T=u16,D=0 0:ppp, 1:sierra_dhcp, 2:hso_dhcp, 3:ipheth, 4: rndis, 5:qmi_dhcp  
#define CSID_S_3G_MULTIWAN_CONNECT_TYPE         _STA_3G(0x4002) //T=u16,D=0 0:ppp, 1:sierra_dhcp, 2:hso_dhcp, 3:ipheth, 4: rndis, 5:qmi_dhcp
#define CSID_S_3G_SIM_CARD_STATUS				_STA_3G(0x5000) //T=str, sim card status
#define CSID_S_3G_SIM_CARD_STATUS_HIJACKING		_STA_3G(0x5001) //T=u32, sim card status for Hijacking
#define CSID_S_3G_SIM_CARD_PIN				_STA_3G(0x5002)  //T=str, sim card pin,if lock sim car pin = null


/* for multi-3G mapping */
#define CSID_S_3G_MAPPING_INDEX             _STA_3G(0x5010) //T=u16, mapping record for usb_index
#define CSID_S_3G_MAPPING_STATUS            _STA_3G(0x5020) //T=u16, device status , 0: no device/1: modem ready
#define CSID_S_3G_WNC_MAL_API_ENABLE    _STA_3G(0x5030) //T=u8, D=0, 0:use at cmd, 1:use WNC MAL API

/* For 3G IPv6 Status */
#define CSID_S_3G_IF_IP_IPV6                    _STA_3G(0x5040)	//T=str, dynamic interface ipv6
#define CSID_S_3G_MULTIWAN_IF_IP_IPV6           _STA_3G(0x5041)	//T=str, dynamic interface ipv6
//#define CSID_S_3G_IF_PREFIX_LEN_IPV6          _STA_3G(0x50)	//T=u32, prefix
//#define CSID_S_3G_MULTIWAN_IF_PREFIX_IPV6     _STA_3G(0x50)	//T=u32, prefix
#define CSID_S_3G_IF_GW_IPV6					_STA_3G(0x5050)	//T=str, dynamic interface gateway
#define CSID_S_3G_MULTIWAN_IF_GW_IPV6           _STA_3G(0x5051)	//T=str, dynamic interface gateway
#define CSID_S_3G_IF_PRIDNS_IPV6				_STA_3G(0x5060)	//T=str, dynamic interface primary DNS
#define CSID_S_3G_MULTIWAN_IF_PRIDNS_IPV6       _STA_3G(0x5061)	//T=str, dynamic interface primary DNS
#define CSID_S_3G_IF_SECDNS_IPV6				_STA_3G(0x5070)	//T=str, dynamic interface secondary DNS
#define CSID_S_3G_MULTIWAN_IF_SECDNS_IPV6       _STA_3G(0x5071)	//T=str, dynamic interface secondary DNS

#define CSID_S_3G_MODEM_FSN						_STA_3G(0x5100) //T=str, modem Factory Serial Number
#define CSID_S_3G_MODEM_TEMPERATURE				_STA_3G(0x5200) //T=str, modem Temperature
#define CSID_S_3G_SYSTEM_MODE					_STA_3G(0x5201) //T=str, 3g system mode
#define CSID_S_3G_WCDMA_BAND					_STA_3G(0x5202) //T=str, 3g WCDMA band
#define CSID_S_3G_WCDMA_CHANNEL					_STA_3G(0x5203) //T=str, 3g WCDMA channel
#define CSID_S_3G_GMM_STATE						_STA_3G(0x5204) //T=str, 3g GMM (PS) state
#define CSID_S_3G_MM_STATE						_STA_3G(0x5205) //T=str, 3g MM (CS) state
#define CSID_S_3G_RX_SIGNAL_STRENGTH			_STA_3G(0x5206) //T=str, 3g RX Signal Strength
#define CSID_S_3G_GET_EXTRA						_STA_3G(0x5207) //T=u8,  3g GET EXTRA
#define CSID_S_3G_NETWORK_SCAN_LIST				_STA_3G(0x5208) //T=str, 3g Network scan list 
#define CSID_S_3G_NETWORK_REG_RESULT			_STA_3G(0x5209) //T=str, network registration result
#define CSID_S_3G_MULTIWAN_NETWORK_SCAN_LIST	_STA_3G(0x5210) //T=str, 3g Network scan list
#define CSID_S_3G_MULTIWAN_NETWORK_REG_RESULT	_STA_3G(0x5220) //T=str, network registration result
#define CSID_S_3G_MULTIWAN_ACTIVE_BAND		_STA_3G(0x5230)	//T=str, active band cotain 2G,3G,4G
#define CSID_S_3G_IMSI							_STA_3G(0x5300) //T=str, 3g IMSI
#define CSID_S_3G_MULTIWAN_IMSI					_STA_3G(0x5301) //T=str, 3g IMSI 
#define CSID_S_3G_ICCID							_STA_3G(0x5310) //T=str, 3g ICCID
#define CSID_S_3G_MULTIWAN_ICCID				_STA_3G(0x5311) //T=str, 3g ICCID
#define CSID_S_3G_MODEM_HW_REVISION				_STA_3G(0x5400) //T=str, modem hardware revision
#define CSID_S_3G_COUNTRY_CODE					_STA_3G(0x5500) //T=str, 3g country code
#define CSID_S_3G_NETWORK_CODE					_STA_3G(0x5600) //T=str, 3g network code
#define CSID_S_3G_CELL_ID						_STA_3G(0x5700) //T=str, Cell ID
#define CSID_S_3G_PRIMARY_SCRAMBLING_CODE		_STA_3G(0x5800) //T=str, Primary Scrambling Code
#define CSID_S_3G_HSUPA_CATEGORY				_STA_3G(0x5900) //T=str, HSUPA category
#define CSID_S_3G_HSDPA_CATEGORY				_STA_3G(0x5a00) //T=str, HSDPA category
#define CSID_S_3G_RSCP							_STA_3G(0x5b00) //T=str, Received Signal Code Power
#define CSID_S_3G_MULTIWAN_RSCP							_STA_3G(0x5b00) //T=str, Received Signal Code Power
#define CSID_S_3G_CELL_BROADCAST_INFO           _STA_3G(0x5c00) //T=str, Cell broadcast information, only show in 2G ONLY mode
#define CSID_S_3G_MULTIWAN_CELL_BROADCAST_INFO  _STA_3G(0x5c00) //T=str, Cell broadcast information, only show in 2G ONLY mode
#define CSID_S_3G_MSISDN          				_STA_3G(0x5d00) //T=str, Msisdn number
#define CSID_S_3G_MULTIWAN_MSISDN               _STA_3G(0x5d00) //T=str, Msisdn number
#define CSID_S_3G_ECIO                       _STA_3G(0x5e00) //T=u32, EC/IO
#define CSID_S_3G_MULTIWAN_ECIO              _STA_3G(0x5e00) //T=u32, EC/IO
#define CSID_S_3G_RSSI                       _STA_3G(0x5f00) //T=u32, Received Signal Strength Indication
#define CSID_S_3G_MULTIWAN_RSSI              _STA_3G(0x5f00) //T=u32, Received Signal Strength Indication

#define CSID_S_3G_MEP_STATUS					_STA_3G(0x6000) //T=u32, MEP_status
#define CSID_S_3G_MEP_ERR_MSG					_STA_3G(0x6001) //T=u32, MEP error message
#define CSID_S_3G_MCC_MNC						_STA_3G(0x6002) //T=str, MCC MNC
#define CSID_S_3G_MEP_CLCK						_STA_3G(0x6003) //T=str, MEP CLCK Status
#define CSID_S_3G_MODEM_RESET_TIME				_STA_3G(0x6004) //T=u8, 3G modem RESER time

/* Connection Control */
#define CSID_S_3G_CNT_FORCE_STOP                _STA_3G(0x6005) //T=u32, D=0, ui alter for stopping 3G Connection

#define CSID_S_3G_SMS_QUERY_FLAG                    _STA_3G(0x6006) //T=u8, sms query flay, let commander know 3G connect or not
#define CSID_S_3G_MULTIWAN_SMS_QUERY_FLAG           _STA_3G(0x6007) //T=u8, sms query flay, let commander know 3G connect or not
#define CSID_S_3G_SMS_FULL_ALERT                    _STA_3G(0x6008) //T=u8, D=0 ,sms full alert. 1:if sms full 
#define CSID_S_3G_SMS_RETURN_STATUS                 _STA_3G(0x6009) //T=u8, D=0 ,sms return status. if send ok D=1, if send fail D=2
#define CSID_S_3G_SMS_REMOTE_MG_PROCESS_CONNECTION  _STA_3G(0x6010) //T=u8, D=0 ,sms remote management processing wan connection . 0: do nothing, 1: stop commander wan process 

#define CSID_S_3G_MULTIWAN_ECIO_STR                 _STA_3G(0x6100) //T=str, Ec/Io string type

/* For Card Permit Function  */
#define CSID_S_3G_MULTIWAN_CARD_PERMIT_STATUS      _STA_3G(0x6500) //T=u8, for card permit status, 0:not permitted, 1:permitted

#define CSID_S_3G_CNT_SCHEDULE_OFF				_STA_3G(0x7000) //T=u32,
#define CSID_S_3G_SIM_PREPAID_STATUS    		_STA_3G(0x7010) //T=u32, D=0, 3G SIM card prepaid status
#define CSID_S_3G_COMM_INITIAL                  _STA_3G(0x7022) //T=u8, D=0 , commer initial 3G ok and get all information, 0: default, 1: ok
#define CSID_S_3G_SIMA_NETWORK_NAME             _STA_3G(0x7023) //T=str, SIMA NETWORK NAME
#define CSID_S_3G_SIMB_NETWORK_NAME             _STA_3G(0x7024) //T=str, SIMA NETWORK NAME
#define CSID_S_3G_SIMA_IMSI                     _STA_3G(0x7025) //T=str, SIMA IMSI
#define CSID_S_3G_SIMB_IMSI                     _STA_3G(0x7026) //T=str, SIMB IMSI
#define CSID_S_3G_SIMA_SERVICE_TYPE             _STA_3G(0x7027) //T=str, SIMA SERVICE TYPE
#define CSID_S_3G_SIMB_SERVICE_TYPE             _STA_3G(0x7028) //T=str, SIMB SERVICE TYPE
#define CSID_S_3G_DUAL_SIM_GPIO                 _STA_3G(0x7029) //T=u32, CHECK GPIO28 DIRECTION, 0: SIMA , 1:SIMB
#define CSID_S_3G_DUAL_SIM_FLAG                 _STA_3G(0x7030) //T=u32, SHOW WHICH SIM SLOT INSERT, 0: NO SIM, 1:SIMA, 2:SIMB, 3:DUAL SIM
#define CSID_S_3G_SIPO_LED_BUF                  _STA_3G(0x7031) //T=str, DUAL SIM SIPO LED BUF
#define CSID_S_3G_SIMSWITCH_READY				_STA_3G(0x7032) //T=u32, CURRENT SIM at WHICH SLOT, 0: NO SIM, 1:SIMA, 2:SIMB, 3:DUAL SIM
#define CSID_S_3G_NOATPORT                      _STA_3G(0x7033) //T=u8,  1: no modem port for device

#define CSID_S_3G_AUTO_APN_STATUS				_STA_3G(0x7100) //T=u32, D=0, 1:success -1: fail
#define CSID_S_3G_TO_UNLOCK_PIN_CODE		    _STA_3G(0x7101) //T=u32, D=0, 1:unlock pin code

/* Modem Restart for multiwan */
#define CSID_S_3G_MODEM_RESTART_COUNT           _STA_3G(0x7120) //T=u32, D=0, count modem connect fail times 
/* modem restart record counter*/
#define CSID_S_3G_MODEM_RESTART_ACCUM_COUNT	_STA_3G(0x7130)	//T=u32, D=0, accumulation count modem connect fail times

#define CSID_S_3G_STK_INDEX                     _STA_3G(0x7200) //T=str, show index for STK, max item = 32
#define CSID_S_3G_STK_MENU                      _STA_3G(0x7220) //T=str, show menu for STK, max item = 32
#define CSID_S_3G_STK_SELECT_MENU               _STA_3G(0x7240) //T=u16, select menu, 0:show index, 1:show menu, 21:show text, 23:input context
#define CSID_S_3G_STK_SELECT_MENU_STATUS        _STA_3G(0x7241) //T=u16, select menu status, 0:show index, 1:show menu, 21:show text, 23:input context
#define CSID_S_3G_STK_MENU_CONTEXT 				      _STA_3G(0x7242) //T=str, show menu context while input or decision
#define CSID_S_3G_STK_INPUT_CONTENT             _STA_3G(0x7250) //T=str, user input content
#define CSID_S_3G_STK_INDEX_ID                  _STA_3G(0x7280) //T=str, use to record index item id
#define CSID_S_3G_STK_MENU_ID                   _STA_3G(0x7300) //T=str, use to record current item id

/* Class 0 SMS */
#define CSID_S_3G_SMS_CLASS0_FLAG               _STA_3G(0x7500) //T=u32, D=0, class0 SMS flag, 0:unused, 1: used
#define CSID_S_3G_SMS_CLASS0_SENDER             _STA_3G(0x7510) //T=str, show sender of class0 SMS
#define CSID_S_3G_SMS_CLASS0_TIME               _STA_3G(0x7520) //T=str, show timestamp of class0 SMS
#define CSID_S_3G_SMS_CLASS0_CONTENT            _STA_3G(0x7530) //T=str, show content of class0 SMS

/* FDN, SPN, PLMN status */

#define CSID_S_3G_FDN_STATUS						  _STA_3G(0x7540) //T=str, show FDN status
#define CSID_S_3G_SPN_DISPLAY_CONDITION				  _STA_3G(0x7541) //T=str, show SPN status
#define CSID_S_3G_SPN								  _STA_3G(0x7542) //T=str, show SPN status

#define CSID_S_3G_STA_SID                             _STA_3G(0x7543) //T=str, show SID
#define CSID_S_3G_STA_NID                             _STA_3G(0x7544) //T=str, show NID
#define CSID_S_3G_STA_RSSI                            _STA_3G(0x7545) //T=str, show RSSI
#define CSID_S_3G_STA_CDMATX                          _STA_3G(0x7546) //T=str, show CDMATX
#define CSID_S_3G_STA_CDMARX                          _STA_3G(0x7547) //T=str, show CDMARX
#define CSID_S_3G_STA_CDMAEC                          _STA_3G(0x7548) //T=str, show CDMAECIO
#define CSID_S_3G_STA_EVDOTX                          _STA_3G(0x7549) //T=str, show EVDOTX
#define CSID_S_3G_STA_EVDORX                          _STA_3G(0x754a) //T=str, show EVDORX
#define CSID_S_3G_STA_EVDOEC                          _STA_3G(0x754b) //T=str, show EVDOECIO

/* Multi 3G Dual SIM connecting config CSID */
#define CSID_S_3G_SIMCARD_UISET_FLAG                _STA_3G(0x7550) //T=u8, D=0, SIM indicate if set by UI, 0:default, 1:set by UI
#define CSID_S_3G_DUALSIM_CARDSTATUS                _STA_3G(0x7560) //T=u16, daul sim card status, 0:No SIM, 1:SIMA, 2:SIMB 
#define CSID_S_3G_DUALSIM_CARDREADY                 _STA_3G(0x7570) //T=u8, if daulsim card ready, 0:not ready 1:ready
#define CSID_S_3G_DUALSIM_USER                      _STA_3G(0x7580) //T=str, dual sim connecting user name
#define CSID_S_3G_DUALSIM_PASSWORD                  _STA_3G(0x7590) //T=str, dual sim connecting user password
#define CSID_S_3G_DUALSIM_APN                       _STA_3G(0x75a0) //T=str, dual sim connecting APN
#define CSID_S_3G_DUALSIM_PIN                       _STA_3G(0x75b0) //T=str, dual sim connecting PIN
#define CSID_S_3G_DUALSIM_NUMBER                    _STA_3G(0x75c0) //T=str, dual sim connecting Dialed Number
#define CSID_S_3G_DUALSIM_IF_PRIDNS                 _STA_3G(0x75d0) //T=ipv4, dual sim connecting dynamic interface primary DNS
#define CSID_S_3G_DUALSIM_IF_SECDNS                 _STA_3G(0x75e0) //T=ipv4, dual sim connecting dynamic interface secondary DNS
#define CSID_S_3G_DUALSIM_AUTHTYPE                  _STA_3G(0x75f0) //T=u16, dual sim connectingauth type, Auto, PAP, CHAP...
#define CSID_S_3G_DUALSIM_AUTO_APN                  _STA_3G(0x7c50) //T=u16, D=1, 0:auto mode 1:Manual 
#define CSID_S_3G_DUALSIM_SIMCARD_FLAG              _STA_3G(0x7c60) //T=u8, let o3g know commander or not
#define CSID_S_3G_DUALSIM_DOWN_FLAG                 _STA_3G(0x7c70) //T=u8, let commander know if sim switch down or not

/* PLMN status */
#define CSID_S_3G_PLMN_NAME							_STA_3G(0x7600) //T=str, show PLMN
#define CSID_S_3G_PLMN_TECHNOLOGY   				_STA_3G(0x7700) //T=str, show PLMN technology
#define CSID_S_3G_OPLMN_NAME						_STA_3G(0x7800) //T=str, show OPLMN
#define CSID_S_3G_OPLMN_TECHNOLOGY  				_STA_3G(0x7900) //T=str, show OPLMN technology
#define CSID_S_3G_HPLMN_NAME						_STA_3G(0x7a00) //T=str, show HPLMN
#define CSID_S_3G_HPLMN_TECHNOLOGY					_STA_3G(0x7b00) //T=str, show HPLMN technology


#define CSID_S_3G_TXPOWER                           _STA_3G(0x7c10) //T=str, Tx power value in dbm.
#define CSID_S_3G_MULTIWAN_TXPOWER                  _STA_3G(0x7c10) //T=str, Tx power value in dbm.
#define CSID_S_3G_RSCP_INT							_STA_3G(0x7c20) //T=u32, Received Signal Code Power in integer type
#define CSID_S_3G_MULTIWAN_RSCP_INT					_STA_3G(0x7c20) //T=u32, Received Signal Code Power in integer type

/* special modem */
#define CSID_S_3G_DNGLINK_FLAG                      _STA_3G(0x7c30) //T=u8, D=0, let UI know show link for sepcoal dongle
#define CSID_S_3G_DNGLINK                           _STA_3G(0x7c40) //T=str, link dongle setting

/* For 3G Profile */
#define CSID_S_3G_PRFILE_ACTNUM_SIMA                _STA_3G(0x7c80) //T=u8, record profile number
#define CSID_S_3G_PRFILE_ACTNUM_SIMB                _STA_3G(0x7c90) //T=u8, record profile number

#define CSID_S_3G_POWER_ON_GPIO_3G_EN               _STA_3G(0x7d00) //T=u8, D=0, indicate if commander power on GPIO_3G_EN pin

/* for A3800 status */
#define CSID_S_3G_MULTIWAN_CURRENT_PLMN   			_STA_3G(0x7e00) //T=str, D=0, Current PLMN
#define CSID_S_3G_MULTIWAN_RSRQ  					_STA_3G(0x7e20) //T=u8, D=0, RSRQ
#define CSID_S_3G_MULTIWAN_PCI  					_STA_3G(0x7e40) //T=u8, D=0, Physical Cell Id
#define CSID_S_3G_MULTIWAN_MIMO  					_STA_3G(0x7e60) //T=u8, D=0, MIMO
#define CSID_S_3G_MULTIWAN_BAND_SUPPORTLIST  		_STA_3G(0x7e80) //T=str, Band Support list

#define CSID_S_3G_MULTIWAN_PRE_PREFER_MODE			_STA_3G(0x7ea0) //T=u16, D=0, 0:auto mode, 1:2g only, 2:3g only, 3:2g first, 4:3g first 
#define CSID_S_3G_MULTIWAN_PRE_NETWORK_SELECT_MODE	_STA_3G(0x7ec0) //T=u8, D=0, 3g network select mode 0:auto, 1:manual 
//Just remind next start from 7ee0

#define CSID_S_3G_MODEM_PORT_LINK_STATUS            _STA_3G(0x7ee0) //T=u8, D=0, Port device connected

//Just remind next start from 7ef0

#define CSID_S_3G_MULTIWAN_SIM_SWITCH_TIME          _STA_3G(0x7ef0) //T=u8, D=0, count switch time
#define CSID_S_3G_MULTIWAN_SIM_SWITCH_FLAG          _STA_3G(0x7f10) //T=u8, D=0, switch flag
#define CSID_S_3G_MULTIWAN_SIM_SWITCH_CURRENT       _STA_3G(0x7f30) //T=u8, D=0, switch current
#define CSID_S_3G_MULTIWAN_SCAN_APPLY_NOW           _STA_3G(0x7f50) //T=u8, D=0, apply now
#define CSID_S_3G_MULTIWAN_SCAN_APPLY_TOSWITCH      _STA_3G(0x7f70) //T=u8, D=0, apply switch
//Just remind next start from 7f90

#define CSID_S_3G_MULTIWAN_SHORT_CELLID_DEC         _STA_3G(0x7f90) //T=u32, Short Cell ID Decimal Type
//Just remind next start from 7fa0

#define CSID_S_3G_PIN_ALTERED                       _STA_3G(0x7fa0)	//T=u8, ui altered
#define CSID_S_3G_SWITCH_ALTERED                    _STA_3G(0x7fb0)	//T=u8, commander altered
//Just remind next start from 7fc0

#define CSID_S_3G_REMOVE_MODULE_FLAG                _STA_3G(0x7fc0)	//T=u8, remove module
#define CSID_S_3G_SMS_ENABLE_ALTERED                _STA_3G(0x7fd0)	//T=u8, sms enable altered
#define CSID_S_3G_MF70           										_STA_3G(0x7fe0)	//T=u8, check MF70
#define CSID_S_3G_TEST           										_STA_3G(0x7fe2)	//T=u8, check MF70
#define CSID_S_3G_NT_SMS_SWITCH_EVENT           		_STA_3G(0x7fe3)	//T=u8, check NT_SMS_SWITCH_EVENT

#define CSID_S_3G_FW_UPGRADE_LOCK_STATUS			_STA_3G(0x7ff0) //T=u16, for FW Upgrade Lock status, 0:unlock, 1:lock

#define CSID_S_3G_MULTIWAN_BAND_CAPABILITY_2G							_STA_3G(0x8000) //T=u32, 3G1~3G16 use bit operator to record band
#define CSID_S_3G_MULTIWAN_BAND_CAPABILITY_3G_FDD					_STA_3G(0x8010) //T=u32, 3G1*2~3G16*2 use bit operator to record band
#define CSID_S_3G_MULTIWAN_BAND_CAPABILITY_3G_TDD					_STA_3G(0x8020) //T=u32, 3G1*2~3G16*2 use bit operator to record band
#define CSID_S_3G_MULTIWAN_BAND_CAPABILITY_4G_FDD					_STA_3G(0x8030) //T=u32, 3G1*2~3G16*2 use bit operator to record band
#define CSID_S_3G_MULTIWAN_BAND_CAPABILITY_4G_TDD					_STA_3G(0x8040) //T=u32, 3G1*2~3G16*2 use bit operator to record band

#define CSID_S_3G_MULTIWAN_BAND_TABLE_2G							_STA_3G(0x8050) //T=str, 3G1~3G16 CTA band table
#define CSID_S_3G_MULTIWAN_BAND_TABLE_3G_FDD					_STA_3G(0x8070) //T=str, 3G1~3G16 CTA band table
#define CSID_S_3G_MULTIWAN_BAND_TABLE_3G_TDD					_STA_3G(0x8090) //T=str, 3G1~3G16 CTA band table
#define CSID_S_3G_MULTIWAN_BAND_TABLE_4G_FDD					_STA_3G(0x80b0) //T=str, 3G1~3G16 CTA band table
#define CSID_S_3G_MULTIWAN_BAND_TABLE_4G_TDD					_STA_3G(0x80d0) //T=str, 3G1~3G16 CTA band table
#define CSID_S_3G_MULTIWAN_BAND_TABLE_ENABLE					_STA_3G(0x80f0) //T=u32, 3G1~3G16 CTA band table

//TEMP CSID
#define CSID_S_3G_MULTIWAN_BAND_CAPABILITY_23G_1TO32							_STA_3G(0x8110) //T=u32, 3G1~3G16 use bit operator to record band
#define CSID_S_3G_MULTIWAN_BAND_CAPABILITY_23G_33TO64							_STA_3G(0x8130) //T=u32, 3G1~3G16 use bit operator to record band

#define CSID_S_3G_DWP157C1_FIRST		_STA_3G(0x8200) //T=u32, Flag to power ON/OFF
#define CSID_S_3G_LU11		_STA_3G(0x8210) //T=u32, Flag to power ON/OFF
#define CSID_S_3G_HW3272		_STA_3G(0x8220) //T=u32, Flag to power ON/OFF
#define CSID_S_3G_HW_DONGLE		_STA_3G(0x8230) //T=u32, Flag to power ON/OFF
#define CSID_S_3G_HW_E3276S_150       _STA_3G(0x8240) //T=u32, Flag to power ON/OFF
#define CSID_S_3G_VODAFONE_FLAG       _STA_3G(0x8250) //T=u32, Flag to power ON/OFF
#define CSID_S_3G_ALCTEL_L850       _STA_3G(0x8260) //T=u32, Flag to get connecting information

//Auto-activation
#define CSID_S_3G_AUTO_ACTIVATION_ISP       _STA_3G(0x8270) //T=u32, Verizon=0, Sprint=1, D=0
#define CSID_S_3G_AUTO_ACTIVATION_MSL_CODE       _STA_3G(0x8280) //T=str, MSL code Only Sprint need
#define CSID_S_3G_AUTO_ACTIVATION_START       _STA_3G(0x8290) //T=str, auto-activation start

/* For 3G APN Profile */
#define CSID_S_3G_PROFILE_REORDER_BY_PRIORITY       _STA_3G(0x8300) //T=u8, indicate the order for the APN profile by priority, 3G1~3G16 with 16 profiles

/* To indicate got altered from network scan page */
#define CSID_S_3G_MULTIWAN_NETWORK_SCAN_ALTERED     _STA_3G(0x8400) //T=u8, D=0, to indicate got altered from UI of network scan page

/* 3G/LTE module temperature */
#define CSID_S_3G_MULTIWAN_TEMPERATURE     _STA_3G(0x8500) //T=u8, D=0, 3G/LTE module temperature

/* 3G/LTE module Manufacturer */
#define CSID_S_3G_MULTIWAN_MANUFACTURER     _STA_3G(0x8600) //T=str, 3G/LTE Manufacturer

/* 3G/LTE module prefer mode for UI show */
#define CSID_S_3G_PREFER_MODE_AUTO_UI_SHOW    _STA_3G(0x8700) //T=u32, d=0, 0=show, 1=not show 3G/LTE module prefer mode for UI show
#define CSID_S_3G_PREFER_MODE_UI_2G_ONLY_SHOW    _STA_3G(0x8720) //T=u32, d=0, 0=show, 1=not show 3G/LTE module prefer mode for UI show
#define CSID_S_3G_PREFER_MODE_UI_3G_ONLY_SHOW    _STA_3G(0x8740) //T=u32, d=0, 0=show, 1=not show 3G/LTE module prefer mode for UI show
#define CSID_S_3G_PREFER_MODE_UI_2G_PREFER_SHOW    _STA_3G(0x8760) //T=u32, d=0, 0=show, 1=not show 3G/LTE module prefer mode for UI show
#define CSID_S_3G_PREFER_MODE_UI_3G_PREFER_SHOW    _STA_3G(0x8780) //T=u32, d=0, 0=show, 1=not show 3G/LTE module prefer mode for UI show
#define CSID_S_3G_PREFER_MODE_UI_LTE_ONLY_SHOW    _STA_3G(0x87a0) //T=u32, d=0, 0=show, 1=not show 3G/LTE module prefer mode for UI show

#define CSID_S_3G_INIT_STATUS    _STA_3G(0x87c0) //T=u32, d=0, 1 = initializing (busy), 0 = initial done, 3G/LTE init done flag

#define CSID_S_3G_MULTIWAN_ARFCN    _STA_3G(0x87e0) //T=u32, d=0, 3G/LTE arfcn(channel number)

#define CSID_S_3G_APN_PID    _STA_3G(0x8800) //T=u32, d=0, APN pid
#define CSID_S_3G_APN_WTIH_MODULE    _STA_3G(0x8820) //T=str, d=0, APN query in module

//Just remind next start from 8800
#endif //ifndef _CSID_3G_H
