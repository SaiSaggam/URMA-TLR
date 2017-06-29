package com.digi.connector.config;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.LinkedHashMap;
import java.util.LinkedList;

import com.digi.connector.config.Element.ElementType;

public abstract class FileGenerator {

	protected boolean future_feature = ConfigGenerator.usePrototypes();

    protected static String TOOL_NAME = "RCI Generator";

    protected final static String HEADER_FILENAME = "connector_api_remote.h";

    protected final static String CONNECTOR_PREFIX = "connector";
    protected final static String DEFINE = "#define ";
    protected final static String INCLUDE = "#include ";
    protected final static String ERROR = "error";

    protected final static String CONNECTOR_GLOBAL_HEADER = "\n\n#include \"connector_api.h\"\n\n";
    protected final static String FLOAT_HEADER = "\"float.h\"\n";

    protected final static String TYPEDEF_ENUM = "typedef enum {\n";

    protected final static String GLOBAL_RCI_ERROR = "connector_rci_error";
    protected final static String GLOBAL_ERROR = "connector_global_error";

    protected final static String CONNECTOR_REMOTE_ALL_STRING = "connector_remote_all_strings";
    protected final static String CONNECTOR_REMOTE_GROUP_TABLE = "connector_group_table";

    protected final static String COUNT_STRING = "COUNT";
    protected final static String OFFSET_STRING = "OFFSET";

    protected final static String CHAR_CONST_STRING = "static char CONST * CONST ";
    protected final static String CONNECTOR_CALLBACK_STATUS = "\nconnector_callback_status_t ";
    protected final static String ID_T_STRING = "_id_t;\n\n";
    protected final static String TYPEDEF_STRUCT = "\ntypedef struct {\n";

    /* Do not change these (if you do, you also need to update connector_remote.h */
    protected final static String RCI_PARSER_USES = "RCI_PARSER_USES_";
    protected final static String RCI_INFO_T = "rci_info_t * const info";
    protected final static String RCI_FUNCTION_T = "(rci_function_t)";
    
    protected final static String RCI_PARSER_USES_ERROR_DESCRIPTIONS = RCI_PARSER_USES + "ERROR_DESCRIPTIONS\n";
    protected final static String RCI_PARSER_USES_STRINGS = RCI_PARSER_USES + "STRINGS\n";
    protected final static String RCI_PARSER_USES_UNSIGNED_INTEGER = RCI_PARSER_USES + "UNSIGNED_INTEGER\n";
    
    protected final static String RCI_PARSER_DATA = "CONNECTOR_RCI_PARSER_INTERNAL_DATA";

    protected final static String CONNECTOR_RCI_INFO = "\ntypedef struct rci_data {\n" +
    " unsigned int group_index;\n" +
    " connector_remote_action_t action;\n" +
    " connector_remote_group_type_t group_type;\n" +
    " char const * error_hint;\n" +
    " void * user_context;\n" +
    "} rci_info_t;\n";

    protected final static String RCI_LEGACY_DEFINE = "\n#define RCI_LEGACY_COMMANDS\n";
    protected final static String RCI_ERROR_NOT_AVAILABLE = "connector_rci_error_not_available = -1,\n";

    /* Following enum has to be in syncr. with sendDescriptors() function */
    protected final static String RCI_QUERY_COMMAND_ATTRIBUTE_ID_T = "\ntypedef enum {\n" +
    "  rci_query_setting_attribute_id_source,\n" +      /* 'source' attribute is bin_id=0 in the uploaded descriptor for query command */
    "  rci_query_setting_attribute_id_compare_to,\n" +  /* 'compare_to' attribute is bin_id=1 in the uploaded descriptor for query command */
    "  rci_query_setting_attribute_id_count\n" +
    "} rci_query_setting_attribute_id_t;\n";

    /* Following enum has to be in syncr. with sendDescriptors() function */
    protected final static String RCI_DO_COMMAND_ATTRIBUTE_ID_T = "\ntypedef enum {\n" +
    "  rci_do_command_attribute_id_target,\n" +      /* 'target' attribute is bin_id=0 in the uploaded descriptor for do_command command */
    "  rci_do_command_attribute_id_count,\n" +
    "} rci_do_command_attribute_id_t;\n";

    protected final static String CONNECTOR_REMOTE_GROUP_TYPE = "\ntypedef enum {\n" +
    "    connector_remote_group_setting,\n" +
    "    connector_remote_group_state\n" +
    "} connector_remote_group_type_t;\n";

    protected final static String CONNECTOR_ELEMENT_ACCESS = "\ntypedef enum {\n" +
    "    connector_element_access_read_only,\n" +
    "    connector_element_access_write_only,\n" +
    "    connector_element_access_read_write\n" +
    "} connector_element_access_t;\n";

    protected final static String RCI_QUERY_SETTING_ATTRIBUTE_SOURCE = "\ntypedef enum {\n" +
    "    rci_query_setting_attribute_source_current,\n" +
    "    rci_query_setting_attribute_source_stored,\n" +
    "    rci_query_setting_attribute_source_defaults\n" +
    "} rci_query_setting_attribute_source_t;\n";

    protected final static String RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO = "\ntypedef enum {\n" +
    "    rci_query_setting_attribute_compare_to_none,\n" +
    "    rci_query_setting_attribute_compare_to_current,\n" +
    "    rci_query_setting_attribute_compare_to_stored,\n" +
    "    rci_query_setting_attribute_compare_to_defaults\n" +
    "} rci_query_setting_attribute_compare_to_t;\n";

    protected final static String CONNECTOR_REMOTE_CONFIG_T = "\ntypedef struct {\n" +
    "  void * user_context;\n" +
    "  connector_remote_action_t CONST action;\n" +
    "  connector_remote_attribute_t CONST attribute;\n" +
    "  connector_remote_group_t CONST group;\n" +
    "  connector_remote_element_t CONST element;\n" +
    "  unsigned int error_id;\n" +
    "\n" +
    "  struct {\n" +
    "      connector_bool_t compare_matches;\n" +
    "      char const * error_hint;\n" +
    "      connector_element_value_t * element_value;\n" +
    "  } response;\n" +
    "} connector_remote_config_t;\n";

    protected final static String CONNECTOR_REMOTE_CONFIG_CANCEL_T = "\ntypedef struct {\n" +
    "  void * user_context;\n" +
    "} connector_remote_config_cancel_t;\n";
        
