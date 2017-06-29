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

#include "test_config_dsl_to_modem.h"
#include "test_dsl_led.h"
#include "test_modem_to_config_state_dsl.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

int main(void)
{
	int result;

	if (CU_initialize_registry() != CUE_SUCCESS)
	{
		result = CU_get_error();
		goto error;
	}

    if (CU_register_nsuites(3,
            test_config_dsl_to_modem_suites,
            test_dsl_led_suites,
            test_modem_to_config_state_dsl_suites) != CUE_SUCCESS)
	{
		result = CU_get_error();
		goto error;
	}

    CU_basic_set_mode(CU_BRM_VERBOSE);
	if (CU_basic_run_tests() != CUE_SUCCESS)
	{
        result = CU_get_error();
        goto error;
	}

    result = CU_get_number_of_failures();

error:
	CU_cleanup_registry();
	return result;
}
