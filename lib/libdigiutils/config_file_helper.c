/*****************************************************************************
* Copyright (c) 2015 Digi International Inc., All Rights Reserved
*
* This software contains proprietary and confidential information of Digi
* International Inc.  By accepting transfer of this copy, Recipient agrees
* to retain this software in confidence, to prevent disclosure to others,
* and to make no use of this software other than that for which it was
* delivered.  This is an unpublished copyrighted work of Digi International
* Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
* prohibited.
*
* Restricted Rights Legend
*
* Use, duplication, or disclosure by the Government is subject to
* restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
* Technical Data and Computer Software clause at DFARS 252.227-7031 or
* subparagraphs (c)(1) and (2) of the Commercial Computer Software -
* Restricted Rights at 48 CFR 52.227-19, as applicable.
*
* Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
*
*****************************************************************************/

#include "config_file_helper.h"
#include "config_file.h"
#include "element.h"
#include "config_mgr.h"
#include "digiuci.h"
#include "unittest_support.h"	/* STATIC_FUNCTION */
#include <stdio.h>
#include <string.h>
#include <uci.h>
#include <time.h>

STATIC_FUNCTION bool parameter_should_be_quoted(config_mgr_group_t const * const group, char const * const digi_parameter_name)
{
	bool needs_quoting = true;	/*default to true. Doesn't seem to do any harm to quote everything*/
	config_mgr_element_type_t element_type;

	if (get_element_type(group, digi_parameter_name, &element_type))
		needs_quoting = element_needs_quoting(element_type);

	return needs_quoting;
}

STATIC_FUNCTION void output_parameter_value(config_mgr_group_t const * const group, char const * const digi_parameter_name, struct uci_option * o, FILE * fp)
{
	struct uci_element * e;
	bool const needs_quoting = parameter_should_be_quoted(group, digi_parameter_name);

	if (needs_quoting)
		fputc('\"', fp);

	switch (o->type)
	{
		case UCI_TYPE_STRING:
			fprintf(fp, "%s", o->v.string);
			break;
		case UCI_TYPE_LIST:
		{
			char const * separator = "";

			uci_foreach_element(&o->v.list, e)
			{
				fprintf(fp, "%s%s", separator, e->name);
				separator = ",";
			}
			break;
		}
		default:
			// XXX should we output anything that doesn't result in a legal command?
			fprintf(fp, "<unknown>");
			break;
	}

	if (needs_quoting)
		fputc('\"', fp);

}

STATIC_FUNCTION void write_config_commands_to_file(config_mgr_group_t const * const group, char const * const section, struct uci_option * o, FILE * const fp)
{
	//debug("%s.%s.%s\n", o->section->package->e.name, o->section->e.name, o->e.name);

	/*
		we determine the instance number by getting the last few characters of the section name
		section names look something like "dsl_pvc1", so if the section name is dsl-pvc, the instance will be 1
	*/
	if (strlen(o->section->e.name) > strlen(section))
	{
	    char * suffix;
		char digi_parameter_name[CONFIG_MGR_NAME_LENGTH];
		char * instance = o->section->e.name + strlen(section);

		strlcpy_uci_to_digi(digi_parameter_name, o->e.name, sizeof digi_parameter_name);

		/* skip all clear-text passwords */
		suffix = strrchr(digi_parameter_name, '-');
		if ((suffix != NULL) && (strcmp(suffix, "-clear") == 0))
		{
		    goto done;
		}

		fprintf(fp, "%s %s %s ", section, instance, digi_parameter_name);

		// at this stage, we have <command> <instance> <parameter> written out.
		// Now it's time for the parameter value.
		output_parameter_value(group, digi_parameter_name, o, fp);

		/*end the line*/
		fprintf(fp, "\n");
	}
	else
	{
		fprintf(stderr, "uci section name too short. section name: '%s', uci name: '%s'\n", o->section->e.name, section);
	}

done:
    return;
}

STATIC_FUNCTION void write_group_section_to_file(FILE * fp, config_mgr_group_t const * const group, struct uci_section * const uci_section)
{
	// group name is in digi format (i.e. may contain '-')
	// uci_section->type will have '-' converted to '_'
	struct uci_element * uci_option_element;

	uci_foreach_element(&uci_section->options, uci_option_element)
	{
		write_config_commands_to_file(group, group->name, uci_to_option(uci_option_element), fp);
	}
}

int write_group_commands_to_file(struct uci_context * const uci_context, FILE * fp, config_mgr_group_t const * const group)
{
	int result = -1;
	struct uci_ptr uci_data;

	memset(&uci_data, 0, sizeof uci_data);
	if ((uci_lookup_ptr(uci_context, &uci_data, (char *)group->filename, true) != UCI_OK) ||
		!(uci_data.flags & UCI_LOOKUP_COMPLETE))
	{
		//fprintf(stderr, "Not found! Skip \"%s (%s)\" configuration\n", group->name, group->filename);
	}
	else
	{
		struct uci_element * uci_elements = uci_data.last;


		switch (uci_elements->type)
		{
			case UCI_TYPE_PACKAGE:
				{
					struct uci_element * uci_section_element;
					struct uci_package * uci_package = uci_data.p;

					//uci_reset_typelist();
					uci_foreach_element(&uci_package->sections, uci_section_element)
					{
						struct uci_section * uci_section = uci_to_section(uci_section_element);

						if (digi_name_matches_uci_name(group->name, uci_section->type))
						{
							write_group_section_to_file(fp, group, uci_section);
						}
					}
					//uci_reset_typelist();
				}

				break;
			case UCI_TYPE_SECTION:
				//uci_show_section(ptr.s);
				break;
			case UCI_TYPE_OPTION:
				//uci_show_option(ptr.o, true);
				break;
			default:
				/* should not happen */
				printf("Unknown package type\n");
				goto error;
		}
	}
	result = 0;

error:
	return result;
}
