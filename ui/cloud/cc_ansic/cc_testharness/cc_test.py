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
# Cloud Connector Test Framework
#

################ Import Common libraries
import sys          ### Access to variables or functions maintained by the interpreter
import os           ### Some useful functions on pathnames
import logging      ### Functions and classes to implement a event logging system
import argparse     ### Parser for command-line options, arguments and sub-commands
import shutil       ### Functions to do high-level operations on files and collections of files
import datetime     ### Supplies classes for manipulating dates and times
import time         ### Provides various time-related functions
import nose         ### nose extends unittest to make testing easier
import requests     ### Makes your integration with web services seamless (HTTP)
import threading    ### Constructs higher-level threading interfaces
import simplejson   ### Simple, fast, extensible JSON (JavaScript Object Notation) encoder/decoder for Python
import xml.etree.ElementTree    ### Flexible container object, designed to store hierarchical data structures in memory.

################ Import Classes path

sys.path.insert(0, os.path.abspath(os.path.join('testharness','host')))
sys.path.insert(0, os.path.abspath(os.path.join('testharness','device')))
sys.path.insert(0, os.path.abspath(os.path.join('testharness','libraries')))

sys.path.insert(0, os.path.abspath(os.path.join('testharness','sares')))
sys.path.insert(0, os.path.abspath(os.path.join('testharness','sares','deque')))
sys.path.insert(0, os.path.abspath(os.path.join('testharness','sares','ExceptionLibrary')))

############### Import Custom libraries
import TestCaseFile
import ColorFormatter   ### Custom Formatter class to allow colors in logging system


