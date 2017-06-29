# -*- coding: iso-8859-1 -*-
import cc_testcase
from string import ascii_letters, digits
import random
import requests.exceptions


class DeviceRequestDvtTestCase(cc_testcase.TestCase):

    def test_01_send_busy_request_target(self):
        """ Verifies that the target "busy request" returns the correct response.
        First, connector shows a "busy" message and then continues the execution normally
        """

        payload_size = 2048
        target = "busy request"
        payload = ''.join(random.choice(ascii_letters + digits) for _ in range(payload_size))

        status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

        self.log.info(response.content)

        if(status):
            # Status 200. Checking the received response
            responseText = response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]["#text"]
            if(responseText == payload):

                self.log.info("Received the expected response")
                self.log.info("Checking the payload received")
                result, line, dataBuffer = self.deviceHandler.readUntilPattern ( pattern="Total Payload received = \"%s\" for %s" %(payload, target), timeout=30)
                if(result):

                    self.log.info("Payload received successfully")
                    if(dataBuffer.find("Connector busy. Please wait...")):
                        self.log.info("Connector busy feedback received correctly")
                    else:
                        self.fail("Console feedback for busy status NOT received")

                else:
                    self.fail("Console feedback for payload NOT found")

            else:
                self.fail("Received response from device: \"%s\" is not the expected" % responseText)
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)



    def test_02_send_cancel_request_target(self):
        """ Verifies that the target "cancel request" returns the correct response.
        Connector should stop the execution and return status 1 in the response
        """

        payload_size = 2048
        target = "cancel request"
        payload = ''.join(random.choice(ascii_letters + digits) for _ in range(payload_size))

        status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

        self.log.info(response.content)

        if(status):
            # Status 200. Checking the received response
            deviceRequestStatus = response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]["@status"]
            if(deviceRequestStatus == "1"):
                self.log.info("Status 1 received successfully")
            else:
                self.fail("Received unexpected status: %s" % deviceRequestStatus)

        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)



    def test_03_send_cancel_in_the_middle_target(self):
        """ Verifies that the target "cancel in the middle" returns the correct response.
        Connector must manage correctly the payload received but abort the response and send an error
        """

        target = "cancel in the middle"
        payload = "payload"

        status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

        self.log.info(response.content)

        if(status):
            # Status 200. Checking the received response
            # Response must have an <error> tag
            if("error" in response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]):
                errorMessage = response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]["error"]

                if(errorMessage == "Message transmission cancelled"):
                    self.log.info("Error message in response is the expected")
                else:
                    self.fail("Error message is not the expected. Received: \"%s\"" % errorMessage)
            else:
                self.fail("Not received error in the response")

        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)



    def test_04_send_cancel_response_target(self):
        """ Verifies that the target "cancel response" returns the correct response.
        Connector must manage correctly the payload received and start writing the response,
        but abort it before the completion
        """

        payload_size = 2048
        target = "cancel response"
        payload = ''.join(random.choice(ascii_letters + digits) for _ in range(payload_size))

        status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

        self.log.info(response.content)

        if(status):
            # Status 200. Checking the received response
            # Response must have an <error> tag

            if("error" in response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]):

                errorMessage = response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]["error"]
                if(errorMessage == "Message transmission cancelled"):
                    self.log.info("Error message in response is the expected")
                else:
                    self.fail("Error message is not the expected. Received: \"%s\"" % errorMessage)

            else:
                self.fail("Not received error in the response")

        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)



    def test_05_send_busy_response_target(self):
        """ Verifies that the target "busy response" returns the correct response.
        Connector must manage correctly a busy status in the middle of the response, After that, it must send the response
        """

        payload_size = 2048
        target = "busy response"
        payload = ''.join(random.choice(ascii_letters + digits) for _ in range(payload_size))

        status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

        self.log.info(response.content)


        if(status):
            # Status 200. Checking the received response
            responseText = response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]["#text"]
            if(responseText == payload):

                self.log.info("Received the expected response")
                self.log.info("Checking the payload received")
                result, line, dataBuffer = self.deviceHandler.readUntilPattern ( pattern="Total Payload received = \"%s\" for %s" %(payload, target), timeout=30)
                if(result):

                    self.log.info("Payload received successfully")
                    if(dataBuffer.find("Connector busy. Wait till response is sent...")):
                        self.log.info("Connector busy feedback received correctly")
                    else:
                        self.fail("Console feedback for busy status NOT received")

                else:
                    self.fail("Console feedback for payload NOT found")

            else:
                self.fail("Received response from device: \"%s\" is not the expected" % responseText)
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)



    def test_06_send_timeout_response_target(self):
        """ Verifies if the connector behaves correctly with the target "timeout response"
        Connector is forced to be busy until the timeout comes.
        """

        payload_size = 2048
        target = "timeout response"
        payload = ''.join(random.choice(ascii_letters + digits) for _ in range(payload_size))

        try:
            status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)
        except requests.exceptions.Timeout, e:
            self.log.info("Raised expected exception from Device")
            return True

        self.fail("Expected exception not found: %s" % response.content)
