package com.digi.connector.config;

/*import java.io.BufferedReader;
import java.io.InputStreamReader;*/

import java.io.Console;
import java.io.File;
import java.io.IOException;
import java.util.Scanner;
import java.util.regex.Pattern;

public class ConfigGenerator {

// 1.0.0.0 version XML RCI
// 2.0.0.0 version Binary RCI only

    public final static String VERSION = "2.0.0.0";
    
    private final static long FIRMWARE_VERSION_MAX_VALUE = 4294967295L;

    private final static String NO_DESC_OPTION = "nodesc";
    private final static String HELP_OPTION = "help";
    private final static String HIDDEN_HELP_OPTION = "hidden_help";
    private final static String VERBOSE_OPTION = "verbose";
    private final static String VENDOR_OPTION = "vendor";
    private final static String DIRECTORY_OPTION = "path";
    private final static String DELETE_DESCRIPTOR_OPTION = "deleteDescriptor";
    private final static String PROTOTYPES_OPTION = "use_prototypes";
    private final static String SAVE_DESCRIPTORS_OPTION = "saveDescriptors";
    private final static String NO_UPLOAD_OPTION = "noUpload";
    private final static String CREATE_BIN_ID_LOG_OPTION = "createBinIdLog";
    private final static String USE_BIN_ID_LOG_OPTION = "useBinIdLog";
    private final static String RCI_LEGACY_COMMANDS_OPTION = "rci_legacy_commands";
    private final static String RCI_DC_TARGET_MAX_OPTION = "rci_dc_attribute_max_len";
    private final static String NO_BACKUP_OPTION = "noBackup";
    private final static String RCI_PARSER_OPTION = "rci_parser";

    private final static String FILE_TYPE_OPTION = "type";

    private final static String URL_OPTION = "url";
    private final static String URL_DEFAULT = "login.etherios.com";

    private final static String USE_NAMES_OPTION = "usenames";
    private final static String USE_NAMES_DEFAULT = "none";

    public final static String PREFIX_OPTION = "prefix";

    public final static String DASH = "-";

    public final static String USERNAME = "username";
    public final static String PASSWORD = "password";
    public final static String DEVICE_TYPE = "deviceType";
    public final static String FIRMWARE_VERSION = "firmwareVersion";
    public final static String CONFIG_FILENAME = "configFileName";

    private static String urlName;
    private static String vendorId;

    private static String deviceType;
    private static long fwVersion;
    private static String username;
    private static String password;
    private static String filename;
    private static String argumentLog;
    private static String directoryPath;
    private static boolean noErrorDescription;
    private static boolean verboseOption;
    private static FileType fileType = FileType.NONE;
    private static UseNames useNames = UseNames.NONE;
    private static String prefix = "";
    private static boolean deleteDescriptor;
    private static boolean prototypes;
    private static boolean saveDescriptor;
    private static boolean noUpload;
    private static boolean createBinIdLog;
    private static boolean useBinIdLog;
    private static boolean rci_legacy;
    private static int rci_dc_attribute_max_len = 0;
    private static boolean noBackup;
    private static boolean rciParser;

    private static boolean hidden_help;

    public enum FileType {
        NONE,
        SOURCE,
        GLOBAL_HEADER;

        public static FileType toFileType(String str) throws Exception {
            try {
                return valueOf(str.toUpperCase());
                
            } catch (Exception e) {
                log("Available file types:");
                for (FileType fileType : FileType.values()) {
                    log(String.format("\t%s",fileType.toString()));
                }
                throw new Exception("Invalid file type: " + str);
            }
        }
        
    }
    
    public enum UseNames {
        NONE,
        GROUPS,
        ELEMENTS,
        ALL;

        public static UseNames toUseNames(String str) throws Exception {
            try {
                return valueOf(str.toUpperCase());
            } catch (Exception e) {
                log("Available use names:");
                for (UseNames useNames : UseNames.values()) {
                    log(String.format("\t%s",useNames.toString()));
                }
                throw new Exception("Invalid use name: " + str);
            }
        }

    }

