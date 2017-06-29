package com.digi.config.command_tool;

public class ConfigValue extends ConfigObject {
	private int number;
	private boolean number_available;
	private boolean exclusive;
	
	public ConfigValue(String name, String description, int number) {
		super(name, description);
		this.number = number;
		this.number_available = true;
		this.exclusive = false;
	}
	public ConfigValue(String name, String description) {
		super(name, description);
		this.number_available = false;
	}
	public boolean isNumberSpecified()
	{
		return number_available;
	}
	public int getNumber() {
		return number;
	}
	
	public void setNumber(int number) {
		this.number = number;
		this.number_available = true;
		
	}
	public void setExclusive(boolean exclusive)
	{
		this.exclusive = exclusive;
	}
	public boolean isExclusive()
	{
		return this.exclusive;
	}
	
}
