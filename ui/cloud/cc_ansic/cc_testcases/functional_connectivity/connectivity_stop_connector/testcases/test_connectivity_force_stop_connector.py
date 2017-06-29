# -*- coding: iso-8859-1 -*-
import cc_testcase
from string import ascii_letters, digits
import random
import time


class DeviceRequestDvtTestCase(cc_testcase.TestCase):


    def test_01_stop_and_terminate_connector(self):
        """ Verifies that Connector must manage correctly stop all transports after complete all opened sessions and finish the connector thread.
            In this case should send the response to the device request correctly.
        """

        payload_size = 2048
        target = "test_stop_and_terminate_connector"
        payload = ''.join(random.choice(ascii_letters + digits) for _ in range(payload_size))

        status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

        self.log.info("Received response: '%s'" % response.content)


        if(status):
            # Status 200. Checking the received response
            try:
                responseText = response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]["#text"]
                if(responseText == "My device response data"):
                    self.log.info("Received the expected response")

                    # Verify connector stops all transports
                    result, line, dataBuffer = self.deviceHandler.readUntilPattern ( pattern="connector_stop_callback: connector_transport_all", timeout=10)
                    if (result):
                        self.log.info("Connector stop callback feedback received correctly")
                    else:
                        self.fail("Console feedback for stop callback status NOT received")

                    # Verify connector terminates
                    result, line, dataBuffer = self.deviceHandler.readUntilPattern ( pattern="Cloud Connector terminated", timeout=10)
                    if (result):
                        self.log.info("Connector exits feedback received correctly")
                    else:
                        self.fail("Console feedback for exits status NOT received")

                else:
                    self.fail("Received response from device: \"%s\" is not the expected" % responseText)
            except KeyError,e:
                self.log.exception("KeyError exception caugth: %s" % e)
                # Read all from the buffer
                self.deviceHandler.readBuffer()
                self.fail("KeyError exception caugth!!")
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)


        # Verify that the device disconnects from Device Cloud
        if (self.cloudHandler.isDeviceConnected(self.device_id)):
            self.log.info("Waiting for Cloud to disconnect device.")
            self.deviceMonitor.waitForDisconnect(30)
            self.log.info("Device disconnected.")
        else:
            self.log.info("Device is disconnected from Device Cloud")



