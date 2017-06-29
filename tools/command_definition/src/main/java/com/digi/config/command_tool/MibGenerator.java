package com.digi.config.command_tool;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.LinkedList;
import java.util.regex.Pattern;
import java.util.Date;
import java.io.File;
// import org.apache.commons.lang3.text.WordUtils;

/**
 * Interface between the parser and the generator modules. It generates source
 * and header files for Config Manager.
**/
public class MibGenerator implements GeneratorInterface {

	private String interfaceName = "MIB Generator";
	private final String MODEL_NAME = "transport-lr54";
	private final String CONFIG = "config";
	private final String SHOW = "show";

	public String getOption() {
		return "mib";
	}

	public String[] getGeneratorOption() {
		String[] options = new String[0];
		return options;
	}
	
	public boolean setGeneratorOption(String options) {
		return false;
	}

	public String getName() {
		return interfaceName;
	}

	public boolean GenerateOutputFile(LinkedList<ConfigurationGroup> groups, LinkedList<ConfigurationTag> platforms, String outputDirectory) {
		boolean isOk = false;
		try {
			String filePath = constructFilePath(outputDirectory);
			FileWriter fileWriter = new FileWriter(filePath);
			BufferedWriter fileBuffer = new BufferedWriter(fileWriter);
			writeMibHeader(fileBuffer);
			writeGroupIds(groups, fileBuffer);
			for (ConfigurationGroup group: groups) {
				ConfigType configType = group.getConfigType();
				if ((configType == ConfigType.SET_GROUP) || (configType == ConfigType.GET_GROUP)) {
					if (group.isNoMib() == false) {
						writeGroupHeader(fileBuffer, group);
						if (group.isMibIndexed()) {
							writeGroupTable(fileBuffer, group);
							writeGroupEntry(fileBuffer, group);
							writeGroupSequence(fileBuffer, group);
							writeGroupMibIndex(fileBuffer, group, getMaxMibIndex(platforms, group));
							writeGroupElements(fileBuffer, group, String.format("%s-%s-entry", getMibType(group), group.getName()));
						}
						else {
							writeGroupElements(fileBuffer, group, String.format("%s-%s", getMibType(group), group.getName()));
						}
					}
				}
			}
			fileBuffer.write("\nEND\n");
			fileBuffer.close();
			isOk = true;
		}
		catch (IOException e) {
			CommandTool.log(e.getMessage());
		}
		catch (Exception e) {
			CommandTool.log(e.getMessage());
		}
		return isOk;
	}

	private String constructFilePath(String outputDirectory) throws Exception {
		boolean isOk = true;
		final String fileName = "lr54.mib";
		final String directory = "/mib_default/";
		if (outputDirectory == null)
			outputDirectory = ".";
		String directoryPath = outputDirectory + directory;
		File path = new File(directoryPath);
		if (!path.exists() || !path.isDirectory()) {
			if (!path.mkdir())
				throw new Exception("Failed to create directory: " + directoryPath);
		}
		String filePath = directoryPath + fileName;
		return filePath;
	}

	private void writeMibHeader(BufferedWriter outputBuffer) throws IOException {
		String line = null;
		String header = "";
		try {
			String headerFileName = "./mib.header";
			FileReader fileReader = new FileReader(headerFileName);
			BufferedReader bufferedReader = new BufferedReader(fileReader);
			while ((line = bufferedReader.readLine()) != null)
				header = header.concat(line + "\n");
			bufferedReader.close();
		}
		catch (FileNotFoundException e) {
			throw new IOException("Unable to locate MIB header template.");
		}
		catch (IOException e) {
			throw new IOException("Error handling MIB header template.");
		}
		Date date = new Date();
		String dateString = new SimpleDateFormat("yyyyMMddHHmm").format(date);
		header = String.format(header, MODEL_NAME, dateString, dateString);
		try {
			outputBuffer.write(header);
		}
		catch (IOException e) {
			throw new IOException("Unable to write header to MIB file.");
		}
	}

