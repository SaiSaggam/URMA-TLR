#include "parse_str.h"

#define	ITEM_LEN	100

int save_item( int cs_fd, char* item_name, char *item_val);

static struct item item_status_set[]=
{
//	{"wanstatus", CSID_S_XDSL_PVC_LINK_STATUS, TYPE_U8},   //w ad s
	{"modem status", CSID_S_XDSL_LINESTATE, TYPE_U8},         //w ad s
	{"Standard", CSID_S_XDSL_MODULATION, TYPE_U8},
	{"noise margin upstream", CSID_S_XDSL_SNR_UP, TYPE_STR},               //w ad l f (noise margin)
	{"noise margin downstream", CSID_S_XDSL_SNR_DOWN, TYPE_STR},           //w ad l n
	{"attenuation upstream", CSID_S_XDSL_ATTEN_UP, TYPE_STR},           //w ad l f (attenuation)
	{"attenuation downstream", CSID_S_XDSL_ATTEN_DOWN, TYPE_STR},       //w ad l n
	{"far-end interleaved", CSID_S_XDSL_DATARATE_UP, TYPE_STR},     //w ad c (near-end interleaved)
	{"near-end interleaved", CSID_S_XDSL_DATARATE_DOWN, TYPE_STR}, //w ad c (far-end interleaved)
//	{"syspwd", CSID_S_XDSL_PASSWORD, TYPE_STR},
	{"MAC Address", CSID_S_XDSL_CPE_MACADDR, TYPE_MAC},            //sys version
	{"DSL FW", CSID_S_XDSL_FW_VER, TYPE_STR},               //sys version
//	{"adslmode", CSID_S_XDSL_CUR_MODE_SETTING, TYPE_U8},
//	{"adsltype", CSID_S_XDSL_CUR_TYPE_SETTING, TYPE_U8},
//	{"adsltx", CSID_S_XDSL_TX_FRAME, TYPE_U32},
//	{"adslrx", CSID_S_XDSL_RX_FRAME, TYPE_U32},
	{"power upstream", CSID_S_XDSL_POWER_US, TYPE_STR},           //w ad l n (output power up)
	{"power downstream", CSID_S_XDSL_POWER_DS, TYPE_STR},         //w ad l f (output power down)
};

int parse_str(char *org_str)
{
	char *str1, *str2, *token, *subtoken;
	char *saveptr1, *saveptr2;
	int i;
	char item[ITEM_LEN], value[ITEM_LEN], extra[ITEM_LEN];
	int cs_fd;

	cs_fd = open_csman(NULL,0);
	if(cs_fd<0)
	{
		return -1;
	}

	for ( str1=org_str ; ; str1 = NULL)
	{
		token = strtok_r(str1, ",", &saveptr1);
		if (token == NULL)
			break;
		//printf("token--> %s\n", token);
		// sub-token
		memset(item, 0x00, sizeof(item));
		memset(value, 0x00, sizeof(value));
		memset(extra, 0x00, sizeof(extra));

		for (i=0, str2=token ; ; i++, str2=NULL)
		{
			subtoken = strtok_r(str2, ":", &saveptr2);
			if (subtoken == NULL)
          			break;
	    		//printf(" ----->i=%d %s\n", i, subtoken);
			if(strlen(subtoken) < ITEM_LEN)
			{
				if(i==0)
					sprintf(item,"%s", subtoken);
				else if(i==1)
					sprintf(value,"%s", subtoken);
        			else
        			{
					sprintf(extra,"%s", subtoken);
					if (strstr(subtoken, "HwVer"))
						strncpy(value, subtoken, 15);
          				else if (strstr(item, "MAC Address")) {
						strcat(value, ":"); strncat(value, extra, 2);
					}else if (strstr(subtoken, "(tone)"))
						return 1;
				}
			}else
			{
			   //printf("token len overflow!\n");
			}
		}
		if((strlen(item)>0) && (strlen(value)>0))
		{
			//printf("item->%s, value->%s, extra=%s\n",item, value, extra);
			save_item( cs_fd, item, value);
		}
	}
	close_csman(cs_fd);
	return 0;
}


int save_item( int cs_fd, char* item_name, char *item_val)
{
	int i, k, value, value2, item_cnt, pvc_index, if_en;
	unsigned int item_csid, item_csid2;
	unsigned char mac[6];
	char csid_str[50];

	for (k=START_INDEX; k<END_INDEX; k++)
	{
		read_csman(cs_fd, CSID_C_LOCAL_MULTIWAN_ENABLE+k, &if_en, 4, CSM_R_ZERO);
		if (if_en==1)
		{
			pvc_index = k-START_INDEX;

    			item_cnt = sizeof(item_status_set)/sizeof(struct item);

    			for(i=0; i< item_cnt; i++)
    			{
    				if( strstr(item_name, item_status_set[i].item_name))
    				{
    					item_csid = item_status_set[i].item_CSID;

    					switch(	item_status_set[i].type)
    					{
    						case TYPE_U8:
    						case TYPE_U16:
    						case TYPE_U32:
    							if (strstr(item_val, "down"))
                						value = 0;
							else if (strstr(item_val, "wait for initialization"))
              							value = 1;
							else if (strstr(item_val, "initializing"))
                						value = 2;
							else if (strstr(item_val, "up"))
                						value = 3;
							else if (strstr(item_val, "ADSL2 PLUS"))
                						value = 4;
							else if (strstr(item_val, "ADSL"))
                						value = 3;
							else if (strstr(item_val, "Multi-Mode"))
								value = 5;
							else
                						value = 0;

							if( item_csid == CSID_S_XDSL_LINESTATE) {

    					  			item_csid2 = CSID_S_XDSL_PVC_LINK_STATUS+pvc_index*16;  //temporily
                						if (strstr(item_val, "up")) {
                  							value2 = 1;
                  							write_csman(cs_fd, item_csid2, &value2, sizeof(value2)/*1<<item_status_set[i].type)*/, 0);
                						}else {
                  							value2 = 0;
                  							write_csman(cs_fd, item_csid2, &value2, sizeof(value2), 0);
                						}
              						}
    							write_csman(cs_fd, item_csid, &value, sizeof(value), 0);

              					break;
    						case TYPE_STR:
    							memset( csid_str, 0x00, sizeof(csid_str));
    							//memcpy( csid_str, item_val, strlen(item_val));
    							sprintf( csid_str, "%s", item_val);
    							write_csman(cs_fd, item_csid, csid_str, strlen(csid_str)+1, 0);
              					break;
    						case TYPE_MAC:
    							sscanf(item_val,"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",&mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
    						//	printf("save->>%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    							write_csman(cs_fd, item_csid, mac, 6, 0);
    						default:
    						break;
    					}
    					//printf("+++++item_csid=0x%08x, value=%s\n", item_csid, item_val);
    					break;
    				}

    			}


    		}
	}


	return 0;
}

