# -*- coding: iso-8859-1 -*-
import cc_testcase
from string import ascii_letters, digits
import random
import requests.exceptions


class DeviceRequestDvtTestCase(cc_testcase.TestCase):


    def test_01_send_abort_response_target(self):
        """ Verifies that the target "abort response" returns the correct response.
        Connector must manage correctly the abort error before the exit
        """

        payload_size = 2048
        target = "abort response"
        payload = ''.join(random.choice(ascii_letters + digits) for _ in range(payload_size))

        status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

        self.log.info(response.content)


        if(status):
            # Status 200. Checking the received response
            responseText = response.resource["sci_reply"]["data_service"]["device"]["error"]["desc"]
            if(responseText == "Device disconnected while processing request"):
                self.log.info("Received the expected response")

                result, line, dataBuffer = self.deviceHandler.readUntilPattern ( pattern="Cloud Connector terminated", timeout=60)

                # Expected that the Cloud Connector dies, then the result is false or true
                if( dataBuffer.find("app_process_device_request_target: return connector_callback_abort") != -1
                    and dataBuffer.find("app_data_service_handler: manage status connector_close_status_abort") != -1
                    and dataBuffer.find("Cloud Connector terminated") != -1 ):
                    self.log.info("Connector abort feedback received correctly")
                else:
                    self.fail("Console feedback for abort status NOT received")
            else:
                self.fail("Received response from device: \"%s\" is not the expected" % responseText)
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)
