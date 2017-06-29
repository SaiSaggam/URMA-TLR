/*
 * Copyright (c) 2014 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */

#include <stdarg.h>

#define BINARY_RCI_ATTRIBUTE_BIT  UINT32_C(0x40)  /* bit 6 */
#define BINARY_RCI_ATTRIBUTE_TYPE_MASK  0x60  /* attr type: [bit 6 and 5] */
#define BINARY_RCI_ATTRIBUTE_TYPE_NORMAL 0x00
#define BINARY_RCI_ATTRIBUTE_TYPE_INDEX  0x20
#define BINARY_RCI_ATTRIBUTE_TYPE_NAME   0x40

#define BINARY_RCI_NO_VALUE        UINT32_C(0xE0)
#define BINARY_RCI_TERMINATOR      UINT32_C(0xE1)

#define BINARY_RCI_ERROR_INDICATOR_BIT  UINT32_C(0x1000) /* bit 12 */

#define BINARY_RCI_FIELD_TYPE_INDICATOR_BIT UINT32_C(0x40) /* bit 6 */
#define BINARY_RCI_FIELD_ATTRIBUTE_BIT      UINT32_C(0x400) /* bit 10 */
/* #define BINARY_RCI_FIELD_ASCENDING_INDICATOR_BIT    (0x1 << 6) */

/*
 *    7 6 5 4 3 2 1 0 bit
 *    0 X X X X X X X   (0 : 0x7F)
 *    1 0 0 X X X X X   + 1 byte followed (0: 0x1FFF)
 *    1 0 1 0 0 0 0 0   + 2 bytes followed (0: 0xFFFF)
 *    1 0 1 0 0 0 0 1   + 4 bytes followed (0: 0xFFFFFFFF)
 *    1 0 1 0 0 0 1 0   + 8 bytes followed (0: 0xFFFFFFFFFFFFFFFF)
 *    1 1 0 0 - - - -   Reserved
 *    1 1 1 0 0 0 0 0   NONUM (No Value)
 *    1 1 1 0 0 0 0 1   TRM (Terminator)
 */
#define BINARY_RCI_SIZE_ALTERNATE_FLAG          UINT32_C(0x80) /* bit 7 */
#define BINARY_RCI_SIZE_MODIFIER_MASK           UINT32_C(0x60) /* bits 6:5 */
#define BINARY_RCI_SIZE_MODIFIER(value)         (rci_size_modifier_t)((value & BINARY_RCI_SIZE_MODIFIER_MASK) >> UINT32_C(5))

typedef enum {
    binary_rci_one_follow_byte,
    binary_rci_multi_follow_byte,
    binary_rci_special_value    = 3
} rci_size_modifier_t;


#define BINARY_RCI_MULTI_FOLLOW_BYTE_MASK       UINT32_C(0x03)
#define BINARY_RCI_MULTI_FOLLOW_BYTES(value)    (value & BINARY_RCI_MULTI_FOLLOW_BYTE_MASK)
#define BINARY_RCI_SET_MULTI_FOLLOW_BYTES(value) (BINARY_RCI_SIZE_ALTERNATE_FLAG | (binary_rci_multi_follow_byte << UINT32_C(5)) |value)

enum {
    binary_rci_two_follow_byte,
    binary_rci_four_follow_byte,
    binary_rci_eight_follow_byte
};

#define RCI_FLAG_GET_ALL_INSTANCES     UINT32_C(0x01)

#define RCI_NO_HINT             NULL
#define INVALID_ID              UINT_MAX
#define INVALID_INDEX           UINT_MAX
#define INVALID_COUNT           ((size_t)-1)

#define ROUND_UP(value, interval)   ((value) + -(value) % (interval))

#define UNHANDLED_CASES_ARE_INVALID default: ASSERT(connector_false); break;

static char const nul = '\0';

enum rci_ber {
    field_define(rci_ber, value, uint8_t),
    record_end(rci_ber)
};

enum rci_ber_u8 {
    field_define(rci_ber_u8, opcode, uint8_t),
    field_define(rci_ber_u8, value, uint8_t),
    record_end(rci_ber_u8)
};

enum rci_ber_u16 {
    field_define(rci_ber_u16, opcode, uint8_t),
    field_define(rci_ber_u16, value, uint16_t),
    record_end(rci_ber_u16)
};

