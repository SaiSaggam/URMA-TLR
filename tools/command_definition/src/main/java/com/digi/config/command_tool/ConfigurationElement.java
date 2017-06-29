package com.digi.config.command_tool;

import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;

public class ConfigurationElement extends ConfigRow {
	private String help_description;
	private ElementType type;
	private String min;
	private String max;
	private String units;
	private String default_value;
	private String max_tag;
	private Boolean optional;
	private Boolean isParameter;
	private LinkedList<ConfigValue> values;
	private ConfigError error;
	private ConfigType config_type;
	private static int countElementHasMaxTag;

	public ConfigurationElement(String name, String description, String help_description, ElementType type) throws IOException {
		super(name, description);
		if (description == null)
			throw new IOException("Missing or bad element description!");
		this.type = type;
		type.set();
		this.values = new LinkedList<ConfigValue>();
		this.optional = false;
		this.isParameter = false;
		this.error = null;
		this.help_description = help_description;
	}

	public String getHelpDescription() {
		return help_description;
	}

	public void setConfigType(ConfigType type) {
		this.config_type = type;
	}

	public void setOptional() {
		optional = true;
	}

	public void setParameter() {
		isParameter = true;
	}

	public void setError(String name, String description) throws IOException {
		if (this.error == null)
			this.error = new ConfigError(name, description);
		else
			throw new IOException("Multiple errors on \"" + name + "\" element not allowe!d");
	}

	public void setMin(String min) throws IOException {
		if (this.min == null) {
			switch (type) {
			case ENCPASSWORD:
			case PASSWORD:
			case STRING:
			case INT32:
			case UINT32:
			case INDEX:
				break;

			case FQDN:
			case INTERFACE:
			case BOOLEAN:
			case ENUM:
			case ENUM_MULTI:
			case ON_OFF:
			case IPADDR:
			case MAC_ADDR:
				throw new IOException(String.format("Unsupported <min> keyword for \"%s\" type on \"%s\" element!",
						type.toString().toLowerCase(), this.getName()));

			case NONE:
				throw new IOException(String.format("<type> keyword is required for <min> keyword on \"%s\" element!",
						this.getName()));

			case COUNTER32:
			case COUNTER64:
				if (config_type != ConfigType.SET_GROUP)
					throw new IOException(String.format("\"%s\" type doesn't support <min> for \"%s\"",
														type.toString().toLowerCase(),
														ConfigType.SET_GROUP.toString().toLowerCase().replace('_', ' ')));

				break;

			}
			this.min = min;
		}
		else
			throw new IOException("Duplicate <min> keyword: " + min);
	}

	public void setMax(String max) throws IOException {
		if (this.max == null) {
			switch (type) {
			case ENCPASSWORD:
			case PASSWORD:
			case STRING:
			case INT32:
			case INDEX:
				this.max = max;
				if (!utilities.isInteger(max)) {
					if (max.matches("[0-9]+")) {
						throw new IOException("Invalid max = " + max);
					}
					max_tag = max;
					countElementHasMaxTag++;
					/* TODO: get the max value from platform file
					 */
					this.max = "1";
				}
				break;

			case INTERFACE:
			case FQDN:
			case BOOLEAN:
			case ENUM:
			case ENUM_MULTI:
			case ON_OFF:
			case IPADDR:
			case MAC_ADDR:
				throw new IOException(String.format("Unsupported <max> keyword for \"%s\" type on \"%s\" element!",
						type.toString().toLowerCase(), this.getName()));
			case NONE:
				throw new IOException(String.format("<type> keyword is required for <max> keyword on \"%s\" element!",
						this.getName()));

			case COUNTER32:
			case COUNTER64:
				if (config_type != ConfigType.SET_GROUP)
					throw new IOException(String.format("\"%s\" type doesnt support <max> for \"%s\"",
														type.toString().toLowerCase(),
														ConfigType.SET_GROUP.toString().toLowerCase().replace('_', ' ')));
				break;
			default:
				this.max = max;
				break;
			}

		} else
			throw new IOException("Duplicate <max> keyword: " + max);
	}

