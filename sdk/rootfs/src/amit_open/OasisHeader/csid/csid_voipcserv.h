/*
 * @file Member ID Definition of Group VoIP Call Service
 *
 * Moderator: Terence Yang
 * Group ID: 0x00270000/0x80270000
 */

#ifndef _CSID_VOIPCSERV_H
#define _CSID_VOIPCSERV_H

#include "csid_gid.h"

#define _CFG_VOIPCSERV(x)		(CSID_GIDC_VOIPCSERV|(x))
#define _STA_VOIPCSERV(x)		(CSID_GIDS_VOIPCSERV|(x))

/* Extra Definitions */
#define FOR_TYPE_DISABLE	0x00
#define FOR_TYPE_ALWAYS		0x01
#define FOR_TYPE_BUSY		0x02
#define FOR_TYPE_NO_ANSWER	0x03


/* Configuration Items */

// Call Service
#define CSID_C_VOIPCSERV_WAIT            _CFG_VOIPCSERV(0x0001)	//T=u8, D=1, Call Waiting, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_WAIT_TO         _CFG_VOIPCSERV(0x0002)	//T=u8, D=30, Call Waiting Timeout (10..300, unlimit:300)
#define CSID_C_VOIPCSERV_REPEAT_TIME     _CFG_VOIPCSERV(0x0003)	//T=u16, D=1800, Call Repeat Activity Time (60..3600)

// Lines
// Line (FXS 0) 
#define CSID_C_VOIPCSERV_FXS_TRANS       _CFG_VOIPCSERV(0x0010)	//T=u8, D=1, Call Transfer Option, 0:Restricted 1:Allowed
#define CSID_C_VOIPCSERV_FXS_FOR         _CFG_VOIPCSERV(0x0020)	//T=u8, D=1, Call Forward Option, 0:Restricted 1:Allowed
#define CSID_C_VOIPCSERV_FXS_FOR_TYPE    _CFG_VOIPCSERV(0x0021)	//T=u8, D=0, Call Forward TYPE, 0:none, 1:always 2:busy 3:no answer
#define CSID_C_VOIPCSERV_FXS_FOR_AUS     _CFG_VOIPCSERV(0x0030)	//T=str, Call Forward Always URI  SIP: @
#define CSID_C_VOIPCSERV_FXS_FOR_AIP     _CFG_VOIPCSERV(0x0040)	//T=ipv4, Call Forward Always URI
#define CSID_C_VOIPCSERV_FXS_FOR_BUS     _CFG_VOIPCSERV(0x0050)	//T=str, Call Forward on Busy URI  SIP: @
#define CSID_C_VOIPCSERV_FXS_FOR_BIP     _CFG_VOIPCSERV(0x0060)	//T=ipv4, Call Forward on Busy
#define CSID_C_VOIPCSERV_FXS_FOR_NUS     _CFG_VOIPCSERV(0x0070)	//T=str, Call Forward on NoAnswer URI  SIP: @
#define CSID_C_VOIPCSERV_FXS_FOR_NIP     _CFG_VOIPCSERV(0x0080)	//T=ipv4, Call Forward on NoAnswer
#define CSID_C_VOIPCSERV_FXS_FOR_NTO     _CFG_VOIPCSERV(0x0090)	//T=u8, D=3, Call Forward on NoAnswer Timeout (number of ring)
#define CSID_C_VOIPCSERV_FXS_FOR_NFTO    _CFG_VOIPCSERV(0x0098)	//T=u8, D=10, Call Forward on NoAnswer Direct Timeout
#define CSID_C_VOIPCSERV_FXS_DND         _CFG_VOIPCSERV(0x00a0)	//T=u8, D=0, Do Not disturb, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS_AutoAns     _CFG_VOIPCSERV(0x00b0)	//T=u8, D=0, Auto Answer, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS_AutoAnsTO   _CFG_VOIPCSERV(0x00c0)	//T=u8, Auto Answer Timeout (10..300)
#define CSID_C_VOIPCSERV_FXS_Hot         _CFG_VOIPCSERV(0x00cf)	//T=u8, D=0, Hot Line, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS_HotUS       _CFG_VOIPCSERV(0x00d0)	//T=str, Hot Line URI SIP:@
#define CSID_C_VOIPCSERV_FXS_HotIP       _CFG_VOIPCSERV(0x00e0)	//T=ipv4, Hot Line URI
#define CSID_C_VOIPCSERV_FXS_HOT_WTIME	 _CFG_VOIPCSERV(0x00e1) //T=u8, D=3, wait time before dial the Hot line number 
#define CSID_C_VOIPCSERV_FXS_Anony       _CFG_VOIPCSERV(0x00f0)	//T=u8, D=0, Anonymous Call, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS_RejAnony    _CFG_VOIPCSERV(0x0100)	//T=u8, D=0, Reject Anonymous Call Coming, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS_CallWaiting _CFG_VOIPCSERV(0x0110)	//T=u8, D=1, Call waiting, 0:Disable 1:Enable