enum rci_ber_u32 {
    field_define(rci_ber_u32, opcode, uint8_t),
    field_define(rci_ber_u32, value, uint32_t),
    record_end(rci_ber_u32)
};


enum
{
    rci_field_type_none,
    rci_field_type_string,
    rci_field_type_multiline_string,
    rci_field_type_password,
    rci_field_type_int23,
    rci_field_type_uint32,
    rci_field_type_hex32,
    rci_field_type_0x_hex32,
    rci_field_type_float,
    rci_field_type_enum,
    rci_field_type_on_off = 0x0B,
    rci_field_type_boolean,
    rci_field_type_ip4,
    rci_field_type_fqdnv4,
    rci_field_type_fqdnv6,
    rci_field_type_mac_addr = 0x15,
    rci_field_type_datetime
};

typedef enum {
    rci_command_query_setting = 1,
    rci_command_set_setting,
    rci_command_query_state,
    rci_command_set_state,
    rci_command_query_descriptor
#if (defined RCI_LEGACY_COMMANDS)
    ,rci_command_do_command,
    rci_command_reboot,
    rci_command_set_factory_default
#endif
} rci_command_t;

typedef enum
{
    attribute_type_enum
#if (defined RCI_LEGACY_COMMANDS)
    ,attribute_type_string
#endif
} rci_command_attribute_type_t;

typedef union
{
    unsigned int val;
    rci_query_setting_attribute_id_t query_setting;
#if (defined RCI_LEGACY_COMMANDS)
    rci_do_command_attribute_id_t do_command;
#endif
} rci_command_attribute_t;

typedef enum
{
    rci_session_start,
    rci_session_active,
    rci_session_lost
} rci_session_t;

typedef enum
{
    rci_status_internal_error,  /* bad code path */
    rci_status_complete,        /* all done */
    rci_status_busy,            /* user callback returned busy */
    rci_status_more_input,      /* have output buffer space to process more input */
    rci_status_flush_output,    /* need more output space, so send out the current buffer */
    rci_status_error            /* error occurred, RCI service should inform messaging layer to cancel the session */
} rci_status_t;

typedef struct
{
    uint8_t * data;
    size_t bytes;
    unsigned int flags;
} rci_service_buffer_t;

typedef struct
{
    connector_data_t * connector_ptr;
    rci_service_buffer_t input;
    rci_service_buffer_t output;
} rci_service_data_t;

typedef struct
{
    uint8_t * start;
    uint8_t * end;
    uint8_t * current;
} rci_buffer_t;

typedef enum
{
    rci_parser_state_input,
    rci_parser_state_output,
    rci_parser_state_traverse,
    rci_parser_state_error
} rci_parser_state_t;


typedef enum
{
    rci_input_state_command_id,
    rci_input_state_command_attribute,
    rci_input_state_command_normal_attribute_id,
    rci_input_state_command_normal_attribute_value,
    rci_input_state_group_id,
    rci_input_state_group_attribute,
    rci_input_state_field_id,
    rci_input_state_field_type,
    rci_input_state_field_no_value,
    rci_input_state_field_value,
#if (defined RCI_LEGACY_COMMANDS)
    rci_input_state_do_command_payload,
#endif
    rci_input_state_done
} rci_input_state_t;

typedef enum
{
    rci_output_state_command_id,
    rci_output_state_command_normal_attribute_count,
    rci_output_state_command_normal_attribute_id,
    rci_output_state_command_normal_attribute_value,
    rci_output_state_group_id,
    rci_output_state_group_attribute,
    rci_output_state_field_id,
    rci_output_state_field_value,
    rci_output_state_field_terminator,
    rci_output_state_group_terminator,
#if (defined RCI_LEGACY_COMMANDS)
    rci_output_state_do_command_payload,
#endif
    rci_output_state_response_done,
    rci_output_state_done
} rci_output_state_t;

typedef enum
{
    rci_traverse_state_none,
    rci_traverse_state_command_id,
    rci_traverse_state_group_id,
    rci_traverse_state_element_id,
    rci_traverse_state_element_end,
    rci_traverse_state_group_end,
    rci_traverse_state_all_groups,
    rci_traverse_state_all_group_instances,
    rci_traverse_state_all_elements
#if (defined RCI_LEGACY_COMMANDS)
    ,rci_traverse_state_command_do_command,
    rci_traverse_state_command_reboot,
    rci_traverse_state_command_set_factory_default
#endif
} rci_traverse_state_t;

