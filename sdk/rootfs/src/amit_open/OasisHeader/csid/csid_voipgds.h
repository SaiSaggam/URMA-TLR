/*
 * @file Member ID Definition of Group SIP General Dailing Setting
 *
 * Moderator: Terence Yang
 * Group ID: 0x002b0000/0x802b0000
 */

#ifndef _CSID_VOIPGDS_H
#define _CSID_VOIPGDS_H

#include "csid_gid.h"

#define _CFG_VOIPGDS(x)		(CSID_GIDC_VOIPGDS|(x))
#define _STA_VOIPGDS(x)		(CSID_GIDS_VOIPGDS|(x))

/* Extra Definitions */


/* Configuration Items */

// General Dialing Setting
#define CSID_C_VOIPGDS_Inter_TO        _CFG_VOIPGDS(0x0001)	//T=u8, D=4, Inter-digit Timeout   seconds (1..20, default:4)
#define CSID_C_VOIPGDS_First_TO        _CFG_VOIPGDS(0x0002)	//T=u8, D=16, First-digit Timeout  seconds (1..60, default:16)
#define CSID_C_VOIPGDS_AtdTrans        _CFG_VOIPGDS(0x0003)	//T=str, D="*02", Attended Transfer Code
#define CSID_C_VOIPGDS_ThreeWay        _CFG_VOIPGDS(0x0004)	//T=str, D="*71", Three Way Confernce Code
#define CSID_C_VOIPGDS_BlindTrans      _CFG_VOIPGDS(0x0005)	//T=str, D="*98", Blind Transfer Code
#define CSID_C_VOIPGDS_CWCancel        _CFG_VOIPGDS(0x0006)	//T=str, D="*70", Call Waiting Cancel (Per Call) Code
#define CSID_C_VOIPGDS_ForA_Act        _CFG_VOIPGDS(0x0007)	//T=str, D="*72", Forward Always Active Code
#define CSID_C_VOIPGDS_ForA_Dea        _CFG_VOIPGDS(0x0008)	//T=str, D="*73", Forward Always Deactive Code
#define CSID_C_VOIPGDS_ForB_Act        _CFG_VOIPGDS(0x0009)	//T=str, D="*90", Forward Busy Active Code
#define CSID_C_VOIPGDS_ForB_Dea        _CFG_VOIPGDS(0x000a)	//T=str, D="*91", Forward Busy Deactive Code
#define CSID_C_VOIPGDS_ForN_Act        _CFG_VOIPGDS(0x000b)	//T=str, D="*92", Forward Noanswer Active Code
#define CSID_C_VOIPGDS_ForN_Dea        _CFG_VOIPGDS(0x000c)	//T=str, D="*93", Forward Noanswer Deactive Code
#define CSID_C_VOIPGDS_DND_Act         _CFG_VOIPGDS(0x000d)	//T=str, D="*78", Do Not Disturb Active Code
#define CSID_C_VOIPGDS_DND_Dea         _CFG_VOIPGDS(0x000e)	//T=str, D="*79", Do Not Disturb Deactive Code
#define CSID_C_VOIPGDS_Return          _CFG_VOIPGDS(0x000f)	//T=str, D="*69", Call Return Code
#define CSID_C_VOIPGDS_Anoy_Act        _CFG_VOIPGDS(0x0010)	//T=str, D="*67", Anonymous Call/Block Caller ID Active Code
#define CSID_C_VOIPGDS_Anoy_Dea        _CFG_VOIPGDS(0x0011)	//T=str, D="*68", Anonymous Call/Block Caller ID Deactive Code
#define CSID_C_VOIPGDS_AnoyP_Act       _CFG_VOIPGDS(0x0012)	//T=str, D="*81", Anonymous Call/Block Caller ID (Per Call) Active Code
#define CSID_C_VOIPGDS_AnoyP_Dea       _CFG_VOIPGDS(0x0013)	//T=str, D="*82", Anonymous Call/Block Caller ID (Per Call) Deactive Code
#define CSID_C_VOIPGDS_AnoyR_Act       _CFG_VOIPGDS(0x0014)	//T=str, D="*77", Reject Anonymous call incoming Active Code
#define CSID_C_VOIPGDS_AnoyR_Dea       _CFG_VOIPGDS(0x0015)	//T=str, D="*87", Reject Anonymous call incoming Deactive Code
#define CSID_C_VOIPGDS_Repeat_Act      _CFG_VOIPGDS(0x0016)	//T=str, D="*66", Call Repeat/Call Back on Busy Active Code
#define CSID_C_VOIPGDS_Repeat_Dea      _CFG_VOIPGDS(0x0017)	//T=str, D="*86", Call Repeat/Call Back on Busy Deactive Code
#define CSID_C_VOIPGDS_Lastinc         _CFG_VOIPGDS(0x0018)	//T=str, D="*60", Screen Last Incoming Call Code
#define CSID_C_VOIPGDS_PSTN_Access     _CFG_VOIPGDS(0x0019)	//T=str, PSTN Access Code
#define CSID_C_VOIPGDS_Send            _CFG_VOIPGDS(0x001a)	//T=u8, D=1, Send # Enable (default:enabled)
#define CSID_C_VOIPGDS_ResetDefault    _CFG_VOIPGDS(0x001b)	//T=str, D="*47991", Reset to Default Code
//@D2_csid
#define CSID_C_VOIPGDS_DND_PC_Act      _CFG_VOIPGDS(0x001e)	//T=str, D="*74", Do Not Disturb Per Call Basis Active Code
#define CSID_C_VOIPGDS_Redial          _CFG_VOIPGDS(0x0020)	//T=str, D="*68", Redial Code
#define CSID_C_CALL_FEATURE_ENABLE     _CFG_VOIPGDS(0x0021)     //T=u8, D=1, Enable/Disable Call Feature function

/* Status Items */

/* Multisession Items (start fromn 0x2000) */

#endif //ifndef _CSID_VOIPGDS_H
