/*
* @file Member ID Definition of Group VoIP Call Service
*
* Moderator: JasonJiang
* Group ID: 0x002f0000/0x802f0000
 */

#ifndef _CSID_VOIP_H
#define _CSID_VOIP_H

#include "csid_gid.h"

#define _CFG_VOIP(x)       (CSID_GIDC_VOIP|(x))
#define _STA_VOIP(x)       (CSID_GIDS_VOIP|(x))

#define VOIP_SIP_STA_UNREGISTERED	0
#define VOIP_SIP_STA_REGISTERED		1

#define VOIP_SIP_PHONE_STATUS_NOTREADY		0x00
#define VOIP_SIP_PHONE_STATUS_READY		0x01
#define VOIP_SIP_PHONE_STATUS_RINGING		0x02
#define VOIP_SIP_PHONE_STATUS_DIALING		0x03
#define VOIP_SIP_PHONE_STATUS_BUSY		0x04
#define VOIP_SIP_PHONE_STATUS_OFFHOOK		0x05

#define VOIP_SIP_FXS_LED_FLASH		0x01
#define VOIP_SIP_FXS_LED_ON		0x02
#define VOIP_SIP_FXS_LED_OFF		0x03
#define VOIP_SIP_FXS_LED_FAST_FLASH		0x04

/* Configuration Items */
#define CSID_C_VOIP_ENABLE	        _CFG_VOIP(0x0000)	//T=u8,	D=1, VoIP enable, 0:Disable 1:Enable
#define CSID_C_VOIP_PROVISIONING	_CFG_VOIP(0x0001)	//T=u8,	D=0, VoIP Provisioning, 0:Disable 1:Enable
#define CSID_C_VOIP_HW_SWITCH		_CFG_VOIP(0x0002)	//T=u8,	D=3, VoIP HW SWITCH, 1:1FXS 2:2FXS 3:1FXO+1FXS
#define CSID_C_VOIP_PHONE_REGION	_CFG_VOIP(0x0003)	//T=u8,	D=0, Region: 0:USA 1:NZ
#define CSID_C_VOIP_PROV_URL		_CFG_VOIP(0x0004)	//T=str, Provision server's URL
#define CSID_C_VOIP_PROV_SERIALNUM	_CFG_VOIP(0x0005)	//T=str, Serial Number used for provision 
#define CSID_C_VOIP_OPEN_VTSP_PORT	_CFG_VOIP(0x0006)	//T=u8,	D=0, Stealth Mode flag for VOIP
#define CSID_C_VOIP_SILENT_MODE	    _CFG_VOIP(0x0007)	//T=u8,	D=0, Silent mode for VOIP
#define CSID_C_VOIP_ADAM_SERVER	    _CFG_VOIP(0x0008)	//T=u8,	D=0, Compatibility Adam SIP server

/* Reserved 16 items for Phone1 ~ Phone16 */
#define CSID_C_VOIP_PHONE_VOIPWAN_NUM	_CFG_VOIP(0x0010)	//T=u8, VoIP WAN Number for Phone 1~Phone 16, 0:WAN 1:VoIPWAN1 2:VoIPWAN2 ...
#define CSID_C_VOIP_VPORT_IP				_CFG_VOIP(0x0020)   //T=ipv4
#define CSID_C_HAVE_FXS_LED 	_CFG_VOIP(0x0030) //T=u8, D=0, HAVE FXS LED for VOIP
#define CSID_C_MULTIWAN_VOIP_ENABLE     _CFG_VOIP(0x0031) //T=u8, D=0, 0:Singlewan voip 1:Multiwan VOIP
#define CSID_C_VAPP_CHECK_MECHANISM_ENABLE  _CFG_VOIP(0x0032) //T=u8, D=0, 0:vapp check mechanism disable 1:Enable
#define CSID_C_CUSTOMER_TRUSTPOWER  _CFG_VOIP(0x0033) //T=u8, D=0, 0:Generic 1:Trustpower
#define CSID_C_CHANGE_INT_CPU2      _CFG_VOIP(0x0034) //T=u8, D=0, 1:Change interrupt to CPU2

