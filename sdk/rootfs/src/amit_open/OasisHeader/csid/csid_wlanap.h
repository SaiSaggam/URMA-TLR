/*
 * @file Member ID Definition of Group Wireless AP. 
 *
 * Moderator: GeorgeWang
 * Group ID: 0x00190000/0x80190000
 */
 
#ifndef _CSID_WLANAP_H
#define _CSID_WLANAP_H

#include "csid_gid.h"

#define _CFG_WLANAP(x)		(CSID_GIDC_WLANAP|(x))
#define _STA_WLANAP(x)		(CSID_GIDS_WLANAP|(x))

#define WLANAP_AUTH_OPEN			0
#define WLANAP_AUTH_SHARED			1
#define WLANAP_AUTH_WEPAUTO			2
#define WLANAP_AUTH_WPAPSK			3
#define WLANAP_AUTH_WPA				4
#define WLANAP_AUTH_WPA2PSK			5
#define WLANAP_AUTH_WPA2			6
#define WLANAP_AUTH_WPAPSKWPA2PSK	        7
#define WLANAP_AUTH_WPAWPA2			8

#define WLANAP_ENCRYPT_NONE		0
#define WLANAP_ENCRYPT_WEP		1
#define WLANAP_ENCRYPT_TKIP		2
#define WLANAP_ENCRYPT_AES		3
#define WLANAP_ENCRYPT_TKIPAES	        4

#define WLANAP_WMODE_BG_MIXED	        0
#define WLANAP_WMODE_B_ONLY		1
#define WLANAP_WMODE_A_ONLY		2
#define WLANAP_WMODE_G_ONLY		4
#define WLANAP_WMODE_N_ONLY		6
#define WLANAP_WMODE_GN_MIXED	        7
#define WLANAP_WMODE_AN_MIXED	        8
#define WLANAP_WMODE_BGN_MIXED	        9
#define WLANAP_WMODE_UNKNOWN	        10

#define WLANAP_AREACODE_1_11		0
#define WLANAP_AREACODE_1_13		1
#define WLANAP_AREACODE_10_11		2
#define WLANAP_AREACODE_10_13		3
#define WLANAP_AREACODE_14			4
#define WLANAP_AREACODE_1_14		5
#define WLANAP_AREACODE_3_9			6
#define WLANAP_AREACODE_5_13		7

#define WLANAP_KEYTYPE_HEX		0
#define WLANAP_KEYTYPE_ASCII		1

#define WLANAP_WPS_CONFMODE_DISABLE     0 
#define WLANAP_WPS_CONFMODE_ENROLEE     1
#define WLANAP_WPS_CONFMODE_PROXY       2
#define WLANAP_WPS_CONFMODE_REGISTRAR   4

#define WLANAP_WPS_CONFSTATUS_UNCONFIGURED 1
#define WLANAP_WPS_CONFSTATUS_CONFIGURED 2

#define WLANAP_WPS_MODE_PIN		1
#define WLANAP_WPS_MODE_PBC		2	

#define WLANAP_WPS_STATUS_NOUSED	0
#define WLANAP_WPS_STATUS_IDLE		1
#define WLANAP_WPS_STATUS_PROCESSFAIL 2
#define WLANAP_WPS_STATUS_STARTPROCESS 3
#define WLANAP_WPS_STATUS_RECVEAPOLSTART 4
#define WLANAP_WPS_STATUS_SENDEAPREQID 5
#define WLANAP_WPS_STATUS_RECVEAPRSPID 6
#define WLANAP_WPS_STATUS_RECVWRGVENDORID 7
#define WLANAP_WPS_STATUS_RECVWRGVENDORTYPE 8
#define WLANAP_WPS_STATUS_SENDEAPREQSTART 9
#define WLANAP_WPS_STATUS_SENDM1 10
#define WLANAP_WPS_STATUS_RECVM1 11
#define WLANAP_WPS_STATUS_SENDM2 12
#define WLANAP_WPS_STATUS_RECVM2 13
#define WLANAP_WPS_STATUS_RECVM2D 14
#define WLANAP_WPS_STATUS_SENDM3 15
#define WLANAP_WPS_STATUS_RECVM3 16
#define WLANAP_WPS_STATUS_SENDM4 17
#define WLANAP_WPS_STATUS_RECVM4 18
#define WLANAP_WPS_STATUS_SENDM5 19
#define WLANAP_WPS_STATUS_RECVM5 20
#define WLANAP_WPS_STATUS_SENDM6 21
#define WLANAP_WPS_STATUS_RECVM6 22
#define WLANAP_WPS_STATUS_SENDM7 23
#define WLANAP_WPS_STATUS_RECVM7 24
#define WLANAP_WPS_STATUS_SENDM8 25
#define WLANAP_WPS_STATUS_RECVM8 26
#define WLANAP_WPS_STATUS_PROCEAPRSPACK 27
#define WLANAP_WPS_STATUS_PROCEAPREQDONE 28
#define WLANAP_WPS_STATUS_PROCEAPRSPDONE 29
#define WLANAP_WPS_STATUS_SENDEAPFAIL 30
#define WLANAP_WPS_STATUS_ERRHASHFAIL 31
#define WLANAP_WPS_STATUS_ERRHMACFAIL 32
#define WLANAP_WPS_STATUS_ERRDEVPWDAUTHFAIL 33
#define WLANAP_WPS_STATUS_CONFIGURED 34
#define WLANAP_WPS_STATUS_WSCSCANAP 35

#define WLANAP_DUALMODE_STATUS_UNLINK 	0
#define WLANAP_DUALMODE_STATUS_GETFILE 	1
#define WLANAP_DUALMODE_STATUS_LINK 	2
#define WLANAP_DUALMODE_STATUS_TRYTOLINK 	10

#define WLANAP_APCLI_LINK_STATUS_UNLINK 	0
#define WLANAP_APCLI_LINK_STATUS_LINK 	2
#define WLANAP_ATE_CMODE_NORMAL 0
#define WLANAP_ATE_CMODE_CONTTX 1
#define WLANAP_ATE_CMODE_CONTRX 2
#define WLANAP_ATE_CMODE_TXCARRIER 3

#define WLANAP_ATE_WMODE_B_ONLY		0
#define WLANAP_ATE_WMODE_G_ONLY		1
#define WLANAP_ATE_WMODE_BGN_MIXED	        2
#define WLANAP_ATE_WMODE_N_ONLY		3

#define WLANAP_ATE_DATERATE_1M 0
#define WLANAP_ATE_DATERATE_2M 1
#define WLANAP_ATE_DATERATE_55M 2
#define WLANAP_ATE_DATERATE_11M 3
#define WLANAP_ATE_DATERATE_6M 4
#define WLANAP_ATE_DATERATE_9M 5
#define WLANAP_ATE_DATERATE_12M 6
#define WLANAP_ATE_DATERATE_18M 7
#define WLANAP_ATE_DATERATE_24M 8
#define WLANAP_ATE_DATERATE_36M 9
#define WLANAP_ATE_DATERATE_48M 10
#define WLANAP_ATE_DATERATE_54M 11
#define WLANAP_ATE_DATERATE_MCS0 12
#define WLANAP_ATE_DATERATE_MCS1 13
#define WLANAP_ATE_DATERATE_MCS2 14
#define WLANAP_ATE_DATERATE_MCS3 15
#define WLANAP_ATE_DATERATE_MCS4 16
#define WLANAP_ATE_DATERATE_MCS5 17
#define WLANAP_ATE_DATERATE_MCS6 18
#define WLANAP_ATE_DATERATE_MCS7 19
#define WLANAP_ATE_DATERATE_MCS8 20
#define WLANAP_ATE_DATERATE_MCS9 21
#define WLANAP_ATE_DATERATE_MCS10 22
#define WLANAP_ATE_DATERATE_MCS11 23
#define WLANAP_ATE_DATERATE_MCS12 24 
#define WLANAP_ATE_DATERATE_MCS13 25
#define WLANAP_ATE_DATERATE_MCS14 26
#define WLANAP_ATE_DATERATE_MCS15 27

#define WLANAP_ATE_BW20 0
#define WLANAP_ATE_BW40 1

#define WLANAP_ATE_TXANT_ALL 	0	//All Antenna
#define WLANAP_ATE_TXANT_1 	1	//Antenna 1
#define WLANAP_ATE_TXANT_2 	2	//Antenna 2

#define WLANAP_PSMODE_DO_NOTHING	0
#define WLANAP_PSMODE_AUTO			1		// power adator in ? high : low
#define WLANAP_PSMODE_HIGH			2		// High Power
#define WLANAP_PSMODE_LOW			3		// Low Power


/* Configuration Items */