    private static void usage() {
        String className = ConfigGenerator.class.getName();

        int firstChar = className.lastIndexOf(".") + 1;
        if (firstChar != -1) {
            className = className.substring(firstChar);
        }

        log(String.format("\nUsage: java -jar %s.jar [", className)
                + DASH
                + HELP_OPTION
                + "] ["
                + DASH
                + VERBOSE_OPTION
                + "] ["
                + DASH
                + NO_DESC_OPTION
                + "] ["
                + DASH
                + VENDOR_OPTION
                + "] ["
                + DASH
                + DIRECTORY_OPTION
                + "] ["
                + DASH
                + URL_OPTION
                + "] ["
                + DASH
                + NO_BACKUP_OPTION
                + "] ["
                + DASH
                + SAVE_DESCRIPTORS_OPTION
                + "] ["
                + DASH
                + NO_UPLOAD_OPTION
                + "] ["
                + DASH
                + USE_NAMES_OPTION
                +"] "
                + String.format("<\"%s\"[:\"%s\"]> <%s> <%s> <%s>\n", USERNAME,
                        PASSWORD, DEVICE_TYPE, FIRMWARE_VERSION,
                        CONFIG_FILENAME));

        log("Description:");
        log("\tIt generates and uploads configuration information (descriptors) to Device Cloud");
        log("\tand it also generates ANSI C API header file (connector_api_remote.h) ");
        log("\tfrom the input configuration file.\n");

        log("Options:");
        log(String.format("\t%-16s \t= show this message", DASH
                + HELP_OPTION));
        log(String
                .format(
                        "\t%-16s \t= optional output messages about what the tool is doing",
                        DASH + VERBOSE_OPTION));
        log(String
                .format(
                        "\t%-16s \t= optional exclusion of error descriptions in the C file (Code size reduction)",
                        DASH + NO_DESC_OPTION));
        log(String
                .format(
                        "\t%-16s \t= optional vendor ID obtained from Device Cloud registration.",
                        DASH + VENDOR_OPTION + "=<vendorID>"));
        log(String
                .format(
                        "\t%-16s \t  If not given, tool tries to retrieve it from Device Cloud",
                        ""));

        log(String
                .format(
                        "\t%-16s \t= optional directory path where the header file will be created.",
                        DASH + DIRECTORY_OPTION + "=<directory path>"));
        log(String
                .format(
                        "\t%-16s \t= optional Device Cloud URL. Default is %s",
                        DASH + URL_OPTION + "=<Device Cloud URL>", URL_DEFAULT));
        log(String
                .format(
                        "\t%-16s \t= do not backup remote_config.c and .h files.",
                        DASH + NO_BACKUP_OPTION));
        log(String
                .format(
                        "\t%-16s \t= save a local copy of the Descriptors that the tool uploads to Device Cloud",
                        DASH + SAVE_DESCRIPTORS_OPTION));
        log(String
                .format(
                        "\t%-16s \t= do not upload Descriptors to Device Cloud.If this option and -vendor are set,you don't" +
                        " need to use a valid username:password.",
                        DASH + NO_UPLOAD_OPTION));
        log(String
                .format(
                        "\t%-16s \t= optional, add ASCIIZ string \"name\" field to connector_remote_group_t and/or connector_remote_element_t structures. Default is %s.",
                        DASH + USE_NAMES_OPTION + "={none|groups|elements|all}", USE_NAMES_DEFAULT));
        log(String
                .format(
                        "\n\t%-16s \t= username to log in to Device Cloud. If no password is given you will be prompted to enter the password",
                        USERNAME));
        log(String
                .format(
                        "\t%-16s \t= optional password to log in to Device Cloud",
                        PASSWORD));
        log(String
                .format(
                        "\t%-16s \t= Device type string with quotes(i.e. \"device type\")",
                        DEVICE_TYPE));
        log(String.format("\t%-16s \t= firmware version (i.e. 1.0.0.0)",
                FIRMWARE_VERSION));
        log(String.format("\t%-16s \t= The Connector Configuration file",
                CONFIG_FILENAME));
        if(hidden_help){
            log("Hidden Options:");
            log(String.format("\t%-16s \t= show this extra message", DASH
                    + HIDDEN_HELP_OPTION));

            log(String
                .format(
                        "\t%-16s \t= delete current Descriptors in Device Cloud",
                        DASH + DELETE_DESCRIPTOR_OPTION));
            log(String
                .format(
                        "\t%-16s \t= optional behavior,adding a prefix to the structures.",
                        DASH + PREFIX_OPTION + "=<prefix>"));
            log(String
                    .format(
                            "\t%-16s \t= Choose the output Type. Default is none.",
                            DASH + FILE_TYPE_OPTION + "={none|source|global_header}" ));
            log(String
                    .format(
                            "\t%-16s \t= create bin_id log",
                            DASH + CREATE_BIN_ID_LOG_OPTION));
            log(String
                    .format(
                            "\t%-16s \t= use bin_id log",
                            DASH + USE_BIN_ID_LOG_OPTION));
            log(String
                    .format(
                            "\t%-16s \t= optional support for RCI do_command,reboot and set_factory_default",
                            DASH + RCI_LEGACY_COMMANDS_OPTION));
            log(String
                    .format(
                            "\t%-16s \t= optional max length for commands attributes type string, default is %d",
                            DASH + RCI_DC_TARGET_MAX_OPTION,ConfigData.AttributeMaxLen()));
            log(String
                    .format(
                            "\t%-16s \t= optional behaviour for future features using prototypes",
                            DASH + PROTOTYPES_OPTION));
            log(String
                    .format(
                            "\t%-16s \t= Defines and enums names to work with RCI Parser.",
                            DASH + RCI_PARSER_OPTION));
         }

        System.exit(1);
    }

