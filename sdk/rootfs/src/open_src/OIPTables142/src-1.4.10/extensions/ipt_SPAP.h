#ifndef _IPT_SPAP_H_target
#define _IPT_SPAP_H_target

#define SPAP_NUM 8

struct ipt_spap_target_info {
#ifdef KERNEL_64_USERSPACE_32
	unsigned long long mark;
#else
	unsigned long mark;
#endif
};

typedef struct info_que {
	unsigned long mark;
	unsigned long src_ip;
	unsigned short trig_port;
	struct info_que *next;	
} Info_Que;
#endif /*_IPT_MARK_H_target*/
