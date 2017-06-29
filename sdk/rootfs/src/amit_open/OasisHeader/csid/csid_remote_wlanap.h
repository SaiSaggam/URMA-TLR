/*
 * @file Member ID Definition of Group REMOTE_WLANAP.
 *
 * Moderator: Aaron Wu
 * Group ID: 0x00770000/0x80770000
 */

#ifndef _CSID_REMOTE_WLANAP_H
#define _CSID_REMOTE_WLANAP_H

#include "csid_gid.h"

#define _CFG_REMOTE_WLANAP(x)       (CSID_GIDC_REMOTE_WLANAP|(x))
#define _STA_REMOTE_WLANAP(x)       (CSID_GIDS_REMOTE_WLANAP|(x))

/* Wireless 2.4G */
#define CSID_S_REMOTE_WLANAP_WLANENABLE                	    _STA_REMOTE_WLANAP(0x0000)  //T=u8, AP Wireless Main function, 0:Disable, 1:Enable
#define CSID_S_REMOTE_WLANAP_WIRELESS_MODE             	    _STA_REMOTE_WLANAP(0x0001)  //T=u8, AP wireless mode
#define CSID_S_REMOTE_WLANAP_GREEN_AP 	                 	_STA_REMOTE_WLANAP(0x0002)  //T=u8, AP Green AP, 0:Disable, 1:Enable
#define CSID_S_REMOTE_WLANAP_LAZY_MODE                 	    _STA_REMOTE_WLANAP(0x0003)  //T=u8, AP Lazy Mode, 0:Disable, 1:Enable
#define CSID_S_REMOTE_WLANAP_WDSMACLIST 					_STA_REMOTE_WLANAP(0x0004)  //T=str, the collection of all ap mac address  
//SSID Enable
#define CSID_S_REMOTE_WLANAP_APID_ENABLE          	    	_STA_REMOTE_WLANAP(0x0100)  //T=u8, AP SSID function, 0:Disable, 1:Enable
//Schedule
#define CSID_S_REMOTE_WLANAP_USE_RULE_NO          	    	_STA_REMOTE_WLANAP(0x0200)  //T=u16, AP schedule number
//SSID
#define CSID_S_REMOTE_WLANAP_SSID             		    	_STA_REMOTE_WLANAP(0x0300)  //T=str, AP SSID Name
//SSID Broadcast
#define CSID_S_REMOTE_WLANAP_HIDESSID                  		_STA_REMOTE_WLANAP(0x0400)  //T=u8, AP SSID Broadcsat, 0:Disable, 1:Enable
//WLAN Pratition
#define CSID_S_REMOTE_WLANAP_NOFORWARDING              			_STA_REMOTE_WLANAP(0x0500)	 //T=u8, AP WLAN Partitation, 0:Disable, 1:Enable
//CHANNEL
#define CSID_S_REMOTE_WLANAP_CHANNEL                   			_STA_REMOTE_WLANAP(0x0600)	//T=u8, APChannel
//WirelessMode
#define CSID_S_REMOTE_WLANAP_WIRLESSMODE               			_STA_REMOTE_WLANAP(0x0700)	//T=u8, AP Wireless Mode
//AuthMode
#define CSID_S_REMOTE_WLANAP_AUTHMODE                  			_STA_REMOTE_WLANAP(0x0800)	//T=u8, AP Authenication Type
//IEEE 8021x       
#define CSID_S_REMOTE_WLANAP_IEEE8021X                 			_STA_REMOTE_WLANAP(0x0900)	//T=u8, AP 8021X, 0:Disable, 1:Enable
//Encryption
#define CSID_S_REMOTE_WLANAP_ENCRYPTYPE                			_STA_REMOTE_WLANAP(0x0a00)	//T=u8, AP Encryption Type
//WebKey Index       
#define CSID_S_REMOTE_WLANAP_DEFKEYID                  			_STA_REMOTE_WLANAP(0x0b00)	//T=u8, D=1, AP Default WEP Key ID
//WebKey key1 Type
#define CSID_S_REMOTE_WLANAP_KEY0TYPE                  			_STA_REMOTE_WLANAP(0x0c00)	//T=u8, D=0, AP WEP Key0 Type
//Webkey 1
#define CSID_S_REMOTE_WLANAP_KEY0                      			_STA_REMOTE_WLANAP(0x0d00)	//T=str, D="1234567890", AP WEP Key0 Value
//WebKey key2 Type
#define CSID_S_REMOTE_WLANAP_KEY1TYPE                  			_STA_REMOTE_WLANAP(0x0e00)	//T=u8, D=0, AP WEP Key1 Type
//Webkey 2
#define CSID_S_REMOTE_WLANAP_KEY1                      			_STA_REMOTE_WLANAP(0x0f00)	//T=str, D="1234567890", APWEP Key1 Value
//WebKey key3 Type
#define CSID_S_REMOTE_WLANAP_KEY2TYPE                  			_STA_REMOTE_WLANAP(0x1000)	//T=u8, D=0, AP WEP Key2 Type
//Webkey 3
#define CSID_S_REMOTE_WLANAP_KEY2                      			_STA_REMOTE_WLANAP(0x1100)	//T=str, D="1234567890", AP WEP Key2 Value
//WebKey key4 Type
#define CSID_S_REMOTE_WLANAP_KEY3TYPE             	    		_STA_REMOTE_WLANAP(0x1200)	//T=u8, D=0, AP WEP Key3 Type
//Webkey 4
#define CSID_S_REMOTE_WLANAP_KEY3                 	    		_STA_REMOTE_WLANAP(0x1300)	//T=str, D="1234567890", AP WEP Key3 Value
//Preshare Key
#define CSID_S_REMOTE_WLANAP_WPAPSK               	    		_STA_REMOTE_WLANAP(0x1400)	//T=str, D="1234567890", AP WPA PSK Value 
//Radius Server IP
#define CSID_S_REMOTE_WLANAP_RADIUS_SRVIP         	    		_STA_REMOTE_WLANAP(0x1500)	//T=ipv4, AP Radius Server IP 
//Radius Port
#define CSID_S_REMOTE_WLANAP_RADIUS_SRVPORT       	    		_STA_REMOTE_WLANAP(0x1600)	//T=u16, D=1812, AP Radius Server Port
//Radius Share Key
#define CSID_S_REMOTE_WLANAP_RADIUS_SRVKEY        	    		_STA_REMOTE_WLANAP(0x1700)	//T=str, AP Radius Server Share Key 
//VLAN ID Enable
#define CSID_S_REMOTE_WLANAP_WIFI1_VLAN_ENABLE    	    		_STA_REMOTE_WLANAP(0x1800)	//T=u8, D=0, AP WIFI1 VLAN ID enable
//VLAN ID Value
#define CSID_S_REMOTE_WLANAP_WIFI1_VLAN_ID        	    		_STA_REMOTE_WLANAP(0x1900)	//T=u32, D=3, APWIFI1_VLAN ID
//Max Supported Stations Enable
#define CSID_S_REMOTE_WLANAP_MAX_CLIENT_LIMIT     					_STA_REMOTE_WLANAP(0x1a00)	//T=u8, D=0, AP Max Supported Stations, 0:Disable, 1:Enablenable
//Max Supported Stations value
#define CSID_S_REMOTE_WLANAP_MAX_CLIENT_NUM       					_STA_REMOTE_WLANAP(0x1b00)	//T=u8, AP Max Supported Stations Value
//Bandwidth
#define CSID_S_REMOTE_WLANAP_HT_BW                	    		_STA_REMOTE_WLANAP(0x1c00)	//T=u8, D=1, AP BandWidth
//RemoteAPMAC
#define CSID_S_REMOTE_WLANAP_WDSAP                		    	_STA_REMOTE_WLANAP(0x1d00)	//T=mac, AP WDS AP MAC Address1

