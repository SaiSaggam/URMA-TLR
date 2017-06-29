/*
 * @file Member ID Definition of Group MAC_LIST.
 *
 * Moderator: GeorgeWang/RupertLi
 * Group ID: 0x00500000/0x80500000
 */

#ifndef _CSID_MAC_LIST_H
#define _CSID_MAC_LIST_H

#include "csid_gid.h"

#define _CFG_MACLIST(x)       (CSID_GIDC_MACLIST|(x))
#define _STA_MACLIST(x)       (CSID_GIDS_MACLIST|(x))



/* Status Items */



#define CSID_S_LOCAL_PORT_SELECTION		_STA_MACLIST(0x0020) //T=u8, SVG Button: 0 1 2 3 
#define CSID_S_LOCAL_MAC_NUMBER 		_STA_MACLIST(0x0021) //T=u8, port mac number
#define CSID_S_LOCAL_PORT_MAC			_STA_MACLIST(0x1000) //T=mac, port mac list (0-255)

#endif //ifndef _CSID_MAC_LIST_H
