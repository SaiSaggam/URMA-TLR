

#ifndef _AMITODM
#define _AMITODM

#define MAX_LEN		1024
#define MAX_NUM_OF_MAC	12	//4 Ethernet, 4 WiFi 2.4G, 4 WiFi 5G

// function prototype
//
// All get_odm_xxx functions use same method to get values from a table file.
// The function search the CSID that you want to get, and then compare the length that you input with the CSID.
// If the input length is smaller than CSID recorded, it returns fail and prints error mesg.
// The functions are thread-safe, since we did not use any global value or static value.
//

// get_odm_ECN(), get the Engineering Change Number in ODM area, assigned by AMIT ERP system.
// num: string buffer that you want to store the data.
// len: length of num.
// return: length of data. zero or less than sero means error.
// ECN format: "200909100001"
int get_odm_ECN(char *num, int len);				// CSID_S_SYS_INFO_ECN_NO		(Engineering Change Number)	"200909100001

// get_odm_PV(), get the PCBA Version in ODM area, assigned by AMIT ERP system.
// pv: variable address that you want to store the data, short type.
// len: length of pv.
// return: length of data. zero or less than sero means error.
// PV format: 0x01
int get_odm_PV(unsigned short *pv, int len);		// CSID_S_SYS_INFO_PV			(PCBA Version)				0x1 @1

// get_odm_ModelName(), get the Model Name in ODM area, assigned by Digi.
// name: string buffer that you want to store the data.
// len: length of name.
// return: length of data. zero or less than sero means error.
// ModelName format: "LR54D", "LR54DC1", "LR54", "LR54W"
int get_odm_ModelName(char *name, int len);			// CSID_S_SYS_INFO_MODEL_NAME	(Model name)				"LR54D", "LR54DC1"...etc.

// get_odm_EIDRule(), get the EID rule in ODM area, assigned by AMIT RD, used in AMIT generic FW and ATS.
// eid: variable address that you want to store the data, int type.
// len: length of eid.
// return: length of data. zero or less than sero means error.
// EIDRule format: 0x1E04
int get_odm_EIDRule(unsigned int *eid, int len);	// CSID_S_SYS_INFO_EID_RULE		(MAC algorithm)				0x1E04 @2

// get_odm_DefIP(), get the Default IP Address in ODM area, assigned by Digi.
// IP: char buffer to store IP address.
// len: length of IP.
// return: length of data. zero or less than sero means error.
// DefIP format: 0xC0A80101, means [192.168.1.1]
int get_odm_DefIP(unsigned char *IP, int len);		// CSID_S_SYS_INFO_DEFAULT_IP	(Default IP)				{192.168.123.254}

// get_odm_ROMID(), get the ROMID in ODM area, assigned by AMIT RD, used to indicate the platform for FW updating.
// romid: string buffer to store ROMID.
// len: length of romid.
// return: length of data. zero or less than sero means error.
// ROMID format: "DGI6E4006001"
int get_odm_ROMID(char *romid, int len);			// CSID_S_SYS_INFO_ROM_ID		(ROM ID)					"DGI6E4006001

// get_odm_FMID(), get the Firmware ID in ODM area, assigned by AMIT RD, used to indicate the FW family for FW updating.
// fmid: variable address that you want to store FMID.
// len: length of fmid.
// return: length of data. zero or less than sero means error.
// FMID format: 0x6E40
int get_odm_FMID(unsigned int *fmid, int len);		// CSID_S_SYS_INFO_FMID         (FMID)						0x6E40 @2

// get_odm_SN(), get the Serial Number in ODM area, assigned by Digi.
// sn: string buffer to store the Serial Number.
// len: length of sn.
// return: length of data. zero or less than sero means error.
// SN format: "LR123456"
int get_odm_SN(char *sn, int len);					// CSID_S_SYS_SERIAL_NUMBER		(Serial Number)				"LR123456"

// get_odm_HWver(), get the HardWare Version in ODM area, assigned by Digi.
// hw: string buffer to store the HardWare Version.
// len: length of hw.
// return: length of data. zero or less than sero means error.
// HWver format: "50001899-xx YY", xx is subnumber: 00, 01, 02...etc. YY is the Revision Number: 01,...,1P,2P,...,A,B,C,...
int get_odm_HWver(char *hw, int len);				// CSID_S_SYS_INFO_MODEL_HW_NAME	(HW Version)			"50001899-xx YY"

// get_odm_PN(), get the Product Number in ODM area, assigned by Digi.
// pn: string buffer to store the Product Number.
// len: length of pn.
// return: length of data. zero or less than sero means error.
// PN format: "LR54-xxxxx" or "LR54-xxxxx-yy", ex: "LR54-DA301", "LR54-DA411-F1"
int get_odm_PN(char *pn, int len);					// CSID_S_SYS_INFO_MODEL_NAME_EXTRA	(Part Number)			"LR54-xxxxx" or "LR54-xxxxx-yy"

// get_odm_MAC(), get the (first) MAC address in ODM area, assigned by Digi.
// mac: char array to store the MAC address.
// len: length of mac.
// return: length of data. zero or less than sero means error.
// MAC format: 0x005018000000, means [00:50:18:00:00:00]
int get_odm_MAC(unsigned char *mac, int len);		// CSID_S_SYS_INFO_BEID			(First MAC address)			@6 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF

// get_valid_MAC(), get valid MAC address from the one written in ODM area.
// number: which MAC address you want to get. The number starts from 0. If number is 0, you get the same result from get_odm_MAC().
// mac: char array to store the MAC address.
// return: length of data. zero means error.
// len: length of mac.
int get_valid_MAC(int number, unsigned char *mac, int len);	// Get a valid MAC address, based on CSID_S_SYS_INFO_BEID. number: start from 0, and less than MAX_NUM_OF_MAC, mac: the buffer, len: length of mac buffer

#endif /* _AMITODM_ */