/*Wireless 2.4G Advanced Network Setting*/
#define CSID_S_REMOTE_WLANAP_AREACODE					    _STA_REMOTE_WLANAP(0x1e00)	//T=u8, AP 2.4G Area Code
#define CSID_S_REMOTE_WLANAP_BEACONINTERVAL       		    _STA_REMOTE_WLANAP(0x1f00)	//T=u16, D=100, AP Beacon Interval 
#define CSID_S_REMOTE_WLANAP_TXPOWER                	    _STA_REMOTE_WLANAP(0x2000)	//T=u8, D=100, AP Tx Power Level 
#define CSID_S_REMOTE_WLANAP_RTSCTSTHRESHOLD           	    _STA_REMOTE_WLANAP(0x2100)	//T=u16, D=2347, AP CTS/RTS Threshold
#define CSID_S_REMOTE_WLANAP_FRAGTHRESHOLD             	    _STA_REMOTE_WLANAP(0x2200)	//T=u16, D=2346, AP Fragmentation Threshold 
#define CSID_S_REMOTE_WLANAP_DTIMINTERVAL              	    _STA_REMOTE_WLANAP(0x2300)	//T=u16, D=1, DTIM interval
#define CSID_S_REMOTE_WLANAP_WMMCAPABLE                	    _STA_REMOTE_WLANAP(0x2400)	//T=u32, D=1, AP WmmCapable  
#define CSID_S_REMOTE_WLANAP_FIXED_DATARATE            	    _STA_REMOTE_WLANAP(0x2500)	//T=u8, AP Wlan Fixed Data Rate
#define CSID_S_REMOTE_WLANAP_NOFORWARDINGBTNBSSID      	    _STA_REMOTE_WLANAP(0x2600)	//T=u8, AP Isolation, 0:Disable, 1:Enable
#define CSID_S_REMOTE_WLANAP_BASICRATE                      _STA_REMOTE_WLANAP(0x2700)	//T=u8, D=15, 2.4G basic rate                   
#define CSID_S_REMOTE_WLANAP_FIXEDTXMODE                    _STA_REMOTE_WLANAP(0x2800)	//T=str, D='HT'  
#define CSID_S_REMOTE_WLANAP_GI                         	_STA_REMOTE_WLANAP(0x2900)	//T=u8, 2.4G Short GI 0: Disable, 1:Enable
#define CSID_S_REMOTE_WLANAP_CLI_MAC                        _STA_REMOTE_WLANAP(0x2a00)	//T=str, 2.4G AP MAC for Universal Repeater Mode
#define CSID_S_REMOTE_WLANAP_CLI_MAC2_ENABLE		    _STA_REMOTE_WLANAP(0x2b00)	//T=u32, 2.4G AP MAC2 for Universal Repeater Mode enable
#define CSID_S_REMOTE_WLANAP_CLI_MAC2			    _STA_REMOTE_WLANAP(0x2c00)  //T=str, 2.4G AP MAC2 for Universal Repeater Mode
#define CSID_S_REMOTE_WLANAP_HT_BSSCOEXISTENCE		    _STA_REMOTE_WLANAP(0x2d00)	//T=u32, D=0, HT_BSS Coexistence
#define CSID_S_REMOTE_WLANAP_HT_VHT_BW			    _STA_REMOTE_WLANAP(0x2e00)  //T=u32, VHT BW
#define CSID_S_REMOTE_WLANAP_HT_TXSTREAM		    _STA_REMOTE_WLANAP(0x2f00)  //T=u32, AP1 TxStream
#define CSID_S_REMOTE_WLANAP_ANTENNA_SWITCH		    _STA_REMOTE_WLANAP(0x3000)  //T=u32, D=0, AP1 antenna switch enable
#define CSID_S_REMOTE_WLANAP_BAND_STEERING		    _STA_REMOTE_WLANAP(0x3100)  //T=u32, D=0, AP 5G band steering enable