    private void queryPassword() {
        
      Console console = System.console();
        
      if (console == null) {
          System.out.println("Couldn't get Console instance, maybe you're running this from within an IDE?");
          System.exit(1);
      }

      char passwordArray[] = console.readPassword("Enter password: ");

      if (passwordArray.length == 0) {
          log("You must enter a password.\nPlease try again!");
          System.exit(1);
      }
      password = new String(passwordArray);

    }

    private static void toOption(String option) {

        /* split the [option]=[option value] */
        try {

            String[] keys = option.split("=", 2);

            if (keys.length == 2) {
                if (keys[0].equals(URL_OPTION)) {
                    urlName = keys[1];
                } else if (keys[0].equals(VENDOR_OPTION)) {

                    if (Pattern.matches("(0[xX])?\\p{XDigit}+", keys[1])) {
                        vendorId = keys[1];
                        if (!vendorId.startsWith("0x") && !vendorId.matches("^\\d+$"))
                        {
                            /* not all digits (mix with hex) */
                            throw new Exception("Invalid Vendor ID! For hexadecimal vendor ID, use 0x prefix");
                        }
                    } else {
                        throw new Exception("Invalid Vendor ID!");
                    }
                } else if (keys[0].equals(DIRECTORY_OPTION)) {
                    if (new File(keys[1]).isDirectory()) {
                        directoryPath = keys[1];
                    } else {
                        throw new Exception("Invalid directory path!");
                    }
                } else if (keys[0].equals(FILE_TYPE_OPTION)) {
                    fileType = FileType.toFileType(keys[1]);

                } else if (keys[0].equals(PREFIX_OPTION)) {
                    prefix = keys[1] + "_";

                } else if (keys[0].equals(USE_NAMES_OPTION)) {
                    useNames = UseNames.toUseNames(keys[1]);

                } else if (keys[0].equals(RCI_DC_TARGET_MAX_OPTION)) {
                    try{
                        rci_dc_attribute_max_len = Integer.parseInt(keys[1]);
                    } catch (NumberFormatException e) {
                        throw new IOException("-rci_dc_attribute_max_len expect an integer value");
                    }
                }
                else {
                    throw new Exception("Invalid Option: " + keys[0]);
                }

            } else if (option.equals(NO_DESC_OPTION)) {
                noErrorDescription = true;
            } else if (option.equals(VERBOSE_OPTION)) {
                verboseOption = true;
            } else if (option.equals(DELETE_DESCRIPTOR_OPTION)) {
                deleteDescriptor = true;
            } else if (option.equals(HELP_OPTION)) {
                usage();
            } else if (option.equals(HIDDEN_HELP_OPTION)) {
                hidden_help = true;
                usage();
            } else if (option.equals(PROTOTYPES_OPTION)) {
                prototypes = true;
            } else if (option.equals(SAVE_DESCRIPTORS_OPTION)) {
                saveDescriptor = true;
            } else if (option.equals(NO_UPLOAD_OPTION)) {
                noUpload = true;
            } else if (option.equals(CREATE_BIN_ID_LOG_OPTION)) {
                createBinIdLog = true;
            } else if (option.equals(USE_BIN_ID_LOG_OPTION)) {
                useBinIdLog = true;
            } else if (option.equals(RCI_LEGACY_COMMANDS_OPTION)) {
                rci_legacy = true;
            } else if (option.equals(NO_BACKUP_OPTION)) {
                noBackup = true;
            } else if (option.equals(RCI_PARSER_OPTION)) {
                rciParser = true;
            } else if (option.isEmpty()) {
                throw new Exception("Missing Option!");
            } else {
                throw new Exception("Invalid Option: " + option);
            }
        } catch (Exception e) {
            log(e.getMessage());
            usage();
            System.exit(1);
        }

    }

