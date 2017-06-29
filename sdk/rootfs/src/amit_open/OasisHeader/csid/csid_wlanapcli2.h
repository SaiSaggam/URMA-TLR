/*
 * @file Member ID Definition of Group Wireless APClient. 
 *
 *
 * Moderator: GeorgeWang
 * Group ID: 0x00450000/0x80450000
 */
 
#ifndef _CSID_WLANAPCLI2_H
#define _CSID_WLANAPCLI2_H

#include "csid_gid.h"

#define _CFG_WLANAPCLI2(x)		(CSID_GIDC_WLANAPCLI2|(x))
#define _STA_WLANAPCLI2(x)		(CSID_GIDS_WLANAPCLI2|(x))

#define WLANAPCLI2_AUTH_OPEN			    	0
#define WLANAPCLI2_AUTH_SHARED			    1
#define WLANAPCLI2_AUTH_WEPAUTO				2
#define WLANAPCLI2_AUTH_WPAPSK			    3
#define WLANAPCLI2_AUTH_WPA		      		4
#define WLANAPCLI2_AUTH_WPA2PSK				5
#define WLANAPCLI2_AUTH_WPA2			    	6
#define WLANAPCLI2_AUTH_WPAPSKWPA2PSK	    7
#define WLANAPCLI2_AUTH_WPAWPA2				8

#define WLANAPCLI2_ENCRYPT_NONE		    	0
#define WLANAPCLI2_ENCRYPT_WEP		    	1
#define WLANAPCLI2_ENCRYPT_TKIP		    	2
#define WLANAPCLI2_ENCRYPT_AES		    	3
#define WLANAPCLI2_ENCRYPT_TKIPAES	       	4

#define WLANAPCLI2_WMODE_BG_MIXED	        0
#define WLANAPCLI2_WMODE_B_ONLY		    	1
#define WLANAPCLI2_WMODE_A_ONLY		    	2
#define WLANAPCLI2_WMODE_G_ONLY		    	4
#define WLANAPCLI2_WMODE_N_ONLY		    	6
#define WLANAPCLI2_WMODE_GN_MIXED	        7
#define WLANAPCLI2_WMODE_AN_MIXED	        8
#define WLANAPCLI2_WMODE_BGN_MIXED	        9
#define WLANAPCLI2_WMODE_UNKNOWN	        	10

#define WLANAPCLI2_AREACODE_1_11	        	0
#define WLANAPCLI2_AREACODE_1_13	        	1
#define WLANAPCLI2_AREACODE_10_11	        2
#define WLANAPCLI2_AREACODE_10_13	        3
#define WLANAPCLI2_AREACODE_14             	4
#define WLANAPCLI2_AREACODE_1_14	        	5
#define WLANAPCLI2_AREACODE_3_9	        	6
#define WLANAPCLI2_AREACODE_5_13	        	7

#define WLANAPCLI2_KEYTYPE_HEX	        	0
#define WLANAPCLI2_KEYTYPE_ASCII	        	1

#define WLANAPCLI2_POWERSAVE_DISABLE       	0
#define WLANAPCLI2_POWERSAVE_NORMAL        	1
#define WLANAPCLI2_POWERSAVE_MAX           	2

#define WLANAPCLI2_STATUS_DISCONNECT       	0
#define WLANAPCLI2_STATUS_CONNECT	        1

#define WLANAPCLI2_WPS_MODE_PIN     			1
#define WLANAPCLI2_WPS_MODE_PBC     			2

/* Configuration Items */

