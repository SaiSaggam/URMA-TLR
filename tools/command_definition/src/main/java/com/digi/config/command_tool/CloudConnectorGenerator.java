package com.digi.config.command_tool;

import java.io.BufferedWriter;
import java.io.Console;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.LinkedList;
import java.util.Scanner;

/**
 * Interface between the parser and the generator modules It generates config
 * file for Cloud Connector.
 * 
 */
public class CloudConnectorGenerator implements GeneratorInterface {

	private final static long FIRMWARE_VERSION_MAX_VALUE = 4294967295L;
	
	private static final String CONFIG_RCI_FILE = "config.rci";
	private static final String CONFIG_GENERATOR_JAR = "ConfigGenerator.jar";
	
	private static final String GLOBAL_ERRORS = "globalerror load_fail \"Load fail\"\n"
											  + "globalerror save_fail \"Save fail\"\n"
											  + "globalerror memory_fail \"Insufficient memory\"\n";

	private static final String DEVICE_CLOUD_USERNAME = "DEVICE_CLOUD_USERNAME";
	private static final String DEVICE_CLOUD_PASSWORD = "DEVICE_CLOUD_PASSWORD";
	private static final String DEVICE_CLOUD_URL	  = "DEVICE_CLOUD_URL";
	//private static final String DEVICE_CLOUD_TYPE	 = "DEVICE_CLOUD_TYPE";
	private static final String SOFTWARE_FIRMWARE_VERSION = "SOFTWARE_FIRMWARE_VERSION";
	
	private static final String CLOUD_CONNECTOR_SETTING = "devicecloud";
	private static final String CLOUD_CONNECTOR_CLOUD_URL = "cloudurl";
	//private static final String CLOUD_CONNECTOR_DEVICE_TYPE = "devicetype";
	
	/* use DEVICE_INFO_SETTING to get firmware version if SOFTWARE_FIRMWARE_VERSION is not specified in platform file */
	private static final String DEVICE_INFO_SETTING		 = "deviceinfo";
	private static final String DEVICE_INFO_SOFTWARE_VERSION = "sw_version";
   
	private String interfaceName = "Cloud Connector Generator";
	private static final String[] GENERATOR_OPTIONS = {"-noUpload", "-vendor", "-url"}; 
	/* parameters to connector generator. <username:password> <device_type> <fw_version> */
	private static final String[] GENERATOR_PARAMETER_OPTIONS = {"-username", "-device_type", "-fw_version"};

	private static final int GENERATOR_PARAMETER_USERNAME_INDEX = 0;
	private static final int GENERATOR_PARAMETER_DEVICE_TYPE_INDEX = 1;
	private static final int GENERATOR_PARAMETER_FW_VERSION_INDEX = 2;
	
	private String username;
	private String password;
	private String url;
	private String vendor_id = "0xFE000009";
	private String device_type;
	private String fw_version;
	private BufferedWriter bufferedWriter;
	private ArrayList<String> rci_options = new ArrayList<String>();
	private String[] rci_parameters = new String[] {"", "", ""};
		
	/**
	 * Method for command tool option to trigger this interface 
	 * @return Name	 Name of the interface
	 */
	public String getOption() {
		return "rci";
	}
	
	/**
	 * Method for its own generator's options
	 */
	public String[] getGeneratorOption() {
		return GENERATOR_OPTIONS;
	}
	
	public boolean setGeneratorOption(String option) {
		
		boolean isOption = false;
		
		/* look for "rci" option */
		if (option.startsWith(getOption()))
		{
			String this_option = option.substring(getOption().length());
			String[] keys = this_option.split("=", 2);
		
			if (keys.length == 2 && Arrays.asList(GENERATOR_PARAMETER_OPTIONS).contains(keys[0])) {
				
				int i=0;
				for (String parameter : GENERATOR_PARAMETER_OPTIONS) {
					if (parameter.equals(keys[0])) {
						this.rci_parameters[i] = keys[1];
						break;
					}
					i++;
				}
				isOption = true;
			}
			/* accept any option with "rci-" end with a dash */
			else if (this_option.startsWith("-")){
//			else if (Arrays.asList(GENERATOR_OPTIONS).contains(keys[0])) {
				this.rci_options.add(this_option);
				isOption = true;
			}
	
		}		
		return isOption;
	}
	/**
	 * Method for the name of the interface.
	 * @return Name	 Name of the interface
	 */
	public String getName() {
		return interfaceName;
	}