    protected final static String CONNECTOR_REMOTE_GROUP_TABLE_T = "\ntypedef struct connector_remote_group_table {\n" +
    "  connector_group_t CONST * groups;\n" +
    "  size_t count;\n" +
    "} connector_remote_group_table_t;\n\n";
    
    protected String filePath = "";
    protected String generatedFile = "";

    protected BufferedWriter fileWriter;
    protected String configType;
    private int prevRemoteStringLength;
    private Boolean isFirstRemoteString;
    protected String prefix;

    public FileGenerator(String directoryPath,String file_name,ConfigGenerator.FileType fileType) throws IOException {
        
        if (directoryPath != null) {
            filePath = directoryPath;
            /* add a / if last char is not / */
            if (!directoryPath.endsWith("/")) filePath += '/';
        }
        
        generatedFile = file_name;

        if(!ConfigGenerator.noBackupOption())
            checkPreviousBuild(filePath + generatedFile);

        fileWriter = new BufferedWriter(new FileWriter(filePath + generatedFile));

        writeHeaderComment(fileWriter);
        isFirstRemoteString = true;
        prefix = ConfigGenerator.getPrefix();
    }

    protected void checkPreviousBuild(String new_path) throws IOException {

        File new_file = new File(new_path);
        int i = 0;

        while(new_file.isFile()){
            i++;
            new_file = new File(new_path + "_bkp_" + i );
        }
        if(i>0){
            String dest = new_path + "_bkp_" + i;
            InputStream in = new FileInputStream(new_path);
            OutputStream out = new FileOutputStream(dest);
            byte[] buf = new byte[1024];
            int len;
            while ((len = in.read(buf)) > 0) {
                out.write(buf, 0, len);
            }
            in.close();
            out.close();

            ConfigGenerator.log("Existing file " + new_path + " saved as: " + dest);
        }
	}

    abstract void writeHeaderComment(BufferedWriter bufferWriter) throws IOException;
    abstract void generateFile(ConfigData configData) throws Exception;

    private void writeOnOffBooleanEnum() throws IOException {

        String enumString = "";

        for (Element.ElementType type : Element.ElementType.values()) {
            if (type.isSet()) {
                switch (type) {
                case ON_OFF:
                    enumString += "\ntypedef enum {\n"
                                    + "    connector_off,\n"
                                    + "    connector_on\n"
                                    + "} connector_on_off_t;\n";
                    break;

                default:
                  break;
                }
            }
        }

        fileWriter.write(enumString);
    }

    private void writeElementTypeEnum() throws IOException {

        String enumString = "\n\ntypedef enum {\n";
        Boolean isFirstEnum = true;
        int value = -1;
       
        for (Element.ElementType type : Element.ElementType.values()) {
            if(type.isSet() && ConfigGenerator.rciParserOption()){
                if (!isFirstEnum) {
                    enumString += ",\n";
                }
                isFirstEnum = false;

                enumString += "    connector_element_type_" + type.toName().toLowerCase();

                if (type.toValue() != (value +1)) {
                    enumString += String.format(" = %d", type.toValue());
                }
                value = type.toValue();
            }
            else if (type.isSet()) {
                if (!isFirstEnum) {
                    enumString += ",\n";
                }
                isFirstEnum = false;
                
                enumString += "    connector_element_type_" + type.toName().toLowerCase();
                
                if (type.toValue() != (value +1)) {
                    enumString += String.format(" = %d", type.toValue());
                }
                value = type.toValue();
            }
        }

        enumString += "\n} connector_element_value_type_t;\n";
        fileWriter.write(enumString);
    }

    private String enumStructureString() {
        String string = TYPEDEF_STRUCT
                      + "    size_t count;\n";
        string += "} connector_element_value_enum_t;\n";

        return string;
    }
    
    protected void writePrototypes(ConfigData configData,BufferedWriter bufferWriter) throws Exception{

        String session_prototype = CONNECTOR_CALLBACK_STATUS;
        session_prototype += prefix + "rci_session_start_cb(" + RCI_INFO_T + ");";
        session_prototype += CONNECTOR_CALLBACK_STATUS;
        session_prototype += prefix + "rci_session_end_cb(" + RCI_INFO_T + ");\n";
        bufferWriter.write(session_prototype);

        for (GroupType type : GroupType.values()) {
	        LinkedList<Group> groups = null;

	        configType = type.toString().toLowerCase();
	        groups = configData.getConfigGroup(configType);

	        if (!groups.isEmpty()) {

	            for (Group group : groups) {
	                String group_prototype = CONNECTOR_CALLBACK_STATUS;
		            group_prototype += String.format("%srci_%s_%s_start(%s);",prefix,configType,group.getName(),RCI_INFO_T);
		            group_prototype += CONNECTOR_CALLBACK_STATUS;
		            group_prototype += String.format("%srci_%s_%s_end(%s);\n",prefix,configType,group.getName(),RCI_INFO_T);
		            bufferWriter.write(group_prototype);

		            for (Element element : group.getElements()) {
		                String protoType = "";
		                switch(ElementType.toElementType(element.getType())){
		                    case UINT32:
		                    case HEX32:
		                    case X_HEX32:
		                        protoType += "uint32_t";
		                        break;
		                    case INT32:
		                        protoType += "int32_t";
		                        break;
		                    case FLOAT:
		                        protoType += "float";
		                        break;
		                    case ON_OFF:
		                        protoType += "connector_on_off_t";
		                        break;
		                    case ENUM:
		                        protoType += String.format("%sconnector_%s_%s_%s_id_t",prefix,configType,group.getName(),element.getName());
		                        break;
		                    case IPV4:
		                    case FQDNV4:
		                    case FQDNV6:
		                    case DATETIME:
		                    case STRING:
		                    case MULTILINE_STRING:
		                    case PASSWORD:
		                    case MAC_ADDR:
		                        protoType += "char const *";
		                        break;
		                    case BOOLEAN:
		                        protoType += "connector_bool_t";
		                        break;
	                        default:
	                            break;
	                }
		                String element_prototype ="";

		                switch(ElementType.toElementType(element.getType())){
		                    case PASSWORD:
                                element_prototype += String.format("\n%s%srci_%s_%s_%s_get    NULL"
                                    ,DEFINE,prefix,configType,group.getName(),element.getName());
			                    break;
			                default:
			                    element_prototype += String.format("%s%srci_%s_%s_%s_get(%s, %s * const value);"
                                    ,CONNECTOR_CALLBACK_STATUS,prefix,configType,group.getName(),element.getName(),RCI_INFO_T,protoType);
			                    break;
		                }

		                if(getAccess(element.getAccess()).equalsIgnoreCase("read_only")) {

		                    element_prototype += String.format("\n%s%srci_%s_%s_%s_set    NULL \n"
                                ,DEFINE,prefix,configType,group.getName(),element.getName());
		                }
		                else{
			                element_prototype += String.format("%s%srci_%s_%s_%s_set(%s, %s const value);\n"
                                ,CONNECTOR_CALLBACK_STATUS,prefix,configType,group.getName(),element.getName(),RCI_INFO_T,protoType);
			                }
		                bufferWriter.write(element_prototype);
		            }
		        }
	        }
	    }
    }

