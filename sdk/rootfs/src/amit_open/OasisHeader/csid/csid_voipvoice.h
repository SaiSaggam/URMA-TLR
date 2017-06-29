/*
 * @file Member ID Definition of Group VoIP Voice setting
 *
 * Moderator: Terence Yang
 * Group ID: 0x00250000/0x80250000
 */

#ifndef _CSID_VOIPVOICE_H
#define _CSID_VOIPVOICE_H

#include "csid_gid.h"

#define _CFG_VOIPVOICE(x)		(CSID_GIDC_VOIPVOICE|(x))
#define _STA_VOIPVOICE(x)		(CSID_GIDS_VOIPVOICE|(x))

/* Extra Definitions */

#define PACKET_10msec           0x00
#define PACKET_20msec           0x01
#define PACKET_30msec           0x02

#define ML_TYPE_NONE            0x00
#define ML_TYPE_MEDIA           0x01
#define ML_TYPE_ENCAPRTP        0x02
#define ML_TYPE_RTPLLOPBACK     0x03

#define CODEC_G711_Ulaw         0x00
#define CODEC_G711_Alaw         0x01
#define CODEC_G729AB            0x02
#define CODEC_G7231             0x03
#define CODEC_G726_16K          0x04
#define CODEC_G726_24K          0x05
#define CODEC_G726_32K          0x06
#define CODEC_G726_40K          0x07
#define CODEC_G722              0x08
#define CODEC_G722P1            0x09
#define CODEC_ILBC              0x0A
#define CODEC_NONE              0x0B

#define DTMF_IN_PASS            0x00
#define DTMF_IN_PCMU            0x01
#define DTMF_IN_PCMA            0x02
#define DTMF_OUT_2833           0x03

#define VAD_Disabled            0x00
#define VAD_Silence_Supression  0x01
#define VAD_No_CNG              0x02
#define VAD_Only_G711_AnnexII   0x03
#define VAD_Codec_Specific_CN   0x04

//@D2_csid : need to confirm the range of CN rate with vendor
#define VAD_CN_RATE_10ms	0x00
#define VAD_CN_RATE_20ms	0x01
#define VAD_CN_RATE_30ms	0x02
#define VAD_CN_RATE_40ms	0x03
#define VAD_CN_RATE_50ms	0x04


#define LEC_TL_Disabled    0x00
#define LEC_TL_8msec       0x01
#define LEC_TL_16msec      0x02
#define LEC_TL_24msec      0x03
#define LEC_TL_32msec      0x04
#define LEC_TL_36msec      0x05
#define LEC_TL_64msec      0x06

#define AEC_TL_Disabled    0x00 
#define AEC_TL_32msec      0x01
#define AEC_TL_64msec      0x02
#define AEC_TL_96msec      0x03
#define AEC_TL_128msec     0x04

#define DTMFR_2833		0x01
#define DTMFR_INBAND	0x02
#define DTMFR_SIPINFO	0x03

/* Configuration Items */

// Voice Setting 
#define CSID_C_VOIPVOICE_PACKET          _CFG_VOIPVOICE(0x0001)	//T=u8, D=1, Packet Length
#define CSID_C_VOIPVOICE_G726_16K        _CFG_VOIPVOICE(0x0002)	//T=u16, D=96, G.726 16K Payload Type Value   
#define CSID_C_VOIPVOICE_G726_24K        _CFG_VOIPVOICE(0x0003)	//T=u16, D=97, G.726 24K Payload Type Value   
#define CSID_C_VOIPVOICE_G726_32K        _CFG_VOIPVOICE(0x0004)	//T=u16, D=98, G.726 32K Payload Type Value   
#define CSID_C_VOIPVOICE_G726_40K        _CFG_VOIPVOICE(0x0005)	//T=u16, D=99, G.726 40K Payload Type Value   
#define CSID_C_VOIPVOICE_ILBC            _CFG_VOIPVOICE(0x0006)	//T=u16, D=104, ILBC Payload Type Value   
#define CSID_C_VOIPVOICE_OUT_2833        _CFG_VOIPVOICE(0x0007)	//T=u16, D=1, DTMF relay type 
#define CSID_C_VOIPVOICE_ML_Encaprtp     _CFG_VOIPVOICE(0x0008)	//T=u8, D=112, Media Loopback Encaprtp Payload Type Value (100..120) 
#define CSID_C_VOIPVOICE_ML_Rtploopback  _CFG_VOIPVOICE(0x0009)	//T=u8, D=113, Media Loopback Rtploopback Payload Type Value (100..120)  
#define CSID_C_VOIPVOICE_ML_TYPE         _CFG_VOIPVOICE(0x000a)	//T=u8, D=0, Media Loopback Type
#define CSID_C_VOIPVOICE_RTP_TO          _CFG_VOIPVOICE(0x000b)	//T=u8, D=25, RTP Timeout second (5..100)  
#define CSID_C_VOIPVOICE_VO_QUALITY      _CFG_VOIPVOICE(0x000c)	//T=u8, D=20, Voice Quality Poor Threshold % (5..100)  
#define CSID_C_VOIPVOICE_MAX_ICMP        _CFG_VOIPVOICE(0x000d)	//T=u16, D=10, Maximum ICMP Unreachable  (0..1000)  

