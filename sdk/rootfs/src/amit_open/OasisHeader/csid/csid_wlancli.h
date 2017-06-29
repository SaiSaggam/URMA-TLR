/*
 * @file Member ID Definition of Group Wireless Client. 
 *
 *
 * Moderator: GeorgeWang
 * Group ID: 0x001c0000/0x801c0000
 */
 
#ifndef _CSID_WLANCLI_H
#define _CSID_WLANCLI_H

#include "csid_gid.h"

#define _CFG_WLANCLI(x)		(CSID_GIDC_WLANCLI|(x))
#define _STA_WLANCLI(x)		(CSID_GIDS_WLANCLI|(x))

#define WLANCLI_AUTH_OPEN				0
#define WLANCLI_AUTH_SHARED				1
#define WLANCLI_AUTH_WEPAUTO			2
#define WLANCLI_AUTH_WPAPSK			    3
#define WLANCLI_AUTH_WPA		      	4
#define WLANCLI_AUTH_WPA2PSK			5
#define WLANCLI_AUTH_WPA2			    6
#define WLANCLI_AUTH_WPAPSKWPA2PSK	    7
#define WLANCLI_AUTH_WPAWPA2			8
#define WLANCLI_AUTH_WPANONE            9

#define WLANCLI_ENCRYPT_NONE		    0
#define WLANCLI_ENCRYPT_WEP		    	1
#define WLANCLI_ENCRYPT_TKIP		   	2
#define WLANCLI_ENCRYPT_AES		    	3
#define WLANCLI_ENCRYPT_TKIPAES         4

#define WLANCLI_WMODE_BG_MIXED	        0
#define WLANCLI_WMODE_B_ONLY			1
#define WLANCLI_WMODE_A_ONLY			2
#define WLANCLI_WMODE_ABG_MIXED			3
#define WLANCLI_WMODE_G_ONLY			4
#define WLANCLI_WMODE_ABGN_MIXED		5
#define WLANCLI_WMODE_N_ONLY			6
#define WLANCLI_WMODE_GN_MIXED	        7
#define WLANCLI_WMODE_AN_MIXED	        8
#define WLANCLI_WMODE_BGN_MIXED	        9
#define WLANCLI_WMODE_AGN_MIXED	        10

#define WLANCLI_AREACODE_1_11	        0
#define WLANCLI_AREACODE_1_13	        1
#define WLANCLI_AREACODE_10_11	        2
#define WLANCLI_AREACODE_10_13	        3
#define WLANCLI_AREACODE_14             4
#define WLANCLI_AREACODE_1_14	        5
#define WLANCLI_AREACODE_3_9	        6
#define WLANCLI_AREACODE_5_13	        7

#define WLANCLI_KEYTYPE_HEX	        	0
#define WLANCLI_KEYTYPE_ASCII	        1

#define WLANCLI_POWERSAVE_DISABLE       0
#define WLANCLI_POWERSAVE_NORMAL        1
#define WLANCLI_POWERSAVE_MAX           2

#define WLANCLI_STATUS_DISCONNECT       0
#define WLANCLI_STATUS_CONNECT	        1

#define WLANCLI_WPS_MODE_PIN     1
#define WLANCLI_WPS_MODE_PBC     2

#define WLANCLI_WPS_CONFMODE_DISABLE     0
#define WLANCLI_WPS_CONFMODE_ENROLEE     1
#define WLANCLI_WPS_CONFMODE_REGISTRAR   2

#define WLANCLI_WPS_CONFSTATUS_UNCONFIGURED 1
#define WLANCLI_WPS_CONFSTATUS_CONFIGURED 2

