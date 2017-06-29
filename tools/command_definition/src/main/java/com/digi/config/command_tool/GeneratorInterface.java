package com.digi.config.command_tool;

import java.util.LinkedList;

/**
 * Interface between the parser and the generator modules
 * 
 */
public interface GeneratorInterface {

	/**
	 * Method used to generate output files.
	 * @param groups Linked list of groups that are used to create the output files
	 * @param actions Linked list of actions
	 * @param outputDirectory Where to place the generated files
	 * @return True	 The output files were successfully created.
	 * @return False	An error was encounter.
	 */
	public boolean GenerateOutputFile(LinkedList<ConfigurationGroup> groups, LinkedList<ConfigurationTag> tags, String outputDirectory);
	public String getName();
	public String getOption();
	public String[] getGeneratorOption();
	public boolean setGeneratorOption(String option);
	
}