#define CSID_C_WLANAPCLI2_ENABLE				_CFG_WLANAPCLI2(0x0001)	//T=u8, D=0, Enable/Disable wireless client function
#define CSID_C_WLANAPCLI2_CHANNEL			_CFG_WLANAPCLI2(0x0002)	//T=u8, D=6, channel
#define CSID_C_WLANAPCLI2_AREACODE			_CFG_WLANAPCLI2(0x0003)	//T=u8,	D=5, ,area code, Ex: 0:ch 1-11, 1:ch 1-13, Refer to Ralink  
#define CSID_C_WLANAPCLI2_SSID				_CFG_WLANAPCLI2(0x0004)	//T=str, ssid
#define CSID_C_WLANAPCLI2_TXRATE				_CFG_WLANAPCLI2(0x0005)	//T=u16, TX rate
#define CSID_C_WLANAPCLI2_FRAGTHRESHOLD    	_CFG_WLANAPCLI2(0x0006)	//T=u16, D=2346, Fragmentation Threshold
#define CSID_C_WLANAPCLI2_RTSCTSTHRESHOLD  	_CFG_WLANAPCLI2(0x0007)	//T=u16, D=2347, CTS/RTS Threshold
#define CSID_C_WLANAPCLI2_WIRLESSMODE  		_CFG_WLANAPCLI2(0x0008)	//T=u8, D=9, wireless mode
#define CSID_C_WLANAPCLI2_POWERSAVE      	_CFG_WLANAPCLI2(0x0009)	//T=u8, power save mode
#define CSID_C_WLANAPCLI2_BGPROTECT    		_CFG_WLANAPCLI2(0x000a)	//T=u8, D=0, B/G protection
#define CSID_C_WLANAPCLI2_TXBURST    		_CFG_WLANAPCLI2(0x000b)	//T=u8, D=1, tx burst
#define CSID_C_WLANAPCLI2_PKTAGGREGATE 		_CFG_WLANAPCLI2(0x000c)	//T=u8, D=1, PktAggregate capable
#define CSID_C_WLANAPCLI2_AUTHMODE   		_CFG_WLANAPCLI2(0x000d)	//T=u8, authentication type
#define CSID_C_WLANAPCLI2_ENCRYPTYPE   		_CFG_WLANAPCLI2(0x000e)	//T=u8, Encryp Type
#define CSID_C_WLANAPCLI2_IEEE8021X   		_CFG_WLANAPCLI2(0x000f)	//T=u8, enable/disable 802_1x
#define CSID_C_WLANAPCLI2_DEFKEYID   		_CFG_WLANAPCLI2(0x0010)	//T=u8, D=1, default wep key id
#define CSID_C_WLANAPCLI2_KEY0TYPE   		_CFG_WLANAPCLI2(0x0011)	//T=u8, D=0, wep key 0 type
#define CSID_C_WLANAPCLI2_KEY0		   		_CFG_WLANAPCLI2(0x0012)	//T=str, D="1234567890", wep key 0 value
#define CSID_C_WLANAPCLI2_KEY1TYPE   		_CFG_WLANAPCLI2(0x0013)	//T=u8, D=0, wep key 1 type
#define CSID_C_WLANAPCLI2_KEY1				_CFG_WLANAPCLI2(0x0014)	//T=str, D="1234567890", wep key 1 value
#define CSID_C_WLANAPCLI2_KEY2TYPE   		_CFG_WLANAPCLI2(0x0015)	//T=u8, D=0, wep key 2 type
#define CSID_C_WLANAPCLI2_KEY2	 			_CFG_WLANAPCLI2(0x0016)	//T=str, D="1234567890", wep key 2 value
#define CSID_C_WLANAPCLI2_KEY3TYPE   		_CFG_WLANAPCLI2(0x0017)	//T=u8, D=0, wep key 3 type
#define CSID_C_WLANAPCLI2_KEY3	  			_CFG_WLANAPCLI2(0x0018)	//T=str, D="1234567890", wep key 3 value
#define CSID_C_WLANAPCLI2_WPAPSK	   			_CFG_WLANAPCLI2(0x0019)	//T=str, D="1234567890", wpa psk value
#define CSID_C_WLANAPCLI2_WMM_ENABLE   	 	_CFG_WLANAPCLI2(0x001a)	//T=u32, Wmm enable/disable
#define CSID_C_WLANAPCLI2_DUALMODE_AP_MAC   	_CFG_WLANAPCLI2(0x001b)	//T=str, destination gateway mac
#define CSID_C_WLANAPCLI2_WIFI_OFFLOAD     	_CFG_WLANAPCLI2(0x001c)	//T=u8, wifi offload
#define CSID_C_WIFI_OFFLOAD2_INTERVAL        _CFG_WLANAPCLI2(0x001d)	//T=u16, D=60, wifi offload interval
#define CSID_C_WIFI_OFFLOAD2_BLINK_TIME      _CFG_WLANAPCLI2(0x001e)	//T=u16, D=120, WISP Led Blink time for wifi offload
#define CSID_C_WLANAPCLI2_MAC				_CFG_WLANAPCLI2(0x001f)	//T=str, MAC
#define CSID_C_WLANAPCLI2_BRIDGE_LAN         _CFG_WLANAPCLI2(0x0020)	//T=u8, bridge to lan
#define CSID_C_WLANAPCLI2_EXTCHANNEL         _CFG_WLANAPCLI2(0x0021)  //T=u8, Extension Channel
#define CSID_C_WLANAPCLI2_MAC2_ENABLE        _CFG_WLANAPCLI2(0x0022)	//T=u8, D=0, Enable MAC2 
#define CSID_C_WLANAPCLI2_MAC2				_CFG_WLANAPCLI2(0x0023)	//T=str, MAC2