// Line (FXO 0) 
#define CSID_C_VOIPCSERV_FXO_TRANS       _CFG_VOIPCSERV(0x0210)	//T=u8, D=1, Call Transfer Option, 0:Restricted 1:Allowed
#define CSID_C_VOIPCSERV_FXO_FOR         _CFG_VOIPCSERV(0x0220)	//T=u8, D=1, Call Forward Option, 0:Restricted 1:Allowed
#define CSID_C_VOIPCSERV_FXO_FOR_AUS     _CFG_VOIPCSERV(0x0230)	//T=str, Call Forward Always URI  SIP: @
#define CSID_C_VOIPCSERV_FXO_FOR_AIP     _CFG_VOIPCSERV(0x0240)	//T=ipv4, Call Forward Always URI
#define CSID_C_VOIPCSERV_FXO_FOR_BUS     _CFG_VOIPCSERV(0x0250)	//T=str, Call Forward on Busy URI  SIP: @
#define CSID_C_VOIPCSERV_FXO_FOR_BIP     _CFG_VOIPCSERV(0x0260)	//T=ipv4, Call Forward on Busy
#define CSID_C_VOIPCSERV_FXO_FOR_NUS     _CFG_VOIPCSERV(0x0270)	//T=str, Call Forward on NoAnswer URI  SIP: @
#define CSID_C_VOIPCSERV_FXO_FOR_NIP     _CFG_VOIPCSERV(0x0280)	//T=ipv4, Call Forward on NoAnswer
#define CSID_C_VOIPCSERV_FXO_FOR_NTO     _CFG_VOIPCSERV(0x0290)	//T=u8, D=30, Call Forward on NoAnswer Timeout (1..300)
#define CSID_C_VOIPCSERV_FXO_DND         _CFG_VOIPCSERV(0x02a0)	//T=u8, D=0, Do Not disturb, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXO_AutoAns     _CFG_VOIPCSERV(0x02b0)	//T=u8, D=0, Auto Answer, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXO_AutoAnsTO   _CFG_VOIPCSERV(0x02c0)	//T=u8, Auto Answer Timeout (10..300)
#define CSID_C_VOIPCSERV_FXO_Hot         _CFG_VOIPCSERV(0x02cf)	//T=u8, D=0, Hot Line, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXO_HotUS       _CFG_VOIPCSERV(0x02d0)	//T=str, Hot Line URI SIP:@
#define CSID_C_VOIPCSERV_FXO_HotIP       _CFG_VOIPCSERV(0x02e0)	//T=ipv4, Hot Line URI
#define CSID_C_VOIPCSERV_FXO_Anony       _CFG_VOIPCSERV(0x02f0)	//T=u8, D=0, Anonymous Call, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXO_RejAnony    _CFG_VOIPCSERV(0x0300)	//T=u8, D=0, Reject Anonymous Call Coming, 0:Disable 1:Enable


