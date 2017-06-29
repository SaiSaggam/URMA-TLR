import time
import cc_testcase
import datetime
import re
import os
import base64
import xml.dom.minidom

test_file_name = 'random.txt'

# timeout used by sci_request
timeout_sci_request = 900

# File transfer size: Each SMS will transport less than 128 bytes after encode85.
# Limit the file trasfer to produce 3 segments
file_transfer_size = 339

def verify_the_file_on_server (self, file_path, file_expected):

    self.log.info("Determining the file and content of %s." %file_path)
    if file_expected:
        # Get File object
        testFile = self.getTestFile(test_file_name)

        expected_content = testFile["data"][:file_transfer_size]

        # Send device request & get plain text
        requestResponse = self.cloudHandler.getRequest(file_path)
        file_content = requestResponse.content

        self.log.info("file_content requestResponse: %s" % requestResponse)
        self.log.info("file_content: \n%s" % file_content)

        #Check if there is a smsRaw file.
        smsRaw_path = 'FileData/~/' + self.device_id + '/smsRaw'
        smsRaw_requestResponse = self.cloudHandler.getRequest(smsRaw_path)
        smsRaw_file_content = smsRaw_requestResponse.content
        self.log.info("smsRawrequestResponse: %s" % smsRaw_requestResponse)
        self.log.info("smsRaw: %s\n" % smsRaw_file_content)

        self.log.info("expected_content: \n%s" % expected_content)

        self.assertEqual(expected_content, file_content,
                        "File's contents do not match what is expected")
        requestResponse = self.cloudHandler.deleteRequest(file_path)
    else:
        # Check that file was not created
        self.assertEquals(404, self.rest_session.get(file_path).status_code)

class SmSmsMultipartGammuDvtTestCase(cc_testcase.SMSTestCase):

    # Note that we are not sending a provisioning message from server to device... so we expect
    # following arguments to be passed in the command line: --cloudphone xxxx --cloudservice_id yyyy

    def test_sm_sms_multipart_0_request(self):
        """ Verifies that the multipart user request with path is processed correctly. """
        # Get File object
        testFile = self.getTestFile(test_file_name)

        file_content = testFile["data"][:file_transfer_size]

        payload = '<user_msg path="myTarget" format="base64">' + base64.b64encode(file_content) + '</user_msg>'
        self.send_sm_sms_request("user_msg", payload, file_transfer_size)


    def test_sm_sms_multipart_1_cli(self):
        """ Verifies that the multipart CLI to the device processed correctly. """
        self.send_sm_sms_request("command", '<command> get file </command>', file_transfer_size)


    def test_sm_sms_multipart_2_cli_limit_1p(self):
        """ Verifies that the multipart CLI, with max 1 packet response, to the device processed correctly. """
        # max_response_bytes = max_payload = 119
        self.send_sm_sms_request("command", '<command maxResponseSize="1"> get 1 packet </command>', 119)


    def test_sm_sms_multipart_3_cli_limit_2p(self):
        """ Verifies that the multipart CLI, with max 2 packet response, to the device processed correctly. """
        # max_response_bytes = max_payload * packets - segment0_ovaerhead_bytes  = 119 * 2 - 2 = 236
        self.send_sm_sms_request("command", '<command maxResponseSize="2"> get 2 packets </command>', 236)


    def test_sm_sms_multipart_4_verify_send_data(self):
        # Make sure through console feedback that the file has been sent & receibed by the server
        status1, line1, buffer1 = self.deviceHandler.readUntilPattern ( pattern="Received success response from server", timeout=30)
        self.log.info("Console feedback for file=%d" % status1)
        if status1 == False:
            self.failAndStopAllTests("Console feedback for file NOT arrived")

        file_path = 'FileData/~/' + self.device_id + '/test/sm_sms_mp0.txt'
        verify_the_file_on_server(self, file_path, True)


    def send_sm_sms_request (self, test_name, command, size_to_read):

        my_sm_sms_request_with_reply = \
            """<sci_request version="1.0">
              <send_message synchronous="true" syncTimeout="%d">
                <targets>
                  <device id="%s"/>
                </targets>
                <sms>
                  %s
                </sms>
              </send_message>
            </sci_request>""" % (timeout_sci_request, self.device_id, command)

        my_sm_sms_request = my_sm_sms_request_with_reply

        self.log.info("Sending SM SMS command \"%s\" to server for device id  %s." % (test_name, self.device_id))

        self.log.info("Sending SCI Request:\n%s\n" % my_sm_sms_request)

        # Send device request
        requestResponse = self.cloudHandler.postRequest("sci", data=my_sm_sms_request)

        # Check query was successfully
        if (requestResponse.status_code != 200):
            self.failAndStopAllTests("Unexpected response from the Cloud")

        # Get xml response in dict format
        dataResponse = requestResponse.resource

        # Show response in xml format
        self.log.info("Got Response:\n%s" % xml.dom.minidom.parseString(requestResponse.content).toprettyxml(indent="  ") )

        # Parse request response 

        # Get element 'sms'
        element = dataResponse["sci_reply"]["send_message"]["device"]["sms"]

        #self.log.info("sms: %s" % element)

        # Get result element
        sm_payload = element[test_name]

        # Check if encoding is base64
        if ( not sm_payload.has_key("@encoding") or sm_payload["@encoding"] != "base64" ):
            self.failAndStopAllTests("Coding for payload is not base64: '%s'" % sm_payload["@encoding"])

        # Get payload
        if ( not sm_payload.has_key("#text") ):
            self.failAndStopAllTests("Missing payload in element '%s'" % sm_payload)
        received_data = base64.b64decode(sm_payload["#text"])
        #self.log.info("sms received_data: '%s'" % received_data)
        self.log.info("sms received_data len: '%d'" % len(received_data))
        if (len(received_data) != size_to_read):
            self.log.info("received_data len does not match expected %d bytes" % size_to_read)

        # Get File object
        fileObject = self.getTestFile(test_file_name)

        # Get only the expected size
        expected_data = fileObject["data"][:size_to_read]

        if (expected_data != received_data):
            self.failAndStopAllTests("Received data does not match what is expected:\n Expected Data:%s\nReceived Data:%s\n" % (repr(expected_data),repr(received_data)))

if __name__ == '__main__':
    unittest.main() 
