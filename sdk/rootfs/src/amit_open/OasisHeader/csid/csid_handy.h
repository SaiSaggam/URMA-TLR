/*
 * @file Member ID Definition of HANDY
 *
 * Moderator: Abby Yang
 * Group ID: 0x00310000/0x80310000
 */

#ifndef _CSID_HANDY_H
#define _CSID_HANDY_H

#include "csid_gid.h"

#define _CFG_HANDY(x)		(CSID_GIDC_HANDY|(x))
#define _STA_HANDY(x)		(CSID_GIDS_HANDY|(x))

/* Extra Definitions */

/* Configuration Items */

// HomeCom 1.0+ Handy Setting
#define CSID_C_HANDY_DEVICENAME		_CFG_HANDY(0x0010) //T=str, D="", Handy Name
#define CSID_C_HANDY_IP				_CFG_HANDY(0x0020) //T=str, D="", Handy IP
#define CSID_C_HANDY_MAC			_CFG_HANDY(0x0030) //T=str, D="", Handy MAC
#define CSID_C_HANDY_CONFIRM		_CFG_HANDY(0x0040) //T=u8, D=0, Handy confirm
#define CSID_C_HANDY_AVAILABLE		_CFG_HANDY(0x0050) //T=u8, D=1, Handy available

// Basic Handy Setting
#define CSID_C_HANDY_ID				_CFG_HANDY(0x0110) //T=str, D="", Handy ID
#define CSID_C_HANDY_RINGTYPE		_CFG_HANDY(0x0120) //T=str, D="", Inter_Call_Ring
#define CSID_C_HANDY_BRIGHTNESS		_CFG_HANDY(0x0130) //T=str, D="", Brightness
#define CSID_C_HANDY_VIEWANGLE		_CFG_HANDY(0x0140) //T=str, D="", ViewAngle
#define CSID_C_HANDY_VOLUME			_CFG_HANDY(0x0150) //T=str, D="", Volume
#define CSID_C_HANDY_RINGVOLUME		_CFG_HANDY(0x0160) //T=str, D="", RingVolume
#define CSID_C_HANDY_SMS_USR		_CFG_HANDY(0x0170) //T=str, D="", SMS_Username
#define CSID_C_HANDY_SMS_PWD		_CFG_HANDY(0x0180) //T=str, D="", SMS_Password
#define CSID_C_HANDY_SMS_NUMBER		_CFG_HANDY(0x0190) //T=str, D="", SMS_Number
#define CSID_C_HANDY_SMS_E			_CFG_HANDY(0x01a0) //T=str, D="", SMS_Enable
#define CSID_C_HANDY_BISOUND_E		_CFG_HANDY(0x01b0) //T=str, D="", BiSound_Enable
#define CSID_C_HANDY_AUTOMONITOR_E	_CFG_HANDY(0x01c0) //T=str, D="", AutoMonitor_Enable

/* Status Items */

// HomeCom 1.0+ Register information
#define CSID_S_HANDY_REGNAME		_STA_HANDY(0x0010) //T=str, D="", New Handy Name
#define CSID_S_HANDY_REGIP			_STA_HANDY(0x0020) //T=str, D="", New Handy IP
#define CSID_S_HANDY_REGMAC			_STA_HANDY(0x0030) //T=str, D="", New Handy MAC
#define CSID_S_HANDY_REGCONFIRM		_STA_HANDY(0x0040) //T=u8,  D=0
#define CSID_S_HANDY_REGAVAILABLE	_STA_HANDY(0x0050) //T=u8,  D=1
#define CSID_S_HANDY_REGFAIL		_STA_HANDY(0x00f0) //T=u8,  D=1

#endif	//ifndef _CSID_HANDY_H
