/*
 * @file Member ID Definition of IPCAM
 *
 * Moderator: Abby Yang
 * Group ID: 0x00330000/0x80330000
 */

#ifndef _CSID_IPCAM_H
#define _CSID_IPCAM_H

#include "csid_gid.h"

#define _CFG_IPCAM(x)		(CSID_GIDC_IPCAM|(x))
#define _STA_IPCAM(x)		(CSID_GIDS_IPCAM|(x))

/* Extra Definitions */

/* Configuration Items */

#define CSID_C_IPCAM_DEVNUM				_CFG_IPCAM(0x0001) //T=u8, D=16, Device number
#define CSID_C_IPCAM_SENDMAIL_ENABLE	_CFG_IPCAM(0x0002) //T=u32, D=0, ipcam send mail enable

//IPCam Setting
#define CSID_C_IPCAM_DEVICENAME		_CFG_IPCAM(0x0010) //T=str, D="", DeviceName
#define CSID_C_IPCAM_IP				_CFG_IPCAM(0x0020) //T=str, D="", IP
#define CSID_C_IPCAM_MAC			_CFG_IPCAM(0x0030) //T=str, D="", MAC
#define CSID_C_IPCAM_STREAMPORT     _CFG_IPCAM(0x0040) //T=u32, D=0, Streaming Port
#define CSID_C_IPCAM_USR			_CFG_IPCAM(0x0050) //T=str, D="", Username
#define CSID_C_IPCAM_PWD			_CFG_IPCAM(0x0060) //T=str, D="", Password
#define CSID_C_IPCAM_VIDEOQUALITY	_CFG_IPCAM(0x0070) //T=str, D="", VideoQuality
#define CSID_C_IPCAM_CONFIRM		_CFG_IPCAM(0x0080) //T=u8, D=0, IPCam confirm
#define CSID_C_IPCAM_AVAILABLE		_CFG_IPCAM(0x0090) //T=u8, D=1, IPCam available
#define CSID_C_IPCAM_NVRINDEX		_CFG_IPCAM(0x00a0) //T=u8, D=0, File save to ,1~3
#define CSID_C_IPCAM_ID				_CFG_IPCAM(0x00b0) //T=u8, D=0, IPCam ID of corresponding NVR.
#define CSID_C_IPCAM_WIRELESS       _CFG_IPCAM(0x00d0) //T=u8, D=0, IPCam wireless setting
#define CSID_C_IPCAM_HAVEWIRELESS   _CFG_IPCAM(0x00e0) //T=u8, D=0, IPCam wireless setting
#define CSID_C_IPCAM_MODELNUM       _CFG_IPCAM(0x00f0) //T=str, D="", Model number
#define CSID_C_IPCAM_HAVECONFIRM	_CFG_IPCAM(0x0100) //T=u8, D=0, IPCam have set port before.
#define CSID_C_IPCAM_ENABLEMD		_CFG_IPCAM(0x0110) //T=u8, D=0, Enable Motion Detect Record.
#define CSID_C_IPCAM_HAVEENABLEMD	_CFG_IPCAM(0x0120) //T=u8, D=0, Have Enable MD Record.
#define CSID_C_IPCAM_HTTP_PORT      _CFG_IPCAM(0x0130) //T=u32, D=8185, Port 8185 for mydlink wc http
#define CSID_C_IPCAM_HTTPS_PORT     _CFG_IPCAM(0x0131) //T=u32, D=8186, Port 8186 for mydlink wc http
//for AirLive IPCam ,Up to 8 sets 
#define CSID_C_IPCAM_AIRLIVE_IP				_CFG_IPCAM(0x0130) //T=ipv4, D="", IP
#define CSID_C_IPCAM_AIRLIVE_MAC			_CFG_IPCAM(0x0140) //T=mac, D="", MAC
#define CSID_C_IPCAM_AIRLIVE_ENABLE			_CFG_IPCAM(0x0150) //T=u8, D=0, ENABLE
#define CSID_C_IPCAM_AIRLIVE_DESCRIPTION	_CFG_IPCAM(0x0160) //T=str, D="", Room outdoor
#define CSID_C_IPCAM_AIRLIVE_HOSTNAME		_CFG_IPCAM(0x0170) //T=str, D="", Hostname
#define CSID_C_IPCAM_AIRLIVE_PORT			_CFG_IPCAM(0x0180) //T=str, D="", Port

