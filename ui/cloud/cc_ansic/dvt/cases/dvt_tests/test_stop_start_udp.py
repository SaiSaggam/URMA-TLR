# -*- coding: utf-8 -*-
import time
import ic_testcase
import datetime
import re
import os
from base64 import b64encode, b64decode

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()

from ..utils import clean_slate, DeviceConnectionMonitor, getText, get_and_verify, monitor_and_return, device_is_connected

filedata = 'FileData/~/'
TERMINATE_TEST_FILE = "terminate_test.txt"
TRANSPORT_ERROR_FILE = "transport_error.txt"

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

        delete_file(self, TRANSPORT_ERROR_FILE)


    @classmethod
    def tearDownClass(cls):
        if StopStartInitiateActionDvtTestCase.monitor is not None:
            StopStartInitiateActionDvtTestCase.monitor.stop()
        ic_testcase.TestCase.tearDownClass()

    def test_app_a_start_initiate(self):
        """ Initiate [application_start_connector] device request. """

        self.tcp_device_request("application_start_connector", "TCP", False)
        self.tcp_device_request("application_start_connector", "UDP", False)
        if device_is_connected(self) == False:
            self.wait_for_connect()
        self.log.info("TCP connected")

        ping_response = self.udp_ping();
        self.assertEqual(ping_response, None, "Error in ping: %s" %ping_response)


    def test_app_b_stop_initiate(self):
        """ Initiate [application_stop_idigi] device request. """
        self.tcp_device_request("application_stop_connector", "TCP", False)
        self.log.info("Waiting for iDigi to disconnect device.")
        self.wait_for_disconnect()

        self.udp_device_request("application_start_connector", "TCP", True)
        self.log.info("Waiting for iDigi to connect device.")
        self.wait_for_connect()

    def test_stop_c_all_initiate(self):
        """ Initiate [stop_all_transports] device request. """
        self.tcp_device_request("stop_all_transports", "TCP", False)
        self.wait_for_disconnect()
        self.wait_for_connect()

    def test_stop_d_initiate(self):
        """ Initiate [stop_connector] device request. """
        self.tcp_device_request("stop_idigi", "TCP", False)
        self.wait_for_disconnect()

        self.udp_device_request("application_start_connector", "TCP", True)
        self.wait_for_connect()

    def test_stop_e_terminate(self):
        """ Initiate [Stop iDigi + terminate] device request. """

        # delete the file that sample is going to send for
        # the reboot result.
        delete_file(self, TERMINATE_TEST_FILE)

        self.tcp_device_request("stop_terminate_connector", "TCP", False)

        self.wait_for_disconnect()

        # Wait for terminate result file.
        # Create content that are expected from the reboot result file.
        expected_content = "terminate_ok"
        datetime_created = []

        # Create path to file for push.
        file_push_location = filedata + self.device_id + '/' + TERMINATE_TEST_FILE

        # Just do push monitor subscription. It wont assert
        # if the file doesn't exist. It will return the content
        # of the file.

        get_and_verify(self, self.push_client, self.device_id,
                       file_push_location, expected_content)
#                       trigger_function=send_data)

        self.log.info("Monitor again %s" %file_push_location)
        # get and verify correct content is pushed.
        content = monitor_and_return(self, self.push_client, self.device_id,
                       file_push_location, expected_content)
#                       trigger_function=send_data)

        if content is not None:
            self.log.info("**********************************")
            self.log.info("2nd Monitor Subscription was expected to return None but it returned %s" % content)
            self.assertEqual(content, None,
                             "Expected None but it is (%s)" % content)

    def test_stop_f_transport_file(self):
        """ Check transport error file. """

        # Wait for terminate result file.
        # Create content that are expected from the reboot result file.
        expected_content = "<TCP>0</TCP><ALL>0</ALL>"
        datetime_created = []

        # Create path to file for push.
        file_push_location = filedata + self.device_id + '/' + TRANSPORT_ERROR_FILE

        # Just do push monitor subscription. It wont assert
        # if the file doesn't exist. It will return the content
        # of the file.

        get_and_verify(self, self.push_client, self.device_id,
                       file_push_location, expected_content)
