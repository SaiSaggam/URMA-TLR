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
#include "config_mgr.h"
#include "config_table.h"
#include "cli_tree.h"
#include "tlr_common.h"
#include "strlcpy.h"

#include <string.h>
#include <search.h>
#include <inttypes.h>

#undef TREE_DEBUG

#ifdef TREE_DEBUG
#include <stdio.h>
#define tree_debug(x...)     do { printf("%s: ", __func__); printf (x); } while (0)

#else
#define tree_debug(x...)
#endif

#define TABLE_config_debug  tree_debug
#define tree_error  tree_debug

#ifdef UNIT_TESTING
void ipm_call_free(void * ptr);
void config_tree_free(void * ptr);
void * config_tree_malloc(size_t size);
void * config_tree_strdup(void * const ptr);
void config_tree_free_strdup(void * ptr);

#else
#define config_tree_free_strdup free
#define config_tree_strdup   strdup
#define config_tree_free     free
#define config_tree_malloc   malloc
#define ipm_call_free            free
#endif

#define CONFIG_IMP_WALK_NEST_COUNT   5

typedef struct {
    void * context;
    config_tree_walk_tree_callback_t callback;
    char name[CONFIG_MGR_NAME_LENGTH];
    //config_tree_semaphore_t semaphore;
} config_tree_walk_t;


typedef struct {
    //ipm_handle_t handle;
    //char const * ifname;
    char const * user;
    config_mgr_access_level_t user_access;

    //config_tree_semaphore_t semaphore;

    void * search_tree[cli_tree_type_COUNT];
    size_t search_tree_count[cli_tree_type_COUNT];
    /* XXX - What's the extra 1 for? */
    config_tree_walk_t walk[cli_tree_data_type_COUNT + 1];

    char * data_buffer;

    struct {
        size_t action_id;
        size_t group_id;
        size_t group_instance;
        size_t count;
        union
        {
            config_tree_group_callback_t group;
            config_tree_action_callback_t action;
        } callback;

        void * context;
        bool action;
    } get;


    struct {
        char data_value[INTEGER_STRING_LENGTH];
        config_mgr_element_type_t type;
        union {
            char const * string_value;
            uint32_t unsigned_integer_value;
            int32_t signed_integer_value;
            unsigned int enum_index;
            size_t counter32;
            int32_t index;
            bool    boolean_value;
            bool    on_off;
        } value;
    } toValue;

    struct {
        cli_tree_search_t * action;
        cli_tree_search_t * command;
        cli_tree_search_t * element;
        cli_tree_search_t * element_enum;
        config_mgr_table_type_t table_type;
    } tree;

} config_tree_t;

config_tree_t * configTree = NULL;

#ifdef UNIT_TESTING
size_t malloc_count = 0;
size_t strdup_count = 0;
size_t ipm_free_count = 0;

void ipm_call_free(void * ptr)
{
    ++ipm_free_count;
    free(ptr);
}

void config_tree_free(void * ptr)
{
    --malloc_count;
    tree_debug("tree_free %p <<<<\n", ptr);
    free(ptr);
}

void * config_tree_malloc(size_t size)
{
    void * ptr = malloc(size);
    if (ptr != NULL)
        ++malloc_count;
    tree_debug("tree_malloc %p >>>>\n", ptr);
    return ptr;
}

void * config_tree_strdup(void * const ptr)
{
    void * rptr = strdup(ptr);

    if (rptr != NULL)
        ++strdup_count;

    return rptr;
}
void config_tree_free_strdup(void * ptr)
{
    --strdup_count;
    free(ptr);
}
#endif

/**
 * Command compare function for the Command table search functions
 */