/*Missed Call Records*/
#define CSID_C_VOIP_MISSED_CALL_INDEX            _CFG_VOIP(0x0040) //T=u32, D=0, Missed call numbers
#define CSID_C_VOIP_MISSED_CALL_INDEX_START      _CFG_VOIP(0x0041) //T=u32, D=0, The index of the latest missed call
#define CSID_C_VOIP_MISSED_CALL_PHONE_NUMBER     _CFG_VOIP(0x0140) //T=str, Missed call phone number
#define CSID_C_VOIP_MISSED_CALL_START_YEAR       _CFG_VOIP(0x0240) //T=u32, Missed call start year
#define CSID_C_VOIP_MISSED_CALL_START_MONTH      _CFG_VOIP(0x0340) //T=u32, 1~12
#define CSID_C_VOIP_MISSED_CALL_START_DAY        _CFG_VOIP(0x0440) //T=u32, 1~31
#define CSID_C_VOIP_MISSED_CALL_START_WDAY       _CFG_VOIP(0x0540) //T=u32, SUN. MON. ... 
#define CSID_C_VOIP_MISSED_CALL_START_HOUR       _CFG_VOIP(0x0640) //T=u32, Hour
#define CSID_C_VOIP_MISSED_CALL_START_MIN        _CFG_VOIP(0x0740) //T=u32, Minute
#define CSID_C_VOIP_MISSED_CALL_START_SEC        _CFG_VOIP(0x0840) //T=u32, Second

/*Incoming Call Records*/
#define CSID_C_VOIP_INCOMING_CALL_INDEX          _CFG_VOIP(0x0940) //T=u32, D=0, Incoming call numbers
#define CSID_C_VOIP_INCOMING_CALL_INDEX_START    _CFG_VOIP(0x0941) //T=u32, D=0, The index of the latest incoming call
#define CSID_C_VOIP_INCOMING_CALL_PHONE_NUMBER   _CFG_VOIP(0x0a40) //T=str, Incoming call phone number
#define CSID_C_VOIP_INCOMING_CALL_START_YEAR     _CFG_VOIP(0x0b40) //T=u32, Incoming call start year
#define CSID_C_VOIP_INCOMING_CALL_START_MONTH    _CFG_VOIP(0x0c40) //T=u32, 1~12
#define CSID_C_VOIP_INCOMING_CALL_START_DAY      _CFG_VOIP(0x0d40) //T=u32, 1~31
#define CSID_C_VOIP_INCOMING_CALL_START_WDAY     _CFG_VOIP(0x0e40) //T=u32, SUN. MON. ... 
#define CSID_C_VOIP_INCOMING_CALL_START_HOUR     _CFG_VOIP(0x0f40) //T=u32, Hour
#define CSID_C_VOIP_INCOMING_CALL_START_MIN      _CFG_VOIP(0x1040) //T=u32, Minute
#define CSID_C_VOIP_INCOMING_CALL_START_SEC      _CFG_VOIP(0x1140) //T=u32, Second
#define CSID_C_VOIP_INCOMING_CALL_END_YEAR       _CFG_VOIP(0x1240) //T=u32, Incoming call end year
#define CSID_C_VOIP_INCOMING_CALL_END_MONTH      _CFG_VOIP(0x1340) //T=u32, 1~12
#define CSID_C_VOIP_INCOMING_CALL_END_DAY        _CFG_VOIP(0x1440) //T=u32, 1~31
#define CSID_C_VOIP_INCOMING_CALL_END_WDAY       _CFG_VOIP(0x1540) //T=u32, SUN. MON. ... 
#define CSID_C_VOIP_INCOMING_CALL_END_HOUR       _CFG_VOIP(0x1640) //T=u32, Hour
#define CSID_C_VOIP_INCOMING_CALL_END_MIN        _CFG_VOIP(0x1740) //T=u32, Minute
#define CSID_C_VOIP_INCOMING_CALL_END_SEC        _CFG_VOIP(0x1840) //T=u32, Second
#define CSID_C_VOIP_INCOMING_CALL_DURATION       _CFG_VOIP(0x1940) //T=u32, D=0, Duration

