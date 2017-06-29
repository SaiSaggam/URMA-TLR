import time
import ic_testcase

from base64 import encodestring

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()

from ..utils import DeviceConnectionMonitor, device_is_connected

nonconnector_host = 'google.com'

DESTINATION = "<destination>%s</destination>"

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

class RedirectTestCase(ic_testcase.TestCase):
    monitor = None

    def setUp(self):
        ic_testcase.TestCase.setUp(self)
        # Optimization, reuse the DeviceConnectionMonitor to avoid creating
        # multiple sessions over and over.
        if RedirectTestCase.monitor is None:
            RedirectTestCase.monitor = DeviceConnectionMonitor(self.push_client, self.dev_id)
            RedirectTestCase.monitor.start()

    @classmethod
    def tearDownClass(cls):
        if RedirectTestCase.monitor is not None:
            RedirectTestCase.monitor.stop()
        ic_testcase.TestCase.tearDownClass()

    def test_redirect(self):

        """ Sends redirect request to given device and verifies that it
        disconnects and reconnects
        """  
        self.log.info("***** Beginnning test_redirect *****")

        self.log.info("Sending Connection Control Redirect to %s." % self.device_id)
        # Create the request to redirect the device to the same server.
        redirect_request = REDIRECT_REQUEST % \
            (self.device_id, DESTINATION % self.hostname)

        count = 5
        while count > 0:        
            # Send SCI redirect request (using admin credentials).
            self.log.info("Sending SCI Request: \n%s\n" % redirect_request)
            response = self.admin_session.post('https://%s/ws/sci' % (self.hostname),
                        data=redirect_request)

            # Print response to request.
            self.log.info("Received SCI Response: \n%s\n" % response.content)

            self.assertEqual(200, response.status_code)

            redirected = response.content.find("redirected")
            if redirected == -1:
                count -= 1
                self.log.info("Got error error. Let's retry")
                if device_is_connected(self):
                    RedirectTestCase.monitor.wait_for_connect(60)
            else:
                count = 0 

        self.log.info("Waiting for Device Cloud to disconnect device.")
        RedirectTestCase.monitor.wait_for_disconnect(30)
        self.log.info("Device disconnected.")

        self.log.info("Waiting for Device to reconnect.")
        RedirectTestCase.monitor.wait_for_connect(60)
        self.log.info("Device connected.") 

    def test_redirect_multi_urls_first_nonidigi(self):

        """ Sends redirect request to given device starting with a nonidigi
        host and verifies that it disconnects and reconnects to an idigi
        server.
        """
        self.log.info("***** Beginnning test_redirect_multi_urls_first_nonidigi *****")

        self.log.info("Sending Connection Control Redirect to %s." % self.device_id)
        # Create the request to redirect the device to a nonidigi server and
        # then an iDigi server
        destination = DESTINATION % nonconnector_host + DESTINATION % self.hostname
        redirect_request = REDIRECT_REQUEST % \
            (self.device_id, destination)

        count = 5
        while count > 0:
            # Send SCI redirect request (using admin credentials).
            self.log.info("Sending SCI Request: \n%s\n" % redirect_request)
            response = self.admin_session.post('https://%s/ws/sci' % (self.hostname),
                        data=redirect_request)

            # Print response to request.
            self.log.info("Received SCI Response: \n%s\n" % response.content)

            self.assertEqual(200, response.status_code)

            redirected = response.content.find("redirected")
            if redirected == -1:
                count -= 1
                self.log.info("Got error error. Let's retry")
                if device_is_connected(self):
                    RedirectTestCase.monitor.wait_for_connect(60)
            else:
                count = 0

        self.log.info("Waiting for Device Cloud to disconnect device.")
        RedirectTestCase.monitor.wait_for_disconnect(30)
        self.log.info("Device disconnected.")

        self.log.info("Waiting for Device to reconnect.")
        RedirectTestCase.monitor.wait_for_connect(60)
        self.log.info("Device connected.")

    def test_redirect_singleurl_nondigi(self):

        """ Sends redirect reqeust to given device starting with a noniDigi
        host and verifies that it disconnets and reconnects to an iDigi
        server.
        """
        self.log.info("***** Beginnning test_redirect_singleurl_nondigi *****")

        self.log.info("Sending Connection Control Redirect to %s." % self.device_id)
        # Create the request to redirect the device to a nonidigi server.
        redirect_request = REDIRECT_REQUEST % \
            (self.device_id, DESTINATION % nonconnector_host)

        count = 5
        while count > 0:
            # Send SCI redirect request (using admin credentials).
            self.log.info("Sending SCI Request: \n%s\n" % redirect_request)
            response = self.admin_session.post('https://%s/ws/sci' % (self.hostname),
                        data=redirect_request)

            # Print response to request.
            self.log.info("Received SCI Response: \n%s\n" % response.content)

            self.assertEqual(200, response.status_code)

            redirected = response.content.find("redirected")
            if redirected == -1:
                count -= 1
                self.log.info("Got error error. Let's retry")
                if device_is_connected(self):
                    RedirectTestCase.monitor.wait_for_connect(60)
            else:
                count = 0

        self.log.info("Waiting for iDigi to disconnect device.")
        RedirectTestCase.monitor.wait_for_disconnect(30)
        self.log.info("Device disconnected.")

        self.log.info("Waiting for Device to reconnect.")
        RedirectTestCase.monitor.wait_for_connect(60)
        self.log.info("Device connected.")

    def test_redirect_zero_destinations(self):

        """ Sends redirect request with no destinations to given device.
        Verifies that an error response is returned.
        """

        self.log.info("***** Beginnning Redirect Test with zero destination URLs *****")

        self.log.info("Sending Connection Control Redirect to %s." % self.device_id)

        # Create redirect request with no destinations
        redirect_request = REDIRECT_REQUEST % (self.device_id, "")

        # Send SCI redirect request (using admin credentials).
        self.log.info("Sending SCI Request: \n%s\n" % redirect_request)
        response = self.admin_session.post('https://%s/ws/sci' % (self.hostname),
                    data=redirect_request)

        # Print response to request.
        self.log.info("Received SCI Response: \n%s\n" % response.content)

        # Verifying the request returns an error.
        self.log.info("Determining if we have received error response.")
        error = response.content.find("error")
        self.assertNotEqual(-1, error, "Received unexpected response from invalid redirect.")

        self.log.info("Make sure device is still connected.")
        if device_is_connected(self):
            self.log.info("Device connected.")
        else:
            self.log.info("Device disconnected.")

 
if __name__ == '__main__':
    unittest.main()
