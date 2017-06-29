/*
 * @file Member ID Definition of Group MROUTING.
 *
 * Moderator: JackChiang
 * Group ID: 0x00180000/0x80180000
 */

#ifndef _CSID_MROUTING_H
#define _CSID_MROUTING_H

#include "csid_gid.h"

#define _CFG_MROUTING(x)		(CSID_GIDC_MROUTING|(x))
#define _STA_MROUTING(x)		(CSID_GIDS_MROUTING|(x))

/* Configuration Items */

/* Routing interface by ip */

#define CSID_C_MROUTING_BY_IP_IP		_CFG_MROUTING(0x0100) //T=ipv4, routing ip
#define CSID_C_MROUTING_BY_IP_NMASK		_CFG_MROUTING(0x0200) //T=ipv4, routing net mask
#define CSID_C_MROUTING_BY_IP_TYPE		_CFG_MROUTING(0x0300) //T=u8, the type of interface , 0:PPPoE 1:L2TP 2:PPTP 
#define CSID_C_MROUTING_BY_IP_INTERFACE		_CFG_MROUTING(0x0400) //T=u8, the interface of multisession ip
#define CSID_C_MROUTING_BY_IP_ENABLE		_CFG_MROUTING(0x0500) //T=u8, enable the rule of multisession ip, 0:no 1:yes 

/* Routing interface by domain */

#define CSID_C_MROUTING_BY_DM_DOMAIN		_CFG_MROUTING(0x0800) //T=str, routing domain
#define CSID_C_MROUTING_BY_DM_TYPE		_CFG_MROUTING(0x0900) //T=u8, the type of interface , 0:PPPoE 1:L2TP 2:PPTP
#define CSID_C_MROUTING_BY_DM_INTERFACE		_CFG_MROUTING(0x0a00) //T=u8, the interface of multisession domain
#define CSID_C_MROUTING_BY_DM_ENABLE		_CFG_MROUTING(0x0b00) //T=u8, enable the rule of multisession domain, 0:no 1:yes

/* Status Items */

/* Routing interface by ip */

#define CSID_S_MROUTING_BY_IP_ALTERED		_STA_MROUTING(0x0100) //T=u8, ui altered by ip

/* Routing interface by domain */

#define CSID_S_MROUTING_BY_DM_ALTERED		_STA_MROUTING(0x0800) //T=u8, ui altered by domain

#endif //ifndef _CSID_MROUTING_H
