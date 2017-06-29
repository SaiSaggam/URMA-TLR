package com.digi.connector.converter;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import org.w3c.dom.Element;
import org.xml.sax.InputSource;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.StringReader;
import java.io.StringWriter;
import java.io.Writer;
import java.util.ArrayList;


public class ParserXML{

    private static NodeList nodeList;
    private static int j;
    private static int len;
    private static String groupType;
    private static String groupName;
    private static String groupDesc;
    private static String attrMax;
    private static BufferedWriter rciWriter = null;

    public static void processFile(ArrayList<String> fileNames,String directoryPath,String fileOut) throws IOException, NullPointerException {

        String filePath="";

        try {
            if (directoryPath != null) {
                filePath = directoryPath;
                /* add a / if last char is not / */
                if (!directoryPath.endsWith("/")) filePath += '/';
            }

            if(fileOut != null){
                filePath += fileOut;
                if (!fileOut.endsWith(".rci")) filePath += ".rci";
            }
            else
                filePath += "config.rci";

            rciWriter = new BufferedWriter(new FileWriter(filePath));

            for(int i=0;i<fileNames.size();i++){
                InputSource is = prettyPrint(fileNames.get(i));

                DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
                DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
                Document docpretty = dBuilder.parse(is);
                docpretty.getDocumentElement().normalize();

                rciWriter.write("\n#File: " + fileNames.get(i) + "\n");
                parseFile(docpretty);

            }
            XMLtoRCIConverter.log("File generated: " + filePath);

        } catch (Exception e) {
            e.printStackTrace();
            File file = new File(filePath);
            file.delete();
            System.exit(1);
        }
        finally {
            rciWriter.close();
        }
    }

    public static final InputSource prettyPrint(String fileName) throws Exception {
        /*Open the doc*/
        File XmlFile = new File(fileName);
        DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
        DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
        Document doc = dBuilder.parse(XmlFile);
        doc.getDocumentElement().normalize();
        /*Transform it */
        Transformer tf = TransformerFactory.newInstance().newTransformer();
        tf.setOutputProperty(OutputKeys.ENCODING, "UTF-8");
        tf.setOutputProperty(OutputKeys.INDENT, "yes");
        Writer out = new StringWriter();
        tf.transform(new DOMSource(doc), new StreamResult(out));
        /*save it in a buffer */
        InputSource is = new InputSource(new StringReader(out.toString()));
        return is;
	}

    private static void parseFile(Document doc) throws Exception {

        nodeList = doc.getElementsByTagName("*");
        Element e;
        /*len = number of tags of the xml*/
        len = nodeList.getLength();

        for (j=0; j < len; j++){
    	   
            e = (Element)nodeList.item(j);
            /*search <query descriptor> tag  */
            if(e.getTagName().equalsIgnoreCase("query_descriptor")){
                int number_types = (e.getChildNodes().getLength()-1)/2;

                /*for each group type: state,setting */
                for(int t =0;t< number_types;t++){
                    j=j+1;
                    e = (Element)nodeList.item(j);
                    if(e.getTagName().equalsIgnoreCase("descriptor"))
                        readGroupType(e);

                    int number_groups = (e.getChildNodes().getLength()-1)/2;
                    parseGroups(e,number_groups);

                }//no more types
            }//end query_descriptor
        }
    }

    private static void parseGroups(Element e, int number_groups) throws Exception {

        for(int n=0; n<number_groups;n++){
            j=j+1;
            e = (Element)nodeList.item(j);
            if(e.getTagName().equalsIgnoreCase("descriptor")){
                readGroupName(e);
                int number_elements = (e.getChildNodes().getLength()-1)/2;

                if(number_elements == 0)
                    rciWriter.write(String.format("\n# TODO_empty_group %s %s %s\n",groupType,groupName,groupDesc));

                parseElements(e,number_elements);
            }
            else
                throw new Exception("XML Error: Invalid tag at : " + e.getTagName());
        }
	}

	private static void parseElements(Element e,int number_elements) throws Exception {

        for(int m=0; m<number_elements;m++){
            j=j+1;
            e = (Element)nodeList.item(j);
            /*check if the group has <attr tag */

            if(e.getTagName().equalsIgnoreCase("attr")){
                writeGroupAttr(e);
                rciWriter.write(String.format("\n%s %s %s %s\n",groupType,groupName,attrMax,groupDesc));
            }
            else{
                /*If the group doesn't have <attr */
                if(m == 0)
                    rciWriter.write(String.format("\n%s %s %s\n",groupType,groupName,groupDesc));
                if(e.getTagName().equalsIgnoreCase("error_descriptor"))
                    writeGroupError(e);
                else if(e.getTagName().equalsIgnoreCase("element")){
                    writeGroupElement(e);

                    int number_values = (e.getChildNodes().getLength()-1)/2;
                    parseValues(e,number_values);
                }
                else
                    throw new Exception("XML Error: Invalid tag : " + e.getTagName());
            }
        }
	}

	private static void parseValues(Element e,int number_values) throws Exception {

        for(int v=0; v<number_values;v++){
            j=j+1;
            e = (Element)nodeList.item(j);
            if(e.getTagName().equalsIgnoreCase("value")){
                writeElementValue(e);
            }
            else{
                throw new Exception("XML Error: Invalid tag : " + e.getTagName());
            }
        }
	}

