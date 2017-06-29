/*
 * @file Member ID Definition of Group SPAP.
 *
 * Moderator: JackChiang
 * Group ID: 0x00170000/0x80170000
 */

#ifndef _CSID_SPAP_H
#define _CSID_SPAP_H

#include "csid_gid.h"

#define _CFG_SPAP(x)			(CSID_GIDC_SPAP|(x))
#define _STA_SPAP(x)			(CSID_GIDS_SPAP|(x))

#define SPAP_NUM    12

/* Configuration Items */
#define CSID_C_SPAP_ENABLE              _CFG_SPAP(0x0001)   //T=u8, enable the rule , 1:yes 0:no
#define CSID_C_SPAP_TRIGGER_PORT        _CFG_SPAP(0x0011)   //T=str, spap trigger port
#define CSID_C_SPAP_INCOMING_PORT       _CFG_SPAP(0x0021)   //T=str, spap incoming port
#define CSID_C_SPAP_RULE_NAME           _CFG_SPAP(0x0031)   //T=str, rule name
#define CSID_C_SPAP_RULE_BOUND_WAN      _CFG_SPAP(0x0041)   //T=u8, rule binds to WAN, 0:all 99:local 1-16:multi
#define CSID_C_SPAP_RECORD_RULENUM      _CFG_SPAP(0x0051)   //T=str, record rule number for ZyXEL UI

#define CSID_C_SPAP_FUNCTION_DISABLE    _CFG_SPAP(0x00F1)   //T=u8, enable/disable the spap function, 0: enable 1: disable

#define CSID_C_SPAP_RULE_SCHE_NO        _CFG_SPAP(0x0100)   //T=u16, schedule number of using rule
#define CSID_C_SPAP_PROTOCOL            _CFG_SPAP(0x0200)   //T=u8, forward ptotocol, 0:BOTH 1:TCP 2:UDP
#define CSID_C_SPAP_INCOMING_PROTOCOL   _CFG_SPAP(0x0300)   //T=u8, incoming ptotocol, 0:Ditto 1:TCP 2:UDP 3:BOTH

/*select index*/
#define CSID_C_SPAP_TRIGGER_INDEX   _CFG_SPAP(0x0400)   //T=u32, D=0, for ui save trigger port index 

/* Multi-WAN Selection for spap */
#define CSID_C_SPAP_WAN_LIST        _CFG_SPAP(0x0500)   //T=u32, WAN List for spap, 0: All, 1: WAN1, 2: WAN2, 3: WAN1~2, etc...

/* Status Items */
#define CSID_S_SPAP_ALTERED             _STA_SPAP(0x0001)   //T=u8, ui altered
#define CSID_S_SPAP_STATUS              _STA_SPAP(0x0002)   //T=u8, 0:off, 1:on

#endif //ifndef _CSID_SPAP_H
