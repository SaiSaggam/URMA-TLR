/*
 * @file Member ID Definition of Group VoIP FXS port setting
 *
 * Moderator: Terence Yang
 * Group ID: 0x00280000/0x80280000
 */

#ifndef _CSID_VOIPFXS_H
#define _CSID_VOIPFXS_H

#include "csid_gid.h"

#define _CFG_VOIPFXS(x)		(CSID_GIDC_VOIPFXS|(x))
#define _STA_VOIPFXS(x)		(CSID_GIDS_VOIPFXS|(x))

/* Extra Definitions */

#define Display_Before_Ring         0x00
#define Display_After_Ring          0x01

#define ID_Type_Disabled            0x00 
#define ID_Type_DTMF                0x01
#define ID_Type_FSK_Bellcore        0x02
#define ID_Type_FSK_ETSI            0x03

#define ID_Alert_No_Alert           0x00
#define ID_Alert_DTAS               0x01
#define ID_Alert_RPAS               0x02
#define ID_Alert_LR                 0x03
#define ID_Alert_LR_DTAS            0x04

#define DIAL_Both                   0x00
#define DIAL_DTMF                   0x01
#define DIAL_PULSE                  0x02

#define Ring_Imp_600ohm             0x00
#define Ring_Imp_900ohm             0x01
#define Ring_Imp_600ohm_1uF         0x02	//Japan (600ohm+1uF) 
#define Ring_Imp_900ohm_216uF       0x03	//900ohm+2.16uF
#define Ring_Imp_270ohm_120nF       0x04	//CTR21 (270ohm+750ohm||120nF)
#define Ring_Imp_220ohm_120uF       0x05	//Australia/New Zealand(220ohm+820ohm||120nF)
#define Ring_Imp_220ohm_115uF       0x06	//Slovakia/Slovenia/South Africa (220ohm+820ohm||115nF)
#define Ring_Imp_200ohm_100nF       0x07	//China (200ohm+600ohm||100nF)

/* Configuration Items */

// FXS Port Setting  
#define CSID_C_VOIPFXS_ID_Type         _CFG_VOIPFXS(0x0010)	//T=u8, D=3, Caller ID Type
#define CSID_C_VOIPFXS_ID_Type2        _CFG_VOIPFXS(0x0020)	//T=u8, D=3, Caller ID Type 2
#define CSID_C_VOIPFXS_ID_Power        _CFG_VOIPFXS(0x0030)	//T=u8, D=10, Caller ID Power Level   dbm. (0..20, default:10) 
#define CSID_C_VOIPFXS_ID_Display      _CFG_VOIPFXS(0x0040)	//T=u8, D=0, Caller ID Display
#define CSID_C_VOIPFXS_ID_Alert        _CFG_VOIPFXS(0x0050)	//T=u8, D=1, Caller ID Type 1 Alerting Signal
#define CSID_C_VOIPFXS_ID_Alert2       _CFG_VOIPFXS(0x0060)	//T=u8, D=1, Caller ID Type 2 Alerting Signal
#define CSID_C_VOIPFXS_ID_RepFlag      _CFG_VOIPFXS(0x0070)	//T=u8, D=0, Caller ID Replacement Flag  0:Dis 1:Enabl
#define CSID_C_VOIPFXS_ID_RepPre       _CFG_VOIPFXS(0x0080)	//T=str, Caller ID Replacement Prefix   Replace when caller id length greater than 7  
#define CSID_C_VOIPFXS_ID_RepStr       _CFG_VOIPFXS(0x0090)	//T=str, Caller ID Replacement String 
#define CSID_C_VOIPFXS_ID_Type3        _CFG_VOIPFXS(0x00a0)     //T=u8, D=3, Caller ID DECT1 Type 
#define CSID_C_VOIPFXS_ID_Type4        _CFG_VOIPFXS(0x00b0)     //T=u8, D=3, Caller ID DECT2 Type 

// Line 0 (FXS 0) 
#define CSID_C_VOIPFXS_DIAL            _CFG_VOIPFXS(0x0100)	//T=u8, D=1, Dial Method
#define CSID_C_VOIPFXS_Ring_Imp        _CFG_VOIPFXS(0x0110)	//T=u8, D=0, Ring Impedance
#define CSID_C_VOIPFXS_HFD_Upper       _CFG_VOIPFXS(0x0120)	//T=u16, D=1000, Hook Flash Detect Upper Bound  msec. (100..3000) 
#define CSID_C_VOIPFXS_HFD_Lower       _CFG_VOIPFXS(0x0130)	//T=u16, D=300, Hook Flash Detect Lower Bound  msec. (100..3000) 
#define CSID_C_VOIPFXS_TX_Level        _CFG_VOIPFXS(0x0140)	//T=u8, D=6, Voice Tx Level   1 2 3 4 5 6 7 8 (default:6) 
#define CSID_C_VOIPFXS_RX_Level        _CFG_VOIPFXS(0x0150)	//T=u8, D=6, Voice Rx Level   1 2 3 4 5 6 7 8 (default:6) 
#define CSID_C_VOIPFXS_NUM        	   _CFG_VOIPFXS(0x0160)	//T=u8, D=1, 1:1-FXS 2:2-FXS
#define CSID_C_VOIPFXS_Idle_Polarity   		_CFG_VOIPFXS(0x0170)	//T=u8, D=0, FXS IDLE Polarity, 0:forward(default) 1:reverse
#define CSID_C_VOIPFXS_Caller_Conn_Polarity	_CFG_VOIPFXS(0x0180) 	//T=u8, D=0, FXS Caller Conn Polarity, 0:forward(default) 1:reverse
#define CSID_C_VOIPFXS_Callee_Conn_Polarity	_CFG_VOIPFXS(0x0190)	//T=u8, D=0, FXS Callee Conn Polarity, 0:forward(default) 1:reverse