    private static void readGroupType(Element e){

        NamedNodeMap nodeMap;
        String attrname;
        String attrval;
        groupType ="";
        Node node;
        nodeMap = e.getAttributes();

        if (nodeMap != null){
            for (int i=0; i<nodeMap.getLength(); i++){
                node = nodeMap.item(i);
                attrname = node.getNodeName();
                attrval = node.getNodeValue();
                if(attrname.equalsIgnoreCase("element") && attrval.equalsIgnoreCase("query_setting")){
	                groupType = "group setting";
	            }
	            else if(attrval.equalsIgnoreCase("query_state")){
	            	groupType = "group state";
	            }
	         }
    	}
    }

    private static void readGroupName(Element e){

        NamedNodeMap nodeMap;
        String attrname;
        String attrval;
        groupName = "";
   	 	groupDesc= "";
        Node node;
    	nodeMap = e.getAttributes();

        if (nodeMap != null){
            for (int i=0; i<nodeMap.getLength(); i++){
                node = nodeMap.item(i);
                attrname = node.getNodeName();
                attrval = node.getNodeValue();

                if(attrname.equalsIgnoreCase("element"))
                	groupName = attrval;
                else if(attrname.equalsIgnoreCase("desc"))
                    groupDesc = "\"" + attrval + "\"";
            }
    	}
    }

    private static void writeGroupAttr(Element e){

        NamedNodeMap nodeMap;
        String attrname;
        String attrval;
        attrMax = "";
        Node node;
    	nodeMap = e.getAttributes();

        if (nodeMap != null){
            for (int i=0; i<nodeMap.getLength(); i++){
	            node = nodeMap.item(i);
	            attrname = node.getNodeName();
	            attrval = node.getNodeValue();

	            if(attrname.equalsIgnoreCase("max"))
	            		attrMax = attrval;
	        }
    	}
    }

    private static void writeGroupElement(Element e) throws IOException{

        NamedNodeMap nodeMap;
        String attrname;
        String attrval;
        String eName = "";
   	 	String eDesc = "";
   	 	String eType = "";
   	 	String eAccess = "";
   	 	String eMax = "";
   	 	String eMin = "";
   	 	String eUnits = "";
        Node node;
    	nodeMap = e.getAttributes();

        if (nodeMap != null){
            for (int i=0; i<nodeMap.getLength(); i++){
                node = nodeMap.item(i);
                attrname = node.getNodeName();
                attrval = node.getNodeValue();

                if(attrname.equalsIgnoreCase("name"))
                    eName = attrval;
                else if(attrname.equalsIgnoreCase("type")){
                	if (attrval.equalsIgnoreCase("long"))
                		attrval = "int32";
                    eType ="type " + attrval;
                }
                else if(attrname.equalsIgnoreCase("access"))
                    eAccess = " access " + attrval;
                else if(attrname.equalsIgnoreCase("max"))
                    eMax = " max " + attrval;
                else if(attrname.equalsIgnoreCase("min"))
                    eMin = " min " + attrval;
                else if(attrname.equalsIgnoreCase("units"))
                    eUnits =" units \"" + attrval + "\"";
                else if(attrname.equalsIgnoreCase("desc"))
                    	eDesc ="\"" + attrval + "\"" ;
            }
            if(eDesc.equalsIgnoreCase("\"\""))
            	eDesc ="\"TODO_empty_desc\"" ;
            rciWriter.write(String.format("    element %s %s %s%s%s%s%s\n",eName,eDesc,eType,eAccess,eMin,eMax,eUnits));
        }
    }
    
    private static void writeElementValue(Element e) throws IOException{

        NamedNodeMap nodeMap;
        String attrname;
        String attrval;
        Node node;
        String value = "";
        String valdesc = "";
        nodeMap = e.getAttributes();

        if (nodeMap != null){
    	    for (int i=0; i<nodeMap.getLength(); i++){
	            node = nodeMap.item(i);
	            attrname = node.getNodeName();
	            attrval = node.getNodeValue();
	            if(attrname.equalsIgnoreCase("value"))
	                value = attrval;
	            else if(attrname.equalsIgnoreCase("desc"))
	                valdesc ="\"" + attrval + "\"" ;
	            else
	                System.out.println(attrname + "tag not identified");

	        }
    	    rciWriter.write(String.format("        value \"%s\" %s\n",value,valdesc));
    	}
    }

    private static void writeGroupError(Element e) throws IOException{

        NamedNodeMap nodeMap;
        String attrname;
        String attrval;
        String errorname = "";
   	 	String errordesc= "";
        Node node;
        nodeMap = e.getAttributes();

        if (nodeMap != null){
            for (int i=0; i<nodeMap.getLength(); i++){
                node = nodeMap.item(i);
                attrname = node.getNodeName();
                attrval = node.getNodeValue();
                if(attrname.equalsIgnoreCase("desc")){
                    errorname = attrval.replace(" ", "_").toLowerCase();
                    errordesc = "\"" + attrval + "\"" ;
                }
            }
            rciWriter.write(String.format("    error %s %s\n",errorname,errordesc));     
        }
    }

}