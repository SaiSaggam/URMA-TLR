package com.digi.connector.config;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.URL;
import java.util.LinkedHashMap;
import java.util.LinkedList;

import javax.net.ssl.HttpsURLConnection;
import javax.xml.bind.DatatypeConverter;

public class Descriptors {

    private final String RCI_VERSION = "1.1";

    private final String RCI_DESCRIPTORS = "<descriptor element=\"rci_request\" desc=\"Remote Command Interface request\">\n"
            + "<attr name=\"version\" desc=\"RCI version of request.  Response will be returned in this versions response format\" default=\""
            + RCI_VERSION
            + "\">\n"
            + "<value value=\""
            + RCI_VERSION
            + "\" desc=\"Version " + RCI_VERSION + "\"/></attr>\n";

    private final String SETTING_DESCRIPTOR_DESCRIPTION = "device configuration";
    private final String STATE_DESCRIPTOR_DESCRIPTION = "device state";

    private final String username;
    private final String password;
    private static String deviceType;
    private static String vendorId;
    private final long fwVersion;
    private Boolean callDeleteFlag;
    private int responseCode;

    public Descriptors(final String username, final String password,
                       final String vendorId, final String deviceType, 
                       final long version) throws IOException  {
        this.username = username;
        this.password = password;
        Descriptors.deviceType = deviceType;
        this.fwVersion = version;

        if(!ConfigGenerator.noUploadOption())
            validateUrlName();

        Descriptors.vendorId = vendorId;
        if (vendorId == null) getVendorId();
        else {
            if (vendorId.startsWith("0x")) {
                String hex = vendorId.substring(2);
                ConfigGenerator.debug_log(String.format("Vendor ID = 0x%08X (%d)\n", Long.parseLong(hex, 16), Long.parseLong(hex, 16)));
            } else {
                String dec = vendorId;
                ConfigGenerator.debug_log(String.format("Vendor ID = 0x%08X (%d)\n", Long.parseLong(dec, 10), Long.parseLong(dec, 10)));
            }
        }
        this.callDeleteFlag = true;
        this.responseCode = 0;
    }

    public void processDescriptors(ConfigData configData) throws Exception {

        if (ConfigGenerator.deleteDescriptorOption()) {
            deleteDescriptors();
        } else {
            ConfigGenerator.log("\nProcessing Descriptors, please wait...");
            int id = 1;
            for (GroupType type : GroupType.values()) {
                LinkedList<Group> groups = null;

                String configType = type.toString().toLowerCase();
                
                try {
                    groups = configData.getConfigGroup(configType);

                } catch (Exception e) {
                    /* end of the ConfigData ConfigType */
                    break;
                }

                if (!groups.isEmpty()) sendDescriptors(configType, groups, configData, id);
                /* add 2 because sendDescriptors uploads 2 sets of descriptors (query and set ). */
                id += 2;
            }

            if(ConfigGenerator.rciLegacyEnabled()){
                sendRebootDescriptor();
                sendDoCommandDescriptor();
                sendSetFactoryDefaultDescriptor();
            }

            sendRciDescriptors(configData);

            if(!ConfigGenerator.noUploadOption())
                ConfigGenerator.log("\nDescriptors were uploaded successfully.");
            if(ConfigGenerator.saveDescriptorOption())
                ConfigGenerator.log("\nDescriptors were saved successfully.");
        }
    }

    private void deleteDescriptors()
    {
        String target = String.format("/ws/DeviceMetaData?condition=dvVendorId=%s and dmDeviceType=\'%s\' and dmVersion=%d",
                vendorId, deviceType, fwVersion);

        String response = sendCloudData(target.replace(" ", "%20"), "DELETE", null);
        if (responseCode != 0)
        {
            ConfigGenerator.debug_log("Response from " + ConfigGenerator.getUrlName());
            switch (responseCode)
            {
            case 401:
            ConfigGenerator.log("Unauthorized: verify username and password are valid\n");
            break;
            
            case 403:
            ConfigGenerator.log("Forbidden: deleting previous RCI descriptors failed, verify that vendor ID is valid and is owned by your account.\n");
            break;
            
            default:
            ConfigGenerator.log("Response status: " + response);
            break;
            }
            
            System.exit(1);
        }
        ConfigGenerator.debug_log("Deleted target: " + target);
        if (vendorId.startsWith("0x"))
            ConfigGenerator.debug_log(String.format("Deleted: 0x%X/%s", Long.parseLong(vendorId.substring(2), 16), deviceType));
        else
            ConfigGenerator.debug_log(String.format("Deleted: 0x%X/%s", Long.parseLong(vendorId, 10), deviceType));
            
        ConfigGenerator.debug_log(response);
    }
    
