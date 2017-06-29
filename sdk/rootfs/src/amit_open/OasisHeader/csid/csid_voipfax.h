/*
 * @file Member ID Definition of Group VoSIP FAX setting
 *
 * Moderator: Terence Yang
 * Group ID: 0x002a0000/0x802a0000
 */

#ifndef _CSID_VOIPFAX_H
#define _CSID_VOIPFAX_H

#include "csid_gid.h"

#define _CFG_VOIPFAX(x)		(CSID_GIDC_VOIPFAX|(x))
#define _STA_VOIPFAX(x)		(CSID_GIDS_VOIPFAX|(x))

/* Extra Definitions */

#define T38_Voice_T38_Relay         0x00	//Voice and T.38 FAX Relay
#define T38_Voice_Pass_Through      0x01	//Voice and FAX Pass Through

#define Tone_Caller                 0x00
#define Tone_Callee                 0x01
#define Tone_Caller_or_Callee       0x02

/* Configuration Items */

// FAX Setting
// Line 0 (FXS 0)
#define CSID_C_VOIPFAX_T38             _CFG_VOIPFAX(0x0010)	//T=u8, D=1, T.38 Option
#define CSID_C_VOIPFAX_ToneDecMode     _CFG_VOIPFAX(0x0020)	//T=u8, D=2, FAX Tone Detection Mode


// Line 1 (FXS 1)
#define CSID_C_VOIPFAX_T38_FXS2           _CFG_VOIPFAX(0x0110)	//T=u8, D=1, T.38 Option
#define CSID_C_VOIPFAX_ToneDecMode_FXS2   _CFG_VOIPFAX(0x0120)	//T=u8, D=2, FAX Tone Detection Mode

/* Status Items */

/* Multisession Items (start fromn 0x2000) */

#endif //ifndef _CSID_VOIPFAX_H