#define WLANCLI_WPS_STATUS_NOUSED        0
#define WLANCLI_WPS_STATUS_IDLE          1
#define WLANCLI_WPS_STATUS_PROCESSFAIL 2
#define WLANCLI_WPS_STATUS_STARTPROCESS 3
#define WLANCLI_WPS_STATUS_RECVEAPOLSTART 4
#define WLANCLI_WPS_STATUS_SENDEAPREQID 5
#define WLANCLI_WPS_STATUS_RECVEAPRSPID 6
#define WLANCLI_WPS_STATUS_RECVWRGVENDORID 7
#define WLANCLI_WPS_STATUS_RECVWRGVENDORTYPE 8
#define WLANCLI_WPS_STATUS_SENDEAPREQSTART 9
#define WLANCLI_WPS_STATUS_SENDM1 10
#define WLANCLI_WPS_STATUS_RECVM1 11
#define WLANCLI_WPS_STATUS_SENDM2 12
#define WLANCLI_WPS_STATUS_RECVM2 13
#define WLANCLI_WPS_STATUS_RECVM2D 14
#define WLANCLI_WPS_STATUS_SENDM3 15
#define WLANCLI_WPS_STATUS_RECVM3 16
#define WLANCLI_WPS_STATUS_SENDM4 17
#define WLANCLI_WPS_STATUS_RECVM4 18
#define WLANCLI_WPS_STATUS_SENDM5 19
#define WLANCLI_WPS_STATUS_RECVM5 20
#define WLANCLI_WPS_STATUS_SENDM6 21
#define WLANCLI_WPS_STATUS_RECVM6 22
#define WLANCLI_WPS_STATUS_SENDM7 23
#define WLANCLI_WPS_STATUS_RECVM7 24
#define WLANCLI_WPS_STATUS_SENDM8 25
#define WLANCLI_WPS_STATUS_RECVM8 26
#define WLANCLI_WPS_STATUS_PROCEAPRSPACK 27
#define WLANCLI_WPS_STATUS_PROCEAPREQDONE 28
#define WLANCLI_WPS_STATUS_PROCEAPRSPDONE 29
#define WLANCLI_WPS_STATUS_SENDEAPFAIL 30
#define WLANCLI_WPS_STATUS_ERRHASHFAIL 31
#define WLANCLI_WPS_STATUS_ERRHMACFAIL 32
#define WLANCLI_WPS_STATUS_ERRDEVPWDAUTHFAIL 33
#define WLANCLI_WPS_STATUS_CONFIGURED 34

/* Configuration Items */