    private String getErrorDescriptors(int id, LinkedHashMap<String, String> errorMap) {
        String descriptor = "";
        int errorId = id;

        for (String errorName : errorMap.keySet()) {
            descriptor += String.format("<error_descriptor id=\"%d\" ", errorId);
            if (errorMap.get(errorName) != null)
                descriptor += String.format("desc=\"%s\" ", errorMap.get(errorName));
            
            descriptor += "/>\n";
            errorId++;
        }

        return descriptor;
    }

    private void sendRebootDescriptor() throws Exception {

        String reboot_descriptor = "<descriptor element=\"reboot\" desc=\"Reboot the device\" bin_id=\"7\" >";
        reboot_descriptor +=       "  <error_descriptor id=\"1\" desc=\"Reboot failed\" />";
        reboot_descriptor +=       "</descriptor>\n";        

        uploadDescriptor("descriptor/reboot", reboot_descriptor);
    }

    private void sendDoCommandDescriptor() throws Exception {

        String do_command_descriptor = "<descriptor element=\"do_command\" bin_id=\"6\" > ";
        do_command_descriptor +=       "  <error_descriptor id=\"1\" desc=\"Invalid arguments\" />";
        do_command_descriptor +=       "  <attr name=\"target\" type=\"string\" max=\"" + ConfigData.AttributeMaxLen()
                              + "\" desc=\"The subsystem that the command is forwarded\" bin_id=\"0\" />";
        do_command_descriptor +=       "</descriptor>";

        uploadDescriptor("descriptor/do_command", do_command_descriptor);
    }

    private void sendSetFactoryDefaultDescriptor() throws Exception {

        String set_factory_default_descriptor = "<descriptor element=\"set_factory_default\" desc=\"Set device configuration to factory defaults\" bin_id=\"8\" >";
        set_factory_default_descriptor +=       "  <error_descriptor id=\"1\" desc=\"Set Factory Default failed\" />";
        set_factory_default_descriptor +=       "</descriptor>\n";        

        uploadDescriptor("descriptor/set_factory_default", set_factory_default_descriptor);
    }

    private int getBinId(BufferedReader bin_id_reader, String BinIdKey) throws IOException {
        String respLine;
        String[] keys = null;
        int id = -1;
        boolean found = false;

        do {
            respLine = bin_id_reader.readLine();
            if (respLine == null)
                break;

            ConfigGenerator.debug_log(respLine);

            keys = respLine.split("=", 2);

            if (keys.length == 2 && keys[0].equals(BinIdKey))
                found = true;
        } while (!found);

        if (found == true)
             id = Integer.parseInt(keys[1]);

        return id;
    }

