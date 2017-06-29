import time
import cc_testcase

from base64 import encodestring

non_etherios_host = 'google.com'

DESTINATION_TAGS = "<destination>%s</destination>"

REDIRECT_REQUEST = \
"""<sci_request version="1.0">
    <redirect>
        <targets>
            <device id="%s"/>
        </targets>
        <destinations>
            %s
        </destinations>
    </redirect>
</sci_request>"""

class RedirectTestCase(cc_testcase.TestCase):

    def test_redirect(self):
        """ Sends redirect request to given device and verifies that it
        disconnects and reconnects"""
        self.log.info("Sending Connection Control Redirect to %s." % self.device_id)
        # Create the request to redirect the device to the same server.
        redirect_request = REDIRECT_REQUEST % (self.device_id, DESTINATION_TAGS % self.hostname)

        # Send SCI redirect request (using admin credentials).
        self.log.info("Sending SCI Request: \n%s\n" % redirect_request)
        requestResponse = self.cloudAdminHandler.postRequest("sci", data=redirect_request)
        if (requestResponse.status_code != 200):
            self.fail("Unexpected response from the Cloud")
        
        # Print response to request.
        self.log.info("Received SCI Response: \n%s\n" % requestResponse.content)

        deviceWasRedirected = "redirected" in requestResponse.resource["sci_reply"]["redirect"]["device"]
        self.assertEqual(deviceWasRedirected, True)
        
        #TODO this feedback only works for debug modes
        #result, indexPattern, dataBuffer = self.deviceHandler.readUntilPattern(pattern="tcp_close_cloud: status = connector_close_status_cloud_redirected", timeout=30)
        #self.log.info("Console feedback for redirection = %d" % result)
        #if (not result):
            #self.fail("Console feedback for redirection NOT arrived")

        self.log.info("Waiting for Device Cloud to disconnect device.")
        self.deviceMonitor.waitForDisconnect(30)
        self.log.info("Device disconnected.")

        self.log.info("Waiting for Device to reconnect.")
        self.deviceMonitor.waitForConnect(60)
        self.log.info("Device connected.") 

    def test_redirect_multi_urls_first_non_etherios(self):

        """ Sends redirect request to given device starting with a non-etherios
        host and verifies that it disconnects and reconnects to an etherios server.
        """
        
        self.log.info("Sending Connection Control Redirect to %s." % self.device_id)
        # Create the request to redirect the device to a nonidigi server and
        # then an iDigi server
        destination = DESTINATION_TAGS % non_etherios_host + DESTINATION_TAGS % self.hostname
        redirect_request = REDIRECT_REQUEST % \
            (self.device_id, destination)

        # Send SCI redirect request (using admin credentials).
        self.log.info("Sending SCI Request: \n%s\n" % redirect_request)
        requestResponse = self.cloudAdminHandler.postRequest("sci", data=redirect_request)
        if (requestResponse.status_code != 200):
            self.fail("Unexpected response from the Cloud")
        
        # Print response to request.
        self.log.info("Received SCI Response: \n%s\n" % requestResponse.content)

        deviceWasRedirected = "redirected" in requestResponse.resource["sci_reply"]["redirect"]["device"]

        #TODO this feedback only works for debug modes
        #result, indexPattern, dataBuffer = self.deviceHandler.readUntilPattern(pattern="tcp_close_cloud: status = connector_close_status_cloud_redirected", timeout=30)
        #self.log.info("Console feedback for redirection = %d" % result)
        #if (not result):
            #self.fail("Console feedback for redirection NOT arrived")

        self.log.info("Waiting for Device Cloud to disconnect device.")
        self.deviceMonitor.waitForDisconnect(30)
        self.log.info("Device disconnected.")

        result, indexPattern, dataBuffer = self.deviceHandler.readUntilPattern(pattern="app_network_tcp_open: failed to connect to %s" % non_etherios_host, timeout=30)
        self.log.info("Console feedback for disconnection = %d" % result)
        if (not result):
            self.fail("Console feedback for disconnection NOT arrived")

        self.log.info("Waiting for Device to reconnect.")
        self.deviceMonitor.waitForConnect(60)
        self.log.info("Device connected.") 

    def test_redirect_singleurl_non_etherios(self):

        """ Sends redirect request to given device starting with a non-etherios
        host and verifies that it disconnets and reconnects to the configured
        server.
        """

        self.log.info("Sending Connection Control Redirect to %s." % self.device_id)
        # Create the request to redirect the device to a nonidigi server.
        redirect_request = REDIRECT_REQUEST % \
            (self.device_id, DESTINATION_TAGS % non_etherios_host)

        # Send SCI redirect request (using admin credentials).
        self.log.info("Sending SCI Request: \n%s\n" % redirect_request)
        requestResponse = self.cloudAdminHandler.postRequest("sci", data=redirect_request)
        if (requestResponse.status_code != 200):
            self.fail("Unexpected response from the Cloud")
        
        # Print response to request.
        self.log.info("Received SCI Response: \n%s\n" % requestResponse.content)

        deviceWasRedirected = "redirected" in requestResponse.resource["sci_reply"]["redirect"]["device"]

        #TODO this feedback only works for debug modes
        #result, indexPattern, dataBuffer = self.deviceHandler.readUntilPattern(pattern="tcp_close_cloud: status = connector_close_status_cloud_redirected", timeout=30)
        #self.log.info("Console feedback for redirection = %d" % result)
        #if (not result):
            #self.fail("Console feedback for redirection NOT arrived")

        self.log.info("Waiting for Device Cloud to disconnect device.")
        self.deviceMonitor.waitForDisconnect(30)
        self.log.info("Device disconnected.")

        result, indexPattern, dataBuffer = self.deviceHandler.readUntilPattern(pattern="app_network_tcp_open: failed to connect to %s" % non_etherios_host, timeout=30)
        self.log.info("Console feedback for disconnection = %d" % result)
        if (not result):
            self.fail("Console feedback for disconnection NOT arrived")


        self.log.info("Waiting for Device to reconnect.")
        self.deviceMonitor.waitForConnect(60)
        self.log.info("Device connected.") 

    def test_redirect_zero_destinations(self):

        """ Sends redirect request with no destinations to given device.
        Verifies that an error response is returned."""

        self.log.info("***** Beginnning Redirect Test with zero destination URLs *****")

        self.log.info("Sending Connection Control Redirect to %s." % self.device_id)

        # Create redirect request with no destinations
        redirect_request = REDIRECT_REQUEST % (self.device_id, "")
        
        # Send SCI redirect request (using admin credentials).
        self.log.info("Sending SCI Request: \n%s\n" % redirect_request)
        requestResponse = self.cloudAdminHandler.postRequest("sci", data=redirect_request)

        #NOTE: This request is a Bad Request (400), most requests expect an OK (200)
        if (requestResponse.status_code != 400):
            self.fail("Unexpected response from the Cloud")
            
        self.log.info("Received SCI Response: \n%s\n" % requestResponse.content)
        self.log.info("Determining if an error response has been received.")
        
        errorReceived = "error" in requestResponse.resource["sci_reply"]["redirect"]
        
        self.assertEqual(errorReceived, True)
        self.assertEqual(requestResponse.resource["sci_reply"]["redirect"]["error"], "Invalid XML format. destination element is required for redirect/destinations.")

        self.log.info("Ensure that device is still connected.")
        self.assertEqual(self.cloudHandler.isDeviceConnected(self.device_id), True)
 
if __name__ == '__main__':
    unittest.main()
