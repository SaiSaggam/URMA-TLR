/*
 * @file Member ID Definition of Group VoIP Phone Book
 *
 * Moderator: Terence Yang
 * Group ID: 0x002d0000/0x802d0000
 */

#ifndef _CSID_VOIPPB_H
#define _CSID_VOIPPB_H

#include "csid_gid.h"

#define _CFG_VOIPPB(x)		(CSID_GIDC_VOIPPB|(x))
#define _STA_VOIPPB(x)		(CSID_GIDS_VOIPPB|(x))

/* Extra Definitions */

#define SDDigit_None                0x00
#define SDDigit_1                   0x01
#define SDDigit_2                   0x02
#define SDDigit_3                   0x03
#define SDDigit_4                   0x04
#define SDDigit_5                   0x05
#define SDDigit_6                   0x06
#define SDDigit_7                   0x07
#define SDDigit_8                   0x08
#define SDDigit_9                   0x09
#define SDDigit_0                   0x0a

/* Configuration Items */

// Phone Book of FXS1 
#define CSID_C_VOIPPB_SDDigit         _CFG_VOIPPB(0x0001)	//T=u8, SpeedDial Digit 
#define CSID_C_VOIPPB_PhoneURI_USER   _CFG_VOIPPB(0x0011)	//T=str, Phone URI 
#define CSID_C_VOIPPB_PhoneURI_ADDR   _CFG_VOIPPB(0x0021)	//T=ipv4
#define CSID_C_VOIPPB_PhoneURI_Note   _CFG_VOIPPB(0x0031)	//T=str, Note
#define CSID_C_VOIPPB_PhoneURI_ENABLE _CFG_VOIPPB(0x0041)	//T=u8, SpeedDial Enable

// Phone Book of FXS2 
#define CSID_C_VOIPPB_SDDigit_FXS2         _CFG_VOIPPB(0x0101)	//T=u8, SpeedDial Digit 
#define CSID_C_VOIPPB_PhoneURI_USER_FXS2   _CFG_VOIPPB(0x0111)	//T=str, Phone URI 
#define CSID_C_VOIPPB_PhoneURI_ADDR_FXS2   _CFG_VOIPPB(0x0121)	//T=ipv4
#define CSID_C_VOIPPB_PhoneURI_Note_FXS2   _CFG_VOIPPB(0x0131)	//T=str, Note
#define CSID_C_VOIPPB_PhoneURI_ENABLE_FXS2 _CFG_VOIPPB(0x0141)	//T=u8, SpeedDial Enable
/* Status Items */

/* Multisession Items (start fromn 0x2000) */

// CIW563AM Phone Book of FXS
#define CSID_C_VOIP_VPORT_NAME         	_CFG_VOIPPB(0x0201)       //T=str, SpeedDial Digit 
#define CSID_C_VOIPPB_VPORT_URL  	    _CFG_VOIPPB(0x0341)       //T=str, Phone URI 
#define CSID_C_VOIPPB_VPORT_SELECT      _CFG_VOIPPB(0x0501)       //T=u8, 0: disable, 1:enable

#define CSID_C_VOIPPB_TPG_NAME         _CFG_VOIPPB(0x0601)       //T=str, TPG SpeedDial Digit
#define CSID_C_VOIPPB_TPG_URL          _CFG_VOIPPB(0x0741)       //T=str, TPG Phone URI
#define CSID_C_VOIPPB_TPG_SELECT       _CFG_VOIPPB(0x0901)       //T=u8, TPG 0: disable, 1:enable
#endif //ifndef _CSID_VOIPPB_H
