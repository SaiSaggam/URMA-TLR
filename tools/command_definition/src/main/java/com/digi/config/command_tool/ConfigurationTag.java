package com.digi.config.command_tool;

import java.io.IOException;
import java.util.LinkedList;

public class ConfigurationTag extends ConfigObject {
	private final boolean platformType;
	private int defineValue;
	private String stringValue;

	private LinkedList<ConfigurationTag> defines = new LinkedList<ConfigurationTag>();

	public ConfigurationTag(String name, String description, boolean platform_type) {
		super(name, description);
		this.platformType = platform_type;
	}

	public void addDefines(ConfigurationTag new_define) throws IOException {
		for (ConfigurationTag define : defines) {
			if (define.getName().equals(new_define.getName())) {
				throw new IOException("Duplicate <value>: "
						+ new_define.getName());
			}
		}
		defines.add(new_define);
	}

	public void setDefineValue(int value) {
		this.defineValue = value;
	}

	public void setStringValue(String value) {
		this.stringValue = value;
	}

	public int getDefineValue() {
		return defineValue;
	}

	public String getStringValue() {
		return stringValue;
	}

	public boolean isPlatformType() {
		return platformType;
	}

	public LinkedList<ConfigurationTag> getDefines() {
		return defines;
	}
	
}
