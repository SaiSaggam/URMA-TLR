/*
 * Copyright (c) 2013 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */
#define CONNECTOR_RCI_PARSER_INTERNAL_DATA

static char const * const element_type_strings[] = {
    "string",
    "multiline_string",
    "password",
    "int32",
    "uint32",
    "hex32",
    "0xhex",
    "float",
    "enum",
    "on_off",
    "boolean",
    "ipv4",
    "fqdnv4",
    "fqdnv6",
    "datetime"
};

static char * const element_access_strings[] = {
    "read_only",
    "write_only",
    "read_write"
};

static void print_name(FILE * fp, char const * const label, char const * const name)
{
    unsigned char length = *name;
    char * ptr = (char *)name+1;

    fprintf(fp, "%s %.*s ", label, length, ptr);
}

static void debug_name(char const * const label, char const * const name)
{
    unsigned char length = *name;
    char * ptr = (char *)name+1;

    printf("%s %.*s ", label, length, ptr);
}
void print_remote_configurations(void)
{
    size_t i;
    int n;
    FILE * fp;
    static char const filename[] = "group_config.ic";

    fp = fopen(filename, "w+");
    if (fp == NULL)
    {
        printf("Unable to create %s file\n", filename );
        goto done;
    }

    for (n = 0; n < 2; n++)
    {

    for (i = 0; i < connector_remote_group_table[n].count; i++)
    {
        connector_remote_group_t const * const group_ptr = &connector_remote_group_table[n].groups[i];
        size_t j;

        print_name(fp, "\n\ngroup", group_ptr->name);
        if (group_ptr->instances > 1)
        {
            fprintf(fp, "[%d] \"<description>\"", group_ptr->instances);
        }

        for (j=0; j < group_ptr->elements.count; j++)
        {
            connector_group_element_t const * const element_ptr = &group_ptr->elements.data[j];
            print_name(fp, "\n\tconfig", element_ptr->name);
            fprintf(fp, "\"<description>\" type= %s ", element_type_strings[element_ptr->type]);
            fprintf(fp, "access = %s ", element_access_strings[element_ptr->access]);

            if (element_ptr->value_limit != NULL)
            {

                switch (element_ptr->type)
                {
#if defined(RCI_PARSER_USES_ENUMERATIONS)
                case connector_element_type_enum:
                {
                    connector_element_value_enum_t const * const enum_ptr = &element_ptr->value_limit->enum_value;
                    size_t n;

                    for (n=0; n < enum_ptr->count; n++)
                    {
                        print_name(fp, "\n\t\tvalue", enum_ptr->value[n]);
                        fprintf(fp, " \"<description>\"");
                    }
                    break;
                }
#endif
                case connector_element_type_int32:
                {
                    connector_element_value_signed_integer_t const * const value_limit = &element_ptr->value_limit->signed_integer_value;
                    if (value_limit->min_value != 0 || value_limit->max_value != 0)
                    {
                        fprintf(fp, " min = %d max = %d ", value_limit->min_value, value_limit->max_value);
                    }
                    break;
                }
                case connector_element_type_uint32:
                {
                    connector_element_value_unsigned_integer_t const * const value_limit = &element_ptr->value_limit->unsigned_integer_value;
                    if (value_limit->min_value != 0 || value_limit->max_value != 0)
                    {
                        fprintf(fp, " min = %u max = %u ", value_limit->min_value, value_limit->max_value);
                    }
                    break;
                }
                case connector_element_type_hex32:
                {
                    connector_element_value_unsigned_integer_t const * const value_limit = &element_ptr->value_limit->unsigned_integer_value;
                    if (value_limit->min_value != 0 || value_limit->max_value != 0)
                    {
                        fprintf(fp, " min = %X max = %X ", value_limit->min_value, value_limit->max_value);
                    }
                    break;
                }
                case connector_element_type_0x_hex32:
                {
                    connector_element_value_unsigned_integer_t const * const value_limit = &element_ptr->value_limit->unsigned_integer_value;
                    if (value_limit->min_value != 0 ||  value_limit->max_value != 0)
                    {
                        fprintf(fp, " min = 0x%X max = 0x%X ", value_limit->min_value, value_limit->max_value);
                    }
                    break;
                }
#if defined(RCI_PARSER_USES_FLOATING_POINT)
                case connector_element_type_float:
                {
                    connector_element_value_float_t const * const value_limit = &element_ptr->value_limit->float_value;
                    if (value_limit->min_value != 0 || value_limit->max_value != 0)
                    {
                      fprintf(fp, " min = \"%f\" max = \"%f\" ", value_limit->min_value, value_limit->max_value);
                    }
                    break;
                }
#endif
                case connector_element_type_string:
                case connector_element_type_multiline_string:
                case connector_element_type_password:
                {
                    connector_element_value_string_t const * const value_limit = &element_ptr->value_limit->string_value;
                    if (value_limit->min_length_in_bytes != 0 || value_limit->max_length_in_bytes != 0)
                    {
                        fprintf(fp, " min = %d max = %d ", value_limit->min_length_in_bytes, value_limit->max_length_in_bytes);
                    }
                    break;
                }
                default:
                    debug_name("element", element_ptr->name);
                    printf("invalid min and max on the type: %d\n", element_ptr->type);
                    break;
                }
            }
        }
/*
        for (j=0; j < connector_group_error_count-1; j++)
        {
            static unsigned int const connector_errors_index[connector_group_error_count] = {
                                                ERROR_FIELD_NOT_EXIT_STRING_INDEX,
                                                ERROR_LOAD_FAILED_STRING_INDEX,
                                                ERROR_SAVE_FAILED_STRING_INDEX,
                                                ERROR_UNKNOWN_VALUE_STRING_INDEX
            };
            print_name(fp, "\n\terror ", &connector_all_strings[connector_errors_index[j]]);

        }
*/
        for (j=0; j < group_ptr->errors.count; j++)
        {
            print_name(fp, "\n\terror <name>", group_ptr->errors.description[j]);
        }
        fprintf(fp, "\n");
    }
    }
    fclose(fp);
done:
    return;
}