    private void writeElementValueStruct() throws IOException {

        String headerString = "";
        String structString = "";
        String elementValueStruct = "";
        String defineElementString = "";
        
        int optionCount = 0;

        Boolean isUnsignedIntegerDefined = false;
        Boolean isStringDefined = false;
        Boolean isEnumValueStructDefined = false;

        for (Element.ElementType type : Element.ElementType.values()) {
            if (type.isSet()) {
                switch (type) {
                case UINT32:
                case HEX32:
                case X_HEX32:
                    if (!isUnsignedIntegerDefined) {
                        /* if not defined yet, then define it */
                        structString += TYPEDEF_STRUCT
                            + "   uint32_t min_value;\n"
                            + "   uint32_t max_value;\n"
                            + "} connector_element_value_unsigned_integer_t;\n";
                        elementValueStruct += "    uint32_t unsigned_integer_value;\n";
                        if(ConfigGenerator.rciParserOption())
                            defineElementString += "#define UNSIGNED_INTEGER_VALUE\n";
                        isUnsignedIntegerDefined = true;
                        optionCount++;
                    }
                    break;
                    
                case INT32:
                    structString += TYPEDEF_STRUCT
                                    + "   int32_t min_value;\n"
                                    + "   int32_t max_value;\n"
                                    + "} connector_element_value_signed_integer_t;\n";
                    elementValueStruct += "    int32_t signed_integer_value;\n";
                    if(ConfigGenerator.rciParserOption())
                        defineElementString += "#define SIGNED_INTEGER_VALUE\n";
                    optionCount++;
                    break;
                    
                case ENUM:
                    if (!isEnumValueStructDefined) {
                        structString += enumStructureString();
                        isEnumValueStructDefined = true;
                    }
                    elementValueStruct += "    unsigned int enum_value;\n";
                    if(ConfigGenerator.rciParserOption())
                        defineElementString += "#define ENUM_VALUE\n";
                    optionCount++;
                    break;
                    
                case FLOAT:
                    structString += TYPEDEF_STRUCT
                                    + "    float min_value;\n"
                                    + "    float max_value;\n"
                                    + "} connector_element_value_float_t;\n";
                    elementValueStruct += "    float float_value;\n";
                    if(ConfigGenerator.rciParserOption())
                        defineElementString += "#define FLOAT_VALUE\n";
                    optionCount++;
                    break;
                    
                case ON_OFF:
                    if (!isEnumValueStructDefined) {
                        /* rci parser needs this structure for on/off type */
                        structString += enumStructureString();
                        isEnumValueStructDefined = true;
                    }
                    elementValueStruct += "    connector_on_off_t  on_off_value;\n";
                    if(ConfigGenerator.rciParserOption())
                        defineElementString += "#define ON_OFF_VALUE\n";
                    optionCount++;
                    break;
                    
                case BOOLEAN:
                    if (!isEnumValueStructDefined) {
                        /* rci parser needs this structure for boolean type */
                        structString += enumStructureString();
                        isEnumValueStructDefined = true;
                    }
                    elementValueStruct += "    connector_bool_t  boolean_value;\n";
                    if(ConfigGenerator.rciParserOption())
                        defineElementString += "#define BOOLEAN_VALUE\n";
                    optionCount++;
                    break;
                    
                default:
                    if (!isStringDefined) {
                        /* if not defined yet then define it */
                        structString += TYPEDEF_STRUCT 
                                        + "    size_t min_length_in_bytes;\n"
                                        + "    size_t max_length_in_bytes;\n"
                                        + "} connector_element_value_string_t;\n";
                        elementValueStruct += "    char const * string_value;\n";
                        if(ConfigGenerator.rciParserOption())
                            defineElementString += "#define STRING_VALUE\n";
                        isStringDefined = true;
                        optionCount++;
                    }
                    break;
                }
            }
        }

        if(ConfigGenerator.rciLegacyEnabled() && !isStringDefined) {
            /* if not defined yet then define it */
            structString += TYPEDEF_STRUCT
                            + "    size_t min_length_in_bytes;\n"
                            + "    size_t max_length_in_bytes;\n"
                            + "} connector_element_value_string_t;\n";
            elementValueStruct += "    char const * string_value;\n";
            isStringDefined = true;
            optionCount++;
        }

        headerString += structString;
        
        if (optionCount > 1) {
            headerString += "\n\ntypedef union {\n";
        } else {
            headerString += "\n\ntypedef struct {\n";
        }

        headerString += elementValueStruct + "} connector_element_value_t;\n";

        if(ConfigGenerator.rciParserOption())
            headerString += defineElementString;
        fileWriter.write(headerString);
    }


    private void writeDefineOptionHeader(ConfigData configData) throws IOException {
        /* if fileType == SOURCE, we should not this function */
        String headerString = "";
        
        if (!ConfigGenerator.excludeErrorDescription()) {
            headerString += DEFINE + RCI_PARSER_USES_ERROR_DESCRIPTIONS;
        }

        String unsignedIntegerString = null;
        String stringsString = null;
        
        String floatInclude = null;

        for (Element.ElementType type : Element.ElementType.values()) {
            if (type.isSet()) {
                
                headerString += DEFINE + RCI_PARSER_USES + type.toName().toUpperCase() + "\n";
                
                switch (type) {
                case UINT32:
                case HEX32:
                case X_HEX32:
                    if (unsignedIntegerString == null) {
                        /* if not defined yet, then define it */
                        unsignedIntegerString = DEFINE + RCI_PARSER_USES_UNSIGNED_INTEGER;
                    }
                    break;
                    
                case INT32:
                case ENUM:
                    break;
                case FLOAT:
                    floatInclude = INCLUDE + FLOAT_HEADER;
                    break;
                    
                case ON_OFF:
                case BOOLEAN:
                    break;
                default:
                    if (stringsString == null) {
                        /* if not defined yet then define it */
                        stringsString = DEFINE + RCI_PARSER_USES_STRINGS;
                    }
                    break;
                }
            } else if (!type.isSet()) {
                switch (type) {
                case ON_OFF:
                    break;
                    
                case BOOLEAN:
                    break;
                default:
                    break;
                }
            }
            
        }
        
        if (unsignedIntegerString != null) headerString += unsignedIntegerString;
        if (stringsString != null) headerString += stringsString;
        
        if (floatInclude != null)
            headerString += "\n\n" + floatInclude;

        fileWriter.write(headerString);
    }

