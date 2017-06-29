#ifndef _CSID_RADIUS_GROUP_H_
#define _CSID_RADIUS_GROUP_H_

#include "csid_gid.h"

#define _CFG_RADIUS_GROUP(x)       (CSID_GIDC_RADIUS_GROUP|(x))
#define _STA_RADIUS_GROUP(x)       (CSID_GIDS_RADIUS_GROUP|(x))

#define CSID_S_RADIUS_GROUP_ALTERED  _STA_RADIUS_GROUP(0x0100) //T=u32, D=0, altered
//used to trigger ALTERED
#define CSID_S_RADIUS_ADD_GROUP           _STA_RADIUS_GROUP(0x0101) //T=u32, D=0
#define CSID_S_RADIUS_DEL_GROUP           _STA_RADIUS_GROUP(0x0102) //T=u32, D=0
#define CSID_S_RADIUS_DEL_GROUP_USER      _STA_RADIUS_GROUP(0x0103) //T=u32, D=0
#define CSID_S_RADIUS_MOD_GROUP           _STA_RADIUS_GROUP(0x0104) //T=u32, D=0
//#define CSID_S_RADIUS_MOD_GROUP_USER    _STA_RADIUS_GROUP(0x0105) //T=u32, D=0
//#define CSID_S_RADIUS_MOD_GROUP_SERVICE _STA_RADIUS_GROUP(0x0106) //T=u32, D=0
#define CSID_S_RADIUS_GROUP_LIST          _STA_RADIUS_GROUP(0x0107) //T=u32, D=0

//add group
#define CSID_C_RADIUS_ADD_GROUP_NAME          _CFG_RADIUS_GROUP(0x1000) //T=str, D=""
#define CSID_C_RADIUS_ADD_GROUP_USER          _CFG_RADIUS_GROUP(0x1010) //T=str, D=""
#define CSID_C_RADIUS_ADD_GROUP_X_Auth        _CFG_RADIUS_GROUP(0x1030) //T=u32, D=0
#define CSID_C_RADIUS_ADD_GROUP_NAS           _CFG_RADIUS_GROUP(0x1031) //T=u32, D=0
#define CSID_C_RADIUS_ADD_GROUP_RADIUS        _CFG_RADIUS_GROUP(0x1032) //T=u32, D=0
#define CSID_C_RADIUS_ADD_GROUP_VPN           _CFG_RADIUS_GROUP(0x1033) //T=u32, D=0
#define CSID_C_RADIUS_ADD_GROUP_Accounting    _CFG_RADIUS_GROUP(0x1034) //T=u32, D=0
#define CSID_C_RADIUS_ADD_GROUP_Billing       _CFG_RADIUS_GROUP(0x1035) //T=u32, D=0                   
#define CSID_C_RADIUS_QOS_CONTROL             _CFG_RADIUS_GROUP(0x1036) //T=u32, D=0
#define CSID_C_RADIUS_QOS_RATE                _CFG_RADIUS_GROUP(0x1037) //T=str, D=""
#define CSID_C_RADIUS_ROUTING_IF              _CFG_RADIUS_GROUP(0x1038) //T=str, D=""
#define CSID_C_RADIUS_QOS_UNIT    _CFG_RADIUS_GROUP(0x1039) //T=u32, D=0

#define CSID_C_RADIUS_ADD_GROUP_ENABLE        _CFG_RADIUS_GROUP(0x1050) //T=u32, D=1

//delete group
#define CSID_C_DEL_GROUP_NAME        _CFG_RADIUS_GROUP(0x2000) //T=str, D=""

