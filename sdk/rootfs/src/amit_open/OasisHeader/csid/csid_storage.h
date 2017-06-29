/*
 * @file Member ID Definition of STORAGE
 *
 * Moderator: Abby Yang
 * Group ID: 0x001f0000/0x801f0000
 */

#ifndef _CSID_STORAGE_H
#define _CSID_STORAGE_H

#include "csid_gid.h"

#define _CFG_STORAGE(x)		(CSID_GIDC_STORAGE|(x))
#define _STA_STORAGE(x)		(CSID_GIDS_STORAGE|(x))

/* Extra Definitions */
#define FTP_CODEPAGE_ARABIC864	            0x00
#define FTP_CODEPAGE_GREEK737	            0x01
#define FTP_CODEPAGE_KOREAN949	            0x02
#define FTP_CODEPAGE_LATIN850	            0x03
#define FTP_CODEPAGE_LATIN852	            0x04
#define FTP_CODEPAGE_RUSSIAN866	            0x05
#define FTP_CODEPAGE_SIMPLIFIED_CHINESE936	0x06
#define FTP_CODEPAGE_THAI					0x07
#define FTP_CODEPAGE_TRADITIONAL_CHINESE950	0x08
#define FTP_CODEPAGE_UNITED_STATE437		0x09
#define FTP_CODEPAGE_JAPANESE	            0x0a
#define FTP_CODEPAGE_HEWBREW862	            0x0b

#define MAX_USER_NUM	20

/* Configuration Items */
#define CSID_C_STORAGE_DEVNUM		        _CFG_STORAGE(0x0001) //T=u8, D=16, Device number
#define CSID_C_STORAGE_IPCAMNUM		        _CFG_STORAGE(0x0002) //T=u8, D=16, IPCam number in each NVR

// Does support Kcodes ?
#define CSID_C_KCODES_SETUP		            _CFG_STORAGE(0x0003) //T=u8, D=0, Kcodes support ?

//USB Port for Storage
#define CSID_C_STORAGE_USB_PORT             _CFG_STORAGE(0x0004) //T=u32, D=0, USB Port Number for Storage

//Storage Setting
#define CSID_C_STORAGE_DEVICENAME	        _CFG_STORAGE(0x0010) //T=str, D="", DeviceName
#define CSID_C_STORAGE_IP			        _CFG_STORAGE(0x0020) //T=str, D="", IP
#define CSID_C_STORAGE_MAC			        _CFG_STORAGE(0x0030) //T=str, D="", MAC
#define CSID_C_STORAGE_ID                   _CFG_STORAGE(0x0040) //T=str, D="", ID
#define CSID_C_STORAGE_USR			        _CFG_STORAGE(0x0050) //T=str, D="", Username
#define CSID_C_STORAGE_PWD			        _CFG_STORAGE(0x0060) //T=str, D="", Password
#define CSID_C_STORAGE_ACTIVE		        _CFG_STORAGE(0x0070) //T=str, D="", Active
#define CSID_C_STORAGE_CONFIRM		        _CFG_STORAGE(0x0080) //T=u8, D=0, Storage confirm
#define CSID_C_STORAGE_AVAILABLE	        _CFG_STORAGE(0x0090) //T=u8, D=1, Storage available
#define	CSID_C_STORAGE_FREENUM		        _CFG_STORAGE(0x00a0) //T=u8, D=4, Free IPCam number
#define CSID_C_STORAGE_FC			        _CFG_STORAGE(0x00c0) //T=u8, D=12, Storage File collector period
#define CSID_C_STORAGE_WIRELESS             _CFG_STORAGE(0x00d0) //T=u8, D=0, Storage wireless enable
#define CSID_C_STORAGE_HAVEWIRELESS         _CFG_STORAGE(0x00e0) //T=u8, D=0, Storage wireless enable
#define CSID_C_STORAGE_MODELNUM	            _CFG_STORAGE(0x00f0) //T=str, D="", Model number

