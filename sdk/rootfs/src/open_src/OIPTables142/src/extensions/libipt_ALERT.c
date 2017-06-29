/* Shared library add-on to iptables to add ALERT target support.
 * (C) 2010 by Edward Lin <edward730109@gmail.com>
 * 
 * This program is distributed under the terms of GNU GPL
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include <iptables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <ipt_ALERT.h>

#ifndef XTABLES_VERSION
#define XTABLES_VERSION IPTABLES_VERSION
#endif

#ifdef IPT_LIB_DIR
#define xtables_target iptables_target
#define xtables_register_target register_target
#endif

#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define IPT_ID_USED		1
#define IPT_MSG_USED		2

/* Function which prints out usage message. */
static void
help(void)
{
printf(
"ALERT target v%s options:\n"
"  --id value             use id\n"
"  --type value            use password\n"
"\n",
XTABLES_VERSION);
}

static struct option opts[] = {
	{ "id", 1, 0, '1' },
	{ "type", 1, 0, '2' },
	{ 0 }
};

/* Initialize the target. */
static void
#ifdef _XTABLES_H
init(struct xt_entry_target *t)
#else
init(struct ipt_entry_target *t, unsigned int *nfcache)
#endif
{
	struct ipt_alert_target_info *alertinfo =
		(struct ipt_alert_target_info *)t->data;

	memset (alertinfo->type, 0, MAIL_LEN);
	alertinfo->id = -1;
}

/* Function which parses command options; returns true if it
   ate an option */
static int
parse(int c, char **argv, int invert, unsigned int *flags,
#ifdef _XTABLES_H
      const void *entry, struct xt_entry_target **target)
#else
      const struct ipt_entry *entry, struct ipt_entry_target **target)
#endif
{
	struct ipt_alert_target_info *alertinfo
		= (struct ipt_alert_target_info *)(*target)->data;

	switch (c) {

	case '1':
		alertinfo->id = atoi(optarg);
		if (*flags & IPT_ID_USED)
			exit_error(PARAMETER_PROBLEM,
			           "ALERT target: Can't specify --id twice");
		*flags |= IPT_ID_USED;
		break;

	case '2':
		strcpy(alertinfo->type, optarg);
		if (*flags & IPT_MSG_USED)
			exit_error(PARAMETER_PROBLEM,
			           "ALERT target: Can't specify --type twice");
		*flags |= IPT_MSG_USED;
		break;

	default:
		return 0;
	}

	return 1;
}

static void
final_check(unsigned int flags)
{
	if (!(flags & IPT_ID_USED))
		exit_error(PARAMETER_PROBLEM,
		           "ALERT target: Parameter --id is required");
	if (!(flags & IPT_MSG_USED))
		exit_error(PARAMETER_PROBLEM,
		           "ALERT target: Parameter --type is required");
}

/* Prints out the targinfo. */
static void
#ifdef _XTABLES_H
print(const void *ip,
      const struct xt_entry_target *target,
#else
print(const struct ipt_ip *ip,
      const struct ipt_entry_target *target,
#endif
      int numeric)
{
	const struct ipt_alert_target_info *alertinfo =
		(const struct ipt_alert_target_info *)target->data;

	printf("%d\n%s", alertinfo->id, alertinfo->type);
}

/* Saves the union ipt_targinfo in parsable form to stdout. */
static void
#ifdef _XTABLES_H
save(const void *ip,
     const struct xt_entry_target *target)
#else
save(const struct ipt_ip *ip,
     const struct ipt_entry_target *target)
#endif
{
	const struct ipt_alert_target_info *alertinfo =
		(const struct ipt_alert_target_info *)target->data;

	printf("--id %d ", alertinfo->id);

	printf("--type %s ", alertinfo->type);
}

static struct xtables_target alert = { 
	.next		= NULL,
	.name		= "ALERT",
	.version	= XTABLES_VERSION,
	.size		= IPT_ALIGN(sizeof(struct ipt_alert_target_info)),
	.userspacesize	= IPT_ALIGN(sizeof(struct ipt_alert_target_info)),
	.help		= &help,
	.init		= &init,
	.parse		= &parse,
	.final_check	= &final_check,
	.print		= &print,
	.save		= &save,
	.extra_opts	= opts
};

void _init(void)
{
	xtables_register_target(&alert);
}
