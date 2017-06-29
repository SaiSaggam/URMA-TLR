import cc_testcase

class DiscoveryTestCase(cc_testcase.TestCase):

    def test_verify_device_type_matches(self):
        """ This test intends to test if the application was able to request a Device ID to Device Cloud and is connected. Also 
        verifies that the Device Type reported by Device Cloud is the same than the one configured in Cloud Connector. """
        if (not self.cloudHandler.isDeviceConnected(self.device_id)):
            self.log.info("Waiting for Device to reconnect.")
            self.deviceMonitor.waitForConnect(30)
            self.log.info("Device connected.")
        self.log.info("Beginning Test to Verify Device Type.")
        
        self.log.info("Retrieving Device type for %s." % self.device_id)
        result, deviceType = self.cloudHandler.getDeviceType(self.device_id)
        if (result):
            self.log.info("Checking if Device Type matches %s" % self.device_type)
            self.assertEqual(deviceType, self.device_type,
                "Device type (%s) does not match device type in configuration (%s)." 
                % (deviceType, self.device_type))
        else:
             self.fail("Could not get device type")
    
    def test_verify_vendor_id_matches(self):
        """ This test intends to test if the application was able to request a Device ID to Device Cloud and is connected. Also 
        verifies that the Vendor ID reported by Device Cloud is the same than the one configured in Cloud Connector. """
        self.log.info("Beginning Test to Verify Vendor ID.")
        self.log.info("Retrieving Vendor ID for %s." % self.device_id)
        result, deviceVendorID = self.cloudHandler.getDeviceVendorID(self.device_id)
        if (result):
            self.log.info("Checking if Device Vendor ID matches %s" % self.vendor_id)
            self.assertEqual(deviceVendorID, self.vendor_id,
                "Vendor ID (%s) does not match Vendor ID in configuration (%s)." 
                % (deviceVendorID, self.vendor_id))
        else:
             self.fail("Could not get device type")
        
if __name__ == '__main__':
    unittest.main()