//Saved IPCam info of each Storage
#define CSID_C_STORAGE_N_SAVEDIPCAM_NAME	_CFG_STORAGE(0x0100) //T=str, D="", DeviceName
#define CSID_C_STORAGE_1_SAVEDIPCAM_NAME	_CFG_STORAGE(0x0110) //T=str, D="", DeviceName
#define CSID_C_STORAGE_2_SAVEDIPCAM_NAME	_CFG_STORAGE(0x0120) //T=str, D="", DeviceName
#define CSID_C_STORAGE_3_SAVEDIPCAM_NAME	_CFG_STORAGE(0x0130) //T=str, D="", DeviceName

//Extra Setting for WebHDD
#define CSID_C_WEBHDD_ENABLE_SHARE	        _CFG_STORAGE(0x0140) //T=u8,  D=0, Enable/Disable Share Folder
#define CSID_C_WEBHDD_CNT_PORT	            _CFG_STORAGE(0x0141) //T=u32, D=8080, Port allow User connect from WAN
#define CSID_C_WEBHDD_SHARE_DIR	            _CFG_STORAGE(0x0142) //T=str, D="", All Files in SD Card/Specific Folder
#define CSID_C_WEBHDD_ACCESS_TYPE	        _CFG_STORAGE(0x0143) //T=u8,  D=0, Access type of shared folder(0: Read Only, 1: Read/Write)
#define CSID_C_WEBHDD_ALLOW_ACCESS	        _CFG_STORAGE(0x0144) //T=u8,  D=0, Allow Access form Internet

//Samba Settings
#define CSID_C_STORAGE_SAMBA_HOSTNAME		_CFG_STORAGE(0x0200) //T=str, D="", Host Name
#define CSID_C_STORAGE_SAMBA_WORKGROUP		_CFG_STORAGE(0x0201) //T=str, D="", WorkGroup
#define CSID_C_STORAGE_SAMBA_SERVERCOMMENT	_CFG_STORAGE(0x0202) //T=str, D="", Server Comment
#define CSID_C_STORAGE_SAMBA_ENABLE			_CFG_STORAGE(0x0203) //T=u8, D=0, 0:Disable , 1:Enable

//FTPd Settings
#define CSID_C_STORAGE_FTP_ENABLE				_CFG_STORAGE(0x0210) //T=u32, D=1,disable=0,enable=1
#define CSID_C_STORAGE_FTP_PORT					_CFG_STORAGE(0x0211) //T=u32, D=21, ftp port.
#define CSID_C_STORAGE_FTP_MAX_CONNECTION		_CFG_STORAGE(0x0212) //T=u32, D=2,max connection per peer.
#define CSID_C_STORAGE_FTP_MAX_CLIENTS			_CFG_STORAGE(0x0213) //T=u32, D=5,max clients.
#define CSID_C_STORAGE_FTP_CLIENT_SUPPORT_UTF8	_CFG_STORAGE(0x0214) //T=u32, D=0, no=0,yes=1
#define CSID_C_STORAGE_FTP_CODEPAGE				_CFG_STORAGE(0x0215) //T=u32, D=0, codepage.
#define CSID_C_STORAGE_FTP_DENY_WAN				_CFG_STORAGE(0x0216) //T=u8,  D=0, Deny from WAN access. No:0, Yes: 1 
#define CSID_C_STORAGE_FTP_ALLOW_WAN			_CFG_STORAGE(0x0217) //T=u8,  D=0, Allow from WAN access. No:0, Yes: 1 

