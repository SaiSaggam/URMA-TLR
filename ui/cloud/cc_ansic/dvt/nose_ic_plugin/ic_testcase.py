import unittest
import logging
import time
import xml.dom.minidom

class BaseICTestCase(unittest.TestCase):
    push_client = None

    def __init__(self, name):
        unittest.TestCase.__init__(self, name)
        self.log = logging.getLogger('ic_testcase')
        self.log.setLevel(logging.INFO)

        if len(self.log.handlers) == 0:
            handler = logging.StreamHandler()
            handler.setLevel(logging.INFO)
            formatter = logging.Formatter('%(asctime)-15s %(name)-20s %(levelname)-8s %(message)s')
            handler.setFormatter(formatter)
            self.log.addHandler(handler)


    def setUp(self):
        # Use the same Push Client at the class level (optimization).
        if TestCase.push_client is None:
            TestCase.push_client = self.push_client

        self.push_client = TestCase.push_client

    @classmethod
    def tearDownClass(cls):
        if TestCase.push_client is not None:
            TestCase.push_client.stop_all()
            TestCase.push_client = None

class TestCase(BaseICTestCase):
    push_client = None

    def setUp(self):
        super(TestCase, self).setUp()

        self.log.info("Ensuring Device %s is connected." \
            % self.device_id)

        self.device_core = None
        for _ in xrange(1,20):
            response = self.rest_session.get_first('DeviceCore', 
                            params={'condition' : "devConnectwareId='%s'" \
                            % self.device_id})

            if response.status_code != 200:
                self.fail("Attempt to retrieve DeviceCore failed: (%d): %s" 
                    % (response.status_code, response.content))

            self.device_core = response.resource
            self.dev_id = self.device_core.id.devId
            if self.device_core.dpConnectionStatus == '1':
                # Device connected.  Sleep a second when it connects for
                # IDIGI-740.
                time.sleep(1.0)
                break

            time.sleep(1.0)

        # If not connected, fail the TestCase.
        self.assertEqual('1', self.device_core.dpConnectionStatus, 
            "Device %s not connected after 20 seconds." % self.device_id)
        self.log.info("Device %s is connected." % self.device_id)

PING_REQUEST = """<sci_request version="1.0"> 
  <send_message synchronous="true"> 
    <targets> 
      <device id="%s"/> 
    </targets>
    <sm_udp retries="5" retryTimeout="10">
      <ping/>
    </sm_udp>
  </send_message>
</sci_request>"""


class UDPTestCase(BaseICTestCase):
    
    def setUp(self):
        super(UDPTestCase, self).setUp()

        self.log.info(
            "Sending Ping Request to Device %s to ensure it is Alive." \
                % self.device_id)

        # Try ping up to 5 times with a 10 second syncTimeout.
        response = self.session.post('https://%s/ws/sci' % self.hostname,
            data=PING_REQUEST % self.device_id)

        # Parse response to determmine whether or not it timed out or
        # was successful.  If successful continue the test.
        dom = xml.dom.minidom.parseString(response.content)
        ping_response = dom.getElementsByTagName("ping")

        self.assertEqual(1, len(ping_response), 
            "No Ping Response Received. Response : %s" % ping_response)

        self.log.info("Ping response received from device, proceeding.")