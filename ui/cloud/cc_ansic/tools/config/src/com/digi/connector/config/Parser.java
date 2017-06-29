package com.digi.connector.config;

import java.io.IOException;
import java.math.BigDecimal;
import java.util.LinkedList;

import javax.script.ScriptEngine;
import javax.script.ScriptEngineManager;

public class Parser {

    private final static int MAX_DESCRIPTION_LENGTH = 200;
    private final static int MAX_NAME_LENGTH = 40;

    // PRIVATE variables
    private static TokenScanner tokenScanner;
    private static boolean isReadToken;
    private static String token;
    private static int groupLineNumber;
    private static int elementLineNumber;
    private static LinkedList<Group> groupConfig;

    
    public static void processFile(String fileName, ConfigData configData) throws IOException, NullPointerException {

        try {
            tokenScanner = new TokenScanner(fileName);

            token = null;
            isReadToken = true;

            /* first use a Scanner to get each word */
            while (!isReadToken || tokenScanner.hasToken()) {
                if (isReadToken) {
                    token = tokenScanner.getToken();
                }

                if (token.equalsIgnoreCase("globalerror")) {
                    configData.addUserGroupError(getName(), getLongDescription());
                    isReadToken = true;
                    
                } else if (token.equalsIgnoreCase("group")) {
                    /*
                     * syntax for parsing group: group setting or state <name>
                     * [instances] <description> [help description]
                     */
                    
                    /* parse setting or state */
                    String groupType = tokenScanner.getToken();
                    
                    /* parse name */
                    String nameStr = getName();

                    /* make sure group name doesn't exist in setting and state config */

                    String typeName = groupType.toString().toLowerCase();
                    LinkedList<Group> typeGroups = configData.getConfigGroup(typeName);

                    for (Group group : typeGroups) {
                        if (group.getName().equals(nameStr)) {
                            /* duplicate */
                            String desc = "name";
                            throw new Exception("Duplicate <group> " + desc + ": " + nameStr);
                        }
                    }

                    groupConfig = configData.getConfigGroup(groupType);

                    
                    groupLineNumber = tokenScanner.getLineNumber();

                    /* parse instances */
                    int groupInstances = 1;

                    if (tokenScanner.hasTokenInt()) {
                        groupInstances = tokenScanner.getTokenInt();
                    }
                    else if (tokenScanner.hasToken("\\(.*")){
                        groupInstances = getMathExpression();
                    }

                    Group theGroup = new Group(nameStr, groupInstances, getDescription(), getLongDescription());

                    isReadToken = true;
                    /*
                     * Parse elements and errors for the group.
                     */
                    while (tokenScanner.hasToken()) {
                        if (isReadToken) {
                            token = tokenScanner.getToken();
                            isReadToken = false; /*
                                                  * token is already obtained
                                                  * from processElement
                                                  */
                        }
                        
                        if (token.equalsIgnoreCase("element")) {
                            Element element = processElement();

                            try{
                                element.validate();
                            }
                            catch(Exception e){
                                throw new Exception("Error in <element>: " + element.getName() + "\n\t" + e.getMessage());
                            }

                            theGroup.addElement(element);
                            
                        } else if (token.equalsIgnoreCase("error")) {
                            theGroup.addError(getName(), getLongDescription());
                            isReadToken = true;
                            
                        } else if (token.startsWith("#")) {
                            tokenScanner.skipCommentLine();
                            isReadToken = true;
                            
                        } else {
                            break;
                        }
                    }

                    try{
                        theGroup.validate();
                    }
                    catch(Exception e){
                        throw new Exception("Error in <group>: " + theGroup.getName() + "\n\t" + e.getMessage());
                    }

                    groupConfig.add(theGroup);

//                    if (!tokenScanner.hasToken()) {
                        /* end of file */
//                        break;
//                    }
                } else if (token.startsWith("#")) {
                    tokenScanner.skipCommentLine();
                    isReadToken = true;
                    
                } else {
                    throw new Exception("Unrecognized keyword: " + token);
                }
            }
        } catch (NullPointerException e) {
            ConfigGenerator.log("Parser NullPointerException");
            ConfigGenerator.log(e.toString());
            throw new NullPointerException();

        } catch (Exception e) {
            throw new IOException(errorFoundLog(fileName, e.getMessage()));
        }
        finally {
            tokenScanner.close();
        }

    }

    private static String errorFoundLog(String fileName, String str) {
        String message = "Error found in " + fileName + ", line ";

        if (str.indexOf("<group>") != -1)
            message += groupLineNumber;
        else if (str.indexOf("<element>") != -1)
            message += elementLineNumber;
        else
            message += tokenScanner.getLineNumber();

        return message + ": " + str;
    }

    private static String getName() throws Exception {
        String name = tokenScanner.getToken(); // tokenScanner.next();

        if (name == null) {
            throw new Exception("Missing name!");
        }
        if (name.length() > MAX_NAME_LENGTH) {
            throw new Exception("The name > the maximum length limited " + MAX_NAME_LENGTH);
        }
        /* Now the descriptor can contain no alphanumeric chars,
         * and the enums will replace them with "_"
            if(!checkAlphaCharacters(name)){
                throw new Exception("Invalid character in the name: " + name);
            }
        }*/
        return name;
    }