/*Wireless 2.4G Mbps*/
#define CSID_S_REMOTE_WLANAP_TX_MBPS						_STA_REMOTE_WLANAP(0x4000)	//T=str, record AP Tx Mbps
#define CSID_S_REMOTE_WLANAP_RX_MBPS						_STA_REMOTE_WLANAP(0x4001)  //T=str, record AP Rx Mbps
#define CSID_S_REMOTE_WLANAP_TXRX_MBPS						_STA_REMOTE_WLANAP(0x4002)	//T=str, record AP Tx+Rx  Mbps
/*Wireless 2.4G each SSID Mbps */
#define CSID_S_REMOTE_WLANAP_VAP_TX_MBPS					_STA_REMOTE_WLANAP(0x4010)	//T=str, record AP VAP Tx Mbps
#define CSID_S_REMOTE_WLANAP_VAP_RX_MBPS					_STA_REMOTE_WLANAP(0x4020)	//T=str, record AP VAP Rx Mbps
#define CSID_S_REMOTE_WLANAP_VAP_TXRX_MBPS					_STA_REMOTE_WLANAP(0x4030)	//T=str, record AP VAP TxRx Mbps
/*Wireless 2.4G each VAP user Mbps */
#define CSID_S_REMOTE_WLANAP_USER_VAP1_TX					_STA_REMOTE_WLANAP(0x4100)	//T=str, record VAP1 User's Tx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP1_RX					_STA_REMOTE_WLANAP(0x4110)	//T=str, record VAP1 User's Rx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP1_TXRX					_STA_REMOTE_WLANAP(0x4120)	//T=str, record VAP1 User's TxRx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP2_TX                   _STA_REMOTE_WLANAP(0x4130)  //T=str, recod VAP2 User's Tx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP2_RX                   _STA_REMOTE_WLANAP(0x4140)  //T=str, record VAP2 User's Rx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP2_TXRX                 _STA_REMOTE_WLANAP(0x4150)  //T=str, record VAP2 User's TxRx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP3_TX                   _STA_REMOTE_WLANAP(0x4160)  //T=str, record VAP3 User's Tx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP3_RX                   _STA_REMOTE_WLANAP(0x4170)  //T=str, record VAP3 User's Rx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP3_TXRX                 _STA_REMOTE_WLANAP(0x4180)  //T=str, record VAP3 User's TxRx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP4_TX                   _STA_REMOTE_WLANAP(0x4190)  //T=str, record VAP4 User's Tx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP4_RX                   _STA_REMOTE_WLANAP(0x41a0)  //T=str, record VAP4 User's Rx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP4_TXRX                 _STA_REMOTE_WLANAP(0x41b0)  //T=str, record VAP4 User's TxRx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP5_TX                   _STA_REMOTE_WLANAP(0x41c0)  //T=str, record VAP5 User's Tx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP5_RX                   _STA_REMOTE_WLANAP(0x41d0)  //T=str, record VAP5 User's Rx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP5_TXRX                 _STA_REMOTE_WLANAP(0x41e0)  //T=str, record VAP5 User's TxRx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP6_TX                   _STA_REMOTE_WLANAP(0x41f0)  //T=str, record VAP6 User's Tx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP6_RX                   _STA_REMOTE_WLANAP(0x4200)  //T=str, record VAP6 User's Rx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP6_TXRX                 _STA_REMOTE_WLANAP(0x4210)  //T=str, record VAP6 User's TxRx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP7_TX                   _STA_REMOTE_WLANAP(0x4220)  //T=str, record VAP7 User's Tx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP7_RX                   _STA_REMOTE_WLANAP(0x4230)  //T=str, record VAP7 User's Rx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP7_TXRX                 _STA_REMOTE_WLANAP(0x4240)  //T=str, record VAP7 User's TxRx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP8_TX                   _STA_REMOTE_WLANAP(0x4250)  //T=str, record VAP8 User's Tx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP8_RX                   _STA_REMOTE_WLANAP(0x4260)  //T=str, record VAP8 User's Rx Mbps
#define CSID_S_REMOTE_WLANAP_USER_VAP8_TXRX                 _STA_REMOTE_WLANAP(0x4270)  //T=str, record VAP8 User's TxRx Mbps
#endif //ifndef _CSID_REMOTE_WLANAP_H