//modify group
#define CSID_C_MOD_GROUP_NAME_ORG    _CFG_RADIUS_GROUP(0x3000)  //T=str, D=""
#define CSID_C_MOD_GROUP_NAME        _CFG_RADIUS_GROUP(0x3001)  //T=str, D=""
#define CSID_C_MOD_GROUP_USER        _CFG_RADIUS_GROUP(0x3010)  //T=str, D=""
#define CSID_C_MOD_GROUP_X_Auth      _CFG_RADIUS_GROUP(0x3030)  //T=u32, D=0
#define CSID_C_MOD_GROUP_NAS         _CFG_RADIUS_GROUP(0x3031)  //T=u32, D=0
#define CSID_C_MOD_GROUP_RADIUS      _CFG_RADIUS_GROUP(0x3032)  //T=u32, D=0
#define CSID_C_MOD_GROUP_VPN         _CFG_RADIUS_GROUP(0x3033)  //T=u32, D=0
#define CSID_C_MOD_GROUP_Accounting  _CFG_RADIUS_GROUP(0x3034)  //T=u32, D=0
#define CSID_C_MOD_GROUP_Billing     _CFG_RADIUS_GROUP(0x3035)  //T=u32, D=0
#define CSID_C_MOD_GROUP_QOS_CONTROL _CFG_RADIUS_GROUP(0x3036) //T=u32, D=0
#define CSID_C_MOD_GROUP_QOS_RATE    _CFG_RADIUS_GROUP(0x3037) //T=str, D=""
#define CSID_C_MOD_GROUP_ROUTING_IF  _CFG_RADIUS_GROUP(0x3038) //T=str, D=""
#define CSID_C_MOD_GROUP_QOS_UNIT    _CFG_RADIUS_GROUP(0x3039) //T=u32, D=0

#define CSID_C_MOD_GROUP_ENABLE      _CFG_RADIUS_GROUP(0x3050)  //T=u32, D=1

//database to store informations of users
#define BEGIN_GROUP_NAME       0x4000
#define BEGIN_GROUP_USER1      0x4020
#define BEGIN_GROUP_USER2      0x4040
#define BEGIN_GROUP_USER3      0x4060
#define BEGIN_GROUP_USER4      0x4080
#define BEGIN_GROUP_USER5      0x40A0
#define BEGIN_GROUP_USER6      0x40C0
#define BEGIN_GROUP_USER7      0x40E0
#define BEGIN_GROUP_USER8      0x4100
#define BEGIN_GROUP_USER9      0x4120
#define BEGIN_GROUP_USER10     0x4140
#define BEGIN_GROUP_USER11     0x4160
#define BEGIN_GROUP_USER12     0x4180
#define BEGIN_GROUP_USER13     0x41A0
#define BEGIN_GROUP_USER14     0x41C0
#define BEGIN_GROUP_USER15     0x41E0
#define BEGIN_GROUP_USER16     0x4200
#define BEGIN_GROUP_USER17     0x4220
#define BEGIN_GROUP_USER18     0x4240
#define BEGIN_GROUP_USER19     0x4260
#define BEGIN_GROUP_USER20     0x4280
#define BEGIN_GROUP_USER21     0x42A0
#define BEGIN_GROUP_USER22     0x42C0
#define BEGIN_GROUP_USER23     0x42E0
#define BEGIN_GROUP_USER24     0x4300
#define BEGIN_GROUP_USER25     0x4320
#define BEGIN_GROUP_USER26     0x4340
#define BEGIN_GROUP_USER27     0x4360
#define BEGIN_GROUP_USER28     0x4380
#define BEGIN_GROUP_USER29     0x43A0
#define BEGIN_GROUP_USER30     0x43C0
#define BEGIN_GROUP_USER31     0x43E0
#define BEGIN_GROUP_USER32     0x4400
#define BEGIN_GROUP_X_Auth     0x4420
#define BEGIN_GROUP_NAS        0x4440
#define BEGIN_GROUP_RADIUS     0x4460
#define BEGIN_GROUP_VPN        0x4480
#define BEGIN_GROUP_Accounting 0x44A0
#define BEGIN_GROUP_Billing    0x44C0
#define BEGIN_GROUP_ENABLE     0x4600

