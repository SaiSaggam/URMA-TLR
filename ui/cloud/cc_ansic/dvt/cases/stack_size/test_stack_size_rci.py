import xpath
import time
import ic_testcase
import datetime
import re
import os
import fileinput
import random
import base64
import tempfile

from base64 import b64encode, b64decode, encodestring

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()

from ..utils import getText
from ..utils import DeviceConnectionMonitor, device_is_connected

RCI_QUERY_SETTING = \
"""<sci_request version="1.0"> 
  <send_message cache="false"> 
    <targets> 
      <device id="%s"/> 
    </targets> 
    <rci_request version="1.1"> 
      <query_setting><system><description/></system></query_setting>
    </rci_request>
  </send_message>
</sci_request>"""

RCI_SET_SETTING = \
"""<sci_request version="1.0"> 
  <send_message cache="false"> 
    <targets> 
      <device id="%s"/> 
    </targets> 
    <rci_request version="1.1"> 
      <set_setting><system><description>%s</description></system></set_setting>
    </rci_request>
  </send_message>
</sci_request>"""

class StackSizeTestCase(ic_testcase.TestCase):
    monitor=None

    def setUp(self):
        ic_testcase.TestCase.setUp(self)
        # Optimization, reuse the DeviceConnectionMonitor to avoid creating
        # multiple sessions over and over.
        if StackSizeTestCase.monitor is None:
            StackSizeTestCase.monitor = DeviceConnectionMonitor(self.push_client, self.dev_id)
            StackSizeTestCase.monitor.start()
        self.monitor = StackSizeTestCase.monitor

    @classmethod
    def tearDownClass(cls):
        if StackSizeTestCase.monitor is not None:
            StackSizeTestCase.monitor.stop()
        ic_testcase.TestCase.tearDownClass()


    def test_stacksize_with_rci_query_setting(self):

        """ Sends query_setting. 
        """
        self.log.info("**** Stack Size (RCI) Test:  RCI query_setting")
        # Get the current value

        rci_request = (RCI_QUERY_SETTING % self.device_id);

        # Send RCI request
        rci_response = self.session.post('http://%s/ws/sci' % self.hostname, data=rci_request).content

        self.log.info('Response from query setting: %s' %rci_response)

        # Parse request response 
        dom = xml.dom.minidom.parseString(rci_response)
        rci_error_response = dom.getElementsByTagName('error')

        if len(rci_error_response) != 0:
            self.log.info("Request: %s" % rci_request)
            self.log.info("Response: %s" % rci_response)
            self.fail("Got error response: '%s'" % rci_response)

    def test_stacksize_with_rci_set_setting(self):

        """ Sends set_setting. 
        """
        self.log.info("**** Stack Size (RCI) Test:  RCI set_setting")
        # Get the current value

        rci_request = (RCI_SET_SETTING % (self.device_id, "ic_stacksize DVT testing"));

        # Send RCI request
        rci_response = self.session.post('http://%s/ws/sci' % self.hostname, data=rci_request).content

        self.log.info('Response from set setting: %s' %rci_response)

        # Parse request response 
        dom = xml.dom.minidom.parseString(rci_response)
        rci_error_response = dom.getElementsByTagName('error')
    
        if len(rci_error_response) != 0:
            self.log.info("Request: %s" % rci_request)
            self.log.info("Response: %s" % rci_response)
            self.fail("Got error response: '%s'" % rci_response)


        if device_is_connected(self) == False:
            self.log.info("Waiting for device to connect before start testing")
            self.monitor.wait_for_connect(30)
        self.log.info("Device is connected. Start testing")

        rci_request = (RCI_SET_SETTING % (self.device_id, "terminate"));

        # Send RCI request
        rci_response = self.session.post('http://%s/ws/sci' % self.hostname, data=rci_request).content

        self.log.info("Waiting for iDigi to disconnect device.")
        self.monitor.wait_for_disconnect(30)
        self.log.info("Device disconnected.")

        self.log.info("Waiting for Device to reconnect.")
        self.monitor.wait_for_connect(30)
        self.log.info("Device connected.")


if __name__ == '__main__':
    unittest.main()
