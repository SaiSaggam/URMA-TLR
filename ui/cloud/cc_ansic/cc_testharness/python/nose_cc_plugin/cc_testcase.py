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
# Cloud Connector Test Framework - Base class for CLoud Connector testcases
#

################ Import Common libraries
import unittest
import logging          ### Functions and classes to implement a event logging system
import time             ### Provides various time-related functions
import xml.dom.minidom  ### Provides a minimal implementation of the Document Object Model interface
import os               ### Some useful functions on pathnames

from nose.plugins.skip import SkipTest  ### Provides an exception class for nose to Skip a testcase



class BaseCloudConnectorTestCase(unittest.TestCase):

    # Logger handler for class
    log=None

    ## Flag to enable/disable the execution of the tests
    stopAllTests=None


    def __init__(self, name):
        unittest.TestCase.__init__(self, name)

        # Get a different logger for the test case level
        self.log = logging.getLogger('cc.testcase')


    @classmethod
    def setUpClass(cls):
        # Use the same Logger at the class level (optimization).
        if TestCase.log is None:
            TestCase.log = logging.getLogger('cc.testcase.class')


    def setUp(self):
        #** Check stop flag to skip the test
        if(TestCase.stopAllTests):
            raise SkipTest("Test was skipped due to STOP_ALL_TESTS flag is enabled")


    @classmethod
    def tearDownClass(cls):
        pass



    ## Custom function to set a test as failed and prevent the execution of more tests
    def failAndStopAllTests(self, message):
        TestCase.stopAllTests = True
        self.fail(message)


    ## Custom function to manage a test file inside the testcases
    def getTestFile(self, filename):
        # Generate absolute path
        filePath = os.path.join(self.testFilesPath,filename)
        # Read the file
        f = open(filePath, 'rb')
        fileData = f.read()
        f.close()

        # Get file size
        fileSize = os.path.getsize(filePath)

        # Create the file object
        fileObject = {}
        fileObject["data"] = fileData
        fileObject["size"] = fileSize
        fileObject["path"] = filePath
        fileObject["name"] = filename

        # Return the file data
        return fileObject




## Main Class for TCP test cases
class TestCase(BaseCloudConnectorTestCase):

    def setUp(self):
        super(TestCase, self).setUp()

        self.log.info("Ensuring Device %s is connected." % self.device_id)

        isConnected = False
        for _ in xrange(1,5):
            # Make sure that device is connected
            if ( self.cloudHandler.isDeviceConnected(self.device_id) ):
                # Device connected
                isConnected = True
                break
            time.sleep(1.0)

        # If not connected, fail the TestCase.
        self.assertEqual(True, isConnected,
            "Device %s not connected after 5 seconds." % self.device_id)






PING_REQUEST = """<sci_request version="1.0"> 
  <send_message synchronous="true"> 
    <targets> 
      <device id="%s"/> 
    </targets>
    <sm_udp retries="5" retryTimeout="10">
      <ping/>
    </sm_udp>
  </send_message>
</sci_request>"""

## Main Class for UDP test cases
class UDPTestCase(BaseCloudConnectorTestCase):

    def setUp(self):
        super(UDPTestCase, self).setUp()

        self.log.info("Sending Ping Request to Device %s to ensure it is Alive." % self.device_id)

        # Create ping request
        pingRequest = PING_REQUEST % (self.device_id)

        # Send device request
        requestResponse = self.cloudHandler.postRequest("sci", data=pingRequest)

        self.log.info("Response Received: %s" % requestResponse.content)

        # Parse response to determmine whether or not it timed out or
        # was successful.  If successful continue the test.

        # Get xml response in dict format
        dataResponse = requestResponse.resource

        # Get element 'sm_udp'
        pingElement = dataResponse["sci_reply"]["send_message"]["device"]["sm_udp"]["ping"]

        self.assertEqual(None, pingElement,
            "No Ping Response Received. Response : %s" % requestResponse.content)

        self.log.info("Ping response received from device, proceeding.")




## Main Class for SMS test cases
class SMSTestCase(BaseCloudConnectorTestCase):

    def setUp(self):
        super(SMSTestCase, self).setUp()

        self.log.info("TODO: SMS setUp, proceeding.")