##  Main Class
#############################################################
class TestHarness(object):
    "Main class to Execute a Cloud Connector Test Framework"

    ################ Environment
    # Identifier for the script
    identifier = None
    # Script Version
    version = None
    # root path
    rootPath = None
    # Output path
    outputPath = None
    # source code path for all tools
    sourcePath = None
    # folder name for source code
    sourceTestCasesFolderName = None
    # source code path for Cloud Connector Test cases
    sourceTestCasesPath = None
    # Temp path for test cases files
    tempPath = None
    # Testharness test files path
    testFilesPath = None
    # Testharness core path
    corePath = None


    ################ Logging
    # Logger class to save system logging
    logger = None
    # Logging level for system to show in the console output
    logConsoleVerbosity = 1 ## 1=Debug mode
                            ## 2=Info Messages
                            ## 3=Warning Messages
                            ## 4=Error messages
                            ## 5=Critical messages
    # Logging level for system to save in the log file
    logFileVerbosity = 1 ## 1=Debug mode
                         ## 2=Info Messages
                         ## 3=Warning Messages
                         ## 4=Error messages
                         ## 5=Critical messages
    # Log filename for System
    logFilename = None


    ################ Command line arguments
    # Original Command line arguments
    commandLineArguments = None
    # Parse for input arguments
    parserArguments = None
    # Array List for input arguments
    inputArguments = None


    ################ Control and monitoring
    # Force to stop the script
    stopExecutionFlag = None
    # Test Result PASSED/FAILED True/False
    testResultFlag = True


    ################ Execution handlers
    ## Handler Test containing the test script object.
    handlerTest = None
    ## Handler Host containing the host script object.
    handlerHost = None
    ## Handler to connect with the Device Cloud object.
    handlerDeviceCloud = None

    ################ Test Cases List
    testCaseslist = None




    ## Class Builder
    ########################################################
    def __init__(self):

        ################ Environment
        self.identifier = "TestHarness"
        self.version = "0.0.11.1"
        self.outputPath = "results"
        self.rootPath = os.path.abspath(".") #ej. "/home/username/TestFolder"
        self.sourcePath = os.path.join(self.rootPath,"source") #ej. "/home/username/TestFolder/source"
        self.sourceTestCasesFolderName = "cc_testcases"
        self.sourceTestCasesPath = os.path.join(self.sourcePath,self.sourceTestCasesFolderName)
        self.tempPath = os.path.join(self.rootPath,"tmp") #ej. "/home/username/TestFolder/tmp"
        self.corePath = os.path.join(self.rootPath,"testharness") #ej. "/home/username/TestFolder/testharness"
        self.testFilesPath = os.path.join(self.corePath,"test_files") #ej. "/home/username/TestFolder/testharness/test_files"


        ################ Command line arguments
        self.parserArguments = argparse.ArgumentParser(
            description="Cloud Connector Test Framework v%s" % self.version,
            formatter_class=argparse.ArgumentDefaultsHelpFormatter,
            add_help=True)


        self.parserArguments.add_argument('--platform', # Either a name or a list of option strings, e.g. foo or -f, --foo.
                                        action='store',
                                        type=str,
                                        default=None,
                                        help='Platforms list.',
                                        required=True,
                                        choices=['ansic', 'fri', 'k60', 'iar', 'java', 'ccapi'])


        self.parserArguments.add_argument('--username', action='store', type=str,
                            #default='iikdvt', help="Username for Cloud Account to run the test.")
                            default='testframework', help="Username for Cloud Account to run the test.")

        self.parserArguments.add_argument('--password', action='store', type=str,
                            #default='iik1sfun', help="Password for Cloud Account to run the test.")
                            default='26003@Spain', help="Password for Cloud Account to run the test.")

        self.parserArguments.add_argument('--server', action='store', type=str,
                            default='test.etherios.com',help='Cloud Server to connect devices to.')

        self.parserArguments.add_argument('--vendorid', action='store', type=str,
                            default=None,help='Cloud Server Vendor ID in decimal: 16777231 or hexadecimal: 0x01000000')

        self.parserArguments.add_argument('--adminusername', action='store', type=str,
                            default='testbed', help="Username for Admin Cloud Account to run the test that required admin privileges.")

        self.parserArguments.add_argument('--adminpassword', action='store', type=str,
                            default='Sa!test11', help="Password for Admin Cloud Account to run the test that required admin privileges.")

        self.parserArguments.add_argument('--deviceid_basedon', action='store', type=str,
                default='mac', help='Method to generate Device ID',
                choices=['mac', 'imei', 'esn', 'meid', 'provision','static'])

        self.parserArguments.add_argument('--deviceid_generation_method', action='store', type=str,
                default='random', help='Method to generate the Device MAC, IMEI, ESN, MEID. "random": Generates a random value, "fixed": Obtain it from a hardcoded list of values',
                choices=['random', 'fixed'])

        self.parserArguments.add_argument('--devicestatic', action='store', type=str,
                    default=None, help='Optional complete Device ID to use in test cases. format: 00000000-00000000-000000FF-FF000000')

        self.parserArguments.add_argument('--devicemac', action='store', type=str,
                    default=None, help='Optional Device MAC address to create Device ID to use in test cases. format: 000000:000000')

        self.parserArguments.add_argument('--deviceimei', action='store', type=str,
                default=None, help='Optional Device IMEI number to create Device ID to use in test cases. format: AAAAAA-BB-CCCCCC-D')

        self.parserArguments.add_argument('--deviceesn', action='store', type=str,
                default=None, help='Optional Device ESN number to create Device ID to use in test cases. format: MMSSSSSS')

        self.parserArguments.add_argument('--devicemeid', action='store', type=str,
            default=None, help='Optional Device MEID number to create Device ID to use in test cases. format: RRXXXXXXZZZZZZ')

        self.parserArguments.add_argument('--devicetype', action='store', type=str,
                    default=None, help='Optional Device type string to identify a Device.')

        self.parserArguments.add_argument('--devicefirmware_version', action='store', type=str,
                    default=None, help='Optional Device Firmware version string to identify a Device.')

        self.parserArguments.add_argument('--devicephone', action='store', type=str,
                    default=None, help='Optional Device Phone number string to use in SMS.')

        self.parserArguments.add_argument('--cloudphone', action='store', type=str,
                    default=None, help='Optional Cloud Phone number string to use in SMS. ej. 447786201217 it is the test.etherios.com phone number')

        self.parserArguments.add_argument('--cloudservice_id', action='store', type=str,
                    default=None, help='Optional Cloud Service ID used as shared-code in SMS.')

        self.parserArguments.add_argument('--descriptor', action='store', type=str,
                            default='', help='A unique descriptor to describe the test.')

        self.parserArguments.add_argument('--architecture', action='store', type=str,
                            default='i386', help='Architecture to run test with.',
                            choices=['i386', 'x64', 'ARM'])

        self.parserArguments.add_argument('--test_config_file', action='store', type=str,
                            default=None, help='Optional config file with the custom tests list.')

        self.parserArguments.add_argument('--test_type', action='store', type=str, nargs="+",
                            default=None, help='Optional test type to execute. Default executes all.')

        self.parserArguments.add_argument('--test_group', action='store', type=str,
                            default=None, help='Optional test group to execute. Default executes all.')

        self.parserArguments.add_argument('--exclude_test_group', action='store', type=str,
                            default=[],
                            nargs='+', # 'N'(an integer), N arguments from the command line will be saved together into a list
                                    # '?', one argument will be consumed from the command line,
                                    #      If no command-line argument is present, the value from default will be produced.
                                    # '*', all command line arguments will be saved into a list
                                    # '+', the same than '*' but an error will be generated if there wasn't at least one
                                    #      command-line argument present.
                            help='Optional test group to exclude from the execution.')

        self.parserArguments.add_argument('--test_name', action='store', type=str,
                            default=None, help='Optional test name to execute. Default executes all.')

        self.parserArguments.add_argument('--exclude_test_name', action='store', type=str,
                            default=[], nargs='+', help='Optional test name to exclude from the execution.')

        self.parserArguments.add_argument('--test_case', action='store', type=str,
                            default=None, help='Optional test case(s) to execute. Default executes all. format: class names that match with a regular expression')

        self.parserArguments.add_argument('--tty', action='store_true',dest='tty', default=False,
            help='Whether or not to run in a separate TTY session (only useful for Jenkins execution).')

        self.parserArguments.add_argument('--gcov', action='store_true', dest='gcov', default=False,
            help='Whether or not to run gcov Code coverage.')

        self.parserArguments.add_argument('--enable-admin-account', action='store_true', dest='enable_admin_account', default=False,
            help='Whether or not to provide a Device Cloud handler with admin privileges for test cases.')

        self.parserArguments.add_argument('--skip-build', action='store_true', dest='skip_build', default=False,
            help='Whether or not to run step to build test project or skip the process.')

        self.parserArguments.add_argument('--skip-flash', action='store_true', dest='skip_flash', default=False,
            help='Whether or not to run step to flash test project or skip the process.')

        self.parserArguments.add_argument('--skip-register', action='store_true', dest='skip_register', default=False,
            help='Whether or not to run step to register device on Cloud server or skip the process.')

        self.parserArguments.add_argument('--skip-run', action='store_true', dest='skip_run', default=False,
            help='Whether or not to run step to run Cloud Connector on Device or skip the process.')

        self.parserArguments.add_argument('--skip-save-source', action='store_true', dest='skip_save_source', default=False,
            help='Whether or not to run step to generate a tar.gz with source code for each test case failed.')

        self.parserArguments.add_argument('--only-build-mode', action='store_true', dest='only_build_mode', default=False,
            help='Enable a special mode to test only the build process that also includes: --skip-flash --skip-register --skip-run')

        self.parserArguments.add_argument('--only-doc-mode', action='store_true', dest='only_doc_mode', default=False,
            help='Enable a special mode to generate only the documentation process that also includes: --documentation')

        self.parserArguments.add_argument('--documentation', action='store_true', dest='documentation', default=False,
            help='Enable to generate the documentation process')

        self.parserArguments.add_argument('--stop-first-failure', action='store_true', dest='stop_first_failure', default=False,
            help='Enable a special mode to stop the test execution after the first test failure')


        #*************** Device configuration options

        # Connectivity settings:
        self.parserArguments.add_argument('--deviceConnectionMethod',
                                          action='store',
                                          type=str,
                                          default=None,
                                          choices=['serial', 'telnet', 'ssh', 'telnet_with_pexpect'],
                                          help='Console type to connect with device',
                                          required=False)

        # Serial parameters
        self.parserArguments.add_argument('--deviceSerialConsolePort',
                                          action='store',
                                          type=str,
                                          default=None,
                                          help='Console path to connect with device')

        self.parserArguments.add_argument('--deviceSerialConsoleBaudrate',
                                          action='store',
                                          type=int,
                                          default=None,
                                          help='Console baudrate to connect with device')

        # Network parameters
        self.parserArguments.add_argument('--deviceIP',
                                          action='store',
                                          type=str,
                                          default=None,
                                          help='IP to connect with device')

        self.parserArguments.add_argument('--deviceUsername',
                                          action='store',
                                          type=str,
                                          default=None,
                                          required=False,
                                          help='Username to use in Device handler')

        self.parserArguments.add_argument('--devicePassword',
                                          action='store',
                                          type=str,
                                          default=None,
                                          required=False,
                                          help='Password to use in Device handler')



        ################ Command line arguments For Cloud Connector for ANSI C
        self.parserArguments.add_argument('--ansic_type', action='store', type=str,
                    default='run',help='Configuration to run against "run" or "step".  Default executes "run".',
                    choices=['run', 'step'])

        self.parserArguments.add_argument('--ansic_configuration', action='store', type=str,
                    default='default',help='Configuration to build Cloud Connector.  Default compile in "default" mode.',
                    choices=[ 'default', 'nodebug', 'debug', 'library', 'no_malloc' ])

        self.parserArguments.add_argument('--ansic_platform', action='store', type=str,
                    default='linux',help='Platform configuration to build Cloud Connector.  Default platform is "linux".',
                    choices=[ 'linux', 'template' ])

        self.parserArguments.add_argument('--ansic_compile_time_data', action='store_true', default=False,
            help='Configuration to build Cloud Connector with data passing in compilation time.  Default is False.')

        self.parserArguments.add_argument('--ansic_enable_compression', action='store_true', default=False,
            help='Configuration to build Cloud Connector with data compression.  Default is False.')

        self.parserArguments.add_argument('--ansic_enable_ssl', action='store_true', default=False,
            help='Configuration to build Cloud Connector with TCP over SSL support.  Default is False.')

        self.parserArguments.add_argument('--ansic_git_commit', action='store', type=str,
                            default=None, help='Move git repository to a specific commit. Default use the lastest code from repository.')

        self.parserArguments.add_argument('--ansic_private_git_commit', action='store', type=str,
                            default=None, help='Use private folder from a specific commit.')

        self.parserArguments.add_argument('--ansic_sms_proxy_ip', action='store', type=str,
                            default=None, help='IP for proxy server to send SMS over the network. Default is None')

        ################ Logging
        self.logFilename = "execution.log"


        ################ Control and monitoring
        self.stopExecutionFlag = False
        self.testResultFlag = False




    def run(self, commandLineArguments):
        # Save Command Line Arguments
        self.commandLineArguments = commandLineArguments

        ################ Parse command line arguments for Execution Script
        if(self.checkInputArguments()):

            ################ Create Result folder to save all test outputs
            if(self.continueExecution()):
                status = self.createResultFolder()
                if(status[0] != True):
                    self.stopExecution(status[1])


            ################ Create system logging
            if(self.continueExecution()):
                self.startLoggingSystem()
                self.logger.info("Cloud Connector Test Framework v%s logging system started successfully" % (self.version))


            ################ Create Temp folder to store all temp file for test cases
            if(self.continueExecution()):
                status = self.createTempFolder()
                if(status[0] != True):
                    self.stopExecution(status[1])


            ################ Create a Device Cloud handler
            if(self.continueExecution()):
                # Get Handler for the Device Cloud
                result,handler = self.getHandlerDeviceCloud()
                if(result):
                    self.handlerDeviceCloud = handler
                else:
                    self.stopExecution(handler)


            ################ Verify Credentials in Cloud Account
            if(self.continueExecution()):
                self.verifyCloudCredentials()


            ################ Create Source folder to store all test cases
            if(self.continueExecution()):
                status = self.createSourceFolder()
                if(status[0] != True):
                    self.stopExecution(status[1])


            ################ Create a Host handler
            if(self.continueExecution()):
                # Get Handler from the Host
                status = self.getHandlerHost()
                if(status[0]):
                    self.handlerHost = status[1]
                else:
                    self.stopExecution(status[1])

            ################ Setup Host to start tests
            if(self.continueExecution()):
                # Provide the instance for DeviceCloud
                self.handlerHost.handlerDeviceCloud = self.handlerDeviceCloud

                # Initialize the environment
                status = self.handlerHost.createEnvironment()
                if(status[0]):
                    self.logger.info(status[1])
                else:
                    self.logger.error(status[1])
                    self.stopExecution(status[1])


            ################ Create tests matrix
            if(self.continueExecution()):
                status = self.createTestCasesMatrix()

                if(status[0]):
                    self.logger.info(status[1])
                else:
                    self.logger.error(status[1])
                    self.stopExecution(status[1])

            ################ Generate Documentation for tests cases
            if( self.continueExecution() ):
                if ( self.inputArguments.documentation or self.inputArguments.only_doc_mode):
                    self.parseDocumentationFromTestCases()


            ################ Launch every test case
            if(self.continueExecution() and not self.inputArguments.only_doc_mode):
                for eachTestType in self.testCaseslist.keys():
                    # Check STOP flag
                    if ( not self.continueExecution() ):
                        self.logger.info("Stop TEST TYPE execution due to a stop flag")
                        # Stop the loop
                        break

                    self.logger.info("\n\n")
                    self.logger.info("*********** Starting TEST TYPE '%s' ***********" % eachTestType)

                    ### Initialize result flag for Test Type
                    typeResult = True

                    ### Get Start time to measure elapsed time in the execution of this TEST_TYPE
                    typeStartTime = time.time()

                    for eachTestGroup in self.testCaseslist[eachTestType]["groups"].keys():
                        # Check STOP flag
                        if ( not self.continueExecution() ):
                            self.logger.info("Stop TEST GROUP execution due to a stop flag")
                            # Stop the loop
                            break

                        self.logger.info("*********** Starting TEST GROUP '%s' ***********" % eachTestGroup)

                        ### Initialize stop flag for Test Group
                        stopTestGroup = False

                        ### Initialize result flag for Test Group
                        groupResult = True

                        ### Get Start time to measure elapsed time in the execution of this TEST_GROUP
                        groupStartTime = time.time()

                        ### Reset Device environment
                        result,details = self.handlerHost.resetDevice()
                        if(not result):
                            self.logger.error("Couldn't reset the Device: '%s'" % details)
                            stopTestGroup = True

                        ### Create a test result folder
                        result,details = self.createTestGroupResultFolder("%s.%s" % (eachTestType,eachTestGroup))
                        if(not result):
                            self.logger.error("Couldn't create a result folder: '%s'" % details)
                            stopTestGroup = True
                        else:
                            ### Save output folder and setup to host
                            testGroupResultFolder = details
                            self.handlerHost.setTestResultPath(testGroupResultFolder)

                        ### Set group path
                        sourceGroupPath = os.path.join(self.sourceTestCasesPath,eachTestType,eachTestGroup)

                        if(stopTestGroup!=True and self.inputArguments.skip_build is False):
                            ### Create source project
                            result,details = self.handlerHost.createSourceProject(sourceGroupPath,eachTestType,eachTestGroup)
                            if(not result):
                                self.logger.error("Create source project process has failed for group '%s/%s': %s" % (eachTestType,eachTestGroup,details))
                                stopTestGroup = True
                                # If generating process is failed all group result is failed
                                groupResult = False
                                # Save log for the build
                                logBuildProcess = details

                            ### Build Source project
                            result,details = self.handlerHost.buildSourceProject()
                            logBuildProcess = ""
                            if(not result):
                                self.logger.error("Build process has failed for group '%s/%s'" % (eachTestType,eachTestGroup))
                                stopTestGroup = True
                                # If build process is failed all group result is failed
                                groupResult = False
                                # Save log for the build
                                logBuildProcess = details

                            ### Generate a test result XML file for build process
                            self.generateTestBuildFileXMLResultContent(testGroupResultFolder,eachTestType,eachTestGroup, groupResult,logBuildProcess)


                        ### Skip test process for ONLY_BUILD_MODE or when build process is failed
                        if(self.inputArguments.only_build_mode is False and stopTestGroup!=True):

                            if(stopTestGroup!=True and self.inputArguments.skip_flash is False):
                                ### Update Device with project
                                self.handlerHost.flashSourceProject()

                            if(stopTestGroup!=True):

                                ### Register Device in Cloud server only if user no skip the process
                                if(self.inputArguments.skip_register or self.inputArguments.deviceid_basedon == "provision"):
                                    self.logger.warning("Device registration process was skipped")
                                else:

                                    result,details = self.registerDeviceOnCloudServer()
                                    if ( result ):
                                        self.logger.info("Device was registered successfully on server")
                                    else:
                                        self.logger.error("Couldn't register the Device on server")

                                        ### Generate a test result XML file for registration process
                                        self.generateRegisterDeviceXMLResultContent(testGroupResultFolder,eachTestType,eachTestGroup,result,details)


                                ### Launch Cloud Connector only if user no skip the process
                                if(self.inputArguments.skip_run is False):
                                    # Start Cloud Connector
                                    self.handlerHost.launchCloudConnectorOnDevice()


                                ### Start test cases
                                for eachTestName in self.testCaseslist[eachTestType]["groups"][eachTestGroup]["tests"].keys():
                                    # Check STOP flag
                                    if ( not self.continueExecution() ):
                                        self.logger.info("Stop TEST NAME execution due to a stop flag")
                                        # Stop the loop
                                        break

                                    self.logger.info("*********** Starting TEST NAME: '/%s/%s/%s' ***********" % (eachTestType,eachTestGroup,eachTestName))

                                    # Get Start time to measure elapsed time in the execution of the TEST_NAME
                                    testStartTime = time.time()
                                    #formatedStartTime = datetime.datetime.fromtimestamp(testStartTime).strftime('%Y-%m-%d %H:%M:%S')


                                    self.logger.info("Launch test case '/%s/%s/%s'" % (eachTestType,eachTestGroup,eachTestName))

                                    ### Check if TEST_NAME is limited by TEST_CASE level
                                    TestCaseList = None
                                    if(self.testCaseslist[eachTestType]["groups"][eachTestGroup]["tests"][eachTestName]["cases"] != None):
                                        # For the test cases list, nose accepts a regular expression
                                        TestCaseList= ""

                                        for eachTestCase in self.testCaseslist[eachTestType]["groups"][eachTestGroup]["tests"][eachTestName]["cases"].keys():
                                            if(TestCaseList != ""):
                                                # For the first item we skip the command
                                                TestCaseList += "|"

                                            # We add each test case to the regular expression
                                            TestCaseList += "%s" % eachTestCase


                                    ### Launch test case
                                    try:
                                        testResult = self.launchTestCase(testGroupResultFolder,sourceGroupPath,eachTestType,eachTestGroup,eachTestName,TestCaseList)
                                    except KeyboardInterrupt,e:
                                        testResult = False
                                        self.logger.exception(e)
                                        self.stopExecution("Ctrl+C event caught executing the test case")

                                    # Get elapsed time after the execution of the TEST_NAME
                                    testEndTime = time.time()
                                    #formatedEndTime = datetime.datetime.fromtimestamp(testEndTime).strftime('%Y-%m-%d %H:%M:%S')
                                    testElapsedTime =  testEndTime - testStartTime
                                    self.logger.info("Test case '/%s/%s/%s' finished, Elapsed time %s (HH:MM:SS)" % (eachTestType,eachTestGroup,eachTestName,self.secondsToStr(testEndTime-testStartTime)))


                                    ### Verify Test Result
                                    if(testResult is False):
                                        # Set Group as Failed
                                        groupResult = False

                                        # If the flag is enabled we skip all the test cases after the first failure
                                        if ( self.inputArguments.stop_first_failure ):
                                            self.stopExecution("Obtained a test failure and the STOP_FIRST_FAILURE flag is enabled")

                                    ### Save Test result on matrix
                                    self.testCaseslist[eachTestType]["groups"][eachTestGroup]["tests"][eachTestName]["options"]["result"] = testResult
                                    self.testCaseslist[eachTestType]["groups"][eachTestGroup]["tests"][eachTestName]["options"]["elapsedTime"] = testElapsedTime

                                if(self.inputArguments.skip_run is False):
                                    ### Stop Cloud Connector
                                    self.handlerHost.stopCloudConnectorOnDevice()

                                ### Clean temporal path
                                self.cleanTempFolder()

                                #else:
                                    #self.logger.error("Couldn't register device on server")
                            else:
                                self.logger.error("Skip execution process on Cloud server, due to a stop flag")
                        else:

                            if(stopTestGroup):
                                ## Build process is failed
                                self.logger.error("Skip test cases process, because build process was failed")
                            else:
                                ## ONLY_BUILD_MODE
                                self.logger.info("Skip test cases process, because ONLY_BUILD_MODE is enabled")



                        if(self.inputArguments.skip_register is False):
                            ### Remove Device from Cloud server
                            self.removeDeviceFromCloudServer()

                        ################ Final actions for each Test Group
                        ### Get elapsed time after the execution of the TEST_GROUP
                        groupEndTime = time.time()
                        groupElapsedTime =  groupEndTime - groupStartTime

                        ### Save Test group results on matrix
                        self.testCaseslist[eachTestType]["groups"][eachTestGroup]["options"]["result"] = groupResult
                        self.testCaseslist[eachTestType]["groups"][eachTestGroup]["options"]["elapsedTime"] = groupElapsedTime

                        ### Verify Test Group result
                        if(groupResult is False):
                            # Set Type as Failed
                            typeResult = False

                        ### Generate a compress file for each test group failed
                        if(self.inputArguments.skip_save_source is False):
                            if(groupResult is False):
                                # Get from Host the source code and all files needed to reproduce this issue
                                self.handlerHost.saveSourceProject(eachTestType,eachTestGroup)

                        ### TODO: Should we delete/destroy Device object

                        ### Remove source project
                        self.handlerHost.removeSourceProject()


                    ### Get elapsed time after the execution of the TEST_TYPE
                    typeEndTime = time.time()
                    typeElapsedTime =  typeEndTime - typeStartTime

                    ### Save Test Type results on matrix
                    self.testCaseslist[eachTestType]["options"]["result"] = typeResult
                    self.testCaseslist[eachTestType]["options"]["elapsedTime"] = typeElapsedTime


            #** Close and Destroy Objects and Handlers
            self.cleanEnvironment()

        ################ Display matrix
        if ( self.inputArguments.documentation or self.inputArguments.only_doc_mode ):
            # Save all reports on the same folder
            testReportFolderPath = os.path.join(self.outputPath,"report")
            # Generate XML and HTML test report
            self.generateDocumentation(testReportFolderPath)
            # Generate a text-plain file with the test report
            outputFilename = "execution_report.txt"
            if ( self.inputArguments.descriptor != "" ):
                outputFilename = "execution_report_%s.txt" % self.inputArguments.descriptor.strip().replace(" ","_")
            resultFilePath = os.path.join(testReportFolderPath,outputFilename)
            # Show by output the test matrix
            self.showTestCasesList(filePath=resultFilePath)
        else:
            self.showTestCasesList()

        self.logger.info("Cloud Connector Test Framework finished")



    def checkInputArguments(self):
        #** Parse input arguments and save it
        self.inputArguments = self.parserArguments.parse_args(self.commandLineArguments)

        #** Unify all command line arguments
        # DeviceMAC
        if(self.inputArguments.devicemac is not None):
            self.inputArguments.devicemac = self.inputArguments.devicemac.upper()

        # DeviceMEI
        if(self.inputArguments.deviceimei is not None):
            self.inputArguments.deviceimei = self.inputArguments.deviceimei.upper()

        # DeviceESN
        if(self.inputArguments.deviceesn is not None):
            self.inputArguments.deviceesn = self.inputArguments.deviceesn.upper()

        # DeviceMEID
        if(self.inputArguments.devicemeid is not None):
            self.inputArguments.devicemeid = self.inputArguments.devicemeid.upper()

        return True



    def stopExecution(self, message = "Unknown"):
        # Activate stop flag
        self.stopExecutionFlag = True
        # Show Stop reason in logging system
        self.logger.info("Test execution stopped due to: %s" % message)



    def continueExecution(self):
        # Check stop flag
        if(self.stopExecutionFlag != True):
            return True
        else:
            return False



    def startLoggingSystem(self):
        # create logger
        self.logger = logging.getLogger('cc')
        self.logger.setLevel(logging.DEBUG)

        # create file handler which logs even debug messages
        fh = logging.FileHandler(os.path.join(self.outputPath,self.logFilename))
        fh.setLevel(self.logFileVerbosity)

        # create console handler and set level to debug
        ch = logging.StreamHandler()
        ch.setLevel(self.logConsoleVerbosity)

        # create formatters
        #** Standard formatter
        formatter = logging.Formatter('%(asctime)s - %(threadName)12s - %(name)-16s - %(levelname)-8s - %(message)s')
        #** Formatter wit color
        colorFormatter = ColorFormatter.ColorFormatter('%(asctime)s - %(threadName)12s - %(name)-16s - %(levelname)-8s - %(message)s')

        # add formatter to console handler and file handler
        ch.setFormatter(colorFormatter)
        fh.setFormatter(formatter)

        # add handlers to logger
        self.logger.addHandler(ch)
        self.logger.addHandler(fh)



    def createResultFolder(self):
        # Create result directory
        if not os.path.exists(self.outputPath):
            os.makedirs(self.outputPath)

        # Create a timestamp folder for results
        timestamp = datetime.datetime.now()
        timestampFolder = "result_%s-%s-%s_%s-%s-%s" % (timestamp.year,
                                                    timestamp.month,
                                                    timestamp.day,
                                                    timestamp.hour,
                                                    timestamp.minute,
                                                    timestamp.second)

        self.outputPath = os.path.join(self.outputPath,timestampFolder)

        # Create Test dir
        if not os.path.exists(self.outputPath):
            os.makedirs(self.outputPath)
            return (True, "Path created for Results")
        else:
            return (False, "Result path was not created")



    def createTempFolder(self):
        # Create temporal directory
        if not os.path.exists(self.tempPath):
            try:
                os.makedirs(self.tempPath)
                _msg = "Path created for temporal file in '%s'" % self.sourcePath
                self.logger.info(_msg)
                return (True, _msg)
            except OSError,e:
                self.logger.exception('Exception caught: %s' % type(e))
                _msg = "Exception [OSError]: %s" % (str(e))
                return (False, _msg)
        else:
            _msg = "Temporal directory was not created in '%s', due to exists" % self.tempPath
            self.logger.warning(_msg)
            # Skip creation, but clean it
            self.cleanTempFolder()
            return (True, _msg)


    def cleanTempFolder(self):
        if os.path.exists(self.tempPath):
            # Security check to avoid remove all disk from "/"
            if self.tempPath != "/" and self.tempPath != "":
                self.logger.info("Clean temp folder for test cases files")
                for root, dirs, files in os.walk(self.tempPath, topdown=False):
                    for name in files:
                        os.remove(os.path.join(root, name))
                        self.logger.debug("Removing temp file from: '%s'" % os.path.join(root, name))
                    for name in dirs:
                        try:
                            shutil.rmtree(os.path.join(root, name))
                            self.logger.debug("Removing temp directory from: '%s'" % os.path.join(root, name))
                        except OSError,e:
                            self.logger.exception('Exception caught: %s' % type(e))
                            _msg = "Exception [OSError]: %s" % (str(e))



    def createTestGroupResultFolder(self,testgroupName):
        # Create a test group folder
        newFolder = os.path.abspath(os.path.join(self.outputPath,testgroupName))

        # Create directory
        if not os.path.exists(newFolder):
            os.makedirs(newFolder)
            return (True, newFolder)
        else:
            return (False, "Result path was not created")



    def verifyCloudCredentials(self):

        self.logger.info("Trying to authenticated with server: '%s'" % self.inputArguments.server)

        # Check Credentials
        if ( self.handlerDeviceCloud.connect() ):
            self.logger.info("Successfully authenticated with server: '%s'" % self.inputArguments.server)
        else:
            raise Exception("Invalid Credentials on Cloud Account with server: '%s'" % self.inputArguments.server)

        # Save Vendor ID
        if(self.inputArguments.vendorid is None):
            self.inputArguments.vendorid = self.handlerDeviceCloud.getDeviceCloudVendorID()
            self.logger.info("Retrieve Vendor ID '%s' from user account" % self.inputArguments.vendorid)
        else:
            # Verify is a decimal number
            try:
                int(self.inputArguments.vendorid)
            except ValueError, e:
                self.logger.error("Vendor ID '%s' is not a valid int with base 10 (dec)" % self.inputArguments.vendorid)
                try:
                    # Verify if is a hexadecimal number
                    decimalVendorID = int(self.inputArguments.vendorid,16)
                    self.inputArguments.vendorid = str(decimalVendorID)
                except ValueError, e:
                    self.logger.error("Vendor ID '%s' is not a valid int with base 16 (hex)" % self.inputArguments.vendorid)
                    raise Exception("Invalid Vendor ID: '%s'" % self.inputArguments.vendorid)



    def verifyDeviceIsConnected(self):
        # Get Device ID
        deviceID = self.handlerHost.getDeviceID()
        self.logger.info("Verify if device with ID '%s' is connected to Cloud server '%s'" % (deviceID,self.inputArguments.server) )

        # Implement a retries system
        deviceStatus = False
        maxRetries = 3
        countRetries = 0
        while(deviceStatus!=True and countRetries<maxRetries):
            countRetries+=1

            if ( self.handlerDeviceCloud.isDeviceConnected(deviceID) ):
                # Device connected
                self.logger.info("Device '%s' is connected in server '%s'" % (deviceID,self.inputArguments.server))
                deviceStatus = True
            else:
                # Device disconnected
                self.logger.warning("Device '%s' is NOT connected in server '%s' (try %s)" % (deviceID,self.inputArguments.server,countRetries))
                deviceStatus = False

            if(deviceStatus!=True):
                self.logger.info("Waiting 10 additional seconds to device connection with server '%s'..." % (self.inputArguments.server))
                # Wait to verify that device is connected
                time.sleep(10)

        return deviceStatus



    def registerDeviceOnCloudServer(self):
        # Get Device ID
        deviceID = self.handlerHost.getDeviceID()
        if(deviceID != None):
            self.logger.info("Trying to register device with ID '%s' on Cloud server '%s'" % (deviceID,self.inputArguments.server) )

            # Send query to register a new device
            result,response = self.handlerDeviceCloud.createDevice( deviceID,
                                                                    dpUdpSmEnabled='true') # UDP enabled

            if (result):
                self.logger.info("Device was successfully registered on Cloud server")
            else:
                self.logger.error("Error registering device on server:\n%s" % response.content)
                return False,response.content


            # If Device phone was passed as argument, we need to associate or update it with the registered device
            if self.inputArguments.devicephone != None:
                self.logger.info("Trying to register devicephone '%s' to device with ID '%s' on Cloud server '%s'" % (self.inputArguments.devicephone, deviceID,self.inputArguments.server) )

                result,response = self.handlerDeviceCloud.associateDevicePhoneNumber(deviceID, self.inputArguments.devicephone)

                if (result):
                    self.logger.info("Configure device phone number in the cloud through web services Success")
                else:
                    self.logger.info("Attempt to Configure device phone number in the cloud through web services Failed")
                    self.logger.debug("Response from Cloud:\n%s" % response.content)
                    return False,response.content

            return True,response.content

        else:
            _msg = "Device returns a Device ID 'None'"
            self.logger.error(_msg)
            return False,_msg



    def removeDeviceFromCloudServer(self):
        # Get Device ID
        deviceID = self.handlerHost.getDeviceID()
        if(deviceID != None):

            maxRetries = 3
            counter = 0
            while(counter < maxRetries):
                counter +=1

                try:
                    self.logger.info("Trying to remove device with ID '%s' from Cloud server '%s'" % (deviceID,self.inputArguments.server) )

                    # Send request to delete a device
                    result,response = self.handlerDeviceCloud.removeDevice(deviceID)

                    if (result):
                        self.logger.info("Device '%s' was successfully removed from Cloud server" % (deviceID) )
                    else:
                        self.logger.error("Error removing Device '%s' from server:\n%s" % (deviceID,response.content) )


                    # Get Device Type
                    deviceType = self.handlerHost.getDeviceType()
                    if(deviceType != None):

                        # Send request to delete the cache for this device
                        result,response = self.handlerDeviceCloud.cleanDeviceCache(deviceType)

                        if ( result ):
                            self.logger.info("Cache for Device '%s' was successfully cleaned from Cloud server" % (deviceID) )
                            return True
                        else:
                            self.logger.error("Error removing Device '%s' cache from server:\n%s" % (deviceID,response.content) )
                            return False

                    else:
                        self.logger.error("Device returns a Device Type 'None'")
                        return False
                except (requests.exceptions.Timeout,requests.exceptions.ConnectionError),e:
                    self.logger.exception("Exception caught: %s" % e)

            # After the retry system
            self.logger.error("Error removing Device '%s' from Cloud server '%s', many retries (%s) " % (deviceID,self.inputArguments.server,counter) )
            return False

        else:
            self.logger.error("Device returns a Device ID 'None'")
            return False



    def launchTestCase(self,resultFolderTestGroup,sourceGroupPath,eachTestType,eachTestGroup,eachTestName,TestCaseList):
        testPath = os.path.join(sourceGroupPath,"testcases",eachTestName) #ej. "/home/username/TestFolder/source/cc_testcases/dvt/reboot/testcases/testname"
        self.logger.info("Launch test from '%s'" % (testPath) )
        # Argument list to call nose with.  Generate a nosetest xml file in
        # current directory
        noseBinaryPath = os.path.join(self.rootPath,"python","bin","nosetests")
        #noseBinaryPath = 'dvt/bin/nosetests'
        testDescriptor = self.inputArguments.descriptor.strip().replace(" ","_")
        resultFilename = "%s_%s_%s_%s_%s.xml" % (testDescriptor,self.inputArguments.architecture, eachTestType, eachTestGroup, eachTestName)

        # Initialize test result flag
        result = False

        # Check arguments before start the testcases, because some of them could be None
        # ej: DeviceID can be None if the provision fails (file does not exist, line not read, etc.).
        if (self.handlerHost.getDeviceID() != None and self.handlerHost.getDeviceType() != None):
            arguments = [noseBinaryPath,
                            '--with-xunit',
                            '-vv',
                            '-s', # Don't capture STDOUT (allow everything else to print)
                            '--xunit-file=%s' % os.path.join(resultFolderTestGroup,resultFilename),
                            ## Pinocchio plugin for Nose
                            #'--with-outputsave',
                            #'--save-directory=%s' % os.path.join(self.outputPath,folderTestGroup),
                            #'--omit-success',
                            ## CloudConnector Plugin for Nose
                            '--with-cc',
                            #'--stop', # Stop running tests after the first error or failure
                            '--cc-username=%s'  % self.inputArguments.username,
                            '--cc-password=%s'  % self.inputArguments.password,
                            '--cc-hostname=%s'  % self.inputArguments.server,
                            '--cc-deviceid=%s' % self.handlerHost.getDeviceID(),
                            '--cc-devicetype=%s' % self.handlerHost.getDeviceType(),
                            '--cc-devicemac=%s' % self.handlerHost.getDeviceMAC(),
                            '--cc-devicefirmware=%s' % self.handlerHost.getDeviceFirmwareVersion(),
                            '--cc-vendorid=%s' % self.inputArguments.vendorid,
                            '--cc-devicephone=%s' % self.inputArguments.devicephone,
                            '--cc-temporalpath=%s' % self.tempPath,
                            '--cc-testfilespath=%s' % self.testFilesPath]


            # Manage console output only if user no skip the run process
            if(self.inputArguments.skip_run is False):

                consoleOutputPath = os.path.join(resultFolderTestGroup,"console_output.log")

                # Check if console output path exist
                if ( os.path.exists( consoleOutputPath ) ):
                    arguments.append( '--cc-consolepath=%s' % consoleOutputPath )
                else:
                    self.logger.error("Missing console output file from result project path")


            # Add test cases pattern if a test cases list was provided
            if(TestCaseList != None):
                arguments.append("--testmatch=%s" % TestCaseList)


            # Add admin account settings only if admin flag is enabled
            if ( self.inputArguments.enable_admin_account ):
                # Add the admin account settings to use in test script from command line
                arguments.append("--cc-adminusername=%s" % self.inputArguments.adminusername)
                arguments.append("--cc-adminpassword=%s" % self.inputArguments.adminpassword)


            self.logger.info("Executing test case using Nose framework")
            self.logger.info("Nose arguments: ' %s '" % " ".join(arguments))

            ############################################################################
            #### NOTE: Exists one test that only works if Cloud Connector is set in ####
            ####       'debug' mode (test_debug_bad_values.py), because the source  ####
            ####       code creates a *.txt file with some debug messages from the  ####
            ####       source code.                                                 ####
            ####       These messages only appears in debug mode, and to avoid that ####
            ####       this test fails always in the other modes ('nodebug','compre-####
            ####       ssion', etc.) in the old code this test was skipped.         ####
            ####       We should find a better solution for this case, for this     ####
            ####       reason, we execute all test cases for all modes.             ####
            ############################################################################
            try:
                result = nose.run(defaultTest=[testPath], argv=arguments)
            except Exception,e:
                result = False
                self.logger.exception("Exception caught executing nose extension:\n %s" % e)
        else:
            # Testcases was not executed because some arguments before start are None
            self.logger.error("TestCase '%s' was not executed due to some arguments are empty" % testPath)

            # Generate a test result file for the testcases skipped
            message = "Running Process Failed"
            logBuildProcess = "TestCase was not executed due to some arguments are empty"
            resultFilePath = os.path.join(resultFolderTestGroup,resultFilename)

            # Generate the XML file
            return self.generateXMLTestResultContent(resultFilePath,eachTestType,eachTestGroup,eachTestName,result,message,logBuildProcess)


        # Parse result file from nose framework
        if ( self.inputArguments.documentation ):
            # Fill the internal structure with the nose info
            self.parseJUnitResultfile( eachTestType, eachTestGroup, eachTestName, os.path.join(resultFolderTestGroup,resultFilename) )


        if(result):
            self.logger.info("Test case '%s' was executed successfully" % testPath)
            return True
        else:
            self.logger.error("An error was occurred executing test case '%s'" % testPath)
            return False



    def createSourceFolder(self):
        # Create source folder for test cases
        if not os.path.exists(self.sourcePath):
            try:
                os.makedirs(self.sourcePath)
                _msg = "Path created for Source in '%s'" % self.sourcePath
                self.logger.info(_msg)
                return (True, _msg)
            except OSError,e:
                self.logger.exception('Exception caught: %s' % type(e))
                _msg = "Exception [OSError]: %s" % (str(e))
                return (False, _msg)
        else:
            _msg = "Source path was not created in '%s', due to exists" % self.sourcePath
            self.logger.warning(_msg)
            return (True, _msg)



    def getHandlerDeviceCloud(self):
        try:
            import DeviceCloud
        except ImportError:
            _msg = "DeviceCloud library was not found."
            self.logger.error(_msg)
            return (False,_msg)

        # Create a main handler
        handler = DeviceCloud.DeviceCloud()

        # Set Device Cloud URL
        handler.setDeviceCloudURL(self.inputArguments.server)
        # Set Account username
        handler.setDeviceCloudUsername(self.inputArguments.username)
        # Set Account password
        handler.setDeviceCloudPassword(self.inputArguments.password)

        return (True,handler)



    def getHandlerHost(self):
        # Create a main handler
        handler = None

        # Get device type from the command line
        platform = self.inputArguments.platform

        # Generic Library Host_platform, ej. Host_ansic
        library = "Host_%s" % (platform)

        # Try to import this class
        try:
            exec("import %s" % library )
            chain = "handler = %s.%s()" % (library,library)
            exec(chain)
            self.logger.info("'%s' library for host was successfully loaded" % library)

            ## Add the command line arguments in Host
            handler.inputArguments = self.inputArguments
            ## Add path to store test cases
            handler.sourceTestCasesPath = self.sourceTestCasesPath

            return (True,handler)
        except ImportError:
            _msg = "'%s' library for host was not found." % library
            self.logger.error(_msg)
            return (False,_msg)



    def showTestCasesList(self,enableOptions=True,filePath=None):

        # Array to save the test report
        testReport = []

        if(self.testCaseslist != None):
            testReport.append("")
            testReport.append("")
            testReport.append("Cloud Connector Test Framework result matrix:")

            # Get sorted list of TEST_TYPE
            testTypeList = sorted(self.testCaseslist.keys())

            for eachType in testTypeList:
                testReport.append("[T] %s " % eachType)
                # Options for TEST_TYPE
                if(enableOptions and self.testCaseslist[eachType]["options"] != None):
                    for eachOption in self.testCaseslist[eachType]["options"].keys():
                        testReport.append("[*]         > %s:%s" % (eachOption,self.testCaseslist[eachType]["options"][eachOption]))

                # each TEST_GROUP for TEST_TYPE
                if(self.testCaseslist[eachType] != None and self.testCaseslist[eachType]["groups"] != None):
                    # Get sorted list of TEST_GROUP
                    testGroupList = sorted(self.testCaseslist[eachType]["groups"].keys())

                    for eachGroup in testGroupList:
                        testReport.append("[G] - %s" % eachGroup)
                        # Options for TEST_GROUP
                        if(enableOptions and self.testCaseslist[eachType]["groups"][eachGroup]["options"] != None):
                            for eachOption in self.testCaseslist[eachType]["groups"][eachGroup]["options"].keys():
                                testReport.append("[*]         > %s:%s" % (eachOption,self.testCaseslist[eachType]["groups"][eachGroup]["options"][eachOption]))

                        # each TEST_NAME for TEST_GROUP
                        if(self.testCaseslist[eachType]["groups"][eachGroup] != None and self.testCaseslist[eachType]["groups"][eachGroup]["tests"] != None):
                            # Get sorted list of TEST_NAME
                            testNameList = sorted(self.testCaseslist[eachType]["groups"][eachGroup]["tests"].keys())

                            for eachName in testNameList:
                                testReport.append("[N] --- * %s" % eachName)
                                # Options for TEST_NAME
                                if(enableOptions and self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["options"] != None):
                                    for eachOption in self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["options"].keys():
                                        testReport.append("[*]         > %s:%s" % (eachOption,self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["options"][eachOption]))

                                # each TEST_CASE for TEST_NAME
                                if(self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName] != None
                                and self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"] != None):
                                    # Get sorted list of TEST_CASE
                                    testCaseList = sorted(self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"].keys())

                                    for eachCase in testCaseList:
                                        testReport.append("[C] ----- # %s" % eachCase)

                                        # Options for TEST_CASE
                                        if(enableOptions and self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"][eachCase]["options"] != None):
                                            for eachOption in self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"][eachCase]["options"].keys():
                                                if ( eachOption != "description"):
                                                    testReport.append("[*]         > %s:%s" % (eachOption,self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"][eachCase]["options"][eachOption]))
                                                else:
                                                    # Show the TEST_CASE description in lines
                                                    descriptionHeader = "[*]         > %s:" % eachOption

                                                    # Get description lines
                                                    description = self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"][eachCase]["options"][eachOption]
                                                    if ( len(description) > 0 ):
                                                        firstLine = True
                                                        linePadding = " "* len(descriptionHeader)

                                                        for eachLine in description:
                                                            if ( firstLine ):
                                                                testReport.append("%s%s" % (descriptionHeader,eachLine) )
                                                                firstLine = False
                                                            else:
                                                                testReport.append("%s%s" % (linePadding,eachLine) )
                                                    else:
                                                        testReport.append("%s <no description found>" % (descriptionHeader) )
            ### Show Legend
            testReport.append("")
            testReport.append("*****************************************************************************")
            testReport.append("[T]: TEST TYPE  |   [G]: TEST GROUP   |   [N]: TEST NAME   |   [C]: TEST CASE")
            testReport.append("")
            testReport.append("")


            ### Send the report to the output
            for eachLine in testReport:
                self.logger.info(eachLine)


            ### Send the report to a file
            if ( filePath is not None ):
                try:
                    # Save the result on a file
                    handlerFile = open(filePath, "w")
                    for eachLine in testReport:
                        handlerFile.write("%s\n" % eachLine)
                    handlerFile.close()
                except IOError,e:
                    self.logger.error("An error was occurred generating execution report file")
                    _msg = "Exception [IOError]: %s" % (str(e))
                    self.logger.error(_msg)
                    self.logger.exception(e)




    def createTestCasesMatrix(self):
        ''' Summary structure
            for every "TEST_TYPE"
             |  for every "TEST_GROUP"
             |   |  for every "TEST_NAME"
             |   |   |  for every "TEST_CASE"
             |   |   |   |  Create new test case

             return array with all tests.
        '''

        ################ Create test matrix
        # Check if a config file was passed in the command line
        if ( self.inputArguments.test_config_file is not None ):
            # Create test cases matrix from command line
            result,arraylistTests = self.createTestCasesMatrixFromConfigFile()
        else:
            # Create test cases matrix from configuration file
            result,arraylistTests = self.createTestCasesMatrixFromCommandLine()


        # Check the result of the generation of the test matrix
        if ( not result ):
            return (False, arraylistTests)


        ################ Save test matrix
        self.testCaseslist = arraylistTests
        # Display matrix
        self.showTestCasesList(enableOptions=False)

        return (True,"Test Cases matrix was successfully created")




    def createTestCasesMatrixFromCommandLine(self):

        arraylistTests = {}


        ################ TEST_TYPE
        listTestType = []
        if(self.inputArguments.test_type != None):
            # We have a list of test types
            listTestType = self.inputArguments.test_type
        else:
            # We need to add all "types" supported for the platform
            listTestType = os.listdir(self.sourceTestCasesPath)

        # We have a list of test types
        for eachItem in listTestType:
            # Create a list for each type
            arraylistTests[eachItem] = {}
            # Add a list for TEST_GROUP
            arraylistTests[eachItem]["groups"] = {}
            # Add a list for options in each TEST_TYPE
            arraylistTests[eachItem]["options"] = {}
            arraylistTests[eachItem]["options"]["result"] = False
            arraylistTests[eachItem]["options"]["elapsedTime"] = 0


        ################ TEST_GROUP
        listTestGroup = []
        if(self.inputArguments.test_group != None):
            # We have a list of test groups
            listTestGroup = self.inputArguments.test_group
            # Add the group to the first test type
            arraylistTests[arraylistTests.keys()[0]]["groups"][self.inputArguments.test_group] = {}
            # Add a list for each TEST_NAME
            arraylistTests[arraylistTests.keys()[0]]["groups"][self.inputArguments.test_group]["tests"] = {}
            # Add a list for options in each TEST_GROUP
            arraylistTests[arraylistTests.keys()[0]]["groups"][self.inputArguments.test_group]["options"] = {}
            arraylistTests[arraylistTests.keys()[0]]["groups"][self.inputArguments.test_group]["options"]["result"] = False
            arraylistTests[arraylistTests.keys()[0]]["groups"][self.inputArguments.test_group]["options"]["elapsedTime"] = 0
        else:
            # for every "TEST_TYPE" we need to found the groups
            for eachType in listTestType:
                # We need to add all "types" supported for the platform
                typePath = os.path.join(self.sourceTestCasesPath,eachType)

                # Check if directory exists
                if(os.path.isdir(typePath)):
                    listTestGroup = os.listdir(typePath)

                    # We have a list of test groups
                    for eachGroup in listTestGroup:

                        # Check if the TEST_GROUP is in the exclude list
                        if ( not eachGroup in self.inputArguments.exclude_test_group ):

                            arraylistTests[eachType]["groups"][eachGroup] = {}
                            # Add a list for each TEST_NAME
                            arraylistTests[eachType]["groups"][eachGroup]["tests"] = {}
                            # Add a list for options in each TEST_GROUP
                            arraylistTests[eachType]["groups"][eachGroup]["options"] = {}
                            arraylistTests[eachType]["groups"][eachGroup]["options"]["result"] = False
                            arraylistTests[eachType]["groups"][eachGroup]["options"]["elapsedTime"] = 0

                else:
                    self.logger.error("There is no such directory in '%s'" % typePath)
                    return (False,"Test Cases matrix was not created, because TYPE path no exists")


        ### Skip TEST_NAME and TEST_CASE for ONLY_BUILD_MODE
        if(self.inputArguments.only_build_mode!=True):
            ################ TEST_NAME
            listTestName = []
            if(self.inputArguments.test_name != None):
                # We have a list of test types
                listTestName = "%s.py" % self.inputArguments.test_name
                # Add the test to the first test groups
                testTypeKey = arraylistTests.keys()[0]
                testGroupKey = arraylistTests[testTypeKey]["groups"].keys()[0]
                arraylistTests[testTypeKey]["groups"][testGroupKey]["tests"][listTestName] = {}
                # Add a list for each TEST_CASE
                arraylistTests[testTypeKey]["groups"][testGroupKey]["tests"][listTestName]["cases"] = {}
                # Add a list for options in each TEST_NAME
                arraylistTests[testTypeKey]["groups"][testGroupKey]["tests"][listTestName]["options"] = {}
                arraylistTests[testTypeKey]["groups"][testGroupKey]["tests"][listTestName]["options"]["result"] = False
                arraylistTests[testTypeKey]["groups"][testGroupKey]["tests"][listTestName]["options"]["elapsedTime"] = 0
            else:
                # for every "TEST_TYPE" we need to found the groups
                for eachType in arraylistTests.keys():
                    # ej. .../dvt
                    typePath = os.path.join(self.sourceTestCasesPath,eachType)
                    # for every "TEST_GROUP" we need to found the names
                    for eachGroup in arraylistTests[eachType]["groups"].keys():
                        # ej. .../dvt/reboot/testcases/
                        groupPath = os.path.join(typePath,eachGroup,"testcases")

                        # Check if directory exists
                        if(os.path.isdir(groupPath)):
                            listTestName = os.listdir(groupPath)

                            # We have a list of test names
                            for eachName in listTestName:
                                # Only *.py files
                                if(eachName.endswith('.py')):

                                    # Check if the TEST_NAME is in the exclude list
                                    if ( not eachName in self.inputArguments.exclude_test_name ):

                                        arraylistTests[eachType]["groups"][eachGroup]["tests"][eachName] = {}
                                        # Add a list for each TEST_CASE
                                        arraylistTests[eachType]["groups"][eachGroup]["tests"][eachName]["cases"] = {}
                                        # Add a list for options in each TEST_NAME
                                        arraylistTests[eachType]["groups"][eachGroup]["tests"][eachName]["options"] = {}
                                        arraylistTests[eachType]["groups"][eachGroup]["tests"][eachName]["options"]["result"] = False
                                        arraylistTests[eachType]["groups"][eachGroup]["tests"][eachName]["options"]["elapsedTime"] = 0

                        else:
                            self.logger.error("There is no such directory in '%s'" % groupPath)
                            return (False,"Test Cases matrix was not created, because GROUP path no exists")


            ################ TEST_CASE
            listTestCase = []
            if(self.inputArguments.test_case != None):
                # Add the TEST_CASE to the first TEST_NAME
                testTypeKey = arraylistTests.keys()[0]
                testGroupKey = arraylistTests[testTypeKey]["groups"].keys()[0]
                testNameKey = arraylistTests[testTypeKey]["groups"][testGroupKey]["tests"].keys()[0]
                arraylistTests[testTypeKey]["groups"][testGroupKey]["tests"][testNameKey]["cases"][self.inputArguments.test_case] = {}

                # Add a list for options in TEST_CASE
                arraylistTests[testTypeKey]["groups"][testGroupKey]["tests"][testNameKey]["cases"][self.inputArguments.test_case]["options"] = {}
                #arraylistTests[testTypeKey]["groups"][testGroupKey]["tests"][testNameKey]["cases"][self.inputArguments.test_case]["options"]["result"] = False
                #arraylistTests[testTypeKey]["groups"][testGroupKey]["tests"][testNameKey]["cases"][self.inputArguments.test_case]["options"]["elapsedTime"] = 0



        # Return the test matrix created
        return (True,arraylistTests)




    def createTestCasesMatrixFromConfigFile(self):
        ''' Example of structure for a config file
            {
                "dvt":
                {   "groups":
                    {
                        "sm_udp_connect":
                        {
                            "tests":
                            {
                                "test_sm_udp_connect.py":
                                {
                                    "cases":
                                    {
                                        "test_sm_udp_0_connect": {},
                                        "test_update_1_firmware": {}
                                    }
                                }
                            }
                        }
                    }
                }
            }
        '''

        # Verify if the file exist
        if(not os.path.isfile( self.inputArguments.test_config_file) ):
            _msg = "Configuration file does not exist in '%s'" % self.inputArguments.test_config_file
            self.logger.error(_msg)
            return (False,_msg)


        # Open and parse the file in JSON format
        try:
            with open(self.inputArguments.test_config_file) as data_file:
                arraylistTests = simplejson.load(data_file)
        except ValueError,e:
            _msg = "Exception Caught: %s" % type(e)
            self.logger.error(_msg)
            self.logger.exception(e)
            return (False,_msg)


        # We fill all parameters needed by the structure
        ################ TEST_TYPE
        for eachType in arraylistTests:

            #** Unify all needed fields
            # Check if each type has the attribute options
            if ( arraylistTests[eachType].has_key("options") ):
                # If result field is missing, we create it
                if ( not arraylistTests[eachType]["options"].has_key("result") ):
                    arraylistTests[eachType]["options"]["result"] = False

                # If elapsedTime field is missing, we create it
                if ( not arraylistTests[eachType]["options"].has_key("elapsedTime") ):
                    arraylistTests[eachType]["options"]["elapsedTime"] = 0

            else:
                # Add a list for options in each TEST_TYPE
                arraylistTests[eachType]["options"] = {}
                arraylistTests[eachType]["options"]["result"] = False
                arraylistTests[eachType]["options"]["elapsedTime"] = 0


            # Check if this TEST_TYPE has TEST_GROUP
            if ( arraylistTests[eachType].has_key("groups") ):

                ################ TEST_GROUP
                for eachGroup in arraylistTests[eachType]["groups"]:

                    #** Unify all needed fields
                    # Check if each group has the attribute options
                    if ( arraylistTests[eachType]["groups"][eachGroup].has_key("options") ):
                        # If result field is missing, we create it
                        if ( not arraylistTests[eachType]["groups"][eachGroup]["options"].has_key("result") ):
                            arraylistTests[eachType]["groups"][eachGroup]["options"]["result"] = False

                        # If elapsedTime field is missing, we create it
                        if ( not arraylistTests[eachType]["groups"][eachGroup]["options"].has_key("elapsedTime") ):
                            arraylistTests[eachType]["groups"][eachGroup]["options"]["elapsedTime"] = 0

                    else:
                        # Add a list for options in each TEST_TYPE
                        arraylistTests[eachType]["groups"][eachGroup]["options"] = {}
                        arraylistTests[eachType]["groups"][eachGroup]["options"]["result"] = False
                        arraylistTests[eachType]["groups"][eachGroup]["options"]["elapsedTime"] = 0


                    # Check if this TEST_GROUP has TEST_NAME
                    if ( arraylistTests[eachType]["groups"][eachGroup].has_key("tests") ):

                        ################ TEST_NAME
                        for eachTest in arraylistTests[eachType]["groups"][eachGroup]["tests"]:

                            #** Unify all needed fields
                            # Check if each test has the attribute options
                            if ( arraylistTests[eachType]["groups"][eachGroup]["tests"][eachTest].has_key("options") ):
                                # If result field is missing, we create it
                                if ( not arraylistTests[eachType]["groups"][eachGroup]["tests"][eachTest]["options"].has_key("result") ):
                                    arraylistTests[eachType]["groups"][eachGroup]["tests"][eachTest]["options"]["result"] = False

                                # If elapsedTime field is missing, we create it
                                if ( not arraylistTests[eachType]["groups"][eachGroup]["tests"][eachTest]["options"].has_key("elapsedTime") ):
                                    arraylistTests[eachType]["groups"][eachGroup]["tests"][eachTest]["options"]["elapsedTime"] = 0

                            else:
                                # Add a list for options in each TEST_TYPE
                                arraylistTests[eachType]["groups"][eachGroup]["tests"][eachTest]["options"] = {}
                                arraylistTests[eachType]["groups"][eachGroup]["tests"][eachTest]["options"]["result"] = False
                                arraylistTests[eachType]["groups"][eachGroup]["tests"][eachTest]["options"]["elapsedTime"] = 0


                            # Check if each test has the attribute cases
                            if ( arraylistTests[eachType]["groups"][eachGroup]["tests"][eachTest].has_key("cases") ):

                                ################ TEST_CASE
                                for eachCase in arraylistTests[eachType]["groups"][eachGroup]["tests"][eachTest]["cases"]:

                                    #** Unify all needed fields
                                    # Check if each test has the attribute options
                                    if ( not arraylistTests[eachType]["groups"][eachGroup]["tests"][eachTest]["cases"][eachCase].has_key("options") ):
                                        # Add a list for options in each TEST_TYPE
                                        arraylistTests[eachType]["groups"][eachGroup]["tests"][eachTest]["cases"][eachCase]["options"] = {}
                                        #arraylistTests[eachType]["groups"][eachGroup]["tests"][eachTest]["cases"][eachCase]["options"]["result"] = False
                                        #arraylistTests[eachType]["groups"][eachGroup]["tests"][eachTest]["cases"][eachCase]["options"]["elapsedTime"] = 0

                            else:
                                # Add a empty list for cases in each TEST_NAME
                                arraylistTests[eachType]["groups"][eachGroup]["tests"][eachTest]["cases"] = {}


        # Return the test matrix created
        return (True,arraylistTests)




    def parseDocumentationFromTestCases(self):
        if(self.testCaseslist != None):
            for eachType in self.testCaseslist.keys():

                # each TEST_GROUP for TEST_TYPE
                if(self.testCaseslist[eachType] != None and self.testCaseslist[eachType]["groups"] != None):
                    for eachGroup in self.testCaseslist[eachType]["groups"].keys():

                        # each TEST_NAME for TEST_GROUP
                        if(self.testCaseslist[eachType]["groups"][eachGroup] != None and self.testCaseslist[eachType]["groups"][eachGroup]["tests"] != None):
                            for eachName in self.testCaseslist[eachType]["groups"][eachGroup]["tests"].keys():

                                testCasesList = []
                                # each TEST_CASE for TEST_NAME
                                if(self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName] != None
                                and self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"] != None):
                                    for eachCase in self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"].keys():
                                        testCasesList.append(eachCase)

                                # Compound test file path
                                filePath = os.path.join(self.sourceTestCasesPath,eachType,eachGroup,"testcases",eachName) #ej. "/home/username/TestFolder/source/cc_testcases/dvt/reboot/testcases/testname"

                                # Verify if this file exists
                                if(os.path.isfile(filePath)):
                                    # Setup app/connector_config.h
                                    testFile = TestCaseFile.TestCaseFile(filePath)
                                    # Read original file
                                    testFile.readFile()
                                    # Check the file content to find each test case
                                    testFile.getTestCasesInfo()
                                    # Get the list of test cases
                                    testCasesDict = testFile.testCaseCommentList
                                    # Get sorted list of test cases
                                    testCasesNames = sorted(testCasesDict.keys())


                                    #for eachCase in testCasesDict.keys():
                                    for eachCase in testCasesNames:

                                        if ( len(testCasesList) == 0 or eachCase in testCasesList):
                                            # Get test case description
                                            description = testCasesDict[eachCase]

                                            # Create the test case entry if not exits
                                            if ( not self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"].has_key(eachCase) ):
                                                self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"][eachCase] = {}

                                            # Check if each test case has the attribute options
                                            if ( not self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"][eachCase].has_key("options") ):
                                                # Add a list for options in each TEST_CASE
                                                self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"][eachCase]["options"] = {}

                                            # Add the comments from the file to the TEST_CASE
                                            self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"][eachCase]["options"]["description"] = description

                                            self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"][eachCase]["options"]["result"] = None
                                            self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"][eachCase]["options"]["elapsedTime"] = 0


                                else:
                                    self.logger.warning("Skip documentation for file '%s' because does not exists in '%s'" % (eachName,filePath))




    def generateDocumentation(self, folderPath):

        # Array to save the test report
        executionReportXML = []

        if(self.testCaseslist != None):
            # Insert the XML headers
            executionReportXML.append('<?xml version="1.0" encoding="ISO-8859-1"?>')
            executionReportXML.append("<!-- Created by Cloud Connector Test Framework -->")
            executionReportXML.append('<?xml-stylesheet type="text/xsl" href="test_report.xsl"?>')

            # Add root label for test_result XML file
            executionReportXML.append('<test_result>')

            # Add the test descriptor item
            executionReportXML.append('<test_descriptor>')
            # Insert the test descriptor name field
            executionReportXML.append('<name>%s</name>' % self.inputArguments.descriptor)

            # Get sorted list of TEST_TYPE
            testTypeList = sorted(self.testCaseslist.keys())

            for eachType in testTypeList:
                executionReportXML.append('<test_type>')
                # Insert the test type name field
                executionReportXML.append('<name>%s</name>' % eachType)

                # Options for TEST_TYPE
                if(self.testCaseslist[eachType]["options"] != None):
                    for eachOption in self.testCaseslist[eachType]["options"].keys():
                        executionReportXML.append('<%s>%s</%s>' % (eachOption,self.testCaseslist[eachType]["options"][eachOption],eachOption) )


                # each TEST_GROUP for TEST_TYPE
                if(self.testCaseslist[eachType] != None and self.testCaseslist[eachType]["groups"] != None):
                    # Insert the groups tag
                    executionReportXML.append('<groups>')

                    # Get sorted list of TEST_GROUP
                    testGroupList = sorted(self.testCaseslist[eachType]["groups"].keys())

                    for eachGroup in testGroupList:
                        executionReportXML.append('<test_group>')
                        # Insert the test group name field
                        executionReportXML.append('<name>%s</name>' % eachGroup)

                        # Options for TEST_GROUP
                        if(self.testCaseslist[eachType]["groups"][eachGroup]["options"] != None):
                            for eachOption in self.testCaseslist[eachType]["groups"][eachGroup]["options"].keys():
                                executionReportXML.append('<%s>%s</%s>' % (eachOption,self.testCaseslist[eachType]["groups"][eachGroup]["options"][eachOption],eachOption) )


                        # each TEST_NAME for TEST_GROUP
                        if(self.testCaseslist[eachType]["groups"][eachGroup] != None and self.testCaseslist[eachType]["groups"][eachGroup]["tests"] != None):
                            # Insert the tests tag
                            executionReportXML.append('<tests>')

                            # Get sorted list of TEST_NAME
                            testNameList = sorted(self.testCaseslist[eachType]["groups"][eachGroup]["tests"].keys())

                            for eachName in testNameList:
                                executionReportXML.append('<test_name>')
                                # Insert the test name name field
                                executionReportXML.append('<name>%s</name>' % eachName)

                                # Options for TEST_NAME
                                if(self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["options"] != None):
                                    for eachOption in self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["options"].keys():
                                        executionReportXML.append('<%s>%s</%s>' % (eachOption,self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["options"][eachOption],eachOption) )

                                # each TEST_CASE for TEST_NAME
                                if(self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName] != None
                                and self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"] != None):
                                    # Insert the cases tag
                                    executionReportXML.append('<cases>')

                                    # Get sorted list of TEST_CASE
                                    testCaseList = sorted(self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"].keys())

                                    for eachCase in testCaseList:

                                        executionReportXML.append('<test_case>')
                                        # Insert the test case name field
                                        executionReportXML.append('<name>%s</name>' % eachCase)

                                        # Options for TEST_CASE
                                        if(self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"][eachCase]["options"] != None):
                                            for eachOption in self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"][eachCase]["options"].keys():
                                                if ( eachOption != "description"):
                                                    executionReportXML.append('<%s>%s</%s>' % (eachOption,self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"][eachCase]["options"][eachOption],eachOption) )

                                                else:
                                                    # Get description lines
                                                    description = self.testCaseslist[eachType]["groups"][eachGroup]["tests"][eachName]["cases"][eachCase]["options"][eachOption]

                                                    executionReportXML.append("<description>")
                                                    for eachLine in description:
                                                        executionReportXML.append(eachLine)
                                                    executionReportXML.append("</description>")

                                        # Close the test name tag
                                        executionReportXML.append('</test_case>')
                                    # Close the tests tag
                                    executionReportXML.append('</cases>')
                                # Close the test name tag
                                executionReportXML.append('</test_name>')
                            # Close the tests tag
                            executionReportXML.append('</tests>')
                        # Close the test group tag
                        executionReportXML.append('</test_group>')
                    # Close the groups tag
                    executionReportXML.append('</groups>')
                # Close the test type tag
                executionReportXML.append('</test_type>')

                # Close the test descriptor tag
                executionReportXML.append('</test_descriptor>')

                # Close the global result tag
                executionReportXML.append('</test_result>')


            ### Create the folder is not exits
            if not os.path.exists(folderPath):
                try:
                    os.makedirs(folderPath)
                    _msg = "Path created for the execution report in '%s'" % folderPath
                    self.logger.info(_msg)
                except OSError,e:
                    self.logger.exception('Exception caught: %s' % type(e))
                    _msg = "Exception [OSError]: %s" % (str(e))
                    return (False, _msg)



            ### Send the report to a XML file
            xmlFilename = "execution_report.xml"
            if ( self.inputArguments.descriptor != "" ):
                xmlFilename = "execution_report_%s.xml" % self.inputArguments.descriptor.strip().replace(" ","_")
            reportXMLFilePath = os.path.abspath(os.path.join(folderPath,xmlFilename))

            try:
                # Save the result on a file
                handlerFile = open(reportXMLFilePath, "w")
                for eachLine in executionReportXML:
                    handlerFile.write("%s\n" % eachLine)
                handlerFile.close()
            except IOError,e:
                self.logger.error("An error was occurred generating execution report XML file")
                _msg = "Exception [IOError]: %s" % (str(e))
                self.logger.error(_msg)
                self.logger.exception(e)


            ### Copy to report folder the necessaries files to support XSLT
            # Original Files
            stylesheetFilePath = os.path.abspath(os.path.join(self.corePath,"report","test_report.css"))
            xsltFilePath = os.path.abspath(os.path.join(self.corePath,"report","test_report.xsl"))
            # Destination files
            reportStylesheetFilePath = os.path.abspath(os.path.join(folderPath,"test_report.css"))
            reportXsltFilePath = os.path.abspath(os.path.join(folderPath,"test_report.xsl"))

            # copy files
            try:
                shutil.copyfile(stylesheetFilePath,reportStylesheetFilePath)
                shutil.copyfile(xsltFilePath,reportXsltFilePath)
            except IOError,e:
                self.logger.info("An error was occurred copying XSLT result files")
                _msg = "Exception [IOError]: %s" % (str(e))
                self.logger.info(_msg)
                self.logger.exception('Exception caught: %s' % type(e))




    ## {{{ http://code.activestate.com/recipes/511486/ (r1)
    ## Here is a small footprint routine to convert a number of floating point seconds (such as an elapsed time,
    ## found by subtracting an earlier return value from time.time() from a later value) to "0:00:00.000" format,
    ## instead of "time.strftime("%H:%M:%S",time.gmtime(t2-t1))+(".%03d" % (((t2-t1)-int(t2-t1))*1000))"
    def secondsToStr(self,t):
        # Method A:
        return time.strftime("%H:%M:%S",time.gmtime(t))+(".%03d" % (((t)-int(t))*1000))
        # Method B:
        # rediv = lambda ll,b : list(divmod(ll[0],b)) + ll[1:]
        # return "%d:%02d:%02d.%03d" % tuple(reduce(rediv,[[t*1000,],1000,60,60]))




    def generateRegisterDeviceXMLResultContent(self,outputFolder,testType,testGroup,result,logRegisterProcess):
        # filePath: destination path to generate a xml file, ej. "/home/username/TestFolder/results/result_xxxxx/dvt_reboot/<filename>.xml"
        # testType: Type name, ej. 'dvt'
        # testGroup: Group name, ej. 'reboot'
        # result: 'True' or 'False'
        # logRegisterProcess: all stack trace for the build process

        # Initialize vars
        testName = "register"
        message = "Register Device Process Failed"
        testDescriptor = self.inputArguments.descriptor.strip().replace(" ","_")
        resultFilename = "%s_%s_%s_%s_%s.xml" % (testDescriptor,self.inputArguments.architecture, testType, testGroup, testName)
        resultFilePath = os.path.abspath(os.path.join(outputFolder,resultFilename))

        # Generate the XML file
        return self.generateXMLTestResultContent(resultFilePath,testType,testGroup,testName,result,message,logRegisterProcess)




    def generateTestBuildFileXMLResultContent(self,outputFolder,testType,testGroup,result,logBuildProcess):
        # filePath: destination path to generate a xml file, ej. "/home/username/TestFolder/results/result_xxxxx/dvt_reboot/<filename>.xml"
        # testType: Type name, ej. 'dvt'
        # testGroup: Group name, ej. 'reboot'
        # result: 'True' or 'False'
        # logBuildProcess: all stack trace for the build process

        # Initialize vars
        testName = "build"
        message = "Build Process Failed"
        testDescriptor = self.inputArguments.descriptor.strip().replace(" ","_")
        resultFilename = "%s_%s_%s_%s_%s.xml" % (testDescriptor,self.inputArguments.architecture, testType, testGroup, testName)
        resultFilePath = os.path.abspath(os.path.join(outputFolder,resultFilename))

        # Generate the XML file
        return self.generateXMLTestResultContent(resultFilePath,testType,testGroup,testName,result,message,logBuildProcess)




    def generateXMLTestResultContent(self,resultFilePath,testType,testGroup,testName,result,message,logProcess):
        # filePath: destination path to generate a xml file, ej. "/home/username/TestFolder/results/result_xxxxx/dvt_reboot/<filename>.xml"
        # testType: Type name, ej. 'dvt'
        # testGroup: Group name, ej. 'reboot'
        # testName: Test name, ej. 'build'
        # result: 'True' or 'False'
        # Message: Summary for this result, ej. 'Build Process Failed'
        # logProcess: all stack trace for the build process

        # Initialize vars
        numTests = 1
        if(result):
            numFailures = 0
        else:
            numFailures = 1


        # Escape characters that will have conflicts with XML
        # "   &quot;
        # '   &apos;
        # <   &lt;
        # >   &gt;
        # &   &amp;
        if(logProcess != None):
            #logProcess = logProcess.replace("<","&lt;")
            #logProcess = logProcess.replace(">","&gt;")
            #logProcess = "<![CDATA[%s]]>" % logProcess
            logProcess = logProcess.replace("<","")
            logProcess = logProcess.replace(">","")


        # File Content
        fileContent = """<?xml version="1.0" encoding="UTF-8"?>"""
        fileContent += """<testsuite name="nosetests" tests="%s" errors="0" failures="%s" skip="0">""" % (numTests,numFailures)

        if(result):
            # Test Case was successfully
            fileContent += """<testcase classname="%s_%s.%sTestCase" name="test_%s_%s_%s" time="1" />""" % (testType,testGroup,testName,testType,testGroup,testName)
        else:
            fileContent += """<testcase classname="%s_%s.%sTestCase" name="test_%s_%s_%s" time="1" >""" % (testType,testGroup,testName,testType,testGroup,testName)
            # Add a Failure line
            fileContent += """<failure type="exceptions.AssertionError" message="%s">%s</failure>""" % (message,logProcess)
            # End Test case
            fileContent += """</testcase>"""

        fileContent += """</testsuite>"""

        # Generate result file
        try:
            handlerFile = open(resultFilePath, "w")
            handlerFile.write(fileContent)
            handlerFile.close()
        except IOError,e:
            self.logger.info("An error was occurred generating XML result file")
            _msg = "Exception [IOError]: %s" % (str(e))
            self.logger.info(_msg)
            self.logger.exception('Exception caught: %s' % type(e))

        return True



    def parseJUnitResultfile(self, testType, testGroup, testName, resultFilePath):

        self.logger.info("Getting the info from the file '%s'" % resultFilePath)

        # Get the root element
        testSuiteElementTree = xml.etree.ElementTree.parse(resultFilePath).getroot() # <testsuite>

        # Get <testcase> element
        testCaseElementTree = testSuiteElementTree.findall("testcase") # <testcase>

        for eachElementTree in testCaseElementTree:
            testName = "%s.py" % eachElementTree.attrib["classname"].split(".")[0]
            testCase = eachElementTree.attrib["name"]
            testTime = eachElementTree.attrib["time"]
            testResult = None

            failureElementTree = eachElementTree.find("failure")
            if ( failureElementTree is None ):
                testResult = True
            else:
                testResult = False

            # Find and fill the info
            self.testCaseslist[testType]["groups"][testGroup]["tests"][testName]["cases"][testCase]["options"]["result"] = testResult
            self.testCaseslist[testType]["groups"][testGroup]["tests"][testName]["cases"][testCase]["options"]["elapsedTime"] = testTime




    def cleanEnvironment(self):
        self.logger.info("Perform clean tasks to destroy all objects and handlers...")
        # Close and Destroy Device Cloud handler
        if ( self.handlerDeviceCloud is not None ):
            self.handlerDeviceCloud.close()


        #*** Kill alive threads, wait max 30 seconds, if not force os kill
        count = 10
        threadList = threading.enumerate()
        # Check if there is more than one thread alive (main thread)
        while (count != 0) and (len(threadList) != 1):
            for thread in threadList:
                if (thread.getName() != "MainThread"):
                    if (thread.isAlive()):
                        threadType = type(thread).__name__
                        self.logger.info("Found a thread alive '%s' of type '%s', stopping it." % ( thread.getName(), threadType) )
                        try:
                            if(threadType == "FileHandler"):
                                # Stop the execution
                                thread.stopExecution()
                            else:
                                # Call to native function to stop threads
                                thread._Thread__stop()
                        except AttributeError, e:
                            self.logger.warning("Unexpected Exception Caught: %s" % type(e))
                            self.logger.exception(e)
            time.sleep(1)
            count -= 1
            threadList = threading.enumerate()

        if (len(threadList) != 1):
            self.logger.error("There are threads alive after reaching maximum timeout ")
            for thread in threadList:
                if (thread.getName() != "MainThread"):
                    if (thread.isAlive()):
                        self.logger.error("Thread alive '%s'" % thread.getName())

        return (len(threadList) == 1)




if __name__ == "__main__":
    ## Create a new CloudConnector Test
    script = TestHarness()
    ## Start Script
    script.run(sys.argv[1:])