    private boolean getDottedFwVersion(String arg) {
        
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

        return true;
    }
    
    private void toArgument(int argNumber, String arg) {
        try {
            switch (argNumber) {
            case 1:
                /* username:password argument */
                if (arg.indexOf(':') != -1) {
                    String[] userpass = arg.split(":", 2);

                    username = userpass[0];
                    argumentLog += username + ":";

                    if (!userpass[1].isEmpty()) {
                        password = userpass[1];
                        argumentLog += "*****";
                    }
                } else {
                    username = arg;
                    argumentLog += username;
                }

                break;
            case 2:
                /* device type */
                deviceType = arg;
                argumentLog += " \"" + arg + "\"";
                break;
            case 3:
                /* firmware version */
                
                Scanner fwVersionScan = new Scanner(arg);
                
                /* see whether it's decimal firmware version number */
                if (!fwVersionScan.hasNextLong()) {
                    /* check hex number */
                    if (arg.startsWith("0x"))
                    {
                        try {
                            fwVersion = Integer.parseInt(arg.substring(2), 16);
                        } catch (Exception e) {
                            throw new Exception("Invalid F/W Version (non-hexadecimal): " + arg);
                        }
                    }
                    /* check dotted format */
                    else if (!getDottedFwVersion(arg))
                    {
                        throw new Exception("Invalid F/W Version (non-digit or exceed maximum number of digits): " + arg);
                    }
                } else {
                    fwVersion = fwVersionScan.nextLong();
                }
                
                if (fwVersion > FIRMWARE_VERSION_MAX_VALUE) {
                    throw new Exception(String.format("Exceeded maximum firmware version number %s > %d (0x%X, or %d.%d.%d.%d)", arg,
                                                      FIRMWARE_VERSION_MAX_VALUE, FIRMWARE_VERSION_MAX_VALUE,
                                                      ((FIRMWARE_VERSION_MAX_VALUE >> 24) & 0xFF),
                                                      ((FIRMWARE_VERSION_MAX_VALUE >> 16) & 0xFF),
                                                      ((FIRMWARE_VERSION_MAX_VALUE >> 8) & 0xFF),
                                                      (FIRMWARE_VERSION_MAX_VALUE & 0xFF)));
                }
                debug_log(String.format("FW version: %s = %d (0x%X)", arg, fwVersion,fwVersion));
                
                argumentLog += " " + arg;
                break;

            case 4:
                filename = arg;
                argumentLog += " " + arg;

                break;
                
            default:
                log("Unkown argument: " + arg);
                return;
            }
        } catch (Exception e) {
            log(e.getMessage());
            System.exit(1);
        }
    }

    public static long getFirmware() {
        return fwVersion;
    }

    public static String getPrefix() {
        return prefix;
    }

    public static String getArgumentLogString() {
        return argumentLog;
    }

    public static String getUrlName() {
        return urlName;
    }

    public ConfigGenerator(String args[]) {
        int argCount = 0;

        argumentLog = "\"";

        urlName = URL_DEFAULT;

        for (String arg : args) {
            if (arg.startsWith(DASH)) {
                String str = arg.substring(DASH.length());

                toOption(str);

                argumentLog += arg + " ";
            } else {
                toArgument(++argCount, arg);
            }
        }
        argumentLog += "\"";

        if (argCount != 4 && fileTypeOption() != FileType.GLOBAL_HEADER) {
            log("Missing arguments!");
            usage();
        }
        if (password == null && fileTypeOption() != FileType.GLOBAL_HEADER) {
            queryPassword();
        }
    }

