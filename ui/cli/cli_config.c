/*****************************************************************************
 * Copyright (c) 2013 Digi International Inc., All Rights Reserved
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
#include "cli.h"

cli_status_t cli_get_uint32_value (config_ipm_tree_type_t const tree_type, char const * const group_name, size_t const instance, char const * const element_name, uint32_t *value)
{
    cli_status_t  retval = cli_config_error;
    config_ipm_command_data_t const * group;

    {
        void * ptr;
        config_ipm_tree_data_type_t ptr_type;

        if (ConfigIpm_Find(tree_type, group_name, &ptr, &ptr_type) != config_ipm_success)
            goto ret;

        group = ptr;
    }

    if (group != NULL)
    {
        config_ipm_element_data_t const * element = NULL;

        {
            void * ptr;
            config_ipm_tree_data_type_t ptr_type;
            if (ConfigIpm_FindTree(group->search_tree, element_name, &ptr, &ptr_type) != config_ipm_success)
                goto ret;

            if (ptr_type != config_ipm_tree_data_type_element)
            {
                cli_debug("Invalid tree data type %d (not element)\n", ptr_type);
            }
            else
            {
                element = ptr;
            }

        }

        if (element != NULL)
        {
            uint32_t  * unsigned_integer_value;
            char const * value_string;

            if (ConfigIpm_GetElement(group->id, instance, element->id, &value_string) != config_ipm_success)
                goto ret;

            if (ConfigIpm_ToElementValue(element, value_string, (void **)&unsigned_integer_value) != config_ipm_success)
                goto ret;

            *value = *unsigned_integer_value;
            retval = cli_success;
        }
    }
ret:
    return retval;
}


cli_status_t cli_get_string_value(config_ipm_tree_type_t const tree_type, char const * const group_name,
                                   size_t const instance, char const * const element_name, char const ** value)
{
    cli_status_t retval = cli_config_error;
    config_ipm_command_data_t const * group;

    {
        void * ptr;
        config_ipm_tree_data_type_t ptr_type;

        ConfigIpm_Find(tree_type, group_name, &ptr, &ptr_type);
        group = ptr;
    }


    if (group != NULL)
    {
      config_ipm_element_data_t const * element = NULL;
      {
          void * ptr;
          config_ipm_tree_data_type_t ptr_type;
          if (ConfigIpm_FindTree(group->search_tree, element_name, &ptr, &ptr_type) != config_ipm_success)
              goto ret;

          if (ptr_type != config_ipm_tree_data_type_element)
          {
              cli_debug("Invalid tree data type %d (not element)\n", ptr_type);
          }
          else
          {
              element = ptr;
          }

      }

      if (element == NULL)
          goto ret;


      if (ConfigIpm_GetElement(group->id, instance, element->id, value) != config_ipm_success)
        goto ret;

      retval = cli_success;
    }

ret:
    return retval;
}