	private String toNormalizeString(String string) {
		return string.replaceAll("\\W", "_");
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
		
		CommandTool.debug_log("\nStart " + interfaceName + "\n");
		
		try {

			if (platforms.size() == 0)
				isOk = false;
			
			else {
				parseCloudConnectorParameters(groups, platforms);
				
				/* create platform to include all groups and shows
				 * No descriptor uploaded. 
				 */
				ConfigurationTag tlr = new ConfigurationTag("tlr", "tlr platforms", true);
				
				for (ConfigurationTag platform : platforms) {
					
					for (ConfigurationTag define : platform.getDefines()) {
						ConfigurationTag tlr_define = CommandTool.getTag(tlr.getDefines(), toNormalizeString(define.getName()));
						
						if (tlr_define != null) {
							int value = define.getDefineValue();
							if (value > tlr_define.getDefineValue())
								tlr_define.setDefineValue(value);
						}
						else {
							ConfigurationTag new_define = new ConfigurationTag(toNormalizeString(define.getName()), define.getDescription(), true);
							new_define.setDefineValue(define.getDefineValue());
							tlr.addDefines(new_define);
						}
						
					}
				}
				CommandTool.log("Platform: " + tlr.getDescription());
				
				device_type = tlr.getDescription();
				outputConfigRCIFile(groups, tlr);
				outputConfigGeneratorSource(false);
				
				/* Platform with descriptors uploaded to device cloud */
				
				if (!rci_parameters[1].isEmpty()) {
					String[] devices = rci_parameters[1].split(",");
					ConfigurationTag tag = tlr;
					
					for (String device : devices) {
						device_type = device.replaceAll("\\s+", "");
						
						for (ConfigurationTag platform : platforms) {
							
							if (!platform.isPlatformType())
								continue;
							
							if (device_type.equals(platform.getDescription()) ||
								device_type.equals(platform.getName())) {
								tag = platform;
								device_type = platform.getDescription();
								CommandTool.log("Found Platform: " + platform.getName());
								break;
							}
						}
						
						CommandTool.log("Platform (device type): " + device_type);
						outputConfigRCIFile(groups, tag);
						
						//outputConfigGeneratorHeader(outputDirectory);
						outputConfigGeneratorSource(true);
						
					}
					
				} else {
				
					for (ConfigurationTag platform : platforms) {
						
						if (!platform.isPlatformType())
							continue;
						
						device_type = platform.getDescription();
						CommandTool.log("Platform (device type): " + device_type);
						CommandTool.log("Platform: " + platform.getName());
						ConfigurationTag tag = platform;
						outputConfigRCIFile(groups, tag);
				
						//outputConfigGeneratorHeader(outputDirectory);
						outputConfigGeneratorSource(true);
					} 
				}				
				
			}
			
		} catch (IOException e) {
			CommandTool.log(e.getMessage());
			isOk = false;
		}
		return isOk;
	}

