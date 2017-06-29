/*
 * @file Member ID Definition of Group xDSL Router Function.
 *
 * Moderator: Eric Chen
 * Group ID: 0x00410000/0x80410000
 */

#ifndef _CSID_XDSL_H
#define _CSID_XDSL_H

#include "csid_gid.h"

#define _CFG_XDSL(x)        (CSID_GIDC_XDSL|(x))		//0x00410000
#define _STA_XDSL(x)        (CSID_GIDS_XDSL|(x))		//0x80410000


#define	ATM_QOS_UBR		0x0
#define ATM_QOS_CBR		0x1
#define ATM_QOS_VBR		0x2
#define ATM_QOS_GFR		0x3
#define ATM_QOS_NRT_VBR		0x4

#define	ATM_ENCAPSULATION_LLC	0x0
#define	ATM_ENCAPSULATION_VCMUX	0x1

#define T1_413			0x0
#define	G_LITE			0x1
#define	G_DMT			0x2
#define	ADSL2			0x3
#define	ADSL2PLUS		0x4
#define	MULTIMODE		0x5

#define	ANNEX_A			0x0
#define	ANNEX_I			0x1
#define	ANNEX_AL		0x2
#define	ANNEX_M			0x3
#define	ANNEX_AIJLM		0x4
#define	ANNEX_B			0x5
#define	ANNEX_BJM		0x6
#define	ANNEX_J			0x7

#define ADSL_OFFLINE         0x0
#define ADSL_WAITFORINIT     0x1
#define ADSL_INIT            0x2
#define ADSL_ONLINE          0x3

/* vdsl profile	
#define PROFILE_30a		0x200
#define PROFILE_17b		0x100
#define PROFILE_12b		0x020
#define PROFILE_12a		0x010
#define PROFILE_8d		0x008
#define PROFILE_8c		0x004
#define PROFILE_8b		0x002
#define PROFILE_8a		0x001
*/


/* Configuration Items */ 

#define CSID_C_XDSL_SUPPORT			_CFG_XDSL(0x0001)	//T=u32, D=0, xDSL modem support(default disable) 
#define CSID_C_XDSL_MODE			_CFG_XDSL(0x0002)	//T=u32, D=5, 0=T1.413,1=G.lite,2=G.dmt,3=ADSL,4=ADSL2+,5=Multimode
#define CSID_C_XDSL_TYPE			_CFG_XDSL(0x0003)	//T=u32, D=2, 0=A,1=I,2=A/L,3=M,4=A/I/J/L/M
#define CSID_C_XDSL_PASSWORD		_CFG_XDSL(0x0004)	//T=str, xDSL modem login password
#define CSID_C_XDSL_CPE_MACADDR		_CFG_XDSL(0x0005)	//T=mac, xDSL modem MAC address
#define CSID_C_XDSL_FW_VER			_CFG_XDSL(0x0006)	//T=str, xDSL FW version
#define CSID_C_XDSL_DEBUG_MODE		_CFG_XDSL(0x0007)	//T=u8,  D=0, xDSL debug mode 0:disable,1:enable
#define CSID_C_XDSL_IP            _CFG_XDSL(0x0008) //T=u8, D=5, xDSL ip

/* vdsl profile	*/
#define CSID_C_XDSL_VDSL2_PROFILE	_CFG_XDSL(0x0010)	//T=u32, D=0x33F, 200=30a,100=17a,020=12b,010=12a,008=8d,004=8c,002=8b,001=8a
#define CSID_C_XDSL_GHS_CARRIER_SET	_CFG_XDSL(0x0011)	//T=u8, D=0, 0=Auto,1=A43,2=B43,7=V43
#define CSID_C_XDSL_VDSL2_MODE		_CFG_XDSL(0x0012)	//T=u8, D=0, 0=CPE, 1=CO
#define CSID_C_XDSL_HAVE_VDSL		_CFG_XDSL(0x0013)	//T=u8, D=0, 1: have vdsl
#define CSID_C_XDSL_VDSL2_TIMEOUT       _CFG_XDSL(0x0014)       //T=u32, D=100, ,VDSL Connect timeout


