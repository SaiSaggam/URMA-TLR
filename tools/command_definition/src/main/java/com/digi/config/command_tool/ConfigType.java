package com.digi.config.command_tool;

public enum ConfigType {
	SET_GROUP(0), GET_GROUP(1), ACTION(2);

	private final int index;
	private static int[] count = new int[4];

	private ConfigType(int index) {
		this.index = index;
	}

	public static ConfigType toConfigType(String config, String type)
			throws Exception {
		try {
			String config_type = config + "_" + type;
			return valueOf(config_type.toUpperCase());

		} catch (Exception e) {
			throw new Exception("Invalid Type: " + config + " " + type);
		}
	}

	public int getIndex() {
		return index;
	}

	public static int getCount(ConfigType type) {
		return count[type.getIndex()];
	}

	public static void setCount(ConfigType type) {
		count[type.getIndex()]++;
	}

}
