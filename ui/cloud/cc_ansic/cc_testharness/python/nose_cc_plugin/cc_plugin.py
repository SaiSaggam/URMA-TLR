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
# Cloud Connector Test Framework - Cloud Connector plugin for nose
#

################ Import Common libraries
import os               ### Some useful functions on pathnames
import logging          ### Functions and classes to implement a event logging system
import time             ### Provides various time-related functions
import traceback        ### Provides a standard interface to extract, format and print stack traces of Python programs
import requests         ### Provide the most of the HTTP capabilities
import nose.plugins     ### nose extends unittest to make testing easier


################ Import Classes path
import DeviceCloud              ### Library to comunicate with the Cloud server
import DeviceConnectionMonitor  ### Library to provide a TCP connection monitor


from nose.tools import *



log = logging.getLogger('cc.nose.plugin')


## Wrapper class to simulate a stream class to send all outputs
## to logging system
class CC_OutputStream():

    def write(self, text=""):
        log.info(text)

    def writeln(self, text=""):
        log.info(text)

    def flush(self):
        pass



class CloudConnectorPlugin(nose.plugins.Plugin):
    ## Plugin identifier to use "--with-XX"
    name = 'cc'
    ## Console output handler for Device under Test
    deviceHandler = None
    ## Cloud server handler
    cloudHandler = None
    ## Cloud server handler with admin privileges
    cloudAdminHandler = None
    ## Device connection monitor handler
    deviceMonitor = None
    # Temp path for test cases files
    tempPath = None
    # Testharness test files path
    testFilesPath = None

    ## SETUP SECTION **************************************************

    ## Redefined: Register Commandline options
    ## Called to allow plugin to register command line options with the parser.
    ## DO NOT return a value from this method unless you want to stop all other plugins from setting their options.
    ## @param: parser (ConfigParser.ConfigParser) - options parser instance
    ## @param: env - environment, default is os.environ
    def options(self, parser, env=os.environ):
        nose.plugins.Plugin.options(self, parser, env=env)
        parser.add_option('--cc-username', action='store', type="string", dest="username", default="", help="Username for the account where the device is registered.")
        parser.add_option('--cc-password', action='store', type="string", dest="password", default="", help="Password for the account where the device is registered.")
        parser.add_option('--cc-hostname', action='store', type="string", dest="hostname", default="", help="Server URL where the device is connected.")
        parser.add_option('--cc-deviceid', action='store', type="string", dest="device_id", default="", help="Device ID of device under test.")
        parser.add_option('--cc-vendorid', action='store', type="string", dest="vendor_id", default="", help="Vendor ID of device under test.")
        parser.add_option('--cc-devicetype', action='store', type="string", dest="device_type", default="", help="Device Type of device under test.")
        parser.add_option('--cc-ipaddr', action='store', type="string", dest="ipaddr", default="", help="IP address of device under test.")
        parser.add_option('--cc-devicemac', action='store', type="string", dest="device_mac", default="", help="Device MAC Address of device under test.")
        parser.add_option('--cc-devicefirmware', action='store', type="string", dest="device_firmware", default="", help="Device Firmware level of device under test.")
        parser.add_option('--cc-consolepath', action='store', type="string", dest="consolepath", default="", help="Absolute path to console output file.")
        parser.add_option('--cc-devicephone', action='store', type="string", dest="devicephone", default="", help="Device Phone Number for SMS")
        parser.add_option('--cc-temporalpath', action='store', type="string", dest="temporalpath", default="", help="Sandbox folder to manage during the execution")
        parser.add_option('--cc-testfilespath', action='store', type="string", dest="testfilespath", default="", help="Folder where to obtain various test files")
        parser.add_option('--cc-adminusername', action='store', type="string", dest="adminusername", default="", help="Username for the Admin account where the device is registered.")
        parser.add_option('--cc-adminpassword', action='store', type="string", dest="adminpassword", default="", help="Password for the Admin account where the device is registered.")



    ## Redefined: Configure the plugin and system, based on selected options
    ## Called after the command line has been parsed, with the parsed options and the config container.
    ## Here, implement any config storage or changes to state or operation that are set by command line options.
    ## DO NOT return a value from this method unless you want to stop all other plugins from being configured.
    ## @param: options - test result object
    def configure(self, options, conf):
        # Call to configure the common options
        nose.plugins.Plugin.configure(self, options, conf)

        if not self.enabled:
            return

        #** Cloud Connector Account settings
        CloudConnectorPlugin.hostname      = options.hostname
        CloudConnectorPlugin.username      = options.username
        CloudConnectorPlugin.password      = options.password
        CloudConnectorPlugin.vendor_id     = options.vendor_id


        #** Cloud Connector Device settings
        CloudConnectorPlugin.device_id          = options.device_id
        CloudConnectorPlugin.device_type        = options.device_type
        CloudConnectorPlugin.ipaddr             = options.ipaddr
        CloudConnectorPlugin.device_mac         = options.device_mac
        CloudConnectorPlugin.device_firmware    = options.device_firmware
        CloudConnectorPlugin.devicephone        = options.devicephone


        #** Device Cloud Admin Account settings
        CloudConnectorPlugin.adminusername = options.adminusername
        CloudConnectorPlugin.adminpassword = options.adminpassword


        #** Cloud Connector Environment settings
        self.tempPath           = options.temporalpath
        self.testFilesPath      = options.testfilespath


        #** Cloud Connector console handler settings
        if( options.consolepath != "" ):
            CloudConnectorPlugin.consolepath = options.consolepath

            import FileHandler

            # Get the serial port path and the baud rate
            self.deviceHandler = FileHandler.FileHandler(CloudConnectorPlugin.consolepath)

            self.deviceHandler.setLogIdentifier("cc")
            self.deviceHandler.setIdentifier("FileHandler")

            # Disable storing in a file
            self.deviceHandler.disableLogFile()

            # Initialize handler
            self.deviceHandler.open()

            # Disable the logging by console
            self.deviceHandler.disableLogExecution()

            # Start read thread
            self.deviceHandler.start()


            # Flush the data buffer to start with a clean output
            retries = 0
            while(retries<10):
                retries+=1
                self.deviceHandler.readBuffer()
                time.sleep(0.1)



        #** Device Connection Monitor handler settings
        # Create new instance for Device Connection Monitor
        self.deviceMonitor = DeviceConnectionMonitor.DeviceConnectionMonitor(CloudConnectorPlugin.device_id)

        # Set Device Cloud URL
        self.deviceMonitor.setDeviceCloudURL(CloudConnectorPlugin.hostname)
        # Set Account username
        self.deviceMonitor.setDeviceCloudUsername(CloudConnectorPlugin.username)
        # Set Account password
        self.deviceMonitor.setDeviceCloudPassword(CloudConnectorPlugin.password)

        # Initialize handler
        self.deviceMonitor.start()



        #** Cloud Server handler settings
        # Create new instance for Device Cloud server
        self.cloudHandler = DeviceCloud.DeviceCloud()

        # Set Device Cloud URL
        self.cloudHandler.setDeviceCloudURL(CloudConnectorPlugin.hostname)
        # Set Account username
        self.cloudHandler.setDeviceCloudUsername(CloudConnectorPlugin.username)
        # Set Account password
        self.cloudHandler.setDeviceCloudPassword(CloudConnectorPlugin.password)

        # Initialize handler
        self.cloudHandler.connect()



        #** Cloud Server handler with Admin privileges settings
        if( options.adminusername != "" and options.adminpassword != "" ):
            # Create new instance for Device Cloud server
            self.cloudAdminHandler = DeviceCloud.DeviceCloud()

            # Set Device Cloud URL
            self.cloudAdminHandler.setDeviceCloudURL(CloudConnectorPlugin.hostname)
            # Set Account username
            self.cloudAdminHandler.setDeviceCloudUsername(CloudConnectorPlugin.adminusername)
            # Set Account password
            self.cloudAdminHandler.setDeviceCloudPassword(CloudConnectorPlugin.adminpassword)

            # Initialize handler
            self.cloudAdminHandler.connect()





    ## Redefined: Prepare or wrap an individual test case. Called before execution of the test.
    ## The test passed here is a nose.case.Test instance; the case to be executed is in the test
    ## attribute of the passed case. To modify the test to be run, you should return a callable
    ## that takes one argument (the test result object) - it is recommended that you do not
    ## side-effect the nose.case.Test instance you have been passed.
    ##
    ## Keep in mind that when you replace the test callable you are replacing the run() method
    ## of the test case - including the exception handling and result calls, etc.
    ## @param: test (nose.case.Test) - the test case
    def prepareTestCase(self, test):
        #** Cloud Connector Account settings
        test.test.username      = CloudConnectorPlugin.username
        test.test.password      = CloudConnectorPlugin.password
        test.test.hostname      = CloudConnectorPlugin.hostname
        test.test.vendor_id     = CloudConnectorPlugin.vendor_id

        #** Cloud Connector Device settings
        test.test.device_id             = CloudConnectorPlugin.device_id
        test.test.device_type           = CloudConnectorPlugin.device_type
        test.test.ipaddr                = CloudConnectorPlugin.ipaddr
        test.test.device_mac            = CloudConnectorPlugin.device_mac
        test.test.device_firmware       = CloudConnectorPlugin.device_firmware
        test.test.devicephone           = CloudConnectorPlugin.devicephone

        #** Cloud Connector Environment settings
        test.test.tempPath      = self.tempPath
        test.test.testFilesPath = self.testFilesPath


        #** Device Connection Monitor handler settings
        test.test.deviceMonitor = self.deviceMonitor

        #** Cloud Server handler settings
        test.test.cloudHandler = self.cloudHandler

        #** Cloud Connector console handler settings
        test.test.deviceHandler = self.deviceHandler

        #** Cloud Server Admin handler settings
        test.test.cloudAdminHandler = self.cloudAdminHandler



    ## TIMELINE SECTION **************************************************

    ## Redefined: Called before each test is run.
    ## DO NOT return a value unless you want to stop other plugins from seeing the test start
    ## @param: test (nose.case.Test) - the test case
    def startTest(self, test):
        msg = "*** Running Test Case '%s' ***" % test
        line = "*" * len(msg)
        log.info("\n\n\n\n")
        log.info(line)
        log.info(msg)
        log.info(line)


    ## Redefined: Called after all report output, including output from all plugins, has been sent to the stream.
    ## Use this to print final test results or perform final cleanup.
    ## Return None to allow other plugins to continue printing, or any other value to stop them.
    ## @param: result - test result object
    def finalize(self, result):
        if (self.deviceHandler is not None):
            log.info('Stopping Device Console handler....')
            self.deviceHandler.stopExecution()

        if (self.cloudHandler is not None):
            log.info('Stopping Device Cloud connection handler....')
            self.cloudHandler.close()

        if (self.deviceMonitor is not None):
            log.info('Stopping Device Connection Monitor handler....')
            self.deviceMonitor.stop()

        if (self.cloudAdminHandler is not None):
            log.info('Stopping Device Cloud Admin connection handler....')
            self.cloudAdminHandler.close()

        log.info('Finished Cloud Connector nose plugin execution!')





    ## OUTPUT CAPTURE SECTION **************************************************

    ## Redefined: Called before test output begins.
    ## To direct test output to a new stream, return a stream object,
    ## which must implement a write(msg) method.
    ## If you only want to note the stream, not capture or redirect it, then return None.
    ## @param: stream (file-like object) - stream object; send your output here
    def setOutputStream(self, stream):
        # grab for own use
        self.stream = stream

        output = CC_OutputStream()

        return output


    ## Redefined: Called when a test passes.
    ## DO NOT return a value unless you want to stop other plugins from seeing the passing test.
    ## @param: test (nose.case.Test) - the test case
    def addSuccess(self, test):
        log.info("Test Case '%s' SUCCESS!!" % test)


    ## Redefined: Called when a test raises an uncaught exception.
    ## DO NOT return a value unless you want to stop other plugins from seeing that the test has raised an error.
    ## @param: test (nose.case.Test) - the test case
    ## @param: err (3-tuple) - sys.exc_info() tuple
    def addError(self, test, err):
        err = self.formatErr(err)
        log.error("Test Case '%s' ERROR!!" % (test))
        log.error("Traceback: %s" % (err))


    ## Redefined: Called when a test fails
    ## DO NOT return a value unless you want to stop other plugins from seeing that the test has failed.
    ## @param: test (nose.case.Test) - the test case
    ## @param: err (3-tuple) - sys.exc_info() tuple
    def addFailure(self, test, err):
        err = self.formatErr(err)
        log.error("Test Case '%s' FAILS!!" % (test))
        log.error("Traceback: %s" % (err))

    def formatErr(self, err):
        exctype, value, tb = err
        return ''.join(traceback.format_exception(exctype, value, tb))
        #self.html.extend([ '<div><span>',
                           #test.shortDescription() or str(test),
                           #'</span>' ])
