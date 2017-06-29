#ifndef _CSID_SSL_VPN_USER_H_
#define _CSID_SSL_VPN_USER_H_

#include "csid_gid.h"

#define _CFG_SSL_VPN_USER(x)       (CSID_GIDC_SSL_VPN_USER|(x))
#define _STA_SSL_VPN_USER(x)       (CSID_GIDS_SSL_VPN_USER|(x))

#define CSID_S_SSL_VPN_USER_ALTERED  _STA_SSL_VPN_USER(0x0110) //T=u8, D=0, altered
#define CSID_S_SSL_VPN_USER_REFRESH  _STA_SSL_VPN_USER(0x0111) //T=u8, D=0, altered

#define BEGIN_USER                          0x1100
#define BEGIN_GROUP                         0x1120
#define BEGIN_TYPE                          0x1140
#define BEGIN_ENABLE                        0x1160
#define BEGIN_ORG_NAME                      0x1180
#define BEGIN_PWD                           0x11A0
//user list
#define CSID_C_SSL_VPN_ORG_USERNAME         _CFG_SSL_VPN_USER(0x1180) //T=str, D=""
#define CSID_C_SSL_VPN_USER_NAME            _CFG_SSL_VPN_USER(0x1100) //T=str, D=""
#define CSID_C_SSL_VPN_USER_GROUPTO         _CFG_SSL_VPN_USER(0x1120) //T=str, D=""
#define CSID_C_SSL_VPN_USER_TYPE            _CFG_SSL_VPN_USER(0x1140) //T=u8, D=0
#define CSID_C_SSL_VPN_USER_ENABLE          _CFG_SSL_VPN_USER(0x1160) //T=u8, D=1
#define CSID_C_SSL_VPN_USER_PASSWD          _CFG_SSL_VPN_USER(0x11A0) //T=str, D=""

#endif
