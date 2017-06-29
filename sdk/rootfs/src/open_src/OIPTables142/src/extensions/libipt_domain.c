#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <iptables.h>
#include "ipt_domain.h"

static void help(void)
{
	printf(
	"domain options:\n"
	"  --name \"www.chinaunix.net\"            Match the domain named \"www.chinaunix.net\"\n"
	"  --name \"chinaunix.net\"                Match the domain named \"chinaunix.net\"\n"
	"                                        include www.chinunix.net, bbs.chinaunix.net, man.chinaunix.net\n"
	"\n");
}


static struct option opts[] = {
	{ "name", 1, 0, '1' },
	{ 0 }
};


static void init(struct ipt_entry_match *m, unsigned int *nfcache)
{
	struct ipt_domain_info *info = (struct ipt_domain_info *)m->data;

	*nfcache |= NFC_UNKNOWN;
	memset(info->name, 0, MAX_CHAR);
	memset(info->org_name, 0, MAX_CHAR);
}


static void parse_name(char *name, struct ipt_domain_info *info)
{
	int i, char_len = 0;
	int len = strlen(name) + 1;

	if (len > MAX_CHAR)
		exit_error(PARAMETER_PROBLEM,
					"The strings `%s' is too long (MAX_CHAR = %d",
					name, MAX_CHAR);
	if(name[0] == '.')
		strcpy(info->name, name);
	else
	{
		strcpy(info->name + 1, name);
		strcpy(info->org_name, name);
		info->name[0] = '.';
	}
#if 0
	for (i = len - 1; i >= 0; i--) {
		if (info->name[i] == '.') {
			info->name[i] = char_len;
			char_len = 0;
		} else {
			char_len++;
		}
	}
	
	info->len = len;
#endif
}

static int parse(int c, char **argv, int invert, unsigned int *flags,
			const void *entry,struct xt_entry_match **match)
{
	struct ipt_domain_info *info = (struct ipt_domain_info *)(*match)->data;

	check_inverse(optarg, &invert, &optind, 0);
	switch (c) {
		case '1':
			parse_name(argv[optind - 1], info);
			*flags = 1;
			break;

		default:
			return 0;
	}

	return 1;
}


static void final_check(unsigned int flags)
{
	if (!flags) exit_error(PARAMETER_PROBLEM, "domain expection an option");
}


static void print(const void *ip, const struct xt_entry_match *match,
			int numeric)
{
	const struct ipt_domain_info *info = (const struct ipt_domain_info *)match->data;

	printf("domain --name \"%s\"", info->org_name);
}


static void save(const void *ip, const struct xt_entry_match *match)
{

	const struct ipt_domain_info *info = (const struct ipt_domain_info *)match->data;

	printf("--name \"%s\" ", info->org_name);
}


static struct xtables_match domain = { 
	.version	= XTABLES_VERSION,
	//.next		= NULL,
	.name		= "domain",
	.revision	= 0,
	.size		= XT_ALIGN(sizeof(struct ipt_domain_info)),
	.userspacesize	= XT_ALIGN(sizeof(struct ipt_domain_info)),
	.help		= help,
	//.init		= &init,
	.parse		= parse,
	.final_check	= final_check,
	.print		= print,
	.save		= save,
	.extra_opts	= opts
};


void _init(void)
{
	xtables_register_match(&domain);
}
