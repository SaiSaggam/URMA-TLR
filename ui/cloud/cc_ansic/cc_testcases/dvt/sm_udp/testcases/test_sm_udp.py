import time
import cc_testcase
import datetime
import re
import os
import base64
import xml.dom.minidom

# timeouts used by readUntilPattern function for consoleFeedback
timeout_reply_yes = 15
timeout_reply_no = 50


def getConsoleFeedback_for_user_request(self, path, message, response):
    buffer_idx = -1
    timeout_reply = 0
    if response == True:
        timeout_reply = timeout_reply_yes
    else:
        timeout_reply = timeout_reply_no

    if path != None:
        status1, line1, buffer1 = self.deviceHandler.readUntilPattern ( pattern="Received device request for target %s." % path, timeout=timeout_reply)
        self.log.info("Console feedback for path=%d" % status1)
        if status1 == False:
            self.log.info("Console feedback for path NOT arrived")
            return False
    else:
        #We are getting the 'target' callback even when no target is sent
        status1, line1, buffer1 = self.deviceHandler.readUntilPattern ( pattern="Received device request with no target.", timeout=timeout_reply)
        self.log.info("Console feedback for path=%d" % status1)
        if status1 == False:
            self.log.info("Console feedback for no path NOT arrived")
            return False
    #Warning Second message is sent together with first message through the console so it's very likely it's in previous readUntilPattern() buffer
    pattern2 = 'Data: "%s".' % message
    buffer_idx = buffer1.find(pattern2)
    if  buffer_idx != -1: 
        #print buffer1
        self.log.info("Console feedback for message got from buffer1 at index %d" % buffer_idx)
        return True

    status2, line2, buffer2 = self.deviceHandler.readUntilPattern ( pattern=pattern2, timeout=timeout_reply)
    self.log.info("Console feedback for message=%d" % status2)
    if status2 == False:
        self.log.info("Console feedback for message NOT arrived")
        return False

    return True

def getConsoleFeedback_for_cli(self, response_message):

    timeout_reply = 0
    if response_message == None:
        timeout_reply = timeout_reply_no
    else:
        timeout_reply = timeout_reply_yes

    status1, line1, buffer1 = self.deviceHandler.readUntilPattern ( pattern="Executing numbers.", timeout=timeout_reply)
    self.log.info("Console feedback for Executing=%d" % status1)
    if status1 == False:
        self.log.info("Console feedback for Executing NOT arrived")
        return False
    if response_message == None:
        return True

    #Warning Second message is sent together with first message through the console so it's very likely it's in previous readUntilPattern() buffer
    buffer_idx = buffer1.find(response_message) 
    if  buffer_idx != -1: 
        #print buffer1
        self.log.info("Console feedback for response_message got from buffer1 at index %d" % buffer_idx)
        return  True

    status2, line2, buffer2 = self.deviceHandler.readUntilPattern ( pattern=response_message, timeout=timeout_reply)
    self.log.info("Console feedback for response_message=%d" % status2)
    if status2 == False:
        self.log.info("Console feedback for response_message NOT arrived")
        return False

    return True