#                       trigger_function=send_data)

        self.log.info("Monitor again %s" %file_push_location)
        # get and verify correct content is pushed.
        content = monitor_and_return(self, self.push_client, self.device_id,
                       file_push_location, expected_content)
#                       trigger_function=send_data)

        if content is not None:
            self.log.info("**********************************")
            self.log.info("2nd Monitor Subscription was expected to return None but it returned %s" % content)
            self.assertEqual(content, None,
                             "Expected None but it is (%s)" % content)

    def udp_ping(self):
        message = """<sci_request version="1.0">
            <send_message>
                <targets>
                    <device id="%s"/>
                </targets>
                <sm_udp retries="3" retryTimeout="5">
                <ping/>
                </sm_udp>
            </send_message>
        </sci_request>
        """%(self.device_id)

        self.log.info("Sending ping to device id  %s." % self.device_id)
        self.log.info("Message: %s" % message)

        # Send device request
        response = self.session.post('http://%s/ws/sci' % self.hostname,
                            data=message).content

        ping_result = None

        self.log.info("Response: %s" % response)
        # print the output to standard out
        dom = xml.dom.minidom.parseString(response)
        ping_response = getText(dom.getElementsByTagName("ping")[0]);
        self.log.info("Response: %d" % len(ping_response))
        self.log.info("Response: %s" % ping_response)
        if len(ping_response) > 0:
            ping_result = ping_response
        else:
            self.log.info("Ping success")

        return ping_result

    def udp_device_request(self, target, data, wait_for_response):

        if wait_for_response:
            reply = "all"
        else:
            reply = "none"

        message = """<sci_request version="1.0">
            <send_message reply="%s">
                <targets>
                    <device id="%s"/>
                </targets>
                <sm_udp retries="3" retryTimeout="5">
                <user_msg path="%s">%s</user_msg>
                </sm_udp>
            </send_message>
        </sci_request>
        """%(reply, self.device_id, target, data)

        self.log.info("Beginning sendng target[%s]" %target)
        self.log.info("Sending device request for \"%s\" target_name to server for device id  %s." % (target, self.device_id))

        self.log.info("Message: %s" % message)

        # Send device request
        device_request_response = self.session.post('http://%s/ws/sci' % self.hostname,
                            data=message).content

        self.log.info("Got Response: %s" % device_request_response)
        dom = xml.dom.minidom.parseString(device_request_response)
        device_response = dom.getElementsByTagName("user_msg")

        if wait_for_response:
            # Parse request response

            if len(device_response) == 0:
                self.fail("Unexpected response from device: %s" % device_request_response)

            # Validate target name
#            self.log.info("Determining if the target_name is \"%s\"." %target)
#            target_name = device_response[0].getAttribute('target_name')
#            self.assertEqual(target_name, target,
#                             "returned target (%s) is not (%s)"
#                             % (target_name, target))

            # Validate status
#            self.log.info("Determining if status is success.")
#            status = device_response[0].getAttribute("status")
#            self.assertEqual(status, '0', "returned error status(%s)" %status)

            response_data = b64decode(getText(device_response[0]))
            self.log.info("Response data: %s" % response_data)
            if response_data == "Initiate action done":
                self.log.info("Response data: %s" % response_data)
            else:
                self.fail("Unexpected response from device: %s" % response_data)


    def tcp_device_request(self, target, data, wait_for_response):

        my_target_device_request = \
            """<sci_request version="1.0">
              <data_service>
                <targets>
                  <device id="%s"/>
                </targets>
                <requests>
                <device_request target_name="%s">%s</device_request>
                </requests>
              </data_service>
            </sci_request>""" % (self.device_id, target, data)

        self.log.info("Beginning sendng target[%s]" %target)
        self.log.info("Sending device request for \"%s\" target_name to server for device id  %s." % (target, self.device_id))
        self.log.info("Message: %s" % my_target_device_request)

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

    def wait_for_disconnect(self):
            self.log.info("Waiting for iDigi to disconnect device.")
            self.monitor.wait_for_disconnect(60)
            self.log.info("Device disconnected.")

    def wait_for_connect(self):
            self.log.info("Waiting for Device to reconnect.")
            self.monitor.wait_for_connect(60)
            self.log.info("Device connected.")

if __name__ == '__main__':
    unittest.main()
