/*
 * @file Member ID Definition of Group MRTG.
 *
 * Moderator: JackChiang
 * Group ID: 0x00600000/0x80600000
 */

#ifndef _CSID_MRTG_H
#define _CSID_MRTG_H

#include "csid_gid.h"

#define _CFG_MRTG(x)       (CSID_GIDC_MRTG|(x))
#define _STA_MRTG(x)       (CSID_GIDS_MRTG|(x))

/* Configuration Items */

#define CSID_C_MRTG_ENABLE				_CFG_MRTG(0x0001) //T=u32, MRTG enable, 0=disable, 1=enable
#define CSID_C_MRTG_RANGE_SEL			_CFG_MRTG(0x0002) //T=u32, range select, 0= real-time, 1= two hours, 2= daily, 3=weekly, 4=monthly

/* Status Items */

#define CSID_S_MRTG_ALTERED				_STA_MRTG(0x0001) //T=u32, ui change altered
#define CSID_S_MRTG_THR_ALTERED			_STA_MRTG(0x0002) //T=u32, ui throughput altered
#define CSID_S_MRTG_CONT_ALTERED		_STA_MRTG(0x0003) //T=u32, ui connections altered
#define CSID_S_MRTG_UPLOAD_WAN0			_STA_MRTG(0x0200) //T=u32, MRTG upload throughput for wan0
#define CSID_S_MRTG_DOWNLOAD_WAN0		_STA_MRTG(0x0400) //T=u32, MRTG download throughput for wan0
#define CSID_S_MRTG_UPLOAD_BR0			_STA_MRTG(0x0600) //T=u32, MRTG upload throughput for br0
#define CSID_S_MRTG_DOWNLOAD_BR0		_STA_MRTG(0x0800) //T=u32, MRTG download throughput for br0
#define CSID_S_MRTG_TCP_CONT			_STA_MRTG(0x0a00) //T=u32, MRTG TCP connections
#define CSID_S_MRTG_UDP_CONT			_STA_MRTG(0x0c00) //T=u32, MRTG UDP connections
#define CSID_S_MRTG_ICMP_CONT			_STA_MRTG(0x0f00) //T=u32, MRTG ICMP connections

#endif //#ifndef _CSID_MRTG_H