class SmUdpDvtTestCase(cc_testcase.UDPTestCase):

    def test_sm_udp_0_ping(self):
        """ Verifies that the device can be pinged. """
        self.send_sm_udp_request("Ping", "<ping/>", True)
        status, unused1, unused2 = self.deviceHandler.readUntilPattern ( pattern="Received ping request. response is needed", timeout=timeout_reply_yes)
        self.log.info("Console feedback=%d" % status)
        if status == False:
           self.fail("Console feedback NOT arrived")

    def test_sm_udp_1_ping_no_reply(self):
        """ Verifies that the device can be pinged. No reply. """
        self.send_sm_udp_request("Ping", "<ping/>", False)
        status, unused1, unused2 = self.deviceHandler.readUntilPattern ( pattern="Received ping request. response is not needed", timeout=timeout_reply_no)
        self.log.info("Console feedback=%d" % status)
        if status == False:
           self.fail("Console feedback NOT arrived")

    def test_sm_udp_2_user_request(self):
        """ Verifies that the user request with path is processed correctly. """

        path = "myTarget"
        message = "My user request message"
        response = True
        received_data = self.send_sm_udp_request("User request", '<user_msg path="%s">%s</user_msg>' %(path,message), response)
        self.assertEqual("My response data for device request data\n", received_data, "received_data do not match what is expected")
        if getConsoleFeedback_for_user_request(self, path, message, response) == False:
           self.fail("Console feedback Failed")

    def test_sm_udp_3_user_request_no_reply(self):
        """ Verifies that the user request with path is processed correctly. No reply. """
        path = "myTarget"
        message = "My user request message"
        response = False
        self.send_sm_udp_request("User request", '<user_msg path="%s">%s</user_msg>' %(path,message), response)
        if getConsoleFeedback_for_user_request(self, path, message, response) == False:
           self.fail("Console feedback Failed")


    def test_sm_udp_4_user_request_no_path(self):
        """ Verifies that the user request without path is processed correctly. """
        path = None
        message = "My user request message without path"
        response = True
        received_data = self.send_sm_udp_request("User request no path", '<user_msg>%s</user_msg>' %(message), response)
        self.assertEqual("My response data for device request data with no path\n", received_data, "received_data do not match what is expected")
        if getConsoleFeedback_for_user_request(self, path, message, response) == False:
           self.fail("Console feedback Failed")


    def test_sm_udp_5_user_request_no_path_no_reply(self):
        """ Verifies that the user request without path is processed correctly. No reply. """
        path = None
        message = "My user request message without path"
        response = False
        self.send_sm_udp_request("User request no path", '<user_msg>%s</user_msg>' %(message), response)
        if getConsoleFeedback_for_user_request(self, path, message, response) == False:
           self.fail("Console feedback Failed")

    def test_sm_udp_6_cli(self):
        """ Verifies that the CLI to the device processed correctly. """
        payload = "Numbers=0123456789"
        response_message = "Sending CLI response, %s." % payload
        received_data = self.send_sm_udp_request("CLI", '<command> numbers </command>', response_message != None)
        self.assertEqual(payload, received_data, "received_data do not match what is expected")
        if getConsoleFeedback_for_cli(self, response_message) == False:
           self.fail("Console feedback Failed")

    def test_sm_udp_7_cli_no_reply(self):
        """ Verifies that the CLI to the device processed correctly. No reply. """
        response_message = None
        self.send_sm_udp_request("CLI", '<command> numbers </command>', response_message != None)
        if getConsoleFeedback_for_cli(self, response_message) == False:
           self.fail("Console feedback Failed")

    def test_sm_udp_a_connect_request(self):
        """ Verifies that the connect request to the device processed correctly. """

        # Device shoud start disconnected as CONNECTOR_NETWORK_TCP_START is set to connector_connect_manual
        if ( self.cloudHandler.isDeviceConnected(self.device_id) ):
            self.fail("Device is Connected. Should be Disconnected")
        else:
            self.log.info("Device is Disconnected (fine)")

        self.send_sm_udp_request("Connect request", '<request_connect/>', True)
        status, index, bufferData = self.deviceHandler.readUntilPattern ( pattern="connector_tcp_communication_started", timeout=timeout_reply_yes)
        self.log.info("Console feedback=%d" % status)
        if status == False:
           self.fail("Console feedback NOT arrived")

        self.log.info("Waiting for Device to connect.")
        self.deviceMonitor.waitForConnect(60)
        self.log.info("Device connected.")

    def test_sm_udp_b_connect_request_no_reply(self):
        """ Verifies that the connect request to the device processed correctly. No reply. """
        # We could disconnect the device to test again... but it has really been testes in previos test.
        self.send_sm_udp_request("Connect request", '<request_connect/>', False)
        # No console feedback possible

    def test_sm_udp_c_reboot(self):
        """ Verifies that the reboot command to the device processed correctly. """
        self.send_sm_udp_request("Reboot", '<reboot/>', True)

        status1, line1, buffer1 = self.deviceHandler.readUntilPattern ( pattern="app_os_reboot!", timeout=30)
        self.log.info("Console feedback for Reboot=%d" % status1)
        if status1 == False:
            self.fail("Console feedback for Reboot NOT arrived")

    def test_sm_udp_d_reboot_no_reply(self):
        """ Verifies that the reboot command to the device processed correctly. No reply. """
        self.send_sm_udp_request("Reboot", '<reboot/>', False)

        status1, line1, buffer1 = self.deviceHandler.readUntilPattern ( pattern="app_os_reboot!", timeout=30)
        self.log.info("Console feedback for Reboot=%d" % status1)
        if status1 == False:
            self.fail("Console feedback for Reboot NOT arrived")

    def test_sm_udp_e_verify_send_data(self):
        file_path = 'FileData/~/' + self.device_id + '/test/sm_udp.txt'
        expected_content = "Etherios sm udp dvt for device data\n"

        # Send device request
        requestResponse = self.cloudHandler.getRequest(file_path)

        # Get plain text
        file_content = requestResponse.content

        self.assertEqual(expected_content, file_content, "File's contents do not match what is expected")

        # Send device request
        requestResponse = self.cloudHandler.deleteRequest(file_path)


    def send_sm_udp_request (self, test_name, command, reply_needed):

        my_sm_udp_request_with_reply = \
            """<sci_request version="1.0">
              <send_message synchronous="true">
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

        self.log.info("Sending SCI Request:\n%s" % my_sm_udp_request)

        # Send device request
        requestResponse = self.cloudHandler.postRequest("sci", data=my_sm_udp_request)

        # Check query was successfully
        if (requestResponse.status_code != 200):
            self.fail("Unexpected response from the Cloud")

        # Get xml response in dict format
        dataResponse = requestResponse.resource

        # Show response in xml format
        self.log.info("Got Response:\n%s" % xml.dom.minidom.parseString(requestResponse.content).toprettyxml(indent="  ") )


        if reply_needed == True:

            #****************#
            #*** METHOD A ***#
            #****************#

            ## Check if exist entry 'sci_reply'
            #if( not dataResponse.has_key("sci_reply") ):
                #self.fail("Response has not element sci_reply")

            #element = dataResponse["sci_reply"]
            #self.log.info("sci_reply: %s" % element)

            ## Check if exist entry 'sm_udp'
            #if( not element.has_key("send_message") ):
                #self.fail("Response has not element send_message")

            #element = element["send_message"]
            #self.log.info("send_message: %s" % element)

            ## Check if exist entry 'sm_udp'
            #if( not element.has_key("device") ):
                #self.fail("Response has not element device")

            #element = element["device"]

            ## Check if exist entry 'sm_udp'
            #if( not element.has_key("sm_udp") ):
                #self.fail("Response has not element sm_udp")

            #element = element["sm_udp"]

            #self.log.info("sm_udp: %s" % element)


            #****************#
            #*** METHOD B ***#
            #****************#

            # Get element 'sm_udp'
            element = dataResponse["sci_reply"]["send_message"]["device"]["sm_udp"]

            #self.log.info("sm_udp: %s" % element)


            #**************#
            #*** COMMON ***#
            #**************#

            # Extract test type
            testType = command.split(">")[0] # ['<user_msg', 'xxxxxxxx</user_msg>']
            testType = testType.replace("<","").replace(">","").replace("/","")
            testType = testType.split(" ")[0]
            self.log.info("Command to test found: '%s'" % testType)


            # Check value
            if ( testType in ["ping","request_connect","reboot"] ):
                if ( element[testType] != None ):
                    self.fail("Value for element '%s' is not None: %s" % (testType, element[testType]) )
            elif ( testType in ("user_msg","command") ):
                # Get result element
                sm_payload = element[testType]

                # Check if encoding is base64
                if ( not sm_payload.has_key("@encoding") or sm_payload["@encoding"] != "base64" ):
                    self.fail("Coding for payload is not base64: '%s'" % sm_payload["@encoding"])

                # Get payload
                if ( not sm_payload.has_key("#text") ):
                    self.fail("Missing payload in element '%s'" % sm_payload)
                received_data = base64.b64decode(sm_payload["#text"])
                self.log.info("sm_udp received_data: '%s'" % received_data)

                return received_data


        else:
            # Check if exist entry 'send_message'
            if( not dataResponse.has_key("sci_reply") and not dataResponse["sci_reply"].has_key("send_message") ):
                self.fail("Unexpected response from device")

        self.log.info("Got expected response from device")



if __name__ == '__main__':
    unittest.main() 
