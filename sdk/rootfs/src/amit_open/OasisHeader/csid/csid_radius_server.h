#ifndef _CSID_RADIUS_SERVER_H_
#define _CSID_RADIUS_SERVER_H_

#include "csid_gid.h"

#define _CFG_RADIUS_SERVER(x)       (CSID_GIDC_RADIUS_SERVER|(x))
#define _STA_RADIUS_SERVER(x)       (CSID_GIDS_RADIUS_SERVER|(x))

#define CSID_S_RADIUS_SERVER_ALTERED _STA_RADIUS_SERVER(0x0100)  //T=u32, D=0
#define CSID_S_RADIUS_SERVER_ENABLE  _STA_RADIUS_SERVER(0x0101)  //T=u32, D=0

#define CSID_C_PRI_KEY_PWD           _CFG_RADIUS_SERVER(0x0000)  //T=str, D=""
#define CSID_C_PRI_KEY_FILE          _CFG_RADIUS_SERVER(0x0001)  //T=str, D=""
#define CSID_C_CERTIFICATION_FILE    _CFG_RADIUS_SERVER(0x0002)  //T=str, D=""
#define CSID_C_CA_FILE               _CFG_RADIUS_SERVER(0x0003)  //T=str, D=""
#define CSID_C_EAP_TYPE              _CFG_RADIUS_SERVER(0x0004)  //T=str, D=""
#define CSID_C_EAP_CHANNEL_TTLS      _CFG_RADIUS_SERVER(0x0005)  //T=str, D=""
#define CSID_C_EAP_CHANNEL_PEAP      _CFG_RADIUS_SERVER(0x0006)  //T=str, D=""


#endif