	private void writeGroupIds(LinkedList<ConfigurationGroup> groups, BufferedWriter outputBuffer) throws IOException {
		int setGroupIndex = 1;
		int getGroupIndex = 1;
		outputBuffer.write(String.format("%-24s OBJECT IDENTIFIER ::= { %s %3d }\n",
			CONFIG,
			MODEL_NAME,
			1));
		outputBuffer.write(String.format("%-24s OBJECT IDENTIFIER ::= { %s %3d }\n",
			SHOW,
			MODEL_NAME,
			2));
		outputBuffer.write('\n');

		for (ConfigurationGroup group: groups) {
			ConfigType configType = group.getConfigType();
			if (configType == ConfigType.SET_GROUP) {
				if (group.isNoMib() == false) {
					String groupName = String.format("%s-%s", getMibType(group), group.getName());
					outputBuffer.write(String.format("%-24s OBJECT IDENTIFIER ::= { %s %3d }   -- %s\n",
					 groupName,
					 CONFIG,
					 setGroupIndex++,
					 group.getDescription()
					 ));
				} else
					setGroupIndex++;
			}
			else if (configType == ConfigType.GET_GROUP) {
				if (group.isNoMib() == false) {
					String groupName = String.format("%s-%s", getMibType(group), group.getName());
					outputBuffer.write(String.format("%-24s OBJECT IDENTIFIER ::= { %s %3d }   -- %s\n",
					 groupName,
					 SHOW,
					 getGroupIndex++,
					 group.getDescription()
					 ));

				} else
					getGroupIndex++;

			}
		}
		outputBuffer.write('\n');
	}

	private void writeGroupHeader(BufferedWriter out, ConfigurationGroup group) throws IOException {
		out.write(String.format("--\n-- %s\n--\n\n", getMibDescription(group)));
	}

	private void writeGroupTable(BufferedWriter out, ConfigurationGroup group) throws IOException {
		out.write(String.format("%s-%s-table OBJECT-TYPE\n", getMibType(group), group.getName()));
		out.write(String.format("\t%-16s\tSEQUENCE OF %s-%s\n", "SYNTAX", Character.toUpperCase(getMibType(group).charAt(0))+getMibType(group).substring(1), group.getName()));
		out.write(String.format("\t%-16s\t%s\n", "MAX-ACCESS", getMibAccess(null)));
		out.write(String.format("\t%-16s\t%s\n", "STATUS", getMibStatus(null)));
		out.write(String.format("\t%-16s\t\"%s\"\n", "DESCRIPTION", getMibDescription(group)));
		out.write(String.format("\t::= { %s-%s 1 }\n\n", getMibType(group), group.getName()));
	}

	private void writeGroupEntry(BufferedWriter out, ConfigurationGroup group) throws IOException {
		out.write(String.format("%s-%s-entry OBJECT-TYPE\n", getMibType(group), group.getName()));
		out.write(String.format("\t%-16s\t%s-%s\n", "SYNTAX",  Character.toUpperCase(getMibType(group).charAt(0))+getMibType(group).substring(1), group.getName()));
		out.write(String.format("\t%-16s\t%s\n", "MAX-ACCESS", getMibAccess(null)));
		out.write(String.format("\t%-16s\t%s\n", "STATUS", getMibStatus(null)));
		out.write(String.format("\t%-16s\t\"%s\"\n", "DESCRIPTION", getMibDescription(group)));
		out.write(String.format("\t%-16s\t{ %s-%s-index }\n", "INDEX", getMibType(group), group.getName()));
		out.write(String.format("\t::= { %s-%s-table 1 }\n\n", getMibType(group), group.getName()));
	}

