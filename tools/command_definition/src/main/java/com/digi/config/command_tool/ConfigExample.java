package com.digi.config.command_tool;

public class ConfigExample {
	private String text;
	private String description;

	public ConfigExample(String text) {
		this.text = text;
	}

	public ConfigExample(String text, String description) {
		this.text = text;
		this.description = description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	public String getText() {
		return this.text;
	}

	public String getDescription() {
		return this.description;
	}
}