//@D2_csid
// Line 1 (FXS 2) 
#define CSID_C_VOIPFXS_DIAL_FXS2            _CFG_VOIPFXS(0x0200)	//T=u8, D=1, Dial Method
#define CSID_C_VOIPFXS_Ring_Imp_FXS2        _CFG_VOIPFXS(0x0210)	//T=u8, D=0, Ring Impedance
#define CSID_C_VOIPFXS_HFD_Upper_FXS2       _CFG_VOIPFXS(0x0220)	//T=u16, D=1000, Hook Flash Detect Upper Bound  msec. (100..3000) 
#define CSID_C_VOIPFXS_HFD_Lower_FXS2       _CFG_VOIPFXS(0x0230)	//T=u16, D=300, Hook Flash Detect Lower Bound  msec. (100..3000) 
#define CSID_C_VOIPFXS_TX_Level_FXS2        _CFG_VOIPFXS(0x0240)	//T=u8, D=6, Voice Tx Level   1 2 3 4 5 6 7 8 (default:6) 
#define CSID_C_VOIPFXS_RX_Level_FXS2        _CFG_VOIPFXS(0x0240)	//T=u8, D=6, Voice Rx Level   1 2 3 4 5 6 7 8 (default:6) 
#define CSID_C_VOIPFXS_Idle_Polarity_FXS2   		_CFG_VOIPFXS(0x0260)	//T=u8, D=0, FXS IDLE Polarity, 0:forward(default) 1:reverse
#define CSID_C_VOIPFXS_Caller_Conn_Polarity_FXS2	_CFG_VOIPFXS(0x0270) 	//T=u8, D=0, FXS Caller Conn Polarity, 0:forward(default) 1:reverse
#define CSID_C_VOIPFXS_Callee_Conn_Polarity_FXS2	_CFG_VOIPFXS(0x0280)	//T=u8, D=0, FXS Callee Conn Polarity, 0:forward(default) 1:reverse
#define CSID_C_VOIPFXS_FXS1_Connection	  			_CFG_VOIPFXS(0x0290)	//T=u8, D=0, Phone FXS 1, 0:disable 1:enable
#define CSID_C_VOIPFXS_FXS2_Connection	  			_CFG_VOIPFXS(0x0291)	//T=u8, D=0, Phone FXS 2, 0:disable 1:enable

/* Status Items */
#define CSID_S_VOIPFXS_ID_Type         		_STA_VOIPFXS(0x0010)	//T=str, D="", caller ID in str
#define CSID_S_VOIPFXS_Ring_Imp        		_STA_VOIPFXS(0x0110)	//T=str, D="", Ring Impedance in str

#define CSID_S_VOIPFXS_Ring_Cancel_Count  _STA_VOIPFXS(0x0130)  //T=u16, D="", Ring Cancel Count
#define CSID_S_VOIPFXS_Ring_and_Offhook   _STA_VOIPFXS(0x0140)  //T=u16, D="", Ring and Offhook
#define CSID_S_VOIPFXS_Tone_and_Onhook    _STA_VOIPFXS(0x0150)  //T=u16, D="", Tone and Onhook
#define CSID_S_VOIPFXS_FXS1_Connection	  _STA_VOIPFXS(0x0160)	//T=u8, D=0, Phone FXS 1, 0:disable 1:enable
#define CSID_S_VOIPFXS_FXS2_Connection	  _STA_VOIPFXS(0x0161)	//T=u8, D=0, Phone FXS 2, 0:disable 1:enable
#define CSID_S_VOIPFXS_FXS3_Connection    _STA_VOIPFXS(0x0162)  //T=u8, D=0, Phone FXS 3, 0:disable 1:enable
#define CSID_S_VOIPFXS_FXS4_Connection    _STA_VOIPFXS(0x0163)  //T=u8, D=0, Phone FXS 4, 0:disable 1:enable


/* Multisession Items (start fromn 0x2000) */

#endif //ifndef _CSID_VOIPFXS_H