// Lines
// Line with odd number (FXS)
#define CSID_C_VOIPVOICE_FXS_CODEC_DISABLE _CFG_VOIPVOICE(0x00f0)	//T=u16, D=0, Disable some codecs  
#define CSID_C_VOIPVOICE_FXS_CODEC_1       _CFG_VOIPVOICE(0x0100)	//T=u8, D=1, Codec Priority 1 (w/o None)
#define CSID_C_VOIPVOICE_FXS_CODEC_2       _CFG_VOIPVOICE(0x0110)	//T=u8, D=0, Codec Priority 2
#define CSID_C_VOIPVOICE_FXS_CODEC_3       _CFG_VOIPVOICE(0x0120)	//T=u8, D=2, Codec Priority 3
#define CSID_C_VOIPVOICE_FXS_CODEC_4       _CFG_VOIPVOICE(0x0130)	//T=u8, D=3, Codec Priority 4
#define CSID_C_VOIPVOICE_FXS_CODEC_5       _CFG_VOIPVOICE(0x0140)	//T=u8, D=4, Codec Priority 5
#define CSID_C_VOIPVOICE_FXS_CODEC_6       _CFG_VOIPVOICE(0x0150)	//T=u8, D=5, Codec Priority 6
#define CSID_C_VOIPVOICE_FXS_CODEC_7       _CFG_VOIPVOICE(0x0160)	//T=u8, D=6, Codec Priority 7
#define CSID_C_VOIPVOICE_FXS_CODEC_8       _CFG_VOIPVOICE(0x0170)	//T=u8, D=7, Codec Priority 8
#define CSID_C_VOIPVOICE_FXS_CODEC_9       _CFG_VOIPVOICE(0x0180)	//T=u8, D=8, Codec Priority 9
#define CSID_C_VOIPVOICE_FXS_G723          _CFG_VOIPVOICE(0x0190)	//T=u8, D=1, G.723 Rate, 0:5.3 Kbps 1:6.3 Kbps
#define CSID_C_VOIPVOICE_FXS_G726          _CFG_VOIPVOICE(0x01a0)	//T=u8, D=0, G.726 Bit Pack Format, 0:RFC 3551 1:I366.2
#define CSID_C_VOIPVOICE_FXS_ILBC          _CFG_VOIPVOICE(0x01b0)	//T=u8, D=1, iLBC mode, 0:20 msec. 1:30 msec.  
#define CSID_C_VOIPVOICE_FXS_DTMF          _CFG_VOIPVOICE(0x01c0)	//T=u8, D=0, DTMF Method
#define CSID_C_VOIPVOICE_FXS_VAD           _CFG_VOIPVOICE(0x01d0)	//T=u8, D=0, Voice Active Detector
#define CSID_C_VOIPVOICE_FXS_CN_RATE       _CFG_VOIPVOICE(0x01d1)	//T=u8, D=30, comfort noice packet rate in ms
#define CSID_C_VOIPVOICE_FXS_LEC_TL        _CFG_VOIPVOICE(0x01e0)	//T=u8, D=2, Line Echo Canceller Tail Length
#define CSID_C_VOIPVOICE_FXS_AEC_TL        _CFG_VOIPVOICE(0x01f0)	//T=u8, D=1. Acoustic Echo Canceller Tail Length
#define CSID_C_VOIPVOICE_FXS_LGC_TX        _CFG_VOIPVOICE(0x0200)	//T=u8, D=0, Linear Gain Control Tx  dbm(0..40)  
#define CSID_C_VOIPVOICE_FXS_LGC_RX        _CFG_VOIPVOICE(0x0210)	//T=u8, D=0, Linear Gain Control Rx  dbm(0..40)  

