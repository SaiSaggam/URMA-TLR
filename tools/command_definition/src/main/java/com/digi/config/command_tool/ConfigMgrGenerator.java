package com.digi.config.command_tool;

import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.LinkedList;
import java.util.regex.Pattern;

/**
 * Interface between the parser and the generator modules It generates source
 * and header files for Config Manager.
 *
 */
public class ConfigMgrGenerator implements GeneratorInterface {

	private static final String CONFIG_DIGI_COPYRIGHT = "/*****************************************************************************\n"
														+ "* Copyright (c) 2016 Digi International Inc., All Rights Reserved\n"
														+ "*\n"
														+ "* This software contains proprietary and confidential information of Digi\n"
														+ "* International Inc.  By accepting transfer of this copy, Recipient agrees\n"
														+ "* to retain this software in confidence, to prevent disclosure to others,\n"
														+ "* and to make no use of this software other than that for which it was\n"
														+ "* delivered.  This is an unpublished copyrighted work of Digi International\n"
														+ "* Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly\n"
														+ "* prohibited.\n"
														+ "*\n"
														+ "* Restricted Rights Legend\n"
														+ "*\n"
														+ "* Use, duplication, or disclosure by the Government is subject to\n"
														+ "* restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in\n"
														+ "* Technical Data and Computer Software clause at DFARS 252.227-7031 or\n"
														+ "* subparagraphs (c)(1) and (2) of the Commercial Computer Software -\n"
														+ "* Restricted Rights at 48 CFR 52.227-19, as applicable.\n"
														+ "*\n"
														+ "* Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343\n"
														+ "*\n"
														+ "****************************************************************************/\n\n";

	private static final String CONST_STRING = "const";
	private static final String CONFIG_MGR_VALID_FILE = "config_valid.c";
	private static final String CONFIG_MGR_CUSTOM_VALID_HEADER_FILE = "custom_valid.h";
	private static final String CONFIG_MGR_TABLE_FILE = "config_table.c";
	private static final String CONFIG_MGR_HEADER_FILE = "config_table.h";
	private static final String CONFIG_MGR_ENUM_HEADER_FILE = "config_def.h";
	private static final String PLATFORM_HEADER_FILE = "platform.h";

	private static final String CONFIG_MGR_STRING_HEADER_FILE = "<string.h>";
	private static final String CONFIG_MGR_INET_HEADER_FILE = "<arpa/inet.h>";
	private static final String CONFIG_MGR_CTYPE_HEADER_FILE = "<ctype.h>";

	private static final String CONFIG_MGR_STD_HEADER = "#include <stddef.h>\n"
													 + "#include <stdbool.h>\n"
													 + "#include <stdint.h>\n"
													 + "#include \"" + CONFIG_MGR_ENUM_HEADER_FILE + "\"\n\n";

	private static final String CONFIG_MGR_ASIZEOF = "#define config_mgr_asizeof(array)  (sizeof array/sizeof array[0])\n\n";
	private static final String CONFIG_MGR_CONFIG_TYPE = "typedef enum {\n"
														+ " config_mgr_set_group,\n"
														+ " config_mgr_get_group,\n"
														+ " config_mgr_group_type_COUNT\n"
														+ "} config_mgr_group_type_t;\n\n";

	private static final String ELEMENT_LIMIT_T = "config_mgr_element_limit_t ";
	private static final String ELEMENT_ENUM_T = "config_mgr_element_enum_t";
	private static final String ELEMENT_ENUM_LIMIT_T = "config_mgr_element_enum_limit_t";
	private static final String ELEMENT_INT32_LIMIT_T = "config_mgr_element_signed_integer_limit_t";
	private static final String ELEMENT_UINT32_LIMIT_T = "config_mgr_element_unsigned_integer_limit_t";
	private static final String ELEMENT_STRING_LIMIT_T = "config_mgr_element_string_limit_t";
	private static final String ELEMENT_T = "config_mgr_element_t";
	private static final String GROUP_T = "config_mgr_group_t";
	private static final String GROUP_TABLE_T = "config_mgr_group_table_t";
	private static final String ACCESS_T = "config_mgr_access_t";
	private static final String ELEMENT_TYPE_T = "config_mgr_element_type_t";
	private static final String VALIDATION_T = "config_mgr_valid_function_t";
	private static final String GROUP_TABLE_VAR = "config_mgr_group_table";

	private static final String TAG_TABLE_T = "config_mgr_tag_table_t";
	private static final String TAG_T = "config_mgr_tag_t";

	private static final String TAG_TABLE_VAR = "config_mgr_tag_table";
	private static final String MAX_TAG_VAR = "config_max_tags";
	private static final String CONFIG_TAG_VAR = "config_tags";

	private static final String SET_GROUP_ENUM_TYPE_T = "config_group_t";
	private static final String GET_GROUP_ENUM_TYPE_T = "config_state_group_t";
	private static final String ACTION_ENUM_TYPE_T = "config_action_t";

	private static final String CONFIG_MGR = "config_mgr";
//	private static final String SHOW_ACTION = "show";

	private static final String ACTION_ELEMENT_TYPE_T = "config_mgr_action_type_t";

	private static final String ACTION_ELEMENT_TYPE = "typedef enum {\n"
														+ " config_mgr_action_group,\n"
														+ " config_mgr_action_element,\n"
														+ " config_mgr_action_parameter\n"
														+ "} " + ACTION_ELEMENT_TYPE_T + ";\n\n";
	private static final String ACTION_PARAMETER_T = "config_mgr_action_parameter_t";

	private static final String ACTION_ELEMENT_T = "config_mgr_action_element_t";

	private static final String ACTION_TABLE_T = "config_mgr_action_table_t";
	private static final String ACTION_T = "config_mgr_action_t";
	private static final String ACTION_VAR = "config_mgr_actions";
	private static final String ACTION_TABLE_VAR = "config_mgr_action_table";
	private static final String DEFAULT_T = "config_mgr_default_t";

	private static final String PLATFORM_TABLE_T = "config_platform_table_t";
	private static final String PLATFORM_TABLE_VAR = "config_platform_table";

	private static final String INTERFACE_VAR = "config_mgr_interfaces";
	private static final String INTERFACE_COUNT_VAR = "config_mgr_interface_count";

	private static final String VALID_FUNCTION_UINT32 = "bool %s (uint32_t const value, uint32_t const min, uint32_t const max)";
	private static final String VALID_FUNCTION_INT32 = "bool %s (int32_t const value, int32_t const min, int32_t const max)";
	private static final String VALID_FUNCTION_STRING = "bool %s (char const * const string, size_t const min_length, size_t const max_length)";
	private static final String VALID_FUNCTION_FQDN = "bool %s (char const * const fqdn)";
	private static final String VALID_FUNCTION_IPADDR = "bool %s (char const * const ipaddr)";
	private static final String VALID_FUNCTION_MAC_ADDR = "bool %s(char const * const mac_addr)";
	private static final String VALID_FUNCTION_ENUM = "bool %s(char const * const string, " + ELEMENT_ENUM_LIMIT_T + " const * const enum_limit)";
	private static final String VALID_FUNCTION_ON_OFF = "bool %s(char const * const string)";
	private static final String VALID_FUNCTION_BOOLEAN = "bool %s(char const * const string)";
	private static final String VALID_FUNCTION_INTERFACE = "bool %s(char const * const interface)";

	private static final String VALID_FUNCTION_UINT32_TYPEDEF = "(* config_mgr_valid_uint32_t)";
	private static final String VALID_FUNCTION_INT32_TYPEDEF = "(* config_mgr_valid_int32_t)";
	private static final String VALID_FUNCTION_STRING_TYPEDEF = "(* config_mgr_valid_string_t)";
	private static final String VALID_FUNCTION_FQDN_TYPEDEF = "(* config_mgr_valid_fqdn_t)";
	private static final String VALID_FUNCTION_IPADDR_TYPEDEF = "(* config_mgr_valid_ipaddr_t)";
	private static final String VALID_FUNCTION_MAC_ADDR_TYPEDEF = "(* config_mgr_valid_mac_addr_t)";
	private static final String VALID_FUNCTION_ENUM_TYPEDEF = "(* config_mgr_valid_enum_t)";
	private static final String VALID_FUNCTION_ENUM_MULTI_TYPEDEF = "(* config_mgr_valid_enum_multi_t)";
	private static final String VALID_FUNCTION_ON_OFF_TYPEDEF = "(* config_mgr_valid_on_off_t)";
	private static final String VALID_FUNCTION_BOOLEAN_TYPEDEF = "(* config_mgr_valid_boolean_t)";
	private static final String VALID_FUNCTION_INTERFACE_TYPEDEF = "(* config_mgr_valid_interface_t)";

	private static final String VALID_FUNCTION_INTEGER_CONTENT = "{\n"
											+ "	bool validated = true;\n\n"
											+ "	if (value < min || value > max)\n"
											+ "		validated = false;\n"
											+ "	return validated;\n"
											+ "}\n\n";

	private static final String VALID_FUNCTION_STRING_CONTENT = "{\n"
											+ "	bool validated = true;\n\n"
											+ "	if (strlen(string) < min_length || strlen(string) > max_length)\n"
											+ "		validated = false;\n"
											+ "	return validated;\n"
											+ "}\n\n";

	private static final String VALID_FUNCTION_FQDN_CONTENT = "{\n"
											+ "	bool validated = true;\n\n"
											+ "	if (strlen(fqdn) > 255)\n"
											+ "		validated = false;\n"
											+ "	return validated;\n"
											+ "}\n\n";

	private static final String VALID_FUNCTION_IPADDR_CONTENT = "{\n"
											+ "	struct sockaddr_in sa;\n"
											+ "	int ok = 1;\n\n"
											+ "	if (strlen(ipaddr) > 0)\n"
											+ "		ok  = inet_pton(AF_INET, ipaddr, &(sa.sin_addr));\n"
/* TODO: IPv6 not supported yet
											+ "	if (!ok) {\n"
											+ "		struct in6_addr in6;\n"
											+ "		ok = inet_pton(AF_INET6, ipaddr, &in6);\n"
											+ "	}\n"
*/
											+ "	return ok != 0;\n"
											+ "}\n\n";

	private static final String VALID_FUNCTION_MAC_ADDR_CONTENT = "{\n"
											+ "	char const * addr = mac_addr;\n"
											+ "	int i = 0;\n"
											+ "	int s = 0;\n"

