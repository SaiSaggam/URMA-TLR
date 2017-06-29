/*
 * @file Member ID Definition of Group L2TP Server.
 *
 * Moderator: CliffWang
 * Group ID: 0x001b0000/0x801b0000
 */

#ifndef _CSID_L2TPSERV_H
#define _CSID_L2TPSERV_H

#include "csid_gid.h"

#define _CFG_L2TPSERV(x)		(CSID_GIDC_L2TPSERV|(x))
#define _STA_L2TPSERV(x)		(CSID_GIDS_L2TPSERV|(x))

/* Extra Definitions */
#define L2TPSERV_AUTHTYPE_AUTO             0x00
#define L2TPSERV_AUTHTYPE_PAP              0x01
#define L2TPSERV_AUTHTYPE_CHAP             0x02
#define L2TPSERV_AUTHTYPE_MSCHAP           0x04
#define L2TPSERV_AUTHTYPE_MSCHAPV2         0x08
#define L2TPSERV_AUTHTYPE_EAP              0x10

/* Configuration Items */
#define CSID_C_L2TPSERV_SUPPORT                  _CFG_L2TPSERV(0x1000)   //T=u8, D=1, Enable/Disable
#define CSID_C_L2TPSERV_ENABLE                   _CFG_L2TPSERV(0x0001)   //T=u8, D=0, Enable/Disable
#define CSID_C_L2TPSERV_ENABLE_AUTHTYPE_AUTO     _CFG_L2TPSERV(0x0002)   //T=u8, D=0, Enable/Disable
#define CSID_C_L2TPSERV_ENABLE_AUTHTYPE_PAP      _CFG_L2TPSERV(0x0003)   //T=u8, D=0, Enable/Disable
#define CSID_C_L2TPSERV_ENABLE_AUTHTYPE_CHAP     _CFG_L2TPSERV(0x0004)   //T=u8, D=0, Enable/Disable
#define CSID_C_L2TPSERV_ENABLE_AUTHTYPE_MSCHAP   _CFG_L2TPSERV(0x0005)   //T=u8, D=0, Enable/Disable
#define CSID_C_L2TPSERV_ENABLE_AUTHTYPE_MSCHAPV2 _CFG_L2TPSERV(0x0006)   //T=u8, D=0, Enable/Disable
#define CSID_C_L2TPSERV_ENABLE_AUTHTYPE_EAP      _CFG_L2TPSERV(0x0007)   //T=u8, D=0, Enable/Disable
#define CSID_C_L2TPSERV_LOCAL_IP             _CFG_L2TPSERV(0x0008)   	 //T=ipv4, D={192.168.10.1}, local lan ip
#define CSID_C_L2TPSERV_REMOTE_IPPOOL_START  _CFG_L2TPSERV(0x0009)   	 //T=ipv4, D={192.168.10.10}, remote start lan ip
#define CSID_C_L2TPSERV_REMOTE_IPPOOL_END    _CFG_L2TPSERV(0x000a)   	 //T=ipv4, D={192.168.10.100}, remote end lan ip
#define CSID_C_L2TPSERV_ENABLE_NAT           _CFG_L2TPSERV(0x000b)       //T=u8, D=0, Enable/Disable

#define CSID_C_L2TPSERV_ENABLE_MPPE              _CFG_L2TPSERV(0x0010)   //T=u8, D=0, Enable/Disable
#define CSID_C_L2TPSERV_ENABLE_MPPE_40           _CFG_L2TPSERV(0x0011)   //T=u8, D=0, Enable/Disable
#define CSID_C_L2TPSERV_ENABLE_MPPE_56           _CFG_L2TPSERV(0x0012)   //T=u8, D=0, Enable/Disable
#define CSID_C_L2TPSERV_ENABLE_MPPE_128          _CFG_L2TPSERV(0x0013)   //T=u8, D=0, Enable/Disable

/* RADIUS settings */
#define CSID_C_L2TPSERV_ENABLE_RADIUSCLIENT			 _CFG_L2TPSERV(0x0014)   //T=u8, D=0, Enable/Disable
#define CSID_C_L2TPSERV_RADIUSCLIENT_SERV_IP			 _CFG_L2TPSERV(0x0015)   //T=ipv4, radiusclient server ip
#define CSID_C_L2TPSERV_RADIUSCLIENT_SERV_PORT			 _CFG_L2TPSERV(0x0016)   //T=u16, D=1812, radiusclient server port
#define CSID_C_L2TPSERV_RADIUSCLIENT_SECRET			 _CFG_L2TPSERV(0x0017)   //T=str, radiusclient sharing secret
/* tunnel name */
#define CSID_C_L2TPSERV_NAME                   			 _CFG_L2TPSERV(0x0018)   //T=str, VPN-L2TP Tunnel Name
/* L2TP Username/Passowrd Account Active Main Switch */
#define CSID_C_L2TPSERV_ENABLE_ACCOUNT_ACTIVE        _CFG_L2TPSERV(0x0019)   //T=u8, Enable account active function
#define CSID_C_L2TPSERV_PORT                                     _CFG_L2TPSERV(0x001a)   //T=u32, D=0, 0:user default value 1701

/* Account settings */
#define CSID_C_L2TPSERV_ACCOUNT_USER         _CFG_L2TPSERV(0x0100)	 //T=str, user name
#define CSID_C_L2TPSERV_ACCOUNT_PASSWORD     _CFG_L2TPSERV(0x0200)	 //T=str, user password
#define CSID_C_L2TPSERV_ACCOUNT_ACTIVE       _CFG_L2TPSERV(0x0300)   //T=u8, D=0, account active
#define CSID_C_L2TPSERV_ACCOUNT_DATABASE       _CFG_L2TPSERV(0x0400)   //T=u8, D=0, account database,0:Group1,1:Group2,2:Group3,3:Group4
#define CSID_C_L2TPSERV_ACCOUNT_GROUPNAME       _CFG_L2TPSERV(0x0500)   //T=str, account group name