	private void outputConfigRCIFile(LinkedList<ConfigurationGroup> groups, ConfigurationTag platform) throws IOException {
		
		String filename;
		
		//if (outputDirectory != null)
			//filename = outputDirectory + "/" + CONFIG_RCI_FILE;
		//else
			filename = CONFIG_RCI_FILE;
		
		CommandTool.debug_log("Generating " + filename + " file");

		try {
			
			bufferedWriter = new BufferedWriter(new FileWriter(filename));
			
			try {

				writeHeaderComment();
				
				writeFile(GLOBAL_ERRORS);
				
				for (ConfigurationGroup group: groups) {
					String string = null;
					
					switch (group.getConfigType()) {
					case SET_GROUP:
						string = "group setting ";
						break;
					case GET_GROUP:
						string = "group state ";
						break;
					default:
						break;
					}
					
					int enabled = 0;
					for (String tag_name : group.getTags()) {
						ConfigurationTag tag = CommandTool.getTag(platform.getDefines(), toNormalizeString(tag_name));
						if (tag != null) enabled += tag.getDefineValue();
					}
					
					if ((enabled == 0) && (group.getTags().size() > 0)) {
						string = null; /* not enabled for this platform */
					}
					
					if (string != null) {
						
						string += toNormalizeString(group.getName());
						
						int row = toNumberInstances(group, platform.getDefines()); 
						if (row > 1)
							string += " " + row; 
						string += " ";
						
						string += String.format("\"%s\" \"%s\"\n", group.getUiName(), group.getDescription());
						
						for (ConfigurationElement element: group.getElements()) {
							
							enabled = 0;
							for (String tag_name : element.getTags()) {
								ConfigurationTag tag = CommandTool.getTag(platform.getDefines(), toNormalizeString(tag_name));
								if (tag != null) enabled += tag.getDefineValue();
							}
							
							if ((enabled > 0) || (element.getTags().size() == 0)) { 
								string += String.format("	element %s \"%s\" type %s access %s", 
														 toNormalizeString(element.getName()), element.getDescription(), 
														 toRciElementType(element.getType()),
														 toRciAccessType(element.getAccess()));
							
								if (element.getMin() != null) 
									string += " min " + element.getMin();
								
								if (element.getMax() != null) 
									string += " max " + element.getMax();
								
								string += "\n";
								
								if (element.getType() == ElementType.ENUM) {
									for (ConfigValue value: element.getValues()) {
										
										enabled = 0;
										for (String tag_name : value.getTags()) {
											ConfigurationTag tag = CommandTool.getTag(platform.getDefines(), toNormalizeString(tag_name));
											if (tag != null) enabled += tag.getDefineValue();
										}
										
										if ((enabled > 0) || (value.getTags().size() == 0)) { 
											string += String.format("		value %s \"%s\"\n", toNormalizeString(value.getName()), value.getDescription());
										}
									}
								}
							}
						}
						
						for (ConfigurationElement element1: group.getElements()) {
							enabled = 0;
							for (String tag_name : element1.getTags()) {
								ConfigurationTag tag = CommandTool.getTag(platform.getDefines(), toNormalizeString(tag_name));
								if (tag != null) enabled += tag.getDefineValue();
							}
							
							if ((enabled > 0) || (element1.getTags().size() == 0)) { 
								ConfigError error = element1.getError();
							
								if (error != null) {
									string += String.format("	error %s \"%s\"\n", toNormalizeString(error.getName()), error.getDescription());
								}
							}
						}
						
						string += "\n";
						writeFile(string);
					}
					
				}
				//if (outputDirectory != null)
					//CommandTool.log(String.format("%s is generated in %s directory.\n", CONFIG_RCI_FILE, outputDirectory));
				//else
					CommandTool.log(String.format("%s is generated.\n", CONFIG_RCI_FILE));
			}
			
			finally {
				bufferedWriter.close();
			}
		} catch (FileNotFoundException e) {
			CommandTool.log("Error generating " + e.getMessage());
			
		} catch (IOException e) {
			CommandTool.log("unable to generate: " + CONFIG_RCI_FILE);
			CommandTool.log(e.getMessage());
		}

		
	}
	