static int tree_compare_cb(const void * search1, const void * search2)
{
    cli_tree_search_t const * tree1 = search1;
    cli_tree_search_t const * tree2 = search2;


    if ((tree1->type == cli_tree_data_type_enum) && (tree2->type == cli_tree_data_type_enum))
    {
        /* no alphabetical order. order by the id */
        int value = 0;
        cli_tree_data_t const * const data_tree1 = tree1->data_tree;
        config_mgr_element_enum_t * const enum_value1 = data_tree1->data;

        cli_tree_data_t const * const data_tree2 = tree2->data_tree;
        config_mgr_element_enum_t * const enum_value2 = data_tree2->data;

        //tree_debug("enum %s %s %d\n", tree1->name, tree1->name, (enum_value1->id - enum_value2->id));

        value = (int)(enum_value1->id - enum_value2->id);
        return value;

    }

    if (tree1->name && tree2->name)
    {

        //tree_debug("%s %s %d\n", tree1->name, tree2->name, strcmp (tree1->name, tree2->name));
        return (strcmp (tree1->name, tree2->name));
    }
    else if (tree2->name)
        return 1;

    return -1;
}

cli_tree_search_t * insert_search_tree(void ** tree, char const * name, void * const data_tree, cli_tree_data_type_t const type, __compar_fn_t __compar)
{

    cli_tree_search_t * add;

    {
        void * ptr;
        ptr = config_tree_malloc(sizeof(*add));
        add = ptr;
    }


    if (add != NULL)
    {
        //printf("insert_search_tree: %p %s %p\n", *tree, name, add);
        add->name = name;
        add->data_tree = data_tree;
        add->search_tree = NULL;
        add->type = type;
        add->max_name_length = 0;
        tsearch (add, tree, __compar);
    }
    else
    {
        tree_debug("config_tree_malloc failed to allocate search tree\n");
    }

    return add;

}

static cli_tree_search_t * find_tree(char const * name, void * const * root_tree, __compar_fn_t __compar)
{
    cli_tree_search_t * tree = NULL;
    void * search_tree;
    cli_tree_search_t req_tree;

    req_tree.name = name;

    search_tree = tfind(&req_tree, root_tree, __compar);
    if (search_tree != NULL) tree = *(cli_tree_search_t **)search_tree;

    return tree;
}

static void * add_group_tree(config_mgr_group_t const * group, config_mgr_table_type_t table_type)
{
    void * result = NULL;

    cli_tree_data_type_t data_type;
    cli_tree_data_t * tree;
	void ** search_tree = NULL;


    {
        void * ptr;

        ptr = config_tree_malloc(sizeof *tree);
        if (ptr == NULL)
        {
            cli_debug("config_tree_malloc failed to allocate command\n");
            goto done;
        }
        tree = ptr;
    }
	tree->name = group->name;
    tree->data = (void *)group;

	switch (table_type)
	{
	case config_mgr_table_type_show:
		search_tree = &configTree->search_tree[cli_tree_type_show];
		configTree->search_tree_count[cli_tree_type_show]++;
		data_type = cli_tree_data_type_group;
		TABLE_config_debug ("insert search tree for STATE configuration: %s\n", group->name);
//            configTree->tree.command = insert_search_tree(&configTree->search_tree[config_tree_type_state], group->name, tree, cli_tree_data_type_group, tree_compare_cb);
		break;
	case config_mgr_table_type_config:
		search_tree = &configTree->search_tree[cli_tree_type_command];
		configTree->search_tree_count[cli_tree_type_command]++;
        data_type = cli_tree_data_type_group;
		TABLE_config_debug ("insert search tree for SETTING configuration: %s\n", group->name);
//            configTree->tree.command = insert_search_tree(&configTree->search_tree[cli_tree_type_command], group->name, tree, cli_tree_data_type_group, tree_compare_cb);
		break;

	case config_mgr_table_type_action:
        TABLE_config_debug ("action is not added to the group tree: %s\n", group->name);
        free (tree);
		goto done;

	}

    tree->tree_data_type = data_type;

    configTree->tree.command = insert_search_tree(search_tree, tree->name, tree, data_type, tree_compare_cb);

    SET_MAX(configTree->tree.command->max_name_length, strlen(tree->name));
    SET_MAX(configTree->tree.command->max_name_length, MIN_NAME_LENGTH);

    tree->search_tree = configTree->tree.command;
    result = tree->search_tree;

done:
    return result;
}