/* WPS Client */
#define CSID_C_WLANAPCLI2_WPS_ENABLE        	_CFG_WLANAPCLI2(0xa000) //T=u8, D=0, enable/disable wps function
#define CSID_C_WLANAPCLI2_WPS_CONFSTATUS    	_CFG_WLANAPCLI2(0xa100) //T=u8, D=1, Set WPS Client Simple Config State,  Un-Configured/Configured
#define CSID_C_WLANAPCLI2_WPS_MODE          	_CFG_WLANAPCLI2(0xa200) //T=u8, D=1, configured metheds, PIN/PBC
#define CSID_C_WLANAPCLI2_WPS_TRIGGER        _CFG_WLANAPCLI2(0xa300) //T=u8, WPS, Trigger Client to do simple config.
#define CSID_C_WLANAPCLI2_WPS_STATUS         _CFG_WLANAPCLI2(0xa301) //T=u8, D=0 ,WPS configured status

/* Saved AP's */
#define CSID_C_WLANAPCLI2_SAVED_AP_NAME       _CFG_WLANAPCLI2(0xa310) //T=str, Saved AP Profile Name
#define CSID_C_WLANAPCLI2_SAVED_AP_SSID       _CFG_WLANAPCLI2(0xa320) //T=str, Saved AP SSID
#define CSID_C_WLANAPCLI2_SAVED_AP_AUTHMODE   _CFG_WLANAPCLI2(0xa330) //T=u8, D=0, Saved AP AUTHMODE
#define CSID_C_WLANAPCLI2_SAVED_AP_ENCRYPTYPE _CFG_WLANAPCLI2(0xa340) //T=u8, D=0, Saved AP ENCRYPTYPE
#define CSID_C_WLANAPCLI2_SAVED_AP_DEFKEYID   _CFG_WLANAPCLI2(0xa350) //T=u8, D=0, Saved AP DEFKEYID
#define CSID_C_WLANAPCLI2_SAVED_AP_KEY0TYPE   _CFG_WLANAPCLI2(0xa360) //T=u8, D=0, Saved AP KEY0TYPE
#define CSID_C_WLANAPCLI2_SAVED_AP_KEY0       _CFG_WLANAPCLI2(0xa370) //T=str, Saved AP KEY0
#define CSID_C_WLANAPCLI2_SAVED_AP_KEY1TYPE   _CFG_WLANAPCLI2(0xa380) //T=u8, D=0, Saved AP KEY1TYPE
#define CSID_C_WLANAPCLI2_SAVED_AP_KEY1       _CFG_WLANAPCLI2(0xa390) //T=str, Saved AP KEY1
#define CSID_C_WLANAPCLI2_SAVED_AP_KEY2TYPE   _CFG_WLANAPCLI2(0xa3a0) //T=u8, D=0, Saved AP KEY2TYPE
#define CSID_C_WLANAPCLI2_SAVED_AP_KEY2       _CFG_WLANAPCLI2(0xa3b0) //T=str, Saved AP KEY2
#define CSID_C_WLANAPCLI2_SAVED_AP_KEY3TYPE   _CFG_WLANAPCLI2(0xa3c0) //T=u8, D=0, Saved AP KEY3TYPE
#define CSID_C_WLANAPCLI2_SAVED_AP_KEY3       _CFG_WLANAPCLI2(0xa3d0) //T=str, Saved AP KEY3 
#define CSID_C_WLANAPCLI2_SAVED_AP_WPAPSK     _CFG_WLANAPCLI2(0xa3e0) //T=str, Saved AP WPAPSK
#define CSID_C_WLANAPCLI2_SAVED_AP_CHANNEL    _CFG_WLANAPCLI2(0xa3f0) //T=u8, D=0, Channel of Saved AP 
#define CSID_C_WLANAPCLI2_SAVED_AP_MAC        _CFG_WLANAPCLI2(0xa400) //T=str, Saved AP MAC

#define CSID_C_WLANAPCLI2_CONFIGBYWPS		  _CFG_WLANAPCLI2(0xa500)	//T=u8, D=0, if user configed by wpsapcv2-action