	private void outputConfigGeneratorSource(boolean useBinLog) throws IOException {
		
		
		
		String filename = toConfigGenerator();
		
		File file = new File(filename);
		if (!file.exists())  
			throw new IOException ("Error found: " + file.getAbsolutePath() + " not found!");

		CommandTool.debug_log("Execute " + filename);

		ArrayList<String> options = new ArrayList<String>();
		
		options.add("java");
		options.add("-jar");
		options.add(filename);
		
		if (useBinLog == true)
			options.add("-useBinIdLog");
		else
			options.add("-createBinIdLog");
		
		String outputDirectory = CommandTool.getOption(CommandTool.RCI_JAR_OUTPUT_OPTION);
		
		for (String rci_option : rci_options) {
			if (rci_option.contains("-url")) {
				url = null;
			}
			else if (rci_option.contains("-path")) {
				outputDirectory = null;
			}
			else if (rci_option.contains("-vendor")) {
				vendor_id = null;
			}

			options.add(rci_option);
		}

		//options.add("-noUpload");		
		//options.add("-type=SOURCE");
		//options.add("-usenames=all");
		options.add("-noBackup");
		options.add("-saveDescriptors");
		
		if (outputDirectory != null)
			options.add("-path=" + outputDirectory);
		
		if (url != null)
			options.add("-url=" + url);
		
		if (vendor_id != null)
			options.add("-vendor=" + vendor_id);

		/* Add all options above this line.
		 * 
		 * Parameters of the connector:
		 */
/*		
		if (rci_parameters[0].isEmpty()) {
			if (username != null) {
				if (password != null)
					options.add(username + ":" + password);
				else
					options.add(username);
			}
		} else {
			options.add(rci_parameters[0]);
		}
		

		options.add(device_type);
		
		if (rci_parameters[2].isEmpty()) {
			options.add(fw_version);
		} else {
			options.add(rci_parameters[2]);
		}
*/
		if (username != null) {
			if (password != null)
				options.add(username + ":" + password);
			else
				options.add(username);
		}
		
		options.add(device_type);
		options.add(fw_version);
		options.add(CONFIG_RCI_FILE);
		
		for (String option : options) {
			CommandTool.debug_log(String.format("\nOption: \"%s\"", option));
		}
		
		String[] arguments = new String[options.size()];
		arguments = options.toArray(arguments);

		runConfigGeneratore(arguments);
	}

	private void outputConfigGeneratorHeader() throws IOException {
		
		String filename = toConfigGenerator();

		File file = new File(filename);
		if (!file.exists())  
			throw new IOException ("Error found: " + file.getAbsolutePath() + " not found!");

		ArrayList<String> options = new ArrayList<String>();
		
		options.add("java");
		options.add("-jar");
		options.add(filename);
		
		
		if (CommandTool.getVerboseOption()) 
			options.add("-verbose");
		
		options.add("-type=GLOBAL_HEADER");
		options.add("-noBackup");
		options.add("-saveDescriptors");
		
		String outputDirectory = CommandTool.getOption(CommandTool.RCI_JAR_OUTPUT_OPTION);
		if (outputDirectory != null)
			options.add("-path=" + outputDirectory);
		
		String[] arguments = new String[options.size()];
		arguments = options.toArray(arguments);
		
		runConfigGeneratore(arguments);
	}
	
	private void runConfigGeneratore(String[] command_arguments) throws IOException {
		
		try {

			Process configGenerator = Runtime.getRuntime().exec(command_arguments);
			configGenerator.waitFor();
			java.io.InputStream stream = configGenerator.getInputStream();
			byte output[] = new byte[stream.available()];
			stream.read(output, 0, output.length);
			
			String response = new String(output);
			if (!response.contains("Done."))
				throw new IOException("Error running Cloud Connector: " + CONFIG_GENERATOR_JAR + "\n" + response);
			
			CommandTool.debug_log(response);
			CommandTool.log("Done Cloud Connector Generator");
			
		} catch (SecurityException e) {
			CommandTool.log(e.getMessage());
			throw new IOException("Security permission error :" + CONFIG_GENERATOR_JAR);
			
		} catch (InterruptedException e) {
			CommandTool.log(e.getMessage());
			throw new IOException("Unable to execute (interrupted): " + CONFIG_GENERATOR_JAR);
			
		} catch (IOException e) {
			CommandTool.log(e.getMessage());
			throw new IOException(e.getMessage());
		}
		
	}
	
	private String toRciElementType(ElementType type) throws IOException{
		
		String string = null;
		
		switch (type) {
		case ENCPASSWORD:
			/* TODO: cloud doesn't support encpassword type */
			string = ElementType.PASSWORD.toString().toLowerCase();
			break;
		case PASSWORD:
		case UINT32:
		case INT32:
		case ENUM:
		case ON_OFF:
		case BOOLEAN:
		case STRING:
			string = type.toString().toLowerCase();
			break;
		case IPADDR:
			string = "ipv4";
			break;
		case FQDN:
			string = "fqdnv6";
			break;
		case ENUM_MULTI:
		case MAC_ADDR:
		case COUNTER64:
		case INTERFACE:
			string = "string";
			break;
		case COUNTER32:
			string = "uint32";
			break;
		case INDEX:
			string = "int32";
			break;
			
		case NONE:
			throw new IOException("Unsupported RCI element type: %s" + type.toString());

		}
		return string;
	}
	