	public void setDefaultValue(String default_value) throws IOException {

		if (this.default_value == null) {

			switch (type) {
			case BOOLEAN:
				if (!default_value.equalsIgnoreCase("true") && !default_value.equalsIgnoreCase("false"))
					throw new IOException("Error in default value of element " + this.getName() + "!");
				break;
			case ON_OFF:
				if (!default_value.equalsIgnoreCase("on") && !default_value.equalsIgnoreCase("off"))
					throw new IOException("Error in default value \"" + default_value + "\" of element: " + this.getName() + "!");
				break;
			case IPADDR:
				if (!utilities.checkIpAddress(default_value))
					throw new IOException("Invalid default IP address \"" + default_value + "\" of element: " + this.getName() + "!");
				break;
			case MAC_ADDR:
				if (!utilities.checkMacAddress(default_value))
					throw new IOException("Invalid default MAC address \"" + default_value + "\" of element: " + this.getName() + "!");
				break;

			case NONE:
				throw new IOException(String.format("No <type> keyword is specified for <default> keyword on \"%s\" element!",
						this.getName()));

			case COUNTER32:
			case COUNTER64:
				if (config_type != ConfigType.SET_GROUP)
					throw new IOException(String.format("\"%s\" type doesnt support <default> for \"%s\"",
														type.toString().toLowerCase(),
														ConfigType.SET_GROUP.toString().toLowerCase().replace('_', ' ')));
			default:
				break;
			}

			this.default_value = default_value;

		} else
			throw new IOException("Duplicate <default> keyword: " + default_value);
	}

	public void addValue(ConfigValue config_value) throws IOException {
		if ((type != ElementType.ENUM) && (type != ElementType.ENUM_MULTI))
			throw new IOException("Invalid or missing <type> on \"" + this.getName() + "\" element");
		for (ConfigValue value : values)
			if (value.getName().equals(config_value.getName()))
				throw new IOException("Duplicate <value>: " + config_value.getName());
		values.add(config_value);
		if (config_value.isNumberSpecified() == false)
		{
			int number_index = values.indexOf(config_value);
			config_value.setNumber(number_index);
			
		}
	}

	public void setUnits(String units) throws IOException {
		if (units == null)
			throw new IOException("Missing or bad units description!");
		else if (this.units != null)
			throw new IOException("Duplicate units: " + units);
		else
			this.units = units;
	}

	public ElementType getType() {
		return type;
	}

	public ConfigError getError() {
		return error;
	}

	public String getMin() {
		return min;
	}

	public String getMax() {
		return max;
	}

	public String getDefaultValue() {
		return default_value;
	}

	public String getUnits() {
		return units;
	}

	public ConfigType getConfigType() {
		return config_type;
	}

	public boolean isOptional() {
		return optional;
	}

	public LinkedList<ConfigValue> getValues() {
		return values;
	}

	public String getMaxTag() {
		return max_tag;
	}

	public boolean isParameter() {
		return isParameter;
	}

	public static int getElementHasMaxTagCount() {
		return countElementHasMaxTag;
	}

