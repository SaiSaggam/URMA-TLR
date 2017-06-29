/*
 * @file Member ID Definition of Group SIP NAT Traversal
 *
 * Moderator: Terence Yang
 * Group ID: 0x00240000/0x80240000
 */

#ifndef _CSID_SIPNAT_H
#define _CSID_SIPNAT_H

#include "csid_gid.h"

#define _CFG_SIPNAT(x)		(CSID_GIDC_SIPNAT|(x))
#define _STA_SIPNAT(x)		(CSID_GIDS_SIPNAT|(x))

/* Extra Definitions */

/* Configuration Items */

// NAT Traversal 
#define CSID_C_SIPNAT_STUN            _CFG_SIPNAT(0x0001)	//T=u8, D=0, STUN Enable, 0:Disable 1:Enable
#define CSID_C_SIPNAT_STUN_ADDR       _CFG_SIPNAT(0x0002)	//T=str, D="", STUN Server Address  
#define CSID_C_SIPNAT_STUN_PORT       _CFG_SIPNAT(0x0003)	//T=u16, STUN Server port 
#define CSID_C_SIPNAT_UPNP            _CFG_SIPNAT(0x0004)	//T=u8, D=0, UPNP Enable, 0:Disable 1:Enable
#define CSID_C_SIPNAT_SIP_ALG         _CFG_SIPNAT(0x0005)	//T=u8, D=0, SIP ALG Enable, 0:Disable 1:Enable

// Lines 
// Line 0 (FXS 0) 
#define CSID_C_FXS_SIPNAT_ALIVE       _CFG_SIPNAT(0x0010)	//T=u8, D=0, SIP NAT Keep Alive, 0:Disble 1:Enable  
#define CSID_C_FXS_SIPNAT_ALIVE_INT   _CFG_SIPNAT(0x0020)	//T=u16, D=15, SIP NAT Keep Alive Interval (1..3600) 
	 
// Line 1 (FXO 0) 
#define CSID_C_FXO_SIPNAT_ALIVE       _CFG_SIPNAT(0x0110)	//T=u8, D=0, SIP NAT Keep Alive, 0:Disble 1:Enable  
#define CSID_C_FXO_SIPNAT_ALIVE_INT   _CFG_SIPNAT(0x0120)	//T=u16, D=15, SIP NAT Keep Alive Interval (1..3600)
   	
/* Status Items */
#define CSID_S_SIP_ALG_ALTERED                         _STA_SIPNAT(0x0001)	//T=u8, SIP alg altered
#define CSID_S_SIPNAT_UDP_CACHE_READY_SIP_ALG_START    _STA_SIPNAT(0x0002)	//T=u8, SIP ALG Start after UDP cache ready
/* Multisession Items (start fromn 0x2000) */

#endif //ifndef _CSID_SIPNAT_H