#define CSID_C_WLANAP_ENABLE			_CFG_WLANAP(0x0001)	//T=u8, D=1, Enable/Disable wireless function
#define CSID_C_WLANAP_BSSIDNUM			_CFG_WLANAP(0x0002)	//T=u8, D=1, Number of BSSID
#define CSID_C_APSWITCH_ENABLE			_CFG_WLANAP(0x0003) //T=u8, apswitch enable.
#define CSID_C_WLANAP_PSMODE			_CFG_WLANAP(0x0004) //T=u32, D=0, WLANAP Power Saving Mode,
#define CSID_C_WLANAP_MP                _CFG_WLANAP(0x0005) //T=u8, D=0, Enable/Disable UDPserver
#define CSID_C_WLANAP_WISP_ENABLE	        _CFG_WLANAP(0x0006)	//T=u8, D=1, Enable/Disable wireless wisp function
#define CSID_C_WLANAP_BUTTON_SWITCH	        _CFG_WLANAP(0x0007)	//T=u8, D=1, Enable/Disable wireless button triger function
#define CSID_C_WLANAP_WIRELESS_MODE         _CFG_WLANAP(0x0008) //T=u8, wireless mode, 0:AP Router Mode 1:AP Only Mode 2:WDS Hybrid 3:WDS Only 4:Adapter 5:Universal Repeater 6:Ad hoc
#define CSID_C_WLANAP_SETTING_SSID	        _CFG_WLANAP(0x0009)	//T=str, D="default_", Setting Default SSID
//#define CSID_C_WLANAP_RDRegion 		_CFG_WLANAP(0x000a)	//T=u8, D=0, Setting RDRegion:CE:0, FCC:1, JAP:2, JAP_W53:3, JAP_W56:4
#define CSID_C_WLANAP_WDS_LAZY_MODE         _CFG_WLANAP(0x0010) //T=u8, Enable/Disable WDS Lazy Mode
#define CSID_C_WLANAP_WLMODE_DHCP_INT       _CFG_WLANAP(0x0011) //T=u8, Wireless Mode DHCP Renew Interval

/*Basic Setting */
//define total 256 Multiple AP id
#define CSID_C_WLANAP_APID_ENABLE		_CFG_WLANAP(0x0100)	//T=u8, D=1, Enable/Disable a specific AP in Multi-BSSID function.
#define CSID_C_WLANAP_CHANNEL			_CFG_WLANAP(0x0200)	//T=u8, D=6, channel
#define CSID_C_WLANAP_AREACODE			_CFG_WLANAP(0x0300)	//T=u8,	D=5, area code CountryRegion , Ex: 0:ch 1-11, 1:ch 1-13, Refer to Ralink  
#define CSID_C_WLANAP_SSID			_CFG_WLANAP(0x0400)	//T=str, D="default", ssid
#define CSID_C_WLANAP_BASICRATE			_CFG_WLANAP(0x0500)	//T=u16, D=15, basic rate
#define CSID_C_WLANAP_SUPPORTRATE		_CFG_WLANAP(0x0600)	//T=u16, D=0xFFF, support rate
#define CSID_C_WLANAP_FRAGTHRESHOLD		_CFG_WLANAP(0x0700)	//T=u16, D=2346, Fragmentation Threshold
#define CSID_C_WLANAP_RTSCTSTHRESHOLD   	_CFG_WLANAP(0x0800)	//T=u16, D=2347, CTS/RTS Threshold
#define CSID_C_WLANAP_BEACONINTERVAL		_CFG_WLANAP(0x0900)	//T=u16, D=100, beacon interval
#define CSID_C_WLANAP_DTIMINTERVAL  		_CFG_WLANAP(0x0a00)	//T=u16, D=1, DTIM interval
#define CSID_C_WLANAP_TXPREAMPLE  		_CFG_WLANAP(0x0b00)	//T=u8, D=0, tx preamble 
#define CSID_C_WLANAP_WIRLESSMODE  		_CFG_WLANAP(0x0c00)	//T=u8, D=9, wireless mode
#define CSID_C_WLANAP_TXPOWER      		_CFG_WLANAP(0x0d00)	//T=u8, D=100, tx power level
#define CSID_C_WLANAP_HIDESSID     		_CFG_WLANAP(0x0e00)	//T=u8, D=0, hiden ssid
#define CSID_C_WLANAP_BGPROTECT    		_CFG_WLANAP(0x0f00)	//T=u8, D=0, B/G protection
#define CSID_C_WLANAP_SHORTSLOT    		_CFG_WLANAP(0x1000)	//T=u8, D=1, short slot
#define CSID_C_WLANAP_TXBURST    		_CFG_WLANAP(0x1100)	//T=u8, D=1, tx burst
#define CSID_C_WLANAP_PKTAGGREGATE 		_CFG_WLANAP(0x1200)	//T=u8, D=1, PktAggregate capable
#define CSID_C_WLANAP_AUTHMODE   		_CFG_WLANAP(0x1300)	//T=u8, D=0, authentication type
#define CSID_C_WLANAP_ENCRYPTYPE   		_CFG_WLANAP(0x1400)	//T=u8, D=0, Encryp Type
#define CSID_C_WLANAP_IEEE8021X   		_CFG_WLANAP(0x1500)	//T=u8, D=0, enable/disable 802_1x
#define CSID_C_WLANAP_DEFKEYID   		_CFG_WLANAP(0x1600)	//T=u8, D=1, default wep key id
#define CSID_C_WLANAP_KEY0TYPE   		_CFG_WLANAP(0x1700)	//T=u8, D=0, wep key 0 type
#define CSID_C_WLANAP_KEY0		   		_CFG_WLANAP(0x1800)	//T=str, D="1234567890", wep key 0 value
#define CSID_C_WLANAP_KEY1TYPE   		_CFG_WLANAP(0x1900)	//T=u8, D=0, wep key 1 type
#define CSID_C_WLANAP_KEY1		  		_CFG_WLANAP(0x1a00)	//T=str, D="1234567890", wep key 1 value
#define CSID_C_WLANAP_KEY2TYPE   		_CFG_WLANAP(0x1b00)	//T=u8, D=0, wep key 2 type
#define CSID_C_WLANAP_KEY2				_CFG_WLANAP(0x1c00)	//T=str, D="1234567890", wep key 2 value
#define CSID_C_WLANAP_KEY3TYPE   		_CFG_WLANAP(0x1d00)	//T=u8, D=0, wep key 3 type
#define CSID_C_WLANAP_KEY3		   		_CFG_WLANAP(0x1e00)	//T=str, D="1234567890", wep key 3 value
#define CSID_C_WLANAP_WPAPSK	   		_CFG_WLANAP(0x1f00)	//T=str, D="1234567890", wpa psk value
#define CSID_C_WLANAP_REKEYINTERVAL		_CFG_WLANAP(0x2000)	//T=u32, D=0, wpa rekey interval
#define CSID_C_WLANAP_IAPP_DISABLE		_CFG_WLANAP(0x2100)	//T=u8, D=0, IAPP
#define CSID_C_WLANAP_WIFI_SPECIFIC		_CFG_WLANAP(0x2101)	//T=u8, D=0, WiFi test only
#define CSID_C_WLANAP_MACCLONE			_CFG_WLANAP(0x2102)	//T=u8, D=0, MAC clone
#define CSID_C_WLANAP_NPROTECTION		_CFG_WLANAP(0x2103)	//T=u8, D=0, 11n protection
#define CSID_C_WLANAP_ACCESS			_CFG_WLANAP(0x2104)	//T=u8, D=0, 11n guess access
#define CSID_C_WLANAP_EXPIRETIME		_CFG_WLANAP(0x2105)	//T=u8, D=0, Client expire time, in seconds,
#define CSID_C_WLANAP_MIMO_TR_MODE     	_CFG_WLANAP(0x2106) //T=u32, D=0, 1: 1T2R, 2: 2T4R, 3: 2T2R, 4: 1T1R
#define CSID_C_WLANAP_REFUSE_CLIENT		_CFG_WLANAP(0x2107) //T=u32, D=0, 1:SSID0, 2:SSID1, 4:SSID2, 8:SSID3 Refuse client
#define CSID_C_WLANAP_CARRIERDETECT	_CFG_WLANAP(0x2206) //T=u8, D=1, Japan=1 others:0 
#define CSID_C_WLANAP_FIXEDTXMODE       _CFG_WLANAP(0x2207) //T=str, D="HT", 11b only:CCK, 11g only & 11b/g mixed:OFDM,11g/n mixed & 11b/g/n mixed & 11n only:HT
#define CSID_C_WLANAP_EXTCHANNEL		_CFG_WLANAP(0x2300)	//T=u8, D=7, Extension channel
#define CSID_C_WLANAP_REKEYMETHOD       _CFG_WLANAP(0x2400) //T=str, D="DISABLE", WPA series:TIME
#define CSID_C_WLANAP_REKEYMETHOD_NU       _CFG_WLANAP(0x2401) //T=u8, D=0, WPA series, 0:DISABLE, 1:TIME, 2: PACKET 
#define CSID_C_WLANAP_CUSTOMIZE_FLAG	_CFG_WLANAP(0x2500) //T=u32, D=0, Wireless setting Customize flag
#define CSID_C_WLANAP_CUSTOMIZE_NTC_SSID_24G	_CFG_WLANAP(0x2501) //T=str, D="NetComm Wireless", Wireless setting Customize flag
#define CSID_C_WLANAP_CUSTOMIZE_NTC_SSID_5G	_CFG_WLANAP(0x2502) //T=str, D="NetComm Wireless", Wireless setting Customize flag
#define CSID_C_WLANAP_CUSTOMIZE_SSID_MAC    _CFG_WLANAP(0x2503) //T=str, D="DIR506L", Wireless ssid with last 4 char of MAC
#define CSID_C_WLANAP_MAX_STA_NUM       _CFG_WLANAP(0x2600) //T=u32, D=32, Max station number
#define CSID_C_WLANAP_NOFORWARDING      _CFG_WLANAP(0x2700) //T=u8, D=0, Isolation
#define CSID_C_WLANAP_NOFORWARDINGBTNBSSID  _CFG_WLANAP(0x2800) //T=u8, D=0, Isolation between BSSID
#define CSID_C_WLANAP_FIXED_DATARATE    _CFG_WLANAP(0x2801) //T=u8, wlan fixed data rate
#define CSID_C_WLANAP_GREEN_AP          _CFG_WLANAP(0x2802) //T=u8, D=0, green ap
#define CSID_C_WLANAP_PWR_SAVING_DELAY  _CFG_WLANAP(0x2803) //T=u32, delay time to enter power saving mode when no client