#define CSID_C_WLANCLI_ENABLE			_CFG_WLANCLI(0x0001)	//T=u8, D=1, Enable/Disable wireless client function
#define CSID_C_WLANCLI_CHANNEL			_CFG_WLANCLI(0x0002)	//T=u8, D=6, channel
#define CSID_C_WLANCLI_AREACODE			_CFG_WLANCLI(0x0003)	//T=u8,	D=5, ,area code, Ex: 0:ch 1-11, 1:ch 1-13, Refer to Ralink  
#define CSID_C_WLANCLI_SSID					_CFG_WLANCLI(0x0004)	//T=str, ssid
#define CSID_C_WLANCLI_TXRATE				_CFG_WLANCLI(0x0005)	//T=u16, TX rate
#define CSID_C_WLANCLI_FRAGTHRESHOLD    	_CFG_WLANCLI(0x0006)	//T=u16, D=2346, Fragmentation Threshold
#define CSID_C_WLANCLI_RTSCTSTHRESHOLD  	_CFG_WLANCLI(0x0007)	//T=u16, D=2347, CTS/RTS Threshold
#define CSID_C_WLANCLI_WIRLESSMODE  		_CFG_WLANCLI(0x0008)	//T=u8, D=9, wireless mode
#define CSID_C_WLANCLI_POWERSAVE      		_CFG_WLANCLI(0x0009)	//T=u8, power save mode
#define CSID_C_WLANCLI_BGPROTECT    		_CFG_WLANCLI(0x000a)	//T=u8, D=0, B/G protection
#define CSID_C_WLANCLI_TXBURST    			_CFG_WLANCLI(0x000b)	//T=u8, D=1, tx burst
#define CSID_C_WLANCLI_PKTAGGREGATE 		_CFG_WLANCLI(0x000c)	//T=u8, D=1, PktAggregate capable
#define CSID_C_WLANCLI_AUTHMODE   			_CFG_WLANCLI(0x000d)	//T=u8, authentication type
#define CSID_C_WLANCLI_ENCRYPTYPE   		_CFG_WLANCLI(0x000e)	//T=u8, Encryp Type
#define CSID_C_WLANCLI_IEEE8021X   			_CFG_WLANCLI(0x000f)	//T=u8, enable/disable 802_1x
#define CSID_C_WLANCLI_DEFKEYID   			_CFG_WLANCLI(0x0010)	//T=u8, D=1, default wep key id
#define CSID_C_WLANCLI_KEY0TYPE   			_CFG_WLANCLI(0x0011)	//T=u8, D=0, wep key 0 type
#define CSID_C_WLANCLI_KEY0		   			_CFG_WLANCLI(0x0012)	//T=str, wep key 0 value
#define CSID_C_WLANCLI_KEY1TYPE   			_CFG_WLANCLI(0x0013)	//T=u8, D=0, wep key 1 type
#define CSID_C_WLANCLI_KEY1					_CFG_WLANCLI(0x0014)	//T=str, wep key 1 value
#define CSID_C_WLANCLI_KEY2TYPE   			_CFG_WLANCLI(0x0015)	//T=u8, D=0, wep key 2 type
#define CSID_C_WLANCLI_KEY2	 				_CFG_WLANCLI(0x0016)	//T=str, wep key 2 value
#define CSID_C_WLANCLI_KEY3TYPE   			_CFG_WLANCLI(0x0017)	//T=u8, D=0, wep key 3 type
#define CSID_C_WLANCLI_KEY3	  				_CFG_WLANCLI(0x0018)	//T=str, wep key 3 value
#define CSID_C_WLANCLI_WPAPSK	   			_CFG_WLANCLI(0x0019)	//T=str, wpa psk value
#define CSID_C_WLANCLI_WMM_ENABLE   	 	_CFG_WLANCLI(0x001a)	//T=u32, Wmm enable/disable
#define CSID_C_WLANCLI_SECURITY   		    _CFG_WLANCLI(0x001b)    //T=u32, Wmm enable/disable elektra
#define CSID_C_WLANCLI_WEP_KEY_SELECT   	_CFG_WLANCLI(0x001c)
#define CSID_C_WLANCLI_PASSPHRASE_KEY   	_CFG_WLANCLI(0x001d)
#define CSID_C_WLANCLI_BSSID            	_CFG_WLANCLI(0x001f)    //T=str, /the mac address of the interface
#define CSID_C_WLANCLI_TXPREAMPLE			_CFG_WLANCLI(0x0020)	//T=u8, D=0, tx preamble
#define CSID_C_WLANCLI_IEEE8021H   			_CFG_WLANCLI(0x0021)	//T=u8, enable/disable 802_1h
#define CSID_C_WLANCLI_NETWORKTYPE   		_CFG_WLANCLI(0x0022)    //T=str, D=Infra, Infra or Adhoc
#define CSID_C_WLANCLI_BRIDGE_LAN   		_CFG_WLANCLI(0x0023)    //T=u8, bridge to lan

