import cc_testcase
import xml.dom.minidom
import base64

class DeviceRequestUdpTestCase(cc_testcase.UDPTestCase):
       
    def test_sm_user_msg_udp(self):
        """ User msg with reply for expected target """
        received_data = self.send_sm_udp_request("User request", '<user_msg path="myTarget">My device request data</user_msg>', True)
        self.assertEqual("My device response data", received_data, "received_data do not match what is expected")

    def test_sm_user_msg_noresp_udp(self):
        """ User msg with no reply """
        self.send_sm_udp_request("user_msg no reply", '<user_msg path="myTarget">My device request data</user_msg>', False)

    def test_sm_user_msg_nulltarget_udp(self):
        """ User msg with reply for empty target """
        received_data = self.send_sm_udp_request("user_msg empty target", '<user_msg>My device request data</user_msg>', True)
        self.assertEqual("Empty target in SM device request", received_data, "received_data do not match what is expected")

    def test_sm_user_msg_badtarget_udp(self):
        """ User msg with reply for unexpected target """
        received_data = self.send_sm_udp_request("user_msg bad target", '<user_msg path="badTarget">My device request data</user_msg>', True)
        self.assertEqual("Unexpected target in SM device request", received_data, "received_data do not match what is expected")

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