											+ "	while (*addr) {\n"
											+ "		if (isxdigit(*addr)) {\n"
											+ "			i++;\n"
											+ "		}\n"
											+ "		else if ((*addr == ':') || (*addr == '-')) {\n"
											+ "			if ((i == 0) || ((i/2 -1) != s))\n"
											+ "				break;\n"
											+ "			++s;\n"
											+ "		}\n"
											+ "		else\n"
											+ "			s = -1;\n"
											+ "		addr++;\n"
											+ "	}\n"
											+ "	return ((i == 12) && ((s == 5) || (s == 0)));\n"
											+ "}\n\n";

	private static final String VALID_FUNCTION_ENUM_CONTENT = "{\n"
											+ "	bool validated = false;\n"
											+ "	" + ELEMENT_ENUM_T + " const * enum_value = enum_limit->value;\n"
											+ "	size_t i;\n"

											+ "	for (i=0; i < enum_limit->count; i++) {\n"
											+ "		if (enum_value[i].access !=" + String.format(" %s", toAccessString(AccessType.NO_ACCESS)) + " ||\n"
											+ "			strcmp(string, enum_value[i].name) == 0) {\n"
											+ "			validated = true;\n"
											+ "			break;\n"
											+ "		}\n"
											+ "	}\n"
											+ "	return validated;\n"
											+ "}\n\n";


	private static final String VALID_FUNCTION_ON_OFF_CONTENT = "{\n"
											+ "	static char const on_string[] = \"on\";\n"
											+ "	static char const off_string[] = \"off\";\n\n"
											+ "	bool validated = false;\n"

											+ "	if ((strcmp(string, on_string) == 0) ||\n"
											+ "		(strcmp(string, off_string) == 0))\n"
											+ "		validated = true;\n"
											+ "	return validated;\n"
											+ "}\n\n";

	private static final String VALID_FUNCTION_BOOLEAN_CONTENT = "{\n"
											+ "	static char const true_string[] = \"true\";\n"
											+ "	static char const false_string[] = \"false\";\n\n"
											+ "	bool validated = false;\n"

											+ "	if ((strcmp(string, true_string) == 0) ||\n"
											+ "		(strcmp(string, false_string) == 0))\n"
											+ "		validated = true;\n"
											+ "	return validated;\n"
											+ "}\n\n";

	private static final String VALID_FUNCTION_INTERFACE_CONTENT = "{\n"
			+ "	extern char * * " + INTERFACE_VAR + ";\n\n"
			+ "	extern size_t " + INTERFACE_COUNT_VAR + ";\n\n"
			+ "	bool validated = false;\n"
			+ "	size_t i;\n"

			+ "	for (i=0; i < " + INTERFACE_COUNT_VAR + "; i++) {\n"
			+ "		if (strcmp(interface, " + INTERFACE_VAR + "[i]) == 0) {\n"
			+ "			validated = true;\n"
			+ "			break;\n"
			+ "		}\n"
			+ "	}\n"
			+ "	return validated;\n"
			+ "}\n\n";

	public enum ValidationType {
		STRING(VALID_FUNCTION_STRING,   VALID_FUNCTION_STRING_TYPEDEF, null),
		PASSWORD(VALID_FUNCTION_STRING, null,  STRING),
		UINT32(VALID_FUNCTION_UINT32,   VALID_FUNCTION_UINT32_TYPEDEF, null),
		INT32(VALID_FUNCTION_INT32,	 VALID_FUNCTION_INT32_TYPEDEF,	   null),
		ENUM(VALID_FUNCTION_ENUM,	   VALID_FUNCTION_ENUM_TYPEDEF,		null),
		ON_OFF(VALID_FUNCTION_ON_OFF,   VALID_FUNCTION_ON_OFF_TYPEDEF,	  null),
		BOOLEAN(VALID_FUNCTION_BOOLEAN, VALID_FUNCTION_BOOLEAN_TYPEDEF,	 null),
		IPADDR(VALID_FUNCTION_IPADDR,   VALID_FUNCTION_IPADDR_TYPEDEF,	  null),
		FQDN(VALID_FUNCTION_FQDN,	   VALID_FUNCTION_FQDN_TYPEDEF,		null),
		MAC_ADDR(VALID_FUNCTION_MAC_ADDR, VALID_FUNCTION_MAC_ADDR_TYPEDEF,  null),
		COUNTER32(null, null, null),
		COUNTER64(null, null, null),
		INDEX(null,	 null, INT32),
		INTERFACE(VALID_FUNCTION_INTERFACE, VALID_FUNCTION_INTERFACE_TYPEDEF, null),
		ENCPASSWORD(VALID_FUNCTION_STRING, null,  STRING),
		ENUM_MULTI(VALID_FUNCTION_ENUM,   VALID_FUNCTION_ENUM_MULTI_TYPEDEF, null),
		NONE(null, null,  null);

		private String prototype;
		private String typedef;
		private ValidationType matched_type;
		private Boolean default_function_set;
		private Boolean default_prototype_set;

		private ValidationType(String prototype, String typedef, ValidationType type) {
			this.prototype = prototype;
			this.typedef = typedef;
			this.matched_type = type;
			this.default_function_set = false;
			this.default_prototype_set = false;
		}

		public String toFunction(String function_name) {
				String function = String.format(prototype, function_name);
				return function;
		}

		public String toDefaultFunctionName() {
			String name = null;

			if (prototype != null) {
				name = "validate_";

				if (matched_type == null) {
					name += toString().toLowerCase();
				}
				else {
					name += matched_type.toString().toLowerCase();
				}
			}
			return name;
		}


		public String toDefaultFunction() {
			String function = null;

			if ((prototype != null) && (default_function_set == false)) {
				String name = toDefaultFunctionName();

				function = toFunction(name) + "\n";

				switch (this)
				{
				case STRING:
					function += VALID_FUNCTION_STRING_CONTENT;
					break;
				case PASSWORD:
					break;
				case UINT32:
				case INT32:
					function += VALID_FUNCTION_INTEGER_CONTENT;
					break;
				case ENUM:
					function += toEnumValidateFunction();
					break;
				case ON_OFF:
					function += VALID_FUNCTION_ON_OFF_CONTENT;
					break;
				case BOOLEAN:
					function += VALID_FUNCTION_BOOLEAN_CONTENT;
					break;
				case IPADDR:
					function += VALID_FUNCTION_IPADDR_CONTENT;
					break;
				case FQDN:
					function += VALID_FUNCTION_FQDN_CONTENT;
					break;
				case MAC_ADDR:
					function += VALID_FUNCTION_MAC_ADDR_CONTENT;
					break;
				case COUNTER32:
				case COUNTER64:
				case INDEX:
					break;
				case INTERFACE:
					function += VALID_FUNCTION_INTERFACE_CONTENT;
					break;

				case ENCPASSWORD:
					break;

				case ENUM_MULTI:
					function += toEnumMultiValidateFunction();
					break;
				case NONE:
					break;
				}
				default_function_set = true;

				for (ValidationType type: ValidationType.values()) {
					if (type.matched_type == this) {
						type.default_function_set = true;
					}
				}
			}
			return function;
		}

		public Boolean getDefaultFunctionSet() {
			return default_function_set;
		}

		public void setDefaultPrototype(boolean set) {
			default_prototype_set = set;
		}

		public Boolean isDefaultPrototypeSet() {
			return default_prototype_set;
		}


	}


	private BufferedWriter bufferedWriter;
	private String prefix;

	private String interfaceName = "Config Manager Generator";

	private boolean include_platform_header = false;
	private boolean enable_to_tag_define = false;
	private LinkedList<ConfigurationTag> platforms;
	private LinkedList<ConfigurationTag> tags;

	/**
	 * Method for command tool option to trigger this interface
	 * @return Name	 Name of the interface
	 */
	public String getOption() {
		return "config_mgr";
	}

	public String[] getGeneratorOption() {
		String[] options = new String[0];
		return options;
	}
	
	public boolean setGeneratorOption(String options) {
		return false;
	}

	/**
	 * Method for the name of the interface.
	 * @return Name	 Name of the interface
	 */
	public String getName() {
		return interfaceName;
	}

	/**
	 * Method used to generate output files.
	 * @param groups Linked list of groups that are used to create the output files
	 * @param actions Linked list of actions
	 * @param outputDirectory Where to place the generated files
	 * @return True	 The output files were successfully created.
	 * @return False	An error was encounter.
	 */
	public boolean GenerateOutputFile(LinkedList<ConfigurationGroup> groups, LinkedList<ConfigurationTag> platforms, String outputDirectory) {

		boolean isOk = true;

		CommandTool.debug_log("Start " + interfaceName);

		//if (tags.size() > 0)
			//include_platform_header = true;


		try {
			LinkedList<ConfigurationTag> tags = new LinkedList<ConfigurationTag>();

			if (platforms.size() > 0) {
				for (ConfigurationTag platform : platforms) {
					for (ConfigurationTag define : platform.getDefines()) {
						boolean new_tag = true;

						for (ConfigurationTag tag : tags) {
							if (tag.getName().equals(define.getName())) {
								new_tag = false;
								break;
							}
						}

						if (new_tag == true) {
							tags.add(define);
						}
					}
				}
			}
			this.platforms = platforms;
			this.tags = tags;
			OutputEnumHeaderFile(groups, outputDirectory);
			OutputTableSourceFile(groups, outputDirectory);
			OutputHeaderFile(groups, outputDirectory);
			OutputValidateSourceFile(groups, outputDirectory);

		} catch (IOException e) {
			CommandTool.log(e.getMessage());
			isOk = false;
		}
		return isOk;
	}