//Download state
#define CSID_C_STORAGE_DL_ENABLE            _CFG_STORAGE(0x0230) //T=u8, D=0, download job enable
#define CSID_C_STORAGE_DL_RUNNING_COUNT		_CFG_STORAGE(0x0231) //T=u8, D=0, running job counts
#define CSID_C_STORAGE_DL_WAITING_COUNT		_CFG_STORAGE(0x0232) //T=u8, D=0, waiting job counts
#define CSID_C_STORAGE_DL_SCHED_COUNT		_CFG_STORAGE(0x0233) //T=u8, D=0, schedule job counts
#define CSID_C_STORAGE_DL_LOG_COUNT			_CFG_STORAGE(0x0234) //T=u8, D=0, schedule job counts
#define CSID_C_STORAGE_DL_RUNNING_INFO		_CFG_STORAGE(0x0240) //T=str, D="", download parameter
#define CSID_C_STORAGE_DL_WAITING_INFO		_CFG_STORAGE(0x0250) //T=str, D="", download parameter
#define CSID_C_STORAGE_DL_SCHED_INFO		_CFG_STORAGE(0x0260) //T=str, D="", download parameter
#define CSID_C_STORAGE_DL_LOG				_CFG_STORAGE(0x0270) //T=str, D="", download parameter

//UPnP Settings
#define CSID_C_STORAGE_UPNPDB_REFRESH_TIME  _CFG_STORAGE(0x0280) //T=u8, D=0, UpnpDatabase regular refresh time
#define CSID_C_STORAGE_UPNPDB_ENABLE		_CFG_STORAGE(0x0281) //T=u8, D=0, 0:Disable , 1:Enable

//USB green state
//#define CSID_C_STORAGE_USB_GREEN_STATE    _CFG_STORAGE(0x0290) //T=u8, D=0, USB green state 0 off
#define CSID_C_STORAGE_USB_GREEN_IDLE_TIME  _CFG_STORAGE(0x0290) //T=u8, D=0, USB idle time unit mins 0 disable

//User configuration
#define CSID_C_STORAGE_AUTH_MODE            _CFG_STORAGE(0x0300) //T=u32, D=0, 0:guest mode 1:authentication mode
#define CSID_C_STORAGE_USERNAME		        _CFG_STORAGE(0x0310) //T=str, D="", 32 username
#define CSID_C_STORAGE_PASSWORD		        _CFG_STORAGE(0x0330) //T=str, D="", 32 password

//USB Space Settings
#define CSID_C_STORAGE_USB_SPACE            _CFG_STORAGE(0x0350) //T=u32, D=0, user default sufficient space(MB)

//DAAP Server Setting
#define CSID_C_STORAGE_DAAP_ENABLE          _CFG_STORAGE(0x0360) //T=u8, D=0, Disable/Enable daap
#define CSID_C_STORAGE_DAAP_SRVNAME		    _CFG_STORAGE(0x0361) //T=str, D="", Hostname
#define CSID_C_STORAGE_DAAP_PORT            _CFG_STORAGE(0x0362) //T=u16, D=3689, Daap service port
#define CSID_C_STORAGE_DAAP_PASSWORD		_CFG_STORAGE(0x0363) //T=str, D="", Service Password
#define CSID_C_STORAGE_DAAP_MP3_FOLDER		_CFG_STORAGE(0x0364) //T=str, D="/var/usb/C", MP3 share folder
#define CSID_C_STORAGE_WEB_GUEST_PASSWORD	_CFG_STORAGE(0x0365) //T=str, D="guest", Guest password

/* DLNA users table, reserved 32 items */
#define CSID_C_STORAGE_DLNA_USERNAME		    _CFG_STORAGE(0x0370) //T=str, D="", Username
#define CSID_C_STORAGE_DLNA_PASSWORD		_CFG_STORAGE(0x0390) //T=str, D="", Password
#define CSID_C_STORAGE_DLNA_LEVEL          _CFG_STORAGE(0x03b0) //T=u8, D=0, user level


#define CSID_C_STORAGE_MODE         		_CFG_STORAGE(0x0401) //T=u8, 1:Reader (ex: card reader) 0:Nas server