/*IO-DATA L2TP server client mode for switch */
/* -------start------ */
#define CSID_C_L2TP_SERV_CLINET_MODE    _CFG_L2TPSERV(0x0600)    //T=u8, D=0,VPN server 0,VPN client 1
#define CSID_C_L2TP_SERV_CUSTOMIZE_FLAG		_CFG_L2TPSERV(0x0601) //T=u32, D=0, L2TPserver default setting 1:IODATA(BX-VP1)
#define CSID_C_L2TP_SERV_PAIRING_USER	    _CFG_L2TPSERV(0x0602) //T=str, L2TP client account 
#define CSID_C_L2TP_SERV_PAIRING_USER_RESV	_CFG_L2TPSERV(0x062F) //T=str, L2TP client account
#define CSID_C_L2TP_SERV_SHARE_KEY_TYPE		_CFG_L2TPSERV(0x0630) //T=u32, D=0, pre share key waring time type, pre share key
#define CSID_C_L2TP_SERV_SHARE_KEY_TIME		_CFG_L2TPSERV(0x0631) //T=str, D="20150101000000", L2TP start time,pre share key
#define CSID_C_L2TP_SERV_SHARE_KEY_TM_ISDST		_CFG_L2TPSERV(0x0632) //T=u32, D=0, pre share key  waring time, day in the year
#define CSID_C_L2TP_SERV_SHARE_KEY_TM_YDAY		_CFG_L2TPSERV(0x0633) //T=u32, D=0, pre share key  waring time, day in the year
#define CSID_C_L2TP_SERV_SHARE_KEY_TM_WDAY		_CFG_L2TPSERV(0x0634) //T=u32, D=0, pre share key  waring time, day in the year
#define CSID_C_L2TP_SERV_SHARE_KEY_TM_YEAR		_CFG_L2TPSERV(0x0635) //T=u32, D=115, pre share key  waring time, year
#define CSID_C_L2TP_SERV_SHARE_KEY_TM_MON		_CFG_L2TPSERV(0x0636) //T=u32, D=1, pre share key  waring time ,  month 
#define CSID_C_L2TP_SERV_SHARE_KEY_TM_MDAY		_CFG_L2TPSERV(0x0637) //T=u32, D=0, pre share key  waring time , day of the month
#define CSID_C_L2TP_SERV_SHARE_KEY_TM_HOUR		_CFG_L2TPSERV(0x0638) //T=u32, D=0, pre share key  waring time , hours
#define CSID_C_L2TP_SERV_SHARE_KEY_TM_MIN		_CFG_L2TPSERV(0x0639) //T=u32, D=0, pre share key  waring time , minutes
#define CSID_C_L2TP_SERV_SHARE_KEY_TM_SEC		_CFG_L2TPSERV(0x063A) //T=u32, D=0, pre share key  waring time , seconds


#define CSID_C_L2TP_SERV_PASSCODE		    _CFG_L2TPSERV(0x0640) //T=u32, CGI PASS CODE
#define CSID_C_L2TP_SERV_PASSCODE_RESV		_CFG_L2TPSERV(0x065F) //T=u32, CGI PASS CODE

/*--------end------------*/
/* Status Items */
#define CSID_S_L2TPSERV_ALTERED		     _STA_L2TPSERV(0x0001)	 //T=u8, ui altered
#define CSID_S_L2TPSERV_ACTIVE_TUNNELS   _STA_L2TPSERV(0x0002)   //T=u8, D=0, tunnel numbers
#define CSID_S_L2TPSERV_ACT_PEER_IDX     _STA_L2TPSERV(0x0003)   //T=u8, D=0, peer index
#define CSID_S_L2TPSERV_ACT_BUTTON       _STA_L2TPSERV(0x0004)   //T=u8, D=0, peer action button

/* Account status */
#define CSID_S_L2TPSERV_PEER_USER       _STA_L2TPSERV(0x0100)    //T=str, username
#define CSID_S_L2TPSERV_PEER_REAL_IP    _STA_L2TPSERV(0x0200)    //T=ipv4, peer real ip
#define CSID_S_L2TPSERV_PEER_IP         _STA_L2TPSERV(0x0300)    //T=ipv4, peer virtual ip
#define CSID_S_L2TPSERV_PEER_CALL_ID    _STA_L2TPSERV(0x0400)    //T=s32, D=-1, peer call id
#define CSID_S_L2TPSERV_PEER_IFNAME     _STA_L2TPSERV(0x0500)    //T=str, peer interface name

/*IO-DATA L2TP server client mode for switch*/
#define CSID_S_L2TP_SERV_CLINET_MODE          _STA_L2TPSERV(0x0600)    //T=u8, D=0,VPN server 0,VPN client 1
#define CSID_S_L2TP_SERV_QUICK_COPY_STATUS    _STA_L2TPSERV(0x0601)    //T=u32, D=0,VPN quick copy status
#define CSID_S_L2TP_SERV_PRE_SHARE_KEY_WARN   _STA_L2TPSERV(0x0602)    //T=u32, D=0,VPN pre share key status
#define CSID_S_L2TP_SERV_PRE_SHARE_KEY        _STA_L2TPSERV(0x0603)    //T=str, VPN pre share key
#define CSID_S_L2TP_SERV_AUTH_CGI_PORT        _STA_L2TPSERV(0x0604)    //T=s32, D=0,cgi port number

#endif //ifndef _CSID_L2TPSERV_H

