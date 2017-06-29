import time
import cc_testcase
import xml.dom.minidom


# Enviroment settings
firmware_targets = [('Text Image', '.*\.[tT][xX][tT]', '1.0.0.0')]
firmware_files   = ["firmware_malloc.txt"]
defaultTarget = 0

#HB: I've checked that sm_udp attributes (retries, retryTimeout) have no effect with request_connect command. 
#    Command times out after 75". So I've removed them
SM_CONNECT_REQUEST = \
"""<sci_request version="1.0">
    <send_message synchronous="true">
        <targets>
            <device id="%s"/>
        </targets>
        <sm_udp>
            <request_connect/>
        </sm_udp>
    </send_message>
</sci_request>"""


class UdpConnectTestCase(cc_testcase.UDPTestCase):


    def test_sm_udp_0_connect(self):

        """ Sends SM UDP connect request to given device and verifies that
        the device connects to an iDigi server. Then sends a disconnect request 
        and verifies that the device disconnects.
        """

        self.log.info("***** Beginning SM UDP Connect / Disconnect Test *****")
 
        self.send_sm_connect_request()
        self.send_sm_disconnect_request()



    def test_update_1_firmware(self):

        self.log.info("***** Beginning SM UDP Connect / Firmware Test *****")
        self.send_sm_connect_request()

        # Get File object
        testFile = self.getTestFile(firmware_files[defaultTarget])

        # Send request to update firmware
        self.log.info("Sending request to update firmware.")
        result,response = self.cloudHandler.sendUpdateFirmwareRequest(self.device_id, testFile["data"], target=defaultTarget)
        if ( not result ):
            self.fail("Response to update_firmware was not sucessfully")


        # Show response in xml format
        self.log.info("Got Response:\n%s" % xml.dom.minidom.parseString(response.content).toprettyxml(indent="  ") )


        result, indexPattern, dataBuffer = self.deviceHandler.readUntilPattern ( pattern="app_firmware_download_complete: target= 0", timeout=30)
        self.log.info("Console feedback for update_firmware completed=%d" % result)
        if ( not result ):
            self.fail("Console feedback for update_firmware completed NOT arrived")

        if (self.cloudHandler.isDeviceConnected(self.device_id)):
            self.log.info("Waiting for Cloud to disconnect device.")
            self.deviceMonitor.waitForDisconnect(30)
        self.log.info("Device disconnected.")



    def send_sm_connect_request (self):

        self.log.info("Sending SM UDP Connect Request to %s." % self.device_id)

        # Create connect request.
        sm_connect_request = SM_CONNECT_REQUEST % (self.device_id)

        # Send SCI SM UDP connect request
        requestResponse = self.cloudHandler.postRequest("sci", data=sm_connect_request)

        # Check query was successfully
        if (requestResponse.status_code != 200):
            self.fail("Unexpected response from the Cloud")

        # Get xml response in dict format
        dataResponse = requestResponse.resource

        # Show response in xml format
        self.log.info("Got Response:\n%s" % xml.dom.minidom.parseString(requestResponse.content).toprettyxml(indent="  ") )

        # Get element 'sm_udp'.
        element = dataResponse["sci_reply"]["send_message"]["device"]["sm_udp"]

        self.log.info("sm_udp: %s" % element)

        if (not self.cloudHandler.isDeviceConnected(self.device_id)):
            self.log.info("Waiting for Device to connect.")
            self.deviceMonitor.waitForConnect(60)

        self.log.info("Device connected.")



    def send_sm_disconnect_request(self):

        self.log.info("Sending Connection Control Disconnect to %s." % self.device_id)

        # Send SCI disconnect request
        result,response = self.cloudHandler.sendDisconnectRequest(self.device_id)

        self.log.info("response to disconnect request = %s" % response.content)

        if ( not result ):
            self.fail("Response to disconnect was not sucessfully")

        if (self.cloudHandler.isDeviceConnected(self.device_id)):
            self.log.info("Waiting for Cloud to disconnect device.")
            self.deviceMonitor.waitForDisconnect(30)
        self.log.info("Device disconnected.")

if __name__ == '__main__':
    unittest.main()
