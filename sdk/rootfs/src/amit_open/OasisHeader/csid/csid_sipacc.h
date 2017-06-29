/*
 * @file Member ID Definition of Group SIP Account
 *
 * Moderator: Terence Yang
 * Group ID: 0x00220000/0x80220000
 */

#ifndef _CSID_SIPACC_H
#define _CSID_SIPACC_H

#include "csid_gid.h"

#define _CFG_SIPACC(x)		(CSID_GIDC_SIPACC|(x))
#define _STA_SIPACC(x)		(CSID_GIDS_SIPACC|(x))

/* Extra Definitions */

/* Configuration Items */

// Table 1 FXS Setting

#define CSID_C_SIPACC_SINGLE             _CFG_SIPACC(0x0001)	//T=u8, D=1, Single Account for all Ua, 0:Disable(Multi) 1:Enable (Single)

#define CSID_C_SIPACC_FXS_ACC            _CFG_SIPACC(0x0010)	//T=u8, D=1, Account, 0:Disable 1:Enable 
#define CSID_C_SIPACC_FXS_USER           _CFG_SIPACC(0x0020)	//T=str, D="2300", User Name
#define CSID_C_SIPACC_FXS_NAME           _CFG_SIPACC(0x0030)	//T=str, D="2300", Display Name   
#define CSID_C_SIPACC_FXS_AUTH           _CFG_SIPACC(0x0040)	//T=str, D="2300", Authentication User Name   
#define CSID_C_SIPACC_FXS_PASS           _CFG_SIPACC(0x0050)	//T=str, D="2300", Authentication Password  
#define CSID_C_SIPACC_FXS_PASSC          _CFG_SIPACC(0x0060)	//T=str, D="2300", Confirmed Password  
#define CSID_C_SIPACC_FXS_REALM          _CFG_SIPACC(0x0061)	//T=str, D="", SIP realm
#define CSID_C_SIPACC_FXS_MWI            _CFG_SIPACC(0x0070)	//T=u8, D=0, MWI Subscribe, 0: Disable 1:Enable 
#define CSID_C_SIPACC_FXS_MWI_USER       _CFG_SIPACC(0x0080)	//T=str, MWI User Name   
#define CSID_C_SIPACC_FXS_MWI_AUTH       _CFG_SIPACC(0x0090)	//T=str, MWI Authentication User Name   
#define CSID_C_SIPACC_FXS_MWI_PASS       _CFG_SIPACC(0x00a0)	//T=str, MWI Authentication Password  
#define CSID_C_SIPACC_FXS_MWI_PASSC      _CFG_SIPACC(0x00b0)	//T=str, MWI Confirmed Password  
#define CSID_C_SIPACC_FXS_MWI_TIME       _CFG_SIPACC(0x00c0)	//T=u16, D=360, MWI Refresh Timeout 
#define CSID_C_SIPACC_FXS_MWI_SRV_URI    _CFG_SIPACC(0x00c1)	//T=str, D="fxs1.mwi.com", MWI server URI 
#define CSID_C_SIPACC_FXS_PA             _CFG_SIPACC(0x00d0)	//T=u8, D=0, P-Asserted, 0:Disable 1:Enable
#define CSID_C_SIPACC_FXS_PA_USER        _CFG_SIPACC(0x00e0)	//T=str, D="2300", Asserted Identity Username   
#define CSID_C_SIPACC_FXS_PA_NAME        _CFG_SIPACC(0x00f0)	//T=str, D="2300", Asserted Identity Displayname   
#define CSID_C_SIPACC_FXS2_ACC           _CFG_SIPACC(0x0110)	//T=u8, D=1, Account, 0:Disable 1:Enable 
#define CSID_C_SIPACC_FXS2_USER          _CFG_SIPACC(0x0120)	//T=str, D="2300", User Name
#define CSID_C_SIPACC_FXS2_NAME          _CFG_SIPACC(0x0130)	//T=str, D="2300", Display Name   
#define CSID_C_SIPACC_FXS2_AUTH          _CFG_SIPACC(0x0140)	//T=str, D="2300", Authentication User Name   
#define CSID_C_SIPACC_FXS2_PASS          _CFG_SIPACC(0x0150)	//T=str, D="2300", Authentication Password  
#define CSID_C_SIPACC_FXS2_REALM         _CFG_SIPACC(0x0151)	//T=str, D="", SIP realm
#define CSID_C_SIPACC_FXS2_MWI           _CFG_SIPACC(0x0160)	//T=u8, D=0, MWI Subscribe, 0: Disable 1:Enable 
#define CSID_C_SIPACC_FXS2_MWI_USER      _CFG_SIPACC(0x0170)	//T=str, MWI User Name   
#define CSID_C_SIPACC_FXS2_MWI_AUTH      _CFG_SIPACC(0x0180)	//T=str, MWI Authentication User Name   
#define CSID_C_SIPACC_FXS2_MWI_PASS      _CFG_SIPACC(0x0190)	//T=str, MWI Authentication Password  
#define CSID_C_SIPACC_FXS2_MWI_PASSC     _CFG_SIPACC(0x01a0)	//T=str, MWI Confirmed Password  
#define CSID_C_SIPACC_FXS2_MWI_TIME      _CFG_SIPACC(0x01b0)	//T=u16, D=3600, MWI Refresh Timeout 
#define CSID_C_SIPACC_FXS2_MWI_SRV_URI   _CFG_SIPACC(0x01c1)	//T=str, D="fxs2.mwi.com", MWI server URI 

