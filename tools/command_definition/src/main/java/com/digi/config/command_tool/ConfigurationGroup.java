package com.digi.config.command_tool;

import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;

/**
 * Class for a configuration group, if more than one row is psepcified then this
 * group is considered a table.
 */
public class ConfigurationGroup extends ConfigRow {
	private String ui_name;
	private String uci_filename;
	private String syntax;
	private LinkedList<ConfigurationElement> elements;
	private int num_rows;
	private int maxInstance = 0;
	private boolean isMaxTagged = false;
	private String maxTag = null;
	private String tag_num_rows;
	private ConfigType config_type;
	private static int countGroupHasMaxTag;
	private boolean mibIndexed = false;
	private boolean noMib = false;
	private String documentDescription;

	public ConfigurationGroup(String name, String ui_name, String description, ConfigType type) throws IOException {
		super(name, description);
		if (ui_name == null
				&& (type == ConfigType.GET_GROUP || type == ConfigType.SET_GROUP)) {
			throw new IOException("Missing RCI name");
		}
		if (description == null)
			throw new IOException("Missing or bad group description");
		this.ui_name = ui_name;
		this.config_type = type;

		ConfigType.setCount(type);

		num_rows = 1;
		elements = new LinkedList<ConfigurationElement>();
	}

	public void setNumRows(int rows) {
		this.num_rows = rows;
	}

	public void setTagNumRows(String tag) throws IOException {
		if (this.tag_num_rows != null)
			throw new IOException("Duplicate MAX <tag> on \"" + this.getName() + "\" "
					+ config_type.toString().toLowerCase().replace('_', ' '));

		this.tag_num_rows = tag;
		this.isMaxTagged = true;
		this.maxTag = tag;
		countGroupHasMaxTag++;
	}

	public void setUciFileName(String filename) {
		this.uci_filename = filename;
	}

	public void setSyntax(String syntax) {
		this.syntax = syntax;
	}

	public int getNumRows() {
		return num_rows;
	}

	public String getTagNumRow() {
		return tag_num_rows;
	}

	public ConfigType getConfigType() {
		return config_type;
	}

	public String getUiName() {
		return ui_name;
	}

	public String getUciName() {
		return uci_filename;
	}

	public String getSyntax() {
		return syntax;
	}

	public LinkedList<ConfigurationElement> getElements() {
		return elements;
	}

	public static int getGroupHasMaxTagCount() {
		return countGroupHasMaxTag;
	}

	public void addElement(ConfigurationElement config_element)
			throws IOException {

		for (ConfigurationElement element : elements) {
			if (element.getName().equals(config_element.getName())) {
				throw new IOException("Duplicate <element>: "
						+ config_element.getName());
			}
		}
		if (config_element.getAccess() == null) {
			if (config_type == ConfigType.SET_GROUP) {
				if ((config_element.getType() == ElementType.PASSWORD) ||
					(config_element.getType() == ElementType.ENCPASSWORD))	
					config_element.setAccess(AccessType.WRITEONLY);
				else
					config_element.setAccess(AccessType.READWRITE);
			}
			else if (config_type == ConfigType.GET_GROUP) {
				config_element.setAccess(AccessType.READONLY);
			}
			else
				config_element.setAccess(AccessType.NONE);
		}
		elements.add(config_element);
	}

	public ConfigurationElement getIndex() {
		ConfigurationElement result = null;
		if ((config_type == ConfigType.SET_GROUP) || (config_type == ConfigType.GET_GROUP)) {
			for (ConfigurationElement element : elements) {
				if (element.getType() == ElementType.INDEX) {
					result = element;
					break;
				}
			}
		}
		return result;
	}

	public void validate() throws Exception {
		switch (config_type) {
			case SET_GROUP:
			case GET_GROUP:
				if (elements.isEmpty())
					throw new Exception("No element specified");
				break;
			case ACTION:
				break;
		}
	}

	public void setMibIndex() {
		this.mibIndexed = true;
	}
	public boolean isMibIndexed() {
		return this.mibIndexed;
	}
	public void setNoMib() {
		this.noMib = true;
	}
	public boolean isNoMib() {
		return this.noMib;
	}

	public void setMaxInstance(int max) {
		this.maxInstance = max;
	}
	public int getMaxInstance() {
		return this.maxInstance;
	}
	public boolean isMaxTagged() {
		return this.isMaxTagged;
	}
	public String getMaxTag() {
		return this.maxTag;
	}
	public String getDocumentDescription() {
		if (this.documentDescription == null)
			return this.getDescription();
		return this.documentDescription;
	}
	public void setDocumentDescription(String description) throws IOException {
		if (this.documentDescription != null)
			throw new IOException("Document description exists.");
		this.documentDescription = description;
	}
}