    private static int getMathExpression() throws Exception {

        String ex = null;
        int result = 0;

        if (tokenScanner.hasToken("\\(.*")) {
            ex = tokenScanner.getToken();
            int count = ex.replace(")", "").length() - ex.replace("(", "").length();

            /*read Tokens till we have the same number of '(' and ')' in the expression */
            while(count > 0){
                if(tokenScanner.hasToken("\\\".*"))
                    throw new Exception("Invalid Math Expression, missing ')'");
                ex += tokenScanner.getToken();
                count = ex.replace(")", "").length() - ex.replace("(", "").length();
            }

            if(count < 0)
                throw new Exception("Invalid Math Expression, missing '('");

            ScriptEngineManager mgr = new ScriptEngineManager();
            ScriptEngine engine = mgr.getEngineByName("js");
            /*eval returns an Object that is a Double */
            try{
                result = new BigDecimal(engine.eval(ex).toString()).intValue();
            }
            catch (Exception e){
                throw new Exception("Bad Expression " + e);
            }

        }
        return result;
    }

    private static String getQuotedName() throws Exception {

        String name = null;
        if (tokenScanner.hasToken("\\\".*")) {
            name = tokenScanner.getTokenInLine("\\\".*?\\\"");
            if (name == null) {
                throw new Exception("Missing name!");
            }

            name = name.substring(1, name
                    .lastIndexOf("\""));

            if (name.length() > MAX_NAME_LENGTH) {
                throw new Exception("The name > the maximum length limited " + MAX_NAME_LENGTH);
            }
        }
        return name;
    }

    private static String getDescription() throws Exception {

        String description = null;
        if (tokenScanner.hasToken("\\\".*")) {
            description = tokenScanner.getTokenInLine("\\\".*?\\\"");
            if (description == null) {
                throw new Exception("Invalid description");
            }

            description = description.substring(1, description
                    .lastIndexOf("\""));

            if (description.length() > MAX_DESCRIPTION_LENGTH) {
                throw new Exception("description > maximum length "
                        + MAX_DESCRIPTION_LENGTH);
            }

            description = description.replace(":", "::");

            if (description.length() == 0)
                description = null;

        }
        return description;
    }

    private static String getLongDescription() throws Exception {

        String description = null;
        if (tokenScanner.hasToken("\\\".*")) {
            description = tokenScanner.getTokenInLine("\\\".*?\\\"");
            if (description == null) {
                throw new Exception("Invalid error description");
            }

            description = description.substring(1, description
                    .lastIndexOf("\""));
            description = description.replace(":", "::");

            if (description.length() == 0)
                description = null;
        }
        return description;
    }

    private static String getType() throws Exception {
        String type = tokenScanner.getToken();

        if (type == null) {
            throw new Exception("Missing type");
        }
        return type;
    }

    private static String getAccess() throws Exception {
        String access = tokenScanner.getToken();

        if (access == null) {
            throw new Exception("Missing access!");

        }
        Element.AccessType.toAccessType(access);

        return access;
    }

    private static String getMinMax() throws Exception {
        String mvalue = null;

        if (tokenScanner.hasToken("\\(.*"))
            mvalue = Integer.toString(getMathExpression());
        else
            mvalue = tokenScanner.getToken();

        if (mvalue == null) {
            throw new Exception("Missing min or max value");
        }

        return mvalue;
    }

    private static final Element processElement() throws Exception {
        /*
         * syntax for parsing element: element <name> <description> [help
         * description] type <type> [min <min>] [max <max>] [access <access>]
         * [units <unit>]
         */
        Element element = new Element(getName(), getDescription(), getLongDescription());
        
        elementLineNumber = tokenScanner.getLineNumber();
        try {

            while (tokenScanner.hasToken()) {
                token = tokenScanner.getToken();
                isReadToken = true; 

                if (token.equalsIgnoreCase("type")) {
                    element.setType(getType());
                    
                } else if (token.equalsIgnoreCase("access")) {
                    element.setAccess(getAccess());
                    
                } else if (token.equalsIgnoreCase("min")) {
                    element.setMin(getMinMax());
                    
                } else if (token.equalsIgnoreCase("max")) {
                    element.setMax(getMinMax());

                } else if (token.equalsIgnoreCase("units")) {
                    element.setUnit(getDescription());
                    
                } else if (token.equalsIgnoreCase("value")) {
                    /*
                     * Parse Value for element with enum type syntax for parsing
                     * value: value <name> [description] [help description]
                     */
                    if (tokenScanner.hasToken("\\\".*"))
                        element.addValue(getQuotedName(), getDescription(), getLongDescription());
                    else
                        element.addValue(getName(), getDescription(), getLongDescription());
                } else if (token.startsWith("#")) {
                    tokenScanner.skipCommentLine();
                    
                } else {
                    isReadToken = false; 
                    break;
                }
            }

            
        } catch (IOException e) {
            throw new IOException(e.toString());
        }

        return element;

    }

}