#define CSID_C_SIPACC_FXS3_ACC           _CFG_SIPACC(0x0310)	//T=u8, D=1, Account, 0:Disable 1:Enable 
#define CSID_C_SIPACC_FXS3_USER          _CFG_SIPACC(0x0320)	//T=str, D="2300", User Name
#define CSID_C_SIPACC_FXS3_NAME          _CFG_SIPACC(0x0330)	//T=str, D="2300", Display Name   
#define CSID_C_SIPACC_FXS3_AUTH          _CFG_SIPACC(0x0340)	//T=str, D="2300", Authentication User Name   
#define CSID_C_SIPACC_FXS3_PASS          _CFG_SIPACC(0x0350)	//T=str, D="2300", Authentication Password  
#define CSID_C_SIPACC_FXS3_REALM         _CFG_SIPACC(0x0351)	//T=str, D="", SIP realm
#define CSID_C_SIPACC_FXS3_MWI           _CFG_SIPACC(0x0360)	//T=u8, D=0, MWI Subscribe, 0: Disable 1:Enable 
#define CSID_C_SIPACC_FXS3_MWI_USER      _CFG_SIPACC(0x0370)	//T=str, MWI User Name   
#define CSID_C_SIPACC_FXS3_MWI_AUTH      _CFG_SIPACC(0x0380)	//T=str, MWI Authentication User Name   
#define CSID_C_SIPACC_FXS3_MWI_PASS      _CFG_SIPACC(0x0390)	//T=str, MWI Authentication Password  
#define CSID_C_SIPACC_FXS3_MWI_PASSC     _CFG_SIPACC(0x03a0)	//T=str, MWI Confirmed Password  
#define CSID_C_SIPACC_FXS3_MWI_TIME      _CFG_SIPACC(0x03b0)	//T=u16, D=3600, MWI Refresh Timeout 
#define CSID_C_SIPACC_FXS3_MWI_SRV_URI   _CFG_SIPACC(0x03c1)	//T=str, D="fxs2.mwi.com", MWI server URI 

#define CSID_C_SIPACC_FXS4_ACC           _CFG_SIPACC(0x0410)	//T=u8, D=1, Account, 0:Disable 1:Enable 
#define CSID_C_SIPACC_FXS4_USER          _CFG_SIPACC(0x0420)	//T=str, D="2300", User Name
#define CSID_C_SIPACC_FXS4_NAME          _CFG_SIPACC(0x0430)	//T=str, D="2300", Display Name   
#define CSID_C_SIPACC_FXS4_AUTH          _CFG_SIPACC(0x0440)	//T=str, D="2300", Authentication User Name   
#define CSID_C_SIPACC_FXS4_PASS          _CFG_SIPACC(0x0450)	//T=str, D="2300", Authentication Password  
#define CSID_C_SIPACC_FXS4_REALM         _CFG_SIPACC(0x0451)	//T=str, D="", SIP realm
#define CSID_C_SIPACC_FXS4_MWI           _CFG_SIPACC(0x0460)	//T=u8, D=0, MWI Subscribe, 0: Disable 1:Enable 
#define CSID_C_SIPACC_FXS4_MWI_USER      _CFG_SIPACC(0x0470)	//T=str, MWI User Name   
#define CSID_C_SIPACC_FXS4_MWI_AUTH      _CFG_SIPACC(0x0480)	//T=str, MWI Authentication User Name   
#define CSID_C_SIPACC_FXS4_MWI_PASS      _CFG_SIPACC(0x0490)	//T=str, MWI Authentication Password  
#define CSID_C_SIPACC_FXS4_MWI_PASSC     _CFG_SIPACC(0x04a0)	//T=str, MWI Confirmed Password  
#define CSID_C_SIPACC_FXS4_MWI_TIME      _CFG_SIPACC(0x04b0)	//T=u16, D=3600, MWI Refresh Timeout 
#define CSID_C_SIPACC_FXS4_MWI_SRV_URI   _CFG_SIPACC(0x04c1)	//T=str, D="fxs2.mwi.com", MWI server URI 

