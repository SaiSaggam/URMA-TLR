package com.digi.config.command_tool;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.LinkedList;
import java.util.ArrayList;

public class DocumentGenerator implements GeneratorInterface {

	public boolean GenerateOutputFile(LinkedList<ConfigurationGroup> groups, LinkedList<ConfigurationTag> platforms, String outputDirectory) {
		boolean isOk = false;
		try {
			String directory = constructDirectory(outputDirectory);
			for (ConfigurationGroup group: groups) {
				String fileName = String.format("R_TLR_cli_%s_%s.htm", getPrettyConfigType(group), group.getName());
				String filePath = directory + fileName;
				FileWriter fileWriter = new FileWriter(filePath);
				BufferedWriter fileBuffer = new BufferedWriter(fileWriter);
				fileBuffer.write(getFileHeader());
				fileBuffer.write(getFileBody(group, platforms));
				fileBuffer.write(getFileFooter());
				fileBuffer.close();
			}
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

	private String constructDirectory(String outputDirectory) throws Exception {
		final String directory = "/reference_docs/";
		if (outputDirectory == null)
			outputDirectory = ".";
		String directoryPath = outputDirectory + directory;
		File path = new File(directoryPath);
		if (!path.exists() || !path.isDirectory())
			if (!path.mkdir())
				throw new Exception("Failed to create directory: " + directoryPath);
		return directoryPath;
	}

	private String getFileHeader() {
		StringBuilder output = new StringBuilder();
		output.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
		output.append("<html xmlns:MadCap=\"http://www.madcapsoftware.com/Schemas/MadCap.xsd\" MadCap:lastBlockDepth=\"3\" MadCap:lastHeight=\"666\" MadCap:lastWidth=\"687\">\n");
		output.append("\t<body>\n");
		output.append("\t\t<div class=\"command\">\n");
		return output.toString();
	}

	private String getFileFooter() {
		StringBuilder output = new StringBuilder();
		output.append("\t\t</div>\n");
		output.append("\t</body>\n");
		output.append("</html>\n");
		return output.toString();
	}

	private String getFileBody(ConfigurationGroup group, LinkedList<ConfigurationTag> platforms) {
		StringBuilder output = new StringBuilder();
		CommandTool.debug_log("getFileBody:" + group.getName());

		output.append(String.format("\t\t\t<h1>%s%s</h1>\n", (group.getConfigType() == ConfigType.GET_GROUP) ? "show " : "", group.getName()));
		output.append(String.format("\t\t\t<p>%s</p>\n", group.getDocumentDescription()));
		output.append(getSupportStatement(group, platforms));
		
		if (group.getConfigType() != ConfigType.GET_GROUP) {
			output.append(getPrettyAccess(group));
			output.append(getPrettySyntax(group, platforms));
		}
		
		output.append(getElements(group));
		output.append(getExamples(group));
		return output.toString();
	}

	private String getSupportStatement(ConfigObject object, LinkedList<ConfigurationTag> platforms) {
		StringBuilder output = new StringBuilder();
		ArrayList<String> tags = getPrettyTags(object, platforms);
		int init_size = tags.size();
		if (init_size == 0)
			return "";
		String className = (object instanceof ConfigurationGroup) ? "group" : "parameter";
		output.append(String.format("\t\t\t<p>This %s is only supported in ", className));
		while (true) {
			String tag = tags.remove(0);
			if (tags.size() > 1)
				output.append(String.format("%s, ", tag));
			else if (tags.size() > 0)
				output.append(String.format("%s and ", tag));
			else {
				output.append(String.format("%s ", tag));
				break;
			}
		}
		output.append((init_size > 1) ? "products.</p>\n" : "product.</p>\n");
		return output.toString();
	}

	private ArrayList<String> getPrettyTags(ConfigObject object, LinkedList<ConfigurationTag> platforms) {
		ArrayList<String> prettyTags = new ArrayList<String>();
		ArrayList<String> rawTags = object.getTags();
		for (String rawTag: rawTags)
			for (ConfigurationTag platform: platforms)
				if (platform.isPlatformType()) {
					LinkedList<ConfigurationTag> defines = platform.getDefines();
					for (ConfigurationTag define: defines)
						if (define.getName().equals(rawTag))
							if (!prettyTags.contains(platform.getDescription()))
								prettyTags.add(platform.getDescription());

				}
		return prettyTags;
	}

	private String getPrettySyntax(ConfigurationGroup group, LinkedList<ConfigurationTag> platforms) {
		StringBuilder output = new StringBuilder();
		StringBuilder syntax = new StringBuilder();
		output.append(String.format("\t\t\t<h2>Syntax</h2>\n"));
		output.append("\t\t\t<pre class=\"prettyprint\">");
		switch(group.getConfigType()) {
			case SET_GROUP:
				syntax.append(String.format("%s ", group.getName()));
				int maxInstance = group.getMaxInstance();
				if (maxInstance == 0) {
					String maxTag = group.getMaxTag();
					for (ConfigurationTag platform: platforms)
						if (platform.isPlatformType()) {
							LinkedList<ConfigurationTag> defines = platform.getDefines();
							for (ConfigurationTag define: defines)
								if (define.getName().equals(maxTag))
									maxInstance = Math.max(maxInstance, define.getDefineValue());
						}
				}
				if (maxInstance != 1)
					syntax.append(String.format("<1 - %d> ", maxInstance));
				syntax.append(String.format("<parameter> <value>"));
				break;
			case GET_GROUP:
				syntax.append(String.format("show %s", group.getName()));
				break;
			case ACTION:
				syntax.append(String.format("%s %s", group.getName(), group.getSyntax()));
				break;
			default:
				break;
		}
		output.append(syntax.toString().replaceAll("<", "&lt;").replaceAll(">", "&gt;"));
		output.append("</pre>\n");
		return output.toString();
	}

	private String getElements(ConfigurationGroup group) {
		StringBuilder output = new StringBuilder();
		String title = "Parameters";
		output.append(String.format("\t\t\t<h2>%s</h2>\n", title));
		LinkedList<ConfigurationElement> elements = group.getElements();
		for (ConfigurationElement element: elements) {

			output.append(String.format("\t\t\t<h3>%s</h3>\n", element.getName()));
			
			// Check to see if there is a long description
			if (element.getHelpDescription() != null)
			{
				output.append(String.format("\t\t\t<p>%s</p>\n", element.getHelpDescription()));
			} 
			else
			{
				output.append(String.format("\t\t\t<p>%s</p>\n", element.getDescription()));
			}
			
			if (group.getConfigType() == ConfigType.SET_GROUP) {
				output.append(getPrettyAccess(element));
				output.append(getPrettyType(element));
			}
		}
		return output.toString();
	}

	private String getPrettyType(ConfigurationElement element) {
		StringBuilder output = new StringBuilder();
		StringBuilder content = new StringBuilder();
		LinkedList<ConfigValue> values;
		long min = 0;
		long max = 0;
		String default_value;
		if (element.isOptional())
			content.append("This parameter is optional.");
		if (content.length() != 0)
			content.append(" ");
		switch (element.getType()) {
			case STRING:
			case PASSWORD:
			case ENCPASSWORD:
			case COUNTER32:
			case COUNTER64:
				try {
					min = Long.parseLong(element.getMin());
				}
				catch (NumberFormatException e) {
					min = 0;
				}
				max = Long.parseLong(element.getMax());
				if (min > 0)
					content.append(String.format("Accepted value is any string between %d and %d characters.", min, max));
				else
					content.append(String.format("Accepted value is any string up to %d characters.", max));
				break;
			case UINT32:
			case INT32:
				min = Long.parseLong(element.getMin());
				max = Long.parseLong(element.getMax());
				content.append(String.format("Accepted value is any integer from %d to %d.", min, max));
				break;
			case ENUM:
				values = element.getValues();
				content.append("Accepted values can be one of ");
				while (true) {
					String value = values.removeFirst().getName();
					if (values.size() > 1)
						content.append(String.format("%s, ", value));
					else if (values.size() > 0)
						content.append(String.format("%s or ", value));
					else {
						content.append(String.format("%s.", value));
						break;
					}
				}
				break;
			case ENUM_MULTI:
				values = element.getValues();
				content.append("Accepted values can be multiple values of ");
				while (true) {
					String value = values.removeFirst().getName();
					if (values.size() > 1)
						content.append(String.format("%s, ", value));
					else if (values.size() > 0)
						content.append(String.format("%s and ", value));
					else {
						content.append(String.format("%s.", value));
						break;
					}
				}
				break;
			case BOOLEAN:
				content.append("Value is either true or false.");
				break;
			case ON_OFF:
				content.append("Value is either on or off.");
				break;
			case IPADDR:
				content.append("Value should be an IPv4 address.");
				break;
			case MAC_ADDR:
				content.append("Value should be a MAC address.");
				break;
			case FQDN:
				content.append("Value should be a fully qualified domain name.");
				break;
			case INTERFACE:
				content.append("Value should be an interface name.");
				break;
			case INDEX:
			case NONE:
			default:
				break;
		}
		if (element.getDefaultValue() != null) {
			if (content.length() != 0)
				content.append(" ");
			content.append(String.format("The default value is %s.", element.getDefaultValue()));
		}
		if (content.length() != 0) {
			output.append(String.format("\t\t\t<p>"));
			output.append(content.toString());
			output.append(String.format("</p>\n"));
		}
		return output.toString();
	}

	private String getExamples(ConfigurationGroup group) {
		StringBuilder output = new StringBuilder();
		ArrayList<ConfigExample> examples = group.getExamples();
		if (examples.size() == 0)
			return "";
		output.append("\t\t\t<h2>Examples</h2>\n");
		output.append("\t\t\t<ul>\n");
		for (ConfigExample example: examples) {
			output.append("\t\t\t\t<li>\n\t\t\t\t\t<pre xml:space=\"preserve\">");
			output.append(String.format("%s", example.getText().replaceAll("<", "&lt;").replaceAll(">", "&gt;")));
			output.append("</pre>\n\t\t\t\t\t<p>");
			output.append(String.format("%s", example.getDescription().replaceAll("<", "&lt;").replaceAll(">", "&gt;")));
			output.append("</p>\n\t\t\t\t</li>\n");
		}
		output.append("\t\t\t</ul>\n");
		return output.toString();
	}

	public String getOption() {
		return "doc";
	}

	public String[] getGeneratorOption() {
		String[] options = new String[0];
		return options;
	}
	
	public boolean setGeneratorOption(String options) {
		return false;
	}

	public String getName() {
		return "Reference Document Generator";
	}

	private String getPrettyConfigType(ConfigurationGroup group) throws Exception {
		switch(group.getConfigType()) {
			case SET_GROUP:
				return "config";
			case GET_GROUP:
				return "show";
			case ACTION:
				return "action";
			default:
				throw new Exception(String.format("ConfigType for group %s not recognized", group.getConfigType()));
		}
	}

	private String getPrettyAccess(ConfigRow object) {
		StringBuilder output = new StringBuilder();
		String className = (object instanceof ConfigurationGroup) ? "command" : "element";
		output.append(String.format("\t\t\t<p>This %s ", className));
		String accessDescription = "is not accessible to any user";
		AccessType access = object.getAccess();
		switch(access) {
			case READONLY:
				accessDescription = "is read-only for all users";
				break;
			case READWRITE:
				// accessDescription = "can be configured by all users";
				// break;
				return "";
			case READONLY_SUPER_READWRITE:
				// accessDescription = "configurable for super users and read-only for others";
				accessDescription = "is available to super users only";
				break;
			case WRITEONLY:
				accessDescription = "is available to all users";
				break;
			case NONE:
			case NO_ACCESS:
			default:
				break;
		}
		output.append(String.format("%s.</p>\n", accessDescription));
		return output.toString();
	}

}
