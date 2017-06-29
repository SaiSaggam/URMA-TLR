import time
import ic_testcase
import datetime
import re
import os
from base64 import b64decode

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()

# from ..utils import clean_slate
from ..utils import getText, clean_slate

impl = getDOMImplementation()
class SmUdpDvtTestCase(ic_testcase.UDPTestCase):

    def test_sm_udp_ping(self):
        """ Verifies that the device can be pinged. """
        self.send_sm_udp_request("Ping", "<ping/>", True)

    def test_sm_udp_ping_no_reply(self):
        """ Verifies that the device can be pinged. No reply. """
        self.send_sm_udp_request("Ping", "<ping/>", False)

    def test_sm_udp_user_request(self):
        """ Verifies that the user request with path is processed correctly. """
        self.send_sm_udp_request("User request", '<user_msg path="myTarget">My user request message</user_msg>', True)

    def test_sm_udp_user_request_no_reply(self):
        """ Verifies that the user request with path is processed correctly. No reply. """
        self.send_sm_udp_request("User request", '<user_msg path="myTarget">My user request message</user_msg>', False)

    def test_sm_udp_user_request_no_path(self):
        """ Verifies that the user request without path is processed correctly. """
        self.send_sm_udp_request("User request no path", '<user_msg>My user request message without path</user_msg>', True)

    def test_sm_udp_user_request_no_path_no_reply(self):
        """ Verifies that the user request without path is processed correctly. No reply. """
        self.send_sm_udp_request("User request no path", '<user_msg>My user request message without path</user_msg>', False)

    def test_sm_udp_cli(self):
        """ Verifies that the CLI to the device processed correctly. """
        self.send_sm_udp_request("CLI", '<command> ctime </command>', True)

    def test_sm_udp_cli_no_reply(self):
        """ Verifies that the CLI to the device processed correctly. No reply. """
        self.send_sm_udp_request("CLI", '<command> ctime </command>', False)

    def test_sm_udp_cli_limit_response(self):
        """ Verifies that the CLI with limited response size to the device processed correctly. """
        self.send_sm_udp_request("CLI with size limit", '<command maxResponseSize="10"> ctime </command>', True)

    def test_sm_udp_cli_limit_response_no_reply(self):
        """ Verifies that the CLI with limited response size to the device processed correctly. No reply. """
        self.send_sm_udp_request("CLI with size limit", '<command maxResponseSize="10"> ctime </command>', False)

    def test_sm_udp_connect_request(self):
        """ Verifies that the connect request to the device processed correctly. """
        self.send_sm_udp_request("Connect request", '<request_connect/>', True)

    def test_sm_udp_connect_request_no_reply(self):
        """ Verifies that the connect request to the device processed correctly. No reply. """
        self.send_sm_udp_request("Connect request", '<request_connect/>', False)

    def test_sm_udp_reboot(self):
        """ Verifies that the reboot command to the device processed correctly. """
        self.send_sm_udp_request("Reboot", '<reboot/>', True)

    def test_sm_udp_reboot_no_reply(self):
        """ Verifies that the reboot command to the device processed correctly. No reply. """
        self.send_sm_udp_request("Reboot", '<reboot/>', False)

    def test_sm_udp_verify_send_data(self):
        file_path = 'FileData/~/' + self.device_id + '/test/sm_udp.txt'
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

if __name__ == '__main__':
    unittest.main() 
