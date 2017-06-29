package com.digi.config.command_tool;

import java.io.IOException;
import java.util.ArrayList;

public class ConfigRow extends ConfigObject {
	private AccessType access;
	private ArrayList<ConfigExample> examples;

	public ConfigRow(String name, String description) {
		super(name, description);
		this.access = null;
		this.examples = new ArrayList<ConfigExample>();
	}

	public void setAccess(AccessType access) throws IOException {
		if (this.access != null)
			throw new IOException("Duplicate access: " + access.toString());
		this.access = access;
	}

	public void addExample(ConfigExample example) {
		this.examples.add(example);
	}

	public AccessType getAccess() {
		return access;
	}

	public ArrayList<ConfigExample> getExamples() {
		return this.examples;
	}

}