static void * add_action_tree(config_mgr_action_t const * const action, cli_tree_data_type_t const tree_type)
{
    void * result = NULL;

    //cli_tree_data_type_t data_type;
    cli_tree_data_t * tree;
    void ** search_tree;


    {
        void * ptr;

        ptr = config_tree_malloc(sizeof *tree);
        if (ptr == NULL)
        {
            cli_debug("config_tree_malloc failed to allocate command\n");
            goto done;
        }
        tree = ptr;
    }
    tree->name = action->name;
    tree->data = (void *)action;
    tree->tree_data_type = tree_type; //cli_tree_data_type_action;

    search_tree = &configTree->search_tree[cli_tree_type_command];
    configTree->search_tree_count[cli_tree_type_command]++;

    configTree->tree.command = configTree->tree.action = insert_search_tree(search_tree, tree->name, tree, cli_tree_data_type_action, tree_compare_cb);

    SET_MAX(configTree->tree.command->max_name_length, strlen(tree->name));
    SET_MAX(configTree->tree.command->max_name_length, MIN_NAME_LENGTH);

    tree->search_tree = configTree->tree.command;
    result = tree->search_tree;

    TABLE_config_debug ("insert search tree for STATE configuration under ACTION: %s\n", tree->name);

    done:
    return result;
}

static void * add_subtree(void * group_tree, char const * const name, void * data, cli_tree_data_type_t const data_type)
{
    void * result = NULL;

    cli_tree_search_t * group_search_tree = group_tree;
    cli_tree_data_t * tree;

    if (group_tree == NULL)
    {
        tree_debug ("No Group tree\n");
        goto done;
    }

    {

        void * ptr;

        ptr = config_tree_malloc(sizeof *tree);
        if (ptr == NULL)
        {
            cli_debug("config_tree_malloc failed to allocate command\n");
            goto done;
        }
        tree = ptr;
    }

    tree->name = name;
    tree->data = data;
    tree->tree_data_type = data_type;

    SET_MAX(group_search_tree->max_name_length, strlen(name));
    SET_MAX(group_search_tree->max_name_length, MIN_NAME_LENGTH);

    {
        cli_tree_search_t * search_tree = insert_search_tree(&group_search_tree->search_tree, tree->name, tree, data_type, tree_compare_cb);

        if (search_tree == NULL)
        {
            goto done;
        }

        switch (data_type)
        {
        case cli_tree_data_type_element:
            configTree->tree.element = search_tree;
            break;
        case cli_tree_data_type_action_element:
            configTree->tree.element = search_tree;
            break;
        case cli_tree_data_type_enum:
            configTree->tree.element = search_tree;
            break;
        case cli_tree_data_type_group:
            configTree->tree.command = search_tree;
            break;
        case cli_tree_data_type_action:
        case cli_tree_data_type_show:
            configTree->tree.action = search_tree;
            break;
        case cli_tree_data_type_COUNT:
            break;
        }

        tree->search_tree = search_tree;
        result = tree->search_tree;
    }
    TABLE_config_debug("insert search tree element: %s\n", tree->name);

done:
    return result;
}


