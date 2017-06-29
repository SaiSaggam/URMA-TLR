/*
 * @file Member ID Definition of Group VoIP FXO port
 *
 * Moderator: Terence Yang
 * Group ID: 0x00290000/0x80290000
 */

#ifndef _CSID_VOIPFXO_H
#define _CSID_VOIPFXO_H

#include "csid_gid.h"

#define _CFG_VOIPFXO(x)		(CSID_GIDC_VOIPFXO|(x))
#define _STA_VOIPFXO(x)		(CSID_GIDS_VOIPFXO|(x))

/* Extra Definitions */

#define DIAL_Auto_Detection         0x00
#define DIAL_DTMF_PULSE             0x01
#define DIAL_20PPS_PULSE            0x02
#define DIAL_10PPS                  0x03

#define DOMAIN_Argentina            0x00
#define DOMAIN_Australia            0x01
#define DOMAIN_Austria              0x02
#define DOMAIN_Bahrain              0x03
#define DOMAIN_Belgium              0x04
#define DOMAIN_Brazil               0x05
#define DOMAIN_Bulgaria             0x06
#define DOMAIN_Canada               0x07
#define DOMAIN_Chile                0x08
#define DOMAIN_China                0x09
#define DOMAIN_Colombia             0x0a
#define DOMAIN_Croatia              0x0b
#define DOMAIN_Cyprus               0x0c
#define DOMAIN_CzechRepublic        0x0d
#define DOMAIN_Denmark              0x0e
#define DOMAIN_Ecuador              0x0f
#define DOMAIN_Egypt                0x10
#define DOMAIN_ElSalvador           0x11
#define DOMAIN_Finland              0x12
#define DOMAIN_France               0x13
#define DOMAIN_Germany              0x14
#define DOMAIN_Greece               0x15
#define DOMAIN_Guam                 0x16
#define DOMAIN_HongKong             0x17
#define DOMAIN_Hungary              0x18
#define DOMAIN_Iceland              0x19
#define DOMAIN_India                0x1a
#define DOMAIN_Indonesia            0x1b
#define DOMAIN_Ireland              0x1c
#define DOMAIN_Israel               0x1d
#define DOMAIN_Italy                0x1e
#define DOMAIN_Japan                0x1f
#define DOMAIN_Jordan               0x20
#define DOMAIN_Kazakhstan           0x21
#define DOMAIN_Kuwait               0x22
#define DOMAIN_Latvia               0x23
#define DOMAIN_Lebanon              0x24
#define DOMAIN_Luxembourg           0x25
#define DOMAIN_Macao                0x26
#define DOMAIN_Malaysia             0x27
#define DOMAIN_Malta                0x28
#define DOMAIN_Mexico               0x29
#define DOMAIN_Morocco              0x2a
#define DOMAIN_Netherlands          0x2b
#define DOMAIN_NewZealand           0x2c
#define DOMAIN_Nigeria              0x2d
#define DOMAIN_Norway               0x2f
#define DOMAIN_Oman                 0x30
#define DOMAIN_Pakistan             0x31
#define DOMAIN_Peru                 0x32
#define DOMAIN_Philippines          0x33
#define DOMAIN_Poland               0x34
#define DOMAIN_Portugal             0x35
#define DOMAIN_Romania              0x36
#define DOMAIN_Russia               0x37
#define DOMAIN_SaudiArabia          0x38
#define DOMAIN_Singapore            0x39
#define DOMAIN_Slovakia             0x3a
#define DOMAIN_Slovenia             0x3b
#define DOMAIN_SouthAfrica          0x3c
#define DOMAIN_SouthKorea           0x3d
#define DOMAIN_Spain                0x3e
#define DOMAIN_Sweden               0x3f
#define DOMAIN_Switzerland          0x40
#define DOMAIN_Taiwan               0x41
#define DOMAIN_TBR21                0x42
#define DOMAIN_Thailand             0x43
#define DOMAIN_UAE                  0x44
#define DOMAIN_UnitedKingdom        0x45
#define DOMAIN_USA                  0x46
#define DOMAIN_Yemen                0x47

/* Configuration Items */

// FXO Port Setting
// Line 1 (FXO 0)
#define CSID_C_VOIPFXO_DIAL            _CFG_VOIPFXO(0x0010)	//T=u8, D=0, Dial Method
#define CSID_C_VOIPFXO_DOMAIN          _CFG_VOIPFXO(0x0020)	//T=u8, Regulation Domain
#define CSID_C_VOIPFXO_Tx_Level        _CFG_VOIPFXO(0x0030)	//T=u8, D=6, Voice Tx Level   1 2 3 4 5 6 7 8 (default:6)
#define CSID_C_VOIPFXO_Rx_Level        _CFG_VOIPFXO(0x0040)	//T=u8, D=6, Voice Rx Level   1 2 3 4 5 6 7 8 (default:6)
#define CSID_C_VOIPFXO_OFFHOOK         _CFG_VOIPFXO(0x0050)	//T=u8, check fxo offhook
#define CSID_C_VOIPFXO_OCCUPY_UA       _CFG_VOIPFXO(0x0051)     //T=u8, D=0, record which UA occupy PSTN

/* Status Items */

/* Multisession Items (start fromn 0x2000) */

#endif //ifndef _CSID_VOIPFXO_H
