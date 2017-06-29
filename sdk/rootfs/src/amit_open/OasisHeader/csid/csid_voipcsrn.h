/*
 * @file Member ID Definition of Group VoIP Call Screen
 *
 * Moderator: Terence Yang
 * Group ID: 0x002e0000/0x802e0000
 */

#ifndef _CSID_VOIPCSRN_H
#define _CSID_VOIPCSRN_H

#include "csid_gid.h"

#define _CFG_VOIPCSRN(x)		(CSID_GIDC_VOIPCSRN|(x))
#define _STA_VOIPCSRN(x)		(CSID_GIDS_VOIPCSRN|(x))

/* Extra Definitions */

/* Configuration Items */

// Call Screen
// Line 0 (FXS 0)
#define CSID_C_VOIPCSRN_FXS_RejIn_U     _CFG_VOIPCSRN(0x0010)	//T=str, Reject Incoming Phone URI Action User
#define CSID_C_VOIPCSRN_FXS_RejIn_A     _CFG_VOIPCSRN(0x0020)	//T=ipv4, Reject Incoming Phone URI Action Address
#define CSID_C_VOIPCSRN_FXS_RejOut_U    _CFG_VOIPCSRN(0x0030)	//T=str, Reject Outgoing Phone URI Action User
#define CSID_C_VOIPCSRN_FXS_RejOut_A    _CFG_VOIPCSRN(0x0040)	//T=ipv4, Reject Outgoing Phone URI Action Address

// Line 1 (FXO 0)
#define CSID_C_VOIPCSRN_FXO_RejIn_U     _CFG_VOIPCSRN(0x0050)	//T=str, Reject Incoming Phone URI Action User
#define CSID_C_VOIPCSRN_FXO_RejIn_A     _CFG_VOIPCSRN(0x0060)	//T=ipv4, Reject Incoming Phone URI Action Address
#define CSID_C_VOIPCSRN_FXO_RejOut_U    _CFG_VOIPCSRN(0x0070)	//T=str, Reject Outgoing Phone URI Action User
#define CSID_C_VOIPCSRN_FXO_RejOut_A    _CFG_VOIPCSRN(0x0080)   //T=ipv4, Reject Outgoing Phone URI Action Address
/* Status Items */

/* Multisession Items (start fromn 0x2000) */

#endif //ifndef _CSID_VOIPCSRN_H
