/*
 * @file Member ID Definition of Group PPTP Server.
 *
 * Moderator: CliffWang
 * Group ID: 0x001a0000/0x801a0000
 */

#ifndef _CSID_PPTPSERV_H
#define _CSID_PPTPSERV_H

#include "csid_gid.h"

#define _CFG_PPTPSERV(x)		(CSID_GIDC_PPTPSERV|(x))
#define _STA_PPTPSERV(x)		(CSID_GIDS_PPTPSERV|(x))

/* Extra Definitions */
#define PPTPSERV_AUTHTYPE_AUTO             0x00
#define PPTPSERV_AUTHTYPE_PAP              0x01
#define PPTPSERV_AUTHTYPE_CHAP             0x02
#define PPTPSERV_AUTHTYPE_MSCHAP           0x04
#define PPTPSERV_AUTHTYPE_MSCHAPV2         0x08
#define PPTPSERV_AUTHTYPE_EAP              0x10

/* Configuration Items */

#define CSID_C_PPTPSERV_SUPPORT                  _CFG_PPTPSERV(0x1000)   //T=u8, D=1, Enable/Disable
#define CSID_C_PPTPSERV_ENABLE                   _CFG_PPTPSERV(0x0001)   //T=u8, D=0, Enable/Disable
#define CSID_C_PPTPSERV_ENABLE_AUTHTYPE_AUTO     _CFG_PPTPSERV(0x0002)   //T=u8, D=0, Enable/Disable
#define CSID_C_PPTPSERV_ENABLE_AUTHTYPE_PAP      _CFG_PPTPSERV(0x0003)   //T=u8, D=0, Enable/Disable
#define CSID_C_PPTPSERV_ENABLE_AUTHTYPE_CHAP     _CFG_PPTPSERV(0x0004)   //T=u8, D=0, Enable/Disable
#define CSID_C_PPTPSERV_ENABLE_AUTHTYPE_MSCHAP   _CFG_PPTPSERV(0x0005)   //T=u8, D=0, Enable/Disable
#define CSID_C_PPTPSERV_ENABLE_AUTHTYPE_MSCHAPV2 _CFG_PPTPSERV(0x0006)   //T=u8, D=0, Enable/Disable
#define CSID_C_PPTPSERV_ENABLE_AUTHTYPE_EAP      _CFG_PPTPSERV(0x0007)   //T=u8, D=0, Enable/Disable
#define CSID_C_PPTPSERV_LOCAL_IP             _CFG_PPTPSERV(0x0008)   	 //T=ipv4, D={192.168.0.1}, local lan ip
#define CSID_C_PPTPSERV_REMOTE_IPPOOL_START  _CFG_PPTPSERV(0x0009)   	 //T=ipv4, D={192.168.0.10}, remote start lan ip
#define CSID_C_PPTPSERV_REMOTE_IPPOOL_END    _CFG_PPTPSERV(0x000a)   	 //T=ipv4, D={192.168.0.100}, remote end lan ip
#define CSID_C_PPTPSERV_ENABLE_NAT			 _CFG_PPTPSERV(0x000b)		 //T=u8, D=0, Enable/Disable
#define CSID_C_PPTPSERV_ENABLE_MPPE				 _CFG_PPTPSERV(0x0010)   //T=u8, D=0, Enable/Disable
#define CSID_C_PPTPSERV_ENABLE_MPPE_40			 _CFG_PPTPSERV(0x0011)   //T=u8, D=0, Enable/Disable
#define CSID_C_PPTPSERV_ENABLE_MPPE_56			 _CFG_PPTPSERV(0x0012)   //T=u8, D=0, Enable/Disable
#define CSID_C_PPTPSERV_ENABLE_MPPE_128			 _CFG_PPTPSERV(0x0013)   //T=u8, D=0, Enable/Disable

/* RADIUS settings */
#define CSID_C_PPTPSERV_ENABLE_RADIUSCLIENT			 _CFG_PPTPSERV(0x0014)   //T=u8, D=0, Enable/Disable
#define CSID_C_PPTPSERV_RADIUSCLIENT_SERV_IP			 _CFG_PPTPSERV(0x0015)   //T=ipv4, radiusclient server ip
#define CSID_C_PPTPSERV_RADIUSCLIENT_SERV_PORT			 _CFG_PPTPSERV(0x0016)   //T=u16, D=1812, radiusclient server port
#define CSID_C_PPTPSERV_RADIUSCLIENT_SECRET			 _CFG_PPTPSERV(0x0017)   //T=str, radiusclient sharing secret
/* PPTP Tunnel name */
#define CSID_C_PPTPSERV_NAME                   			_CFG_PPTPSERV(0x0018)   //T=str, VPN-PPTP Tunnel Name
/* PPTP Username/Passowrd Account Active Main Switch */
#define CSID_C_PPTPSERV_ENABLE_ACCOUNT_ACTIVE       _CFG_PPTPSERV(0x0019)   //T=u8, Enable account active function

/* Account settings */
#define CSID_C_PPTPSERV_ACCOUNT_USER         _CFG_PPTPSERV(0x0100)	 //T=str, user name
#define CSID_C_PPTPSERV_ACCOUNT_PASSWORD     _CFG_PPTPSERV(0x0200)	 //T=str, user password
#define CSID_C_PPTPSERV_ACCOUNT_ACTIVE         _CFG_PPTPSERV(0x0300)     //T=u8, D=0, account active
#define CSID_C_PPTPSERV_ACCOUNT_DATABASE         _CFG_PPTPSERV(0x0400)  //T=u8, D=0, account datebase,0: Group1,1:Group2,2:Group3,3:Group4.
#define CSID_C_PPTPSERV_ACCOUNT_GROUPNAME         _CFG_PPTPSERV(0x0500)  //T=str, account group name


/* Status Items */
#define CSID_S_PPTPSERV_ALTERED		     _STA_PPTPSERV(0x0001)	 //T=u8, ui altered
#define CSID_S_PPTPSERV_ACTIVE_TUNNELS 	 _STA_PPTPSERV(0x0002)	 //T=u8, D=0, tunnel numbers
#define CSID_S_PPTPSERV_ACT_PEER_IDX	 _STA_PPTPSERV(0x0003)   //T=u8, D=0, peer index
#define CSID_S_PPTPSERV_ACT_BUTTON		 _STA_PPTPSERV(0x0004)   //T=u8, D=0, peer action button
/* Account status */
#define CSID_S_PPTPSERV_PEER_USER     	_STA_PPTPSERV(0x0100)	 //T=str, username
#define CSID_S_PPTPSERV_PEER_REAL_IP	_STA_PPTPSERV(0x0200)	 //T=ipv4, peer real ip
#define CSID_S_PPTPSERV_PEER_IP			_STA_PPTPSERV(0x0300)	 //T=ipv4, peer virtual ip
#define CSID_S_PPTPSERV_PEER_CALL_ID	_STA_PPTPSERV(0x0400)	 //T=s32, D=-1, peer call id
#define CSID_S_PPTPSERV_PEER_IFNAME 	_STA_PPTPSERV(0x0500)	 //T=str, peer interface name

#endif //ifndef _CSID_PPTPSERV_H

