#define FW_FAIL			0x00
#define FW_SUCCESS		0x01

//AMIT SDK FW
#define FW_AMIT			0x02

//3rd Party FW with AMIT header
#define FW_3rdPARTY		0x10
#define FW_ECONETADSL	0x11

//Module FW
#define FW_TELIT		0x31



//This function checks the binary file to see if it has AMIT header.
//If AMIT header existed, this function returns the FW type, ie: FW_ECONETADSL...etc.
//If there is no AMIT header, the function returns FW_FAIL
int is_3rdParty_FW(char *filepath);

//This function removes AMIT header from the file "filepath", and writes it back based on header info.
//If any error, returns FW_FAIL, otherwise returns FW_SUCCESS.
int remove_AMIT_hdr(char *filepath);

//This function checks the binary file to see if it is TELIT FW.
//If it is, return FW_TELIT. (Maybe return module types, but it will be a future work.)
//If it is not, return FW_FAIL.
int is_TELIT(char *filepath);
