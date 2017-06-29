/*
 * @file Member ID Definition of Group Test.
 *
 * Moderator: anybody
 * Group ID: 0x0FFF0000/0x8FFF0000
 */

#ifndef _CSID_TEST_H
#define _CSID_TEST_H

#include "csid_gid.h"

#define _CFG_TEST(x)		(CSID_GIDC_TEST|(x))
#define _STA_TEST(x)		(CSID_GIDS_TEST|(x))

/* Status Items */

// To test csid output format, these items should be created before testing
#define CSID_S_TEST_U32		_STA_TEST(0x0010)	//T=u32, u32 type csid (should exist)
#define CSID_S_TEST_U16		_STA_TEST(0x0020)	//T=u16, u16 type csid (should exist)
#define CSID_S_TEST_U8		_STA_TEST(0x0030)	//T=u8, u8 type csid (should exist)
#define CSID_S_TEST_S32		_STA_TEST(0x0040)	//T=s32, s32 type csid (should exist)
#define CSID_S_TEST_S16		_STA_TEST(0x0050)	//T=s16, s16 type csid (should exist)
#define CSID_S_TEST_S8		_STA_TEST(0x0060)	//T=s8, s8 type csid (should exist)
#define CSID_S_TEST_IPV4	_STA_TEST(0x0070)	//T=ipv4, ipv4 type csid (should exist)
#define CSID_S_TEST_MAC		_STA_TEST(0x0080)	//T=mac, mac type csid (should exist)
#define CSID_S_TEST_BIN		_STA_TEST(0x0090)	//T=bin, bin type csid (should exist)
#define CSID_S_TEST_STR		_STA_TEST(0x00A0)	//T=str, str type csid (should exist)

// To test default value (in csidbase), these items should not exist
#define CSID_S_TEST_U32_D	_STA_TEST(0x0110)	//T=u32, D=32, u32 with default (should not exist)
#define CSID_S_TEST_U16_D	_STA_TEST(0x0120)	//T=u16, D=16, u16 with default (should not exist)
#define CSID_S_TEST_U8_D	_STA_TEST(0x0130)	//T=u8, D=8, u8 with default (should not exist)
#define CSID_S_TEST_S32_D	_STA_TEST(0x0140)	//T=s32, D=-32, s32 with default (should not exist)
#define CSID_S_TEST_S16_D	_STA_TEST(0x0150)	//T=s16, D=-16, s16 with default (should not exist)
#define CSID_S_TEST_S8_D	_STA_TEST(0x0160)	//T=s8, D=-8, s8 with default (should not exist)
#define CSID_S_TEST_IPV4_D	_STA_TEST(0x0170)	//T=ipv4, D={192.168.10.24}, ipv4 with default (should not exist)
#define CSID_S_TEST_MAC_D	_STA_TEST(0x0180)	//T=mac, D={00-50-18-03-06-14}, mac with default (should not exist)
#define CSID_S_TEST_BIN_D	_STA_TEST(0x0190)	//T=bin, D={05-04-11-12-10-24-05-30}, bin with default (should not exist)
#define CSID_S_TEST_STR_D	_STA_TEST(0x01A0)	//T=str, D="Default String", str with default (should not exist)

// To test default value (in macro), these items should not exist
#define CSID_S_TEST_U32_X	_STA_TEST(0x0210)	//T=u32, u32 type csid (should not exist)
#define CSID_S_TEST_U16_X	_STA_TEST(0x0220)	//T=u16, u16 type csid (should not exist)
#define CSID_S_TEST_U8_X	_STA_TEST(0x0230)	//T=u8, u8 type csid (should not exist)
#define CSID_S_TEST_S32_X	_STA_TEST(0x0240)	//T=s32, s32 type csid (should not exist)
#define CSID_S_TEST_S16_X	_STA_TEST(0x0250)	//T=s16, s16 type csid (should not exist)
#define CSID_S_TEST_S8_X	_STA_TEST(0x0260)	//T=s8, s8 type csid (should not exist)
#define CSID_S_TEST_IPV4_X	_STA_TEST(0x0270)	//T=ipv4, ipv4 type csid (should not exist)
#define CSID_S_TEST_MAC_X	_STA_TEST(0x0280)	//T=mac, mac type csid (should not exist)
#define CSID_S_TEST_BIN_X	_STA_TEST(0x0290)	//T=bin, bin type csid (should not exist)
#define CSID_S_TEST_STR_X	_STA_TEST(0x02A0)	//T=str, str with default (should not exist)

#endif //ifndef _CSID_TEST_H
