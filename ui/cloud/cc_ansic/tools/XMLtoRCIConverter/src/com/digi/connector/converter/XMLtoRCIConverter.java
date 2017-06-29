package com.digi.connector.converter;

import java.io.File;
import java.util.ArrayList;

import com.digi.connector.converter.ParserXML;

public class XMLtoRCIConverter {

	private final static String DIRECTORY_OPTION = "path";
	private final static String NAME_OPTION = "filename";
	private final static String XML_FILENAME = "XMLFileName";
	private final static String DASH = "-";

	/*args variables*/
    private static ArrayList<String> fileNames = new ArrayList<String>();
    private static String directoryPath;
    private static String fileOut;

    private static void usage() {
        String className = XMLtoRCIConverter.class.getName();

        int firstChar = className.lastIndexOf(".") + 1;
        if (firstChar != -1) {
            className = className.substring(firstChar);
        }

        log(String.format("\nUsage: java -jar %s.jar [", className)
        	    + DASH
        	    + DIRECTORY_OPTION
        	    + "] ["
                + DASH
                + NAME_OPTION
                + "] "
        	    + String.format("<%s>\n",XML_FILENAME));

        log("Description:");
        log("\tIt generates a configuration file : config.rci");

        log("Options:");
        log(String.format("\t%-16s \t= optional directory path where the configuration file will be created.",
               DASH + DIRECTORY_OPTION + "=<directory path>"));
        log(String.format("\t%-16s \t= optional output name for the configuration file.",
                DASH + NAME_OPTION + "=<output filename>"));
        log(String.format("\n\t%-16s \t= The XML file",XML_FILENAME));

        System.exit(1);
    }

    public static void log(Object aObject) {
        System.out.println(String.valueOf(aObject));
    }

    public XMLtoRCIConverter(String args[]) {
        int argCount = 0;

        try{
	        for (String arg : args) {
	            if (arg.startsWith(DASH)) {
	                String option = arg.substring(DASH.length());
	
	                String[] keys = option.split("=", 2);
	                
	                if(keys.length == 2) {
	                	if (keys[0].equals(DIRECTORY_OPTION)){
	                		if (new File(keys[1]).isDirectory()) {
	                            directoryPath = keys[1];
	                        } else {
	                            throw new Exception("Invalid directory path!");
	                        }
	                	} else if(keys[0].equals(NAME_OPTION)){
	                			fileOut = keys[1];
	                	}else {
	                        throw new Exception("Invalid Option: " + keys[0]);
	                	}
	                	
	                } else if (option.isEmpty()) {
	                    throw new Exception("Missing Option!");
	                } else {
	                    throw new Exception("Invalid Option: " + option);
	                }
	
	            } else {
	            	if(arg.endsWith(".xml") && new File(arg).isFile()){
                        fileNames.add(arg);
                        argCount++;
	            	}
	            	else
	            		throw new Exception ("Invalid input file: " + arg);
	            }
	        }
        } catch (Exception e){
        	log(e.getMessage());
            usage();
        }

        if (argCount < 1 ) {
            log("Missing arguments!");
            usage();
        }

    }

    public static void main(String[] args) {

        try {
            new XMLtoRCIConverter(args);

            ParserXML.processFile(fileNames,directoryPath,fileOut);
            
            log("\nDone.\n");
            System.exit(0);
        }
        catch (Exception e) {
        	log(e.getMessage());
        }
    }

}