//@D2_csid
// the FXO above should be used for FXS2 in BDQ231
// we define new csid for FXS2 to avoic misunderstand
// Line (FXO 0) 
#define CSID_C_VOIPCSERV_FXS2_TRANS       _CFG_VOIPCSERV(0x0410)	//T=u8, D=1, Call Transfer Option, 0:Restricted 1:Allowed
#define CSID_C_VOIPCSERV_FXS2_FOR         _CFG_VOIPCSERV(0x0420)	//T=u8, D=1, Call Forward Option, 0:Restricted 1:Allowed
#define CSID_C_VOIPCSERV_FXS2_FOR_TYPE    _CFG_VOIPCSERV(0x0421)	//T=u8, D=0, Call Forward TYPE, 0:none, 1:always 2:busy 3:no answer
#define CSID_C_VOIPCSERV_FXS2_FOR_AUS     _CFG_VOIPCSERV(0x0430)	//T=str, Call Forward Always URI  SIP: @
#define CSID_C_VOIPCSERV_FXS2_FOR_AIP     _CFG_VOIPCSERV(0x0440)	//T=ipv4, Call Forward Always URI
#define CSID_C_VOIPCSERV_FXS2_FOR_BUS     _CFG_VOIPCSERV(0x0450)	//T=str, Call Forward on Busy URI  SIP: @
#define CSID_C_VOIPCSERV_FXS2_FOR_BIP     _CFG_VOIPCSERV(0x0460)	//T=ipv4, Call Forward on Busy
#define CSID_C_VOIPCSERV_FXS2_FOR_NUS     _CFG_VOIPCSERV(0x0470)	//T=str, Call Forward on NoAnswer URI  SIP: @
#define CSID_C_VOIPCSERV_FXS2_FOR_NIP     _CFG_VOIPCSERV(0x0480)	//T=ipv4, Call Forward on NoAnswer
#define CSID_C_VOIPCSERV_FXS2_FOR_NTO     _CFG_VOIPCSERV(0x0490)	//T=u8, D=3, Call Forward on NoAnswer Timeout (number of ring)
#define CSID_C_VOIPCSERV_FXS2_FOR_NFTO    _CFG_VOIPCSERV(0x0498)	//T=u8, D=10, Call Forward on NoAnswer Direct Timeout
#define CSID_C_VOIPCSERV_FXS2_DND         _CFG_VOIPCSERV(0x04a0)	//T=u8, D=0, Do Not disturb, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS2_AutoAns     _CFG_VOIPCSERV(0x04b0)	//T=u8, D=0, Auto Answer, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS2_AutoAnsTO   _CFG_VOIPCSERV(0x04c0)	//T=u8, Auto Answer Timeout (10..300)
#define CSID_C_VOIPCSERV_FXS2_Hot         _CFG_VOIPCSERV(0x04cf)	//T=u8, D=0, Hot Line, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS2_HotUS       _CFG_VOIPCSERV(0x04d0)	//T=str, Hot Line URI SIP:@
#define CSID_C_VOIPCSERV_FXS2_HotIP       _CFG_VOIPCSERV(0x04e0)	//T=ipv4, Hot Line URI
#define CSID_C_VOIPCSERV_FXS2_HOT_WTIME	  _CFG_VOIPCSERV(0x04e1)    //T=u8, D=3, wait time before dial the Hot line number 
#define CSID_C_VOIPCSERV_FXS2_Anony       _CFG_VOIPCSERV(0x04f0)	//T=u8, D=0, Anonymous Call, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS2_RejAnony    _CFG_VOIPCSERV(0x0500)	//T=u8, D=0, Reject Anonymous Call Coming, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS2_CallWaiting _CFG_VOIPCSERV(0x0510)	//T=u8, D=1, Call waiting, 0:Disable 1:Enable

//DECT 1
#define CSID_C_VOIPCSERV_FXS3_TRANS       _CFG_VOIPCSERV(0x0610)	//T=u8, D=1, Call Transfer Option, 0:Restricted 1:Allowed
#define CSID_C_VOIPCSERV_FXS3_FOR         _CFG_VOIPCSERV(0x0620)	//T=u8, D=1, Call Forward Option, 0:Restricted 1:Allowed
#define CSID_C_VOIPCSERV_FXS3_FOR_TYPE    _CFG_VOIPCSERV(0x0621)	//T=u8, D=0, Call Forward TYPE, 0:none, 1:always 2:busy 3:no answer
#define CSID_C_VOIPCSERV_FXS3_FOR_AUS     _CFG_VOIPCSERV(0x0630)	//T=str, Call Forward Always URI  SIP: @
#define CSID_C_VOIPCSERV_FXS3_FOR_AIP     _CFG_VOIPCSERV(0x0640)	//T=ipv4, Call Forward Always URI
#define CSID_C_VOIPCSERV_FXS3_FOR_BUS     _CFG_VOIPCSERV(0x0650)	//T=str, Call Forward on Busy URI  SIP: @
#define CSID_C_VOIPCSERV_FXS3_FOR_BIP     _CFG_VOIPCSERV(0x0660)	//T=ipv4, Call Forward on Busy
#define CSID_C_VOIPCSERV_FXS3_FOR_NUS     _CFG_VOIPCSERV(0x0670)	//T=str, Call Forward on NoAnswer URI  SIP: @
#define CSID_C_VOIPCSERV_FXS3_FOR_NIP     _CFG_VOIPCSERV(0x0680)	//T=ipv4, Call Forward on NoAnswer
#define CSID_C_VOIPCSERV_FXS3_FOR_NTO     _CFG_VOIPCSERV(0x0690)	//T=u8, D=3, Call Forward on NoAnswer Timeout (number of ring)
#define CSID_C_VOIPCSERV_FXS3_FOR_NFTO    _CFG_VOIPCSERV(0x0698)	//T=u8, D=10, Call Forward on NoAnswer Direct Timeout
#define CSID_C_VOIPCSERV_FXS3_DND         _CFG_VOIPCSERV(0x06a0)	//T=u8, D=0, Do Not disturb, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS3_AutoAns     _CFG_VOIPCSERV(0x06b0)	//T=u8, D=0, Auto Answer, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS3_AutoAnsTO   _CFG_VOIPCSERV(0x06c0)	//T=u8, Auto Answer Timeout (10..300)
#define CSID_C_VOIPCSERV_FXS3_Hot         _CFG_VOIPCSERV(0x06cf)	//T=u8, D=0, Hot Line, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS3_HotUS       _CFG_VOIPCSERV(0x06d0)	//T=str, Hot Line URI SIP:@
#define CSID_C_VOIPCSERV_FXS3_HotIP       _CFG_VOIPCSERV(0x06e0)	//T=ipv4, Hot Line URI
#define CSID_C_VOIPCSERV_FXS3_HOT_WTIME	  _CFG_VOIPCSERV(0x06e1)    //T=u8, D=3, wait time before dial the Hot line number 
#define CSID_C_VOIPCSERV_FXS3_Anony       _CFG_VOIPCSERV(0x06f0)	//T=u8, D=0, Anonymous Call, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS3_RejAnony    _CFG_VOIPCSERV(0x0700)	//T=u8, D=0, Reject Anonymous Call Coming, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS3_CallWaiting _CFG_VOIPCSERV(0x0710)	//T=u8, D=1, Call waiting, 0:Disable 1:Enable