//list
#define CSID_C_RADIUS_GROUP              _CFG_RADIUS_GROUP(0x4000)  //T=str, D="" ,group name
#define CSID_C_RADIUS_GROUP1_USER        _CFG_RADIUS_GROUP(0x4020)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP2_USER        _CFG_RADIUS_GROUP(0x4040)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP3_USER        _CFG_RADIUS_GROUP(0x4060)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP4_USER        _CFG_RADIUS_GROUP(0x4080)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP5_USER        _CFG_RADIUS_GROUP(0x40A0)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP6_USER        _CFG_RADIUS_GROUP(0x40C0)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP7_USER        _CFG_RADIUS_GROUP(0x40E0)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP8_USER        _CFG_RADIUS_GROUP(0x4100)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP9_USER        _CFG_RADIUS_GROUP(0x4120)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP10_USER       _CFG_RADIUS_GROUP(0x4140)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP11_USER       _CFG_RADIUS_GROUP(0x4160)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP12_USER       _CFG_RADIUS_GROUP(0x4180)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP13_USER       _CFG_RADIUS_GROUP(0x41A0)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP14_USER       _CFG_RADIUS_GROUP(0x41C0)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP15_USER       _CFG_RADIUS_GROUP(0x41E0)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP16_USER       _CFG_RADIUS_GROUP(0x4200)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP17_USER       _CFG_RADIUS_GROUP(0x4220)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP18_USER       _CFG_RADIUS_GROUP(0x4240)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP19_USER       _CFG_RADIUS_GROUP(0x4260)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP20_USER       _CFG_RADIUS_GROUP(0x4280)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP21_USER       _CFG_RADIUS_GROUP(0x42A0)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP22_USER       _CFG_RADIUS_GROUP(0x42C0)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP23_USER       _CFG_RADIUS_GROUP(0x42E0)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP24_USER       _CFG_RADIUS_GROUP(0x4300)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP25_USER       _CFG_RADIUS_GROUP(0x4320)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP26_USER       _CFG_RADIUS_GROUP(0x4340)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP27_USER       _CFG_RADIUS_GROUP(0x4360)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP28_USER       _CFG_RADIUS_GROUP(0x4380)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP29_USER       _CFG_RADIUS_GROUP(0x43A0)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP30_USER       _CFG_RADIUS_GROUP(0x43C0)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP31_USER       _CFG_RADIUS_GROUP(0x43E0)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP32_USER       _CFG_RADIUS_GROUP(0x4400)  //T=str, D="" ,user name
#define CSID_C_RADIUS_GROUP_X_Auth       _CFG_RADIUS_GROUP(0x4420)  //T=u32, D=0 
#define CSID_C_RADIUS_GROUP_NAS          _CFG_RADIUS_GROUP(0x4440)  //T=u32, D=0 
#define CSID_C_RADIUS_GROUP_RADIUS       _CFG_RADIUS_GROUP(0x4460)  //T=u32, D=0 
#define CSID_C_RADIUS_GROUP_VPN          _CFG_RADIUS_GROUP(0x4480)  //T=u32, D=0 
#define CSID_C_RADIUS_GROUP_Accounting   _CFG_RADIUS_GROUP(0x44A0)  //T=u32, D=0 
#define CSID_C_RADIUS_GROUP_Billing      _CFG_RADIUS_GROUP(0x44C0)  //T=u32, D=0 
#define CSID_C_RADIUS_GROUP_ENABLE       _CFG_RADIUS_GROUP(0x4600)  //T=u32, D=1, enable flag 
#define CSID_C_RADIUS_GROUP_QOS_CONTROL  _CFG_RADIUS_GROUP(0x4620) //T=u32, D=0
#define CSID_C_RADIUS_GROUP_QOS_RATE     _CFG_RADIUS_GROUP(0x4640) //T=str, D=""
#define CSID_C_RADIUS_GROUP_ROUTING_IF   _CFG_RADIUS_GROUP(0x4680) //T=str, D=""
#define CSID_C_RADIUS_GROUP_QOS_UNIT    _CFG_RADIUS_GROUP(0x46A0) //T=u32, D=0


#endif

