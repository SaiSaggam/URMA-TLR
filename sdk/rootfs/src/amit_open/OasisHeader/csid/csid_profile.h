/*
 * @file Member ID Definition of Group Profile
 *
 * Moderator: RD1-1
 * Group ID: 0x00980000/0x80980000
 */
 
#ifndef _CSID_PROFILE_H
#define _CSID_PROFILE_H

#include "csid_gid.h"

#define _CFG_PROFILE(x)		(CSID_GIDC_PROFILE|(x))
#define _STA_PROFILE(x)		(CSID_GIDS_PROFILE|(x))

/* Configuration Items */
// template: #define CSID_C_PROFILE_description     	    _CFG_PROFILE(0x0000)	//T=u32, D=0, CSID description


/* misc */
#define CSID_C_PROFILE_RECORDED				_CFG_PROFILE(0x0000)	//T=u32, D=0, ETH profile count recorded
#define CSID_C_PROFILE_MANUAL               _CFG_PROFILE(0x0001)    //T=u32, D=0, Manual select wantype profile
#define CSID_C_PROFILE_WEBCAM_ENABLE        _CFG_PROFILE(0x0002)    //T=u32, D=1, Enable Webcam remote access

/*MCU test for ATS*/
#define CSID_C_PROFILE_MCU_DATA             _CFG_PROFILE(0x0010)    //T=str, Record data for MCU
//#define                                     _CFG_PROFILE0x0014)

/* Profile for PPPOE, PPTP and L2TP*/
#define CSID_C_PROFILE_PROF_NAME			_CFG_PROFILE(0x1000)	//T=str, Profile name for eth. WAN
#define CSID_C_PROFILE_WANTYPE				_CFG_PROFILE(0x1100)	//T=u32, Profile wantype
#define CSID_C_PROFILE_IP_STATUS			_CFG_PROFILE(0x1200)	//T=u32, D=1, 1:dynamic ip, 0:static ip
#define CSID_C_PROFILE_IP					_CFG_PROFILE(0x1300)	//T=ipv4, ip addr for static ip status
#define CSID_C_PROFILE_SUBNET				_CFG_PROFILE(0x1400)	//T=ipv4, subnet addr for static ip status
#define CSID_C_PROFILE_GATEWAY				_CFG_PROFILE(0x1500)	//T=ipv4, gateway addr for static ip status
#define CSID_C_PROFILE_SERVER_IP			_CFG_PROFILE(0x1600)	//T=ipv4, server ip for connect
#define CSID_C_PROFILE_USERNAME				_CFG_PROFILE(0x1700)	//T=str, username for connect
#define CSID_C_PROFILE_PASSWORD				_CFG_PROFILE(0x1800)	//T=str, password for connect
#define CSID_C_PROFILE_SERVICE_NAME			_CFG_PROFILE(0x1900)	//T=str, service name for pppoe
#define CSID_C_PROFILE_CNT_TYPE				_CFG_PROFILE(0x1a00)	//T=u32, 0:on demand, 1:always on or 2:manual
#define CSID_C_PROFILE_IDLE_TIME			_CFG_PROFILE(0x1b00)	//T=u32, maximum idel time for connect
#define CSID_C_PROFILE_PRI_DNS				_CFG_PROFILE(0x1c00)	//T=ipv4, primary dns
#define CSID_C_PROFILE_SEC_DNS				_CFG_PROFILE(0x1d00)	//T=ipv4, secondary dns
#define CSID_C_PROFILE_MTU					_CFG_PROFILE(0x1e00)	//T=u32, MTU
#define CSID_C_PROFILE_ACNAME               _CFG_PROFILE(0x1f00)    //T=str, AC_NAME

/* Profile for 3G */
#define CSID_C_PROFILE_AUTO_ENABLE			_CFG_PROFILE(0x3000)	//T=u32, 3G config 1:auto/0:manual
#define CSID_C_PROFILE_COUNTRY				_CFG_PROFILE(0x3100)	//T=str, country name for 3G
#define CSID_C_PROFILE_ISP					_CFG_PROFILE(0x3200)	//T=str, ISP name for 3G
#define CSID_C_PROFILE_NET_TYPE				_CFG_PROFILE(0x3300)	//T=u32, 0:WCDMA 1:CDMA2000
#define CSID_C_PROFILE_DIAL_NUM				_CFG_PROFILE(0x3400)	//T=str, dialed number for 3g
#define CSID_C_PROFILE_AUTH					_CFG_PROFILE(0x3500)	//T=u32, 0:auto 1:pap 2:chap
#define CSID_C_PROFILE_PIN					_CFG_PROFILE(0x3600)	//T=str, pin for 3G
#define CSID_C_PROFILE_APN					_CFG_PROFILE(0x3700)	//T=str, APN for 3G
#define CSID_C_PROFILE_DATA_LIMIT			_CFG_PROFILE(0x3800)	//T=u32, maximum data for 3G use
#define CSID_C_PROFILE_LOCK                 _CFG_PROFILE(0x3900)    //T=u32, sim card lock, 0:unlock, 1:lock
#define CSID_C_PROFILE_PLAN                 _CFG_PROFILE(0x3a00)    //T=u32, 3G data plan
#define CSID_C_PROFILE_IDLE_UNIT            _CFG_PROFILE(0x3b00)    //T=u32, idle time unit, 0:sec, 1:min
#define CSID_C_PROFILE_DATA_UNIT            _CFG_PROFILE(0x3c00)    //T=u32, data limit unit

