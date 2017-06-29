package com.digi.config.command_tool;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.LinkedList;

public class DefaultUCIGenerator implements GeneratorInterface {

	private static final String UCI_DEFAULT_DIRECTORY = "/uci_default/";
	private String fullDirectory;
	private BufferedWriter bufferedWriter;
	private String interfaceName = "Default UCI File Generator";

	public String getOption() {
		return "uci";
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

	public boolean GenerateOutputFile(LinkedList<ConfigurationGroup> groups, LinkedList<ConfigurationTag> tags, String outputDirectory) {
		boolean isOk = true;
		fullDirectory = toDefaultDirectory(outputDirectory);
		CommandTool.log(String.format("Create UCI files in %s directory", fullDirectory));
		try {
			for (ConfigurationGroup group: groups) {
				if (group.getConfigType() == ConfigType.SET_GROUP) {
					String filename = fullDirectory + group.getUciName();
					File file = new File (filename);
					if (file.isFile()) {
						CommandTool.debug_log("Deleting the existing " + filename + " default UCI file.");
						if (!file.delete())
							throw new IOException ("Unable to delete the existing " + filename + " default UCI file");
					} else if (file.exists())
						throw new IOException ("Unable to delete the existing " + filename + " default UCI file");
				}
			}
			for (ConfigurationGroup group: groups) {
				if (group.getConfigType() == ConfigType.SET_GROUP) {
					CommandTool.debug_log("Generating default UCI file for " + group.getName() + "in" + fullDirectory);
					openFileWriter(fullDirectory, group.getUciName());
					String string = String.format("\nconfig %s %s%d\n", 
													group.getName().replace('-', '_'), 
													group.getName().replace('-', '_'),
													1);
					for (ConfigurationElement element: group.getElements()) {
						String defaultValue = element.getDefaultValue();
						if (defaultValue != null) {
							switch (element.getType()) {
								case STRING:
								case PASSWORD:
								case ENCPASSWORD:
									string += String.format("  option %s \"%s\"\n", 
															element.getName().replace('-', '_'),
															defaultValue);
									break;
								case ENUM_MULTI:
									String[] default_array = defaultValue.split(",");
									
									for (String each_default : default_array) {
										CommandTool.debug_log(String.format("list %s '%s'\n", element.getName().replace('-', '_'), each_default));
										string += String.format("  list %s %s\n", element.getName().replace('-', '_'), each_default);
									}
									
									break;
								default:
									string += String.format("  option %s \"%s\"\n", 
											element.getName().replace('-', '_'),
											defaultValue);
									break;
							}
						}
					}
					
					int tag_count = 0;
					
					for (ConfigurationElement element: group.getElements()) {
						for (ConfigValue value : element.getValues()) {
							for (String tag : value.getTags()) {
								if (tag_count == 0)
									string += String.format("config tag tag1\n");
								
								CommandTool.debug_log(" UCI tag " + tag + "\n");
								string += String.format("  option %s 0\n", toNormalizeString(tag));
								tag_count++;
							}
						}
						
						for (String tag : element.getTags()) {
							if (tag_count == 0)
								string += String.format("config tag tag\n");
							
							CommandTool.debug_log(" UCI tag " + tag + "\n");
							string += String.format("  option %s 0\n",  toNormalizeString(tag));
							tag_count++;
						}
					}					
					writeFile(string);
					closeFileWriter();
				}
			}
		}
		catch (IOException e) {
			if (e.getMessage() != null)
				CommandTool.log(e.getMessage());
			isOk = false;
		}
		return isOk;
	}

	private String toDefaultDirectory(String outputDirectory) {
		String fullpath = "." + UCI_DEFAULT_DIRECTORY;
		if (outputDirectory != null)
			fullpath = outputDirectory  + UCI_DEFAULT_DIRECTORY;
		File path = new File(fullpath);
		if (!path.exists() || !path.isDirectory())
			if (!path.mkdir())
				CommandTool.log("Failed to create directory: " + fullpath);
		return (fullpath);
	}

	private void openFileWriter(String directory, String filename) throws IOException {
		String fullname = directory +filename;
		try {
			CommandTool.debug_log("Generating " + fullname + " file");

			bufferedWriter = new BufferedWriter(new FileWriter(fullname, true));
		}
		catch (IOException e) {
			throw new IOException ("Unable to create default UCI " + fullname + " file");
		}
	}

	private void closeFileWriter() throws IOException {
		try {
			bufferedWriter.close();
		}
		catch (IOException e) {
			throw new IOException ("Unable to close default UCI file");
		}
	}

	private void writeFile(String string) throws IOException {
		try {
			bufferedWriter.write(string);
		}
		catch (IOException e) {
			throw new IOException ("Writing \"" + string + "\" failed");
		}
	}
	
	private String toNormalizeString(String string) {
//		return string.replaceAll("^[!@#$%^*(, replacement),.<>~`[]{}\\/+=-\\s]*$", "_");
		return string.replaceAll("\\W", "_");
	}

}
