#!/usr/bin/env python
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
# Cloud Connector test framework - Makefile Generator for Sample projects for ANSI C
#

################ Import Common libraries
import os           ### Some useful functions on pathnames
import argparse     ### Parser for command-line options, arguments and sub-commands


############### Import Custom libraries
import Makefile_ansic


def createSamplesMatrix(path):
    # Get absolute source path
    sourcePath = os.path.abspath(path)

    # Initialize sample projects matrix
    samplesMatrix = []

    # Generate Makefile for all samples in run directory
    for eachLibType in ["run","step"]:
        # Get the base path for each type
        samplesPath = os.path.join(sourcePath,"public",eachLibType,"samples")
        # Search every sample project
        for eachFolder in os.listdir(samplesPath):

            # Create a new entry
            newSample = {}
            newSample["path"] = os.path.join(samplesPath,eachFolder)
            newSample["lib"] = eachLibType
            newSample["name"] = eachFolder
            newSample["type"] = "samples"

            # Add to the matrix
            samplesMatrix.append(newSample)

    return samplesMatrix



if __name__ == "__main__":

    #** Command line arguments
    parserArguments = argparse.ArgumentParser(
        description="Cloud Connector Test Framework",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
        add_help=True)


    parserArguments.add_argument('--path', # Either a name or a list of option strings, e.g. foo or -f, --foo.
                                    action='store',
                                    type=str,
                                    default=None,
                                    help='Base directory path where is the cloud connector repository.',
                                    required=True)


    parserArguments.add_argument('--architecture', action='store', type=str,
                    default='i386', help='Architecture to run test with.',
                    choices=['i386', 'x64', 'ARM'])

    parserArguments.add_argument('--platform', action='store', type=str,
                    default='linux',help='Platform configuration to build Cloud Connector.  Default platform is "linux".',
                    choices=[ 'linux', 'template' ])


    #** Parse input arguments and save it
    inputArguments = parserArguments.parse_args()



    #** Generate the sample projects matrix to create a Makefile
    samplesMatrix = createSamplesMatrix(inputArguments.path)


    for eachItem in samplesMatrix:
        print("Generating Makefile in %s" % (eachItem["path"]) )

        # Get Library type
        libType = eachItem["lib"] # 'run' or 'step'

        # Get Test type
        testType = eachItem["type"] # 'samples', 'dvt', etc.

        # Get Test name
        groupName = eachItem["name"] # 'connect_on_ssl', 'data_point'

        # Get Output path to save the Makefile
        outputPath = eachItem["path"]


        # Setup Makefile
        makeFile = Makefile_ansic.Makefile_ansic(mkTemplatePath=None, libType=libType, sType=testType, sGroup=groupName)
        # Set Default compiler for each architecture
        makeFile.setCompiler(inputArguments.architecture)
        # Set Platform to compile
        makeFile.setPlatform(inputArguments.platform)
        # Read Makefile template file
        makeFile.readTemplateFile()
        # Generate Makefile for this sample project
        makeFile.generateMakefile(outputPath)
        # Write Makefile in output path
        makeFile.writeDestinationFile()

    print("Makefile Generator finished")