#define CSID_C_WLANAP_IP				_CFG_WLANAP(0x2900)	//T=ipv4, SSID IP
#define CSID_C_WLANAP_MASK				_CFG_WLANAP(0x2910)	//T=ipv4, SSID netmask
#define CSID_C_WLANAP_DHCP_SERVER				_CFG_WLANAP(0x2920)	//T=str, DHCP server ip or domain name
#define CSID_C_WLANAP_HTTP_REDIRECT_ENABLE				_CFG_WLANAP(0x2930)	//T=u8, D=0, enable http redirect, yes=1, no=0
#define CSID_C_WLANAP_HTTPS_REDIRECT_ENABLE				_CFG_WLANAP(0x2940)	//T=u8, D=0, enable https redirect, yes=1, no=0
#define CSID_C_WLANAP_REDIRECT_URI				_CFG_WLANAP(0x2950)	//T=str, http redirect uri

#define CSID_C_WLANAP_ATE_CHIP_SEL		_CFG_WLANAP(0x3100) //T=u32, RT3883 or RT5392

#define CSID_C_WLANAP_DUALMODE_ENABLE		_CFG_WLANAP(0x3200) //T=u32, RT3883 or RT5392
#define CSID_C_WLANAP_DUALBAND				_CFG_WLANAP(0x3201) //T=u32, check use dual band for wps csid
#define CSID_C_WLANAP_8021X_STATICWEP_ENABLE _CFG_WLANAP(0x3300) //T=u8, D=0, enable/disable 8021x static WEP support

#define CSID_C_WLANAP_AUTOCHANNELSELECT    		_CFG_WLANAP(0x3400)	//T=u8, D=0, 0: Random /1: AP counter /2: FalseCCA
#define CSID_C_WLANAP_AUTOCHANNELSKIPLIST  		_CFG_WLANAP(0x3401)	//T=str, AutoChannelSkipList
#define CSID_C_WLANAP_PMKCACHEPERIOD  		_CFG_WLANAP(0x3500)	//T=u32, D=10, PMKCachePeriod

/*Radius Setting */
#define CSID_C_WLANAP_RADIUS_SRVIP	   	    _CFG_WLANAP(0x3000)	//T=ipv4, radius server ip
#define CSID_C_WLANAP_RADIUS_SRVPORT   		_CFG_WLANAP(0x3800)	//T=u16, D=1812, radius server port
#define CSID_C_WLANAP_RADIUS_SRVKEY   		_CFG_WLANAP(0x4000)	//T=str, radius server share key
#define CSID_C_WLANAP_RADIUS_MACAUTH        _CFG_WLANAP(0x4100) //T=u8, 0:disable 1:enable mac address authentication
#define CSID_C_WLANAP_RADIUS_AUTH_TIMEOUT   _CFG_WLANAP(0x4200) //T=u8, 0:disable 1:enable mac address authentication timeout
#define CSID_C_WLANAP_RADIUS_SS_TIMEOUT_INT _CFG_WLANAP(0x4201) //T=u32, D=0, session_timeout_interval
#define CSID_C_WLANAP_RADIUS_SECOND_SRVIP	_CFG_WLANAP(0x4300)	//T=ipv4, radius secondary server ip
#define CSID_C_WLANAP_RADIUS_SECOND_SRVPORT _CFG_WLANAP(0x4400)	//T=u16, D=1812, radius secondary server port
#define CSID_C_WLANAP_RADIUS_SECOND_SRVKEY  _CFG_WLANAP(0x4500)	//T=str, radius secondary server share key
#define CSID_C_WLANAP_RADIUS_MACAUTH_SRVIP	        _CFG_WLANAP(0x4600)	//T=ipv4, macauth radius server ip
#define CSID_C_WLANAP_RADIUS_MACAUTH_SRVPORT   	    _CFG_WLANAP(0x4700)	//T=u16, macauth radius server port
#define CSID_C_WLANAP_RADIUS_MACAUTH_SRVKEY   	    _CFG_WLANAP(0x4800)	//T=str, radius macauth server share key
#define CSID_C_WLANAP_RADIUS_MACAUTH_SECOND_SRVIP   _CFG_WLANAP(0x4900)	//T=ipv4, macauth radius secondary server ip
#define CSID_C_WLANAP_RADIUS_MACAUTH_SECOND_SRVPORT _CFG_WLANAP(0x4a00)	//T=u16, macauth radius secondary server port
#define CSID_C_WLANAP_RADIUS_MACAUTH_SECOND_SRVKEY  _CFG_WLANAP(0x4b00)	//T=str, macauth radius secondary server share key
#define CSID_C_WLANAP_RADIUS_MACAUTH_ENABLE         _CFG_WLANAP(0x4c00)	//T=u8, enable/disable RADIUS MAC AUTH
#define CSID_C_WLANAP_RADIUS_EAPIFNAME              _CFG_WLANAP(0x4d00) //T=str, EAPifname
#define CSID_C_WLANAP_RADIUS_PREAUTHIFNAME          _CFG_WLANAP(0x4e00) //T=str, PreAuthifname

// define total support 8 radius server

/*WDS Setting */
#define CSID_C_WLANAP_WDSENABLE   		_CFG_WLANAP(0x5000)	//T=u8, D=0, enable/disable wds function
#define CSID_C_WLANAP_WDSMACLIST		_CFG_WLANAP(0x5001) //T=str, the collection of all ap mac address, 
#define CSID_C_WLANAP_WDSPHYMODELIST   	_CFG_WLANAP(0x5002) //T=str, D="HTMIX;HTMIX;HTMIX;HTMIX", wds phymode list 
#define CSID_C_WLANAP_WDS_CONNECTED_MACLIST _CFG_WLANAP(0x5003) //T=str, all connected mac address, separated by ";"
#define CSID_C_WLANAP_WDSMACNUM         _CFG_WLANAP(0x5004) //T=u8, D=0, number of mac address
#define CSID_C_WLANAP_WDSENCRYP   		_CFG_WLANAP(0x5100)	//T=u8, wds encryption type
#define CSID_C_WLANAP_WDSKEY    		_CFG_WLANAP(0x5200)	//T=str, wds encryption key
#define CSID_C_WLANAP_WDSAP     		_CFG_WLANAP(0x5800)	//T=mac, wds ap mac address
// define total support 8 wds ap

/*WMM Setting */
#define CSID_C_WLANAP_WMMENABLE    		_CFG_WLANAP(0x6000)	//T=u8, D=0, enable/disable WMM
#define CSID_C_WLANAP_APAIFSN     		_CFG_WLANAP(0x6100)	//T=u32, APAifsn
#define CSID_C_WLANAP_APCWMIN    		_CFG_WLANAP(0x6200)	//T=u32, APCwmin
#define CSID_C_WLANAP_APCWMAX    		_CFG_WLANAP(0x6300)	//T=u32, APCwmax
#define CSID_C_WLANAP_APACM    			_CFG_WLANAP(0x6400)	//T=u32, APACM
#define CSID_C_WLANAP_APTXOP    		_CFG_WLANAP(0x6500)	//T=u32, APTxop
#define CSID_C_WLANAP_BSSAIFSN     		_CFG_WLANAP(0x6600)	//T=u32, BSSAifsn
#define CSID_C_WLANAP_BSSCWMIN    		_CFG_WLANAP(0x6700)	//T=u32, BSSCwmin
#define CSID_C_WLANAP_BSSCWMAX    		_CFG_WLANAP(0x6800)	//T=u32, BSSCwmax
#define CSID_C_WLANAP_BSSACM    		_CFG_WLANAP(0x6900)	//T=u32, BSSACM
#define CSID_C_WLANAP_BSSTXOP    		_CFG_WLANAP(0x6a00)	//T=u32, BSSTxop
#define CSID_C_WLANAP_ACKPOLICY    		_CFG_WLANAP(0x6b00)	//T=u32, AckPolicy
#define CSID_C_WLANAP_WMMCAPABLE    		_CFG_WLANAP(0x6c00)	//T=u32, D=1, WmmCapable
#define CSID_C_WLANAP_DLSCAPABLE    		_CFG_WLANAP(0x6d00)	//T=u32, D=0, DlsCapable
#define CSID_C_WLANAP_APSDCAPABLE    		_CFG_WLANAP(0x6e00)	//T=u32, D=0, ApsdCapable

/* RADIUS ACCOUNT */
#define CSID_C_WLANAP_RADIUS_ACCTAUTH_ENABLE         _CFG_WLANAP(0x7000)	//T=u8, enable/disable RADIUS ACCT AUTH
#define CSID_C_WLANAP_RADIUS_ACCTAUTH_SRVIP	         _CFG_WLANAP(0x7100)	//T=ipv4, acctauth radius server ip
#define CSID_C_WLANAP_RADIUS_ACCTAUTH_SRVPORT   	 _CFG_WLANAP(0x7200)	//T=u16, acctauth radius server port
#define CSID_C_WLANAP_RADIUS_ACCTAUTH_SRVKEY   	     _CFG_WLANAP(0x7300)	//T=str, radius acctauth server share key
#define CSID_C_WLANAP_RADIUS_ACCTAUTH_SECOND_SRVIP   _CFG_WLANAP(0x7400)	//T=ipv4, acctauth radius secondary server ip
#define CSID_C_WLANAP_RADIUS_ACCTAUTH_SECOND_SRVPORT _CFG_WLANAP(0x7500)	//T=u16, acctauth radius secondary server port
#define CSID_C_WLANAP_RADIUS_ACCTAUTH_SECOND_SRVKEY  _CFG_WLANAP(0x7600)	//T=str, acctauth radius secondary server share key
#define CSID_C_WLANAP_RADIUS_ACCTAUTH_INTERIM_UPDATE_INTERVAL  _CFG_WLANAP(0x7700)  //T=u32, D=300, radius acctauth interim update interval


