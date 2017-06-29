# -*- coding: utf-8 -*-
import time
import cc_testcase
import datetime
import re
import os
import xml.dom.minidom

# timeout used by sci_request
timeout_sci_request = 900

# timeouts used by readUntilPattern function for consoleFeedback
timeout_reply_yes =15   # A low value as the SMS has already been send to the device and answered to the server
timeout_reply_no = (timeout_sci_request/2)  # As we din't wait for a reply to the server, we have to wait the time an SMS can be delayed 


class SmSmsDvtTestCase(cc_testcase.SMSTestCase):
    sendDataArrived = 0

    def test_first_sm_SMS_ping_no_reply(self):
        """ Verifies that the device can be pinged. No reply. """
        self.send_sm_sms_request("Ping", "<ping/>", False)

        status, index, buffer = self.deviceHandler.readUntilPattern ( pattern="Received ping request. response is not needed", timeout=timeout_reply_no)
        self.checkSendMessage(buffer)
        self.log.info("Console feedback=%d" % status)
        if status == False:
            self.failAndStopAllTests("Console feedback NOT arrived")

    def test_sm_SMS_user_request_no_reply(self):
        """ Verifies that the user request with path is processed correctly. No reply. """
        self.send_sm_sms_request("User request", '<user_msg path="myTarget">My user request message</user_msg>', False)

        status, index, buffer = self.deviceHandler.readUntilPattern ( pattern='Data: "My user request message"', timeout=timeout_reply_no)
        self.checkSendMessage(buffer)
        self.log.info("Console feedback=%d" % status)
        if status == False:
            self.failAndStopAllTests("Console feedback NOT arrived")

    def test_sm_SMS_user_request_no_path_no_reply(self):
        """ Verifies that the user request without path is processed correctly. No reply. """
        self.send_sm_sms_request("User request no path", '<user_msg>My user request message without path</user_msg>', False)

        status, index, buffer = self.deviceHandler.readUntilPattern ( pattern='Data: "My user request message without path"', timeout=timeout_reply_no)
        self.checkSendMessage(buffer)
        self.log.info("Console feedback=%d" % status)
        if status == False:
            self.failAndStopAllTests("Console feedback NOT arrived")

    def test_sm_SMS_cli_no_reply(self):
        """ Verifies that the CLI to the device processed correctly. No reply. """
        self.send_sm_sms_request("CLI", '<command> ctime </command>', False)

        status, index, buffer = self.deviceHandler.readUntilPattern ( pattern='Executing ctime.', timeout=timeout_reply_no)
        self.checkSendMessage(buffer)
        self.log.info("Console feedback=%d" % status)
        if status == False:
            self.failAndStopAllTests("Console feedback NOT arrived")

    def test_sm_SMS_ping(self):
        """ Verifies that the device can be pinged. """
        self.send_sm_sms_request("Ping", "<ping/>", True)

        status, index, buffer = self.deviceHandler.readUntilPattern ( pattern="Received ping request. response is needed", timeout=timeout_reply_yes)
        self.checkSendMessage(buffer)
        self.log.info("Console feedback=%d" % status)
        if status == False:
            self.failAndStopAllTests("Console feedback NOT arrived")

    def test_sm_SMS_user_request(self):
        """ Verifies that the user request with path is processed correctly. """
        self.send_sm_sms_request("User request", '<user_msg path="myTarget">My user request message</user_msg>', True)

        status, index, buffer = self.deviceHandler.readUntilPattern ( pattern='Data: "My user request message"', timeout=timeout_reply_yes)
        self.checkSendMessage(buffer)
        self.log.info("Console feedback=%d" % status)
        if status == False:
            self.failAndStopAllTests("Console feedback NOT arrived")
		
    def test_sm_SMS_user_request_no_path(self):
        """ Verifies that the user request without path is processed correctly. """
        self.send_sm_sms_request("User request no path", '<user_msg>My user request message without path</user_msg>', True)

        status, index, buffer = self.deviceHandler.readUntilPattern ( pattern='Data: "My user request message without path"', timeout=timeout_reply_yes)
        self.checkSendMessage(buffer)
        self.log.info("Console feedback=%d" % status)
        if status == False:
            self.failAndStopAllTests("Console feedback NOT arrived")

    def test_sm_SMS_cli(self):
        """ Verifies that the CLI to the device processed correctly. """
        self.send_sm_sms_request("CLI", '<command> ctime </command>', True)

        status, index, buffer = self.deviceHandler.readUntilPattern ( pattern='Executing ctime.', timeout=timeout_reply_yes)
        self.checkSendMessage(buffer)
        self.log.info("Console feedback=%d" % status)
        if status == False:
            self.failAndStopAllTests("Console feedback NOT arrived")

    def test_sm_SMS_cli_limit_response(self):
        """ Verifies that the CLI with limited response size to the device processed correctly. """
        self.send_sm_sms_request("CLI with size limit", '<command maxResponseSize="10"> ctime </command>', True)

        status, index, buffer = self.deviceHandler.readUntilPattern ( pattern='Executing ctime.', timeout=timeout_reply_yes)
        self.checkSendMessage(buffer)
        self.log.info("Console feedback=%d" % status)
        if status == False:
            self.failAndStopAllTests("Console feedback NOT arrived")

    def test_sm_SMS_cli_limit_response_no_reply(self):
        """ Verifies that the CLI with limited response size to the device processed correctly. No reply. """
        self.send_sm_sms_request("CLI with size limit", '<command maxResponseSize="10"> ctime </command>', False)

        status, index, buffer = self.deviceHandler.readUntilPattern ( pattern='Executing ctime.', timeout=timeout_reply_no)
        self.checkSendMessage(buffer)
        self.log.info("Console feedback=%d" % status)
        if status == False:
            self.failAndStopAllTests("Console feedback NOT arrived")

    def test_sm_SMS_connect_request(self):
        """ Verifies that the connect request to the device processed correctly. """
        self.send_sm_sms_request("Connect request", '<request_connect/>', True)

    def test_sm_SMS_connect_request_no_reply(self):
        """ Verifies that the connect request to the device processed correctly. No reply. """
        self.send_sm_sms_request("Connect request", '<request_connect/>', False)

    def test_sm_SMS_reboot(self):
        """ Verifies that the reboot command to the device processed correctly. """
        self.send_sm_sms_request("Reboot", '<reboot/>', True)

        status, index, buffer = self.deviceHandler.readUntilPattern ( pattern="app_os_reboot!", timeout=timeout_reply_yes)
        self.checkSendMessage(buffer)
        self.log.info("Console feedback for Reboot=%d" % status)
        if status == False:
            self.failAndStopAllTests("Console feedback for Reboot NOT arrived")

    def test_sm_sms_reboot_no_reply(self):
        """ Verifies that the reboot command to the device processed correctly. No reply. """
        self.send_sm_sms_request("Reboot", '<reboot/>', False)

        status, index, buffer = self.deviceHandler.readUntilPattern ( pattern="app_os_reboot!", timeout=timeout_reply_no)
        self.checkSendMessage(buffer)
        self.log.info("Console feedback for Reboot=%d" % status)
        if status == False:
            self.failAndStopAllTests("Console feedback for Reboot NOT arrived")
        
    def test_sm_sms_verify_send_data(self):
        file_path = 'FileData/~/' + self.device_id + '/test/sm_sms_neg.txt'
        expected_content = "iDigi sm sms dvt for device data\n"

        self.log.info("Initial sendDataArrived=%d" % SmSmsDvtTestCase.sendDataArrived)

        if(SmSmsDvtTestCase.sendDataArrived==0):
            status, index, buffer = self.deviceHandler.readUntilPattern ( pattern="Received success response from server", timeout=900)
            self.log.info("Console feedback for verify_send_data=%d" % status)
            if status == False:
                self.failAndStopAllTests("Console feedback for verify_send_data NOT arrived")	

        requestResponse = self.cloudHandler.getRequest(file_path)
        file_content = requestResponse.content

        self.log.info("file_content requestResponse: %s" % requestResponse)
        self.log.info("file_content: \n%s" % file_content)

        self.assertEqual(expected_content, file_content, "File's contents do not match what is expected")

        requestResponse = self.cloudHandler.deleteRequest(file_path)

    def send_sm_sms_request (self, test_name, command, reply_needed):

        my_sm_request_with_reply = \
            """            <sci_request version="1.0">
              <send_message synchronous="true" syncTimeout="%d">
                <targets>
                  <device id="%s"/>
                </targets>
                <sms>
                  %s
                </sms>
              </send_message>
            </sci_request>""" % (timeout_sci_request,self.device_id, command)

        my_sm_request_no_reply = \
            """            <sci_request version="1.0">
              <send_message reply="none">
                <targets>
                  <device id="%s"/>
                </targets>
                <sms>
                  %s
                </sms>
              </send_message>
            </sci_request>""" % (self.device_id, command)

        if reply_needed == True:
            my_sm_request = my_sm_request_with_reply
        else:
            my_sm_request = my_sm_request_no_reply

        self.log.info("Sending SM SMS command \"%s\" to server for device id  %s." % (test_name, self.device_id))

        self.log.info("Sending SCI Request:\n%s\n" % my_sm_request)

        # Send device request
        requestResponse = self.cloudHandler.postRequest("sci", data=my_sm_request)

        # Check query was successfully
        if (requestResponse.status_code != 200):
            self.failAndStopAllTests("Unexpected response from the Cloud")

        # Get xml response in dict format
        dataResponse = requestResponse.resource

        # Show response in xml format
        self.log.info("Got Response:\n%s" % xml.dom.minidom.parseString(requestResponse.content).toprettyxml(indent="  ") )

        if reply_needed == True:
            # Get element 'sms'
            element = dataResponse["sci_reply"]["send_message"]["device"]["sms"]
        else:
            # Get element 'send_message'
            element = dataResponse["sci_reply"]["send_message"]
        self.log.info("element: %s" % element)

    def checkSendMessage(self, buffer):
        buffer_idx = buffer.find("Received success response from server") 
        if  buffer_idx != -1:
            SmSmsDvtTestCase.sendDataArrived = SmSmsDvtTestCase.sendDataArrived + 1
            self.log.info("++sendDataArrived=%d!" % SmSmsDvtTestCase.sendDataArrived)


if __name__ == '__main__':
    unittest.main()
