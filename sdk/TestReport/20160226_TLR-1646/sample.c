#include <stdio.h>
#include <stdlib.h>
#include "flashbuf.h"
#include "fw_util.h"

int check_FWs(char *filepath)
{
	int ret=0;
	//check FW type
	if(check_firmware(filepath,1))
	{
		return FW_AMIT;
	}
	else if(ret = is_3rdParty_FW(filepath))
	{
		return ret;
	}
	else if(is_TELIT(filepath))
	{
		return FW_TELIT;
	}

	return FW_FAIL;
}


int main(int argc, char *argv[])
{
	int ret;

	ret = check_FWs(argv[1]);

	switch(ret)
	{
		case FW_AMIT:
			//AMIT SDK FW
			//Do what you want...
			break;

		case FW_3rdPARTY:
			//templete for other module FW with AMIT header
			//remove_AMIT_hdr(argv[1]);
			//sprintf(strbuf, "XXX_FW_upg.sh %s\n", NEWPATH);
			//system(strbuf);
			break;

		case FW_ECONETADSL:
			//ECONET ADSL FW with AMIT header
			remove_AMIT_hdr(argv[1]);
			//sprintf(strbuf, "XXX_FW_upg.sh %s\n", NEWPATH);
			//system(strbuf);
			break;

		case FW_TELIT:
			//ECONET ADSL FW
			printf("This is Telit FW\n");
			//sprintf(strbuf, "ADSL_FW_upg.sh %s\n", NEWPATH);
			//system(strbuf);
			break;

		default:
			printf("Unknown file format.\n");
			break;
	}
}
