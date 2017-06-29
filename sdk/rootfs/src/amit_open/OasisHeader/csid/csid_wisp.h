/*
 * @file Member ID Definition of Group WISP
 *
 * Moderator: StevenYen
 * Group ID: 0x00360000/0x80360000
 */

#ifndef _CSID_WISP_H
#define _CSID_WISP_H

#include "csid_gid.h"

#define _CFG_WISP(x)					(CSID_GIDC_WISP|(x))
#define _STA_WISP(x)					(CSID_GIDS_WISP|(x))

/* Extra Definitions */
#define WISP_CNT_TYPE_ONDEMAND			0x00
#define WISP_CNT_TYPE_AUTORECONNECT	0x01
#define WISP_CNT_TYPE_MANUAL			0x02

#define WISP_CNT_STATUS_DISCONNECT		0x00
#define WISP_CNT_STATUS_CONNECTING		0x01
#define WISP_CNT_STATUS_CONNECT		0x02
#define WISP_CNT_STATUS_DISCONNECTING	0x03
#define WISP_CNT_STATUS_LEASEFAIL      0x04
#define WISP_CNT_STATUS_WAIT_TRAFFIC	0x05

/* Configuration Items */
#define CSID_C_WISP_CNT_TYPE_UI        _CFG_WISP(0x5001)   //T=u8, Connect Type
#define CSID_C_WISP_HOSTNAME           _CFG_WISP(0x5002)   //T=str, Client's Host Name
#define CSID_C_WISP_IF_IP              _CFG_WISP(0x5004)   //T=ipv4, static interface ip
#define CSID_C_WISP_IF_NM              _CFG_WISP(0x5005)   //T=ipv4, static interface netmask
#define CSID_C_WISP_IF_GW              _CFG_WISP(0x5006)   //T=ipv4, static interface gateway
#define CSID_C_WISP_IF_PRIDNS          _CFG_WISP(0x5007)   //T=ipv4, static interface Primary DNS
#define CSID_C_WISP_IF_SECDNS          _CFG_WISP(0x5008)   //T=ipv4, static interface Second DNS 
#define CSID_C_WISP_MTU                _CFG_WISP(0x5009)   //T=u32, MTU
#define CSID_C_WISP_CNT_TYPE           _CFG_WISP(0x500a)   //T=u32, Connect Type
#define CSID_C_WISP_CHGTTL             _CFG_WISP(0x500b)   //T=u8, Enable/Disable , Change the TTL value
#define CSID_C_WISP_AUTOBKP            _CFG_WISP(0x500c)   //T=u8, Enable/Disable , Auto Backup
#define CSID_C_WISP_IDLETIME           _CFG_WISP(0x500d)   //T=u16, D=600, max idle time
#define CSID_C_WISP_NAT_DISABLE        _CFG_WISP(0x500f)   //T=u32, Dynamic IP wantype NAT disable

#define CSID_C_WISP_SCAN_BAND        _CFG_WISP(0x5010)   //T=u8, Scan Band

/* Status Items */
#define CSID_S_WISP_MULTIWAN_CMD_STATUS _STA_WISP(0x2000)   //T=u8, multiwan cmd status
#define CSID_S_WISP_PID                _STA_WISP(0x5001)   //T=u16, The PID of DHCP Client 
#define CSID_S_WISP_IF_IP              _STA_WISP(0x5002)   //T=ipv4, dynamic interface ip  
#define CSID_S_WISP_IF_NM              _STA_WISP(0x5003)   //T=ipv4, dynamic interface netmask
#define CSID_S_WISP_IF_GW              _STA_WISP(0x5004)   //T=ipv4, dynamic interface gateway
#define CSID_S_WISP_IF_PRIDNS          _STA_WISP(0x5005)   //T=ipv4, dynamic interface Primary DNS
#define CSID_S_WISP_IF_SECDNS          _STA_WISP(0x5006)   //T=ipv4, dynamic interface Second DNS 
#define CSID_S_WISP_CNT_STATUS         _STA_WISP(0x5007)   //T=u32, connect status
#define CSID_S_WISP_ALTERED            _STA_WISP(0x5008)   //T=u32, ui altered
#define CSID_S_WISP_LEASE_TIME         _STA_WISP(0x5009)   //T=u32, uptime + leasetime
#define CSID_S_WISP_CNT_TIME           _STA_WISP(0x500a)   //T=u32, connect time
#define CSID_S_WISP_PKTIN_NUM          _STA_WISP(0x500b)   //T=u32, number of packet input
#define CSID_S_WISP_PKTOUT_NUM         _STA_WISP(0x500c)   //T=u32, number of packet output

#endif //ifndef _CSID_WISP_H
