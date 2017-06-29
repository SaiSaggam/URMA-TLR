package com.digi.config.command_tool;

import java.io.File;

public class ConfigOption {

	private String option;
	private String argument;
	private String description;
	private String value;
	private boolean isDirectory;

	public ConfigOption(String name, String argument, String description) {

		this.option = name;
		this.argument = argument;
		this.description = description;
	}

	public String getOption() {
		return option;
	}

	public String getArgument() {
		return argument;
	}
	
	public String getDescription() {
		return description;
	}
	
	public String getValue() {
		return value;
	}

	public void setValue(String value) throws Exception {
		if (isDirectory) {
			if (!new File(value).isDirectory()) 
				throw new Exception("Invalid directory path \"" + value + "\"!");
		}
		this.value = value;
	}
	
	public void toDirectoryOption() {
		this.isDirectory = true;
	}

}
