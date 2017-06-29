package com.digi.config.command_tool;

import java.io.File;
import java.io.IOException;
import java.util.LinkedList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Parser {

	public final static int MAX_NAME_LENGTH = 40;
	public final static int MAX_ERROR_NAME_LENGTH = 40;
	private final static int MAX_DESCRIPTION_LENGTH = 40;
	private final static int MAX_UI_NAME_LENGTH = 40;

	// PRIVATE variables
	private static TokenScanner tokenScanner;
	private static String token;
	private static int max_action_parameters = 0;

	public static void ParseFile(String fileName, LinkedList<ConfigurationGroup> groups) throws IOException, NullPointerException {

		CommandTool.debug_log("Parsing command file");

		try {
			tokenScanner = new TokenScanner(fileName);

			if (tokenScanner.hasToken())
				token = tokenScanner.getToken();

			/* first use a Scanner to get each word */
			while (tokenScanner.hasToken()) {

				if ((token.equalsIgnoreCase("show")) || (token.equalsIgnoreCase("table"))) {

					CommandTool.debug_log("Parsing " + token);

					ConfigurationGroup new_group = parseGroup(token.equalsIgnoreCase("table"));

					/* make sure group name doesn't exist*/
					checkGroup(groups, new_group);

					new_group.validate();
					if (new_group.getAccess() == null) {
						if (new_group.getConfigType() == ConfigType.GET_GROUP)
							new_group.setAccess(AccessType.READONLY);
						else
							new_group.setAccess(AccessType.READWRITE);
					}

					groups.add(new_group);
					continue;

				} else if (token.startsWith("#")) {
					tokenScanner.skipCommentLine();

				} else if ((token.equalsIgnoreCase("action")) ) {
					/* TODO: action */
					CommandTool.debug_log("Parsing " + token);

					ConfigurationGroup new_action = parseAction();

					/* make sure group name doesn't exist */
					checkGroup(groups, new_action);

					new_action.validate();

					if (new_action.getAccess() == null) {
					   new_action.setAccess(AccessType.READWRITE);
					}
					groups.add(new_action);
					continue;


				} else {
					throw new IOException("Unrecognized keyword: " + token);
				}

				token = tokenScanner.getToken();

			}

		} catch (NullPointerException e) {
			CommandTool.log("Parser NullPointerException");
			CommandTool.log(e.toString());
			throw new NullPointerException();
		} catch (IllegalArgumentException e) {
			throw new IOException(e.getMessage());
		} catch (Exception e) {
			throw new IOException(printFileError(fileName, e.getMessage()));
		}
		finally {
			tokenScanner.close();
		}

	}


	public static void ParsePlatformFile(String fileName, LinkedList<ConfigurationTag> tags) throws IOException, NullPointerException {

		CommandTool.debug_log("Parsing platform file");

		try {

			tokenScanner = new TokenScanner(fileName);
			ConfigurationTag tag = null;
			/* first use a Scanner to get each word */
			while (tokenScanner.hasToken()) {

				String token = readToken();

				if (token.equalsIgnoreCase("platform")) {

					/* parse name */
					String name = readName(MAX_NAME_LENGTH);

					/* parse description */
					String description = readDescription();

					tag = new ConfigurationTag(name, description, true);

					checkTag(tags, tag);

					tags.add(tag);
				}
				else if (token.equalsIgnoreCase("define")) {
					/* parse name */
					String name= readName(MAX_NAME_LENGTH);

					/* parse number */
					if (!tokenScanner.hasTokenInt())
						throw new Exception("Missing value number");

					int number = tokenScanner.getTokenInt();

					ConfigurationTag define = new ConfigurationTag(name, null, false);

					define.setDefineValue(number);
					if (tag != null)
						tag.addDefines(define);

				} else if (token.startsWith("#")) {
					tokenScanner.skipCommentLine();
				}
				else {
					tag = new ConfigurationTag(token, null, false);

					checkTag(tags, tag);

					/* parse description */
					String description = readDescription();
					tag.setStringValue(description);
					tags.add(tag);

					tag = null;

				}

			}

		} catch (NullPointerException e) {
			CommandTool.log("Parser NullPointerException");
			CommandTool.log(e.toString());
			throw new NullPointerException();
		} catch (IllegalArgumentException e) {
			throw new IOException(e.getMessage());
		} catch (Exception e) {
			throw new IOException(printFileError(fileName, e.getMessage()));
		}
		finally {
			tokenScanner.close();
		}

	}

	private static String readToken() throws Exception {
		String string = null;

		while (tokenScanner.hasToken()) {
			string = readLongDescription(false);

			if (string != null)
				break;

			string = tokenScanner.getToken();
			if (!string.startsWith("#"))
				break;

			tokenScanner.skipCommentLine();
		}

		return string;
	}

	private static void checkGroup(LinkedList<ConfigurationGroup> groups, ConfigurationGroup new_group) throws IOException {

		for (ConfigurationGroup group : groups) {
			if (group.getName().equals(new_group.getName())) {
				switch (group.getConfigType()) {
				case GET_GROUP:
					if (new_group.getConfigType() == ConfigType.GET_GROUP)
						throw new IOException("Duplicate <group>: " + group.getName());
					break;
				case SET_GROUP:
				case ACTION:
					switch (new_group.getConfigType()) {
					case GET_GROUP:
						break;
					default:
						throw new IOException(String.format("Duplicate \"%s\" in %s!",
													group.getName(),
													group.getConfigType().toString().toLowerCase().replace('_', ' ')));
					}
					break;
				}

			}
		}

	}

	private static void checkTag(LinkedList<ConfigurationTag> tags, ConfigurationTag new_tag) throws IOException {
		for (ConfigurationTag tag : tags) {
			if (tag.getName().equals(new_tag.getName())) {
				throw new IOException("Duplicate Tag: "
						+ new_tag.getName());
			}
		}

	}
	//private static final Pattern ALPHACHARACTERS = Pattern.compile("\\w+");
	private static final Pattern ALPHACHARACTERS = Pattern.compile("\\p{ASCII}*$");

	private static boolean checkAlphaCharacters(String s) {
		if (s == null) {
			return false;
		} else {
			Matcher m =  ALPHACHARACTERS.matcher(s);
			return m.matches();
		}
	}

	public static int getMaxActionParameters()
	{
		return max_action_parameters;
	}

	private static String readName(int length) throws Exception {
		String name = tokenScanner.getToken(); // tokenScanner.next();

		if (name == null) {
			throw new Exception("Missing name!");
		}
		if (name.length() > length) {
			throw new Exception("The name > the maximum length limited " + length);
		}
		else if (!checkAlphaCharacters(name)) {
			throw new Exception("Invalid character in the name: " + name);
		}
		return name;
	}

	private static String readString() throws IOException {

		String string = null;
		if (tokenScanner.hasToken("\\\".*")) {
			string = tokenScanner.getTokenInLine("\\\".*?\\\"");
			if (string == null) {
				throw new IOException("Invalid String without ending quote (\")");
			}

			string = string.substring(1, string.lastIndexOf("\""));

		}
		return string;
	}

	private static String readDefault() throws Exception {
		String string = readString();

		if (string == null) {
			string = tokenScanner.getToken();
		}

		return string;
	}

	private static String readUiName() throws Exception {
		String name = readString();

		if (name != null) {
			if (name.length() > MAX_UI_NAME_LENGTH) {
				throw new Exception("The name > the maximum length limited " + MAX_UI_NAME_LENGTH);
			}
		}
		return name;
	}

	private static String readUciFileName() throws IOException {

		String string;

		try {
			string = readString();

			if (string != null && !isFilenameValid(string))
				throw new IOException ("Invalid UCI file name");

		} catch (Exception e) {
			throw new IOException ("Invalid UCI file name");
		}
		return string;
	}

	private static String readDescription() throws Exception {

		String description = readString();
		if (description != null) {
			if (description.length() > MAX_DESCRIPTION_LENGTH) {
				throw new Exception("desciption > maximum length "
						+ MAX_DESCRIPTION_LENGTH);
			}

			description = description.replace(":", "::");

			if (description.length() == 0)
				description = null;

		}
		return description;
	}

	private static String readLongDescription(boolean cloudDescription) throws Exception {

		String description = readString();
		if ((description != null) && (cloudDescription == true)) {
			description = description.replace(":", "::");

			if (description.length() == 0)
				description = null;
		}
		return description;
	}

	private static ElementType readElementType() throws Exception {
		String type = tokenScanner.getToken();

		if (type == null) {
			return ElementType.NONE;
		}

		return ElementType.toElementType(type);
	}

	private static String printFileError(String fileName, String str) {
		String message = "Error found in " + fileName + ", line ";

		message += tokenScanner.getLineNumber();

		return message + ": " + str;
	}

	private static ConfigExample parseExample() throws Exception {
		String text = readString();
		if (text == null)
			throw new IOException("Missing text for example!");
		ConfigExample example = new ConfigExample(text);
		String desciption = readString();
		if (desciption != null)
			example.setDescription(desciption);
		return example;
	}

	private static String parseDocument() throws IOException {
		String text = readString();
		if (text == null)
			throw new IOException("Missing text for document description.");
		return text;
	}

	private static ConfigurationGroup parseGroup(boolean isTable) throws Exception {
		boolean set_max_table = false;
		ConfigType config_type = isTable ? ConfigType.SET_GROUP : ConfigType.GET_GROUP;
		String name = readName(MAX_NAME_LENGTH);
		String ui_name = readUiName();
		String description = readLongDescription(true); //readDescription();

		ConfigurationGroup group = new ConfigurationGroup(name, ui_name, description, config_type);
		if (config_type == ConfigType.SET_GROUP) {
			String uci_filename = readUciFileName();
			if (uci_filename == null)
				throw new Exception(String.format("Missing <UCI filename> for %s group!", name));
			group.setUciFileName(uci_filename);
		}

		token = tokenScanner.getToken();
		while (tokenScanner.hasToken()) {
			if (token.equalsIgnoreCase("tag")) {
				String tag = tokenScanner.getToken();
				group.addTag(tag);
			}
			else if (token.equalsIgnoreCase("super")) {
				group.setAccess(AccessType.READONLY_SUPER_READWRITE);
			}
			else if (token.equalsIgnoreCase("readwrite")) {
				group.setAccess(AccessType.READWRITE);
			}
			else if (token.equalsIgnoreCase("readonly")) {
				group.setAccess(AccessType.READONLY);
			}
			else if (token.equalsIgnoreCase("writeonly")) {
				group.setAccess(AccessType.WRITEONLY);
			}
			else if (token.equalsIgnoreCase("max")) {
				if (tokenScanner.hasTokenInt()) {
					int max = tokenScanner.getTokenInt();
					group.setNumRows(max);
					group.setMaxInstance(max);
				}
				else {
					String tag = tokenScanner.getToken();
					if (tag.matches("[0-9]+"))
						throw new IOException("Invalid max = " + tag + " on \"" + name + "\" table");
					group.setTagNumRows(tag);
				}
				set_max_table = true;
			}
			else if (token.equalsIgnoreCase("index")) {
				ConfigurationElement element = parseIndex();
				element.validate(group.getConfigType());
				group.addElement(element);
				continue;
			}
			else if (token.equalsIgnoreCase("element")) {
				ConfigurationElement element = parseElement(config_type);
				element.validate(group.getConfigType());
				group.addElement(element);
				continue;
			}
			else if (token.equalsIgnoreCase("example")) {
				ConfigExample example = parseExample();
				group.addExample(example);
			}
			else if (token.equalsIgnoreCase("document")) {
				String text = parseDocument();
				group.setDocumentDescription(text);
			}
			else if (token.equalsIgnoreCase("mib-indexed")) {
				group.setMibIndex();
			}
			else if (token.equalsIgnoreCase("no-mib")) {
				group.setNoMib();
			}
			else if (token.startsWith("#")) {
				tokenScanner.skipCommentLine();
			}
			else if (!token.equalsIgnoreCase("show") && !token.equalsIgnoreCase("table") && !token.equalsIgnoreCase("action")) {
				throw new Exception(String.format("Invalid \"%s\" for \"%s\" %s!", token, name, config_type.toString().toLowerCase().replace('_', ' ')));
			}
			else {
				break;
			}
			token = tokenScanner.getToken();
		}
		if (isTable && !set_max_table) {
			throw new IllegalArgumentException(String.format("Missing <max> for %s table!", name));
		}
		CommandTool.debug_log(String.format("Parsed %s \"%s\"", group.getConfigType().toString(), group.getName()));
		return group;
	}

	private static ConfigurationElement parseElement(ConfigType config_type) throws Exception {
		String name = readName(MAX_NAME_LENGTH);
		String description = readDescription();
		String help_description = readLongDescription(true);
		token = tokenScanner.getToken();
		ElementType type;
		try {
			type = ElementType.toElementType(token);
			token = tokenScanner.getToken();
		}
		catch (Exception e) {
			type =  ElementType.NONE;
			if (token == null)
				token = tokenScanner.getToken();
		}
		ConfigurationElement element = new ConfigurationElement(name, description, help_description, type);
		element.setConfigType(config_type);
		while (tokenScanner.hasToken()) {
			if (token.equalsIgnoreCase("min")) {
				String min = tokenScanner.getToken();
				element.setMin(min);
			}
			else if (token.equalsIgnoreCase("max")) {
				String max = tokenScanner.getToken();
				element.setMax(max);
			}
			else if (token.equalsIgnoreCase("default")) {
				String default_value = readDefault();
				element.setDefaultValue(default_value);
			}
			else if (token.equalsIgnoreCase("super")) {
				element.setAccess(AccessType.READONLY_SUPER_READWRITE);
			}
			else if (token.equalsIgnoreCase("readwrite")) {
				element.setAccess(AccessType.READWRITE);
			}
			else if (token.equalsIgnoreCase("readonly")) {
				element.setAccess(AccessType.READONLY);
			}
			else if (token.equalsIgnoreCase("writeonly")) {
				element.setAccess(AccessType.WRITEONLY);
			}
			else if (token.equalsIgnoreCase("tag")) {
				String tag = tokenScanner.getToken();
				element.addTag(tag);
			}
			else if (token.equalsIgnoreCase("value")) {
				ConfigValue value = parseValue();
				element.addValue(value);
				continue;
			}
			else if (token.equalsIgnoreCase("error")) {
				String error_name = readName(MAX_ERROR_NAME_LENGTH);
				String error_desc = readLongDescription(true);
				element.setError(error_name, error_desc);
			}
			else if (token.equalsIgnoreCase("units")) {
				String units = tokenScanner.getToken();
				element.setUnits(units);
			}
			else if (token.startsWith("#")) {
				tokenScanner.skipCommentLine();
			}
			else {
				break;
			}
			token = tokenScanner.getToken();
		}
		CommandTool.debug_log("Parsed element \"" + element.getName() + "\"");
		return element;
	}

	private static ConfigurationElement parseIndex() throws Exception {

		/* parse name */
		String name = readName(MAX_NAME_LENGTH);

		/* parse description */
		String description = readDescription();

		/* parse type. We only support int32 and uint32 types  */
		ElementType type = readElementType();
		switch (type) {
		case UINT32:
		case INT32:
			break;
		default:
			throw new IOException(String.format("Only <uint32> or <int32> type is supported for\"%s\" index!", name));
		}
		ConfigurationElement element = new ConfigurationElement(name, description, null, ElementType.INDEX);
		element.setAccess(AccessType.READONLY);
		token = tokenScanner.getToken();
		while (tokenScanner.hasToken()) {
			if (token.equalsIgnoreCase("min")) {
				String min = tokenScanner.getToken();
				element.setMin(min);
			}
			else if (token.equalsIgnoreCase("max")) {
				String max = tokenScanner.getToken();
				element.setMax(max);
			}
			else if (token.equalsIgnoreCase("readonly")) {
				element.setAccess(AccessType.READONLY);
			}
			else if (token.equalsIgnoreCase("tag")) {
				String tag = tokenScanner.getToken();
				element.addTag(tag);
			}
			else if (token.startsWith("#")) {
				tokenScanner.skipCommentLine();
			}
			else {
				break;
			}
			token = tokenScanner.getToken();
		}
		if (element.getMin() == null)
			element.setMin("1");
		element.setDefaultValue("1");
		CommandTool.debug_log("Parsed index \"" + element.getName() + "\"");
		return element;
	}

	private static ConfigValue parseValue() throws Exception {
		String name = readName(MAX_NAME_LENGTH);
		String description = readDescription();
		ConfigValue value;
		if (!tokenScanner.hasTokenInt()) {
			value = new ConfigValue(name, description);
		}
		else {
			int number = tokenScanner.getTokenInt();
			value = new ConfigValue(name, description, number);
		}
		
		while (tokenScanner.hasToken()) {
			token = tokenScanner.getToken();
			if (token.equalsIgnoreCase("tag")) {
				String tag = tokenScanner.getToken();
				value.addTag(tag);
			}
			else if (token.equalsIgnoreCase("exclusive")) {
				value.setExclusive(true);
			}
			else if (token.startsWith("#"))
				tokenScanner.skipCommentLine();
			else
				break;
		}
		CommandTool.debug_log("Parsed value \"" + value.getName() + "\"");
		return value;
	}

	private static ConfigurationGroup parseAction() throws Exception {
		String name = readName(MAX_NAME_LENGTH);
		String description = readDescription();
		ConfigurationGroup group = new ConfigurationGroup(name, null, description, ConfigType.ACTION);
		token = tokenScanner.getToken();
		while (tokenScanner.hasToken()) {
			if (token.equalsIgnoreCase("tag")) {
				String tag = tokenScanner.getToken();
				group.addTag(tag);
			}
			else if (token.equalsIgnoreCase("super")) {
				group.setAccess(AccessType.READONLY_SUPER_READWRITE);
			}
			else if (token.equalsIgnoreCase("readwrite")) {
				group.setAccess(AccessType.READWRITE);
			}
			else if (token.equalsIgnoreCase("readonly")) {
				group.setAccess(AccessType.READONLY);
			}
			else if (token.equalsIgnoreCase("writeonly")) {
				group.setAccess(AccessType.WRITEONLY);
			}
			else if (token.equalsIgnoreCase("parameter")) {
				ConfigurationElement parameter = parseParameter(ConfigType.ACTION);
				parameter.setParameter();
				parameter.validate(group.getConfigType());
				group.addElement(parameter);
				if (group.getElements().size() > max_action_parameters)
					max_action_parameters = group.getElements().size();
				continue;
			}
			else if (token.equalsIgnoreCase("element")) {
				ConfigurationElement element = parseElement(ConfigType.ACTION);
				element.validate(group.getConfigType());
				group.addElement(element);
				continue;
			}
			else if (token.equalsIgnoreCase("syntax")) {
				String syntax = readLongDescription(false);
				group.setSyntax(syntax);
			}
			else if (token.equalsIgnoreCase("example")) {
				ConfigExample example = parseExample();
				group.addExample(example);
			}
			else if (token.equalsIgnoreCase("document")) {
				String text = parseDocument();
				group.setDocumentDescription(text);
			}
			else if (token.startsWith("#")) {
				tokenScanner.skipCommentLine();
			}
			else {
				break;
			}
			token = tokenScanner.getToken();
		}
		CommandTool.debug_log(String.format("Parsed %s \"%s\"", group.getConfigType().toString(), group.getName()));
		return group;
	}

	private static ConfigurationElement parseParameter(ConfigType config_type) throws Exception {
		String name = readName(MAX_NAME_LENGTH);
		String description = readDescription();
		String help_description = readLongDescription(false);
		token = tokenScanner.getToken();
		ElementType type;
		try {
			type = ElementType.toElementType(token);
			token = tokenScanner.getToken();
		} catch (Exception e) {
			type =  ElementType.NONE;
			if (token == null)
				token = tokenScanner.getToken();
		}

		ConfigurationElement parameter = new ConfigurationElement(name, description, help_description, type);

		parameter.setConfigType(config_type);

		while (tokenScanner.hasToken()) {
			if (token.equalsIgnoreCase("min")) {
				String min = tokenScanner.getToken();
				parameter.setMin(min);
			}
			else if (token.equalsIgnoreCase("max")) {
				String max = tokenScanner.getToken();
				parameter.setMax(max);
			}
			else if (token.equalsIgnoreCase("default")) {
				String default_value = readDefault();
				parameter.setDefaultValue(default_value);
			}
			else if (token.equalsIgnoreCase("optional")) {
				parameter.setOptional();
			}
			else if (token.equalsIgnoreCase("error")) {
				/* parse description */
				String error_name = readName(MAX_ERROR_NAME_LENGTH);
				String error_desc = readLongDescription(false);
				parameter.setError(error_name, error_desc);
			}
			else if (token.equalsIgnoreCase("value")) {
				ConfigValue value = parseValue();
				parameter.addValue(value);
				continue;
			}
			else if (token.startsWith("#")) {
				tokenScanner.skipCommentLine();
			}
			else {
				break;
			}
			token = tokenScanner.getToken();
		}

		CommandTool.debug_log("Parsed parameter \"" + parameter.getName() + "\"");

		return parameter;
	}

	public static boolean isFilenameValid(String file) {
		File f = new File(file);
		try {
			f.getCanonicalPath();
			return true;
		}
		catch (IOException e) {
			return false;
		}
	}
}