static void free_search_tree(cli_tree_search_t * const search_tree)
{

    if (search_tree == NULL) goto done;

    switch (search_tree->type)
    {
    case cli_tree_data_type_show:
    case cli_tree_data_type_action:
    case cli_tree_data_type_group:
    case cli_tree_data_type_action_element:
        tree_debug("free_search_tree: name = %s\n", search_tree->name);
        break;

    case cli_tree_data_type_element:
    	{
        cli_tree_data_t * data_tree = search_tree->data_tree;
        config_mgr_element_t * element = data_tree->data;

        switch (element->type)
        {
            case config_mgr_element_type_enum_multi:
            case config_mgr_element_type_enum:
            case config_mgr_element_type_string:
            case config_mgr_element_type_password:
            case config_mgr_element_type_ipaddr:
            case config_mgr_element_type_fqdn:
            case config_mgr_element_type_mac_addr:
            case config_mgr_element_type_uint32:
            case config_mgr_element_type_int32:
            case config_mgr_element_type_index:
            case config_mgr_element_type_on_off:
            case config_mgr_element_type_boolean:
            case config_mgr_element_type_counter32:
            case config_mgr_element_type_counter64:
            case config_mgr_element_type_interface:
            case config_mgr_element_type_encpassword:
            case config_mgr_element_type_none:
                break;
		}

        tree_debug("free_search_tree: element name = %s\n", element->name);
        }

        break;

    case cli_tree_data_type_enum:
        tree_debug("free_search_tree: enum name = %s\n", search_tree->name);
        break;

    default:
        tree_debug("unknown type %d\n", search_tree->type);
        break;
    }

    if (search_tree->data_tree != NULL)
        config_tree_free(search_tree->data_tree);


done:
    return;
}

// XXX why do we have this #def?
void free_search_tree_cb(void * node)
{
    if (node != NULL)
    {
        cli_tree_search_t * tree = node;

        if (tree->search_tree != NULL)
            tdestroy(tree->search_tree, free_search_tree_cb);

        free_search_tree(tree);
        config_tree_free(tree);
    }

}

static void walk_tree_cb(cli_tree_data_type_t const tree_data_type, const void *node, const VISIT which, const int depth)
{
    cli_tree_search_t const * search_tree = *(cli_tree_search_t **)node;
    config_tree_walk_t * const walk = &configTree->walk[tree_data_type];

    if (search_tree != NULL)
    {
        if ((which == postorder) || (which == leaf))
        {
           if (strncmp(search_tree->name, walk->name, strlen(walk->name)) ==  0)
            {
               config_tree_walk_tree_callback_t callback = walk->callback;

                if (strlen(walk->name) == 0)
                {
                    callback(search_tree->type, NULL, search_tree->data_tree, walk->context);
                }
                else
                {
                    callback(search_tree->type, walk->name, search_tree->data_tree, walk->context);
                }
            }
        }

        if (((which == endorder) || (which == leaf)) && (depth == 0))
        {
            config_tree_walk_tree_callback_t callback = walk->callback;
            if (strlen(walk->name) == 0)
            {
                callback(tree_data_type, NULL, NULL, walk->context);
            }
            else
            {
               config_tree_walk_tree_callback_t callback = walk->callback;
               callback(tree_data_type, walk->name, NULL, walk->context);
            }
        }
    }
}

static void walk_tree_group_cb(const void *node, const VISIT which, const int depth)
{
    walk_tree_cb(cli_tree_data_type_group, node, which, depth);
}
static void walk_tree_element_cb(const void *node, const VISIT which, const int depth)
{
    walk_tree_cb(cli_tree_data_type_element, node, which, depth);
}
static void walk_tree_enum_cb(const void *node, const VISIT which, const int depth)
{
    walk_tree_cb(cli_tree_data_type_enum, node, which, depth);
}
static void walk_tree_action_cb(const void *node, const VISIT which, const int depth)
{
    walk_tree_cb(cli_tree_data_type_action, node, which, depth);
}
static void walk_tree_show_cb(const void *node, const VISIT which, const int depth)
{
    walk_tree_cb(cli_tree_data_type_show, node, which, depth);
}
static void walk_tree_parameter_cb(const void *node, const VISIT which, const int depth)
{
    walk_tree_cb(cli_tree_data_type_action_element, node, which, depth);
}
static void walk_tree_command_cb(const void *node, const VISIT which, const int depth)
{
    walk_tree_cb(cli_tree_data_type_COUNT, node, which, depth);
}