/* Sacn AP */
#define CSID_S_WLANAPCLI2_AP_SSID			_STA_WLANAPCLI2(0x1000) //T=str, Scan AP SSID
#define CSID_S_WLANAPCLI2_AP_ENCRYP			_STA_WLANAPCLI2(0x2000) //T=u8, AP Encryption type
#define CSID_S_WLANAPCLI2_AP_QUALITY			_STA_WLANAPCLI2(0x3000) //T=str, AP QUALITY
#define CSID_S_WLANAPCLI2_AP_AUTHMODE		_STA_WLANAPCLI2(0x4000) //T=u8, AP authentication type
#define CSID_S_WLANAPCLI2_AP_MAC             _STA_WLANAPCLI2(0x5000) //T=str, Scan AP MAC Address
#define CSID_S_WLANAPCLI2_AP_CHANNEL         _STA_WLANAPCLI2(0x6000) //T=str, Scan AP Channel
#define CSID_S_WLANAPCLI2_AP_EXTCHANNEL      _STA_WLANAPCLI2(0x7000) //T=str, Scan AP Extension Channel
#define CSID_S_WLANAPCLI2_AP_WIRELESSMODE  	_STA_WLANAPCLI2(0x8000)	//T=str, Scan AP wireless mode
#define CSID_S_WLANAPCLI2_AP_SCANOK  		_STA_WLANAPCLI2(0x9000)	//T=u8, D=0, Scan AP complete ,1:ok,o:not yet
#define CSID_S_WLANAPCLI2_AP_COUNT  			_STA_WLANAPCLI2(0xa000)	//T=u8, The number of Scan AP 
#define CSID_S_WLANAPCLI2_AP_NETWORK_TYPE    _STA_WLANAPCLI2(0xb000)    //T=u8, 0:Infra,1:Adhoc,Scan AP Network type
#define CSID_S_WLANAPCLI2_AP_INPROF			_STA_WLANAPCLI2(0xc000)	//T=u32, record number in profile
/* Status Items */

#define CSID_S_WLANAPCLI2_ALTERED			_STA_WLANAPCLI2(0x0002)  //T=u8, ui altered
#define CSID_S_WLANAPCLI2_STATUS				_STA_WLANAPCLI2(0x0003)  //T=u8, connect status
#define CSID_S_WLANAPCLI2_WPS_ALTERED      	_STA_WLANAPCLI2(0x0004)  //T=u8, WPS, ui altered
#define CSID_S_WLANAPCLI2_AUTHMODE           _STA_WLANAPCLI2(0x0005)  //T=str, authentication type
#define CSID_S_WLANAPCLI2_ENCRYPTYPE         _STA_WLANAPCLI2(0x0006)  //T=str, Encryp Type
#define CSID_S_WLANAPCLI2_DEFKEYID           _STA_WLANAPCLI2(0x0007)  //T=str, D=1, default wep key id
#define CSID_S_WLANAPCLI2_KEY0TYPE           _STA_WLANAPCLI2(0x0008)  //T=str, D=0, wep key 0 type
#define CSID_S_WLANAPCLI2_KEY1TYPE           _STA_WLANAPCLI2(0x0009)  //T=str, D=0, wep key 1 type
#define CSID_S_WLANAPCLI2_KEY2TYPE           _STA_WLANAPCLI2(0x000a)  //T=str, D=0, wep key 2 type
#define CSID_S_WLANAPCLI2_KEY3TYPE           _STA_WLANAPCLI2(0x000b)  //T=str, D=0, wep key 3 type

/* Reserv 5 profiles */
#define CSID_S_WLANAPCLI2_SAVED_AP_ALTER     _STA_WLANAPCLI2(0x000c)  //T=u8, D=0, Alter WISP profile 
#define CSID_S_WLANAPCLI2_SAVED_AP_INUSE     _STA_WLANAPCLI2(0x0010)  //T=u8, D=0, whether the saved AP profile in used or not

#define CSID_S_WLANAPCLI2_WPS_STATUS         _STA_WLANAPCLI2(0x0100)  //T=u8, WPS Status
#define CSID_S_WLANAPCLI2_WPS_TRIGGER        _STA_WLANAPCLI2(0x0101)  //T=u8, WPS Trigger
#define CSID_S_WLANAPCLI2_WPS_ACTION_RUNNING _STA_WLANAPCLI2(0x0102)  //T=u8, WPS action is running

#define CSID_S_WLANAPCLI2_MAC                _STA_WLANAPCLI2(0x0200)  //T=str, apcli0 MAC address

#define CSID_S_WLANAPCLI2_SIGNAL             _STA_WLANAPCLI2(0x0300)  //T=u32, apcli0 signal
#endif