/*HT parameter Setting */
#define CSID_C_WLANCLI_HT_RDG               _CFG_WLANCLI(0x1000)     //T=u8, D=1, HT_RDG
#define CSID_C_WLANCLI_HT_EXTCHA            _CFG_WLANCLI(0x1100)     //T=u8, HT_EXTCHA
#define CSID_C_WLANCLI_HT_OPMODE            _CFG_WLANCLI(0x1200)     //T=u8, HT_OpMode
#define CSID_C_WLANCLI_HT_MPDUDENSITY       _CFG_WLANCLI(0x1300)     //T=u8, D=5, HT_MpduDensity
#define CSID_C_WLANCLI_HT_BW                _CFG_WLANCLI(0x1400)     //T=u8, D=1, HT_BW
#define CSID_C_WLANCLI_HT_AUTOBA            _CFG_WLANCLI(0x1500)     //T=u8, D=1, HT_AutoBA
#define CSID_C_WLANCLI_HT_AMSDU             _CFG_WLANCLI(0x1600)     //T=u8, HT_AMSDU
#define CSID_C_WLANCLI_HT_BAWINSZ           _CFG_WLANCLI(0x1700)     //T=u8, D=64, HT_BAWinSize
#define CSID_C_WLANCLI_HT_GI                _CFG_WLANCLI(0x1800)     //T=u8, D=1, HT_GI
#define CSID_C_WLANCLI_HT_MCS               _CFG_WLANCLI(0x1900)     //T=u8, D=3, HT_MCS
#define CSID_C_WLANCLI_HT_MIMOPSMODE    	_CFG_WLANCLI(0x2000)     //T=u8, D=2, MIMO Power Save Mode
#define CSID_C_WLANCLI_HT_BADECLINE    	 	_CFG_WLANCLI(0x2100)     //T=u8, D=0, Reject BA request from AP,0:Disabled,1:Enabled

/* WPS Client Setting*/
#define CSID_C_WLANCLI_WPS_ENABLE        	 _CFG_WLANCLI(0xa000) //T=u8, D=0, enable/disable wps function
#define CSID_C_WLANCLI_WPS_CONFSTATUS    	 _CFG_WLANCLI(0xa100) //T=u8, D=1, Set WPS Client Simple Config State,  Un-Configured/Configured
#define CSID_C_WLANCLI_WPS_MODE          	 _CFG_WLANCLI(0xa200) //T=u8, D=1, configured metheds, PIN/PBC
#define CSID_C_WLANCLI_WPS_TRIGGER           _CFG_WLANCLI(0xa300) //T=u8, WPS, Trigger Client to do simple config.
#define CSID_C_WLANCLI_WPS_STATUS            _CFG_WLANCLI(0xa301) //T=u8, D=0 ,WPS configured status
#define CSID_C_WLANCLI_WPS_CONFMODE              _CFG_WLANCLI(0xa400) //T=u8, Set WPS function, Enrollee, Proxy, Registrar. bitwise

/* Status Items */

#define CSID_S_WLANCLI_ALTERED			_STA_WLANCLI(0x0001) //T=u8, ui altered
#define CSID_S_WLANCLI_STATUS			_STA_WLANCLI(0x0002) //T=u8, connect status
#define CSID_S_WLANCLI_AUTHMODE   			_STA_WLANCLI(0x0003)	//T=str, authentication type used for uo file.
#define CSID_S_WLANCLI_ENCRYPTYPE   		_STA_WLANCLI(0x0004)	//T=str, Encryp Type used for uo file.
#define CSID_S_WLANCLI_DEFKEYID               	_STA_WLANCLI(0x0005)  //T=str, D=1, default wep key id
#define CSID_S_WLANCLI_KEY0TYPE               	_STA_WLANCLI(0x0006)  //T=str, D=0, wep key 0 type
#define CSID_S_WLANCLI_KEY1TYPE               	_STA_WLANCLI(0x0007)  //T=str, D=0, wep key 1 type
#define CSID_S_WLANCLI_KEY2TYPE               	_STA_WLANCLI(0x0008)  //T=str, D=0, wep key 2 type
#define CSID_S_WLANCLI_KEY3TYPE               	_STA_WLANCLI(0x0009)  //T=str, D=0, wep key 3 type

#define CSID_S_WLANCLI_CURRENT_SSID				_STA_WLANCLI(0x0050)  //T=str, D="", current connected SSID.
#define CSID_S_WLANCLI_CURRENT_AP				_STA_WLANCLI(0x0051)  //T=str, D="", current connected AP MAC address.
#define CSID_S_WLANCLI_CURRENT_BITRATE			_STA_WLANCLI(0x0052)  //T=str, D="", current connected Bit Rate.
#define CSID_S_WLANCLI_CURRENT_QUALITY			_STA_WLANCLI(0x0053)  //T=str, D="", current connected Link Quality.
#define CSID_S_WLANCLI_RA0_OK			_STA_WLANCLI(0x0054)  //T=u8, D=0, 1:ra0 up ok,0:ra0 up not ok
#define CSID_S_WLANCLI_SIGNAL_LEVEL			_STA_WLANCLI(0x0055)  //T=str, D="", current connected Link Quality.


