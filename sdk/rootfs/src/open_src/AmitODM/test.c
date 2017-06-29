//
// Example program to show how to use ODM library
//
//			-- Lily 20150723
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "odm.h"

int main()
{
	int ret;
	unsigned char buf[MAX_LEN]={0};
	unsigned short pv=0;
	unsigned int eid=0, fmid=0;

	memset(buf,0x0,sizeof(buf));
	ret = get_odm_ECN((char *)buf, sizeof(buf));
	if(ret<=0)
	{
		printf("ODMtest: ECN get failed.\n");
	}
	else
	{
		printf("get_odm_ECN: ret=%d, ECN=[%s]\n", ret, buf);
	}

	pv = 0;
	ret = get_odm_PV(&pv, sizeof(pv));
	if(ret<=0)
	{
		printf("ODMtest: PV get failed.\n");
	}
	else
	{
		printf("get_odm_PV: ret=%d, PV=[%d]\n", ret, pv);
	}

	memset(buf,0x0,sizeof(buf));
	ret = get_odm_ModelName((char *)buf, sizeof(buf));
	if(ret<=0)
	{
		printf("ODMtest: ModelName get failed.\n");
	}
	else
	{
		printf("get_odm_ModelName: ret=%d, ModelName=[%s]\n", ret, buf);
	}

	eid = 0;
	ret = get_odm_EIDRule(&eid, sizeof(eid));
	if(ret<=0)
	{
		printf("ODMtest: EIDRule get failed.\n");
	}
	else
	{
		printf("get_odm_EIDRule: ret=%d, EIDRule=[%x]\n", ret, eid);
	}

	memset(buf,0x0,sizeof(buf));
	ret = get_odm_DefIP(buf, sizeof(buf));
	if(ret<=0)
	{
		printf("ODMtest: DefIP get failed.\n");
	}
	else
	{
		printf("get_odm_DefIP: ret=%d, DefIP=[%d.%d.%d.%d]\n", ret, buf[0],buf[1],buf[2],buf[3]);
	}

	memset(buf,0x0,sizeof(buf));
	ret = get_odm_ROMID((char *)buf, sizeof(buf));
	if(ret<=0)
	{
		printf("ODMtest: ROMID get failed.\n");
	}
	else
	{
		printf("get_odm_ROMID: ret=%d, ROMID=[%s]\n", ret, buf);
	}

	fmid = 0;
	ret = get_odm_FMID(&fmid, sizeof(fmid));
	if(ret<=0)
	{
		printf("ODMtest: FMID get failed.\n");
	}
	else
	{
		printf("get_odm_FMID: ret=%d, FMID=[%x]\n", ret, fmid);
	}

	memset(buf,0x0,sizeof(buf));
	ret = get_odm_SN((char *)buf, sizeof(buf));
	if(ret<=0)
	{
		printf("ODMtest: Serial Number get failed.\n");
	}
	else
	{
		printf("get_odm_SN: ret=%d, Serial Number=[%s]\n", ret, buf);
	}

	memset(buf,0x0,sizeof(buf));
	ret = get_odm_HWver((char *)buf, sizeof(buf));
	if(ret<=0)
	{
		printf("ODMtest: HW version get failed.\n");
	}
	else
	{
		printf("get_odm_HWver: ret=%d, HardWare Version=[%s]\n", ret, buf);
	}

	memset(buf,0x0,sizeof(buf));
	ret = get_odm_PN((char *)buf, sizeof(buf));
	if(ret<=0)
	{
		printf("ODMtest: Part Number get failed.\n");
	}
	else
	{
		printf("get_odm_PN: ret=%d, Part Number=[%s]\n", ret, buf);
	}

	memset(buf,0x0,sizeof(buf));
	ret = get_odm_MAC(buf, sizeof(buf));
	if(ret<=0)
	{
		printf("ODMtest: MAC get failed.\n");
	}
	else
	{
		printf("get_odm_MAC: ret=%d, MAC=[%02X:%02X:%02X:%02X:%02X:%02X]\n", ret, buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
	}

	memset(buf,0x0,sizeof(buf));
	ret = get_valid_MAC(0, buf, 6);
	if(ret<=0)
	{
		printf("ODMtest: valid MAC[0] get failed.\n");
	}
	else
	{
		printf("get_valid_MAC(0): ret=%d, MAC=[%02X:%02X:%02X:%02X:%02X:%02X]\n", ret, buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
	}
	ret = get_valid_MAC(1, buf, 6);
	if(ret<=0)
	{
		printf("ODMtest: valid MAC[1] get failed.\n");
	}
	else
	{
		printf("get_valid_MAC(1): ret=%d, MAC=[%02X:%02X:%02X:%02X:%02X:%02X]\n", ret, buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
	}
	ret = get_valid_MAC(MAX_NUM_OF_MAC-1, buf, 6);
	if(ret<=0)
	{
		printf("ODMtest: valid MAC[%d] get failed.\n", MAX_NUM_OF_MAC-1);
	}
	else
	{
		printf("get_valid_MAC(%d): ret=%d, MAC=[%02X:%02X:%02X:%02X:%02X:%02X]\n", MAX_NUM_OF_MAC-1, ret, buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
	}

	return 0;
}