/*HT parameter Setting */
#define CSID_C_WLANAP_HT_HTC    		_CFG_WLANAP(0x8000)	//T=u8, HT_HTC
#define CSID_C_WLANAP_HT_RDG    		_CFG_WLANAP(0x8100)	//T=u8, D=1, HT_RDG
#define CSID_C_WLANAP_HT_EXTCHA    		_CFG_WLANAP(0x8200)	//T=u8, HT_EXTCHA
#define CSID_C_WLANAP_HT_LINKADAPT 		_CFG_WLANAP(0x8300)	//T=u8, HT_LinkAdapt
#define CSID_C_WLANAP_HT_OPMODE    		_CFG_WLANAP(0x8400)	//T=u8, HT_OpMode
#define CSID_C_WLANAP_HT_MPDUDENSITY		_CFG_WLANAP(0x8500)	//T=u8, D=5, HT_MpduDensity
#define CSID_C_WLANAP_HT_BW     		_CFG_WLANAP(0x8600)	//T=u8, D=1, HT_BW
#define CSID_C_WLANAP_HT_AUTOBA    		_CFG_WLANAP(0x8700)	//T=u8, D=1, HT_AutoBA
#define CSID_C_WLANAP_HT_AMSDU    		_CFG_WLANAP(0x8800)	//T=u8, HT_AMSDU
#define CSID_C_WLANAP_HT_BAWINSZ   		_CFG_WLANAP(0x8900)	//T=u8, D=64, HT_BAWinSize
#define CSID_C_WLANAP_HT_GI    			_CFG_WLANAP(0x8a00)	//T=u8, D=1, HT_GI
#define CSID_C_WLANAP_HT_STBC    		_CFG_WLANAP(0x8b00)	//T=u8, D=1, HT_STBC
#define CSID_C_WLANAP_HT_MCS    		_CFG_WLANAP(0x8c00)	//T=str, D=33, HT_MCS
#define CSID_C_WLANAP_HT_BLOCKRELAY 	_CFG_WLANAP(0x8d00)	//T=u8, D=0, Block Relay
#define CSID_C_WLANAP_HT_TXSTREAM 	_CFG_WLANAP(0x8e00)	//T=u8, D=1, Tx Stream
#define CSID_C_WLANAP_HT_RXSTREAM 	_CFG_WLANAP(0x8f00)	//T=u8, D=1, Rx Stream
#define CSID_C_WLANAP_HT_BSSCOEXISTENCE _CFG_WLANAP(0x8f01) //T=u8, D=0, HT_BSSCoexistence
#define CSID_C_WLANAP_HT_BSSCOEXAPCNTTHR _CFG_WLANAP(0x8f02) //T=u8, D=0, HT_BSSCoexAPCntThr

#define CSID_C_WLANAP_VHT_BW     		_CFG_WLANAP(0x8f03)	//T=u8, D=1, VHT_BW

/* DISCARD STA WHEN LOW RSSI */
#define CSID_C_WLANAP_PROBEASSOCREQ_ENABLE  		_CFG_WLANAP(0x9000)	//T=u8, D=0, PROBEASSOCREQ_ENABLE
#define CSID_C_WLANAP_PROBEASSOCREQ_RSSITHRESHOLD  		_CFG_WLANAP(0x9100)	//T=s8, D=-75,  PROBEASSOCREQ_RSSITHRESHOLD
#define CSID_C_WLANAP_DATALOWRSSIKICK_ENABLE  		_CFG_WLANAP(0x9200)	//T=u8, D=0, DATALOWRSSIKICK_ENABLE
#define CSID_C_WLANAP_DATALOWRSSIKICK_RSSITHRESHOLD_LOW  		_CFG_WLANAP(0x9300)	//T=s8, D=-75, DATALOWRSSIKICK_RSSITHRESHOLD_LOW
#define CSID_C_WLANAP_DATALOWRSSIKICK_RSSITHRESHOLD_HIGH  		_CFG_WLANAP(0x9400)	//T=s8, D=-75, DATALOWRSSIKICK_RSSITHRESHOLD_HIGH
#define CSID_C_WLANAP_DATALOWRSSIKICK_TIMESSENDDISASSO  		_CFG_WLANAP(0x9500)	//T=u32, D=3, DATALOWRSSIKICK_TIMESSENDDISASSO
#define CSID_C_WLANAP_DATALOWRSSIKICK_REJDURSENDDISASSO  		_CFG_WLANAP(0x9600)	//T=u32, D=5, DATALOWRSSIKICK_REJDURSENDDISASSO
#define CSID_C_WLANAP_DATALOWRSSIKICK_WAITPERIOD  		_CFG_WLANAP(0x9700)	//T=u32, D=60, DATALOWRSSIKICK_WAITPERIOD
#define CSID_C_WLANAP_NOWANSTOPWIFI_ENABLE  		_CFG_WLANAP(0x9800)	//T=u8, D=0, NOWANSTOPWIFI_ENABLE
#define CSID_C_WLANAP_NOWANSTOPWIFI_FAILCOUNT  	   _CFG_WLANAP(0x9900)	//T=u32,  NOWANSTOPWIFI_FAILCOUNT
#define CSID_C_WLANAP_SPECIFIC_RATE_ENABLE  	   _CFG_WLANAP(0x9a00)	//T=u8, D=0, SPECIFIC_RATE_ENABLE
#define CSID_C_WLANAP_SPECIFIC_RATE_TX  	   _CFG_WLANAP(0x9b00)	//T=str,  SPECIFIC_RATE_TX
#define CSID_C_WLANAP_SPECIFIC_RATE_BEACON  	   _CFG_WLANAP(0x9c00)	//T=str,  SPECIFIC_RATE_BEACON
#define CSID_C_WLANAP_SPECIFIC_RATE_MULTICAST  	   _CFG_WLANAP(0x9d00)	//T=str,  SPECIFIC_RATE_MULTICAST

/*WPS Setting */
#define CSID_C_WLANAP_WPS_ENABLE   		_CFG_WLANAP(0xa000)	//T=u8, D=0, enable/disable wps function
#define CSID_C_WLANAP_WPS_AP_PINCODE	_CFG_WLANAP(0xa001) //T=str,WPS AP's PIN Code
#define CSID_C_WLANAP_WPS_CONFMODE		_CFG_WLANAP(0xa100) //T=u8, Set WPS function, Enrollee, Proxy, Registrar. bitwise
#define CSID_C_WLANAP_WPS_CONFSTATUS	_CFG_WLANAP(0xa200) //T=u8, D=1, Set WPS AP Simple Config State, Un-Configured/Configured
#define CSID_C_WLANAP_WPS_MODE  		_CFG_WLANAP(0xa300) //T=u8, D=1, configured metheds, PIN/PBC  
#define CSID_C_WLANAP_ATE_ENABLE		_CFG_WLANAP(0xa301) //T=u8, D=0,0:ATE diable,1:ATE enable
#define CSID_C_WLANAP_WSCSECURITYMODE   _CFG_WLANAP(0xa302) //T=u8, D=0, WscSecurityMode
#define CSID_C_WLANAP_WSCCONFMETHODS    _CFG_WLANAP(0xa303) //T=str, D="268C", WscConfMethods
#define CSID_C_WLANAP_WPS_PREFIX		_CFG_WLANAP(0xa304) //T=str, WPS SSID prefix, can chagne by custmer define.
#define CSID_C_WLANAP_WPS_DISABLE_PIN   _CFG_WLANAP(0xa305) //T=u8, WPS disable PIN
#define CSID_C_WLANAP_WPS_SET_DISABLE_PIN _CFG_WLANAP(0xa306) //T=u8, Disable PIN when AP changed from unconfigured to configured
#define CSID_C_WLANAP_WSCV2SUPPORT      _CFG_WLANAP(0xa307) //T=u8, D=0, WscV2Support
#define CSID_C_WLANAP_WSCPINSETLOCK     _CFG_WLANAP(0xa308) //T=u8, D=0, WscPINSetupLock

/*TXPOWER*/
#define CSID_C_WLANAP_ATE_TXPOWER		_CFG_WLANAP(0xa400) //T=u32, Antenna 1 Tx power level(0.5dBm/unit)

/*Default PinConde*/
#define CSID_C_WLANAP_WPS_AP_DEFAULT_PINCODE		_CFG_WLANAP(0xa402) //T=str, WPS AP's Default PIN Code

/* Country Region Code from EEPROM */
#define CSID_C_WLANAP_AREACODE_EEPROM   _CFG_WLANAP(0xa403) //T=u32, Area Code from EEPROM for 2.4G, Refer to Ralink
#define CSID_C_WLANAP_AREACODE_EEPROM2  _CFG_WLANAP(0xa404) //T=u32, Area Code from EEPROM for 5G, Refer to Ralink

/* Default Preshare key */
#define CSID_C_WLANAP_PRESHAREKEY_FLAG		_CFG_WLANAP(0xa405) //T=u32, Default Preshare key flag

//WLAN Station list log
#define CSID_C_WLANAP_STA_LIST_MAC_LOG		_CFG_WLANAP(0xa406) //T=mac, 
#define CSID_C_WLANAP_STA_LIST_NAME_LOG		_CFG_WLANAP(0xa506) //T=str, 
#define CSID_C_WLANAP_STA_LIST_IP_LOG		_CFG_WLANAP(0xa606) //T=ipv4,
#define CSID_C_WLANAP_STA_LIST_IPV6_IP_LOG	_CFG_WLANAP(0xa706) //T=str, 
#define CSID_C_WLANAP_STA_LIST_VENDOR_NAME	_CFG_WLANAP(0xa806) //T=str, 

