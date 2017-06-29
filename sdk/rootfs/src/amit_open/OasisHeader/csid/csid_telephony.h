/*
   * @file Member ID Definition of Group  Telephony Service
   *
   * Moderator: JasonJiang
   * Group ID: 0x00800000/0x80800000
*/

#ifndef _CSID_TELEPHONY_H
#define _CSID_TELEPHONY_H

#include "csid_gid.h"

#define _CFG_TELEPHONY(x)       (CSID_GIDC_TELEPHONY|(x))
#define _STA_TELEPHONY(x)       (CSID_GIDS_TELEPHONY|(x))

//callerID type
#define CALLER_ID_FSK_RING_PLUSE 1
#define CALLER_ID_DTMF 2
//special key
#define SPECIAL_KEY_PROMPT	1
#define SPECIAL_KEY_FLASH	2
#define SPECIAL_KEY_STAR	3

/* Configuration Items */
#define CSID_C_TELEPHONY_CALLER_ID        		_CFG_TELEPHONY(0x0001) //T=u8, callerID type 
#define CSID_C_TELEPHONY_DIALING_TIMEOUT        _CFG_TELEPHONY(0x0002) //T=u16, dialing timeout.
#define CSID_C_TELEPHONY_IMMEDIATE_DIALING		_CFG_TELEPHONY(0x0003) //T=u8, immidiate dialing, enable:1,diable:0 
#define CSID_C_TELEPHONY_CALL_LOG				_CFG_TELEPHONY(0x0004) //T=u8, call log. enable:1,diable:0
#define CSID_C_TELEPHONY_CALL_FORWARDING_ALL	_CFG_TELEPHONY(0x0005) //T=u8, unregister:0, register:1, activate:2, deactivate:3
#define CSID_C_TELEPHONY_CALL_FORWARDING_NOANS	_CFG_TELEPHONY(0x0006) //T=u8, unregister:0, register:1, activate:2, deactivate:3
#define CSID_C_TELEPHONY_CALL_FORWARDING_UNREACHABLE  _CFG_TELEPHONY(0x0007) //T=u8, unregister:0, register:1, activate:2, deactivate:3
#define CSID_C_TELEPHONY_CALL_FORWARDING_BUSY	_CFG_TELEPHONY(0x0008) //T=u8, unregister:0, register:1, activate:2, deactivate:3
#define CSID_C_TELEPHONY_CALL_FORWARDING_ALL_NUM    _CFG_TELEPHONY(0x0009) //T=str, phone number
#define CSID_C_TELEPHONY_CALL_FORWARDING_NOANS_NUM  _CFG_TELEPHONY(0x000A) //T=str, phone number
#define CSID_C_TELEPHONY_CALL_FORWARDING_UNREACHABLE_NUM  _CFG_TELEPHONY(0x000B) //T=str, phone number
#define CSID_C_TELEPHONY_CALL_FORWARDING_BUSY_NUM   _CFG_TELEPHONY(0x000C) //T=str, phone number
#define CSID_C_TELEPHONY_CALL_WAITING_ENABLE	_CFG_TELEPHONY(0x000D) //T=u8, enable:1 , diable:0
#define CSID_C_TELEPHONY_CALL_WAITING_SPECIAL_KEY	_CFG_TELEPHONY(0x000E) //T=u8, the definition of special key. (*,#,flash)
#define CSID_C_TELEPHONY_CALL_LOG_CLEAN_OUTGOING	_CFG_TELEPHONY(0x000F) //T=u8, clean the outgoing call log
#define CSID_C_TELEPHONY_CALL_LOG_CLEAN_INCOMING	_CFG_TELEPHONY(0x0010) //T=u8, clean the incoming call log
#define CSID_C_TELEPHONY_CALL_LOG_CLEAN_MISSED		_CFG_TELEPHONY(0x0011) //T=u8, clean the missed call log
#define CSID_C_TELEPHONY_PHONE_SUPPORT              _CFG_TELEPHONY(0x0012) //T=u8, support/enable the telephony capability 
#define CSID_C_TELEPHONY_TALKING_OR_NOT             _CFG_TELEPHONY(0x0013) //T=u8, talking or not, enable:1,diable:0
#define CSID_C_TELEPHONY_TALKING_TIME               _CFG_TELEPHONY(0x0014) //T=u32, D=0, talking time
#define CSID_C_TELEPHONY_FIRST_CALL		              _CFG_TELEPHONY(0x0015) //T=u32, D=0, record it's first time call or not,1:open OB-DTMF
#define CSID_C_TELEPHONY_CALL_FORWARDING_ENABLE		  _CFG_TELEPHONY(0x0016) //T=u8, D=0, call forward switch
#define CSID_C_TELEPHONY_RESTRICT_OUTGOING_CALL		  _CFG_TELEPHONY(0x0017) //T=u8, D=0, enable:1 , diable:0, restrict outgoing call switch