    private void sendDescriptors(String config_type, LinkedList<Group> groups, ConfigData configData, int id) throws Exception {
        String desc = SETTING_DESCRIPTOR_DESCRIPTION;
        boolean createBinIdLog = ConfigGenerator.createBinIdLogOption();
        boolean useBinIdLog = ConfigGenerator.useBinIdLogOption();
        String createBinIdLogBuffer = "";
        BufferedReader bin_id_reader = null;

        if (config_type.equalsIgnoreCase(GroupType.STATE.toString()))
            desc = STATE_DESCRIPTOR_DESCRIPTION;

        String query_descriptors = String.format("<descriptor element=\"query_%s\" desc=\"Retrieve %s\" format=\"all_%ss_groups\"\n",
                                                  config_type, desc, config_type);
        
        /* setup query setting command descriptor */
        query_descriptors += String.format(" bin_id=\"%d\">\n", id);

        if (config_type.equalsIgnoreCase("setting"))
            query_descriptors += "  <attr name=\"source\" type=\"enum\" desc=\"Source of settings returned\" bin_id=\"0\" default=\"current\" >"
                               + "      <value value=\"current\" desc=\"Current settings\" bin_id=\"0\" />"
                               + "      <value value=\"stored\" desc=\"Settings stored in flash\" bin_id=\"1\" />"
                               + "      <value value=\"defaults\" desc=\"Device defaults\" bin_id=\"2\" />"
                               + "  </attr>"
                               + "  <attr name=\"compare_to\" type=\"enum\" desc=\"Return only differences from this source\" bin_id=\"1\" default=\"none\" >"
                               + "      <value value=\"none\" desc=\"Return all settings\" bin_id=\"0\" />"
                               + "      <value value=\"current\" desc=\"Current settings\" bin_id=\"1\" />"
                               + "      <value value=\"stored\" desc=\"Settings stored in flash\" bin_id=\"2\" />"
                               + "      <value value=\"defaults\" desc=\"Device defaults\" bin_id=\"3\" />"
                               + "  </attr>";

        query_descriptors += String.format("<format_define name=\"all_%ss_groups\">\n", config_type);

        /*
         * get all errors for query command descriptor. 1. common errors. 2.
         * command errors. 3. user global errors
         * 
         * We must offset the error_id for command errors.
         */
        query_descriptors +=  getErrorDescriptors(configData.getUserGlobalErrorsIndex(), configData.getUserGlobalErrors());

        String set_descriptors = String.format("<descriptor element=\"set_%s\" desc=\"Set %s\" format=\"all_%ss_groups\"\n",
                                                config_type, desc, config_type);
        
        set_descriptors += String.format(" bin_id=\"%d\">\n", id+1);
    
        /*
         * get all errors for set command descriptor. 1. common errors. 2.
         * command errors. 3. user global errors
         * 
         * We must offset the error_id for command errors.
         */
        set_descriptors += getErrorDescriptors(configData.getUserGlobalErrorsIndex(), configData.getUserGlobalErrors());

        set_descriptors += "</descriptor>";

        if (useBinIdLog)
            bin_id_reader = new BufferedReader(new FileReader("bin_id_" + config_type + ".log"));

        for (Group group : groups) {
            if (!useBinIdLog)
                query_descriptors += group.toString(groups.indexOf(group));
            else {
                String BinIdKey = String.format("group_%s_%s_bin_id", config_type, group.getName());
                int group_id = getBinId(bin_id_reader, BinIdKey);

                if (group_id == -1) {
                    ConfigGenerator.log("Fatal: BinIdKey not found: " + BinIdKey + " !!!!!!");
                    return;
                }

                query_descriptors += group.toString(group_id);
            }

            if (createBinIdLog)
                createBinIdLogBuffer += String.format("group_%s_%s_bin_id=%d\n", config_type, group.getName(), groups.indexOf(group));

            query_descriptors += String.format("<attr name=\"index\" desc=\"item number\" type=\"int32\" min=\"1\" max=\"%d\" />",
                                                    group.getInstances());

            for (Element element : group.getElements()) {

                if (!useBinIdLog)
                    query_descriptors += element.toString(group.getElements().indexOf(element));
                else {
                    String BinIdKey = String.format("group_%s_%s_%s_bin_id", config_type, group.getName(), element.getName());
                    int element_id = getBinId(bin_id_reader, BinIdKey);

                    if (element_id == -1) {
                        ConfigGenerator.log("Fatal: BinIdKey not found: " + BinIdKey + " !!!!!!");
                        return;
                    }

                    query_descriptors += element.toString(element_id);
                }

                if (createBinIdLog)
                    createBinIdLogBuffer += String.format("group_%s_%s_%s_bin_id=%d\n", config_type, group.getName(), element.getName(), group.getElements().indexOf(element));
                
                if (Element.ElementType.toElementType(element.getType()) == Element.ElementType.ENUM) {

                    for (ValueStruct value : element.getValues()) {
                        if (!useBinIdLog)
                            query_descriptors += value.toString(element.getValues().indexOf(value));
                        else {
                            String BinIdKey = String.format("group_%s_%s_%s_%s_bin_id", config_type, group.getName(), element.getName(), value.getName());
                            int value_id = getBinId(bin_id_reader, BinIdKey);

                            if (value_id == -1) {
                                ConfigGenerator.log("Fatal: BinIdKey not found: " + BinIdKey + " !!!!!!");
                                return;
                            }

                            query_descriptors += value.toString(value_id);
                        }


                        if (createBinIdLog)
                            createBinIdLogBuffer += String.format("group_%s_%s_%s_%s_bin_id=%d\n", config_type, group.getName(), element.getName(), value.getName(), element.getValues().indexOf(value));
                    }

                    query_descriptors += "</element>\n";

                }
            }

            /*
             * Write error for individual groups 1. common errors 2. group
             * errors 3. user global error 4. user group error
             * 
             * We must offset the error id for group errors.
             */
            query_descriptors += getErrorDescriptors(configData.getUserGlobalErrorsIndex(), configData.getUserGlobalErrors());

            query_descriptors += getErrorDescriptors(configData.getAllErrorsSize() + 1, group.getErrors());

            query_descriptors += "</descriptor>";
        }
        query_descriptors += "</format_define>\n</descriptor>\n";

        uploadDescriptor("descriptor/query_" + config_type, query_descriptors);
        uploadDescriptor("descriptor/set_" + config_type, set_descriptors);

        if (createBinIdLog) {
            try {
                BufferedWriter fileWriter = new BufferedWriter(new FileWriter("bin_id_" + config_type + ".log"));

                fileWriter.write(createBinIdLogBuffer);
                fileWriter.close();

             } catch (IOException e) {
                e.printStackTrace();
             }
         }

        if (useBinIdLog)
            bin_id_reader.close();
    }