#define CSID_C_WLANAP_STA_LIST1_MAC_LOG		_CFG_WLANAP(0xa906) //T=mac, 
#define CSID_C_WLANAP_STA_LIST1_NAME_LOG	_CFG_WLANAP(0xaa06) //T=str, 
#define CSID_C_WLANAP_STA_LIST1_IP_LOG		_CFG_WLANAP(0xab06) //T=ipv4,
#define CSID_C_WLANAP_STA_LIST1_IPV6_IP_LOG	_CFG_WLANAP(0xac06) //T=str, 
#define CSID_C_WLANAP_STA_LIST1_VENDOR_NAME	_CFG_WLANAP(0xad06) //T=str, 

//HOTSPOT 2.0 multi SSID ==>b000~b00F
#define CSID_C_WLANAP_HOTSPOT_APP_ENABLE	_CFG_WLANAP(0xb000) //T=u8, D=0, 0:Disable,1:enable

/*NAI Realm list max 4*/
#define CSID_C_WLANAP_NAI_LIST_ENABLE		_CFG_WLANAP(0xb010) //T=u8, D=0, 0:Disable,1:enable

#define CSID_C_WLANAP_NAI_REALM_DATA0		_CFG_WLANAP(0xb020) //T=str,NAI Realm data
#define CSID_C_WLANAP_REALM_EAP_METHOD0		_CFG_WLANAP(0xb030) //T=u8, D=0,0:TTLS 1:TLS

#define CSID_C_WLANAP_NAI_REALM_DATA1		_CFG_WLANAP(0xb040) //T=str,NAI Realm data
#define CSID_C_WLANAP_REALM_EAP_METHOD1		_CFG_WLANAP(0xb050) //T=u8, D=1,0:TTLS 1:TLS

#define CSID_C_WLANAP_NAI_REALM_DATA2		_CFG_WLANAP(0xb060) //T=str,NAI Realm data
#define CSID_C_WLANAP_REALM_EAP_METHOD2		_CFG_WLANAP(0xb070) //T=u8, D=0,0:TTLS 1:TLS

#define CSID_C_WLANAP_NAI_REALM_DATA3		_CFG_WLANAP(0xb080) //T=str,NAI Realm data
#define CSID_C_WLANAP_REALM_EAP_METHOD3		_CFG_WLANAP(0xb090) //T=u8, D=0,0:TTLS 1:TLS

/*COUNTRY_CODE*/
#define CSID_C_WLANAP_COUNTRY_CODE			_CFG_WLANAP(0xb400) //T=str, D="TW"

/*Max Client limit */
#define CSID_C_WLANAP_MAX_CLIENT_LIMIT	_CFG_WLANAP(0xb500) //T=u8, D=0
#define CSID_C_WLANAP_MAX_CLIENT_NUM	_CFG_WLANAP(0xb600) //T=u8, D=0

//WLAN Station list log
#define CSID_C_WLANAP_STA_LIST_CONNECT_CNT      _CFG_WLANAP(0xb700) //T=u32, D=0
#define CSID_C_WLANAP_STA_LIST1_CONNECT_CNT     _CFG_WLANAP(0xb800) //T=u32, D=0

#define CSID_C_WLANAP_DENY_ALL     _CFG_WLANAP(0xb900) //T=u8, D=0, allow/deny all wireless clients. 0:allow 1:deny

/*AP IDS*/
#define CSID_C_WLANAP_AP_WIDS_ENABLE		_CFG_WLANAP(0xba00) //T=u32, D=0

/* AP ASSOCIATE */
#define CSID_C_WLANAP_AP_MAXASSOCITIMEGAP		_CFG_WLANAP(0xba03) //T=u32, D=0
#define CSID_C_WLANAP_AP_MAXASSOCIREQCOUNTER	_CFG_WLANAP(0xba04) //T=u32, D=0
#define CSID_C_WLANAP_AP_BANDSTEERING_ENABLE	_CFG_WLANAP(0xba06) //T=u32, D=0

/*EDCCA function*/
#define CSID_C_WLANAP_EDCCA_AP_STA_TH		_CFG_WLANAP(0xbb00) //T=u32, D=20, for AP connected client's count
#define CSID_C_WLANAP_EDCCA_AP_AP_TH		_CFG_WLANAP(0xbb01) //T=u32, D=20, for APs found in working channel
#define CSID_C_WLANAP_EDCCA_FALSE_CCA_TH	_CFG_WLANAP(0xbb02) //T=u32, D=3000, false CCA threshold to disable EDCCA
#define CSID_C_WLANAP_EDCCA_SUPPORT         _CFG_WLANAP(0xbb03) //T=u32, D=0, for supporting EDCCA, 0: disable, 1: enable

/*ANTENNA*/
#define CSID_C_WLANAP_ANTENNA_SW			_CFG_WLANAP(0xbb10) //T=u32, D=0

/* Status Items */
#define CSID_S_WLANAP_HOMECOM_ALTERED	_STA_WLANAP(0x0001) //T=u8, homecom wlan altered.
#define CSID_S_WLANAP_ALTERED			_STA_WLANAP(0x0002) //T=u8, ui altered
#define CSID_S_WLANAP_ATE_START			_STA_WLANAP(0x0003) //T=u32, D=0,0:stop,1:start, ate start
#define CSID_S_WLANAP_IF_STATUS			_STA_WLANAP(0x0004) //T=u8, D=0, 0:off ,1:on, record current WLAN's status
#define CSID_S_WLANAP_WIRELESS_MODE_ALTERED    _STA_WLANAP(0x0005) //T=u8, Wireless Mode altered.

