import time
import ic_testcase

import xml.dom.minidom
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()

from ..utils import getText, update_firmware

firmware_targets = [('Text Image', '.*\.[tT][xX][tT]', '1.0.0.0')]
firmware_files   = ['dvt/cases/test_files/firmware_malloc.txt'] 
 
target = 0

DISCONNECT_REQUEST = \
"""<sci_request version="1.0">
    <disconnect>
        <targets>
            <device id="%s"/>
        </targets>
    </disconnect>
</sci_request>"""

SM_CONNECT_REQUEST = \
"""<sci_request version="1.0">
    <send_message>
        <targets>
            <device id="%s"/>
        </targets>
        <sm_udp retries="3" retryTimeout="10">
            <request_connect/>
        </sm_udp>
    </send_message>
</sci_request>"""
    

class UdpConnectTestCase(ic_testcase.UDPTestCase):

    def test_update_firmware(self):

        self.log.info("***** Beginning SM UDP Connect / Firmware Test *****")
        self.send_sm_connect_request()

        # Send request to update firmware
        self.log.info("Sending request to update firmware.")
        response = update_firmware(self.session, self.hostname, self.device_id, 
            firmware_files[target], "%d" % target)

        # Print response
        self.log.info("response: \n%s" % response)

        # Determine if Firmware was submitted
        submitted = response.find("submitted")

        self.assertNotEqual(-1, submitted, "Firmware was not submitted.")  

        self.check_disconnected()


    def test_sm_udp_connect(self):

        """ Sends SM UDP connect request to given device and verifies that
        the device connects to an iDigi server. Then sends a disconnect request 
        and verifies that the device disconnects.
        """

        self.log.info("***** Beginning SM UDP Connect / Disconnect Test *****")
 
        self.send_sm_connect_request()
        self.send_sm_disconnect_request()
        self.check_disconnected()


    def send_sm_disconnect_request(self):

        self.log.info("Sending SCI Disconnect Request to %s." % self.device_id)
        
        # Create disconnect request
        disconnect_request = DISCONNECT_REQUEST % (self.device_id)

        # Send SCI disconnect request
        self.session.post('http://%s/ws/sci' % self.hostname,data=disconnect_request).content



    def check_disconnected(self):

        self.device_core = None
        time.sleep(5)
        for _ in xrange(1,30):
            response = self.rest_session.get_first('DeviceCore', 
                            params={'condition' : "devConnectwareId='%s'" \
                            % self.device_id})

            if response.status_code != 200:
                break;

            self.device_core = response.resource
            if self.device_core.dpConnectionStatus != '1':
                self.log.info("Device %s is disconnected." % self.device_id)
                # Device disconnected
                break

            time.sleep(1.0)

        # If not connected, fail the TestCase.
        self.assertEqual('0', self.device_core.dpConnectionStatus, 
            "Device %s still connected after 30 seconds." % self.device_id)

    def send_sm_connect_request (self):

        for i in range(3):

            self.log.info("Sending SM UDP Connect Request to %s." % self.device_id)
    
            # Create connect request, up to 3 times with a 10 second syncTimeout.
            sm_connect_request = SM_CONNECT_REQUEST % (self.device_id)
    
            # Send SCI SM UDP connect request
            response = self.session.post('http://%s/ws/sci' % self.hostname, data=sm_connect_request).content
    
            # Parse response to determmine whether or not it timed out or
            # was successful.  If successful continue the test.
            dom = xml.dom.minidom.parseString(response)

            connect_response = dom.getElementsByTagName("request_connect")
            self.assertEqual(1, len(connect_response), 
                "No Connect Response Received. Response : %s" % connect_response)
    
            self.log.info("Connect response received from device, proceeding.")
    
            self.log.info("Ensuring Device %s is connected." \
                % self.device_id)
    
            self.device_core = None
            for _ in xrange(1,30):
                response = self.rest_session.get_first('DeviceCore', 
                                params={'condition' : "devConnectwareId='%s'" \
                                % self.device_id})
    
                if response.status_code != 200:
                    self.fail("Attempt to retrieve DeviceCore failed: (%d): %s" 
                        % (response.status_code, response.content))
    
                self.device_core = response.resource
                if self.device_core.dpConnectionStatus == '1':
                    # Device connected
                    self.log.info("Device %s is connected." % self.device_id)
                    break
    
                time.sleep(1.0)

            if self.device_core.dpConnectionStatus == '1':
                break

        # If not connected, fail the TestCase.
        self.assertEqual('1', self.device_core.dpConnectionStatus, 
            "Device %s not connected after 30 seconds." % self.device_id)


if __name__ == '__main__':
    unittest.main()