	private void OutputValidateSourceFile(LinkedList<ConfigurationGroup> groups, String outputDirectory) throws IOException {

		String filename;

		if (outputDirectory != null)
			filename = outputDirectory + "/" + CONFIG_MGR_VALID_FILE;
		else
			filename = CONFIG_MGR_VALID_FILE;

		CommandTool.debug_log("Generating " + filename + " file");

		try {

			bufferedWriter = new BufferedWriter(new FileWriter(filename));

			try {
				writeHeaderComment();
				String string = String.format("\n#include \"%s\"\n", CONFIG_MGR_HEADER_FILE)
								+ String.format("#include \"%s\"\n", CONFIG_MGR_CUSTOM_VALID_HEADER_FILE);
				boolean includeStringHeader = true;

				/* include headers */
				for (ElementType type: ElementType.values()) {
					switch (type)
					{
					case STRING:
					case ENCPASSWORD:
					case PASSWORD:
					case ON_OFF:
					case BOOLEAN:
					case FQDN:
					case ENUM:
					case ENUM_MULTI:
						if (includeStringHeader && type.isSet()) {
							string += String.format("#include %s\n", CONFIG_MGR_STRING_HEADER_FILE);
							includeStringHeader = false;
						}
						break;
					case UINT32:
					case INT32:
					case INDEX:
						break;

					case IPADDR:
						if (type.isSet())
							string += String.format("#include %s\n", CONFIG_MGR_INET_HEADER_FILE);
						break;
					case MAC_ADDR:
						if (type.isSet())
							string += String.format("#include %s\n", CONFIG_MGR_CTYPE_HEADER_FILE);
						break;
					case COUNTER32:
					case COUNTER64:
						break;
					case INTERFACE:
					case NONE:
						break;
					}
				}
				writeFile(string);
				string = "\n";

				/* default validation functions */
				for (ElementType type: ElementType.values()) {
					ValidationType validationType = ValidationType.valueOf(type.toString());

					Boolean write_default_function = false;

					for (ConfigurationGroup group: groups) {
						if (group.getConfigType() != ConfigType.GET_GROUP) {
							for (ConfigurationElement element: group.getElements()) {
								if (element.getType() == type) {
									write_default_function = type.isSet() && (validationType.prototype != null);
									break;
								}
							}
							if (write_default_function) break;
						}
					}

					CommandTool.debug_log(String.format("validate write function: %s %s", type.toString(), write_default_function.toString()));

					if (!type.isSet() && validationType.matched_type != null) {
						CommandTool.debug_log(String.format("validate write function: match_type %s",
								validationType.matched_type.toString()));

						validationType = validationType.matched_type;
						ElementType t = ElementType.valueOf(validationType.toString());
						write_default_function = t.isSet() && (validationType.prototype != null);
					}
					if (!validationType.getDefaultFunctionSet() && write_default_function) {
						string += validationType.toDefaultFunction();
					}
				}
				writeFile(string);

				string = "\n";
				/* user's validation functions */
				for (ConfigType type: ConfigType.values()) {
					prefix = type.toString().toLowerCase();

					for (ConfigurationGroup group: groups) {

						ConfigType group_type = group.getConfigType();

						if (group_type == type) {
							String group_name = group.getName();
							for (ConfigurationElement element: group.getElements()) {

								string = "\n";
								ConfigError error = element.getError();
								ValidationType validationType = ValidationType.valueOf(element.getType().toString());

								if ((error != null) && (validationType.prototype != null)) {
									string += String.format("#if !defined %s\n", toValidationFunction(prefix, group_name, element.getName()).toUpperCase())
											+ validationType.toFunction(toValidationFunction(prefix, group_name, element.getName()))
											+ "\n{\n"
											+ "	bool validated = validate_" + validationType.toString().toLowerCase();

									switch (element.getType())
									{
									case STRING:
									case PASSWORD:
									case ENCPASSWORD:
										string += "(string, min_length, max_length);\n";
										break;
									case ON_OFF:
									case BOOLEAN:
										string += "(string);\n";
										break;
									case FQDN:
										string += "(fqdn);\n";
										break;

									case ENUM:
									case ENUM_MULTI:
										string += "(string, enum_limit);\n";
										break;
									case UINT32:
									case INT32:
									case INDEX:
										string += "(value, min, max);\n";
										break;

									case IPADDR:
										string += "(ipaddr);\n";
										break;
									case MAC_ADDR:
										string += "(mac_addr);\n";
										break;
									case INTERFACE:
										string += "(interface);\n";
										break;
									case COUNTER32:
									case COUNTER64:
									case NONE:
										break;
									}
									string += "	return validated;\n}\n"
											+ "#endif\n\n";
									writeFile(string);
								}
							}
						}
					}

				}

				if (outputDirectory != null)
					CommandTool.log(String.format("%s is generated in %s directory.\n", CONFIG_MGR_VALID_FILE, outputDirectory));
				else
					CommandTool.log(String.format("%s is generated.\n", CONFIG_MGR_VALID_FILE));

			}

			finally {
				bufferedWriter.close();
			}
		} catch (FileNotFoundException e) {
			CommandTool.log("Error generating " + e.getMessage());

		} catch (IOException e) {
			CommandTool.log("Unable to create a file from ConfigManager: " + filename);
		}
	}

	private void OutputTableSourceFile(LinkedList<ConfigurationGroup> groups, String outputDirectory) throws IOException {

		String filename;

		if (outputDirectory != null)
			filename = outputDirectory + "/" + CONFIG_MGR_TABLE_FILE;
		else
			filename = CONFIG_MGR_TABLE_FILE;

		CommandTool.debug_log("Generating " + filename + " file");

		try {

			bufferedWriter = new BufferedWriter(new FileWriter(filename));

			try {
				writeHeaderComment();
				String string = String.format("\n#include \"%s\"", CONFIG_MGR_HEADER_FILE)
							  + String.format("\n#include \"%s\"\n\n", CONFIG_MGR_ENUM_HEADER_FILE);

				if (include_platform_header)
					string += String.format("#include \"%s\"\n\n", PLATFORM_HEADER_FILE);

				if (ConfigType.getCount(ConfigType.ACTION) > 0)
					string += "\n#define NO_ERROR		 NULL\n"
							+ "#define NO_DEFAULT	   NULL\n"
							+ "#define NO_LIMIT		 NULL\n"
							+ "#define NO_VALIDATION	NULL\n\n";

				writeFile(string);

				/* TODO: write supported interface */
				if (ElementType.INTERFACE.isSet()) {
					string = "char * * " + INTERFACE_VAR + " = NULL;\n\n"
						   + "size_t " + INTERFACE_COUNT_VAR + " = 0;\n\n";
					writeFile(string);
				}

				boolean[] isGroupNotEmpty = new boolean[ConfigType.values().length];
				int group_write = 0;
				int action_write = 0;
				for (ConfigType type: ConfigType.values()) {

					prefix = type.toString().toLowerCase();
					string = String.format("\n/*  %s  */\n", prefix);
					writeFile(string);

					int i = type.getIndex();

					/* write configuration for the type*/
					isGroupNotEmpty[i] = (writeConfigurations(type, groups) > 0);

					switch (type) {
					case SET_GROUP:
					case GET_GROUP:
						group_write++;
						break;
					case ACTION:
						action_write++;
					}

					if (group_write == 2 || action_write == 2) {
						writeGroupTable(type,  groups, isGroupNotEmpty);
						if (group_write == 2) group_write = 0;
						if (action_write == 2) action_write = 0;
					}
				}

				writeTags(groups);

				if (platforms.size() > 0) {
					int platform_count = 0;
					for (ConfigurationTag platform : platforms) {
						if (platform.isPlatformType()) {
							platform_count++;
						}
					}

					string = String.format("%s %s %s[] = {\n", PLATFORM_TABLE_T, CONST_STRING, PLATFORM_TABLE_VAR);
					if (platform_count > 0) {
						int count = 0;
						for (ConfigurationTag platform : platforms) {

							if (platform.isPlatformType()) {
								if (count > 0) string += "\n  ,\n";
								string += String.format("  {\"%s\", platform_%s}", platform.getDescription(), toNormalizeString(platform.getName()));
								count++;
							}
						}
					} else {
						string += "  {\"\", 0}";
						
					}
					string += "\n};\n\n";
					writeFile(string);
				}


				if (outputDirectory != null)
					CommandTool.log(String.format("%s is generated in %s directory.\n", CONFIG_MGR_TABLE_FILE, outputDirectory));
				else
					CommandTool.log(String.format("%s is generated.\n", CONFIG_MGR_TABLE_FILE));
			}

			finally {
				bufferedWriter.close();
			}

		} catch (FileNotFoundException e) {
			CommandTool.log("Error generating " + e.getMessage());

		} catch (IOException e) {
			CommandTool.log("Unable to create a file from ConfigManager: " + filename);
		}
	}

	private void OutputHeaderFile(LinkedList<ConfigurationGroup> groups, String outputDirectory) throws IOException {

		String filename;

		if (outputDirectory != null)
			filename = outputDirectory + "/" + CONFIG_MGR_HEADER_FILE;
		else
			filename = CONFIG_MGR_HEADER_FILE;

		CommandTool.debug_log("Generating " + filename + " file");

		try {

			bufferedWriter = new BufferedWriter(new FileWriter(filename));

			try {
				writeHeaderComment();
				String string = String.format("#ifndef %s\n", CONFIG_MGR_HEADER_FILE.replace('.', '_'))
								+ String.format("#define %s\n\n", CONFIG_MGR_HEADER_FILE.replace('.', '_'))
								+ CONFIG_MGR_STD_HEADER + CONFIG_MGR_ASIZEOF
								+ "#define CONFIG_MGR_NAME_LENGTH " + Parser.MAX_NAME_LENGTH + "\n\n"
								+  CONFIG_MGR_CONFIG_TYPE;
				writeFile(string);

				writeElementTypedef();
				writeAccessTypedef();
				writeStructureTypedef();

				writePlatformTagsHeader(platforms, tags);

				writeTagStructureTypedef(groups);

				writeActionStructureTypedef();

				string = "typedef struct {\n" +
						"  char * name;\n" +
						"  platform_type_t type;\n" +
						" } " + PLATFORM_TABLE_T + ";\n\n";

				string += String.format("extern %s %s %s[];\n\n", PLATFORM_TABLE_T, CONST_STRING, PLATFORM_TABLE_VAR);
				writeFile(string);

				writeValidationFunctionPrototypes(groups);

				string = String.format("\n#endif /* %s */\n", CONFIG_MGR_HEADER_FILE.replace('.', '_'));
				writeFile(string);


				if (outputDirectory != null)
					CommandTool.log(String.format("%s is generated in %s directory.\n", CONFIG_MGR_HEADER_FILE, outputDirectory));
				else
					CommandTool.log(String.format("%s is generated.\n", CONFIG_MGR_HEADER_FILE));
			}

			finally {
				bufferedWriter.close();
			}

		} catch (FileNotFoundException e) {
			CommandTool.log("Error generating " + e.getMessage());

		} catch (IOException e) {
			CommandTool.log("Unable to create a file from configManager: " + filename);
		}
	}