/* PVC gap	*/
#define PVC_GAP	0x10
#define CSID_C_XDSL_PVC_BASE		_CFG_XDSL(0x0101)	
/* PVC0	*/
#define CSID_C_XDSL_PVC0_ACTIVE		_CFG_XDSL(0x0101)	//T=u8, D=1, 0:disable,1:enable
#define CSID_C_XDSL_PVC0_VPI		_CFG_XDSL(0x0102)	//T=u8, 0~255
#define CSID_C_XDSL_PVC0_VCI		_CFG_XDSL(0x0103)	//T=u16, 1~65535
#define CSID_C_XDSL_PVC0_ENCAPSULATION	_CFG_XDSL(0x0104)	//T=u8,	0=LLC,1=VC
#define CSID_C_XDSL_PVC0_ATM_QOSTYPE	_CFG_XDSL(0x0105)	//T=u8, D=0, 0=UBR, 1=CBR, 2=VBR, 3=GFR
#define CSID_C_XDSL_PVC0_ATM_QOS_PCR	_CFG_XDSL(0x0106)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC0_ATM_QOS_SCR	_CFG_XDSL(0x0107)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC0_ATM_QOS_MBS	_CFG_XDSL(0x0108)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC0_F5_LOOP_ENABLE _CFG_XDSL(0x0109)	//T=u8,  D=0, xDSL oam F5 loopback enable 0:disable,1:enable
#define CSID_C_XDSL_PVC0_VID           	_CFG_XDSL(0x010A)       //T=u8, vlan id
#define CSID_C_XDSL_PVC0_BRIDGE		_CFG_XDSL(0x010B)       //T=u8, D=0, use for bridge or not
#define CSID_C_XDSL_PVC0_COUNTRY	_CFG_XDSL(0x010C)       //T=u8, D=0, Country selection for vpi/vci applied
#define CSID_C_XDSL_PVC0_ISP		_CFG_XDSL(0x010D)       //T=u8, D=0, ISP selection for vpi/vci applied
#define CSID_C_XDSL_PVC0_USE_RULE_NO _CFG_XDSL(0x010E)      //T=u8, D=0, use_rule number for scheduling

/* PVC1	*/
#define CSID_C_XDSL_PVC1_ACTIVE		_CFG_XDSL(0x0111)	//T=u8, D=1, 0:disable,1:enable
#define CSID_C_XDSL_PVC1_VPI		_CFG_XDSL(0x0112)	//T=u8, 0~255
#define CSID_C_XDSL_PVC1_VCI		_CFG_XDSL(0x0113)	//T=u16, 1~65535
#define CSID_C_XDSL_PVC1_ENCAPSULATION	_CFG_XDSL(0x0114)	//T=u8,	0=LLC,1=VC
#define CSID_C_XDSL_PVC1_ATM_QOSTYPE	_CFG_XDSL(0x0115)	//T=u8, D=0, 0=UBR, 1=CBR, 2=VBR, 3=GFR
#define CSID_C_XDSL_PVC1_ATM_QOS_PCR	_CFG_XDSL(0x0116)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC1_ATM_QOS_SCR	_CFG_XDSL(0x0117)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC1_ATM_QOS_MBS	_CFG_XDSL(0x0118)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC1_F5_LOOP_ENABLE _CFG_XDSL(0x0119)	//T=u8,  D=0, xDSL oam F5 loopback enable 0:disable,1:enable
#define CSID_C_XDSL_PVC1_VID           	_CFG_XDSL(0x011A)       //T=u8, vlan id
#define CSID_C_XDSL_PVC1_BRIDGE		_CFG_XDSL(0x011B)       //T=u8, D=0, use for bridge or not
#define CSID_C_XDSL_PVC1_COUNTRY	_CFG_XDSL(0x011C)       //T=u8, D=0, Country selection for vpi/vci applied
#define CSID_C_XDSL_PVC1_ISP		_CFG_XDSL(0x011D)       //T=u8, D=0, ISP selection for vpi/vci applied

/* PVC2	*/
#define CSID_C_XDSL_PVC2_ACTIVE		_CFG_XDSL(0x0121)	//T=u8, D=1, 0:disable,1:enable
#define CSID_C_XDSL_PVC2_VPI		_CFG_XDSL(0x0122)	//T=u8, 0~255
#define CSID_C_XDSL_PVC2_VCI		_CFG_XDSL(0x0123)	//T=u16, 1~65535
#define CSID_C_XDSL_PVC2_ENCAPSULATION	_CFG_XDSL(0x0124)	//T=u8,	0=LLC,1=VC
#define CSID_C_XDSL_PVC2_ATM_QOSTYPE	_CFG_XDSL(0x0125)	//T=u8, D=0, 0=UBR, 1=CBR, 2=VBR, 3=GFR
#define CSID_C_XDSL_PVC2_ATM_QOS_PCR	_CFG_XDSL(0x0126)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC2_ATM_QOS_SCR	_CFG_XDSL(0x0127)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC2_ATM_QOS_MBS	_CFG_XDSL(0x0128)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC2_F5_LOOP_ENABLE _CFG_XDSL(0x0129)	//T=u8,  D=0, xDSL oam F5 loopback enable 0:disable,1:enable
#define CSID_C_XDSL_PVC2_VID           	_CFG_XDSL(0x012A)       //T=u8, vlan id
#define CSID_C_XDSL_PVC2_BRIDGE		_CFG_XDSL(0x012B)       //T=u8, D=0, use for bridge or not
#define CSID_C_XDSL_PVC2_COUNTRY	_CFG_XDSL(0x012C)       //T=u8, D=0, Country selection for vpi/vci applied
#define CSID_C_XDSL_PVC2_ISP		_CFG_XDSL(0x012D)       //T=u8, D=0, ISP selection for vpi/vci applied