	private String toRciAccessType(AccessType type) throws IOException {
		
		String string = null;
		
		switch (type) {
		case READONLY_SUPER_READWRITE:
		case READWRITE:
			string = "read_write";
			break;
		case READONLY:
			string = "read_only";
			break;
		case WRITEONLY:
			string = "write_only";
			break;
		case NO_ACCESS:
			string = "no_access";
			break;
		case NONE:
			throw new IOException("Unsupported RCI access type: %s" + type.toString());
		}
		return string;
	}
	
	private void writeFile(String string) throws IOException {
		
		try {
			bufferedWriter.write(string);
		} catch (IOException e) {
			throw new IOException ("Writing \"" + string + "\" failed");
		}
	}

	private void writeHeaderComment() throws IOException {
		
		try {
			
			String className = CommandTool.class.getName();
			DateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
			Date date = new Date();
			
			String comment = "#\n# This is an auto-generated file - DO NOT EDIT! \n";
			
			comment += String.format("# This is a file generated by %s tool.\n", className)
			+ String.format("# This file was generated on: %s \n", dateFormat.format(date))
			+ String.format("# The command definition syntax file: %s\n", CommandTool.getCommandFileName())
			//+ String.format("# The platform file: %s\n", CommandTool.getOption(CommandTool.PLATFORM_FILE_OPTION))
			+ String.format("# The version of %s tool was: %s */\n\n", className, CommandTool.VERSION);

			bufferedWriter.write(comment);
			
		} catch (IOException e) {
			throw new IOException ("Unable to write comment");
		}
	}
 
	private String queryPassword() throws IOException {
		
		Console console = System.console();
		  
		if (console == null) 
			throw new IOException("Couldn't get Console instance, maybe you're running this from within an IDE?");
		
		char passwordArray[] = console.readPassword("Enter password: ");
		
		if (passwordArray.length == 0) {
			throw new IOException("You must enter a password.\nPlease try again!");
		}
		
		return new String(passwordArray);
		
	  }
	
	private void parseCloudConnectorParameters(LinkedList<ConfigurationGroup> groups, LinkedList<ConfigurationTag> tags) throws IOException {
				
		if (!rci_parameters[GENERATOR_PARAMETER_USERNAME_INDEX].isEmpty()) {
			String[] keys = rci_parameters[GENERATOR_PARAMETER_USERNAME_INDEX].split(":");
			username = keys[0];
			if (keys.length == 2)
				password = keys[1];
			
		}
		
		if (username == null) {
			ConfigurationTag tag = CommandTool.getTag(tags, DEVICE_CLOUD_USERNAME);
			if (tag == null) {
				throw new IOException("No username is specified for Cloud Connector support.");
			}
			username = tag.getStringValue();
		}
		
		if (password == null) {
			ConfigurationTag tag = CommandTool.getTag(tags, DEVICE_CLOUD_PASSWORD);
			if (tag == null) {
				password = queryPassword();
			}
			else
				password = tag.getStringValue();
		}
		
		/* cloud URL */
		ConfigurationTag tag = CommandTool.getTag(tags, DEVICE_CLOUD_URL);
		if (tag == null) {
			ConfigurationGroup group = CommandTool.getGroup(groups, CLOUD_CONNECTOR_SETTING);
			if (group != null) {
				ConfigurationElement element = CommandTool.getElement(group.getElements(), CLOUD_CONNECTOR_CLOUD_URL);
				if (element != null) 
					url = element.getDefaultValue();
			}
			
		} else {
			url = tag.getStringValue();
		}
		
		
		/* firmware version */
		if (!rci_parameters[GENERATOR_PARAMETER_FW_VERSION_INDEX].isEmpty()) {
			fw_version = rci_parameters[GENERATOR_PARAMETER_FW_VERSION_INDEX];
		} 
			
		if (fw_version == null) {
			tag = CommandTool.getTag(tags, SOFTWARE_FIRMWARE_VERSION);
			if (tag == null) {
				ConfigurationGroup group = CommandTool.getGroup(groups, DEVICE_INFO_SETTING);
				if (group == null)
					throw new IOException(SOFTWARE_FIRMWARE_VERSION + " and " + DEVICE_INFO_SETTING + " not found.");
				
				ConfigurationElement element = CommandTool.getElement(group.getElements(), DEVICE_INFO_SOFTWARE_VERSION);
				if (element == null)
					throw new IOException("No firmware version is specified in" + DEVICE_INFO_SETTING + "configuration.");
					
				fw_version = element.getDefaultValue();
	
				if (fw_version == null)
					throw new IOException("No default firmware version type is specified in " + DEVICE_INFO_SETTING + " configuration.");
				
			} else {
				fw_version = tag.getStringValue();
			}
		}	
	
		toFirmwareVersion(fw_version);
	}
	
