import time
import ic_testcase

from base64 import encodestring

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()

from ..utils import DeviceConnectionMonitor

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

class Redirect3UrlsTestCase(ic_testcase.TestCase):

    monitor = None

    def setUp(self):
        ic_testcase.TestCase.setUp(self)
        # Optimization, reuse the DeviceConnectionMonitor to avoid creating
        # multiple sessions over and over.
        if Redirect3UrlsTestCase.monitor is None:
            Redirect3UrlsTestCase.monitor = DeviceConnectionMonitor(self.push_client, self.dev_id)
            Redirect3UrlsTestCase.monitor.start()
    
    @classmethod
    def tearDownClass(cls):
        if Redirect3UrlsTestCase.monitor is not None:
            Redirect3UrlsTestCase.monitor.stop()
        ic_testcase.TestCase.tearDownClass()
    
    def test_redirect_three_destinations(self):
    
        """ Sends redirect request to given device and verifies that
        the device disconnects and reconnects to the iDigi Device Cloud.
        
        Test may terminate iDigi Connector device session running in debug.
        """
        
        self.log.info("***** Beginning Redirect Test with three destination URLs *****")
        monitor = DeviceConnectionMonitor(self.push_client, self.dev_id)

        try:
            monitor.start()
            
            # Send redirect with three destinations
            self.log.info("Sending Connection Control Redirect to %s." % self.device_id)
            destinations = DESTINATION % self.hostname + DESTINATION % self.hostname + DESTINATION % self.hostname
            redirect_request = REDIRECT_REQUEST % \
                (self.device_id, destinations)

            count = 5
            while count > 0:
                # Send SCI redirect request (using admin credentials).
                response = self.admin_session.post('https://%s/ws/sci' % (self.hostname), 
                            data=redirect_request)
        
                self.assertEqual(200, response.status_code)

                # Print response to request.
                self.log.info("response:\n%s" % response.content)

                redirected = response.content.find("redirected")
                if redirected == -1:
                    count -= 1
                    self.log.info("Got error error. Let's retry")
                    if device_is_connected(self):
                        monitor.wait_for_connect(60)
                else:
                    count = 0

            self.log.info("Determining if device was redirected.")
            redirected = response.content.find("redirected")
            self.assertNotEqual(-1, redirected, 
                "Response did not contain 'redirected' element.")
            
            self.log.info("Waiting for iDigi to disconnect device.")
            monitor.wait_for_disconnect(30)
            self.log.info("Device disconnected.")
            
            self.log.info("Waiting for Device to reconnect.")
            monitor.wait_for_connect(30)
            self.log.info("Device connected.") 
        finally:
            monitor.stop()
 
if __name__ == '__main__':
    unittest.main()
