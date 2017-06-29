# -*- coding: iso-8859-1 -*-
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

TARGET_DEVICE_REQUEST = \
"""<sci_request version="1.0">
  <data_service>
    <targets>
      <device id="%s"/>
    </targets>
    <requests>
    <device_request target_name="%s">%s</device_request>
    </requests>
  </data_service>
</sci_request>"""

RCI_QUERY_STATE = \
"""<sci_request version="1.0">
  <send_message cache="false">
    <targets>
      <device id="%s"/>
    </targets>
    <rci_request version="1.1">
      <query_state/>
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


    def test_stacksize_with_begin_rci(self):

        """ Sends query_setting.
        """
        self.log.info("**** Beginnin Stack Size DS Test:  start RCI query_state")
        # Get the current value

        rci_request = (RCI_QUERY_STATE % self.device_id);

        # Send RCI request
        rci_response = self.session.post('http://%s/ws/sci' % self.hostname, data=rci_request).content

        self.log.info('Response from query state: %s' %rci_response)

        # Parse request response
        dom = xml.dom.minidom.parseString(rci_response)
        rci_error_response = dom.getElementsByTagName('error')

        if len(rci_error_response) != 0:
            self.log.info("Request: %s" % rci_request)
            self.log.info("Response: %s" % rci_response)
            self.fail("Got error response: '%s'" % rci_response)

    def test_stacksize_with_ds(self):

        """ Sends device request.
        """
        self.log.info("**** Beginning Stack Size DVT:  Device request")
        self.valid_target("ds_stacksize_test", 2048)
        self.valid_target("put_ds_stacksize_test", 100)


        if device_is_connected(self) == False:
            self.log.info("Waiting for device to connect before start testing")
            self.monitor.wait_for_connect(30)
        self.log.info("Device is connected. Start testing")

        # Send device request to request terminate the sample
        device_request_response = self.send_device_request('request_terminate', 0)
        self.log.info("RESPONSE %s" %device_request_response)

        self.log.info("Waiting for cloud to disconnect device.")
        self.monitor.wait_for_disconnect(30)
        self.log.info("Device disconnected.")

        self.log.info("Waiting for Device to reconnect.")
        self.monitor.wait_for_connect(30)
        self.log.info("Device connected.")



    def invalid_target(self, my_target_name, data_len):

        """ Verifies that the device request returned match the expected
        response.
        """

        # Send device request
        device_request_response = self.send_device_request(my_target_name, data_len)
        self.log.info("RESPONSE %s" %device_request_response)
        if device_request_response.find('error id="2108"') != -1:
            self.log.info("Service not available.")
            return "no_service"

        # Parse request response
        dom = xml.dom.minidom.parseString(device_request_response)
        device_response = dom.getElementsByTagName("device_request")

        # Validate cancel message response
        self.log.info("Determining if \"%s\" target is cancelled." % my_target_name)
        error = getText(device_response[0].getElementsByTagName('error')[0])

        cancelled_response = "Message transmission cancelled"

        self.assertEqual(error, cancelled_response,
            "returned target (%s) is not (%s)"
            % (error, cancelled_response))
        return "ok"

    def valid_target(self, my_target_name, data_len):

        """ Verifies that the device request returned match the expected
        response.
        """
        #if DS_SERVICE == 0:
        #    return

        device_request_response = self.send_device_request(my_target_name, data_len)
        #self.log.info("RESPONSE: %s " % device_request_response)
        if device_request_response.find('error id="2108"') != -1:
            self.log.info("Service not available.")
            return "no_service"

        # Parse request response
        dom = xml.dom.minidom.parseString(device_request_response)
        device_response = dom.getElementsByTagName("device_request")

        # Validate target name
        self.log.info("Determining if the target_name is \"%s\"." % my_target_name)
        target_name = device_response[0].getAttribute('target_name')
        self.log.info("Target: %s " % target_name)
        self.assertEqual(target_name, my_target_name,
            "returned target (%s) is not (%s)"
            % (target_name, my_target_name))

        # Validate status
        self.log.info("Determining if status is success.")
        status = device_response[0].getAttribute("status")
        self.assertEqual(status, '0',
            "returned status (%s) is not success status"
            % status)
        return "ok"

    def send_device_request(self, target_name, request_length):

        request_data = self.get_random_word(request_length);

        request = (TARGET_DEVICE_REQUEST % (self.device_id, target_name, request_data))

        self.log.info("Sending device request for \"%s\" target_name to server for device id  %s." % (target_name, self.device_id))
        device_request_response = self.session.post('http://%s/ws/sci' % self.hostname, data=request, timeout=120).content

        return device_request_response

    def get_random_word(self, wordLen):
        word = ''
        for i in range(wordLen):
            word += random.choice('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789')
        return word

    def parse_error(doc):
        errors = xpath.find('//error',doc)
        if len(errors) > 0:
            error = errors[0]

            desc = xpath.find('desc/text()', error)
            if len(desc) > 0:
                desc = desc[0].data

            hint = xpath.find('hint/text()', error)
            if len(hint) > 0:
                hint = hint[0].data

            return (error.getAttribute('id'), desc, hint)
        return None

if __name__ == '__main__':
    unittest.main()
