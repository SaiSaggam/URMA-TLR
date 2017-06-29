import time
import cc_testcase


DISCONNECT_REQUEST = \
"""<sci_request version="1.0">
    <disconnect reply="none">
        <targets>
            <device id="%s"/>
        </targets>
    </disconnect>
</sci_request>"""

REBOOT_REQUEST = \
"""<sci_request version="1.0">
    <reboot reply="none">
        <targets>
            <device id="%s"/>
        </targets>
    </reboot>
</sci_request>"""

class ConnectionTestCase(cc_testcase.TestCase):

    def test_disconnect(self):

        """ Sends disconnect request to given device and verifies that
        the device disconnects and reconnects with Device Cloud server.
        """

        self.log.info("***** Beginning Disconnect Test *****")

        # Make sure that device is connected
        if (not self.cloudHandler.isDeviceConnected(self.device_id)):
            self.log.info("Waiting for Device to reconnect.")
            self.deviceMonitor.waitForConnect(30)
            self.log.info("Device connected.")


        self.log.info("Sending Connection Control Disconnect to %s." % self.device_id)

        # Create disconnect request
        disconnect_request = DISCONNECT_REQUEST % (self.device_id)

        # Send SCI disconnect request
        self.cloudHandler.postRequest("sci", data=disconnect_request)

        self.log.info("Waiting for Cloud to disconnect device.")
        self.deviceMonitor.waitForDisconnect(30)
        self.log.info("Device disconnected.")

        self.log.info("Waiting for Device to reconnect.")
        self.deviceMonitor.waitForConnect(30)
        self.log.info("Device connected.")

        #this set the test result as failed
        #self.fail("Reason for the test fail")



    def test_disconnect_correct(self):

        """ Sends disconnect request to given device and verifies that
        the device disconnects and reconnects with Device Cloud server,
        using the Python API to manage Device Cloud.
        """

        self.log.info("***** Beginning Disconnect Test *****")

        # Make sure that device is connected
        if (not self.cloudHandler.isDeviceConnected(self.device_id)):
            self.log.info("Waiting for Device to reconnect.")
            self.deviceMonitor.waitForConnect(30)
            self.log.info("Device connected.")

        self.log.info("Sending Connection Control Disconnect to %s." % self.device_id)

        # Send SCI disconnect request
        result,response = self.cloudHandler.sendDisconnectRequest(self.device_id)

        self.log.info("response to disconnect request = %s" % response.content)

        if ( not result ):
            self.fail("Response to disconnect was not sucessfully")

        self.log.info("Waiting for Cloud to disconnect device.")
        self.deviceMonitor.waitForDisconnect(30)
        self.log.info("Device disconnected.")

        self.log.info("Waiting for Device to reconnect.")
        self.deviceMonitor.waitForConnect(30)
        self.log.info("Device connected.")



    def test_disconnect_loop(self):

        """ Sends disconnect request to given device and verifies that
        the device disconnects and reconnects with Device Cloud server
        several times to verify the process repeatability.
        """

        self.log.info("***** Beginning Disconnect Loop Test *****")

        # Make sure that device is connected
        if (not self.cloudHandler.isDeviceConnected(self.device_id)):
                self.log.info("Waiting for Device to reconnect.")
                self.deviceMonitor.waitForConnect(30)
                self.log.info("Device connected.")

        # Flush previous messages
        self.deviceHandler.readBuffer()

        w1 = 0
        w2 = 0

        for i in range(0,50):
            self.log.info("*** LOOP %s ***" % i)

            # Send SCI disconnect request
            result = False
            while ( not result ):
                self.log.info("Sending Connection Control Disconnect to %s." % (self.device_id) )
                result,response = self.cloudHandler.sendDisconnectRequest(self.device_id,replyRequired=True)

                self.log.info("response to disconnect request = %s" % response.content)

                if ( not result ):
                    #self.fail("Response to disconnect was not successfull")
                    self.log.info("Response to disconnect was not successfull")
                    #disconnectResponse_x = """<sci_reply version="1.0"><disconnect><device id="%s"><disconnected connected="false"/></device></disconnect></sci_reply>"""
                    disconnectResponse_x = """<sci_reply version="1.0"><disconnect><device id="%s"><error id="2006"><desc>Device disconnected while processing request</desc></error></device></disconnect></sci_reply>"""
                    expectedResponse_x = disconnectResponse_x % self.device_id
                    if ( response.content == expectedResponse_x ):
                        # Consider this as good. It's really managing to disconnect
                        w1 = w1 + 1
                        result = True
                    else:
                        # repeat request
                        self.log.info("Response to disconnect was disconnected!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
                        w2 = w2 + 1

            self.log.info("Waiting for Cloud to disconnect device.")
            self.deviceMonitor.waitForDisconnect(60)
            self.log.info("Device disconnected.")

            status1, line1, buffer1 = self.deviceHandler.readUntilPattern ( pattern="network_tcp_close:", timeout=30)
            self.log.info("Console feedback for Disconnect=%d" % status1)
            if status1 == False:
                self.fail("Console feedback for Disconnect NOT arrived")

            self.log.info("Waiting for Device to reconnect.")
            self.deviceMonitor.waitForConnect(60)
            self.log.info("Device connected.")

            connect_message = "connector_tcp_communication_started"
            #Warning Second message is sent together with first message through the console so it's very likely it's in previous readUntilPattern() buffer
            buffer_idx = buffer1.find(connect_message)
            if  buffer_idx != -1:
                self.log.info("Console feedback for Connect got from buffer1 at index %d" % buffer_idx)
            else:
                status2, line2, buffer2 = self.deviceHandler.readUntilPattern ( pattern=connect_message, timeout=30)
                self.log.info("Console feedback for Connect=%d" % status2)
                if status2 == False:
                    self.fail("Console feedback for Connect NOT arrived")
        self.log.info("w1 %d." % w1 )
        self.log.info("w2 %d." % w2 )


    def test_reboot(self):

        """ Sends reboot request to given device and verifies that
        the device disconnects and reconnects with Device Cloud server.
        """

        self.log.info("***** Beginning Reboot Test *****")

        # Make sure that device is connected
        if (not self.cloudHandler.isDeviceConnected(self.device_id)):
            self.log.info("Waiting for Device to reconnect.")
            self.deviceMonitor.waitForConnect(30)
            self.log.info("Device connected.")

        self.log.info("Sending Reboot to %s." % self.device_id)
        # Create reboot request
        reboot_request = REBOOT_REQUEST % (self.device_id)

        # Send SCI reboot request
        response = self.cloudHandler.postRequest("sci", data=reboot_request)

        self.log.info("response to reboot request = %s" % response.content)

        self.log.info("Waiting for Cloud to disconnect device.")
        self.deviceMonitor.waitForDisconnect(30)
        self.log.info("Device disconnected.")

        self.log.info("Waiting for Device to reconnect.")
        self.deviceMonitor.waitForConnect(30)
        self.log.info("Device connected.")


    def test_reboot_correct(self):

        """ Sends reboot request to given device and verifies that
        the device disconnects and reconnects with Device Cloud server,
        using the Python API to manage Device Cloud.
        """

        self.log.info("***** Beginning Reboot Test *****")

        # Make sure that device is connected
        if (not self.cloudHandler.isDeviceConnected(self.device_id)):
            self.log.info("Waiting for Device to reconnect.")
            self.deviceMonitor.waitForConnect(30)
            self.log.info("Device connected.")

        self.log.info("Sending Reboot to %s." % self.device_id)

        # Send SCI reboot request
        result,response = self.cloudHandler.sendRebootRequest(self.device_id)

        self.log.info("response to reboot request = %s" % response.content)

        if ( not result ):
            self.fail("Response to reboot was not sucessfully")

        self.log.info("Waiting for Cloud to disconnect device.")
        self.deviceMonitor.waitForDisconnect(30)
        self.log.info("Device disconnected.")

        status1, line1, buffer1 = self.deviceHandler.readUntilPattern ( pattern="app_os_reboot!", timeout=30)
        self.log.info("Console feedback for Reboot=%d" % status1)
        if status1 == False:
            self.fail("Console feedback for Reboot NOT arrived")

        self.log.info("Waiting for Device to reconnect.")
        self.deviceMonitor.waitForConnect(30)
        self.log.info("Device connected.")