#define CSID_C_SIPACC_FXS5_ACC           _CFG_SIPACC(0x0510)	//T=u8, D=1, Account, 0:Disable 1:Enable 
#define CSID_C_SIPACC_FXS5_USER          _CFG_SIPACC(0x0520)	//T=str, D="2300", User Name
#define CSID_C_SIPACC_FXS5_NAME          _CFG_SIPACC(0x0530)	//T=str, D="2300", Display Name   
#define CSID_C_SIPACC_FXS5_AUTH          _CFG_SIPACC(0x0540)	//T=str, D="2300", Authentication User Name   
#define CSID_C_SIPACC_FXS5_PASS          _CFG_SIPACC(0x0550)	//T=str, D="2300", Authentication Password  
#define CSID_C_SIPACC_FXS5_REALM         _CFG_SIPACC(0x0551)	//T=str, D="", SIP realm
#define CSID_C_SIPACC_FXS5_MWI           _CFG_SIPACC(0x0560)	//T=u8, D=0, MWI Subscribe, 0: Disable 1:Enable 
#define CSID_C_SIPACC_FXS5_MWI_USER      _CFG_SIPACC(0x0570)	//T=str, MWI User Name   
#define CSID_C_SIPACC_FXS5_MWI_AUTH      _CFG_SIPACC(0x0580)	//T=str, MWI Authentication User Name   
#define CSID_C_SIPACC_FXS5_MWI_PASS      _CFG_SIPACC(0x0590)	//T=str, MWI Authentication Password  
#define CSID_C_SIPACC_FXS5_MWI_PASSC     _CFG_SIPACC(0x05a0)	//T=str, MWI Confirmed Password  
#define CSID_C_SIPACC_FXS5_MWI_TIME      _CFG_SIPACC(0x05b0)	//T=u16, D=3600, MWI Refresh Timeout 
#define CSID_C_SIPACC_FXS5_MWI_SRV_URI   _CFG_SIPACC(0x05c1)	//T=str, D="fxs2.mwi.com", MWI server URI 

// Table 2 FXO Setting

#define CSID_C_SIPACC_FXO_ACC           _CFG_SIPACC(0x0210)	//T=u8, D=1, Account, 0:Disable 1:Enable
#define CSID_C_SIPACC_FXO_USER          _CFG_SIPACC(0x0220)	//T=str, D="2301", User Name
#define CSID_C_SIPACC_FXO_NAME          _CFG_SIPACC(0x0230)	//T=str, D="2301", Display Name
#define CSID_C_SIPACC_FXO_AUTH          _CFG_SIPACC(0x0240)	//T=str, D="2301", Authentication User Name
#define CSID_C_SIPACC_FXO_PASS          _CFG_SIPACC(0x0250)	//T=str, D="2301", Authentication Password
#define CSID_C_SIPACC_FXO_PASSC         _CFG_SIPACC(0x0260)	//T=str, D="2301", Confirmed Password
#define CSID_C_SIPACC_FXO_MWI           _CFG_SIPACC(0x0270)	//T=u8, D=0, MWI Subscribe, 0: Disable 1:Enable
#define CSID_C_SIPACC_FXO_MWI_USER      _CFG_SIPACC(0x0280)	//T=str, MWI User Name
#define CSID_C_SIPACC_FXO_MWI_AUTH      _CFG_SIPACC(0x0290)	//T=str, MWI Authentication User Name
#define CSID_C_SIPACC_FXO_MWI_PASS      _CFG_SIPACC(0x02a0)	//T=str, MWI Authentication Password
#define CSID_C_SIPACC_FXO_MWI_PASSC     _CFG_SIPACC(0x02b0)	//T=str, MWI Confirmed Password
#define CSID_C_SIPACC_FXO_MWI_TIME      _CFG_SIPACC(0x02c0)	//T=u16, D=3600, MWI Refresh Timeout
#define CSID_C_SIPACC_FXO_PA            _CFG_SIPACC(0x02d0)	//T=u8, D=0, P-Asserted, 0:Disable 1:Enable
#define CSID_C_SIPACC_FXO_PA_USER       _CFG_SIPACC(0x02e0)	//T=str, D="2301", Asserted Identity Username
#define CSID_C_SIPACC_FXO_PA_NAME       _CFG_SIPACC(0x02f0)	//T=str, D="2301", Asserted Identity Displayname

/* Status Items */

/* Multisession Items (start fromn 0x2000) */

#endif //ifndef _CSID_SIPACC_H