/* PVC3	*/
#define CSID_C_XDSL_PVC3_ACTIVE		_CFG_XDSL(0x0131)	//T=u8, D=1, 0:disable,1:enable
#define CSID_C_XDSL_PVC3_VPI		_CFG_XDSL(0x0132)	//T=u8, 0~255
#define CSID_C_XDSL_PVC3_VCI		_CFG_XDSL(0x0133)	//T=u16, 1~65535
#define CSID_C_XDSL_PVC3_ENCAPSULATION	_CFG_XDSL(0x0134)	//T=u8,	0=LLC,1=VC
#define CSID_C_XDSL_PVC3_ATM_QOSTYPE	_CFG_XDSL(0x0135)	//T=u8, D=0, 0=UBR, 1=CBR, 2=VBR, 3=GFR
#define CSID_C_XDSL_PVC3_ATM_QOS_PCR	_CFG_XDSL(0x0136)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC3_ATM_QOS_SCR	_CFG_XDSL(0x0137)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC3_ATM_QOS_MBS	_CFG_XDSL(0x0138)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC3_F5_LOOP_ENABLE _CFG_XDSL(0x0139)	//T=u8,  D=0, xDSL oam F5 loopback enable 0:disable,1:enable
#define CSID_C_XDSL_PVC3_VID            _CFG_XDSL(0x013A)       //T=u8, vlan id
#define CSID_C_XDSL_PVC3_BRIDGE		_CFG_XDSL(0x013B)       //T=u8, D=0, use for bridge or not
#define CSID_C_XDSL_PVC3_COUNTRY	_CFG_XDSL(0x013C)       //T=u8, D=0, Country selection for vpi/vci applied
#define CSID_C_XDSL_PVC3_ISP		_CFG_XDSL(0x013D)       //T=u8, D=0, ISP selection for vpi/vci applied

/* PVC4	*/
#define CSID_C_XDSL_PVC4_ACTIVE		_CFG_XDSL(0x0141)	//T=u8, D=1, 0:disable,1:enable
#define CSID_C_XDSL_PVC4_VPI		_CFG_XDSL(0x0142)	//T=u8, 0~255
#define CSID_C_XDSL_PVC4_VCI		_CFG_XDSL(0x0143)	//T=u16, 1~65535
#define CSID_C_XDSL_PVC4_ENCAPSULATION	_CFG_XDSL(0x0144)	//T=u8,	0=LLC,1=VC
#define CSID_C_XDSL_PVC4_ATM_QOSTYPE	_CFG_XDSL(0x0145)	//T=u8, D=0, 0=UBR, 1=CBR, 2=VBR, 3=GFR
#define CSID_C_XDSL_PVC4_ATM_QOS_PCR	_CFG_XDSL(0x0146)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC4_ATM_QOS_SCR	_CFG_XDSL(0x0147)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC4_ATM_QOS_MBS	_CFG_XDSL(0x0148)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC4_F5_LOOP_ENABLE _CFG_XDSL(0x0149)	//T=u8,  D=0, xDSL oam F5 loopback enable 0:disable,1:enable
#define CSID_C_XDSL_PVC4_VID            _CFG_XDSL(0x014A)       //T=u8, vlan id
#define CSID_C_XDSL_PVC4_BRIDGE		_CFG_XDSL(0x014B)       //T=u8, D=0, use for bridge or not
#define CSID_C_XDSL_PVC4_COUNTRY	_CFG_XDSL(0x014C)       //T=u8, D=0, Country selection for vpi/vci applied
#define CSID_C_XDSL_PVC4_ISP		_CFG_XDSL(0x014D)       //T=u8, D=0, ISP selection for vpi/vci applied

/* PVC5	*/
#define CSID_C_XDSL_PVC5_ACTIVE		_CFG_XDSL(0x0151)	//T=u8, D=1, 0:disable,1:enable
#define CSID_C_XDSL_PVC5_VPI		_CFG_XDSL(0x0152)	//T=u8, 0~255
#define CSID_C_XDSL_PVC5_VCI		_CFG_XDSL(0x0153)	//T=u16, 1~65535
#define CSID_C_XDSL_PVC5_ENCAPSULATION	_CFG_XDSL(0x0154)	//T=u8,	0=LLC,1=VC
#define CSID_C_XDSL_PVC5_ATM_QOSTYPE	_CFG_XDSL(0x0155)	//T=u8, D=0, 0=UBR, 1=CBR, 2=VBR, 3=GFR
#define CSID_C_XDSL_PVC5_ATM_QOS_PCR	_CFG_XDSL(0x0156)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC5_ATM_QOS_SCR	_CFG_XDSL(0x0157)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC5_ATM_QOS_MBS	_CFG_XDSL(0x0158)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC5_F5_LOOP_ENABLE _CFG_XDSL(0x0159)	//T=u8,  D=0, xDSL oam F5 loopback enable 0:disable,1:enable
#define CSID_C_XDSL_PVC5_VID           	_CFG_XDSL(0x015A)       //T=u8, vlan id
#define CSID_C_XDSL_PVC5_BRIDGE		_CFG_XDSL(0x015B)       //T=u8, D=0, use for bridge or not
#define CSID_C_XDSL_PVC5_COUNTRY	_CFG_XDSL(0x015C)       //T=u8, D=0, Country selection for vpi/vci applied
#define CSID_C_XDSL_PVC5_ISP		_CFG_XDSL(0x015D)       //T=u8, D=0, ISP selection for vpi/vci applied

