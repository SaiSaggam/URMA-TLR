/*
 * @file Member ID Definition of Group I/O Management.
 *
 * Moderator: Mike
 * Group ID: 0x003B0000/0x803B0000
 */

#ifndef _CSID_IOMGT_H
#define _CSID_IOMGT_H

#include "csid_gid.h"

#define _CFG_IOMGT(x)			(CSID_GIDC_IOMGT|(x))
#define _STA_IOMGT(x)			(CSID_GIDS_IOMGT|(x))

#define IOMGT_EVENT_NONE 	0
#define IOMGT_EVENT_DI 		1
#define IOMGT_EVENT_SMS 	2
#define IOMGT_EVENT_PWRCHG 	3
#define IOMGT_EVENT_MODBUS 	4

#define IOMGT_HANDLER_NONE 	0
#define IOMGT_HANDLER_DO 	1
#define IOMGT_HANDLER_SMS 	2
#define IOMGT_HANDLER_SYSLOG 	3
#define IOMGT_HANDLER_SNMP 	4
#define IOMGT_HANDLER_EMAIL 	5
#define IOMGT_HANDLER_REBOOT 	6
#define IOMGT_HANDLER_MODBUS 	7

#define MODBUS_CMP_NONE 	0
#define MODBUS_CMP_GT 		1
#define MODBUS_CMP_EQ 		2
#define MODBUS_CMP_LT 		3

#define IOMGT_RULE_NUM 		16

/* Configuration Items */
#define CSID_C_IOMGT_ENABLE 			_CFG_IOMGT(0x0001) //T=u8, D=0, enable event handler


#define CSID_C_IOMGT_RULE_ENABLE 		_CFG_IOMGT(0x0100) //T=u8, D=0, control each rule of event handler
#define CSID_C_IOMGT_EVENT_TYPE 		_CFG_IOMGT(0x0200) //T=u8, D=0, event type, default none
#define CSID_C_IOMGT_HANDLER_TYPE 		_CFG_IOMGT(0x0300) //T=u8, D=0, handler type, default none
#define CSID_C_IOMGT_RULE_SCHE_NO 		_CFG_IOMGT(0x0400) //T=u16, number of schedule rule


#define CSID_C_IOMGT_EVENT_SMS_MSG 		_CFG_IOMGT(0x3000) //T=str, D="", message to trigger handler

#define CSID_C_IOMGT_EVENT_MODBUS_DEV 		_CFG_IOMGT(0x3100) //T=u32, D=0, device ID for comparison
#define CSID_C_IOMGT_EVENT_MODBUS_REG 		_CFG_IOMGT(0x3200) //T=u32, D=0, register for comparison
#define CSID_C_IOMGT_EVENT_MODBUS_CMP 		_CFG_IOMGT(0x3300) //T=u8, D=0, logic comparator for comparison
#define CSID_C_IOMGT_EVENT_MODBUS_THR 		_CFG_IOMGT(0x3400) //T=u32, threshold for comparison


#define CSID_C_IOMGT_HANDLER_SMS_PNO 		_CFG_IOMGT(0x6000) //T=str, D="", phone number
#define CSID_C_IOMGT_HANDLER_SMS_MSG 		_CFG_IOMGT(0x6100) //T=str, D="", message

#define CSID_C_IOMGT_HANDLER_SYSLOG_SVR 	_CFG_IOMGT(0x6200) //T=u32, D=0, server
#define CSID_C_IOMGT_HANDLER_SYSLOG_MSG 	_CFG_IOMGT(0x6300) //T=str, D="", message

#define CSID_C_IOMGT_HANDLER_SNMP_TRIP_INDEX 	_CFG_IOMGT(0x7000) //T=u32, D=0, snmp trap receiver, get IP from CSID_C_SNMP_TRIP_IP, value is from 1, 2, 3, 4
#define CSID_C_IOMGT_HANDLER_SNMP_MSG 		_CFG_IOMGT(0x6400) //T=str, D="", snmp trap message

#define CSID_C_IOMGT_HANDLER_EMAIL_SVR 		_CFG_IOMGT(0x6500) //T=u32, D=0, server
#define CSID_C_IOMGT_HANDLER_EMAIL_ADD 		_CFG_IOMGT(0x6600) //T=str, D="", address
#define CSID_C_IOMGT_HANDLER_EMAIL_MSG 		_CFG_IOMGT(0x6700) //T=str, D="", message

#define CSID_C_IOMGT_HANDLER_MODBUS_DEV 	_CFG_IOMGT(0x6800) //T=u32, D=0, device ID
#define CSID_C_IOMGT_HANDLER_MODBUS_REG 	_CFG_IOMGT(0x6900) //T=u32, D=0, register
#define CSID_C_IOMGT_HANDLER_MODBUS_VAL 	_CFG_IOMGT(0x6A00) //T=u32, value used to set

/* Status Items */
#define CSID_S_IOMGT_ALTERED             	_STA_IOMGT(0x0001) //T=u8, ui altered
#define CSID_S_IOMGT_CUR_POWER                  _STA_IOMGT(0x0011)  //T=u32, D=0, current power. 1: power 1; 2: power 2


#endif //ifndef _CSID_IOMGT_H