bool walk_tree(char const * name, cli_tree_data_type_t const tree_data_type, void * search_tree,
               config_tree_walk_tree_callback_t callback, void * const context)
{
    bool error = false;
    __action_fn_t __action_cb[] = {
        walk_tree_group_cb, walk_tree_element_cb, walk_tree_enum_cb, walk_tree_action_cb, walk_tree_show_cb, walk_tree_parameter_cb, walk_tree_command_cb
    };

    if (search_tree != NULL)
    {
        config_tree_walk_t * const walk = &configTree->walk[tree_data_type];

        if (name != NULL)
        {
            strlcpy(walk->name, name, sizeof walk->name);
        }
        else
        {
            walk->name[0] = '\0';
        }

        walk->context = context;
        walk->callback = callback;

        twalk(search_tree, __action_cb[tree_data_type]);
    }

    return error;
}

bool valid_access(config_mgr_access_level_t const user_access_level, config_mgr_access_t const access)
{
    bool is_access = false;

    switch (user_access_level)
    {
        case config_mgr_access_level_super:
            if (access != config_mgr_access_no_access)
                is_access = true;
            break;
        case config_mgr_access_level_read_write:
            if ((access != config_mgr_access_no_access) &&
                (access != config_mgr_access_readonly_super_readwrite))
                is_access = true;
            break;
        case config_mgr_access_level_read_only:
            if ((access == config_mgr_access_readonly) ||
                (access == config_mgr_access_readwrite))
                is_access = true;
            break;
        default:
            break;
    }

    return is_access;
}


static int add_group_table(void * tree, config_mgr_table_type_t table_type)
{

    int result = 0;

    config_mgr_group_table_t const * const table = tlr_get_table(table_type);

    if (table != NULL)
    {
        size_t i;

        for (i=0; i < table->count; i++)
        {
            size_t e;
            config_mgr_group_t const * group = &table->groups[i];

            //fprintf(stderr, "%s user access %d access %d\n", group->name, configTree->user_access, group->access);

            if (valid_access(configTree->user_access, group->access))
            {
                void * group_tree;

                if (tree == NULL)
                    group_tree = add_group_tree(group, table_type);
                else
                    group_tree = add_subtree(tree, group->name, (void *)group, cli_tree_data_type_group);

                if (group_tree == NULL)
                {
                    TABLE_config_debug("cli_walk_init: unable to add group %zd id to the tree\n", i);
                    result = -1;
                    goto done;
                }

                SET_MAX(screen_length.name_length, strlen(group->name));
                SET_MAX(screen_length.desc_length, strlen(group->description));

                for (e=0; e < group->elements.count; e++)
                {
                    config_mgr_element_t const * element = &group->elements.element[e];

                    //printf("--> %s user access %d access %d\n", element->name, configTree->user_access, element->access);

                    if (valid_access(configTree->user_access, element->access))
                    {
                        void * element_tree = add_subtree(group_tree, element->name, (void *)element, cli_tree_data_type_element);

                        if (element_tree == NULL)
                        {
                            TABLE_config_debug("cli_walk_init: unable to add element %zd id to the tree\n", e);
                            result = -1;
                            goto done;
                        }

                        SET_MAX(screen_length.element_length, strlen(element->name));
                        //SET_MAX(screen_length.name_length, strlen(element->name));
                        SET_MAX(screen_length.desc_length, strlen(element->description));

                        if ((element->type == config_mgr_element_type_enum) ||
                            (element->type == config_mgr_element_type_enum_multi))
                        {
                            size_t v;
                            config_mgr_element_enum_limit_t const * enum_limit = element->value_limit;

                            for (v=0; v < enum_limit->count; v++)
                            {
                                config_mgr_element_enum_t const * enum_value = &enum_limit->value[v];

                                if (valid_access(configTree->user_access, enum_value->access))
                                {
                                    void * enum_tree = add_subtree(element_tree, enum_value->name, (void *)enum_value, cli_tree_data_type_enum);

                                    if (enum_tree == NULL)
                                    {

                                        TABLE_config_debug("cli_walk_init: unable to add enum %zd id to the tree\n", v);
                                        result = -1;
                                        goto done;
                                    }

                                    SET_MAX(screen_length.value_length, strlen(enum_value->description));

                                }
                            }
                        }

                        {
                            cli_tree_search_t * element_search_tree = element_tree;

                            TABLE_config_debug("add_group_table: ELEMENT %s %zu\n",
                                                element_search_tree->name,
                                                element_search_tree->max_name_length);
                            element_search_tree->max_name_length += 2;

                        }
                    }
                }

                {
                    cli_tree_search_t * group_search_tree = group_tree;

                    TABLE_config_debug("add_group_table: GROUP %s %zu\n",
                                        group_search_tree->name,
                                        group_search_tree->max_name_length);
                    group_search_tree->max_name_length += 2;
                }
            }
        }
    }

done:
    return result;
}