	private void OutputEnumHeaderFile(LinkedList<ConfigurationGroup> groups, String outputDirectory) throws IOException {

		String filename;

		if (outputDirectory != null)
			filename = outputDirectory + "/" + CONFIG_MGR_ENUM_HEADER_FILE;
		else
			filename = CONFIG_MGR_ENUM_HEADER_FILE;

		CommandTool.debug_log("Generating " + filename + " file");

		try {

			bufferedWriter = new BufferedWriter(new FileWriter(filename));

			try {
				writeHeaderComment();
				String string = String.format("#ifndef %s\n", CONFIG_MGR_ENUM_HEADER_FILE.replace('.', '_'))
								+ String.format("#define %s\n\n", CONFIG_MGR_ENUM_HEADER_FILE.replace('.', '_'));

				int count = 0;
				
				string += "typedef enum {\n";
				
				for (ConfigurationTag platform : platforms) {
					if (platform.isPlatformType()) {
						count++;
						string += String.format("  platform_%s,\n", toNormalizeString(platform.getName()));

					}
				}

				string += "  platform_COUNT" + ((count == 0) ? " = 1\n" : "\n") 
						+ "} platform_type_t;\n\n";

				/* set group enum */
				string += "typedef enum {\n";
				count = 0;
				for (ConfigurationGroup group: groups) {
					switch (group.getConfigType())
					{
					case GET_GROUP:
					case ACTION:
						break;
					case SET_GROUP:
						string += String.format(" %s", toGroupEnumName(group.getConfigType(),group.getName()));
						count++;
						if (count < ConfigType.getCount(group.getConfigType()))
							string += ",";
						string += "\n";
					}
				}
				if (count > 0) {
					string += String.format("} %s;\n\n", SET_GROUP_ENUM_TYPE_T);
					writeFile(string);
				}

				/* action enum */
				string = "typedef enum {\n";
				count = 0;
				for (ConfigurationGroup group: groups) {
					switch (group.getConfigType())
					{
					case GET_GROUP:
					case SET_GROUP:
						break;
					case ACTION:
						string += String.format(" %s", toGroupEnumName(group.getConfigType(), group.getName()));
						count++;
						if (count < ConfigType.getCount(group.getConfigType()))
							string += ",";
						string += "\n";
					}
				}
				if (count > 0) {
					string += String.format("} %s;\n\n", ACTION_ENUM_TYPE_T);
					writeFile(string);
				}

				/* get group enum */
				string = "typedef enum {\n";
				count = 0;
				for (ConfigurationGroup group: groups) {
					switch (group.getConfigType())
					{
					case SET_GROUP:
					case ACTION:
						break;
					case GET_GROUP:
						string += String.format(" %s", toGroupEnumName(group.getConfigType(), group.getName()));
						count++;
						if (count < ConfigType.getCount(group.getConfigType()))
							string += ",";
						string += "\n";
					}
				}
				if (count > 0) {
					string += String.format("} %s;\n\n", GET_GROUP_ENUM_TYPE_T);
					writeFile(string);
				}
				else {
					string = "typedef unsigned int " + GET_GROUP_ENUM_TYPE_T + ";\n\n";
					writeFile(string);
				}

				/* element enum */
				for (ConfigurationGroup group: groups) {
					if (group.getElements().size() > 0) {
						string = "typedef enum {\n";
						count = 0;
						for (ConfigurationElement element: group.getElements()) {
							string += String.format(" %s", toElementEnumName(group.getConfigType(), group.getName(), element.getName()));
							count++;
							if (count < group.getElements().size())
								string += ",";
							string += "\n";
						}
						string += String.format("} %s_t;\n\n", toElementEnumName(group.getConfigType(), group.getName()));
						writeFile(string);
					}
				}

				/* enum for enum type */
				for (ConfigurationGroup group: groups) {
					for (ConfigurationElement element: group.getElements()) {
						if (element.getType() == ElementType.ENUM || element.getType() == ElementType.ENUM_MULTI) {
							string = "typedef enum {\n";
							count = 0;
							for (ConfigValue value: element.getValues()) {
								string += String.format(" %s", toEnumValueName(group.getConfigType(), group.getName(),
																			   element.getName(), value.getName()));
								count++;
								if (count < element.getValues().size())
									string += ",";
								string += "\n";
							}
							string += String.format("} %s_t;\n\n", toEnumValueName(group.getConfigType(), group.getName(), element.getName()));
							writeFile(string);
						}
					}
				}

				writeTagEnum(groups);

				string = String.format("\n#endif /* %s */\n", CONFIG_MGR_ENUM_HEADER_FILE.replace('.', '_'));
				writeFile(string);

				if (outputDirectory != null)
					CommandTool.log(String.format("%s is generated in %s directory.\n", CONFIG_MGR_ENUM_HEADER_FILE, outputDirectory));
				else
					CommandTool.log(String.format("%s is generated.\n", CONFIG_MGR_ENUM_HEADER_FILE));
			}

			finally {
				bufferedWriter.close();
			}

		} catch (FileNotFoundException e) {
			CommandTool.log("Error generating " + e.getMessage());

		} catch (IOException e) {
			CommandTool.log("Unable to create a file from configManager: " + filename);
		}
	}

	private void writeValidationFunctionPrototypes(LinkedList<ConfigurationGroup> groups) throws IOException {
		String string = "";

		//int[] prototype = new int[ElementType.values().length];

		for (ConfigurationGroup group: groups) {

			switch (group.getConfigType())
			{
			case GET_GROUP:
				break;
			case SET_GROUP:
			case ACTION:
				/* validation function prototypes */
				for (ConfigurationElement element: group.getElements()) {
					ValidationType validationType = ValidationType.valueOf(element.getType().toString());

					CommandTool.debug_log("validate type: " +  element.getType().toString());

					if (element.getError() != null) {
						/* customized error function */
						String validationFunction = toValidationFunction(group.getConfigType().toString().toLowerCase(), group.getName(), element.getName());
						string += String.format("extern %s;\n", validationType.toFunction(validationFunction));

					} else if (!validationType.isDefaultPrototypeSet() && validationType.prototype != null) {
//						String validationFunction = "validate_" + validationType.toString().toLowerCase();
						String validationFunction = validationType.toDefaultFunctionName();
                                                string += String.format("#define HAS_%s_VALIDATION_FUNCTION\n", validationType.toString().toUpperCase());
						string += String.format("extern %s;\n", validationType.toFunction(validationFunction));
						validationType.setDefaultPrototype(true);

						/* skip duplicate validation type */
						for (ValidationType type : ValidationType.values()) {
							if (type.matched_type != null && type.matched_type == validationType) {
								type.setDefaultPrototype(true);
							}
						}
					}

				}
				break;

			}
		}
		writeFile(string);

	}

	private int writeConfigurations(ConfigType type, LinkedList<ConfigurationGroup> groups) throws IOException {

		int count = 0;
		for (ConfigurationGroup group: groups) {

			if (group.getConfigType() == type) {
				String group_name = group.getName();

				writeFile(toTagDefine(group.getTags()));

				for (ConfigurationElement element: group.getElements()) {

					writeElementEnumValue(group, element);
					writeElementLimit(group_name, element);
				}

				switch (type)
				{
				case SET_GROUP:
				case GET_GROUP:
					writeElements(group, group.getElements());
					break;
				case ACTION:
					writeParameters(group, group.getElements());
					break;
				}
				writeFile(toTagDefineClose(group.getTags()));

				count++;
			}
		}

		if (count > 0) {
			switch (type)
			{
			case SET_GROUP:
			case GET_GROUP:
				writeGroups(type, groups);
				break;
			case ACTION:
				writeAction(type, groups);
				break;
			}
		}
		else if (type == ConfigType.ACTION) {
			String string = String.format("%s %s[1] = {(NULL, 0}} ;\n\n", ACTION_TABLE_T, ACTION_TABLE_VAR);
			writeFile(string);
		}

		return count;
	}

	private void writeGroupTable(ConfigType group_type, LinkedList<ConfigurationGroup> groups, boolean[] isGroupNotEmpty) throws IOException {

		ConfigType[] types = new ConfigType[2];

		String table_struct=null;
		String table_var=null;

		switch (group_type) {
		case SET_GROUP:
		case GET_GROUP:
		   table_struct = GROUP_TABLE_T;
		   table_var = GROUP_TABLE_VAR;
		   types[0] = ConfigType.SET_GROUP;
		   types[1] = ConfigType.GET_GROUP;
		   break;
		default:
			return;
		}

		String string = String.format("%s %s %s[] = {\n", table_struct, CONST_STRING, table_var);
		int count = 0;


		for(ConfigType type: types){

			count++;
			string += " {\n";
			if (isGroupNotEmpty[type.getIndex()]) {
				prefix = type.toString().toLowerCase();
				string += String.format("   %s,\n", toGroupVariable(prefix))
						+ String.format("   config_mgr_asizeof(%s)\n", toGroupVariable(prefix));
			} else {
				string += "   NULL,\n"
						+ "   0\n";
			}

			string += " }";
			if (count < types.length)
				string += ",";
			string += "\n";

		}
		string += "};\n";
		writeFile(string);

	}