//DECT 2
#define CSID_C_VOIPCSERV_FXS4_TRANS       _CFG_VOIPCSERV(0x0810)	//T=u8, D=1, Call Transfer Option, 0:Restricted 1:Allowed
#define CSID_C_VOIPCSERV_FXS4_FOR         _CFG_VOIPCSERV(0x0820)	//T=u8, D=1, Call Forward Option, 0:Restricted 1:Allowed
#define CSID_C_VOIPCSERV_FXS4_FOR_TYPE    _CFG_VOIPCSERV(0x0821)	//T=u8, D=0, Call Forward TYPE, 0:none, 1:always 2:busy 3:no answer
#define CSID_C_VOIPCSERV_FXS4_FOR_AUS     _CFG_VOIPCSERV(0x0830)	//T=str, Call Forward Always URI  SIP: @
#define CSID_C_VOIPCSERV_FXS4_FOR_AIP     _CFG_VOIPCSERV(0x0840)	//T=ipv4, Call Forward Always URI
#define CSID_C_VOIPCSERV_FXS4_FOR_BUS     _CFG_VOIPCSERV(0x0850)	//T=str, Call Forward on Busy URI  SIP: @
#define CSID_C_VOIPCSERV_FXS4_FOR_BIP     _CFG_VOIPCSERV(0x0860)	//T=ipv4, Call Forward on Busy
#define CSID_C_VOIPCSERV_FXS4_FOR_NUS     _CFG_VOIPCSERV(0x0870)	//T=str, Call Forward on NoAnswer URI  SIP: @
#define CSID_C_VOIPCSERV_FXS4_FOR_NIP     _CFG_VOIPCSERV(0x0880)	//T=ipv4, Call Forward on NoAnswer
#define CSID_C_VOIPCSERV_FXS4_FOR_NTO     _CFG_VOIPCSERV(0x0890)	//T=u8, D=3, Call Forward on NoAnswer Timeout (number of ring)
#define CSID_C_VOIPCSERV_FXS4_FOR_NFTO    _CFG_VOIPCSERV(0x0898)	//T=u8, D=10, Call Forward on NoAnswer Direct Timeout
#define CSID_C_VOIPCSERV_FXS4_DND         _CFG_VOIPCSERV(0x08a0)	//T=u8, D=0, Do Not disturb, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS4_AutoAns     _CFG_VOIPCSERV(0x08b0)	//T=u8, D=0, Auto Answer, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS4_AutoAnsTO   _CFG_VOIPCSERV(0x08c0)	//T=u8, Auto Answer Timeout (10..300)
#define CSID_C_VOIPCSERV_FXS4_Hot         _CFG_VOIPCSERV(0x08cf)	//T=u8, D=0, Hot Line, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS4_HotUS       _CFG_VOIPCSERV(0x08d0)	//T=str, Hot Line URI SIP:@
#define CSID_C_VOIPCSERV_FXS4_HotIP       _CFG_VOIPCSERV(0x08e0)	//T=ipv4, Hot Line URI
#define CSID_C_VOIPCSERV_FXS4_HOT_WTIME	  _CFG_VOIPCSERV(0x08e1)    //T=u8, D=3, wait time before dial the Hot line number 
#define CSID_C_VOIPCSERV_FXS4_Anony       _CFG_VOIPCSERV(0x08f0)	//T=u8, D=0, Anonymous Call, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS4_RejAnony    _CFG_VOIPCSERV(0x0900)	//T=u8, D=0, Reject Anonymous Call Coming, 0:Disable 1:Enable
#define CSID_C_VOIPCSERV_FXS4_CallWaiting _CFG_VOIPCSERV(0x0910)	//T=u8, D=1, Call waiting, 0:Disable 1:Enable

/* Status Items */

/* Multisession Items (start fromn 0x2000) */

#endif //ifndef _CSID_VOIPCSERV_H