    private void sendRciDescriptors(ConfigData configData) throws IOException {
        String descriptors = RCI_DESCRIPTORS;

        for (GroupType type : GroupType.values()) {
            LinkedList<Group> groups = null;

            String configType = type.toString().toLowerCase();

            try {

                groups = configData.getConfigGroup(configType);

            } catch (Exception e) {
                /* end of the ConfigData ConfigType */
                break;
            }
            if (!groups.isEmpty()) {
                descriptors += String.format("<descriptor element=\"query_%s\" dscr_avail=\"true\" />\n", configType)
                               + String.format("<descriptor element=\"set_%s\" dscr_avail=\"true\" />\n", configType);
            }
        }

        if(ConfigGenerator.rciLegacyEnabled()){
            descriptors += String.format("<descriptor element=\"reboot\" dscr_avail=\"true\" />\n");

            descriptors += String.format("<descriptor element=\"do_command\" dscr_avail=\"true\" />\n");

            descriptors += String.format("<descriptor element=\"set_factory_default\" dscr_avail=\"true\" />\n");
        }

        descriptors += getErrorDescriptors(configData.getRciGlobalErrorsIndex(), configData.getRciGlobalErrors()) 
                     + "</descriptor>";

        uploadDescriptor("descriptor", descriptors);
    }

    private String sendCloudData(String target, String method, String message) {
        String response = "";
        String cloud = "https://" + ConfigGenerator.getUrlName() + target;
        String credential = username + ":" + password;
        String encodedCredential = DatatypeConverter.printBase64Binary(credential.getBytes());
        HttpsURLConnection connection = null;

        responseCode = 0;
        try {
            URL url = new URL(cloud);

            connection = (HttpsURLConnection) url.openConnection();
            connection.setRequestMethod(method);
            connection.setRequestProperty("Content-Type", "text/xml");
            connection.setRequestProperty("Authorization", "Basic " + encodedCredential);

            if (message != null) {
                connection.setDoOutput(true);

                OutputStreamWriter request = new OutputStreamWriter(connection.getOutputStream());
                request.write(message);
                request.close();
            }

            connection.connect();
            BufferedReader reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
            String respLine;

            while ((respLine = reader.readLine()) != null) {
                response += respLine;
            }
            reader.close();
            connection.disconnect();

        } catch (Exception resp) {
            try
            {
                responseCode = connection.getResponseCode();
                response = connection.getHeaderField(0);
            }
            catch (Exception e)
            {
                ConfigGenerator.log("ERROR: Invalid Device Cloud URL\n");
                ConfigGenerator.debug_log("Invalid URL: " + ConfigGenerator.getUrlName());
                System.exit(1);
            }
        }

        return response;
    }

    private void getVendorId() {

        ConfigGenerator.debug_log("Query vendor ID");

        // Request an existing vendor ID
        String response = sendCloudData("/ws/DeviceVendor", "GET", null);

        if (responseCode == 0) {
            int startIndex = response.indexOf("<dvVendorId>");

            if (startIndex == -1 || startIndex != response.lastIndexOf("<dvVendorId>")) {
                ConfigGenerator.debug_log("No vendor ID is found.");

            } else {
                startIndex += "<dvVendorId>".length();
                vendorId = response.substring(startIndex, response.indexOf("</dvVendorId>"));
            }

        } else {
            ConfigGenerator.debug_log("Response from " + ConfigGenerator.getUrlName());
            switch (responseCode) {
            case 401:
                ConfigGenerator.log("Unauthorized: verify username and password are valid\n");
                break;

            case 403:
                ConfigGenerator
                        .log("Forbidden: verify that your account has the \'Embedded Device Customization\' service subscribed.\n");
                break;

            default:
                ConfigGenerator.log("Response status: " + response);
                break;
            }
            System.exit(1);
        }

        if (vendorId == null) {
            // Request to create a new Vendor ID
            ConfigGenerator.debug_log("Create a new vendor ID");
            sendCloudData("/ws/DeviceVendor", "POST", "<DeviceVendor></DeviceVendor>");

            response = sendCloudData("/ws/DeviceVendor", "GET", null);

            if (responseCode != 0) {
                ConfigGenerator.log("Response status: " + response);
            }

            int startIndex = response.indexOf("<dvVendorId>");
            if (startIndex == -1) {
                ConfigGenerator
                        .log("Cannot create a new vendor ID for "
                                + username
                                + "user. User needs to manually create one. Refer to \"Setup your Device Cloud Acount\" section of the Getting started guide to obtain one.");
                System.exit(1);
            }

            if (startIndex != response.lastIndexOf("<dvVendorId>")) {
                ConfigGenerator.log(username + " has more than one vendor ID, so please specify the correct one.");
                System.exit(1);
            }

            startIndex += "<dvVendorId>".length();
            vendorId = response.substring(startIndex, response.indexOf("</dvVendorId>"));
        }

        if (vendorId != null)
            ConfigGenerator.log(String.format("Device Cloud registered vendor ID: 0x%X", Long.parseLong(vendorId)));
    }