	private void writeTags(LinkedList<ConfigurationGroup> groups) throws IOException {

		writeMaxTags(groups, platforms);
		//writeGroupTags(groups);
		//writeElementTags(groups);
		//writeValueTags(groups);

		if (ConfigObject.getNumTagsTotal() > 0) {

			int count = 0;
			String string = String.format("static %s %s[] = {\n", TAG_T, CONFIG_TAG_VAR);
			writeFile(string);

			for (ConfigurationGroup group : groups) {
				ConfigType type = group.getConfigType();
				string = "";

				for (String group_tag: group.getTags()) {

					if (count > 0)
						string += " ,\n";
					count++;
					String var_name = toGroupVariable(type.toString().toLowerCase());
					CommandTool.debug_log(group.getName() + " " + var_name);

					string += " {\n"
							+ String.format("    %s,\n", toGroupEnumName(type, group.getName()))
							+ String.format("    \"%s\",\n", toNormalizeString(group_tag))
							+ String.format("    &%s[%s%s],\n", toGroupVariable(type.toString().toLowerCase()), toConfigTypeName(type), group.getName().replace('-', '_'));

					if (type == ConfigType.ACTION)
						string += "    config_mgr_tag_action,\n";
					else
						string += "    config_mgr_tag_group,\n";

					string += writePlatformTags(group_tag)
							+ "\n }\n";
				}

				LinkedList<ConfigurationElement> elements = group.getElements();
				for (ConfigurationElement element : elements) {
					/* element tag */
					for (String element_tag : element.getTags()) {
						if (count > 0)
							string += " ,\n";
						count++;
						string += " {\n"
								+ String.format("    %s,\n", toGroupEnumName(type, group.getName()))
								+ String.format("    \"%s\",\n", toNormalizeString(element_tag))
								+ String.format("    &%s[%s],\n",
										toGroupElementVariable(type.toString().toLowerCase(), group.getName()),
										toElementEnumName(type, group.getName(), element.getName()));

						if (type == ConfigType.ACTION)
							string += "    config_mgr_tag_action_element,\n";
						else
							string += "    config_mgr_tag_element,\n";

						string += writePlatformTags(element_tag)
								+ "\n }\n";
					}


					LinkedList<ConfigValue> values = element.getValues();
					for (ConfigValue value : values) {
						/* value tag */
						for (String value_tag : value.getTags()) {
							if (count > 0)
								string += " ,\n";
							count++;
							String enum_var = toElementEnumVariable(type.toString().toLowerCase(), group.getName(), element.getName());
							string += " {\n"
									+ String.format("    %s,\n", toGroupEnumName(type, group.getName()))
									+ String.format("    \"%s\",\n", toNormalizeString(value_tag))
									+ String.format("    &%s[%s],\n", enum_var, toEnumValueName(type, group.getName(), element.getName(), value.getName()))
									+ "    config_mgr_tag_value,\n"
									+ writePlatformTags(value_tag)
									+ "\n }\n";
						}
					}
				}
				writeFile(string);
			}
			string = "};\n\n";
			writeFile(string);
		}

		 String string = String.format("%s %s[] = {\n", TAG_TABLE_T, TAG_TABLE_VAR)
				 + " {\n";
		 if (ConfigurationGroup.getGroupHasMaxTagCount() > 0) {
			 string += String.format("   %s, \n", MAX_TAG_VAR)
					 + String.format("   config_mgr_asizeof(%s)\n", MAX_TAG_VAR);
		 } else {
			 string += "   NULL,\n"
					 + "   0\n";
		 }
		 string += " },\n {\n";

		 if (ConfigObject.getNumTagsTotal() > 0) {
			 string += String.format("   %s, \n", CONFIG_TAG_VAR)
					 + String.format("   config_mgr_asizeof(%s)\n", CONFIG_TAG_VAR);
		 } else {
			 string += "   NULL,\n"
					 + "   0\n";
		 }
		 string += " }\n};\n\n";
		 writeFile(string);
	}

	private String writePlatformTags(String tag_name) throws IOException
	{
		String string = "    {\n";
		int count = 0;

		for (ConfigurationTag platform : platforms) {
			//CommandTool.debug_log(String.format("platform %s\n", platform.getName()));

			if (platform.isPlatformType()) {

				if (count > 0)
					string += ",\n";

				if ((CommandTool.getTag(platform.getDefines(), tag_name) != null) &&
						(CommandTool.getTag(tags, tag_name) != null))
					string += "      " + String.format("PLATFORM_%s_%s", platform.getName().toUpperCase(),
							tag_name.toUpperCase());
				else
					string += "      0";

				count++;
			}
		}
		if (count == 0)
			string += "      0";

		string += "\n    }\n";

		return string;
	}

	private void writeMaxTags(LinkedList<ConfigurationGroup> groups, LinkedList<ConfigurationTag> platforms) throws IOException {

		int total_tags = ConfigurationGroup.getGroupHasMaxTagCount() +
						 ConfigurationElement.getElementHasMaxTagCount();

		if (total_tags > 0) {

			int count = 0;
			String string = String.format("static %s %s[] = {\n", TAG_T, MAX_TAG_VAR);
			for (ConfigurationGroup group : groups) {
				ConfigType type = group.getConfigType();

				String tag = group.getTagNumRow();

				if (tag != null) {

					count++;
					string += " {\n"
							+ String.format("   %s,\n", toGroupEnumName(type, group.getName()))
							+ String.format("   \"%s\",\n", toNormalizeString(tag))
							+ String.format("   &%s[%s%s],\n", toGroupVariable(type.toString().toLowerCase()), toConfigTypeName(type), group.getName())
							+ "   config_mgr_tag_group,\n";
					string += "   {\n";

					int i=0;
					for (ConfigurationTag platform : platforms) {
						if (platform.isPlatformType()) {
							if (i > 0)
								string += ",\n";

							if (CommandTool.getTag(tags, tag) != null)
								string += String.format("      PLATFORM_%s_%s", platform.getName().toUpperCase(), tag);
							else
								string += "      0";
							i++;
						}
					}
					if (i == 0)
						string += "      0";

					string += "\n   }\n"
							+ "\n }\n";

					if (count < total_tags)
						string += ",";

					string += "\n";
				}

				LinkedList<ConfigurationElement> elements = group.getElements();
				for (int n=0; n < elements.size(); n++) {
					ConfigurationElement element = elements.get(n);
					String max_tag = element.getMaxTag();
					if (max_tag != null) {
						count++;
						//String var_name = toElementMaxTagVariable(type.toString().toLowerCase(), group.getName(), element.getName());
						string += " {\n"
								+ String.format("   %s,\n", toGroupEnumName(type, group.getName()))
								+ String.format("   \"%s\",\n", toNormalizeString(max_tag))
								+ String.format("   &%s[%d],\n", toGroupElementVariable(type.toString().toLowerCase(), group.getName()), n)
								+ "   config_mgr_tag_element,\n"
								+ "   { NULL, 0 }\n }";

						if (count < total_tags)
							string += ",";

						string += "\n";
					}
				}
			}
			string += "};\n\n";
			writeFile(string);
		}

	}
	private void writeGroups(ConfigType type, LinkedList<ConfigurationGroup> groups) throws IOException {

		String string = String.format("static %s %s[] = {\n", GROUP_T, toGroupVariable(prefix));
		writeFile(string);

		int count = 0;
		for (ConfigurationGroup group: groups) {

			String group_name = group.getName();

			ConfigType group_type = group.getConfigType();

			if (group_type == type) {


				string = toTagDefine(group.getTags());

				//if (count < ConfigType.getCount(group_type))
				if (count != 0)
					string += " ,\n";

				count++;

				string += " {\n" + String.format("   \"%s\",\n", group_name);

				if (group.getUciName() != null)
					string += String.format("   \"%s\",\n", group.getUciName());
				else
					string += "   NULL,\n";

				String var_name = toGroupElementVariable(prefix, group_name);
				string += String.format("   \"%s\",\n", group.getDescription())
//						+ String.format("   %d,\n", (count -1))
						+ String.format("   %s,\n", toGroupEnumName(group.getConfigType(), group_name));

				if ((enable_to_tag_define == true) && (group.getTagNumRow() != null)){
					string += String.format("   %s,\n", group.getTagNumRow());

				} else if (group.getTagNumRow() != null) {
					int max_rows = group.getNumRows();
					for (ConfigurationTag platform : platforms) {

						for (ConfigurationTag define: platform.getDefines()) {
							if (define.getName().equals(group.getTagNumRow())) {
								//CommandTool.log(String.format("define %s = %d", define.getName(), define.getDefineValue()));
								if (max_rows < define.getDefineValue())
									max_rows = define.getDefineValue();
							}
						}
					}

					string += String.format("   %d,\n", max_rows);

				} else {
					string += String.format("   %d,\n", group.getNumRows());
				}

				string += String.format("   %s,\n", (group.isNoMib() == false)? "true" : "false");
				string += String.format("   %s,\n", (group.isMibIndexed() == true) ? "true" : "false");
				string += String.format("   %s,\n", toAccessString(group.getAccess()))
						+ "   {\n"
						+ String.format("   %s,\n", var_name)
						+ String.format("   config_mgr_asizeof(%s)\n", var_name)
						+ "   }\n"
						+ " }";

				string += "\n";
				string += toTagDefineClose(group.getTags());

				writeFile(string);


			}

		}
		string = "};\n";
		writeFile(string);

	}


	private void writeElements(ConfigurationGroup group, LinkedList<ConfigurationElement> elements) throws IOException {
		int count = 0;

		String string = String.format("\nstatic %s %s[] = {\n", ELEMENT_T, toGroupElementVariable(prefix, group.getName()));
		writeFile(string);

		for (ConfigurationElement element: elements) {
			String element_name = element.getName();
			ConfigError error = element.getError();

			string = toTagDefine(element.getTags());

			 if (count != 0)
				 string += " ,\n";

			 count++;
			/* element structure:
			 * name, description, access, type, limit, error
			 */
			string += " {\n"
					+ String.format("   \"%s\",\n", element_name)
					+ String.format("   \"%s\",\n", element.getDescription());

			if (error != null)
				string += String.format("   \"%s\",\n", error.getDescription());
			else
				string += "   NULL,\n";

			if (element.getDefaultValue() != null)
				string += String.format("   \"%s\",\n", element.getDefaultValue());
			else
				string += "   NULL,\n";

			string += String.format("   %s, \n", toAccessString(element.getAccess()))
					+ String.format("   %s,\n", toElementTypeString(element.getType()));

			/* value limit */
			if (element.getMin() != null || element.getMax() != null ||
				element.getType() == ElementType.ENUM || element.getType() == ElementType.ENUM_MULTI) {
				string += String.format("   &%s,\n", toElementLimitVariable(prefix, group.getName(), element_name));
			} else
				string += "   NULL,\n";

			/* get validation function */
			ValidationType validationType = ValidationType.valueOf(element.getType().toString());
			String validationFunction = null;

			if (error != null)
				validationFunction = toValidationFunction(prefix, group.getName(), element_name);
			else if (validationType.prototype != null && group.getConfigType() != ConfigType.GET_GROUP)
//				validationFunction = "validate_" + validationType.toString().toLowerCase();
				validationFunction = validationType.toDefaultFunctionName();
			else
				validationFunction = "NULL";

			string += String.format("   %s,\n", validationFunction)
					+ String.format("   %s,\n", toElementEnumName(group.getConfigType(), group.getName(), element_name));

			 string += " }\n";

			 string += toTagDefineClose(element.getTags());

			 writeFile(string);
		}

		string = "};\n\n";
		writeFile(string);
	}

	private void writeElementTypedef() throws IOException {
		String string = "typedef enum {\n";
		int count = 0;
		for (ElementType type: ElementType.values()) {
			string += String.format(" %s", toElementTypeString(type));
			count++;
			if (count < ElementType.values().length)
				string += ",";
			string += "\n";
		}
		string += String.format("} %s;\n\n", ELEMENT_TYPE_T);
		writeFile(string);
	}