static int add_action_table(void * tree)
{

    int result = 0;
    config_mgr_action_table_t const * const table = tlr_get_table(config_mgr_table_type_action);

    if (table != NULL)
    {

        size_t i;

        for (i=0; i < table->count; i++)
        {

            size_t e;
            config_mgr_action_t const * action = &table->actions[i];

            if (valid_access(configTree->user_access, action->access))
            {


                void * action_tree;

                if (tree == NULL)
                    action_tree = add_action_tree(action, cli_tree_data_type_action);
                else
                    action_tree = add_subtree(tree, action->name, (void *)action, cli_tree_data_type_action);

                if (action_tree == NULL)
                {
                    TABLE_config_debug("cli_walk_init: unable to add action %zd id to the tree\n", i);
                    result = -1;
                    goto done;
                }

                SET_MAX(screen_length.name_length, strlen(action->name));
                SET_MAX(screen_length.desc_length, strlen(action->description));

                for (e=0; e < action->elements.count; e++)
                {
                    config_mgr_action_element_t const * action_element = &action->elements.element[e];
                    //void * element_tree = add_element_tree(group_tree, element);
                    if (valid_access(configTree->user_access, action_element->access))
                    {
                        void * action_element_tree = add_subtree(action_tree, action_element->name, (void *)action_element, cli_tree_data_type_action_element);

                        if (action_element_tree == NULL)
                        {

                            TABLE_config_debug("cli_walk_init: unable to add element %zd id to the tree\n", e);
                            result = -1;
                            goto done;
                        }

                        SET_MAX(screen_length.element_length, strlen(action_element->name));
                        SET_MAX(screen_length.desc_length, strlen(action_element->description));

                        switch (action_element->type)
                        {
                        case config_mgr_action_group:
                            break;
                        case config_mgr_action_element:
                            {
                                config_mgr_element_t const * element = action_element->element;

                                if ((element->type == config_mgr_element_type_enum)
                                    || (element->type == config_mgr_element_type_enum_multi))
                                {
                                    size_t v;
                                    config_mgr_element_enum_limit_t const * enum_limit = element->value_limit;

                                    for (v=0; v < enum_limit->count; v++)
                                    {
                                        config_mgr_element_enum_t const * enum_value = &enum_limit->value[v];
                                        //void * enum_tree = add_enum_tree(element_tree, enum_value);
                                        void * enum_tree = add_subtree(action_element_tree, enum_value->name, (void *)enum_value, cli_tree_data_type_enum);
                                        if (enum_tree == NULL)
                                        {

                                            TABLE_config_debug("cli_walk_init: unable to add enum %zd id to the tree\n", v);
                                            result = -1;
                                            goto done;
                                        }

                                        SET_MAX(screen_length.value_length, strlen(enum_value->description));

                                    }

                                }


                                break;
                            }
                        case config_mgr_action_parameter:
                            {
                                config_mgr_action_parameter_t const * parameter = action_element->element;

                                if ((parameter->type == config_mgr_element_type_enum)
                                    || (parameter->type == config_mgr_element_type_enum_multi))
                                {
                                    size_t v;
                                    config_mgr_element_enum_limit_t const * enum_limit = parameter->value_limit;

                                    for (v=0; v < enum_limit->count; v++)
                                    {
                                        config_mgr_element_enum_t const * enum_value = &enum_limit->value[v];
                                        //void * enum_tree = add_enum_tree(element_tree, enum_value);
                                        void * enum_tree = add_subtree(action_element_tree, enum_value->name, (void *)enum_value, cli_tree_data_type_enum);
                                        if (enum_tree == NULL)
                                        {

                                            TABLE_config_debug("cli_walk_init: unable to add enum %zd id to the tree\n", v);
                                            result = -1;
                                            goto done;
                                        }

                                        SET_MAX(screen_length.value_length, strlen(enum_value->description));

                                    }

                                }


                                break;
                            }
                        }

                        {
                            cli_tree_search_t * element_search_tree = action_element_tree;

                            TABLE_config_debug("add_action_table: ELEMENT %s %zu\n",
                                                element_search_tree->name,
                                                element_search_tree->max_name_length);

                            element_search_tree->max_name_length += 2;
                        }
                    }
                }
                {
                    cli_tree_search_t * action_search_tree = action_tree;

                    TABLE_config_debug("add_action_table: %s %zu\n",
                                        action_search_tree->name,
                                        action_search_tree->max_name_length);

                    action_search_tree->max_name_length += 2;
                }
            }
        }
    }
done:
    return result;
}

