/* Shared library add-on to iptables to add MARK target support. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include <iptables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
/* For 64bit kernel / 32bit userspace */
//#include "../include/linux/netfilter_ipv4/ipt_SPAP.h"
#include "ipt_SPAP.h"

/* Function which prints out usage message. */
static void
help(void)
{
	printf(
"SPAP target options:\n"
"--queue\n");
}
#if 0
static struct option opts[] = {
	{ "queue", 1, 0, '1' },
	{ .name = NULL }
};
#endif
static struct option opts[] = {
	{ .name = "queue", .has_arg = false, .val = '1' },
    XT_GETOPT_TABLEEND,
};

static void
final_check(unsigned int flags)
{
	if (!flags)
		exit_error(PARAMETER_PROBLEM,
		           "SPAP target: --queue");
}

/* Function which parses command options; returns true if it
   ate an option */
static int
parse(int c, char **argv, int invert, unsigned int *flags,
	 const void *entry,
	 struct xt_entry_target **target)
{
	struct ipt_spap_target_info *spapinfo
		= (struct ipt_spap_target_info *)(*target)->data;
	
	*flags = 1;
	return 1;
}

/* Prints out the targinfo. */
static void
print(const void *ip,
	 const struct xt_entry_target *target,
	 int numeric)
{
	printf("SPAP target\n");
}

/* Saves the union ipt_targinfo in parsable form to stdout. */
static void
save(const void *ip, const struct xt_entry_target *target)
{
	printf("set SPAP\n");
}


static
struct xtables_target spap_tg_reg = {
	.name			= "SPAP",
	.version		= XTABLES_VERSION,
    .revision       = 0,
	.family			= NFPROTO_IPV4,
	.size			= XT_ALIGN(sizeof(struct ipt_spap_target_info)),
	.userspacesize	= XT_ALIGN(sizeof(struct ipt_spap_target_info)),
	.help			= help,
	.parse			= parse,
	.final_check	= final_check,
	.print			= print,
	.save			= save,
	.extra_opts		= opts
};

void _init(void)
{
	xtables_register_target(&spap_tg_reg);
}
