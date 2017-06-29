#ifndef _IPT_ALERT_H_target
#define _IPT_ALERT_H_target

#define MAIL_LEN	300

struct ipt_alert_target_info {
	char type[MAIL_LEN];
	int id;
};

#endif /*_IPT_ALERT_H_target*/
