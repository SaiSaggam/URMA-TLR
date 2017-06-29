/*
 * @file Member ID Definition of Group DNSPROXY.
 *
 * Moderator: GeorgeWang
 * Group ID: 0x000A0000/0x800A0000
 */  

#ifndef _CSID_DNSPROXY_H
#define _CSID_DNSPROXY_H

#include "csid_gid.h"

#define _CFG_DNSPROXY(x)		(CSID_GIDC_DNSPROXY|(x))
#define _STA_DNSPROXY(x)		(CSID_GIDS_DNSPROXY|(x))

/* Configuration Items */ 

#define CSID_C_DNSPROXY_ENABLE	           _CFG_DNSPROXY(0x0001)	//T=u8, enble/disable
#define CSID_C_DNSPROXY_SERVER	           _CFG_DNSPROXY(0x0002)	//T=ipv4, Authoritative server
#define CSID_C_DNSPROXY_SERVER_PORT        _CFG_DNSPROXY(0x0003)	//T=u16, D=53, Authoritative server port
#define CSID_C_DNSPROXY_SERVER_TIMEOUT     _CFG_DNSPROXY(0x0004)	//T=u32, D=10,Seconds to wait for answers
#define CSID_C_DNSPROXY_RESOLVER           _CFG_DNSPROXY(0x0005)	//T=ipv4, Recursive resolver
#define CSID_C_DNSPROXY_RESOLVER_PORT      _CFG_DNSPROXY(0x0006)	//T=u16, D=53, Recursive resolver port
#define CSID_C_DNSPROXY_RESOLVER_TIMEOUT   _CFG_DNSPROXY(0x0007)	//T=u32, D=90, Seconds to wait for answers
#define CSID_C_DNSPROXY_LISTENPORT         _CFG_DNSPROXY(0x0008)	//T=u16, D=53, Local port


#define CSID_C_DNSPROXY_LOCAL_PROFILE_NAME _CFG_DNSPROXY(0x0100)	//T=str, rule name
#define CSID_C_DNSPROXY_LOCAL_DOMAIN_NAME  _CFG_DNSPROXY(0x0200)	//T=str, domain name
#define CSID_C_DNSPROXY_LOCAL_IP           _CFG_DNSPROXY(0x0300)	//T=ipv4, domain name 's mapping IP 
#define CSID_C_DNSPROXY_LOCAL_DESCRIPTION  _CFG_DNSPROXY(0x0400)	//T=str, profile 's description
#define CSID_C_DNSPROXY_LOCAL_RULE_ENABLE  _CFG_DNSPROXY(0x0500)	//T=u8, rule enable/disable



/* Status Items */ 
#define CSID_S_DNSPROXY_LOCAL_ALTERED      _STA_DNSPROXY(0x0001)	//T=u8, altered

#endif //ifndef _CSID_DNSPROXY_H
