/*
 * @file Member ID Definition of Group VoIP Dial Plan
 *
 * Moderator: Terence Yang
 * Group ID: 0x002c0000/0x802c0000
 */

#ifndef _CSID_VOIPDP_H
#define _CSID_VOIPDP_H

#include "csid_gid.h"

#define _CFG_VOIPDP(x)		(CSID_GIDC_VOIPDP|(x))
#define _STA_VOIPDP(x)		(CSID_GIDS_VOIPDP|(x))

/* Extra Definitions */

#define Display_Before_Ring         0x00
#define Display_After_Ring          0x01

#define App_Port_All                0x00
#define App_Port_FXS                0x01
#define App_Port_FXO                0x02

#define Policy_DialImm              0x00	//Dial Immediately
#define Policy_PrefixSub            0x01	//Prefix Substitute
#define Policy_DialPSTN             0x02	//Dial to PSTN
#define Policy_PrefixPSTN           0x03	//Prefix Substitue and Dial to PSTN

#define Dial_PSTN_FXO               0x00

/* Configuration Items */

// Dial Plan  
#define CSID_C_VOIPDP_App_Port        _CFG_VOIPDP(0x0010)	//T=u8, Applied Port
#define CSID_C_VOIPDP_Digital_Seq     _CFG_VOIPDP(0x0020)	//T=str, Digit Sequence
#define CSID_C_VOIPDP_Policy          _CFG_VOIPDP(0x0030)	//T=u8, Policy
#define CSID_C_VOIPDP_PrfSub_old      _CFG_VOIPDP(0x0040)	//T=str, Prefix Substitute, old prefix
#define CSID_C_VOIPDP_PrfSub_new      _CFG_VOIPDP(0x0050)	//T=str, Prefix Substitute, new prefix
#define CSID_C_VOIPDP_Dial_PSTN       _CFG_VOIPDP(0x0060)	//T=u8, Dial to PSTN
#define CSID_C_VOIPDP_Note            _CFG_VOIPDP(0x0070)	//T=str, Note
#define CSID_C_VOIPDP_DialPlan_ENABLE _CFG_VOIPDP(0x007f)	//T=u8, D=0, DialPlan: 0:Disable 1:Enable
#define CSID_C_VOIPDP_DialPlan        _CFG_VOIPDP(0x0080)	//T=str, D="", User defined DialPlan
#define CSID_C_VOIPDP_DialPlan_FXS2   _CFG_VOIPDP(0x0090)	//T=str, D="", User defined DialPlan
#define CSID_C_VOIPDP_DialPlan_FXS3   _CFG_VOIPDP(0x0091)	//T=str, D="", User defined DialPlan
#define CSID_C_VOIPDP_DialPlan_FXS4   _CFG_VOIPDP(0x0092)	//T=str, D="", User defined DialPlan

/* Status Items */

/* Multisession Items (start fromn 0x2000) */

#endif //ifndef _CSID_VOIPDP_H