//For MutiSSID
//#define CSID_S_WLANAP_SSID_LIST		_STA_WLANAP(0x0006) //T=str, D="AP1;AP2;AP3;AP4;AP5;AP6;AP7;AP8", MutiBSSID
#define CSID_S_WLANAP_AUTHMODE_LIST		_STA_WLANAP(0x0007) //T=str, D="OPEN;OPEN;OPEN;OPEN;OPEN;OPEN;OPEN;OPEN", Authentication
#define CSID_S_WLANAP_ENCRYPTYPE_LIST	_STA_WLANAP(0x0008) //T=str, D="NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE", Encryption mathod
#define CSID_S_WLANAP_DEFKEYID_LIST		_STA_WLANAP(0x000A) //T=str, D="1;1;1;1;1;1;1;1", list of default wep key id
#define CSID_S_WLANAP_KEY0TYPE_LIST     _STA_WLANAP(0x000B) //T=str, D="0;0;0;0;0;0;0;0", list of wep key0 type 
#define CSID_S_WLANAP_KEY1TYPE_LIST		_STA_WLANAP(0x000D) //T=str, D="0;0;0;0;0;0;0;0", list of wep key1 type         
#define CSID_S_WLANAP_KEY2TYPE_LIST		_STA_WLANAP(0x000F) //T=str, D="0;0;0;0;0;0;0;0", list of wep key2 type
#define CSID_S_WLANAP_KEY3TYPE_LIST		_STA_WLANAP(0x0012) //T=str, D="0;0;0;0;0;0;0;0", list of wep key3 type
#define CSID_S_WLANAP_IEEE8021X_LIST	_STA_WLANAP(0x0014) //T=str, D="0;0;0;0;0;0;0;0", list of whether enabling 8021X function in mutissid
#define CSID_S_WLANAP_HIDESSID_LIST		_STA_WLANAP(0x0015) //T=str, D="0;0;0;0;0;0;0;0", list of whether enabling HIDESSID. 
#define CSID_S_WLANAP_RADIUS_SRVIP_LIST		_STA_WLANAP(0x0016)	//T=str, list of radius server ip 			 
#define CSID_S_WLANAP_RADIUS_SRVPORT_LIST	_STA_WLANAP(0x0017)	//T=str, list of radius port
#define CSID_S_WLANAP_RADIUS_SRVKEY_LIST	_STA_WLANAP(0x0018) //T=str, list of radius key
#define CSID_S_WLANAP_8021X_STATICWEP_LIST	_STA_WLANAP(0x0019) //T=str, D="0;0;0;0;0;0;0;0", list of whether enabling 8021X static wep function in mutissid
#define CSID_S_WLANAP_NOFORWARDING_LIST 	_STA_WLANAP(0x0020) //T=str, D="0;0;0;0;0;0;0;0", list of whether enabling noforwarding in mutissid
#define CSID_S_WLANAP_RADIUS_SECOND_SRVIP_LIST		_STA_WLANAP(0x0021)	//T=str, list of secondary radius server ip 			 
#define CSID_S_WLANAP_RADIUS_SECOND_SRVPORT_LIST	_STA_WLANAP(0x0022)	//T=str, list of secondary radius port
#define CSID_S_WLANAP_RADIUS_SECOND_SRVKEY_LIST	    _STA_WLANAP(0x0023) //T=str, list of secondary radius key
#define CSID_S_WLANAP_RADIUS_MACAUTH_SRVIP_LIST		_STA_WLANAP(0x0024)	//T=str, list of macauth radius server ip 			 
#define CSID_S_WLANAP_RADIUS_MACAUTH_SRVPORT_LIST	_STA_WLANAP(0x0025)	//T=str, list of macauth radius port
#define CSID_S_WLANAP_RADIUS_MACAUTH_SRVKEY_LIST	_STA_WLANAP(0x0026) //T=str, list of macauth radius key
#define CSID_S_WLANAP_RADIUS_MACAUTH_SECOND_SRVIP_LIST	    _STA_WLANAP(0x0027)	//T=str, list of macauth secondary radius server ip 			 
#define CSID_S_WLANAP_RADIUS_MACAUTH_SECOND_SRVPORT_LIST	_STA_WLANAP(0x0028)	//T=str, list of macauth secondary radius port
#define CSID_S_WLANAP_RADIUS_MACAUTH_SECOND_SRVKEY_LIST	    _STA_WLANAP(0x0029) //T=str, list of macauth secondary radius key
#define CSID_S_WLANAP_RADIUS_EAPIFNAME_LIST	        _STA_WLANAP(0x0030) //T=str, D="br0", list of EAPifname
#define CSID_S_WLANAP_RADIUS_PREAUTHIFNAME_LIST     _STA_WLANAP(0x0031) //T=str, D="br0", list of PreAuthifname
#define CSID_S_WLANAP_REFUSE_CLIENT _STA_WLANAP(0x0032) //T=u32, D=0, 1:SSID0, 2:SSID1, 4:SSID2, 8:SSID3 Refuse client
#define CSID_S_WLANAP_REFUSE_CLIENT_ALTERED		_STA_WLANAP(0x0033) //T=u8,D=1, refuse altered
#define CSID_S_WLANAP_RADIUS_MACAUTH_ENABLE_LIST     _STA_WLANAP(0x0034) //T=str, list of macauth radius enable  
#define CSID_S_WLANAP_RADIUS_ACCTAUTH_SRVIP_LIST			_STA_WLANAP(0x0035)	//T=str, list of acctauth radius server ip 			 
#define CSID_S_WLANAP_RADIUS_ACCTAUTH_SRVPORT_LIST			_STA_WLANAP(0x0036)	//T=str, list of acctauth radius port
#define CSID_S_WLANAP_RADIUS_ACCTAUTH_SRVKEY_LIST			_STA_WLANAP(0x0037) //T=str, list of acctauth radius key
#define CSID_S_WLANAP_RADIUS_ACCTAUTH_SECOND_SRVIP_LIST	    _STA_WLANAP(0x0038)	//T=str, list of acctauth secondary radius server ip 			 
#define CSID_S_WLANAP_RADIUS_ACCTAUTH_SECOND_SRVPORT_LIST	_STA_WLANAP(0x0039)	//T=str, list of acctauth secondary radius port
#define CSID_S_WLANAP_RADIUS_ACCTAUTH_SECOND_SRVKEY_LIST	_STA_WLANAP(0x003a) //T=str, list of acctauth secondary radius key
#define CSID_S_WLANAP_RADIUS_ACCTAUTH_ENABLE_LIST           _STA_WLANAP(0x003b) //T=str, list of acctauth radius enable     
#define CSID_S_WLANAP_RADIUS_ACCTAUTH_INTERIM_UPDATE_INTERVAL_LIST  _STA_WLANAP(0x003c)  //T=str, list of radius acctauth interim update interval
#define CSID_S_WLANAP_PMKCACHEPERIOD_LIST  		_STA_WLANAP(0x003d)	//T=str, list of PMKCachePeriod
#define CSID_S_WLANAP_PROBEASSOCREQ_ENABLE_LIST  		_STA_WLANAP(0x003e)	//T=str, list of PROBEASSOCREQ_ENABLE
#define CSID_S_WLANAP_PROBEASSOCREQ_RSSITHRESHOLD_LIST  		_STA_WLANAP(0x003f)	//T=str, list of PROBEASSOCREQ_RSSITHRESHOLD
#define CSID_S_WLANAP_DATALOWRSSIKICK_ENABLE_LIST  		_STA_WLANAP(0x0040)	//T=str, list of DATALOWRSSIKICK_ENABLE
#define CSID_S_WLANAP_DATALOWRSSIKICK_RSSITHRESHOLD_LOW_LIST  		_STA_WLANAP(0x0041)	//T=str, list of DATALOWRSSIKICK_RSSITHRESHOLD_LOW
#define CSID_S_WLANAP_DATALOWRSSIKICK_RSSITHRESHOLD_HIGH_LIST  		_STA_WLANAP(0x0042)	//T=str, list of DATALOWRSSIKICK_RSSITHRESHOLD_HIGH
#define CSID_S_WLANAP_DATALOWRSSIKICK_TIMESSENDDISASSO_LIST  		_STA_WLANAP(0x0043)	//T=str, list of DATALOWRSSIKICK_TIMESSENDDISASSO
#define CSID_S_WLANAP_DATALOWRSSIKICK_REJDURSENDDISASSO_LIST  		_STA_WLANAP(0x0044)	//T=str, list of DATALOWRSSIKICK_REJDURSENDDISASSO
#define CSID_S_WLANAP_DATALOWRSSIKICK_WAITPERIOD_LIST  		_STA_WLANAP(0x0045)	//T=str, list of DATALOWRSSIKICK_WAITPERIOD
#define CSID_S_WLANAP_SPECIFIC_RATE_ENABLE_LIST  		_STA_WLANAP(0x0046)	//T=str, list of SPECIFIC_RATE_ENABLE

// WPS Status 
#define CSID_S_WLANAP_WPS_ENROLLEE_PINCODE	_STA_WLANAP(0x0100) //T=str, WPS, Enrollee's PIN Code
#define CSID_S_WLANAP_WPS_TRIGGER			_STA_WLANAP(0x0101) //T=u8, WPS, Trigger AP to do simple config.
#define CSID_S_WLANAP_WPS_ALTERED			_STA_WLANAP(0x0102) //T=u8, WPS, ui altered
#define CSID_S_WLANAP_WPS_STATUS			_STA_WLANAP(0x0103) //T=u8, D=0, WPS configured status
#define CSID_S_WLANAP_WPS_AP_PINCODE		_STA_WLANAP(0x0104) //T=str, WPS AP's PIN Code
#define CSID_S_WLANAP_WPS_AP_PUSH			_STA_WLANAP(0x0105) //T=u8, WPS, Push Button Enable/Disable
#define CSID_S_WLANAP_WPS_ACTION_INTERVAL	_STA_WLANAP(0x0106)	//T=u32, D=30,WPS Action Time Interval
#define CSID_S_WLANAP_WPS_ACTION_RUNNING    _STA_WLANAP(0x0107)	//T=u8, D=0, WPS action is running or not
#define CSID_S_WLANAP_WPS_ACTION_LED        _STA_WLANAP(0x0108)	//T=u8, D=0, WPS action is running or not for led

#define CSID_S_WLANAP_WPS_DEF_AUTHMODE		_STA_WLANAP(0x0110) //T=u32, Default WPS AuthMode
#define CSID_S_WLANAP_WPS_DEF_ENCRYPTYPE	_STA_WLANAP(0x0111) //T=u32, Default WPS ENCRYPTYPE
#define CSID_S_WLANAP_WPS_DEF_WPAPSK		_STA_WLANAP(0x0112) //T=str, Default WPS WPAPSK

#define CSID_S_WLANAP_WPS_WSC_MANUFACTURE	_STA_WLANAP(0x0113) //T=str, D="Amit", WSC UPnP Manufacture
#define CSID_S_WLANAP_WPS_WSC_MODEL_NAME	_STA_WLANAP(0x0114) //T=str, D="3G Router", WSC UPnP Model Name
#define CSID_S_WLANAP_WPS_WSC_DEVICE_NAME	_STA_WLANAP(0x0115) //T=str, D="WBR-0001", WSC UPnP Device Name 
#define CSID_S_WLANAP_WPS_WSC_MODEL_NUMBER	_STA_WLANAP(0x0116) //T=str, D="WBR-0001", WSC UPnP Model Numer

#define CSID_S_WLANAP_PID		_STA_WLANAP(0x0117) //T=u32, 802.1X PID
#define CSID_S_WLANAP_WLMODE_CNT_STATUS     _STA_WLANAP(0x0118) //T=u32, wireless mode DHCP connect status
#define CSID_S_WLANAP_WSCSETUPLOCK_STATUS	_STA_WLANAP(0x0119) //T=u8, D=0, WscSetupLock status

/* MBSS Altered 16 items are reserved */
#define CSID_S_WLANAP_MBSS_ALTERED		_STA_WLANAP(0x0200) //T=u8, mbss altered

/* Wireless Client-List, 256 items are reserved */
#define CSID_S_WLANAP_CLILIST_MAC			_STA_WLANAP(0x1000)	//T=mac, The MAC of the Client in Client List

/* Ralink ATE Command */
#define CSID_S_WLANAP_ATE_DATARATE		_STA_WLANAP(0x2000) //T=u32, D=11,Date rate		
#define CSID_S_WLANAP_ATE_CERTIFICATIONMODE 	_STA_WLANAP(0x2100) //T=u32, D=0,0:Normal,1:Cont.Tx,2:Cont. Rx,3:Tx carrier
#define CSID_S_WLANAP_ATE_CHANNEL 		_STA_WLANAP(0x2200) //T=u32, D=6, channel
#define CSID_S_WLANAP_ATE_WIRELESSMODE 		_STA_WLANAP(0x2300) //T=u32,D=1,0:b only,1:g only,2:bgn mixed,3:n only,wireless mode
#define CSID_S_WLANAP_ATE_TXPOWER		_STA_WLANAP(0x2400) //T=u32, Antenna 1 Tx power level(0.5dBm/unit)
#define CSID_S_WLANAP_ATE_BW 			_STA_WLANAP(0x2500) //T=u32,D=0,0:20MHz,1:40MHz, BandWidth
#define CSID_S_WLANAP_ATE_Antenna		_STA_WLANAP(0x2600) //T=u32, D=0, 0:All, 1:Antenna 1, 2:Antenna 2 
#define CSID_S_WLANAP_ATE_TXPOWER1		_STA_WLANAP(0x2700) //T=u32, Antenna 2 Tx power level(0.5dBm/unit)
#define CSID_S_WLANAP_ATE_TXPOWER2		_STA_WLANAP(0x2701) //T=u32, Antenna 2 Tx power level(0.5dBm/unit)
#define CSID_S_WLANAP_ATE_REPEATER		_STA_WLANAP(0x2702) //T=u32, Repeater count
#define CSID_S_WLANAP_ATE_MAC    			_STA_WLANAP(0x2703)	//T=mac, ated ap mac address
#define CSID_S_WLANAP_ATE_IRCONT   			_STA_WLANAP(0x2704)	//T=u8, infrared continue mode
#define CSID_S_WLANAP_ATE_IRRXDATA 			_STA_WLANAP(0x2705)	//T=str, infrared received data
#define CSID_S_WLANAP_ATE_IRRX_RUNNING		_STA_WLANAP(0x2706)	//T=u8, irrx running

