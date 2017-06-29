/*
 * @file Member ID Definition of Group VoIP Tone Setting
 *
 * Moderator: Terence Yang
 * Group ID: 0x00260000/0x80260000
 */

#ifndef _CSID_VOIPTONE_H
#define _CSID_VOIPTONE_H

#include "csid_gid.h"

#define _CFG_VOIPTONE(x)		(CSID_GIDC_VOIPTONE|(x))
#define _STA_VOIPTONE(x)		(CSID_GIDS_VOIPTONE|(x))

/* Extra Definitions */

#define Country_Custom      0x00
#define Country_Taiwan      0x01
#define Country_China       0x02
#define Country_US          0x03
#define Country_Japan       0x04
#define Country_India       0x05
#define Country_UK          0x06

/* Configuration Items */

// Tone Setting 
#define CSID_C_VOIPTONE_COUNTRY         _CFG_VOIPTONE(0x0001)	//T=u8, D=3, Country Code
#define CSID_C_VOIPTONE_4FHTone         _CFG_VOIPTONE(0x0002)	//T=u8, D=0, 4 Frequency Howler Tone, 0:Dis 1:Enable

// Custom Dial Tone 
// Dial Tone     
#define CSID_C_VOIPTONE_CUS_DT_On       _CFG_VOIPTONE(0x0010)	//T=u16, On time 
#define CSID_C_VOIPTONE_CUS_DT_Off      _CFG_VOIPTONE(0x0011)	//T=u16, Off time 
#define CSID_C_VOIPTONE_CUS_DT_Freq1    _CFG_VOIPTONE(0x0012)	//T=u16, Freq1 
#define CSID_C_VOIPTONE_CUS_DT_Freq2    _CFG_VOIPTONE(0x0013)	//T=u16, Freq2 
#define CSID_C_VOIPTONE_CUS_DT_Freq1dB  _CFG_VOIPTONE(0x0014)	//T=u16, Freq1dB 
#define CSID_C_VOIPTONE_CUS_DT_Freq2dB  _CFG_VOIPTONE(0x0015)	//T=u16, Freq2dB 
#define CSID_C_VOIPTONE_CUS_DT_Repeat   _CFG_VOIPTONE(0x0016)	//T=u16, Repeat
 
// Busy Tone     
#define CSID_C_VOIPTONE_CUS_BT_On       _CFG_VOIPTONE(0x0020)	//T=u16, On time 
#define CSID_C_VOIPTONE_CUS_BT_Off      _CFG_VOIPTONE(0x0021)	//T=u16, Off time 
#define CSID_C_VOIPTONE_CUS_BT_Freq1    _CFG_VOIPTONE(0x0022)	//T=u16, Freq1 
#define CSID_C_VOIPTONE_CUS_BT_Freq2    _CFG_VOIPTONE(0x0023)	//T=u16, Freq2 
#define CSID_C_VOIPTONE_CUS_BT_Freq1dB  _CFG_VOIPTONE(0x0024)	//T=u16, Freq1dB 
#define CSID_C_VOIPTONE_CUS_BT_Freq2dB  _CFG_VOIPTONE(0x0025)	//T=u16, Freq2dB 
#define CSID_C_VOIPTONE_CUS_BT_Repeat   _CFG_VOIPTONE(0x0026)	//T=u16, Repeat 
 
// RingBack Tone     
#define CSID_C_VOIPTONE_CUS_RB_On       _CFG_VOIPTONE(0x0030)	//T=u16, On time 
#define CSID_C_VOIPTONE_CUS_RB_Off      _CFG_VOIPTONE(0x0031)	//T=u16, Off time 
#define CSID_C_VOIPTONE_CUS_RB_Freq1    _CFG_VOIPTONE(0x0032)	//T=u16, Freq1 
#define CSID_C_VOIPTONE_CUS_RB_Freq2    _CFG_VOIPTONE(0x0033)	//T=u16, Freq2 
#define CSID_C_VOIPTONE_CUS_RB_Freq1dB  _CFG_VOIPTONE(0x0034)	//T=u16, Freq1dB 
#define CSID_C_VOIPTONE_CUS_RB_Freq2dB  _CFG_VOIPTONE(0x0035)	//T=u16, Freq2dB 
#define CSID_C_VOIPTONE_CUS_RB_Repeat   _CFG_VOIPTONE(0x0036)	//T=u16, Repeat
 
// Call Waiting Tone     
#define CSID_C_VOIPTONE_CUS_CW_On       _CFG_VOIPTONE(0x0040)	//T=u16, On time 
#define CSID_C_VOIPTONE_CUS_CW_Off      _CFG_VOIPTONE(0x0041)	//T=u16, Off time 
#define CSID_C_VOIPTONE_CUS_CW_Freq1    _CFG_VOIPTONE(0x0042)	//T=u16, Freq1 
#define CSID_C_VOIPTONE_CUS_CW_Freq2    _CFG_VOIPTONE(0x0043)	//T=u16, Freq2 
#define CSID_C_VOIPTONE_CUS_CW_Freq1dB  _CFG_VOIPTONE(0x0044)	//T=u16, Freq1dB 
#define CSID_C_VOIPTONE_CUS_CW_Freq2dB  _CFG_VOIPTONE(0x0045)	//T=u16, Freq2dB 
#define CSID_C_VOIPTONE_CUS_CW_Repeat   _CFG_VOIPTONE(0x0046)	//T=u16, Repeat 
   	
/* Status Items */

/* Multisession Items (start fromn 0x2000) */

#endif //ifndef _CSID_VOIPTONE_H
