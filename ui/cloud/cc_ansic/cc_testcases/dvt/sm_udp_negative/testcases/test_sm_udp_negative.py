# -*- coding: utf-8 -*-
import time
import cc_testcase
import datetime
import re
import os
import xml.dom.minidom

class SmUdpDvtTestCase(cc_testcase.UDPTestCase):

    def test_first_sm_UDP_ping_no_reply(self):
        
        self.cloudHandler.updateBatteryOperatedSupport(self.device_id,True)

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

        self.cloudHandler.updateBatteryOperatedSupport(self.device_id,False)

    def test_sm_udp_verify_send_data(self):
        file_path = 'FileData/~/' + self.device_id + '/test/sm_udp_neg.txt'
        expected_content = "iDigi sm udp dvt for device data\n"
       
        requestResponse = self.cloudHandler.getRequest(file_path)
        file_content = requestResponse.content

        self.assertEqual(expected_content, file_content, "File's contents do not match what is expected")

        requestResponse = self.cloudHandler.deleteRequest(file_path)

    def send_sm_udp_request (self, test_name, command, reply_needed):

        my_sm_request_with_reply = \
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

        my_sm_request_no_reply = \
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
            my_sm_request = my_sm_request_with_reply
        else:
            my_sm_request = my_sm_request_no_reply

        self.log.info("Sending SM UDP command \"%s\" to server for device id  %s." % (test_name, self.device_id))

        self.log.info("Sending SCI Request:\n%s\n" % my_sm_request)

        # Send device request
        requestResponse = self.cloudHandler.postRequest("sci", data=my_sm_request)

        # Check query was successfully
        if (requestResponse.status_code != 200):
            self.fail("Unexpected response from the Cloud")

        # Get xml response in dict format
        dataResponse = requestResponse.resource

        # Show response in xml format
        self.log.info("Got Response:\n%s" % xml.dom.minidom.parseString(requestResponse.content).toprettyxml(indent="  ") )

        if reply_needed == True:
            # Get element 'sm_udp'
            element = dataResponse["sci_reply"]["send_message"]["device"]["sm_udp"]
        else:
            # Get element 'send_message'
            element = dataResponse["sci_reply"]["send_message"]
        self.log.info("element: %s" % element)

if __name__ == '__main__':
    unittest.main()