    protected void writeDefinesAndStructures(ConfigData configData) throws IOException {

        writeDefineOptionHeader(configData);

        if(ConfigGenerator.rciLegacyEnabled()){
            fileWriter.write(RCI_LEGACY_DEFINE);
        }

        fileWriter.write(String.format("%sRCI_COMMANDS_ATTRIBUTE_MAX_LEN %d\n", DEFINE,ConfigData.AttributeMaxLen()));


        writeOnOffBooleanEnum();
        writeElementTypeEnum();
        writeElementValueStruct();

        fileWriter.write("\ntypedef enum {\n" +
                         "    connector_request_id_remote_config_session_start,\n" +
                         "    connector_request_id_remote_config_action_start,\n" +
                         "    connector_request_id_remote_config_group_start,\n" +
                         "    connector_request_id_remote_config_group_process,\n" +
                         "    connector_request_id_remote_config_group_end,\n" +
                         "    connector_request_id_remote_config_action_end,\n" +
                         "    connector_request_id_remote_config_session_end,\n" +
                         "    connector_request_id_remote_config_session_cancel");
        if(ConfigGenerator.rciLegacyEnabled()){
            fileWriter.write(",\n    connector_request_id_remote_config_do_command,\n" +
                             "    connector_request_id_remote_config_reboot,\n" +
                             "    connector_request_id_remote_config_set_factory_def");
        }
        fileWriter.write("\n} connector_request_id_remote_config_t;\n");

        fileWriter.write("\ntypedef enum {\n" +
                         "    connector_remote_action_set,\n" +
                         "    connector_remote_action_query");
        if(ConfigGenerator.rciLegacyEnabled()){
            fileWriter.write(",\n    connector_remote_action_do_command,\n" +
                             "    connector_remote_action_reboot,\n" +
                             "    connector_remote_action_set_factory_def");
        }
        fileWriter.write("\n} connector_remote_action_t;\n");

        fileWriter.write(CONNECTOR_REMOTE_GROUP_TYPE);

        fileWriter.write(CONNECTOR_ELEMENT_ACCESS);

        fileWriter.write(CONNECTOR_RCI_INFO);
        fileWriter.write("\ntypedef connector_callback_status_t (*rci_function_t)(rci_info_t * const info, ...);\n");

        writeGroupElementStructs(configData);

        String const_name = "";
        if(ConfigGenerator.useNamesOption() == ConfigGenerator.UseNames.ALL || ConfigGenerator.useNamesOption() == ConfigGenerator.UseNames.GROUPS){
            const_name = "  char const * name;\n";
            fileWriter.write("\n"+ DEFINE + "RCI_PARSER_USES_GROUP_NAMES\n");
        }
        fileWriter.write(String.format("\ntypedef struct {\n" +
            "  connector_remote_group_type_t type;\n" +
            "  unsigned int id;\n" +
            "  unsigned int index;\n" +
            "%s" +
            "} connector_remote_group_t;\n", const_name));

        if(ConfigGenerator.useNamesOption() == ConfigGenerator.UseNames.ALL || ConfigGenerator.useNamesOption() == ConfigGenerator.UseNames.ELEMENTS){
            const_name = "  char const * name;\n";
            fileWriter.write("\n" + DEFINE + "RCI_PARSER_USES_ELEMENT_NAMES\n");
        }
        else
            const_name = "";
        fileWriter.write(String.format("\ntypedef struct {\n" +
            "  unsigned int id;\n" +
            "  connector_element_value_type_t type;\n" +
            "  connector_element_value_t * value;\n" +
            "%s" +
            "} connector_remote_element_t;\n",const_name));

        fileWriter.write(RCI_QUERY_SETTING_ATTRIBUTE_SOURCE);
        fileWriter.write(RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO);

        fileWriter.write("\ntypedef struct {\n" +
                         "  rci_query_setting_attribute_source_t source;\n" +
                         "  rci_query_setting_attribute_compare_to_t compare_to;\n");
        if(ConfigGenerator.rciLegacyEnabled()){
            fileWriter.write("  char const * target;\n");
        }
        fileWriter.write("} connector_remote_attribute_t;\n");

        fileWriter.write(RCI_QUERY_COMMAND_ATTRIBUTE_ID_T);

        if(ConfigGenerator.rciLegacyEnabled()){
            fileWriter.write(RCI_DO_COMMAND_ATTRIBUTE_ID_T);
        }

        fileWriter.write(CONNECTOR_REMOTE_CONFIG_T);
        fileWriter.write(CONNECTOR_REMOTE_CONFIG_CANCEL_T);
        fileWriter.write(CONNECTOR_REMOTE_GROUP_TABLE_T);

    }