	private void writeGroupSequence(BufferedWriter out, ConfigurationGroup group) throws IOException {
		LinkedList<ConfigurationElement> elements = group.getElements();
		ConfigurationElement lastElement = elements.getLast();
		String elementName;
		out.write(String.format("%s-%s ::= SEQUENCE {\n", Character.toUpperCase(getMibType(group).charAt(0))+getMibType(group).substring(1), group.getName()));
		if (!elements.isEmpty()) {
			elementName = String.format("%s-%s-index", getMibType(group), group.getName());
			out.write(String.format("\t%-32s\tInteger32,\n", elementName));
		}
		for (ConfigurationElement element: elements) {
			elementName = String.format("%s-%s-%s", getMibType(group), group.getName(), element.getName());
			out.write(String.format("\t%-32s\t%s", elementName, getMibType(element)));
			if (element != lastElement)
				out.write(",");
			out.write("\n");
		}
		out.write("}\n\n");
	}

	private void writeGroupMibIndex(BufferedWriter out, ConfigurationGroup group, int mibIndexMax) throws IOException {
		out.write(String.format("%s-%s-index OBJECT-TYPE\n", getMibType(group), group.getName()));
		out.write(String.format("\t%-16s\tInteger32 (1..%d)\n", "SYNTAX", mibIndexMax));
		out.write(String.format("\t%-16s\t%s\n", "MAX-ACCESS", "read-only"));
		out.write(String.format("\t%-16s\t%s\n", "STATUS", "current"));
		out.write(String.format("\t%-16s\t\"%s index\"\n", "DESCRIPTION", group.getUiName()));
		out.write(String.format("\t::= { %s-%s-entry 1 }\n", getMibType(group), group.getName()));
		out.write("\n");
	}

	private int getMaxMibIndex(LinkedList<ConfigurationTag> platforms, ConfigurationGroup group) {
		int max = group.getMaxInstance();
		if (max == 0) {
			if (!group.isMaxTagged())
				max = Integer.MAX_VALUE;
			else {
				String maxTag = group.getMaxTag();
				for (ConfigurationTag platform: platforms)
					if (platform.isPlatformType()) {
						LinkedList<ConfigurationTag> defines = platform.getDefines();
						for (ConfigurationTag define: defines)
							if (define.getName().equals(maxTag)) {
								max = Math.max(max, define.getDefineValue());
							}
					}
			}
		}
		return max;
	}

	private void writeGroupElements(BufferedWriter out, ConfigurationGroup group, String objectId) throws IOException {
		int index = group.isMibIndexed() ? 2 : 1;
		String groupNameFormat = String.format("%s-%s", getMibType(group), group.getName());
		LinkedList<ConfigurationElement> elements = group.getElements();
		for (ConfigurationElement element: elements) {
			out.write(String.format("%s-%s OBJECT-TYPE\n", groupNameFormat, element.getName()));
			out.write(String.format("\t%-16s\t%s\n", "SYNTAX", getMibSyntax(element)));
			out.write(String.format("\t%-16s\t%s\n", "MAX-ACCESS", getMibAccess(element)));
			out.write(String.format("\t%-16s\t%s\n", "STATUS", getMibStatus(element)));
			out.write(String.format("\t%-16s\t\"%s\"\n", "DESCRIPTION", getMibDescription(element)));
			out.write(String.format("\t::= { %s %d }\n", objectId, index++));
			out.write("\n");
		}
	}

	private String capitalizeFirstLetter(String text) {
		if (text == null)
			text = "";
		if (!text.isEmpty())
			text = Character.toUpperCase(text.charAt(0)) + text.substring(1);
		return text;
	}

