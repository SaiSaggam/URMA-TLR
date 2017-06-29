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
# Cloud Connector test framework - Test Case Script class
#

#************** Import Common libraries
import logging      ### Functions and classes to implement a event logging system
import re           ### This module provides regular expression matching operations



##  Main Class for Test Case Script File
class TestCaseFile(object):
    "TestCase file class to generate the documentation for Cloud Connector Test Framework"

    #************** Environment
    # Identifier for the script
    identifier = None
    # Source code base
    filePath = None
    # File Content
    fileContent = None
    # Test cases comments
    testCaseCommentList = None


    #************** Logging
    ## Logger class to save system logging
    logger = None





    ## Class Builder
    def __init__(self, filePath):

        #************** Environment
        self.identifier = "TestCaseFile"
        self.filePath = filePath
        self.testCaseCommentList = {}

        #************** Logging
        self.logger = logging.getLogger('cc.testcasefile')


    ## Read file
    def readFile(self):
        handlerFile = open(self.filePath, "r")
        self.fileContent = handlerFile.read()
        handlerFile.close()




    ## Get test cases info from file content
    def getTestCasesInfo(self):

        # Split the file content in lines
        lines = self.fileContent.splitlines()

        # index for the test cases
        listTestCasesIndex = []

        # Check all lines in the file to find all testcases
        index = 0
        for eachLine in lines:
            pattern = "^ +def +test_.+\(self\)\:"
            # Find the line with the test cases header
            if ( re.search(pattern, eachLine, flags=re.IGNORECASE) ):
                #self.logger.info("Find line: '%s'" % eachLine)
                listTestCasesIndex.append(index)

            index+=1


        # Extract the comment for each testcase
        for eachIndex in listTestCasesIndex:
            #*** Extract the test case name
            line = lines[eachIndex]
            testCaseName = re.sub("\(self\)\:",'',re.sub("^ +def +",'',line))

            #*** Save the new test case
            self.testCaseCommentList[testCaseName] = None


            #*** Find the comments for this test case
            startCommentFound = False
            endCommentFound = False
            comments = []

            for eachLine in lines[eachIndex+1:]:
                # Checkpoint to avoid add a comment from another test case
                pattern = "^ +def +test_.+\(self\)\:"
                # Find in the line the test cases header
                if ( re.search(pattern, eachLine, flags=re.IGNORECASE) ):
                    break

                # Checkpoint to avoid add a comment from another function
                pattern = "^ +def +.+\(.+\)\:"
                # Find in the line with a function header
                if ( re.search(pattern, eachLine, flags=re.IGNORECASE) ):
                    break


                # Find the line with the start comment flag
                if ( re.search('"""', eachLine, flags=re.IGNORECASE) ):
                    if ( not startCommentFound ):
                        # First time, we set the start comment flag
                        startCommentFound = True
                    else:
                        # Second time, we set the end comment flag
                        endCommentFound = True

                    # Verify if we have a comment of a single line
                    if ( eachLine.count('"""') > 1 ):
                        # We set the end comment flag
                        endCommentFound = True


                if (startCommentFound):
                    newLine = eachLine.lstrip() # Remove all whitespace at the left
                    newLine = newLine.replace('"""',"") # Remove the comment flag
                    comments.append(newLine)

                if (endCommentFound):
                    break



            #*** Save the comments found for this test case
            if( len(comments) != 0 ):
                self.testCaseCommentList[testCaseName] = comments
            else:
                self.testCaseCommentList[testCaseName] = []


        return self.testCaseCommentList
