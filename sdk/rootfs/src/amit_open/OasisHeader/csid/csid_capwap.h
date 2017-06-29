/*
 * @file Member ID Definition that the CAPWAP protocol.
 *
 * Moderator: Derek
 * Group ID: 0x009B0000/0x809B0000
 */

#ifndef _CSID_CAPWAP_H
#define _CSID_CAPWAP_H

#include "csid_gid.h"

#define _CFG_CAPWAP(x)       (CSID_GIDC_CAPWAP|(x))
#define _STA_CAPWAP(x)       (CSID_GIDS_CAPWAP|(x))

//CAPWAP UI
#define CSID_S_CAPWAP_ALTERED					_STA_CAPWAP(0x0000) //T=u32, ui altered of CAPWAP

//CAPWAP Configure
#define CSID_C_CAPWAP_EN						_CFG_CAPWAP(0x0000) //T=u32, D=0, 1: CAPWAP Enable,
#define CSID_C_CAPWAP_CONTROLLER				_CFG_CAPWAP(0x0001) //T=u32, D=0, 1:Controller, 0:WTP

//CAPWAP WTP Configure
#define CSID_C_CAPWAP_WTP_ACADDR			_CFG_CAPWAP(0x0002) //T=ipv4, D=255.255.255.255
#define CSID_C_CAPWAP_WTP_DEV_NAME			_CFG_CAPWAP(0x0003) //T=str, D=MY WTP
#define CSID_C_CAPWAP_WTP_DEV_CERT			_CFG_CAPWAP(0x0004) //T=str,
#define CSID_C_CAPWAP_WTP_VAP				_CFG_CAPWAP(0x0005) //T=str, D=rai0

//CAPWAP AC Management WTP LIST
#define CSID_C_CAPWAP_WTP_NAME_LIST			_CFG_CAPWAP(0x0100) //T=str, WTP name
#define CSID_C_CAPWAP_WTP_STATUS_LIST		_CFG_CAPWAP(0x0200) //T=u32, D=0, WTP status
#define CSID_C_CAPWAP_WTP_MAC_LIST			_CFG_CAPWAP(0x0300) //T=mac, Managent WTP MAC


#define CSID_S_CAPWAP_WTP_NAME_LIST			_STA_CAPWAP(0x0100) //T=str, WTP name
#define CSID_S_CAPWAP_WTP_STATUS_LIST		_STA_CAPWAP(0x0200) //T=u32, D=0, WTP status
#define CSID_S_CAPWAP_WTP_MAC_LIST			_STA_CAPWAP(0x0300) //T=mac, Managent WTP MAC

#endif //ifndef _CSID_CAPWAP


