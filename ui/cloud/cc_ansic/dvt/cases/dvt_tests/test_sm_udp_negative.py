# -*- coding: utf-8 -*-
import time
import ic_testcase
import datetime
import re
import os
from base64 import b64decode
from requests import sessions
from idigi_rest_api import rest_mapper

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()

# from ..utils import clean_slate
from ..utils import getText, clean_slate

impl = getDOMImplementation()
class SmUdpDvtTestCase(ic_testcase.UDPTestCase):

    def test_first_sm_UDP_ping_no_reply(self):
        self.set_battery_mode('true')
        """ Verifies that the device can be pinged. No reply. """
        self.send_sm_udp_request("Ping", "<ping/>", False)

    def test_sm_UDP_user_request_no_reply(self):
        """ Verifies that the user request with path is processed correctly. No reply. """
        self.send_sm_udp_request("User request", '<user_msg path="myTarget">My user request message</user_msg>', False)

    def test_sm_UDP_user_request_no_path_no_reply(self):
        """ Verifies that the user request without path is processed correctly. No reply. """
        self.send_sm_udp_request("User request no path", '<user_msg>My user request message without path</user_msg>', False)

    def test_sm_UDP_cli_no_reply(self):
        """ Verifies that the CLI to the device processed correctly. No reply. """
        self.send_sm_udp_request("CLI", '<command> ctime </command>', False)

    def test_sm_UDP_ping(self):
        """ Verifies that the device can be pinged. """
        self.send_sm_udp_request("Ping", "<ping/>", True)

    def test_sm_UDP_user_request(self):
        """ Verifies that the user request with path is processed correctly. """
        self.send_sm_udp_request("User request", '<user_msg path="myTarget">My user request message</user_msg>', True)

    def test_sm_UDP_user_request_no_path(self):
        """ Verifies that the user request without path is processed correctly. """
        self.send_sm_udp_request("User request no path", '<user_msg>My user request message without path</user_msg>', True)

    def test_sm_UDP_cli(self):
        """ Verifies that the CLI to the device processed correctly. """
        self.send_sm_udp_request("CLI", '<command> ctime </command>', True)

    def test_sm_UDP_cli_limit_response(self):
        """ Verifies that the CLI with limited response size to the device processed correctly. """
        self.send_sm_udp_request("CLI with size limit", '<command maxResponseSize="10"> ctime </command>', True)

    def test_sm_UDP_cli_limit_response_no_reply(self):
        """ Verifies that the CLI with limited response size to the device processed correctly. No reply. """
        self.send_sm_udp_request("CLI with size limit", '<command maxResponseSize="10"> ctime </command>', False)

    def test_sm_UDP_connect_request(self):
        """ Verifies that the connect request to the device processed correctly. """
        self.send_sm_udp_request("Connect request", '<request_connect/>', True)

    def test_sm_UDP_connect_request_no_reply(self):
        """ Verifies that the connect request to the device processed correctly. No reply. """
        self.send_sm_udp_request("Connect request", '<request_connect/>', False)

    def test_sm_UDP_reboot(self):
        """ Verifies that the reboot command to the device processed correctly. """
        self.send_sm_udp_request("Reboot", '<reboot/>', True)

    def test_sm_udp_reboot_no_reply(self):
        """ Verifies that the reboot command to the device processed correctly. No reply. """
        self.send_sm_udp_request("Reboot", '<reboot/>', False)
        self.set_battery_mode('false')

    def test_sm_udp_verify_send_data(self):
        file_path = 'FileData/~/' + self.device_id + '/test/sm_udp_neg.txt'
        expected_content = "iDigi sm udp dvt for device data\n"
        file_content = self.session.get('http://%s/ws/%s' % (self.hostname, file_path)).content
        self.assertEqual(expected_content, file_content, "File's contents do not match what is expected")
        self.rest_session.delete(file_path)

    def send_sm_udp_request (self, test_name, command, reply_needed):

        my_sm_udp_request_with_reply = \
            """<sci_request version="1.0">
              <send_message>
                <targets>
                  <device id="%s"/>
                </targets>
                <sm_udp retries="5" retryTimeout="10">
                  %s
                </sm_udp>
              </send_message>
            </sci_request>""" % (self.device_id, command)

        my_sm_udp_request_no_reply = \
            """<sci_request version="1.0">
              <send_message reply="none">
                <targets>
                  <device id="%s"/>
                </targets>
                <sm_udp retries="5" retryTimeout="10">
                  %s
                </sm_udp>
              </send_message>
            </sci_request>""" % (self.device_id, command)

        if reply_needed == True:
            my_sm_udp_request = my_sm_udp_request_with_reply
        else:
            my_sm_udp_request = my_sm_udp_request_no_reply

        self.log.info("Sending SM UDP command \"%s\" to server for device id  %s." % (test_name, self.device_id))

        self.log.info("Sending SCI Request:\n%s\n" % my_sm_udp_request)

        # Send device request
        sm_udp_response = self.session.post('http://%s/ws/sci' % self.hostname, data=my_sm_udp_request).content

        # Parse request response 
        self.log.info("Got Response: %s" % sm_udp_response)
        dom = xml.dom.minidom.parseString(sm_udp_response)
        if reply_needed == True:
            sm_response = dom.getElementsByTagName("sm_udp")
        else:
            sm_response = dom.getElementsByTagName("send_message")

        if len(sm_response) == 0:
            self.fail("Unexpected response from device: %s" % sm_udp_response)

    def set_battery_mode (self, mode):
        # retrieve the dev_id for the Device
        dev_id = self.rest_session.get_first('DeviceCore', params={'condition': "devConnectwareId='%s'" % self.device_id}).resource.id.devId
        # create DeviceCore resource with battery operated set to true
        device_core = rest_mapper.create_resource('DeviceCore', dpSmBatteryOperated=mode)
        # update the resource
        self.rest_session.put(device_core, dev_id)

if __name__ == '__main__':
    unittest.main()
