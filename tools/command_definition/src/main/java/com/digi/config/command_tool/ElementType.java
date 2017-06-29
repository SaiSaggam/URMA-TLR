package com.digi.config.command_tool;

public enum ElementType {
	STRING(0, false, true, false), 
	PASSWORD(1, false, true, false),
	UINT32(2,true, true, true),
	INT32(3, true, true, true),
	ENUM(4, false, false, true),
	ON_OFF(5, false, false, true),
	BOOLEAN(6, false, false, true),
	IPADDR(7, false, false, false),
	FQDN(8, false, false, false),
	MAC_ADDR(9, false, false, false),
	COUNTER32(10, false,false, false),
	COUNTER64(11, false, false, false),
	INDEX(12, false, true, false),
	INTERFACE(13, false, false, false),
	ENCPASSWORD(14, false, true, false),
	ENUM_MULTI(15, false, false, false),
	NONE(16, false, false, false);

	private int index;
	private boolean min_required;
	private boolean max_required;
	private boolean default_required;
	private boolean isSet;

	private ElementType(int index, boolean min_required, boolean max_required,
			boolean default_required) {
		this.index = index;
		this.min_required = min_required;
		this.max_required = max_required;
		this.default_required = default_required;
		this.isSet = false;
	}

	public static ElementType toElementType(String type) throws Exception {
		try {
			ElementType elementType = valueOf(type.toUpperCase());
			/* INDEX is special type. It's only used for 'index' element */
			if (elementType == INDEX)
				throw new Exception("Invalid access Type: " + type);
			else
				return elementType;

		} catch (Exception e) {
			throw new Exception("Invalid access Type: " + type);
		}

	}

	public void set() {
		isSet = true;
	}

	public boolean isSet() {
		return isSet;
	}

	public boolean isMinRequired() {
		return min_required;
	}

	public boolean isMaxRequired() {
		return max_required;
	}

	public boolean isDefaultRequired() {
		return default_required;
	}

	public int getIndex() {
		return index;
	}
}
