#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <xtables.h>
#include "ipt_webstr.h"

/* Function which prints out usage message. */
static void help(void)
{
    printf("WEBSTR match v%s options:\n"
           "--webstr [!] host            Match a http string in a packet\n"
           "--webstr [!] url             Match a http string in a packet\n"
           "--webstr [!] content         Match a http string in a packet\n"
           "--webstr [!] fext            Match a http string in a packet\n",
           XTABLES_VERSION);

    fputc('\n', stdout);
}     

static const struct option opts[] = { 
    {.name = "host",    .has_arg = true, .val = '1'},
    {.name = "url",     .has_arg = true, .val = '2'},
    {.name = "content", .has_arg = true,  .val = '3'},
    {.name = "fext",    .has_arg = true,  .val = '4'}, 
    XT_GETOPT_TABLEEND,                                                                                                      
};

/* Initialize the match. */
static void init(struct xt_entry_match *m)
{
	//struct ipt_webstr_info *info = (struct ipt_webstr_info *)m->data;
	//memset(info->name, 0, MAX_CHAR);
	//memset(info->org_name, 0, MAX_CHAR);
    return;
}

static void
parse_string(const unsigned char *s, struct ipt_webstr_info *info)
{       
    if (strlen(s) <= BM_MAX_NLEN) strcpy(info->string, s);
    else xtables_error(PARAMETER_PROBLEM, "WEBSTR too long `%s'", s);
}   

static int parse(int c, char **argv, int invert, unsigned int *flags,
			const void *entry,struct xt_entry_match **match)
{
    struct ipt_webstr_info *stringinfo = (struct ipt_webstr_info *)(*match)->data;

    switch (c) {
        case '1':
            xtables_check_inverse(optarg, &invert, &optind, 0, argv);
            parse_string(argv[optind-1], stringinfo);
            if (invert)
                stringinfo->invert = 1;
            stringinfo->len=strlen((char *)&stringinfo->string);
            stringinfo->type = IPT_WEBSTR_HOST;
            break;

        case '2':
            xtables_check_inverse(optarg, &invert, &optind, 0, argv);
            parse_string(argv[optind-1], stringinfo);
            if (invert)
                stringinfo->invert = 1;
            stringinfo->len=strlen((char *)&stringinfo->string);
            stringinfo->type = IPT_WEBSTR_URL;
            break;

        case '3':
            xtables_check_inverse(optarg, &invert, &optind, 0, argv);
            parse_string(argv[optind-1], stringinfo);
            if (invert)
                stringinfo->invert = 1;
            stringinfo->len=strlen((char *)&stringinfo->string);
            stringinfo->type = IPT_WEBSTR_CONTENT;
            break;
        case '4':
            xtables_check_inverse(optarg, &invert, &optind, 0, argv);
            parse_string(argv[optind-1], stringinfo);
            if (invert)
                stringinfo->invert = 1;
            stringinfo->len=strlen((char *)&stringinfo->string);
            stringinfo->type = IPT_WEBSTR_FEXT;
            break;
        default:
            return 0;
    }

    *flags = 1;
    return 1;   
}


static void print_string(char string[], int invert, int numeric)
{
    if (invert)
        fputc('!', stdout);
    printf("%s ",string);
} 

static void print(const void *ip, const struct xt_entry_match *match,
			int numeric)
{
    struct ipt_webstr_info *stringinfo = (struct ipt_webstr_info *)match->data;
    printf("WEBSTR match ");

    switch (stringinfo->type) {
        case IPT_WEBSTR_HOST:
            printf("host ");
            break;

        case IPT_WEBSTR_URL:
            printf("url ");
            break;

        case IPT_WEBSTR_CONTENT:
            printf("content ");
            break;
        case IPT_WEBSTR_FEXT:
            printf("fext ");
            break;
        default:
            printf("ERROR ");
            break;
    }
    print_string(((struct ipt_webstr_info *)match->data)->string,
              ((struct ipt_webstr_info *)match->data)->invert, numeric);
}


static void save(const void *ip, const struct xt_entry_match *match)
{
    printf("--webstr ");
    print_string(((struct ipt_webstr_info *)match->data)->string,
                 ((struct ipt_webstr_info *)match->data)->invert, 0);
}


static struct xtables_match webstr = { 
	.name		= "webstr",
	.version    = XTABLES_VERSION,
	.family		= NFPROTO_IPV4,
	.size		= XT_ALIGN(sizeof(struct ipt_webstr_info)),
	.userspacesize	= XT_ALIGN(sizeof(struct ipt_webstr_info)),
	.help		= help,
    .init		= init,
	.parse		= parse,
	.print		= print,
	.save		= save,
	.extra_opts	= opts,
};


void _init(void)
{
	xtables_register_match(&webstr);
}