typedef enum
{
    rci_traverse_process_group,
    rci_traverse_process_element,
    rci_traverse_process_next_instance
} rci_traverse_process_state_t;

typedef enum
{
    rci_error_state_id,
    rci_error_state_description,
    rci_error_state_hint,
    rci_error_state_callback
} rci_error_state_t;

typedef struct
{
    uint8_t * data;
    size_t length;
} rcistr_t;

typedef struct
{
    rcistr_t name;
    rcistr_t value;
} rci_attribute_t;

typedef struct rci
{
    rci_service_data_t * service_data;
    rci_status_t status;
    struct {
        connector_request_id_t request;
        connector_callback_status_t status;
    } callback;

    struct {
        rci_buffer_t input;
        rci_buffer_t output;
    } buffer;

    struct {
        rci_parser_state_t state;
    } parser;

    struct {
        rci_command_t command_id;
        unsigned int attribute_count;
        unsigned int attributes_processed;

#if (defined RCI_LEGACY_COMMANDS)
#define MAX_ATTRIBUTES MAX_VALUE((unsigned int)rci_query_setting_attribute_id_count, (unsigned int)rci_do_command_attribute_id_count)
#else
#define MAX_ATTRIBUTES rci_query_setting_attribute_id_count
#endif

        struct 
        {
            rci_command_attribute_t id;
            rci_command_attribute_type_t type;
            union
            {
                uint32_t enum_val; 
#if (defined RCI_LEGACY_COMMANDS)
                char string_val[RCI_COMMANDS_ATTRIBUTE_MAX_LEN + 1];
#endif
            } value;
        } attribute[MAX_ATTRIBUTES];

#if (defined RCI_LEGACY_COMMANDS)
        struct 
        {
            connector_element_value_t response_value;
        } do_command;
#endif
    } command;

    struct {
        rci_traverse_state_t state;
        rci_traverse_process_state_t process_state;
    } traverse;

    struct {
        rci_input_state_t state;
        unsigned int flag;
        uint8_t * destination;
#if (defined CONNECTOR_NO_MALLOC)
        uint8_t storage[CONNECTOR_NO_MALLOC_RCI_MAXIMUM_CONTENT_LENGTH + sizeof nul + sizeof(uint32_t)];
#else
        uint8_t * storage;
        size_t storage_len;
#endif
    } input;

    struct {
        rcistr_t content;
        rci_output_state_t state;
        connector_bool_t group_skip;
        connector_bool_t element_skip;
    } output;

    struct {
        rci_error_state_t state;
        connector_bool_t command_error;
        char const * description;
    } error;

    struct {
        rcistr_t content;

        struct {
            unsigned int id;
            unsigned int index;
        } group;

        struct {
            unsigned int id;
        } element;

        connector_element_value_t value;
        size_t string_value_length;

        connector_remote_config_t callback_data;
    } shared;
} rci_t;

#define set_rci_input_flag(rci, value)     (rci)->input.flag |= (value)
#define is_rci_input_flag(rci, value)      (((rci)->input.flag & (value)) == (value))
#define clea_rci_input_flag(rci, value)    (rci)->input.flag &= ~(value)

#define set_rci_input_state(rci, value)     (rci)->input.state = (value)
#define get_rci_input_state(rci)            (rci)->input.state

#define set_rci_traverse_state(rci, value)     (rci)->traverse.state = (value)
#define get_rci_traverse_state(rci)            (rci)->traverse.state

#define set_rci_output_state(rci, value)     (rci)->output.state = (value)
#define get_rci_output_state(rci)            (rci)->output.state

#define set_rci_error_state(rci, value)     (rci)->error.state = (value);
#define get_rci_error_state(rci)            (rci)->error.state;

#define set_rci_command_error(rci)          (rci)->error.command_error = connector_true;
#define clear_rci_command_error(rci)        (rci)->error.command_error = connector_false;

STATIC connector_bool_t destination_in_storage(rci_t const * const rci);
STATIC void reset_input_content(rci_t * const rci);
