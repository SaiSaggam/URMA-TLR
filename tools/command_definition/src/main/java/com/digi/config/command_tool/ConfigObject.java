package com.digi.config.command_tool;

import java.util.ArrayList;
import java.io.IOException;

public class ConfigObject {
	private String name;
	private String description;
	private ArrayList<String> tags;
	private static int numTagsTotal;

	public ConfigObject(String name, String description) {
		this.name = name;
		this.description = description;
		this.tags = new ArrayList<String>();
	}

	public String getName() {
		return name;
	}

	public String getDescription() {
		return description;
	}

	public ArrayList<String> getTags() {
		return tags;
	}

	public void addTag(String tag) throws IOException {
		if (this.tags.contains(tag))
			throw new IOException(String.format("Duplicate <tag> on \"%s\" element!", this.name));
		tags.add(tag);
		this.numTagsTotal++;
		CommandTool.debug_log(String.format("Tag \"%s\" for \"%s\" element", tag, this.name));
	}

	public static int getNumTagsTotal() {
		return numTagsTotal;
	}

}
