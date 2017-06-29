# -*- coding: utf-8 -*-
import time
import ic_testcase
import datetime
import re
import os
from base64 import b64encode

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()

from ..utils import clean_slate, DeviceConnectionMonitor, getText, get_and_verify

filedata = 'FileData/~/'
TERMINATE_TEST_FILE = "terminate_test.txt"

impl = getDOMImplementation()

def delete_file(self, file_name):

    # Create paths to delete the file.
    file_location = filedata + self.device_id + '/' + file_name

    self.log.info("delete file %s" % file_location)
    # If the file exists, delete it.
    clean_slate(self.rest_session, file_location)

class StopStartInitiateActionDvtTestCase(ic_testcase.TestCase):
    monitor=None

    def setUp(self):
        ic_testcase.TestCase.setUp(self)
        # Optimization, reuse the DeviceConnectionMonitor to avoid creating
        # multiple sessions over and over.
        if StopStartInitiateActionDvtTestCase.monitor is None:
            StopStartInitiateActionDvtTestCase.monitor = DeviceConnectionMonitor(self.push_client, self.dev_id)
            StopStartInitiateActionDvtTestCase.monitor.start()
        self.monitor = StopStartInitiateActionDvtTestCase.monitor

    @classmethod
    def tearDownClass(cls):
        if StopStartInitiateActionDvtTestCase.monitor is not None:
            StopStartInitiateActionDvtTestCase.monitor.stop()
        ic_testcase.TestCase.tearDownClass()


    def test_terminate_request(self):
        """ Initiate [terminate_idigi] device request. """

        # delete the file that sample is going to send for
        # the reboot result.
        delete_file(self, TERMINATE_TEST_FILE)

        self.send_device_request("terminate_connector", False, False)

        # Wait for terminate result file.
        # Create content that are expected from the reboot result file.
        expected_content = "terminate_ok"
        datetime_created = []

        # Create path to file for push.
        file_push_location = filedata + self.device_id + '/' + TERMINATE_TEST_FILE

        # get and verify correct content is pushed.
        get_and_verify(self, self.push_client, self.device_id,
                       file_push_location, expected_content)
#                       trigger_function=send_data)


    def send_device_request(self, target, wait_for_response, wait_for_disconnect):

        my_target_device_request = \
            """<sci_request version="1.0">
              <data_service>
                <targets>
                  <device id="%s"/>
                </targets>
                <requests>
                <device_request target_name="%s">TCP</device_request>
                </requests>
              </data_service>
            </sci_request>""" % (self.device_id, target)

        self.log.info("Beginning sendng target[%s]" %target)
        self.log.info("Sending device request for \"%s\" target_name to server for device id  %s." % (target, self.device_id))

        # Send device request
        device_request_response = self.session.post('http://%s/ws/sci' % self.hostname,
                            data=my_target_device_request).content

        self.log.info("Got Response: %s" % device_request_response)
        dom = xml.dom.minidom.parseString(device_request_response)
        device_response = dom.getElementsByTagName("device_request")

        if len(device_response) > 0:
            self.log.info("Response for \"%s\" target: %s" % (target, device_request_response))

        if wait_for_response:
            # Parse request response
            self.log.info("Got Response: %s" % device_request_response)
            dom = xml.dom.minidom.parseString(device_request_response)
            device_response = dom.getElementsByTagName("device_request")

            if len(device_response) == 0:
                self.fail("Unexpected response from device: %s" % device_request_response)

            # Validate target name
            self.log.info("Determining if the target_name is \"%s\"." %target)
            target_name = device_response[0].getAttribute('target_name')
            self.assertEqual(target_name, target,
                             "returned target (%s) is not (%s)"
                             % (target_name, target))

            # Validate status
            self.log.info("Determining if status is success.")
            status = device_response[0].getAttribute("status")
            self.assertEqual(status, '0', "returned error status(%s)" %status)

        if wait_for_disconnect:
            print "wait for disconnect = %d" % wait_for_disconnect
            self.log.info("Waiting for Device Cloud to disconnect device.")
            self.monitor.wait_for_disconnect(30)
            self.log.info("Device disconnected.")

            self.log.info("Waiting for Device to reconnect.")
            self.monitor.wait_for_connect(30)
            self.log.info("Device connected.")

if __name__ == '__main__':
    unittest.main()
