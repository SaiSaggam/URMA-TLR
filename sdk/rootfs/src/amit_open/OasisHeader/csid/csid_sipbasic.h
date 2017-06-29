/*
 * @file Member ID Definition of Group SIP Basic
 *
 * Moderator: Terence Yang
 * Group ID: 0x00210000/0x80210000
 */

#ifndef _CSID_SIPBASIC_H
#define _CSID_SIPBASIC_H

#include "csid_gid.h"

#define _CFG_SIPBASIC(x)		(CSID_GIDC_SIPBASIC|(x))
#define _STA_SIPBASIC(x)		(CSID_GIDS_SIPBASIC|(x))

/* Extra Definitions */

/* Configuration Items */

#define CSID_C_SIPBASIC_STFLAG          _CFG_SIPBASIC(0x0001)	//T=u8, D=0, session timer flag, 0:no 1:yes
#define CSID_C_SIPBASIC_STIMER          _CFG_SIPBASIC(0x0002)	//T=u16, D=1800, session timer, 90-65535
#define CSID_C_SIPBASIC_MPORTST         _CFG_SIPBASIC(0x0003)	//T=u16, D=5000, media port start, 1024-65535
#define CSID_C_SIPBASIC_MPORTED         _CFG_SIPBASIC(0x0006)	//T=u16, D=5009, media port end, 1024-65535
#define CSID_C_SIPBASIC_TRANSPORT       _CFG_SIPBASIC(0x0007)	//T=u8, D=0, Transport, 0:udp 1:tcp
#define CSID_C_SIPBASIC_SIPTIME         _CFG_SIPBASIC(0x0008)	//T=u16, D=500, SIP Time Interval, 100-1000
#define CSID_C_SIPBASIC_TIMEOUT_I       _CFG_SIPBASIC(0x0009)	//T=u8, D=12, Timeout for Invite, 1-100
#define CSID_C_SIPBASIC_TIMEOUT_RB      _CFG_SIPBASIC(0x000a)	//T=u8, D=180, Timeout for Ring Back, 1-300
#define CSID_C_SIPBASIC_TIMEOUT_RE      _CFG_SIPBASIC(0x000b)	//T=u8, D=4, Timeout for Release, 1-10
#define CSID_C_SIPBASIC_REG_RETRY_C     _CFG_SIPBASIC(0x000c)	//T=u16, D=65535, Registration Retry Count, 1-65535
#define CSID_C_SIPBASIC_REG_RETRY_I     _CFG_SIPBASIC(0x000d)	//T=u16, D=30, Registration Retry Interval, 1-65535
#define CSID_C_SIPBASIC_PING            _CFG_SIPBASIC(0x000e)	//T=u16, D=0, PING Interval, 0-65535
#define CSID_C_SIPBASIC_PRACK           _CFG_SIPBASIC(0x000f)	//T=u8, D=0, PRACK Flag, 0:disable 1:enable
#define CSID_C_SIPBASIC_SIPUA           _CFG_SIPBASIC(0x0010)   //T=str, D="VOIP_Agent_001", SIP User Agent Name

#define CSID_C_SIPBASIC_FXSPORT         _CFG_SIPBASIC(0x0011)	//T=u16, D=5060, FXS SIP Source Port, 1-65535
#define CSID_C_SIPBASIC_FXOPORT         _CFG_SIPBASIC(0x0012)	//T=u16, D=5060, FXO SIP Source Port, 1-65535
#define CSID_C_SIPBASIC_SIPDCSP         _CFG_SIPBASIC(0x0013)	//T=u8, D=0, DSCP for SIP packet, 0-63
#define CSID_C_SIPBASIC_RTPDSCP         _CFG_SIPBASIC(0x0014)	//T=u8, D=0, DSCP for RTP packet, 0-63
#define CSID_C_SIPBASIC_RTCPDSCP        _CFG_SIPBASIC(0x0015)	//T=u8, D=0, DSCP for RTCP packet, 0-63

/* Status Items */

/* Multisession Items (start fromn 0x2000) */

#endif //ifndef _CSID_SIPBASIC_H