	private String getMibSyntax(ConfigurationElement element) {
		String syntax;
		String min = element.getMin();
		String max = element.getMax();
		LinkedList<ConfigValue> values;
		ConfigValue lastValue;

		switch (element.getType()) {
			case ENCPASSWORD:
			case PASSWORD:
			case STRING:
				if (min != null)
					syntax = String.format("OCTET STRING (SIZE(%s..%s))", min, max);
				else
					syntax = String.format("OCTET STRING (SIZE(0..%s))", max);
				break;
			case UINT32:
				if (max == null)
					syntax = "Unsigned32";
				else {
					min = (min == null) ? "1" : min;
					syntax = String.format("Unsigned32 (%s..%s)", min, max);
				}
				break;
			case INT32:
			case INDEX:
				if (max == null)
					syntax = "Integer32";
				else {
					min = (min == null) ? "1" : min;
					syntax = String.format("Integer32 (%s..%s)", min, max);
				}
				break;
			case ON_OFF:
				syntax = "INTEGER {\n\t\t\t\toff(0),\n\t\t\t\ton(1)\n\t\t\t\t}";
				break;
			case BOOLEAN:
				syntax = "INTEGER {\n\t\t\t\tfalse(0),\n\t\t\t\ttrue(1)\n\t\t\t\t}";
				break;
			case IPADDR:
				syntax = "IpAddress";
				break;
			case FQDN:
				syntax = "OCTET STRING (SIZE(1..255))";
				break;
			case MAC_ADDR:
				syntax = "OCTET STRING (SIZE(17))";
				break;
			case COUNTER32:
				syntax = "Counter32";
				break;
			case COUNTER64:
				syntax = "Counter64";
				break;
			case INTERFACE:
				syntax = "OCTET STRING (SIZE(1..64))";
				break;
			case ENUM_MULTI:
				values = element.getValues();
				lastValue = values.getLast();
				int max_length = 0;
				for (ConfigValue value: values) {
					String name = value.getName();
					max_length += (name.length() + 1);
				}
				syntax = String.format("OCTET STRING (SIZE(0..%d))", max_length-1);
				break;
			case ENUM:
				syntax = "INTEGER {\n";
				values = element.getValues();
				lastValue = values.getLast();
				for (ConfigValue value: values) {
					String name = value.getName();
					syntax = syntax.concat( "\t\t\t\t" );
					syntax = syntax.concat( Character.isDigit(name.charAt(0)) ? "s" : "" );
					syntax = syntax.concat( String.format("%s(%s)", value.getName(), value.getNumber()) );
					syntax = syntax.concat( (value == lastValue) ? "\n" : ",\n" );
				}
				syntax = syntax.concat("\t\t\t\t}");
				break;
			default:
				syntax = "";
				break;
		}
		return syntax;
	}

	private String getMibAccess(ConfigurationElement element) {
		String access = "not-accessible";
		if (element != null) {
			switch(element.getAccess()) {
				case NONE:
				case NO_ACCESS:
					access = "not-accessible"; break;
				case READONLY:
					access = "read-only"; break;
				case READWRITE:
				case READONLY_SUPER_READWRITE:
				case WRITEONLY: /* SMIv2 does not support write-only*/
					access = "read-write"; break;
				default: break;
			}
		}
		return access;
	}

	private String getMibStatus(ConfigurationElement element) {
		return "current";
	}

	private String getMibDescription(ConfigurationGroup group) {
		String description = group.getDescription();
		if (description == null)
			description = "";
		return description;
	}

	private String getMibDescription(ConfigurationElement element) {
		String description = element.getDescription();
		if (description == null)
			description = "";
		return description;
	}

	private String getMibType(ConfigurationElement element) {
		switch (element.getType()) {
			case STRING:
			case ENCPASSWORD:
			case PASSWORD:
			case FQDN:
			case INTERFACE:
			case MAC_ADDR:
			case ENUM_MULTI:
				return "OCTET STRING";
			case UINT32:
				return "Unsigned32";
			case INDEX:
			case INT32:
				return "Integer32";
			case ENUM:
			case ON_OFF:
			case BOOLEAN:
				return "INTEGER";
			case IPADDR:
				return "IpAddress";
			case COUNTER32:
				return "Counter32";
			case COUNTER64:
				return "Counter64";
			default:
				return "";
		}
	}

	private String getMibType(ConfigurationGroup group) {
		switch (group.getConfigType()) {
			case SET_GROUP:
				return "config";
			case GET_GROUP:
				return "show";
			case ACTION:
				return "action";
			default:
				return "";
		}
	}

}