#define CSID_S_WLANCLI_PHASE2_COMPLETE			_STA_WLANCLI(0x0056)  //T=u8, D=0, wlancli phase2 complete

// WPS Status
#define CSID_S_WLANCLI_WPS_ENROLLEE_PINCODE      _STA_WLANCLI(0x0100) //T=str, WPS, Enrollee's PIN Code
#define CSID_S_WLANCLI_WPS_TRIGGER                       _STA_WLANCLI(0x0101) //T=u8, WPS, Trigger STA to do simple config.
#define CSID_S_WLANCLI_WPS_ALTERED                       _STA_WLANCLI(0x0102) //T=u8, WPS, ui altered
#define CSID_S_WLANCLI_WPS_STATUS                        _STA_WLANCLI(0x0103) //T=u8, D=0 ,WPS configured status
#define CSID_S_WLANCLI_WPS_AP_PINCODE            _STA_WLANCLI(0x0104) //T=str, WPS AP's PIN Code
#define CSID_S_WLANCLI_WPS_AP_PUSH                       _STA_WLANCLI(0x0105) //T=u8, WPS, Push Button Enable/Disable
#define CSID_S_WLANCLI_WPS_ACTION_INTERVAL       _STA_WLANCLI(0x0106)     //T=u32, D=30,WPS Action Time Interval

#define CSID_S_WLANCLI_WPS_DEF_AUTHMODE          _STA_WLANCLI(0x0110) //T=u32, Default WPS AuthMode
#define CSID_S_WLANCLI_WPS_DEF_ENCRYPTYPE        _STA_WLANCLI(0x0111) //T=u32, Default WPS ENCRYPTYPE
#define CSID_S_WLANCLI_WPS_DEF_WPAPSK            _STA_WLANCLI(0x0112) //T=str, Default WPS WPAPSK
#define CSID_S_WLANCLI_WPS_ACTION_RUNNING        _STA_WLANCLI(0x0113) //T=u8, wlancli WPS action running

/* Sacn AP */
#define CSID_S_WLANCLI_AP_SSID			_STA_WLANCLI(0x1000) //T=str, Scan AP SSID
#define CSID_S_WLANCLI_AP_ENCRYP		_STA_WLANCLI(0x2000) //T=u8, AP Encryption type
#define CSID_S_WLANCLI_AP_QUALITY		_STA_WLANCLI(0x3000) //T=str, AP QUALITY
#define CSID_S_WLANCLI_AP_AUTHMODE		_STA_WLANCLI(0x4000) //T=u8, AP authentication type
#define CSID_S_WLANCLI_AP_MAC			_STA_WLANCLI(0x5000) //T=str, Scan AP MAC Address
#define CSID_S_WLANCLI_AP_CHANNEL		_STA_WLANCLI(0x6000) //T=str, Scan AP Channel
#define CSID_S_WLANCLI_AP_EXTCHANNEL		_STA_WLANCLI(0x7000) //T=str, Scan AP Extension Channel
#define CSID_S_WLANCLI_AP_WIRELESSMODE  	_STA_WLANCLI(0x8000)	//T=str, Scan AP wireless mode
#define CSID_S_WLANCLI_AP_NETWORK_TYPE  	_STA_WLANCLI(0x9000)	//T=u8, 0:Infra,1:Adhoc,Scan AP Network type
#define CSID_S_WLANCLI_AP_WPS  			_STA_WLANCLI(0xa000)	//T=u8, 0:off,1:on,Scan AP wps on or not

#endif
