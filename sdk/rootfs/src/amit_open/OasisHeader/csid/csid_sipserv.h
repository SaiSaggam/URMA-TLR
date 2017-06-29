/*
 * @file Member ID Definition of Group SIP Server Setting
 *
 * Moderator: Terence Yang
 * Group ID: 0x00230000/0x80230000
 */

#ifndef _CSID_SIPSERV_H
#define _CSID_SIPSERV_H

#include "csid_gid.h"

#define _CFG_SIPSERV(x)		(CSID_GIDC_SIPSERV|(x))
#define _STA_SIPSERV(x)		(CSID_GIDS_SIPSERV|(x))

/* Extra Definitions */

/* Configuration Items */

// Server Setting 
#define CSID_C_SIPSERV_AUTH_EX         _CFG_SIPSERV(0x0001)	//T=u16, D=3600, Authentication Expired Time (60..65535) 
#define CSID_C_SIPSERV_AUTH_EXP        _CFG_SIPSERV(0x0002)	//T=u8, D=50, Authentication Expired Time Percentage (50% ~ 90%) 

// Lines 
// Line 0 (FXS)
#define CSID_C_SIPSERV_FXS_DOMAIN      _CFG_SIPSERV(0x0010)	//T=str, Domain Name  
#define CSID_C_SIPSERV_FXS_REG_ADDR    _CFG_SIPSERV(0x0020)	//T=str, Registrar Server Address  
#define CSID_C_SIPSERV_FXS_REG_PORT    _CFG_SIPSERV(0x0030)	//T=u16, D=5060, Registrar Server Port (1-65535) 
#define CSID_C_SIPSERV_FXS_PRO_ADDR    _CFG_SIPSERV(0x0040)	//T=str, Proxy Address  
#define CSID_C_SIPSERV_FXS_PRO_PORT    _CFG_SIPSERV(0x0050)	//T=u16, Proxy Port (1-65535) 
#define CSID_C_SIPSERV_FXS_OPR         _CFG_SIPSERV(0x0060)	//T=u8, D=0, Use Outbound Proxy, 0:dis 1:Enable  
#define CSID_C_SIPSERV_FXS_OPR_ADDR    _CFG_SIPSERV(0x0070)	//T=str, D="",Outbound Proxy Address  
#define CSID_C_SIPSERV_FXS_OPR_PORT    _CFG_SIPSERV(0x0080)	//T=u16, D=5060, Outbound Proxy Port (1-65535) 
#define CSID_C_SIPSERV_FXS_DNSSRV      _CFG_SIPSERV(0x0090)	//T=u8, D=0, DNS SRV support, 0:dis 1:Enable  
#define CSID_C_SIPSERV_FXS_STATUS      _CFG_SIPSERV(0x00a0)	//T=u8, registed status, 0:unrgisted 1:registed  
#define CSID_C_SIPSERV_FXS_REG_ADDR_IP _CFG_SIPSERV(0x00b0)	//T=str, RADDR->IP
#define CSID_C_SIPSERV_FXS_STATE       _CFG_SIPSERV(0x00c0)	//T=str, Australian state in which the router is located
#define CSID_C_SIPSERV_FXS2_DOMAIN     _CFG_SIPSERV(0x0110)	//T=str, Domain Name  
#define CSID_C_SIPSERV_FXS2_REG_ADDR   _CFG_SIPSERV(0x0111)	//T=str, Registrar Server Address  
#define CSID_C_SIPSERV_FXS2_PRO_ADDR   _CFG_SIPSERV(0x0120)	//T=str, Proxy Address  
#define CSID_C_SIPSERV_FXS2_OPR_ADDR   _CFG_SIPSERV(0x0130)	//T=str, Outbound Proxy Address  
#define CSID_C_SIPSERV_FXS2_OPR        _CFG_SIPSERV(0x0131)	//T=u8, D=0, Use Outbound Proxy, 0:dis 1:Enable  
#define CSID_C_SIPSERV_FXS2_STATUS     _CFG_SIPSERV(0x0140)	//T=u8, registed status, 0:unrgisted 1:registed  
#define CSID_C_SIPSERV_FXS2_REG_ADDR_IP _CFG_SIPSERV(0x0150)//T=str, RADDR->IP

#define CSID_C_SIPSERV_FXS3_DOMAIN     _CFG_SIPSERV(0x0310)	//T=str, Domain Name  
#define CSID_C_SIPSERV_FXS3_REG_ADDR   _CFG_SIPSERV(0x0311)	//T=str, Registrar Server Address  
#define CSID_C_SIPSERV_FXS3_PRO_ADDR   _CFG_SIPSERV(0x0320)	//T=str, Proxy Address  
#define CSID_C_SIPSERV_FXS3_OPR_ADDR   _CFG_SIPSERV(0x0330)	//T=str, Outbound Proxy Address  
#define CSID_C_SIPSERV_FXS3_OPR        _CFG_SIPSERV(0x0331)	//T=u8, D=0, Use Outbound Proxy, 0:dis 1:Enable  
#define CSID_C_SIPSERV_FXS3_STATUS     _CFG_SIPSERV(0x0340)	//T=u8, registed status, 0:unrgisted 1:registed 
#define CSID_C_SIPSERV_FXS3_REG_ADDR_IP _CFG_SIPSERV(0x0350)	//T=str, RADDR->IP