/* PVC6	*/
#define CSID_C_XDSL_PVC6_ACTIVE		_CFG_XDSL(0x0161)	//T=u8, D=1, 0:disable,1:enable
#define CSID_C_XDSL_PVC6_VPI		_CFG_XDSL(0x0162)	//T=u8, 0~255
#define CSID_C_XDSL_PVC6_VCI		_CFG_XDSL(0x0163)	//T=u16, 1~65535
#define CSID_C_XDSL_PVC6_ENCAPSULATION	_CFG_XDSL(0x0164)	//T=u8,	0=LLC,1=VC
#define CSID_C_XDSL_PVC6_ATM_QOSTYPE	_CFG_XDSL(0x0165)	//T=u8, D=0, 0=UBR, 1=CBR, 2=VBR, 3=GFR
#define CSID_C_XDSL_PVC6_ATM_QOS_PCR	_CFG_XDSL(0x0166)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC6_ATM_QOS_SCR	_CFG_XDSL(0x0167)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC6_ATM_QOS_MBS	_CFG_XDSL(0x0168)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC6_F5_LOOP_ENABLE _CFG_XDSL(0x0169)	//T=u8,  D=0, xDSL oam F5 loopback enable 0:disable,1:enable
#define CSID_C_XDSL_PVC6_VID           	_CFG_XDSL(0x016A)       //T=u8, vlan id
#define CSID_C_XDSL_PVC6_BRIDGE		_CFG_XDSL(0x016B)       //T=u8, D=0, use for bridge or not
#define CSID_C_XDSL_PVC6_COUNTRY	_CFG_XDSL(0x016C)       //T=u8, D=0, Country selection for vpi/vci applied
#define CSID_C_XDSL_PVC6_ISP		_CFG_XDSL(0x016D)       //T=u8, D=0, ISP selection for vpi/vci applied

/* PVC7	*/
#define CSID_C_XDSL_PVC7_ACTIVE		_CFG_XDSL(0x0171)	//T=u8, D=1, 0:disable,1:enable
#define CSID_C_XDSL_PVC7_VPI		_CFG_XDSL(0x0172)	//T=u8, 0~255
#define CSID_C_XDSL_PVC7_VCI		_CFG_XDSL(0x0173)	//T=u16, 1~65535
#define CSID_C_XDSL_PVC7_ENCAPSULATION	_CFG_XDSL(0x0174)	//T=u8,	0=LLC,1=VC
#define CSID_C_XDSL_PVC7_ATM_QOSTYPE	_CFG_XDSL(0x0175)	//T=u8, D=0, 0=UBR, 1=CBR, 2=VBR, 3=GFR
#define CSID_C_XDSL_PVC7_ATM_QOS_PCR	_CFG_XDSL(0x0176)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC7_ATM_QOS_SCR	_CFG_XDSL(0x0177)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC7_ATM_QOS_MBS	_CFG_XDSL(0x0178)	//T=u8, D=0, 
#define CSID_C_XDSL_PVC7_F5_LOOP_ENABLE _CFG_XDSL(0x0179)	//T=u8,  D=0, xDSL oam F5 loopback enable 0:disable,1:enable
#define CSID_C_XDSL_PVC7_VID           	_CFG_XDSL(0x017A)       //T=u8, vlan id
#define CSID_C_XDSL_PVC7_BRIDGE		_CFG_XDSL(0x017B)       //T=u8, D=0, use for bridge or not
#define CSID_C_XDSL_PVC7_COUNTRY	_CFG_XDSL(0x017C)       //T=u8, D=0, Country selection for vpi/vci applied
#define CSID_C_XDSL_PVC7_ISP		_CFG_XDSL(0x017D)       //T=u8, D=0, ISP selection for vpi/vci applied

/* AutoPVC*/
#define CSID_C_XDSL_AUTOPVC_ACTIVE		_CFG_XDSL(0x0180)	//T=u8, D=0, 0:disable,1:enable
#define CSID_C_XDSL_AUTOPVC_ENCAPSULATION	_CFG_XDSL(0x0181)	//T=u8,	0=LLC,1=VC
/* Reserved 32 items for vcpool*/
#define CSID_C_XDSL_AUTOPVC_VCPOOL_PVC		_CFG_XDSL(0x0190)	//T=u8, pvc index
#define CSID_C_XDSL_AUTOPVC_VCPOOL_VPI		_CFG_XDSL(0x01B0)	//T=u8, 0~255
#define CSID_C_XDSL_AUTOPVC_VCPOOL_VCI		_CFG_XDSL(0x01D0)	//T=u16, 1~65535

