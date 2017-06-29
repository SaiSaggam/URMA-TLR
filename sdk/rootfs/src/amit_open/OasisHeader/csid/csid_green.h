/*
 * @file Member ID Definition of Group Green Router Function.
 *
 * Moderator: Eric Chen
 * Group ID: 0x00400000/0x80400000
 */

#ifndef _CSID_GREEN_H
#define _CSID_GREEN_H

#include "csid_gid.h"

#define _CFG_GREEN(x)        (CSID_GIDC_GREEN|(x))		//0x00400000
#define _STA_GREEN(x)        (CSID_GIDS_GREEN|(x))		//0x80400000

/* Define MAX Rules number and LOG number */

#define MAX_RULES           32
#define TIME_LOG_MAX        10
#define TIME_LOG_LENGTH     25


/* Default Policy of Rule */

#define SMART_POLICY_FORCE				0
#define SMART_RULE_DETECT_FLOW			1

/* Bit-mapped Weekdays */

#define RULE_EX_DAYS_MASK               0x7F

#define RULE_EX_DAYS_NONE				0x00
#define RULE_EX_DAYS_SUNDAY				0x01
#define RULE_EX_DAYS_MONDAY				0x02
#define RULE_EX_DAYS_TUESDAY			0x04
#define RULE_EX_DAYS_WEDNESDAY			0x08
#define RULE_EX_DAYS_THURSDAY			0x10
#define RULE_EX_DAYS_FRIDAY				0x20
#define RULE_EX_DAYS_SATURDAY			0x40
#define RULE_EX_DAYS_EVERYDAY			0x7F

/* Configuration Items */ 

#define CSID_C_GREEN_HW_SUPPORT			_CFG_GREEN(0x0001)	//T=u32, D=1, wheather Green function support or not
#define CSID_C_GREEN_STANDBY_ENABLE		_CFG_GREEN(0x0002)	//T=u32, wheather wireless standby mode is enabled or not
#define CSID_C_GREEN_SLEEP_ENABLE		_CFG_GREEN(0x0003)	//T=u32, wheather system sleep mode is enabled or not
#define CSID_C_GREEN_STANDBY_TIME		_CFG_GREEN(0x0004)	//T=u32, wait how many minutes enter Standby mode

/* Max 1024 rules */

#define GREEN_RULE_CSID_GAP				0x10				//gap of csid between each rule
#define CSID_C_GREEN_RULE_SMART_ENABLE	_CFG_GREEN(0x0400)	//T=u32, smart green functio enable or not
#define CSID_C_GREEN_RULE_SMART_POLICY	_CFG_GREEN(0x0800)	//T=u32, 0=force off, 1=detect flow

/* 8 EXceptions per rule */

#define CSID_C_GREEN_RULE_POWEROFF_DAYS	_CFG_GREEN(0x2000)	//T=u32, bit-mapped weekdays of exception (poweroff)
#define CSID_C_GREEN_RULE_POWEROFF_TIME	_CFG_GREEN(0x4000)	//T=u32, Power off time
#define CSID_C_GREEN_RULE_POWERON_DAYS	_CFG_GREEN(0x6000)	//T=u32, bit-mapped weekdays of exception (poweron)
#define CSID_C_GREEN_RULE_POWERON_TIME	_CFG_GREEN(0x8000)	//T=u32, Power on time

/* Last ten Bootup/Sleep Time */

#define CSID_C_GREEN_BOOTUP_TIME		_CFG_GREEN(0xA000)	//T=str, system bootup time
#define CSID_C_GREEN_SLEEP_TIME			_CFG_GREEN(0xA100)	//T=str, system sleep time

/* Status Items */

#define CSID_S_GREEN_ALTERED			_STA_GREEN(0x0001)	//T=u32, setting is altered

#endif //ifndef _CSID_GREEN_H