#define CSID_C_TELEPHONY_AREA_CODE_ENABLE						_CFG_TELEPHONY(0x0018) //T=u8, D=0, enable:1 , diable:0, add area code in the beginning
#define CSID_C_TELEPHONY_AREA_CODE									_CFG_TELEPHONY(0x0019) //T=str, area code

#define CSID_C_TELEPHONY_CALL_FORWARDING_ALL_CONDITIONAL	      _CFG_TELEPHONY(0x001A) //T=u8, unregister:0, register:1, activate:2, deactivate:3
#define CSID_C_TELEPHONY_CALL_FORWARDING_ALL_CONDITIONAL_NUM    _CFG_TELEPHONY(0x001B) //T=str, phone number

#define CSID_C_TELEPHONY_SPEED_DIAL_ENABLE			_CFG_TELEPHONY(0x0040) //T=u8, enable:1. disable:0
#define CSID_C_TELEPHONY_SPEED_DIAL_NUM				_CFG_TELEPHONY(0x0051) //T=str, number of speed dial.
#define CSID_C_TELEPHONY_PROFILE					_CFG_TELEPHONY(0x0061) //T=u8, phone profile
#define CSID_C_TELEPHONY_CALL_FORWARDING_NOANS_WAIT_TIME        _CFG_TELEPHONY(0x0062) //T=u8, waiting time for call forwarding of no answer

#define CSID_C_TELEPHONY_EMERGENCYCALL_ENABLE					_CFG_TELEPHONY(0x0070) //T=u8, D=0, enable:1 , diable:0, emergency call enable switch

#define CSID_C_TELEPHONY_CALL_LOG_OUTGOING_COUNT    _CFG_TELEPHONY(0x0080)  //T=u8, the count of outgoing call log
#define CSID_C_TELEPHONY_CALL_LOG_INCOMING_COUNT    _CFG_TELEPHONY(0x0081)  //T=u8, the count of incoming call log
#define CSID_C_TELEPHONY_CALL_LOG_MISSED_COUNT      _CFG_TELEPHONY(0x0082)  //T=u8, the count of missed call log

#define CSID_C_TELEPHONY_CALL_LOG_OUTGOING_TABLE_START  _CFG_TELEPHONY(0x0090)  ///T=u8, the start address of outgoing call log
#define CSID_C_TELEPHONY_CALL_LOG_INCOMING_TABLE_START  _CFG_TELEPHONY(0x0091)  ///T=u8, the start address of incoming call log
#define CSID_C_TELEPHONY_CALL_LOG_MISSED_TABLE_START    _CFG_TELEPHONY(0x0092)  ///T=u8, the start address of missed call log

#define CSID_C_TELEPHONY_CALL_BARR_ALLBARRING						_CFG_TELEPHONY(0x00A0)	//T=u8, D=0, enable:1 , diable:0, All calls are barred
#define CSID_C_TELEPHONY_CALL_BARR_ALLOUTGOING					_CFG_TELEPHONY(0x00A1)	//T=u8, D=0, enable:1 , diable:0, All outgoing
#define CSID_C_TELEPHONY_CALL_BARR_OUTGOINGINT					_CFG_TELEPHONY(0x00A2)	//T=u8, D=0, enable:1 , diable:0, Outgoing internal
#define CSID_C_TELEPHONY_CALL_BARR_OUTGOINGINTEXTOHOME	_CFG_TELEPHONY(0x00A3)	//T=u8, D=0, enable:1 , diable:0, Outgoing external to home
#define CSID_C_TELEPHONY_CALL_BARR_ALLINCOMING					_CFG_TELEPHONY(0x00A4)	//T=u8, D=0, enable:1 , diable:0, All incoming
#define CSID_C_TELEPHONY_CALL_BARR_INCOMINGROAMING			_CFG_TELEPHONY(0x00A5)	//T=u8, D=0, enable:1 , diable:0, Roaming incoming

#define CSID_C_TELEPHONY_CLIP			_CFG_TELEPHONY(0x00B0)	//T=u8, D=0, enable:1 , diable:0
#define CSID_C_TELEPHONY_CLIR			_CFG_TELEPHONY(0x00B1)	//T=u8, D=0, enable:1 , diable:0
#define CSID_C_TELEPHONY_COLP			_CFG_TELEPHONY(0x00B2)	//T=u8, D=0, enable:1 , diable:0
#define CSID_C_TELEPHONY_COLR			_CFG_TELEPHONY(0x00B3)	//T=u8, D=0, enable:1 , diable:0