    private void validateUrlName() {
        ConfigGenerator.debug_log("Start validating device cloud url " + ConfigGenerator.getUrlName());
        String response = sendCloudData("/ws/UserInfo", "GET", null);

        if (responseCode != 0)
        {
            ConfigGenerator.debug_log("Response from " + ConfigGenerator.getUrlName());
            switch (responseCode)
            {
                case 401:
                    ConfigGenerator.log("Unauthorized: verify username and password are valid\n");
                    break;

                case 403:
                    ConfigGenerator.log("Forbidden: Failed to get user info.\n");
                    break;

                default:
                    ConfigGenerator.log("Response status: " + response);
                    break;
            }

            System.exit(1);
        }
    }

    private String tagMessageSegment(String tagName, String value) {
        return "<" + tagName + ">" + value + "</" + tagName + ">";
    }

    private String replaceXmlEntities(String buffer) {
        return buffer.replace("<", "&lt;").replace(">", "&gt;");
    }

    private void uploadDescriptor(String descName, String buffer) {

        if(!ConfigGenerator.noUploadOption())
            ConfigGenerator.debug_log("Uploading descriptor:" + descName);

        if (callDeleteFlag && !ConfigGenerator.noUploadOption()) {
            deleteDescriptors();
            callDeleteFlag = false;
        }

        String message = "<DeviceMetaData>";
        message += tagMessageSegment("dvVendorId", vendorId);
        message += tagMessageSegment("dmDeviceType", deviceType);
        message += tagMessageSegment("dmVersion", String.format("%d", fwVersion));
        message += tagMessageSegment("dmName", descName);
        message += tagMessageSegment("dmData", replaceXmlEntities(buffer));
        message += "</DeviceMetaData>";

        ConfigGenerator.debug_log(message);
        if(ConfigGenerator.saveDescriptorOption()){
	        try {
	            BufferedWriter fileWriter = new BufferedWriter(new FileWriter(descName.replace("/", "_")+".xml"));
		        String message2 = "<DeviceMetaData>";
		        message2 += tagMessageSegment("dvVendorId", vendorId);
		        message2 += tagMessageSegment("dmDeviceType", deviceType);
		        message2 += tagMessageSegment("dmVersion", String.format("%d", fwVersion));
		        message2 += tagMessageSegment("dmName", descName);
		        message2 += tagMessageSegment("dmData", buffer);
		        message2 += "</DeviceMetaData>";

				fileWriter.write(message2);
				fileWriter.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
        }
        if(!ConfigGenerator.noUploadOption()){
	        String response = sendCloudData("/ws/DeviceMetaData", "POST", message);
	        if (responseCode != 0)
	        {
	            ConfigGenerator.debug_log("Response from " + ConfigGenerator.getUrlName());
	            switch (responseCode)
	            {
	                case 401:
	                    ConfigGenerator.log("Unauthorized: verify username and password are valid\n");
	                    break;

	                case 403:
	                    ConfigGenerator.log("Forbidden: Uploading " + descName + " failed, verify that vendor ID is valid and is owned by your account.\n");
	                    break;

	                default:
	                    ConfigGenerator.log("Response status: " + response);
	                    break;
	            }

	            System.exit(1);
	        }

	        ConfigGenerator.debug_log("Created: " + vendorId + "/" + deviceType + "/" + descName);
	        ConfigGenerator.debug_log(response);
	        /* prevent error HTTP/1.1 429 Too Many Requests */
	        try {
	            Thread.sleep(1000);
	        } catch (InterruptedException e) {
	            e.printStackTrace();
	        }
        }
    }

    public static String vendorId(){
        try{
            return String.format("0x%X", Long.parseLong(vendorId));
        }
        catch (NumberFormatException nfe){
            return vendorId;
        }
    }

    public static String deviceType(){
        return deviceType;
    }

}