/* Profile for WISP */
#define CSID_C_PROFILE_SSID                 _CFG_PROFILE(0x5000)    //T=str, AP SSID
#define CSID_C_PROFILE_AUTHMODE             _CFG_PROFILE(0x5100)    //T=u32, D=0, AP AUTHMODE
#define CSID_C_PROFILE_ENCRYPTYPE           _CFG_PROFILE(0x5200)    //T=u32, D=0, AP ENCRYPTYPE
#define CSID_C_PROFILE_DEFKEYID             _CFG_PROFILE(0x5300)    //T=u32, D=0, AP DEFKEYID
#define CSID_C_PROFILE_KEY0TYPE             _CFG_PROFILE(0x5400)    //T=u32, D=0, AP KEY0TYPE
#define CSID_C_PROFILE_KEY0                 _CFG_PROFILE(0x5500)    //T=str, AP KEY0
#define CSID_C_PROFILE_KEY1TYPE             _CFG_PROFILE(0x5600)    //T=u32, D=0, AP KEY1TYPE
#define CSID_C_PROFILE_KEY1                 _CFG_PROFILE(0x5700)    //T=str, AP KEY1
#define CSID_C_PROFILE_KEY2TYPE             _CFG_PROFILE(0x5800)    //T=u32, D=0, AP KEY2TYPE
#define CSID_C_PROFILE_KEY2                 _CFG_PROFILE(0x5900)    //T=str, AP KEY2
#define CSID_C_PROFILE_KEY3TYPE             _CFG_PROFILE(0x5a00)    //T=u32, D=0, AP KEY3TYPE
#define CSID_C_PROFILE_KEY3                 _CFG_PROFILE(0x5b00)    //T=str, AP KEY3 
#define CSID_C_PROFILE_WPAPSK               _CFG_PROFILE(0x5c00)    //T=str, AP WPAPSK
#define CSID_C_PROFILE_CHANNEL              _CFG_PROFILE(0x5d00)    //T=u32, D=0, Channel of AP 
#define CSID_C_PROFILE_MAC                  _CFG_PROFILE(0x5e00)    //T=str, AP MAC
#define CSID_C_PROFILE_EXTCHANNEL           _CFG_PROFILE(0x5f00)    //T=u32, Extension Channel
#define CSID_C_PROFILE_WIRELESSMODE         _CFG_PROFILE(0x6000)    //T=u32, Wireless mode

/* Next start position _CFG_PROFILE(0x7000) */


/* Status Items */
#define CSID_S_PROFILE_UI_NOTIFY    		_STA_PROFILE(0x0000)	//T=u32, D=0, 1:Need UI setup profile
#define CSID_S_PROFILE_DET_FINISH			_STA_PROFILE(0x0001)	//T=u32, D=0, 1:wantype detect finish
#define CSID_S_PROFILE_DET_WANTYPE			_STA_PROFILE(0x0002)	//T=u32, the detect result of wantype
#define CSID_S_PROFILE_PPPOE_ACNAME			_STA_PROFILE(0x0003)	//T=str, the AC NAME for PPPOE
#define CSID_S_PROFILE_WANSET_FIN			_STA_PROFILE(0x0004)	//T=u32, wan profile set finish
#define CSID_S_PROFILE_ADDING_ALTER         _STA_PROFILE(0x0005)    //T=u32, alter after adding temporary profile
#define CSID_S_PROFILE_MODIFY               _STA_PROFILE(0x0006)    //T=u32, 0:none, 1:add, 2:remove, 3:modify
#define CSID_S_PROFILE_MODIFY_INDEX         _STA_PROFILE(0x0007)    //T=u32, modify index number
#define CSID_S_PROFILE_CONNECT_TIME         _STA_PROFILE(0x0008)    //T=u32, record connect time for wantype
#define CSID_S_PROFILE_WISP_UI_RESELECT     _STA_PROFILE(0x0009)    //T=u32, Need UI to reselect a AP

