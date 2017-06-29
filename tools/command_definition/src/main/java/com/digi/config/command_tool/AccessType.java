package com.digi.config.command_tool;

public enum AccessType {
	READWRITE, READONLY, WRITEONLY, READONLY_SUPER_READWRITE, NONE, NO_ACCESS;

	public static AccessType toAccessType(String str) throws Exception {
		try {
			return valueOf(str.toUpperCase());

		} catch (Exception e) {
			throw new Exception("Invalid access Type: " + str);
		}
	}

}