	private int toNumberInstances(ConfigurationGroup group, LinkedList<ConfigurationTag> defines) 
	{
		int row = group.getNumRows();
		
		if (group.getTagNumRow() != null) {
			ConfigurationTag tag = CommandTool.getTag(defines, group.getTagNumRow());
			if (tag != null)
				row = tag.getDefineValue(); 
		}
		
		return row;
	}
	
	private String toConfigGenerator() throws IOException
	{
		String path = CommandTool.getOption(CommandTool.RCI_JAR_OPTION);
		
		String tool = CONFIG_GENERATOR_JAR;
		
		if (path != null)
			tool = path + "/" + CONFIG_GENERATOR_JAR;

		return tool;
	}
	
	private long toFirmwareVersion(String string) throws IOException {
		/* firmware version */
		long version = 0;
		
		Scanner fw_versionScan = new Scanner(string);
		
		/* see whether it's decimal firmware version number */
		if (!fw_versionScan.hasNextLong()) {
			/* check hex number */
			fw_versionScan.close();
			if (string.startsWith("0x"))
			{
				try {
					version = Integer.parseInt(string.substring(2), 16);
				} catch (Exception e) {
					throw new IOException("Invalid F/W Version (non-hexadecimal): " + string);
				}
			}
			/* check dotted format */
			else if (!getDottedFwVersion(string))
			{
				throw new IOException("Invalid F/W Version (non-digit or exceed maximum number of digits): " + string);
			}
		} else {
			version = fw_versionScan.nextLong();
			fw_versionScan.close();
		}
		
		
		if (version > FIRMWARE_VERSION_MAX_VALUE) {
			throw new IOException(String.format("Exceeded maximum firmware version number %s > %d (0x%X, or %d.%d.%d.%d)", string,
											  FIRMWARE_VERSION_MAX_VALUE, FIRMWARE_VERSION_MAX_VALUE,
											  ((FIRMWARE_VERSION_MAX_VALUE >> 24) & 0xFF),
											  ((FIRMWARE_VERSION_MAX_VALUE >> 16) & 0xFF),
											  ((FIRMWARE_VERSION_MAX_VALUE >> 8) & 0xFF),
											  (FIRMWARE_VERSION_MAX_VALUE & 0xFF)));
		}
		CommandTool.debug_log(String.format("FW version: %s = %d (0x%X)", string, version, version));
		return version;
	}

	private boolean getDottedFwVersion(String arg) throws IOException {
		
		long fwVersion = 0;
		String[] versions = arg.split("\\.");
		int length = versions.length;
		
		if (length == 0 || length > 4) return false;
		
		for (String ver : versions)
		{
			int vnumber;
			try {
				vnumber = Integer.parseInt(ver);
				if (vnumber > 255) {
					/* let's make > max fw version and return true
					 * for exceeded error msg.
					 */
					fwVersion = FIRMWARE_VERSION_MAX_VALUE + 1;
					break;
				}
			} catch (Exception e) {
				return false;
			}
			length--;
			fwVersion += ((vnumber << (8 * length)) & FIRMWARE_VERSION_MAX_VALUE);
		}

		if (fwVersion > FIRMWARE_VERSION_MAX_VALUE)
			throw new IOException("Invalid F/W Version: " + arg);
		
		return true;
	}

}