	private void writeAccessTypedef() throws IOException {
		String string = "typedef enum {\n";
		int count = 0;
		for (AccessType access: AccessType.values()) {
			if (access != AccessType.NONE) {

				string += String.format(" %s", toAccessString(access));
				if (access == AccessType.NO_ACCESS)
					string += " = 0x80";
				count++;
				if (count < AccessType.values().length)
					string += ",";
				string += "\n";
			}
		}
		string += String.format("} %s;\n\n", ACCESS_T);
		writeFile(string);
	}

	private void writeStructureTypedef() throws IOException {

		String string;
		boolean writeString = true;
		boolean enumString = true;

		/* enum value and value limit */
		for (ElementType type: ElementType.values()) {
			switch (type) {
			case STRING:
			case FQDN:
			case ENCPASSWORD:
			case PASSWORD:
				if (writeString) {
					string = "typedef struct {\n"
									+ " size_t min_length_in_bytes;\n"
									+ " size_t max_length_in_bytes;\n"
									+ String.format("} %s;\n\n", ELEMENT_STRING_LIMIT_T);
					writeFile(string);
					writeString = false;
				}
				break;
			case ENUM:
			case ENUM_MULTI:
				if (enumString) {
					string = "typedef struct {\n"
									+ " char const * name;\n"
									+ " char const * description;\n"
									+ " unsigned int value;\n"
									+ " size_t id;\n"
									+ " bool multi_exclusive;\n"
									+ String.format(" %s access;\n", ACCESS_T)
									+ "} " + ELEMENT_ENUM_T + ";\n\n";
					string += "typedef struct {\n "
						+ ELEMENT_ENUM_T + " const * const value;\n"
						+ " size_t count;\n"
						+ String.format("} %s;\n\n", ELEMENT_ENUM_LIMIT_T);

					writeFile(string);
					enumString = false;
				}

				break;
			case INT32:
				string = "typedef struct {\n"
								+ " int32_t min_value;\n"
								+ " int32_t max_value;\n"
								+ String.format("} %s;\n\n", ELEMENT_INT32_LIMIT_T);
				writeFile(string);
				break;
			case UINT32:
				string = "typedef struct {\n"
								+ " uint32_t min_value;\n"
								+ " uint32_t max_value;\n"
								+ String.format("} %s;\n\n", ELEMENT_UINT32_LIMIT_T);
				writeFile(string);
				break;
			case INDEX:
				/* same as INT32 */
				break;
			default:
				break;
			}
		}

		/* void validation */
		string = String.format("typedef void * %s;\n\n", ELEMENT_LIMIT_T)
				+ "\n/* validation function typedef */\n";

		/* validation function prototype */
		for (ElementType type: ElementType.values()) {
			ValidationType validationType = ValidationType.valueOf(type.toString());

			try {
				if (validationType.typedef != null)
					string += String.format("typedef %s;\n", validationType.toFunction(validationType.typedef));
			} catch (Exception e) {
				throw new IOException("Invalid type for validation!");
			}
		}
		string += String.format("\ntypedef void * %s;\n\n",VALIDATION_T);;
		writeFile(string);


		/* void limit */
		string = "\n/* Element and Group structures */\n"
		/* element */
			+ "typedef struct {\n"
			+ " char const * name;\n"
			+ " char const * description;\n"
			+ " char const * error;\n"
			+ " char const * default_value;\n"
			+ String.format(" %s access;\n", ACCESS_T)
			+ String.format(" %s type;\n", ELEMENT_TYPE_T)
			+ String.format(" %s value_limit;\n", ELEMENT_LIMIT_T)
			+ String.format(" %s validation;\n", VALIDATION_T)
			+ " size_t id;\n"
			+ String.format("} %s;\n\n", ELEMENT_T)

		/* group */
		  + "typedef struct {\n"
		  + " char const * name;\n"
		  + " char const * filename;\n"
		  + " char const * description;\n"
		  + " size_t id;\n"
		  + " size_t instances;\n"
		  + " bool in_mib;\n"
		  + " bool mib_indexed;\n"
		  + String.format(" %s access; \n", ACCESS_T)

		  + " struct {\n"
		  + String.format("   %s * element;\n", ELEMENT_T)
		  + "   size_t count;\n"
		  + " } elements;\n"
		  + String.format("} %s;\n\n", GROUP_T)

		/* group table */
		  + "typedef struct {\n"
		  + String.format(" %s * groups;\n", GROUP_T)
		  + " size_t count;\n"
		  + String.format(" } %s;\n\n", GROUP_TABLE_T)

		  + String.format("extern %s %s %s[];\n\n", GROUP_TABLE_T, CONST_STRING, GROUP_TABLE_VAR);

		writeFile(string);
	}

	private void writeActionStructureTypedef() throws IOException {

		String string;

		/* void limit */
		string = "\n/* parameter and action structures */\n"
			   + String.format("\n#define CONFIG_MGR_MAX_ACTION_PARAMETER_COUNT  %d\n\n", Parser.getMaxActionParameters())
			   + ACTION_ELEMENT_TYPE
			   + String.format("typedef void * %s;\n\n", DEFAULT_T)

		/* action element parameter */
			+ "typedef struct {\n"
			+ "  char const * error;\n"
			+ String.format("  %s default_value;\n", DEFAULT_T)
			+ String.format("  %s type;\n", ELEMENT_TYPE_T)
			+ String.format("  %s value_limit;\n", ELEMENT_LIMIT_T)
			+ String.format("  %s validation;\n", VALIDATION_T)
			+ "  bool optional;\n"
			+ String.format("} %s;\n\n", ACTION_PARAMETER_T)

		/* action element */
			+ "typedef struct {\n"
			+ "  char const * name;\n"
			+ "  char const * description;\n"
			+ "  void * element;\n"
			+ String.format("  %s type;\n", ACTION_ELEMENT_TYPE_T)
			+ "  size_t id;\n"
			+ String.format("  %s access;\n", ACCESS_T)
			+ String.format("} %s;\n\n", ACTION_ELEMENT_T)

		/* action */
		  + "typedef struct {\n"
		  + "  char const * name;\n"
		  + "  char const * description;\n"
		  + "  char const * syntax;\n"
		  + String.format("  %s access; \n", ACCESS_T)
		  + "  size_t id;\n"

		  + " struct {\n"
		  + String.format("	%s * element;\n", ACTION_ELEMENT_T)
		  + "    size_t count;\n"
		  + "  } elements;\n"
		  + String.format("} %s;\n\n", ACTION_T)

		  /* action table */
		  + "typedef struct {\n"
		  + String.format("	%s * actions;\n", ACTION_T)
		  + "    size_t count;\n"
		  + String.format("} %s;\n\n", ACTION_TABLE_T)

		  + String.format("extern %s %s[];\n\n", ACTION_TABLE_T, ACTION_TABLE_VAR);

		writeFile(string);
	}

	private void writeTagEnum(LinkedList<ConfigurationGroup> groups) throws IOException {


		int total_tags = ConfigurationGroup.getGroupHasMaxTagCount() +
				 ConfigurationElement.getElementHasMaxTagCount();

		if (total_tags > 0) {

			String string = "typedef enum {\n";

			int count = 0;
			for (ConfigurationGroup group : groups) {
				ConfigType type = group.getConfigType();

				String tag = group.getTagNumRow();

				if (tag != null) {

					count++;
					string += String.format("   %s_%s_max_tag\n",
										toGroupEnumName(type, group.getName()),
										tag.toLowerCase());


					if (count < total_tags)
						string += ",\n";

				}

				LinkedList<ConfigurationElement> elements = group.getElements();
				for (int n=0; n < elements.size(); n++) {
					ConfigurationElement element = elements.get(n);
					String max_tag = element.getMaxTag();
					if (max_tag != null) {
						count++;
						//String var_name = toElementMaxTagVariable(type.toString().toLowerCase(), group.getName(), element.getName());
						string += String.format("   %s%s_max_tag",
											toGroupElementVariable(type.toString().toLowerCase(),
											group.getName()),
											max_tag.toLowerCase());

						if (count < total_tags)
							string += ",\n";

					}
				}
			}
			string += "\n} config_max_tag_t;\n\n";
			writeFile(string);
		}

		if (ConfigObject.getNumTagsTotal() > 0) {

			int count = 0;
			String string = String.format("typedef enum {\n");
			writeFile(string);

			for (ConfigurationGroup group : groups) {
				ConfigType type = group.getConfigType();
				string = "";

				for (String group_tag: group.getTags()) {

					if (count > 0)
						string += " ,\n";
					count++;
					String var_name = toGroupVariable(type.toString().toLowerCase());
					CommandTool.debug_log(group.getName() + " " + var_name);

					string += String.format("    %s_%s_tag",
										toGroupEnumName(type, group.getName()),
										group_tag.toLowerCase());
				}

				LinkedList<ConfigurationElement> elements = group.getElements();
				for (ConfigurationElement element : elements) {
					/* element tag */
					for (String element_tag : element.getTags()) {
						if (count > 0)
							string += ",\n";
						count++;
						string += String.format("    %s_%s_tag",
//										toGroupElementVariable(type.toString().toLowerCase(), group.getName()),
										toElementEnumName(type, group.getName(), element.getName()),
										element_tag.toLowerCase());

					}


					LinkedList<ConfigValue> values = element.getValues();
					for (ConfigValue value : values) {
						/* value tag */
						for (String value_tag : value.getTags()) {
							if (count > 0)
								string += ",\n";
							count++;
//							String enum_var = toElementEnumVariable(type.toString().toLowerCase(), group.getName(), element.getName());
							string +=  String.format("    %s_%s_tag",
												//enum_var,
												toEnumValueName(type, group.getName(), element.getName(), value.getName()),
												value_tag.toLowerCase());
						}
					}
				}
				writeFile(string);
			}
			string = "\n} config_tag_t;\n\n";
			writeFile(string);
		}
	}


	private void writePlatformTagsHeader(LinkedList<ConfigurationTag> platforms, LinkedList<ConfigurationTag> tags) throws IOException {

		String string = "";

		if (tags.size() > 0)
			string += "typedef enum {\n";

		for (ConfigurationTag tag: tags) {
			string += String.format("  tag_%s,\n", tag.getName());
		}

		if (tags.size() > 0)
			string += "  tag_COUNT\n"
					+ "} tags_type_t;\n\n";

		for (ConfigurationTag platform : platforms) {

			for (ConfigurationTag define: platform.getDefines()) {
				string += String.format("#define PLATFORM_%s_%s     %d\n", platform.getName().toUpperCase(),
										define.getName().toUpperCase(), define.getDefineValue());
			}
			string += "\n";

		}

		writeFile(string);

	}