    public static boolean excludeErrorDescription() {
        return noErrorDescription;
    }

    public static boolean usePrototypes() {
        return prototypes;
    }

    public static boolean rciLegacyEnabled() {
        return rci_legacy;
    }

    public static void log(Object aObject) {
        System.out.println(String.valueOf(aObject));
    }

    public static void debug_log(Object aObject) {
        if (verboseOption) {
            System.out.println(String.valueOf(aObject));
        }
    }
    
    public static FileType fileTypeOption() {
        return fileType;
    }

    public static UseNames useNamesOption() {
        return useNames;
    }

    public static boolean deleteDescriptorOption() {
        return deleteDescriptor;
    }

    public static boolean saveDescriptorOption() {
        return saveDescriptor;
    }

    public static boolean noUploadOption() {
        return noUpload;
    }

    public static boolean noBackupOption() {
        return noBackup;
    }

    public static boolean createBinIdLogOption() {
        return createBinIdLog && !useBinIdLog;
    }

    public static boolean useBinIdLogOption() {
        return useBinIdLog;
    }


    public static boolean rciParserOption() {
        return rciParser;
    }

	public static void setRciParser(boolean b) {
		rciParser = b;
	}
    public static String filename() {
        return filename;
    }

    public static void main(String[] args) {
        
        try {
            new ConfigGenerator(args);

            if (deleteDescriptorOption()) {
                /* descriptor constructor for arguments */
                Descriptors descriptors = new Descriptors(username, password,
                        vendorId, deviceType, fwVersion);
                
                /* Generate and upload descriptors */
                debug_log("Start deleting descriptors");
                descriptors.processDescriptors(null);
                log("\nDescriptors deleted.\n");
                System.exit(0);
            }

            /* parse file */
            debug_log("Start reading filename: " + filename);
            ConfigData configData = new ConfigData();

            if(rci_legacy){
                debug_log("rci legacy commands enable");
            }

            if(rci_dc_attribute_max_len != 0)
                configData.setAttributeMaxLen(rci_dc_attribute_max_len);

            if (fileTypeOption() != FileType.GLOBAL_HEADER) {
                Parser.processFile(filename, configData);
    
                if ((configData.getSettingGroups().isEmpty())
                        && (configData.getStateGroups().isEmpty())) {
                    throw new IOException("No groups specified in file: "
                            + filename);
                }

                debug_log("Number of setting groups: "
                        + configData.getSettingGroups().size());
                debug_log("Number of state groups: "
                        + configData.getStateGroups().size());
            }
            
            /* Generate H and C files */
            debug_log("Start generating files");


            switch(fileTypeOption())
            {
            case NONE:
                /* descriptor constructor for arguments */
                Descriptors descriptors = new Descriptors(username, password,
                        vendorId, deviceType, fwVersion);

                FileNone fileNone = new FileNone(directoryPath);
                fileNone.generateFile(configData);

                /* Generate and upload descriptors */
                debug_log("Start Generating/loading descriptors");
                if(!noUpload || saveDescriptor)
					descriptors.processDescriptors(configData);

                break;

            case SOURCE:
                /* descriptor constructor for arguments */
                Descriptors descriptorsSource = new Descriptors(username, password,
                        vendorId, deviceType, fwVersion);

                FileSource fileSource = new FileSource(directoryPath);
                fileSource.generateFile(configData);

                /* Generate and upload descriptors */
                debug_log("Start Generating/loading descriptors");
                if(!noUpload || saveDescriptor)
                    descriptorsSource.processDescriptors(configData);
                break;

            case GLOBAL_HEADER:
                FileGlobalHeader fileGlobalHeader = new FileGlobalHeader(directoryPath);
                fileGlobalHeader.generateFile(configData);
                break;
            }

            
            log("\nDone.\n");
            System.exit(0);

        } catch (Exception e) {

            if (e.getMessage() != null) {
                log(e.getMessage());
            }
            
            if (verboseOption) {
                e.printStackTrace();
                if (e.getCause() != null)
                    System.err.println(e.getMessage());
            }
            System.exit(1);
        }
    }

}
