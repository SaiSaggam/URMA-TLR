/*
 * @file Member ID Definition of DECT Phone.
 *
 * Moderator: 
 * Group ID: 0x00950000/0x80950000
 */
 
#ifndef _CSID_DECT_H
#define _CSID_DECT_H

#include "csid_gid.h"

#define _CFG_DECT(x)        (CSID_GIDC_DECT|(x))
#define _STA_DECT(x)        (CSID_GIDS_DECT|(x))

#define REGIST_OPEN 1
#define REGIST_CLOSE 2
#define REGIST_DEL 3
#define CONFIG_CHG 4
#define REGIST_DEL_SINGLE 5
#define DECT_RING_TEST_START 6
#define DECT_RING_TEST_STOP 7
#define DECT_PIN_CODE_SET 8
#define DECT_ALL_PAGE_OPEN 9
#define DECT_ALL_PAGE_CLOSE 10

#define DECT_PHONE_STATUS_NORMAL		0x00
#define DECT_PHONE_STATUS_READY			0x01
#define DECT_PHONE_STATUS_DEREGISTER		0x02

/* Configuration Items */

#define CSID_C_DECT_ENABLE          		_CFG_DECT(0x0001) //T=u8, D=1, wheather DECT is enabled or not
#define CSID_C_DECT_CONSOLE_ENABLE          _CFG_DECT(0x0002) //T=u8, D=0, wheather DECT console enabled or not
#define CSID_C_DECT_STATUS          		_CFG_DECT(0x0010) //T=u8, D=0, DECT MODULE Status
#define CSID_C_DECT_PHONE_NUM          		_CFG_DECT(0x0011) //T=u8, D=0, How many DECT Phones can be used.
#define CSID_C_DECT_PHONE_GROUP          	_CFG_DECT(0x0020) //T=u8, Which GROUP DECT Phone 1 belongs to.
#define CSID_C_DECT_PHONE_STATUS          	_CFG_DECT(0x0030) //T=u8, D=0, DECT PHONE Status : 0 for unregister , 1 for registered.
#define CSID_C_DECT_PHONE_STATUS1          	_CFG_DECT(0x0031) //T=u8, D=0, DECT PHONE Status
#define CSID_C_DECT_PHONE_STATUS2         	_CFG_DECT(0x0032) //T=u8, D=0, DECT PHONE Status
#define CSID_C_DECT_PHONE_STATUS3         	_CFG_DECT(0x0033) //T=u8, D=0, DECT PHONE Status
#define CSID_C_DECT_DEL_ALL_HANDSETS            _CFG_DECT(0x0034) //T=u8, D=0, Delete all registered handsets
#define CSID_C_DECT_RFPI                        _CFG_DECT(0x0035) //T=str, Record DECT module RFPI
#define CSID_C_DECT_FXS_NUM                     _CFG_DECT(0x0036) //T=u8, D=0, 3:FXS+2*DECT
#define CSID_C_DECT_PIN_CODE                    _CFG_DECT(0x0037) //T=str, DECT PIN code
#define CSID_C_DECT_PAGE_ENABLE                 _CFG_DECT(0x0038) //T=u8, D=0, 0:disable 1:enable
#define CSID_C_DECT_REG_ENABLE                  _CFG_DECT(0x0039) //T=u8, D=0, 0:disable 1:enable
/*state */

#define CSID_S_DECT_CON_ALTERED      _STA_DECT(0x0001) //T=u16, D=0,condition altered.
#define CSID_S_DECT_CMD				 _STA_DECT(0x0002) //T=u16, D=0,Set 1 to enable handset registration.
#define CSID_S_DECT_DE_NUMBER 		 _STA_DECT(0x0003) //T=u16, D=0,if CMD = REGIST_DEL_SINGLE, get this number to derigster that phone.
#define CSID_S_DECT_CALL_WAITING  	 _STA_DECT(0x0004) //T=u16, D=0,if 1 dect is call_waiting.
#define CSID_S_DECT_FW_UP_STATUS  	 _STA_DECT(0x0010) //T=u16, D=0,if 0 fail ,1 start ,2 success
#define CSID_S_DECT_FW_FILE_NAME  	 _STA_DECT(0x0011) //T=str, firmware upgrade file name
#define CSID_S_DECT_UPGRADE_ALERT  	 _STA_DECT(0x0120) //T=u16, D=0,upgrade alert
#define CSID_S_DECT_BASE_STATION_STATUS  _STA_DECT(0x0121) //T=u8, D=0, 0:normal 1:register open 2:Handset Page

#endif
