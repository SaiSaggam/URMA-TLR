/*
 * @file Member ID Definition of Group AZLINK
 *
 * Moderator: 
 * Group ID: 0x00950000/0x80950000
 */

#ifndef _CSID_AZLINK_H
#define _CSID_AZLINK_H

#include "csid_gid.h"

#define _CFG_AZLINK(x)		(CSID_GIDC_AZLINK|(x))
#define _STA_AZLINK(x)		(CSID_GIDS_AZLINK|(x))

/* Extra Definitions */
#define AZLINK_CNT_TYPE_ONDEMAND		0x00
#define AZLINK_CNT_TYPE_AUTORECONNECT	0x01
#define AZLINK_CNT_TYPE_MANUAL			0x02

#define AZLINK_CNT_STATUS_DISCONNECT	0x00
#define AZLINK_CNT_STATUS_CONNECTING	0x01
#define AZLINK_CNT_STATUS_CONNECT		0x02
#define AZLINK_CNT_STATUS_DISCONNECTING	0x03
#define AZLINK_CNT_STATUS_WAIT_TRAFFIC	0x05
#define AZLINK_CNT_STATUS_AUTH_FAIL		0x06


/* Configuration Items */
#define CSID_C_AZLINK_CNT_TYPE_UI 			_CFG_AZLINK(0x0001)	//T=u8, connect type, Dial-on-Demand, autoconnect...
#define CSID_C_AZLINK_MTU 			        _CFG_AZLINK(0x0002)	//T=u32, Tunel MTU
#define CSID_C_AZLINK_IDLE_TIME 			_CFG_AZLINK(0x0003)	//T=u32, Idle time for AZ Link
#define CSID_C_AZLINK_NAT_DISABLE        	_CFG_AZLINK(0x000f)	//T=u32, AZLINK wantype NAT disable
#define CSID_C_AZLINK_USER           	  	_CFG_AZLINK(0x0100)	//T=str, user name
#define CSID_C_AZLINK_PASSWORD       		_CFG_AZLINK(0x0200)	//T=str, user password
#define CSID_C_AZLINK_IF_IP          		_CFG_AZLINK(0x0300)	//T=ipv4, static interface ip
#define CSID_C_AZLINK_SERVICENAME    		_CFG_AZLINK(0x0600)	//T=str, service name
#define CSID_C_AZLINK_IF_PRIDNS      		_CFG_AZLINK(0x0700)	//T=ipv4, static interface primary DNS
#define CSID_C_AZLINK_IF_SECDNS       		_CFG_AZLINK(0x0800)	//T=ipv4, static interface secondary DNS
#define CSID_C_AZLINK_CNT_TYPE        		_CFG_AZLINK(0x0900)	//T=u32, connect type, Dial-on-Demand, autoconnect...


/* Status Items */
#define CSID_S_AZLINK_CNT_TIME        			_STA_AZLINK(0x0100)	//T=u32, connect time
#define CSID_S_AZLINK_IF_IP           			_STA_AZLINK(0x0200)	//T=ipv4, dynamic interface ip
#define CSID_S_AZLINK_IF_NM           			_STA_AZLINK(0x0300)	//T=ipv4, dynamic interface netmask
#define CSID_S_AZLINK_IF_GW           			_STA_AZLINK(0x0400)	//T=ipv4, dynamic interface gateway

#define CSID_S_AZLINK_IF_PRIDNS       			_STA_AZLINK(0x0500)	//T=ipv4, dynamic interface primary DNS
#define CSID_S_AZLINK_IF_SECDNS       			_STA_AZLINK(0x0600)	//T=ipv4, dynamic interface secondary DNS
#define CSID_S_AZLINK_CNT_STATUS      			_STA_AZLINK(0x0700)	//T=u32, connect status
#define CSID_S_AZLINK_PKTIN_NUM       			_STA_AZLINK(0x0800)	//T=u32, number of packet input
#define CSID_S_AZLINK_PKTOUT_NUM      			_STA_AZLINK(0x0900)	//T=u32, number of packet output
#define CSID_S_AZLINK_WAN_IF_NAME				_STA_AZLINK(0x2200)	//T=str, tun

#define CSID_S_AZLINK_ALTERED         			_STA_AZLINK(0x0A00)	//T=u32, ui altered


#define CSID_S_AZLINK_INTERFACE    				_STA_AZLINK(0x0B00)	//T=str, service name


#endif //ifndef _CSID_AZLINK_H