/* For MiiiCaSa NAS */
#define CSID_C_STORAGE_NAS_SHARE_IP          _CFG_STORAGE(0x0420) //T=ipv4, NAS Share IP for MiiiCaSa
#define CSID_C_STORAGE_NAS_SHARE_NAME        _CFG_STORAGE(0x0430) //T=str, NAS Share NAME for MiiiCaSa

//User configuration
#define CSID_C_STORAGE_USER_ENABLE          _CFG_STORAGE(0x0450) //T=u8, D=0, 32 user enable

/* Status Items */

//HomeCom 1.0+ Register information
#define CSID_S_STORAGE_AGPINFO              _STA_STORAGE(0x0001) //T=u8,  D=1

#define CSID_S_STORAGE_REGNAME		        _STA_STORAGE(0x0010) //T=str, D="", New NVR Name
#define CSID_S_STORAGE_REGIP		        _STA_STORAGE(0x0020) //T=str, D="", New NVR IP
#define CSID_S_STORAGE_REGMAC		        _STA_STORAGE(0x0030) //T=str, D="", New NVR MAC
#define CSID_S_STORAGE_REGCONFIRM	        _STA_STORAGE(0x0040) //T=u8,  D=0
#define CSID_S_STORAGE_REGAVAILABLE	        _STA_STORAGE(0x0050) //T=u8,  D=1
#define CSID_S_STORAGE_REGMODELNUM          _STA_STORAGE(0x0060) //T=str, D="", Device model number
#define CSID_S_STORAGE_REGINDEX		        _STA_STORAGE(0x00a0) //T=u8,  D=-1, Status to Conf. area index
#define CSID_S_STORAGE_REGFAIL              _STA_STORAGE(0x00f0) //T=u8,  D=1

#define CSID_S_STORAGE_CAPACITY	            _STA_STORAGE(0x0110) //T=str, D="", NVR Capacity

//HomeCom 1.0+ Setting information
#define CSID_S_STORAGE_WIRELESSFAIL	        _STA_STORAGE(0x0210) //T=u8, D=0, Wireless setting fail.
#define CSID_S_STORAGE_WIRELESSCHK	        _STA_STORAGE(0x0220) //T=u8, D=0, Check wireless setting result.
#define CSID_S_STORAGE_WIRELESSCHANGE	    _STA_STORAGE(0x0230) //T=u8, D=0, Wireless setting change.

//RT305CDS USB and BT status
//#define CSID_S_STORAGE_BT			        _STA_STORAGE(0x0240) //T=u8, D=0, BT avail?.
#define CSID_S_STORAGE_USB			        _STA_STORAGE(0x0240) //T=u8, D=0, USB avail? 1 avail,0 not avail
#define CSID_S_STORAGE_USB_FS		        _STA_STORAGE(0x0241) //T=str, D="", USB disk file system type(N/A fat32 ntfs ext).
#define CSID_S_STORAGE_USB_MOUNT            _STA_STORAGE(0x0242) ////T=u8, D=0, USB MOUNTED or not
#define CSID_S_STORAGE_REMOVE_USB           _STA_STORAGE(0x0243) ////T=u8, D=0, user remove disk normally or not
#define CSID_S_STORAGE_PARTITION	        _STA_STORAGE(0x0244) //T=u8, D=0, USB partition sda1 or 5 if avail;
#define CSID_S_STORAGE_3G			        _STA_STORAGE(0x0245) //T=u8, D=0, USB partition sda1 or 5 if avail;
#define CSID_S_STORAGE_USB_PRE			        _STA_STORAGE(0x0246) //T=u8, D=0, previous USB avail? 1 avail,0 not avail

//Download Assistant Status
#define CSID_S_STORAGE_DL_TYPE		        _STA_STORAGE(0x0250) //T=u8, D=0, download type ,HTTP FTP
#define CSID_S_STORAGE_DL_INFO		        _STA_STORAGE(0x0251) //T=str, D="", download parameter dst url
#define CSID_S_STORAGE_DL_ALTERED	        _STA_STORAGE(0x0252) //T=u8, D=0, download page UI changed ,
#define CSID_S_STORAGE_DL_ACTION_ALTERED	_STA_STORAGE(0x0253) //T=u8, D=0, download status UI changed ,
#define CSID_S_STORAGE_DL_ACTION_INFO	    _STA_STORAGE(0x0254) //T=str, D="", download page UI changed ,