    private void writeGroupElementStructs(ConfigData configData) throws IOException {

        String name_g = "";
        String name_e = "";
        int max_len = 0;
        int max_len_el = 0;
        int max_len_enum = 0;

        try {
            for (GroupType type : GroupType.values()) {

                String typeName = type.toString().toLowerCase();
	            LinkedList<Group> typeGroups = configData.getConfigGroup(typeName);

	            for (Group group : typeGroups) {
	                if(max_len < group.getName().length())
	                {
	                    max_len = group.getName().length();
	                }
	                for (Element element : group.getElements()) {
	                    if(max_len_el< element.getName().length())
	                    {
	                        max_len_el = element.getName().length();
	                    }
	                    if (ConfigGenerator.rciParserOption() && Element.ElementType.toElementType(element.getType()) == Element.ElementType.ENUM){
	                        for (ValueStruct value : element.getValues()) {
	                            if(max_len_enum < value.getName().length())
	                            {
	                                max_len_enum = value.getName().length();
	                            }
	                        }
	                    }
	                }
	            }
	        }
        } catch (Exception e) {
            ConfigGenerator.log(e.toString());
        }
        /*if no enums found, disable enumNames option */
        if(max_len_enum == 0)
            ConfigGenerator.setRciParser(false);
        else
            max_len_enum ++; /* '/0'*/
        max_len ++;
        max_len_el ++;

        if (ConfigGenerator.rciParserOption()){
            fileWriter.write(String.format("\ntypedef struct {\n" +
                    "    char name[%d];\n" +
                    "} connector_element_enum_t;\n", max_len_enum));
        }

        switch(ConfigGenerator.useNamesOption()){
            case NONE:
                break;
            case ALL:
            case ELEMENTS:
                name_e = String.format("    char name[%d];\n",max_len_el + 1);
                if(ConfigGenerator.useNamesOption() == ConfigGenerator.UseNames.ELEMENTS)
                    break;
            case GROUPS:
                name_g = String.format("  char name[%d];\n",max_len + 1);
                break;
        }

        fileWriter.write(String.format("\ntypedef struct {\n" +
                "%s" +
                "    connector_element_access_t access;\n" +
                "    connector_element_value_type_t type;\n",name_e));

if(future_feature){
	    fileWriter.write("    rci_function_t set_cb;\n" +
                "    rci_function_t get_cb;\n");
}

		if(ConfigGenerator.rciParserOption()){
			fileWriter.write("    struct {\n"+
				    "        size_t count;\n"+
				    "        connector_element_enum_t CONST * CONST data;\n"+
				    "    } enums;\n");
		}

		fileWriter.write("} connector_group_element_t;\n");

        fileWriter.write(String.format("\ntypedef struct {\n" +
                "%s" +
                "  size_t instances;\n" +
                "  struct {\n" +
                "      size_t count;\n" +
                "      connector_group_element_t CONST * CONST data;\n" +
                "  } elements;\n\n" +
                "  struct {\n" +
                "      size_t count;\n" +
                "      char CONST * CONST * description;\n" +
                "  } errors;\n\n",name_g));
if(future_feature){
		fileWriter.write("  rci_function_t start_cb;\n" +
                "  rci_function_t end_cb;\n");
}
		fileWriter.write("} connector_group_t;\n");

    }

    protected void writeRemoteAllStrings(ConfigData configData, BufferedWriter bufferWriter) throws Exception {
        if (!ConfigGenerator.excludeErrorDescription()) {
            bufferWriter.write(String.format("\nchar CONST %s[] = {\n",
                    CONNECTOR_REMOTE_ALL_STRING));
        }
        
        writeRciErrorsRemoteAllStrings(configData, bufferWriter);

        for (GroupType type : GroupType.values()) {
            LinkedList<Group> theConfig = null;

            configType = type.toString().toLowerCase();

            theConfig = configData.getConfigGroup(configType);

            if (!theConfig.isEmpty()) {
                writeGroupRemoteAllStrings(theConfig, bufferWriter);
            }
        }
        writeErrorsRemoteAllStrings(configData, bufferWriter);

        if (!ConfigGenerator.excludeErrorDescription()) {
            bufferWriter.write("\n};\n\n"); // end of CONNECTOR_REMOTE_ALL_STRING
        }
    }

    protected void writeDefineGroupErrors(ConfigData configData,BufferedWriter bufferWriter) throws Exception {
        for (GroupType type : GroupType.values()) {
            String defineName = null;
            LinkedList<Group> groups = null;

            configType = type.toString().toLowerCase();

            groups = configData.getConfigGroup(configType);

            for (Group group : groups) {
                defineName = getDefineString(group.getName());
                /* define name string index
                 * #define [group name]
                 */

                if ((!ConfigGenerator.excludeErrorDescription()) && (!group.getErrors().isEmpty())) {
                    LinkedHashMap<String, String> errorMap = group.getErrors();
                    for (String key : errorMap.keySet()) {
                        defineName = getDefineString(group.getName() + "_" + ERROR + "_" + key);
                        /* define name string index for each error in the group
                         * #define [group name + ERROR + error name]
                         */
                        bufferWriter.write(getDefineStringIndex(defineName, errorMap.get(key), ConfigGenerator.FileType.SOURCE));
                    }
                }
            }
        }

    }

    private void writeGroupRemoteAllStrings(LinkedList<Group> groups, BufferedWriter bufferWriter) throws Exception {
        for (Group group : groups) {
            if ((!ConfigGenerator.excludeErrorDescription()) && (!group.getErrors().isEmpty())) {
                LinkedHashMap<String, String> errorMap = group.getErrors();
                for (String key : errorMap.keySet()) {
                    bufferWriter.write(getCharString(errorMap.get(key)));
                }
            }
        }
    }

    private void writeDefineErrors(String prefixName, LinkedHashMap<String, String> errorMap, ConfigGenerator.FileType fileType,BufferedWriter bufferWriter) throws IOException {
        for (String key : errorMap.keySet()) {
            String defineName = prefixName.toUpperCase() + "_" + key.toUpperCase();
            /* define name string index for each error */
            bufferWriter.write(getDefineStringIndex(defineName, errorMap.get(key), fileType));
        }
    }

    protected void writeDefineGlobalErrors(ConfigData configData, BufferedWriter bufferWriter) throws IOException {
        if (!ConfigGenerator.excludeErrorDescription()) {
            writeDefineErrors(GLOBAL_ERROR, configData.getUserGlobalErrors(), ConfigGenerator.FileType.SOURCE,bufferWriter);
        }
    }

    protected void writeDefineRciErrors(ConfigData configData,BufferedWriter bufferWriter) throws IOException {
        if (!ConfigGenerator.excludeErrorDescription()) {
            writeDefineErrors(GLOBAL_RCI_ERROR, configData.getRciGlobalErrors(), ConfigGenerator.FileType.SOURCE,bufferWriter);
        }
    }
    
    private void writeLinkedHashMapStrings(LinkedHashMap<String, String> stringMap,BufferedWriter bufferWriter) throws IOException {
        for (String key : stringMap.keySet()) {
            bufferWriter.write(getCharString(stringMap.get(key)));
        }
    }

    private void writeErrorsRemoteAllStrings(ConfigData configData, BufferedWriter bufferWriter) throws IOException {
        if (!ConfigGenerator.excludeErrorDescription()) {
            writeLinkedHashMapStrings(configData.getUserGlobalErrors(),bufferWriter);
        }
    }