	public void validate(ConfigType config_type) throws IllegalArgumentException {
		try {
			String eString = "element";

			switch (config_type)
			{
			case SET_GROUP:
				if ((type.isDefaultRequired()) && (default_value == null))
					throw new Exception("\"" + this.getName() + "\" element is missing DEFAULT value!");
				
				if ((default_value != null) && (type == ElementType.ENCPASSWORD))
					throw new Exception("\"" + type.toString().toLowerCase() + "\" type does not support default value!");
					

				if ((type  == ElementType.COUNTER32) || (type == ElementType.COUNTER64))
					throw new Exception("\"" + type.toString().toLowerCase() + "\" type is not supported for SET group/table!");

				break;

			case GET_GROUP:
				if (error != null)
					CommandTool.log("Ignore <error> specified for \"" + this.getName() + "\" element in GET group/table!");

				if (default_value != null)
					CommandTool.log("Ignore <default> specifed for \"" + this.getName() + "\" element in GET group/table!");
				break;

			case ACTION:
				if (isParameter)
					eString = "parameter";
				if (type == ElementType.NONE) return;
				break;
			}

			if ((config_type == ConfigType.GET_GROUP || config_type == ConfigType.SET_GROUP) && type == ElementType.NONE)
				throw new Exception("\"" + this.getName() + "\" " + eString + " is missing <type>!");

			if (config_type != ConfigType.GET_GROUP) {
				if ((type.isMinRequired()) && (min == null))
					throw new Exception("\"" + this.getName() + "\" " + eString + " is missing MIN value!");

				if ((type.isMaxRequired()) && (max == null))
					throw new Exception("\"" + this.getName() + "\" " + eString + " is missing MAX value!");
			}

			if (type != ElementType.ENUM && type != ElementType.ENUM_MULTI && !values.isEmpty()) {
				throw new Exception("\"" + this.getName() + "\" " + eString + " is specified with <value> for non-enum type: " + type.toString().toLowerCase());
			}

			switch (type) {

			case ENUM:
				if (values.isEmpty()) {
					throw new Exception("Missing <value> on \"" + this.getName() + "\" " + eString + "!");
				}
				/* no break since enum_multi accepts empty string */
			case ENUM_MULTI:

				if ((min != null) || (max != null)) {
					throw new Exception("<min>/<max> are not allowed for ENUM type on \"" + this.getName() +  "\" " + eString + "!");
				}
/*
 *			  TODO: check default and TAG
 */
				if (default_value != null) {
					boolean isValid = false;

					if (type == ElementType.ENUM) {
						for (ConfigValue value: values) {
							if (value.getName().equals(default_value)) {
								isValid = true;
								break;
							}
						}
					}
					else {
						String[] default_array = default_value.split(",");
				
						for (String each_default : default_array) {
							CommandTool.debug_log("default multi_enum : " + each_default);
							isValid = false;
							for (ConfigValue value: values) {
								if (value.getName().equals(each_default)) {
									isValid = true;
									break;
								}
							}
						}
					}
					
					if (!isValid)
						throw new Exception(String.format("Invalid \"%s\" default value on \"%s\" %s!", default_value, this.getName(), eString));
				}
				break;

			case IPADDR:
				if (default_value != null && !utilities.checkIpAddress(default_value))
					throw new Exception(String.format("Invalid \"%s\" default value on \"%s\" %s!", default_value, this.getName(), eString));
				break;

			case BOOLEAN:
			case ON_OFF:
				/* default is already validated @setDefaultValue */
				break;

			case INTERFACE:
				/* TODO: make sure interfaces are specified in platform file */
				break;
			case COUNTER32:
			case COUNTER64:
				if (config_type == ConfigType.ACTION || config_type == ConfigType.SET_GROUP)
					throw new Exception(String.format("Invalid \"%s\" type on \"%s\" %s %s!\n %s is only available for \"%s\"",
							type.toString().toLowerCase(), this.getName(), eString, config_type.toString().toLowerCase(),
							type.toString().toLowerCase(), ConfigType.GET_GROUP.toString().toLowerCase().replace('_', ' ')));
				break;
			case NONE:
				break;

			default:
				long minValue = (type == ElementType.INT32) ? utilities.INT32_MIN_VALUE
						: 0;
				long maxValue = (type == ElementType.INT32) ? utilities.INT32_MAX_VALUE
						: utilities.UINT32_MAX_VALUE;

				switch (type) {
					case MAC_ADDR:
						maxValue = 17;
						break;
					case FQDN:
						maxValue = 255;
						break;
					default:
						break;
				}

				if (min != null) {
					try {
						minValue = Long.parseLong(min);
					} catch (NumberFormatException e) {
						throw new Exception("Invalid <min> value on \"" + this.getName() + "\" " + eString + "!");
					}
				}
				if (max != null) {

					try {
						maxValue = Long.parseLong(max);
					} catch (NumberFormatException e) {
						throw new Exception("Invalid <max> value on \"" + this.getName() + "\" " + eString + "!");
					}

				}

				if (type != ElementType.INT32) {
					if ((minValue < 0) || (maxValue < 0)) {
						throw new Exception(
								"Invalid min or max value for type: "
										+ type.toString().toLowerCase());
					}
					if ((minValue > utilities.UINT32_MAX_VALUE)
					 || (maxValue > utilities.UINT32_MAX_VALUE)) {
						throw new Exception(
								"Overflow min or max value for type: "
										+ type.toString().toLowerCase());
					}
				} else if ((minValue < utilities.INT32_MIN_VALUE)
						|| (minValue > utilities.INT32_MAX_VALUE)
						|| (maxValue < utilities.INT32_MIN_VALUE)
						|| (maxValue > utilities.INT32_MAX_VALUE)) {
					throw new Exception("Overflow min or max value for type: "
							+ type.toString().toLowerCase());
				}

				if (minValue > maxValue) {
					throw new Exception("Error min value > max value!");
				}

				if (default_value != null) {
					switch (type) {
					case FQDN:
						CommandTool.debug_log("default length : " + default_value.length());
					case MAC_ADDR:
					case STRING:
						if ((default_value.length() < minValue) || (default_value.length() > maxValue))
							throw new Exception(String.format("Default %s is out-of-range on \"%s\" %s!",
									type.toString().toLowerCase(), this.getName(), eString));

						break;
					default:

						try {
							long defaultMaxValue = Long.parseLong(default_value);
							if ((defaultMaxValue < minValue) || (defaultMaxValue > maxValue))
									throw new Exception("Default value is out-of-range on \"" + this.getName() + "\" " + eString + "!");
						} catch (NumberFormatException e) {
							throw new Exception("Invalid <default> value on \"" + this.getName() + "\" " + eString + "!");
						}
					}
				}
				break;
			}
		} catch (Exception e) {
			throw new IllegalArgumentException(e);
		}
	}

}