// Line with even number (FXO) 
#define CSID_C_VOIPVOICE_FXO_CODEC_1     _CFG_VOIPVOICE(0x0300)	//T=u8, D=1, Codec Priority 1 (w/o None)
#define CSID_C_VOIPVOICE_FXO_CODEC_2     _CFG_VOIPVOICE(0x0310)	//T=u8, D=0, Codec Priority 2
#define CSID_C_VOIPVOICE_FXO_CODEC_3     _CFG_VOIPVOICE(0x0320)	//T=u8, D=2, Codec Priority 3
#define CSID_C_VOIPVOICE_FXO_CODEC_4     _CFG_VOIPVOICE(0x0330)	//T=u8, D=3, Codec Priority 4
#define CSID_C_VOIPVOICE_FXO_CODEC_5     _CFG_VOIPVOICE(0x0340)	//T=u8, D=4, Codec Priority 5
#define CSID_C_VOIPVOICE_FXO_CODEC_6     _CFG_VOIPVOICE(0x0350)	//T=u8, D=5, Codec Priority 6
#define CSID_C_VOIPVOICE_FXO_CODEC_7     _CFG_VOIPVOICE(0x0360)	//T=u8, D=6, Codec Priority 7
#define CSID_C_VOIPVOICE_FXO_CODEC_8     _CFG_VOIPVOICE(0x0370)	//T=u8, D=7, Codec Priority 8
#define CSID_C_VOIPVOICE_FXO_CODEC_9     _CFG_VOIPVOICE(0x0380)	//T=u8, D=8, Codec Priority 9
#define CSID_C_VOIPVOICE_FXO_G723        _CFG_VOIPVOICE(0x0390)	//T=u8, D=1, G.723 Rate, 0:5.3 Kbps 1:6.3 Kbps
#define CSID_C_VOIPVOICE_FXO_G726        _CFG_VOIPVOICE(0x03a0)	//T=u8, D=0, G.726 Bit Pack Format,0:RFC 3551 1:I366.2
#define CSID_C_VOIPVOICE_FXO_ILBC        _CFG_VOIPVOICE(0x03b0)	//T=u8, D=1, iLBC mode, 0:20 msec. 1:30 msec.
#define CSID_C_VOIPVOICE_FXO_DTMF        _CFG_VOIPVOICE(0x03c0)	//T=u8, D=0, DTMF Method
#define CSID_C_VOIPVOICE_FXO_VAD         _CFG_VOIPVOICE(0x03d0)	//T=u8, D=0, Voice Active Detector
#define CSID_C_VOIPVOICE_FXO_LEC_TL      _CFG_VOIPVOICE(0x03e0)	//T=u8, D=2, Line Echo Canceller Tail Length
#define CSID_C_VOIPVOICE_FXO_AEC_TL      _CFG_VOIPVOICE(0x03f0)	//T=u8, D=1, Acoustic Echo Canceller Tail Length
#define CSID_C_VOIPVOICE_FXO_LGC_TX      _CFG_VOIPVOICE(0x0400)	//T=u8, D=0, Linear Gain Control Tx  dbm(0..40)
#define CSID_C_VOIPVOICE_FXO_LGC_RX      _CFG_VOIPVOICE(0x0410)	//T=u8, D=0, Linear Gain Control Rx  dbm(0..40)


