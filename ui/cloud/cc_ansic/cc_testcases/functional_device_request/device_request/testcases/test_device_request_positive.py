# -*- coding: iso-8859-1 -*-
import cc_testcase
from string import ascii_letters, digits
import random
import requests.exceptions


class DeviceRequestDvtTestCase(cc_testcase.TestCase):

    def test_01_send_invalid_target_without_payload(self):
        """ Sends an invalid target to the device without payload
        Verifies if the returned response is the expected.
        """

        invalidTarget = "invalidTarget"
        payload = ""

        status, response = self.cloudHandler.sendDeviceRequest(self.device_id, invalidTarget, payload)

        if(status):
            # Status 200. Checking the received response
            responseText = response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]["#text"]
            if(responseText == "Target is not supported"):
                self.log.info("Received the expected response")
            else:
                self.fail("Received response from device: \"%s\" is not the expected" % responseText)
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)



    def test_02_send_invalid_target_with_payload(self):
        """ Sends an invalid target to the device with payload
        Verifies if the returned response is the expected.
        """

        invalidTarget = "invalidTarget"
        payload = "payload"


        status, response = self.cloudHandler.sendDeviceRequest(self.device_id, invalidTarget, payload)

        if(status):
            # Status 200. Checking the received response
            responseText = response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]["#text"]
            if(responseText == "Target is not supported"):
                self.log.info("Received the expected response")
            else:
                self.fail("Received response from device: \"%s\" is not the expected" % responseText)
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)



    def test_03_send_valid_target_without_payload(self):
        """ Sends a valid target to the device without payload
        Verifies if the returned response is the expected.
        """

        target = "myTarget"
        payload = ""

        status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

        self.log.info(response.content)
        self.log.info(response.resource)

        if(status):
            # Status 200. Checking the received response
            if(not "#text" in response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]):
                self.log.info("Received the expected response (empty)")

                self.log.info("Checking the payload received")
                result, line, buffer1 = self.deviceHandler.readUntilPattern ( pattern="Total Payload received = \"%s\" for %s" %(payload, target), timeout=30)
                if(result):
                    self.log.info("Payload received successfully!!")
                else:
                    self.fail("Console feedback for payload NOT found")

            else:
                self.fail("Received response from device: \"%s\" is not the expected" % responseText)
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)



    def test_04_send_valid_target_with_payload(self):
        """ Sends a valid target to the device with payload
        Verifies if the returned response is the expected.
        """

        target = "myTarget"
        payload = "payload"

        status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

        self.log.info(response.content)

        if(status):
            # Status 200. Checking the received response
            responseText = response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]["#text"]
            if(responseText == payload):
                self.log.info("Received the expected response")

                self.log.info("Checking the payload received")
                result, line, buffer1 = self.deviceHandler.readUntilPattern ( pattern="Total Payload received = \"%s\" for %s" %(payload, target), timeout=30)
                if(result):
                    self.log.info("Payload received successfully!!")
                else:
                    self.fail("Console feedback for payload NOT found")

            else:
                self.fail("Received response from device: \"%s\" is not the expected" % responseText)
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)



    def test_05_send_valid_target_several_times_with_increasing_payload(self):
        """ Sends a device request several times with the same target.
        The payload is increased in each iteration, up to a maximum of 2048 bytes (above 1389 requires more than one packet)
        """

        target = "myTarget"
        payload = ""
        payload_size = 64
        iterator = 16

        while(iterator > 1):
            # This loop generates random strings of with the lenght of 'payload_size' value
            # Then, each string is sent in a device request
            payload = ''.join(random.choice(ascii_letters + digits) for _ in range(payload_size))
            self.log.info("**** Sending a payload size of %s ...." % len(payload) )

            status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

            if(status):
                # Status 200. Checking the received response
                responseText = response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]["#text"]
                if(responseText == payload):
                    self.log.info("Received the expected response!")

                    self.log.info("Checking the payload received")
                    result, line, buffer1 = self.deviceHandler.readUntilPattern ( pattern="Total Payload received = \"%s\" for %s" %(payload, target), timeout=30)
                    if(result):
                        self.log.info("Payload received successfully!")
                        #result, line, buffer1 = self.deviceHandler.readUntilPattern ( pattern="device_request memory was released", timeout=10)
                    else:
                        self.fail("Console feedback for payload size %d NOT found" % payload_size)

                else:
                    self.fail("Received response from device: \"%s\" is not the expected" % responseText)
            else:
                self.log.error("Response content from device: %s" % response.content)
                self.fail("Incorrect response code: %d" % response.status_code)

            payload_size *= (iterator)
            iterator = iterator/2
