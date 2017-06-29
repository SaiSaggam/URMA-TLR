# ***************************************************************************
# Copyright (c) 2013 Digi International Inc.,
# All rights not expressly granted are reserved.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
#
# ***************************************************************************
#
# Cloud Connector test framework - Makefile Generator class for CCAPI
#

################ Import Common libraries
import sys          ### Access to variables or functions maintained by the interpreter
import os           ### Some useful functions on pathnames
import re           ### Provides regular expression matching operations similar to those found in Perl
import logging      ### Functions and classes to implement a event logging system



##  Main Class for Test Case
#############################################################

class Makefile_ccapi(object):
    "Makefile class to work with Cloud Connector Test Framework"

    ################ Environment
    # Identifier for the script
    identifier = None
    # Template filename
    makefileTemplate = None
    # Template file path
    makefileTemplatePath = None
    # Template file content
    makefileTemplateContent = None
    # Destination filename
    makefileDestination = None
    # Destination file path
    makefileDestinationPath = None
    # Destination file content
    makefileDestinationContent = None


    ################ Logging
    ## Logger class to save system logging
    logger = None


    ################ Script
    # Compiler
    defaultCompiler = None
    # Substitutions dictionary
    substitutionsList = None




    ## Class Builder
    ########################################################
    def __init__(self, mkTemplatePath=None ):

        ################ Environment
        self.identifier = "Makefile"
        self.makefileTemplate = "Makefile_ccapi.template"
        self.makefileDestination = "Makefile"

        if(mkTemplatePath is None):
            self.makefileTemplatePath = os.path.abspath(os.path.join(os.path.dirname(__file__), self.makefileTemplate))
        else:
            self.makefileTemplatePath = os.path.abspath(os.path.join(mkTemplatePath, self.makefileTemplate))


        ################ Logging
        self.logger = logging.getLogger('cc.makefile')

        ################ Script
        self.substitutionsList = {}
        self.substitutionsList["${CC_VAR_COMPILER}"] = ""



    ## Read file
    def readTemplateFile(self):
        self.logger.info("Read Makefile teamplate from '%s'" % self.makefileTemplatePath)
        handlerFile = open(self.makefileTemplatePath, "r")
        self.makefileTemplateContent = handlerFile.read()
        handlerFile.close()

    ## Write file
    def writeDestinationFile(self):
        self.logger.info("Write Makefile on '%s'" % self.makefileDestinationPath)
        handlerFile = open(self.makefileDestinationPath, "w")
        handlerFile.write(self.makefileDestinationContent)
        handlerFile.close()


    ## Set compiler
    def setCompiler(self, architecture):
        if(architecture == "ARM"):
            self.defaultCompiler = "arm-linux-gcc"
        else:
            self.defaultCompiler = "gcc"


    ## Generate Makefile for sample
    def generateMakefile(self, outputPath):
        # Save output path
        self.makefileDestinationPath = os.path.abspath(os.path.join(outputPath, self.makefileDestination))
        # Get content from template
        self.makefileDestinationContent = self.makefileTemplateContent


        ################ Script
        # Compiler
        self.substitutionsList["${CC_VAR_COMPILER}"] = self.defaultCompiler


        ################ Modify all rules
        for eachLine in self.substitutionsList:
            self.makefileDestinationContent = self.makefileDestinationContent.replace(eachLine, self.substitutionsList[eachLine])


    def setupFile(self):
        # Write custom File
        self.writeDestinationFile()