/* Status Items */

#define CSID_S_XDSL_ALTERED			_STA_XDSL(0x0001)	//T=u32, setting is altered
#define CSID_S_XDSL_SUPPORT_STATUS	_STA_XDSL(0x0002)	//T=u32, ADSL support status

#define CSID_S_XDSL_LINESTATE		_STA_XDSL(0x0011)	//T=u8, 0=OFFLINE, 1:Wait for init, 2:Init, 3:ONLINE
#define CSID_S_XDSL_MODULATION		_STA_XDSL(0x0012)	//T=u8, 0=T1.413,1=G.lite,2=G.dmt,3=ADSL2,4=ADSL2+,5=Multimode
#define CSID_S_XDSL_SNR_UP			_STA_XDSL(0x0013)	//T=str, SNR up, if ADSL is down, value=N/A
#define CSID_S_XDSL_SNR_DOWN		_STA_XDSL(0x0014)	//T=str, SNR down, if ADSL is down, value=N/A
#define CSID_S_XDSL_ATTEN_UP		_STA_XDSL(0x0015)	//T=str, ATTEN up, if ADSL is down, value=N/A
#define CSID_S_XDSL_ATTEN_DOWN		_STA_XDSL(0x0016)	//T=str, ATTEN down, if ADSL is down, value=N/A
#define CSID_S_XDSL_DATARATE_UP		_STA_XDSL(0x0017)	//T=str, DATARATE up, if ADSL is down, value=N/A
#define CSID_S_XDSL_DATARATE_DOWN	_STA_XDSL(0x0018)	//T=str, DATARATE down, if ADSL is down, value=N/A
#define CSID_S_XDSL_PASSWORD		_STA_XDSL(0x0019)	//T=str, xDSL modem current password
#define CSID_S_XDSL_CPE_MACADDR		_STA_XDSL(0x001A)	//T=mac, xDSL modem MAC address
#define CSID_S_XDSL_FW_VER			_STA_XDSL(0x001B)	//T=str, xDSL FW version
#define CSID_S_XDSL_CUR_MODE_SETTING	_STA_XDSL(0x001C)	//T=u8, 0=T1.413,1=G.lite,2=G.dmt,3=ADSL2,4=ADSL2+,5=Multimode
#define CSID_S_XDSL_CUR_TYPE_SETTING	_STA_XDSL(0x001D)	//T=u8, 0=A,1=I,2=A/L,3=M,4=A/I/J/L/M
#define CSID_S_XDSL_DEBUG_STATUS	_STA_XDSL(0x001E)	//T=u8, 0:init,1:start,2:complete

#define CSID_S_XDSL_PVC_LINK_STATUS	_STA_XDSL(0x0100)	//T=u8, PVC BASE index, 0=disconnect,1=connected
#define CSID_S_XDSL_PVC0_LINK_STATUS	_STA_XDSL(0x0100)	//T=u8, PVC0 link status, 0=disconnect,1=connected
#define CSID_S_XDSL_PVC1_LINK_STATUS	_STA_XDSL(0x0101)	//T=u8, PVC1 link status, 0=disconnect,1=connected
#define CSID_S_XDSL_PVC2_LINK_STATUS	_STA_XDSL(0x0102)	//T=u8, PVC2 link status, 0=disconnect,1=connected
#define CSID_S_XDSL_PVC3_LINK_STATUS	_STA_XDSL(0x0103)	//T=u8, PVC3 link status, 0=disconnect,1=connected
#define CSID_S_XDSL_PVC4_LINK_STATUS	_STA_XDSL(0x0104)	//T=u8, PVC4 link status, 0=disconnect,1=connected
#define CSID_S_XDSL_PVC5_LINK_STATUS	_STA_XDSL(0x0105)	//T=u8, PVC5 link status, 0=disconnect,1=connected
#define CSID_S_XDSL_PVC6_LINK_STATUS	_STA_XDSL(0x0106)	//T=u8, PVC6 link status, 0=disconnect,1=connected
#define CSID_S_XDSL_PVC7_LINK_STATUS	_STA_XDSL(0x0107)	//T=u8, PVC7 link status, 0=disconnect,1=connected
#define CSID_S_XDSL_BEGIN_ADSL	_STA_XDSL(0x0108)	//T=u32, begin adsl, 1=begin,

