import time
import cc_testcase

filedata = 'FileData/~/'
file_name = 'test/sm_data.txt'
file_name1 = 'test/sm_data_noresp.txt'



class SmUdpSendDataTestCase(cc_testcase.UDPTestCase):

    def test_01_sm_udp_send_data(self):
    
        """ This routine gets the sent file from the cloud and   
        verifies that the correct data is pushed """
        
        self.log.info("Begin sm_send_data test.")

        # We should check console feedback for both files:
        #    - 'test/sm_data.txt': "Received success response from server"
        #    - 'test/sm_data_noresp.txt': "Data service error: 0"
        # but data is send so early that it's flushed before this case starts.

        time.sleep(10)

        # Create paths to files.
        expected_content = 'Etherios sm_send_data sample data\n'
        file_location = filedata + self.device_id + '/' + file_name
        file_location1 = filedata + self.device_id + '/' + file_name1
        self.log.info("Attempting to Retrieve data from File Locations: %s, %s" % (file_location, file_location1))

        # Check for existing file and its contents.
        try:
            file_content = self.cloudHandler.getRequest(file_location).content
        except Exception, e:
            self.fail('Failed to Get Device Contents.  Reason: %s' % e)
        
        # Verify file's contents
        if file_content != expected_content:
           self.fail("Received content: %s does not match expected: %s" % (file_content, expected_content))

        self.log.info("File %s: %s" % (file_name, file_content))
        # Check for existing file and its contents.
        try:
            file_content = self.cloudHandler.getRequest(file_location1).content
        except Exception, e:
            self.fail('Failed to Get Device Contents.  Reason: %s' % e)

        self.log.info("File %s: %s" % (file_name1, file_content))
        
        # Verify file's contents
        if file_content != expected_content:
           self.fail("Received content: %s does not match expected: %s" % (file_content, expected_content))

    def test_02_sm_udp_connect_without_tcp_transport(self):
        """ This test sends a request connect to an application that does not have
        TCP transport enabled. In releases previous to 2.1.x this caused an abort."""
        
        # This is executed in second place because otherwise the tearDown() function deletes the 
        SM_CONNECT_REQUEST = \
        """<sci_request version="1.0">
               <send_message synchronous="false"> 
                   <targets>
                       <device id="%s"/>
                   </targets>
                   <sm_udp>
                       <request_connect/>
                   </sm_udp>
               </send_message>
           </sci_request>"""
        #CONNECTOR_CONSOLE_FEEDBACK = "WARNING: received a 'request connect' but TCP transport is not available"
        
        self.log.info("Sending SM UDP Connect Request to %s. And wait 10 seconds before checking connectivity." % self.device_id)
        time.sleep(10)
        # Create connect request.
        sm_connect_request = SM_CONNECT_REQUEST % (self.device_id)

        # Send SCI SM UDP connect request
        requestResponse = self.cloudHandler.postRequest("sci", data=sm_connect_request)
        # Check query was successfully
        if (requestResponse.status_code != 202): # It's an asyncrhonous job, so we want to know that it was accepted only
            self.fail("Unexpected response from the Cloud %s" % (requestResponse.status_code))

        #result, indexPattern, dataBuffer = self.deviceHandler.readUntilPattern ( pattern=CONNECTOR_CONSOLE_FEEDBACK, timeout=30)
        #self.log.info("Console feedback for request connect without TCP enabled = %d" % result)
        #if ( not result ):
        #    self.fail("Console feedback for request connect without TCP enabled NOT arrived")

        # Verifies that the device can be pinged.
        self.send_sm_udp_ping_request(True)
        status, unused1, unused2 = self.deviceHandler.readUntilPattern ( pattern="Received ping request. response is needed", timeout=30)
        self.log.info("Console feedback=%d" % status)
        if status == False:
           self.fail("Console feedback NOT arrived")
        if (self.cloudHandler.isDeviceConnected(self.device_id)):
            self.fail("Device connected, it should not have TCP transport.")

    def send_sm_udp_ping_request (self, reply_needed):
        my_sm_udp_request_with_reply = \
            """            <sci_request version="1.0">
              <send_message synchronous="true">
                <targets>
                  <device id="%s"/>
                </targets>
                <sm_udp retries="5" retryTimeout="10">
                  <ping/>
                </sm_udp>
              </send_message>
            </sci_request>""" % (self.device_id)

        my_sm_udp_request_no_reply = \
            """            <sci_request version="1.0">
              <send_message reply="none">
                <targets>
                  <device id="%s"/>
                </targets>
                <sm_udp retries="5" retryTimeout="10">
                  <ping/>
                </sm_udp>
              </send_message>
            </sci_request>""" % (self.device_id)

        if reply_needed == True:
            my_sm_udp_request = my_sm_udp_request_with_reply
        else:
            my_sm_udp_request = my_sm_udp_request_no_reply

        self.log.info("Sending SM UDP command \"ping\" to server for device id  %s." % (self.device_id))

        self.log.info("Sending SCI Request:\n%s" % my_sm_udp_request)

        # Send device request
        requestResponse = self.cloudHandler.postRequest("sci", data=my_sm_udp_request)

        # Check query was successfully
        if (requestResponse.status_code != 200):
            self.fail("Unexpected response from the Cloud")

        # Get xml response in dict format
        dataResponse = requestResponse.resource

        if reply_needed == True:
            # Get element 'sm_udp'
            ping = dataResponse["sci_reply"]["send_message"]["device"]["sm_udp"]["ping"]

            # Check value
            if ( ping != None ):
                self.fail("Value for element 'ping' is not None: %s" % (testType, ping) )
        else:
            # Check if exist entry 'send_message'
            if( not dataResponse.has_key("sci_reply") and not dataResponse["sci_reply"].has_key("send_message") ):
                self.fail("Unexpected response from device")

        self.log.info("Got expected response from device")

    def tearDown(self):
        self.log.info("Performing cleanup.")
        file_location = filedata + self.device_id + '/' + file_name
        file_location1 = filedata + self.device_id + '/' + file_name1
        self.cloudHandler.deleteRequest(file_location)
        self.cloudHandler.deleteRequest(file_location1)

if __name__ == '__main__':
    unittest.main() 
