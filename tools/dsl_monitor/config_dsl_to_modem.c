/*****************************************************************************
* Copyright (c) 2016 Digi International Inc., All Rights Reserved
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

#include "config_dsl_to_modem.h"
#include "config_mgr.h"

#include <assert.h>

char const * config_dsl_mode_to_modem(config_dsl_mode_t const mode)
{
	char const * result = NULL;

	switch (mode)
	{
		case config_dsl_mode_auto:
			result = "adsl2plus_multi";
			break;
		case config_dsl_mode_adsl2_plus:
			result = "adsl2plus";
			break;
		case config_dsl_mode_adsl2:
			result = "adsl2";
			break;
		case config_dsl_mode_gdmt:
			result = "adsl1";
			break;
        case config_dsl_mode_glite:
			result = "lite";
			break;
	}

    assert(result != NULL);

	return result;
}

int config_dsl_encapsulation_to_modem_encap(config_dsl_encapsulation_t const encapsulation)
{
	int result = -1;

	switch (encapsulation)
	{
		case config_dsl_encapsulation_pppoa_llc:
		case config_dsl_encapsulation_pppoe_llc:
			result = 0;
			break;
		case config_dsl_encapsulation_pppoa_vcmux:
		case config_dsl_encapsulation_pppoe_vcmux:
			result = 1;
			break;
	}

    assert(result != -1);

	return result;
}

int config_dsl_encapsulation_to_modem_mode(config_dsl_encapsulation_t const encapsulation)
{
	int result = -1;

	switch (encapsulation)
	{
		case config_dsl_encapsulation_pppoe_vcmux:
		case config_dsl_encapsulation_pppoe_llc:
			result = 0;
			break;
		case config_dsl_encapsulation_pppoa_vcmux:
		case config_dsl_encapsulation_pppoa_llc:
			result = 1;
			break;
	}

    assert(result != -1);

	return result;
}