#define CSID_S_XDSL_PVC_NUM					_STA_XDSL(0x01FF)	//T=u8,  pvc num: 0~7 for F5 LOOP ping test
#define CSID_S_XDSL_PVC_F5_LOOP_MODE    	_STA_XDSL(0x0200)	//T=u8,  0:seg init 1:seg check,2:end-to-end init 3: end-to-end check
#define CSID_S_XDSL_PVC_F5_LOOP_STATUS	    _STA_XDSL(0x0201)	//T=str, OK or FAIL
#define CSID_S_XDSL_PVC_F5_LOOP_SEG_STATUS	_STA_XDSL(0x0202)	//T=u8,  0:init,1:ok 2:fail
#define CSID_S_XDSL_PVC_F5_LOOP_END_STATUS	_STA_XDSL(0x0203)	//T=u8,  0:init,1:ok 2:fail
#define CSID_S_XDSL_PVC0_F5_LOOP_MODE   	_STA_XDSL(0x0200)	//T=u8,  0:seg init 1:seg check,2:end-to-end init 3: end-to-end check
#define CSID_S_XDSL_PVC0_F5_LOOP_STATUS	    _STA_XDSL(0x0201)	//T=str, OK or FAIL
#define CSID_S_XDSL_PVC0_F5_LOOP_SEG_STATUS	_STA_XDSL(0x0202)	//T=u8,  0:init,1:ok 2:fail
#define CSID_S_XDSL_PVC0_F5_LOOP_END_STATUS	_STA_XDSL(0x0203)	//T=u8,  0:init,1:ok 2:fail
#define CSID_S_XDSL_PVC1_F5_LOOP_MODE   	_STA_XDSL(0x0210)	//T=u8,  0:seg init 1:seg check,2:end-to-end init 3: end-to-end check
#define CSID_S_XDSL_PVC1_F5_LOOP_STATUS	    _STA_XDSL(0x0211)	//T=str, OK or FAIL
#define CSID_S_XDSL_PVC1_F5_LOOP_SEG_STATUS	_STA_XDSL(0x0212)	//T=u8,  0:init,1:ok 2:fail
#define CSID_S_XDSL_PVC1_F5_LOOP_END_STATUS	_STA_XDSL(0x0213)	//T=u8,  0:init,1:ok 2:fail

#define CSID_S_XDSL_TX_FRAME		_STA_XDSL(0x0030)	//T=u32, TX traffic
#define CSID_S_XDSL_RX_FRAME		_STA_XDSL(0x0031)	//T=u32, RX traffic

/* VDSL STATUS	*/
#define CSID_S_XDSL_LINK_STATUS	    		_STA_XDSL(0x1001)	//T=u8,  0=IDLE,1=SHOWTIME,2=HANDSHAKE,3=TRAINING
#define CSID_S_XDSL_FIRMWARE_VERSION	    _STA_XDSL(0x1002)	//T=str,  VDSL FIRMWARE VERSION
#define CSID_S_XDSL_LINE_RATE_US		    _STA_XDSL(0x1003)	//T=str,  VDSL Line Rate Upstream(Resrv.)
#define CSID_S_XDSL_LINE_RATE_DS		    _STA_XDSL(0x1004)	//T=str,  VDSL Line Rate Downstream(Resrv.) 
#define CSID_S_XDSL_ACTURE_RATE_US		    _STA_XDSL(0x1005)	//T=str,  VDSL Actual Data Rate Upstream
#define CSID_S_XDSL_ACTURE_RATE_DS		    _STA_XDSL(0x1006)	//T=str,  VDSL Actual Data Rate Downstream
#define CSID_S_XDSL_SNR_US			 	   	_STA_XDSL(0x1007)	//T=str,  VDSL SNR Margin Upstream
#define CSID_S_XDSL_SNR_DS				    _STA_XDSL(0x1008)	//T=str,  VDSL SNR Margin Downstream