#define CSID_C_SIPSERV_FXS4_DOMAIN     _CFG_SIPSERV(0x0410)	//T=str, Domain Name  
#define CSID_C_SIPSERV_FXS4_REG_ADDR   _CFG_SIPSERV(0x0411)	//T=str, Registrar Server Address  
#define CSID_C_SIPSERV_FXS4_PRO_ADDR   _CFG_SIPSERV(0x0420)	//T=str, Proxy Address  
#define CSID_C_SIPSERV_FXS4_OPR_ADDR   _CFG_SIPSERV(0x0430)	//T=str, Outbound Proxy Address  
#define CSID_C_SIPSERV_FXS4_OPR        _CFG_SIPSERV(0x0431)	//T=u8, D=0, Use Outbound Proxy, 0:dis 1:Enable  
#define CSID_C_SIPSERV_FXS4_STATUS     _CFG_SIPSERV(0x0440)	//T=u8, registed status, 0:unrgisted 1:registed 
#define CSID_C_SIPSERV_FXS4_REG_ADDR_IP _CFG_SIPSERV(0x0450)	//T=str, RADDR->IP

#define CSID_C_SIPSERV_FXS5_DOMAIN     _CFG_SIPSERV(0x0510)	//T=str, Domain Name  
#define CSID_C_SIPSERV_FXS5_REG_ADDR   _CFG_SIPSERV(0x0511)	//T=str, Registrar Server Address  
#define CSID_C_SIPSERV_FXS5_PRO_ADDR   _CFG_SIPSERV(0x0520)	//T=str, Proxy Address  
#define CSID_C_SIPSERV_FXS5_OPR_ADDR   _CFG_SIPSERV(0x0530)	//T=str, Outbound Proxy Address  
#define CSID_C_SIPSERV_FXS5_OPR        _CFG_SIPSERV(0x0531)	//T=u8, D=0, Use Outbound Proxy, 0:dis 1:Enable  
#define CSID_C_SIPSERV_FXS5_STATUS     _CFG_SIPSERV(0x0540)	//T=u8, registed status, 0:unrgisted 1:registed 
#define CSID_C_SIPSERV_FXS5_REG_ADDR_IP _CFG_SIPSERV(0x0550)	//T=str, RADDR->IP
 
// Line 1 (FXO) 
#define CSID_C_SIPSERV_FXO_DOMAIN      _CFG_SIPSERV(0x0210)	//T=str, Domain Name  
#define CSID_C_SIPSERV_FXO_REG_ADDR    _CFG_SIPSERV(0x0220)	//T=str, Registrar Server Address  
#define CSID_C_SIPSERV_FXO_REG_PORT    _CFG_SIPSERV(0x0230)	//T=u16, D=5060, Registrar Server Port (1-65535) 
#define CSID_C_SIPSERV_FXO_PRO_ADDR    _CFG_SIPSERV(0x0240)	//T=str, Proxy Address  
#define CSID_C_SIPSERV_FXO_PRO_PORT    _CFG_SIPSERV(0x0250)	//T=u16, D=5060, Proxy Port (1-65535) 
#define CSID_C_SIPSERV_FXO_OPR         _CFG_SIPSERV(0x0260)	//T=u8, D=0, Use Outbound Proxy, 0:dis 1:Enable  
#define CSID_C_SIPSERV_FXO_OPR_ADDR    _CFG_SIPSERV(0x0270)	//T=str, D="", Outbound Proxy Address  
#define CSID_C_SIPSERV_FXO_OPR_PORT    _CFG_SIPSERV(0x0280)	//T=u16, D=5060, Outbound Proxy Port (1-65535)
#define CSID_C_SIPSERV_FXO_DNSSRV      _CFG_SIPSERV(0x0290)	//T=u8, D=0, DNS SRV support, 0:dis 1:Enable


//@D2_csid
#define CSID_C_SIPSERV_SRTP_EN         _CFG_SIPSERV(0x0500)	//T=u8, D=0, Secure RTP support, 0:dis 1:Enable
#define CSID_C_SIPSERV_RPORT_EN        _CFG_SIPSERV(0x0501)     //T=u8, D=1, Secure RPORT support, 0:dis 1:Enable

/* Status Items */

/* Multisession Items (start fromn 0x2000) */

#endif //ifndef _CSID_SIPSERV_H