#define CSID_S_WLANAP_CLILIST_MODE          _STA_WLANAP(0x2800) //T=str, The Mode of the Client in Client List,1:B mode 2:G mode 3:N mode
#define CSID_S_WLANAP_CLILIST_RATE          _STA_WLANAP(0x2900) //T=u32, The Rate of the Client in Client List
#define CSID_S_WLANAP_CLILIST_IP            _STA_WLANAP(0x3000) //T=ipv4, The Rate of the Client in Client List
#define CSID_S_WLANAP_CLILIST_SSID          _STA_WLANAP(0x4000) //T=str, The Rate of the Client in Client List

#define CSID_S_WLANAP_DUALMODE_TRIGGER          _STA_WLANAP(0x5000) //T=u32, Dual mode trigger
#define CSID_S_WLANAP_DUALMODE_INIT          		_STA_WLANAP(0x5001) //T=u32, Dual mode init
#define CSID_S_WLANAP_DUALMODE_STATUS						_STA_WLANAP(0x5002)	//T=u32, Dual mode satellite status
//#define CSID_S_WLANAP_APCLIENT_LINK_STATUS		_STA_WLANAP(0x5003)	//T=u32, apcli mode link status status

/* STAINFO */
#define CSID_S_WLANAP_STAINFO_MAC           	_STA_WLANAP(0x6000) //T=mac, station MAC
#define CSID_S_WLANAP_STAINFO_AID           	_STA_WLANAP(0x6100) //T=u8, station AID
#define CSID_S_WLANAP_STAINFO_BSS           	_STA_WLANAP(0x6200) //T=u8, station BSS
#define CSID_S_WLANAP_STAINFO_PSM           	_STA_WLANAP(0x6300) //T=u8, station PSM
#define CSID_S_WLANAP_STAINFO_WMM           	_STA_WLANAP(0x6400) //T=u8, station WMM
#define CSID_S_WLANAP_STAINFO_MIMOPS        	_STA_WLANAP(0x6500) //T=u8, station MIMOPS
#define CSID_S_WLANAP_STAINFO_RSSI0         	_STA_WLANAP(0x6600) //T=u32, station RSSI0
#define CSID_S_WLANAP_STAINFO_RSSI1         	_STA_WLANAP(0x6700) //T=u32, station RSSI1
#define CSID_S_WLANAP_STAINFO_RSSI2         	_STA_WLANAP(0x6800) //T=u32, station RSSI2
#define CSID_S_WLANAP_STAINFO_PHMD          	_STA_WLANAP(0x6900) //T=str, station PHMD
#define CSID_S_WLANAP_STAINFO_BW            	_STA_WLANAP(0x6a00) //T=str, station BW
#define CSID_S_WLANAP_STAINFO_MCS           	_STA_WLANAP(0x6b00) //T=u8, station MCS
#define CSID_S_WLANAP_STAINFO_SGI           	_STA_WLANAP(0x6c00) //T=u8, station SGI
#define CSID_S_WLANAP_STAINFO_STBC          	_STA_WLANAP(0x6d00) //T=u8, station STBC
#define CSID_S_WLANAP_STAINFO_IDLE          	_STA_WLANAP(0x6e00) //T=u32, station IDLE
#define CSID_S_WLANAP_STAINFO_RATE          	_STA_WLANAP(0x6f00) //T=u32, station RATE
#define CSID_S_WLANAP_STAINFO_C_TIME        	_STA_WLANAP(0x7000) //T=u32, station ConnectTime
#define CSID_S_WLANAP_STAINFO_COUNT        	 	_STA_WLANAP(0x7100) //T=u32, station count in AP
#define CSID_S_WLANAP_STAINFO_SIGNAL_STR    	_STA_WLANAP(0x7200) //T=u32,  station SIGNAL strength
#define CSID_S_WLANAP_TOTAL_STATION         	_STA_WLANAP(0x7300) //T=u32, total station count in APs
#define CSID_S_WLANAP_STAINFO_IP     	    	_STA_WLANAP(0x7400) //T=ipv4, station IP address
#define CSID_S_WLANAP_STAINFO_HOSTNAME     		_STA_WLANAP(0x7500) //T=str, station Host Name
#define CSID_S_WLANAP_STAINFO_MODE    	 		_STA_WLANAP(0x7600) //T=str, station Wireless Mode
#define CSID_S_WLANAP_STAINFO_INTERFACE    		_STA_WLANAP(0x7700) //T=str, station AP Interface
#define CSID_S_WLANAP_STAINFO_USING_CHANNEL	    _STA_WLANAP(0x7800) //T=u32, station AP real using channel



/*2.4G AP INFO max16*/
#define CSID_S_WLANAP_APINFO_MAC           _STA_WLANAP(0x8000) //T=mac, AP MAC
#define CSID_S_WLANAP_APINFO_AID           _STA_WLANAP(0x8010) //T=u8, D=0, AP index
#define CSID_S_WLANAP_APINFO_RX_PACKETS    _STA_WLANAP(0x8020) //T=u32,D=0, AP RxCount Packet
#define CSID_S_WLANAP_APINFO_TX_PACKETS    _STA_WLANAP(0x8030) //T=u32,D=0, AP TxCount Packet
#define CSID_S_WLANAP_APINFO_RX_BYTES      _STA_WLANAP(0x8040) //T=u32,D=0, AP Received Byte Count
#define CSID_S_WLANAP_APINFO_TX_BYTES      _STA_WLANAP(0x8050) //T=u32,D=0, AP Transmitted Byte Count
#define CSID_S_WLANAP_APINFO_RX_ERR_PKS    _STA_WLANAP(0x8060) //T=u32,D=0, AP Rx Error Count
#define CSID_S_WLANAP_APINFO_RX_DROP_PKS   _STA_WLANAP(0x8070) //T=u32,D=0, AP Rx Drop Count
#define CSID_S_WLANAP_APINFO_STACOUNT      _STA_WLANAP(0x8080) //T=u8,D=0 ,total station count

/*AP STATISTICS*/
#define CSID_S_WLANAP_AP_STATISTICS       _STA_WLANAP(0x8100) //T=str, AP STATISTICS

/*WDS INFO*/
#define CSID_S_WLANAP_WDSINFO_MAC           _STA_WLANAP(0x8110) //T=mac, WDS Entry MAC
#define CSID_S_WLANAP_WDSINFO_RX_PACKETS    _STA_WLANAP(0x8120) //T=u32,D=0, WDS RxCount Packet
#define CSID_S_WLANAP_WDSINFO_TX_PACKETS    _STA_WLANAP(0x8130) //T=u32,D=0, WDS TxCount Packet
#define CSID_S_WLANAP_WDSINFO_RX_BYTES      _STA_WLANAP(0x8140) //T=u32,D=0, WDS Received Byte Count
#define CSID_S_WLANAP_WDSINFO_TX_BYTES      _STA_WLANAP(0x8150) //T=u32,D=0, WDS Transmitted Byte Count

/*AP IDS*/
#define CSID_S_WLANAP_AP_WIDSAuthCount			_STA_WLANAP(0x8200) //T=u32, D=0
#define CSID_S_WLANAP_AP_WIDSAssocReqCount		_STA_WLANAP(0x8201) //T=u32, D=0
#define CSID_S_WLANAP_AP_WIDSReassocReqCount	_STA_WLANAP(0x8202) //T=u32, D=0
#define CSID_S_WLANAP_AP_WIDSProbeReqCount		_STA_WLANAP(0x8203) //T=u32, D=0
#define CSID_S_WLANAP_AP_WIDSDisassocCount		_STA_WLANAP(0x8204) //T=u32, D=0
#define CSID_S_WLANAP_AP_WIDSDeauthCount		_STA_WLANAP(0x8205) //T=u32, D=0
#define CSID_S_WLANAP_AP_WIDSEapReqCount		_STA_WLANAP(0x8206) //T=u32, D=0
#define CSID_S_WLANAP_AP_WIDSMaliciousData		_STA_WLANAP(0x8207) //T=u32, D=0