	private void writeTagStructureTypedef(LinkedList<ConfigurationGroup> groups) throws IOException {

		String string = "/*\n"
						 + "* TAG structure"
						 + "* used for any groups/tables/elements/values/actions\n"
						 + "* that have TAG. Tells configManager to update\n"
						 + "* the table according to TAG during runtime.\n"
						 + "*/\n\n"

						+ "enum {\n"
						+ " config_mgr_max_tag,\n"
						+ " config_mgr_feature_tag\n"
						+ "};\n\n"

						+ "typedef enum {\n"
						+ " config_mgr_tag_group,\n"
						+ " config_mgr_tag_element,\n"
						+ " config_mgr_tag_value,\n"
						+ " config_mgr_tag_action,\n"
						+ " config_mgr_tag_action_element\n"
						+ "} config_mgr_tag_type_t;\n\n"

						+ "typedef struct {\n"
						+ SET_GROUP_ENUM_TYPE_T + " group_id;\n"
						+ " char const * name;\n"
						+ " void * data;\n"
						+ " config_mgr_tag_type_t type;\n"
						+ " int tag_value[platform_COUNT];\n"
						+ "} " + TAG_T + ";\n\n"

						+ "typedef struct {\n"
						+ " config_mgr_tag_t * objects;\n"
						+ " size_t count;\n"
						+ "} " + TAG_TABLE_T + ";\n\n";


		writeFile(string);

		string = String.format("extern %s %s[];\n\n", TAG_TABLE_T, TAG_TABLE_VAR);
		writeFile(string);
	}

	private void writeElementLimit(String group_name, ConfigurationElement element) throws IOException {

		if ((element.getMin() != null) || (element.getMax() != null) ||
			(element.getType() == ElementType.ENUM) || (element.getType() == ElementType.ENUM_MULTI)) {
			String element_name = element.getName();
			String element_limit_name = toElementLimitVariable(prefix, group_name, element_name);

			String string = toTagDefine(element.getTags());

			switch (element.getType()) {
			case INT32:
			case INDEX:
				string += String.format("static %s %s = {\n", ELEMENT_INT32_LIMIT_T, element_limit_name);
				if (element.getMin() != null)
					string += String.format("  %s," , element.getMin());
				else
					string += String.format(" %d, ",  utilities.INT32_MIN_VALUE);

				if (element.getMax() != null)
					string += String.format("  %s\n" , element.getMax());
				else
					string += String.format(" %d\n",  utilities.INT32_MAX_VALUE);

				break;
			case ENUM:
			case ENUM_MULTI:
				string += String.format("static %s %s = {\n", ELEMENT_ENUM_LIMIT_T, element_limit_name);
				String enum_name = toElementEnumVariable(prefix, group_name, element_name);
				string += String.format("  %s,\n", enum_name)
					  + String.format("  config_mgr_asizeof(%s)\n", enum_name);
				break;

			case UINT32:
				string += String.format("static %s %s = {\n", ELEMENT_UINT32_LIMIT_T, element_limit_name);
				string += String.format("  %s," , ((element.getMin() != null) ? element.getMin() : "0"));

				if (element.getMax() != null)
					string += String.format("  %s\n" , element.getMax());
				else
					string += String.format(" %d\n",  utilities.UINT32_MAX_VALUE);
				break;

			default:
				string += String.format("static %s %s = {\n", ELEMENT_STRING_LIMIT_T, element_limit_name);
				string += String.format("  %s," , ((element.getMin() != null) ? element.getMin() : "0"));

				if (element.getMax() != null)
					string += String.format("  %s\n" , element.getMax());
				else
					string += String.format(" %d\n",  utilities.UINT32_MAX_VALUE);
				break;
			}

			string += "};\n\n";

			string += toTagDefineClose(element.getTags());

			writeFile(string);
		}

	}

	private void writeElementEnumValue(ConfigurationGroup group, ConfigurationElement element) throws IOException {

		LinkedList<ConfigValue> values = element.getValues();

		if (values != null && values.size() > 0) {
			String enum_name = toElementEnumVariable(prefix, group.getName(), element.getName());

			String enum_string = String.format("static %s %s[] = {\n", ELEMENT_ENUM_T, enum_name);
			int index = 0;

			for (ConfigValue value: values) {

				enum_string += toTagDefine(value.getTags());

				if (index != 0)
					enum_string += "  ,\n";

				index++;
				enum_string += String.format("  {\"%s\", \"%s\", %d, %s, %s, %s}",
											 value.getName(),
											 value.getDescription(),
											 value.getNumber(),
											 toEnumValueName(group.getConfigType(), group.getName(), element.getName(), value.getName()),
											 (value.isExclusive()) ? "true" : "false",
											 toAccessString(element.getAccess()));

				enum_string += toTagDefineClose(value.getTags());

				enum_string += "\n";

			}

			enum_string += "};\n\n";

			writeFile(enum_string);
		}

	}


	private void writeHeaderComment() throws IOException {

		try {

			String className = CommandTool.class.getName();
			DateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
			Date date = new Date();

			String comment = CONFIG_DIGI_COPYRIGHT
							+ "/*\n * This is an auto-generated file - DO NOT EDIT! \n";

			comment += String.format(" * This is a file generated by %s tool.\n", className)
			+ String.format(" * This file was generated on: %s \n", dateFormat.format(date))
			+ String.format(" * The command definition syntax file: %s\n", CommandTool.getCommandFileName())
			+ String.format(" * The platform file: %s\n", CommandTool.getOption(CommandTool.PLATFORM_FILE_OPTION))
			+ String.format(" * The version of %s tool was: %s */\n\n", className, CommandTool.VERSION);

			bufferedWriter.write(comment);

		} catch (IOException e) {
			throw new IOException ("Unable to write comment");
		}
	}

	private void writeAction(ConfigType type, LinkedList<ConfigurationGroup> actions) throws IOException {

		String string = String.format("static %s %s[] = {\n", ACTION_T, ACTION_VAR);
		writeFile(string);

		int count = 0;
		for (ConfigurationGroup action: actions) {

			String action_name = action.getName();

			ConfigType action_type = action.getConfigType();

			if (action_type == type) {

				string = toTagDefine(action.getTags());

				if (count != 0)
					string += " ,\n";

				count++;

				string += " {\n" + String.format("   \"%s\",\n", action_name);


				string += String.format("   \"%s\",\n", action.getDescription());

				if (action.getSyntax() != null)
					string += String.format("   \"%s\",\n", action.getSyntax());
				else
					string += String.format("   NULL,\n");

				string += String.format("   %s,\n", toAccessString(action.getAccess()))
							 + String.format("   %s,\n", toGroupEnumName(action.getConfigType(), action.getName()))
							 + "   {\n";

				if (action.getElements().size() > 0) {
					String var_name = toActionVariable(prefix, action_name);

					string += String.format("   %s,\n", var_name)
							 + String.format("   config_mgr_asizeof(%s)\n", var_name);
				} else {
					string += "   NULL,\n"
							+ "   0\n";
				}
				string += "   }\n"
						+ " }";

				string += toTagDefineClose(action.getTags());

				string += "\n";
				writeFile(string);
			}

		}
		string = "};\n\n"
				+ String.format("%s %s[1] = {\n", ACTION_TABLE_T, ACTION_TABLE_VAR)
				+ String.format("  { %s,\n", ACTION_VAR)
				+ String.format("    %d\n  }\n", count)
				+ String.format("};\n\n");

		writeFile(string);

	}

	private void writeParameters(ConfigurationGroup action, LinkedList<ConfigurationElement> parameters) throws IOException {

		//if (parameters.size() > 0)
		{
			for (ConfigurationElement parameter: parameters) {
				/* default variable */
				writeFile(toTagDefine(parameter.getTags()));

				writeDefault(action.getName(), parameter);

				/* action parameter */
//				if (!action.getName().equalsIgnoreCase(SHOW_ACTION) || !parameter.getName().equalsIgnoreCase(SHOW_CONFIG_ACTION))
//				if (!parameter.getName().equalsIgnoreCase(SHOW_CONFIG_ACTION))
					writeActionParameter(action.getName(), parameter);

				writeFile(toTagDefineClose(parameter.getTags()));
			}

			writeActionElement(action, parameters);
		}
	}

	private void writeActionElement(ConfigurationGroup action, LinkedList<ConfigurationElement> elements) throws IOException {

		String string = String.format("\nstatic %s %s[] = {\n", ACTION_ELEMENT_T, toActionVariable(prefix, action.getName()));

		int count = 0;


		for (ConfigurationElement element: elements) {

			string += toTagDefine(element.getTags());

			if (count != 0)
				string += ",\n";

			count++;

			/* element structure:
			 * name, description, type, pointer, id
			 */
			string += "{"
						+ String.format("  \"%s\",\n", element.getName())
						+ String.format("   \"%s\",\n", element.getDescription())
						+ "   &";


			string += toActionParameterVariable(prefix, action.getName(), element.getName())
					+ ",\n";

			if (element.isParameter())
				string += "   config_mgr_action_parameter,\n";
			else
				string += "   config_mgr_action_element,\n";

//			string += String.format("%d", count -1);
			string += String.format("   %s,\n", toElementEnumName(action.getConfigType(), action.getName(), element.getName()));

			if (element.getAccess() == AccessType.NONE)
				string += String.format("   %s\n", toAccessString(action.getAccess()));
			else
				string += String.format("   %s\n", toAccessString(element.getAccess()));

			 string += "}\n";

			 string += toTagDefineClose(element.getTags());
		}

		string += "};\n\n";
		if (count > 0)
			writeFile(string);

	}