/* Status Items */

//Altered
#define CSID_S_IPCAM_ALTERED		_STA_IPCAM(0x0001) //T=u32, IPCam Altered

//HomeCom 1.0+ Register information
#define CSID_S_IPCAM_REGNAME		_STA_IPCAM(0x0010) //T=str, D="", New IPCam Name
#define CSID_S_IPCAM_REGIP			_STA_IPCAM(0x0020) //T=str, D="", New IPCam IP
#define CSID_S_IPCAM_REGMAC			_STA_IPCAM(0x0030) //T=str, D="", New IPCam MAC
#define CSID_S_IPCAM_REGSTREAMPORT	_STA_IPCAM(0x0040) //T=u32, D=0, Streaming Port
#define CSID_S_IPCAM_REGUSR         _STA_IPCAM(0x0050) //T=str, D="", Username
#define CSID_S_IPCAM_REGPWD         _STA_IPCAM(0x0060) //T=str, D="", Password
#define CSID_S_IPCAM_REGCONFIRM		_STA_IPCAM(0x0070) //T=u8,  D=0
#define CSID_S_IPCAM_REGAVAILABLE	_STA_IPCAM(0x0080) //T=u8,  D=1
#define CSID_S_IPCAM_REGMODELNUM    _STA_IPCAM(0x0090) //T=str, D="", Device model number
#define CSID_S_IPCAM_REGINDEX		_STA_IPCAM(0x00a0) //T=u8,  D=-1, Status to Conf. area index
#define CSID_S_IPCAM_REGFAIL		_STA_IPCAM(0x00f0) //T=u8,  D=1, Register setting result

//HomeCom 1.0+ IPCam Status
#define CSID_S_IPCAM_BUSY			_STA_IPCAM(0x0100) //T=u8, D=0, 1:in use 0:free.
#define CSID_S_IPCAM_NVRCAPACITY	_STA_IPCAM(0x0110) //T=str, D="", NVR capacity of this IPCam.
#define CSID_S_IPCAM_SETFTPFAIL		_STA_IPCAM(0x0120) //T=u8,  D=1, NVR setting result

//HomeCom 1.0+ Setting information
#define CSID_S_IPCAM_WIRELESSFAIL	_STA_IPCAM(0x0210) //T=u8, D=0, Wireless setting fail.
#define CSID_S_IPCAM_WIRELESSCHK	_STA_IPCAM(0x0220) //T=u8, D=0, Check wireless setting result.
#define CSID_S_IPCAM_WIRELESSCHANGE	_STA_IPCAM(0x0230) //T=u8, D=0, Wireless setting changed.
#define CSID_S_IPCAM_VIDEO_CODEC	_STA_IPCAM(0x0240) //T=str, D="H264", Video codec
#define CSID_S_IPCAM_VIDEO_WIDTH	_STA_IPCAM(0x0250) //T=u32, D=320, Video Width
#define CSID_S_IPCAM_VIDEO_HEIGHT	_STA_IPCAM(0x0260) //T=u32, D=192, Video Height
#define CSID_S_IPCAM_VIDEO_FRAMERATE	_STA_IPCAM(0x0270) //T=u8, D=10, Video frame rate
#define CSID_S_IPCAM_AUDIO_CODEC	_STA_IPCAM(0x0280) //T=str, D="auto", Audio codec
#define CSID_S_IPCAM_AUDIO_SUBCODEC	_STA_IPCAM(0x0290) //T=str, D="PCMU", Audio Subcodec

//Airlive IPCam status
#define CSID_S_IPCAM_AIRLIVE_STATUS	_STA_IPCAM(0x0300) //T=u8, D=0, connecting status

#endif	//ifndef _CSID_IPCAM_H