//Samba Status
#define CSID_S_STORAGE_SAMBA_ALTERED	    _STA_STORAGE(0x0300) //T=u32, D=0, Samba Setting change alter.

//FTPd Status
#define CSID_S_STORAGE_FTP_ALTERED	        _STA_STORAGE(0x0310) //T=u32, D=0, FTP Setting change alter.

//User Configuration Status
#define CSID_S_STORAGE_USER_ALTERED	        _STA_STORAGE(0x0311) //T=u32, D=0, User Configuration change alter.

//USB disk operation Status
#define CSID_S_STORAGE_DISK_CAPACITY		_STA_STORAGE(0x0320) //T=str, D="",capacity
#define CSID_S_STORAGE_DISK_ACT		        _STA_STORAGE(0x0321) //T=u8, D=0, Act
#define CSID_S_STORAGE_DISK_PROGRESS		_STA_STORAGE(0x0322) //T=u8, D=0, Progress
#define CSID_S_STORAGE_DISK_FREE		    _STA_STORAGE(0x0323) //T=str, D="", Progress
#define CSID_S_STORAGE_DISK_USED	        _STA_STORAGE(0x0324) //T=str, D="", Progress
#define CSID_S_STORAGE_DISK_TOTAL		    _STA_STORAGE(0x0325) //T=str, D="", Progress
#define CSID_S_STORAGE_DISK_IS_PROGRESS		_STA_STORAGE(0x0326) //T=u8, D=0, Act
#define CSID_S_STORAGE_DISK_ALTERED		    _STA_STORAGE(0x0327) //T=u8, D=0, ui flag
#define CSID_S_STORAGE_USB_GREEN_ALTERED    _STA_STORAGE(0x0328) //T=u8, D=0, USB green function altered
#define CSID_S_STORAGE_DEVNAME		        _STA_STORAGE(0x0329) //T=str, D="",device name ie sda sdb sdc

//daap status
#define CSID_S_STORAGE_DAAP_ALTERED	        _STA_STORAGE(0x0330) //T=u8, D=0, Itune Setting change alter.

//hotplug status
#define CSID_S_STORAGE_HOTPLUG_ALTERED	    _STA_STORAGE(0x0331) //T=u8, D=0, Itune Setting change alter.
#define CSID_S_STORAGE_WEB_ALTERED	        _STA_STORAGE(0x0332) //T=u8, D=0, add folder or delete button status changed
#define CSID_S_STORAGE_WEB_ACT	            _STA_STORAGE(0x0333) //T=u8, D=0, add folder=0 delete=1
#define CSID_S_STORAGE_WEB_CURDIR	        _STA_STORAGE(0x0334) //T=str, D="", current directory
#define CSID_S_STORAGE_WEB_FILENAME	        _STA_STORAGE(0x0335) //T=str, D="", choiced filename
#define CSID_S_STORAGE_WEB_USER	            _STA_STORAGE(0x0336) //T=str, D="", username
#define CSID_S_STORAGE_WEB_FILE_COUNT	    _STA_STORAGE(0x0337) //T=u32, D=0, Web file number
#define CSID_S_STORAGE_DEVICE_TYPE	        _STA_STORAGE(0x0338) //T=str, D="", device type disk/printer
#define CSID_S_STORAGE_SIZE_NEEDCHECK	    _STA_STORAGE(0x0339) //T=u8, D=0, 1 need check
#define CSID_S_STORAGE_NAS_ALTERED	    _STA_STORAGE(0x0340) //T=u8, D=0, Itune Setting change alter.