#define CSID_C_TELEPHONY_MUTE_TX_NUMBER			_CFG_TELEPHONY(0x00C0)	//T=str, if dial this number, mute TX don't send in-band DTMF

#define CSID_C_TELEPHONY_OUTGOING_CALL_NUM      _CFG_TELEPHONY(0x100)   //T=str, phone number of this outgoing call
#define CSID_C_TELEPHONY_OUTGOING_CALL_ST       _CFG_TELEPHONY(0x200)   //T=str, start time of this call
#define CSID_C_TELEPHONY_OUTGOING_CALL_ET       _CFG_TELEPHONY(0x300)   //T=str, end time of this call
#define CSID_C_TELEPHONY_OUTGOING_CALL_DURATION     _CFG_TELEPHONY(0x400)   //T=str, duration of this call

#define CSID_C_TELEPHONY_INCOMING_CALL_NUM      _CFG_TELEPHONY(0x500)   //T=str, phone number of this incoming call
#define CSID_C_TELEPHONY_INCOMING_CALL_ST       _CFG_TELEPHONY(0x600)   //T=str, start time of this call
#define CSID_C_TELEPHONY_INCOMING_CALL_ET       _CFG_TELEPHONY(0x700)   //T=str, end time of this call
#define CSID_C_TELEPHONY_INCOMING_CALL_DURATION     _CFG_TELEPHONY(0x800)   //T=str, duration of this call

#define CSID_C_TELEPHONY_MISSED_CALL_NUM        _CFG_TELEPHONY(0x900)   //T=str, phone number of this missed call
#define CSID_C_TELEPHONY_MISSED_CALL_TIME       _CFG_TELEPHONY(0xA00)   //T=str, the  time of this missed call arrival

#define CSID_C_TELEPHONY_OB_DTMF       			_CFG_TELEPHONY(0xB00)   //T=u8, D=0, enable out-of-band dtmf signaling 
#define CSID_C_TELEPHONY_SLIC_RESET_ENABLE  _CFG_TELEPHONY(0xB01)   //T=u32, D=0, enable slic reset 
#define CSID_C_TELEPHONY_SLIC_RESET_TIMEER  _CFG_TELEPHONY(0xB02)   //T=u32, D=0, enable slic reset time

#define CSID_C_TELEPHONY_VO3G_ENABLE    _CFG_TELEPHONY(0xC00) //T=u32, D=0, enable = 1, disable = 0



/* Status Items */
#define CSID_S_TELEPHONY_ALTERED					_STA_TELEPHONY(0x0001)	//T=u8, telephony,except call fwd altered.
#define CSID_S_TELEPHONY_CALL_FWD_ALTERED			_STA_TELEPHONY(0x0002)	//T=u8, call forward page altered.
#define CSID_S_TELEPHONY_CF_ALL_STATUS				_STA_TELEPHONY(0x0003) 	//T=str, status of setting all call forwarding
#define CSID_S_TELEPHONY_CF_NOANS_STATUS			_STA_TELEPHONY(0x0004) 	//T=str, status of setting noanswer forwarding
#define CSID_S_TELEPHONY_CF_UNREACHABLE_STATUS		_STA_TELEPHONY(0x0005) 	//T=str, status of unreachable forwarding
#define CSID_S_TELEPHONY_CF_BUSY_STATUS				_STA_TELEPHONY(0x0006) 	//T=str, status of busy forwardin

#define CSID_S_TELEPHONY_CALL_LOG_OUTGOING_CLEAN	_STA_TELEPHONY(0x0010)	//T=u8, the to clean outgoing call log
#define CSID_S_TELEPHONY_CALL_LOG_INCOMING_CLEAN	_STA_TELEPHONY(0x0011)	//T=u8, the to clean  incoming call log
#define CSID_S_TELEPHONY_CALL_LOG_MISSED_CLEAN		_STA_TELEPHONY(0x0012)	//T=u8, the to clean missed call log
#define CSID_S_TELEPHONY_CF_QUERY_STATUS		_STA_TELEPHONY(0x0013)	//T=u8, call forwarding query status
#define CSID_S_TELEPHONY_ON_TALKING_STATUS		_STA_TELEPHONY(0x0014)	//T=u8, call on talking status
#define CSID_S_TELEPHONY_DTMF_TXM_MUTE_INTERVAL		_STA_TELEPHONY(0x0015)	//T=u32, DTMF TXM mute interval
#define CSID_S_TELEPHONY_ONHOOK_STATUS		_STA_TELEPHONY(0x0016)	//T=u8, onhook status

