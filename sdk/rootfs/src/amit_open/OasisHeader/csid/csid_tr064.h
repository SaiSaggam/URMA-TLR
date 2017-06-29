/*
 * @file Member ID Definition of TR064.
 *
 * Moderator: 
 * Group ID: 0x00940000/0x80940000
 */
 
#ifndef _CSID_TR064_H
#define _CSID_TR064_H

#include "csid_gid.h"

#define _CFG_TR064(x)        (CSID_GIDC_TR064|(x))
#define _STA_TR064(x)        (CSID_GIDS_TR064|(x))



/* Configuration Items */

#define CSID_C_TR064_ENABLE          		_CFG_TR064(0x0001) //T=u8, wheather TR064 is enabled or not
/*connection Service*/
#define CSID_C_TR064_WANSNUM		_CFG_TR064(0x0002) //T=u16, the number of WANConnection services
#define CSID_C_TR064_DESTADDR		_CFG_TR064(0x0100) //T=str, Destination address of WANConnectionDevice
#define CSID_C_TR064_LINKTYPE		_CFG_TR064(0x0200) //T=str, Link Type
#define CSID_C_TR064_CONTYPE		_CFG_TR064(0x0300) //T=str, Conneciton Type
#define CSID_C_TR064_SNAME		_CFG_TR064(0x0400) //T=str, Friendly name of WAN**connection Service

#define CSID_C_TR064_DEFAULT_CONNECTION _CFG_TR064(0x0003) //T=u8, Which Connection should be used.



/*state */

#define CSID_S_TR064_CON_ALTERED      _STA_TR064(0x0001) //T=u16, connection altered.

#endif