//USB Device Status
#define CSID_S_STORAGE_DEVICE_STATUS		_STA_STORAGE(0x0370) //T=u32, D=0, 0:Not Ready / 1:Ready
#define CSID_S_STORAGE_UNMOUNT_ACT		_STA_STORAGE(0x0371) //T=u32, D=0, 0:NA  1:Success  2:Fail
#define CSID_S_STORAGE_UNMOUNT_STATUS		_STA_STORAGE(0x0372) //T=u32, D=0, 0:NA  1:Success  2:Fail

//UPnP AV status
#define CSID_S_STORAGE_UPNP_AV_ALTERED		_STA_STORAGE(0x0373) //T=u8, D=0, Itune Setting change alter.

/* Total 256 account */
// USB mount storage info
#define CSID_S_STORAGE_USB_PARTNAME		_STA_STORAGE(0x0400) //T=str, D="", USB Patition Name.
#define CSID_S_STORAGE_USB_PARTSIZE		_STA_STORAGE(0x0500) //T=str, D="", USB Patition Block Size.
#define CSID_S_STORAGE_USB_FILETYPE		_STA_STORAGE(0x0600) //T=str, D="", USB disk Partition file system type(N/A fat32 ntfs ext).
#define CSID_S_STORAGE_USB_FREESIZE		_STA_STORAGE(0x0700) //T=str, D="", USB Patition Available Size.
#define CSID_S_STORAGE_USB_USEDSIZE		_STA_STORAGE(0x0800) //T=str, D="", USB Patition Used Size.

#define CSID_S_STORAGE_USB_FORMAT_INDEX		_STA_STORAGE(0x0900) //T=u8, D=0, USB Partition name , sda1 or sda2 or sdb1 ...

/* Total 256 account */
#define CSID_S_STORAGE_USB_DISK_NAME	_STA_STORAGE(0x0a00) //T=str, D="", USB Partition disk name ,C D E...


//SD card
#define CSID_S_STORAGE_HOTPLUG_ALTERED_SD	    _STA_STORAGE(0x0b00) //T=u8, D=0, Itune Setting change alter.
#define CSID_S_STORAGE_SD			       _STA_STORAGE(0x0b01) //T=u8, D=0, SD avail? 1 avail,0 not avail
// SD mount storage info
#define CSID_S_STORAGE_SD_PARTNAME		_STA_STORAGE(0x0b02) //T=str, D="", SD Patition Name.
#define CSID_S_STORAGE_SD_PARTSIZE		_STA_STORAGE(0x0b03) //T=str, D="", SD Patition Block Size.
#define CSID_S_STORAGE_SD_FILETYPE		_STA_STORAGE(0x0b04) //T=str, D="", SD disk Partition file system type(N/A fat32 ntfs ext).
#define CSID_S_STORAGE_SD_FREESIZE		_STA_STORAGE(0x0b05) //T=str, D="", SD Patition Available Size.
#define CSID_S_STORAGE_SD_USEDSIZE		_STA_STORAGE(0x0b06) //T=str, D="", SD Patition Used Size.
#define CSID_S_STORAGE_SD_DISK_NAME	    _STA_STORAGE(0x0b07) //T=str, D="", SD Partition disk name ,C D E...
#define CSID_S_STORAGE_SD_LABEL_NAME	_STA_STORAGE(0x0b08) //T=str, D="", SD label name 

///* Total 256 account */
#define CSID_S_STORAGE_USB_LABEL_NAME	_STA_STORAGE(0x0c00) //T=str, D="", USB label name 
#define CSID_S_STORAGE_USB_PORT	        _STA_STORAGE(0x0d00) //T=u32, D=0, USB Port Number for storage 

#define CSID_S_STORAGE_USBPLUG_ALTERED  _STA_STORAGE(0x1001) //T=u8, 1:changed, client PC/power bank plug in/out


#endif	//ifndef _CSID_STORAGE_H