#define CSID_S_PROFILE_STATUS               _STA_PROFILE(0x000a)    //T=u32, record the process of profile status
#define CSID_S_PROFILE_ALTER_TYPE           _STA_PROFILE(0x000b)    //T=u32,  D=0, type of profile alter 
#define CSID_S_PROFILE_RECORDED             _STA_PROFILE(0x000c)    //T=u32, D=0, the largest number of profile index
#define CSID_S_PROFILE_WANEXEC              _STA_PROFILE(0x000d)    //T=u32, D=0, start wan type flag, 0: have not executed, 1: executed

#define CSID_S_PROFILE_CONNECT_TIMEV6       _STA_PROFILE(0x000e)    //T=u32, D=0, record connect time for v6 wantype
#define CSID_S_PROFILE_DO_SCAN              _STA_PROFILE(0x000f)    //T=u32, D=0, do AP scan before WISP

#define CSID_S_PROFILE_GW_ALIVE             _STA_PROFILE(0x0010)    //T=u32, D=0, Check Gateway, 1:alive
#define CSID_S_PROFILE_MANUAL               _STA_PROFILE(0x0011)    //T=u32, D=0, Profile Select by Manual, 0: No Specify, 1: Specify

/*MCU Information*/
#define CSID_S_PROFILE_MCU_VERSION          _STA_PROFILE(0x0100)    //T=str, D=0, record MCU version
#define CSID_S_PROFILE_MCU_BATVAL           _STA_PROFILE(0x0101)    //T=u32, D=0, record Battery value
#define CSID_S_PROFILE_MCU_CHARGE           _STA_PROFILE(0x0102)    //T=u32, D=0, charge status, 0:discharge, 1:charge
#define CSID_S_PROFILE_MCU_BATSTA           _STA_PROFILE(0x0103)    //T=u32, D=0, 0:without BAT, 1:with BAT
#define CSID_S_PROFILE_MCU_EN_DEV           _STA_PROFILE(0x0104)    //T=u32, D=0, 1:enable, 0:disable
#define CSID_S_PROFILE_MCU_UPEND            _STA_PROFILE(0x0105)    //T=u32, D=0, 1:download finish
#define CSID_S_PROFILE_MCU_RECORD           _STA_PROFILE(0x0106)    //T=u32, D=0, 1:ready for MCU test record
#define CSID_S_PROFILE_MCU_VFLAG            _STA_PROFILE(0x0107)    //T=u32, D=0, 1:for special version

/*Webcam status*/
#define CSID_S_PROFILE_WEBCAM_STATUS        _STA_PROFILE(0x0110)    //T=u32, D=0, 0: not exist, 1: exist
#define CSID_S_PROFILE_WEBCAM_MODEL         _STA_PROFILE(0x0115)    //T=str, Webcam model name
#define CSID_S_PROFILE_WEBCAM_HW            _STA_PROFILE(0x011A)    //T=str, Webcam HW version
#define CSID_S_PROFILE_WEBCAM_SW            _STA_PROFILE(0x011F)    //T=str, Webcam SW version

/*hotspot*/
#define CSID_S_PROFILE_INUSE    			_STA_PROFILE(0x1000)	//T=u32, Profile in use
#define CSID_S_PROFILE_APNUM                _STA_PROFILE(0x1100)    //T=u32, AP NUM in Scan list

/* temporaty Profile for PPPOE, L2TP & PPTP*/
#define CSID_S_PROFILE_PROF_NAME			_STA_PROFILE(0x2000)	//T=str, Profile name for eth. WAN
#define CSID_S_PROFILE_WANTYPE				_STA_PROFILE(0x2001)	//T=u32, Profile wantype
#define CSID_S_PROFILE_IP_STATUS			_STA_PROFILE(0x2002)	//T=u32, D=1, 1:dynamic ip, 0:static ip
#define CSID_S_PROFILE_IP					_STA_PROFILE(0x2003)	//T=ipv4, ip addr for static ip status
#define CSID_S_PROFILE_SUBNET				_STA_PROFILE(0x2004)	//T=ipv4, subnet addr for static ip status
#define CSID_S_PROFILE_GATEWAY				_STA_PROFILE(0x2005)	//T=ipv4, gateway addr for static ip status
#define CSID_S_PROFILE_SERVER_IP			_STA_PROFILE(0x2006)	//T=ipv4, server ip for connect
#define CSID_S_PROFILE_USERNAME				_STA_PROFILE(0x2007)	//T=str, username for connect
#define CSID_S_PROFILE_PASSWORD				_STA_PROFILE(0x2008)	//T=str, password for connect
#define CSID_S_PROFILE_SERVICE_NAME			_STA_PROFILE(0x2009)	//T=str, service name for pppoe
#define CSID_S_PROFILE_CNT_TYPE				_STA_PROFILE(0x200a)	//T=u32, 0:on demand, 1:always on or 2:manual
#define CSID_S_PROFILE_IDLE_TIME			_STA_PROFILE(0x200b)	//T=u32, maximum idel time for connect
#define CSID_S_PROFILE_PRI_DNS				_STA_PROFILE(0x200c)	//T=ipv4, primary dns
#define CSID_S_PROFILE_SEC_DNS				_STA_PROFILE(0x200d)	//T=ipv4, secondary dns
#define CSID_S_PROFILE_MTU					_STA_PROFILE(0x200e)	//T=u32, MTU
#define CSID_S_PROFILE_ACNAME               _STA_PROFILE(0x200f)    //T=str, AC_NAME

