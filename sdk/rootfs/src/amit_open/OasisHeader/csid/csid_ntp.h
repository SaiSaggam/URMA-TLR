/*
 * @file Member ID Definition of Group NTP
 * 
 * This group is for not only NTP (RFC-1305) but also other protocols, 
 * likes Time Protocol (RFC-868) and Daytime Protocol (RFC-867).
 *
 * Moderator: RupertLi
 * Group ID: 0x00070000/0x80070000
 */

#ifndef _CSID_NTP_H
#define _CSID_NTP_H

#include "csid_gid.h"

#define _CFG_NTP(x)     (CSID_GIDC_NTP|(x))
#define _STA_NTP(x)     (CSID_GIDS_NTP|(x))

/* synchronization status */

#define NTP_SYNC_STATUS_IDLE		0	//idle or just started
#define NTP_SYNC_STATUS_BUSY		1	//sync process is in progress
#define NTP_SYNC_STATUS_OK			2	//last sync completed and OK
#define NTP_SYNC_STATUS_FAIL		3	//last sync completed and FAIL

/* Configuration Items */

#define CSID_C_NTP_TZ_ENVAR         _CFG_NTP(0x0001) //T=str, TZ environment var
#define CSID_C_NTP_TIMEZONE_ID      _CFG_NTP(0x0002) //T=u16, value in time zone selecting options
#define CSID_C_NTP_CURRENT_YEAR     _CFG_NTP(0x0003) //current year
#define CSID_C_NTP_CURRENT_MONTH    _CFG_NTP(0x0004) //current month
#define CSID_C_NTP_CURRENT_DAY      _CFG_NTP(0x0005) //current day
#define CSID_C_NTP_CURRENT_HOUR     _CFG_NTP(0x0006) //current hour
#define CSID_C_NTP_CURRENT_MIN      _CFG_NTP(0x0007) //current min
#define CSID_C_NTP_AUTO_SYNC_ENABLE _CFG_NTP(0x0008) //T=u8, auto-sync enable
#define CSID_C_NTP_CURRENT_SEC      _CFG_NTP(0x0009) //current sec
#define CSID_C_NTP_SCH_NOCHK_SYNC   _CFG_NTP(0x000a) //T=u8, D=0, Schedule run before check NTP status (0:check, 1:no check)
#define CSID_C_NTP_SYNC_TIME        _CFG_NTP(0x000b) //T=u32, D=0(mean 43200 sec), Next sync time
//reserve 16 time servers
#define CSID_C_NTP_SERVER_NAME      _CFG_NTP(0x0010) //T=str, time server name
#define CSID_C_NTP_SERVER_NAME_1    _CFG_NTP(0x0011) //T=str, time server name 1
#define CSID_C_NTP_SERVER_NAME_2    _CFG_NTP(0x0012) //T=str, time server name 2
#define CSID_C_NTP_SERVER_NAME_3    _CFG_NTP(0x0013) //T=str, time server name 3
#define CSID_C_NTP_SERVER_NAME_4    _CFG_NTP(0x0014) //T=str, time server name 4
#define CSID_C_NTP_SERVER_NAME_5    _CFG_NTP(0x0015) //T=str, time server name 5
#define CSID_C_NTP_SERVER_NAME_6    _CFG_NTP(0x0016) //T=str, time server name 6
#define CSID_C_NTP_SERVER_NAME_7    _CFG_NTP(0x0017) //T=str, time server name 7
#define CSID_C_NTP_SERVER_NAME_8    _CFG_NTP(0x0018) //T=str, time server name 8
#define CSID_C_NTP_SERVER_NAME_9    _CFG_NTP(0x0019) //T=str, time server name 9 
#define CSID_C_NTP_SERVER_NAME_A    _CFG_NTP(0x001A) //T=str, time server name 10
#define CSID_C_NTP_SERVER_NAME_B    _CFG_NTP(0x001B) //T=str, time server name 11
#define CSID_C_NTP_SERVER_NAME_C    _CFG_NTP(0x001C) //T=str, time server name 12
#define CSID_C_NTP_SERVER_NAME_D    _CFG_NTP(0x001D) //T=str, time server name 13
#define CSID_C_NTP_SERVER_NAME_E    _CFG_NTP(0x001E) //T=str, time server name 14
#define CSID_C_NTP_SERVER_NAME_F    _CFG_NTP(0x001F) //T=str, time server name 15

//daylight setting
#define CSID_C_NTP_DAYLIGHT_MODE         _CFG_NTP(0x0021) //T=u8, D=0, Disable/
#define CSID_C_NTP_DAYLIGHT_START_MONTH  _CFG_NTP(0x0022) //T=u32, daylight start month
#define CSID_C_NTP_DAYLIGHT_END_MONTH    _CFG_NTP(0x0023) //T=u32, daylight end month
#define CSID_C_NTP_DAYLIGHT_START_DAY    _CFG_NTP(0x0024) //T=u32, daylight start day
#define CSID_C_NTP_DAYLIGHT_END_DAY      _CFG_NTP(0x0025) //T=u32, daylight end day
#define CSID_C_NTP_DAYLIGHT_START_HOUR   _CFG_NTP(0x0026) //T=u32, daylight start hour
#define CSID_C_NTP_DAYLIGHT_END_HOUR     _CFG_NTP(0x0027) //T=u32, daylight end hour

#define CSID_C_NTP_DAYLIGHT_START_WEEK	    _CFG_NTP(0x0028) //T=u32, daylight start week(1,2..5)
#define CSID_C_NTP_DAYLIGHT_END_WEEK   	    _CFG_NTP(0x0029) //T=u32, daylight end week
#define CSID_C_NTP_DAYLIGHT_START_DAYWEEK   _CFG_NTP(0x0030) //T=u32, daylight start day of week(0.1..6)
#define CSID_C_NTP_DAYLIGHT_END_DAYWEEK   	_CFG_NTP(0x0031) //T=u32, daylight end day of week

/* set date & time manually */
#define CSID_C_NTP_MANUALLY_YEAR        _CFG_NTP(0x0040) //T=str, year set manually
#define CSID_C_NTP_MANUALLY_MONTH       _CFG_NTP(0x0041) //T=str, month set manually
#define CSID_C_NTP_MANUALLY_DAY         _CFG_NTP(0x0042) //T=str, day set manually
#define CSID_C_NTP_MANUALLY_HOUR        _CFG_NTP(0x0043) //T=str, hour set manually
#define CSID_C_NTP_MANUALLY_MINUTE      _CFG_NTP(0x0044) //T=str, minute set manually
#define CSID_C_NTP_MANUALLY_SECOND      _CFG_NTP(0x0045) //T=str, second set manually

/* Status Items */
#define CSID_S_NTP_ALTERED          _STA_NTP(0x0001) //T=u8, ui altered
#define CSID_S_NTP_SYNC_STATUS      _STA_NTP(0x0002) //T=u8, synchronization status
#define CSID_S_PCTIME_SYNC_ALTERED	_STA_NTP(0x0003) //T=u32, setting is altered
#define CSID_S_NTP_RTC_STATUS		_STA_NTP(0x0004) //T=u8, RTC synchronization status
#define CSID_S_NTP_CHECK_ENABLE     _STA_NTP(0x0005) //T=u8, synchronization check 0:disable 1:enable
#define CSID_S_NTP_FIRST_SUCCESS    _STA_NTP(0x0006) //T=u8, synchronization first success

#endif //ifndef _CSID_NTP_H