//@D2_csid : the FXO config above should be used foe FXS2 in BDQ231, 
//	         to avoid misunderstand and keep the compatibility to BDQ231
//			 we add new def for FXS2
// Line 2(FXS2) 
#define CSID_C_VOIPVOICE_FXS2_CODEC_1     _CFG_VOIPVOICE(0x0500)	//T=u8, D=1, Codec Priority 1 (w/o None)
#define CSID_C_VOIPVOICE_FXS2_CODEC_2     _CFG_VOIPVOICE(0x0510)	//T=u8, D=0, Codec Priority 2
#define CSID_C_VOIPVOICE_FXS2_CODEC_3     _CFG_VOIPVOICE(0x0520)	//T=u8, D=2, Codec Priority 3
#define CSID_C_VOIPVOICE_FXS2_CODEC_4     _CFG_VOIPVOICE(0x0530)	//T=u8, D=3, Codec Priority 4
#define CSID_C_VOIPVOICE_FXS2_CODEC_5     _CFG_VOIPVOICE(0x0540)	//T=u8, D=4, Codec Priority 5
#define CSID_C_VOIPVOICE_FXS2_CODEC_6     _CFG_VOIPVOICE(0x0550)	//T=u8, D=5, Codec Priority 6
#define CSID_C_VOIPVOICE_FXS2_CODEC_7     _CFG_VOIPVOICE(0x0560)	//T=u8, D=6, Codec Priority 7
#define CSID_C_VOIPVOICE_FXS2_CODEC_8     _CFG_VOIPVOICE(0x0570)	//T=u8, D=7, Codec Priority 8
#define CSID_C_VOIPVOICE_FXS2_CODEC_9     _CFG_VOIPVOICE(0x0580)	//T=u8, D=8, Codec Priority 9
#define CSID_C_VOIPVOICE_FXS2_G723        _CFG_VOIPVOICE(0x0590)	//T=u8, D=1, G.723 Rate, 0:5.3 Kbps 1:6.3 Kbps
#define CSID_C_VOIPVOICE_FXS2_G726        _CFG_VOIPVOICE(0x05a0)	//T=u8, D=0, G.726 Bit Pack Format,0:RFC 3551 1:I366.2
#define CSID_C_VOIPVOICE_FXS2_ILBC        _CFG_VOIPVOICE(0x05b0)	//T=u8, D=1, iLBC mode, 0:20 msec. 1:30 msec.
#define CSID_C_VOIPVOICE_FXS2_DTMF        _CFG_VOIPVOICE(0x05c0)	//T=u8, D=0, DTMF Method
#define CSID_C_VOIPVOICE_FXS2_VAD         _CFG_VOIPVOICE(0x05d0)	//T=u8, D=0, Voice Active Detector
#define CSID_C_VOIPVOICE_FXS2_CN_RATE     _CFG_VOIPVOICE(0x05d1)	//T=u8, D=30, comfort noice packet rate in ms
#define CSID_C_VOIPVOICE_FXS2_LEC_TL      _CFG_VOIPVOICE(0x05e0)	//T=u8, D=2, Line Echo Canceller Tail Length
#define CSID_C_VOIPVOICE_FXS2_AEC_TL      _CFG_VOIPVOICE(0x05f0)	//T=u8, D=1, Acoustic Echo Canceller Tail Length
#define CSID_C_VOIPVOICE_FXS2_LGC_TX      _CFG_VOIPVOICE(0x0600)	//T=u8, D=0, Linear Gain Control Tx  dbm(0..40)
#define CSID_C_VOIPVOICE_FXS2_LGC_RX      _CFG_VOIPVOICE(0x0610)	//T=u8, D=0, Linear Gain Control Rx  dbm(0..40)
#define CSID_C_VOIPVOICE_PACKET_FXS2      _CFG_VOIPVOICE(0x0620)	//T=u8, D=1, Packet Length
 
 
/* Status Items */
#define CSID_S_VOIPVOICE_OUT_2833        _STA_VOIPVOICE(0x0010)	//T=u16, D=101, DTMF relay type 
#define CSID_S_VOIPVOICE_OUT_2833_EN     _STA_VOIPVOICE(0x0020)	//T=u16, D=1, out of band relay enable 
#define CSID_S_VOIPVOICE_FXS_CODEC_1     _STA_VOIPVOICE(0x0030)	//T=u8, D=1, Codec Priority 1 for vendor definition
#define CSID_S_VOIPVOICE_FXS_CODEC_2     _STA_VOIPVOICE(0x0040)	//T=u8, D=1, Codec Priority 2 for vendor definition
#define CSID_S_VOIPVOICE_FXS_CODEC_3     _STA_VOIPVOICE(0x0050)	//T=u8, D=1, Codec Priority 3 for vendor definition
#define CSID_S_VOIPVOICE_FXS_CODEC_4     _STA_VOIPVOICE(0x0060)	//T=u8, D=1, Codec Priority 4 for vendor definition

#define CSID_S_VOIPVOICE_FXS2_CODEC_1     _STA_VOIPVOICE(0x0070)	//T=u8, D=1, Codec Priority 1 for vendor definition
#define CSID_S_VOIPVOICE_FXS2_CODEC_2     _STA_VOIPVOICE(0x0080)	//T=u8, D=1, Codec Priority 2 for vendor definition
#define CSID_S_VOIPVOICE_FXS2_CODEC_3     _STA_VOIPVOICE(0x0090)	//T=u8, D=1, Codec Priority 3 for vendor definition
#define CSID_S_VOIPVOICE_FXS2_CODEC_4     _STA_VOIPVOICE(0x00a0)	//T=u8, D=1, Codec Priority 4 for vendor definition
/* Multisession Items (start fromn 0x2000) */

#define CSID_S_VOIPVOICE_SELECT_CPU_3G    _STA_VOIPVOICE(0x00b0)	//T=u8, D=0, SLIC to cpu ;D=1, SLIC to 3G
#endif //ifndef _CSID_VOIPVOICE_H