/* temporaty Profile for 3G */
#define CSID_S_PROFILE_AUTO_ENABLE			_STA_PROFILE(0x2020)	//T=u32, 3G config 1:auto/0:manual
#define CSID_S_PROFILE_COUNTRY				_STA_PROFILE(0x2021)	//T=str, country name for 3G
#define CSID_S_PROFILE_ISP					_STA_PROFILE(0x2022)	//T=str, ISP name for 3G
#define CSID_S_PROFILE_NET_TYPE				_STA_PROFILE(0x2023)	//T=u32, 0:WCDMA 1:CDMA2000
#define CSID_S_PROFILE_DIAL_NUM				_STA_PROFILE(0x2024)	//T=str, dialed number for 3g
#define CSID_S_PROFILE_AUTH					_STA_PROFILE(0x2025)	//T=u32, 0:auto 1:pap 2:chap
#define CSID_S_PROFILE_PIN					_STA_PROFILE(0x2026)	//T=str, pin for 3G
#define CSID_S_PROFILE_APN					_STA_PROFILE(0x2027)	//T=str, APN for 3G
#define CSID_S_PROFILE_DATA_LIMIT			_STA_PROFILE(0x2028)	//T=u32, maximum data for 3G use
#define CSID_S_PROFILE_LOCK                 _STA_PROFILE(0x2029)    //T=u32, sim card lock, 0:unlock, 1:lock
#define CSID_S_PROFILE_PLAN                 _STA_PROFILE(0x202a)    //T=u32, 3G data plan
#define CSID_S_PROFILE_IDLE_UNIT            _STA_PROFILE(0x202b)    //T=u32, idle time unit, 0:sec, 1:min
#define CSID_S_PROFILE_DATA_UNIT            _STA_PROFILE(0x202c)    //T=u32, data limit unit

/* temporaty Profile for WISP */
#define CSID_S_PROFILE_SSID                 _STA_PROFILE(0x2040)    //T=str, AP SSID
#define CSID_S_PROFILE_AUTHMODE             _STA_PROFILE(0x2041)    //T=u32, D=0, AP AUTHMODE
#define CSID_S_PROFILE_ENCRYPTYPE           _STA_PROFILE(0x2042)    //T=u32, D=0, AP ENCRYPTYPE
#define CSID_S_PROFILE_DEFKEYID             _STA_PROFILE(0x2043)    //T=u32, D=0, AP DEFKEYID
#define CSID_S_PROFILE_KEY0TYPE             _STA_PROFILE(0x2044)    //T=u32, D=0, AP KEY0TYPE
#define CSID_S_PROFILE_KEY0                 _STA_PROFILE(0x2045)    //T=str, AP KEY0
#define CSID_S_PROFILE_KEY1TYPE             _STA_PROFILE(0x2046)    //T=u32, D=0, AP KEY1TYPE
#define CSID_S_PROFILE_KEY1                 _STA_PROFILE(0x2047)    //T=str, AP KEY1
#define CSID_S_PROFILE_KEY2TYPE             _STA_PROFILE(0x2048)    //T=u32, D=0, AP KEY2TYPE
#define CSID_S_PROFILE_KEY2                 _STA_PROFILE(0x2049)    //T=str, AP KEY2
#define CSID_S_PROFILE_KEY3TYPE             _STA_PROFILE(0x204a)    //T=u32, D=0, AP KEY3TYPE
#define CSID_S_PROFILE_KEY3                 _STA_PROFILE(0x204b)    //T=str, AP KEY3 
#define CSID_S_PROFILE_WPAPSK               _STA_PROFILE(0x204c)    //T=str, AP WPAPSK
#define CSID_S_PROFILE_CHANNEL              _STA_PROFILE(0x204d)    //T=u32, D=0, Channel of AP 
#define CSID_S_PROFILE_MAC                  _STA_PROFILE(0x204e)    //T=str, AP MAC
#define CSID_S_PROFILE_EXTCHANNEL           _STA_PROFILE(0x204f)    //T=u32, Extension Channel
#define CSID_S_PROFILE_WIRELESSMODE         _STA_PROFILE(0x2050)    //T=u32, Wireless Mode

/* Next start position _CFG_PROFILE(0x2060) */


#endif
