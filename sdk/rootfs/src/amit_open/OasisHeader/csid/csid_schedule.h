/*
 * @file Member ID Definition of Group SCHEDULE.
 *
 * Moderator: JasonJiang/RupertLi
 * Group ID: 0x000B0000/0x800B0000
 */

#ifndef _CSID_SCHEDULE_H
#define _CSID_SCHEDULE_H

#include "csid_gid.h"

#define _CFG_SCHEDULE(x)        (CSID_GIDC_SCHEDULE|(x))
#define _STA_SCHEDULE(x)        (CSID_GIDS_SCHEDULE|(x))

/* Default Policy of Rule */

#define RULE_POLICY_INACTIVATE          0
#define RULE_POLICY_ACTIVATE            1

/* Bit-mapped Weekdays */

#define RULE_EX_DAYS_MASK               0x7F

#define RULE_EX_DAYS_NONE               0x00
#define RULE_EX_DAYS_SUNDAY             0x01
#define RULE_EX_DAYS_MONDAY             0x02
#define RULE_EX_DAYS_TUESDAY            0x04
#define RULE_EX_DAYS_WEDNESDAY          0x08
#define RULE_EX_DAYS_THURSDAY           0x10
#define RULE_EX_DAYS_FRIDAY             0x20
#define RULE_EX_DAYS_SATURDAY           0x40
#define RULE_EX_DAYS_EVERYDAY           0x7F

/* Configuration Items */ 

#define CSID_C_SCHEDULE_ENABLE          _CFG_SCHEDULE(0x0001) //T=u8, wheather scheduling is enabled or not
#define CSID_C_SCHEDULE_RULE_GAP        _CFG_SCHEDULE(0x0002) //T=u32, if GAP = 0 then use RULE_CSID_GAP

/* Max 1024 rules */

#define RULE_CSID_GAP                   0x400                 //gap of csid between each rule

#define CSID_C_APSWITCH_USE_RULE_NO     _CFG_SCHEDULE(0x0100) //T=u8, 2.4G apswitch use rule number
#define CSID_C_SCHEDULE_APACTIVE_MODE   _CFG_SCHEDULE(0x0101) //T=u8, D=0, 0:Follow RULE_POLICY by UI, 1:Force active in schedule, 2:Force inactive in schedule
#define CSID_C_REBOOT_USE_RULE_NO       _CFG_SCHEDULE(0x0200) //T=u8, reboot use rule number
#define CSID_C_REBOOT_USE_DAILY       	_CFG_SCHEDULE(0x0210) //T=u8, D=0,reboot use daily
#define CSID_C_AP2SWITCH_USE_RULE_NO    _CFG_SCHEDULE(0x0300) //T=u8, 5G apswitch use rule number
#define CSID_C_SCHEDULE_AP2ACTIVE_MODE  _CFG_SCHEDULE(0x0301) //T=u8, D=0, 0:Follow RULE_POLICY by UI, 1:Force active in schedule, 2:Force inactive in schedule
#define CSID_C_SCHEDULE_RULE_NAME       _CFG_SCHEDULE(0x0400) //T=str, name of schedule rule

#define CSID_C_SCHEDULE_RULE_POLICY     _CFG_SCHEDULE(0x0C00) //T=u8, rule is to inactivate or activate by default

/* 8 EXceptions per rule */
#define CSID_C_SCHEDULE_RULE_DAY_FORMAT _CFG_SCHEDULE(0x1000) //T=u32, D=0,Time format,0: 24hours,1: 12hours
#define CSID_C_SCHEDULE_RULE_EX_DAYS    _CFG_SCHEDULE(0x2000) //T=u32, bit-mapped weekdays of exception
#define CSID_C_SCHEDULE_RULE_EX_START   _CFG_SCHEDULE(0x4000) //T=u32, start time of exception
#define CSID_C_SCHEDULE_RULE_EX_STOP    _CFG_SCHEDULE(0x6000) //T=u32, stop time of exception
#define CSID_C_SCHEDULE_REBOOT_ENABLE	_CFG_SCHEDULE(0x7000) //T=u8, reboot device by schedule
#define CSID_C_SCHEDULE_REBOOT_DONE	_CFG_SCHEDULE(0x8000) //T=u8, reboot done by schedule
#define CSID_C_SCHEDULE_RULE_EX_DATE    _CFG_SCHEDULE(0x9000) //T=u32, date of exception

/* Status Items */
#define CSID_S_SCHEDULE_ALTERED         _STA_SCHEDULE(0x0001) //T=u8, D=0,setting is altered
#define CSID_S_SCHEDULE_CHECK 			_STA_SCHEDULE(0x0002) //T=u8, D=0,etting is altered
#endif //ifndef _CSID_SCHEDULE_H