#define CSID_S_XDSL_ACTURE_DELAY_US		    _STA_XDSL(0x1009)	//T=str,  VDSL Actual delay Upstream
#define CSID_S_XDSL_ACTURE_DELAY_DS		    _STA_XDSL(0x100A)	//T=str,  VDSL Actual delay Downstream
#define CSID_S_XDSL_ACTURE_INP_US		    _STA_XDSL(0x100B)	//T=str,  VDSL Actual INP Upstream
#define CSID_S_XDSL_ACTURE_INP_DS		    _STA_XDSL(0x100C)	//T=str,  VDSL Actual INP Downstream
#define CSID_S_XDSL_15M_CV_US		    	_STA_XDSL(0x100D)	//T=str,  VDSL 15M CV Upstream
#define CSID_S_XDSL_15M_CV_DS			    _STA_XDSL(0x100E)	//T=str,  VDSL 15M CV Downstream
#define CSID_S_XDSL_1DAY_CV_US		    	_STA_XDSL(0x100F)	//T=str,  VDSL 1Day CV Upstream
#define CSID_S_XDSL_1DAY_CV_DS			    _STA_XDSL(0x1010)	//T=str,  VDSL 1Day CV Downstream
#define CSID_S_XDSL_15M_FEC_US			    _STA_XDSL(0x1011)	//T=str,  VDSL 15M FEC Upstream
#define CSID_S_XDSL_15M_FEC_DS			    _STA_XDSL(0x1012)	//T=str,  VDSL 15M FEC Downstream
#define CSID_S_XDSL_1DAY_FEC_US		    	_STA_XDSL(0x1013)	//T=str,  VDSL 1Day FEC Upstream
#define CSID_S_XDSL_1DAY_FEC_DS		    	_STA_XDSL(0x1014)	//T=str,  VDSL 1Day FEC Downstream
#define CSID_S_XDSL_TOTAL_FEC_US		    _STA_XDSL(0x1015)	//T=str,  VDSL Total FEC Downstream
#define CSID_S_XDSL_TOTAL_FEC_DS		    _STA_XDSL(0x1016)	//T=str,  VDSL Total FEC Downstream
#define CSID_S_XDSL_PRE_DATA_RATE_US		_STA_XDSL(0x1017)	//T=str,  VDSL Previous Data Rate Upstream
#define CSID_S_XDSL_PRE_DATA_RATE_DS		_STA_XDSL(0x1018)	//T=str,  VDSL Previous Data Rate Downstream
#define CSID_S_XDSL_ATTAINABLE_RATE_US	    _STA_XDSL(0x1019)	//T=str,  VDSL Attainable Rate Upstream
#define CSID_S_XDSL_ATTAINABLE_RATE_DS	    _STA_XDSL(0x101A)	//T=str,  VDSL Attainable Rate Downstream
#define CSID_S_XDSL_ELECTRICAL_LENGTH_US	_STA_XDSL(0x101B)	//T=str,  VDSL Electrical Length Upstream
#define CSID_S_XDSL_ELECTRICAL_LENGTH_DS	_STA_XDSL(0x101C)	//T=str,  VDSL Electrical Length Downstream
#define CSID_S_XDSL_SNR_MARGIN1_US		    _STA_XDSL(0x101D)	//T=str,  VDSL SNR Margin1 Upstream
#define CSID_S_XDSL_SNR_MARGIN1_DS		    _STA_XDSL(0x101E)	//T=str,  VDSL SNR Margin1 Downstream
#define CSID_S_XDSL_SNR_MARGIN2_US		    _STA_XDSL(0x101F)	//T=str,  VDSL SNR Margin2 Upstream
#define CSID_S_XDSL_SNR_MARGIN2_DS		    _STA_XDSL(0x1020)	//T=str,  VDSL SNR Margin2 Downstream
#define CSID_S_XDSL_SNR_MARGIN3_US		    _STA_XDSL(0x1021)	//T=str,  VDSL SNR Margin3 Upstream
#define CSID_S_XDSL_SNR_MARGIN3_DS		    _STA_XDSL(0x1022)	//T=str,  VDSL SNR Margin3 Downstream
#define CSID_S_XDSL_SNR_MARGIN4_US		    _STA_XDSL(0x1023)	//T=str,  VDSL SNR Margin4 Upstream
#define CSID_S_XDSL_SNR_MARGIN4_DS		    _STA_XDSL(0x1024)	//T=str,  VDSL SNR Margin4 Downstream
#define CSID_S_XDSL_SNR_MARGIN5_US		    _STA_XDSL(0x1025)	//T=str,  VDSL SNR Margin5 Upstream
#define CSID_S_XDSL_SNR_MARGIN5_DS		    _STA_XDSL(0x1026)	//T=str,  VDSL SNR Margin5 Downstream
#define CSID_S_XDSL_15M_ELAPSED_TIME_US 	_STA_XDSL(0x1027)	//T=str,  VDSL 15M Elapsed time Upstream
#define CSID_S_XDSL_15M_ELAPSED_TIME_DS 	_STA_XDSL(0x1028)	//T=str,  VDSL 15M Elapsed time Downstream
#define CSID_S_XDSL_15M_FECS_US		    	_STA_XDSL(0x1029)	//T=str,  VDSL 15M FECS Upstream
#define CSID_S_XDSL_15M_FECS_DS		    	_STA_XDSL(0x102A)	//T=str,  VDSL 15M FECS Downstream
#define CSID_S_XDSL_15M_ES_US		    	_STA_XDSL(0x102B)	//T=str,  VDSL 15M ES Upstream
#define CSID_S_XDSL_15M_ES_DS		    	_STA_XDSL(0x102C)	//T=str,  VDSL 15M ES Downstream
#define CSID_S_XDSL_15M_SES_US		    	_STA_XDSL(0x102D)	//T=str,  VDSL 15M SES Upstream
#define CSID_S_XDSL_15M_SES_DS		    	_STA_XDSL(0x102E)	//T=str,  VDSL 15M SES Downstream
#define CSID_S_XDSL_15M_LOSS_US		    	_STA_XDSL(0x102F)	//T=str,  VDSL 15M LOSS Upstream
#define CSID_S_XDSL_15M_LOSS_DS		    	_STA_XDSL(0x1030)	//T=str,  VDSL 15M LOSS Downstream
#define CSID_S_XDSL_15M_UAS_US		    	_STA_XDSL(0x1031)	//T=str,  VDSL 15M UAS Upstream
#define CSID_S_XDSL_15M_UAS_DS		    	_STA_XDSL(0x1032)	//T=str,  VDSL 15M UAS Downstream
#define CSID_S_XDSL_1DAY_ELAPSED_TIME_US	_STA_XDSL(0x1033)	//T=str,  VDSL 1Day Elapsed tim Upstreame
#define CSID_S_XDSL_1DAY_ELAPSED_TIME_DS	_STA_XDSL(0x1034)	//T=str,  VDSL 1Day Elapsed time Downstream
#define CSID_S_XDSL_1DAY_FECS_US		    _STA_XDSL(0x1035)	//T=str,  VDSL 1Day FECS Upstream
#define CSID_S_XDSL_1DAY_FECS_DS		    _STA_XDSL(0x1036)	//T=str,  VDSL 1Day FECS Downstream
#define CSID_S_XDSL_1DAY_ES_US		   	 	_STA_XDSL(0x1037)	//T=str,  VDSL 1Day ES Upstream
#define CSID_S_XDSL_1DAY_ES_DS		   	 	_STA_XDSL(0x1038)	//T=str,  VDSL 1Day ES Downstream
#define CSID_S_XDSL_1DAY_SES_US		    	_STA_XDSL(0x1039)	//T=str,  VDSL 1Day SES Upstream
#define CSID_S_XDSL_1DAY_SES_DS		    	_STA_XDSL(0x103A)	//T=str,  VDSL 1Day SES Downstream
#define CSID_S_XDSL_1DAY_LOSS_US 		    _STA_XDSL(0x103B)	//T=str,  VDSL 1Day LOSS Upstream
#define CSID_S_XDSL_1DAY_LOSS_DS 		    _STA_XDSL(0x103C)	//T=str,  VDSL 1Day LOSS Downstream
#define CSID_S_XDSL_1DAY_UAS_US		    	_STA_XDSL(0x103D)	//T=str,  VDSL 1Day UAS Upstream
#define CSID_S_XDSL_1DAY_UAS_DS		    	_STA_XDSL(0x103E)	//T=str,  VDSL 1Day UAS Downstream
#define CSID_S_XDSL_TOTAL_FECS_US		    _STA_XDSL(0x103F)	//T=str,  VDSL Total FECS Upstream
#define CSID_S_XDSL_TOTAL_FECS_DS		    _STA_XDSL(0x1040)	//T=str,  VDSL Total FECS Downstream
#define CSID_S_XDSL_TOTAL_ES_US		    	_STA_XDSL(0x1041)	//T=str,  VDSL Total ES Upstream
#define CSID_S_XDSL_TOTAL_ES_DS		    	_STA_XDSL(0x1042)	//T=str,  VDSL Total ES Downstream
#define CSID_S_XDSL_TOTAL_SES_US		    _STA_XDSL(0x1043)	//T=str,  VDSL Total SES Upstream
#define CSID_S_XDSL_TOTAL_SES_DS		    _STA_XDSL(0x1044)	//T=str,  VDSL Total SES Downstream
#define CSID_S_XDSL_TOTAL_LOSS_US		    _STA_XDSL(0x1045)	//T=str,  VDSL Total LOSS Upstream
#define CSID_S_XDSL_TOTAL_LOSS_DS		    _STA_XDSL(0x1046)	//T=str,  VDSL Total LOSS Downstream
#define CSID_S_XDSL_TOTAL_UAS_US		    _STA_XDSL(0x1047)	//T=str,  VDSL Total UAS Upstream
#define CSID_S_XDSL_TOTAL_UAS_DS		    _STA_XDSL(0x1048)	//T=str,  VDSL Total UAS Downstream
#define CSID_S_XDSL_POWER_US		 	    _STA_XDSL(0x1049)	//T=str, POWER upstream, if ADSL is down, value=N/A
#define CSID_S_XDSL_POWER_DS		            _STA_XDSL(0x104A)	//T=str, POWER downstream, if ADSL is down, value=N/A
#define CSID_S_XDSL_DATA_PATH		            _STA_XDSL(0x104B)       //T=u8, D=0, 0= None,1=Fast,2=Interleave
#define CSID_S_XDSL_INTERLEAVE_DEPTH	            _STA_XDSL(0x104C)	//T=str, interleave depth, if ADSL is down, value=N/A

#define CSID_S_XDSL_VDSL2_ACT_PROFILE		   	 	_STA_XDSL(0x1081)	//T=u8,  VDSL actual profile, 0=8a,1=8b,2=8c,3=8d,4=12a,5=12b,6=17a,7=30a
#define CSID_S_XDSL_VDSL2_PROFILE       _STA_XDSL(0x1082)       //T=u32, D=0x33F, 200=30a,100=17a,020=12b,010=12a,008=8d,004=8c,002=8b,001=8a
#define CSID_S_XDSL_GHS_CARRIER_SET     _STA_XDSL(0x1083)       //T=u8, D=0, 0=Auto,1=A43,2=B43,7=V43
#define CSID_S_XDSL_AUTOPVC_STATE       _STA_XDSL(0x1084)       //T=u8, D=0, 0=Start,1=Probing,2=Finish,3=Fail

#endif //ifndef _CSID_XDSL_H