	private void writeActionParameter(String group_name, ConfigurationElement element) throws IOException {

		String string = String.format("\nstatic %s %s = {\n", ACTION_PARAMETER_T, toActionParameterVariable(prefix, group_name, element.getName()))
						+ "  ";

		ConfigError error = element.getError();

		if (error == null)
			string += "NO_ERROR";
		else
			string += String.format("\"%s\"", error.getDescription());

		string += ", ";

		if (element.getDefaultValue() == null)
			string += "NO_DEFAULT";
		else {
			switch (element.getType()) {

			case ENUM_MULTI:
			case ENUM:
			case STRING:
			case IPADDR:
			case FQDN:
			case MAC_ADDR:
			case ENCPASSWORD:
			case PASSWORD:
			case INTERFACE:
				string += String.format("(%s)%s", DEFAULT_T, toParameterDefaultString(prefix, group_name, element.getName()));
				break;

			case UINT32:
			case INT32:
			case ON_OFF:
			case BOOLEAN:
				string += String.format("(%s)&%s",DEFAULT_T, toParameterDefaultString(prefix, group_name, element.getName()));
				break;

			case COUNTER32:
			case COUNTER64:
			case INDEX:
			case NONE:
				string += "NO_DEFAULT";

			}
		}

		string += ", "
				+ toElementTypeString(element.getType())
				+ ", ";

		/* limit */
		if ((element.getMin() != null) || (element.getMax() != null) ||
			(element.getType() == ElementType.ENUM) || (element.getType() == ElementType.ENUM_MULTI))
			string += "&" + toElementLimitVariable(prefix, group_name, element.getName());
		else
			string += "NO_LIMIT";

		string += ", ";

		/* validation function */
		ValidationType validationType = ValidationType.valueOf(element.getType().toString());
		String validationFunction = null;

		if (error != null)
			validationFunction = toValidationFunction(prefix, group_name, element.getName());
		else if (validationType.prototype != null)
//			validationFunction = "validate_" + validationType.toString().toLowerCase();
			validationFunction = validationType.toDefaultFunctionName();
		else
			validationFunction = "NO_VALIDATION";

		string += String.format("%s, ", validationFunction)
				+ String.format("%s\n",(element.isOptional() ? "true" : "false"))
				+ "};\n\n";

		writeFile(string);

	}
	private void writeDefault(String group_name, ConfigurationElement element) throws IOException {

		String string = null;

		if (element.getDefaultValue() != null) {
			switch (element.getType()) {

			case ENUM_MULTI:
			case ENUM:
			case STRING:
			case IPADDR:
			case FQDN:
			case MAC_ADDR:
			case ENCPASSWORD:
			case PASSWORD:
			case INTERFACE:
				string = String.format("static char const %s[] = \"%s\";\n",
										toParameterDefaultString(prefix, group_name, element.getName()),
										element.getDefaultValue());
				break;

			case UINT32:
				string = String.format("static uint32_t const %s = %s;\n",
						toParameterDefaultString(prefix, group_name, element.getName()),
						element.getDefaultValue());
				break;

			case INT32:
				string = String.format("static int32_t const %s = %s;\n",
						toParameterDefaultString(prefix, group_name, element.getName()),
						element.getDefaultValue());
				break;

			case ON_OFF:
				String boolean_string = element.getDefaultValue().equalsIgnoreCase("on") ? "false" : "true";

				string = String.format("static bool const %s = %s;\n",
						toParameterDefaultString(prefix, group_name, element.getName()),
						boolean_string);
				break;

			case BOOLEAN:
				string = String.format("static bool const %s = %s;\n",
						toParameterDefaultString(prefix, group_name, element.getName()),
						element.getDefaultValue());
				break;

			case COUNTER32:
			case COUNTER64:
			case INDEX:
			case NONE:

			}

			if (string != null)
				writeFile(string);
		}
	}

	private void writeFile(String string) throws IOException {

		try {
			bufferedWriter.write(string);
		} catch (IOException e) {
			throw new IOException ("Writing \"" + string + "\" failed");
		}
	}

	private String toTagDefine(ArrayList<String> tags) {

		String string = "";

		int tags_size = tags.size();

		if (enable_to_tag_define == true) {
			if (tags_size > 0)
				string += "#if ";

			if (tags_size > 1)
				string += " (";

			for (String tag : tags) {
				string += String.format("(%s == 1)", tag);
				tags_size -= 1;

				if (tags_size > 0)
					string += " || ";
				else if (tags.size() > 1)
					string += ")";
			}

			if (tags.size() > 0)
				string += "\r\n";
		}

		return string;
	}

	private String toTagDefineClose(ArrayList<String> tags) {

		String string = "";

		if (enable_to_tag_define == true) {
			if (tags.size() > 0) {
				string = "\r\n#endif /* ";
				for (String tag : tags) {
					string += tag + " ";
				}
				string += "*/\r\n\n";
			}
		}

		return string;
	}

	private String toNormalizeString(String string) {
//		return string.replaceAll("^[!@#$%^*(, replacement),.<>~`[]{}\\/+=-\\s]*$", "_");
		return string.replaceAll("\\W", "_");
	}

	private static String toAccessString(AccessType access) {
		String string = String.format("%s_access_%s", CONFIG_MGR, access.toString().toLowerCase());
		return string;
	}

	private String toElementTypeString(ElementType type) {
		String string = String.format("%s_element_type_%s", CONFIG_MGR, type.toString().toLowerCase());
		return string;
	}

	private String toElementEnumVariable(String prefix, String group, String element) {
		String string = String.format("%s_%s_%s_%s_enums", CONFIG_MGR, prefix, toNormalizeString(group), toNormalizeString(element));
		return string;
	}
	private String toElementLimitVariable(String prefix, String group, String element) {
		String string = String.format("%s_%s_%s_%s_limit", CONFIG_MGR, prefix, toNormalizeString(group), toNormalizeString(element));
		return string;
	}

	private String toGroupElementVariable(String prefix, String group) {
		String string = String.format("%s_%s_%s_elements", CONFIG_MGR, prefix, toNormalizeString(group));
		return string;
	}

	private String toGroupVariable(String prefix) {
		String string = String.format("%s_%ss", CONFIG_MGR, prefix);
		return string;
	}

	private String toActionParameterVariable(String prefix, String group, String parameter) {
		String string = String.format("%s_%s_%s_%s", CONFIG_MGR, prefix, toNormalizeString(group), toNormalizeString(parameter));
		return string;
	}

	private String toActionVariable(String prefix, String group) {
		String string = String.format("%s_%s_%s", CONFIG_MGR, prefix, toNormalizeString(group));
		return string;
	}

	private String toParameterDefaultString(String prefix, String action, String parameter) {
		String string = String.format("%s_%s_%s_default", prefix, toNormalizeString(action), toNormalizeString(parameter));
		return string;
	}

	private String toValidationFunction(String prefix, String group, String element) {
		String string = String.format("validate_%s_%s_%s", prefix, toNormalizeString(group), toNormalizeString(element));

		return string;
	}

	private String toConfigTypeName(ConfigType type) {
		String string = "";

		switch (type) {
		case SET_GROUP:
			string = "config_";
			break;
		case GET_GROUP:
			string = "config_state_";
			break;
		case ACTION:
			string = "action_";
			break;
		}
		return string;
	}

	private String toGroupEnumName(ConfigType type, String group) {
		String string = toConfigTypeName(type) + toNormalizeString(group);
		return string;
	}

	private String toElementEnumName(ConfigType type, String group, String element) {
		String string = toConfigTypeName(type) + toNormalizeString(group) + "_" + toNormalizeString(element);
		return string;
	}

	private String toElementEnumName(ConfigType type, String group) {
		String string = toConfigTypeName(type) + toNormalizeString(group);
		return string;
	}

	private String toEnumValueName(ConfigType type, String group, String element, String value) {
		String string = toConfigTypeName(type) + toNormalizeString(group) + "_" + toNormalizeString(element) + "_" + toNormalizeString(value);
		return string;
	}

	private String toEnumValueName(ConfigType type, String group, String element) {
		String string = toConfigTypeName(type) + toNormalizeString(group) + "_" + toNormalizeString(element);
		return string;
	}

	private static String toEnumValidateFunction() {
		String string = "{\n"
				+ "	bool validated = false;\n"
				+ "	" + ELEMENT_ENUM_T + " const * enum_value = enum_limit->value;\n"
				+ "	size_t i;\n"

				+ "	for (i=0; i < enum_limit->count; i++) {\n"
				+ "		if (enum_value[i].access !=" + String.format(" %s", toAccessString(AccessType.NO_ACCESS)) + " &&\n"
				+ "			strcmp(string, enum_value[i].name) == 0) {\n"
				+ "			validated = true;\n"
				+ "			break;\n"
				+ "		}\n"
				+ "	}\n"
				+ "	return validated;\n"
				+ "}\n\n";

		return string;

	}

	private static String toEnumMultiValidateFunction() {
		String string = "{\n"
				+ "	bool validated = false;\n"
				+ "	char * ptr = (char *)string;\n"
				+ "	size_t i;\n"
				+ "	size_t enum_count = 0;\n"
				+ "	bool exclusive = false;\n\n"

				+ "	{\n"
				+ "		size_t length = strlen(ptr);\n"
				+ "		while (length > 0) {\n"
				+ "			if (*ptr != ' ') break;\n"
				+ "			ptr++;\n"
				+ "			length--;\n"
				+ "		}\n"
				+ "		if (length == 0) {\n"
				+ "			validated = true;\n"
				+ "			goto done;\n"
				+ "		}\n"
				+ "	}\n"

				+ "	while (ptr != NULL) {\n"
				+ "		char * p = strchr(ptr, ',');\n\n"
				+ "		size_t length = (p != NULL) ? (p - ptr) : strlen(ptr);\n"
				+ "		while (length > 0) {\n"
				+ "			if (*ptr != ' ') break;\n"
				+ "			ptr++;\n"
				+ "			length--;\n"
				+ "		}\n"

				+ "		if (length > 0) {\n"

				+ "			validated = false;\n\n"
				+ "			for (i=0; i < enum_limit->count; i++) {\n"
				+ "				" + ELEMENT_ENUM_T + " const * enum_value = &enum_limit->value[i];\n"
				+ "				if ((enum_value->access !=" + String.format(" %s", toAccessString(AccessType.NO_ACCESS)) + ") &&\n"
				+ "					(length == strlen(enum_value->name)) && \n"
				+ "					(memcmp(ptr, enum_value->name, (int)length) == 0)) {\n"
				+ "					validated = true;\n"
				+ "					exclusive |= enum_value->multi_exclusive;\n"
				+ "					break;\n"
				+ "				}\n"
				+ "			}\n"
				+ "			if (!validated) break;\n"
				+ "			enum_count++;\n"
				+ "		}\n"

				+ "		ptr = (p != NULL) ? (p + 1) : p;\n"
				+ "		if (exclusive && enum_count > 1) {\n"
				+ "			validated = false;\n\n"
				+ "			break;\n"
				+ "		}\n\n"
				+ "	}\n\n"


				+ "done:\n"
				+ "	return validated;\n"
				+ "}\n\n";

		return string;
	}
}
