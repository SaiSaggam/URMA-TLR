/*
 * @file Member ID Definition of Group IBURST
 *
 * Moderator: CharlesTu
 * Group ID: 0x00340000/0x80340000
 */

#ifndef _CSID_IBURST_H
#define _CSID_IBURST_H

#include "csid_gid.h"

#define _CFG_IBURST(x)		(CSID_GIDC_IBURST|(x))
#define _STA_IBURST(x)		(CSID_GIDS_IBURST|(x))

/* Extra Definitions */
#define IBURST_CNT_TYPE_ONDEMAND		0x00
#define IBURST_CNT_TYPE_AUTORECONNECT	0x01
#define IBURST_CNT_TYPE_MANUAL			0x02

#define IBURST_AUTHTYPE_AUTO			0x00
#define IBURST_AUTHTYPE_PAP				0x01
#define IBURST_AUTHTYPE_CHAP			0x02
#define IBURST_AUTHTYPE_MSCHAP			0x04
#define IBURST_AUTHTYPE_MSCHAPV2		0x08
#define IBURST_AUTHTYPE_EAP				0x10

#define IBURST_CNT_STATUS_DISCONNECT	0x00
#define IBURST_CNT_STATUS_CONNECTING	0x01
#define IBURST_CNT_STATUS_CONNECT		0x02
#define IBURST_CNT_STATUS_DISCONNECTING	0x03
#define IBURST_CNT_STATUS_WAIT_TRAFFIC	0x05
#define IBURST_CNT_STATUS_AUTH_FAIL		0x06
#define IBURST_CNT_STATUS_BACKUP		0x07

/* Configuration Items */
#define CSID_C_IBURST_CNT_TYPE_UI 			_CFG_IBURST(0x0001)	//T=u8, connect type, Dial-on-Demand, autoconnect...
#define CSID_C_IBURST_NAT_DISABLE        	_CFG_IBURST(0x000f)	//T=u32, iBurst wantype NAT disable
#define CSID_C_IBURST_MULTIWAN_NAT_DISABLE	_CFG_IBURST(0x00f0)	//T=u32, iBurst wantype NAT disable in Multiwan
#define CSID_C_IBURST_USER           	  	_CFG_IBURST(0x0100)	//T=str, user name
#define CSID_C_IBURST_MULTIWAN_USER         _CFG_IBURST(0x0101)	//T=str, user name
#define CSID_C_IBURST_PASSWORD       		_CFG_IBURST(0x0200)	//T=str, user password
#define CSID_C_IBURST_MULTIWAN_PASSWORD     _CFG_IBURST(0x0201)	//T=str, user password
#define CSID_C_IBURST_IF_IP          		_CFG_IBURST(0x0300)	//T=ipv4, static interface ip
#define CSID_C_IBURST_MULTIWAN_IF_IP        _CFG_IBURST(0x0301) //T=ipv4, static interface ip
#define CSID_C_IBURST_SERVICENAME    		_CFG_IBURST(0x0600)	//T=str, service name
#define CSID_C_IBURST_MULTIWAN_SERVICENAME  _CFG_IBURST(0x0601)	//T=str, service name
#define CSID_C_IBURST_IF_PRIDNS      		_CFG_IBURST(0x0700)	//T=ipv4, static interface primary DNS
#define CSID_C_IBURST_MULTIWAN_IF_PRIDNS    _CFG_IBURST(0x0701)	//T=ipv4, static interface primary DNS
#define CSID_C_IBURST_IF_SECDNS       		_CFG_IBURST(0x0800)	//T=ipv4, static interface secondary DNS
#define CSID_C_IBURST_MULTIWAN_IF_SECDNS    _CFG_IBURST(0x0801)	//T=ipv4, static interface secondary DNS
#define CSID_C_IBURST_CNT_TYPE        		_CFG_IBURST(0x0900)	//T=u32, connect type, Dial-on-Demand, autoconnect...
#define CSID_C_IBURST_MULTIWAN_CNT_TYPE		_CFG_IBURST(0x0901)	//T=u32, connect type, Dial-on-Demand, autoconnect...
#define CSID_C_IBURST_MULTIWAN_CNT_TYPE_UI  _CFG_IBURST(0x0920) //T=u32, connect type, Dial-on-Demand, autoconnect...



