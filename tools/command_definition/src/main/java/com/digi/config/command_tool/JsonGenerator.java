package com.digi.config.command_tool;

import java.io.File;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.util.LinkedList;
import java.util.ArrayList;
import java.util.Arrays;

import org.json.simple.*;

public class JsonGenerator implements GeneratorInterface {

	private String interfaceName = "JSON Generator";
	private final String MODEL_NAME = "lr54";
	int counter[] = {0, 0, 0};

	public String getOption() {
		return "json";
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
			JSONObject obj_parent = new JSONObject();
			ArrayList<JSONObject> obj_table = new ArrayList<JSONObject>(3);
			for (int i = 0; i < 3; i++)
				obj_table.add(new JSONObject());
			for (ConfigurationGroup group: groups) {
				int index = group.getConfigType().getIndex();
				obj_table.get(index).put(group.getName(), constructTable(group));
			}
			obj_parent.put("config", obj_table.get(0));
			obj_parent.put("show", obj_table.get(1));
			obj_parent.put("action", obj_table.get(2));
			obj_parent.put("platforms", constructPlatforms(platforms));
			fileBuffer.write(obj_parent.toJSONString());
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

	private JSONObject constructPlatforms(LinkedList<ConfigurationTag> platforms) {
		JSONObject obj_platforms = new JSONObject();
		for (ConfigurationTag platform: platforms) {
			if (platform.isPlatformType()) {
				JSONObject obj_platform = new JSONObject();
				obj_platform.put("name", platform.getName());
				obj_platform.put("descr", platform.getDescription());
				JSONObject obj_properties = new JSONObject();
				LinkedList<ConfigurationTag> defines = platform.getDefines();
				for (ConfigurationTag define: defines)
					obj_properties.put(define.getName(), define.getDefineValue());
				obj_platform.put("properties", obj_properties);
				obj_platforms.put(platform.getName(), obj_platform);
			}
		}
		return obj_platforms;
	}

	private JSONObject constructTable(ConfigurationGroup group) {
		JSONObject obj_table = new JSONObject();
		int index = group.getConfigType().getIndex();
		obj_table.put("index", new Integer(counter[index]++));
		obj_table.put("descr", group.getDescription());
		// obj_table.put("rci", group.getUiName());
		if (group.getConfigType() == ConfigType.SET_GROUP)
			obj_table.put("uci", group.getUciName());
		obj_table.put("access", group.getAccess().toString());
		obj_table.put("tags", getTags(group));
		int j = 0;
		JSONObject obj_elements = new JSONObject();
		LinkedList<ConfigurationElement> elements = group.getElements();
		for (ConfigurationElement element: elements) {
			JSONObject obj_element = new JSONObject();
			obj_element.put("index", new Integer(j++));
			obj_element.put("descr", element.getDescription());
			obj_element.put("help", element.getHelpDescription());
			if (group.getConfigType() != ConfigType.GET_GROUP) {
				obj_element.put("type", element.getType().toString());
				obj_element.put("default", element.getDefaultValue());
				obj_element.put("min", element.getMin());
				obj_element.put("max", element.getMax());
			}
			obj_element.put("access", element.getAccess().toString());
			obj_element.put("tags", getTags(element));
			obj_element.put("values", getValues(element));
			obj_elements.put(element.getName(), obj_element);
		}
		obj_table.put("elements", obj_elements);
		return obj_table;
	}

	private JSONArray getTags(ConfigObject object) {
		JSONArray json_obj = new JSONArray();
		ArrayList<String> tags = object.getTags();
		for (String tag: tags)
			json_obj.add(tag);
		if (json_obj.size() == 0)
			return null;
		return json_obj;
	}

	private JSONObject getValues(ConfigurationElement element) {
		JSONObject obj_element_values = new JSONObject();
		LinkedList<ConfigValue> element_values = element.getValues();
		for (ConfigValue value: element_values) {
			JSONObject obj_element_value = new JSONObject();
			obj_element_value.put("descr", value.getDescription());
			obj_element_value.put("num", value.getNumber());
			obj_element_value.put("tags", getTags(value));
			obj_element_values.put(value.getName(), obj_element_value);
		}
		if (obj_element_values.size() == 0)
			return null;
		return obj_element_values;
	}

	private String constructFilePath(String outputDirectory) throws Exception {
		boolean isOk = true;
		final String fileName = String.format("%s.json", MODEL_NAME);
		final String directory = "/json_default/";
		if (outputDirectory == null)
			outputDirectory = ".";
		String directoryPath = outputDirectory + directory;
		File path = new File(directoryPath);
		if (!path.exists() || !path.isDirectory())
			if (!path.mkdir())
				throw new Exception("Failed to create directory: " + directoryPath);
		String filePath = directoryPath + fileName;
		return filePath;
	}
}