#define CSID_S_TELEPHONY_NEW_SMS_WHEN_TALKING      _STA_TELEPHONY(0x0017)  //T=u8, a new sms when talking
#define CSID_S_TELEPHONY_SIM_PHB_USED_NUM  			    _STA_TELEPHONY(0x0018)  //T=u32, used record of SIM phone book
#define CSID_S_TELEPHONY_SIM_PHB_CAPACITY_NUM	      _STA_TELEPHONY(0x0019)  //T=u32, phone book capacity of SIM
#define CSID_S_TELEPHONY_TALKING_OR_NOT             _STA_TELEPHONY(0x001A)	//T=u8, talking or not, enable:1,diable:0

#define CSID_S_TELEPHONY_CW_QUERY_STATUS		_STA_TELEPHONY(0x001B)	//T=u8, call waiting query status, querying:1,diable:0
#define CSID_S_TELEPHONY_CW_STATUS		    _STA_TELEPHONY(0x001C)	//T=u8, D=0, call waiting status, enable:1,diable:2

#define CSID_S_TELEPHONY_SIM_PHB_NAME								_STA_TELEPHONY(0x0100)  //T=str, SIM phone book name, reserve 256
#define CSID_S_TELEPHONY_SIM_PHB_NUMBER							_STA_TELEPHONY(0x0200)  //T=str, SIM phone book number, reserve 256
#define CSID_S_TELEPHONY_FDN_NAME										_STA_TELEPHONY(0x0100)  //T=str, SIM phone book name, reserve 256
#define CSID_S_TELEPHONY_FDN_NUMBER									_STA_TELEPHONY(0x0100)  //T=str, SIM phone book name, reserve 256

#define CSID_S_TELEPHONY_VO3G_STATUS       _STA_TELEPHONY(0xB00)   //T=u32, Vo3G status
#define CSID_S_TELEPHONY_VO3G_OUTCALL_STATUS _STA_TELEPHONY(0xC00)  //T=u32, outcalling = 1
#define CSID_S_TELEPHONY_SLIC_INIT_STATUS       _STA_TELEPHONY(0xD00)   //T=u32, initial = 1

/*
#define CSID_S_TELEPHONY_CALL_LOG_OUTGOING_COUNT	_STA_TELEPHONY(0x0020)	//T=u8, the count of outgoing call log
#define CSID_S_TELEPHONY_CALL_LOG_INCOMING_COUNT	_STA_TELEPHONY(0x0021)	//T=u8, the count of incoming call log
#define CSID_S_TELEPHONY_CALL_LOG_MISSED_COUNT		_STA_TELEPHONY(0x0022)	//T=u8, the count of missed call log

#define CSID_S_TELEPHONY_CALL_LOG_OUTGOING_TABLE_START  _STA_TELEPHONY(0x0030)  ///T=u8, the start address of outgoing call log
#define CSID_S_TELEPHONY_CALL_LOG_INCOMING_TABLE_START  _STA_TELEPHONY(0x0031)  ///T=u8, the start address of incoming call log
#define CSID_S_TELEPHONY_CALL_LOG_MISSED_TABLE_START    _STA_TELEPHONY(0x0032)  ///T=u8, the start address of missed call log

#define CSID_S_TELEPHONY_OUTGOING_CALL_NUM		_STA_TELEPHONY(0x100)	//T=str, phone number of this outgoing call
#define CSID_S_TELEPHONY_OUTGOING_CALL_ST		_STA_TELEPHONY(0x200)	//T=str, start time of this call
#define CSID_S_TELEPHONY_OUTGOING_CALL_ET		_STA_TELEPHONY(0x300)	//T=str, end time of this call
#define CSID_S_TELEPHONY_OUTGOING_CALL_DURATION		_STA_TELEPHONY(0x400)	//T=str, duration of this call

#define CSID_S_TELEPHONY_INCOMING_CALL_NUM      _STA_TELEPHONY(0x500)   //T=str, phone number of this incoming call
#define CSID_S_TELEPHONY_INCOMING_CALL_ST       _STA_TELEPHONY(0x600)   //T=str, start time of this call
#define CSID_S_TELEPHONY_INCOMING_CALL_ET       _STA_TELEPHONY(0x700)   //T=str, end time of this call
#define CSID_S_TELEPHONY_INCOMING_CALL_DURATION     _STA_TELEPHONY(0x800)   //T=str, duration of this call

#define CSID_S_TELEPHONY_MISSED_CALL_NUM      	_STA_TELEPHONY(0x900)   //T=str, phone number of this missed call
#define CSID_S_TELEPHONY_MISSED_CALL_TIME       _STA_TELEPHONY(0xA00)   //T=str, the  time of this missed call arrival
*/

#endif