    private void writeRciErrorsRemoteAllStrings(ConfigData configData, BufferedWriter bufferWriter) throws IOException {
        if (!ConfigGenerator.excludeErrorDescription()) {
            writeLinkedHashMapStrings(configData.getRciGlobalErrors(),bufferWriter);
        }
    }

    private void writeEnumArrays(String group_name, LinkedList<Element> elements, BufferedWriter bufferWriter) throws Exception{

        for (int element_index = 0; element_index < elements.size(); element_index++) {

            Element element = elements.get(element_index);
            if (Element.ElementType.toElementType(element.getType()) == Element.ElementType.ENUM){
                boolean first = true;
                String define_name = getDefineString(group_name + "_" + element.getName() + "_enum");
                bufferWriter.write(String.format("static connector_element_enum_t CONST %s%s[] = {\n",
                        prefix,define_name.toLowerCase()));
                for (ValueStruct value : element.getValues()) {
                    if(first){
                        bufferWriter.write(String.format("    {\"%s\"}",value.getName()));
                        first = false;
                    }
                    else
                        bufferWriter.write(String.format(",\n    {\"%s\"}",value.getName()));
                }
                bufferWriter.write("\n};\n\n");
            }
        }
    }

    private void writeElementArrays(String group_name, LinkedList<Element> elements, BufferedWriter bufferWriter) throws Exception {
        /* write group element structure array */
        if (ConfigGenerator.rciParserOption()){
            writeEnumArrays(group_name, elements, bufferWriter);
        }

        bufferWriter.write(String.format("static connector_group_element_t CONST %s%s_elements[] = {",
                                        prefix,getDefineString(group_name).toLowerCase().replaceAll("\\W", "_")));

        for (int element_index = 0; element_index < elements.size(); element_index++) {
            Element element = elements.get(element_index);
            String element_string = "";
            if(ConfigGenerator.useNamesOption() == ConfigGenerator.UseNames.NONE || ConfigGenerator.useNamesOption() == ConfigGenerator.UseNames.GROUPS){
                element_string = String.format("\n { %s", COMMENTED(element.getName()));
            }
            else{
                element_string = String.format("\n { \"%s\",\n", element.getName());
            }

            element_string += String.format("   %s,\n",  getElementDefine("access", getAccess(element.getAccess())));
            element_string += String.format("   %s", getElementDefine("type", element.getType()));

if(future_feature){

            if(!getAccess(element.getAccess()).equalsIgnoreCase("read_only")) {
	            element_string += String.format(",\n   %s%srci_%s_%s_set,\n",RCI_FUNCTION_T,prefix, getDefineString(group_name).toLowerCase(),element.getName());
            }
            else{
                element_string +=",\n   NULL,\n";
            }

            element_string += String.format("   %s%srci_%s_%s_get", RCI_FUNCTION_T,prefix,getDefineString(group_name).toLowerCase(),element.getName());
}

	if(ConfigGenerator.rciParserOption()){
		 if (Element.ElementType.toElementType(element.getType()) == Element.ElementType.ENUM){
			String define_name = getDefineString(group_name + "_" + element.getName() + "_enum");
			element_string += String.format(",\n   { ARRAY_SIZE(%s%s),\n", prefix, define_name.toLowerCase())
				    + String.format("     %s%s\n   }\n", prefix, define_name.toLowerCase());
		 }
		 else {
			 element_string += ",\n   { 0,\n     NULL\n   }\n";
		 }
	}
	else{
		element_string +="\n";
	}
            element_string += " }";

            if (element_index < (elements.size() - 1)) {
                element_string += ",";
            }

            bufferWriter.write(element_string);
        }
        bufferWriter.write("\n};\n\n");

    }

    protected void writeGlobalErrorStructures(ConfigData configData, BufferedWriter bufferWriter) throws IOException {
        
        if (!ConfigGenerator.excludeErrorDescription()) {
            int errorCount = configData.getRciGlobalErrors().size()
                            + configData.getUserGlobalErrors().size();

            if (errorCount > 0) {
                String staticString = "static ";
                
                if (ConfigGenerator.fileTypeOption() == ConfigGenerator.FileType.SOURCE)  staticString = "";

                bufferWriter.write(String.format("%schar const * const %ss[] = {\n", staticString, GLOBAL_RCI_ERROR));
                        
                /* top-level global errors */
                errorCount = writeErrorStructures(errorCount, GLOBAL_RCI_ERROR,
                             configData.getRciGlobalErrors(),  bufferWriter);

                /* group global errors */
                errorCount = writeErrorStructures(errorCount, GLOBAL_ERROR,
                             configData.getUserGlobalErrors(), bufferWriter);

                bufferWriter.write("};\n\n");
            }
        }
    }

    private int writeErrorStructures(int errorCount, String defineName, LinkedHashMap<String, String> errorMap, BufferedWriter bufferWriter) throws IOException {
        
        for (String key : errorMap.keySet()) {
            bufferWriter.write(getRemoteString(defineName.toUpperCase() + "_" + key));
            errorCount--;
            if (errorCount > 0) {
                bufferWriter.write(",");
            }
            bufferWriter.write(COMMENTED(key));
        }

        return errorCount;
    }

    private void writeErrorStructures(String error_name, LinkedHashMap<String, String> localErrors , BufferedWriter bufferWriter) throws IOException {
        
        if (!ConfigGenerator.excludeErrorDescription()) {
            String define_name;

            if (!localErrors.isEmpty()) {
                define_name = getDefineString(error_name + "_" + ERROR);
                bufferWriter.write(CHAR_CONST_STRING + prefix + define_name.toLowerCase() + "s[] = {\n");

                /* local local errors */
                define_name = getDefineString(error_name + "_" + ERROR);
                int error_count = localErrors.size();
                writeErrorStructures(error_count, define_name, localErrors, bufferWriter);

                bufferWriter.write("};\n\n");
            }
        }
    }

    private void writeGroupStructures(LinkedList<Group> groups, BufferedWriter bufferWriter) throws Exception {
        
        for (int group_index = 0; group_index < groups.size(); group_index++) {
            Group group = groups.get(group_index);

            /* write element structure */
            writeElementArrays(group.getName(), group.getElements(), bufferWriter);

            writeErrorStructures(group.getName(), group.getErrors(), bufferWriter);
        }

    }