/*Outgoing Call Records*/
#define CSID_C_VOIP_OUTGOING_CALL_INDEX          _CFG_VOIP(0x1a40) //T=u32, D=0, Outgoing call numbers
#define CSID_C_VOIP_OUTGOING_CALL_INDEX_START    _CFG_VOIP(0x1a41) //T=u32, D=0, The index of the latest outgoing call
#define CSID_C_VOIP_OUTGOING_CALL_PHONE_NUMBER   _CFG_VOIP(0x1b40) //T=str, Outgoing call phone number
#define CSID_C_VOIP_OUTGOING_CALL_START_YEAR     _CFG_VOIP(0x1c40) //T=u32, Outgoing call start year
#define CSID_C_VOIP_OUTGOING_CALL_START_MONTH    _CFG_VOIP(0x1d40) //T=u32, 1~12
#define CSID_C_VOIP_OUTGOING_CALL_START_DAY      _CFG_VOIP(0x1e40) //T=u32, 1~31
#define CSID_C_VOIP_OUTGOING_CALL_START_WDAY     _CFG_VOIP(0x1f40) //T=u32, SUN. MON. ... 
#define CSID_C_VOIP_OUTGOING_CALL_START_HOUR     _CFG_VOIP(0x2040) //T=u32, Hour
#define CSID_C_VOIP_OUTGOING_CALL_START_MIN      _CFG_VOIP(0x2140) //T=u32, Minute
#define CSID_C_VOIP_OUTGOING_CALL_START_SEC      _CFG_VOIP(0x2240) //T=u32, Second
#define CSID_C_VOIP_OUTGOING_CALL_END_YEAR       _CFG_VOIP(0x2340) //T=u32, Outgoing call end year
#define CSID_C_VOIP_OUTGOING_CALL_END_MONTH      _CFG_VOIP(0x2440) //T=u32, 1~12
#define CSID_C_VOIP_OUTGOING_CALL_END_DAY        _CFG_VOIP(0x2540) //T=u32, 1~31
#define CSID_C_VOIP_OUTGOING_CALL_END_WDAY       _CFG_VOIP(0x2640) //T=u32, SUN. MON. ... 
#define CSID_C_VOIP_OUTGOING_CALL_END_HOUR       _CFG_VOIP(0x2740) //T=u32, Hour
#define CSID_C_VOIP_OUTGOING_CALL_END_MIN        _CFG_VOIP(0x2840) //T=u32, Minute
#define CSID_C_VOIP_OUTGOING_CALL_END_SEC        _CFG_VOIP(0x2940) //T=u32, Second
#define CSID_C_VOIP_OUTGOING_CALL_DURATION       _CFG_VOIP(0x2a40) //T=u32, D=0, Duration

/*Last Registration Time Record*/
#define CSID_C_VOIP_LAST_REG_TIME_YEAR           _CFG_VOIP(0x2b40) //T=u32, Last Registration Time year
#define CSID_C_VOIP_LAST_REG_TIME_MONTH          _CFG_VOIP(0x2b41) //T=u32, 1~12
#define CSID_C_VOIP_LAST_REG_TIME_DAY            _CFG_VOIP(0x2b42) //T=u32, 1~31
#define CSID_C_VOIP_LAST_REG_TIME_WDAY           _CFG_VOIP(0x2b43) //T=u32, SUN. MON. ... 
#define CSID_C_VOIP_LAST_REG_TIME_HOUR           _CFG_VOIP(0x2b44) //T=u32, Hour
#define CSID_C_VOIP_LAST_REG_TIME_MIN            _CFG_VOIP(0x2b45) //T=u32, Minute
#define CSID_C_VOIP_LAST_REG_TIME_SEC            _CFG_VOIP(0x2b46) //T=u32, Second

/* Status Items */
#define CSID_S_SIP_ALTERED            _STA_VOIP(0x0001) //T=u8, ui altered
#define CSID_S_VOIP_ALTERED           _STA_VOIP(0x0002) //T=u8, ui altered
#define CSID_S_VOIP_LOCK	          _STA_VOIP(0x0003) //T=u32, D=0, voip lock
#define CSID_S_VOIP_MESSAGE_WAITING	  _STA_VOIP(0x0004) //T=u32, D=0, Record message waiting numbers

/* Allocate 10 status for future use */
#define CSID_S_SIP_REGISTERED_STATUS	_STA_VOIP(0x0010) //T=u32, Registered Status
#define CSID_S_SIP_PHONE_STATUS 	_STA_VOIP(0x0011) //T=u32, Phone Status
#define CSID_S_FXS_LED 		_STA_VOIP(0x0020) //T=u32, LED For FXS status
#define CSID_S_FXS2_LED 	_STA_VOIP(0x0021) //T=u32, LED For FXS2 status
#define CSID_S_FXS3_LED 	_STA_VOIP(0x0022) //T=u32, LED For FXS3 status
#define CSID_S_FXS4_LED 	_STA_VOIP(0x0023) //T=u32, LED For FXS4 status

/*Record Qos rule index*/
#define CSID_S_VOIP_SIPPORT_QOS_INDEX 	      _STA_VOIP(0x0030) //T=u8, SIPPORT QOS INDEX
#define CSID_S_VOIP_SIPIPADDR_QOS_INDEX 	  _STA_VOIP(0x0040) //T=u8, SIPIPADDR QOS INDEX
#define CSID_S_VOIP_RTPPORT_START_QOS_INDEX   _STA_VOIP(0x0050) //T=u8, RTPPORT START QOS INDEX
#define CSID_S_VOIP_RTPPORT_END_QOS_INDEX 	  _STA_VOIP(0x0060) //T=u8, RTPPORT END QOS INDEX
#endif
