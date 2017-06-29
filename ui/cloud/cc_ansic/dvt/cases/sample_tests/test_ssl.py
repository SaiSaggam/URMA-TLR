import ic_testcase
import shutil
from ..utils import DeviceConnectionMonitor, ConnectivityException

DISCONNECT_REQUEST = \
"""<sci_request version="1.0">
    <disconnect>
        <targets>
            <device id="%s"/>
        </targets>
    </disconnect>
</sci_request>"""

good_ca_cert_src = './dvt/cases/test_files/Digi_Int-ca-cert-public.crt'
good_ca_cert_dst = './public/certificates/Digi_Int-ca-cert-public.crt'
bad_ca_cert_src  = './dvt/cases/test_files/intruder-ca-cert.crt'

class SSLTestCase(ic_testcase.TestCase):
    monitor=None

    def setUp(self):
        ic_testcase.TestCase.setUp(self)
        # Optimization, reuse the DeviceConnectionMonitor to avoid creating
        # multiple sessions over and over.
        if SSLTestCase.monitor is None:
            SSLTestCase.monitor = DeviceConnectionMonitor(self.push_client, self.dev_id)
            SSLTestCase.monitor.start()
        self.monitor = SSLTestCase.monitor

    @classmethod
    def tearDownClass(cls):
        if SSLTestCase.monitor is not None:
            SSLTestCase.monitor.stop()
        ic_testcase.TestCase.tearDownClass()


    def test_ssl_with_good_ca_cert(self):
        self.log.info("SSL connection test with valid ca certificate")

        self.log.info("Retrieving Device type for %s." % self.device_id)
        # Send request for new device_core
        response = self.rest_session.get_first('DeviceCore',
                    params={'condition' : "devConnectwareId='%s'" % self.device_id})
        self.assertEqual(200, response.status_code)
        new_device_core = response.resource
        # Verify that device type returned matches that in the configuration.
        self.assertEqual(new_device_core.dpDeviceType, self.device_type,
            "Device type (%s) does not match device type in configuration (%s)."
            % (new_device_core.dpDeviceType, self.device_type))

        # Verify that device's vendor id matches that in the configuration.
        self.assertEqual(new_device_core.dvVendorId, self.vendor_id,
            "Vendor ID (%s) does not match Vendor ID in configuration (%s)."
            % (new_device_core.dvVendorId, self.vendor_id))

    def test_ssl_with_intruder_ca_cert(self):
        self.log.info("SSL connection test with intruder ca certificate")

        try:
            shutil.copy(bad_ca_cert_src, good_ca_cert_dst);
        except Exception, e:
            print ">>> Error: %s" %e

        self.log.info("Sending Connection Control Disconnect to %s." % self.device_id)
        # Create disconnect request
        disconnect_request = DISCONNECT_REQUEST % (self.device_id)

        # Send SCI disconnect request
        response = self.session.post('http://%s/ws/sci' % self.hostname, data=disconnect_request)
        self.log.info(response.content)

        self.log.info("Waiting for iDigi to disconnect device.")
        self.monitor.wait_for_disconnect(30)
        self.log.info("Device disconnected.")

        self.log.info("Waiting for Device to reconnect.")

        timeout = 10
        try:
            self.monitor.wait_for_connect(timeout)
            # If no exception raised, we connected (which shouldn't happen)
            raise Exception('Device %s did connect within %d seconds with bad CA certificate.' % \
                (self.device_id, timeout))
        except ConnectivityException:
            # Exception expected, assume device did not connect.
            self.log.info("Device is still disconnected... copying valid certificate back")

        try:
            shutil.copy(good_ca_cert_src, good_ca_cert_dst);
        except Exception, e:
            print ">>> Error: %s" %e

        self.monitor.wait_for_connect(timeout)
        self.log.info("Device connected.")


if __name__ == '__main__':
    unittest.main()