#define CSID_C_IBURST_MTU             		_CFG_IBURST(0x0a00)	//T=u32, D=0, max transmission unit
#define CSID_C_IBURST_MULTIWAN_MTU        	_CFG_IBURST(0x0a01)	//T=u32, D=0, max transmission unit
#define CSID_C_IBURST_IDLETIME        		_CFG_IBURST(0x0b00)	//T=u32, D=600, max idle time
#define CSID_C_IBURST_MULTIWAN_IDLETIME     _CFG_IBURST(0x0b01)	//T=u32, D=600, max idle time
#define CSID_C_IBURST_HOLDOFFTIME     		_CFG_IBURST(0x0c00)	//T=u32, D=300, wait time before rebuild
#define CSID_C_IBURST_MULTIWAN_HOLDOFFTIME  _CFG_IBURST(0x0c01)	//T=u32, D=300, wait time before rebuild
#define CSID_C_IBURST_AUTHTYPE        		_CFG_IBURST(0x0d00)	//T=u32, auth type
#define CSID_C_IBURST_MULTIWAN_AUTHTYPE 	_CFG_IBURST(0x0d01)	//T=u32, auth type

/* Status Items */
#define CSID_S_IBURST_CNT_TIME        			_STA_IBURST(0x0100)	//T=u32, connect time
#define CSID_S_IBURST_MULTIWAN_CNT_TIME        	_STA_IBURST(0x0101)	//T=u32, connect time
#define CSID_S_IBURST_IF_IP           			_STA_IBURST(0x0200)	//T=ipv4, dynamic interface ip
#define CSID_S_IBURST_MULTIWAN_IF_IP           	_STA_IBURST(0x0201)	//T=ipv4, dynamic interface ip
#define CSID_S_IBURST_IF_NM           			_STA_IBURST(0x0300)	//T=ipv4, dynamic interface netmask
#define CSID_S_IBURST_MULTIWAN_IF_NM           	_STA_IBURST(0x0301)	//T=ipv4, dynamic interface netmask

#define CSID_S_IBURST_IF_GW           			_STA_IBURST(0x0400)	//T=ipv4, dynamic interface gateway
#define CSID_S_IBURST_MULTIWAN_IF_GW           	_STA_IBURST(0x0401)	//T=ipv4, dynamic interface gateway

#define CSID_S_IBURST_IF_PRIDNS       			_STA_IBURST(0x0500)	//T=ipv4, dynamic interface primary DNS
#define CSID_S_IBURST_MULTIWAN_IF_PRIDNS       	_STA_IBURST(0x0501)	//T=ipv4, dynamic interface primary DNS
#define CSID_S_IBURST_IF_SECDNS       			_STA_IBURST(0x0600)	//T=ipv4, dynamic interface secondary DNS
#define CSID_S_IBURST_MULTIWAN_IF_SECDNS       	_STA_IBURST(0x0601)	//T=ipv4, dynamic interface secondary DNS
#define CSID_S_IBURST_CNT_STATUS      			_STA_IBURST(0x0700)	//T=u32, connect status
#define CSID_S_IBURST_MULTIWAN_CNT_STATUS      	_STA_IBURST(0x0701)	//T=u32, connect status
#define CSID_S_IBURST_PKTIN_NUM       			_STA_IBURST(0x0800)	//T=u32, number of packet input
#define CSID_S_IBURST_MULTIWAN_PKTIN_NUM       	_STA_IBURST(0x0801)	//T=u32, number of packet input
#define CSID_S_IBURST_PKTOUT_NUM      			_STA_IBURST(0x0900)	//T=u32, number of packet output
#define CSID_S_IBURST_MULTIWAN_PKTOUT_NUM      	_STA_IBURST(0x0901)	//T=u32, number of packet output

#define CSID_S_IBURST_ALTERED         			_STA_IBURST(0x0A00)	//T=u32, ui altered
#define CSID_S_IBURST_MULTIWAN_ALTERED         	_STA_IBURST(0x0A01)	//T=u32, ui altered

#define CSID_S_IBURST_INTERFACE    				_STA_IBURST(0x0B00)	//T=str, service name
#define CSID_S_IBURST_MULTIWAN_INTERFACE		_STA_IBURST(0x0B01)	//T=str, service name

#endif //ifndef _CSID_IBURST_H
