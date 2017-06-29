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

class SmSmsMultipartDvtTestCase(cc_testcase.SMSTestCase):
    sendDataArrived = 0

    # Note that we are not sending a provisioning message from server to device... so we expect
    # following arguments to be passed in the command line: --cloudphone xxxx --cloudservice_id yyyy

    def test_sm_sms_multipart_0_request(self):
        """ Verifies that the multipart user request with path is processed correctly. """
        # Get File object
        testFile = self.getTestFile(test_file_name)

        file_content = testFile["data"][:file_transfer_size]

        payload = '<user_msg path="myTarget" format="base64">' + base64.b64encode(file_content) + '</user_msg>'
        received_data, buffer = self.send_sm_sms_request("user_msg", payload)
        self.verify_sm_sms_request(file_transfer_size, received_data)

        buffer_idx = buffer.find("Received success response from server")
        if  buffer_idx == -1:
            status, line, buffer = self.deviceHandler.readUntilPattern ( pattern="Received success response from server", timeout=timeout_sci_request)
            self.log.info("Console feedback for verify_send_data=%d" % status)
            if status == False:
                self.fail("Console feedback for verify_send_data NOT arrived")
        else:
            self.log.info("Console feedback for verify_send_data arrived")


    def test_sm_sms_multipart_1_cli(self):
        """ Verifies that the multipart CLI to the device processed correctly. """
        received_data, buffer = self.send_sm_sms_request("command", '<command> get file </command>')
        self.verify_sm_sms_request(file_transfer_size, received_data)


    def test_sm_sms_multipart_2_cli_limit_1p(self):
        """ Verifies that the multipart CLI, with max 1 packet response, to the device processed correctly. """

        # Initialize expected number of packets for this test case
        expectedPackets = 1

        received_data, buffer = self.send_sm_sms_request("command", '<command maxResponseSize="%d"> get %d packet </command>' % (expectedPackets,expectedPackets) )
        totalBytesSent = self.check_maxResponseSize(expectedPackets, buffer)
        self.verify_sm_sms_request(totalBytesSent, received_data)


    def test_sm_sms_multipart_3_cli_limit_2p(self):
        """ Verifies that the multipart CLI, with max 2 packet response, to the device processed correctly. """

        # Initialize expected number of packets for this test case
        expectedPackets = 2

        received_data, buffer = self.send_sm_sms_request("command", '<command maxResponseSize="%d"> get %d packets </command>' % (expectedPackets,expectedPackets) )
        totalBytesSent = self.check_maxResponseSize(expectedPackets, buffer)
        self.verify_sm_sms_request(totalBytesSent, received_data)


    def test_sm_sms_multipart_4_cli_longrequest(self):
        """ Verifies that CLI with a long request is correctly split in several requests. """

        # Get File object
        testFile = self.getTestFile(test_file_name)

        received_data, buffer = self.send_sm_sms_request("command", '''<command>i.......................................................................
............................0.........1.........2.........3.........4.........5.........</command>''')

        self.verify_sm_sms_request(file_transfer_size, received_data)

        buffer_idx = buffer.find("More request chunks arriving !!!")
        self.log.info("buffer_idx=%d" % buffer_idx)
        if buffer_idx == -1:
            self.fail("Console feedback for More request chunks NOT arrived")

        buffer_idx = buffer.find("Last request chuck.")
        if buffer_idx == -1:
            self.fail("Console feedback for Last request chuck NOT arrived")

    def test_sm_sms_multipart_5_verify_send_data(self):
        file_path = 'FileData/~/' + self.device_id + '/test/sm_sms_mp0.txt'
        verify_the_file_on_server(self, file_path, True)


    def send_sm_sms_request (self, test_name, command):

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

        # Clean buffer
        buffer = self.deviceHandler.readBuffer()

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
        self.log.info("sms received_data len: '%d'" % len(received_data))

        buffer = self.deviceHandler.readBuffer()

        return received_data, buffer


    def verify_sm_sms_request (self, size_to_read, received_data):
        if (len(received_data) != size_to_read):
            self.log.info("received_data len does not match expected %d bytes" % size_to_read)

        # Get File object
        fileObject = self.getTestFile(test_file_name)

        # Get only the expected size
        expected_data = fileObject["data"][:size_to_read]

        if (expected_data != received_data):
            self.failAndStopAllTests("Received data does not match what is expected:\n Expected Data:%s\nReceived Data:%s\n" % (repr(expected_data),repr(received_data)))


    def check_maxResponseSize (self, expectedPackets, dataBuffer):
        # This is a example of the sequence ):
        #       CLI request [get xxx packets] returning busy...
        # (*)   Executing: "get xxx packets".
        #       Last request chuck.
        #       CLI get length returning busy...
        #       CLI response returning busy...
        # (*)   Sending CLI response [yyy bytes]
        # (*)   app_network_sms_send: send zzzz bytes
        # (*)   app_network_sms_send: send zzzz bytes

        # Note that on sms application we don't send ping to filter
        # It'd be difficult to filter pings as they have different lenght 
        # depending on if sevice-id is used or not


        # Verify we are sent the CLI response
        indexCLIResponse = dataBuffer.find("Sending CLI response")
        if (indexCLIResponse == -1): 
            self.fail("Console feedback for CLI response NOT arrived")

        # Get total bytes sent from the console
        lineCLIResponse = dataBuffer[indexCLIResponse] # 'Sending CLI response [yyy bytes]'
        lineCLIResponse = lineCLIResponse[lineCLIResponse.find("[")+1 : lineCLIResponse.find("]")] # 'yyy bytes'
        totalBytesSent = int(lineCLIResponse.split()[0]) # 'yyy'
        self.log.info("Got response_bytes = %d bytes from console" % totalBytesSent)


        # Get total packet to sent from the console
        indexBuffer = dataBuffer.find("Executing:")
        if (indexBuffer == -1):
            self.fail("Console feedback for Number of packets NOT arrived")
        linePackets = dataBuffer[indexBuffer] # 'Executing: "get xxx packets"'
        linePackets = linePackets.split() # ['Executing:','"get','xxx','packets"']
        totalPacketsSent = int(linePackets[-2]) # 'xxx'
        self.log.info("Got response_packets = %d packets from console" % totalPacketsSent)


        # Verify that the number of packets sent match with the expected in this test case (xxx)
        if (totalPacketsSent == expectedPackets):
            self.log.info("Console feedback for num_packets_needed correct (%d)" % totalPacketsSent)
        else:
            self.log.info("buffer=%s" % dataBuffer)
            self.fail("Console feedback for num_packets_needed NOT correct (%d) expected (%d)" % (totalPacketsSent,expectedPackets) )


        # Verify that connector sent the expected number of packets
        counterPackets = 0

        #** Remove the lines before the CLI response
        for indexLine in range( indexCLIResponse, len(dataBuffer) ):
            eachLine = dataBuffer[indexLine]

            if ( eachLine.find("app_network_sms_send") != -1 ):
                counterPackets += 1


        # Some packets could be sent with a delay, we wait for these packets
        timeout = 20 # seconds to wait for missing packets
        counter = 0
        while ( counterPackets < expectedPackets and counter<=timeout ):
            counter += 1
            # Get the console output and check it
            dataBuffer = self.deviceHandler.readBuffer()
            if(dataBuffer != None):
                for eachLine in dataBuffer:
                    if ( eachLine.find("app_network_sms_send") != -1 ):
                        counterPackets += 1

            if(counterPackets < expectedPackets):
                self.log.info("Waiting for console feedback to capture packets sent...")
                time.sleep(1)


        # Final verification to make sure that all expected packets were sent sucessfully
        if ( counterPackets == expectedPackets ): 
            self.log.info("Console feedback for num_packets_sent correct (%d)" % counterPackets)
        else:
            self.fail("Console feedback for num_packets_sent NOT correct (%d) expected (%d)" % (counterPackets,expectedPackets) )     

        return totalBytesSent


if __name__ == '__main__':
    unittest.main() 
