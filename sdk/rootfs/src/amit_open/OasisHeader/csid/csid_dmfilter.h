/*
 * @file Member ID Definition of Group DMFILTER.
 *
 * Moderator: JasonJiang
 * Group ID: 0x000F0000/0x800F0000
 */

#ifndef _CSID_DMFILTER_H
#define _CSID_DMFILTER_H

#include "csid_gid.h"

#define _CFG_DMFILTER(x)         (CSID_GIDC_DMFILTER|(x))
#define _STA_DMFILTER(x)         (CSID_GIDS_DMFILTER|(x))

/* Configuration Items */

//Domain Filter
#define CSID_C_DMFILTER_ENABLE				_CFG_DMFILTER(0x0001)	//T=u8, whether starting domain-filter, 1:yes 0:no
#define CSID_C_DMFILTER_LOG_ENABLE			_CFG_DMFILTER(0x0002)	//T=u8, whether starting log-function, 1:yes, 0:no
#define CSID_C_PRIVILEGE_IP0				_CFG_DMFILTER(0x0003)	//T=u8, start of privilege ip 
#define CSID_C_PRIVILEGE_IP1				_CFG_DMFILTER(0x0004)   //T=u8, end of privilege ip 
#define CSID_C_DMFILTER_OTHERS_ACTION_DROP	_CFG_DMFILTER(0x0005)   //T=u8, the action of others is drop
#define CSID_C_DMFILTER_OTHERS_ACTION_LOG	_CFG_DMFILTER(0x0006)   //T=u8, the action of others is log
#define CSID_C_DMFILTER_OTHERS_SCHE_NO	    _CFG_DMFILTER(0x0007)   //T=u8, number of using rule

#define CSID_C_DMFILTER_RULE_SUFFIX			_CFG_DMFILTER(0x1000)	//T=str, domain suffix
#define CSID_C_DMFILTER_RULE_ACTION_DROP	_CFG_DMFILTER(0x2000)	//T=u8, drop, 1:enable 0:disable
#define CSID_C_DMFILTER_RULE_ACTION_LOG		_CFG_DMFILTER(0x3000)	//T=u8, log, 1:enable 0:disbale
#define CSID_C_DMFILTER_RULE_ENABLE			_CFG_DMFILTER(0x4000)	//T=u8, enable, 1:yes 0:no
#define CSID_C_DMFILTER_RULE_SCHE_NO        _CFG_DMFILTER(0x5000)   //T=u16, number of using rule


/* Status Items */ 

#define CSID_S_DMFILTER_ALTERED			    _STA_DMFILTER(0x0001)	//T=u8, ui altered
#define CSID_S_DMFILTER_OTHERS_SCHE_STATUS	_STA_DMFILTER(0x0002)   //T=u8, schedule rule status for others
#define CSID_S_DMFILTER_RULE_SCHE_STATUS    _STA_DMFILTER(0x0100)   //T=u8, schedule rules status 

#endif //ifndef _CSID_DMFILTER_H
