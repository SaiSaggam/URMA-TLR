/*
 * @file Member ID Definition of Group Virtual Computers.
 *
 * Moderator: JasonJiang
 * Group ID: 0x00140000/0x80140000
 */

#ifndef _CSID_VCOMPUTER_H
#define _CSID_VCOMPUTER_H

#include "csid_gid.h"

#define _CFG_VCOMPUTER(x)         (CSID_GIDC_VCOMPUTER|(x))
#define _STA_VCOMPUTER(x)         (CSID_GIDS_VCOMPUTER|(x))

/* Configuration Items */
#define CSID_C_VCOMPUTER_GLOBAL_IP_SUBNET	_CFG_VCOMPUTER(0x0001)		//T=ipv4, global ip subnet
#define CSID_C_VCOMPUTER_FUNCTION_DISABLE       _CFG_VCOMPUTER(0x0002)          //T=u8, virtual computer function disable, 0: Enable, 1: Disable 

#define CSID_C_VCOMPUTER_RULE_ENABLE		_CFG_VCOMPUTER(0x1000)		//T=u8, rule enable, 1:yes 0:no
#define CSID_C_VCOMPUTER_GLOBAL_IP			_CFG_VCOMPUTER(0x2000)		//T=ipv4, global ip
#define CSID_C_VCOMPUTER_LOCAL_IP			_CFG_VCOMPUTER(0x3000)		//T=ipv4, local ip

/* Status Items */
#define CSID_S_VCOMPUTER_RULE_NUM			_STA_VCOMPUTER(0x0001)		//T=u16, number of virtual computers rules
#define CSID_S_VCOMPUTER_ALTERED			_STA_VCOMPUTER(0x0002)		//T=u8, ui altered

#endif //ifndef _CSID_VCOMPUTER_H