int cli_tree_init(config_mgr_access_level_t const access_level)
{
    int result = 0;

    if (configTree != NULL) {
        if (configTree->user_access != access_level)
            cli_tree_destroy();
        else
            goto done; /* already initialized */
    }

    {
        size_t i;
        void * ptr;
        ptr = config_tree_malloc(sizeof *configTree);
        configTree = ptr;

        if (ptr == NULL) {
            result = -1;
            goto done;
        }
        configTree->user_access = access_level;
        //configTree->semaphore = CONFIG_SEMAPHORE_INVALID;
        configTree->data_buffer = NULL;
        //configTree->tree.action = NULL;
        //configTree->tree.action_element = NULL;
        configTree->tree.command = NULL;
        configTree->tree.element = NULL;
        configTree->tree.element_enum = NULL;

        for (i=0; i < config_mgr_asizeof(sizeof configTree->search_tree); i++)
        {
            configTree->search_tree[i] = NULL;
            configTree->search_tree_count[i] = 0;
        }
    }

    if (add_group_table(NULL, config_mgr_table_type_config) != 0) goto done;


    if (add_action_table(NULL) != 0) goto done;



    if (tlr_get_table(config_mgr_table_type_show) != NULL)
    {
        static config_mgr_action_t const show_command = {
                .name = "show",
                .id = (size_t)-1,
                .description = "Show status and statistics",
                .access = config_mgr_access_readonly,
        };
        void * action_tree = add_action_tree(&show_command, cli_tree_data_type_show);

        if (action_tree == NULL)
        {
            TABLE_config_debug("cli_walk_init: unable to add show command to the tree\n");
            result = -1;
            goto done;
        }

        if (add_group_table(action_tree, config_mgr_table_type_show) != 0) goto done;
    }


done:
    if (result != config_tree_success)
    {
        cli_tree_destroy();
    }

    return result;
}

