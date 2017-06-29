# -*- coding: iso-8859-1 -*-
import cc_testcase
from string import ascii_letters, digits
import random
import time


class DeviceRequestDvtTestCase(cc_testcase.TestCase):


    def test_01_stop_connector_wait_sessions_complete_for_transport_all(self):
        """ Verifies that Connector must manage correctly stop all transports after complete all opened sessions.
            In this case should send the response to the device request correctly.
        """

        payload_size = 2048
        target = "test_connector_wait_sessions_complete_for_transport_all"
        payload = ''.join(random.choice(ascii_letters + digits) for _ in range(payload_size))

        status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

        self.log.info("Received response: '%s'" % response.content)


        if(status):
            # Status 200. Checking the received response
            try:
                responseText = response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]["#text"]
                if(responseText == "My device response data"):
                    self.log.info("Received the expected response")

                    result, line, dataBuffer = self.deviceHandler.readUntilPattern ( pattern="connector_stop_callback: connector_transport_all", timeout=10)

                    if (result):
                        self.log.info("Connector stop callback feedback received correctly")
                    else:
                        self.fail("Console feedback for stop callback status NOT received")
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

        # Connector waits a few seconds to relaunch the transports on the device and we must verify that the device is connected again
        if (not self.cloudHandler.isDeviceConnected(self.device_id)):
            self.log.info("Waiting for Device to reconnect.")
            self.deviceMonitor.waitForConnect(30)
            self.log.info("Device is connected again.")



    def test_02_stop_connector_wait_sessions_complete_for_transport_tcp(self):
        """ Verifies that Connector must manage correctly stop TCP transport after complete all opened sessions.
            In this case should send the response to the device request correctly.
        """

        payload_size = 2048
        target = "test_connector_wait_sessions_complete_for_transport_tcp"
        payload = ''.join(random.choice(ascii_letters + digits) for _ in range(payload_size))

        status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

        self.log.info("Received response: '%s'" % response.content)


        if(status):
            # Status 200. Checking the received response
            try:
                responseText = response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]["#text"]
                if(responseText == "My device response data"):
                    self.log.info("Received the expected response")

                    result, line, dataBuffer = self.deviceHandler.readUntilPattern ( pattern="connector_stop_callback: connector_transport_tcp", timeout=10)

                    if (result):
                        self.log.info("Connector stop callback feedback received correctly")
                    else:
                        self.fail("Console feedback for stop callback status NOT received")
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

        # Connector waits a few seconds to relaunch the transports on the device and we must verify that the device is connected again
        if (not self.cloudHandler.isDeviceConnected(self.device_id)):
            self.log.info("Waiting for Device to reconnect.")
            self.deviceMonitor.waitForConnect(30)
            self.log.info("Device is connected again.")



    def test_03_stop_connector_wait_sessions_complete_several_times(self):
        """ Verifies that Connector must manage correctly all transports after complete all opened sessions
            several times to make sure that the process is robust.
        """

        totalLoops = 2

        for i in range(0,totalLoops):
            self.log.info("")
            self.log.info("**** Loop %s of %s" % (i,totalLoops) )

            payload_size = 2048
            target = "test_connector_wait_sessions_complete_for_transport_all"
            payload = ''.join(random.choice(ascii_letters + digits) for _ in range(payload_size))

            status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

            self.log.info("Received response: '%s'" % response.content)


            if(status):
                # Status 200. Checking the received response
                try:
                    responseText = response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]["#text"]
                    if(responseText == "My device response data"):
                        self.log.info("Received the expected response")

                        result, line, dataBuffer = self.deviceHandler.readUntilPattern ( pattern="connector_stop_callback: connector_transport_all", timeout=10)

                        if (result):
                            self.log.info("Connector stop callback feedback received correctly")
                        else:
                            self.fail("Console feedback for stop callback status NOT received")
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
            #if (self.cloudHandler.isDeviceConnected(self.device_id)):
            self.log.info("Waiting for Cloud to disconnect device.")
            self.deviceMonitor.waitForDisconnect(30)
            self.log.info("Device disconnected.")
            #else:
                #self.log.info("Device is disconnected from Device Cloud")

            # Connector waits a few seconds to relaunch the transports on the device and we must verify that the device is connected again
            if (not self.cloudHandler.isDeviceConnected(self.device_id)):
                self.log.info("Waiting for Device to reconnect.")
                self.deviceMonitor.waitForConnect(30)
                self.log.info("Device is connected again.")




    def test_04_stop_connector_stop_immediately_for_transport_all(self):
        """ Verifies that Connector must manage correctly stop all transports immediately before complete the opened sessions.
            In this case should close the transports and not send the response to the device request.
        """

        payload_size = 2048
        target = "test_connector_stop_immediately_for_transport_all"
        payload = ''.join(random.choice(ascii_letters + digits) for _ in range(payload_size))

        status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

        self.log.info("Received response: '%s'" % response.content)


        if(status):
            # Status 200. Checking the received response
            try:
                responseText = response.resource["sci_reply"]["data_service"]["device"]["error"]["desc"]
                if(responseText == "Device disconnected while processing request"):
                    self.log.info("Received the expected response")

                    result, line, dataBuffer = self.deviceHandler.readUntilPattern ( pattern="connector_stop_callback: connector_transport_all", timeout=10)

                    if (result):
                        self.log.info("Connector stop callback feedback received correctly")
                    else:
                        self.fail("Console feedback for stop callback status NOT received")
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

        # Connector waits a few seconds to relaunch the transports on the device and we must verify that the device is connected again
        if (not self.cloudHandler.isDeviceConnected(self.device_id)):
            self.log.info("Waiting for Device to reconnect.")
            self.deviceMonitor.waitForConnect(30)
            self.log.info("Device is connected again.")



    def test_05_stop_connector_stop_immediately_for_transport_tcp(self):
        """ Verifies that Connector must manage correctly stop TCP transport immediately before complete the opened sessions.
            In this case should close the transports and not send the response to the device request.
        """

        payload_size = 2048
        target = "test_connector_stop_immediately_for_transport_tcp"
        payload = ''.join(random.choice(ascii_letters + digits) for _ in range(payload_size))

        status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

        self.log.info("Received response: '%s'" % response.content)


        if(status):
            # Status 200. Checking the received response
            try:
                responseText = response.resource["sci_reply"]["data_service"]["device"]["error"]["desc"]
                if(responseText == "Device disconnected while processing request"):
                    self.log.info("Received the expected response")

                    result, line, dataBuffer = self.deviceHandler.readUntilPattern ( pattern="connector_stop_callback: connector_transport_tcp", timeout=10)

                    if (result):
                        self.log.info("Connector stop callback feedback received correctly")
                    else:
                        self.fail("Console feedback for stop callback status NOT received")
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

        # Connector waits a few seconds to relaunch the transports on the device and we must verify that the device is connected again
        if (not self.cloudHandler.isDeviceConnected(self.device_id)):
            self.log.info("Waiting for Device to reconnect.")
            self.deviceMonitor.waitForConnect(30)
            self.log.info("Device is connected again.")




    def test_06_stop_connector_stop_immediately_several_times(self):
        """ Verifies that Connector must manage correctly stop all transports immediately before complete the opened sessions
            several times to make sure that the process is robust.
        """

        totalLoops = 2

        for i in range(0,totalLoops):
            self.log.info("")
            self.log.info("**** Loop %s of %s" % (i,totalLoops) )

            payload_size = 2048
            target = "test_connector_stop_immediately_for_transport_all"
            payload = ''.join(random.choice(ascii_letters + digits) for _ in range(payload_size))

            status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

            self.log.info("Received response: '%s'" % response.content)


            if(status):
                # Status 200. Checking the received response
                try:
                    responseText = response.resource["sci_reply"]["data_service"]["device"]["error"]["desc"]
                    if(responseText == "Device disconnected while processing request"):
                        self.log.info("Received the expected response")

                        result, line, dataBuffer = self.deviceHandler.readUntilPattern ( pattern="connector_stop_callback: connector_transport_all", timeout=10)

                        if (result):
                            self.log.info("Connector stop callback feedback received correctly")
                        else:
                            self.fail("Console feedback for stop callback status NOT received")
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

            # Connector waits a few seconds to relaunch the transports on the device and we must verify that the device is connected again
            if (not self.cloudHandler.isDeviceConnected(self.device_id)):
                self.log.info("Waiting for Device to reconnect.")
                self.deviceMonitor.waitForConnect(30)
                self.log.info("Device is connected again.")



    def test_07_connector_terminate(self):
        """ Verifies that Connector must manage correctly the terminate action.
            In this case should end the connector thread and the application thread.
        """

        payload_size = 2048
        target = "test_connector_terminate"
        payload = ''.join(random.choice(ascii_letters + digits) for _ in range(payload_size))

        status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

        self.log.info("Received response: '%s'" % response.content)


        if(status):
            # Status 200. Checking the received response
            try:
                responseText = response.resource["sci_reply"]["data_service"]["device"]["error"]["desc"]
                if(responseText == "Device disconnected while processing request"):
                    self.log.info("Received the expected response")

                    result, line, dataBuffer = self.deviceHandler.readUntilPattern ( pattern="Cloud Connector terminated", timeout=10)

                    if (result):
                        self.log.info("Connector terminate action feedback received correctly")
                    else:
                        self.fail("Console feedback for terminate action status NOT received")
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