/* H/W settings, MUST in P section. */
#define CSID_S_WLANAP_WLAN0_REG_DOMAIN		_STA_WLANAP(0x9001)	//T=u32, D=1, FCC=1,IC=2,ETSI=3,SPAIN=4,FRANCE=5,MKK=6 
#define CSID_S_WLANAP_RF_TYPE				_STA_WLANAP(0x9002)	//T=u32, D=0, RF
#define CSID_S_WLANAP_LED_TYPE				_STA_WLANAP(0x9003)	//T=u32, D=0, LED
#define CSID_S_WLANAP_WLAN0_11N_LOFDMPWD    _STA_WLANAP(0x9004)	//T=u32, D=0, OFDM diff
#define CSID_S_WLANAP_WLAN0_RX_Antenna      _STA_WLANAP(0x9005) //T=u32, D=0, RX antenna
#define CSID_S_WLANAP_WLAN0_11N_THER_RFIC   _STA_WLANAP(0x9006)	//T=u32, D=0, no used
#define CSID_S_WLANAP_WLAN0_11N_XCAP	    _STA_WLANAP(0x9007)	//T=u32, D=0, xcap
#define CSID_S_WLANAP_WLAN0_11N_AntCdiff    _STA_WLANAP(0x9008) //T=u32, D=0, AntCdiff
#define CSID_S_WLANAP_WLAN0_TX_Antenna		_STA_WLANAP(0x9009)	//T=u32, D=0, TX antenna
#define CSID_S_WLANAP_WLAN0_Channel			_STA_WLANAP(0x900a) //T=u32, D=0, channel
#define CSID_S_WLANAP_WLAN0_DataRate		_STA_WLANAP(0x900b) //T=u32, D=0, DataRate
#define CSID_S_WLANAP_WLAN0_Bandwith		_STA_WLANAP(0x900c) //T=u32, D=0, Bandwith
#define CSID_S_WLANAP_WLAN0_GI				_STA_WLANAP(0x900d) //T=u32, D=0, GI
#define CSID_S_WLANAP_Packet_Count			_STA_WLANAP(0x900e) //T=u32, D=0, Packet Count
#define CSID_S_WLANAP_Packet_Length			_STA_WLANAP(0x900f) //T=u32, D=0, Packet Length
#define CSID_S_WLANAP_RX_OK					_STA_WLANAP(0x9010) //T=u32, D=0, RX OK
#define CSID_S_WLANAP_RX_Error				_STA_WLANAP(0x9011) //T=u32, D=0, RX Error
#define CSID_S_WLANAP_TX_Count				_STA_WLANAP(0x9012) //T=u32, D=0, TX Count
#define CSID_S_WLANAP_A_Rssi              	_STA_WLANAP(0x9013) //T=u32, D=0, A rssi
#define CSID_S_WLANAP_B_Rssi              	_STA_WLANAP(0x9014) //T=u32, D=0, B rssi
#define CSID_S_WLANAP_C_Rssi              	_STA_WLANAP(0x9015) //T=u32, D=0, C rssi
#define CSID_S_WLANAP_D_Rssi              	_STA_WLANAP(0x9016) //T=u32, D=0, D rssi
#define CSID_S_WLANAP_WLAN0_11N_LOFDMPWDA	_STA_WLANAP(0x9017) //T=u32, D=0, OFDM A diff for RTL8192SE
#define CSID_S_WLANAP_WLAN0_11N_LOFDMPWDB	_STA_WLANAP(0x9018) //T=u32, D=0, OFDM B diff for RTL8192SE
#define CSID_S_WLANAP_WLAN0_HW_11N_THER		_STA_WLANAP(0x9019) //T=u32, D=0, Thermal TSSI for RTL8192SE
#define CSID_S_WLANAP_WLAN0_11N_AntBdiff	_STA_WLANAP(0x901a) //T=u32, D=0, AntBdiff for RTL8192SE
#define CSID_S_WLANAP_TX_POWER_CCK			_STA_WLANAP(0x9100)	//T=u32, D=0, 11b 255 entries
#define CSID_S_WLANAP_TX_POWER_OFDM         _STA_WLANAP(0x9200)	//T=u32, D=0, 11g 255 entries
#define CSID_S_WLANAP_TX_POWER_MCSA         _STA_WLANAP(0x9300) //T=u32, D=0, 11n Antenna A 255 entries
#define CSID_S_WLANAP_TX_POWER_MCSC         _STA_WLANAP(0x9400) //T=u32, D=0, 11n Antenna C 255 entries
#define CSID_S_WLANAP_TX_POWER_MCSA_1S		_STA_WLANAP(0x9500) //T=u32, D=0, 11n Antenna A 255 entries for RTL8192SE
#define CSID_S_WLANAP_TX_POWER_MCSB_1S		_STA_WLANAP(0x9600) //T=u32, D=0, 11n Antenna B 255 entries for RTL8192SE
#define CSID_S_WLANAP_TX_POWER_MCSA_2S		_STA_WLANAP(0x9700) //T=u32, D=0, 11n Antenna A 255 entries for RTL8192SE
#define CSID_S_WLANAP_TX_POWER_MCSB_2S		_STA_WLANAP(0x9800) //T=u32, D=0, 11n Antenna B 255 entries for RTL8192SE
#define CSID_S_WLANAP_TX_POWER_CCK_A		_STA_WLANAP(0xa000)	//T=u32, D=0, 11b 255 entries for RTL8192CE
#define CSID_S_WLANAP_TX_POWER_CCK_B		_STA_WLANAP(0xa100)	//T=u32, D=0, 11b 255 entries for RTL8192CE
#define CSID_S_WLANAP_TX_POWER_HT40_1S_A	_STA_WLANAP(0xa200)	//T=u32, D=0, 11b 255 entries for RTL8192CE
#define CSID_S_WLANAP_TX_POWER_HT40_1S_B	_STA_WLANAP(0xa300)	//T=u32, D=0, 11b 255 entries for RTL8192CE
#define CSID_S_WLANAP_TX_POWER_DIFF_HT40_2S	_STA_WLANAP(0xa400)	//T=u32, D=0, 11b 255 entries for RTL8192CE
#define CSID_S_WLANAP_TX_POWER_DIFF_HT20	_STA_WLANAP(0xa500)	//T=u32, D=0, 11b 255 entries for RTL8192CE
#define CSID_S_WLANAP_TX_POWER_DIFF_OFDM	_STA_WLANAP(0xa600)	//T=u32, D=0, 11b 255 entries for RTL8192CE
#define CSID_S_WLANAP_TR_SWITCH				_STA_WLANAP(0xa700)	//T=u32, D=0, 11b 255 entries for RTL8192CE
#define CSID_S_WLANAP_DISABLE_CH14_OFDM		_STA_WLANAP(0xa800)	//T=u32, D=0, 11b 255 entries for RTL8192CE
#define CSID_S_WLANAP_TX_POWER_OFDM_A		_STA_WLANAP(0xa900) //T=u32, D=0, 11b 255 entries for RTL8192CE
#define CSID_S_WLANAP_TX_POWER_OFDM_B       _STA_WLANAP(0xaa00) //T=u32, D=0, 11b 255 entries for RTL8192CE
#define CSID_S_WLANAP_TX_POWER_HT40_2S_A	_STA_WLANAP(0xab00)	//T=u32, D=0, 11b 255 entries for RTL8192CE
#define CSID_S_WLANAP_TX_POWER_HT40_2S_B	_STA_WLANAP(0xac00)	//T=u32, D=0, 11b 255 entries for RTL8192CE
#define CSID_S_WLANAP_TX_POWER_HT20_A		_STA_WLANAP(0xad00)	//T=u32, D=0, 11b 255 entries for RTL8192CE
#define CSID_S_WLANAP_TX_POWER_HT20_B		_STA_WLANAP(0xae00)	//T=u32, D=0, 11b 255 entries for RTL8192CE

//WLAN DHCP list status
#define CSID_S_DHCP_WLANAP_LIST_MAC			_STA_WLANAP(0xaf00) //T=mac, 
#define CSID_S_DHCP_WLANAP_LIST_IP			_STA_WLANAP(0xb000) //T=ipv4
#define CSID_S_DHCP_WLANAP_LIST_HOSTNAME	_STA_WLANAP(0xb100) //T=str

#define CSID_S_DHCP_WLANAP_LIST1_MAC		_STA_WLANAP(0xb200) //T=mac
#define CSID_S_DHCP_WLANAP_LIST1_IP			_STA_WLANAP(0xb300) //T=ipv4
#define CSID_S_DHCP_WLANAP_LIST1_HOSTNAME	_STA_WLANAP(0xb400) //T=str

//if RF module is exist
#define CSID_S_WLANAP_RF_MUDULE_EXIST		_STA_WLANAP(0xb500)	//T=u32, D=0, if this RF module is not exist, write it to 99

#define CSID_S_WLANAP_WIRELESS_MODE         _STA_WLANAP(0xb510) //T=u32, wireless mode, 0:AP Router Mode 1:AP Only Mode 2:WDS Hybrid 3:WDS Only 4:Adapter 5:Universal Repeater 6:Ad hoc

#define CSID_S_WLANAP_RF_WDSLINK_STATUS		_STA_WLANAP(0xb700)	//T=u32, D=0, if this WDSLINK is not connect


//WLAN DHCP list status
#define CSID_S_DHCP_WLANAP_LIST_TRIGGER     _STA_WLANAP(0xb701) //T=u32, D=0, WLAN Client List Log Trigger

//WLAN Status
#define CSID_S_WLANAP_AUTO_CHANNEL          _STA_WLANAP(0xb702) //T=u32, D=0, Channel Value of Auto Channel

#define CSID_S_WLANAP_ROUTINE_RUNNING       _STA_WLANAP(0xb703) //T=u8, D=0, indicate if AP routine is running

//WLAN QR Code
#define CSID_S_WLANAP_QRCODE_S				_STA_WLANAP(0xb704) //T=u8, D=0, wifi QR code
#define CSID_S_WLANAP_QRCODE_E				_STA_WLANAP(0xb70b) //T=u8, D=0, wifi QR code

#endif