    protected void writeAllStructures(ConfigData configData, BufferedWriter bufferWriter) throws Exception {
        String define_name;

        for (GroupType type : GroupType.values()) {
            LinkedList<Group> groups = null;

            configType = type.toString().toLowerCase();

            groups = configData.getConfigGroup(configType);

            if (!groups.isEmpty()) {
                writeGroupStructures(groups, bufferWriter);

                bufferWriter.write(String.format("static connector_group_t CONST %sconnector_%s_groups[] = {", prefix, configType));

                for (int group_index = 0; group_index < groups.size(); group_index++) {
                    Group group = groups.get(group_index);

                    define_name = getDefineString(group.getName().replaceAll("\\W", "_") + "_elements");
                    String group_string = "";

                    if(ConfigGenerator.useNamesOption() == ConfigGenerator.UseNames.NONE || ConfigGenerator.useNamesOption() == ConfigGenerator.UseNames.ELEMENTS){
                        group_string = String.format("\n { %s", COMMENTED(group.getName()));
                    }
                    else{
                        group_string = String.format("\n { \"%s\",\n", group.getName());
                    }

                    group_string += String.format("   %d ,%s", group.getInstances(),COMMENTED(" instances "))
                                  + String.format("   { ARRAY_SIZE(%s%s),\n", prefix, define_name.toLowerCase())
                                  + String.format("     %s%s\n   },\n", prefix, define_name.toLowerCase());
                    if ((!ConfigGenerator.excludeErrorDescription()) && (!group.getErrors().isEmpty())) {
                        define_name = getDefineString(group.getName() + "_errors");

                        group_string += String.format("   { ARRAY_SIZE(%s%s),\n", prefix, define_name.toLowerCase())
                                        + String.format("     %s%s\n   }", prefix, define_name.toLowerCase());

                    } else {
                        group_string += "   { 0,\n     NULL\n   }";
                    }
if(future_feature){
                    group_string +=  String.format(", %s",COMMENTED(" errors"));

                    group_string += String.format("   %s%srci_%s_start,\n" , RCI_FUNCTION_T,prefix,getDefineString(group.getName()).toLowerCase())
                                + String.format("   %s%srci_%s_end\n }\n" , RCI_FUNCTION_T,prefix,getDefineString(group.getName()).toLowerCase());
}
else{
					group_string +=  String.format(" %s}\n",COMMENTED(" errors"));
}
                    if (group_index < (groups.size() - 1)) {
                        group_string += ",";
                    }

                    bufferWriter.write(group_string);
                }
                bufferWriter.write("\n};\n\n");
            }
        }

        String rciGroupString = "static ";
        if (ConfigGenerator.fileTypeOption() == ConfigGenerator.FileType.SOURCE) rciGroupString = "";
        
        rciGroupString += String.format("connector_remote_group_table_t CONST %s[] = {\n",
                                                CONNECTOR_REMOTE_GROUP_TABLE);

        for (GroupType type : GroupType.values()) {
            LinkedList<Group> groups = null;

            configType = type.toString().toLowerCase();

            groups = configData.getConfigGroup(configType);

            if (type.getIndex() != 0) {
                rciGroupString += ",\n";
            }

            rciGroupString += " {";
            if (!groups.isEmpty()) {
                rciGroupString += String.format(" %sconnector_%s_groups,\n   ARRAY_SIZE(%sconnector_%s_groups)\n }",
                                                   prefix,configType,prefix, configType);

            } else {
                rciGroupString += "NULL,\n 0\n }";
            }

        }
        rciGroupString += "\n};\n\n";

        bufferWriter.write(rciGroupString);
    }

    protected void writeErrorHeader(int errorIndex, String enumDefine, LinkedHashMap<String, String> errorMap, BufferedWriter bufferWriter) throws IOException {
        
        for (String key : errorMap.keySet()) {
            String error_string = " " + enumDefine + "_" + key;

            if (errorIndex == 1) {
                error_string += " = " + " " + enumDefine + "_" + OFFSET_STRING;
            }
            errorIndex++;

            error_string += ",\n";

            bufferWriter.write(error_string);
        }

    }

    protected void writeErrorHeader(String type,int errorIndex, String enumDefine, LinkedHashMap<String, String> errorMap, BufferedWriter bufferWriter) throws IOException {

        for (String key : errorMap.keySet()) {
            String error_string = enumDefine + "_" + key;

            if(type.equalsIgnoreCase("rci")){
                if (errorIndex == 1) error_string += " = 1," + COMMENTED(" Protocol defined ");
                else error_string += ",\n";
                errorIndex++;
            } else if(type.equalsIgnoreCase("global")){
                if (errorIndex == 1) error_string += "," +  COMMENTED(" User defined (global errors) ");
                else error_string += ",\n";
                errorIndex++;
            } else {
                error_string += ",\n";
            }
            bufferWriter.write(error_string);
        }

    }

    protected void writeRciErrorEnumHeader(ConfigData configData, BufferedWriter bufferWriter ) throws IOException {

    /* write typedef enum for rci errors */
        bufferWriter.write("\n" + TYPEDEF_ENUM + " " + GLOBAL_RCI_ERROR + "_" + OFFSET_STRING + " = 1,\n");
        writeErrorHeader(configData.getRciGlobalErrorsIndex(),GLOBAL_RCI_ERROR, configData.getRciGlobalErrors(), bufferWriter);
        bufferWriter.write(" " + GLOBAL_RCI_ERROR + "_" + COUNT_STRING + "\n} " + prefix  + GLOBAL_RCI_ERROR + ID_T_STRING);
    }

    protected void writeGlobalErrorEnumHeader(ConfigData configData, BufferedWriter bufferWriter) throws IOException {

        String index_string = "";
        if(bufferWriter == FileSource.getHeaderWriter())
        {
            index_string = "_INDEX";
            fileWriter.write(String.format("unsigned int %s_%s = %s_%s%s;\n\n", GLOBAL_ERROR, COUNT_STRING, GLOBAL_ERROR, COUNT_STRING, index_string));
        }
        /* write typedef enum for user global error */
        String enumName = GLOBAL_ERROR + "_" + OFFSET_STRING;

        bufferWriter.write("\n" + TYPEDEF_ENUM + " " + enumName + " = " + GLOBAL_RCI_ERROR + "_" + COUNT_STRING + ",\n");

        writeErrorHeader(1,GLOBAL_ERROR, configData.getUserGlobalErrors(), bufferWriter);

        String endString = String.format(" %s_%s%s", GLOBAL_ERROR, COUNT_STRING, index_string);
        
        if (configData.getUserGlobalErrors().isEmpty()) {
            endString += " = " + enumName;
        }
        endString += "\n} " + prefix + GLOBAL_ERROR + ID_T_STRING;
        
        bufferWriter.write(endString);
        
    }