int cli_tree_destroy(void)
{
    config_tree_status_t result = config_tree_success;
    size_t i;

    if (configTree == NULL) {
        /* already initialized */
        goto done;
    }

    configTree->tree.command = NULL;
    configTree->tree.element = NULL;
    configTree->tree.action = NULL;
    configTree->tree.element_enum = NULL;

    for (i=0; i < config_mgr_asizeof(configTree->search_tree); i++)
    {
        if (configTree->search_tree[i] != NULL)
        {
            tdestroy(configTree->search_tree[i], free_search_tree_cb);
        }
    }

    for (i=0; i < config_mgr_asizeof(sizeof configTree->search_tree); i++)
    {
        configTree->search_tree[i] = NULL;
        configTree->search_tree_count[i] = 0;
    }

    {
        void * ptr = configTree;

        configTree = NULL;
        config_tree_free(ptr);
    }

#ifdef UNIT_TESTING
    tree_debug("malloc count = %zu\n", malloc_count);
    tree_debug("strdup_count = %zu\n", strdup_count);
    tree_debug("ipm_free_count = %zu\n", ipm_free_count);
#endif

done:
    return result;
}

int cli_tree_walk(cli_tree_type_t const tree_type, char const * const name,
                    config_tree_walk_tree_callback_t const tree_callback, void * const context)
{
    int result = 0;

    if (configTree == NULL)
    {
        cli_debug("ConfigTree not initialized\n");
        result = -1;
        goto done;
    }
    switch (tree_type)
    {
        case cli_tree_type_command:
        case cli_tree_type_show:
            if (tree_callback != NULL)
            {
                if (!walk_tree(name, cli_tree_data_type_COUNT, configTree->search_tree[tree_type], tree_callback, context))
                    break;
            }
            /* no break */
        default:
            cli_debug("Invalid type %d\n", tree_type);
            result = -1;
            break;
    }

done:
    return result;

}

int cli_tree_walk_subtree(void * search_tree, char const * name, config_tree_walk_tree_callback_t tree_callback, void * const context)
{
    int result = -1;
    cli_tree_search_t * const tree = search_tree;

    if (configTree == NULL)
    {
        cli_debug("ConfigTree not initialized\n");
        goto done;
    }

    if (tree != NULL && tree_callback != NULL)
    {
        if (!walk_tree(name, tree->type, tree->search_tree, tree_callback, context))
        {
            result = 0;
        }
    }
    else
    {
        cli_debug("Invalid parameter\n");
    }

done:
    return result;
}

int cli_tree_find(cli_tree_type_t tree_type, char const * name, void ** data, cli_tree_data_type_t * const data_type)
{
    int result = 0;

    if (configTree == NULL)
    {
        cli_debug("Config Tree not initialized\n");
        result = -1;
        goto done;
    }

    switch (tree_type)
    {
        case cli_tree_type_command:
        case cli_tree_type_show:
        if (data != NULL && data_type != NULL)
        {
            break;
        }
        /* no break */
        default:
            cli_debug("Invalid type %d or parameter\n", tree_type);
            result = -1;
            goto done;
    }

    {
        cli_tree_search_t const * search_tree = find_tree(name, &configTree->search_tree[tree_type], tree_compare_cb);

        if (search_tree != NULL)
        {
            *data = search_tree->data_tree;
            *data_type = search_tree->type;
        }
        else
        {
            *data = NULL;
        }
    }

done:
    return result;
}

int cli_tree_find_subtree(void * const search_tree, char const * name, void ** data, cli_tree_data_type_t * const data_type)
{
    int result = 0;

    if (search_tree == NULL || name == NULL || data == NULL || data_type == NULL)
    {
        cli_debug("Invalid parameters\n");
        result = -1;
        goto done;
    }

    {
        cli_tree_search_t * const tree = search_tree;
        cli_tree_search_t const * tree_found = find_tree(name, &tree->search_tree, tree_compare_cb);

        if (tree_found != NULL)
        {
            *data = tree_found->data_tree;
            *data_type = tree_found->type;
        }
        else
        {
            *data = NULL;
        }
    }

done:
    return result;
}