    private void writeEnumHeader(ConfigData configData,LinkedList<Group> groups, BufferedWriter bufferWriter) throws Exception {

        for (Group group : groups) {
            /* build element enum string for element enum */
            String element_enum_string = TYPEDEF_ENUM;

            for (Element element : group.getElements()) {
                /* add element name */
                element_enum_string += getEnumString(group.getName() + "_"
                        + element.getName())
                        + ",\n";

                if (Element.ElementType.toElementType(element.getType()) == Element.ElementType.ENUM) {
                    /* write typedef enum for value */
                    bufferWriter.write(TYPEDEF_ENUM);

                    boolean previousempty = false;
                    int i = 0;
                    for (ValueStruct value : element.getValues()) {
                        if(value.getName().equalsIgnoreCase(""))
                            previousempty = true;
                        else if(previousempty){
                            bufferWriter.write(getEnumString(group.getName() + "_" + element.getName() + "_" + value.getName().replaceAll("[^a-zA-Z_0-9]", "_")) + " = " + i + ",\n");
                            previousempty = false;
                        }
                        else
                            bufferWriter.write(getEnumString(group.getName() + "_" + element.getName() + "_" + value.getName().replaceAll("[^a-zA-Z_0-9]", "_")) + ",\n");
                        i = i+1;
                    }
                    /* done typedef enum for value */
                    bufferWriter.write(endEnumString(group.getName() + "_" + element.getName()));
                }
            }
            /* done typedef enum for element */

            element_enum_string += endEnumString(group.getName());

            bufferWriter.write(element_enum_string);

            if (!group.getErrors().isEmpty()) {
                bufferWriter.write(TYPEDEF_ENUM);

                writeErrorHeader("rci",1, getEnumString(group.getName() + "_" + ERROR), configData.getRciGlobalErrors(), bufferWriter);
                writeErrorHeader("global",1, getEnumString(group.getName() + "_" + ERROR), configData.getUserGlobalErrors(), bufferWriter);

                LinkedHashMap<String, String> errorMap = group.getErrors();
                int index = 0;

                for (String key : errorMap.keySet()) {
                    String enumString = getEnumString(group.getName() + "_" + ERROR + "_" + key);

                    if (index++ == 0) {
                        /*Set start index to the global count */
                        String startIndexString = COUNT_STRING;
                        if(bufferWriter == FileSource.getHeaderWriter()){
                            startIndexString += "_INDEX";
                            enumString += " = " + GLOBAL_ERROR + "_" + startIndexString;
                        }
                        else{
                            enumString += ", " + COMMENTED(" User defined (group errors)");
                        }
                    }
                    else{
                    enumString += ",\n";
                    }

                    bufferWriter.write(enumString);
                }
                bufferWriter.write(endEnumString(group.getName() + "_" + ERROR));
            }

        }

    }

    protected void writeGroupTypeAndErrorEnum(ConfigData configData,BufferedWriter bufferWriter) throws Exception {

        for (GroupType type : GroupType.values()) {
	        LinkedList<Group> groups = null;

	        configType = type.toString().toLowerCase();
	        groups = configData.getConfigGroup(configType);

	        if (!groups.isEmpty()) {
	            /* build group enum string for group enum */
	            String group_enum_string = TYPEDEF_ENUM;

	            /* Write all enum in H file */
	            writeEnumHeader(configData,groups, bufferWriter);

	            for (Group group : groups) {
	                /* add each group enum */
                    group_enum_string += getEnumString(group.getName()) + ",\n";
	            }

	            /* write group enum buffer to fileWriter */
	            group_enum_string += endEnumString(null);
	            bufferWriter.write(group_enum_string);
	        }
	    }
    }

    private String COMMENTED(String comment) {
        return " /*" + comment + "*/\n";
    }

    private String getEnumString(String enum_name) {
        String str = " " + CONNECTOR_PREFIX + "_" + configType;

        if (enum_name != null) {
            str += "_" + enum_name.replaceAll("\\W", "_");
        }
        return str;
    }

    private String endEnumString(String group_name) {
        /*Add _COUNT */
        String str = " " + CONNECTOR_PREFIX + "_" + configType;
        if(group_name!=null)
           str += "_" + group_name.replaceAll("\\W", "_");
        str += "_" + COUNT_STRING +"\n";

        str += "} " + prefix + CONNECTOR_PREFIX + "_" + configType;
        if(group_name!=null)
            str += "_" + group_name.replaceAll("\\W", "_");
        str +=ID_T_STRING;

        return str;
    }

    private String getDefineString(String define_name) {
        return (configType.toUpperCase() + "_" + define_name.toUpperCase());
    }

    private String getDefineStringIndex(String define_name, String string, ConfigGenerator.FileType fileType) {
        String str = DEFINE + define_name + " " + "(" + CONNECTOR_REMOTE_ALL_STRING + "+" + prevRemoteStringLength + ")\n";
        /* NOTE: this prevRemoteStringLength is important.
         * Must keep track this variable for #define string.
         */
        if (string != null) {
            prevRemoteStringLength += string.length();
        }
        prevRemoteStringLength++; /* add extra byte for the size */
        
        if (fileType == ConfigGenerator.fileTypeOption() || 
            ConfigGenerator.fileTypeOption() == ConfigGenerator.FileType.NONE) {
            return str;
        } else {
            return "";
        }
    }

    private String getCharString(String string) {
        
        String quote_char = (isFirstRemoteString) ? "": ",\n";
        
        isFirstRemoteString = false;
        
        if (string != null) {
            
            char[] characters = string.toCharArray();
    
            quote_char += " " + string.length() + ",";
    
            int length = characters.length;
            
            for (int i=0; i < length; i++)
            {
                quote_char += "\'" + characters[i] + "\'";
                if (i < length-1) {
                    //last character
                    quote_char += ",";
                }
            }
        }
        else {
            quote_char += " 0";
        }

        return quote_char;
    }

    private String getRemoteString(String define_name) {
        return (" " + define_name.toUpperCase());

    }

    private String getElementDefine(String type_name, String element_name) {
        return (String.format("%s_element_%s_%s", CONNECTOR_PREFIX, type_name, element_name));
    }

    protected String getAccess(String access) {
        if (access == null) {
            return "read_write";
        }
        return access;
    }

}
